#include "CMGTools/H2TauTau/interface/TauMuAnalysis.h"
#include <THStack.h>
#include <TString.h>
#include <TVector3.h>
#include <TLegend.h>

// #include "DataFormats/Common/interface/TriggerResults.h"
// #include "DataFormats/HLTReco/interface/TriggerEvent.h"
// #include "FWCore/Common/interface/TriggerNames.h"

//#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
//#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"

#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Run.h"




TauMuAnalysis::TauMuAnalysis():
  BaseAnalysis(),
  inputTag_(""),
  diTauNHisto_(NULL),
  diTauMassHisto_(NULL),
  diTauEtaHisto_(NULL),
  diTauPtHisto_(NULL),
  muPtHisto_(NULL),
  muIsoHisto_(NULL),
  tauPtHisto_(NULL),
  tauIsoHisto_(NULL),
  metHisto_(NULL),
  pZetaVisHisto_(NULL),
  pZetaMETHisto_(NULL),
  pZetaHisto_(NULL),
  diTauMassPUPHisto_(NULL),
  diTauEtaPUPHisto_(NULL),
  diTauPtPUPHisto_(NULL),
  muPtPUPHisto_(NULL),
  muIsoPUPHisto_(NULL),
  tauPtPUPHisto_(NULL),
  tauIsoPUPHisto_(NULL),
  tauTrackIsoPUPHisto_(NULL),
  tauNeutralIsoPUPHisto_(NULL),
  tauRhoPUPHisto_(NULL),
  metPUPHisto_(NULL),
  pZetaVisPUPHisto_(NULL),
  pZetaMETPUPHisto_(NULL),
  pZetaPUPHisto_(NULL)
{
}

TauMuAnalysis::TauMuAnalysis(const char * name):
  BaseAnalysis(name),
  inputTag_("cmgTauMuSelClean"),
  diTauNHisto_(NULL),
  diTauMassHisto_(NULL),
  diTauEtaHisto_(NULL),
  muPtHisto_(NULL),
  muIsoHisto_(NULL),
  muDxyHisto_(NULL),
  muDzHisto_(NULL),
  tauPtHisto_(NULL),
  tauIsoHisto_(NULL),
  tauDxyHisto_(NULL),
  tauDzHisto_(NULL),
  metHisto_(NULL),
  pZetaVisHisto_(NULL),
  pZetaMETHisto_(NULL),
  pZetaHisto_(NULL),
  diTauMassPUPHisto_(NULL),
  diTauEtaPUPHisto_(NULL),
  diTauPtPUPHisto_(NULL),
  muPtPUPHisto_(NULL),
  muIsoPUPHisto_(NULL),
  muDxyPUPHisto_(NULL),
  muDzPUPHisto_(NULL),
  tauPtPUPHisto_(NULL),
  tauIsoPUPHisto_(NULL),
  tauTrackIsoPUPHisto_(NULL),
  tauNeutralIsoPUPHisto_(NULL),
  tauRhoPUPHisto_(NULL),
  tauDxyPUPHisto_(NULL),
  tauDzPUPHisto_(NULL),
  metPUPHisto_(NULL),
  pZetaVisPUPHisto_(NULL),
  pZetaMETPUPHisto_(NULL),
  pZetaPUPHisto_(NULL)
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

  //print out the trigger eff:
  cout<<"trigger efficiencies"<<endl;
  for(Int_t p=0;p<100;p+=10)
    cout<<p<<" "<<triggerEff_.ratio((double)p)<<endl;


  
  return 1;
}

bool TauMuAnalysis::addHistos(Sample* s){
  if(!s) return 0;
  
  if(!BaseAnalysis::addHistos(s))return 0;
  

  //add histos to s
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauNHisto","",5,-.5,4.5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauMassHisto","",400,0,400))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauEtaHisto","",120,-6,6))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauPtHisto","",200,0,200))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muPtHisto","",200,0,200))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muIsoHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muDxyHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muDzHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauPtHisto","",200,0,200))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauIsoHisto","",100,0,5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauDxyHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauDzHisto","",100,0,.5))))return 0;  
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_metHisto","",200,0,200))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaVisHisto","",120,-10,110))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaMETHisto","",100,-40,60))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaHisto","",180,-140,40))))return 0;

  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauMassPUPHisto","",400,0,400))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauEtaPUPHisto","",120,-6,6))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauPtPUPHisto","",200,0,200))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muPtPUPHisto","",200,0,200))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muIsoPUPHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muDxyPUPHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muDzPUPHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauPtPUPHisto","",200,0,200))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauIsoPUPHisto","",100,0,5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauTrackIsoPUPHisto","",100,0,5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauNeutralIsoPUPHisto","",100,0,5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauRhoPUPHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauDxyPUPHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauDzPUPHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_metPUPHisto","",200,0,200))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaVisPUPHisto","",120,-10,110))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaMETPUPHisto","",100,-40,60))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaPUPHisto","",180,-140,40))))return 0;

  return 1;
}


