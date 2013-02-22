

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
sed -i 's/bool log=true/bool log=false/g' *.C
sed -i 's/BLIND_DATA = true/BLIND_DATA = false/g' *.C
python run_macros.py  -a sm -c 'mt, et, em'  -p "7TeV 8TeV"
python run_macros.py  -a sm -c 'tt'  -p "8TeV"
root -b /afs/cern.ch/user/b/benitezj/scratch1/V5_8_0/CMGTools/CMSSW_5_3_3_patch3/src/CMGTools/H2TauTau/workdir/combineAll.C

-Now one can execute this combineAll.C macro

*/


#include "weightedCombine.C"


void combine(TString name, TString* Input,const char* dataset , const char* channel, int weight=1){

  weightedCombine(Input,name.Data(),weight);
  /*                |     |          |->option to not apply weights
                    |     |-> name for this plot
                    |->array of file names 
  */

  //weightedPlot(name,dataset,channel);
  weightedPlotInset(name,dataset,channel);

}


void combineAll(){

  TString All[NMAXINPUT];
  All[0]="emu_boost_low_rescaled_7TeV_";
  All[1]="eleTau_boost_low_rescaled_7TeV_";
  All[2]="muTau_boost_low_rescaled_7TeV_";
  All[3]="emu_boost_low_rescaled_8TeV_";
  All[4]="eleTau_boost_low_rescaled_8TeV_";
  All[5]="muTau_boost_low_rescaled_8TeV_";
  All[6]="emu_boost_high_rescaled_7TeV_";
  All[7]="eleTau_boost_high_rescaled_7TeV_";
  All[8]="muTau_boost_high_rescaled_7TeV_";
  All[9]="emu_boost_high_rescaled_8TeV_";
  All[10]="eleTau_boost_high_rescaled_8TeV_";
  All[11]="muTau_boost_high_rescaled_8TeV_";
  All[12]="tauTau_boost_rescaled_8TeV_";
  All[13]="emu_vbf_rescaled_7TeV_";
  All[14]="eleTau_vbf_rescaled_7TeV_";
  All[15]="muTau_vbf_rescaled_7TeV_";
  All[16]="emu_vbf_rescaled_8TeV_";
  All[17]="eleTau_vbf_rescaled_8TeV_";
  All[18]="muTau_vbf_rescaled_8TeV_";
  All[19]="tauTau_vbf_rescaled_8TeV_";


  TString EMu[NMAXINPUT];
  EMu[0]="emu_boost_low_rescaled_7TeV_";
  EMu[1]="emu_boost_low_rescaled_8TeV_";
  EMu[2]="emu_boost_high_rescaled_7TeV_";
  EMu[3]="emu_boost_high_rescaled_8TeV_";
  EMu[4]="emu_vbf_rescaled_7TeV_";
  EMu[5]="emu_vbf_rescaled_8TeV_";



  TString ETau[NMAXINPUT];
  ETau[0]="eleTau_boost_low_rescaled_7TeV_";
  ETau[1]="eleTau_boost_low_rescaled_8TeV_";
  ETau[2]="eleTau_boost_high_rescaled_7TeV_";
  ETau[3]="eleTau_boost_high_rescaled_8TeV_";
  ETau[4]="eleTau_vbf_rescaled_7TeV_";
  ETau[5]="eleTau_vbf_rescaled_8TeV_";
  

  TString MuTau[NMAXINPUT];
  MuTau[0]="muTau_boost_low_rescaled_7TeV_";
  MuTau[1]="muTau_boost_low_rescaled_8TeV_";
  MuTau[2]="muTau_boost_high_rescaled_7TeV_";
  MuTau[3]="muTau_boost_high_rescaled_8TeV_";
  MuTau[4]="muTau_vbf_rescaled_7TeV_";
  MuTau[5]="muTau_vbf_rescaled_8TeV_";



  TString TauTau[NMAXINPUT];
  TauTau[0]="tauTau_boost_rescaled_8TeV_";
  TauTau[1]="tauTau_vbf_rescaled_8TeV_";



  TString BoostLow[NMAXINPUT];
  BoostLow[0]="emu_boost_low_rescaled_7TeV_";
  BoostLow[1]="eleTau_boost_low_rescaled_7TeV_";
  BoostLow[2]="muTau_boost_low_rescaled_7TeV_";
  BoostLow[3]="emu_boost_low_rescaled_8TeV_";
  BoostLow[4]="eleTau_boost_low_rescaled_8TeV_";
  BoostLow[5]="muTau_boost_low_rescaled_8TeV_";




  TString BoostHigh[NMAXINPUT];
  BoostHigh[0]="emu_boost_high_rescaled_7TeV_";
  BoostHigh[1]="eleTau_boost_high_rescaled_7TeV_";
  BoostHigh[2]="muTau_boost_high_rescaled_7TeV_";
  BoostHigh[3]="emu_boost_high_rescaled_8TeV_";
  BoostHigh[4]="eleTau_boost_high_rescaled_8TeV_";
  BoostHigh[5]="muTau_boost_high_rescaled_8TeV_";
  BoostHigh[6]="tauTau_boost_rescaled_8TeV_";
 
  TString Boost[NMAXINPUT];
  Boost[0]="emu_boost_low_rescaled_7TeV_";
  Boost[1]="eleTau_boost_low_rescaled_7TeV_";
  Boost[2]="muTau_boost_low_rescaled_7TeV_";
  Boost[3]="emu_boost_low_rescaled_8TeV_";
  Boost[4]="eleTau_boost_low_rescaled_8TeV_";
  Boost[5]="muTau_boost_low_rescaled_8TeV_";
  Boost[6]="emu_boost_high_rescaled_7TeV_";
  Boost[7]="eleTau_boost_high_rescaled_7TeV_";
  Boost[8]="muTau_boost_high_rescaled_7TeV_";
  Boost[9]="emu_boost_high_rescaled_8TeV_";
  Boost[10]="eleTau_boost_high_rescaled_8TeV_";
  Boost[11]="muTau_boost_high_rescaled_8TeV_";
  Boost[12]="tauTau_boost_rescaled_8TeV_";
 



  TString VBF[NMAXINPUT];
  VBF[0]="emu_vbf_rescaled_7TeV_";
  VBF[1]="eleTau_vbf_rescaled_7TeV_";
  VBF[2]="muTau_vbf_rescaled_7TeV_";
  VBF[3]="emu_vbf_rescaled_8TeV_";
  VBF[4]="eleTau_vbf_rescaled_8TeV_";
  VBF[5]="muTau_vbf_rescaled_8TeV_";
  VBF[6]="tauTau_vbf_rescaled_8TeV_";



  TString E7TeV[NMAXINPUT];
  E7TeV[0]="eleTau_boost_low_rescaled_7TeV_";
  E7TeV[1]="muTau_boost_low_rescaled_7TeV_";
  E7TeV[2]="emu_boost_high_rescaled_7TeV_";
  E7TeV[3]="eleTau_boost_high_rescaled_7TeV_";
  E7TeV[4]="muTau_boost_high_rescaled_7TeV_";
  E7TeV[5]="emu_vbf_rescaled_7TeV_";
  E7TeV[6]="eleTau_vbf_rescaled_7TeV_";
  E7TeV[7]="muTau_vbf_rescaled_7TeV_";
  


  TString E8TeV[NMAXINPUT];
  E8TeV[0]="emu_boost_low_rescaled_8TeV_";
  E8TeV[1]="eleTau_boost_low_rescaled_8TeV_";
  E8TeV[2]="muTau_boost_low_rescaled_8TeV_";
  E8TeV[3]="emu_boost_high_rescaled_8TeV_";
  E8TeV[4]="eleTau_boost_high_rescaled_8TeV_";
  E8TeV[5]="muTau_boost_high_rescaled_8TeV_";
  E8TeV[6]="tauTau_boost_rescaled_8TeV_";
  E8TeV[7]="emu_vbf_rescaled_8TeV_";
  E8TeV[8]="eleTau_vbf_rescaled_8TeV_";
  E8TeV[9]="muTau_vbf_rescaled_8TeV_";
  E8TeV[10]="tauTau_vbf_rescaled_8TeV_";



  TString MuTau8TeV[NMAXINPUT];
  MuTau8TeV[0]="muTau_boost_low_rescaled_8TeV_";
  MuTau8TeV[1]="muTau_boost_high_rescaled_8TeV_";
  MuTau8TeV[2]="muTau_vbf_rescaled_8TeV_";

  TString MuTau0Jet8TeV[NMAXINPUT];
  MuTau0Jet8TeV[0]="muTau_0jet_low_rescaled_8TeV_";
  MuTau0Jet8TeV[1]="muTau_0jet_high_rescaled_8TeV_";

  TString MuTauBoost8TeV[NMAXINPUT];
  MuTauBoost8TeV[0]="muTau_boost_low_rescaled_8TeV_";
  MuTauBoost8TeV[1]="muTau_boost_high_rescaled_8TeV_";




  TString AllNoTauTau[NMAXINPUT];
  AllNoTauTau[0]="emu_boost_low_rescaled_7TeV_";
  AllNoTauTau[1]="eleTau_boost_low_rescaled_7TeV_";
  AllNoTauTau[2]="muTau_boost_low_rescaled_7TeV_";
  AllNoTauTau[3]="emu_boost_low_rescaled_8TeV_";
  AllNoTauTau[4]="eleTau_boost_low_rescaled_8TeV_";
  AllNoTauTau[5]="muTau_boost_low_rescaled_8TeV_";
  AllNoTauTau[6]="emu_boost_high_rescaled_7TeV_";
  AllNoTauTau[7]="eleTau_boost_high_rescaled_7TeV_";
  AllNoTauTau[8]="muTau_boost_high_rescaled_7TeV_";
  AllNoTauTau[9]="emu_boost_high_rescaled_8TeV_";
  AllNoTauTau[10]="eleTau_boost_high_rescaled_8TeV_";
  AllNoTauTau[11]="muTau_boost_high_rescaled_8TeV_";
  AllNoTauTau[12]="emu_vbf_rescaled_7TeV_";
  AllNoTauTau[13]="eleTau_vbf_rescaled_7TeV_";
  AllNoTauTau[14]="muTau_vbf_rescaled_7TeV_";
  AllNoTauTau[15]="emu_vbf_rescaled_8TeV_";
  AllNoTauTau[16]="eleTau_vbf_rescaled_8TeV_";
  AllNoTauTau[17]="muTau_vbf_rescaled_8TeV_";


  TString BoostHighNoTauTau[NMAXINPUT];
  BoostHighNoTauTau[0]="emu_boost_high_rescaled_7TeV_";
  BoostHighNoTauTau[1]="eleTau_boost_high_rescaled_7TeV_";
  BoostHighNoTauTau[2]="muTau_boost_high_rescaled_7TeV_";
  BoostHighNoTauTau[3]="emu_boost_high_rescaled_8TeV_";
  BoostHighNoTauTau[4]="eleTau_boost_high_rescaled_8TeV_";
  BoostHighNoTauTau[5]="muTau_boost_high_rescaled_8TeV_";


  TString VBFNoTauTau[NMAXINPUT];
  VBFNoTauTau[0]="emu_vbf_rescaled_7TeV_";
  VBFNoTauTau[1]="eleTau_vbf_rescaled_7TeV_";
  VBFNoTauTau[2]="muTau_vbf_rescaled_7TeV_";
  VBFNoTauTau[3]="emu_vbf_rescaled_8TeV_";
  VBFNoTauTau[4]="eleTau_vbf_rescaled_8TeV_";
  VBFNoTauTau[5]="muTau_vbf_rescaled_8TeV_";

  TString E8TeVNoTauTau[NMAXINPUT];
  E8TeVNoTauTau[0]="emu_boost_low_rescaled_8TeV_";
  E8TeVNoTauTau[1]="eleTau_boost_low_rescaled_8TeV_";
  E8TeVNoTauTau[2]="muTau_boost_low_rescaled_8TeV_";
  E8TeVNoTauTau[3]="emu_boost_high_rescaled_8TeV_";
  E8TeVNoTauTau[4]="eleTau_boost_high_rescaled_8TeV_";
  E8TeVNoTauTau[5]="muTau_boost_high_rescaled_8TeV_";
  E8TeVNoTauTau[6]="emu_vbf_rescaled_8TeV_";
  E8TeVNoTauTau[7]="eleTau_vbf_rescaled_8TeV_";
  E8TeVNoTauTau[8]="muTau_vbf_rescaled_8TeV_";


  combine("All",All,"#sqrt{s} = 7 - 8 TeV ,   L = 24.3 fb^{-1}","e-#mu,e-#tau_{h},#mu-#tau_{h},#tau_{h}-#tau_{h}",1);
  combine("Boost",Boost,"#sqrt{s} = 7 - 8 TeV ,   L = 24.3 fb^{-1}","e-#mu,e-#tau_{h},#mu-#tau_{h},#tau_{h}-#tau_{h}",1);
  combine("VBF",VBF,"#sqrt{s} = 7 - 8 TeV ,   L = 24.3 fb^{-1}","e-#mu,e-#tau_{h},#mu-#tau_{h},#tau_{h}-#tau_{h}",1);
  
//   combine("EMu",EMu,"#sqrt{s} = 7 - 8 TeV ,   L = 24.3 fb^{-1}","e-#mu",1);
//   combine("ETau",ETau,"#sqrt{s} = 7 - 8 TeV ,   L = 24.3 fb^{-1}","e-#tau_{h}",1);
//   combine("MuTau",MuTau,"#sqrt{s} = 7 - 8 TeV ,   L = 24.3 fb^{-1}","#mu-#tau_{h}",1);
//   combine("TauTau",TauTau,"#sqrt{s} = 8 TeV ,   L = 19.4 fb^{-1}","#tau_{h}-#tau_{h}",1);
//   combine("BoostLow",BoostLow,"#sqrt{s} = 7 - 8 TeV ,   L = 24.3 fb^{-1}","1jet_low",1);
//   combine("BoostHigh",BoostHigh,"#sqrt{s} = 7 - 8 TeV ,   L = 24.3 fb^{-1}","1jet_high",1);
//   combine("E7TeV",E7TeV,"#sqrt{s} = 7 TeV ,   L = 4.9 fb^{-1}","e-#mu,e-#tau_{h},#mu-#tau_{h}",1);
//   combine("E8TeV",E8TeV,"#sqrt{s} = 8 TeV ,   L = 19.4 fb^{-1}","e-#mu,e-#tau_{h},#mu-#tau_{h},#tau_{h}-#tau_{h}",1);
  
//   combine("AllNoTauTau",AllNoTauTau,"#sqrt{s} = 7 - 8 TeV ,   L = 24.3 fb^{-1}","e-#mu,e-#tau_{h},#mu-#tau_{h}",1);
//   combine("BoostHighNoTauTau",BoostHighNoTauTau,"#sqrt{s} = 7 - 8 TeV ,   L = 24.3 fb^{-1}","Boost_High No TauTau",1);
//   combine("VBFNoTauTau",VBFNoTauTau,"#sqrt{s} = 7 - 8 TeV ,   L = 24.3 fb^{-1}","VBF No TauTau",1);
//   combine("E8TeVNoTauTau",E8TeVNoTauTau,"#sqrt{s} = 8 TeV ,   L = 19.4 fb^{-1}","e-#mu,e-#tau_{h},#mu-#tau_{h}",1);

//   combine("MuTauBoost8TeV",MuTauBoost8TeV,"#sqrt{s} = 8 TeV ,   L = 19.4 fb^{-1}","#mu-#tau_{h} Boost",1);
//   combine("MuTau8TeV",MuTau8TeV,"#sqrt{s} = 8 TeV ,   L = 19.4 fb^{-1}","#mu-#tau_{h}",1);
//  combine("MuTau0Jet8TeV",MuTau0Jet8TeV,2,0,"#sqrt{s} = 8 TeV ,   L = 19.4 fb^{-1}","#mu-#tau_{h} 0-Jet",1);


   gROOT->ProcessLine(".q");
}


