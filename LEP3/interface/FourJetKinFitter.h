#ifndef __CMGTools_LEP3_FourJetKinFitter__
#define __CMGTools_LEP3_FourJetKinFitter__

#include "PhysicsTools/KinFitter/interface/TKinFitter.h"

class FourJetKinFitter : public TKinFitter {

 public: 
  FourJetKinFitter(const char* name, const char* title); 

  ClassDef(FourJetKinFitter, 1);
};

#endif 
