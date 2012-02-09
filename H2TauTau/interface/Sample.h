#ifndef CMGTools_H2TauTau_Sample_H
#define CMGTools_H2TauTau_Sample_H

#include <memory>
#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include <TROOT.h>
#include <TNamed.h>
#include <TSystem.h>
#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TList.h>
#include <TIterator.h>
#include <TKey.h>

#include "DataFormats/FWLite/interface/ChainEvent.h"

class Sample : public TNamed {
  
public:
  
  Sample();
  Sample(const char * name, const char * path);
  virtual ~Sample();
  
  bool init();

  void setOutputPath(TString path){outputpath_=path;}
  void setCrossection(float crossection){crossection_=crossection;} //in pb
  void setSampleLumi(float lumi){lumi_=lumi;}
  void setSampleGenEvents(int Ngen){genEvents_=Ngen;}
  void setPileupWeight(std::string weight){pileupWeight_=weight;}

  void addTrigPath(std::string pathname, std::string filterLeg1 = "", std::string filterLeg2 = ""){
    std::vector<std::string> pth; pth.push_back(pathname); pth.push_back(filterLeg1);pth.push_back(filterLeg2);
    trigPaths_.push_back(pth);
  }
  void addTrigPaths(std::vector<std::vector<std::string> > * paths){
    if(!paths)return;
    for(std::vector<std::vector<std::string> >::const_iterator p=paths->begin(); p!=paths->end(); ++p)
      trigPaths_.push_back(*p);
  }
  void setRunRange(unsigned int first, unsigned int last){firstrun_=first; lastrun_=last;}
  void setEffCorrFactor(float factor){effCorrFactor_=factor;}




  bool addHisto(TH1* hist) {
    if(!hist)return 0;
    hist->Sumw2();
    sampleHist_.push_back(hist);
    mainsampleHist_.push_back(hist);
    return 1;
  }
  void cloneHistos(TString tag){
    for(std::vector<TH1*>::const_iterator h=mainsampleHist_.begin(); h!=mainsampleHist_.end(); ++h)
      sampleHist_.push_back((TH1*)(*h)->Clone(TString((*h)->GetName())+"_"+tag));

    //counters for n-1 cut events
    countername_.push_back(tag);
    countervalue_.push_back(new int(0));

  }
  void addTree(TTree* tree){tree_=tree;}
  
  void setDataType(TString type){
    dataType_=type;
  }
  
  void incrementCounter(TString tag){    
    int idx=0;
    for(std::vector<TString>::const_iterator name=countername_.begin(); name!=countername_.end(); ++name,idx++)
      if((*name)==tag) (*(countervalue_[idx]))++;    
  }
  void printCounters(){    
    cout<<GetName()<<" Selection counters :"<<endl;
    int idx=0;
    for(std::vector<TString>::const_iterator name=countername_.begin(); name!=countername_.end(); ++name,idx++)
      cout<<(*name)<<" "<<*(countervalue_[idx])<<endl;
  }

  void setColor(Int_t color){color_=color;}
  void setLineColor(Int_t color){lcolor_=color;}
  void setLineStyle(Int_t lstyle){lstyle_=lstyle;}
  void setPlotOrder(unsigned int n){plotOrder_=n;}
  void setPlotLabel(TString label){plotLabel_=label;}
  void setLegendOption(TString opt){legendOption_=opt;}
  void setRecoilCorr(TString processfile){applyRecoilCorr_=1; recoilCorrProcessFile_=processfile;}
  void setApplyTauRateWeight(bool applyWeight){applyTauRateWeight_=applyWeight;}
  void setGenEventType(unsigned int eventtype){genEventType_=eventtype;}
  void setTruthEventType(unsigned int eventtype){truthEventType_=eventtype;}


  bool scale(Float_t factor){  
    normFactor_*=factor;

    return 1;
  }




  //access
  TString getPath(){return GetTitle();}
  int getNEvents(){return nEvents_;}
  float getCrossection(){return crossection_;}
  TString getDataType(){return dataType_;}
  const std::string * getPileupWeight() const {return &pileupWeight_;}
  int getSampleGenEvents(){return genEvents_;}
  float getEffCorrFactor(){return effCorrFactor_;}
  float getLumi(){ 
    if(dataType_=="Data" || dataType_=="Data_SS" || dataType_=="Embedded" || dataType_=="Embedded_SS")return lumi_;
    else if(crossection_>0.) return genEvents_/crossection_;
    return 0.;
  }
  float getNorm(){return normFactor_;}
  
