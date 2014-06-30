#include "BoxCollisionHandling.h"
using namespace SIMULATOR;

int main(int argc, char *argv[]){

  // initialize
  assert_ge(argc,2);
  const string ini_file = argv[1];
  pTetMesh tet_mesh = pTetMesh(new TetMesh());
  { // load tet mesh
	JsonFilePaser jsonf;
	bool succ = jsonf.open(ini_file); assert(succ);
	string tet_file;
	succ = jsonf.readFilePath("vol_file",tet_file); assert(succ);
	succ = tet_mesh->load(tet_file); assert(succ);
  }

  pFullStVKSimModel def_model = pFullStVKSimModel(new FullStVKSimModel());
  def_model->setTetMesh(tet_mesh);

  BoxCollisionHandling collision_sim(def_model);
  bool succ = collision_sim.init(ini_file); assert(succ);
  succ = collision_sim.prepare(); assert(succ);
  
  {// set bounding box
	collision_sim.setYBBox(0,std::numeric_limits<double>::max());
  }

  {// set external forces
	collision_sim.setExtForceForAllNodes(0.0f,-1.0f,0.0f);
  }

  {// simulation, save results
	vector<VectorXd> record_u;
	for (int i = 0; i < 1000; ++i){
	  cout << "step: " << i << endl;
	  collision_sim.forward();
	  record_u.push_back(collision_sim.getU());
	}
	bool succ = tet_mesh->writeVTK("./tempt/BB_collision", record_u);
	assert(succ);
  }

  return 0;
}
