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
			   const double v=DEFULT_POSSION);
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
	void getMtlColors(vector<double> &colors)const;
	void getGroupColor(const int gid,double color[3])const;
	int numGroups()const{
	  return _tetGroups.numGroup();
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