  Int_t getColor(){return color_;}
  Int_t getLineColor(){return lcolor_;}
  Int_t getLineStyle(){return lstyle_;}
  unsigned int getPlotOrder(){return plotOrder_;}
  TString getPlotLabel(){return plotLabel_;}
  TString getLegendOption(){return legendOption_;}
  bool getApplyRecoilCorr(){return applyRecoilCorr_;}
  TString getRecoilCorrProcessFile(){return recoilCorrProcessFile_;}
  bool getApplyTauRateWeight(){return applyTauRateWeight_;}
  unsigned int getGenEventType(){return genEventType_;}
  unsigned int getTruthEventType(){return truthEventType_;}

  fwlite::ChainEvent* getEvents();
  
  TH1* getHisto(TString name){    
    for(std::vector<TH1*>::const_iterator h=sampleHist_.begin(); h!=sampleHist_.end(); ++h)
      if(TString((*h)->GetName()) == TString(GetName())+"_"+name) return *h;
    cout<<"Sample "<<GetName()<<" histo "<<name<<" not found "<<endl;
    return NULL;
  }

  
  const TH1* getHistoFromFile(TString name){
    if(!histFile_){
      histFile_=new TFile(outputpath_+"/"+GetName()+"_Sample_Histograms.root","read");
      if(histFile_->IsZombie()) return NULL;
      if(!histFile_->GetListOfKeys()) return NULL;
      if(histFile_->GetListOfKeys()->GetSize()==0) return NULL;
      gROOT->cd();
    }
    return (TH1*)(histFile_->Get(TString(GetName())+"_"+name)) ;
  }

  TH1F* getHistoNtpFile(TString name,Int_t nbins,Float_t xmin,Float_t xmax,TString selection=""){
    if(!ntpTree_){
      ntpFile_=new TFile(TString(GetTitle())+"/tauMuHistograms_"+GetName()+".root","read");
      if(ntpFile_->IsZombie()) return NULL;
      if(!ntpFile_->GetListOfKeys()) return NULL;
      if(ntpFile_->GetListOfKeys()->GetSize()==0) return NULL;
      gROOT->cd();
      //ntpFile_->ls();

      ntpTree_=(TTree*)ntpFile_->Get(TString("tauMuHistogramer")+GetName()+"/tree");

      if(!ntpTree_)return 0;
    }
    
    TH1F* h=new TH1F(TString("Sample")+GetName()+name,name,nbins,xmin,xmax);
    h->Sumw2();
    ntpTree_->Draw(TString("")+name+">>"+h->GetName(),selection);
    return h;
  }

  std::vector<std::vector<std::string> > * getTrigPaths(){return &trigPaths_;}
  unsigned int getFirstRun(){return firstrun_;}
  unsigned int getLastRun(){return lastrun_;}

  bool save();


  void print();

protected:


private:

  float crossection_;
  float lumi_;//for Data
  int genEvents_;//for MC
  int nEvents_;
  int chainNEvents_;
  std::vector<std::string> sampleList_; 
  fwlite::ChainEvent* sampleChain_;
  std::vector<TH1*> sampleHist_;//all histos including clones
  std::vector<TH1*> mainsampleHist_;//list of histos added with addHisto()
  std::vector<std::vector<std::string> > trigPaths_;
  unsigned int firstrun_;
  unsigned int lastrun_;
  float effCorrFactor_;
  TString recoilCorrProcessFile_;

  TString outputpath_;
  TFile* histFile_;
  TTree* tree_;

  TFile* ntpFile_;
  TTree* ntpTree_;

  TString dataType_;
  std::string pileupWeight_;
  Int_t color_;
  Int_t lcolor_;
  Int_t lstyle_;
  unsigned int plotOrder_;
  TString plotLabel_;
  TString legendOption_;
  bool applyRecoilCorr_;
  bool applyTauRateWeight_;
  unsigned int genEventType_;
  unsigned int truthEventType_;

  std::vector<TString> countername_;
  std::vector<int*> countervalue_;

  float normFactor_;
  bool init_;


  ClassDef(Sample, 1);
};

#endif 
