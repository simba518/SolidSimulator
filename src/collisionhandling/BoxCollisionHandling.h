#ifndef _BOXCOLLISIONHANDLING_H_
#define _BOXCOLLISIONHANDLING_H_

#include <limits>
#include <FullSimulator.h>
#include "MPRGP.h"

namespace SIMULATOR{
  
  /**
   * @class BoxCollisionHandling handling the most simple case of
   * collision using MPRGP with boundary constraints.
   * 
   */
  class BoxCollisionHandling:public BaseFullSim{
	
  public:
	BoxCollisionHandling():BaseFullSim(){removeBBox();}
	BoxCollisionHandling(pBaseFullModel def_model):BaseFullSim(def_model){removeBBox();}
	void setConM(const VecT &C_triplet,const int C_rows,const int C_cols){/*no constraints*/}
	bool prepare(){
	  bool succ = BaseFullSim::prepare();
	  if (succ)
		succ = def_model->evaluateM(M);
	  return succ;
	}
	bool forward(){
	  
	  return true;
	}

	// bounding box
	void removeBBox(){
	  const double dMax = std::numeric_limits<double>::max();
	  Lower << -dMax, -dMax, -dMax;
	  Upper << dMax, dMax, dMax;
	}
	void setXBBox(const double L, const double U){
	  assert_gt(U,L);
	  Lower[0] = L;
	  Upper[0] = U;
	}
	void setYBBox(const double L, const double U){
	  assert_gt(U,L);
	  Lower[1] = L;
	  Upper[1] = U;
	}
	void setZBBox(const double L, const double U){
	  assert_gt(U,L);
	  Lower[2] = L;
	  Upper[2] = U;
	}
	void printBBox()const{
	  cout << "The bounding box is: " << Lower.transpose() << ", "<< Upper.transpose() <<"\n";
	}

  private:
	Vector3d Lower, Upper; // bounding box.
	SparseMatrix<double> M; // mass matrix.
  };
  
}//end of namespace

#endif /*_BOXCOLLISIONHANDLING_H_*/
