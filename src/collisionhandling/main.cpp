#include "BoxCollisionHandling.h"
using namespace SIMULATOR;

#include <float.h>
float ScalarUtil<float>::scalar_max=FLT_MAX;
float ScalarUtil<float>::scalar_eps=1E-5f;

double ScalarUtil<double>::scalar_max=DBL_MAX;
double ScalarUtil<double>::scalar_eps=1E-9;

void simulateAndSave(const string ini_file){

  JsonFilePaser jsonf;
  bool succ = jsonf.open(ini_file); assert(succ);
  string save_to;
  int num_frames;
  succ = jsonf.readFilePath("save_to",save_to,false); assert(succ);
  succ = jsonf.read("num_frames",num_frames,200); assert_ge(num_frames,0);

  pTetMesh tet_mesh = pTetMesh(new TetMesh());
  { // load tet mesh
	string tet_file;
	succ = jsonf.readFilePath("vol_file",tet_file); assert(succ);
	succ = tet_mesh->load(tet_file); assert(succ);

	string mtl_file;
	succ = jsonf.readFilePath("elastic_mtl",mtl_file); assert(succ);
	succ = tet_mesh->loadElasticMtl(mtl_file); assert(succ);
  }

  pFullStVKSimModel def_model = pFullStVKSimModel(new FullStVKSimModel());
  def_model->setTetMesh(tet_mesh);

  BoxCollisionHandling collision_sim(def_model);
  succ = collision_sim.prepare(); assert(succ);
  succ = collision_sim.init(ini_file); assert(succ);
  
  {// simulation, save results
	vector<VectorXd> record_u;
	for (int i = 0; i < num_frames; ++i){
	  cout << "step: " << i << endl;
	  collision_sim.forward();
	  record_u.push_back(collision_sim.getU());
	}
	bool succ = tet_mesh->writeVTK(save_to, record_u);
	assert(succ);
  }
}

int main(int argc, char *argv[]){

  // initialize
  assert_ge(argc,2);
  simulateAndSave(argv[1]);
  return 0;
}
