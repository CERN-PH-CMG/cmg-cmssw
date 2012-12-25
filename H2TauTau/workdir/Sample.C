#include "Sample.h"

Sample::Sample():
  TNamed("Sample","Sample")
{}

Sample::Sample(const char * name, const char * path):
  TNamed(name,path),
  crossection_(1.0),
  lumi_(0.0),
  genEvents_(0.),
  outputpath_(path),
  nNames_(0),
  ntpChain_(NULL),
  NMAXFILES_(500),
  dataType_(""),
  color_(0),
  lcolor_(0),
  lstyle_(0),
  normFactor_(1),
  init_(0),
  signalMass_(0.)
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
  if(ntpChain_->GetNtrees()>0){
    ntpChain_->Draw(xvar+">>"+h->GetName(),selection);
    h->Scale(getNorm());
  }

  return h;
}

TH1F * Sample::getHistoNtpFile(TString xvar,Int_t Nbins, Float_t * Xbins, TString selection){
  if(!ntpChain_) openNtpFile();

  TH1F* h=new TH1F(TString("Sample")+GetName(),xvar,Nbins,Xbins);
  h->GetXaxis()->SetTitle(xvar);
  h->Sumw2();
  if(ntpChain_->GetNtrees()>0){
    ntpChain_->Draw(xvar+">>"+h->GetName(),selection);
    h->Scale(getNorm());
  }

  return h;
}


TH2F* Sample::getHistoNtpFile(TString xvar, Int_t xnbins, Float_t xmin, Float_t xmax, TString yvar, Int_t ynbins, Float_t ymin, Float_t ymax, TString selection){
  if(!ntpChain_) openNtpFile();
    
  TH2F* h=new TH2F(TString("Sample")+GetName(),"",xnbins,xmin,xmax,ynbins,ymin,ymax);
  h->Sumw2();
  if(ntpChain_->GetNtrees()>0){
    ntpChain_->Draw(xvar+":"+yvar+">>"+h->GetName(),selection);
    h->Scale(getNorm());
  }

  return h;
}

int Sample::getNProcEvents(TString logfile){
  Int_t Nevents=0;

  ifstream InputStream;
  InputStream.open((const char*)(logfile));
  if(!InputStream.is_open()){cout<<"log file not found:"<<logfile<<endl; return 0;}

  TString trigreport;
  Int_t ntrigreports=0;
  Bool_t END=0;
  while(Nevents==0&&!InputStream.eof()&&!END){
    InputStream>>trigreport;
    if(trigreport=="END") END=1;
    if(trigreport=="TrigReport"){
      ntrigreports++;
      if(ntrigreports==2){//found it 
	InputStream>>trigreport>>trigreport>>trigreport;
	InputStream>>Nevents;
	END=1;
      }
    }
  }

  return Nevents;
}

bool Sample::openNtpFile(){
  if(ntpChain_) return 0;

  ntpChain_=new TChain("flatNtp/tree");

  Int_t nProcEv=0;
  //chain the root files for this sample
  for(Int_t i=0;i<=NMAXFILES_;i++){
    TString fname=TString(GetTitle())+"/"+GetName()+"/flatNtp_"+(long)i+".root";

    ////check the file exists
    struct stat st;
    if(stat(fname.Data(),&st) != 0) continue;
    
    //check the file is good 
    TFile file(fname.Data(),"read");
    if(file.IsZombie()) continue;
    if(!file.GetListOfKeys()) continue;
    if(file.GetListOfKeys()->GetSize()==0) continue;
    
    //get the number of events processed for MC normalization
    nProcEv+=getNProcEvents((TString(GetTitle())+"/"+GetName()+"/flatNtp_"+(long)i+".log").Data());

    ntpChain_->Add(fname.Data());
    if(i==NMAXFILES_){
      cout<<"Number of files added for sample "<<GetName()<<" "<<GetTitle()<<" is at max "<<NMAXFILES_<<endl;
      return 0;
    }
  }
  

  //Add additional root files
  for(Int_t n=0;n<nNames_;n++){
    for(Int_t i=0;i<=NMAXFILES_;i++){
      TString fname=TString(GetTitle())+"/"+addFileNames[n]+"/flatNtp_"+(long)i+".root";
      struct stat st;
      if(stat(fname.Data(),&st) != 0) continue;
   
      TFile file(fname.Data(),"read");
      if(file.IsZombie()) continue;
      if(!file.GetListOfKeys()) continue;
      if(file.GetListOfKeys()->GetSize()==0) continue;            
      ntpChain_->Add(fname.Data());

      nProcEv+=getNProcEvents((TString(GetTitle())+"/"+addFileNames[n]+"/flatNtp_"+(long)i+".log").Data());
    

      if(i==NMAXFILES_){
	cout<<"Number of files added for sample "<<GetName()<<" "<<GetTitle()<<" is at max "<<NMAXFILES_<<endl;
	return 0;
      }
    }
  }

  
  if(ntpChain_->GetNtrees()==0){
    cout<<"Sample "<<GetName()<<" "<<GetTitle()<<" 0 files chained"<<endl;
    return 0;
  }

  //set the number of generated events for MC needed for normalization
  if(ntpChain_->GetNtrees()>0)
    setSampleGenEvents(nProcEv);

  cout<<GetName()<<" "<<TString(GetTitle())<<" : files = "<<ntpChain_->GetNtrees()<<" , nProcEv ="<<nProcEv<<" , entries = "<<ntpChain_->GetEntries()<<endl;
  
  return 1;
}
