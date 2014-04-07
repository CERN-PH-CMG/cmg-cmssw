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
  NMAXFILES_(1000),
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
    //cout<<"Scaling "<<GetName()<<" by "<<getNorm()<<endl;
    h->Scale(getNorm());
  }

  return h;
}


TH2F* Sample::getHistoNtpFile(TString xvar, Int_t xnbins, Float_t xmin, Float_t xmax, TString yvar, Int_t ynbins, Float_t ymin, Float_t ymax, TString selection){
  if(!ntpChain_) openNtpFile();
    
  TH2F* h=new TH2F(TString("Sample")+GetName(),"",xnbins,xmin,xmax,ynbins,ymin,ymax);
  h->Sumw2();
  if(ntpChain_->GetNtrees()>0){
    ntpChain_->Draw(yvar+":"+xvar+">>"+h->GetName(),selection);
    h->Scale(getNorm());
  }

  return h;
}

int Sample::getNProcEvents(TString logfile){
  Int_t Nevents=0;

  ifstream InputStream;
  InputStream.open((const char*)(logfile));
  if(!InputStream.is_open()){
    cout<<"Sample::getNProcEvents Error: log file not found:"<<endl;
    cout<<logfile<<endl;
    return -1;
  }
  
  //skip to the end of the file
  //-note MC has longer end of log file than Data due to vertex weight modules
  //-if one uses too large number it gets stuck with files which did not process any events
  InputStream.seekg(0,ios_base::end);//go to end
  //cout<<logfile<<" "<<InputStream.tellg()<<endl;
  if(InputStream.tellg()<1000){
    cout<<"Sample::getNProcEvents Error: log file size is too small:"<<endl;
    cout<<logfile<<endl;
    return -1;
  }
  if(InputStream.tellg() > 22000 ) InputStream.seekg(-22000 , ios_base::end);
  else InputStream.seekg(0, ios_base::beg);


  TString trigreport;
  bool foundSummary=0;
  while(Nevents==0&&!InputStream.eof()){
    InputStream>>trigreport;
    //cout<<trigreport;
    if(trigreport=="END") break;

    //look for this:
// TrigReport ---------- Event  Summary ------------
// TrigReport Events total = 229063 passed = 144751 failed = 84312

    if(trigreport=="TrigReport"){
      InputStream>>trigreport;
      if(trigreport=="Events"){
	InputStream>>trigreport;
	if(trigreport=="total"){
	  InputStream>>trigreport;
	  if(trigreport=="="){
	    InputStream>>Nevents;
	    foundSummary=1;
	    break;
	  }
	}
      }
    }

  }

  if(!foundSummary){
    cout<<" Sample::getNProcEvents Error: reached end of file without finding TriggerReport summary"<<endl;
    cout<<logfile<<endl;
  }
  if(Nevents==0){
    cout<<" Sample::getNProcEvents Warning:  TriggerReport summary reports 0 events processed"<<endl;
    cout<<logfile<<endl;
  }

  //cout<<" ***** "<<logfile<<"   "<<Nevents<<endl;
  return Nevents;
}

bool Sample::openNtpFile(){
  if(ntpChain_) return 0;

  ntpChain_=new TChain("flatNtp/tree");

  Int_t nProcEv=0;
  Int_t nonExistentCounter=0;
  Int_t nMaxNonExistent=5;
  //chain the root files for this sample
  for(Int_t i=0;i<=NMAXFILES_ && nonExistentCounter<nMaxNonExistent ;i++){
    TString fname=TString(GetTitle())+"/"+GetName()+"/flatNtp_"+(long)i+".root";

    ////check the file exists
    struct stat st;
    if(stat(fname.Data(),&st) != 0){ 
      nonExistentCounter++; //terminate the loop if we find nMaxNonExistent non-existing root files
      continue; 
    }
    
    
    //check the file is good 
    TFile file(fname.Data(),"read");
    if(file.IsZombie()) continue;
    if(!file.GetListOfKeys()) continue;
    if(file.GetListOfKeys()->GetSize()==0) continue;
    
    //get the number of events processed for MC normalization
    int nproc=getNProcEvents((TString(GetTitle())+"/"+GetName()+"/flatNtp_"+(long)i+".log").Data());
    if(nproc<=0)continue;//could be no events were processed in data due to json file --> no point in adding to chain

    nProcEv+=nproc;
    ntpChain_->Add(fname.Data());
    if(i==NMAXFILES_){
      cout<<"Number of files added for sample "<<GetName()<<" "<<GetTitle()<<" is at max "<<NMAXFILES_<<endl;
      return 0;
    }
  }
  

  //Add additional root files
  for(Int_t n=0;n<nNames_;n++){
    cout<<"Adding root files from : "<<addFileNames[n]<<endl;
    nonExistentCounter=0;
    for(Int_t i=0;i<=NMAXFILES_ && nonExistentCounter<nMaxNonExistent ;i++){
      TString fname=TString(GetTitle())+"/"+addFileNames[n]+"/flatNtp_"+(long)i+".root";
      struct stat st;
      if(stat(fname.Data(),&st) != 0){ 
	nonExistentCounter++; 
	continue;
      }
   
      TFile file(fname.Data(),"read");
      if(file.IsZombie()) continue;
      if(!file.GetListOfKeys()) continue;
      if(file.GetListOfKeys()->GetSize()==0) continue;            


      int nproc=getNProcEvents((TString(GetTitle())+"/"+addFileNames[n]+"/flatNtp_"+(long)i+".log").Data());
      if(nproc<=0)continue;

      nProcEv+=nproc;
      ntpChain_->Add(fname.Data());    
      if(i==NMAXFILES_){
	cout<<"Number of files added for sample "<<GetName()<<" "<<GetTitle()<<" is at max "<<NMAXFILES_<<endl;
	return 0;
      }
    }
  }

  
  if(ntpChain_->GetNtrees()==0 || nProcEv == 0){
    cout<<"Sample "<<GetName()<<" "<<GetTitle()<<" 0 files/events chained: "<<ntpChain_->GetNtrees()<<"  "<<nProcEv<<endl;
    return 0;
  }

  setSampleGenEvents(nProcEv);

  cout<<GetName()
      <<" "<<TString(GetTitle())
      <<" : files = "<<ntpChain_->GetNtrees()
      <<" , nProcEv ="<<nProcEv
      <<" , entries = "<<ntpChain_->GetEntries()<<endl;
  
  return 1;
}
