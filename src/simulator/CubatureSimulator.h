#ifndef _CUBATURESIMULATOR_H_
#define _CUBATURESIMULATOR_H_

#include "Simulator.h"
#include <ReducedSimulator.h>
using namespace UTILITY;

namespace SIMULATOR{
  
  /**
   * @class CubatureSimulator solid simulation using cubature method.
   * 
   */
  class CubatureSimulator:public Simulator{
	
  public:
	CubatureSimulator(){
	  stvkModel = pCubaturedElasticModel(new CubaturedElasticModel());
	  simulator = pReducedSimulator(new ReducedImpLogConSimulator(stvkModel));
	}
	void setVolMesh(pTetMesh_const tetMesh){
	  stvkModel->setVolMesh(tetMesh);
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
	  vector<int> con_nodes;
	  con_nodes.reserve(nodes.size());
	  BOOST_FOREACH(const int ele, nodes){
		con_nodes.push_back(ele);
	  }
	  simulator->setConGroups(con_nodes);
	}
	void setUc(const VectorXd &uc){
	  simulator->setUc(uc);
	}
	void removeAllConNodes(){
	  simulator->removeAllCon();
	}
	
	void setExtForces(const int node_id,const double f[3]){

	  static VectorXd full_ext;
	  full_ext.resize(stvkModel->fullDim());
	  full_ext.setZero();
	  full_ext[node_id*3+0] = f[0];
	  full_ext[node_id*3+1] = f[1];
	  full_ext[node_id*3+2] = f[2];
	  simulator->setExtForce(full_ext);
	}
	void clearExtForces(){
	  VectorXd full_ext(stvkModel->fullDim());
	  full_ext.setZero();
	  simulator->setExtForce(full_ext);
	}

	bool precompute(){
	  return simulator->prepare();
	}
	bool forward(){
	  bool succ = simulator->forward();
	  succ &= stvkModel->computeFullDisp(simulator->getQ(),full_disp);
	  return succ;
	}

	const VectorXd &getFullDisp()const{
	  return full_disp;
	}

  private:
	pCubaturedElasticModel stvkModel;
	pReducedSimulator simulator;
	VectorXd full_disp;
  };
  
  typedef boost::shared_ptr<CubatureSimulator> pCubatureSimulator;
  
}//end of namespace

#endif /*_CUBATURESIMULATOR_H_*/
