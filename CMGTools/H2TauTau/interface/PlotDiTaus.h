#ifndef PLOTDITAUS_H_
#define PLOTDITAUS_H_

#include <iomanip>
#include <TString.h>
#include <string>
#include <vector>

class PlotDiTaus {
public:
  
  PlotDiTaus():
    inputdir_(""){
  };
  PlotDiTaus(TString inputdir);
  ~PlotDiTaus(){};

  void plotDiTau(TString inputtag="cmgDiTauSelClean");
  void plotTauE(TString inputtag="cmgTauESelClean");
  void plotTauMu(TString inputtag="cmgTauMuSelClean");
  void plotMuE(TString inputtag="cmgMuESelClean");
  void plotEE(TString inputtag="cmgDiESelClean");
  void plotMuMu(TString inputtag="cmgDiMuSelClean");
  


private:
  
  TString inputdir_;  
  std::vector< std::string > filenames_;
};



#endif 
