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
#include "AnalysisDataFormats/CMGTools/interface/Candidate.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "CMGTools/WMass/plugins/MyMetFlavorProducer.h"

using namespace edm;
using namespace std;
using namespace reco;

MyMetFlavorProducer::MyMetFlavorProducer(const edm::ParameterSet& iConfig) {
  produces<reco::PFMETCollection>();
  //fPatJetName     = iConfig.getParameter<edm::InputTag>("CorrJetName");
  fCorrJetName    = iConfig.getParameter<edm::InputTag>("CorrJetName");
  // fUnCorrJetName  = iConfig.getParameter<edm::InputTag>("JetName");
  fPFCandName     = iConfig.getParameter<edm::InputTag>("PFCandidateName");
  fVertexName     = iConfig.getParameter<edm::InputTag>("VertexName");
  fRhoName        = iConfig.getParameter<edm::InputTag>("RhoName");
  fJetPtMin       = iConfig.getParameter<double>       ("JetPtMin");
  fDZMin          = iConfig.getParameter<double>       ("dZMin");  
  fCandEtaMin     = iConfig.getParameter<double>       ("candEtaMin");  
  fCandEtaMax     = iConfig.getParameter<double>       ("candEtaMax");  
  fChargedCandPtMin      = iConfig.getParameter<double>       ("chargedCandPtMin");  
  fNeutralCandPMin       = iConfig.getParameter<double>       ("neutralCandPMin");  
  fMetFlavor      = iConfig.getParameter<int>          ("MetFlavor");
  fCandFlavor     = iConfig.getParameter<std::string>          ("candFlavor");
  fUtils          = new MetUtilities( iConfig.getParameter<unsigned>("WorkingPointId") );
  fPUJetIdAlgo         = new PileupJetIdAlgo(iConfig.getParameter<edm::ParameterSet>("PUJetId"));
  fPUJetIdAlgoLowPt    = new PileupJetIdAlgo(iConfig.getParameter<edm::ParameterSet>("PUJetIdLowPt"));
}
MyMetFlavorProducer::~MyMetFlavorProducer() { 
  delete fUtils;
}
void MyMetFlavorProducer::beginJob() { }

void MyMetFlavorProducer::endJob() { } 

void MyMetFlavorProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {  
  //Uncorrected Jets                                                                                                                                                                                            
//   Handle<PFJetCollection>       lHUCJets;
//   iEvent.getByLabel(fUnCorrJetName, lHUCJets);
//   PFJetCollection               lUCJets = *lHUCJets;

  //Corrected Jets                                                                                                                                                                                              
  Handle< std::vector<cmg::PFJet> >   lHCJets;
  iEvent.getByLabel(fCorrJetName  , lHCJets);
  const std::vector<cmg::PFJet>&      lCJets = *lHCJets;

  //Pat Jets
  //Handle<PFJetCollection>       lHCJets;
  //iEvent.getByLabel(fCorrJetName  , lHCJets);
  //PFJetCollection               lCJets = *lHCJets;

  //Get pfCandidates
  Handle<std::vector<cmg::Candidate> > lHCands;
  iEvent.getByLabel(fPFCandName   , lHCands);
  const std::vector<cmg::Candidate>&         lCands = *lHCands;
  // cmg::Candidate
  
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
  std::vector<LorentzVector >                    lVisible;
  std::vector<std::pair<LorentzVector,double> >  lPFInfo;  makeCandidates(lPFInfo, lCands,lPV, fCandFlavor,fCandEtaMin,fCandEtaMax,fChargedCandPtMin,fNeutralCandPMin);
  std::vector<MetUtilities::JetInfo>             lJetInfo; makeJets      (lJetInfo, lCJets,lVertices,lRho);
  // std::vector<Vector>                                             lVtxInfo; makeVertices  (lVtxInfo,lVertices);
 
  // //Calculate the Met
  std::pair<LorentzVector,double> lMetInfo; //MET, SumEt
  if(fMetFlavor == 0)   lMetInfo = fUtils->TKMet  (lPFInfo,fDZMin,2);
  if(fMetFlavor == 1)   lMetInfo = fUtils->TKMet  (lPFInfo,fDZMin,0);
  // if(fMetFlavor == 2)   lMetInfo = fUtils->NoPUMet(lPFInfo,lJetInfo,fDZMin);
  // if(fMetFlavor == 3)   lMetInfo = fUtils->PUMet  (lPFInfo,lJetInfo,fDZMin);//0.2 for DZ
  // if(fMetFlavor == 4)   lMetInfo = fUtils->PUCMet (lPFInfo,lJetInfo,fDZMin);

  //Add a PF Met object and put it in the event
  PFMET lDummy;
  PFMET lMet(lDummy.getSpecific(),lMetInfo.second,lMetInfo.first,lPV->position()); //Use PFMET sum Et

  std::auto_ptr<reco::PFMETCollection> lPFMetColl;
  lPFMetColl.reset     ( new reco::PFMETCollection);
  lPFMetColl->push_back( lMet);
  iEvent.put( lPFMetColl );
}

