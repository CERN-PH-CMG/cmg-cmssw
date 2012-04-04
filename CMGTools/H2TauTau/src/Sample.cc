#include "CMGTools/H2TauTau/interface/Sample.h"

//ClassImp(Sample)

Sample::Sample():
  TNamed("Sample","Sample")
{}

Sample::Sample(const char * name, const char * path):
  TNamed(name,path),
  crossection_(1.0),
  lumi_(0.0),
  genEvents_(0.),
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



TH1F* Sample::getHistoNtpFile(TString xvar,Int_t xnbins,Float_t xmin,Float_t xmax,TString selection){
  if(!ntpChain_) openNtpFile();
    
  TH1F* h=new TH1F(TString("Sample")+GetName(),xvar,xnbins,xmin,xmax);
  h->GetXaxis()->SetTitle(xvar);
  h->Sumw2();
  ntpChain_->Draw(xvar+">>"+h->GetName(),selection);
  
//   Float_t x;
//   ntpChain_->SetBranchAddress(xvar,&x);
//   for(int e=1;e<=ntpChain_->GetEntriesFast();e++){
//     ntpChain_->GetEvent(e);
//     h->Fill(x);
//   }

  return h;
}


TH2F* Sample::getHistoNtpFile(TString xvar, Int_t xnbins, Float_t xmin, Float_t xmax, TString yvar, Int_t ynbins, Float_t ymin, Float_t ymax, TString selection){
  if(!ntpChain_) openNtpFile();
    
  TH2F* h=new TH2F(TString("Sample")+GetName(),"",xnbins,xmin,xmax,ynbins,ymin,ymax);
  h->Sumw2();
  ntpChain_->Draw(xvar+":"+yvar+">>"+h->GetName(),selection);
  return h;
}



bool Sample::openNtpFile(){
  if(ntpChain_) return 0;

  //ntpChain_=new TChain(TString("flatNtp")+GetName()+"/tree");
  ntpChain_=new TChain("flatNtp/tree");

  for(Int_t i=0;i<200;i++){
    TString fname=TString(GetTitle())+"/flatNtp_"+GetName()+"_"+(long)i+".root";
    
    struct stat st;
    if(stat(fname.Data(),&st) != 0) continue;
    
    TFile file(fname.Data(),"read");
    if(file.IsZombie()) continue;
    if(!file.GetListOfKeys()) continue;
    if(file.GetListOfKeys()->GetSize()==0) continue;

    if(ntpChain_->GetNtrees()==0){
      TTree* TTest=(TTree*)file.Get("flatNtp/tree");
      if(!TTest){//cope with previous versions
	ntpChain_->SetName(TString("flatNtp")+GetName()+"/tree");
      }
    }

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

  cout<<GetName()<<" chained "<<ntpChain_->GetNtrees()<<" files, with "<<ntpChain_->GetEntries()<<" entries"<<endl;
  
  return 1;
}
