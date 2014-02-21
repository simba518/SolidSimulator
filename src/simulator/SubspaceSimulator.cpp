#include "SubspaceSimulator.h" 
using namespace SIMULATOR;

SubspaceSimulator::SubspaceSimulator(){

  // stvkModel = pReducedElasticModel(new DirectReductionElasticModel());
  stvkModel = pReducedElasticModel(new CubaturedElasticModel());
  simulator = pReducedSimulator(new ReducedImpLogConSimulator(stvkModel));
}

bool SubspaceSimulator::forward(){

  bool succ = simulator->forward();
  succ &= stvkModel->computeFullDisp(simulator->getQ(),full_disp);
  return succ;
}
