// ----------------------------------------------------------------------------
// Created: Mon Oct  8 18:42:59 2012 by mkhelper.py
// Author:  Sezen Sekmen      
// ----------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/UserBuffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
#include "Ntuples/TNMc1/interface/LHEEventProductHelper.h"
typedef UserBuffer<LHEEventProduct, LHEEventProductHelper, SINGLETON>
LHEEventProductHelper_t;
DEFINE_EDM_PLUGIN(BufferFactory, LHEEventProductHelper_t,
                  "LHEEventProductHelper");
