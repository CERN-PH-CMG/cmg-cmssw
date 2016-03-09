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

  const int Nvars=28;
  TString original, tokenized[Nvars];
  original = Form("%s",argv[1]);
      
  TObjArray* LineColumns = original.Tokenize(",");
  
  for(int j=0;j<Nvars;j++)
  {
    tokenized[j] = ((TObjString *)LineColumns->At(j))->GetString();
  }

  int GetEntriesOrNchunck=tokenized[0].Atof();
  if(GetEntriesOrNchunck>-1) cout << "GetEntriesOrNchunck= " << GetEntriesOrNchunck;
  int Entry_ini=tokenized[1].Atof();
  if(GetEntriesOrNchunck>-1) cout << " Entry_ini= " << Entry_ini;
  int Entry_fin=tokenized[2].Atof();
  if(GetEntriesOrNchunck>-1) cout << " Entry_fin= " << Entry_fin;
  TString WfileDATA=tokenized[3];
  if(GetEntriesOrNchunck>-1) cout << " WfileDATA= " << WfileDATA;
  double WfileDATA_lumi_SF=tokenized[4].Atof();
  if(GetEntriesOrNchunck>-1) cout << " WfileDATA_lumi_SF= " << WfileDATA_lumi_SF;
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
  int controlplots=tokenized[15].Atof();
  if(GetEntriesOrNchunck>-1) cout << " controlplots= " << controlplots;
  int generated_PDF_set=tokenized[16].Atof();
  if(GetEntriesOrNchunck>-1) cout << " generated_PDF_set= " << generated_PDF_set;
  int generated_PDF_member=tokenized[17].Atof();
  if(GetEntriesOrNchunck>-1) cout << " generated_PDF_member= " << generated_PDF_member;
  int contains_PDF_reweight=tokenized[18].Atof();
  if(GetEntriesOrNchunck>-1) cout << " contains_PDF_reweight= " << contains_PDF_reweight;
  int usePhiMETCorr=tokenized[19].Atof();
  if(GetEntriesOrNchunck>-1) cout << " usePhiMETCorr= " << usePhiMETCorr;
  int useRecoilCorr=tokenized[20].Atof();
  if(GetEntriesOrNchunck>-1) cout << " useRecoilCorr= " << useRecoilCorr;
  int RecoilCorrVarDiagoParN=tokenized[21].Atof();
  if(GetEntriesOrNchunck>-1) cout << " RecoilCorrVarDiagoParN= " << RecoilCorrVarDiagoParN;
  int RecoilCorrVarDiagoParSigmas=tokenized[22].Atof();
  if(GetEntriesOrNchunck>-1) cout << " RecoilCorrVarDiagoParSigmas= " << RecoilCorrVarDiagoParSigmas;
  int RecoilCorrVarDiagoParU1orU2=tokenized[23].Atof();
  if(GetEntriesOrNchunck>-1) cout << " RecoilCorrVarDiagoParU1orU2= " << RecoilCorrVarDiagoParU1orU2;
  int use_PForNoPUorTKmet=tokenized[24].Atof();
  if(GetEntriesOrNchunck>-1) cout << " use_PForNoPUorTKmet= " << use_PForNoPUorTKmet;
  int use_syst_ewk_Alcaraz=tokenized[25].Atof();
  if(GetEntriesOrNchunck>-1) cout << " use_syst_ewk_Alcaraz= " << use_syst_ewk_Alcaraz;
  int gen_mass_value_MeV=tokenized[26].Atof();
  if(GetEntriesOrNchunck>-1) cout << " gen_mass_value_MeV= " << gen_mass_value_MeV;
  int contains_LHE_weights=tokenized[27].Atof();
  if(GetEntriesOrNchunck>-1) cout << " contains_LHE_weights= " << contains_LHE_weights;
  if(GetEntriesOrNchunck>-1) cout << endl;
  
  if(GetEntriesOrNchunck>-1) cout << "processing line "<< Form("Wanalysis wDATA(\"%s\",%f,%d)",WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJets") && !sample.Contains("Fake")?useAlsoGenPforSig:0) << endl;
  Wanalysis wDATA(WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJets") && !sample.Contains("Fake")?useAlsoGenPforSig:0);
  if(GetEntriesOrNchunck>-1) cout << "processing line "<< Form("wDATA.Loop(%d,%d,\"%s\",%d,%d,%d,%d,%d,%d,\"%s\",%d,%d,%d,%d,%d)",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight,usePhiMETCorr,useRecoilCorr,RecoilCorrVarDiagoParN,RecoilCorrVarDiagoParSigmas,RecoilCorrVarDiagoParU1orU2,use_PForNoPUorTKmet,use_syst_ewk_Alcaraz,gen_mass_value_MeV,contains_LHE_weights) << endl;
  int nEntries;
  if(GetEntriesOrNchunck==-1){
    nEntries = wDATA.NumEntries();
    cout << "nEntries= " <<  nEntries << endl;
    return 0;
  }
  
  cout<< "chunk " << GetEntriesOrNchunck << " Entry_ini= " << Entry_ini << " Entry_fin= " << Entry_fin << endl;
  wDATA.Loop(GetEntriesOrNchunck,Entry_ini,Entry_fin,IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePtSF,usePileupSF,controlplots,sample.Data(),generated_PDF_set,generated_PDF_member,contains_PDF_reweight,usePhiMETCorr,useRecoilCorr,RecoilCorrVarDiagoParN,RecoilCorrVarDiagoParSigmas,RecoilCorrVarDiagoParU1orU2,use_PForNoPUorTKmet,use_syst_ewk_Alcaraz,gen_mass_value_MeV,contains_LHE_weights);
  TString chunk_str = GetEntriesOrNchunck>0? Form("_chunk%d",GetEntriesOrNchunck) : "";
  if(GetEntriesOrNchunck==0)
    gROOT->ProcessLine(Form(".! mv %s/Wanalysis%s.root %s/WanalysisOnDATA.root",filename_outputdir.Data(),chunk_str.Data(),filename_outputdir.Data()));
  gROOT->ProcessLine(Form(".! cp Wanalysis.* %s/",filename_outputdir.Data()));

}
