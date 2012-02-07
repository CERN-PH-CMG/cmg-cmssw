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
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muIsoHisto","",1000,0,1.0))))return 0; //WARNING: this binning choice is assumed below
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
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_transverseMassHisto","",200,0,200))))return 0;//WARNING: this binning choice is assumed below
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
  if(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS" 
     || sample_->getDataType()=="MCCat" || sample_->getDataType()=="MCCat_SS" 
     || sample_->getDataType()=="Signal"){  
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
  if(!(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS" 
       || sample_->getDataType()=="MCCat" || sample_->getDataType()=="MCCat_SS" 
       || sample_->getDataType()=="Signal")) return 0;

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
  
  //apply gen level separation here
  if( sample_->getGenEventType() !=0
      && sample_->getGenEventType()!=genEventType_) return 0;

  //separate the Z->ll at gen level here for truth-matching just to speed up processing, actual truth-matching done below
  if((sample_->getTruthEventType()==1 
     || sample_->getTruthEventType()==3 
     || sample_->getTruthEventType()==5
     || sample_->getTruthEventType()==11 
     || sample_->getTruthEventType()==13 
     || sample_->getTruthEventType()==15)
     && sample_->getTruthEventType()!=genEventType_) return 0;

  ////other 
  if(exceptcut!="dileptonveto") if(computeDiLeptonVeto()) return 0;

  //apply full selections on Tau and Mu cands
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=diTauList_->begin(); cand!=diTauList_->end(); ++cand){    
      
    ////selections on the tau
    if(exceptcut!="taueop") //this cut needs to be enabled in the baseline cuts
      if(cand->leg1().decayMode()==0&&cand->leg1().p()>0.) 
	if(cand->leg1().eOverP()<0.2) continue;
    //    if(((cand->leg1().leadChargedHadrECalEnergy()+cand->leg1().leadChargedHadrHCalEnergy())/cand->leg1().p())<0.2) continue; 
    if(exceptcut!="trigmatch"){
      bool match=0;
      if(sample_->getTrigPaths()->size()==0)match=1;//no match requirement
      for(std::vector<std::vector<std::string> >::const_iterator path=sample_->getTrigPaths()->begin(); path!=sample_->getTrigPaths()->end(); path++){
	if(trigObjMatch(cand->leg1().eta(),cand->leg1().phi(),(*path)[0],(*path)[1]))
	  match=1;
      }
      if(!match)continue;
    }

    ////selections on the muon
    //if(exceptcut!="mupt") if(cand->leg2().pt()<17.0) continue;//this cut needs to be updated in the python baseline cuts
    if(exceptcut!="trigmatch"){
      bool match=0;
      if(sample_->getTrigPaths()->size()==0) match=1;//no match requirement
      for(std::vector<std::vector<std::string> >::const_iterator path=sample_->getTrigPaths()->begin(); path!=sample_->getTrigPaths()->end(); path++){
	if(trigObjMatch(cand->leg2().eta(),cand->leg2().phi(),(*path)[0],(*path)[2]))
	  match=1;
      }
      if(!match)continue;
    }

    
    //selections on the mu-tau combination
    //this cut has no effect no effect after the full selections
    //if(exceptcut!="mutaudr") if(reco::deltaR(cand->leg1().eta(),cand->leg1().phi(),cand->leg2().eta(),cand->leg2().phi())<0.3) continue;    

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

  //lepton clean the jet list
  fillPFJetListLC(diTauSel_);
  
  ///apply selections for control regions here
  if(exceptcut!="muiso") if(diTauSel_->leg2().relIso(0.5)>0.1) return 0;      
  if(exceptcut!="massT") if(diTauSel_->mTLeg2()>40.0) return 0;

  ///SS or OS separation 
  if(sample_->getDataType()=="MC_SS" || sample_->getDataType()=="MCCat_SS" || sample_->getDataType()=="Data_SS" || sample_->getDataType()=="Embedded_SS"){
    if(fabs(diTauSel_->charge())!=2.)return 0;
  }else if(sample_->getDataType()=="MC" || sample_->getDataType()=="MCCat" || sample_->getDataType()=="Signal" || sample_->getDataType()=="Data" || sample_->getDataType()=="Embedded"){
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

  


  ////Define event weights here, these should not depend on the event categorization

  /////trigger efficiency weight
  triggerEffWeight_=1.;
  selectionEffWeight_=1.;
  if(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS" 
     || sample_->getDataType()=="MCCat" || sample_->getDataType()=="MCCat_SS" 
     || sample_->getDataType()=="Signal"
     || sample_->getDataType()=="Embedded" || sample_->getDataType()=="Embedded_SS"){

    ///trigger corrections
    if(sample_->getTrigPaths()->size()>0){//trigger applied--> apply a correction factor
      triggerEffWeight_ *= triggerEff_.effTau2011AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	                  /triggerEff_.effLooseTau15MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
      triggerEffWeight_ *= triggerEff_.effMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	                  /triggerEff_.effIsoMu15MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
    }else{//no trigger applied --> apply efficiency
      triggerEffWeight_ *= triggerEff_.effTau2011AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
      triggerEffWeight_ *= triggerEff_.effMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
    }

    //id+isolation corrections
    selectionEffWeight_ *= selectionEff_.effCorrMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());

  }
  
  //total event weight
  eventWeight_ = pupWeight_*embeddedGenWeight_*triggerEffWeight_*selectionEffWeight_;

  
  ////////////////////fill tree variables here
  tree_eventweight_=eventWeight_;
  tree_ditaumass_=diTauSel_->mass();

  tree_mupt_=diTauSel_->leg2().pt();
  tree_mueta_=diTauSel_->leg2().eta();
  tree_muiso_=diTauSel_->leg2().relIso(0.5);

  tree_taupt_=diTauSel_->leg1().pt();
  tree_taueta_=diTauSel_->leg1().eta();
  tree_tautruth_=truthMatchTau();
  tree_tauehop_=diTauSel_->leg1().eOverP();
  tree_taueop_=diTauSel_->leg1().leadChargedHadrEcalEnergy()/diTauSel_->leg1().p();
  tree_taudecaymode_=diTauSel_->leg1().decayMode();

  tree_transversemass_=diTauSel_->mTLeg2();
  tree_met_=diTauSel_->met().pt();
  tree_svfitmass_=diTauSel_->massSVFit();


  return 1;
}

