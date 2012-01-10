#ifndef Bianchi_TauTauStudies_MuTauStreamAnalyzer_h
#define Bianchi_TauTauStudies_MuTauStreamAnalyzer_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include <TRandom3.h>


#include <string>
#include <utility>
#include <map>


class MuTauStreamAnalyzer : public edm::EDAnalyzer{


 public:
  typedef cmg::PFJet Jet; 
  typedef std::vector< cmg::PFJet > JetCollection;

  struct more {
    bool operator() (const double& lhs, const double& rhs) const
    {return lhs>rhs;}
  };

  explicit MuTauStreamAnalyzer(const edm::ParameterSet&);
  ~MuTauStreamAnalyzer();

  /*   unsigned int jetID( const Jet* jet, const reco::Vertex* vtx, std::vector<float> vtxZ, std::map<std::string,float>& map_); */
  //COLINB4: cannot do PV vertex assoc in JetID -> simplifying interface
  unsigned int jetID( const Jet& jet );

  Jet* newJetMatched( const Jet* oldJet , const JetCollection* newJets);

  void beginJob() ;
  void analyze(const edm::Event&  iEvent, const edm::EventSetup& iSetup);
  void endJob() ;

 private:

  TFile* file_;
  TTree* tree_;

  TRandom3* tRandom_;

  edm::LumiReWeighting LumiWeights_;

  edm::InputTag diTauTag_;
  edm::InputTag jetsTag_;
  edm::InputTag newJetsTag_;
  edm::InputTag metTag_;
  edm::InputTag rawMetTag_;
  edm::InputTag muonsTag_;
  edm::InputTag muonsRelTag_;
  edm::InputTag verticesTag_;
  edm::InputTag triggerResultsTag_;

  bool isMC_;
  bool verbose_;
  float minCorrPt_;
  float minJetID_;
  float deltaRLegJet_;

  // below are all members of the TTree. -------------------------

  // high efficiency 
  std::vector< double >* jetsBtagHE_;
  
  // high purity
  std::vector< double >* jetsBtagHP_;

  std::vector< float >* jetsChNfraction_;
  std::vector< float >* jetsChEfraction_;

  // not used 
  std::vector< float >* jetMoments_;

  // deta, dphi between photon candidates in the tau and leading charged pion candidate
  // one value per photon candidate in the tau
  std::vector< float >* gammadEta_;
  std::vector< float >* gammadPhi_;
  std::vector< float >* gammadR_;
  // pt of the photon candidate 
  std::vector< float >* gammaPt_;

  //? trigger bits or trigger matching 
  std::vector< int >* tauXTriggers_;
  //?
  std::vector< int >* triggerBits_;


  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* jetsP4_; 
  // same, passing the id
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* jetsIDP4_;
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* jetsIDUpP4_;
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* jetsIDDownP4_;
  // same jets corrected for L1Offset instead of L1FastJet. 
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* jetsIDL1OffsetP4_;
  // gen jets matched to the corresponding reco jets at pat level
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* genJetsIDP4_; 

  // only one di-tau is filled
 
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* diTauVisP4_; 
  // collinear approx
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* diTauCAP4_; 
  // improved collinear approx
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* diTauICAP4_; 
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* diTauSVfitP4_; 

  // first mu then tau 
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* diTauLegsP4_;
  // see code
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* genDiTauLegsP4_; 

  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >  >* METP4_;
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >  >* genMETP4_;
  // gen boson 
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >  >* genVP4_;
  int genDecay_;
  
  // additional muons w/r to the one in the di-tau
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* extraMuons_; 

  // all muons
  std::vector< ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* pfMuons_; 
  
  unsigned long run_,event_,lumi_;
  // not used 
  float sumEt_;
  // 2 versions of the isolation
  float chIsoLeg1v1_,nhIsoLeg1v1_,phIsoLeg1v1_,elecIsoLeg1v1_,muIsoLeg1v1_;
  float chIsoPULeg1v1_,nhIsoPULeg1v1_,phIsoPULeg1v1_;
  float chIsoLeg1v2_,nhIsoLeg1v2_,phIsoLeg1v2_,elecIsoLeg1v2_,muIsoLeg1v2_;
  float chIsoPULeg1v2_,nhIsoPULeg1v2_,phIsoPULeg1v2_;
  float chIsoLeg2_,nhIsoLeg2_,phIsoLeg2_;
  float dxy1_,dxy2_;
  float dz1_,dz2_;
  float MtLeg1_;
  float pZeta_;
  float pZetaVis_;
  // a method of the di-tau / not used yet
  float pZetaSig_;
  // also
  float mTauTauMin_;
  // number of prim vertices
  float numPV_;
  int numOfDiTaus_;
  int numOfLooseIsoDiTaus_;
  int decayMode_;
  // NSVFit -> new svfit (too slow, not used)
  float diTauNSVfitMass_;
  float diTauNSVfitMassErrUp_;
  float diTauNSVfitMassErrDown_;
  float visibleTauMass_;

  float leadPFChargedHadrMva_;
  float leadPFChargedHadrHcalEnergy_;
  float leadPFChargedHadrEcalEnergy_;
  float leadPFChargedHadrTrackPt_;
  float leadPFChargedHadrTrackP_;
  float leadPFChargedHadrPt_;
  float leadPFChargedHadrP_;
  float leadPFCandMva_;
  float leadPFCandHcalEnergy_;
  float leadPFCandEcalEnergy_;
  float leadPFCandPt_;
  float leadPFCandP_;
  int signalPFChargedHadrCands_;
  int signalPFGammaCands_;
  float emFraction_;
  float hasGsf_;

  // check HPSWP, and check which is the tightest that is passed.
  // 0: loose ; 1:medium, etc
  int tightestHPSWP_;
  // same for isolation
  int tightestHPSDBWP_;
  // matched to generated tau lepton
  int isTauLegMatched_;
  // same 
  int isMuLegMatched_;
  // second lepton veto
  int muFlag_;
  // does the tau leading charged hadron has a kalman filter track
  int hasKft_;

  float diTauCharge_;
  float rhoFastJet_;
  float rhoNeutralFastJet_;
  // generator weight 
  float embeddingWeight_;
  // all the following is from PUSummaryInfo
  int nPUVertices_;
  // bunch crossing before the one 
  int nPUVerticesM1_;
  // after the one 
  int nPUVerticesP1_;
  // true number of pu interactions 
  int nPUtruth_;

  float mcPUweight_;

};


#endif
