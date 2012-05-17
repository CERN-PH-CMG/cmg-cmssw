// -*- C++ -*-
//
// Package:    JetAnalyzer
// Class:      JetAnalyzer
// 
/**\class JetAnalyzer JetAnalyzer.cc HggAnalysis/JetAnalyzer/src/JetAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Martina Malberti,27 2-019,+41227678349,
//         Created:  Mon Mar  5 16:39:53 CET 2012
// $Id: JetAnalyzer.h,v 1.9 2012/04/23 11:25:58 malberti Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"


#include "PileupJetIdNtupleAlgo.h"


// ROOT includes
#include "TH1.h"

//
// class declaration
//

class JetAnalyzer : public edm::EDAnalyzer 
{
public:
  explicit JetAnalyzer(const edm::ParameterSet&);
  ~JetAnalyzer();

private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  void bookTree();
  void ResetTreeVariables();
  void FillMCPileUpInfo(const edm::Event&, const edm::EventSetup&);
  bool matchingToGenJet( const pat::Jet jet, edm::View<reco::GenJet> genJets , float& genPt, float& genDr);
  void DiMuonSelection(edm::View<pat::Muon> muons, int& goodMuon1, int& goodMuon2, bool& isZcandidate);

  // ----------member data ---------------------------

  // PU jet identifier 
  PileupJetIdNtupleAlgo *puIdAlgo_;
      
  /// input tag for jets, etc...
  edm::InputTag MCPileupTag_;    
  edm::InputTag PVTag_;    
  edm::InputTag JetTag_;
  edm::InputTag GenJetTag_;
  edm::InputTag MuonTag_; 
  std::vector<edm::InputTag> MvaTags_;
  std::vector<edm::InputTag> IdTags_;
  edm::InputTag RhoTag_;
  std::string jecTag_;
  bool applyJec_;
  
  void initJetEnergyCorrector(const edm::EventSetup &iSetup, bool isData);
  FactorizedJetCorrector *jecCor_;
  std::vector<JetCorrectorParameters> jetCorPars_;
  
  bool dataFlag_;
  bool computeTMVA_, requireZ_;

  double impactParTkThreshold_;
  std::string tmvaWeights_;
  std::string tmvaMethod_;

  double jetPtThreshold_;

  int PUoot_early_NumInteractions     ;
  float PUoot_early_TrueNumInteractions ;
  int PUoot_late_NumInteractions      ;
  float PUoot_late_TrueNumInteractions  ;
  int PUit_NumInteractions            ;
  float PUit_TrueNumInteractions        ;
  std::vector<float> mvas_;
  std::vector<int> ids_;
	
  PFJetIDSelectionFunctor  pfjetIdLoose_;


  /// output tree variables
  TTree *tree ;
  
  // tree var
  
  int PUit_n ;
  float PUit_nTrue ;
  int PUoot_early_n ;
  float PUoot_early_nTrue ;
  int PUoot_late_n ;
  float PUoot_late_nTrue ;
  
  int nvtx ;

  bool jetLooseID;
  bool isMatched;
  int  jetFlavour;
  float jetGenPt;
  float jetGenDr;
  int njets;

  float dimuonPt;
  float dphiZJet;

  ////  float mva;

   
};
