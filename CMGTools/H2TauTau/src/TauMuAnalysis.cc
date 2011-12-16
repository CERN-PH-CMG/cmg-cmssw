#include "CMGTools/H2TauTau/interface/TauMuAnalysis.h"
#include <THStack.h>
#include <TString.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TLegend.h>
#include <TLine.h>

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Run.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/SVFitStandAlone/interface/NSVfitStandaloneAlgorithm.h"

#include <TVectorD.h>

TauMuAnalysis::TauMuAnalysis():
  BaseAnalysis(),
  inputTag_(""),
  diLeptonVetoListName_(""),
  calcsvfit_(0),
  makeAllHistos_(1),
  deltaRTruth_(0.2),
  QCDOStoSSRatio_(0),
  QCDColor_(kMagenta-10)
{
}

TauMuAnalysis::TauMuAnalysis(const char * name):
  BaseAnalysis(name),
  inputTag_(""),
  diLeptonVetoListName_(""),
  calcsvfit_(0),
  makeAllHistos_(1),
  deltaRTruth_(0.2),
  QCDOStoSSRatio_(0),
  QCDColor_(kMagenta-10)
{
 
}

TauMuAnalysis::~TauMuAnalysis(){
}


bool TauMuAnalysis::init(){


  if(!BaseAnalysis::init())return 0;

  std::vector<Sample*>::const_iterator s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){    
    cout<<"TauMuAnalysis  Initialized sample "<<samples_[i]->GetName()<<" "<<samples_[i]->GetTitle()<<endl;
  }
  
  return 1;
}

bool TauMuAnalysis::addHistos(Sample* s){
  if(!s) return 0;
  
  if(!BaseAnalysis::addHistos(s))return 0;
  
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauMassHisto","",1000,0,1000))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauMassSVFitHisto","",1000,0,1000))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauEtaHisto","",120,-6,6))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauPtHisto","",400,0,200))))return 0; 

  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muPtHisto","",400,0,200))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muEtaHisto","",120,-3,3))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muIsoHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muDxyHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muDzHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muTruthDeltaRHisto","",500,0,5.0))))return 0;

  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauPtHisto","",400,0,200))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauEtaHisto","",120,-3,3))))return 0;  
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauIsoHisto","",100,0,5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauDxyHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauDzHisto","",100,0,.5))))return 0;  
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauEoPHisto","",500,0.,5.0))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauEoP1Histo","",200,0.999,1.001))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauEtaEoP1Histo","",120,-3,3))))return 0;  
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauPtEoP1Histo","",400,0,200))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauHoPHisto","",500,0.,5.0))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauEHoPHisto","",500,0.,5.0))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauTruthDeltaRHisto","",500,0,5.0))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauTruthHisto","",10,0,10))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauDecayModeHisto","",10,0,10))))return 0;

  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_metHisto","",200,0,200))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_transverseMassHisto","",200,0,200))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_metphiHisto","",64,-3.2,3.2))))return 0;

  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_njetHisto","",20,-0.5,19.5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_njetLCHisto","",20,-0.5,19.5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_leadJetPtHisto","",500,0,500))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_leadJetEtaHisto","",100,-5,5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diJetMassHisto","",1000,0,1000))))return 0;    
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diJetDeltaEtaHisto","",200,-10,10))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diJetEta1Eta2Histo","",60,-30,30))))return 0;

  return 1;
}


bool TauMuAnalysis::getHistos(TString tag){
  if(!sample_) return 0;

  if(tag!="")tag=TString("_")+tag;

  if(!(diTauMassHisto_=(TH1F*)(sample_->getHisto(TString("diTauMassHisto")+tag))))return 0;
  if(!(diTauMassSVFitHisto_=(TH1F*)(sample_->getHisto(TString("diTauMassSVFitHisto")+tag))))return 0;
  if(!(diTauEtaHisto_=(TH1F*)(sample_->getHisto(TString("diTauEtaHisto")+tag))))return 0;
  if(!(diTauPtHisto_=(TH1F*)(sample_->getHisto(TString("diTauPtHisto")+tag))))return 0;

  if(!(muPtHisto_=(TH1F*)(sample_->getHisto(TString("muPtHisto")+tag))))return 0;
  if(!(muEtaHisto_=(TH1F*)(sample_->getHisto(TString("muEtaHisto")+tag))))return 0;
  if(!(muIsoHisto_=(TH1F*)(sample_->getHisto(TString("muIsoHisto")+tag))))return 0;
  if(!(muDxyHisto_=(TH1F*)(sample_->getHisto(TString("muDxyHisto")+tag))))return 0;
  if(!(muDzHisto_=(TH1F*)(sample_->getHisto(TString("muDzHisto")+tag))))return 0;
  if(!(muTruthDeltaRHisto_=(TH1F*)(sample_->getHisto(TString("muTruthDeltaRHisto")+tag))))return 0;

  if(!(tauPtHisto_=(TH1F*)(sample_->getHisto(TString("tauPtHisto")+tag))))return 0;
  if(!(tauEtaHisto_=(TH1F*)(sample_->getHisto(TString("tauEtaHisto")+tag))))return 0;
  if(!(tauIsoHisto_=(TH1F*)(sample_->getHisto(TString("tauIsoHisto")+tag))))return 0;
  if(!(tauDxyHisto_=(TH1F*)(sample_->getHisto(TString("tauDxyHisto")+tag))))return 0;
  if(!(tauDzHisto_=(TH1F*)(sample_->getHisto(TString("tauDzHisto")+tag))))return 0;
  if(!(tauEoPHisto_=(TH1F*)(sample_->getHisto(TString("tauEoPHisto")+tag))))return 0;
  if(!(tauEoP1Histo_=(TH1F*)(sample_->getHisto(TString("tauEoP1Histo")+tag))))return 0;
  if(!(tauEtaEoP1Histo_=(TH1F*)(sample_->getHisto(TString("tauEtaEoP1Histo")+tag))))return 0;
  if(!(tauPtEoP1Histo_=(TH1F*)(sample_->getHisto(TString("tauPtEoP1Histo")+tag))))return 0;
  if(!(tauHoPHisto_=(TH1F*)(sample_->getHisto(TString("tauHoPHisto")+tag))))return 0;
  if(!(tauEHoPHisto_=(TH1F*)(sample_->getHisto(TString("tauEHoPHisto")+tag))))return 0;
  if(!(tauTruthDeltaRHisto_=(TH1F*)(sample_->getHisto(TString("tauTruthDeltaRHisto")+tag))))return 0;
  if(!(tauTruthHisto_=(TH1F*)(sample_->getHisto(TString("tauTruthHisto")+tag))))return 0;
  if(!(tauDecayModeHisto_=(TH1F*)(sample_->getHisto(TString("tauDecayModeHisto")+tag))))return 0;

  if(!(metHisto_=(TH1F*)(sample_->getHisto(TString("metHisto")+tag))))return 0;
  if(!(metphiHisto_=(TH1F*)(sample_->getHisto(TString("metphiHisto")+tag))))return 0;
  if(!(transverseMassHisto_=(TH1F*)(sample_->getHisto(TString("transverseMassHisto")+tag))))return 0;

  if(!(njetHisto_=(TH1F*)(sample_->getHisto(TString("njetHisto")+tag))))return 0;
  if(!(njetLCHisto_=(TH1F*)(sample_->getHisto(TString("njetLCHisto")+tag))))return 0;
  if(!(leadJetPtHisto_=(TH1F*)(sample_->getHisto(TString("leadJetPtHisto")+tag))))return 0;
  if(!(leadJetEtaHisto_=(TH1F*)(sample_->getHisto(TString("leadJetEtaHisto")+tag))))return 0;
  if(!(diJetMassHisto_=(TH1F*)(sample_->getHisto(TString("diJetMassHisto")+tag))))return 0;
  if(!(diJetDeltaEtaHisto_=(TH1F*)(sample_->getHisto(TString("diJetDeltaEtaHisto")+tag))))return 0;
  if(!(diJetEta1Eta2Histo_=(TH1F*)(sample_->getHisto(TString("diJetEta1Eta2Histo")+tag))))return 0;

  return 1;
}


void TauMuAnalysis::fillPFJetListLC(const cmg::TauMu * cand){
  pfJetListLC_.clear();

  //lepton clean the Jets
  std::vector<cmg::PFJet> jetlist;
  for(std::vector<cmg::PFJet>::const_iterator jet=pfJetList_.begin(); jet!=pfJetList_.end(); ++jet){
    if(reco::deltaR(jet->eta(),jet->phi(),cand->leg1().eta(),cand->leg1().phi())<0.5) continue;     
    if(reco::deltaR(jet->eta(),jet->phi(),cand->leg2().eta(),cand->leg2().phi())<0.5) continue;         
    jetlist.push_back(*jet);
  }

  //order by pt
  const cmg::PFJet * lastjet=NULL;
  for(std::vector<cmg::PFJet>::const_iterator jet1=jetlist.begin(); jet1!=jetlist.end(); ++jet1){
    const cmg::PFJet * highjet=NULL;
    for(std::vector<cmg::PFJet>::const_iterator jet2=jetlist.begin(); jet2!=jetlist.end(); ++jet2){
      if(lastjet){
	if(jet2->pt()<lastjet->pt()){
	  if(highjet){
	    if(jet2->pt()>highjet->pt())highjet=&(*jet2);
	  }else highjet=&(*jet2);
	}
      }else{
	if(highjet){
	  if(jet2->pt()>highjet->pt())highjet=&(*jet2);
	}else highjet=&(*jet2);
      }    
    }  
    pfJetListLC_.push_back(*highjet);
    lastjet=highjet;
  }
  
}

bool TauMuAnalysis::computeDiLeptonVeto(){
  
  int nmuons=0;
  for(std::vector<cmg::Muon>::const_iterator m=diLeptonVetoList_->begin(); m!=diLeptonVetoList_->end(); ++m){  
    if(m->pt()<15.0)continue;
    if(fabs(m->eta())>2.5)continue;
    if(m->relIso(0.5)>0.3)continue;    
    if(m->isGlobal()<0.5)continue; 
    if(m->dxy()>0.45)continue; 
    if(m->dz()>0.2)continue; 
    nmuons++;
  }
    
  return nmuons>=2;
}


