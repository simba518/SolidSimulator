#ifndef _DATAMODEL_H_
#define _DATAMODEL_H_

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <Log.h>
#include <PartialConstraints.h>
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
	  _posCon.addConNodes(sel_ids);
	}
	void removeFixedNodes(const vector<int> &sel_ids){
	  _posCon.rmConNodes(sel_ids);
	}

	// perturbation
	void setForces(const int nodeId,const double force[3]){
	  ///@todo
	}

	// get data
	const pObjmesh_const getObjMesh()const{
	  return _volObj->getObjMesh();
	}
	const pTetMesh_const getVolMesh()const{
	  return _volObj->getTetMesh();
	}
	const PartialConstraints &getPosConstraints()const{
	  return _posCon;
	}
	const ElasticMtlGroups &getElasticMtlGroups()const{
	  return _mtlGroups;
	}
	ElasticMtlGroups &getElasticMtlGroups(){
	  return _mtlGroups;
	}

	// io
	bool saveConNodes(const string filename)const{
	  ofstream outf(filename.c_str());
	  if(!outf.is_open()){
		ERROR_LOG("failed to open file " << filename);
		return false;
	  }
	  const vector<set<int> > &vs = _posCon.getConNodesSet();
	  outf << _posCon.numConNodes() << "\n";
	  BOOST_FOREACH(const set<int> &s, vs){
		BOOST_FOREACH(const int ele, s){
		  outf << ele << " ";
		}
	  }
	  const bool succ = outf.good();
	  outf.close();
	  return succ;
	}
	bool loadConNodes(const string filename){
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
	void setMaterial(){
	  if(_volObj && _volObj->getTetMesh())
		_mtlGroups.setMaterial(_volObj->getTetMesh()->material());
	}
	bool simulate(){
	  if(_simulator)
		return _simulator->simulate();
	  return false;
	}
	
  private:
	pSimulator _simulator;
	pTetMeshEmbeding _volObj;
	PartialConstraints _posCon;
	ElasticMtlGroups _mtlGroups;
  };
  
  typedef boost::shared_ptr<DataModel> pDataModel;
  
}//end of namespace

#endif /*_DATAMODEL_H_*/
