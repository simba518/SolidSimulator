#include <Timer.h>
#include <FullStVKSimulator.h>
#include <SubspaceSimulator.h>
using namespace UTILITY;
using namespace SIMULATOR;

// a simple example demonstrate the usage of the solid simulator.
int main(int argc, char *argv[]){

  // open init json file
  // const string ini_file = "./data/dino/simu_cubature.ini";
  const string ini_file = "./data/dino/simu_full.ini";
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

  // pReducedElasticModel elas_m = pReducedElasticModel(new CubaturedElasticModel());
  // pSimulator simulator = pSimulator(new SubspaceSimulator(elas_m,string("cubature")));
  pSimulator simulator = pSimulator(new FullStVKSimulator());
  { // init simulator
	succ = simulator->init(ini_file);
	assert(succ);
	simulator->setVolMesh(tet_mesh);
	simulator->precompute();
  }

  { // load and set fixed nodes
	vector<int> nodes;
	succ = jsonf.readVecFile("fixed_nodes", nodes,UTILITY::TEXT);
	cout << "num of fixed nodes: " << nodes.size() << endl;
	simulator->setConNodes(nodes);
	VectorXd uc(nodes.size()*3);
	uc.setZero();	
	simulator->setUc(uc);
  }

  // simulation for 200 steps, record displacements
  vector<VectorXd> record_u;
  Timer timer;
  { // set external forces, then simulate 50 steps
	const int nodeId = 333;
	const double f[3] = {204.175, -210.993, 0};
	simulator->setExtForceOfNode(nodeId, f);

	for (int i = 0; i < 50; ++i){
	  cout << "step " << i << endl;
	  simulator->forward();
	  record_u.push_back(simulator->getFullDisp());
	}

	// remove forces, then simulate 150 steps
	simulator->clearExtForce();
	for (int i = 50; i < 200; ++i){
	  cout << "step " << i << endl;
	  simulator->forward();
	  record_u.push_back(simulator->getFullDisp());
	}
  }
  timer.stop("total simulation time for 200 steps.");

  { // save as vtk files.
	succ = tet_mesh->writeVTK("./tempt/hello_dino", record_u);
	assert(succ);
  }
          
  return 0;
}
