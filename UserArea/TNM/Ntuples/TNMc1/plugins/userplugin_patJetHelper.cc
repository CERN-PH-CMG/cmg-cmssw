// ----------------------------------------------------------------------------
// Created: Fri Sep 28 10:54:23 2012 by mkhelper.py
// Author:  Sezen Sekmen      
// ----------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/UserBuffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
#include "Ntuples/TNMc1/interface/patJetHelper.h"
typedef UserBuffer<pat::Jet, pat::JetHelper, COLLECTION>
patJetHelper_t;
DEFINE_EDM_PLUGIN(BufferFactory, patJetHelper_t,
                  "patJetHelper");
