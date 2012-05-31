//-----------------------------------------------------------------------------
// Subsystem:   Ntuples
// Package:     TNMc1
// Description: TheNtupleMaker helper class for pat::Jet
// Created:     Tue May  8 17:05:51 2012
// Author:      Sezen Sekmen      
//-----------------------------------------------------------------------------
#include "Ntuples/TNMc1/interface/patJetHelper.h"
//-----------------------------------------------------------------------------
using namespace std;
using namespace pat;
//-----------------------------------------------------------------------------
// This constructor is called once per job
JetHelper::JetHelper()
  : HelperFor<pat::Jet>() {}
    
JetHelper::~JetHelper() {}

// -- Called once per event
//void JetHelper::analyzeEvent()
//{
//
//}

// -- Called once per object
void JetHelper::analyzeObject()
{
  
  // write only jets with pt > 15:

  if (!(object->pt() > 15)) {
    count = 0;
  }

}

// -- User access methods
//double JetHelper::someMethod()  const
//{
//  return  //-- some-value --
//}
