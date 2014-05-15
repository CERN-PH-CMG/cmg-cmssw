#include <iostream>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TROOT.h>
#include "Zanalysis.h"
// #include "rochcor_44X_v3.h"

using namespace std;
int main(int argc, char ** argv) 
{

  const int Nvars=27;
  TString original, tokenized[Nvars];
  original = Form("%s",argv[1]);
      
  TObjArray* LineColumns = original.Tokenize(",");
  
  for(int j=0;j<Nvars;j++)
  {
    tokenized[j] = ((TObjString *)LineColumns->At(j))->GetString();
    // cout << "tokenized["<<j<<"]= " << tokenized[j] << endl;
  }
  int GetEntriesOrNchunck=tokenized[0].Atof();
  if(GetEntriesOrNchunck>-1) cout << "GetEntriesOrNchunck= " << GetEntriesOrNchunck;
  int Entry_ini=tokenized[1].Atof();
  if(GetEntriesOrNchunck>-1) cout << " Entry_ini= " << Entry_ini;
  int Entry_fin=tokenized[2].Atof();
  if(GetEntriesOrNchunck>-1) cout << " Entry_fin= " << Entry_fin;
  TString ZfileDATA=tokenized[3];
  if(GetEntriesOrNchunck>-1) cout << " ZfileDATA= " << ZfileDATA;
  double ZfileDATA_lumi_SF=tokenized[4].Atof();
  if(GetEntriesOrNchunck>-1) cout << " ZfileDATA_lumi_SF= " << ZfileDATA_lumi_SF;
  TString sample=tokenized[5];
  if(GetEntriesOrNchunck>-1) cout << " sample= " << sample;
  int useAlsoGenPforSig=tokenized[6].Atof();
  if(GetEntriesOrNchunck>-1) cout << " useAlsoGenPforSig= " << useAlsoGenPforSig;
  int IS_MC_CLOSURE_TEST=tokenized[7].Atof();
  if(GetEntriesOrNchunck>-1) cout << " IS_MC_CLOSURE_TEST= " << IS_MC_CLOSURE_TEST;
  int isMCorDATA=tokenized[8].Atof();
  if(GetEntriesOrNchunck>-1) cout << " isMCorDATA= " << isMCorDATA;
  TString filename_outputdir=tokenized[9];
  if(GetEntriesOrNchunck>-1) cout << " filename_outputdir= " << filename_outputdir;
  int useMomentumCorr=tokenized[10].Atof();
  if(GetEntriesOrNchunck>-1) cout << " useMomentumCorr= " << useMomentumCorr;
  int smearRochCorrByNsigma=tokenized[11].Atof();
  if(GetEntriesOrNchunck>-1) cout << " smearRochCorrByNsigma= " << smearRochCorrByNsigma;
  int useEffSF=tokenized[12].Atof();
  if(GetEntriesOrNchunck>-1) cout << " useEffSF= " << useEffSF;
  int usePtSF=tokenized[13].Atof();
  if(GetEntriesOrNchunck>-1) cout << " usePtSF= " << usePtSF;
  int usePileupSF=tokenized[14].Atof();
  if(GetEntriesOrNchunck>-1) cout << " usePileupSF= " << usePileupSF;
  int run_BuildEvByEvTemplates=tokenized[15].Atof();
  if(GetEntriesOrNchunck>-1) cout << " run_BuildEvByEvTemplates= " << run_BuildEvByEvTemplates;
  int controlplots=tokenized[16].Atof();
  if(GetEntriesOrNchunck>-1) cout << " controlplots= " << controlplots;
  int generated_PDF_set=tokenized[17].Atof();
  if(GetEntriesOrNchunck>-1) cout << " generated_PDF_set= " << generated_PDF_set;
  int generated_PDF_member=tokenized[18].Atof();
  if(GetEntriesOrNchunck>-1) cout << " generated_PDF_member= " << generated_PDF_member;
  int contains_PDF_reweight=tokenized[19].Atof();
  if(GetEntriesOrNchunck>-1) cout << " contains_PDF_reweight= " << contains_PDF_reweight;
  int usePhiMETCorr=tokenized[20].Atof();
  if(GetEntriesOrNchunck>-1) cout << " usePhiMETCorr= " << usePhiMETCorr;
  int useRecoilCorr=tokenized[21].Atof();
  if(GetEntriesOrNchunck>-1) cout << " useRecoilCorr= " << useRecoilCorr;
  int RecoilCorrResolutionNSigmaU1=tokenized[22].Atof();
  if(GetEntriesOrNchunck>-1) cout << " RecoilCorrResolutionNSigmaU1= " << RecoilCorrResolutionNSigmaU1;
  int RecoilCorrScaleNSigmaU1=tokenized[23].Atof();
  if(GetEntriesOrNchunck>-1) cout << " RecoilCorrScaleNSigmaU1= " << RecoilCorrScaleNSigmaU1;
  int RecoilCorrResolutionNSigmaU2=tokenized[24].Atof();
  if(GetEntriesOrNchunck>-1) cout << " RecoilCorrResolutionNSigmaU2= " << RecoilCorrResolutionNSigmaU2;
  int use_PForNoPUorTKmet=tokenized[25].Atof();
  if(GetEntriesOrNchunck>-1) cout << " use_PForNoPUorTKmet= " << use_PForNoPUorTKmet;
  int use_syst_ewk_Alcaraz=tokenized[26].Atof();
  if(GetEntriesOrNchunck>-1) cout << " use_syst_ewk_Alcaraz= " << use_syst_ewk_Alcaraz;
  if(GetEntriesOrNchunck>-1) cout << endl;
  
//////////////  
  // cout << "processing line "<< Form("Zanalysis zDATA(\"%s\",%f,%d)",ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0) << endl;
  // Zanalysis zDATA(ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0);
  // cout << "processing line "<< Form("zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,%d,\"%s\",%d,%d,%d)",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight) << endl;
  // zDATA.Loop(IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),run_BuildEvByEvTemplates,useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight);

//////////////
  if(GetEntriesOrNchunck>-1) cout << "processing line "<< Form("Zanalysis zDATA(\"%s\",%f,%d)",ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0) << endl;
  Zanalysis zDATA(ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0);
                             
  if(GetEntriesOrNchunck>-1) cout << "processing line "<< Form("zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,%d,\"%s\",%d,%d,%d,%d,%d,%d,%d)",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight,usePhiMETCorr,useRecoilCorr,RecoilCorrResolutionNSigmaU1,RecoilCorrScaleNSigmaU1,RecoilCorrResolutionNSigmaU2,use_PForNoPUorTKmet,use_syst_ewk_Alcaraz) << endl;
  int nEntries;
  if(GetEntriesOrNchunck==-1){
    nEntries = zDATA.NumEntries();
    cout << "nEntries= " <<  nEntries << endl;
    return 0;
  }
  
  cout<< "chunk " << GetEntriesOrNchunck << " Entry_ini= " << Entry_ini << " Entry_fin= " << Entry_fin << endl;
  zDATA.Loop(GetEntriesOrNchunck,Entry_ini,Entry_fin,IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),run_BuildEvByEvTemplates,useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight,usePhiMETCorr,useRecoilCorr,RecoilCorrResolutionNSigmaU1,RecoilCorrScaleNSigmaU1,RecoilCorrResolutionNSigmaU2,use_PForNoPUorTKmet,use_syst_ewk_Alcaraz);
  TString chunk_str = GetEntriesOrNchunck>0? Form("_chunk%d",GetEntriesOrNchunck) : "";
  TString outputfilename = run_BuildEvByEvTemplates ? "WEvByEvTemplates" : "ZanalysisOnDATA";
  if(GetEntriesOrNchunck==0)
    gROOT->ProcessLine(Form(".! mv ../%s/Zanalysis%s.root ../%s/%s.root",filename_outputdir.Data(),chunk_str.Data(),filename_outputdir.Data(),outputfilename.Data()));
  gROOT->ProcessLine(Form(".! cp Zanalysis.* ../%s/",filename_outputdir.Data()));
  
  
}


