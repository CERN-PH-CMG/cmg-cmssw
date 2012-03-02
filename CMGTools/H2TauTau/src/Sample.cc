#include "CMGTools/H2TauTau/interface/Sample.h"

//ClassImp(Sample)

Sample::Sample():
  TNamed("Sample","Sample")
{}

Sample::Sample(const char * name, const char * path):
  TNamed(name,path),
  crossection_(1.0),
  lumi_(0.0),
  genEvents_(0),
  outputpath_(path),
  ntpChain_(NULL),
  dataType_(""),
  color_(0),
  lcolor_(0),
  lstyle_(0),
  normFactor_(1),
  init_(0)
{
  
}

Sample::~Sample(){
  
  if(ntpChain_!=NULL)  delete ntpChain_;
  
}



TH1F* Sample::getHistoNtpFile(TString var,Int_t nbins,Float_t xmin,Float_t xmax,TString selection){
  if(!ntpChain_){
    ntpChain_=new TChain(TString("tauMuFlatNtp")+GetName()+"/tree");
    for(Int_t i=0;i<=200;i++){
      TString fname=TString(GetTitle())+"/tauMuFlatNtp_"+GetName()+"_"+(long)i+".root";

      struct stat st;
      if(stat(fname.Data(),&st) != 0) continue;

      TFile file(fname.Data(),"read");
      if(file.IsZombie()) continue;
      if(!file.GetListOfKeys()) continue;
      if(file.GetListOfKeys()->GetSize()==0) continue;
      ntpChain_->Add(fname.Data());
    }
   

    if(ntpChain_->GetNtrees()==0){
      cout<<"Sample "<<GetName()<<" 0 files chained"<<endl;
      return 0;
    }
    if(ntpChain_->GetNtrees()==200){
      cout<<"Number of files added for sample "<<GetName()<<" is at max"<<endl;
      return 0;
    }
  }
    
  TH1F* h=new TH1F(TString("Sample")+GetName(),var,nbins,xmin,xmax);
  h->Sumw2();
  ntpChain_->Draw(var+">>"+h->GetName(),selection);
  return h;
}

