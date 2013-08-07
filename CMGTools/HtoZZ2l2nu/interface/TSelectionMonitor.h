#ifndef tselection_monitor_h
#define tselection_monitor_h

#include "TFile.h"

#include "CMGTools/HtoZZ2l2nu/interface/SelectionMonitor.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

class TSelectionMonitor :  public SelectionMonitor
{
  
public:
  
  /**
     CTOR
   */
  TSelectionMonitor(TString globalTag) 
  {
    //get the file service
    edm::Service<TFileService> fs;

    //init the directory
    std::string theTag(globalTag);
    tags_.push_back("all");
    dirs_.push_back( fs->mkdir(theTag.c_str()) );
  }
  
  /**
     @short add histograms to the collection
   */
  TH1D *addHistogram(TString name,TString title,Int_t xdivs, Double_t xmin, Double_t xmax)
  {
    return (TH1D *) SelectionMonitor::addHistogram( dirs_[0].make<TH1D>(name,title,xdivs,xmin,xmax), name );
  }
  TH1D *addHistogram(TString name,TString title,Int_t xdivs, Double_t *x)
  {
    return (TH1D *) SelectionMonitor::addHistogram( dirs_[0].make<TH1D>(name,title,xdivs,x), name );
  }
  TH2D *addHistogram(TString name,TString title,Int_t xdivs, Double_t xmin, Double_t xmax, Int_t ydivs, Double_t ymin,Double_t ymax)
  {
    return (TH2D *) SelectionMonitor::addHistogram( dirs_[0].make<TH2D>(name,title,xdivs,xmin,xmax,ydivs,ymin,ymax), name );
  }
  TH2D *addHistogram(TString name,TString title,Int_t xdivs, Double_t *x,Int_t ydivs, Double_t *y)
  {
    return (TH2D *) SelectionMonitor::addHistogram( dirs_[0].make<TH2D>(name,title,xdivs,x,ydivs,y), name );
  } 
  TH2D *addHistogram(TString name,TString title,Int_t xdivs, Double_t xmin, Double_t xmax, Int_t ydivs, Double_t *y)
  {
    return (TH2D *) SelectionMonitor::addHistogram( dirs_[0].make<TH2D>(name,title,xdivs,xmin,xmax,ydivs,y), name );
  }
  TH2D *addHistogram(TString name,TString title,Int_t xdivs, Double_t *x, Int_t ydivs, Double_t ymin,Double_t ymax)
  {
    return (TH2D *) SelectionMonitor::addHistogram( dirs_[0].make<TH2D>(name,title,xdivs,x,ydivs,ymin,ymax), name );
  } 
  
  /**
     @short inits the monitor plots for a new step
   */
  void initMonitorForStep(TString tag);

  /**
     @short return the directory for a given tag
   */
  TFileDirectory *getDirectoryForStep(TString tag="all", bool forceCreation=true);

  /**
     DTOR
  */
  ~TSelectionMonitor() { }

private:

  //local directories
  std::vector<TString> tags_;
  std::vector<TFileDirectory> dirs_;
};

#endif
