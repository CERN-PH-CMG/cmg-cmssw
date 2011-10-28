#include "CMGTools/H2TauTau/interface/Sample.h"

//ClassImp(Sample);

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
  dataType_(""),
  color_(0),
  lstyle_(0),
  plotOrder_(0),
  applyRecoilCorr_(0),
  applyTauRateWeight_(0),
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
  dataType_(""),
  color_(0),
  lstyle_(0),
  plotOrder_(0),
  applyRecoilCorr_(0),
  applyTauRateWeight_(0),
  init_(0)
{
  
}

Sample::~Sample(){
  
  if(sampleChain_!=NULL)delete sampleChain_;
  if(histFile_!=NULL)  delete histFile_;
  
}


bool Sample::init(){
  if(init_)return 1;

  //print the trigger paths  
  cout<<"TriggerPaths:"<<endl;
  for(std::vector<std::string>::const_iterator trig=trigPaths_.begin(); trig!=trigPaths_.end(); trig++)
    cout<<trig->c_str()<<endl;
  

  return init_=1;
}


fwlite::ChainEvent*  Sample::getEvents(){
  
  if(!sampleChain_){
//     //check if the summary file exists
//     struct stat stFileInfo;
//     if(stat((const char*)(TString(GetTitle())+"/summary.txt"),&stFileInfo) != 0) {
//       cout<<"unable to find "<<TString(GetTitle())+"/summary.txt"<<endl;
//       return NULL;
//     }
//     //read in the files 
//     ifstream input;
//     input.open((const char*)(TString(GetTitle())+"/summary.txt"));
//     int good=0; string colln; int nevt=0; int nevtpass=0; int N=-1;
//     input>>good>>colln>>nevt>>nevtpass;
//     while(colln != "total" && N <10000){
//       //test the files
//       if(good==1){
// 	TFile ftest(colln.c_str(),"read");
// 	if(ftest.IsZombie()){
// 	  cout<<colln<<"  is declared good but is Zombie"<<endl;
// 	  good=0;
// 	}
// 	if(!ftest.GetListOfKeys()){
// 	  cout<<colln<<"  is declared good but does not have Keys"<<endl;
// 	  good=0;
// 	}
// 	TTree*tree=(TTree*)ftest.Get("Events");
// 	if(!tree){
// 	  cout<<colln<<"  is declared good but does not have Events"<<endl;
// 	  good=0;
// 	}
      
// 	//add the good files to the chain
// 	if(good==1){
// 	  sampleList_.push_back(colln);
// 	  nEvents_+=nevt;
// 	  chainNEvents_+=tree->GetEntriesFast();
// 	}
//       }

//       input>>good>>colln>>nevt>>nevtpass;
//       N++;
//     }
//     if(N==10000){cout<<GetName()<<" Too many files"<<endl; return 0;}
//     cout<<GetName()<<" "<<sampleList_.size()<<" files added for "<<nEvents_<<" events, with "<<chainNEvents_<<" chain events."<<endl;
//     input.close();

    chainNEvents_=0;
    for(Long_t id=0;id<5000;id++){
      //test the files
      std::string colln=(const char*)(TString(GetTitle())+"/tree_CMG_"+id+".root");
      if(access(colln.c_str(), F_OK ) == -1 )continue; 
      TFile ftest(colln.c_str(),"read");
      if(ftest.IsZombie()){ cout<<colln<<" is Zombie "<<endl; continue;}
      if(!ftest.GetListOfKeys()){ cout<<colln<<" has no keys "<<endl; continue;}
      TTree*tree=(TTree*)ftest.Get("Events");
      if(!tree){ cout<<colln<<" has no tree "<<endl; continue;}
      //add the good files to the chain
      sampleList_.push_back(colln);
      chainNEvents_+=tree->GetEntriesFast();      
    }
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
  std::vector<TH1*>::const_iterator h=sampleHist_.begin();
  for( int i=0; h!=sampleHist_.end(); ++h, i++){
    cout<<"save: "<<i<<" "<<sampleHist_[i]->GetName()<<endl;
    sampleHist_[i]->Write();
  }

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