bool TauMuAnalysis::getHistos(Sample* s){
  if(!s) return 0;
  
  if(!BaseAnalysis::getHistos(s))return 0;

  if(!(diTauNHisto_=(TH1F*)(s->getHisto("diTauNHisto"))))return 0;
  if(!(diTauMassHisto_=(TH1F*)(s->getHisto("diTauMassHisto"))))return 0;
  if(!(diTauEtaHisto_=(TH1F*)(s->getHisto("diTauEtaHisto"))))return 0;
  if(!(diTauPtHisto_=(TH1F*)(s->getHisto("diTauPtHisto"))))return 0;
  if(!(muPtHisto_=(TH1F*)(s->getHisto("muPtHisto"))))return 0;
  if(!(muIsoHisto_=(TH1F*)(s->getHisto("muIsoHisto"))))return 0;
  if(!(muPtHisto_=(TH1F*)(s->getHisto("muPtHisto"))))return 0;
  if(!(muDxyHisto_=(TH1F*)(s->getHisto("muDxyHisto"))))return 0;
  if(!(muDzHisto_=(TH1F*)(s->getHisto("muDzHisto"))))return 0;
  if(!(tauPtHisto_=(TH1F*)(s->getHisto("tauPtHisto"))))return 0;
  if(!(tauIsoHisto_=(TH1F*)(s->getHisto("tauIsoHisto"))))return 0;
  if(!(tauDxyHisto_=(TH1F*)(s->getHisto("tauDxyHisto"))))return 0;
  if(!(tauDzHisto_=(TH1F*)(s->getHisto("tauDzHisto"))))return 0;
  if(!(metHisto_=(TH1F*)(s->getHisto("metHisto"))))return 0;
  if(!(pZetaVisHisto_=(TH1F*)(s->getHisto("pZetaVisHisto"))))return 0;
  if(!(pZetaMETHisto_=(TH1F*)(s->getHisto("pZetaMETHisto"))))return 0;
  if(!(pZetaHisto_=(TH1F*)(s->getHisto("pZetaHisto"))))return 0;

  if(!(diTauMassPUPHisto_=(TH1F*)(s->getHisto("diTauMassPUPHisto"))))return 0;
  if(!(diTauEtaPUPHisto_=(TH1F*)(s->getHisto("diTauEtaPUPHisto"))))return 0;
  if(!(diTauPtPUPHisto_=(TH1F*)(s->getHisto("diTauPtPUPHisto"))))return 0;
  if(!(muPtPUPHisto_=(TH1F*)(s->getHisto("muPtPUPHisto"))))return 0;
  if(!(muIsoPUPHisto_=(TH1F*)(s->getHisto("muIsoPUPHisto"))))return 0;
  if(!(muDxyPUPHisto_=(TH1F*)(s->getHisto("muDxyPUPHisto"))))return 0;
  if(!(muDzPUPHisto_=(TH1F*)(s->getHisto("muDzPUPHisto"))))return 0;
  if(!(tauPtPUPHisto_=(TH1F*)(s->getHisto("tauPtPUPHisto"))))return 0;
  if(!(tauIsoPUPHisto_=(TH1F*)(s->getHisto("tauIsoPUPHisto"))))return 0;
  if(!(tauTrackIsoPUPHisto_=(TH1F*)(s->getHisto("tauTrackIsoPUPHisto"))))return 0;
  if(!(tauNeutralIsoPUPHisto_=(TH1F*)(s->getHisto("tauNeutralIsoPUPHisto"))))return 0;
  if(!(tauRhoPUPHisto_=(TH1F*)(s->getHisto("tauRhoPUPHisto"))))return 0;
  if(!(tauDxyPUPHisto_=(TH1F*)(s->getHisto("tauDxyPUPHisto"))))return 0;
  if(!(tauDzPUPHisto_=(TH1F*)(s->getHisto("tauDzPUPHisto"))))return 0;
  if(!(metPUPHisto_=(TH1F*)(s->getHisto("metPUPHisto"))))return 0;
  if(!(pZetaVisPUPHisto_=(TH1F*)(s->getHisto("pZetaVisPUPHisto"))))return 0;
  if(!(pZetaMETPUPHisto_=(TH1F*)(s->getHisto("pZetaMETPUPHisto"))))return 0;
  if(!(pZetaPUPHisto_=(TH1F*)(s->getHisto("pZetaPUPHisto"))))return 0;

  return 1;
}

float TauMuAnalysis::computePZeta(const cmg::Tau * tau1, const cmg::Muon * tau2, const cmg::BaseMET * met){
  if(!tau1 || !tau2 || !met) return 0.;
  //1) determine bisector axis between tau1 and tau2 momentum vectors
  //2) project visible energy onto zeta axis
  //3) project MET onto zeta axis
  TVector3 tau1P=TVector3(tau1->p4().x(),tau1->p4().y(),tau1->p4().z());
  TVector3 tau2P=TVector3(tau2->p4().x(),tau2->p4().y(),tau2->p4().z());
  TVector3 tau1PT=TVector3(tau1->p4().x(),tau1->p4().y(),0.);
  TVector3 tau2PT=TVector3(tau2->p4().x(),tau2->p4().y(),0.);
  TVector3 metPT=TVector3(met->p4().x(),met->p4().y(),0.);
  TVector3 zetaAxis=(tau1PT.Unit() + tau2PT.Unit()).Unit();
  Float_t pZetaVis=tau1PT*zetaAxis + tau2PT*zetaAxis;
  Float_t pZetaMET=metPT*zetaAxis;
  Float_t pZeta=pZetaVis+pZetaMET;
  
  return pZeta-1.5*pZetaVis;
}

