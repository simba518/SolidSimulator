#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

#include <boost/shared_ptr.hpp>
#include <set>
#include <TetMesh.h>
#include <eigen3/Eigen/Dense>
using namespace std;
using namespace Eigen;
using namespace UTILITY;

namespace SIMULATOR{
  
  class Simulator{
	
  public:
	Simulator(const double h = 0.02f,const double ak=0.0,const double am=0.0){
	  setTimeStep(h);
	  setDamping(ak,am);
	  _eigenNum = 30;
	}
	void setVolMesh(pTetMesh_const tetMesh){
	  _tetMesh = tetMesh;
	}
	void setTimeStep(const double h){
	  assert_gt(h,0.0f);
	  _h = h;
	}
	void setDamping(const double ak,const double am){
	  assert_ge(ak,0.0f);
	  assert_ge(am,0.0f);
	  _ak = ak;
	  _am = am;
	}
	void setEigenNumbers(const int n){
	  assert_ge(n,1);
	  _eigenNum = n;
	}
	void setFixedNodes(const set<int> &nodes){
	  _fixedNodes = nodes;
	}

	bool precompute();
	
	void clearExtForces(){
	  if(_tetMesh){
		_fext.resize(_tetMesh->nodes().size()*3);
		_fext.setZero();
	  }
	}
	void setExtForces(const int nodeId,const double f[3]){

	  if (!_tetMesh)
		return;

	  if(_fext.size() != _tetMesh->nodes().size()*3)
		clearExtForces();
	  assert_in(nodeId*3+2,0,_fext.size()-1);
	  _fext[nodeId*3+0] = f[0];
	  _fext[nodeId*3+1] = f[1];
	  _fext[nodeId*3+2] = f[2];
	}
	bool simulate();

	const VectorXd &getU()const{
	  return _u;
	}
	const set<int> &getFixedNodes()const{
	  return _fixedNodes;
	}
	void reset(){
	  if(_tetMesh){
		_u.resize(_tetMesh->nodes().size()*3);
		_u.setZero();

		_v.resize(_W.cols());
		_v.setZero(_W.cols());
		_z.resize(_W.cols());
		_z.setZero(_W.cols());
	  }
	  clearExtForces();
	}

  protected:
	void forward();
	
  private:
	pTetMesh_const _tetMesh;
	set<int> _fixedNodes;
	int _eigenNum;

	MatrixXd _W;
	VectorXd _lambda;

	double _h,_ak,_am;
	VectorXd _z,_v,_F_reduced,_u, _fext;
  };
  
  typedef boost::shared_ptr<Simulator> pSimulator;
  
}//end of namespace

#endif /*_SIMULATOR_H_*/