void MyMetFlavorProducer::makeJets(std::vector<MetUtilities::JetInfo> &iJetInfo,
				 const std::vector<cmg::PFJet>& iCJets,
				 const VertexCollection &iVertices,double iRho) { 
  
  for(int i1 = 0; i1 < (int) iCJets .size(); i1++) {   // corrected jets collection                                         
    const cmg::PFJet     *pCJet  = &(iCJets.at(i1));

    // //std::cout<<" MyMetFlavorProducer::makeJets  input :  "<<i1<<"  "<<pCJet->pt()<<" "<<pCJet->eta()<<" "<<pCJet->correctedJet(0).pt()<<" "<<pCJet->neutralEmEnergy()<<" "<<pCJet->neutralHadronEnergy()<<std::endl;
    
    if(pCJet->pt() < fJetPtMin) continue;

    // if( !passPFLooseId(pCJet) ) continue;

    // //std::cout<<" MyMetFlavorProducer::makeJets  PFLooseId :  "<<i1<<"  "<<pCJet->pt()<<" "<<pCJet->eta()<<" "<<pCJet->correctedJet(0).pt()<<" "<<pCJet->neutralEmEnergy()<<" "<<pCJet->neutralHadronEnergy()<<std::endl;

    // double lJec = 0;
    // double lMVA = jetMVA(pCJet,lJec,iVertices[0],iVertices,false);
    // double lJetEnergy = pCJet->correctedJet(0).pt()/pCJet->pt()*pCJet->energy();
    // // COLIN 53 
    // double lNeuFrac = 1.;
    // if (fabs(pCJet->eta())<2.5)
      // lNeuFrac = (pCJet->neutralEmEnergy() + pCJet->neutralHadronEnergy())/lJetEnergy;
    // // Old 52 recipe:
    // // double lNeuFrac = (pCJet->neutralEmEnergy() + pCJet->neutralHadronEnergy())/lJetEnergy;
    // MetUtilities::JetInfo pJetObject; 
    // pJetObject.p4       = pCJet->p4(); 
    // pJetObject.mva      = lMVA;
    // pJetObject.neutFrac = lNeuFrac;
    // //std::cout<<" MyMetFlavorProducer::makeJets  final:  "<<i1<<"  "<<pJetObject.p4.pt()<<" "<<pJetObject.mva<<" "<<pJetObject.neutFrac<<std::endl;
    // iJetInfo.push_back(pJetObject);
  }
}

