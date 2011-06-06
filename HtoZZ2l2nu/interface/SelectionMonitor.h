#ifndef selection_monitor_hh
#define selection_monitor_hh


// system include files
#include<iostream>
#include<string>
#include<map>
#include<algorithm>
#include<vector>
#include<memory>

// user include files
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"


#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"

class SelectionMonitor {
  
public:

  /**
     CTOR
   */
  SelectionMonitor() 
  { 
  }

  //types
  typedef std::map<TString, TH1 *> Monitor_t;
  typedef std::map<TString, Monitor_t> StepMonitor_t;

  /**
     @short getters
   */
  inline StepMonitor_t &getAllMonitors() { return allMonitors_; }
  inline Monitor_t &getMonitorsAtStep(TString tag)
  {
    if(allMonitors_.find(tag) == allMonitors_.end()) initMonitorForStep(tag);
    return allMonitors_[tag];
  }
  inline TH1 *getHisto(TString name,TString tag="all") 
  {
    if( !hasStep(tag) ) return 0;
    if(allMonitors_[tag].find(name) == allMonitors_[tag].end()) return 0;
    return allMonitors_[tag][name];
  }

  /**
     @short checks if category exists
   */
  inline bool hasStep(TString tag) 
  {
    return ( allMonitors_.find(tag) != allMonitors_.end() );
  }

  /**
     @short takes care of filling an histogram
   */
  inline bool fillHisto(TString name, TString tag, double val, double weight=1,bool useBinWidth=false)
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
  inline bool fill2DHisto(TString name, TString tag, double valx, double valy, double weight=1,bool useBinWidth=false)
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
     @short inits the monitor plots for a new step
   */
  inline void initMonitorForStep(TString tag)
  {
    //clone all all histograms to the new monitor
    Monitor_t newMonitor;
    int monitorCtr = allMonitors_.size();
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
  TH1 * addHistogram(TH1 *h, TString tag, bool format=true) 
    { 
      if(format) formatPlot(h,1,1,1,20,0,true,true,1,1,1 );
      allMonitors_["all"][tag] = h; 
      return allMonitors_["all"][tag];
    }
  TH1 * addHistogram(TH1 *h, bool format=true) 
    {
      if(h==0) return 0;
      return addHistogram(h,h->GetName(),format);
    }
  
    /**
     DTOR
  */
  ~SelectionMonitor() { }

private:

  //all the selection step monitors
  StepMonitor_t allMonitors_;
};

#endif
