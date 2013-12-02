#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

#include <boost/shared_ptr.hpp>

namespace SIMULATOR{
  
  class Simulator{
	
  public:
	Simulator(){}
	bool simulate();
	
  protected:
	
  private:
	
  };
  
  typedef boost::shared_ptr<Simulator> pSimulator;
  
}//end of namespace

#endif /*_SIMULATOR_H_*/
