#ifndef CMGTools_Common_AnalysisHistograms_h
#define CMGTools_Common_AnalysisHistograms_h

// Base class for histogram managing.
//
// Original Author:  Artur Kalinowski
//         Created:  Tue Oct 24 15:06:12 CEST 2006
// $Id: AnalysisHistograms.h,v 1.1 2010/11/11 14:09:03 wreece Exp $
//
//
#include <cmath>
#include <string>
#include <map>
#include <fstream>
#include <iomanip>
#include <vector>

#include "CommonTools/Utils/interface/TFileDirectory.h"

//ROOT include files
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"

//TODO Colin: no need to copy strings as arguments
//TODO Colin: many map searches are unnecessary

class AnalysisHistograms {
 public:

  
  AnalysisHistograms():histosInitialized_(false){};

  /*
  AnalysisHistograms(TFileDirectory *myDir,
		     const std::string & name=""):
    histosInitialized_(false), name_(name){};
  */

  virtual ~AnalysisHistograms();

  
  void fillProfile(const std::string& name, float x, float val, float weight=1.0);
  void fill1DHistogram(std::string name, float val, float weight=1.0);
  void fill2DHistogram(std::string name, float val1, float val2, float weight=1.0);
  void fill3DHistogram(std::string name, float val1, float val2, float val3, float weight=1.0);
  
  TProfile* getProfile(const std::string& name);
  TH1F* get1DHistogram(std::string name);
  TH2F* get2DHistogram(std::string name);
  TH3F* get3DHistogram(std::string name);
  

 protected:

  void clear();
  
  void write();

  virtual void init(TFileDirectory *myDir, const std::string & name=""); 

  ///Method for backward compatibility. 
  ///Will be removed at some time
  virtual void init(const TFileDirectory *myDir); 

  virtual void init(const std::string & name=""){}; 

  virtual void defineHistograms() = 0;

  virtual void finalizeHistograms();



  /// The ROOT file with histograms
  TFileDirectory *file_;

  ///The secondary directory.
  std::vector<TFileDirectory> mySecondaryDirs_;

  /// The histograms
  std::map<std::string,TProfile*> myProfiles_;
  std::map<std::string,TH1F*> my1Dhistograms_;
  std::map<std::string,TH2F*> my2Dhistograms_;
  std::map<std::string,TH3F*> my3Dhistograms_;

  void addProfile(const std::string& name, const std::string& title, 
		  int nBinsX, float xlow, float xhigh, 
		  const TFileDirectory* myDir);

  void add1DHistogram(std::string name, std::string title, 
		      int nBinsX, float xlow, float xhigh, 
		      const TFileDirectory* myDir);

  void add1DHistogram(std::string name, std::string title, 
		      int nBinsX, float* bins, 
		      const TFileDirectory* myDir);

  void add2DHistogram(std::string name, std::string title, 
		      int nBinsX, float xlow, float xhigh,
		      int nBinsY, float ylow, float yhigh,
		      const TFileDirectory* myDir);
  void add2DHistogram(std::string name, std::string title, 
		      int nBinsX, float* binsX,
		      int nBinsY, float* binsY,
		      const TFileDirectory* myDir);
  void add2DHistogram(std::string name, std::string title, 
		      int nBinsX, float xlow, float xhigh,
		      int nBinsY, double* binsY,
		      const TFileDirectory* myDir);
  void add3DHistogram(std::string name, std::string title, 
		      int nBinsX, float xlow, float xhigh,
		      int nBinsY, float ylow, float yhigh,
		      int nBinsZ, float zlow, float zhigh,
		      const TFileDirectory* myDir);
  void add3DHistogram(std::string name, std::string title, 
		      int nBinsX, double* binsX,		                                           
		      int nBinsY, double* binsY,
		      int nBinsZ, double* binsZ, 
		      const TFileDirectory* myDir);

  static void resetHistos(std::pair<const std::string, TH1*> aPair);


  double* equalRanges(int nSteps, double min, double max, double *ranges);
  bool histosInitialized_;


  ///Name of the histograms set instance
  std::string name_;

};

#endif