void MyMetFlavorProducer::makeCandidates(std::vector<std::pair<LorentzVector,double> > &iPFInfo, 
                                         const std::vector<cmg::Candidate> &iCands,const Vertex *iPV, std::string candFlavor,double iEtaMin,double iEtaMax,
                                         double iPtMin, double iPMin) { 

  // PF CANDIDATE FLAVORS
  // https://cmssdt.cern.ch/SDT/doxygen/CMSSW_4_4_2/doc/html/dc/d55/classreco_1_1PFCandidate.html#af39a4e9ae718041649773fa7ca0919bc
  // 0: X 	
  // 1: h 	
  // 2: e 	
  // 3: mu 	
  // 4: gamma 	
  // 5: h0 	
  // 6: h_HF 	
  // 7: egamma_HF 	
  
  // switch (std::abs(pdgid)) {
  // case 211: return h;
  // case 11: return e;
  // case 13: return mu;
  // case 22: return gamma;
  // case 130: return h0;
  // case 1: return h_HF;
  // case 2: return egamma_HF;
  // case 0: return X;  
  // default: return X;

  const int pfcandTypes=8;
  TString      pfcandName[pfcandTypes]={"X",  "h", "e","mu","gamma","h0","h_HF","egamma_HF"};
  unsigned int pfcandPdg[pfcandTypes]={   0,  211,  11,  13,     22, 130,     1,          2};
  std::vector<unsigned int> flavors;
  TString candFlavor_str = candFlavor.c_str();
  
  // cout << "candFlavor_str= " << candFlavor_str << endl;
  if(candFlavor_str!=""){
    // cout << "REQUESTED MET WILL BE BUILT WITH " << candFlavor.c_str() << " PF CANDIDATES" <<endl;
    
    TObjArray* candFlavors = candFlavor_str.Tokenize(",");
    
    for(int j=0;j<candFlavors->GetEntriesFast();j++){
      TString tokenized = ((TObjString *)candFlavors->At(j))->GetString();
      // int j=0;
      for(int i=0;i<pfcandTypes;i++){
        if(tokenized==pfcandName[i]){
          flavors.push_back(i);
          // cout << "j= " << j << " i= " << i << " tokenized= " << tokenized << " pfcandName= " << pfcandName[i] << " flavors["<<j<<"]= "<< flavors[j] << endl;
          break;
        }
        // j++;
      }
        
    }
    
    // for(unsigned int j=0;j<flavors.size();j++){
      // cout << "flavors["<<j<<"]= " << flavors[j] << " " << pfcandName[flavors[j]] << " pfcandPdg[flavors["<<j<<"]]= " << pfcandPdg[flavors[j]]<< endl;
    // }

  }
  
  
  for(int i0 = 0; i0 < (int)iCands.size(); i0++) {
    const cmg::Candidate*  pflowCand = &(iCands.at(i0));
    
    // cout << "\ncand "<<i0<<"/"<<(int)iCands.size() << " pdgId= " <<pflowCand->pdgId()<< " p= " <<pflowCand->p() << " pt= " <<pflowCand->pt() << " eta= " <<pflowCand->eta();
    
    // cout << " " << candFlavor_str;
    // if(pflowCand->charge()!=0) cout << " charge!=0 and pT>"<< iPtMin;
    // else if(pflowCand->charge()==0) cout << " charge=0 and p>"<< iPMin;
    // cout << " " << iEtaMin << "<pflowCand->eta()<"<<iEtaMax;

    
    // cout << "pflowCand->charge() " << pflowCand->charge() << endl;
    
    if((pflowCand->charge()!=0 || TMath::Abs(pflowCand->pdgId()==22))&& pflowCand->pt()<iPtMin) continue;
    else if(pflowCand->charge()==0 && pflowCand->p()<iPMin) continue;
    
    if(
      //pflowCand->pt()<iPtMin
      // || pflowCand->p()<iPMin
      // || TMath::Abs(pflowCand->eta())<iEtaMin // ETA SIGN BLIND
      // || TMath::Abs(pflowCand->eta())>iEtaMax // ETA SIGN BLIND
      // || 
      (pflowCand->eta())<iEtaMin || (pflowCand->eta())>iEtaMax
      ) continue;
    
    // cout << " passed acceptance! ";
    
    
    if(flavors.size()>0){
      bool matched=false;
      // for(int i=0; i<pfcandTypes; i++){
        // unsigned int particleId=pflowCand->pdgId();
        // if(particleId==flavors[i]){ 
          // matched=true;
          // // cout << " particleId= " << particleId << " flavors["<<i<<"]= " << flavors[i] << " matched= " << matched << " " << endl;
          // break;
        // }
      // }
      for(unsigned int i=0; i<flavors.size(); i++){
        unsigned int particleId=TMath::Abs(pflowCand->pdgId());
        if(particleId==pfcandPdg[flavors[i]]){ 
          matched=true;
          // cout << " pfcandPdg[flavors["<<i<<"]]= " << pfcandPdg[flavors[i]] << " matched= " << matched << " " << endl;
          break;
        }
      }
      if(!matched) continue;
    }
    
    // cout << " passed flavor " << candFlavor_str << " --> using it";
    
    double pDZ = -999;
    if(pflowCand->fromPV()) pDZ  = pfCandDz(pflowCand,iPV); //If there is no track return negative number -999
    //LorentzVector pVec; pVec.SetCoordinates(pflowCand->pt(),pflowCand->eta(),pflowCand->phi(),pflowCand->mass());
    // cout << "pflowCand->pt()= " << pflowCand->p4().Pt() << " pflowCand->eta()= " << pflowCand->p4().Eta() << " pDZ= " << pDZ << endl;
    TLorentzVector icand; 
    icand.SetPtEtaPhiM(pflowCand->pt(),pflowCand->eta(),pflowCand->phi(),pflowCand->mass());
    LorentzVector icand2; icand2.SetPxPyPzE(icand.Px(),icand.Py(),icand.Pz(),icand.E());
    // cout << " " << icand2.Px() << " "<< icand2.Py() << " "<< icand2.Pz() << " "<< icand2.E() << " ";
    std::pair<LorentzVector,double> pPFObject(icand2,pDZ);
    iPFInfo.push_back(pPFObject);
  }
  // cout << "\nstarting= " << (int)iCands.size() << " used= " << iPFInfo.size() << endl;
}