bool TauMuAnalysis::fillVariables(const fwlite::Event * event){

  //event level variables
  if(!BaseAnalysis::fillVariables(event))return 0;
  
  //embedded samples generator weight
  embeddedGenWeight_=1.0;
  if(sample_->getDataType()=="Embedded" || sample_->getDataType()=="Embedded_SS"){
    edm::Handle< double > embeddedGenWeight;
    event->getByLabel(edm::InputTag("generator","weight",""),embeddedGenWeight);
    embeddedGenWeight_=*embeddedGenWeight;
  }  
 
  ///get the TauMu cands 
  event->getByLabel(edm::InputTag(inputTag_.c_str()),diTauList_);

  //get the muons for the di-lepton veto
  event->getByLabel(edm::InputTag(diLeptonVetoListName_.c_str()),diLeptonVetoList_);
  
  ///get the gen Boson and set the genEventType
  genBoson_ = NULL;
  genBosonL1_ = NULL;
  genBosonL2_ = NULL;
  genEventType_=0;
  if(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS" || sample_->getDataType()=="Signal"){  
    event->getByLabel(edm::InputTag("genParticlesStatus3"),genParticles_);    
    for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
      //cout<<g->pdgId()<<" "<<g->p4().pt()<<endl;
      if((abs(g->pdgId())==23 || abs(g->pdgId())==24) && genBoson_==NULL )
	genBoson_=&(*g);
    }
    //if(genBoson_)cout<<"genBoson_ ref = "<<genBoson_<<endl;

    if(genBoson_){      
      //determine type of generated event: Z-->ee,mumu,tautau,, W-->e nu, mu nu, tau mu
      int genTaus=0;
      int genMuons=0;
      int genElectrons=0;
      for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
	if(abs(g->pdgId())==11 && g->mother()==genBoson_) genElectrons++;
	if(abs(g->pdgId())==13 && g->mother()==genBoson_) genMuons++;
	if(abs(g->pdgId())==15 && g->mother()==genBoson_) genTaus++;
      }
      if(abs(genBoson_->pdgId())==23 && genElectrons==2)   genEventType_=1;
      if(abs(genBoson_->pdgId())==23 && genMuons==2)       genEventType_=3;
      if(abs(genBoson_->pdgId())==23 && genTaus==2)        genEventType_=5;
      if(abs(genBoson_->pdgId())==24 && genElectrons==1)   genEventType_=11;
      if(abs(genBoson_->pdgId())==24 && genMuons==1)       genEventType_=13;
      if(abs(genBoson_->pdgId())==24 && genTaus==1)        genEventType_=15;

      //get the leptons from the genBoson
      for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
	if((g->pdgId()==11 || g->pdgId()==13 || g->pdgId()==15 ) && g->mother()==genBoson_) genBosonL1_=&(*g);
	if((g->pdgId()==-11 || g->pdgId()==-13 || g->pdgId()==-15 ) && g->mother()==genBoson_) genBosonL2_=&(*g);
      }      

    }

  }


  //fill the jet list with basic selections, lepton cleaning done later for each TauMu candidate
  pfJetList_.clear();
  edm::Handle< std::vector<cmg::PFJet> > jetlist;
  event->getByLabel(edm::InputTag("cmgPFJetSel"),jetlist);
  for(std::vector<cmg::PFJet>::const_iterator jet=jetlist->begin(); jet!=jetlist->end(); ++jet){
    if(jet->pt()<30.0)continue;  
    if(fabs(jet->eta())>4.5)continue;        
    pfJetList_.push_back(*jet);
  }
  

  
  return 1;
}

int TauMuAnalysis::truthMatchTau(){
  if(!diTauSel_ )return 0;
  if(!(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS" || sample_->getDataType()=="Signal")) return 0;

  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if(abs(g->pdgId())==11) if(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),g->eta(),g->phi())<deltaRTruth_) return 1;
    if(abs(g->pdgId())==13) if(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),g->eta(),g->phi())<deltaRTruth_) return 3;
    if(abs(g->pdgId())==15) if(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),g->eta(),g->phi())<deltaRTruth_) return 5;
  }
  

  return 9;
}


bool TauMuAnalysis::applySelections(TString exceptcut){

  //if none are selected returns 0
  diTauSel_=NULL;
  
  //separate the Z->ll at gen level here, will speed up processing
  //cand be done because a reconstruced tau-tau candidate can only be truth-matched if there is actually a generated Z-->tau tau, for example
  if(sample_->getTruthEventType()==1 || sample_->getTruthEventType()==3 || sample_->getTruthEventType()==5
     || sample_->getTruthEventType()==11 || sample_->getTruthEventType()==13 || sample_->getTruthEventType()==15)
    if(sample_->getTruthEventType()!=genEventType_) return 0;

  ////other 
  if(exceptcut!="dileptonveto") if(computeDiLeptonVeto()) return 0;

  //apply full selections on Tau and Mu cands
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=diTauList_->begin(); cand!=diTauList_->end(); ++cand){    
      
    ////selections on the tau
    if(exceptcut!="taupt") if(cand->leg1().pt()<20.0)continue;
    if(exceptcut!="taueta") if(fabs(cand->leg1().eta())>2.3)continue;
    if(exceptcut!="tauagainstelectron") if(cand->leg1().tauID("againstElectronLoose")<0.5) continue;
    if(exceptcut!="tauagainstmuon") if(cand->leg1().tauID("againstMuonTight")<0.5) continue;
    if(exceptcut!="tauiso") if(cand->leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr")<0.5) continue; 
    if(exceptcut!="taueop") 
      if(cand->leg1().decayMode()==0&&cand->leg1().p()>0.) 
	if(((cand->leg1().leadChargedHadrECalEnergy()+cand->leg1().leadChargedHadrHCalEnergy())/cand->leg1().p())<0.2) continue; 
        
    ////selections on the muon
    if(exceptcut!="mupt") if(cand->leg2().pt()<17.0)continue;
    if(exceptcut!="mueta") if(fabs(cand->leg2().eta())>2.1)continue;
    if(exceptcut!="muid")
      if(cand->leg2().dxy() > 0.045
	 || cand->leg2().dz() > 0.2
	 || cand->leg2().isGlobal() < 0.5 
	 || cand->leg2().isTracker() < 0.5
	 || cand->leg2().numberOfValidTrackerHits() <= 10
	 || cand->leg2().numberOfValidPixelHits() == 0
	 || cand->leg2().numberOfValidMuonHits() == 0
	 || cand->leg2().numberOfMatches() <= 1
	 || cand->leg2().normalizedChi2() >= 10
	 ) continue;    
    if(exceptcut!="muiso") if(cand->leg2().relIso(0.5)>0.1)continue;    

    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0) return 0;

  //choose the best candidate
  diTauSel_=&(*diTauSelList_.begin());
  double highsumpt=diTauSel_->leg1().pt()+diTauSel_->leg2().pt();
  for(std::vector<cmg::TauMu>::const_iterator cand=diTauSelList_.begin(); cand!=diTauSelList_.end(); ++cand)
    if(cand->leg1().pt()+cand->leg2().pt()>highsumpt){
      diTauSel_=&(*cand);
      highsumpt=diTauSel_->leg1().pt()+diTauSel_->leg2().pt();
    }

  //determine the jets
  fillPFJetListLC(diTauSel_);
  
  ///apply selections on di-tau cands here
  if(exceptcut!="massT") if(diTauSel_->mTLeg2()>40.0) return 0;
  
  ///SS or OS separation 
  if(sample_->getDataType()=="MC_SS" || sample_->getDataType()=="Signal_SS" || sample_->getDataType()=="Data_SS" || sample_->getDataType()=="Embedded_SS"){
    if(fabs(diTauSel_->charge())!=2.)return 0;
  }else if(sample_->getDataType()=="MC" || sample_->getDataType()=="Signal" || sample_->getDataType()=="Data" || sample_->getDataType()=="Embedded"){
    if(diTauSel_->charge()!=0.)return 0;
  }else{
    cout<<sample_->GetName()<<" Unrecognized data type"<<endl;
    return 0;
  }


  //truth match the reconstructed candidates:
  truthEventType_=0;
  if(genBoson_){
    //Z-->ll
    if(abs(genBoson_->pdgId())==23){
      if(genBosonL1_&&genBosonL2_){
	if(((reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),genBosonL1_->eta(),genBosonL1_->phi())<deltaRTruth_)
	    ||(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),genBosonL2_->eta(),genBosonL2_->phi())<deltaRTruth_))
	   && ((reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL1_->eta(),genBosonL1_->phi())<deltaRTruth_)
	       ||(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL2_->eta(),genBosonL2_->phi())<deltaRTruth_))
	   ) truthEventType_=abs(genBosonL1_->pdgId())-10;
	else truthEventType_=6;
      }else truthEventType_=6;
    }

    //W-->lnu
    if(abs(genBoson_->pdgId())==24){
      if(genBosonL1_){
	if(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL1_->eta(),genBosonL1_->phi())<deltaRTruth_)
	  truthEventType_=abs(genBosonL1_->pdgId());
	else truthEventType_=16;
      }else if(genBosonL2_){	  
	if(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL2_->eta(),genBosonL2_->phi())<deltaRTruth_)
	  truthEventType_=abs(genBosonL2_->pdgId());
	else truthEventType_=16;
      }else truthEventType_=16;
    }
  }


  //separate DYToLL here
  if(sample_->getTruthEventType()>0)
    if(sample_->getTruthEventType()!=truthEventType_) return 0;



  //Define SM event category:
  eventCategorySM_=9;//inclusive
  if(pfJetListLC_.size()>=2){//VBF: two leading jets must have  m>400, |eta1-eta2| < 4 and no other jet high pt in between    
    if((pfJetListLC_[0].p4()+pfJetListLC_[1].p4()).mass() > 400.0 
       && fabs(pfJetListLC_[0].eta() - pfJetListLC_[1].eta()) > 4.0 
       && pfJetListLC_[0].eta()*pfJetListLC_[1].eta() < 0.0
       ){
      bool pass=1;
      if(pfJetListLC_.size()>2){// check there is no additional central jet
	for(std::vector<cmg::PFJet>::const_iterator jet3=pfJetListLC_.begin(); jet3!=pfJetListLC_.end(); ++jet3){
	  if(pfJetListLC_[0].eta()<pfJetListLC_[1].eta()) 
	    if(pfJetListLC_[0].eta()<jet3->eta()&&jet3->eta()<pfJetListLC_[1].eta()) pass=0;
	  if(pfJetListLC_[0].eta()>pfJetListLC_[1].eta()) 
	    if(pfJetListLC_[1].eta()<jet3->eta()&&jet3->eta()<pfJetListLC_[0].eta()) pass=0;
	}
      }
      if(pass) eventCategorySM_=2;
    }
  }
  if(eventCategorySM_!=2&&pfJetListLC_.size()>=1){//Boosted: 1 jet with pt>150 and no other jets
    if(pfJetListLC_.begin()->pt()>=150.0)
      eventCategorySM_=1;
  }
  if(eventCategorySM_!=2&&eventCategorySM_!=1&&pfJetListLC_.size()<=1){//inclusive 
    if(pfJetListLC_.size()==1){
      if(pfJetListLC_.begin()->pt()<150.0)
	eventCategorySM_=0;
    }else eventCategorySM_=0;
  }


  return 1;
}

