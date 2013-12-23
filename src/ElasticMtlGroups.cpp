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

bool ElasticMtlGroups::save(const string filename)const{
	  
  ofstream out(filename.c_str());
  if (!out.is_open()){
	ERROR_LOG("failed to open file for saving: " << filename);
	return false;
  }
  
  const vector<set<int> > &groups = _tetGroups.getGroup();
  assert_eq(groups.size(),_rho.size());
  assert_eq(groups.size(),_E.size());
  assert_eq(groups.size(),_v.size());

  out << "*groups " << groups.size() << "\n\n";
  for (int i = 0; i < groups.size(); ++i){
	out << "*MATERIAL, NAME="<< "GROUP_"<< i << endl;
	out << "*ELASTIC\n" << _E[i] << ", "<< _v[i] << endl;
	out << "*DENSITY " << _rho[i] << endl;
	out << "*num_of_tets " << groups[i].size() << "\n\n";
  }

  for (int i = 0; i < groups.size(); ++i){
	out<< endl << "*ELSET, ELSET=GROUP_"<< i << endl;
	int p = 0;
	BOOST_FOREACH(const int tet_i, groups[i]){
	  out << tet_i;
	  p ++;
	  if (p != groups[i].size())
		out << ", ";
	}
  }

  return true;
}

bool ElasticMtlGroups::load(const string filename){
  
  ifstream in(filename.c_str());
  if (!in.is_open()){
	ERROR_LOG("failed to open file " << filename);
	return false;
  }

  string tempt;
  int groups_num = 0;
  in >> tempt >> groups_num;
  assert_gt(groups_num,0);

  vector<int> num_tets(groups_num);
  _rho.resize(groups_num);
  _E.resize(groups_num);
  _v.resize(groups_num);

  for (int i = 0; i < groups_num; ++i){
	in>>tempt>>tempt>>tempt>>_E[i]>>tempt>>_v[i];
	in>>tempt>>_rho[i]>>tempt>>num_tets[i];
	cout << _E[i] << "," << _v[i]<< endl;
  }

  _tetGroups.clear();
  for (int i = 0; i < num_tets.size(); ++i){
	
	assert_ge(num_tets.size(),0);
	vector<int> tets(num_tets[i]);
	in >> tempt >> tempt;
	cout << tempt << endl;
    for (int j = 0;  j < num_tets[i]; ++j){
	  in >> tets[j];
	  if(j != num_tets[i]-1){
	  	in >> tempt;
	  }
	}
	_tetGroups.addGroup(tets);
  }

  cout << "total nodes in groups: "<< _tetGroups.numNodes() << endl;

  return true;
}

void ElasticMtlGroups::setMaterial(ElasticMaterial<double> &mtl)const{

  const vector<set<int> > &groups = _tetGroups.getGroup();
  assert_eq(_rho.size(),groups.size());
  assert_eq(_E.size(),groups.size());
  assert_eq(_v.size(),groups.size());

  mtl.reset(_elementsNum,DEFULT_DENSITY,DEFULT_YOUNG,DEFULT_POSSION);
  for (int i = 0; i < groups.size(); ++i){
	BOOST_FOREACH(const int tet_i, groups[i]){
	  assert_in(tet_i, 0, _elementsNum-1);
	  const Matrix<double,2,1> G_L = ElasticMaterial<double>::toLameConstant(_E[i],_v[i]);
	  mtl._rho[tet_i] = _rho[i];
	  mtl._G[tet_i] = G_L[0];
	  mtl._lambda[tet_i] = G_L[1];
	}
  }	  
}
