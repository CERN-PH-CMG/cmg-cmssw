#include "UserCode/EWKV/interface/SmartSelectionMonitor.h"


// add new histogram
TH1 * SmartSelectionMonitor::addHistogram(TH1* h, TString histo){
  h->Sumw2();
  if(!hasBaseHisto(histo)){
     allMonitors_[histo] = new std::map<TString, TH1*>; 
  }
  (*allMonitors_[histo])["all"] = h;
  return (*allMonitors_[histo])["all"];
}

TH1 * SmartSelectionMonitor::addHistogram(TH1 *h){
  if(h==0) return 0;
  return addHistogram(h,h->GetName());
}



// takes care of filling an histogram
bool SmartSelectionMonitor::fillHisto(TString name, TString tag, double val, double weight, bool useBinWidth)
{
  TH1 *h = getHisto(name,tag);
  if(h==0) return false;
  if(useBinWidth){ int ibin =h->FindBin(val); double width = h->GetBinWidth(ibin);   weight /= width;  }
  h->Fill(val,weight);
  return true;
}

bool SmartSelectionMonitor::fillHisto(TString name, std::vector<TString> tags, double val, double weight, bool useBinWidth){
  for(unsigned int i=0;i<tags.size();i++){fillHisto(name, tags[i], val, weight, useBinWidth);}
  return true;
}


// takes care of filling an histogram
bool SmartSelectionMonitor::fillHisto(TString name, TString tag, double valx, double valy, double weight, bool useBinWidth)
{
  TH2 *h = (TH2 *)getHisto(name,tag);
  if(h==0) return false;
  if(useBinWidth){ int ibin =h->FindBin(valx,valy); double width = h->GetBinWidth(ibin); weight /= width; }
  h->Fill(valx,valy,weight);
  return true;
}

bool SmartSelectionMonitor::fillHisto(TString name, std::vector<TString> tags, double valx, double valy, double weight, bool useBinWidth){
  for(unsigned int i=0;i<tags.size();i++){fillHisto(name, tags[i], valx, valy, weight, useBinWidth);}
  return true; 
}



// takes care of filling an histogram
bool SmartSelectionMonitor::fillProfile(TString name, TString tag, double valx, double valy, double weight)
{
  TProfile *h = (TProfile *)getHisto(name,tag);
  if(h==0) return false;
  h->Fill(valx,valy,weight);
  return true;
}


bool SmartSelectionMonitor::fillProfile(TString name, std::vector<TString> tags, double valx, double valy, double weight){
  for(unsigned int i=0;i<tags.size();i++){fillProfile(name, tags[i], valx, valy, weight);}
  return true;
}








