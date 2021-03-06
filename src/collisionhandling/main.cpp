#include "BoxCollisionHandling.h"
using namespace SIMULATOR;

#include <float.h>
float ScalarUtil<float>::scalar_max=FLT_MAX;
float ScalarUtil<float>::scalar_eps=1E-5f;

double ScalarUtil<double>::scalar_max=DBL_MAX;
double ScalarUtil<double>::scalar_eps=1E-9;

void simulateAndSave(const string ini_file){

  INFO_LOG("init file: "<< ini_file);

  JsonFilePaser jsonf;
  bool succ = jsonf.open(ini_file); assert(succ);
  string save_to;
  int num_frames;
  succ = jsonf.readFilePath("save_to",save_to,false); assert(succ);
  succ = jsonf.read("num_frames",num_frames,200); assert_ge(num_frames,0);
  INFO_LOG("total frames: "<<num_frames);

  pTetMesh tet_mesh = pTetMesh(new TetMesh());
  { // load tet mesh
	string tet_file;
	succ = jsonf.readFilePath("vol_file",tet_file); assert(succ);
	succ = tet_mesh->load(tet_file); assert(succ);
	INFO_LOG("number of nodes: " << tet_mesh->nodes().size());
	INFO_LOG("number of tets: " << tet_mesh->tets().size());
	vector<double> move;
	if (jsonf.read("move_mesh",move)){
	  assert_eq(move.size(),3);
	  VectorXd u(tet_mesh->nodes().size()*3);
	  for (int i = 0; i < u.size()/3; ++i){
		u[i*3+0] = move[0];
		u[i*3+1] = move[1];
		u[i*3+2] = move[2];
	  }
	  tet_mesh->applyDeformation(u);
	}

	string mtl_file;
	succ = jsonf.readFilePath("elastic_mtl",mtl_file); assert(succ);
	succ = tet_mesh->loadElasticMtl(mtl_file); assert(succ);
  }

  pFullStVKSimModel def_model = pFullStVKSimModel(new FullStVKSimModel());
  def_model->setTetMesh(tet_mesh);

  // BoxCollisionHandling collision_sim(def_model);
  PlaneCollisionHandling collision_sim(def_model);
  succ = collision_sim.prepare(); assert(succ);
  succ = collision_sim.init(ini_file); assert(succ);

  // set gravity
  vector<double> gravity;
  succ &= jsonf.read("gravity",gravity);
  assert_eq(gravity.size(),3);
  VectorXd gravity_forces;
  tet_mesh->computeGravity(gravity, gravity_forces);
  collision_sim.setExtForce(gravity_forces);
  
  {// simulation, save results
	vector<VectorXd> record_u;
	
	Timer timer;
	timer.start();
	for (int i = 0; i < num_frames; ++i){
	  INFO_LOG("step: " << i);
	  collision_sim.forward();
	  record_u.push_back(collision_sim.getU());
	}
	timer.stop("total simulation time: ");

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
