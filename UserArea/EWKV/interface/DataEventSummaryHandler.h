#ifndef dataeventsummaryhandler_h
#define dataeventsummaryhandler_h

#include "UserCode/EWKV/interface/DataEventSummary.h"

#include "TTree.h"

class DataEventSummaryHandler{

 public:

  //c/dtor
  DataEventSummaryHandler()  { };
  ~DataEventSummaryHandler() { };
  
  //initialize
  bool init(TTree *t,bool needsToRecreate=true);
  bool attach(TTree *t);

  //r/w mode
  void fill() { if(t_) t_->Fill(); }
  int getEntries() { return (t_ ? t_->GetEntriesFast() : 0); }
  void getEntry(int ientry) { evSummary_.reset(); if(t_) t_->GetEntry(ientry);  }

  //getters
  TTree *getTree() { return t_; }
  DataEventSummary &getEvent() { return evSummary_; }
  
  //the tree and the data holders
  TTree *t_;
  DataEventSummary evSummary_;
};

#endif