bool TauMuAnalysis::fillHistos(TString tag){
  
  if(!diTauSel_){
    cout<<" diTauSel_ is NULL"<<endl; return 0;
  }

  sample_->incrementCounter(tag);//fill counter
  
  //tau fake rate reweight
  tauFakeWeight_=1.;//do not comment out needs to be used
  //   if(sample_->getApplyTauRateWeight()){
  //     tauFakeWeight_=tauRate_.getScale(TauRate::Inclusive,diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
  //     if(diTauSel_->leg1().decayMode()==0)tauFakeWeight_=tauRate_.getScale(TauRate::OneProng,diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
  //     if(diTauSel_->leg1().decayMode()==1)tauFakeWeight_=tauRate_.getScale(TauRate::OneProngEM,diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
  //     if(diTauSel_->leg1().decayMode()==2)tauFakeWeight_=tauRate_.getScale(TauRate::OneProngEM,diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
  //     if(diTauSel_->leg1().decayMode()==10)tauFakeWeight_=tauRate_.getScale(TauRate::ThreeProng,diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
  //   }


  /////trigger efficiency weight
  triggerEffWeight_=1.;
  if(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS" 
     || sample_->getDataType()=="Signal" || sample_->getDataType()=="Signal_SS" 
     || sample_->getDataType()=="Embedded" || sample_->getDataType()=="Embedded_SS"){
    triggerEffWeight_ *= triggerEff_.effTau2011AB(diTauSel_->leg1().pt());
    triggerEffWeight_ *= triggerEff_.effMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
    //triggerEffWeight_ *= triggerEff_.effTau2011B(diTauSel_->leg1().pt());
    //triggerEffWeight_ *= triggerEff_.effMu2011B(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
  }
  
  //total event weight
  eventWeight_ = pupWeight_*embeddedGenWeight_*triggerEffWeight_;//*tauFakeWeight_

  //event histos
  if(!BaseAnalysis::fillHistos(tag)) return 0;

  //di-tau histos
  if(!TauMuAnalysis::getHistos(tag)) return 0;

  tree_ditaumass_=diTauSel_->mass();

  tree_mupt_=diTauSel_->leg2().pt();
  tree_mueta_=diTauSel_->leg2().eta();
  tree_muiso_=diTauSel_->leg2().relIso(0.5);

  tree_taupt_=diTauSel_->leg1().pt();
  tree_taueta_=diTauSel_->leg1().eta();
  tree_tautruth_=truthMatchTau();
  tree_tauehop_=diTauSel_->leg1().eOverP();
  tree_taueop_=diTauSel_->leg1().leadChargedHadrECalEnergy()/diTauSel_->leg1().p();
  tree_taudecaymode_=diTauSel_->leg1().decayMode();

  tree_transversemass_=diTauSel_->mTLeg2();
  tree_met_=diTauSel_->met().pt();
  tree_svfitmass_=diTauSel_->massSVFit();

  diTauMassHisto_->Fill(tree_ditaumass_,eventWeight_); 
  diTauPtHisto_->Fill(diTauSel_->pt(),eventWeight_);  
  diTauEtaHisto_->Fill(diTauSel_->eta(),eventWeight_);
  diTauMassSVFitHisto_->Fill(tree_svfitmass_,eventWeight_);   

  tauPtHisto_->Fill(tree_taupt_,eventWeight_); 
  tauEtaHisto_->Fill(tree_taueta_,eventWeight_); 
  tauIsoHisto_->Fill(diTauSel_->leg1().relIso(0.5),eventWeight_);
  tauDxyHisto_->Fill(diTauSel_->leg1().dxy(),eventWeight_); 
  tauDzHisto_->Fill(diTauSel_->leg1().dz(),eventWeight_);
  tauDecayModeHisto_->Fill(tree_taudecaymode_);
  if(tree_taudecaymode_==0&&diTauSel_->leg1().p()>0.){
    tauEoPHisto_->Fill(tree_taueop_,eventWeight_);
    tauEoP1Histo_->Fill(tree_taueop_,eventWeight_);
    tauHoPHisto_->Fill(diTauSel_->leg1().leadChargedHadrHCalEnergy()/diTauSel_->leg1().p(),eventWeight_);
    tauEHoPHisto_->Fill(tree_tauehop_,eventWeight_);
    if(fabs(tree_taueop_-1.0)<.0001){
      tauEtaEoP1Histo_->Fill(tree_taueta_,eventWeight_);
      tauPtEoP1Histo_->Fill(tree_taupt_,eventWeight_);
    }
  }
  if(genBosonL1_&&genBosonL2_){
    if(diTauSel_->leg1().charge()==genBosonL1_->charge())
      tauTruthDeltaRHisto_->Fill(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),genBosonL1_->eta(),genBosonL1_->phi()),eventWeight_);
    else tauTruthDeltaRHisto_->Fill(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),genBosonL2_->eta(),genBosonL2_->phi()),eventWeight_);
  }
  tauTruthHisto_->Fill(tree_tautruth_,eventWeight_);



  muPtHisto_->Fill(tree_mupt_,eventWeight_);
  muEtaHisto_->Fill(tree_mueta_,eventWeight_);
  muIsoHisto_->Fill(tree_muiso_,eventWeight_);
  muDxyHisto_->Fill(diTauSel_->leg2().dxy(),eventWeight_);
  muDzHisto_->Fill(diTauSel_->leg2().dz(),eventWeight_);
  if(genBosonL1_&&genBosonL2_){
    if(diTauSel_->leg2().charge()==genBosonL1_->charge())
      muTruthDeltaRHisto_->Fill(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL1_->eta(),genBosonL1_->phi()),eventWeight_);
    else muTruthDeltaRHisto_->Fill(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL2_->eta(),genBosonL2_->phi()),eventWeight_);
  }  


  transverseMassHisto_->Fill(tree_transversemass_,eventWeight_);    
  metHisto_->Fill(tree_met_,eventWeight_);  
  metphiHisto_->Fill(diTauSel_->met().phi(),eventWeight_);  

  //jets 
  njetHisto_->Fill(pfJetList_.size(),eventWeight_);
  njetLCHisto_->Fill(pfJetListLC_.size(),eventWeight_);

  //
  if(pfJetListLC_.size()>=1){
    leadJetPtHisto_->Fill(pfJetListLC_.begin()->pt(),eventWeight_);
    leadJetEtaHisto_->Fill(pfJetListLC_.begin()->eta(),eventWeight_);
  }
  if(pfJetListLC_.size()>=2){
    diJetMassHisto_->Fill((pfJetListLC_[0].p4()+pfJetListLC_[1].p4()).mass(),eventWeight_);
    diJetDeltaEtaHisto_->Fill(pfJetListLC_[0].eta() - pfJetListLC_[1].eta(),eventWeight_);
    diJetEta1Eta2Histo_->Fill(pfJetListLC_[0].eta()*pfJetListLC_[1].eta(),eventWeight_);
  }


  return 1;
}