float TauMuAnalysis::computeTauIso(const cmg::Tau * tau){
  return (tau->trackIso()+std::max( (tau->gammaIso() - tau->userData("rho")*TMath::Pi()*0.5*0.5), 0.0)); 
}

bool TauMuAnalysis::applySelections(const fwlite::Event * event){

  if(!BaseAnalysis::applySelections(event)) return 0;
  
  ///check the trigger
  edm::InputTag trigtag_("cmgTriggerObjectSel","","PAT");
  edm::Handle< std::vector<cmg::TriggerObject> > trig;
  event->getByLabel(trigtag_,trig);
  int pi=1; bool trigpass=0;
  if(triggerPaths_.size()==0)trigpass=1;//no trigger requirements desired
  for(std::vector<std::string>::const_iterator path=triggerPaths_.begin(); path!=triggerPaths_.end(); path++,pi++){
    //trig->begin()->printSelections(cout);
    //if(trig->begin()->hasSelection(path->c_str()) && trig->begin()->getSelection(path->c_str())){
    if(trig->begin()->hasSelection(path->c_str()))
      if(trig->begin()->getSelection(path->c_str())){
	triggerHisto_->Fill(pi);
	trigpass=1;
      }
  }
  if(!trigpass) return 0;
  triggerHisto_->Fill(triggerHisto_->GetXaxis()->GetNbins());
  
  ///aply PZeta selection 
  edm::InputTag mettag("cmgPFMET");
  edm::Handle< std::vector<cmg::BaseMET> > met;
  event->getByLabel(mettag,met);
   
  edm::InputTag particletag_(inputTag_.c_str());
  edm::Handle< std::vector<cmg::TauMu> > vec;
  event->getByLabel(particletag_, vec);
  int icand=0;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=vec->begin(); cand!=vec->end(); ++cand, icand++){    
    
    ////selections on the tau
    //     if(cand->leg1().pt()<20.0)continue;
    if(cand->leg1().tauID("againstMuonTight")!=1) continue;
    //if(computeTauIso(&(cand->leg1()))> 2.0) continue; // rho parameter isn't right
    

    ////selections on the muon
    //     if(cand->leg2().pt()<15.0)continue;
    //    if(cand->leg2().dxy()>0.045)continue;    
    //    if(cand->leg2().dz()>0.2)continue;        
    if(cand->leg2().relIso()>0.1)continue;    
    //    if(ptResolution/pt>0.1)continue;    

    ////other 
    if(computePZeta(&(cand->leg1()),&(cand->leg2()),&(*(met->begin())))<-20.0)continue;

    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0) return 0;
  

  return 1;
}

bool TauMuAnalysis::fillHistos(const fwlite::Event * event ){
  
  //fill the event level histograms first, mcPUPweight is determined here
  if(!BaseAnalysis::fillHistos(event)) return 0;
  
  edm::InputTag mettag("cmgPFMET");
  edm::Handle< std::vector<cmg::BaseMET> > met;
  event->getByLabel(mettag,met);
   
  //fill the diTau histograms
  diTauNHisto_->Fill(diTauSelList_.size());
  for(std::vector<cmg::TauMu>::const_iterator cand=diTauSelList_.begin(); cand!=diTauSelList_.end(); ++cand){
    
    float tauiso=computeTauIso(&(cand->leg1()));
    float pzeta=computePZeta(&(cand->leg1()),&(cand->leg2()),&(*(met->begin())));

    diTauMassHisto_->Fill(cand->mass());
    diTauEtaHisto_->Fill(cand->eta());
    diTauPtHisto_->Fill(cand->eta());

    metHisto_->Fill(met->begin()->energy());

    tauPtHisto_->Fill(cand->leg1().pt());
    tauIsoHisto_->Fill(tauiso);
    tauDxyHisto_->Fill(cand->leg1().dxy());
    tauDzHisto_->Fill(cand->leg1().dz());

    muPtHisto_->Fill(cand->leg2().pt());
    muIsoHisto_->Fill(cand->leg2().relIso());
    muDxyHisto_->Fill(cand->leg2().dxy());
    muDzHisto_->Fill(cand->leg2().dz());

    //pZetaVisHisto_->Fill(pZetaVis);
    //pZetaMETHisto_->Fill(pZetaMET);
    pZetaHisto_->Fill(pzeta);


    ////////////Reweighted histograms
    diTauMassPUPHisto_->Fill(cand->mass(),mcPUPWeight_);
    diTauEtaPUPHisto_->Fill(cand->eta(),mcPUPWeight_);
    diTauPtPUPHisto_->Fill(cand->eta(),mcPUPWeight_);  

    metPUPHisto_->Fill(met->begin()->energy(),mcPUPWeight_);
 
    tauPtPUPHisto_->Fill(cand->leg1().pt(),mcPUPWeight_);
    tauIsoPUPHisto_->Fill(tauiso,mcPUPWeight_);
    tauTrackIsoPUPHisto_->Fill(cand->leg1().trackIso(),mcPUPWeight_);
    tauNeutralIsoPUPHisto_->Fill(cand->leg1().gammaIso(),mcPUPWeight_);
    tauRhoPUPHisto_->Fill((cand->leg1().userData("rho"))*TMath::Pi()*0.5*0.5,mcPUPWeight_);
    tauDxyPUPHisto_->Fill(cand->leg1().dxy(),mcPUPWeight_);
    tauDzPUPHisto_->Fill(cand->leg1().dz(),mcPUPWeight_);

    muPtPUPHisto_->Fill(cand->leg2().pt(),mcPUPWeight_);
    muIsoPUPHisto_->Fill(cand->leg2().relIso(),mcPUPWeight_);
    muDxyPUPHisto_->Fill(cand->leg2().dxy(),mcPUPWeight_);
    muDzPUPHisto_->Fill(cand->leg2().dz(),mcPUPWeight_);
  
    //     pZetaVisPUPHisto_->Fill(pZetaVis,mcPUPWeight_);
    //     pZetaMETPUPHisto_->Fill(pZetaMET,mcPUPWeight_);
    pZetaPUPHisto_->Fill(pzeta,mcPUPWeight_);  

  }
  


 
  return 1;
}


