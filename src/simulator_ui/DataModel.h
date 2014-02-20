#ifndef _DATAMODEL_H_
#define _DATAMODEL_H_

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <Log.h>
#include <AuxTools.h>
#include <MatrixIO.h>
#include <TetMeshEmbeding.h>
#include <JsonFilePaser.h>
#include <Simulator.h>
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
	const string simulatorName()const{
	  if (_simulator) return _simulator->name();
	  return "no simulator.";
	}
	pSimulator createSimulator(const string filename)const;
	bool loadSetting(const string filename);

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
	void setForces(const int nodeId,const double force[3]);

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
	const VectorXd &getU()const{
	  assert(_simulator);
	  return _simulator->getFullDisp();
	}

	// io
	bool saveFixedNodes(const string filename)const;
	bool loadFixedNodes(const string filename);
	void print()const{}

  public slots:
	void prepareSimulation();
	bool simulate();
	void reset(){
	  if(_simulator) _simulator->reset();
	}
	
  private:
	pSimulator _simulator;
	pTetMeshEmbeding _volObj;
	set<int> _fixedNodes;
  };
  
  typedef boost::shared_ptr<DataModel> pDataModel;
  
}//end of namespace

#endif /*_DATAMODEL_H_*/
