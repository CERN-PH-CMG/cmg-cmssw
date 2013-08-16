// user include files
#include <utility>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "DataFormats/Math/interface/Vector.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include "CMG/MetAnalysis/interface/MetUtilities.h"
#include "CMG/MetAnalysis/interface/MVAMet.h"

using namespace reco;

class MVAMetProducerFromScratch : public edm::EDProducer {
 public:
  typedef math::XYZTLorentzVector LorentzVector;
  typedef math::XYZVector         Vector;
  explicit MVAMetProducerFromScratch(const edm::ParameterSet&);
  ~MVAMetProducerFromScratch();
  
 private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  edm::InputTag fCorrJetName;     
  edm::InputTag fUnCorrJetName; 
  edm::InputTag fPFCandName; 
  edm::InputTag fVertexName; 

  double        fJetPtMin;
  MetUtilities *fUtils;
  MVAMet       *fMVAMet;
  void makeJets      (std::vector<std::pair<LorentzVector,double> > &iJetInfo,PFJetCollection     &iUCJets,PFJetCollection &iCJets,VertexCollection &iVertices); 
  void makeCandidates(std::vector<std::pair<LorentzVector,double> > &iPFInfo,PFCandidateCollection &iCands,Vertex *iPV);
  void makeVertices  (std::vector<Vector>        &iPVInfo,VertexCollection &iVertices);
};
