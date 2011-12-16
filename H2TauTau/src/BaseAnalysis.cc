#include "CMGTools/H2TauTau/interface/BaseAnalysis.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"


BaseAnalysis::BaseAnalysis():
  TNamed("BaseAnalysis","BaseAnalysis"),
  sample_(NULL),
  verbosity_(0),
  truncateEvents_(100000000),
  printFreq_(100),
  outputpath_("./")
{
}

BaseAnalysis::BaseAnalysis(const char * name):
  TNamed(name,name),
  sample_(NULL),
  verbosity_(0),
  truncateEvents_(100000000),
  printFreq_(100),
  pupWeightName_(""),
  outputpath_("./")
{

}

BaseAnalysis::~BaseAnalysis(){
  
  delete mcPUPWeightHisto_;
}


bool BaseAnalysis::init(){
  
  //if(!fillPUPWeightHisto())return 0;
 
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


  ///fill trigger flag
  trigpass_=0;
  edm::Handle< std::vector<cmg::TriggerObject> > trig;
  event->getByLabel(edm::InputTag("cmgTriggerObjectSel","","PAT"),trig);
  if(sample_->getTrigPaths()->size()==0)trigpass_=1;//no trigger requirement
  for(std::vector<std::string>::const_iterator path=sample_->getTrigPaths()->begin(); path!=sample_->getTrigPaths()->end(); path++){
    //trig->begin()->printSelections(cout);
    if(trig->begin()->hasSelection(path->c_str()))
      if(trig->begin()->getSelection(path->c_str())){
	trigpass_=1;
      }
  }

 
  ///Event weight definition starts here:
  pupWeight_=1.;//do not comment out needs to be used.

//   //determine PUP weight
//   mcPUPWeight_=1.0;
//   if(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS"){
//     edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
//     event->getByLabel(edm::InputTag("addPileupInfo"), PupInfo);    
//     int npv=-1;
//     for( std::vector<PileupSummaryInfo>::const_iterator PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {    
//       int BX = PVI->getBunchCrossing();    
//       if(BX == 0) { 
// 	npv = PVI->getPU_NumInteractions();
// 	continue;
//       }    
//     }
//     if(npv == -1)return 0;
//     if(-1 < npv && npv < mcPUPWeightHisto_->GetXaxis()->GetNbins())
//       mcPUPWeight_= mcPUPWeightHisto_->GetBinContent(npv+1);//npv=0 corresponds to bin # 1
//   }

  if(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS"){
    edm::Handle<double>  PupWeight;
    event->getByLabel(edm::InputTag(pupWeightName_.c_str()), PupWeight);    
    pupWeight_=(*PupWeight);
  }

  return 1;
}

bool BaseAnalysis::applySelections(){

  if(sample_->getFirstRun()!=0) if(runnumber_<sample_->getFirstRun())return 0;
  if(sample_->getLastRun()!=0) if(sample_->getLastRun()<runnumber_)return 0;

  if(!trigpass_) return 0;

  return 1;
}

bool BaseAnalysis::fillHistos(TString tag){
  if(!BaseAnalysis::getHistos(tag)) return 0;

  runNumberHisto_->Fill(runnumber_);
  
  nVertexHisto_->Fill(vertices_->size(),eventWeight_);
  vertexXYHisto_->Fill(vertices_->begin()->x(),vertices_->begin()->y(),eventWeight_);
  vertexZHisto_->Fill(vertices_->begin()->z(),eventWeight_);

  return 1;
}


bool BaseAnalysis::fillPUPWeightHisto(){

  //read the pile-up weights
  TFile DataPUP("../data/PileUp_160404-173692_2.1invfb.pileup.root","read");
  TFile MCPUP("../data/Pileup_Summer11MC_36bins.root","read");
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

  return 1;
}



void BaseAnalysis::printMCGen(edm::Handle< std::vector<reco::GenParticle> > & genList){
  cout<<" Begin MC print out for event: "<<runnumber_<<" "<<lumiblock_<<" "<<eventid_<<endl;
  for(std::vector<reco::GenParticle>::const_iterator g=genList->begin(); g!=genList->end(); ++g){    
    cout<<"pdgID = "<<g->pdgId()<<" , pt = "<<g->p4().pt()<<" motherRef="<<g->mother()<<endl;
  }
}
