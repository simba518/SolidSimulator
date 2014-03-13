#ifndef _MASIMULATOR_H_
#define _MASIMULATOR_H_

#include "Simulator.h"
#include <boost/foreach.hpp>
using namespace UTILITY;

namespace SIMULATOR{
  
  /**
   * @class MASimulator solid simulation using modal analysis.
   * 
   */
  class MASimulator:public Simulator{
	
  public:
	MASimulator(const string name = "Modal analysis"):sim_name(name){
	  
	}
	string name()const{
	  return sim_name;
	}
	bool init(const string filename);
	void setVolMesh(pTetMesh_const tetMesh){
	  _tetMesh = tetMesh;
	}
	bool precompute();
	void reset(){
	  // Simulator::reset();
	  if(_tetMesh){
		_u.resize(_tetMesh->nodes().size()*3);
		_u.setZero();

		_v.resize(_W.cols());
		_v.setZero(_W.cols());
		_z.resize(_W.cols());
		_z.setZero(_W.cols());
	  }
	}

	void setConNodes(const vector<int> &nodes){
	  _fixedNodes.clear();
	  for (int i = 0; i < nodes.size(); ++i){
		assert_ge(nodes[i],0);
		_fixedNodes.insert(nodes[i]);
	  }
	}
	void setUc(const VectorXd &uc){
	  
	}
	void removeAllConNodes(){
	  _fixedNodes.clear();
	}
	
	void setExtForce(const VectorXd &f_ext){
	  _fext = f_ext;
	}

	bool forward();

	const VectorXd &getFullDisp()const{
	  return _u;
	}

  protected:
	void solve();

  private:
	const string sim_name;

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

#endif /* _MASIMULATOR_H_ */
