#ifndef _ELASTICMTLGROUPS_H_
#define _ELASTICMTLGROUPS_H_

#include <boost/shared_ptr.hpp>
#include <SelectionGroup.h>
#include <TetMesh.h>
using namespace UTILITY;

#define DEFULT_DENSITY 1000.0f
#define DEFULT_YOUNG 2e6
#define DEFULT_POSSION 0.45f

namespace SIMULATOR{
  
  class ElasticMtlGroups{
	
  public:
	ElasticMtlGroups(){
	  _elementsNum = 0;
	}
	void reset(const int totalElements,
			   const double rho=DEFULT_DENSITY,
			   const double E=DEFULT_YOUNG,
			   const double v=DEFULT_POSSION){

	  assert_ge(totalElements,0);
	  assert_gt(rho,0.0f);
	  assert_gt(E,0.0f);
	  assert_in(v,0.0f,0.5f);
	  _rho.clear();
	  _E.clear();
	  _v.clear();
	  _rho.push_back(rho);
	  _E.push_back(E);
	  _v.push_back(v);

	  _tetGroups.clear();
	  vector<int> g(totalElements);
	  for (size_t i = 0; i < g.size(); ++i)
		g[i] = i;
	  _tetGroups.addGroup(g);
	  _elementsNum = totalElements;
	}
	const vector<double> &density()const{ return _rho;}
	const vector<double> &young()const{ return _E;}
	const vector<double> &possion()const{ return _v;}
	const vector<set<int> > &groups()const{ return _tetGroups.getGroup();}
	void addGroup(const vector<int> &g,const int totalElements){
	  if(totalElements != _elementsNum)
		reset(totalElements);
	  if(g.size() > 0){
		if(_tetGroups.addGroup(g).size() > 0)
		  resetMtl();
	  }
	}
	void removeGroup(const vector<int> &g,const int totalElements){
	  if(totalElements != _elementsNum)
		reset(totalElements);
	  if(g.size() > 0){
		if(_tetGroups.removeGroup(g).size() > 0)
		  resetMtl();
	  }
	}
	void getColors(vector<double> &colors)const{
	  
	  colors.resize(numGroups()*3);
	  double min,max;
	  MaxMin(_rho,min,max);
	  for (size_t i = 0; i < _rho.size(); ++i){
		if (max-min > 0.0f){
		  colors[i*3+0] = (_rho[i]-min)/(max-min);
		}else{
		  colors[i*3+0] = 1.0f;
		}
	  }

	  MaxMin(_E,min,max);
	  for (size_t i = 0; i < _E.size(); ++i){
		if (max-min > 0.0f){
		  colors[i*3+1] = (_E[i]-min)/(max-min);
		}else{
		  colors[i*3+1] = 1.0f;
		}
	  }

	  MaxMin(_v,min,max);
	  for (size_t i = 0; i < _v.size(); ++i){
		if (max-min > 0.0f){
		  colors[i*3+2] = (_v[i]-min)/(max-min);
		}else{
		  colors[i*3+2] = 1.0f;
		}
	  }

	}
	int numGroups()const{
	  return _rho.size();
	}
	bool save(const string filename)const{
	  /// @todo
	  return false;
	}
	void setMaterial(ElasticMaterial<double> &mtl)const{
	  mtl.reset(_elementsNum);
	  /// @todo
	}
	
  protected:
	void MaxMin(const vector<double> &v, double &max,double &min)const{
	  max = min = (v.size()>0?v[0]:0.0f);
	  for (size_t i = 1; i < v.size(); ++i){
		if(v[i] > max) max = v[i];
		if(v[i] < min) min = v[i];
	  }
	}
	void resetMtl(){
	  _rho = vector<double>(_tetGroups.numGroup(),DEFULT_DENSITY);
	  _E = vector<double>(_tetGroups.numGroup(),DEFULT_YOUNG);
	  _v = vector<double>(_tetGroups.numGroup(),DEFULT_POSSION);
	}
	
  private:
	int _elementsNum;
	vector<double> _rho; // Density
	vector<double> _E; // Young's modulus.
	vector<double> _v; // Poisson's ratio.
	SelectionGroup _tetGroups;
  };
  
  typedef boost::shared_ptr<ElasticMtlGroups> pElasticMtlGroups;
  
}//end of namespace

#endif /*_ELASTICMTLGROUPS_H_*/