bool TauMuAnalysis::createHistos(TString samplename){

  std::vector<Sample*>::const_iterator s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++)
    if(samples_[i]->GetName()==samplename) sample_=samples_[i];
  if(!sample_){cout<<"sample "<<samplename<<" was not found"<<endl; return 0;}

  if(!addHistos(sample_))return 0;
  
  if(makeAllHistos_){
    //sample_->cloneHistos("dileptonveto");
    //sample_->cloneHistos("tauiso");
    //sample_->cloneHistos("tauagainstmuon");
    sample_->cloneHistos("taueop");
    //sample_->cloneHistos("muiso");    tauEoPHisto_->Fill(diTauSel_->leg1().leadChargedHadrECalEnergy()/diTauSel_->leg1().p(),eventWeight_);
    sample_->cloneHistos("mupt");
    sample_->cloneHistos("massT");

    sample_->cloneHistos("SM0");
    sample_->cloneHistos("SM1");
    sample_->cloneHistos("SM2");
    sample_->cloneHistos("SM9");
  }


  //create the flat tree
  tree_=new TTree(TString(sample_->GetName())+"_tree",TString(sample_->GetName())+"_tree",0);
  tree_->Branch("eventweight",&tree_eventweight_,"eventweight/F");
  tree_->Branch("ditaumass",&tree_ditaumass_,"ditaumass/F");
  tree_->Branch("taupt",&tree_taupt_,"taupt/F");
  tree_->Branch("taueta",&tree_taueta_,"taueta/F");
  tree_->Branch("tauehop",&tree_tauehop_,"tauehop/F");
  tree_->Branch("taueop",&tree_taueop_,"taueop/F");
  tree_->Branch("tautruth",&tree_tautruth_,"tautruth/I");
  tree_->Branch("taudecaymode",&tree_taudecaymode_,"taudecaymode/I");
  tree_->Branch("mupt",&tree_mupt_,"mupt/F");
  tree_->Branch("mueta",&tree_mueta_,"mueta/F");
  tree_->Branch("met",&tree_met_,"met/F");
  tree_->Branch("transversemass",&tree_transversemass_,"transversemass/F");
  tree_->Branch("svfitmass",&tree_svfitmass_,"svfitmass/F");
  sample_->addTree(tree_);


  if(!sample_->getEvents()){
    cout<<" No fwlit::ChainEvent found in sample "<<sample_->GetName()<<endl;
    return 0;
  }


  fwlite::ChainEvent chain=*(sample_->getEvents());
  //note: cannot use the pointer to the chain in the sample, event loop crashes after first file


  Int_t ievt=0;
  Int_t goodtrigevts=0;
  for(chain.toBegin(); !chain.atEnd() && ievt < truncateEvents_; ++chain, ++ievt){
    const fwlite::Event * event = chain.event();        

    if(!fillVariables(event)) return 0;

    if(ievt%printFreq_==0)cout<<sample_->GetName()<<" "<<ievt<<" events done , run = "<<runnumber_<<" , lumiblock = "<<lumiblock_<<" , eventid = "<<eventid_<<endl;    
    
    //event level selections
    if(!BaseAnalysis::applySelections()) continue;
    goodtrigevts++;

    //fill (N-1) cut histograms here
    if(makeAllHistos_){
      //      if(applySelections("tauiso")) if(!fillHistos("tauiso")) return 0;
      //      if(applySelections("tauagainstmuon")) if(!fillHistos("tauagainstmuon")) return 0;
      if(applySelections("taueop")) if(!fillHistos("taueop")) return 0;
      //      if(applySelections("muiso")) if(!fillHistos("muiso")) return 0;
      if(applySelections("mupt")) if(!fillHistos("mupt")) return 0;
      //      if(applySelections("dileptonveto")) if(!fillHistos("dileptonveto")) return 0;
      if(applySelections("massT")) if(!fillHistos("massT")) return 0;   
    }

    //fill fully selected inclusive histograms 
    if(applySelections()){
      if(!fillHistos())return 0;
      tree_->Fill();
      
      //category histos
      if(makeAllHistos_){
	if(eventCategorySM_==0) if(!fillHistos("SM0"))return 0;
	if(eventCategorySM_==1) if(!fillHistos("SM1"))return 0;
	if(eventCategorySM_==2) if(!fillHistos("SM2"))return 0;
	if(eventCategorySM_==9) if(!fillHistos("SM9"))return 0; 
      }
    }
    
    
  }
  cout<<sample_->GetName()<<" "<<ievt<<" events processed."<<endl;   
  cout<<sample_->GetName()<<" "<<goodtrigevts<<" events passed trigger."<<endl;
  sample_->printCounters();

  //
  if(!sample_->save())return 0;
  
  
  return 1;
}



bool TauMuAnalysis::printRawYields(TString histoname){
  
  Float_t totalData=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      totalData+=hmass->Integral(0,hmass->GetNbinsX()+1);
    }
  }
  cout<<"Total Data  = "<<(int)(totalData)<<endl;

  Float_t totalDataSS=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data_SS"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      totalDataSS+=hmass->Integral(0,hmass->GetNbinsX()+1);
    }
  }
  cout<<"Total Data SS = "<<(int)(totalDataSS)<<endl;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
    }
  }
  
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC_SS"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
    }
  }

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Embedded"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
    }
  }
  
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Embedded_SS"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
    }
  }

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Signal"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
    }
  }
  
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Signal_SS"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
    }
  }

  return 1;
}

Float_t TauMuAnalysis::getWJetsSignalToSBFraction(){

  //////needed by the data-card for limits before any rescalings
  Float_t WJetsSignal=0.;  Float_t WJetsSideBand=0.;
  if(transverseMassSignalMax_>transverseMassSideBandMin_)
    cout<<"Inconsistent values: transverseMassSignalMax_ > transverseMassSideBandMin_"<<endl;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if(TString((*s)->GetName())=="WJetsToLNu"){
      TH1* h=(*s)->getHistoFromFile("transverseMassHisto_massT");
      WJetsSignal=h->Integral(1,transverseMassSignalMax_); 
      WJetsSideBand=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX()); 
    }
  }
  if(WJetsSideBand==0.)return 0.;

  cout<<" Ratio of (WJets below "<<transverseMassSignalMax_<<" GeV)/(WJets above "<<transverseMassSideBandMin_<<" GeV) = "<<WJetsSignal/WJetsSideBand<<endl;
 
  return WJetsSignal/WJetsSideBand;
}


bool TauMuAnalysis::scaleHistos(){



  /////////determine total lumi and scale MCs
  cout<<" List of Data samples: "<<endl;
  float totalDataLumi=0.;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      totalDataLumi+=(*s)->getLumi();
      cout<<(*s)->GetName()<<endl;  
    }
    if((*s)->getDataType()=="Data_SS"){
      cout<<(*s)->GetName()<<endl;  
    }
  }
  cout<<"Total Data lumi = "<<totalDataLumi<<endl;

  cout<<"  Normalizing MCs to luminosity: "<<endl;
  cout<<" List of MC samples: "<<endl;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC" || (*s)->getDataType()=="Signal"){
      (*s)->scale((*s)->getEffCorrFactor());
      (*s)->scale(totalDataLumi/(*s)->getLumi());
      cout<<(*s)->GetName()<<endl; 
    }
    if((*s)->getDataType()=="MC_SS" || (*s)->getDataType()=="Signal_SS"){
      (*s)->scale((*s)->getEffCorrFactor());
      (*s)->scale(totalDataLumi/(*s)->getLumi());
      cout<<(*s)->GetName()<<endl;  
    }
  }




  ///////////////////////////////
  /////Determine correction factor for WJets from massT sideband
  //////////////////////////////
  cout<<"Determining WJets  from sidebands"<<endl;

  Float_t WJetsSSSideCorr=1.; Float_t WJetsSSSide=0; Float_t MCSSSide=0; Float_t DataSSSide=0;
  Float_t WJetsSideCorr=1.; Float_t WJetsSide=0; Float_t MCSide=0; Float_t DataSide=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    TH1* h=(*s)->getHistoFromFile("transverseMassHisto_massT");

    if((*s)->getDataType()=="Data_SS")
      DataSSSide+=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX());  
    
    if((*s)->getDataType()=="MC_SS"){
      if(TString((*s)->GetName())=="WJetsToLNu_SS")
	WJetsSSSide=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX());    
      else MCSSSide+=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX());
    }

    if((*s)->getDataType()=="Data")
      DataSide+=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX()); 
    
    if((*s)->getDataType()=="MC"){
      if(TString((*s)->GetName())=="WJetsToLNu")
	WJetsSide=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX());    
      else MCSide+=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX());
    }
  }
  WJetsSSSideCorr=(DataSSSide-MCSSSide)/WJetsSSSide;
  WJetsSideCorr=(DataSide-MCSide)/WJetsSide;


  cout<<"Data SS yield in side-band = "<<DataSSSide<<endl;
  cout<<"MC SS yield in side-band = "<<MCSSSide<<endl;
  cout<<"WJetsSS Correction = "<<WJetsSSSideCorr<<endl;
  cout<<"WJetsOS Correction = "<<WJetsSideCorr<<endl;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if(TString((*s)->GetName())=="WJetsToLNu_SS"){
      (*s)->scale(WJetsSSSideCorr);
    }
    if(TString((*s)->GetName())=="WJetsToLNu"){
      (*s)->scale(WJetsSideCorr);
    }
  }



  ///////////////////////////////////
  ////Scale the embedded samples:
  /////////////////////////////////
  Float_t ZToTauTauMC=0.;
  Float_t EmbeddedOS=0.;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Embedded")
      EmbeddedOS+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral();  
    if(TString((*s)->GetName())=="ZToTauTau")
      ZToTauTauMC=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral();  
  }
  if(ZToTauTauMC==0. || EmbeddedOS==0.){ cout<<" bad ZToTauTauMC or EmbeddedOS yields"<<endl; return 0;}
  cout<<"Rescaling Embedded samples by factor : "<<ZToTauTauMC/EmbeddedOS<<endl;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Embedded" || (*s)->getDataType()=="Embedded_SS")
      (*s)->scale(ZToTauTauMC/EmbeddedOS);

  return 1;
}




TH1F* TauMuAnalysis::getTotalDataSS(TString histoname){

  cout<<" Calculating Total Data SS: "<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hData_SS","Data SS",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data_SS")
      h->Add((*s)->getHistoFromFile(histoname));  
 
  return h;
}

TH1F* TauMuAnalysis::getTotalData(TString histoname){

  cout<<" Calculating Total Data: "<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hData","Data",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data")
      h->Add((*s)->getHistoFromFile(histoname));  
 
  return h;
}

TH1F* TauMuAnalysis::getTotalEmbeddedSS(TString histoname){
  cout<<" Calculating Total Embedded SS: "<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hEmbeddedSS","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Embedded_SS")
      h->Add((*s)->getHistoFromFile(histoname));  
 
  return h;
}

TH1F* TauMuAnalysis::getTotalEmbedded(TString histoname){
  cout<<" Calculating Total Embedded: "<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hEmbedded","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Embedded")
      h->Add((*s)->getHistoFromFile(histoname));  
 
  return h;
}


TH1F* TauMuAnalysis::getMCSS(TString histoname){
  ///Does not include Signals
  ///currently uses ZToTauTau MC not the embedded

  cout<<" Calculating Total MC SS: "<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hMC_SS","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC_SS")
      h->Add((*s)->getHistoFromFile(histoname));  
 
  return h;
}


