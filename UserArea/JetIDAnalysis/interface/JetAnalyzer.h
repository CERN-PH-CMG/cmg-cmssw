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
// $Id$
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
#include "CMGTools/External/interface/PileupJetIdentifier.h"


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
      bool matchingToGenJet( const pat::Jet jet, edm::View<reco::GenJet> genJets );
  void DiMuonSelection(edm::View<pat::Muon> muons, int goodMuon1, int goodMuon2, bool isZcandidate);

      // ----------member data ---------------------------

      // PU jet identifier 
      PileupJetIdentifier *puIdentifier;
      
      /// input tag for jets, etc...
  edm::InputTag PVTag_;    
  edm::InputTag JetTag_;
  edm::InputTag GenJetTag_;
  edm::InputTag MuonTag_; 

      bool dataFlag_;
      bool computeTMVA_;

  double impactParTkThreshold_;
  std::string tmvaWeights_;
  std::string tmvaMethod_;


      /// output tree variables
      TTree *tree ;
 
      // tree var
      int nvtx ;
      float jetPt, jetEta, jetPhi, jetM;
      float nCharged, nNeutrals, chgEMfrac, neuEMfrac, chgHadrfrac, neuHadrfrac, nParticles;
      float leadPt, leadEta, secondPt, secondEta, leadNeutPt, leadNeutEta, leadEmPt, leadEmEta, leadChPt, leadChEta;
      float dRLeadCent, dRLead2nd, dRMean, dRMeanNeut, dRMeanEm, dRMeanCh, ptD;

      float leadFrac, leadChFrac, leadNeutFrac;
      float majW, minW; 
      
      float isMatched;
      float jetFlavour;
      float mva;

   
};
