#include "CMGTools/HtoZZ2l2nu/interface/TSelectionMonitor.h"
#include <algorithm>

using namespace std;

//
TFileDirectory *TSelectionMonitor::getDirectoryForStep(TString tag,bool forceCreation)
{
  //create new dir if not found
  if(forceCreation &&
     find(tags_.begin(),tags_.end(),tag) == tags_.end())
    {
      std::string theTag(tag);
      tags_.push_back(tag);
      dirs_.push_back( dirs_[0].mkdir(theTag.c_str()) );
    }
  
  //get the directory
  size_t idir=0;
  for(;idir<tags_.size(); ++idir)
    if(tags_[idir]==tag) 
      return &dirs_[idir];

  //if not found
  return 0;
}


//
void TSelectionMonitor::initMonitorForStep(TString tag)
{
  TFileDirectory *theDir = getDirectoryForStep(tag,true);

  //check all the base histograms
  StepMonitor_t &allMonitors = getAllMonitors();
  for(Monitor_t::iterator hit = allMonitors["all"].begin();
      hit != allMonitors["all"].end();
      hit++)
    {
      //check if it already exists
      TString allName = hit->second->GetName();
      if(allMonitors.find(tag) != allMonitors.end() &&
	 allMonitors[tag].find(allName) != allMonitors[tag].end()) continue;
      
      //create the new histogram      
      TH1 *h=0;
      TString name = tag + "_" + allName;
      TString className( hit->second->ClassName() );

      if( className == "TH1I")       h = theDir->make<TH1I>( *(TH1I *)hit->second );
      else if(className == "TH1F")   h = theDir->make<TH1F>( *(TH1F *)hit->second );
      else if(className == "TH1D")   h = theDir->make<TH1D>( *(TH1D *)hit->second );
      else if(className == "TH2I")   h = theDir->make<TH2I>( *(TH2I *)hit->second );
      else if(className == "TH2F")   h = theDir->make<TH2F>( *(TH2F *)hit->second );
      else if(className == "TH2D")   h = theDir->make<TH2D>( *(TH2D *)hit->second );
      
      if(h==0) continue;
      h->SetName(name);
      h->SetTitle(tag);
      h->Reset("ICE");
      allMonitors[tag][allName] = h;
    }
}

