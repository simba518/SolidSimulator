#include "ElasticMtlGroups.h"
using namespace SIMULATOR;

#define TOTAL_COLOR_TYPES 10
const double ElasticGroupColors[TOTAL_COLOR_TYPES][3] = {

  0.5,  0.0,  0.0, 
  0.0,  0.5,  0.0,
  0.0,  0.0,  0.5, 

  0.5,  0.5,  0.0, 
  0.5,  0.0,  0.5,
  0.0,  0.5,  0.5,

  0.5,  0.5,  1.0, 
  0.5,  1.0,  0.5,
  1.0,  0.5,  0.5,

  1.0,  1.0,  1.0
};

void ElasticMtlGroups::reset(const int totalElements,const double rho,const double E,const double v){

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

void ElasticMtlGroups::getMtlColors(vector<double> &colors)const{
	  
  colors.resize(numGroups()*3);
  double min,max;
  MaxMin(_rho,min,max);
  for (size_t i = 0; i < _rho.size(); ++i){
	if (max-min > 0.0f){
	  colors[i*3+0] = (_rho[i]-min)/(max-min);
	}else{
	  colors[i*3+0] = 0.0f;
	}
  }

  MaxMin(_E,min,max);
  for (size_t i = 0; i < _E.size(); ++i){
	if (max-min > 0.0f){
	  colors[i*3+1] = (_E[i]-min)/(max-min);
	}else{
	  colors[i*3+1] = 0.0f;
	}
  }

  MaxMin(_v,min,max);
  for (size_t i = 0; i < _v.size(); ++i){
	if (max-min > 0.0f){
	  colors[i*3+2] = (_v[i]-min)/(max-min);
	}else{
	  colors[i*3+2] = 0.0f;
	}
  }

}

void ElasticMtlGroups::getGroupColor(const int gid,double color[3])const{

  assert_in(gid,0,numGroups()-1);
  const int n = numGroups();
  color[0] = ElasticGroupColors[gid%TOTAL_COLOR_TYPES][0];
  color[1] = ElasticGroupColors[gid%TOTAL_COLOR_TYPES][1];
  color[2] = ElasticGroupColors[gid%TOTAL_COLOR_TYPES][2];
}
