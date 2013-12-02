#ifndef _DATAMODEL_H_
#define _DATAMODEL_H_

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <Log.h>
#include <AuxTools.h>
#include <TetMeshEmbeding.h>
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
	}
	bool loadSetting(const string filename){
	  return false;
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
	  return false;
	}
	bool saveElasticMaterial(const string filename)const{
	  return _mtlGroups.save(filename);
	}

  public slots:
	void resetMaterialGroup(){
	  if(_volObj->getTetMesh()){
		_mtlGroups.reset(_volObj->getTetMesh()->tets().size());
	  }
	}
	void prepareSimulation(){

	  if(_simulator && _volObj->getTetMesh()){
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
	  return succ;
	}
	void reset(){
	  if(_simulator)
		_simulator->reset();
	}
	
  private:
	pSimulator _simulator;
	pTetMeshEmbeding _volObj;
	set<int> _fixedNodes;
	ElasticMtlGroups _mtlGroups;
  };
  
  typedef boost::shared_ptr<DataModel> pDataModel;
  
}//end of namespace

#endif /*_DATAMODEL_H_*/
