#include "CMGTools/H2TauTau/interface/Sample.h"

//ClassImp(Sample);

//do not use this default constructor may not be up to date
Sample::Sample():
  TNamed("Sample","Sample"),
  crossection_(1.0),
  lumi_(0.0),
  genEvents_(0),
  nEvents_(0),
  chainNEvents_(0),
  sampleChain_(NULL),
  firstrun_(0),
  lastrun_(0),
  effCorrFactor_(1.0),
  outputpath_(GetTitle()),
  histFile_(NULL),
  tree_(NULL),
  dataType_(""),
  color_(0),
  lcolor_(0),
  lstyle_(0),
  plotOrder_(0),
  applyRecoilCorr_(0),
  applyTauRateWeight_(0),
  genEventType_(0),
  truthEventType_(0),
  normFactor_(1),
  init_(0)
{}

Sample::Sample(const char * name, const char * path):
  TNamed(name,path),
  crossection_(1.0),
  lumi_(0.0),
  genEvents_(0),
  nEvents_(0),
  chainNEvents_(0),
  sampleChain_(NULL),
  firstrun_(0),
  lastrun_(0),
  effCorrFactor_(1.0),
  outputpath_(path),
  histFile_(NULL),
  tree_(NULL),
  ntpFile_(NULL),
  ntpTree_(NULL),
  dataType_(""),
  color_(0),
  lcolor_(0),
  lstyle_(0),
  plotOrder_(0),
  applyRecoilCorr_(0),
  applyTauRateWeight_(0),
  genEventType_(0),
  truthEventType_(0),
  normFactor_(1),
  init_(0)
{
  
}

Sample::~Sample(){
  
  if(sampleChain_!=NULL)delete sampleChain_;
  if(histFile_!=NULL)  delete histFile_;

  std::vector<TH1*>::const_iterator h=sampleHist_.begin();
  for( int i=0; h!=sampleHist_.end(); ++h, i++)
    delete sampleHist_[i];

  if(tree_!=NULL) delete tree_;

  for(std::vector<int*>::const_iterator c=countervalue_.begin(); c!=countervalue_.end(); ++c)
    delete *c;    

  if(ntpFile_!=NULL)  delete ntpFile_;
  
}


bool Sample::init(){
  if(init_)return 1;


  //counter for fully selected events
  countername_.push_back(TString(""));
  countervalue_.push_back(new int(0));


  //print the trigger paths  
  cout<<"TriggerPaths:"<<endl;
  for(std::vector<std::vector<std::string> >::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++)
    cout<<(*path)[0].c_str()<<" "<<(*path)[1].c_str()<<" "<<(*path)[2].c_str()<<endl;
  

  return init_=1;
}


fwlite::ChainEvent*  Sample::getEvents(){
  
  if(!sampleChain_){
    chainNEvents_=0;
    for(Long_t id=0;id<5000;id++){
      std::string collnpre=(const char*)(TString(GetTitle())+"/h2TauTau_presel_tree_CMG_"+id+".root");
      std::string collnfull=(const char*)(TString(GetTitle())+"/h2TauTau_fullsel_tree_CMG_"+id+".root");
      TFile* ftest=NULL;
      if(access(collnpre.c_str(), F_OK ) != -1 )
	ftest=new TFile(collnpre.c_str(),"read");
      else if(access(collnfull.c_str(), F_OK ) != -1 )
	ftest=new TFile(collnfull.c_str(),"read");
      if(!ftest)continue;
      if(ftest->IsZombie()){ cout<<ftest->GetName()<<" is Zombie "<<endl; continue;}
      if(!ftest->GetListOfKeys()){ cout<<ftest->GetName()<<" has no keys "<<endl; continue;}
      TTree*tree=(TTree*)ftest->Get("Events");
      if(!tree){ cout<<ftest->GetName()<<" has no tree "<<endl; continue;}
      sampleList_.push_back(ftest->GetName());      //add the good files to the chain
      chainNEvents_+=tree->GetEntriesFast();      
      delete ftest;
    }
    if(sampleList_.size()>5000){cout<<"too many files in directory"<<endl; return 0;}
    cout<<GetName()<<" "<<sampleList_.size()<<" files added,  with "<<chainNEvents_<<" chain events."<<endl;

    if(sampleList_.size()==0){
      cout<<"No files added for this sample."<<endl;
      return NULL;
    }
    sampleChain_=new fwlite::ChainEvent(sampleList_);
  }

 
  return sampleChain_;
}


bool Sample::save(){
  
  if(sampleHist_.size()==0){
    cout<<GetName()<<": no histos to save"<<endl; return 0;
  }
  
  TFile fi(outputpath_+"/"+GetName()+"_Sample_Histograms.root","recreate");    
  if(fi.IsZombie()){
    cout<<"Sample "<<GetName()<<" Unable to create histograms file"<<endl;
    return 0;
  }
  
  fi.cd();

  ///save histos
  std::vector<TH1*>::const_iterator h=sampleHist_.begin();
  for( int i=0; h!=sampleHist_.end(); ++h, i++){
    //cout<<"save: "<<i<<" "<<sampleHist_[i]->GetName()<<endl;
    sampleHist_[i]->Write();
  }

  //save tree
  if(tree_)tree_->Write();

  fi.Close();
  cout<<"Sample "<<GetName()<<" saved histograms in "<<fi.GetName()<<endl;  

 
  return 1;
}



void Sample::print(){

  cout<<"Sample "<<GetName()<<" "<<this<<endl;
  cout<<"  path: "<<GetTitle()<<endl;
  cout<<"  NEvents: "<<nEvents_<<endl;

  std::vector<TH1*>::const_iterator h=sampleHist_.begin();
  for(int i=0; h!=sampleHist_.end(); ++h, i++)
    cout<<"  histo"<<i<<": "<<sampleHist_[i]->GetName()<<endl;


  
}
