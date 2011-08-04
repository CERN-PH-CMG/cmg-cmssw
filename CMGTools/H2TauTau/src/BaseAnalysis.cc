#include "CMGTools/H2TauTau/interface/BaseAnalysis.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"


// ClassImp(BaseAnalysis)
// ;

BaseAnalysis::BaseAnalysis():
  TNamed("BaseAnalysis","BaseAnalysis"),
  truncateEvents_(100000000),
  printFreq_(100),
  mcPUPWeight_(1),
  nPUPVertexHisto_(NULL),
  nVertexHisto_(NULL),
  vertexXYHisto_(NULL),
  vertexZHisto_(NULL),
  nVertexPUPWeightHisto_(NULL),
  mcPUPWeightHisto_(NULL)
{
}

BaseAnalysis::BaseAnalysis(const char * name):
  TNamed(name,name),
  truncateEvents_(10000),//(100000000),
  printFreq_(100),
  mcPUPWeight_(1),
  nPUPVertexHisto_(NULL),
  nVertexHisto_(NULL),
  vertexXYHisto_(NULL),
  vertexZHisto_(NULL),
  nVertexPUPWeightHisto_(NULL),
  mcPUPWeightHisto_(NULL)
{
  //addSample(new Sample("RelValZTT","Tuples/CMGDiTau/RelValZTT_GEN_SIM_RECO_START42_V12_v2"));    
}

BaseAnalysis::~BaseAnalysis(){
  
}


bool BaseAnalysis::init(){
  
  //read the pile-up weights
  TFile DataPUP("Pileup_2011_EPS_8_jul.root","read");
  TH1D* DataPUPHisto=(TH1D*)DataPUP.Get("pileup");
  TFile MCPUP("MCPileUp.root","read");
  TH1D* MCPUPHisto=(TH1D*)MCPUP.Get("nPUPVertexHisto_");
  if(!DataPUPHisto || !MCPUPHisto){
    cout<<"DataPUPHisto or MCPUPHisto do not exist"<<endl;
    return 0;
  }
  if(DataPUPHisto->GetXaxis()->GetNbins()!=MCPUPHisto->GetXaxis()->GetNbins()){
    cout<<"DataPUPHisto does not have same number of bins as  MCPUPHisto"<<endl;
    return 0;
  }
  

  ///create histograms each sample
  int si=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s, si++){
    if(!(samples_[si]->init())) return 0;
    
    if(!samples_[si]->addHisto((TH1*)(mcPUPWeightHisto_=new TH1F("mcPUPWeightHisto","",25,-.5,24.5))))return 0;
    if(!samples_[si]->addHisto((TH1*)(new TH1D("nPUPVertexHisto","",25,-.5,24.5))))return 0;
    if(!samples_[si]->addHisto((TH1*)(new TH1F("nVertexHisto","",25,-.5,24.5))))return 0;
    if(!samples_[si]->addHisto((TH1*)(new TH2F("vertexXYHisto","",100,0.2,0.3,100,0.34,0.44))))return 0;
    if(!samples_[si]->addHisto((TH1*)(new TH1F("vertexZHisto","",100,-20,20))))return 0;

    if(!samples_[si]->addHisto((TH1*)(new TH1F("nVertexPUPWeightHisto","",25,-.5,24.5))))return 0;
    

    //fill the PUP weights for this sample
    for(Int_t i=1;i<=DataPUPHisto->GetXaxis()->GetNbins();i++)
      if(MCPUPHisto->GetBinContent(i)>0)mcPUPWeightHisto_->SetBinContent(i,DataPUPHisto->GetBinContent(i)/MCPUPHisto->GetBinContent(i));
      else mcPUPWeightHisto_->SetBinContent(i,0);
    //mcPUPWeightHisto_->Scale(1./mcPUPWeightHisto_->Integral());
    
    cout<<"BaseAnalysis Initialized sample "<<samples_[si]->GetName()<<endl;

  }
  
  return 1;
}

bool BaseAnalysis::getHistos(Sample* s){
  if(!s)return 0;

  if(!(mcPUPWeightHisto_=(TH1F*)(s->getHisto("mcPUPWeightHisto"))))return 0;
  if(!(nPUPVertexHisto_=(TH1D*)(s->getHisto("nPUPVertexHisto"))))return 0;
  if(!(nVertexHisto_=(TH1F*)(s->getHisto("nVertexHisto"))))return 0;
  if(!(vertexXYHisto_=(TH2F*)(s->getHisto("vertexXYHisto"))))return 0;
  if(!(vertexZHisto_=(TH1F*)(s->getHisto("vertexZHisto"))))return 0;

  if(!(nVertexPUPWeightHisto_=(TH1F*)(s->getHisto("nVertexPUPWeightHisto"))))return 0;


  return 1;
}


bool BaseAnalysis::fill(edm::EventBase const & event ){

  //get the number of pile up vertexes
  edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
  event.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
  std::vector<PileupSummaryInfo>::const_iterator PVI;  
  int npv = -1;
  for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {    
    int BX = PVI->getBunchCrossing();    
    if(BX == 0) { 
      npv = PVI->getPU_NumInteractions();
      continue;
    }    
  }
  nPUPVertexHisto_->Fill(npv);

  ///get the PUP weight for this event
  mcPUPWeight_= mcPUPWeightHisto_->GetBinContent(npv);


  //// look at vertex collection
  edm::InputTag particletag_("offlinePrimaryVertices");
  edm::Handle< std::vector<reco::Vertex> > vec;
  event.getByLabel(particletag_, vec);
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

bool BaseAnalysis::analyze(TString samplename){
  
  Sample* sample=0;
  int si=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s,si++){
    if(samples_[si]->GetName()==samplename) sample = samples_[si];
  }
  if(!sample){cout<<"sample "<<samplename<<" was not found"<<endl; return 0;}
  sample->print();

  if(!getHistos(sample))return 0;

  if(!sample->getEvents()){
    cout<<" No fwlite::ChainEvent found in sample "<<sample->GetName()<<endl;
    return 0;
  }
  fwlite::ChainEvent chain=*(sample->getEvents());
  //note: cannot use the pointer to the chain in the sample, event loop crashes after first file
  
  Int_t ievt=0;
  for(chain.toBegin(); !chain.atEnd() && ievt <  truncateEvents_; ++chain, ++ievt){
    if(ievt%printFreq_==0)cout<<ievt<<" done"<<endl;
    edm::EventBase const & event = chain;

    if(!fill(event))return 0;

  }

  return 1;
}


bool BaseAnalysis::scaleWeightHistos(Sample* s){
  if(!getHistos(s))return 0;  
  nVertexPUPWeightHisto_->Scale(nVertexHisto_->Integral()/nVertexPUPWeightHisto_->Integral());
  
  return 1;
}


bool BaseAnalysis::end(){
  int si=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s,si++){

    if(!scaleWeightHistos(samples_[si]))return 0;

    samples_[si]->save();
  }

  return 1;
}


