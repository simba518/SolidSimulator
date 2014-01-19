#include <boost/filesystem.hpp> 
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

void ElasticMtlGroups::reset(const pTetMesh tetmesh,const double rho,
							 const double E,const double v){

  assert(tetmesh);
  assert_gt(rho,0.0f);
  assert_gt(E,0.0f);
  assert_in(v,0.0f,0.5f);
  _rho.clear();
  _E.clear();
  _v.clear();
  _rho.push_back(rho);
  _E.push_back(E);
  _v.push_back(v);

  this->tetmesh = tetmesh;
  _elementsNum = tetmesh->tets().size();
  _tetGroups.clear();
  vector<int> g(_elementsNum);
  for (size_t i = 0; i < g.size(); ++i)
	g[i] = i;
  _tetGroups.addGroup(g);
}

void ElasticMtlGroups::getMtlColors(vector<double> &colors)const{

  double maxR, minR,  maxE, minE,  maxV, minV;
  MaxMin(_rho, maxR, minR);
  MaxMin(_E, maxE, minE);
  MaxMin(_v, maxV, minV);
  colors.resize(numGroups()*3);
  for (size_t i = 0; i < _rho.size(); ++i){
	colors[i*3+0] = (0==maxE-minE)?0:(_E[i]-minE)/(maxE-minE)+0.1f;
	colors[i*3+1] = (0==maxR-minR)?0:(_rho[i]-minR)/(maxR-minR)+0.1f;
	colors[i*3+2] = 0.1f;
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

// the format used by Barbic's volumetric mesh.
bool ElasticMtlGroups::saveAsHinp(const string filename)const{

  // save material
  ofstream out(filename.c_str());
  if (!out.is_open()){
	ERROR_LOG("failed to open file for saving: " << filename);
	return false;
  }
  
  const vector<set<int> > &groups = _tetGroups.getGroup();
  assert_eq(groups.size(),_rho.size());
  assert_eq(groups.size(),_E.size());
  assert_eq(groups.size(),_v.size());

  out << "*HEADING " << "\n";
  out << "Model: tet mesh\n";
  out << "*INCLUDE, INPUT=mesh.abq\n";
  if (groups.size() > 1)
	out << "*INCLUDE, INPUT=mesh.elset\n";
  for (int i = 0; i < groups.size(); ++i){
	out << "*MATERIAL, NAME="<< "GROUP_"<< i << endl;
	out << "*ELASTIC\n" << _E[i] << ", "<< _v[i] << endl;
	out << "*DENSITY\n" << _rho[i] << endl;
  }
  if (groups.size() > 1){
	for (int i = 0; i < groups.size(); ++i)
	  out << "*SOLID SECTION,MATERIAL=GROUP_"<<i<<",ELSET=GROUP_"<<i<<endl;
  }else{
	out << "*SOLID SECTION,MATERIAL=GROUP_0,ELSET=EALL"<<endl;
  }

  out.close();

  // save tetrahedron groups
  boost::filesystem::path fpath(filename);
  const string group_file = fpath.parent_path().string()+"//mesh.elset";
  ofstream out_group(group_file.c_str());
  if (!out_group.is_open()){
	ERROR_LOG("failed to open file for saving: " << group_file);
	return false;
  }

  for (int i = 0; i < groups.size(); ++i){
	out_group << "*ELSET, ELSET=GROUP_"<< i << endl;
	int p = 0;
	BOOST_FOREACH(const int tet_i, groups[i]){
	  out_group << tet_i;
	  p ++;
	  if (p != groups[i].size()) out_group << ", ";
	}
	out_group << endl;
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
  assert_ge(groups_num,0);

  if (0 == groups_num ){

	_rho.resize(1);
	_E.resize(1);
	_v.resize(1);
	in>>tempt>>tempt>>tempt>>_E[0]>>tempt>>_v[0];
	in>>tempt>>_rho[0];

	INFO_LOG("E,v,rho: "<<_E[0]<<","<<_v[0]<<","<<_rho[0]);

	assert_gt(_elementsNum,0);
	vector<int> tets(_elementsNum);
	for (int i = 0; i < _elementsNum; ++i)
	  tets[i] = i;
	_tetGroups.addGroup(tets);

  }else{
	vector<int> num_tets(groups_num);
	_rho.resize(groups_num);
	_E.resize(groups_num);
	_v.resize(groups_num);

	for (int i = 0; i < groups_num; ++i){
	  in>>tempt>>tempt>>tempt>>_E[i]>>tempt>>_v[i];
	  in>>tempt>>_rho[i]>>tempt>>num_tets[i];
	  INFO_LOG("E,v,rho: "<<_E[i]<<","<<_v[i]<<","<<_rho[i]);
	}
	_tetGroups.clear();
	if (num_tets.size() >= 2){
	  for (int i = 0; i < num_tets.size(); ++i){
	
		assert_ge(num_tets.size(),0);
		vector<int> tets(num_tets[i]);
		in >> tempt >> tempt;
		for (int j = 0;  j < num_tets[i]; ++j){
		  in >> tets[j];
		  if(j != num_tets[i]-1){
			in >> tempt;
		  }
		}
		_tetGroups.addGroup(tets);
	  }
	}
  }

  INFO_LOG("total tets that are set materail from file: "<< _tetGroups.numNodes());
  interpolateMaterials();
  return in.good();
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

void ElasticMtlGroups::interpolateMaterials(){

  TRACE_FUN();
  assert(tetmesh);
  assert_eq(tetmesh->tets().size(),_elementsNum);
  
  const vector<set<int> > &groups = _tetGroups.getGroup();
  assert_ge(groups.size(),1);
  if (1 == groups.size()){
	if (_tetGroups.numNodes() != _elementsNum){
	  reset(tetmesh, _rho[0], _E[0], _v[0]);
	}
	return ;
  }
  
  vector<int> tets;
  for (int i = 0; i < _elementsNum; ++i){

    if (_tetGroups.contain(i))
	  continue;

	int g1=0, g2=0, t1=0, t2=0;
	double dis1 = +2e19, dis2 = +2e19;
	const Vector3d p = tetmesh->getTet(i).center();

	for (int g = 0; g < groups.size(); ++g){
	  
	  int t3=0, dis3=+2e19;
	  BOOST_FOREACH(const int &t, groups[g]){
		const double d = (p-tetmesh->getTet(t).center()).norm();
		if (d < dis3){
		  dis3 = d;
		  t3 = t;
		}
	  }
	  if (dis3 < dis1){
		dis2 = dis1;
		dis1 = dis3;
		t2 = t1;
		t1 = t3;
		g2 = g1;
		g1 = g;
	  }else if (dis3 < dis2){
		dis2 = dis3;
		t2 = t3;
		g2 = g;
	  }
	}
	assert_ne(g1,g2);
	assert_ne(t1,t2);
	assert_le(dis1,dis2);

	assert_in(g1,0,_E.size()-1);
	assert_in(g2,0,_E.size()-1);
	const Vector3d c1 = tetmesh->getTet(t1).center();
	const Vector3d c2 = tetmesh->getTet(t2).center();
	const Vector3d c = tetmesh->getTet(i).center();
	const double d = (c-c1).norm()+(c-c2).norm();
	assert_gt(d,0);
	const double x = (c-c1).norm()/d;
	assert_in(x,0,1);

	_E.push_back( (_E[g2]-_E[g1])*x + _E[g1]);
	_v.push_back( (_v[g2]-_v[g1])*x + _v[g1]);
	_rho.push_back( (_rho[g2]-_rho[g1])*x + _rho[g1] );

	tets.push_back(i);
  }

  for (int i = 0; i < tets.size(); ++i){
	vector<int> t;
	t.push_back(tets[i]);
	_tetGroups.addGroup(t);
  }
}
