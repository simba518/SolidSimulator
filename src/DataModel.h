#ifndef _DATAMODEL_H_
#define _DATAMODEL_H_

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <Log.h>
#include <AuxTools.h>
#include <MatrixIO.h>
#include <TetMeshEmbeding.h>
#include <JsonFilePaser.h>
#include "Simulator.h"
#include "ElasticMtlGroups.h"
using namespace std;
using namespace UTILITY;

namespace SIMULATOR{
  
  /**
   * @class DataModel contains all the data for simulation.
   * 
   */
  class DataModel:public QObject{
	
	Q_OBJECT

  public:
	DataModel(pTetMeshEmbeding embeding):_volObj(embeding){
	  assert(embeding);
	  _simulator = pSimulator(new Simulator());
	  _record = false;
	}
	bool loadSetting(const string filename){
	  
	  JsonFilePaser jsonf;
	  if (!jsonf.open(filename)){
		ERROR_LOG("failed to open: " << filename);
		return false;
	  }

	  bool succ = true;
	  string mtlfile;
	  if (jsonf.readFilePath("elastic_mtl",mtlfile)){
		succ = loadElasticMaterial(mtlfile);
	  }

	  string fixed_node_file;
	  if (jsonf.readFilePath("fixed_nodes", fixed_node_file)){
		succ &= loadFixedNodes(fixed_node_file);
	  }
	  
	  if (_simulator){
		succ &= _simulator->loadSetting(filename);
	  }

	  print();
	  return succ;
	}

	// set fixed nodes
	void addFixedNodes(const vector<int> &sel_ids){
	  for (size_t i = 0; i < sel_ids.size(); ++i){
		assert_ge(sel_ids[i],0);
		_fixedNodes.insert(sel_ids[i]);
	  }
	}
	void removeFixedNodes(const vector<int> &sel_ids){
	  for (size_t i = 0; i < sel_ids.size(); ++i){
		_fixedNodes.erase(sel_ids[i]);
	  }
	}

	// perturbation
	void setForces(const int nodeId,const double force[3]){
	  if(_simulator){
		_simulator->setExtForces(nodeId,force);
		this->simulate();
	  }
	}

	// get data
	const pObjmesh_const getObjMesh()const{
	  return _volObj->getObjMesh();
	}
	const pTetMesh_const getVolMesh()const{
	  return _volObj->getTetMesh();
	}
	const set<int> &getFixedNodes()const{
	  return _fixedNodes;
	}
	const ElasticMtlGroups &getElasticMtlGroups()const{
	  return _mtlGroups;
	}
	ElasticMtlGroups &getElasticMtlGroups(){
	  return _mtlGroups;
	}
	const VectorXd &getU()const{
	  assert(_simulator);
	  return _simulator->getU();
	}

	// io
	bool saveFixedNodes(const string filename)const{

	  OUTFILE(outf,filename.c_str());
	  if(!outf.is_open()) return false;
	  outf << _fixedNodes.size() << "\n";
	  BOOST_FOREACH(const int ele, _fixedNodes){
		outf << ele << " ";
	  }
	  const bool succ = outf.good();
	  outf.close();
	  return succ;
	}
	bool loadFixedNodes(const string filename){
	  
	  INFILE(in,filename.c_str());
	  if(!in.is_open()) return false;

	  bool succ = false;
	  int len = 0;
	  in >> len;
	  if (len > 0){

		succ = true;
		_fixedNodes.clear();
		for (int i = 0; i < len; ++i){

		  int nodeid = 0;
		  in >> nodeid;
		  if (nodeid >= 0){
			_fixedNodes.insert(nodeid);
		  }else{
			succ = false;
			ERROR_LOG("the fixed node's id is invalid: " << nodeid);
		  }
		}
	  }
	  return succ;
	}

	bool saveElasticMaterial(const string filename)const{
	  return _mtlGroups.save(filename);
	}
	bool loadElasticMaterial(const string filename){
	  bool succ = _mtlGroups.load(filename);
	  if (succ)	setMaterial();
	  return succ;
	}

	bool saveEigenVectors(const string filename)const{
	  return EIGEN3EXT::write(filename,_simulator->getW());
	}
	bool saveEigenValues(const string filename)const{
	  return EIGEN3EXT::write(filename,_simulator->getLambda());
	}

	// record
	const vector<VectorXd> &getRecoredZ()const{
	  return _recorded_z;
	}
	bool saveRecordZ(const string filename)const{
	  return EIGEN3EXT::write(filename,_recorded_z);
	}
	void print()const{
	  if (_simulator)
		_simulator->print();
	}

  public slots:
	void resetMaterialGroup(){
	  if(_volObj->getTetMesh()){
		_mtlGroups.reset(_volObj->getTetMesh()->tets().size());
	  }
	}
	void prepareSimulation(){

	  if(_simulator && _volObj->getTetMesh()){
		setMaterial();
		_simulator->setVolMesh(_volObj->getTetMesh());
		_simulator->setFixedNodes(_fixedNodes);
		const bool succ = _simulator->precompute();
		ERROR_LOG_COND("the precomputation is failed.",succ);
	  }
	}
	void setMaterial(){
	  if(_volObj && _volObj->getTetMesh())
		_mtlGroups.setMaterial(_volObj->getTetMesh()->material());
	}
	bool simulate(){
	  bool succ = false;
	  if(_simulator) succ = _simulator->simulate();
	  ERROR_LOG_COND("simulation failed.",succ);
	  if (succ && _record){
		_recorded_z.push_back(_simulator->getZ());
	  }
	  return succ;
	}
	void reset(){
	  if(_simulator)
		_simulator->reset();
	}
	void toggleRecord(){
	  _record = _record ? false:true;
	}
	void clearRecored(){
	  _recorded_z.clear();
	}
	
  private:
	pSimulator _simulator;
	pTetMeshEmbeding _volObj;
	set<int> _fixedNodes;
	ElasticMtlGroups _mtlGroups;
	bool _record;
	vector<VectorXd> _recorded_z;
  };
  
  typedef boost::shared_ptr<DataModel> pDataModel;
  
}//end of namespace

#endif /*_DATAMODEL_H_*/