TH1F* TauMuAnalysis::getSample(TString samplename, TString histoname){

  cout<<" Extracting sample: "<<samplename<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F(samplename,"",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  bool found=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if(TString((*s)->GetName())==samplename){
      h->Add((*s)->getHistoFromFile(histoname));
      h->SetTitle((*s)->getPlotLabel());
      found=1;
    }  
  
  if(!found){
    cout<<"Sample distribution "<<samplename<<" "<<histoname<<" not found"<<endl;
    return NULL;
  }

  return h;
}

TH1F* TauMuAnalysis::getQCD(TString histoname){

  cout<<" Calculating QCD: "<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hQCD","QCD",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  
  TH1F* hDataSS=getTotalDataSS(histoname);
  if(hDataSS){
    cout<<"Data SS yield = "<<hDataSS->Integral()<<endl;
    h->Add(hDataSS);
  }
  else {cout<<"Data SS not determined"<<endl; return 0;}
  delete hDataSS;

  TH1F* hMCSS=getMCSS(histoname);
  if(hMCSS){
    cout<<"MC SS yield = "<<hMCSS->Integral()<<endl;
    h->Add(hMCSS,-1.);
  }
  else {cout<<"MC SS not determined"<<endl; return 0;}
  delete hMCSS;

  cout<<"QCD before OS/SS rescaling = "<<h->Integral()<<endl;
  h->Scale(QCDOStoSSRatio_);
  cout<<"QCD after OS/SS rescaling = "<<h->Integral()<<endl;
  /////////////////////////////////////

  return h;
}

TH1F* TauMuAnalysis::getDiBoson(TString histoname){
  cout<<" Calculating Di-Bosons: "<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hVV","VV",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  
  TH1F* hWW=getSample("WW",histoname);
  if(!hWW)return 0;
  cout<<"   WW "<<hWW->Integral(0,hWW->GetNbinsX()+1)<<endl;
  h->Add(hWW);
  delete hWW;

  TH1F* hWZ=getSample("WZ",histoname);
  if(!hWZ)return 0;
  h->Add(hWZ);
  cout<<"   WZ "<<hWZ->Integral(0,hWZ->GetNbinsX()+1)<<endl;
  delete hWZ;

  TH1F* hZZ=getSample("ZZ",histoname);
  if(!hZZ)return 0;
  cout<<"   ZZ "<<hZZ->Integral(0,hZZ->GetNbinsX()+1)<<endl; 
  h->Add(hZZ);
  delete hZZ;

  return h;
}


TH1F* TauMuAnalysis::getZToTauTau(TString histoname){
  
  //TH1F*h=getSample("ZToTauTau",histoname);
  TH1F*h=getTotalEmbedded(histoname);
  
  h->SetTitle("Z#rightarrow#tau^{+}#tau^{-}");

  return h;
}

TH1F* TauMuAnalysis::getTotalBackground(TString histoname){

  cout<<" Calculating Total Background: "<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hBackground","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());

  TH1F*hQCD=getQCD(histoname);
  if(!hQCD)return 0;
  h->Add(hQCD);
  delete hQCD;

  TH1F*hZToTauTau=getZToTauTau(histoname);
  if(!hZToTauTau)return 0;
  h->Add(hZToTauTau);
  delete hZToTauTau;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC" && TString((*s)->GetName()) != "ZToTauTau")
      h->Add((*s)->getHistoFromFile(histoname)); 

  
  return h;
}



bool TauMuAnalysis::plot(TString histoname, Int_t rebin, TString xlabel, TString ylabel, Float_t* legendcoords, Float_t* axesrange, bool log){
  if(!legendcoords || !axesrange){
    cout<<"No legend cordinates or axes ranges"<<endl; return 0;
  }
  
  if(!scaleHistos())return 0;


  TCanvas C("C",TString("TauMuAnalysis_")+histoname+".ps");
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps[");
  
  
  ////Show Data SS and MC SS
  TH1F* hDataSS=getTotalDataSS(histoname);
  if(!hDataSS){cout<<" Total Data SS not determined "<<endl; return 0;}
  hDataSS->Rebin(rebin);
  TH1F* hMCSS=getMCSS(histoname);
  if(!hMCSS){cout<<" Total MC SS not determined "<<endl; return 0;}
  hMCSS->Rebin(rebin);
  C.Clear();
  hDataSS->GetXaxis()->SetRangeUser(axesrange[0],axesrange[1]);
  hDataSS->Draw("pe");
  hMCSS->Draw("hist same");
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps"); 
  delete hDataSS;
  delete hMCSS;


  ////Total MC
  ///must get here before getting individual components below because same histo object name is used for each component inside
  TH1F* hBkg=getTotalBackground(histoname);
  if(!hBkg){cout<<" Total Background not determined "<<endl; return 0;}
  hBkg->Rebin(rebin);
  hBkg->SetLineWidth(1);

  ////////////////////////
  cout<<"Creating Bkg Stack:"<<endl;
  THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
 
  //
  TH1F*hTTJets=getSample("TTJets",histoname);
  if(!hTTJets)return 0;
  hTTJets->Rebin(rebin);
  hTTJets->SetLineWidth(1);
  hTTJets->SetLineColor(TTJetsColor_);
  hTTJets->SetFillColor(TTJetsColor_);
  cout<<"TTJets "<<hTTJets->Integral(0,hTTJets->GetNbinsX()+1)<<endl;
  hMCStack.Add(hTTJets,"hist");
 
  //
  TH1F*hQCD=getQCD(histoname);
  if(!hQCD)return 0;
  hQCD->Rebin(rebin);
  hQCD->SetLineWidth(1);
  hQCD->SetLineColor(QCDColor_);
  hQCD->SetFillColor(QCDColor_);
  hMCStack.Add(hQCD,"hist");
  cout<<"   QCD "<<hQCD->Integral(0,hQCD->GetNbinsX()+1)<<endl;

  //combine ZLJet with QCD 
  TH1F*hZToLJet=getSample("ZToLJet",histoname);
  if(!hZToLJet)return 0;
  hZToLJet->Rebin(rebin);
  cout<<"   ZToLJet "<<hZToLJet->Integral(0,hZToLJet->GetNbinsX()+1)<<endl;  
  hQCD->Add(hZToLJet);
  delete hZToLJet;

  cout<<"Fakes "<<hQCD->Integral(0,hQCD->GetNbinsX()+1)<<endl;


  TH1F*hWJetsToLNu=getSample("WJetsToLNu",histoname);
  if(!hWJetsToLNu)return 0;
  hWJetsToLNu->Rebin(rebin);
  hWJetsToLNu->SetLineWidth(1);
  hWJetsToLNu->SetLineColor(WJetsColor_);
  hWJetsToLNu->SetFillColor(WJetsColor_);
  cout<<"   WJetsToLNu "<<hWJetsToLNu->Integral(0,hWJetsToLNu->GetNbinsX()+1)<<endl;
  hMCStack.Add(hWJetsToLNu,"hist");

  //combine Di-Bosons with WJets
  TH1F*hVV=getDiBoson(histoname);
  if(!hVV)return 0;
  hVV->Rebin(rebin);  
  cout<<"   Di-Boson "<<hVV->Integral(0,hVV->GetNbinsX()+1)<<endl;
  hWJetsToLNu->Add(hVV);
  delete hVV;

  cout<<"ElectroWeak "<<hWJetsToLNu->Integral(0,hWJetsToLNu->GetNbinsX()+1)<<endl;

  //combine Z->MuMu with WJets
  TH1F*hZToMuMu=getSample("ZToMuMu",histoname);
  if(!hZToMuMu)return 0;
  hZToMuMu->Rebin(rebin);
  hZToMuMu->SetLineWidth(1);
  hZToMuMu->SetLineColor(ZMuMuColor_);
  hZToMuMu->SetFillColor(ZMuMuColor_);
  hMCStack.Add(hZToMuMu,"hist");
  cout<<"   ZToMuMu "<<hZToMuMu->Integral(0,hZToMuMu->GetNbinsX()+1)<<endl;
  //hWJetsToLNu->Add(hZToMuMu);
  //delete hZToMuMu;
  


  TH1F*hZToTauTau=getZToTauTau(histoname);
  if(!hZToTauTau)return 0;
  hZToTauTau->Rebin(rebin);
  hZToTauTau->SetLineWidth(1);
  hZToTauTau->SetLineColor(ZTauTauColor_);
  hZToTauTau->SetFillColor(ZTauTauColor_);
  hMCStack.Add(hZToTauTau,"hist");
  cout<<"ZToTauTau "<<hZToTauTau->Integral(0,hZToTauTau->GetNbinsX()+1)<<endl;

  ////Total Data
  TH1F* hData=getTotalData(histoname);
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
  hData->Rebin(rebin);

  cout<<"Total Background "<<(int)(hBkg->Integral(0,hBkg->GetNbinsX()+1))<<endl;
  cout<<"Total Data "<<(int)(hData->Integral(0,hData->GetNbinsX()+1))<<endl;

  //create legend key
  TLegend legend;
  legend.SetLineColor(0);
  legend.SetBorderSize(1);
  legend.AddEntry(hData,hData->GetTitle(),"p");
  legend.AddEntry(hZToTauTau,hZToTauTau->GetTitle(),"f");
  legend.AddEntry(hZToMuMu,hZToMuMu->GetTitle(),"f");
  legend.AddEntry(hTTJets,hTTJets->GetTitle(),"f");
  legend.AddEntry(hWJetsToLNu,hWJetsToLNu->GetTitle(),"f");
  legend.AddEntry(hQCD,"Fakes","f");

 

  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  hData->GetXaxis()->SetRangeUser(axesrange[0],axesrange[1]);
  hData->GetYaxis()->SetRangeUser(axesrange[2],axesrange[3]);
  if(log) C.SetLogy(1);
  hData->SetStats(0);
  hData->GetXaxis()->SetTitle(xlabel);
  hData->GetYaxis()->SetTitle(ylabel);
  hData->Draw("hist pe");
  //////////////////////////
  hMCStack.Draw("hist same");  
  hData->Draw("hist pe same");//bring Data points back to front
  hBkg->SetFillColor(0);
  hBkg->SetLineColor(1);
  hBkg->Draw("histsame");//draw outline of MC        
  hMCStack.GetHistogram()->Draw("same");//bring axes back to front 
  legend.SetX1NDC(legendcoords[0]);
  legend.SetX2NDC(legendcoords[1]);
  legend.SetY1NDC(legendcoords[2]);
  legend.SetY2NDC(legendcoords[3]);
  legend.SetTextSize(.04);
  legend.Draw();
  C.Update();
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  

  
  TH1F*hRatio=(TH1F*)hData->Clone("hRatio");//get here before make stats 
  TH1F*hResid=(TH1F*)hData->Clone("hResid");//get here before make stats 
  TLine line;

  ///////////////////////////////////
  //show the histogram stats
  C.Clear();
  hData->SetStats(1);
  hData->Draw();
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps"); 
  C.Clear();
  hBkg->GetXaxis()->SetRange(hData->GetXaxis()->GetFirst(),hData->GetXaxis()->GetLast());
  hBkg->GetYaxis()->SetRange(hData->GetYaxis()->GetFirst(),hData->GetYaxis()->GetLast());
  hBkg->SetStats(1);
  hBkg->Draw();
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps"); 


  ////////////////////////////////////
  cout<<"Making ratio plot"<<endl;
  C.Clear();
  hRatio->Sumw2();
  hRatio->Divide(hBkg);
  hRatio->GetYaxis()->SetTitle("Data/Background");
  hRatio->SetStats(0);
  //if(0.5<hRatio->GetMinimum()&&hRatio->GetMaximum()<1.5)
  hRatio->GetYaxis()->SetRangeUser(.5,1.5);
  //else hRatio->GetYaxis()->SetRangeUser(0.,hRatio->GetMaximum());
  hRatio->Draw("hist pe");
  if(axesrange)
    line.DrawLine(axesrange[0],1,axesrange[1],1);
  else line.DrawLine(hRatio->GetXaxis()->GetXmin(),0,hRatio->GetXaxis()->GetXmax(),0);
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  
  delete hRatio;

  cout<<"Making residual plot"<<endl;
  C.Clear();
  hResid->Sumw2();
  hResid->Add(hBkg,-1);
  hResid->GetYaxis()->SetTitle("Data - Background");
  hResid->SetStats(0);
  hResid->Draw("hist pe");
  if(axesrange)
    line.DrawLine(axesrange[0],0,axesrange[1],0);
  else line.DrawLine(hResid->GetXaxis()->GetXmin(),0,hResid->GetXaxis()->GetXmax(),0);
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  


  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps]");
  delete hResid;

  delete hQCD;
  delete hTTJets;
  delete hWJetsToLNu;
  delete hZToTauTau;
  delete hBkg;
  delete hData;


  return 1;

}


