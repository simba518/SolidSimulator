#include <JsonFilePaser.h>
#include <SparseMatrixTools.h>
#include <ElasticForceTetFullStVK.h>
#include <SparseGenEigenSolver.h>
#include <MassMatrix.h>
#include <SparseMatrixTools.h>
#include "MASimulator.h"

using namespace UTILITY;
using namespace EIGEN3EXT;
using namespace SIMULATOR;

bool MASimulator::init(const string filename){

  TRACE_FUN();
  JsonFilePaser jsonf;
  if (!jsonf.open(filename)){
	return false;
  }

  bool succ = true;
  succ &= jsonf.read("h",_h);
  succ &= jsonf.read("alpha_k",_ak);
  succ &= jsonf.read("alpha_m",_am);
  succ &= jsonf.read("rw",_eigenNum);

  vector<int> fixed_nodes;
  jsonf.readVecFile("fixed_nodes",fixed_nodes,TEXT);
  setConNodes(fixed_nodes);

  jsonf.readVecFile("eigenvalues",_lambda);
  jsonf.readMatFile("eigenvectors",_W);
  
  if (_eigenNum > 0 && _eigenNum < _lambda.size()){

	const MatrixXd tv = _W;
	_W = tv.leftCols(_eigenNum);
	const VectorXd tl = _lambda;
	_lambda = tl.head(_eigenNum);
  }
  
  return succ;
}

bool MASimulator::precompute(){
  
  TRACE_FUN();
  if(!_tetMesh)
	return false;

  if (_W.cols() == _lambda.size() && _W.rows() == _tetMesh->nodes().size()*3){
	INFO_LOG("use W and lambda read from file");
	INFO_LOG("number of eigen vectors: " << _lambda.size());
	reset();
	return true;
  }

  /// compute full K, M
  ElasticForceTetFullStVK ela(_tetMesh);
  bool succ = ela.prepare();
  VectorXd x0;
  _tetMesh->nodes(x0);
  SparseMatrix<double> K = ela.K(x0)*(-1.0f);

  MassMatrix mass;
  DiagonalMatrix<double,-1> M;
  mass.compute(M,*_tetMesh);

  /// remove fixed nodes
  INFO_LOG("num of fixed nodes: " << _fixedNodes.size());
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
  succ &= EigenSparseGenEigenSolver::solve(Klower,diagM,_W,_lambda,_eigenNum);
#endif

  // _W.col(0) += VectorXd::Random(_W.rows())*_W.col(0).norm()*0.005f;
  _W = P.transpose()*_W;

  reset();
  return succ;
}

bool MASimulator::forward(){

  if (_W.rows() != _fext.size()){
	_fext.resize(_W.rows());
	_fext.setZero();
  }
  _F_reduced = _W.transpose()*_fext;
  solve();
  assert_eq(_W.cols(),_z.size());
  _u = _W*_z;
  return true;
}

void MASimulator::solve(){
  
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
