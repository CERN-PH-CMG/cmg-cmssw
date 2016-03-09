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
  if (argc != 2) {
    cout << "ERROR: " << argc-1 << " arguments provided instead of 1";
    exit(1);
  }
  const int Nvars=26;
  TString original, tokenized[Nvars];
  original = Form("%s",argv[1]);
      
  TObjArray* LineColumns = original.Tokenize(",");
  
  for(int j=0;j<Nvars;j++)
  {
    tokenized[j] = ((TObjString *)LineColumns->At(j))->GetString();
    // cout << "tokenized["<<j<<"]= " << tokenized[j] << endl;
  }
  int GetEntriesOrNchunck=tokenized[0].Atof();
  if(GetEntriesOrNchunck>-1) cout << "GetEntriesOrNchunck= " << GetEntriesOrNchunck << endl;
  int Entry_ini=tokenized[1].Atof();
  if(GetEntriesOrNchunck>-1) cout << "Entry_ini= " << Entry_ini << endl;
  int Entry_fin=tokenized[2].Atof();
  if(GetEntriesOrNchunck>-1) cout << "Entry_fin= " << Entry_fin << endl;
  TString ZfileDATA=tokenized[3];
  if(GetEntriesOrNchunck>-1) cout << "ZfileDATA= " << ZfileDATA << endl;
  double ZfileDATA_lumi_SF=tokenized[4].Atof();
  if(GetEntriesOrNchunck>-1) cout << "ZfileDATA_lumi_SF= " << ZfileDATA_lumi_SF << endl;
  TString sample=tokenized[5];
  if(GetEntriesOrNchunck>-1) cout << "sample= " << sample << endl;
  int useAlsoGenPforSig=tokenized[6].Atof();
  if(GetEntriesOrNchunck>-1) cout << "useAlsoGenPforSig= " << useAlsoGenPforSig << endl;
  int IS_MC_CLOSURE_TEST=tokenized[7].Atof();
  if(GetEntriesOrNchunck>-1) cout << "IS_MC_CLOSURE_TEST= " << IS_MC_CLOSURE_TEST << endl;
  int isMCorDATA=tokenized[8].Atof();
  if(GetEntriesOrNchunck>-1) cout << "isMCorDATA= " << isMCorDATA << endl;
  TString filename_outputdir=tokenized[9];
  if(GetEntriesOrNchunck>-1) cout << "filename_outputdir= " << filename_outputdir << endl;
  int useMomentumCorr=tokenized[10].Atof();
  if(GetEntriesOrNchunck>-1) cout << "useMomentumCorr= " << useMomentumCorr << endl;
  int useEffSF=tokenized[11].Atof();
  if(GetEntriesOrNchunck>-1) cout << "useEffSF= " << useEffSF << endl;
  int usePtSF=tokenized[12].Atof();
  if(GetEntriesOrNchunck>-1) cout << "usePtSF= " << usePtSF << endl;
  int usePileupSF=tokenized[13].Atof();
  if(GetEntriesOrNchunck>-1) cout << "usePileupSF= " << usePileupSF << endl;
  int controlplots=tokenized[14].Atof();
  if(GetEntriesOrNchunck>-1) cout << "controlplots= " << controlplots << endl;
  int generated_PDF_set=tokenized[15].Atof();
  if(GetEntriesOrNchunck>-1) cout << "generated_PDF_set= " << generated_PDF_set << endl;
  int generated_PDF_member=tokenized[16].Atof();
  if(GetEntriesOrNchunck>-1) cout << "generated_PDF_member= " << generated_PDF_member << endl;
  int contains_PDF_reweight=tokenized[17].Atof();
  if(GetEntriesOrNchunck>-1) cout << "contains_PDF_reweight= " << contains_PDF_reweight << endl;
  int usePhiMETCorr=tokenized[18].Atof();
  if(GetEntriesOrNchunck>-1) cout << "usePhiMETCorr= " << usePhiMETCorr << endl;
  int useRecoilCorr=tokenized[19].Atof();
  if(GetEntriesOrNchunck>-1) cout << "useRecoilCorr= " << useRecoilCorr << endl;
  int correctToMadgraph=tokenized[20].Atof();
  if(GetEntriesOrNchunck>-1) cout << "correctToMadgraph= " << correctToMadgraph << endl;
  int use_PForNoPUorTKmet=tokenized[21].Atof();
  if(GetEntriesOrNchunck>-1) cout << "use_PForNoPUorTKmet= " << use_PForNoPUorTKmet << endl;
  int use_syst_ewk_Alcaraz=tokenized[22].Atof();
  if(GetEntriesOrNchunck>-1) cout << "use_syst_ewk_Alcaraz= " << use_syst_ewk_Alcaraz << endl;
  int gen_mass_value_MeV=tokenized[23].Atof();
  if(GetEntriesOrNchunck>-1) cout << "gen_mass_value_MeV= " << gen_mass_value_MeV << endl;
  int contains_LHE_weights=tokenized[24].Atof();
  if(GetEntriesOrNchunck>-1) cout << "contains_LHE_weights= " << contains_LHE_weights << endl;
  int reweight_polarization=tokenized[25].Atof();
  if(GetEntriesOrNchunck>-1) cout << "reweight_polarization= " << reweight_polarization << endl;
  if(GetEntriesOrNchunck>-1) cout << endl;
  