// void runZanalysis(TString ZfileDATA, double ZfileDATA_lumi_SF, TString sample, int useAlsoGenPforSig, int IS_MC_CLOSURE_TEST, int isMCorDATA, TString filename_outputdir, int useMomentumCorr, int smearRochCorrByNsigma, int useEffSF, int usePileupSF, int run_BuildEvByEvTemplates, int controlplots)
// {

  // // gSystem->CompileMacro("MuScleFitCorrector.cc");
  // // gSystem->CompileMacro("rochcor_44X_v3.C");
  // // gSystem->CompileMacro("rochcor_42X.C");
  // // gSystem->CompileMacro("Zanalysis.C","kO-");
  // gROOT->ProcessLine(".L rochcor_44X_v3.C+");
  // gROOT->ProcessLine(".L Zanalysis.C+");
  // cout << "processing line "<< Form("Zanalysis zDATA(\"%s\",%f,%d)",ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0) << endl;
  // gROOT->ProcessLine(Form("Zanalysis zDATA(\"%s\",%f,%d)",ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0));
  // cout << Form("zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),run_BuildEvByEvTemplates,useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,controlplots,sample.Data()) << endl;
  // gROOT->ProcessLine(Form("zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),run_BuildEvByEvTemplates,useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,controlplots,sample.Data()));

  // TString outputfilename = run_BuildEvByEvTemplates ? "WEvByEvTemplates" : "ZanalysisOnDATA";
  // gROOT->ProcessLine(Form(".! mv ../%s/Zanalysis.root ../%s/%s.root",filename_outputdir.Data(),filename_outputdir.Data(),outputfilename.Data()));
  // gROOT->ProcessLine(Form(".! cp Zanalysis.\* ../%s/",filename_outputdir.Data()));
  // // // if(IS_MC_CLOSURE_TEST and sample.contains("DYJetsSig")){
    // // // cout << Form("making direcory ../JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data())) << endl;
    // // // gSystem->MakeDirectory(Form("../JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data())));
    // // // TString filename_outputdir2 = Form("JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data()));
    // // // cout << Form("zDATA.Loop(%d,%d,\"../%s\",0,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,1,filename_outputdir2.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()) << endl;
    // // // gROOT->ProcessLine(Form("zDATA.Loop(%d,%d,\"../%s\",0,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,1,filename_outputdir2.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()));
    // // // gROOT->ProcessLine(Form(".! mv ../%s/Zanalysis.root ../%s/ZanalysisOnDATA.root",filename_outputdir2.Data(),filename_outputdir2.Data()));
  // // // }




  // // gSystem->CompileMacro("Zanalysis.C");
  // // cout << "processing line "<< Form("Zanalysis zDATA(\"%s\",%f,%d)",WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0) << endl;
  // // // gROOT->ProcessLine(Form("Zanalysis zDATA(\"%s\",%f,%d); zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0,IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample.Data()));
  // // gROOT->ProcessLine(Form("Zanalysis zDATA(\"%s\",%f,%d)",WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0));
  // // cout << "processing line "<< Form("zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample.Data()) << endl;
  // // gROOT->ProcessLine(Form("zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample.Data()));
  // // gROOT->ProcessLine(Form(".! mv ../%s/Zanalysis.root ../%s/WanalysisOnDATA.root",filename_outputdir.Data(),filename_outputdir.Data()));
  // // gROOT->ProcessLine(Form(".! cp Zanalysis.\* ../JobOutputs/%s/",filename_outputdir.Data()));

// }
