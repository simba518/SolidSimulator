#include <Timer.h>
#include <FullStVKSimulator.h>
using namespace UTILITY;
using namespace SIMULATOR;

// a simple example demonstrate the usage of the solid simulator.
int main(int argc, char *argv[]){

  // open init json file
  assert(argc >= 2);
  const string ini_file = argv[1];
  JsonFilePaser jsonf;
  bool succ = jsonf.open(ini_file);
  assert(succ);

  pTetMesh tet_mesh = pTetMesh(new TetMesh());
  { // init tetrahedron mesh.
	string vol_file;
	succ = jsonf.readFilePath("vol_file",vol_file);
	assert(succ);
	succ = tet_mesh->load(vol_file);
	assert(succ);

	string mtl_file;
	jsonf.readFilePath("elastic_mtl",mtl_file);
	succ = tet_mesh->loadElasticMtl(mtl_file);
	assert(succ);
  }

  pSimulator simulator = pSimulator(new FullStVKSimulator());
  { // init simulator
	succ = simulator->init(ini_file);
	assert(succ);
	simulator->setVolMesh(tet_mesh);
	simulator->precompute();
  }

  { // set gravity
	VectorXd gravity(tet_mesh->nodes().size()*3);
	gravity.setZero();
	for (int i = 1; i < gravity.size(); i+=3)
	  gravity[i] = -0.0098f*0.5f;
	simulator->setExtForce(gravity);
  }

  { // simulation for 200 steps, record displacements
	vector<VectorXd> record_u;

	const double plane_y = -0.2f;
	Vector3d plane_normal; 
	plane_normal << 0,1,0;
	const double ks = 0.9f;
	const double kd = 0.9f;

	VectorXd rest;
	tet_mesh->nodes(rest);

	Timer timer;
	for (int i = 0; i < 10000; ++i){
	  
	  VectorXd &u = simulator->getFullDisp();
	  VectorXd &v = simulator->getFullVelocity();
	  const VectorXd x = u+rest;
	  for (int j = 0; j < x.size()/3; ++j){
		const int cy = j*3+1;
		if (x[cy] < plane_y){
		  u[cy] = plane_y-rest[cy];
		  const Vector3d vi = v.segment<3>(j*3);
		  const Vector3d v_v = vi.dot(plane_normal)*plane_normal;
		  const Vector3d v_h = vi-v_v;
		  v.segment<3>(j*3) = ks*v_h-kd*v_v;
		}
	  }

	  cout << "step " << i << endl;
	  for (int i = 0; i < 5; ++i){
		simulator->forward();
	  }

	  record_u.push_back(simulator->getFullDisp());
	}
	timer.stop("total simulation time for 200 steps.");
	
	// save as vtk files.
	succ = tet_mesh->writeVTK("./tempt/hello_dino", record_u);
	assert(succ);
  }
  
  return 0;
}