bool TauMuAnalysis::fillHistos(TString tag){
  
  if(!diTauSel_){
    cout<<" diTauSel_ is NULL"<<endl; return 0;
  }

  sample_->incrementCounter(tag);//fill counter
  
  //event histos
  if(!BaseAnalysis::fillHistos(tag)) return 0;

  //di-tau histos
  if(!TauMuAnalysis::getHistos(tag)) return 0;

  //di-tau histos
  diTauMassHisto_->Fill(tree_ditaumass_,eventWeight_); 
  diTauPtHisto_->Fill(diTauSel_->pt(),eventWeight_);  
  diTauEtaHisto_->Fill(diTauSel_->eta(),eventWeight_);
  diTauMassSVFitHisto_->Fill(tree_svfitmass_,eventWeight_);   

  //tau histos
  tauPtHisto_->Fill(tree_taupt_,eventWeight_); 
  tauEtaHisto_->Fill(tree_taueta_,eventWeight_); 
  tauIsoHisto_->Fill(diTauSel_->leg1().relIso(0.5),eventWeight_);
  tauDxyHisto_->Fill(diTauSel_->leg1().dxy(),eventWeight_); 
  tauDzHisto_->Fill(diTauSel_->leg1().dz(),eventWeight_);
  tauDecayModeHisto_->Fill(tree_taudecaymode_);
  if(tree_taudecaymode_==0&&diTauSel_->leg1().p()>0.){
    tauEoPHisto_->Fill(tree_taueop_,eventWeight_);
    tauEoP1Histo_->Fill(tree_taueop_,eventWeight_);
    tauHoPHisto_->Fill(diTauSel_->leg1().leadChargedHadrHcalEnergy()/diTauSel_->leg1().p(),eventWeight_);
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

  //muon histos
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

  //MET histos
  transverseMassHisto_->Fill(tree_transversemass_,eventWeight_);    
  metHisto_->Fill(tree_met_,eventWeight_);  
  metphiHisto_->Fill(diTauSel_->met().phi(),eventWeight_);  

  //jet histos 
  njetHisto_->Fill(pfJetList_.size(),eventWeight_);
  njetLCHisto_->Fill(pfJetListLC_.size(),eventWeight_);

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
    sample_->cloneHistos("muiso");
    //sample_->cloneHistos("mupt");
    sample_->cloneHistos("massT");//gives normalization of WJets
    sample_->cloneHistos("trigmatch");

    sample_->cloneHistos("muisoJet0");
    sample_->cloneHistos("muisoJet1");
    sample_->cloneHistos("muisoJet2");

    sample_->cloneHistos("SM0");
    sample_->cloneHistos("SM1");
    sample_->cloneHistos("SM2");
    sample_->cloneHistos("SM9");

    sample_->cloneHistos("SM1QCD");//for Boosted QCD determination
    sample_->cloneHistos("SM2QCD");//for VBF QCD determination
    sample_->cloneHistos("SM1QCD2");//for Boosted QCD determination
    sample_->cloneHistos("SM2QCD2");//for VBF QCD determination

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
      if(applySelections("taueop")) if(!fillHistos("taueop")) return 0;
      if(applySelections("muiso")){
	if(!fillHistos("muiso")) return 0;
	if(eventCategorySM_==1 && 0.3<tree_muiso_ && tree_muiso_<0.5) if(!fillHistos("SM1QCD"))return 0;
	if(eventCategorySM_==2 && 0.3<tree_muiso_ && tree_muiso_<0.5) if(!fillHistos("SM2QCD"))return 0;
	if(eventCategorySM_==1 && 0.1<tree_muiso_) if(!fillHistos("SM1QCD2"))return 0;
	if(eventCategorySM_==2 && 0.1<tree_muiso_) if(!fillHistos("SM2QCD2"))return 0;

	if(pfJetListLC_.size()==0)if(!fillHistos("muisoJet0"))return 0;
	if(pfJetListLC_.size()==1)if(!fillHistos("muisoJet1"))return 0;
	if(pfJetListLC_.size()==2)if(!fillHistos("muisoJet2"))return 0;

      }
      if(applySelections("massT")) if(!fillHistos("massT")) return 0;   
      if(applySelections("trigmatch")) if(!fillHistos("trigmatch")) return 0;   
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
    if((*s)->getDataType()=="MCCat"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
    }
  }
  
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MCCat_SS"){
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
  

  return 1;
}

