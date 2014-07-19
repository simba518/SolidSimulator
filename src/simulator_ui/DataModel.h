#ifndef _DATAMODEL_H_
#define _DATAMODEL_H_

#include <QObject>
#include <boost/shared_ptr.hpp>
#include <Log.h>
#include <AuxTools.h>
#include <MatrixIO.h>
#include <TetMeshEmbeding.h>
#include <JsonFilePaser.h>
#include <Simulator.h>
#include <PartialConstraints.h>
#include "PassiveObject.h"
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
	DataModel(pTetMeshEmbeding embeding);
	void setPassiveObject(pPassiveObject p){
	  this->_passiveObject = p;
	}
	const string simulatorName()const{
	  if (_simulator) return _simulator->name();
	  return "no simulator.";
	}
	pSimulator createSimulator(const string filename)const;
	bool loadSetting(const string filename);
	void setRecord(const bool record){
	  _record = record;
	}

	// set fixed nodes
	void addConNodes(const vector<int> &sel_ids){
	  if (sel_ids.size() > 0){
		_partialCon.addConNodes(sel_ids);
		_partialCon_x0.addConNodes(sel_ids);
		resetPartialCon();
	  }
	}
	void removeConNodes(const vector<int> &sel_ids){
	  if (sel_ids.size() > 0){
		_partialCon.rmConNodes(sel_ids);
		_partialCon_x0.rmConNodes(sel_ids);
		resetPartialCon();
	  }
	}
	const vector<set<int> > &getConNodes()const{
	  return _partialCon.getConNodesSet();
	}
	void updateUc(const Matrix<double,3,-1> &uc,const int group_id);
	void updateXc(const Matrix<double,3,-1> &xc,const int group_id);

	// perturbation
	void setForces(const int nodeId,const double force[3]);

	// get data
	const pObjmesh_const getObjMesh()const{
	  return _volObj->getObjMesh();
	}
	const pTetMesh_const getVolMesh()const{
	  return _volObj->getTetMesh();
	}
	const pObjmesh_const getRestObjMesh()const{
	  return _restObj;
	}
	const pObjmesh_const getScene()const{
	  return scene;
	}
	const VectorXd &getU()const{
	  static VectorXd tempt_u;
	  if (_simulator){
		return _simulator->getFullDisp();
	  }
	  return tempt_u;
	}
	const Matrix<double,3,-1> getUc(const int group)const{
	  assert_in(group,0,_partialCon.numGroup()-1);
	  return _partialCon.getPc(group);
	}
	const Matrix<double,3,-1> getXc(const int group)const;

	// io
	void print()const{}
	const vector<VectorXd> &getRecord()const{
	  return _recorded_vol_u;
	}
	bool saveRecord(const string filename)const{
	  return EIGEN3EXT::write(filename,_recorded_vol_u);
	}
	bool saveRecordAsVTK(const string filename)const{
	  if(_volObj && _volObj->getTetMesh())
		return _volObj->getTetMesh()->writeVTK(filename, _recorded_vol_u);
	  return false;
	}
	bool saveMtlAsVTK(const string filename)const{
	  if(_volObj && _volObj->getTetMesh())
		return _volObj->getTetMesh()->writeElasticMtlVTK(filename);
	  return false;
	}
	bool saveVolMesh(const string filename)const{
	  if(_volObj && _volObj->getTetMesh())
		return _volObj->getTetMesh()->write(filename,getU());
	  return false;
	}
	bool saveConNodes(const string filename)const;

  public slots:
	void prepareSimulation();
	bool simulate();
	void reset(){
	  if(_simulator) _simulator->reset();
	  _partialCon.setZero();
	  resetPartialCon();
	}
	void toggleRecord(){
	  _record = _record ? false:true;
	}
	void clearRecord(){
	  _recorded_vol_u.clear();
	}

  protected:
	void resetPartialCon();
	void getSubUc(const vector<set<int> > &groups,const VectorXd &full_u,Matrix<double,3,-1> &sub_u)const;
	
  private:
	PartialConstraints _partialCon;
	PartialConstraints _partialCon_x0;
	pSimulator _simulator;
	pTetMeshEmbeding _volObj;
	pObjmesh _restObj;
	VectorXd rest_shape;
	int steps; // number of steps for each simulation.
	bool _record;
	vector<VectorXd> _recorded_vol_u;
	pObjmesh scene;
	pPassiveObject _passiveObject;
  };
  
  typedef boost::shared_ptr<DataModel> pDataModel;
  
}//end of namespace

#endif /*_DATAMODEL_H_*/
