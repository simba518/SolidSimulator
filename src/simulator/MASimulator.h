#ifndef _MASIMULATOR_H_
#define _MASIMULATOR_H_

#include "Simulator.h"

namespace SIMULATOR{
  
  /**
   * @class MASimulator solid simulation using modal analysis.
   * 
   */
  class MASimulator:public Simulator{
	
  public:
	MASimulator(){
	  _h = 0.02f;
	  _ak = 0.0f;
	  _am = 0.0f;
	}
	void setVolMesh(pTetMesh_const tetMesh){
	  _tetMesh = tetMesh;
	}
	bool init(const string filename);
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
	  removeAllConNodes();
	}

	void setConNodes(const set<int> &nodes){
	  _fixedNodes = nodes;
	}
	void setUc(const VectorXd &uc){}
	void removeAllConNodes(){
	  _fixedNodes.clear();
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
	void clearExtForces(){
	  if(_tetMesh){
		_fext.resize(_tetMesh->nodes().size()*3);
		_fext.setZero();
	  }
	}

	bool precompute();
	bool forward();

	const VectorXd &getFullDisp()const{
	  return _u;
	}

  private:
	pTetMesh_const _tetMesh;
	set<int> _fixedNodes;
	int _eigenNum;

	MatrixXd _W;
	VectorXd _lambda;

	double _h,_ak,_am;
	VectorXd _z,_v,_F_reduced,_u, _fext;
  };
  
  typedef boost::shared_ptr<MASimulator> pMASimulator;
  
}//end of namespace

#endif /*_MASIMULATOR_H_*/
