#include <iostream>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TROOT.h>
#include "Wanalysis.h"
// #include "rochcor_44X_v3.h"

using namespace std;
int main(int argc, char ** argv) 
{

  TString original, tokenized[15];
  original = Form("%s",argv[1]);
      
  TObjArray* LineColumns = original.Tokenize(",");
  
  for(int j=0;j<15;j++)
  {
    tokenized[j] = ((TObjString *)LineColumns->At(j))->GetString();
  }

  TString WfileDATA=tokenized[0];
  double WfileDATA_lumi_SF=tokenized[1].Atof();
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
  int controlplots=tokenized[12].Atof();
  int generated_PDF_set=tokenized[13].Atof();
  int generated_PDF_member=tokenized[14].Atof();
  int contains_PDF_reweight=tokenized[15].Atof();
  cout << "processing line "<< Form("Wanalysis wDATA(\"%s\",%f,%d)",WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0) << endl;
  Wanalysis wDATA(WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0);
  cout << "processing line "<< Form("wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,%d,\"%s\",%d,%d,%d)",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight) << endl;
  wDATA.Loop(IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight);
  gROOT->ProcessLine(Form(".! mv ../%s/Wanalysis.root ../%s/WanalysisOnDATA.root",filename_outputdir.Data(),filename_outputdir.Data()));
  gROOT->ProcessLine(Form(".! cp Wanalysis.* ../%s/",filename_outputdir.Data()));

}

// void runWanalysis(TString WfileDATA, double WfileDATA_lumi_SF, TString sample, int useAlsoGenPforSig, int IS_MC_CLOSURE_TEST, int isMCorDATA, TString filename_outputdir, int useMomentumCorr, int smearRochCorrByNsigma, int useEffSF, int usePileupSF, int controlplots)
// {

  // // gSystem->CompileMacro("MuScleFitCorrector.cc");
  // gSystem->CompileMacro("rochcor_44X_v3.C");
  // // gSystem->CompileMacro("rochcor_42X.C");
  // gSystem->CompileMacro("Wanalysis.C");
  // cout << "processing line "<< Form("Wanalysis wDATA(\"%s\",%f,%d)",WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0) << endl;
  // // gROOT->ProcessLine(Form("Wanalysis wDATA(\"%s\",%f,%d); wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0,IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample.Data()));
  // gROOT->ProcessLine(Form("Wanalysis wDATA(\"%s\",%f,%d)",WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0));
  // cout << "processing line "<< Form("wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,controlplots,sample.Data()) << endl;
  // gROOT->ProcessLine(Form("wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,controlplots,sample.Data()));
  // gROOT->ProcessLine(Form(".! mv ../%s/Wanalysis.root ../%s/WanalysisOnDATA.root",filename_outputdir.Data(),filename_outputdir.Data()));
  // gROOT->ProcessLine(Form(".! cp Wanalysis.\* ../%s/",filename_outputdir.Data()));
  // // if(IS_MC_CLOSURE_TEST && sample.Contains("WJetsSig")){
    // // cout << Form("making direcory ../JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data())) << endl;
    // // gSystem->MakeDirectory(Form("../JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data())));
    // // TString filename_outputdir2 = Form("JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data()));
    // // cout << Form("wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,1,filename_outputdir2.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()) << endl;
    // // gROOT->ProcessLine(Form("wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,1,filename_outputdir2.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()));
    // // gROOT->ProcessLine(Form(".! mv ../%s/Wanalysis.root ../%s/WanalysisOnDATA.root",filename_outputdir2.Data(),filename_outputdir2.Data()));
  // // }

// }
