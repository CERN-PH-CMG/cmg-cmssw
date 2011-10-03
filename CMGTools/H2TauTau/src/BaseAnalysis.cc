#include "CMGTools/H2TauTau/interface/BaseAnalysis.h"

//#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"


// ClassImp(BaseAnalysis)
// ;

BaseAnalysis::BaseAnalysis():
  TNamed("BaseAnalysis","BaseAnalysis"),
  sample_(NULL),
  truncateEvents_(100000000),
  printFreq_(100),
  mcPUPWeight_(1),
  runNumberHisto_(NULL),
  nPUPVertexHisto_(NULL),
  nVertexHisto_(NULL),
  vertexXYHisto_(NULL),
  vertexZHisto_(NULL),
  nVertexPUPWeightHisto_(NULL),
  triggerHisto_(NULL),
  mcPUPWeightHisto_(NULL)
{
}

BaseAnalysis::BaseAnalysis(const char * name):
  TNamed(name,name),
  sample_(NULL),
  truncateEvents_(100000000),
  printFreq_(100),
  mcPUPWeight_(1),
  runNumberHisto_(NULL),
  nPUPVertexHisto_(NULL),
  nVertexHisto_(NULL),
  vertexXYHisto_(NULL),
  vertexZHisto_(NULL),
  nVertexPUPWeightHisto_(NULL),
  triggerHisto_(NULL),
  mcPUPWeightHisto_(NULL)
{
  //addSample(new Sample("RelValZTT","Tuples/CMGDiTau/RelValZTT_GEN_SIM_RECO_START42_V12_v2"));    
}

BaseAnalysis::~BaseAnalysis(){
  
  delete mcPUPWeightHisto_;
}


bool BaseAnalysis::init(){
  
  //read the pile-up weights
  TFile DataPUP("tuples/Pileup_2011_EPS_8_jul.root","read");
  TFile MCPUP("tuples/MCPUP.root","read");
  gROOT->cd();
  TH1D* DataPUPHisto=(TH1D*)DataPUP.Get("pileup");
  TH1D* MCPUPHisto=(TH1D*)MCPUP.Get("MCPUPHisto");
  if(!DataPUPHisto || !MCPUPHisto){
    cout<<"DataPUPHisto or MCPUPHisto do not exist"<<endl;
    return 0;
  }
  if(DataPUPHisto->GetXaxis()->GetNbins()<25 || MCPUPHisto->GetXaxis()->GetNbins()< 25){
    cout<<"DataPUPHisto does not have same number of bins as  MCPUPHisto"<<endl;
    return 0;
  }
  DataPUPHisto->Scale(1./DataPUPHisto->Integral());
  MCPUPHisto->Scale(1./MCPUPHisto->Integral());
  mcPUPWeightHisto_=new TH1F("mcPUPWeightHisto","",25,-.5,24.5);
  for(int b=1;b<=25;b++)
    if(MCPUPHisto->GetBinContent(b)>0) 
      mcPUPWeightHisto_->SetBinContent(b,DataPUPHisto->GetBinContent(b)/MCPUPHisto->GetBinContent(b));
    else mcPUPWeightHisto_->SetBinContent(b,0);

 

  ///init each sample
  std::vector<Sample*>::const_iterator s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){
    if(!(samples_[i]->init())) return 0;
 
    cout<<"BaseAnalysis Initialized sample "<<samples_[i]->GetName()<<" "<<samples_[i]->GetTitle()<<endl;
  }
  
  return 1;
}

