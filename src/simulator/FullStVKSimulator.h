#ifndef _FULLSTVKSIMULATOR_H_
#define _FULLSTVKSIMULATOR_H_

#include "Simulator.h"
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
	  
	}
	bool init(const string filename){
	  
	}
	void reset(){
	  clearExtForces();
	  removeAllConNodes();
	}

	void setConNodes(const set<int> &nodes){
	  
	}
	void setUc(const VectorXd &uc){
	  
	}
	void removeAllConNodes(){
	  
	}
	
	void setExtForces(const int nodeId,const double f[3]){
	  
	}
	void clearExtForces(){
	  
	}

	bool precompute(){
	  
	}
	bool forward(){
	  
	}

	const VectorXd &getFullDisp()const{
	  
	}
	
  private:
	pFullStVKSimModel stvkModel;
	pLagImpFullSim simulator;	
	VectorXd f_ext;
	
  };
  
  typedef boost::shared_ptr<FullStVKSimulator> pFullStVKSimulator;
  
}//end of namespace

#endif /*_FULLSTVKSIMULATOR_H_*/
