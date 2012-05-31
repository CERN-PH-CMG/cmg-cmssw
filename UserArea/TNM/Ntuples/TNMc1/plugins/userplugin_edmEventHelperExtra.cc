// ----------------------------------------------------------------------------
// Created: Thu May 31 10:11:50 2012 by mkhelper.py
// Author:  Sezen Sekmen      
// ----------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/UserBuffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
#include "Ntuples/TNMc1/interface/edmEventHelperExtra.h"
typedef UserBuffer<edm::Event, edm::EventHelperExtra, SINGLETON>
edmEventHelperExtra_t;
DEFINE_EDM_PLUGIN(BufferFactory, edmEventHelperExtra_t,
                  "edmEventHelperExtra");