bool BaseAnalysis::addHistos(Sample* s){
  if(!s)return 0;
     
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_triggerHisto","",20,0.5,20.5))))return 0;  

  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_runNumberHisto","",1500,160000,175000))))return 0;
  if(!s->addHisto((TH1*)(new TH1D(TString(s->GetName())+"_nPUPVertexHisto","",25,-.5,24.5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_nVertexHisto","",25,-.5,24.5))))return 0;
  if(!s->addHisto((TH1*)(new TH2F(TString(s->GetName())+"_vertexXYHisto","",100,0.2,0.3,100,0.34,0.44))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_vertexZHisto","",100,-20,20))))return 0;
  
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_nVertexPUPHisto","",25,-.5,24.5))))return 0;
  
  return 1;
}

bool BaseAnalysis::getHistos(Sample* s){
  if(!s)return 0;

  if(!(triggerHisto_=(TH1F*)(s->getHisto("triggerHisto"))))return 0;

  if(!(runNumberHisto_=(TH1F*)(s->getHisto("runNumberHisto"))))return 0;
  if(!(nPUPVertexHisto_=(TH1D*)(s->getHisto("nPUPVertexHisto"))))return 0;
  if(!(nVertexHisto_=(TH1F*)(s->getHisto("nVertexHisto"))))return 0;
  if(!(vertexXYHisto_=(TH2F*)(s->getHisto("vertexXYHisto"))))return 0;
  if(!(vertexZHisto_=(TH1F*)(s->getHisto("vertexZHisto"))))return 0;

  if(!(nVertexPUPWeightHisto_=(TH1F*)(s->getHisto("nVertexPUPHisto"))))return 0;


  return 1;
}

bool BaseAnalysis::applySelections(const fwlite::Event * event){

  if(sample_->getFirstRun()!=0) if(event->getRun().run()<sample_->getFirstRun())return 0;
  if(sample_->getLastRun()!=0) if(sample_->getLastRun()<event->getRun().run())return 0;

  return 1;
}

bool BaseAnalysis::fillHistos(const fwlite::Event * event ){
  //cout<<event->getRun()<<endl;
  runNumberHisto_->Fill(event->getRun().run());

  mcPUPWeight_=1.0;
  if(sample_->getDataType()!="Data" && sample_->getDataType()!="Data_SS"){
    //get the number of pile up vertexes
    edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    event->getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
    std::vector<PileupSummaryInfo>::const_iterator PVI;  
    int npv=-1;
    for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {    
      int BX = PVI->getBunchCrossing();    
      if(BX == 0) { 
	npv = PVI->getPU_NumInteractions();
	continue;
      }    
    }
    nPUPVertexHisto_->Fill(npv);
    if(-1 < npv && npv < mcPUPWeightHisto_->GetXaxis()->GetNbins()) mcPUPWeight_= mcPUPWeightHisto_->GetBinContent(npv+1);//npv starts at 0
  }


  
  //// look at vertex collection
  edm::InputTag particletag_("offlinePrimaryVertices");
  edm::Handle< std::vector<reco::Vertex> > vec;
  event->getByLabel(particletag_, vec);
  Int_t icand=0;
  for(std::vector<reco::Vertex>::const_iterator cand=vec->begin(); cand!=vec->end(); ++cand, icand++){
    if(icand==0){//only save the primary vertex
      vertexXYHisto_->Fill(cand->x(),cand->y());
      vertexZHisto_->Fill(cand->z());
      
    }
  }
  nVertexHisto_->Fill(icand);
  nVertexPUPWeightHisto_->Fill(icand,mcPUPWeight_);



  return 1;
}

bool BaseAnalysis::scaleWeightHistos(Sample* s){
  if(!getHistos(s))return 0;  
  nVertexPUPWeightHisto_->Scale(nVertexHisto_->Integral()/nVertexPUPWeightHisto_->Integral());
  
  return 1;
}



bool BaseAnalysis::createHistos(TString samplename){
  
  std::vector<Sample*>::const_iterator s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s,i++)
    if(samples_[i]->GetName()==samplename) sample_ = samples_[i];

  if(!sample_){cout<<"sample "<<samplename<<" was not found"<<endl; return 0;}
  //sample_->print();

  if(!addHistos(sample_))return 0;

  if(!getHistos(sample_))return 0;

  if(!sample_->getEvents()){
    cout<<" No fwlite::ChainEvent found in sample "<<sample_->GetName()<<endl;
    return 0;
  }
  fwlite::ChainEvent chain=*(sample_->getEvents());
  //note: cannot use the pointer to the chain in the sample, event loop crashes after first file
  
  Int_t ievt=0;
  for(chain.toBegin(); !chain.atEnd() && ievt <  truncateEvents_; ++chain, ++ievt){
    if(ievt%printFreq_==0)cout<<ievt<<" done"<<endl;
    const fwlite::Event * event = chain.event();

    if(!applySelections(event))continue;

    if(!fillHistos(event))return 0;

  }

  //scale weighted histograms
  if(!scaleWeightHistos(sample_))return 0;

  //
  if(!sample_->save())return 0;

  return 1;
}



bool BaseAnalysis::createMCPUPHisto(){
  //create a histogram with the MC PUP distribution
  //this must be done with unselected MC events
  TH1D MCPUPHisto("MCPUPHisto","Npv",31,-.5,30.5);//must include 0

  std::vector<std::string> list;
  list.push_back("/tmp/tree_CMG_1.root");
  list.push_back("/tmp/tree_CMG_2.root");
  list.push_back("/tmp/tree_CMG_3.root");
  list.push_back("/tmp/tree_CMG_4.root");
  list.push_back("/tmp/tree_CMG_5.root");
  fwlite::ChainEvent chain(list);


  Int_t ievt=0;
  for(chain.toBegin(); !chain.atEnd() && ievt <  truncateEvents_; ++chain, ++ievt){
    if(ievt%printFreq_==0)cout<<ievt<<" done"<<endl;
    const fwlite::Event * event = chain.event();

    //get the number of pile up vertexes
    edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    event->getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
    std::vector<PileupSummaryInfo>::const_iterator PVI;  
    int npv=-1;
    for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {    
      int BX = PVI->getBunchCrossing();    
      if(BX == 0) { 
	npv = PVI->getPU_NumInteractions();
	continue;
      }    
    }
    MCPUPHisto.Fill(npv);
    
  }

  TFile F("./MCPUP.root","recreate");
  if(F.IsZombie())return 0;
  MCPUPHisto.Write();
  F.Close();

  return 1;
}
