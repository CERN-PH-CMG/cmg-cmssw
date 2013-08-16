// ----------------------------------------------------------------------------
// Created: Fri Sep 28 11:17:12 2012 by mkhelper.py
// Author:  Sezen Sekmen      
// ----------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/UserBuffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
#include "Ntuples/TNMc1/interface/patElectronHelper.h"
typedef UserBuffer<pat::Electron, pat::ElectronHelper, COLLECTION>
patElectronHelper_t;
DEFINE_EDM_PLUGIN(BufferFactory, patElectronHelper_t,
                  "patElectronHelper");
