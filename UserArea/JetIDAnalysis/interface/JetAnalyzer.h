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
// $Id: JetAnalyzer.h,v 1.2 2012/03/14 12:05:21 musella Exp $
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


#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 

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

  bool dataFlag_;
  bool computeTMVA_;

  double impactParTkThreshold_;
  std::string tmvaWeights_;
  std::string tmvaMethod_;

  int PUoot_early_NumInteractions     ;
  float PUoot_early_TrueNumInteractions ;
  int PUoot_late_NumInteractions      ;
  float PUoot_late_TrueNumInteractions  ;
  int PUit_NumInteractions            ;
  float PUit_TrueNumInteractions        ;


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

  //// float jetPt, jetEta, jetPhi, jetM;
  //// float nCharged, nNeutrals, chgEMfrac, neuEMfrac, chgHadrfrac, neuHadrfrac, nParticles;
  //// float leadPt, leadEta, secondPt, secondEta, leadNeutPt, leadNeutEta, leadEmPt, leadEmEta, leadChPt, leadChEta;
  //// float dRLeadCent, dRLead2nd, dRMean, dRMeanNeut, dRMeanEm, dRMeanCh, ptD;
  //// 
  //// float leadFrac, leadChFrac, leadNeutFrac;
  //// float majW, minW; 
      
  bool isMatched;
  int  jetFlavour;
  float jetGenPt;
  float jetGenDr;
  int njets;

  ////  float mva;

   
};