bool TauMuAnalysis::scaleWeightHistos(Sample* s){

  if(!BaseAnalysis::scaleWeightHistos(s))return 0;


  diTauMassPUPHisto_->Scale(diTauMassHisto_->Integral()/diTauMassPUPHisto_->Integral());
  diTauEtaPUPHisto_->Scale(diTauEtaHisto_->Integral()/diTauEtaPUPHisto_->Integral());
  diTauPtPUPHisto_->Scale(diTauPtHisto_->Integral()/diTauPtPUPHisto_->Integral());  

  muPtPUPHisto_->Scale(muPtHisto_->Integral()/muPtPUPHisto_->Integral());
  muIsoPUPHisto_->Scale(muIsoHisto_->Integral()/muIsoPUPHisto_->Integral());
  muDxyPUPHisto_->Scale(muDxyHisto_->Integral()/muDxyPUPHisto_->Integral());
  muDzPUPHisto_->Scale(muDzHisto_->Integral()/muDzPUPHisto_->Integral());

  tauPtPUPHisto_->Scale(tauPtHisto_->Integral()/tauPtPUPHisto_->Integral());
  tauIsoPUPHisto_->Scale(tauIsoHisto_->Integral()/tauIsoPUPHisto_->Integral());
  tauDxyPUPHisto_->Scale(tauDxyHisto_->Integral()/tauDxyPUPHisto_->Integral());
  tauDzPUPHisto_->Scale(tauDzHisto_->Integral()/tauDzPUPHisto_->Integral());

  metPUPHisto_->Scale(metHisto_->Integral()/metPUPHisto_->Integral());
  pZetaVisPUPHisto_->Scale(pZetaHisto_->Integral()/pZetaVisPUPHisto_->Integral());
  pZetaMETPUPHisto_->Scale(pZetaHisto_->Integral()/pZetaMETPUPHisto_->Integral());
  pZetaPUPHisto_->Scale(pZetaHisto_->Integral()/pZetaPUPHisto_->Integral());

  //->Scale(->Integral()/->Integral());
  
  return 1;
}  

bool TauMuAnalysis::createHistos(TString samplename){

  std::vector<Sample*>::const_iterator s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++)
    if(samples_[i]->GetName()==samplename) sample_=samples_[i];

  if(!sample_){cout<<"sample "<<samplename<<" was not found"<<endl; return 0;}

  if(!addHistos(sample_))return 0;

  if(!getHistos(sample_))return 0;

  if(!sample_->getEvents()){
    cout<<" No fwlit::ChainEvent found in sample "<<sample_->GetName()<<endl;
    return 0;
  }
  triggerPaths_=sample_->getTrigPaths();
 

  fwlite::ChainEvent chain=*(sample_->getEvents());
  //note: cannot use the pointer to the chain in the sample, event loop crashes after first file

  //event loop
  Int_t ievt=0;
  Int_t goodevts=0;
  for(chain.toBegin(); !chain.atEnd() && ievt <  truncateEvents_; ++chain, ++ievt){
    if(ievt%printFreq_==0)cout<<ievt<<" done"<<endl;
    const fwlite::Event * event = chain.event();    

    if(!applySelections(event)) continue;

    if(!fillHistos(event)) return 0;
    goodevts++;

  }
  cout<<goodevts<<" events passed selections"<<endl;
  
  //
  if(!sample_->save())return 0;
  
  
  return 1;
}


