#include "CMGTools/H2TauTau/interface/BaseAnalysis.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"


BaseAnalysis::BaseAnalysis():
  TNamed("BaseAnalysis","BaseAnalysis"),
  sample_(NULL),
  verbosity_(0),
  truncateEvents_(100000000),
  printFreq_(100),
  mcPUPWeight_(1),
  outputpath_("./")
{
}

BaseAnalysis::BaseAnalysis(const char * name):
  TNamed(name,name),
  sample_(NULL),
  verbosity_(0),
  truncateEvents_(100000000),
  printFreq_(100),
  mcPUPWeight_(1),
  outputpath_("./")
{

}

BaseAnalysis::~BaseAnalysis(){
  
  delete mcPUPWeightHisto_;
}


bool BaseAnalysis::init(){
  
  //read the pile-up weights
  //TFile DataPUP("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/Pileup_2011_to_172802_LP_LumiScale.root","read");
  TFile DataPUP("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/PileUp_160404-173692_2.1invfb.pileup.root","read");
  TFile MCPUP("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/Pileup_Summer11MC_36bins.root","read");
  if(DataPUP.IsZombie()||MCPUP.IsZombie()){
    cout<<DataPUP.GetName()<<" or "<<MCPUP.GetName()<<" not valid "<<endl;
  }
  gROOT->cd();
  TH1D* DataPUPHisto=(TH1D*)DataPUP.Get("pileup");
  if(!DataPUPHisto){
    cout<<"Data pileup do not exist"<<endl;    return 0;
  }
  TH1D* MCPUPHisto=(TH1D*)MCPUP.Get("pileup");
  if(!MCPUPHisto){
    cout<<"MC pileup do not exist"<<endl; return 0;
  }

  if(DataPUPHisto->GetXaxis()->GetNbins()!=MCPUPHisto->GetXaxis()->GetNbins()){
    cout<<"DataPUPHisto does not have same number of bins as  MCPUPHisto"<<endl;
    return 0;
  }
  DataPUPHisto->Scale(1./DataPUPHisto->Integral());
  MCPUPHisto->Scale(1./MCPUPHisto->Integral());
  mcPUPWeightHisto_=new TH1F("mcPUPWeightHisto","",DataPUPHisto->GetXaxis()->GetNbins(),-0.5,DataPUPHisto->GetXaxis()->GetNbins()-0.5);
  for(int b=1;b<=DataPUPHisto->GetXaxis()->GetNbins();b++)
    if(MCPUPHisto->GetBinContent(b)>0) 
      mcPUPWeightHisto_->SetBinContent(b,DataPUPHisto->GetBinContent(b)/MCPUPHisto->GetBinContent(b));
    else mcPUPWeightHisto_->SetBinContent(b,0);

 
  ///init each sample
  std::vector<Sample*>::const_iterator s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){
    samples_[i]->setOutputPath(outputpath_);
    if(!(samples_[i]->init())) return 0;
 
    cout<<"BaseAnalysis Initialized sample "<<samples_[i]->GetName()<<" "<<samples_[i]->GetTitle()<<endl;
  }
  
  return 1;
}

bool BaseAnalysis::createHistos(TString samplename){
  //currently undefined

  return 1;
}

