#ifndef _BOXCOLLISIONHANDLING_H_
#define _BOXCOLLISIONHANDLING_H_

#include <limits>
#include <FullSimulator.h>
#include <Eigen/UmfPackSupport>
#include <MPRGPSolver.h>
using namespace MATH;
// #include "QPSolver.h"

namespace SIMULATOR{
  
  /**
   * @class BoxCollisionHandling handling the most simple case of
   * collision using MPRGP with boundary constraints.
   * 
   */
  class BoxCollisionHandling:public BaseFullSim{
	
  public:
	BoxCollisionHandling():BaseFullSim(){
	  tolerance = 1e-3;
	  max_iteration = 1000;
	  removeBBox();
	}
	BoxCollisionHandling(pBaseFullModel def_model):BaseFullSim(def_model){removeBBox();}
	bool init(const string ini_file){
	  bool succ = BaseFullSim::init(ini_file);
	  if (succ){
		JsonFilePaser jsonf;
		succ = jsonf.open(ini_file);
		if (succ){
		  vector<double> BBox;
		  succ = jsonf.read("bound_box",BBox);
		  if(succ) setBBox(BBox);
		  vector<double> gravity;
		  succ &= jsonf.read("gravity",gravity);
		  if (gravity.size() == 3)
			this->setExtForceForAllNodes(gravity[0],gravity[1],gravity[2]);
		  jsonf.read("max_iteration",max_iteration,(unsigned int)1000); 
		  assert_gt(max_iteration,0);
		  jsonf.read("tolerance",tolerance,1e-3); assert_ge(tolerance,0.0f);
		}
	  }
	  return succ;
	}
	void setConM(const VecT &C_triplet,const int C_rows,const int C_cols){/*no constraints*/}
	bool prepare(){
	  bool succ = BaseFullSim::prepare();
	  if (succ)
		succ = def_model->evaluateM(M);
	  return succ;
	}
	bool forward(){

	  // initialize u,v,f,K
	  const VectorXd u1 = u;
	  const VectorXd u0 = u1-h*v;
	  const VectorXd &x0 = def_model->getRestShape();
	  SparseMatrix<double> K;
	  def_model->evaluateK(u1,K);
	  VectorXd f_int;
	  def_model->evaluateF(u1,f_int);

	  // assemble A,b
	  const SparseMatrix<double> A = M*(1.0f/(h*h))+(1.0f/h)*(alpha_m*M+alpha_k*K)+K;
	  const VectorXd b = fext-( M*(-2.0f*u1+u0)*(1.0f/(h*h))-(1.0f/h)*(alpha_m*M+alpha_k*K)*u1+f_int-K*u1 )+A*x0;
	  const FixedSparseMatrix<double> As(A);

	  // set bound constraints
	  VectorXd L(x0.size()),U(x0.size());
	  for (int i = 0; i < L.size()/3; ++i){
		L.segment<3>(i*3) = Lower;
		U.segment<3>(i*3) = Upper;
	  }

	  // solve
	  // UmfPackLU<SparseMatrix<double> > solver;
	  // solver.compute(A);
	  // const VectorXd x = solver.solve(b);

	  // MPRGPQPSolver<double> solver(As,b,L,U,true);
	  // solver.setSolverParameters(tolerance,max_iteration);
	  // VectorXd x = x0+u1;
	  // solver.solve(x);
	  // cout << "\niterations: " << solver.iterationsOut();
	  // cout << "\nresidual: " << solver.residualOut() << "\n";

	  // VectorXd x = x0+u1;
	  // MPRGPBoxBound<double>::solve(As,b,L,U,x,tolerance,max_iteration);


	  VectorXd x = x0+u1;
	  vector<Vector4d,aligned_allocator<Vector4d> > planes;
	  Vector4d p;

	  p << 1,0.1,0,1.0;
	  p.head(3) /= p.head(3).norm();
	  planes.push_back(p);

	  // p << -1,1,0,1.0;
	  // p.head(3) /= p.head(3).norm();
	  // planes.push_back(p);
	  MPRGPPlane<double>::solve(As,b,planes,x,tolerance,max_iteration);

	  // update u,v
	  u = x-x0;
	  v = (u-u1)*(1.0f/h);
	  return true;
	}

	// bounding box
	void removeBBox(){
	  const double dMax = std::numeric_limits<double>::max();
	  Lower << -dMax, -dMax, -dMax;
	  Upper << dMax, dMax, dMax;
	}
	template<class VECTOR>
	void setBBox(const VECTOR &XYZ){
	  assert_eq(XYZ.size(),6);
	  Lower << XYZ[0],XYZ[2],XYZ[4];
	  Upper << XYZ[1],XYZ[3],XYZ[5];
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
	double tolerance;
	unsigned int max_iteration;
	Vector3d Lower, Upper; // bounding box.
	SparseMatrix<double> M; // mass matrix.
  };
  
}//end of namespace

#endif /*_BOXCOLLISIONHANDLING_H_*/