void MyMetFlavorProducer::makeVertices(std::vector<Vector>        &iPVInfo,const VertexCollection &iVertices) { 
  for(int i0    = 0; i0 < (int)iVertices.size(); i0++) {
    const Vertex       *pVertex = &(iVertices.at(i0));

    if(fabs(pVertex->z())           > 24.) continue;
    if(pVertex->ndof()              <  4.) continue;
    if(pVertex->position().Rho()    >  2.) continue;

    Vector pVec; pVec.SetCoordinates(pVertex->x(),pVertex->y(),pVertex->z());
    iPVInfo.push_back(pVec);
  }
}
bool MyMetFlavorProducer::passPFLooseId(const cmg::PFJet *iJet) { 


  // //std::cout << " ==> " << iJet->pt() << " - " << iJet->eta() << " : " << iJet->energy() << " - " << iJet->neutralEmEnergy() << " - " << iJet->nConstituents() << " - " << iJet->chargedHadronEnergy() << " _ " << iJet->chargedEmEnergy() << " - " << iJet->chargedMultiplicity() << std::endl;
  // //if(iJet->energy()== 0)                                                           return false;

  // double lJetEnergy = iJet->correctedJet(0).pt()/iJet->pt()*iJet->energy();
  // //if((iJet->neutralHadronEnergy()+iJet->HFHadronEnergy())/lJetEnergy > 0.99)      return false;//Jose: this is the correct PFLoose id 
  // if((iJet->neutralHadronEnergy())/lJetEnergy > 0.99)      return false;//Jose: HF energy is not used in the MVA Met training
  // if(iJet->neutralEmEnergy()/lJetEnergy     > 0.99)                               return false;
  // if(iJet->nConstituents()                  <  2)                                 return false;
  // if(iJet->chargedHadronEnergy()/lJetEnergy <= 0     && fabs(iJet->eta()) < 2.4 ) return false;
  // if(iJet->chargedEmEnergy()/lJetEnergy     >  0.99  && fabs(iJet->eta()) < 2.4 ) return false;
  // if(iJet->chargedMultiplicity()            < 1      && fabs(iJet->eta()) < 2.4 ) return false;
  return true;
}
double MyMetFlavorProducer::pfCandDz(const cmg::Candidate* iPFCand, const Vertex *iPV) { 
  double lDz = -999;
  // if(iPFCand->trackRef().isNonnull())    lDz = fabs(iPFCand->   trackRef()->dz(iPV->position()));
  // else if(iPFCand->gsfTrackRef().isNonnull()) lDz = fabs(iPFCand->gsfTrackRef()->dz(iPV->position()));
  if(iPFCand->fromPV())    lDz = fabs(iPFCand->vertex().z() - iPV->position().z());
  
  return lDz;
}
double MyMetFlavorProducer::jetMVA (const Jet *iCorrJet,double iJec, const Vertex iPV, const reco::VertexCollection &iAllvtx,bool iPrintDebug) { 
  PileupJetIdentifier lPUJetId     =  fPUJetIdAlgo->computeIdVariables(iCorrJet,iJec,&iPV,iAllvtx,true);
  PileupJetIdentifier *lPUJetIdRef =  &lPUJetId;
  if(iCorrJet->pt() < 10) {
    PileupJetIdentifier pPUJetId   =  fPUJetIdAlgoLowPt->computeIdVariables(iCorrJet,iJec,&iPV,iAllvtx,true);
    lPUJetIdRef = &pPUJetId;
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

DEFINE_FWK_MODULE (MyMetFlavorProducer);
