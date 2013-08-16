#ifndef CMGTAUTEST_CMGTauEfficiency_h
#define CMGTAUTEST_CMGTauEfficiency_h

#include <memory>
#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>
#include <TROOT.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TString.h>
using namespace std;

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"

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
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"



class CMGTauEfficiency : public edm::EDAnalyzer{


 public:
  explicit CMGTauEfficiency(const edm::ParameterSet&);
  ~CMGTauEfficiency();

  virtual void beginJob() ;
  virtual void analyze(const edm::Event&  iEvent, const edm::EventSetup& iSetup);
  virtual void endJob() ;
  
protected:

  edm::Service<TFileService> * file_;

  
private:

  int effOrFake_;
  bool chamonix_;

  edm::Handle< std::vector<reco::GenParticle> > genParticles_;
  edm::Handle< std::vector<reco::GenParticle> > genParticles3_;
  edm::Handle< std::vector<reco::GenParticle> > genLeptons1_;    
  edm::Handle< std::vector< cmg::PhysicsObjectWithPtr< edm::Ptr< reco::GenJet > > > > genJets_;    

  
  void findGenTauVisPart();
  void findGenTauVisPartChamonix();//match leptonic decays and neutrinon using deltaR since mother ref is messed up
  //void findGenTauJets();
  void findGenFakeJets();
  int bosonType_;  
  float genJetPt_[2];
  float genJetEta_[2];
  float genJetPhi_[2];
  int genJetCh_[2];
  float genTauPt_[2];
  float genTauEta_[2];
  float deltaRCheck_[2];
  int nMatchingToTau_[2];

 
  TTree* tree_;
  int npv_;
  int npv1_;
  float gentaupt_;
  float gentaueta_;
  float genjetpt_;
  float genjeteta_;
  float genjetphi_;
  int   genjetnmatching_;
  int   genjetcharge_;
  float recojetpt_;
  float recojeteta_;
  float recojetdeltaR_;
  float recojetdeltaRToPFJet_;
  int recojetiso_;
  int recojetiso2_;
  int recojetiso3_;
  int recojetantimu_;
  int recojetantie_;

  std::string OOTWeightFall11File_;
  std::string OOTWeightChamonixFile_;
  TH2F * H2ChamonixOOTWeight_;
  float_t ootweight_;


  int counterall_;
  int countztt_;
  int countttoverlap_;
  int counttjet1_;
  int counttjet2_;
  int counttlep_;
  int goodevtcounter_;
  int ngenjets_;
  int nrecojets_;
};


#endif