bool BaseAnalysis::addHistos(Sample* s){
  if(!s)return 0;
     
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_runNumberHisto","",1500,160000,175000))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_nVertexHisto","",25,-.5,24.5))))return 0;
  if(!s->addHisto((TH1*)(new TH2F(TString(s->GetName())+"_vertexXYHisto","",100,0.2,0.3,100,0.34,0.44))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_vertexZHisto","",100,-20,20))))return 0;
  
  return 1;
}

bool BaseAnalysis::getHistos(TString tag){
  if(!sample_)return 0;

  if(tag!="")tag=TString("_")+tag;

  if(!(runNumberHisto_=(TH1F*)(sample_->getHisto(TString("runNumberHisto")+tag))))return 0;
  if(!(nVertexHisto_=(TH1F*)(sample_->getHisto(TString("nVertexHisto")+tag))))return 0;
  if(!(vertexXYHisto_=(TH2F*)(sample_->getHisto(TString("vertexXYHisto")+tag))))return 0;
  if(!(vertexZHisto_=(TH1F*)(sample_->getHisto(TString("vertexZHisto")+tag))))return 0;

  return 1;
}


bool BaseAnalysis::fillVariables(const fwlite::Event * event){
    
  runnumber_=event->getRun().run();
  lumiblock_=event->luminosityBlock();
  eventid_=event->id().event();

  event->getByLabel(edm::InputTag("offlinePrimaryVertices"), vertices_);

  //determine PUP weight
  mcPUPWeight_=1.0;
  if(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS" || sample_->getDataType()=="Signal"){
    edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    event->getByLabel(edm::InputTag("addPileupInfo"), PupInfo);    
    int npv=-1;
    for( std::vector<PileupSummaryInfo>::const_iterator PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {    
      int BX = PVI->getBunchCrossing();    
      if(BX == 0) { 
	npv = PVI->getPU_NumInteractions();
	continue;
      }    
    }
    if(npv == -1)return 0;
    if(-1 < npv && npv < mcPUPWeightHisto_->GetXaxis()->GetNbins())
      mcPUPWeight_= mcPUPWeightHisto_->GetBinContent(npv+1);//npv=0 corresponds to bin # 1
  }

  
  return 1;
}

bool BaseAnalysis::applySelections(){

  if(sample_->getFirstRun()!=0) if(runnumber_<sample_->getFirstRun())return 0;
  if(sample_->getLastRun()!=0) if(sample_->getLastRun()<runnumber_)return 0;

  return 1;
}

bool BaseAnalysis::fillHistos(TString tag, double weight){
  if(!BaseAnalysis::getHistos(tag)) return 0;

  runNumberHisto_->Fill(runnumber_);
  
  nVertexHisto_->Fill(vertices_->size(),weight);
  vertexXYHisto_->Fill(vertices_->begin()->x(),vertices_->begin()->y(),weight);
  vertexZHisto_->Fill(vertices_->begin()->z(),weight);

  return 1;
}


bool BaseAnalysis::createMCPUPHisto(){

  //create a histogram with the MC PUP distribution
  //this must be done with unselected MC events
  TH1D MCPUPHisto("pileup","",36,-.5,35.5);//must include 0
  MCPUPHisto.Sumw2();

  std::vector<std::string> list;
  list.push_back("/tmp/tree_CMG_0.root");
  list.push_back("/tmp/tree_CMG_1.root");
  list.push_back("/tmp/tree_CMG_2.root");
  list.push_back("/tmp/tree_CMG_3.root");
  list.push_back("/tmp/tree_CMG_4.root");
  list.push_back("/tmp/tree_CMG_5.root");
  list.push_back("/tmp/tree_CMG_6.root");
  list.push_back("/tmp/tree_CMG_7.root");
  list.push_back("/tmp/tree_CMG_8.root");
  list.push_back("/tmp/tree_CMG_9.root");
  list.push_back("/tmp/tree_CMG_10.root");
  list.push_back("/tmp/tree_CMG_11.root");
  list.push_back("/tmp/tree_CMG_12.root");
  list.push_back("/tmp/tree_CMG_13.root");
  list.push_back("/tmp/tree_CMG_14.root");
  list.push_back("/tmp/tree_CMG_15.root");
  list.push_back("/tmp/tree_CMG_16.root");
  list.push_back("/tmp/tree_CMG_17.root");
  list.push_back("/tmp/tree_CMG_18.root");
  list.push_back("/tmp/tree_CMG_19.root");
  list.push_back("/tmp/tree_CMG_20.root");
  list.push_back("/tmp/tree_CMG_21.root");
  list.push_back("/tmp/tree_CMG_22.root");
  list.push_back("/tmp/tree_CMG_23.root");
  list.push_back("/tmp/tree_CMG_24.root");
  list.push_back("/tmp/tree_CMG_25.root");
  list.push_back("/tmp/tree_CMG_26.root");
  list.push_back("/tmp/tree_CMG_27.root");
  list.push_back("/tmp/tree_CMG_28.root");
  list.push_back("/tmp/tree_CMG_29.root");
  list.push_back("/tmp/tree_CMG_30.root");
  fwlite::ChainEvent chain(list);


  Int_t ievt=0;
  for(chain.toBegin(); !chain.atEnd() ; ++chain, ++ievt){
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

  MCPUPHisto.Scale(1./MCPUPHisto.Integral());
  TFile F("./MCPUP.root","recreate");
  if(F.IsZombie())return 0;
  MCPUPHisto.Write();
  F.Close();

  return 1;
}



void BaseAnalysis::printMCGen(edm::Handle< std::vector<reco::GenParticle> > & genList){
  cout<<" Begin MC print out for event: "<<runnumber_<<" "<<lumiblock_<<" "<<eventid_<<endl;
  for(std::vector<reco::GenParticle>::const_iterator g=genList->begin(); g!=genList->end(); ++g){    
    cout<<"pdgID = "<<g->pdgId()<<" , pt = "<<g->p4().pt()<<" motherRef="<<g->mother()<<endl;
  }
}
