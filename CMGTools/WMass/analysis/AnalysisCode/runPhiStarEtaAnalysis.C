#include <iostream>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TROOT.h>
#include "PhiStarEtaAnalysis.h"
// #include "rochcor_44X_v3.h"

using namespace std;
int main(int argc, char ** argv) 
{

  TString original, tokenized[20];
  original = Form("%s",argv[1]);
      
  TObjArray* LineColumns = original.Tokenize(",");
  
  for(int j=0;j<20;j++)
  {
    tokenized[j] = ((TObjString *)LineColumns->At(j))->GetString();
    // cout << "tokenized["<<j<<"]= " << tokenized[j] << endl;
  }
  int GetEntriesOrNchunck=tokenized[0].Atof();
  int Entry_ini=tokenized[1].Atof();
  int Entry_fin=tokenized[2].Atof();  
  TString ZfileDATA=tokenized[3];
  double ZfileDATA_lumi_SF=tokenized[4].Atof();
  TString sample=tokenized[5];
  int useAlsoGenPforSig=tokenized[6].Atof();
  int IS_MC_CLOSURE_TEST=tokenized[7].Atof();
  int isMCorDATA=tokenized[8].Atof();
  TString filename_outputdir=tokenized[9];
  int useMomentumCorr=tokenized[10].Atof();
  int smearRochCorrByNsigma=tokenized[11].Atof();
  int useEffSF=tokenized[12].Atof();
  int usePtSF=tokenized[13].Atof();
  int usePileupSF=tokenized[14].Atof();
  int run_BuildEvByEvTemplates=tokenized[15].Atof();
  int controlplots=tokenized[16].Atof();
  int generated_PDF_set=tokenized[17].Atof();
  int generated_PDF_member=tokenized[18].Atof();
  int contains_PDF_reweight=tokenized[19].Atof();
  
//////////////////////////////////////////////////
  
  // if(GetEntriesOrNchunck>-1) cout << "processing line "<< Form("Wanalysis wDATA(\"%s\",%f,%d)",WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0) << endl;
  if(GetEntriesOrNchunck>-1) cout << "processing line "<< Form("PhiStarEtaAnalysis zDATA(\"%s\",%f,%d)",ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0) << endl;
  // Wanalysis wDATA(WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0);
  PhiStarEtaAnalysis zDATA(ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0);
  // if(GetEntriesOrNchunck>-1) cout << "processing line "<< Form("wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,%d,\"%s\",%d,%d,%d,%d,%d)",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight,usePhiMETCorr,useRecoilCorr,RecoilCorrResolutionNSigma,RecoilCorrScaleNSigma) << endl;
  if(GetEntriesOrNchunck>-1) cout << "processing line "<< Form("zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,%d,\"%s\",%d,%d,%d)",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight) << endl;  
  
  int nEntries;
  if(GetEntriesOrNchunck==-1){
    // nEntries = wDATA.NumEntries();
    nEntries = zDATA.NumEntries();
    cout << "nEntries= " <<  nEntries << endl;
    return 0;
  }
  
  cout<< "chunk " << GetEntriesOrNchunck << " Entry_ini= " << Entry_ini << " Entry_fin= " << Entry_fin << endl;
  // wDATA.Loop(GetEntriesOrNchunck,Entry_ini,Entry_fin,IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight,usePhiMETCorr,useRecoilCorr,RecoilCorrResolutionNSigma,RecoilCorrScaleNSigma);
  zDATA.Loop(GetEntriesOrNchunck,Entry_ini,Entry_fin,IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),run_BuildEvByEvTemplates,useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight);
  TString chunk_str = GetEntriesOrNchunck>0? Form("_chunk%d",GetEntriesOrNchunck) : "";
  TString outputfilename = run_BuildEvByEvTemplates ? "WEvByEvTemplates" : "PhiStarEtaAnalysisOnDATA";
  if(GetEntriesOrNchunck==0)
    // gROOT->ProcessLine(Form(".! mv ../%s/Wanalysis%s.root ../%s/WanalysisOnDATA.root",filename_outputdir.Data(),chunk_str.Data(),filename_outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv ../%s/PhiStarEtaAnalysis%s.root ../%s/%s.root",filename_outputdir.Data(),filename_outputdir.Data(),outputfilename.Data()));
  // gROOT->ProcessLine(Form(".! cp Wanalysis.* ../%s/",filename_outputdir.Data()));
  gROOT->ProcessLine(Form(".! cp PhiStarEtaAnalysis.* ../%s/",filename_outputdir.Data()));
  
//////////////////////////////////////////////////  

}