//   ///get the MET 
//   edm::Handle< std::vector<cmg::BaseMET> > met;
//   event->getByLabel(edm::InputTag("cmgPFMET"),met);
//   met_=&(*met->begin());
  
//   //met significance
//   edm::Handle< cmg::METSignificance > metsig;
//   event->getByLabel(edm::InputTag("PFMETSignificanceAK5"),metsig);  
//   metsig_=metsig->significance();

  /////init recoil corrector
//   recoilCorr_=NULL;
//   RecoilCorrector recoilCorr((const char*)(sample_->getRecoilCorrProcessFile()));
//   cout<<(const char*)(sample_->getRecoilCorrProcessFile())<<endl;
//   if(sample_->getApplyRecoilCorr()){
//     recoilCorr.addMCFile("../data/recoilfit_zmm42X_njet.root");
//     recoilCorr.addDataFile("../data/recoilfit_datamm_njet.root");
//     recoilCorr_=&recoilCorr;
//   }
  ////use with correctAll
  ////with datamm in constructor and not adding any other files correctAll over corrects 
  ////with zmm in constructor and adding datamm DataFile and zmm MCFile over corrects a bit


//     if(verbosity_>1) cout<<"Calculating SVFit mass"<<endl;
//     NSVfitStandalone::Vector measuredMET(metCorr.x(),metCorr.y(),0);
//     std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
//     NSVfitStandalone::LorentzVector p1(diTauSel_->leg1().p4());
//     measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay,p1));    
//     NSVfitStandalone::LorentzVector p2(diTauSel_->leg2().p4());
//     measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay,p2));

//     TMatrixD metsigCorr(2,2);
//     metsigCorr[0][0]=(*metsig_)[0][0];
//     metsigCorr[0][1]=(*metsig_)[0][1];
//     metsigCorr[1][0]=(*metsig_)[1][0];
//     metsigCorr[1][1]=(*metsig_)[1][1];

//     //     ///////fix the eigenvalues of the matrix to match the Data:
//     //     if(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS"){
//     //       TVectorD eigenVals(2);
//     //       TMatrixD eigenVects(2,2);
//     //       eigenVects=metsig_->EigenVectors(eigenVals);    
//     //       TMatrixD eigenVectsInv(2,2);
//     //       eigenVectsInv=eigenVects;
//     //       eigenVectsInv.Invert();
//     //       TMatrixD metsigEigen(2,2);
//     //       metsigEigen[0][0]=(124.079/117.751)*eigenVals[0];//correction factors determined from 2.1 fb-1 and Summer11 MC after pile-up reweighting
//     //       metsigEigen[0][1]=0.;
//     //       metsigEigen[1][0]=0.;
//     //       metsigEigen[1][1]=(84.422/78.108)*eigenVals[1];      
//     //       metsigCorr=eigenVects*metsigEigen*eigenVectsInv;
//     //     }
//     //     ///==>Very small correction obtained on svfit mass, mean of MC: 103.38 --> 103.36  (mean of Data: 102.03)


//     NSVfitStandaloneAlgorithm algo(measuredTauLeptons,measuredMET,&metsigCorr,verbosity_);
//     algo.maxObjFunctionCalls(5000);
//     algo.fit();
    
//     tree_svfitstatus_=algo.fitStatus();
//     tree_svfitmass_=algo.fittedDiTauSystem().mass();
//     tree_svfitedm_=algo.edm();
    
//     diTauMassSVFitHisto_->Fill(tree_svfitmass_,eventWeight_);        
//     svFitCov00Histo_->Fill(metsigCorr[0][0],eventWeight_);
//     svFitConvergeHisto_->Fill(tree_svfitstatus_,eventWeight_);
    
//     //study the matrix elements
//     TVectorD eigenValues(2);
//     TMatrixD eigenVectors(2,2);
//     eigenVectors=metsigCorr.EigenVectors(eigenValues);
//     tree_svfiteigenval0_=eigenValues[0];  
//     tree_svfiteigenval1_=eigenValues[1];
//     svFitEigen0Histo_->Fill(tree_svfiteigenval0_,eventWeight_);
//     svFitEigen1Histo_->Fill(tree_svfiteigenval1_,eventWeight_);
    


// void TauMuAnalysis::applyRecoilCorr(const cmg::TauMu * cand, TVector3 * MET){
//   if(!(sample_->getApplyRecoilCorr()
//        && genBoson_ 
//        && (genEventType_==1 || genEventType_==3 || genEventType_==5
// 	   || genEventType_==11 || genEventType_==13 || genEventType_==15))) return;

//   TVector3 tau1PT=TVector3(cand->leg1().p4().x(),cand->leg1().p4().y(),0.);
//   TVector3 tau2PT=TVector3(cand->leg2().p4().x(),cand->leg2().p4().y(),0.);
//   TVector3 genBosonPT=TVector3(genBoson_->p4().x(),genBoson_->p4().y(),0.);
//   double met = MET->Mag();
//   double metphi = MET->Phi();    
//   TVector3 recoLeptonPT = tau2PT; //use only the muon for WJets
//   if(genEventType_==1 || genEventType_==3 || genEventType_==5) recoLeptonPT += tau1PT; 
//   double pU1      = 0;  //--
//   double pU2      = 0;  //--
  
//   if(verbosity_>0)cout<<met<<"   "<<metphi<<"   "<<genBosonPT.Mag()<<"   "<<genBosonPT.Phi()<<"   "<<recoLeptonPT.Mag()<<"   "<<recoLeptonPT.Phi()<<"   "<<pfJetListLC_.size()<<endl;
//   recoilCorr_->CorrectType1(met,metphi,genBosonPT.Mag(),genBosonPT.Phi(),recoLeptonPT.Mag(),recoLeptonPT.Phi(),pU1,pU2,0.,pfJetListLC_.size());
//   //recoilCorr_->CorrectType2(met,metphi,genBosonPT.Mag(),genBosonPT.Phi(),recoLeptonPT.Mag(),recoLeptonPT.Phi(),pU1,pU2,0.,pfJetListLC_.size());
//   //recoilCorr_->CorrectAll(met,metphi,genBosonPT.Mag(),genBosonPT.Phi(),recoLeptonPT.Mag(),recoLeptonPT.Phi(),pU1,pU2,0.,pfJetListLC_.size());
//   if(verbosity_>0)cout<<met<<"   "<<metphi<<"   "<<genBosonPT.Mag()<<"   "<<genBosonPT.Phi()<<"   "<<recoLeptonPT.Mag()<<"   "<<recoLeptonPT.Phi()<<"   "<<pfJetListLC_.size()<<endl;

//   MET->SetMagThetaPhi(met,TMath::Pi()/2.,metphi);
// }



// float TauMuAnalysis::computePZeta(const cmg::TauMu * cand){
//   if(!cand || !met_) return 0.;
//   //1) zeta axis is the bisector between tau1 and tau2 momentum vectors
//   //2) project visible energy onto zeta axis
//   //3) project MET onto zeta axis
//   TVector3 tau1PT=TVector3(cand->leg1().p4().x(),cand->leg1().p4().y(),0.);
//   TVector3 tau2PT=TVector3(cand->leg2().p4().x(),cand->leg2().p4().y(),0.);
//   TVector3 metPT=TVector3(met_->p4().x(),met_->p4().y(),0.);
//   applyRecoilCorr(cand,&metPT);

