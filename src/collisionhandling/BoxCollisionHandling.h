#ifndef _BOXCOLLISIONHANDLING_H_
#define _BOXCOLLISIONHANDLING_H_

#include <limits>
#include <FullSimulator.h>
#include <Eigen/UmfPackSupport>
#include <MPRGPSolver.h>
using namespace MATH;
// #include "QPSolver.h"

namespace SIMULATOR{

  // base class for the collision handling simulator.
  class CollisionHandlingSim:public BaseFullSim{
	
  public:
	CollisionHandlingSim():BaseFullSim(){
	  tolerance = 1e-3;
	  max_iteration = 1000;
	}
	CollisionHandlingSim(pBaseFullModel def_model):BaseFullSim(def_model){}
	virtual bool init(const string ini_file){
	  bool succ = BaseFullSim::init(ini_file);
	  if (succ){
		JsonFilePaser jsonf;
		succ = jsonf.open(ini_file);
		if (succ){
		  jsonf.read("max_iteration",max_iteration,(unsigned int)1000); 
		  assert_gt(max_iteration,0);
		  jsonf.read("tolerance",tolerance,1e-3); assert_ge(tolerance,0.0f);
		}
	  }
	  return succ;
	}
	virtual void setConM(const VecT &C_triplet,const int C_rows,const int C_cols){/*no constraints*/}
	virtual bool prepare(){
	  bool succ = BaseFullSim::prepare();
	  if (succ)
		succ = def_model->evaluateM(M);
	  return succ;
	}
	virtual bool forward(){

	  // initialize u,v,f,K
	  const VectorXd u1 = u;
	  const VectorXd u0 = u1-h*v;
	  const VectorXd &x0 = def_model->getRestShape();
	  SparseMatrix<double> K;
	  def_model->evaluateK(u1,K);
	  VectorXd f_int;
	  def_model->evaluateF(u1,f_int);

	  // assemble A,b for implicit integration.
	  const SparseMatrix<double> A = M*(1.0f/(h*h))+(1.0f/h)*(alpha_m*M+alpha_k*K)+K;
	  const VectorXd b = fext-( M*(-2.0f*u1+u0)*(1.0f/(h*h))-(1.0f/h)*(alpha_m*M+alpha_k*K)*u1+f_int-K*u1 )+A*x0;

	  // set initial value for x, then solve it.
	  VectorXd x = x0+u1;
	  bool succ = solve(A,b,x);

	  // update u,v
	  u = x-x0;
	  v = (u-u1)*(1.0f/h);
	  return succ;
	}

  protected:
	virtual bool solve(const SparseMatrix<double> &A, const VectorXd &b, VectorXd &x){

	  UmfPackLU<SparseMatrix<double> > solver;
	  solver.compute(A);
	  x = solver.solve(b);
	  return true;
	}

  protected:
	double tolerance;
	unsigned int max_iteration;
	SparseMatrix<double> M; // mass matrix.
  };
  
  /**
   * @class BoxCollisionHandling handling the most simple case of
   * collision using MPRGP with boundary constraints.
   * 
   */
  class BoxCollisionHandling:public CollisionHandlingSim{
	
  public:
	BoxCollisionHandling():CollisionHandlingSim(){
	  removeBBox();
	}
	BoxCollisionHandling(pBaseFullModel def_model):CollisionHandlingSim(def_model){removeBBox();}
	bool init(const string ini_file){
	  bool succ = CollisionHandlingSim::init(ini_file);
	  if (succ){
		JsonFilePaser jsonf;
		succ = jsonf.open(ini_file);
		if (succ){
		  vector<double> BBox;
		  succ = jsonf.read("bound_box",BBox);
		  if(succ) setBBox(BBox);
		}
	  }
	  return succ;
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

  protected:
	bool solve(const SparseMatrix<double> &A, const VectorXd &b, VectorXd &x){

	  // set bound constraints
	  VectorXd L(x.size()),U(x.size());
	  for (int i = 0; i < L.size()/3; ++i){
		L.segment<3>(i*3) = Lower;
		U.segment<3>(i*3) = Upper;
	  }
	  
	  const FixedSparseMatrix<double> As(A);
	  MPRGPBoxBound<double>::solve(As,b,L,U,x,tolerance,max_iteration);
	  // MPRGPQPSolver<double> solver(As,b,L,U,true);
	  // solver.setSolverParameters(tolerance,max_iteration);
	  // solver.solve(x);
	  // cout << "\niterations: " << solver.iterationsOut();
	  // cout << "\nresidual: " << solver.residualOut() << "\n";
	  return true;
	}

  protected:
	Vector3d Lower, Upper; // bounding box.
  };

  /**
   * @class PlaneCollisionHandling handling the collision between
   * the general planes and solid objects.
   * 
   * The planes are defined as a set of 4d vectors, and each 4d vector
   * is [n,p], where n is the normal and p is the distance between
   * (0,0,0) and the plane. Thus the equatio of a plane is:
   * n.dot(x)+p=0.
   */
  class PlaneCollisionHandling:public CollisionHandlingSim{
	
  public:
	PlaneCollisionHandling():CollisionHandlingSim(){}
	PlaneCollisionHandling(pBaseFullModel def_model):CollisionHandlingSim(def_model){}
	bool init(const string ini_file){

	  bool succ = CollisionHandlingSim::init(ini_file);
	  if (succ){
		JsonFilePaser jsonf;
		succ = jsonf.open(ini_file);
		if (succ){
		  vector<double> planes;
		  succ = jsonf.read("planes",planes);
		  if(succ) setPlanes(planes);
		}
	  }
	  return succ;
	}

	template<class VECTOR>
	void setPlanes(const VECTOR &planes_vec_n_p){
	  
	  assert_eq(planes_vec_n_p.size()%4,0);
	  planes.clear();
	  planes.reserve(planes_vec_n_p.size()/4);

	  Vector4d p;
	  p.setZero();
	  for (int i = 0; i < planes_vec_n_p.size(); i+=4){
		p << planes_vec_n_p[i], planes_vec_n_p[i+1], planes_vec_n_p[i+2], planes_vec_n_p[i+3];
		const double p_norm = p.head(3).norm();
		assert_gt(p_norm,0);
		p.head(3) *= 1.0/p_norm;
		planes.push_back(p);
	  }
	}

  protected:
	bool solve(const SparseMatrix<double> &A, const VectorXd &b, VectorXd &x){
	  const FixedSparseMatrix<double> As(A);
	  MPRGPPlane<double>::solve(As,b,planes,x,tolerance,max_iteration);
	  return true;
	}

  private:
	vector<Vector4d,aligned_allocator<Vector4d> > planes;
  };
  
}//end of namespace

#endif /*_BOXCOLLISIONHANDLING_H_*/
