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
#include "TProfile.h"
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
  bool fillHisto(TString name, TString tag, double val, double weight=1,bool useBinWidth=false);
  bool fill2DHisto(TString name, TString tag, double valx, double valy, double weight=1,bool useBinWidth=false);
  bool fillProfile(TString name, TString tag, double valx, double valy, double weight=1);

  /**
     @short inits the monitor plots for a new step
   */
  void initMonitorForStep(TString tag);
  
  /**
     @short add new histogram
  */
  TH1 * addHistogram(TH1 *h, TString tag, bool format=true);
  TH1 * addHistogram(TH1 *h, bool format=true) ;
  
  /**
     DTOR
  */
  ~SelectionMonitor() { }
  
private:

  //all the selection step monitors
  StepMonitor_t allMonitors_;
};

#endif
