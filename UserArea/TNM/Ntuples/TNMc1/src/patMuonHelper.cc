//-----------------------------------------------------------------------------
// Subsystem:   Ntuples
// Package:     TNMc1
// Description: TheNtupleMaker helper class for pat::Muon
// Created:     Tue May  8 18:07:16 2012
// Author:      Sezen Sekmen      
//-----------------------------------------------------------------------------
#include "Ntuples/TNMc1/interface/patMuonHelper.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//-----------------------------------------------------------------------------
using namespace std;
using namespace pat;
//-----------------------------------------------------------------------------
// This constructor is called once per job
MuonHelper::MuonHelper()
  : HelperFor<pat::Muon>() {}
    
MuonHelper::~MuonHelper() {}

// -- Called once per event
void MuonHelper::analyzeEvent()
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
    //cout << "PVs, selected PVs " << PVs->size() << " " << selPVs.size() << endl;
    _PVexists = true;
    _PVposition = selPVs[0]->position();
  }
  

}

// -- Called once per object
//void MuonHelper::analyzeObject()
//{
//
//}

// -- User access methods
//double MuonHelper::someMethod()  const
//{
//  return  //-- some-value --
//}

// dxy wrt primary vertex
// pat::Muon::innerTrack()->dxy(vertex->position()) 
double MuonHelper::dxywrtPV()  const
{
  if (!object->innerTrack().isNull() && _PVexists)
    return object->innerTrack()->dxy(_PVposition); 
  else
    return -9999;
}
// dz wrt primary vertex
// pat::Muon::innerTrack()->dz(vertex->position())
double MuonHelper::dzwrtPV()  const
{
  if (!object->innerTrack().isNull() && _PVexists)
    return object->innerTrack()->dz(_PVposition); 
  else
    return -9999;
}