Float_t TauMuAnalysis::getWJetsScaleFactor(Int_t SMType){
  float ratio=0.;

  if(SMType==0) ratio = getWJetsSignalToSBFraction();
  
  if(SMType==1){
    for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
      if(TString((*s)->GetName())=="W2JetsToLNu")
	ratio=(*s)->getNorm();
  }
  
  if(SMType==2){
    for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
      if(TString((*s)->GetName())=="W3JetsToLNu")
	ratio=(*s)->getNorm();
  }
  
  cout<<"WJets Scale Factor : "<<ratio<<endl;
  
  return ratio;
}

Float_t TauMuAnalysis::getWJetsSignalToSBFraction(){

  //////needed by the data-card for limits before any rescalings
  Float_t WJetsSignal=0.;  Float_t WJetsSideBand=0.;
  if(transverseMassSignalMax_>transverseMassSideBandMin_)
    cout<<"Inconsistent values: transverseMassSignalMax_ > transverseMassSideBandMin_"<<endl;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if(TString((*s)->GetName())=="WJetsToLNu"){
      TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));
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
  float totalDataLumi=0.;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data")
      totalDataLumi+=(*s)->getLumi();
  cout<<"Total Data lumi = "<<totalDataLumi<<endl;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC" || (*s)->getDataType()=="MC_SS"
       || (*s)->getDataType()=="MCCat" || (*s)->getDataType()=="MCCat_SS"
       || (*s)->getDataType()=="Signal" ){
      (*s)->scale((*s)->getEffCorrFactor());
      (*s)->scale(totalDataLumi/(*s)->getLumi());
    }




  ///////////////////////////////
  /////Determine correction factor for WJets from massT sideband
  //////////////////////////////
  cout<<"Determining WJets  from sidebands"<<endl;

  Float_t WJetsSSSideCorr=1.; Float_t WJetsSSSide=0; Float_t MCSSSide=0; Float_t DataSSSide=0;
  Float_t WJetsSideCorr=1.; Float_t WJetsSide=0; Float_t MCSide=0; Float_t DataSide=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));

    if((*s)->getDataType()=="Data_SS")
      DataSSSide+=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX());  
    
    if((*s)->getDataType()=="MC_SS"){
      if(TString((*s)->GetName())=="WJetsToLNu_SS")
	WJetsSSSide=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX())*(*s)->getNorm();    
      else MCSSSide+=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX())*(*s)->getNorm();
    }

    if((*s)->getDataType()=="Data")
      DataSide+=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX()); 
    
    if((*s)->getDataType()=="MC"){
      if(TString((*s)->GetName())=="WJetsToLNu")
	WJetsSide=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX())*(*s)->getNorm();    
      else MCSide+=h->Integral(transverseMassSideBandMin_+1,h->GetNbinsX())*(*s)->getNorm();
    }
  }
  WJetsSSSideCorr=(DataSSSide-MCSSSide)/WJetsSSSide;
  WJetsSideCorr=(DataSide-MCSide)/WJetsSide;

  cout<<"WJetsSS Correction = "<<WJetsSSSideCorr<<endl;
  cout<<"WJetsOS Correction = "<<WJetsSideCorr<<endl;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if(TString((*s)->GetName())=="WJetsToLNu")(*s)->scale(WJetsSideCorr);
    if(TString((*s)->GetName())=="WJetsToLNu_SS")(*s)->scale(WJetsSSSideCorr);
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
      ZToTauTauMC=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral()*(*s)->getNorm();  
  }
  if(ZToTauTauMC==0. || EmbeddedOS==0.){
    cout<<"Warning!!!: bad ZToTauTauMC or EmbeddedOS yields: ZToTauTauMC="<<ZToTauTauMC<<" EmbeddedOS="<<EmbeddedOS<<endl;
  } else {
    cout<<"Rescaling Embedded samples by factor : "<<ZToTauTauMC/EmbeddedOS<<endl;
    for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
      if((*s)->getDataType()=="Embedded" || (*s)->getDataType()=="Embedded_SS")
	(*s)->scale(ZToTauTauMC/EmbeddedOS);
  }

  return 1;
}




