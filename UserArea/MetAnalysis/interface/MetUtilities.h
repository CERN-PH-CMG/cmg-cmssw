#ifndef METUTILITIES_H
#define METUTILITIES_H

#include <utility>
#include <vector>
#include "DataFormats/Math/interface/Vector.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "CMG/JetIDAnalysis/interface/PileupJetIdNtupleAlgo.h"

using namespace reco;

class MetUtilities {
 public:
  typedef math::XYZTLorentzVector LorentzVector;
  typedef math::RhoEtaPhiVector   Vector;

  MetUtilities(const edm::ParameterSet &iConfig);//, bool isData);
  virtual ~MetUtilities();

  bool filter(const PFJet *iJet,Double_t iPhi1,Double_t iEta1,Double_t iPhi2,Double_t iEta2);

  bool passPFLooseId(const PFJet *iJet);

  float correctedJetPt(const PFJet *iJet,double iRho);      

  void addNeut(const PFJet *iuncorrJet, const PFJet *icorrJet, Candidate::LorentzVector *iVec, double *iSumEt, int iSign);
  void addNeut(                         const PFJet *icorrJet, Candidate::LorentzVector *iVec, double *iSumEt, int iSign);
  double jetMVA   (const PFJet *iuncorrJet,double iJec,            const Vertex iPV, const reco::VertexCollection &iAllvtx);
  bool   passJetId(const PFJet *iuncorrJet, const PFJet *icorrJet, const Vertex iPV, const reco::VertexCollection &iAllvtx);

  float pfCandDz(const PFCandidate*  iPFCand, const Vertex *iPV);
 
  const PFMET clean (const PFMET *iMet,            double iEta1,double iPhi1,            double iEta2,double iPhi2,const PFJetCollection *iJets);
  const PFMET recoil(const PFMET *iMet,double iPt1,double iEta1,double iPhi1,double iPt2,double iEta2,double iPhi2,const PFJetCollection *iJets=0);
  const PFMET recoil(const PFMET *iMet,double iPtVis,double iPhiVis,double iSumEtVis);
  ////
  bool              passMVA  (std::pair<LorentzVector,double> iJet);
  LorentzVector    *leadPt   (std::vector<std::pair<LorentzVector,double> > &iJets,bool iFirst);
  int               NJets    (std::vector<std::pair<LorentzVector,double> > &iJets,double iPt);
  double            deltaR   (LorentzVector &iVec1,LorentzVector &iVec2);
  void              cleanJets(std::vector<LorentzVector> &iVis,std::vector<std::pair<LorentzVector,double> > &iJets);
  std::pair<LorentzVector,double> TKMet   (std::vector<std::pair<LorentzVector,double> > &iCands,int iDZ,bool iLowDz);
  std::pair<LorentzVector,double> JetMet  (std::vector<std::pair<LorentzVector,double> > &iJets ,bool iPassMVA);
  std::pair<LorentzVector,double> NoPUMet (std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<std::pair<LorentzVector,double> > &iJets,double iDZ);
  std::pair<LorentzVector,double> PUMet   (std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<std::pair<LorentzVector,double> > &iJets,double iDZ);
  std::pair<LorentzVector,double> PUCMet  (std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<std::pair<LorentzVector,double> > &iJets,double iDZ);

  std::pair<LorentzVector,double> PFRecoil(double iSumEt,LorentzVector iVis,std::vector<std::pair<LorentzVector,double> > &iCands,double iDZ);
  std::pair<LorentzVector,double> TKRecoil(double iSumEt,LorentzVector iVis,std::vector<std::pair<LorentzVector,double> > &iCands,double iDZ);
  std::pair<LorentzVector,double> NoPURecoil(double iSumEt,LorentzVector iVis,
				      std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<std::pair<LorentzVector,double> > &iJets,double iDZ);
  std::pair<LorentzVector,double> PUCRecoil(double iSumEt,LorentzVector iVis,
				      std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<std::pair<LorentzVector,double> > &iJets,double iDZ);
protected:
  // PU jet identifier 
  PileupJetIdNtupleAlgo *fPUJetIdAlgo;
  Float_t mvacut_[3][4][4];  //Jet Id MVA
};

#endif
