#include <iostream>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TROOT.h>
#include "Zanalysis_controlplots.h"
// #include "rochcor_44X_v3.h"

using namespace std;
int main(int argc, char ** argv) 
{

  TString original, tokenized[17];
  original = Form("%s",argv[1]);
      
  TObjArray* LineColumns = original.Tokenize(",");
  
  for(int j=0;j<17;j++)
  {
    tokenized[j] = ((TObjString *)LineColumns->At(j))->GetString();
    // cout << "tokenized["<<j<<"]= " << tokenized[j] << endl;
  }
  TString ZfileDATA=tokenized[0];
  double ZfileDATA_lumi_SF=tokenized[1].Atof();
  TString sample=tokenized[2];
  int useAlsoGenPforSig=tokenized[3].Atof();
  int IS_MC_CLOSURE_TEST=tokenized[4].Atof();
  int isMCorDATA=tokenized[5].Atof();
  TString filename_outputdir=tokenized[6];
  int useMomentumCorr=tokenized[7].Atof();
  int smearRochCorrByNsigma=tokenized[8].Atof();
  int useEffSF=tokenized[9].Atof();
  int usePtSF=tokenized[10].Atof();
  int usePileupSF=tokenized[11].Atof();
  int run_BuildEvByEvTemplates=tokenized[12].Atof();
  int controlplots=tokenized[13].Atof();
  int generated_PDF_set=tokenized[14].Atof();
  int generated_PDF_member=tokenized[15].Atof();
  int contains_PDF_reweight=tokenized[16].Atof();
  
  cout << "processing line "<< Form("Zanalysis_controlplots zDATA(\"%s\",%f,%d)",ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0) << endl;
  Zanalysis_controlplots zDATA(ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0);
  cout << "processing line "<< Form("zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,%d,\"%s\",%d,%d,%d)",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight) << endl;
  zDATA.Loop(IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),run_BuildEvByEvTemplates,useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight);
  TString outputfilename = run_BuildEvByEvTemplates ? "WEvByEvTemplates" : "Zanalysis_controlplotsOnDATA";
  gROOT->ProcessLine(Form(".! mv ../%s/Zanalysis_controlplots.root ../%s/%s.root",filename_outputdir.Data(),filename_outputdir.Data(),outputfilename.Data()));
  gROOT->ProcessLine(Form(".! cp Zanalysis_controlplots.* ../%s/",filename_outputdir.Data()));

}
