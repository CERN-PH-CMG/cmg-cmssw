#include <iostream>
#include <TROOT.h>
#include "../test_numbers_DATA/common.h"
#include "../../../includes/common2.h"

void ClosureTest_fits(int generated_PDF_set=1, int generated_PDF_member=0, TString WorZ="W"){

  TString original;
  std::vector<TString> tokenized;
  original = WorZ;
  TString     WCharge_str[]={"Pos","Neg"};
      
  TObjArray* LineColumns = original.Tokenize(",");
  
  for(int j=0;j<LineColumns->GetEntriesFast();j++)
  {
    tokenized.push_back(((TObjString *)LineColumns->At(j))->GetString());
    // cout << "tokenized["<<j<<"]= " << tokenized[j] << endl;
  }
  
  for(unsigned int itoken=0; itoken<tokenized.size(); itoken++){

    // TString WorZ = "Z"; // "Z" or "W"
    WorZ = tokenized.at(itoken); // "Z" or "W"
    TString Wlike = WorZ.Contains("W")?"":"like";
    int charges = WorZ.Contains("W")?2:1;
    
    for(int c=0; c<charges; c++){
      cout << "Analyzing W"<<Wlike<<" " << WCharge_str[c].Data() << endl;
      int counter=1;
      for(int h=0; h<WMass::PDF_members; h++){
        cout << "using pdf " << (WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets) << "-"<<h<< endl;
        
        for(int i=0; i<WMass::etaMuonNSteps; i++){
          TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
          cout << "preparing pdf eta bin= " << i; fflush(stdout);
          for(int m=0; m<WMass::NVarRecoilCorr; m++){
              if(WMass::NVarRecoilCorr>1) cout << " MomCorr toy " << m; fflush(stdout);
            for(int j=0; j<2*WMass2::WMassNSteps+1; j++){
              // for(int j=WMass2::WMassNSteps-3; j<WMass2::WMassNSteps+3; j++){
                // int jWmass = WMass2::WMassCentral_MeV-(WMass2::WMassNSteps-j)*WMass2::WMassStep_MeV;
                int jWmass = WorZ.Contains("Z")? WMass2::Zmass_values_array[j] : WMass2::Wmass_values_array[j];
                cout << " mass value= " << jWmass; fflush(stdout);
                  // gROOT->ProcessLine(Form(".! combine -M MaxLikelihoodFit --saveNorm --justFit dummy_datacard_Wmass_MuPos_eta%s_%d.txt  -v10 -m %d  > dummy_datacard_Wmass_MuPos_eta%s_%d.log 2>&1",eta_str.Data(),jWmass,jWmass,eta_str.Data(),jWmass));
                  // gROOT->ProcessLine(Form(".! combine -M MaxLikelihoodFit --saveNorm --justFit dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.txt  -v10 -m %d  > dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",eta_str.Data(),jWmass,jWmass,eta_str.Data(),jWmass));
                  // gROOT->ProcessLine(Form(".! combine -m %d -M HybridNew --testStat=TEV --singlePoint 1 --onlyTestStat -t 10000 output_eta%s_%d.root   >> dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",jWmass,eta_str.Data(),jWmass,eta_str.Data(),jWmass));
                  for(int k=0;k<WMass::NFitVar;k++){
                    counter++;
                    if(counter>1 && WMass::PDF_members>1) gROOT->ProcessLine(".! sleep 1");
                    else if(counter>1) gROOT->ProcessLine(".! usleep 300000");
                    // if(counter%5==0) gROOT->ProcessLine(".! sleep 1");
                    // uncomment lines below
                    gROOT->ProcessLine(Form(".!  text2workspace.py -P HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs dummy_datacard_Wmass_Mu%s%s_pdf%d-%d%s_eta%s_%d_%sNonScaled.txt -o output_W%s%s_%s_pdf%d-%d%s_eta%s_%d.root -m %d  > dummy_datacard_Wmass_Mu%s%s_pdf%d-%d%s_eta%s_%d_%sNonScaled.log 2>&1 &",Wlike.Data(),WCharge_str[c].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data(),Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass,jWmass,Wlike.Data(),WCharge_str[c].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data()));
                    // gROOT->ProcessLine(Form(".! combine -m %d -M HybridNew --testStat=TEV --singlePoint 1 --onlyTestStat output_eta%s_%d.root   >> dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",jWmass,eta_str.Data(),jWmass,eta_str.Data(),jWmass));
                  }
              // }
            }
             cout << endl;
          }
        }
      // }
      
      gROOT->ProcessLine(".! sleep 3");
      
        for(int i=0; i<WMass::etaMuonNSteps; i++){
          TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
          cout << "fitting eta bin= " << i ; fflush(stdout);
          for(int m=0; m<WMass::NVarRecoilCorr; m++){
            if(WMass::NVarRecoilCorr>1) cout << " MomCorr toy " << m; fflush(stdout);        
            for(int j=0; j<2*WMass2::WMassNSteps+1; j++){
              // for(int j=WMass2::WMassNSteps-3; j<WMass2::WMassNSteps+3; j++){
              // int jWmass = WMass2::WMassCentral_MeV-(WMass2::WMassNSteps-j)*WMass2::WMassStep_MeV;
              int jWmass = WorZ.Contains("Z")? WMass2::Zmass_values_array[j] : WMass2::Wmass_values_array[j];
                cout << " mass value= " << jWmass; fflush(stdout);
                // gROOT->ProcessLine(Form(".! combine -M MaxLikelihoodFit --saveNorm --justFit dummy_datacard_Wmass_MuPos_eta%s_%d.txt  -v10 -m %d  > dummy_datacard_Wmass_MuPos_eta%s_%d.log 2>&1",eta_str.Data(),jWmass,jWmass,eta_str.Data(),jWmass));
                // gROOT->ProcessLine(Form(".! combine -M MaxLikelihoodFit --saveNorm --justFit dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.txt  -v10 -m %d  > dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",eta_str.Data(),jWmass,jWmass,eta_str.Data(),jWmass));
                // gROOT->ProcessLine(Form(".! combine -m %d -M HybridNew --testStat=TEV --singlePoint 1 --onlyTestStat -t 10000 output_eta%s_%d.root   >> dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",jWmass,eta_str.Data(),jWmass,eta_str.Data(),jWmass));
                for(int k=0;k<WMass::NFitVar;k++){
                  counter++;
                  // if(counter>1) gROOT->ProcessLine(".! sleep 1");
                  if(counter>1 && WMass::PDF_members>1) gROOT->ProcessLine(".! sleep 1");
                    else if(counter>1)  gROOT->ProcessLine(".! usleep 300000");
                  // if(counter%5==0) gROOT->ProcessLine(".! sleep 1");
                  // uncomment lines below
                  // gROOT->ProcessLine(Form(".!  text2workspace.py -P HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.txt -o output_eta%s_%d.root -m %d  > dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log 2>&1",eta_str.Data(),jWmass,eta_str.Data(),jWmass,jWmass,eta_str.Data(),jWmass));
                  gROOT->ProcessLine(Form(".! combine -m %d -M HybridNew --testStat=TEV --singlePoint 1 --onlyTestStat output_W%s%s_%s_pdf%d-%d%s_eta%s_%d.root   >> dummy_datacard_Wmass_Mu%s%s_pdf%d-%d%s_eta%s_%d_%sNonScaled.log 2>&1 &",jWmass,Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass,Wlike.Data(),WCharge_str[c].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data()));
                }
            }
          }
        }
        cout << endl;
      
      }
    }
  }
     // gROOT->ProcessLine(".! sleep 3");
}