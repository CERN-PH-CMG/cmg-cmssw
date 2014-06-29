// system include files
#include <memory>
#include <cmath>
#include <algorithm>
#include <TLorentzVector.h>

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/METReco/interface/CommonMETData.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"

#include "RecoMET/METAlgorithms/interface/PFSpecificAlgo.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "CMGTools/Common/plugins/MetFlavorProducer.h"

using namespace edm;
using namespace std;
using namespace reco;

MetFlavorProducer::MetFlavorProducer(const edm::ParameterSet& iConfig) {
  produces<reco::PFMETCollection>();
  //fPatJetName     = iConfig.getParameter<edm::InputTag>("CorrJetName");
  fCorrJetName    = iConfig.getParameter<edm::InputTag>("CorrJetName");
  // fUnCorrJetName  = iConfig.getParameter<edm::InputTag>("JetName");
  fPFCandName     = iConfig.getParameter<edm::InputTag>("PFCandidateName");
  fVertexName     = iConfig.getParameter<edm::InputTag>("VertexName");
  fRhoName        = iConfig.getParameter<edm::InputTag>("RhoName");
  fJetPtMin       = iConfig.getParameter<double>       ("JetPtMin");
  fDZMin          = iConfig.getParameter<double>       ("dZMin");  
  fMetFlavor      = iConfig.getParameter<int>          ("MetFlavor");
  fUtils          = new MetUtilities( iConfig.getParameter<unsigned>("WorkingPointId") );
  fPUJetIdAlgo         = new PileupJetIdAlgo(iConfig.getParameter<edm::ParameterSet>("PUJetId"));
  fPUJetIdAlgoLowPt    = new PileupJetIdAlgo(iConfig.getParameter<edm::ParameterSet>("PUJetIdLowPt"));
}
MetFlavorProducer::~MetFlavorProducer() { 
  delete fUtils;
}
void MetFlavorProducer::beginJob() { }

void MetFlavorProducer::endJob() { } 

void MetFlavorProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {  
  //Uncorrected Jets                                                                                                                                                                                            
//   Handle<PFJetCollection>       lHUCJets;
//   iEvent.getByLabel(fUnCorrJetName, lHUCJets);
//   PFJetCollection               lUCJets = *lHUCJets;

  //Corrected Jets                                                                                                                                                                                              
  Handle< std::vector<pat::Jet> >   lHCJets;
  iEvent.getByLabel(fCorrJetName  , lHCJets);
  const std::vector<pat::Jet>&      lCJets = *lHCJets;

  //Pat Jets
  //Handle<PFJetCollection>       lHCJets;
  //iEvent.getByLabel(fCorrJetName  , lHCJets);
  //PFJetCollection               lCJets = *lHCJets;

  //Get pfCandidates
  Handle<PFCandidateCollection> lHCands;
  iEvent.getByLabel(fPFCandName   , lHCands);
  const PFCandidateCollection&         lCands = *lHCands;

  // vertices    
  Handle<reco::VertexCollection> lHVertices;
  iEvent.getByLabel(fVertexName      , lHVertices); 
  const VertexCollection& lVertices = *lHVertices;
  const Vertex *lPV = 0; if(lVertices.size() > 0) lPV = &lVertices[0]; 

  //Get Rho
  Handle<double>                                        lHRho;
  iEvent.getByLabel(fRhoName                          , lHRho);
  double lRho = *lHRho;
  
  //Make Generic Objects
  std::vector<LorentzVector >                                     lVisible;
  std::vector<std::pair<LorentzVector,double> >         lPFInfo;  makeCandidates(lPFInfo, lCands,lPV);
  std::vector<MetUtilities::JetInfo>                              lJetInfo; makeJets      (lJetInfo, lCJets,lVertices,lRho);
  std::vector<Vector>                                             lVtxInfo; makeVertices  (lVtxInfo,lVertices);
 
  //Calculate the Met
  std::pair<LorentzVector,double> lMetInfo; //MET, SumEt
  if(fMetFlavor == 0)   lMetInfo = fUtils->TKMet  (lPFInfo,fDZMin,2);
  if(fMetFlavor == 1)   lMetInfo = fUtils->TKMet  (lPFInfo,fDZMin,0);
  if(fMetFlavor == 2)   lMetInfo = fUtils->NoPUMet(lPFInfo,lJetInfo,fDZMin);
  if(fMetFlavor == 3)   lMetInfo = fUtils->PUMet  (lPFInfo,lJetInfo,fDZMin);//0.2 for DZ
  if(fMetFlavor == 4)   lMetInfo = fUtils->PUCMet (lPFInfo,lJetInfo,fDZMin);

  //Add a PF Met object and put it in the event
  PFMET lDummy;
  PFMET lMet(lDummy.getSpecific(),lMetInfo.second,lMetInfo.first,lPV->position()); //Use PFMET sum Et

  std::auto_ptr<reco::PFMETCollection> lPFMetColl;
  lPFMetColl.reset     ( new reco::PFMETCollection);
  lPFMetColl->push_back( lMet);
  iEvent.put( lPFMetColl );
}

