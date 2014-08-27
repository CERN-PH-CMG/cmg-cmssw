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
  fPUJetIDName    = iConfig.getParameter<std::string>("puJetIDName");
  fPUJetIDNameLowPt = iConfig.getParameter<std::string>("puJetIDNameLowPt");
  // fPUJetIdAlgo         = new PileupJetIdAlgo(iConfig.getParameter<edm::ParameterSet>("PUJetId"));
  // fPUJetIdAlgoLowPt    = new PileupJetIdAlgo(iConfig.getParameter<edm::ParameterSet>("PUJetIdLowPt"));
}
MetFlavorProducer::~MetFlavorProducer() { 
  delete fUtils;
}
void MetFlavorProducer::beginJob() { }

void MetFlavorProducer::endJob() { } 

void MetFlavorProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {  
  //Uncorrected Jets                                                                                                                                                                                            
//   edm::Handle<PFJetCollection>       lHUCJets;
//   iEvent.getByLabel(fUnCorrJetName, lHUCJets);
//   PFJetCollection               lUCJets = *lHUCJets;

  //Corrected Jets                                                                                                                                                                                              
  edm::Handle< std::vector<pat::Jet> >   lHCJets;
  iEvent.getByLabel(fCorrJetName  , lHCJets);
  const std::vector<pat::Jet>&      lCJets = *lHCJets;

  //Pat Jets
  //Handle<PFJetCollection>       lHCJets;
  //iEvent.getByLabel(fCorrJetName  , lHCJets);
  //PFJetCollection               lCJets = *lHCJets;

  //Get pfCandidates
  // edm::Handle<PFCandidateCollection> lHCands;
  edm::Handle<std::vector<pat::PackedCandidate>> lHCands;
  iEvent.getByLabel(fPFCandName   , lHCands);
  const std::vector<pat::PackedCandidate>& lCands = *lHCands;

  // vertices    
  edm::Handle<reco::VertexCollection> lHVertices;
  iEvent.getByLabel(fVertexName      , lHVertices); 
  const reco::VertexCollection& lVertices = *lHVertices;
  const reco::Vertex *lPV = 0; 
  if(lVertices.size() > 0) 
    lPV = &lVertices[0]; 

  //Get Rho
  edm::Handle<double> lHRho;
  iEvent.getByLabel(fRhoName, lHRho);
  double lRho = *lHRho;
  
  //Make Generic Objects
  std::vector<std::pair<LorentzVector,double> > lPFInfo;
  makeCandidates(lPFInfo, lCands,lPV);

  std::vector<MetUtilities::JetInfo> lJetInfo; 
  makeJets(lJetInfo, lCJets, lVertices, lRho);

  std::vector<Vector> lVtxInfo;
  makeVertices(lVtxInfo, lVertices);
 
  //Calculate the Met
  std::pair<LorentzVector,double> lMetInfo; //MET, SumEt
  if(fMetFlavor == 0)   lMetInfo = fUtils->TKMet  (lPFInfo,fDZMin,2);
  if(fMetFlavor == 1)   lMetInfo = fUtils->TKMet  (lPFInfo,fDZMin,0);
  if(fMetFlavor == 2)   lMetInfo = fUtils->NoPUMet(lPFInfo,lJetInfo,fDZMin);
  if(fMetFlavor == 3)   lMetInfo = fUtils->PUMet  (lPFInfo,lJetInfo,fDZMin);//0.2 for DZ
  if(fMetFlavor == 4)   lMetInfo = fUtils->PUCMet (lPFInfo,lJetInfo,fDZMin);

  //Add a PF Met object and put it in the event
  reco::PFMET lDummy;
  reco::PFMET lMet(lDummy.getSpecific(), lMetInfo.second, lMetInfo.first, lPV->position()); //Use PFMET sum Et

  std::auto_ptr<reco::PFMETCollection> lPFMetColl;
  lPFMetColl.reset(new reco::PFMETCollection);
  lPFMetColl->push_back(lMet);
  iEvent.put(lPFMetColl);
}

void MetFlavorProducer::makeJets(std::vector<MetUtilities::JetInfo> &iJetInfo,
				 const std::vector<pat::Jet>& iCJets,
				 const reco::VertexCollection &iVertices,double iRho) { 
  
  for(int i1 = 0; i1 < (int) iCJets .size(); i1++) {   // corrected jets collection                                         
    const pat::Jet* pCJet = &(iCJets.at(i1));

    //std::cout<<" MetFlavorProducer::makeJets  input :  "<<i1<<"  "<<pCJet->pt()<<" "<<pCJet->eta()<<" "<<pCJet->correctedJet(0).pt()<<" "<<pCJet->neutralEmEnergy()<<" "<<pCJet->neutralHadronEnergy()<<std::endl;
    
    if(pCJet->pt() < fJetPtMin) continue;

    if( !passPFLooseId(pCJet) ) continue;

    //std::cout<<" MetFlavorProducer::makeJets  PFLooseId :  "<<i1<<"  "<<pCJet->pt()<<" "<<pCJet->eta()<<" "<<pCJet->correctedJet(0).pt()<<" "<<pCJet->neutralEmEnergy()<<" "<<pCJet->neutralHadronEnergy()<<std::endl;

    double lMVA = jetMVA(pCJet);
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

void MetFlavorProducer::makeCandidates(std::vector<std::pair<LorentzVector,double> > &iPFInfo,const std::vector<pat::PackedCandidate> &iCands,const reco::Vertex *iPV) { 
  for(int i0 = 0; i0 < (int)iCands.size(); i0++) {
    const pat::PackedCandidate*  pflowCand = &(iCands.at(i0));
    double pDZ = -999;
    if(iPV != 0) pDZ  = pfCandDz(pflowCand,iPV); //If there is no track return negative number -999
    //LorentzVector pVec; pVec.SetCoordinates(pflowCand->pt(),pflowCand->eta(),pflowCand->phi(),pflowCand->mass());
    std::pair<LorentzVector,double> pPFObject(pflowCand->p4(),pDZ);
    iPFInfo.push_back(pPFObject);
  }
}
void MetFlavorProducer::makeVertices(std::vector<Vector>        &iPVInfo,const reco::VertexCollection &iVertices) { 
  for(int i0    = 0; i0 < (int)iVertices.size(); i0++) {
    const reco::Vertex       *pVertex = &(iVertices.at(i0));

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
double MetFlavorProducer::pfCandDz(const pat::PackedCandidate* iPFCand, const reco::Vertex *iPV) { 
  double lDz = -999;
  // FIXME - JAN - does pseudoTrack give something different from 
  // PackedCandidate::dz(..) ?
  lDz = fabs(iPFCand->dz(iPV->position()));
  return lDz;
}
double MetFlavorProducer::jetMVA (const pat::Jet *iCorrJet) { 
  // JAN - this returns the pre-computed PU jet ID present in miniAOD
  // if you need another primary vertex, simply redo the PU jet ID production
  if (iCorrJet->pt() > 10.)
    return iCorrJet->userFloat(fPUJetIDName);

  return iCorrJet->userFloat(fPUJetIDNameLowPt);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE (MetFlavorProducer);
