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
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "CMGTools/Common/interface/MetUtilities.h"


namespace pat {
  class Jet;
}

class MetFlavorProducer : public edm::EDProducer {
 public:
  typedef math::XYZTLorentzVector LorentzVector;
  typedef math::XYZVector         Vector;
  struct JetInfo {
    LorentzVector p4;
    double        mva;
    double        neutFrac;  
  };

  explicit MetFlavorProducer(const edm::ParameterSet&);
  ~MetFlavorProducer();
  
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

  std::string fPUJetIDName;
  std::string fPUJetIDNameLowPt;

  double                  fJetPtMin;
  double                  fDZMin;
  MetUtilities           *fUtils;
  int                     fMetFlavor;

  void makeCandidates(std::vector<std::pair<LorentzVector,double> >         &iPFInfo,
		      const std::vector<pat::PackedCandidate> &iCands,const reco::Vertex *iPV);
  void makeVertices  (std::vector<Vector>& iPVInfo,
		      const reco:: VertexCollection &iVertices);

  void makeJets      (std::vector<MetUtilities::JetInfo> &iJetInfo,
		      const std::vector<pat::Jet>&  CJets,
		      const reco::VertexCollection& iVertices,double iRho); 

  bool   passPFLooseId(const pat::Jet *iJet);
  double pfCandDz(const pat::PackedCandidate* iPFCand, const reco::Vertex *iPV) ;
  double jetMVA (const pat::Jet *iuncorrJet);
};
