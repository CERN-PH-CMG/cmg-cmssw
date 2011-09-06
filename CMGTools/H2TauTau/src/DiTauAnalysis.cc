#include "CMGTools/H2TauTau/interface/DiTauAnalysis.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include <THStack.h>
#include <TString.h>
#include <TVector3.h>

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




DiTauAnalysis::DiTauAnalysis():
  BaseAnalysis(),
  inputTag_(""),
  nDiTauHisto_(NULL),
  diTauMassHisto_(NULL),
  diTauEtaHisto_(NULL),
  diTauPtHisto_(NULL),
  metHisto_(NULL),
  pZetaVisHisto_(NULL),
  pZetaMETHisto_(NULL),
  pZetaHisto_(NULL),
  diTauMassPUPWeightHisto_(NULL),
  diTauEtaPUPWeightHisto_(NULL),
  diTauPtPUPWeightHisto_(NULL),
  metPUPWeightHisto_(NULL),
  pZetaVisPUPWeightHisto_(NULL),
  pZetaMETPUPWeightHisto_(NULL),
  pZetaPUPWeightHisto_(NULL)
{
}

DiTauAnalysis::DiTauAnalysis(const char * name):
  BaseAnalysis(name),
  inputTag_("cmgDiTauSelClean"),
  nDiTauHisto_(NULL),
  diTauMassHisto_(NULL),
  diTauEtaHisto_(NULL),
  metHisto_(NULL),
  pZetaVisHisto_(NULL),
  pZetaMETHisto_(NULL),
  pZetaHisto_(NULL),
  diTauMassPUPWeightHisto_(NULL),
  diTauEtaPUPWeightHisto_(NULL),
  diTauPtPUPWeightHisto_(NULL),
  metPUPWeightHisto_(NULL),
  pZetaVisPUPWeightHisto_(NULL),
  pZetaMETPUPWeightHisto_(NULL),
  pZetaPUPWeightHisto_(NULL)
{
 
}

DiTauAnalysis::~DiTauAnalysis(){
}


bool DiTauAnalysis::init(){
  //for mu-tau
  //triggerPaths_.push_back("HLT_IsoMu15");//for run < 163261
  triggerPaths_.push_back("HLT_IsoMu12_LooseIsoPFTau10_*");
  triggerPaths_.push_back("HLT_IsoMu15_LooseIsoPFTau15_*");
  triggerPaths_.push_back("HLT_IsoMu15_LooseIsoPFTau20_*");


//   //for e-tau                            
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1");
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2");
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4");
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v6");
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2");
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8");
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v9");
//   triggerPaths_.push_back("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v1");
//   triggerPaths_.push_back("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v2");


  //for e-mu
//   triggerPaths_.push_back("HLT_Mu8_Ele17_CaloIdL_v1");
//   triggerPaths_.push_back("HLT_Mu17_Ele8_CaloIdL_v1");
//   triggerPaths_.push_back("HLT_Mu8_Ele17_CaloIdL_v2");
//   triggerPaths_.push_back("HLT_Mu17_Ele8_CaloIdL_v2");


  if(triggerPaths_.size()>=20){ cout<<" too many trigger paths"<<endl; return 0;}
  for(std::vector<std::string>::const_iterator path=triggerPaths_.begin(); path!=triggerPaths_.end(); path++){
    cout<<"Trigger Path: "<<path->c_str()<<endl;
  }


  if(!BaseAnalysis::init())return 0;

  std::vector<Sample*>::const_iterator s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){
    
    cout<<"DiTauAnalysis  Initialized sample "<<samples_[i]->GetName()<<" "<<samples_[i]->GetTitle()<<endl;
  }
  
  return 1;
}

bool DiTauAnalysis::addHistos(Sample* s){
  if(!s) return 0;
  
  if(!BaseAnalysis::addHistos(s))return 0;
  
  //add histos to s
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_nDiTauHisto","",5,-.5,4.5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauMassHisto","",300,0,300))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauEtaHisto","",120,-6,6))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauPtHisto","",100,0,100))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_metHisto","",100,0,100))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaVisHisto","",100,-10,90))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaMETHisto","",100,-30,30))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaHisto","",150,-100,50))))return 0;

  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauMassPUPWeightHisto","",300,0,300))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauEtaPUPWeightHisto","",120,-6,6))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauPtPUPWeightHisto","",100,0,100))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_metPUPWeightHisto","",100,0,100))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaVisPUPWeightHisto","",100,-10,90))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaMETPUPWeightHisto","",100,-30,30))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaPUPWeightHisto","",150,-100,50))))return 0;

  return 1;
}


