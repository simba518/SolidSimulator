#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

#include <boost/shared_ptr.hpp>
#include <set>
#include <eigen3/Eigen/Dense>
#include <TetMesh.h>
using namespace std;
using namespace Eigen;
using namespace UTILITY;

namespace SIMULATOR{

  /**
   * @class Simulator interface for the solid simulation.
   * 
   */
  class Simulator{
	
  public:
	virtual string name()const{
	  return "un-known-simulator";
	}
	virtual bool init(const string filename) = 0;
	virtual void setVolMesh(pTetMesh_const tetMesh) = 0;
	virtual bool precompute(){}
	virtual void reset() = 0;

	virtual void setConNodes(const set<int> &con_nodes) = 0;
	virtual void setUc(const VectorXd &uc) = 0;
	virtual void removeAllConNodes() = 0;

	virtual void setExtForces(const int nodeId,const double f[3]) = 0;
	virtual void clearExtForces() = 0;

	virtual bool forward() = 0;

	virtual const VectorXd &getFullDisp()const = 0;

  };
  typedef boost::shared_ptr<Simulator> pSimulator;
  
}//end of namespace

#endif /*_SIMULATOR_H_*/
