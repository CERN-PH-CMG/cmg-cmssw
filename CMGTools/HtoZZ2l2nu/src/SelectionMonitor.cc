#include "CMGTools/HtoZZ2l2nu/interface/SelectionMonitor.h"

/**
   @short takes care of filling an histogram
*/
bool SelectionMonitor::fillHisto(TString name, TString tag, double val, double weight,bool useBinWidth)
{
  TH1 *h = getHisto(name,tag);
  if(h==0) return false;
  if(useBinWidth)  
    {
      int ibin =h->FindBin(val);
      double width = h->GetBinWidth(ibin);
      weight /= width;
    }
  h->Fill(val,weight);
  return true;
}

/**
   @short takes care of filling an histogram
*/
bool SelectionMonitor::fill2DHisto(TString name, TString tag, double valx, double valy, double weight,bool useBinWidth)
{
  TH2 *h = (TH2 *)getHisto(name,tag);
  if(h==0) return false;
  if(useBinWidth)  
    {
      int ibin =h->FindBin(valx,valy);
      double width = h->GetBinWidth(ibin);
      weight /= width;
    }
  h->Fill(valx,valy,weight);
  return true;
}

/**
   @short takes care of filling an histogram
*/
bool SelectionMonitor::fillProfile(TString name, TString tag, double valx, double valy, double weight)
{
  TProfile *h = (TProfile *)getHisto(name,tag);
  if(h==0) return false;
  h->Fill(valx,valy,weight);
  return true;
}

/**
   @short inits the monitor plots for a new step
*/
void SelectionMonitor::initMonitorForStep(TString tag)
{
  //clone all all histograms to the new monitor
  Monitor_t newMonitor;
  for(Monitor_t::iterator hit = allMonitors_["all"].begin();
      hit != allMonitors_["all"].end();
      hit++)
    {
      TString allName = hit->second->GetName();
      TString name = tag + "_" + allName;
      TH1 *h = (TH1 *) hit->second->Clone(name);
      h->SetTitle(tag);
      h->Reset("ICE");
      newMonitor[allName] = h;
    }
  
  //add new monitor to the map
  allMonitors_[tag]=newMonitor;
}

/**
   @short add new histogram
*/
TH1 * SelectionMonitor::addHistogram(TH1 *h, TString tag, bool format) 
{ 
  if(format) formatPlot(h,1,1,1,20,0,false,true,1,1,1 );
  allMonitors_["all"][tag] = h; 
  return allMonitors_["all"][tag];
}
TH1 * SelectionMonitor::addHistogram(TH1 *h, bool format) 
{
  if(h==0) return 0;
  return addHistogram(h,h->GetName(),format);
}

