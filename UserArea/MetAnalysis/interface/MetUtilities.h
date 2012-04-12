#ifndef METUTILITIES_H
#define METUTILITIES_H

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "CMG/JetIDAnalysis/interface/PileupJetIdNtupleAlgo.h"

#include <vector>

using namespace reco;

class MetUtilities {

public:

  MetUtilities(const edm::ParameterSet &iConfig);//, bool isData);
  virtual ~MetUtilities();

  bool filter(const PFJet *iJet,Double_t iPhi1,Double_t iEta1,Double_t iPhi2,Double_t iEta2);

  bool passPFLooseId(const PFJet *iJet);

  float correctedJetPt(const PFJet *iJet,double iRho);      

  void addNeut(const PFJet *iuncorrJet, const PFJet *icorrJet, Candidate::LorentzVector *iVec, double *iSumEt, int iSign);
  void addNeut(                         const PFJet *icorrJet, Candidate::LorentzVector *iVec, double *iSumEt, int iSign);

  float passJetId(const PFJet *iuncorrJet, const PFJet *icorrJet, const Vertex iPV, const reco::VertexCollection &iAllvtx);

  float pfCandDz(const PFCandidateRef iPFCandRef, const Vertex iPV);
 
  const PFMET clean (const PFMET *iMet,            double iEta1,double iPhi1,            double iEta2,double iPhi2,const PFJetCollection *iJets);
  const PFMET recoil(const PFMET *iMet,double iPt1,double iEta1,double iPhi1,double iPt2,double iEta2,double iPhi2,const PFJetCollection *iJets=0);
  const PFMET recoil(const PFMET *iMet,double iPtVis,double iPhiVis,double iSumEtVis);
protected:
  
  // PU jet identifier 
  PileupJetIdNtupleAlgo *fPUJetIdAlgo;

  // jet corrections
  //bool fData;
  //FactorizedJetCorrector *fJetCorrector;
};

#endif
