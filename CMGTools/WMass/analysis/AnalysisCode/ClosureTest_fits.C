#include "../output_DATA/common.h"

void ClosureTest_fits(){

  int counter=1;
  // for(int i=0; i<WMass::etaMuonNSteps; i++){
  for(int i=0; i<1; i++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;
        // gROOT->ProcessLine(Form(".! combine -M MaxLikelihoodFit --saveNorm --justFit dummy_datacard_Wmass_MuPos_eta%s_%d.txt  -v10 -m %d  > dummy_datacard_Wmass_MuPos_eta%s_%d.log 2>&1",eta_str.Data(),jWmass,jWmass,eta_str.Data(),jWmass));
        gROOT->ProcessLine(Form(".! combine -M MaxLikelihoodFit --saveNorm --justFit dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.txt  -v10 -m %d  > dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",eta_str.Data(),jWmass,jWmass,eta_str.Data(),jWmass));
        // if(counter%5==0) gROOT->ProcessLine(".! sleep 1");
    }
  }
  
}