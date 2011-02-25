#include "PFAnalyses/RootTools/interface/EventSummary.h"

#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
 
using namespace std;

typedef  EventSummary::RunsInfo::const_iterator IRun;
typedef  EventSummary::LumiSectionsInfo::const_iterator ILum;


void EventSummary::addEvent(  unsigned run, unsigned event, unsigned lumiSection ) {
  
  events_[ run ][ lumiSection ]++;
  nEvents_++;
}  

void EventSummary::addTree( TTree* tree, const char* cuts) {

  int run = 0;
  int event = 0; 
  int lumiSection = 0;

  TBranch* brun = tree->GetBranch("Run");
  TBranch* bevent = tree->GetBranch("Event");
  TBranch* bLumiSection = tree->GetBranch("LumiSection");
  tree->SetBranchAddress("Run", &run, &brun );
  tree->SetBranchAddress("Event", &event, &bevent );
  tree->SetBranchAddress("LumiSection", &lumiSection, &bLumiSection );

  for(unsigned i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    addEvent( run, event, lumiSection);
//     if(i==5000) break;
  }
} 


void EventSummary::clear() {
  events_.clear();
  nEvents_ = 0;

  if(nEventsPerRun_) {
    delete nEventsPerRun_;
    nEventsPerRun_ = 0;
  }
  if(nEventsPerLumiSectionPerRun_) {
    delete nEventsPerLumiSectionPerRun_;
    nEventsPerLumiSectionPerRun_ = 0;
  } 
}



int EventSummary::nEventsInRun( const EventSummary::LumiSectionsInfo& lumsInfo ) const {

  int nEv = 0;
  for( ILum iLum = lumsInfo.begin(); iLum!= lumsInfo.end(); ++iLum) {
    nEv += iLum->second;
  } 
  return nEv;
}


void EventSummary::print( ostream& out ) const {
  
  out<<"Runs used: "<<endl;
  
  for( IRun iRun = events_.begin(); iRun!= events_.end(); ++iRun) {
    out<<"\trun: "<<iRun->first<<" nEvents: "<<nEventsInRun(iRun->second)<<endl;
    out<<"\t\tlumi sections:"<<endl;
    for( ILum iLum = iRun->second.begin(); iLum!= iRun->second.end(); ++iLum) {
      out<<"\t\t\tlum: "<<iLum->first<<" nEvents: "<<iLum->second<<endl;
      
    }
  }
  out<<"Total number of events: "<<nEvents_<<endl;
} 

void EventSummary::printLatex( ostream& out ) const {
  for( IRun iRun = events_.begin(); iRun!= events_.end(); ++iRun) {
    out<<iRun->first<<" & "<<nEventsInRun(iRun->second)<<" \\\\"<<endl;
  }
  out<<" & "<<nEvents_<<" \\\\"<<endl;

}
