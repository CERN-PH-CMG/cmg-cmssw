#include "../test_numbers_DATA/common.h"

void ClosureTest_fits(int generated_PDF_set=1, int generated_PDF_member=0){

  int counter=1;
  for(int h=0; h<WMass::PDF_members; h++){
    cout << "using pdf " << (WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets) << "-"<<h<< endl;
    
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      cout << "preparing pdf eta bin= " << i; fflush(stdout);
      for(int m=0; m<WMass::NtoysMomCorr; m++){
          if(WMass::NtoysMomCorr>1) cout << " MomCorr toy " << m; fflush(stdout);
        for(int j=0; j<2*WMass::WMassNSteps+1; j++){
          // for(int j=WMass::WMassNSteps-3; j<WMass::WMassNSteps+3; j++){
            int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;
            cout << " mass value= " << jWmass; fflush(stdout);
              // gROOT->ProcessLine(Form(".! combine -M MaxLikelihoodFit --saveNorm --justFit dummy_datacard_Wmass_MuPos_eta%s_%d.txt  -v10 -m %d  > dummy_datacard_Wmass_MuPos_eta%s_%d.log 2>&1",eta_str.Data(),jWmass,jWmass,eta_str.Data(),jWmass));
              // gROOT->ProcessLine(Form(".! combine -M MaxLikelihoodFit --saveNorm --justFit dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.txt  -v10 -m %d  > dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",eta_str.Data(),jWmass,jWmass,eta_str.Data(),jWmass));
              // gROOT->ProcessLine(Form(".! combine -m %d -M HybridNew --testStat=TEV --singlePoint 1 --onlyTestStat -t 10000 output_eta%s_%d.root   >> dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",jWmass,eta_str.Data(),jWmass,eta_str.Data(),jWmass));
              for(int k=0;k<3;k++){
                counter++;
                if(counter>1) gROOT->ProcessLine(".! sleep 1");
                if(counter%8==0) gROOT->ProcessLine(".! sleep 1");
                // cout << (Form(".!  text2workspace.py -P HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs dummy_datacard_Wmass_MuPos_pdf%d-%d%s_eta%s_%d_%sNonScaled.txt -o output_%s_pdf%d-%d%s_eta%s_%d.root -m %d  > dummy_datacard_Wmass_MuPos_pdf%d-%d%s_eta%s_%d_%sNonScaled.log 2>&1 &",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass,jWmass,WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data())) << endl;
                // uncomment lines below
                gROOT->ProcessLine(Form(".!  text2workspace.py -P HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs dummy_datacard_Wmass_MuPos_pdf%d-%d%s_eta%s_%d_%sNonScaled.txt -o output_%s_pdf%d-%d%s_eta%s_%d.root -m %d  > dummy_datacard_Wmass_MuPos_pdf%d-%d%s_eta%s_%d_%sNonScaled.log 2>&1 &",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass,jWmass,WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data()));
                // gROOT->ProcessLine(Form(".! combine -m %d -M HybridNew --testStat=TEV --singlePoint 1 --onlyTestStat output_eta%s_%d.root   >> dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",jWmass,eta_str.Data(),jWmass,eta_str.Data(),jWmass));
              }
          // }
        }
         cout << endl;
      }
    }
  // }

    for(int i=0; i<WMass::etaMuonNSteps; i++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      cout << "fitting eta bin= " << i ; fflush(stdout);
      for(int m=0; m<WMass::NtoysMomCorr; m++){
        if(WMass::NtoysMomCorr>1) cout << " MomCorr toy " << m; fflush(stdout);        
        for(int j=0; j<2*WMass::WMassNSteps+1; j++){
          // for(int j=WMass::WMassNSteps-3; j<WMass::WMassNSteps+3; j++){
          int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;
            cout << " mass value= " << jWmass; fflush(stdout);
            // gROOT->ProcessLine(Form(".! combine -M MaxLikelihoodFit --saveNorm --justFit dummy_datacard_Wmass_MuPos_eta%s_%d.txt  -v10 -m %d  > dummy_datacard_Wmass_MuPos_eta%s_%d.log 2>&1",eta_str.Data(),jWmass,jWmass,eta_str.Data(),jWmass));
            // gROOT->ProcessLine(Form(".! combine -M MaxLikelihoodFit --saveNorm --justFit dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.txt  -v10 -m %d  > dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",eta_str.Data(),jWmass,jWmass,eta_str.Data(),jWmass));
            // gROOT->ProcessLine(Form(".! combine -m %d -M HybridNew --testStat=TEV --singlePoint 1 --onlyTestStat -t 10000 output_eta%s_%d.root   >> dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",jWmass,eta_str.Data(),jWmass,eta_str.Data(),jWmass));
            for(int k=0;k<3;k++){
              counter++;
              if(counter>1) gROOT->ProcessLine(".! sleep 1");
              if(counter%8==0) gROOT->ProcessLine(".! sleep 1");
              // uncomment lines below
              // gROOT->ProcessLine(Form(".!  text2workspace.py -P HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.txt -o output_eta%s_%d.root -m %d  > dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",eta_str.Data(),jWmass,eta_str.Data(),jWmass,jWmass,eta_str.Data(),jWmass));
              gROOT->ProcessLine(Form(".! combine -m %d -M HybridNew --testStat=TEV --singlePoint 1 --onlyTestStat output_%s_pdf%d-%d%s_eta%s_%d.root   >> dummy_datacard_Wmass_MuPos_pdf%d-%d%s_eta%s_%d_%sNonScaled.log 2>&1 &",jWmass,WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass,WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data()));
            }
        }
      }
    }
    cout << endl;
  }
  
     // gROOT->ProcessLine(".! sleep 3");
}