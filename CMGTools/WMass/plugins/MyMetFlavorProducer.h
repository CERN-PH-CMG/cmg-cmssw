// user include files
#include <utility>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/Vector.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "CMGTools/External/interface/PileupJetIdAlgo.h"

#include "CMGTools/Common/interface/MetUtilities.h"

//COLIN don't do that!!
using namespace reco;

namespace pat {
  class Jet;
}

class MyMetFlavorProducer : public edm::EDProducer {
 public:
  typedef math::XYZTLorentzVector LorentzVector;
  typedef math::XYZVector         Vector;
  struct JetInfo {
    LorentzVector p4;
    double        mva;
    double        neutFrac;  
  };

  explicit MyMetFlavorProducer(const edm::ParameterSet&);
  ~MyMetFlavorProducer();
  
 private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  //edm::InputTag fPatJetName;     
  edm::InputTag fCorrJetName;     
  edm::InputTag fUnCorrJetName; 
  edm::InputTag fPFCandName; 
  edm::InputTag fVertexName; 
  edm::InputTag fRhoName; 

  double                  fJetPtMin;
  double                  fDZMin;
  double                  fCandEtaMin;
  double                  fCandEtaMax;
  double                  fChargedCandPtMin;
  double                  fNeutralCandPMin;
  std::string             fCandFlavor;
  PileupJetIdAlgo        *fPUJetIdAlgo;
  PileupJetIdAlgo        *fPUJetIdAlgoLowPt;
  MetUtilities           *fUtils;
  int                     fMetFlavor;

  void makeCandidates(std::vector<std::pair<LorentzVector,double> >         &iPFInfo,
		      const std::vector<cmg::Candidate> &iCands,const Vertex *iPV, std::string candFlavor,double iEtaMin,double iEtaMax,double iPtMin,double iPMin);
  void makeVertices  (std::vector<Vector>& iPVInfo,
		      const VertexCollection &iVertices);

  void makeJets      (std::vector<MetUtilities::JetInfo> &iJetInfo,
		      const std::vector<cmg::PFJet>&  CJets,
		      const VertexCollection& iVertices,double iRho); 

  bool   passPFLooseId(const cmg::PFJet *iJet);
  double pfCandDz(const cmg::Candidate* iPFCand, const Vertex *iPV) ;
  double jetMVA  (const Jet *iuncorrJet,double iJec, const Vertex iPV, const reco::VertexCollection &iAllvtx,bool iPrintDebug=false);
};