bool  TauMuAnalysis::plotVisMass(){
  
  TCanvas C("C","TauMuAnalysis_plotVisMass.ps");
  C.Print("TauMuAnalysis_plotVisMass.ps[");
  
  TString histoname="diTauMassPUPHisto";
  float totalDataLumi=0.;
  Float_t Nevt[100];
  Int_t rebin=10;

  ///////////////////////////////
  ////plot the raw distributions
  ///////////////////////////
  C.Clear();
  C.Divide(2,2);
  Int_t padcounter=0;
  std::vector<Sample*>::const_iterator s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){
    if(samples_[i]->getDataType()=="Data"){
      C.cd(++padcounter);
      TH1F*hmass=(TH1F*)(samples_[i]->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<samples_[i]->GetName()<<endl; return 0;}
      hmass->Rebin(rebin);
      hmass->SetTitle(samples_[i]->GetName());
      hmass->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
      hmass->GetXaxis()->SetTitle("m_{vis}   (GeV)");
      hmass->Draw("hist");         
      totalDataLumi+=samples_[i]->getLumi()*samples_[i]->getProcessEff();
    }
  }
  cout<<" total Data lumi "<<totalDataLumi<<endl;
  C.Print("TauMuAnalysis_plotVisMass.ps");  

  C.Clear();
  C.Divide(2,2);
  padcounter=0;
  s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){
    if(samples_[i]->getDataType()=="Data_SS"){
      C.cd(++padcounter);
      TH1F*hmass=(TH1F*)(samples_[i]->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<samples_[i]->GetName()<<endl; return 0;}
      hmass->Rebin(rebin);
      hmass->SetTitle(samples_[i]->GetName());
      hmass->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
      hmass->GetXaxis()->SetTitle("m_{vis}   (GeV)");
      hmass->Draw("hist");         
    }
  }
  C.Print("TauMuAnalysis_plotVisMass.ps");  


  C.Clear();
  C.Divide(3,3);
  padcounter=0;
  s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){
    if(samples_[i]->getDataType()=="MC" || samples_[i]->getDataType()=="Signal" ){
      C.cd(++padcounter);
      samples_[i]->scaleLumi(totalDataLumi);
      TH1F*hmass=(TH1F*)(samples_[i]->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<samples_[i]->GetName()<<endl; return 0;}
      cout<<samples_[i]->GetName()<<" "<<hmass->Integral()<<endl;  
      hmass->Rebin(rebin);
      hmass->SetTitle(samples_[i]->GetName());
      hmass->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
      hmass->GetXaxis()->SetTitle("m_{vis}   (GeV)");
      hmass->Draw("hist");
    }
  }
  C.Print("TauMuAnalysis_plotVisMass.ps");  

  C.Clear();
  C.Divide(3,3);
  padcounter=0;
  s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){
    if(samples_[i]->getDataType()=="MC_SS"){
      C.cd(++padcounter);
      samples_[i]->scaleLumi(totalDataLumi);
      TH1F*hmass=(TH1F*)(samples_[i]->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<samples_[i]->GetName()<<endl; return 0;}
      hmass->Rebin(rebin);
      hmass->SetTitle(samples_[i]->GetName());
      hmass->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
      hmass->GetXaxis()->SetTitle("m_{vis}   (GeV)");
      hmass->Draw("hist");
    }
  }
  C.Print("TauMuAnalysis_plotVisMass.ps");  


  /////////////////////////
  ////table of number of events for each MC sample
  //////////////////////////
  s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){
    Nevt[i]=0;
    if(samples_[i]->getDataType()=="MC")
      Nevt[i]=samples_[i]->getHistoFromFile(histoname)->Integral();
  }
  Float_t NevtSort[100];
  unsigned int  NevtSortIndex[100];
  for(unsigned int i=0;i<samples_.size();i++){
    NevtSort[i]=0.0;    NevtSortIndex[i]=100;
    for(unsigned int j=0;j<samples_.size();j++){
      if(samples_[j]->getDataType()=="MC")
	if(Nevt[j]>NevtSort[i]&&(i==0 ? 1 : Nevt[j]<NevtSort[i-1])){
	  NevtSort[i]=Nevt[j];	  NevtSortIndex[i]=j;
	}
    }   
  }

 


  ////////////////////////////////////
  //Plot the weighted Same-Sign MC and Data
  ///////////////////////////////////
  Sample totalMC_SS("totalMC_SS",".");
  addHistos(&totalMC_SS);
  TH1F*hMC_SS=(TH1F*)totalMC_SS.getHisto(histoname);
  Sample totalData_SS("totalData_SS",".");
  addHistos(&totalData_SS);
  TH1F*hData_SS=(TH1F*)totalData_SS.getHisto(histoname);
  if(!hMC_SS || !hData_SS) return 0;
  hMC_SS->Rebin(rebin);
  hMC_SS->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
  hMC_SS->GetXaxis()->SetTitle("m_{vis}   (GeV)");
  hData_SS->Rebin(rebin);
  hData_SS->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
  hData_SS->GetXaxis()->SetTitle("m_{vis}   (GeV)");

  THStack hMCSSStack("hMCSSStack","MCSS");

  s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){
    if(samples_[i]->getDataType()=="Data_SS"){
      hData_SS->Add(samples_[i]->getHistoFromFile(histoname));  
      hData_SS->SetFillColor(samples_[i]->getColor());
      hData_SS->SetLineColor(samples_[i]->getColor());
    }
    if(samples_[i]->getDataType()=="MC_SS"){
      hMC_SS->Add(samples_[i]->getHistoFromFile(histoname));
      if(samples_[i]->getHistoFromFile(histoname)->Integral()/(hData_SS->Integral())>0.01)//only plot if significant
	hMCSSStack.Add(samples_[i]->getHistoFromFile(histoname),"hist");
    }
  }

  C.Clear();
  hData_SS->Draw("hist pe");
  hMCSSStack.Draw("hist same");
  hMC_SS->Draw("hist same");
  C.Print("TauMuAnalysis_plotVisMass.ps");  


  ////////////////////////
  /////Plot the stacked MC
  ////////////////////////
  THStack hMCStack("hMCStack","MC");
  //hMCStack.GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
  //hMCStack.GetXaxis()->SetTitle("m_{vis}   (GeV)");

  ///QCD background determined from SS Data - SS MC, should be biggest
  hData_SS->Add(hMC_SS,-1.0);
  hMCStack.Add(hData_SS,"hist");
  ///////add MC's
  for(unsigned int i=0;NevtSortIndex[i]!=100;i++){
    if(samples_[i]->getHistoFromFile(histoname)->Integral()/(2*hData_SS->Integral())>0.01)//only plot if significant
      hMCStack.Add(samples_[NevtSortIndex[i]]->getHistoFromFile(histoname),"hist");
  }
  C.Clear();
  hMCStack.Draw("hist");
  C.Print("TauMuAnalysis_plotVisMass.ps");  
  ///////////////////////

  ///print background yields
  cout<<"QCD "<<hData_SS->Integral()<<endl;
  for(unsigned int i=0;i<samples_.size();i++)
    if(NevtSort[i]>0.)cout<<samples_[NevtSortIndex[i]]->GetName()<<" "<<floor(NevtSort[i])<<endl;   


  ////////////////////////////////////
  //Plot the weighted MC and Data
  ///////////////////////////////////
  Sample totalMC("totalMC",".");
  addHistos(&totalMC);
  TH1F*hMC=(TH1F*)totalMC.getHisto(histoname);
  Sample totalData("totalData",".");
  addHistos(&totalData);
  TH1F*hData=(TH1F*)totalData.getHisto(histoname);
  Sample Signal("Signal",".");
  addHistos(&Signal);
  TH1F*hSignal=(TH1F*)Signal.getHisto(histoname);
  if(!hMC || !hData || !hSignal) return 0;
  hMC->Rebin(rebin);
  hMC->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
  hMC->GetXaxis()->SetTitle("m_{vis}   (GeV)");
  hData->Rebin(rebin);
  hData->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
  hData->GetXaxis()->SetTitle("m_{vis}   (GeV)");
  hSignal->Rebin(rebin);
  hSignal->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
  hSignal->GetXaxis()->SetTitle("m_{vis}   (GeV)");

  ///Sum all samples
  hMC->Add(hData_SS);
  s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){
    if(samples_[i]->getDataType()=="Data"){
      hData->Add(samples_[i]->getHistoFromFile(histoname));  
    }
    if(samples_[i]->getDataType()=="MC"){
      hMC->Add(samples_[i]->getHistoFromFile(histoname));
    }
    if(samples_[i]->getDataType()=="Signal"){
      hSignal->Add(samples_[i]->getHistoFromFile(histoname));
    }
  }


  C.Clear();
  hData->SetStats(0);
  hData->Draw("hist pe");
  hMCStack.Draw("hist same");
  hMC->Draw("hist same");
  hSignal->Draw("hist same");
  hData->Draw("hist pe same");
  C.Print("TauMuAnalysis_plotVisMass.ps");  
  /////////////////////////////////////////////////



  C.Print("TauMuAnalysis_plotVisMass.ps]");

  return 1;
}


