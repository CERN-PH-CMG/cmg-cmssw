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
#include "TROOT.h"


#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"

class SmartSelectionMonitor {
  
public:

  SmartSelectionMonitor(){}
  ~SmartSelectionMonitor() { }


  //types
  typedef std::map<TString, std::map<TString, TH1*>* > Monitor_t;

  //short getters
  inline Monitor_t &getAllMonitors() { return allMonitors_; }

  //checks if base Histo Exist
  inline bool hasBaseHisto(TString histo){
    if(allMonitors_.find(histo) == allMonitors_.end())return false;
    return true;
  }

  //checks if tag Exist for a given histo
  inline bool hasTag(std::map<TString, TH1*>* map, TString tag){
    if( map->find(tag) != map->end() )return true;
    if( map->find("all") == map->end() )return false;
  
    TH1* base = (*map)["all"];
    TString allName = base->GetName();
    TString name = tag + "_" + allName;
    TH1* h = (TH1*) base->Clone(name);
    h->SetName(name);
    h->SetTitle(name);
    h->Reset("ICE");
    h->SetDirectory(gROOT);
    (*map)[tag] = h; 
//    printf("new histo created with name = %30s and tag = %15s: Name=%s\n",allName.Data(), tag.Data(), h->GetName());
    return true;
  }

  //checks if tag Exist for a given histo
  inline bool hasTag(TString histo, TString tag){
    if(!hasBaseHisto(histo))return false;
    std::map<TString, TH1*>* map = allMonitors_[histo];
    return hasTag(map, tag);
  }

  //get histo
  inline TH1 *getHisto(TString histo,TString tag="all"){
    if( !hasBaseHisto(histo) )return NULL;
    std::map<TString, TH1*>* map = allMonitors_[histo];
    if( !hasTag(map, tag) )return NULL;
    return (*map)[tag];
  }

  //write all histo
  inline void Write(){
     for(Monitor_t::iterator it =allMonitors_.begin(); it!= allMonitors_.end(); it++){
        std::map<TString, TH1*>* map = it->second;
        for(std::map<TString, TH1*>::iterator h =map->begin(); h!= map->end(); h++){
	  if(!(h->second)){printf("histo = %30s %15s IS NULL",it->first.Data(), h->first.Data());continue;}
          //printf("histo = %30s tag = %15s Name = %s\n",it->first.Data(), h->first.Data(),  h->second->GetName());
          h->second->Write();
        }
     }
  
  }


  //takes care of filling an histogram
  bool fillHisto(TString name, TString tag, double val, double weight=1,bool useBinWidth=false);
  bool fill2DHisto(TString name, TString tag, double valx, double valy, double weight=1,bool useBinWidth=false);
  bool fillProfile(TString name, TString tag, double valx, double valy, double weight=1);

  bool fillHisto  (TString name, std::vector<TString> tags, double val, double weight=1,bool useBinWidth=false);
  bool fill2DHisto(TString name, std::vector<TString> tags, double valx, double valy, double weight=1,bool useBinWidth=false);
  bool fillProfile(TString name, std::vector<TString> tags, double valx, double valy, double weight=1);



   //short inits the monitor plots for a new step
  void initMonitorForStep(TString tag);
  
  //short add new histogram
  TH1 * addHistogram(TH1 *h, TString tag, bool format=true);
  TH1 * addHistogram(TH1 *h, bool format=true) ;
  
private:

  //all the selection step monitors
  Monitor_t allMonitors_;
};

#endif