//////////////  
  // cout << "processing line "<< Form("Zanalysis zDATA(\"%s\",%f,%d)",ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0) << endl;
  // Zanalysis zDATA(ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0);
  // cout << "processing line "<< Form("zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,%d,\"%s\",%d,%d,%d)",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight) << endl;
  // zDATA.Loop(IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),run_BuildEvByEvTemplates,useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight);

//////////////
  if(GetEntriesOrNchunck>-1) cout << "processing line "<< Form("Zanalysis zDATA(\"%s\",%f,%d)",ZfileDATA.Data(),ZfileDATA_lumi_SF,(sample.Contains("DYJetsPow") || sample.Contains("DYJetsMadSig"))?useAlsoGenPforSig:0) << endl << endl;
  Zanalysis zDATA(ZfileDATA.Data(),ZfileDATA_lumi_SF,(sample.Contains("DYJetsPow") || sample.Contains("DYJetsMadSig"))?useAlsoGenPforSig:0);
                             
  if(GetEntriesOrNchunck>-1) cout << "processing line "<< Form("zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,\"%s\",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight,usePhiMETCorr,useRecoilCorr,correctToMadgraph,use_PForNoPUorTKmet,use_syst_ewk_Alcaraz,gen_mass_value_MeV,contains_LHE_weights,reweight_polarization) << endl << endl;
  int nEntries;
  if(GetEntriesOrNchunck==-1){
    nEntries = zDATA.NumEntries();
    cout << "nEntries= " <<  nEntries << endl;
    return 0;
  }
  
  cout<< "chunk " << GetEntriesOrNchunck << " Entry_ini= " << Entry_ini << " Entry_fin= " << Entry_fin << endl;
  zDATA.Loop(GetEntriesOrNchunck,Entry_ini,Entry_fin,IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight,usePhiMETCorr,useRecoilCorr,correctToMadgraph,use_PForNoPUorTKmet,use_syst_ewk_Alcaraz,gen_mass_value_MeV,contains_LHE_weights,reweight_polarization);
  TString chunk_str = GetEntriesOrNchunck>0? Form("_chunk%d",GetEntriesOrNchunck) : "";
  TString outputfilename = "ZanalysisOnDATA";
  if(GetEntriesOrNchunck==0)
    gROOT->ProcessLine(Form(".! mv %s/Zanalysis%s.root %s/%s.root",filename_outputdir.Data(),chunk_str.Data(),filename_outputdir.Data(),outputfilename.Data()));
  // Race conditions
  gROOT->ProcessLine(".! sleep 1");
}