bool TauMuAnalysis::plotDistribution(TString histoname, Int_t rebin, TString xlabel, TString ylabel, Float_t* legendcoords, Float_t* axesrange, bool log){

  
  TCanvas C("C",TString("TauMuAnalysis_")+histoname+".ps");
  C.Print(TString("TauMuAnalysis_")+histoname+".ps[");
  
  float totalDataLumi=0.;

  Int_t padcounter=0; 
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      C.cd(++padcounter);
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      hmass->Rebin(rebin);
      totalDataLumi+=(*s)->getLumi()*(*s)->getProcessEff();
      cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
    }
  }


  padcounter=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data_SS"){
      C.cd(++padcounter);
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      hmass->Rebin(rebin);
      cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
    }
  }


  padcounter=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC" || (*s)->getDataType()=="Signal" ){
      C.cd(++padcounter);
      (*s)->scaleLumi(totalDataLumi);
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      hmass->Rebin(rebin);
      cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
    }
  }
  

  padcounter=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC_SS"){
      C.cd(++padcounter);
      (*s)->scaleLumi(totalDataLumi);
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      hmass->Rebin(rebin);
      cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
    }
  }


  ////////////////////////////////////
  //Determine the QCD background
  ///////////////////////////////////
  Sample QCD("QCD",".");
  QCD.setDataType("MC");//MC type need below
  QCD.setPlotOrder(0);
  addHistos(&QCD);
  samples_.push_back(&QCD);//include the QCD for ordering as well
  TH1F*hQCD=(TH1F*)QCD.getHisto(histoname);
  hQCD->Rebin(rebin);

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data_SS"){
      hQCD->Add((*s)->getHistoFromFile(histoname));  
      hQCD->SetFillColor((*s)->getColor());
      //hQCD->SetLineColor((*s)->getColor());
    }
    if((*s)->getDataType()=="MC_SS"){
      ///QCD background determined from SS Data - SS MC, should be biggest
      hQCD->Add((*s)->getHistoFromFile(histoname),-1.0);
    }
  }

  /////////////////////////
  ////sort the samples accordin to plot order
  //////////////////////////
  std::vector<Sample*> orderedSamples;
  unsigned int orderindex=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC"){      
      for(std::vector<Sample*>::const_iterator sj=samples_.begin(); sj!=samples_.end(); ++sj){
	if((*sj)->getDataType()=="MC"){
	  if((*sj)->getPlotOrder()==orderindex){
	    orderedSamples.push_back((*sj));
	    orderindex++;		  
	  }
	}
      }
    }
  }


  ////////////////////////////////////
  //Plot the weighted MC and Data
  ///////////////////////////////////
  Sample totalMC("totalMC",".");
  addHistos(&totalMC);
  TH1F*hMC=(TH1F*)totalMC.getHisto(histoname);
  hMC->Rebin(rebin);

  Sample totalData("totalData",".");
  addHistos(&totalData);
  TH1F*hData=(TH1F*)totalData.getHisto(histoname);
  hData->Rebin(rebin);

  Sample Signal("Signal",".");
  addHistos(&Signal);
  TH1F*hSignal=(TH1F*)Signal.getHisto(histoname);
  hSignal->Rebin(rebin);

  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      hData->Add((*s)->getHistoFromFile(histoname));  
    }
    if((*s)->getDataType()=="MC"){
      if(TString((*s)->GetName())=="QCD") hMC->Add((*s)->getHisto(histoname));
      else hMC->Add((*s)->getHistoFromFile(histoname));
    }
    if((*s)->getDataType()=="Signal"){
      hSignal->Add((*s)->getHistoFromFile(histoname));
    }
  }

  ////////////////////////
  /////Stack the MC
  ////////////////////////
  TLegend legend;
  THStack hMCStack("hMCStack","MC");
  for(std::vector<Sample*>::const_iterator s=orderedSamples.begin(); s!=orderedSamples.end(); ++s){
    TH1* h=NULL;
    if(TString((*s)->GetName())=="QCD") h=(*s)->getHisto(histoname);
    else h=(*s)->getHistoFromFile(histoname);
    
    if((h->Integral()/hData->Integral())>0.01){//only plot if significant
      hMCStack.Add(h,"hist");
      legend.AddEntry(h,(*s)->GetName(),"f");
    }
  }

  
  ///////////////////////
  ///Make plot 
  ///////////////////////
  legend.AddEntry(hSignal,"H#rightarrow#tau#tau","l");
  legend.AddEntry(hData,TString("Data  (")+(long)totalDataLumi+" pb^{-1})","p");
  C.Clear();
  if(log){
    hData->GetYaxis()->SetRangeUser(1,hData->GetMaximum()*1.3);
    C.SetLogy(1);
  }else {
    hData->GetYaxis()->SetRangeUser(0,hData->GetMaximum()*1.2);
  }
  if(axesrange){
    hData->GetXaxis()->SetRangeUser(axesrange[0],axesrange[1]);
    hData->GetYaxis()->SetRangeUser(axesrange[2],axesrange[3]);
  }
  hData->SetStats(0);
  hData->GetXaxis()->SetTitle(xlabel);
  hData->GetYaxis()->SetTitle(ylabel);
  hData->Draw("hist pe");
  hMCStack.Draw("hist same");
  hMC->Draw("hist same");
  hSignal->Draw("hist same");
  hData->Draw("hist pe same");
  legend.SetX1NDC(legendcoords[0]);
  legend.SetX2NDC(legendcoords[1]);
  legend.SetY1NDC(legendcoords[2]);
  legend.SetY2NDC(legendcoords[3]);
  legend.SetTextSize(.04);
  legend.Draw();
  C.Update();
  C.Print(TString("TauMuAnalysis_")+histoname+".ps");  
  /////////////////////////////////////////////////


  /////////////////////////
  ////Print out the event yields
  //////////////////////////
  std::vector<Sample*> sizeOrderedSamples;  
  float sizeorder=10000000;
  for(std::vector<Sample*>::const_iterator si=samples_.begin(); si!=samples_.end(); ++si)
    if((*si)->getDataType()=="MC"){
      Sample* biggestSample=NULL;
      float sizelargest=0;
      for(std::vector<Sample*>::const_iterator sj=samples_.begin(); sj!=samples_.end(); ++sj)
	if((*sj)->getDataType()=="MC"){
	  TH1* h=NULL;
	  if(TString((*sj)->GetName())=="QCD") h = (*sj)->getHisto(histoname);
	  else h = (*sj)->getHistoFromFile(histoname);
	  
	  if(h->Integral() < (sizeorder-0.01) && h->Integral() > (sizelargest+0.01)){	    
	    biggestSample= *sj ;
	    sizelargest=h->Integral();
	  }
	}
      
      sizeOrderedSamples.push_back(biggestSample);    
      sizeorder = sizelargest;
    }
  Float_t totalMCYield=0.;
  for(std::vector<Sample*>::const_iterator s=sizeOrderedSamples.begin(); s!=sizeOrderedSamples.end(); ++s){
    Int_t yield = 0;
    if(TString((*s)->GetName())=="QCD")  yield = (*s)->getHisto(histoname)->Integral();
    else  yield = (*s)->getHistoFromFile(histoname)->Integral();

    cout<<yield<<" "<<(*s)->GetName()<<endl;
    totalMCYield+=yield;
  }
  cout<<"Total MC = "<<totalMCYield<<endl;
  cout<<"Higgs "<<hSignal->Integral()<<endl;
  cout<<"Data "<<hData->Integral()<<endl;




  C.Print(TString("TauMuAnalysis_")+histoname+".ps]");

  return 1;

}



    ///From CMGTools factory
