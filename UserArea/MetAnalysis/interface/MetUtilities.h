#ifndef METUTILITIES_H
#define METUTILITIES_H

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "CMG/JetIDAnalysis/interface/PileupJetIdNtupleAlgo.h"

#include <vector>

using namespace reco;

class MetUtilities {

public:

  MetUtilities(const edm::ParameterSet &iConfig, bool isData);
  virtual ~MetUtilities();

  bool filter(const PFJet *iJet,Double_t iPhi1,Double_t iEta1,Double_t iPhi2,Double_t iEta2);

  bool passPFLooseId(const PFJet *iJet);

  // double correctedJetPt(const PFJet *iJet,double iRho);
  float correctedJetPt(const PFJet *iJet,double iRho);      // chiara

  void addNeut(const PFJet *iJet, Candidate::LorentzVector &iVec, Double_t &iSumEt, double iRho, int iSign);

  float passJetId(const PFJet *iJet, const Vertex iPV, const reco::VertexCollection &iAllvtx, double iRho);

  float pfCandDz(const PFCandidateRef iPFCandRef, const Vertex iPV);

protected:

  bool fData;
  
  // PU jet identifier 
  PileupJetIdNtupleAlgo *fPUJetIdAlgo;

  // jet corrections
  FactorizedJetCorrector *fJetCorrector;
};

#endif
