#include "CMGTools/H2TauTau/interface/Sample.h"

//ClassImp(Sample);

Sample::Sample():
  TNamed("Sample","Sample"),
  sampleCrossection_(1),
  sampleNevents_(0),
  sampleChainNevents_(0),
  sampleChain_(NULL),
  init_(0)
{}

Sample::Sample(const char * name, const char * path):
  TNamed(name,path),
  sampleCrossection_(1),
  sampleNevents_(0),
  sampleChainNevents_(0),
  sampleChain_(NULL),
  init_(0)
{
  
}

Sample::~Sample(){
  if(sampleChain_!=NULL)delete sampleChain_;
  
//   int hi=0;
//   for(std::vector<TH1*>::const_iterator h=sampleHist_.begin(); h!=sampleHist_.end(); ++h, hi++){
//     delete sampleHist_[hi];
//   }
  //sampleHist_.clear();
  
}


bool Sample::init(){

  if(init_)return 1;

  //check if the summary file exists
  struct stat stFileInfo;
  if(stat((const char*)(TString(GetTitle())+"/summary.txt"),&stFileInfo) != 0) {
    cout<<"unable to find "<<TString(GetTitle())+"/summary.txt"<<endl;
  }
  //read in the files 
  ifstream input;
  input.open((const char*)(TString(GetTitle())+"/summary.txt"));
  int good=0; string colln; int nevt=0; int N=-1;
  input>>good>>colln>>nevt;
  while(colln != "total" && N <10000){
    //cout<<good<<" "<<colln<<" "<<nevt<<endl;

    //test the files
    if(good==1){
      TFile ftest(colln.c_str(),"read");
      if(ftest.IsZombie()){
	cout<<colln<<"  is declared good but is Zombie"<<endl;
	good=0;
      }
      if(!ftest.GetListOfKeys()){
	cout<<colln<<"  is declared good but does not have Keys"<<endl;
	good=0;
      }
      TTree*tree=(TTree*)ftest.Get("Events");
      if(!tree){
	cout<<colln<<"  is declared good but does not have Events"<<endl;
	good=0;
      }else sampleChainNevents_+=tree->GetEntriesFast();
      
      //add the good files to the chain
      if(good==1){
      sampleList_.push_back(colln);
      sampleNevents_+=nevt;
      }
    }

    input>>good>>colln>>nevt;
    N++;
  }
  cout<<sampleList_.size()<<" files added for "<<sampleNevents_<<" events, with "<<sampleChainNevents_<<" chain events."<<endl;
  input.close();

  sampleChain_=new fwlite::ChainEvent(sampleList_);


  return init_=1;
}

bool Sample::save(TFile* file){

  TFile* fi;
  if(file)fi=file;
  else {
    fi=new TFile(TString(GetTitle())+"/Sample_"+GetName()+"_Histograms.root","recreate");    
    if(fi->IsZombie()){
      cout<<"Sample "<<GetName()<<" Unable to create histograms file"<<endl;
      return 0;
    }
  }
    
  fi->cd();
  int hi=0;
  for(std::vector<TH1*>::const_iterator h=sampleHist_.begin(); h!=sampleHist_.end(); ++h, hi++){
    //  cout<<sampleHist_[hi]<<endl;
    sampleHist_[hi]->Write();
  }

  cout<<"Sample "<<GetName()<<" saved histograms in "<<fi->GetName()<<endl;

  
  if(!file){
    fi->Close();
    delete fi;
  }
 
  return 1;
}



void Sample::print(){

  cout<<"Sample "<<GetName()<<endl;
  cout<<"  path: "<<GetTitle()<<endl;
  cout<<"  NEvents: "<<sampleNevents_<<endl;

  int hi=0;
  for(std::vector<TH1*>::const_iterator h=sampleHist_.begin(); h!=sampleHist_.end(); ++h, hi++)
    cout<<"  histo: "<<sampleHist_[hi]->GetName()<<endl;


  
}
