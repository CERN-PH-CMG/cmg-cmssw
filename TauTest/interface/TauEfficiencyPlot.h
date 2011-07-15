
#ifndef TauEfficiencyPlot_HH
#define TauEfficiencyPlot_HH
#include <TString.h>
#define NCMGTAUEFFPLOTS 20
  
class TauEfficiencyPlot {
    
public:

  TauEfficiencyPlot():
    file_(""){
    for(Int_t i=0;i<NCMGTAUEFFPLOTS;i++){
      histId_[i]="-";
    }
  }

  TauEfficiencyPlot(TString filename):
    file_(filename){
    for(Int_t i=0;i<NCMGTAUEFFPLOTS;i++){
      histId_[i]="-";
    }
  }
    
  Bool_t addHistId(TString histId){
    for(Int_t i=0;i<NCMGTAUEFFPLOTS;i++)
      if(histId_[i]=="-"){
	histId_[i]=histId; return 1;
      }
    return 0;
  }


  void plotEfficiencyVsPt();


private:

  TString file_;
  TString histId_[NCMGTAUEFFPLOTS];


};

#endif
