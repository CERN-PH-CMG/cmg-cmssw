// ----------------------------------------------------------------------------
// Created: Thu May 31 10:08:13 2012 by mkhelper.py
// Author:  Sezen Sekmen      
// ----------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/UserBuffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
#include "Ntuples/TNMc1/interface/patMuonHelper.h"
typedef UserBuffer<pat::Muon, pat::MuonHelper, COLLECTION>
patMuonHelper_t;
DEFINE_EDM_PLUGIN(BufferFactory, patMuonHelper_t,
                  "patMuonHelper");
