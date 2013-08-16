//-----------------------------------------------------------------------------
// Subsystem:   Ntuples
// Package:     TNMc1
// Description: TheNtupleMaker helper class for pat::Electron
// Created:     Wed May  9 15:36:04 2012
// Author:      Sezen Sekmen      
//-----------------------------------------------------------------------------
#include "Ntuples/TNMc1/interface/patElectronHelper.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//-----------------------------------------------------------------------------
using namespace std;
using namespace pat;
//-----------------------------------------------------------------------------
// This constructor is called once per job
ElectronHelper::ElectronHelper()
  : HelperFor<pat::Electron>() {}
    
ElectronHelper::~ElectronHelper() {}

// -- Called once per event
void ElectronHelper::analyzeEvent()
{

  // Get the primary vertex:
  edm::Handle<std::vector<reco::Vertex> > PVs;
  event->getByLabel("offlinePrimaryVertices", PVs);

  _PVexists = false;

  std::vector<const reco::Vertex*> selPVs;             

  bool good1stPV = false;
  for (unsigned int i=0; i<PVs->size(); i++) {
    const reco::Vertex* v = &((*PVs)[i]);
    if (v->isFake() ) continue;
    if (!(fabs(v->z()) < 24) ) continue;
    if (!(v->position().Rho() < 2) ) continue;
    if (!(v->ndof() > 4) ) continue;
    if (i==0) good1stPV = true;
    selPVs.push_back(v);
  }

  if (selPVs.size() > 0 && good1stPV) {
    //cout << "PVs, selected PVs: " << PVs->size() << " " << selPVs.size() << endl;
    _PVexists = true;
    _PVposition = selPVs[0]->position();
  }

}

// -- Called once per object
//void ElectronHelper::analyzeObject()
//{
//
//}

// -- User access methods
//double ElectronHelper::someMethod()  const
//{
//  return  //-- some-value --
//}
// dxy wrt primary vertex
// pat::Electron::gsfTrack()->dxy(vertex->position())
double ElectronHelper::dxywrtPV()  const
{
  if (!object->gsfTrack().isNull() && _PVexists)
    return object->gsfTrack()->dxy(_PVposition);
  else
    return -9999;
}
// dz wrt primary vertex
// pat::Electron::gsfTrack()->dz(vertex->position())
double ElectronHelper::dzwrtPV()  const
{
  if (!object->gsfTrack().isNull() && _PVexists)
    return object->gsfTrack()->dz(_PVposition);
  else
    return -9999;
}

