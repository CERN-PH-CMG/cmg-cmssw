#include "CMGTools/LEP3/interface/FourJetKinFitter.h"

#include <TString.h>
#include <iostream>

using namespace std;

FourJetKinFitter::FourJetKinFitter(const char* name, 
				   const char* title) : 
  TKinFitter( TString(name), TString(title) ) {
  cout<<"FourJetKinFitter: "<<name<<", "<<title<<endl;
}