//   TVector3 zetaAxis=(tau1PT.Unit() + tau2PT.Unit()).Unit();
//   Float_t pZetaVis=(tau1PT + tau2PT)*zetaAxis;
//   Float_t pZetaMET=metPT*zetaAxis;
//   Float_t pZeta=pZetaVis+pZetaMET;
  
//   return pZeta-1.5*pZetaVis;
// }

// float TauMuAnalysis::computeTransverseMass(const cmg::TauMu * cand){
//   if(!cand || !met_) return 0.;

//   TVector3 muonPT=TVector3(cand->leg2().p4().x(),cand->leg2().p4().y(),0.);
//   TVector3 metPT=TVector3(met_->p4().x(),met_->p4().y(),0.);
//   applyRecoilCorr(cand,&metPT);

//   //return sqrt(2.0*muonPT.Mag()*metPT.Mag()*(1-cos(metPT.Phi()-muonPT.Phi())));
//   return (TLorentzVector(muonPT,muonPT.Mag()) + TLorentzVector(metPT,metPT.Mag())).Mag();
// }

// float TauMuAnalysis::computeTauIso(const cmg::Tau * tau){
//   return (tau->trackIso()+std::max( (tau->gammaIso() - tau->userData("rho")*TMath::Pi()*0.5*0.5), 0.0)); 
// }


// bool TauMuAnalysis::plotDistribution(TString histoname, Int_t rebin, TString xlabel, TString ylabel, Float_t* legendcoords, Float_t* axesrange, bool log){

  
//   TCanvas C("C",TString("TauMuAnalysis_")+histoname+".ps");
//   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps[");
  



//   TH1F* href=NULL;

//   cout<<" List of Data and MC samples: "<<endl;
//   float totalDataLumi=0.;
//   Int_t padcounter=0; 
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data"){
//       C.cd(++padcounter);
//       TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
//       totalDataLumi+=(*s)->getLumi();
//       cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
//       href=hmass;
//     }
//   }

//   padcounter=0;
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data_SS"){
//       C.cd(++padcounter);
//       TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
//       cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
//     }
//   }

