#include "DataModel.h"
#include <MASimulator.h>
#include <SubspaceSimulator.h>
#include <FullStVKSimulator.h>
#include <MatrixIO.h>
using namespace SIMULATOR;

DataModel::DataModel(pTetMeshEmbeding embeding):_volObj(embeding){

  assert(embeding);
  steps = 1;
  _record = false;
  _restObj = pObjmesh(new UTILITY::Objmesh());
}

pSimulator DataModel::createSimulator(const string filename)const{
  
  string simulator_name = "full_stvk";
  JsonFilePaser jsonf;
  if (jsonf.open(filename)){
	jsonf.read("simulator", simulator_name, string("full_stvk"));
  }

  pSimulator sim;
  if ("subspace" == simulator_name){
	pReducedElasticModel elas_m = pReducedElasticModel(new DirectReductionElasticModel());
	sim = pSimulator(new SubspaceSimulator(elas_m,string("subspace")));
  }else if ("cubature" == simulator_name){
	pReducedElasticModel elas_m = pReducedElasticModel(new CubaturedElasticModel());
	sim = pSimulator(new SubspaceSimulator(elas_m,string("cubature")));
  }else if ("full_static" == simulator_name){
	pBaseFullSim static_sim = pBaseFullSim(new PenStaticFullSim());
	sim = pSimulator(new FullStVKSimulator(static_sim,"full static"));
  }else if ("subspace_static" == simulator_name){
	pReducedElasticModel elas_m = pReducedElasticModel(new DirectReductionElasticModel());
	pReducedSimulator static_sim =pReducedSimulator(new ReducedStaticPenConSimulator(elas_m));
	sim = pSimulator(new SubspaceSimulator(elas_m,static_sim,string("subspace static")));
  }else if ("cubature_static" == simulator_name){
	pReducedElasticModel elas_m = pReducedElasticModel(new CubaturedElasticModel());
	pReducedSimulator static_sim =pReducedSimulator(new ReducedStaticPenConSimulator(elas_m));
	sim = pSimulator(new SubspaceSimulator(elas_m,static_sim,string("cubature static")));
  }else if ("modal_analysis" == simulator_name){
	sim = pSimulator(new MASimulator());
  }else{
	pBaseFullSim static_sim = pBaseFullSim(new LagImpFullSim());
	sim = pSimulator(new FullStVKSimulator(static_sim,"full stvk"));
  }

  return sim;
}

bool DataModel::loadSetting(const string filename){

  JsonFilePaser jsonf;
  if (!jsonf.open(filename)){
	ERROR_LOG("failed to open: " << filename);
	return false;
  }

  _simulator = createSimulator(filename);

  bool succ = true;
  string mtlfile;
  if (jsonf.readFilePath("elastic_mtl",mtlfile)){
	if (_volObj && _volObj->getTetMesh()){
	  succ = _volObj->getTetMesh()->loadElasticMtl(mtlfile);
	}
  }
  jsonf.read("num_step", steps, 1);
  if (_simulator){
	succ &= _simulator->init(filename);
	if (succ){
	  vector<int> fixed_nodes;
	  if(jsonf.readVecFile("fixed_nodes",fixed_nodes,TEXT)){
		prepareSimulation();
		addConNodes(fixed_nodes);
	  }
	}
  }

  string objfile;
  if (jsonf.readFilePath("obj_file", objfile)){
	_restObj->load(objfile);
	_restObj->setMtl(ObjMtl());
  }

  string sf;
  if (jsonf.readFilePath("scene", sf,true)){
	if (!scene) scene = pObjmesh(new Objmesh());
	scene->load(sf);
  }

  print();
  return succ;
}

void DataModel::prepareSimulation(){

  if(_simulator){
	_simulator->setVolMesh(_volObj->getTetMesh());
	const bool succ = _simulator->precompute();
	ERROR_LOG_COND("the precomputation is failed.",succ);
  }
}

void DataModel::setForces(const int nodeId,const double force[3]){

  if(_simulator){
	_simulator->setExtForceOfNode(nodeId,force);
	this->simulate();
  }
}

void DataModel::getSubUc(const vector<set<int> > &groups,const VectorXd &full_u,Matrix<double,3,-1> &sub_u)const{

  int nodes = 0;
  BOOST_FOREACH(const set<int>& s, groups)
	nodes += s.size();

  sub_u.resize(3,nodes);
  int index = 0;
  BOOST_FOREACH(const set<int>& s, groups){
	BOOST_FOREACH(const int i, s){
	  assert_in(i*3,0,full_u.size()-3);
	  sub_u.col(index) = full_u.segment<3>(i*3);
	  index++;
	}
  }
}

void DataModel::updateUc(const Matrix<double,3,-1> &uc,const int group_id){

  if (_simulator){
	assert_gt(uc.size(),0);
	_partialCon.updatePc(uc,group_id);
	const int n = _partialCon.getPc().size();
	_simulator->setUc(Map<VectorXd>(const_cast<double*>(&(_partialCon.getPc()(0,0))),n));
  }
}

void DataModel::updateXc(const Matrix<double,3,-1> &xc,const int group_id){

  const Matrix<double,3,-1> xc_0 = _partialCon_x0.getPc(group_id);
  assert_eq(xc_0.size(), xc.size());
  const Matrix<double,3,-1> uc = xc - xc_0;
  updateUc(uc, group_id);
}

const Matrix<double,3,-1> DataModel::getXc(const int group)const{

  assert_in(group,0,_partialCon.numGroup()-1);
  const Matrix<double,3,-1> xc = _partialCon_x0.getPc(group);
  const Matrix<double,3,-1> uc = _partialCon.getPc(group);
  assert_eq(xc.size(), uc.size());
  return xc + uc;
}

void DataModel::resetPartialCon(){

  if (!_simulator){
	return ;
  }

  Matrix<double,3,-1> pc;
  getSubUc(_partialCon.getConNodesSet(),getU(),pc);
  _partialCon.updatePc(pc);

  if (rest_shape.size() != getVolMesh()->nodes().size()*3){
	getVolMesh()->nodes(rest_shape);
  }
  getSubUc(_partialCon_x0.getConNodesSet(),rest_shape,pc);
  _partialCon_x0.updatePc(pc);

  static vector<int> con_nodes;
  static VectorXd con_uc;
  _partialCon.getPartialCon(con_nodes, con_uc);
  _simulator->setConNodes(con_nodes);
  _simulator->setUc(con_uc);
}

bool DataModel::simulate(){

  bool succ = false;
  if(_simulator){
	succ = _simulator->forward();
	for (int i = 1; i < steps; ++i){
	  succ &= _simulator->forward();
	}
	_volObj->interpolate(getU());
  }

  // recording.
  if (succ && _record)
	_recorded_vol_u.push_back(getU());

  ERROR_LOG_COND("simulation failed.",succ);

  return succ;
}

bool DataModel::saveConNodes(const string filename)const{

  const vector<set<int> > &con_nodes = getConNodes();
  vector<int> nodes;
  for (int i = 0; i < con_nodes.size(); ++i)
    BOOST_FOREACH(const int ele, con_nodes[i])
	  nodes.push_back(ele);
  return writeVec(filename,nodes, UTILITY::TEXT);
}