bool DiTauAnalysis::getHistos(Sample* s){
  if(!s) return 0;
  
  if(!BaseAnalysis::getHistos(s))return 0;
  
  if(!(nDiTauHisto_=(TH1F*)(s->getHisto("nDiTauHisto"))))return 0;
  if(!(diTauMassHisto_=(TH1F*)(s->getHisto("diTauMassHisto"))))return 0;
  if(!(diTauEtaHisto_=(TH1F*)(s->getHisto("diTauEtaHisto"))))return 0;
  if(!(diTauPtHisto_=(TH1F*)(s->getHisto("diTauPtHisto"))))return 0;
  if(!(metHisto_=(TH1F*)(s->getHisto("metHisto"))))return 0;
  if(!(pZetaVisHisto_=(TH1F*)(s->getHisto("pZetaVisHisto"))))return 0;
  if(!(pZetaMETHisto_=(TH1F*)(s->getHisto("pZetaMETHisto"))))return 0;
  if(!(pZetaHisto_=(TH1F*)(s->getHisto("pZetaHisto"))))return 0;

  if(!(diTauMassPUPWeightHisto_=(TH1F*)(s->getHisto("diTauMassPUPWeightHisto"))))return 0;
  if(!(diTauEtaPUPWeightHisto_=(TH1F*)(s->getHisto("diTauEtaPUPWeightHisto"))))return 0;
  if(!(diTauPtPUPWeightHisto_=(TH1F*)(s->getHisto("diTauPtPUPWeightHisto"))))return 0;
  if(!(metPUPWeightHisto_=(TH1F*)(s->getHisto("metPUPWeightHisto"))))return 0;
  if(!(pZetaVisPUPWeightHisto_=(TH1F*)(s->getHisto("pZetaVisPUPWeightHisto"))))return 0;
  if(!(pZetaMETPUPWeightHisto_=(TH1F*)(s->getHisto("pZetaMETPUPWeightHisto"))))return 0;
  if(!(pZetaPUPWeightHisto_=(TH1F*)(s->getHisto("pZetaPUPWeightHisto"))))return 0;

  return 1;
}



bool DiTauAnalysis::fillHistos(const fwlite::Event * event ){
  
  ///check the trigger
  edm::InputTag trigtag_("cmgTriggerObjectSel","","PAT");
  edm::Handle< std::vector<cmg::TriggerObject> > trig;
  event->getByLabel(trigtag_,trig);
  int pi=1; bool trigpass=0;
  for(std::vector<std::string>::const_iterator path=triggerPaths_.begin(); path!=triggerPaths_.end(); path++,pi++){
    if(trig->begin()->getSelectionRegExp(path->c_str())){
      triggerHisto_->Fill(pi);
      trigpass=1;
    }
  }
  if(trigpass)triggerHisto_->Fill(triggerHisto_->GetXaxis()->GetNbins());
  else return 1; //remove events which do not pass the desired triggers
  

  //fill the event level histograms
  if(!BaseAnalysis::fillHistos(event)) return 0;

  
  ////////////////////////////
  //Fill histograms only after fully selected events
  //Needs to be fixed to work with PUP weight currently determied in BaseAnalysis::fillHistos
  //////////////////////////

  ///aply PZeta selection here so that EventHistos are filled only if there is at least one diTau selected.
  edm::InputTag mettag("cmgPFMET");
  edm::Handle< std::vector<cmg::BaseMET> > met;
  event->getByLabel(mettag,met);
  metHisto_->Fill(met->begin()->energy());
  metPUPWeightHisto_->Fill(met->begin()->energy(),mcPUPWeight_);
    
  edm::InputTag particletag_(inputTag_.c_str());
  edm::Handle< std::vector<cmg::DiTau> > vec;
  event->getByLabel(particletag_, vec);
  int nselected=0;
  Int_t icand=0;
  bool candsel[20];
  for(std::vector<cmg::DiTau>::const_iterator cand=vec->begin(); cand!=vec->end(); ++cand, icand++){    
    //compute Pzeta 
    //1) determine bisector axis between tau - tau
    //2) project visible energy onto z axis
    //3) project MET onto z axis
    TVector3 tau1P=TVector3(cand->leg1().p4().x(),cand->leg1().p4().y(),cand->leg1().p4().z());
    TVector3 tau2P=TVector3(cand->leg2().p4().x(),cand->leg2().p4().y(),cand->leg2().p4().z());
    TVector3 metP=TVector3(met->begin()->p4().x(),met->begin()->p4().y(),met->begin()->p4().z());
    TVector3 bisectDir=(TVector3(tau1P.X(),tau1P.Y(),tau1P.Z()).Unit() + TVector3(tau2P.X(),tau2P.Y(),tau2P.Z()).Unit()).Unit();
    Float_t pZetaVis=tau1P*bisectDir + tau2P*bisectDir;
    Float_t pZetaMET=metP*bisectDir;
    Float_t pZeta=pZetaVis+pZetaMET;
    Float_t pZetaCut=pZeta-1.5*pZeta;
    pZetaVisHisto_->Fill(pZetaVis);
    pZetaMETHisto_->Fill(pZetaMET);
    pZetaHisto_->Fill(pZetaCut);
    pZetaVisPUPWeightHisto_->Fill(pZetaVis,mcPUPWeight_);
    pZetaMETPUPWeightHisto_->Fill(pZetaMET,mcPUPWeight_);
    pZetaPUPWeightHisto_->Fill(pZetaCut,mcPUPWeight_);
    
    candsel[icand]=1;
    if(pZetaCut<-20.0){ candsel[icand]=0; continue;}
    
    nselected++;
  }
  //if(nselected==0) return 1;


  //fill the diTau histograms
  icand=0;
  for(std::vector<cmg::DiTau>::const_iterator cand=vec->begin(); cand!=vec->end(); ++cand, icand++){
    if(candsel[icand]==0) continue; //remove candidates not selected above

    diTauMassHisto_->Fill(cand->mass());
    diTauEtaHisto_->Fill(cand->eta());
    diTauPtHisto_->Fill(cand->eta());

    diTauMassPUPWeightHisto_->Fill(cand->mass(),mcPUPWeight_);
    diTauEtaPUPWeightHisto_->Fill(cand->eta(),mcPUPWeight_);
    diTauPtPUPWeightHisto_->Fill(cand->eta(),mcPUPWeight_);    
  }
  nDiTauHisto_->Fill(icand);


 
  return 1;
}


