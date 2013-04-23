#ifndef smartselectionmonitor_hh
#define smartselectionmonitor_hh

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

#include <ext/hash_map>

namespace __gnu_cxx{
        template<> struct hash< TString >{ size_t operator()( const TString& x ) const{ return hash< const char* >()( x.Data() );  }  };
}


class SmartSelectionMonitor {
  
public:

  SmartSelectionMonitor(){}
  ~SmartSelectionMonitor() { }


  //types
//  typedef std::map<TString, std::map<TString, TH1*>* > Monitor_t;
  typedef __gnu_cxx::hash_map<TString, std::map<TString, TH1*>* > Monitor_t;


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
        bool neverFilled = true;

        for(std::map<TString, TH1*>::iterator h =map->begin(); h!= map->end(); h++){
	  if(!(h->second)){printf("histo = %30s %15s IS NULL",it->first.Data(), h->first.Data());continue;}
	  if(h->second->GetEntries()>0)neverFilled = false;

          //printf("histo = %30s tag = %15s Name = %s\n",it->first.Data(), h->first.Data(),  h->second->GetName());
          if(h->first=="all"){h->second->Write(h->first+"_"+h->second->GetName());
          }else{              h->second->Write();}
        }

        if(neverFilled){printf("SmartSelectionMonitor: histo = '%s' is empty for all the category, you may want to cleanup your project to remove this histogram\n",it->first.Data());}
     }
  
  }


  //takes care of filling an histogram
  bool fillHisto  (TString name, TString tag, double valx, double weight, bool useBinWidth=false);
  bool fillHisto  (TString name, TString tag, double valx, double valy, double weight,  bool useBinWidth=false);
  bool fillProfile(TString name, TString tag, double valx, double valy, double weight);

  bool fillHisto(TString name, std::vector<TString> tags, double valx, double weight,  bool useBinWidth=false);
  bool fillHisto(TString name, std::vector<TString> tags, double valx, double valy, double weight,  bool useBinWidth=false);
  bool fillProfile(TString name, std::vector<TString> tags, double valx, double valy, double weight);

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
