#ifndef TAUEFFICIENCY_H_
#define TAUEFFICIENCY_H_

#include <iomanip>
#include <TString.h>
#include <memory>
#include <iostream>
#include <vector>
#include <string.h>
#include <TROOT.h>
#include <TSystem.h>
using namespace std;

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"   
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"


#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TText.h>
#include <TString.h>


class TauEfficiency {
public:
  
  TauEfficiency():
    filepath_(""),
    effOrFake_(1)
  {};
  TauEfficiency(TString filepath,Int_t EffOrFake=1):
    filepath_(filepath),
    effOrFake_(EffOrFake)
  {};
  ~TauEfficiency(){};
  
  void processEvents(Int_t nfiles=1);
  
private:
  
  TString filepath_;
  Int_t effOrFake_;

  edm::Handle< std::vector<reco::GenParticle> > genParticles_;
  edm::Handle< std::vector< cmg::PhysicsObjectWithPtr< edm::Ptr< reco::GenJet > > > > genJets_;    
  edm::Handle< std::vector<reco::GenParticle> > genLeptons1_;    
  
  void findGenTaus();
  void findGenJets();
  int bosonType_;
  const reco::GenParticle * genBosonLeg_[2];
  const cmg::PhysicsObjectWithPtr<edm::Ptr<reco::GenJet> > * genJet_[2]; 

};

#endif /*TAUEFFICIENCY_H_*/