//    edm::Handle<edm::TriggerResults> triggerResults;
//    //const edm::InputTag triggerResultsLabel = edm::InputTag( triggerResultsLabel_.label(), triggerResultsLabel_.instance(), processName_);
//    const edm::InputTag triggerResultsLabel = edm::InputTag("TriggerResults","","PAT");
//    //const edm::InputTag triggerResultsLabel = edm::InputTag("TriggerResults");
//    event->getByLabel(triggerResultsLabel, triggerResults);
//    const edm::TriggerNames& triggerNames = event->triggerNames(*triggerResults);
//    edm::TriggerNames::Strings const& names = triggerNames.triggerNames();
//    cout<<" names.size()"<<names.size()<<endl;
//    bool trigpass=0;  
//    for(edm::TriggerNames::Strings::const_iterator it = names.begin(); it != names.end(); ++it) {
//      const unsigned int i = triggerNames.triggerIndex(*it);
//      cout<<" "<<*it<<" "<<triggerResults->wasrun(i) <<" "<<triggerResults->accept(i)<<endl;
//      if(*it==TString("HLT_IsoMu12")){
//        cout<<"trigger "<<triggerResults->wasrun(i) <<" "<<triggerResults->accept(i)<<endl;
//        if(triggerResults->wasrun(i) && triggerResults->accept(i))trigpass=1;
//      }
//    }
//    if(!trigpass)continue;

    ///From Lorenzo