TH1F* TauMuAnalysis::getTotalDataSS(TString histoname){

  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hData_SS","Data SS",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data_SS")
      h->Add((TH1F*)((*s)->getHistoFromFile(histoname)));  
 
  cout<<"TotalDataSS : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}

TH1F* TauMuAnalysis::getTotalData(TString histoname){

  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F(histoname+"hData","Data",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data")
      h->Add((TH1F*)((*s)->getHistoFromFile(histoname)));  
 
  cout<<"TotalData : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}

TH1F* TauMuAnalysis::getTotalMC(TString histoname, Int_t SMType){

  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F(histoname+"hTotalMC","TotalMC",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  h->Sumw2();

  TH1F*hZToTauTau=getZToTauTau(histoname);
  if(!hZToTauTau)return 0;
  h->Add(hZToTauTau);
  delete hZToTauTau;

  TH1F*hWJets=getWJets(histoname,SMType);
  if(!hWJets)return 0;
  h->Add(hWJets);
  delete hWJets;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC" && TString((*s)->GetName()) != "ZToTauTau" && TString((*s)->GetName()) != "WJetsToLNu")
      h->Add((TH1F*)((*s)->getHistoFromFile(histoname)),(*s)->getNorm());  
 
  cout<<"Total MC : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}

TH1F* TauMuAnalysis::getTotalEmbeddedSS(TString histoname){

  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hEmbeddedSS","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Embedded_SS")
      h->Add((TH1F*)((*s)->getHistoFromFile(histoname)),(*s)->getNorm());  
 
  cout<<"TotalEmbeddedSS : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}

TH1F* TauMuAnalysis::getTotalEmbedded(TString histoname){

  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hEmbedded","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Embedded")
      h->Add((TH1F*)((*s)->getHistoFromFile(histoname)),(*s)->getNorm());  
 
  cout<<"TotalEmbedded : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}


TH1F* TauMuAnalysis::getMCSS(TString histoname, Int_t SMType){

  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hMC_SS","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  h->Sumw2();

  TH1F*hZToTauTau=getZToTauTauSS(histoname);
  if(!hZToTauTau)return 0;
  h->Add(hZToTauTau);
  delete hZToTauTau;

  TH1F*hWJets=getWJetsSS(histoname,SMType);
  if(!hWJets)return 0;
  h->Add(hWJets);
  delete hWJets;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC_SS"  && TString((*s)->GetName()) != "ZToTauTau_SS" && TString((*s)->GetName()) != "WJetsToLNu_SS" )
      h->Add((TH1F*)((*s)->getHistoFromFile(histoname)),(*s)->getNorm());  
 
  cout<<"MCSS : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}


TH1F* TauMuAnalysis::getSample(TString samplename, TString histoname){

  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F(samplename+histoname,"",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  h->Sumw2();

  bool found=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if(TString((*s)->GetName())==samplename){
      h->Add((TH1F*)((*s)->getHistoFromFile(histoname)),(*s)->getNorm());
      h->SetTitle((*s)->getPlotLabel());
      found=1;
    }  
  
  if(!found){
    cout<<"Sample distribution "<<samplename<<" "<<histoname<<" not found"<<endl;
    return NULL;
  }

  cout<<samplename<<" : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}

TH1F* TauMuAnalysis::getQCD(TString histoname, Int_t SMType){

  TH1F* h=0;
  if(SMType==0)h=getQCDSS(histoname);
  if(SMType==1){
    TH1F* hs=getQCDSide(histoname);
    //TH1F* hs=getQCDFullSide(histoname,SMType);
    //NOTE that changing the method has to be done in sync with the getQCDScaleFactor() method below
    
    h=smearHisto(hs);
    delete hs;
  }

  if(SMType==2){
    TH1F* hs=getQCDSide(histoname);
    //TH1F* hs=getQCDFullSide(histoname,SMType);
    h=smearHisto(hs);
    delete hs;
  }

  double factor=getQCDScaleFactor(SMType);   
  cout<<"QCD scale factor: "<<factor<<endl;

  //qcd integral before scaling here must be integer for datacards
  int qcdint=(int)(h->Integral()+0.5);
  h->Scale((qcdint/h->Integral())*factor);

  cout<<"QCD : "<<h->Integral()<<endl;

  return h;
}


//////////Different QCD determinations

Float_t TauMuAnalysis::getQCDScaleFactor(Int_t SMType){
  float ratio=0.;
  
  if(SMType==0)ratio=QCDOStoSSRatio_;
 
  if(SMType==1 || SMType==2){

    //extrapolation factor for QCD is determined at the inclusive level
    //from the QCD distribution obtained from the SS samples

    TH1F* hDataSS=getTotalDataSS("muIsoHisto_muiso");//histo from inclusive with no muon iso cut
    if(!hDataSS){cout<<" Total Data SS not determined "<<endl; return 0;}
    TH1F* hMCSS=getMCSS("muIsoHisto_muiso");
    if(!hMCSS){cout<<" Total MC SS not determined "<<endl; return 0;}
    hDataSS->Add(hMCSS,-1);
    ratio=hDataSS->Integral(1,100)/hDataSS->Integral(301,500);
    delete hDataSS;
    delete hMCSS;

//     //determine the ratio to extrapolate into the signal region
//     TH1F* hDataSS=getTotalDataSS("muIsoHisto_muiso");//histo from inclusive with no muon iso cut
//     if(!hDataSS){cout<<" Total Data SS not determined "<<endl; return 0;}
//     TH1F* hMCSS=getMCSS("muIsoHisto_muiso");
//     if(!hMCSS){cout<<" Total MC SS not determined "<<endl; return 0;}
//     hDataSS->Add(hMCSS,-1);
//     float ratio=hDataSS->Integral(1,100)/hDataSS->Integral(101,1000);
//     delete hDataSS;
//     delete hMCSS;

  }

  return ratio;
}


TH1F* TauMuAnalysis::getQCDSS(TString histoname){

  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hQCD","QCD",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  h->Sumw2();
  
  TH1F* hDataSS=getTotalDataSS(histoname);
  h->Add(hDataSS);
  delete hDataSS;

  TH1F* hMCSS=getMCSS(histoname);
  h->Add(hMCSS,-1.);
  delete hMCSS;

  return h;
}

TH1F* TauMuAnalysis::getQCDSide(TString histoname){

  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname+"QCD"));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}
  TH1F* h=new TH1F("hSMQCD","SMQCD",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  h->Sumw2();
  
  TH1F* hData=getTotalData(histoname+"QCD");
  h->Add(hData);
  delete hData;

  return h;
}


TH1F* TauMuAnalysis::getQCDFullSide(TString histoname,Int_t SMType){

  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname+"QCD2"));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}
  TH1F* h=new TH1F("hSMQCD2","SMQCD2",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  h->Sumw2();

  TH1F* hData=getTotalData(histoname+"QCD2");
  h->Add(hData);
  delete hData;

  TH1F* hMC=getTotalMC(histoname+"QCD2",SMType);
  h->Add(hMC,-1);
  delete hMC;

  return h;
}

TH1F* TauMuAnalysis::getDiBoson(TString histoname){

  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* h=new TH1F("hVV","VV",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  h->Sumw2();  

  TH1F* hWW=getSample("WW",histoname);
  if(!hWW)return 0;
  h->Add(hWW);
  delete hWW;

  TH1F* hWZ=getSample("WZ",histoname);
  if(!hWZ)return 0;
  h->Add(hWZ);
  delete hWZ;

  TH1F* hZZ=getSample("ZZ",histoname);
  if(!hZZ)return 0;
  h->Add(hZZ);
  delete hZZ;

  cout<<"DiBoson : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}


TH1F* TauMuAnalysis::getZToTauTau(TString histoname){  
  //TH1F*h=getSample("ZToTauTau",histoname);
  TH1F*h=getTotalEmbedded(histoname); 
  return h;
}

TH1F* TauMuAnalysis::getZToTauTauSS(TString histoname){  
  //TH1F*h=getSample("ZToTauTau_SS",histoname);
  TH1F*h=getTotalEmbeddedSS(histoname);  
  return h;
}

TH1F* TauMuAnalysis::getWJets(TString histoname, Int_t SMType){

  TH1F*h=0;
  if(SMType==0)h=getSample("WJetsToLNu",histoname);
  if(SMType==1){
    TH1F*hs=getSample("W2JetsToLNu",histoname);
    h=smearHisto(hs);
    delete hs;
  }
  if(SMType==2){
    TH1F*hs=getSample("W3JetsToLNu",histoname);
    h=smearHisto(hs);
    delete hs;
  }

  return h;
}

TH1F* TauMuAnalysis::getWJetsSS(TString histoname, Int_t SMType){

  TH1F*h=0;
  if(SMType==0)h=getSample("WJetsToLNu_SS",histoname);
  if(SMType==1)h=getSample("W2JetsToLNu_SS",histoname);
  if(SMType==2)h=getSample("W3JetsToLNu_SS",histoname);
    
  return h;
}


TH1F* TauMuAnalysis::getTotalBackground(TString histoname, Int_t SMType){

  cout<<" Calculating Total Background: "<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}
  TH1F* h=new TH1F("hBackground","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  h->Sumw2();

  TH1F*hQCD=getQCD(histoname,SMType);
  if(!hQCD)return 0;
  h->Add(hQCD);
  delete hQCD;

  TH1F*hZToTauTau=getZToTauTau(histoname);
  if(!hZToTauTau)return 0;
  h->Add(hZToTauTau);
  delete hZToTauTau;

  TH1F*hWJets=getWJets(histoname,SMType);
  if(!hWJets)return 0;
  h->Add(hWJets);
  delete hWJets;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC" && TString((*s)->GetName()) != "ZToTauTau" && TString((*s)->GetName()) != "WJetsToLNu")
      h->Add((TH1F*)((*s)->getHistoFromFile(histoname)),(*s)->getNorm()); 

  
  return h;
}



bool TauMuAnalysis::plot(TString histoname, Int_t SMType, Int_t rebin, TString xlabel, TString ylabel, Float_t* legendcoords, Float_t* axesrange, bool log){
  if(!legendcoords || !axesrange){
    cout<<"No legend cordinates or axes ranges"<<endl; return 0;
  }
  
  if(!scaleHistos())return 0;


  TCanvas C("C",histoname);
  TString filename=outputpath_+"/TauMuAnalysis_"+histoname+"_QCD"+(long)SMType+".ps";
  C.Print(filename+"[");
  
  
//   ////Show Data SS and MC SS
//   TH1F* hDataQCD=0;
//   TH1F* hMCQCD=0;
//   if(SMType==0){
//     hDataQCD=getTotalDataSS(histoname);
//     hMCQCD=getMCSS(histoname);
//   }
//   if(SMType==1){
//     hDataQCD=getTotalData(histoname+"QCD");
//     //hMCQCD=getTotalMC(histoname+"QCD2",1);
//   } 
//   if(SMType==2){
//     hDataQCD=getTotalData(histoname+"QCD");
//     //hMCQCD=getTotalMC(histoname+"QCD2",2);
//   }
//   if(!hDataQCD){cout<<" Total Data for QCD not determined "<<endl; return 0;}
//   hDataQCD->Rebin(rebin);
  
//   C.Clear();
//   if(axesrange)hDataQCD->GetXaxis()->SetRangeUser(axesrange[0],axesrange[1]);
//   hDataQCD->Draw("pe");
//   if(hMCQCD){
//     hMCQCD->Rebin(rebin);
//     hMCQCD->Draw("hist same");
//   }
//   C.Print(filename); 
//   delete hDataQCD;
//   delete hMCQCD;
  


  ////Total Data
  TH1F* hData=getTotalData(histoname);
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
  hData->Rebin(rebin);

  ////Total MC
  ///must get here before getting individual components below because same histo object name is used for each component inside
  TH1F* hBkg=getTotalBackground(histoname,SMType);
  if(!hBkg){cout<<" Total Background not determined "<<endl; return 0;}
  hBkg->Rebin(rebin);
  hBkg->SetLineWidth(1);

  
  //
  TH1F*hQCD=getQCD(histoname,SMType);
  if(!hQCD)return 0;
  hQCD->Rebin(rebin);
  hQCD->SetLineWidth(1);
  hQCD->SetLineColor(QCDColor_);
  hQCD->SetFillColor(QCDColor_);

  TH1F*hWJetsToLNu=getWJets(histoname,SMType);
  if(!hWJetsToLNu)return 0;
  hWJetsToLNu->Rebin(rebin);
  hWJetsToLNu->SetLineWidth(1);
  hWJetsToLNu->SetLineColor(WJetsColor_);
  hWJetsToLNu->SetFillColor(WJetsColor_);

  TH1F*hEWK=(TH1F*)hWJetsToLNu->Clone("ElectroWeak");
  delete hWJetsToLNu;

  //combine Di-Bosons
  TH1F*hVV=getDiBoson(histoname);
  if(!hVV)return 0;
  hVV->Rebin(rebin);  
  hEWK->Add(hVV);
  delete hVV;

  //combine ZLJet
  TH1F*hZToLJet=getSample("ZToLJet",histoname);
  if(!hZToLJet)return 0;
  hZToLJet->Rebin(rebin);
  hEWK->Add(hZToLJet);
  delete hZToLJet;  

  //combine Z->MuMu
  TH1F*hZToMuMu=getSample("ZToMuMu",histoname);
  if(!hZToMuMu)return 0;
  hZToMuMu->Rebin(rebin);
  hZToMuMu->SetLineWidth(1);
  hZToMuMu->SetLineColor(ZMuMuColor_);
  hZToMuMu->SetFillColor(ZMuMuColor_);
  hEWK->Add(hZToMuMu);
  delete hZToMuMu;

  //
  TH1F*hTTJets=getSample("TTJets",histoname);
  if(!hTTJets)return 0;
  hTTJets->Rebin(rebin);
  hTTJets->SetLineWidth(1);
  hTTJets->SetLineColor(TTJetsColor_);
  hTTJets->SetFillColor(TTJetsColor_);
 

  ///Z->tau tau
  TH1F*hZToTauTau=getZToTauTau(histoname);
  if(!hZToTauTau)return 0;
  hZToTauTau->Rebin(rebin);
  hZToTauTau->SetLineWidth(1);
  hZToTauTau->SetLineColor(ZTauTauColor_);
  hZToTauTau->SetFillColor(ZTauTauColor_);


  cout<<" Summary of yields : "<<endl;
  cout<<"QCD "<<hQCD->Integral(0,hQCD->GetNbinsX()+1)<<endl;
  cout<<"ElectroWeak "<<hEWK->Integral(0,hEWK->GetNbinsX()+1)<<endl;
  cout<<"TTJets "<<hTTJets->Integral(0,hTTJets->GetNbinsX()+1)<<endl;
  cout<<"ZToTauTau "<<hZToTauTau->Integral(0,hZToTauTau->GetNbinsX()+1)<<endl;
  cout<<"Total Background "<<(int)(hBkg->Integral(0,hBkg->GetNbinsX()+1))<<endl;
  cout<<"Total Data "<<(int)(hData->Integral(0,hData->GetNbinsX()+1))<<endl;

  
  ////////////////////////
  cout<<"Creating Bkg Stack:"<<endl;
  THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
  hMCStack.Add(hQCD,"hist");
  hMCStack.Add(hEWK,"hist");
  hMCStack.Add(hTTJets,"hist");
  hMCStack.Add(hZToTauTau,"hist");

  //create legend key
  TLegend legend;
  legend.SetLineColor(0);
  legend.SetBorderSize(1);
  legend.AddEntry(hData,hData->GetTitle(),"p");
  legend.AddEntry(hZToTauTau,"Z#rightarrow#tau^{+}#tau^{-}","f");
  legend.AddEntry(hTTJets,"t#bar{t}","f");
  legend.AddEntry(hEWK,"ElectroWeak","f");
  legend.AddEntry(hQCD,"QCD","f");


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
  C.Print(filename);  

  
  TH1F*hRatio=(TH1F*)hData->Clone("hRatio");//get here before make stats 
  TH1F*hResid=(TH1F*)hData->Clone("hResid");//get here before make stats 
  TLine line;

  ///////////////////////////////////
  //show the histogram stats
  C.Clear();
  hData->SetStats(1);
  hData->Draw();
  C.Print(filename); 
  C.Clear();
  hBkg->GetXaxis()->SetRange(hData->GetXaxis()->GetFirst(),hData->GetXaxis()->GetLast());
  hBkg->GetYaxis()->SetRange(hData->GetYaxis()->GetFirst(),hData->GetYaxis()->GetLast());
  hBkg->SetStats(1);
  hBkg->Draw();
  C.Print(filename); 


  ////////////////////////////////////
  cout<<"Making ratio plot"<<endl;
  C.Clear();
  hRatio->Divide(hBkg);
  hRatio->GetYaxis()->SetTitle("Data/Background");
  hRatio->SetStats(0);
  hRatio->GetYaxis()->SetRangeUser(.5,1.5);
  hRatio->Draw("hist pe");
  if(axesrange)
    line.DrawLine(axesrange[0],1,axesrange[1],1);
  else line.DrawLine(hRatio->GetXaxis()->GetXmin(),0,hRatio->GetXaxis()->GetXmax(),0);
  C.Print(filename);  
  delete hRatio;

  cout<<"Making residual plot"<<endl;
  C.Clear();
  hResid->Add(hBkg,-1);
  hResid->GetYaxis()->SetTitle("Data - Background");
  hResid->SetStats(0);
  hResid->Draw("hist pe");
  if(axesrange)
    line.DrawLine(axesrange[0],0,axesrange[1],0);
  else line.DrawLine(hResid->GetXaxis()->GetXmin(),0,hResid->GetXaxis()->GetXmax(),0);
  C.Print(filename);  
  delete hResid;


 
  C.Print(filename+"]");
  cout<<"closed ps file"<<endl;

  //clean up
  delete hQCD;
  delete hEWK;
  delete hTTJets;
  delete hZToTauTau;
  delete hBkg;
  delete hData;


  return 1;

}

