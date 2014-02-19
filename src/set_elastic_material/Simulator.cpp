#include <SparseMatrixTools.h>
#include <ElasticForceTetFullStVK.h>
#include <SparseGenEigenSolver.h>
#include <MassMatrix.h>
#include <SparseMatrixTools.h>
#include "Simulator.h"

using namespace UTILITY;
using namespace SIMULATOR;
using namespace EIGEN3EXT;

bool Simulator::loadSetting(const string filename){
  
  JsonFilePaser jsonf;
  if (!jsonf.open(filename)){
	return false;
  }

  bool succ = true;
  succ &= jsonf.read("h",_h);
  succ &= jsonf.read("alpha_k",_ak);
  succ &= jsonf.read("alpha_m",_am);
  succ &= jsonf.read("rw",_eigenNum);
  
  return succ;
}

bool Simulator::precompute(){

  if(!_tetMesh)
	return false;

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

bool Simulator::simulate(){

  assert_eq(_W.rows(),_fext.size());
  _F_reduced = _W.transpose()*_fext;
  forward();
  assert_eq(_W.cols(),_z.size());
  _u = _W*_z;
  return true;
}

void Simulator::forward(){
  
  assert_eq(_F_reduced.size(),_z.size());
  assert_eq(_z.size(),_v.size());
  
  // implicit integration
  const int r = _z.size();
  for (int i = 0; i < r; ++i){
	const double d = _am+_ak*_lambda[i];
	_v[i] = (_v[i]+_h*_F_reduced[i]-_h*_lambda[i]*_z[i])/(1+_h*d+_lambda[i]*_h*_h);
    _z[i] = _z[i]+_h*_v[i];
  }
}

void Simulator::print()const{
  
  cout << "num of fixed nodes: " << _fixedNodes.size() << endl;
  cout << "num of eigen vales: " << _eigenNum << endl;;
  cout << "h: " << _h << endl;
  cout << "ak: " << _ak << endl;
  cout << "am: " << _am << endl;
}
