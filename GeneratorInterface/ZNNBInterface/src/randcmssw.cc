#include "GeneratorInterface/Core/interface/RNDMEngineAccess.h"
extern "C"{
  double rndmcmssw_(double& dummy){
    //std::cout << "Using CMSSW random number generator" << std::endl;
    return gen::getEngineReference().flat();
  }
}  