//    edm::Handle<pat::TriggerEvent> triggerHandle;
//    const edm::InputTag triggerResultsTag_ = edm::InputTag("triggerResults");
//    event->getByLabel(triggerResultsTag_,triggerHandle);
//    const pat::TriggerEvent * trigger = triggerHandle.product();
//    if(!trigger){cout<<"no trigger"<<endl; return 0;}
//    const pat::TriggerPath * triggerPath =  trigger->path("HLT_IsoMu12");
//    if(!triggerPath){cout<<" no path "<<endl; continue;}
//    cout<<"trigger "<<triggerPath->wasRun()<<" "<<triggerPath->wasAccept()<<endl;
    
// m + th channel
// HLT IsoMu12
// HLT IsoMu12 LooseIsoPFTau10
      // HLT_IsoMu12_LooseIsoPFTau10_v2
      // HLT_IsoMu12_v1
      // HLT_IsoMu15_v5
      // HLT_IsoMu17_CentralJet40_BTagIP_v2    
      // HLT_IsoMu17_v5
      // HLT_IsoMu24_v1
      // HLT_IsoMu30_v1

// e + th channel
// HLT Ele15 CaloIdVT CaloIsoT TrkIdT TrkIsoT LooseIsoPFTau15
      // HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2 
      // HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v2 
      // HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2 
      // HLT_Ele15_CaloIdVT_TrkIdT_LooseIsoPFTau15_v2  

// e + m channel
// HLT Mu8 Ele17 CaloIdL
// HLT Mu17 Ele8 CaloIdL
      // HLT_Mu8_Ele17_CaloIdL_v2      
      // HLT_Mu8_HT200_v3      
      // HLT_Mu8_Jet40_v3      
      // HLT_Mu8_Photon20_CaloIdVT_IsoT_v2     
      // HLT_Mu8_v1       
      // HLT_Mu17_CentralJet30_v2      
      // HLT_Mu17_CentralJet40_BTagIP_v2 
      // HLT_Mu17_DiCentralJet30_v2    
      // HLT_Mu17_Ele8_CaloIdL_v2      
      // HLT_Mu17_TriCentralJet30_v2   
