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
	SubspaceSimulator();
	SubspaceSimulator(pReducedElasticModel elasticModel,const string name):sim_name(name){
	  stvkModel = elasticModel;
	  simulator = pReducedSimulator(new ReducedImpLogConSimulator(stvkModel));
	}
	SubspaceSimulator(pReducedElasticModel elasticModel,pReducedSimulator sim,const string name):sim_name(name){
	  stvkModel = elasticModel;
	  simulator = sim;
	}
	string name()const{
	  return sim_name;
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
		full_velocity.resize(stvkModel->fullDim());
		full_velocity.setZero();
	  }
	  return succ;
	}
	void reset(){
	  Simulator::reset();
	  simulator->reset();
	  if (stvkModel){
		full_disp.resize(stvkModel->fullDim());
		full_disp.setZero();
		full_velocity.resize(stvkModel->fullDim());
		full_velocity.setZero();
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
	
	void setExtForce(const VectorXd &f_ext){
	  simulator->setExtForce(f_ext);
	}

	bool forward();

	const VectorXd &getFullDisp()const{
	  return full_disp;
	}
	const VectorXd &getFullVelocity()const{
	  return full_velocity;
	}
	VectorXd &getFullDisp(){
	  return full_disp;
	}
	VectorXd &getFullVelocity(){
	  return full_velocity;
	}
	bool computeElasticForce(const VectorXd &u,VectorXd &f)const{
	  bool succ = false;
	  if (stvkModel){
		const MatrixXd &B = stvkModel->getModalBasis();
		const VectorXd q = B.transpose()*u;
		VectorXd fq;
		succ = stvkModel->evaluateF(q,fq);
		f = B*fq;
	  }
	  return succ;
	}

  private:
	const string sim_name;
	pReducedElasticModel stvkModel;
	pReducedSimulator simulator;
	VectorXd full_disp;
	VectorXd full_velocity;
  };
  
  typedef boost::shared_ptr<SubspaceSimulator> pSubspaceSimulator;
  
}//end of namespace

#endif /* _SUBSPACESIMULATOR_H_ */