//   padcounter=0;
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="MC" || (*s)->getDataType()=="Signal" ){
//       C.cd(++padcounter);
//       (*s)->scaleLumi(totalDataLumi);
//       TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
//       hmass->SetFillColor((*s)->getColor());
//       hmass->SetLineColor(1);
//       hmass->SetLineWidth(1);
//       cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl; 
//     }
//   }
  
//   padcounter=0;
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="MC_SS"){
//       C.cd(++padcounter);
//       (*s)->scaleLumi(totalDataLumi);
//       TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
//       hmass->SetFillColor((*s)->getColor());
//       hmass->SetLineColor((*s)->getColor());
//       hmass->SetLineWidth(1);
//       cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
//     }
//   }


//   Float_t WJetsBelowCorr=1.;
//   Float_t WJetsSSBelowCorr=1.;

// //   ///////////////////////////////
// //   /////Determine correction factor for WJets from Data < -40 , assuming binning pZetaHisto","",180,-140,40)))
// //   //////////////////////////////
// //   cout<<"Determine WJetsSS from pZeta below -40: assume original binning pZetaHisto ,180,-140,40"<<endl;
// //   Float_t WJetsSSBelow=0; Float_t MCSSBelow=0; Float_t DataSSBelow=0;
// //   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
// //     if((*s)->getDataType()=="Data_SS")
// //       DataSSBelow+=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);  
// //     else if(TString((*s)->GetName())=="WJetsToLNu_SS")
// //       WJetsSSBelow=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);    
// //     else if((*s)->getDataType()=="MC_SS") MCSSBelow+=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);
// //   }
// //   WJetsSSBelowCorr=(DataSSBelow-MCSSBelow)/WJetsSSBelow;
 
// //   Float_t WJetsBelow=0; Float_t MCBelow=0; Float_t DataBelow=0;
// //   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
// //     if((*s)->getDataType()=="Data")
// //       DataBelow+=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100); 
// //     else if((*s)->getDataType()=="MC"){
// //       if(TString((*s)->GetName())=="WJetsToLNu") WJetsBelow=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);    
// //       else MCBelow+=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);
// //     }
// //   }
// //   WJetsBelowCorr=(DataBelow-MCBelow)/WJetsBelow;


//   ///////////////////////////////
//   /////Determine correction factor for WJets from massT > 50, assuming binning ,200,0,200)))
//   //////////////////////////////
//   cout<<"Determine WJetsSS from transverse mass > 50, assuming binning ,200,0,200"<<endl;
//   Float_t WJetsSSBelow=0; Float_t MCSSBelow=0; Float_t DataSSBelow=0;
//   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data_SS")
//       DataSSBelow+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);  
//     else if(TString((*s)->GetName())=="WJetsToLNu_SS")
//       WJetsSSBelow=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);    
//     else if((*s)->getDataType()=="MC_SS") MCSSBelow+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);
//   }
//   WJetsSSBelowCorr=(DataSSBelow-MCSSBelow)/WJetsSSBelow;
 
//   Float_t WJetsBelow=0; Float_t MCBelow=0; Float_t DataBelow=0;
//   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data")
//       DataBelow+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200); 
//     else if((*s)->getDataType()=="MC"){
//       if(TString((*s)->GetName())=="WJetsToLNu") WJetsBelow=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);    
//       else MCBelow+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);
//     }
//   }
//   WJetsBelowCorr=(DataBelow-MCBelow)/WJetsBelow;


//   ///////////////////////////////////////////////////
//   ////Rebin only after doing necessary calculations
//   ////////////////////////////////////////////////////
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
//     ((TH1F*)((*s)->getHistoFromFile(histoname)))->Rebin(rebin);
    

// //   //////////////////////////////////////////////////////////
// //   ///Make a plot of the pZeta Distribution in the SS sample
// //   ///////////////////////////////////////////////////////////
// //   TH1F hDataSSpZeta("hDataSSpZeta","",30,-140,40);
// //   THStack hMCSSStackpZeta("hMCSSStackpZeta","MC");
// //   TLegend legendSSpZeta;
// //   legendSSpZeta.AddEntry(&hDataSSpZeta,TString("DataSS (")+(long)totalDataLumi+" pb^{-1})","p");
// //   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
// //     if((*s)->getDataType()=="Data_SS"){
// //       TH1F* h=(TH1F*)((*s)->getHistoFromFile("pZetaHisto_pzeta"));    
// //       h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// //       if(h->GetXaxis()->GetNbins()!=30)h->Rebin(6);//before rebinning check if it is already rebinned above
// //       hDataSSpZeta.Add(h);
// //       delete h;
// //     }
// //     if((*s)->getDataType()=="MC_SS"){
// //       TH1F* h=(TH1F*)((*s)->getHistoFromFile("pZetaHisto_pzeta"));    
// //       h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// //       if(h->GetXaxis()->GetNbins()!=30)h->Rebin(6);
// //       hMCSSStackpZeta.Add(h,"hist");
// //       legendSSpZeta.AddEntry(h,(*s)->GetName(),"f");
// //     }
// //   }
// //   C.Clear();
// //   hDataSSpZeta.SetStats(0);
// //   hDataSSpZeta.GetYaxis()->SetTitle("Evetns / 6 GeV");
// //   hDataSSpZeta.GetXaxis()->SetTitle("P_{#zeta} - 1.5*P_{#zeta}^{vis}    (GeV)");
// //   hDataSSpZeta.Draw("hist pe");
// //   hMCSSStackpZeta.Draw("same");
// //   legendSSpZeta.SetX1NDC(0.16);
// //   legendSSpZeta.SetX2NDC(0.45);
// //   legendSSpZeta.SetY1NDC(0.45);
// //   legendSSpZeta.SetY2NDC(0.8);
// //   legendSSpZeta.SetTextSize(.04);
// //   legendSSpZeta.Draw();
// //   hDataSSpZeta.Draw("pe same");
// //   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  


// //   ///Make a plot of the pZeta Distribution in the OS sample
// //   TH1F hDataOSpZeta("hDataOSpZeta","",30,-140,40);
// //   THStack hMCOSStackpZeta("hMCOSStackpZeta","MC");
// //   TLegend legendOSpZeta;
// //   legendOSpZeta.AddEntry(&hDataOSpZeta,TString("Data (")+(long)totalDataLumi+" pb^{-1})","p");
// //   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
// //     if((*s)->getDataType()=="Data"){
// //       TH1F* h=(TH1F*)((*s)->getHistoFromFile("pZetaHisto_pzeta"));    
// //       h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// //       if(h->GetXaxis()->GetNbins()!=30)h->Rebin(6);
// //       hDataOSpZeta.Add(h);
// //       delete h;
// //     }
// //     if((*s)->getDataType()=="MC"){
// //       TH1F* h=(TH1F*)((*s)->getHistoFromFile("pZetaHisto_pzeta"));    
// //       h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// //       if(h->GetXaxis()->GetNbins()!=30)h->Rebin(6);
// //       hMCOSStackpZeta.Add(h,"hist");
// //       legendOSpZeta.AddEntry(h,(*s)->GetName(),"f");
// //     }
// //   }
// //   C.Clear();
// //   hDataOSpZeta.SetStats(0);
// //   hDataOSpZeta.GetYaxis()->SetTitle("Evetns / 6 GeV");
// //   hDataOSpZeta.GetXaxis()->SetTitle("P_{#zeta} - 1.5*P_{#zeta}^{vis}    (GeV)");
// //   hDataOSpZeta.Draw("hist pe");
// //   hMCOSStackpZeta.Draw("same");
// //   legendOSpZeta.SetX1NDC(0.16);
// //   legendOSpZeta.SetX2NDC(0.4);
// //   legendOSpZeta.SetY1NDC(0.45);
// //   legendOSpZeta.SetY2NDC(0.8);
// //   legendOSpZeta.SetTextSize(.04);
// //   legendOSpZeta.Draw();
// //   hDataOSpZeta.Draw("pe same");
// //   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  
// //   /////////////////////////////////////////////////////////////////


//   if(histoname=="transverseMassHisto_massT"){
//     //////////////////////////////////////////////////////////
//     ///Make a plot of the mT Distribution in the SS sample
//     ///////////////////////////////////////////////////////////
//     TH1F hDataSSmassT("hDataSSmassT","",100,0,200);
//     THStack hMCSSStackmassT("hMCSSStackmassT","MC");
//     TLegend legendSSmassT;
//     legendSSmassT.SetLineColor(0);
//     legendSSmassT.SetBorderSize(1);
//     legendSSmassT.AddEntry(&hDataSSmassT,TString("DataSS (")+(long)totalDataLumi+" pb^{-1})","p");
//     for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//       if((*s)->getDataType()=="Data_SS"){
// 	TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));    
// 	h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// 	if(h->GetXaxis()->GetNbins()!=100)h->Rebin(2);//before rebinning check if it is already rebinned above
// 	hDataSSmassT.Add(h);
// 	delete h;
//       }
//       if((*s)->getDataType()=="MC_SS"){
// 	TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));    
// 	h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// 	if(h->GetXaxis()->GetNbins()!=100)h->Rebin(2);
// 	hMCSSStackmassT.Add(h,"hist");
// 	legendSSmassT.AddEntry(h,(*s)->GetName(),"f");
//       }
//     }
//     C.Clear();
//     hDataSSmassT.SetStats(0);
//     hDataSSmassT.GetYaxis()->SetTitle("Events / 2 GeV");
//     hDataSSmassT.GetXaxis()->SetTitle("mass_{T}    (GeV)");
//     hDataSSmassT.Draw("hist pe");
//     hMCSSStackmassT.Draw("same");
//     legendSSmassT.SetX1NDC(0.50);
//     legendSSmassT.SetX2NDC(0.78);
//     legendSSmassT.SetY1NDC(0.45);
//     legendSSmassT.SetY2NDC(0.8);
//     legendSSmassT.SetTextSize(.04);
//     legendSSmassT.Draw();
//     hDataSSmassT.Draw("pe same");
//     C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  


//     ///Make a plot of the massT Distribution in the OS sample
//     TH1F hDataOSmassT("hDataOSmassT","",100,0,200);
//     THStack hMCOSStackmassT("hMCOSStackmassT","MC");
//     TLegend legendOSmassT;   
//     legendOSmassT.SetLineColor(0);
//     legendOSmassT.SetBorderSize(1);
//     legendOSmassT.AddEntry(&hDataOSmassT,TString("Data (")+(long)totalDataLumi+" pb^{-1})","p");
//     for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//       if((*s)->getDataType()=="Data"){
// 	TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));    
// 	h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// 	if(h->GetXaxis()->GetNbins()!=100)h->Rebin(2);
// 	hDataOSmassT.Add(h);
// 	delete h;
//       }
//       if((*s)->getDataType()=="MC"){
// 	TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));    
// 	h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// 	if(h->GetXaxis()->GetNbins()!=100)h->Rebin(2);
// 	hMCOSStackmassT.Add(h,"hist");
// 	legendOSmassT.AddEntry(h,(*s)->GetName(),"f");
//       }
//     }
//     C.Clear();
//     hDataOSmassT.SetStats(0);
//     hDataOSmassT.GetYaxis()->SetTitle("Events / 2 GeV");
//     hDataOSmassT.GetXaxis()->SetTitle("mass_{T}    (GeV)");
//     hDataOSmassT.Draw("hist pe");
//     hMCOSStackmassT.Draw("same");
//     legendOSmassT.SetX1NDC(0.50);
//     legendOSmassT.SetX2NDC(0.78);
//     legendOSmassT.SetY1NDC(0.45);
//     legendOSmassT.SetY2NDC(0.8);
//     legendOSmassT.SetTextSize(.04);
//     legendOSmassT.Draw();
//     hDataOSmassT.Draw("pe same");
//     C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  
//     /////////////////////////////////////////////////////////////////
//   }

  

//   cout<<"Yields: WJetsSS="<<WJetsSSBelow<<" OtherMCSS="<<MCSSBelow<<" DataSS="<<DataSSBelow<<" WJetsSSCorr="<<WJetsSSBelowCorr<<endl;
//   cout<<"Yields: WJets="<<WJetsBelow<<" OtherMC="<<MCBelow<<" Data="<<DataBelow<<" WJetsCorr="<<WJetsBelowCorr<<endl;
//   /////////////////////////////////////////
//   ////////Correct the WJets 
//   //////////////////////////////////////////
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if(TString((*s)->GetName())=="WJetsToLNu_SS"){
//       TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
//       hmass->Scale(WJetsSSBelowCorr);
//     }
//     if(TString((*s)->GetName())=="WJetsToLNu"){
//       TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
//       hmass->Scale(WJetsBelowCorr);
//     }
//   }
  

  
//   cout<<" Calculating QCD: "<<endl;
//   ////////////////////////////////////
//   //Determine the QCD background
//   ///////////////////////////////////
//   TH1F hDataSS("hDataSS","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
//   TH1F hQCD("hQCD","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
//   hQCD.SetLineColor(1);
//   hQCD.SetLineWidth(1);
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
//     if((*s)->getDataType()=="Data_SS"){
//       hDataSS.Add((*s)->getHistoFromFile(histoname));  
//       hQCD.SetFillColor((*s)->getColor());
//     }
//   cout<<"Data_SS = "<<hDataSS.Integral()<<endl;
//   hQCD.Add(&hDataSS);
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
//     if((*s)->getDataType()=="MC_SS"){
//       hQCD.Add((*s)->getHistoFromFile(histoname),-1.0);
//     }
//   cout<<"QCD before OS/SS rescaling = "<<hQCD.Integral()<<endl;
//   hQCD.Scale(QCDOStoSSRatio_);
//   cout<<"QCD = "<<hQCD.Integral()<<endl;
//   /////////////////////////////////////

  

//   cout<<" Sorting the MC samples by plot order"<<endl;
//   /////////////////////////
//   ////sort the samples accordin to plot order
//   //////////////////////////
//   std::vector<Sample*> orderedMCSamples;
//   unsigned int orderindex=1;
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="MC"){      
//       for(std::vector<Sample*>::const_iterator sj=samples_.begin(); sj!=samples_.end(); ++sj){
// 	if((*sj)->getDataType()=="MC"){
// 	  if((*sj)->getPlotOrder()==orderindex){
// 	    cout<<orderindex<<" "<<(*sj)->GetName()<<endl;
// 	    orderedMCSamples.push_back((*sj));
// 	    orderindex++;		  
// 	  }
// 	}
//       }
//     }
//   }


//   cout<<" Merging Data: "<<endl;
//   ///////////////////////////////////
//   TH1F hData("hData","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
//   hData.SetLineWidth(1);
//   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data")
//       hData.Add((*s)->getHistoFromFile(histoname));  
//   }
//   cout<<"Data "<<(int)(hData.Integral())<<endl;

//   TLegend legend;
//   legend.SetLineColor(0);
//   legend.SetBorderSize(1);
//   legend.AddEntry(&hData,TString("Data  (")+(long)totalDataLumi+" pb^{-1})","p");

//   cout<<"Creating MCStack:"<<endl;
//   ////////////////////////
//   /////Stack the MC
//   ////////////////////////
//   THStack hMCStack("hMCStack","MC");//dont't set any of the regular histogram properties on the THStack will crash.
//   hMCStack.Add(&hQCD,"hist");
//   legend.AddEntry(&hQCD,"QCD","f");
//   Int_t MCTotalYield=hQCD.Integral();
//   cout<<hQCD.Integral()<<"  QCD"<<endl;
//   for(std::vector<Sample*>::const_iterator s=orderedMCSamples.begin(); s!=orderedMCSamples.end(); ++s){
//     TH1F* h=(TH1F*)((*s)->getHistoFromFile(histoname));    
//     hMCStack.Add(h,"hist");
//     legend.AddEntry(h,(*s)->GetName(),"f");
//     Int_t yield=(int)(h->Integral());
//     MCTotalYield+=yield;
//     cout<<yield<<" "<<h->GetName()<<endl;
//   }
//   cout<<"Total MC = "<<MCTotalYield<<endl;
  

//   cout<<" Creating Plot:"<<endl;
//   ///////////////////////
//   ///Make plot 
//   ///////////////////////
//   C.Clear();
//   hData.GetYaxis()->SetRangeUser(0, hData.GetMaximum()*1.2);
//   if(log){
//     hData.GetYaxis()->SetRangeUser(1, hData.GetMaximum()*1.3);
//     C.SetLogy(1);
//   }
//   if(axesrange){
//     hData.GetXaxis()->SetRangeUser(axesrange[0],axesrange[1]);
//     hData.GetYaxis()->SetRangeUser(axesrange[2],axesrange[3]);
//   }
//   hData.SetStats(0);
//   hData.GetXaxis()->SetTitle(xlabel);
//   hData.GetYaxis()->SetTitle(ylabel);
//   //hMC.Draw("hist");
//   hData.Draw("hist pe");
//   //////////////////////////
//   hMCStack.Draw("hist same");  
//   hData.Draw("hist pe same");
//   legend.SetX1NDC(legendcoords[0]);
//   legend.SetX2NDC(legendcoords[1]);
//   legend.SetY1NDC(legendcoords[2]);
//   legend.SetY2NDC(legendcoords[3]);
//   legend.SetTextSize(.04);
//   legend.Draw();
//   C.Update();
//   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  
//   /////////////////////////////////////////////////


//   cout<<"Making residual plot"<<endl;
//   C.Clear();
//   TLine line;
//   hData.Sumw2();
//   hData.Add(((TH1F*)(hMCStack.GetStack()->Last())),-1);
//   //hData.GetYaxis()->SetRangeUser(-5,5);
//   hData.GetYaxis()->SetTitle("Data - MC");
//   hData.Draw("hist pe");
//   if(axesrange)
//     line.DrawLine(axesrange[0],0,axesrange[1],0);
//   else line.DrawLine(hData.GetXaxis()->GetXmin(),0,hData.GetXaxis()->GetXmax(),0);
//   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  

  


//   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps]");

//   return 1;

// }



