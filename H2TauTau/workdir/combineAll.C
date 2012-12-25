

/*
INSTRUCTIONS:
-Execute from this directory:
HiggsAnalysis/HiggsToTauTau/test

-In there you need 3 directories:
HiggsAnalysis/HiggsToTauTau/test/root/ (input root-cards)
HiggsAnalysis/HiggsToTauTau/test/datacards/ (.txt data-cards)
HiggsAnalysis/HiggsToTauTau/test/fitresults/ (mlfit result for the pulls in a file called mlfit_sm.txt)

-Run the following commands to generate the root files containing the postfit histograms:
python produce_macros.py  -a sm -c 'mt, et, em' -u 1 -p "7TeV 8TeV"
python produce_macros.py  -a sm -c 'tt' -u 1 -p "8TeV"
sed -i 's/bool log=true/bool log=false/g' *_0_*.C
sed -i 's/BLIND_DATA = true/BLIND_DATA = false/g' *.C
python run_macros.py  -a sm -c 'mt, et, em'  -p "7TeV 8TeV"
python run_macros.py  -a sm -c 'tt'  -p "8TeV"

-Now one can execute this combineAll.C macro

*/



#include "weightedCombine.C"

void combineAll(int weight=1){

  TString Input[40];


  Input[0]="emu_boost_low_rescaled_7TeV_";
  Input[1]="eleTau_boost_low_rescaled_7TeV_";
  Input[2]="muTau_boost_low_rescaled_7TeV_";
  Input[3]="emu_boost_low_rescaled_8TeV_";
  Input[4]="eleTau_boost_low_rescaled_8TeV_";
  Input[5]="muTau_boost_low_rescaled_8TeV_";

  Input[6]="emu_boost_high_rescaled_7TeV_";
  Input[7]="eleTau_boost_high_rescaled_7TeV_";
  Input[8]="muTau_boost_high_rescaled_7TeV_";
  Input[9]="emu_boost_high_rescaled_8TeV_";
  Input[10]="eleTau_boost_high_rescaled_8TeV_";
  Input[11]="muTau_boost_high_rescaled_8TeV_";

  Input[12]="tauTau_boost_rescaled_8TeV_";

  Input[13]="emu_vbf_rescaled_7TeV_";
  Input[14]="eleTau_vbf_rescaled_7TeV_";
  Input[15]="muTau_vbf_rescaled_7TeV_";
  Input[16]="emu_vbf_rescaled_8TeV_";
  Input[17]="eleTau_vbf_rescaled_8TeV_";
  Input[18]="muTau_vbf_rescaled_8TeV_";

  Input[19]="tauTau_vbf_rescaled_8TeV_";

  
  weightedCombine(Input,20,13,"combineAll",weight);
  /*                     |  |      |->Output file name must match below
                         |  |-> number of files to be rebinned by 2
                         |->Total number of input files
  */


  if(weight==0)weightedPlot("combineAll_UnWeighted");
  if(weight==1)weightedPlot("combineAll_Weighted");



  gROOT->ProcessLine(".q");
}

