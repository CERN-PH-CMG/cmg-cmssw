#include <iostream>
#include <sstream>
#include <fstream>
#include <TROOT.h>
#include <TString.h>
#include <TObjString.h>
#include "TObjArray.h"
#include "../common.h"
#include "../common2.h"

using namespace std;

void ClosureTest_fits(int generated_PDF_set=1, int generated_PDF_member=0, TString WorZ="W", int useBatch=0, TString currentdir_str="", int start_at_half=0, int RecoilCorrVarDiagoParU1orU2fromDATAorMC=0){

  int m_start = WMass::RecoilCorrIniVarDiagoParU1orU2fromDATAorMC_[RecoilCorrVarDiagoParU1orU2fromDATAorMC];
  int m_end = WMass::RecoilCorrNVarDiagoParU1orU2fromDATAorMC_[RecoilCorrVarDiagoParU1orU2fromDATAorMC];

  cout << "currentdir_str= " << currentdir_str << endl;
  TString original;
  std::vector<TString> tokenized;
  original = WorZ;
  TString     WCharge_str[]={"Pos","Neg"};
  
  TObjArray* LineColumns = original.Tokenize(",");
  
  for(int j=0;j<LineColumns->GetEntriesFast();j++)
  {
    tokenized.push_back(((TObjString *)LineColumns->At(j))->GetString());
  }
  
  for(unsigned int itoken=0; itoken<tokenized.size(); itoken++){

    WorZ = tokenized.at(itoken); // "Z" or "W"
    TString Wlike = WorZ.Contains("W")?"":"like";
    int charge_start = (WMass::WlikeCharge==1 || WorZ.Contains("W"))?0:1;
    int charge_end   = (WMass::WlikeCharge==1)                      ?1:2;
    
    for(int c=charge_start; c<charge_end; c++){
      cout << "Analyzing W"<<Wlike<<" " << WCharge_str[c].Data() << endl;
      for(int h=0; h<WMass::PDF_members; h++){
        cout << "using pdf " << (WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets) << "-"<<h<< endl;
        
        for(int i=0; i<WMass::etaMuonNSteps; i++){
          TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
          cout << "preparing pdf eta bin = " << i << endl;
          for(int m=m_start; m<m_end; m++){
            if(RecoilCorrVarDiagoParU1orU2fromDATAorMC>0)
              cout << "recoil eigenvar = " << m << endl;
            int KalmanNvariations_start=0;
            int KalmanNvariations_end=WMass::KalmanNvariations;
            for(int n=KalmanNvariations_start; n<KalmanNvariations_end; n++){
              cout << "kalman corr variation = " << n << endl;
              for(int j=0; j<2*WMass2::WMassNSteps+1; j++){
                int jWmass = WorZ.Contains("Z")? WMass2::Zmass_values_array[j] : WMass2::Wmass_values_array[j];
                cout << "mass value = " << jWmass << endl;
                for(int k=0;k<WMass::NFitVar;k++){
                  TString text2workspace_str = Form("text2workspace.py -P HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs dummy_datacard_Wmass_Mu%s%s_pdf%d-%d%s%s_eta%s_%d_%sNonScaled.txt -o output_W%s%s_%s_pdf%d-%d%s%s_eta%s_%d.root -m %d  > dummy_datacard_Wmass_Mu%s%s_pdf%d-%d%s%s_eta%s_%d_%sNonScaled.log 2>&1",Wlike.Data(),WCharge_str[c].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data(),Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),jWmass,jWmass,Wlike.Data(),WCharge_str[c].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data());
                  TString combine_str = Form("combine -S 0 -m %d -M HybridNew --testStat=TEV --singlePoint 1 --onlyTestStat output_W%s%s_%s_pdf%d-%d%s%s_eta%s_%d.root   >> dummy_datacard_Wmass_Mu%s%s_pdf%d-%d%s%s_eta%s_%d_%sNonScaled.log 2>&1 ",jWmass,Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),jWmass,Wlike.Data(),WCharge_str[c].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data());
                  TString outfilename_str = Form("submit_datacard_Wmass_Mu%s%s_pdf%d-%d%s%s_eta%s_%d_%sNonScaled.sh",Wlike.Data(),WCharge_str[c].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data());
                  if(useBatch==0){
                    gROOT->ProcessLine(".! "+text2workspace_str);
                    gROOT->ProcessLine(".! "+combine_str+" &");
                  }else{
                    ofstream outTXTfile;
                    cout << "creating " << outfilename_str << endl;
                    outTXTfile.open(outfilename_str);
                    outTXTfile << "user=$(whoami); cd /afs/cern.ch/work/${user:0:1}/${user}/private/CMSSW_6_1_1/src; SCRAM_ARCH=slc5_amd64_gcc462; eval `scramv1 runtime -sh`; cd -;\n";
                    outTXTfile << "source /afs/cern.ch/sw/lcg/contrib/gcc/4.6/x86_64-slc6-gcc46-opt/setup.sh;\n";
                    outTXTfile << "cd "<<currentdir_str<<"\n";
                    outTXTfile << text2workspace_str << endl;
                    outTXTfile << combine_str << endl;
                    outTXTfile.close();
                    // gROOT->ProcessLine(".! usleep 200");
                    gROOT->ProcessLine(".! sleep 1");
                    // gROOT->ProcessLine(".! ls -lrt "+outfilename_str);
                    gROOT->ProcessLine(".! chmod 755 "+outfilename_str);
                    gROOT->ProcessLine(".! bsub -C 0 -u pippo123 -q 1nh -J runMfit "+outfilename_str);
                    gROOT->ProcessLine(".! rm -rf LSF* ");

                  }
                }
              }
              cout << endl;
            }
          }
        }
        
      }
    }
  }
  // The sleep 1 fixes a race condition with the last fit (afs is sloooow)
  gROOT->ProcessLine(".! sleep 1");
  gROOT->ProcessLine(".! rm -rf LSF* ");
}