void MetFlavorProducer::makeJets(std::vector<MetUtilities::JetInfo> &iJetInfo,
				 const std::vector<pat::Jet>& iCJets,
				 const VertexCollection &iVertices,double iRho) { 
  
  for(int i1 = 0; i1 < (int) iCJets .size(); i1++) {   // corrected jets collection                                         
    const pat::Jet     *pCJet  = &(iCJets.at(i1));

    //std::cout<<" MetFlavorProducer::makeJets  input :  "<<i1<<"  "<<pCJet->pt()<<" "<<pCJet->eta()<<" "<<pCJet->correctedJet(0).pt()<<" "<<pCJet->neutralEmEnergy()<<" "<<pCJet->neutralHadronEnergy()<<std::endl;
    
    if(pCJet->pt() < fJetPtMin) continue;

    if( !passPFLooseId(pCJet) ) continue;

    //std::cout<<" MetFlavorProducer::makeJets  PFLooseId :  "<<i1<<"  "<<pCJet->pt()<<" "<<pCJet->eta()<<" "<<pCJet->correctedJet(0).pt()<<" "<<pCJet->neutralEmEnergy()<<" "<<pCJet->neutralHadronEnergy()<<std::endl;

    double lJec = 0;
    double lMVA = jetMVA(pCJet,lJec,iVertices[0],iVertices,false);
    double lJetEnergy = pCJet->correctedJet(0).pt()/pCJet->pt()*pCJet->energy();
    // COLIN 53 
    double lNeuFrac = 1.;
    if (fabs(pCJet->eta())<2.5)
      lNeuFrac = (pCJet->neutralEmEnergy() + pCJet->neutralHadronEnergy())/lJetEnergy;
    // Old 52 recipe:
    // double lNeuFrac = (pCJet->neutralEmEnergy() + pCJet->neutralHadronEnergy())/lJetEnergy;
    MetUtilities::JetInfo pJetObject; 
    pJetObject.p4       = pCJet->p4(); 
    pJetObject.mva      = lMVA;
    pJetObject.neutFrac = lNeuFrac;
    //std::cout<<" MetFlavorProducer::makeJets  final:  "<<i1<<"  "<<pJetObject.p4.pt()<<" "<<pJetObject.mva<<" "<<pJetObject.neutFrac<<std::endl;
    iJetInfo.push_back(pJetObject);
  }
}