bool DiTauAnalysis::scaleWeightHistos(Sample* s){

  if(!BaseAnalysis::scaleWeightHistos(s))return 0;

  diTauMassPUPWeightHisto_->Scale(diTauMassHisto_->Integral()/diTauMassPUPWeightHisto_->Integral());
  diTauEtaPUPWeightHisto_->Scale(diTauEtaHisto_->Integral()/diTauEtaPUPWeightHisto_->Integral());
  diTauPtPUPWeightHisto_->Scale(diTauPtHisto_->Integral()/diTauPtPUPWeightHisto_->Integral());  
  metPUPWeightHisto_->Scale(metHisto_->Integral()/metPUPWeightHisto_->Integral());
  pZetaVisPUPWeightHisto_->Scale(pZetaHisto_->Integral()/pZetaVisPUPWeightHisto_->Integral());
  pZetaMETPUPWeightHisto_->Scale(pZetaHisto_->Integral()/pZetaMETPUPWeightHisto_->Integral());
  pZetaPUPWeightHisto_->Scale(pZetaHisto_->Integral()/pZetaPUPWeightHisto_->Integral());

  //->Scale(->Integral()/->Integral());
  
  return 1;
}  

bool DiTauAnalysis::createHistos(TString samplename){

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

  fwlite::ChainEvent chain=*(sample_->getEvents());
  //note: cannot use the pointer to the chain in the sample, event loop crashes after first file

  //event loop
  Int_t ievt=0;
  for(chain.toBegin(); !chain.atEnd() && ievt <  truncateEvents_; ++chain, ++ievt){
    if(ievt%printFreq_==0)cout<<ievt<<" done"<<endl;
    const fwlite::Event * event = chain.event();    

    if(!fillHistos(event)) return 0;

  }

  //rescale
  if(!scaleWeightHistos(sample_))return 0;
  
  
  return 1;
}


bool  DiTauAnalysis::createTotalMass(){
  
  TCanvas C("C","DiTauAnalysis_createTotalMass.ps");
  C.Print("DiTauAnalysis_createTotalMass.ps[");
  
  TString histoname="diTauMassPUPWeightHisto";
  float totalDataLumi=0.;
  Float_t Nevt[100];
  Int_t rebin=5;

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
      totalDataLumi+=samples_[i]->getLumi();
    }
  }
  C.Print("DiTauAnalysis_createTotalMass.ps");  

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
  C.Print("DiTauAnalysis_createTotalMass.ps");  


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
  C.Print("DiTauAnalysis_createTotalMass.ps");  

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
  C.Print("DiTauAnalysis_createTotalMass.ps");  


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

  s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){
    if(samples_[i]->getDataType()=="Data_SS"){
      hData_SS->Add(samples_[i]->getHistoFromFile(histoname));  
      hData_SS->SetFillColor(samples_[i]->getColor());
      hData_SS->SetLineColor(samples_[i]->getColor());
    }
    if(samples_[i]->getDataType()=="MC_SS"){
      hMC_SS->Add(samples_[i]->getHistoFromFile(histoname));
    }
  }

  C.Clear();
  hData_SS->Draw("hist pe");
  hMC_SS->Draw("hist same");
  C.Print("DiTauAnalysis_createTotalMass.ps");  


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
  C.Print("DiTauAnalysis_createTotalMass.ps");  
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
  C.Print("DiTauAnalysis_createTotalMass.ps");  
  /////////////////////////////////////////////////



  C.Print("DiTauAnalysis_createTotalMass.ps]");

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
