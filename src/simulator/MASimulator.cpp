#include <SparseMatrixTools.h>
#include <ElasticForceTetFullStVK.h>
#include <SparseGenEigenSolver.h>
#include <MassMatrix.h>
#include <SparseMatrixTools.h>
#include <JsonFilePaser.h>
#include "MASimulator.h"

using namespace UTILITY;
using namespace SIMULATOR;
using namespace EIGEN3EXT;

bool MASimulator::init(const string filename){

  TRACE_FUN();

  JsonFilePaser jsonf;
  if (!jsonf.open(filename)){
	return false;
  }

  bool succ = true;
  succ &= jsonf.read("h",_h,0.02);
  succ &= jsonf.read("alpha_k",_ak,0.0);
  succ &= jsonf.read("alpha_m",_am,0.0);
  succ &= jsonf.read("rw",_eigenNum,80);
  
  return succ;
}

bool MASimulator::precompute(){

  TRACE_FUN();
  if(!_tetMesh){
	ERROR_LOG("the tet mesh is null");
	return false;
  }

  /// compute full K, M
  ElasticForceTetFullStVK ela(_tetMesh);
  VectorXd x0;
  _tetMesh->nodes(x0);
  SparseMatrix<double> K = ela.K(x0)*(-1.0f);

  MassMatrix mass;
  DiagonalMatrix<double,-1> M;
  mass.compute(M,*_tetMesh);

  /// remove fixed nodes
  SparseMatrix<double> P;
  EIGEN3EXT::genReshapeMatrix(K.rows(),3,_fixedNodes,P);
  K = P*(K*P.transpose());
  const SparseMatrix<double> diagM = P*(M*P.transpose());

  /// compute W, lambda
  const SparseMatrix<double> Klower = EIGEN3EXT::getLower(K);

  /// @bug the eigen solver(using arpack) can not be compiled in windows now.
#ifdef WIN32
  ERROR_LOG("no arpack solver, can not compute the general eigenvalue problem.");
  const bool succ=false;
  _W = MatrixXd::Identity(Klower.rows(),_eigenNum);
  _lambda = VectorXd::Ones(_eigenNum);
#else
  const bool succ=EigenSparseGenEigenSolver::solve(Klower,diagM,_W,_lambda,_eigenNum);
#endif

  _W = P.transpose()*_W;
  reset();

  // test Wt*M*W
  const MatrixXd WtMW_I = _W.transpose()*M*_W-MatrixXd::Identity(_W.cols(),_W.cols());
  cout << "(WtMW-I).norm(): " << WtMW_I.norm() << endl;
  cout << "eigenvalues: " << _lambda.transpose() << endl;
  cout << "norm(Klower): " << Klower.norm() << endl;
  cout << "norm(M): " << diagM.norm() << endl;

  return succ;
}

bool MASimulator::forward(){

  assert_eq(_W.rows(),_fext.size());
  _F_reduced = _W.transpose()*_fext;
  
  // implicit integration
  assert_eq(_F_reduced.size(),_z.size());
  assert_eq(_z.size(),_v.size());
  const int r = _z.size();
  for (int i = 0; i < r; ++i){
	const double d = _am+_ak*_lambda[i];
	_v[i] = (_v[i]+_h*_F_reduced[i]-_h*_lambda[i]*_z[i])/(1+_h*d+_lambda[i]*_h*_h);
    _z[i] = _z[i]+_h*_v[i];
  }

  assert_eq(_W.cols(),_z.size());
  _u = _W*_z;
  

}
