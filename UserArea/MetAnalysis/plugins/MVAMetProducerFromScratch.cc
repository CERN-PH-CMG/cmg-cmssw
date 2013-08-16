// system include files
#include <memory>
#include <cmath>
#include <algorithm>

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/METReco/interface/CommonMETData.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"

#include "RecoMET/METAlgorithms/interface/PFSpecificAlgo.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "CMG/MetAnalysis/plugins/MVAMetProducerFromScratch.h"

using namespace edm;
using namespace std;
using namespace reco;

MVAMetProducerFromScratch::MVAMetProducerFromScratch(const edm::ParameterSet& iConfig) {
  produces<reco::PFMETCollection>();
  fCorrJetName    = iConfig.getParameter<edm::InputTag>("JetName");
  fUnCorrJetName  = iConfig.getParameter<edm::InputTag>("CorrJetName");
  fPFCandName     = iConfig.getParameter<edm::InputTag>("PFCandidateName");
  fVertexName     = iConfig.getParameter<edm::InputTag>("VertexName");
  fJetPtMin       = iConfig.getParameter<double>       ("JetPtMin");
  fUtils          = new MetUtilities(iConfig.getParameter<edm::ParameterSet>("puJetIDAlgo"));
  fMVAMet         = new MVAMet();
  fMVAMet         ->Initialize(iConfig,
			       TString((getenv("CMSSW_BASE")+string("/src/CMG/MetAnalysis/data/gbrmet.root"))),
			       TString((getenv("CMSSW_BASE")+string("/src/CMG/MetAnalysis/data/gbrmetphi.root")))
			       );

}
MVAMetProducerFromScratch::~MVAMetProducerFromScratch() { 
  delete fMVAMet;
}
void MVAMetProducerFromScratch::beginJob() { }

void MVAMetProducerFromScratch::endJob() { } 

void MVAMetProducerFromScratch::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {  
  //Uncorrected Jets
  Handle<PFJetCollection>       lHUCJets;
  iEvent.getByLabel(fUnCorrJetName, lHUCJets);
  PFJetCollection               lUCJets = *lHUCJets;

  //Corrected Jets
  Handle<PFJetCollection>       lHCJets;
  iEvent.getByLabel(fCorrJetName  , lHCJets);
  PFJetCollection               lCJets = *lHCJets;

  //Get pfCandidates
  Handle<PFCandidateCollection> lHCands;
  iEvent.getByLabel(fPFCandName   , lHCands);
  PFCandidateCollection         lCands = *lHCands;

  // vertices    
  Handle<reco::VertexCollection> lHVertices;
  iEvent.getByLabel(fVertexName      , lHVertices); 
  VertexCollection::const_iterator lIter = lHVertices->begin();
  Vertex lPV; if (lHVertices->size()>0) lPV = *lIter;
  VertexCollection lVertices = *lHVertices;

  //Make Generic Objects
  std::vector<LorentzVector >                   lVisible;
  std::vector<std::pair<LorentzVector,double> > lPFInfo;  makeCandidates(lPFInfo, lCands,&lPV);
  std::vector<std::pair<LorentzVector,double> > lJetInfo; makeJets      (lJetInfo,lUCJets,lCJets,lVertices);
  std::vector<Vector>                           lVtxInfo; makeVertices  (lVtxInfo,lVertices);
  
  //Dummy visible stuff
  double lDummyPt0  = 10; 
  double lDummyPhi0 = 0;
  double lDummyEta0 = 1;
  double lDummyPt1  = 10;
  double lDummyPhi1 = 2.; 
  double lDummyEta1 = -1;
  TLorentzVector lD0; lD0.SetPtEtaPhiM(lDummyPt0,lDummyPhi0,lDummyEta0,0.);
  TLorentzVector lD1; lD1.SetPtEtaPhiM(lDummyPt1,lDummyPhi1,lDummyEta1,0.);

  LorentzVector lVis0; lVis0.SetCoordinates(lD0.Px(),lD0.Py(),lD0.Pz(),lD0.E());
  LorentzVector lVis1; lVis1.SetCoordinates(lD1.Px(),lD1.Py(),lD1.Pz(),lD1.E());
  lVisible.push_back(lVis0);
  lVisible.push_back(lVis1);
  //Calculate the MVA
  std::pair<LorentzVector,double> lMVAMetInfo = fMVAMet->GetMet(lVisible,lPFInfo,lJetInfo,lVtxInfo);

  //Add a PF Met object and put it in the event
  PFMET lDummy;
  PFMET lMVAMet(lDummy.getSpecific(),lMVAMetInfo.second,lMVAMetInfo.first,lPV.position());
  std::auto_ptr<reco::PFMETCollection> pfmetcoll;
  pfmetcoll.reset     ( new reco::PFMETCollection);
  pfmetcoll->push_back( lMVAMet);
  iEvent.put( pfmetcoll );
}
void MVAMetProducerFromScratch::makeJets(std::vector<std::pair<LorentzVector,double> > &iJetInfo,PFJetCollection &iUCJets,PFJetCollection &iCJets,VertexCollection &iVertices) { 
  for(int i0   = 0; i0 < (int) iUCJets.size(); i0++) {   // uncorrecte jets collection                                                                                                        
    const PFJet       *pUCJet = &(iUCJets.at(i0));
    for(int i1 = 0; i1 < (int) iCJets .size(); i1++) {   // corrected jets collection                                                                                                   
      const PFJet     *pCJet  = &(iCJets.at(i1));
      if(       pUCJet->jetArea() != pCJet->jetArea()                  ) continue;
      if( fabs(pUCJet->eta() - pCJet->eta())         < 0.01            ) continue;
      if( pCJet->pt()                                < fJetPtMin       ) continue;
      double lJec = pCJet ->pt()/pUCJet->pt();
      double lMVA = fUtils->jetMVA(pUCJet,lJec,iVertices.at(0),iVertices);
      //LorentzVector pVec; pVec.SetCoordinates(pCJet->pt(),pCJet->eta(),pCJet->phi(),pCJet->mass());
      std::pair<LorentzVector,double>  pJetObject(pCJet->p4(),lMVA);
      iJetInfo.push_back(pJetObject);
    }
  }
}
void MVAMetProducerFromScratch::makeCandidates(std::vector<std::pair<LorentzVector,double> > &iPFInfo,PFCandidateCollection &iCands,Vertex *iPV) { 
  for(int i0 = 0; i0 < (int)iCands.size(); i0++) {
    const PFCandidate*  pflowCand = &(iCands.at(i0));
    double pDZ = -999;
    if(iPV == 0) pDZ  = fUtils->pfCandDz(pflowCand,iPV); //If there is no track return negative number -999
    //LorentzVector pVec; pVec.SetCoordinates(pflowCand->pt(),pflowCand->eta(),pflowCand->phi(),pflowCand->mass());
    std::pair<LorentzVector,double> pPFObject(pflowCand->p4(),pDZ);
    iPFInfo.push_back(pPFObject);
  }
}
void MVAMetProducerFromScratch::makeVertices(std::vector<Vector>        &iPVInfo,VertexCollection &iVertices) { 
  for(int i0    = 0; i0 < (int)iVertices.size(); i0++) {
    const Vertex       *pVertex = &(iVertices.at(i0));
    Vector pVec; pVec.SetCoordinates(pVertex->x(),pVertex->y(),pVertex->z());
    iPVInfo.push_back(pVec);
  }
}
