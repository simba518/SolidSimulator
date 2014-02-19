#ifndef _FULLSTVKSIMULATOR_H_
#define _FULLSTVKSIMULATOR_H_

#include "Simulator.h"
#include <ConMatrixTools.h>
#include <LagImpFullSim.h>
#include <FullStVKSimModel.h>
using namespace UTILITY;

namespace SIMULATOR{
  
  /**
   * @class FullStVKSimulator solid simulation using full stvk model.
   * 
   */
  class FullStVKSimulator:public Simulator{
	
  public:
	FullStVKSimulator(){
	  stvkModel = pFullStVKSimModel(new FullStVKSimModel());
	  simulator = pLagImpFullSim(new LagImpFullSim(stvkModel));
	}
	void setVolMesh(pTetMesh_const tetMesh){
	  stvkModel->setTetMesh(tetMesh);
	}
	bool init(const string filename){
	  bool succ = stvkModel->init(filename);
	  if (succ){
		succ = simulator->init(filename);
	  }
	  return succ;
	}
	void reset(){
	  clearExtForces();
	  removeAllConNodes();
	  simulator->reset();
	}

	void setConNodes(const set<int> &nodes){

	  VecT trip_C;
	  const int n = stvkModel->dimension()/3;
	  UTILITY::computeConM(nodes, trip_C, n);
	  simulator->setConM(trip_C);
	}
	void setUc(const VectorXd &uc){
	  simulator->setUc(uc);
	}
	void removeAllConNodes(){
	  simulator->removeAllCon();
	}
	
	void setExtForces(const int nodeId,const double f[3]){
	  simulator->setExtForceOfNode(f, nodeId);
	}
	void clearExtForces(){
	  simulator->setExtForceForAllNodes(0.0f,0.0f,0.0f);
	}

	bool precompute(){
	  return simulator->prepare();
	}
	bool forward(){
	  return simulator->forward();
	}

	const VectorXd &getFullDisp()const{
	  return simulator->getU();
	}
	
  private:
	pFullStVKSimModel stvkModel;
	pLagImpFullSim simulator;
  };
  
  typedef boost::shared_ptr<FullStVKSimulator> pFullStVKSimulator;
  
}//end of namespace

#endif /*_FULLSTVKSIMULATOR_H_*/