void MetFlavorProducer::makeCandidates(std::vector<std::pair<LorentzVector,double> > &iPFInfo,const PFCandidateCollection &iCands,const Vertex *iPV) { 
  for(int i0 = 0; i0 < (int)iCands.size(); i0++) {
    const PFCandidate*  pflowCand = &(iCands.at(i0));
    double pDZ = -999;
    if(iPV != 0) pDZ  = pfCandDz(pflowCand,iPV); //If there is no track return negative number -999
    //LorentzVector pVec; pVec.SetCoordinates(pflowCand->pt(),pflowCand->eta(),pflowCand->phi(),pflowCand->mass());
    std::pair<LorentzVector,double> pPFObject(pflowCand->p4(),pDZ);
    iPFInfo.push_back(pPFObject);
  }
}
void MetFlavorProducer::makeVertices(std::vector<Vector>        &iPVInfo,const VertexCollection &iVertices) { 
  for(int i0    = 0; i0 < (int)iVertices.size(); i0++) {
    const Vertex       *pVertex = &(iVertices.at(i0));

    if(fabs(pVertex->z())           > 24.) continue;
    if(pVertex->ndof()              <  4.) continue;
    if(pVertex->position().Rho()    >  2.) continue;

    Vector pVec; pVec.SetCoordinates(pVertex->x(),pVertex->y(),pVertex->z());
    iPVInfo.push_back(pVec);
  }
}
bool MetFlavorProducer::passPFLooseId(const pat::Jet *iJet) { 


  //std::cout << " ==> " << iJet->pt() << " - " << iJet->eta() << " : " << iJet->energy() << " - " << iJet->neutralEmEnergy() << " - " << iJet->nConstituents() << " - " << iJet->chargedHadronEnergy() << " _ " << iJet->chargedEmEnergy() << " - " << iJet->chargedMultiplicity() << std::endl;
  //if(iJet->energy()== 0)                                                           return false;

  double lJetEnergy = iJet->correctedJet(0).pt()/iJet->pt()*iJet->energy();
  //if((iJet->neutralHadronEnergy()+iJet->HFHadronEnergy())/lJetEnergy > 0.99)      return false;//Jose: this is the correct PFLoose id 
  if((iJet->neutralHadronEnergy())/lJetEnergy > 0.99)      return false;//Jose: HF energy is not used in the MVA Met training
  if(iJet->neutralEmEnergy()/lJetEnergy     > 0.99)                               return false;
  if(iJet->nConstituents()                  <  2)                                 return false;
  if(iJet->chargedHadronEnergy()/lJetEnergy <= 0     && fabs(iJet->eta()) < 2.4 ) return false;
  if(iJet->chargedEmEnergy()/lJetEnergy     >  0.99  && fabs(iJet->eta()) < 2.4 ) return false;
  if(iJet->chargedMultiplicity()            < 1      && fabs(iJet->eta()) < 2.4 ) return false;
  return true;
}
double MetFlavorProducer::pfCandDz(const PFCandidate* iPFCand, const Vertex *iPV) { 
  double lDz = -999;
  if(iPFCand->trackRef().isNonnull())    lDz = fabs(iPFCand->   trackRef()->dz(iPV->position()));
  else if(iPFCand->gsfTrackRef().isNonnull()) lDz = fabs(iPFCand->gsfTrackRef()->dz(iPV->position()));
  return lDz;
}
double MetFlavorProducer::jetMVA (const Jet *iCorrJet,double iJec, const Vertex iPV, const reco::VertexCollection &iAllvtx,bool iPrintDebug) { 
  PileupJetIdentifier lPUJetId     =  fPUJetIdAlgo->computeIdVariables(iCorrJet,iJec,&iPV,iAllvtx,true);
  //PileupJetIdentifier *lPUJetIdRef =  &lPUJetId;
  if(iCorrJet->pt() < 10) {
    PileupJetIdentifier pPUJetId   =  fPUJetIdAlgoLowPt->computeIdVariables(iCorrJet,iJec,&iPV,iAllvtx,true);
    //lPUJetIdRef = &pPUJetId;
  }
  if(iPrintDebug) { std::cout << "Debug Jet MVA: "
			      << lPUJetId.nvtx()      << " "
			      << iCorrJet->pt()       << " "
			      << lPUJetId.jetEta()    << " "
			      << lPUJetId.jetPhi()    << " "
			      << lPUJetId.d0()        << " "
			      << lPUJetId.dZ()        << " "
			      << lPUJetId.beta()      << " "
			      << lPUJetId.betaStar()  << " "
			      << lPUJetId.nCharged()  << " "
			      << lPUJetId.nNeutrals() << " "
			      << lPUJetId.dRMean()    << " "
			      << lPUJetId.frac01()    << " "
			      << lPUJetId.frac02()    << " "
			      << lPUJetId.frac03()    << " "
			      << lPUJetId.frac04()    << " "
			      << lPUJetId.frac05()
			      << " === : === "
			      << lPUJetId.mva() << " " << endl;
  }

  return lPUJetId.mva();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE (MetFlavorProducer);
