#ifndef _SUBSPACESIMULATOR_H_
#define _SUBSPACESIMULATOR_H_

#include "Simulator.h"
#include <ReducedSimulator.h>
#include <boost/foreach.hpp>
using namespace UTILITY;

namespace SIMULATOR{
  
  /**
   * @class SubspaceSimulator solid simulation using cubature method.
   * 
   */
  class SubspaceSimulator:public Simulator{
	
  public:
	SubspaceSimulator(){
	  stvkModel = pReducedElasticModel(new DirectReductionElasticModel());
	  simulator = pReducedSimulator(new ReducedImpLogConSimulator(stvkModel));
	}
	string name()const{
	  return "subspace";
	}
	bool init(const string filename){
	  bool succ = stvkModel->init(filename);
	  if (succ){
		succ = simulator->init(filename);
	  }
	  return succ;
	}
	void setVolMesh(pTetMesh_const tetMesh){
	  stvkModel->setVolMesh(tetMesh);
	}
	bool precompute(){
	  const bool succ = simulator->prepare();
	  if (stvkModel){
		full_disp.resize(stvkModel->fullDim());
		full_disp.setZero();
	  }
	  return succ;
	}
	void reset(){
	  clearExtForce();
	  removeAllConNodes();
	  simulator->reset();
	  if (stvkModel){
		full_disp.resize(stvkModel->fullDim());
		full_disp.setZero();
	  }
	}

	void setConNodes(const vector<int> &nodes){
	  simulator->setConGroups(nodes);
	}
	void setUc(const VectorXd &uc){
	  simulator->setUc(uc);
	}
	void removeAllConNodes(){
	  simulator->removeAllCon();
	}
	
	void setExtForceOfNode(const int node_id,const double f[3]){

	  static VectorXd full_ext;
	  full_ext.resize(stvkModel->fullDim());
	  full_ext.setZero();
	  full_ext[node_id*3+0] = f[0];
	  full_ext[node_id*3+1] = f[1];
	  full_ext[node_id*3+2] = f[2];
	  simulator->setExtForce(full_ext);
	}
	void setExtForce(const VectorXd &f_ext){
	  simulator->setExtForce(f_ext);
	}
	void clearExtForce(){
	  VectorXd full_ext(stvkModel->fullDim());
	  full_ext.setZero();
	  simulator->setExtForce(full_ext);
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
	pReducedElasticModel stvkModel;
	pReducedSimulator simulator;
	VectorXd full_disp;
  };
  
  typedef boost::shared_ptr<SubspaceSimulator> pSubspaceSimulator;
  
}//end of namespace

#endif /* _SUBSPACESIMULATOR_H_ */
