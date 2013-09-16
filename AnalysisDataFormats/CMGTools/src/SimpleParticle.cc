# include "AnalysisDataFormats/CMGTools/interface/SimpleParticle.h"

#include <iostream>
#include <cstdlib>

using namespace cmg;

float SimpleParticle::charge() const {
  
  float theCharge = -99;
  switch( std::abs(type()) ) {
  case 211:
  case 11:
  case 13:
    theCharge = 1;
    break;
  case 22:
  case 130:
  case 1: // HF ...
  case 2:
    theCharge = 0;
    break;
  default:
    std::cerr<<"unknown pdgid "<<type()<<std::endl;
    assert(false);
  }

  if( type()<0 )
    theCharge *= -1;

  return theCharge;
}
