#ifndef __PFAnalysis_RootTools_EventSummary__
#define __PFAnalysis_RootTools_EventSummary__

#include <map>
#include <iostream>

namespace edm {
  class EventAuxiliary;
}

class TH1F;
class TH2F;
class TTree;


class EventSummary {
  
 public:
 EventSummary() : 
  nEvents_(0), nEventsPerRun_(0), nEventsPerLumiSectionPerRun_(0) {}

  void addEvent( unsigned run, unsigned event, unsigned lumiSection=0);
  void addTree( TTree* tree, const char* cuts); 
  void clear(); 

  void makeHistograms() {}

  void print( std::ostream& out = std::cout) const;

  void printLatex( std::ostream& out = std::cout) const;

  typedef std::map<unsigned, unsigned> LumiSectionsInfo;
  typedef std::map<unsigned, LumiSectionsInfo> RunsInfo;

 private:


  int nEventsInRun( const LumiSectionsInfo& lumsInfo ) const;

  RunsInfo events_; 
  unsigned nEvents_;

  TH1F* nEventsPerRun_;
  TH2F* nEventsPerLumiSectionPerRun_;
};

#endif
