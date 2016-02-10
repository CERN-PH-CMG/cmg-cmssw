// UNCOMMENT TO USE PDF REWEIGHTING
//#define LHAPDF_ON

#ifdef LHAPDF_ON
  #include "LHAPDF/LHAPDF.h"
#endif 

#define Zanalysis_cxx
#include "Zanalysis.h"
#include "common.hpp"
#include "common_stuff.h"
// #include "rochcor_42X.h"
// #include "rochcor_44X_v3.h"
// #include "MuScleFitCorrector.h"
// #include "KalmanCalibrator.h"
#include "KalmanCalibratorParam.h"
#include "RecoilCorrector.h"
#include "HTransformToHelicityFrame.c"
#include "HTransformToCS.c"
#include <TH3.h>
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <ctime>
#include <string>

TLorentzVector muPosNoCorr,muNegNoCorr,ZNocorr;
TLorentzVector muPosGen_status3,muNegGen_status3,ZGen_status3;
TLorentzVector muPosCorr,muNegCorr,muPosCorrCentral,muNegCorrCentral;
TLorentzVector Zcorr, ZcorrCentral; //TLorentzVector of the reconstructed muon
TLorentzVector Z_met,Z_metCentral,Wlike_met,Wlike_metCentral;
TLorentzVector Wlike,WlikeCentral;

TRandom3 *random_ = new TRandom3(10101982);

HTransformToHelicityFrame *GoToHXframe;
double costh_HX = -1e10, phi_HX = -1e10;
double costh_HX_gen = -1e10, phi_HX_gen = -1e10;
double costh_HX_gen_pietro = -1e10, phi_HX_gen_pietro = -1e10;
HTransformToCS *GoToCSframe;
double costh_CS = -1e10, phi_CS = -1e10;
double costh_CS_gen_pietro = -1e10, phi_CS_gen_pietro = -1e10;
double costh_CS_gen = -1e10, phi_CS_gen = -1e10;

const double ZPt_cut = 30; // ADDED DURING PLOTS PRE-UNBLINDING
const bool preUnblinding = false; // this is meant to save only the inclusive plots the controlplots variable is used for the various binned plots


void Zanalysis::Loop(int chunk, int Entry_ini, int Entry_fin, int IS_MC_CLOSURE_TEST, int isMCorDATA, TString outputdir, int useMomentumCorr, int useEffSF, int usePtSF, int useVtxSF, int controlplots, TString sampleName, int generated_PDF_set, int generated_PDF_member, int contains_PDF_reweight, int usePhiMETCorr, int useRecoilCorr, int correctToMadgraph, int use_PForNoPUorTKmet, int use_syst_ewk_Alcaraz, int gen_mass_value_MeV, int contains_LHE_weights, int reweight_polarization)
{

  if (fChain == 0) return;

  using namespace std;

  // maps needed for common_stuff histos
  std::map<string, TH1D*> h_1d;
  std::map<string, TH2D*> h_2d;
  std::map<string, TH3D*> h_3d;

  cout << "generated_PDF_set= "<<generated_PDF_set
       << " generated_PDF_member= " << generated_PDF_member
       << " contains_PDF_reweight= " << contains_PDF_reweight
       << " WMass::WlikeCharge= " << WMass::WlikeCharge
       << endl;

  TString chunk_str = chunk>0? Form("_chunk%d",chunk) : "";
  cout << "output filename= " << Form("%s/Zanalysis%s.root",outputdir.Data(),chunk_str.Data()) << endl;

  //------------------------------------------------------
  // Set variables for charge
  //------------------------------------------------------
  
  bool isChargePos = WMass::WlikeCharge==1;
  TString WCharge_str = isChargePos?"Pos":"Neg";
  TLorentzVector& muNoCorr            = isChargePos?muPosNoCorr     :muNegNoCorr;
  TLorentzVector& neutrinoNoCorr      = isChargePos?muNegNoCorr     :muPosNoCorr;
  TLorentzVector& muCorr              = isChargePos?muPosCorr       :muNegCorr;
  //TLorentzVector& neutrinoCorr        = isChargePos?muNegCorr       :muPosCorr;
  TLorentzVector& muCorrCentral       = isChargePos?muPosCorrCentral:muNegCorrCentral;
  TLorentzVector& neutrinoCorrCentral = isChargePos?muNegCorrCentral:muPosCorrCentral;
  TLorentzVector& muGen_status3       = isChargePos?muPosGen_status3:muNegGen_status3;
  TLorentzVector& neutrinoGen_status3 = isChargePos?muNegGen_status3:muPosGen_status3;
  Double_t&       muRelIso            = isChargePos?MuPosRelIso     :MuNegRelIso;
  Double_t&       neutrinoRelIso      = isChargePos?MuNegRelIso     :MuPosRelIso;
  Int_t&          muTrg               = isChargePos?MuPosTrg        :MuNegTrg;


  //------------------------------------------------------
  // Initialize PDF
  //------------------------------------------------------

  int PDF_reweighting_central_Index = -1;
  if(contains_LHE_weights && WMass::PDF_sets==229800) PDF_reweighting_central_Index = WMass::LHE_NNPDF2p3_NLO_central_index;
  if(contains_LHE_weights && WMass::PDF_sets==11000)  PDF_reweighting_central_Index = WMass::LHE_CT10_NLO_central_index;
  if(contains_LHE_weights && WMass::PDF_sets==232000) PDF_reweighting_central_Index = WMass::LHE_NNPDF2p3_NNLO_central_index;
  
  #ifdef LHAPDF_ON
    // LHAPDF::initPDFSet(1,"CT10nnlo.LHgrid");
    if(!sampleName.Contains("DATA")){
      cout << "inizializing LHAPDF::initPDFSet(1)" << endl;
      LHAPDF::initPDFSet(1,generated_PDF_set,generated_PDF_member); // CMSSW DEFAULT
      cout << "finished inizializing LHAPDF" << endl;

      cout << "inizializing LHAPDF::initPDFSet(0)" << endl;
      // LHAPDF::initPDFSet();
      if(WMass::PDF_sets==11200)
        // LHAPDF::initPDFSet(0,"CT10nnlo.LHgrid");
        LHAPDF::initPDFSet(0,11200,0);
      else if(WMass::PDF_sets==232000)
        // LHAPDF::initPDFSet(0,"NNPDF23_nnlo_as_0118.LHgrid");
        LHAPDF::initPDFSet(0,232000,0);
      else if(WMass::PDF_sets==229800)
        // LHAPDF::initPDFSet(0,"NNPDF23_nnlo_as_0118.LHgrid");
        LHAPDF::initPDFSet(0,229800,0);
      else if(WMass::PDF_sets==21200)
        // LHAPDF::initPDFSet(0,"MSTW2008nnlo68cl.LHgrid");
        LHAPDF::initPDFSet(0,21200,0);
      else if(WMass::PDF_sets==21241)
        LHAPDF::initPDFSet(0,21241,0);  // else if(WMass::PDF_sets<0)
        // LHAPDF::initPDFSet(0,generated_PDF_set,generated_PDF_member);
    
    }
  #endif
  // TFile*feffSF = new TFile(Form("../Zanalysis.root",outputdir.Data()),"RECREATE");
  
  //------------------------------------------------------
  // retrieve efficiencies SF
  //------------------------------------------------------
  TH3F *SF_HLT;
  TH2F *SF_TIGHT_ISO;
  TH2F *SF_TIGHT_PT10;
  TH3F *SF_ISO05_PT10;
  
  cout << "checKING EFFICIENCIES, useEffSF= " << useEffSF << endl;
  if(((useEffSF>=2 && useEffSF<=6) || (useEffSF>=13 && useEffSF<=16)) && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    
    TFile* finEffSF = new TFile("../utils/MuonEfficiencies_SF_2011_53X_DataMC_Heiner.root"); // used only to build templates
    if(!finEffSF){
      cout << "file MuonEfficiencies_SF_2011_53X_DataMC_Heiner.root is missing, impossible to retrieve efficiency scale factors" << endl;
      return;
    }
    SF_HLT=(TH3F*)finEffSF->Get("SF_2011_HLT_TisoMu24eta2p1_IsoMu24_eta2p1_charge_eta_pt_PLOT"); // x=charge, y=eta, z=pt
    SF_TIGHT_ISO=(TH2F*)finEffSF->Get("SF_2011_TIGHT_ISO_PT25_PtrgL_eta_pt_PLOT");
    SF_TIGHT_PT10=(TH2F*)finEffSF->Get("SF_2011_TIGHT_PT10_ETA2P4_Tid_iso_trg_eta_pt_PLOT");
    SF_ISO05_PT10=(TH3F*)finEffSF->Get("SF_2011_ISO05_PT10_ETA2P4_Tid_iso_trg_CosThetaStar_PhiStarAbs_pair_pt_PLOT");
    
  }else{
    cout << "NOT APPLYING EFFICIENCIES SF (not requested or analyzing data)"<< endl;
  }
  //------------------------------------------------------
  // retrieve pileup SF
  //------------------------------------------------------
  TH1D*hPileupSF;

  if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    TString vtx_str = sampleName; vtx_str.ReplaceAll("Sig",""); vtx_str.ReplaceAll("Fake","");
    TFile* finPileupSF = new TFile(Form("../utils/pileup/pileup_reweighting_Fall11.root")); // used only to build templates
    if(!finPileupSF){
      cout << "file " << Form("../utils/pileup/pileup_reweighting_Fall11.root") << " is missing, impossible to retrieve pileup reweighting factors" << endl;
      return;
    }
    hPileupSF=(TH1D*)finPileupSF->Get("hpileup_reweighting_Fall11");
  }

  //------------------------------------------------------
  // retrieve invariant mass SF (for EWK)
  //------------------------------------------------------
  TH1D*hZmassSF_central,*hZmassSF_syst;

  if(use_syst_ewk_Alcaraz>-1 && use_syst_ewk_Alcaraz!=100){
    TFile* finZmassSF = new TFile("../utils/fout_invariant_mass_qed_higstat.root"); // used only to build templates
    if(!finZmassSF){
      cout << "file " << Form("../utils/fout_invariant_mass_qed_higstat.root") << " is missing, impossible to retrieve invariant mass SF (for EWK)" << endl;
      return;
    }
    cout << "hZmassSF_central = ewk_p8std_div_ewk_p8bad" << endl;
    hZmassSF_central=(TH1D*)finZmassSF->Get("ewk_p8std_div_ewk_p8bad"); hZmassSF_central->Sumw2();
    if(use_syst_ewk_Alcaraz==1 || use_syst_ewk_Alcaraz==31){
      cout << "hZmassSF_syst = ewk_p8photos_div_ewk_p8std" << endl;
      hZmassSF_syst=(TH1D*)finZmassSF->Get("ewk_p8photos_div_ewk_p8std"); hZmassSF_syst->Sumw2();
    }else if(use_syst_ewk_Alcaraz==2 || use_syst_ewk_Alcaraz==32){
      cout << "hZmassSF_syst = noewk_p8std_div_ewk_p8std" << endl;
      hZmassSF_syst=(TH1D*)finZmassSF->Get("noewk_p8std_div_ewk_p8std"); hZmassSF_syst->Sumw2();
    }
  }
  //------------------------------------------------------
  // retrieve angular coefficients SF (for POWHEG vs DATA)
  //------------------------------------------------------
  const int fitcoeff = 6;  
  const int nrapbins = 2;
  const int nptbins = 8;
  double rapbins[nrapbins+1] = {0,1,2.1};
  TH1D *hrapbins = new TH1D("hrapbins","hrapbins",nrapbins,rapbins);
  double ptbins[nptbins+1] = {0.1,10,20,35,55,80,120,200,1000};
  TH1D *hptbins = new TH1D("hptbins","hptbins",nptbins,ptbins);
  TH1D *hratioAngCoefSF[nrapbins][fitcoeff];
  if(true){
    TFile* finAngCoefSF = new TFile("../utils/polarization_data_powheg.root"); // used only to build templates
    if(!finAngCoefSF){
      cout << "file " << Form("../utils/polarization_data_powheg.root") << " is missing, impossible to retrieve Angular coefficients reweighting factors" << endl;
      return;
    }
    for(int irap=0; irap<nrapbins; irap++){
      for(int ipt=0; ipt<nptbins; ipt++){
        hratioAngCoefSF[irap][ipt] = (TH1D*) finAngCoefSF->Get(Form("ratio_phi_vs_costh_CS_pietro_ptbin%d_rapbin%d_POWHEG_over_data",ipt+1,irap+1));
        // cout << Form("ratio_phi_vs_costh_CS_pietro_ptbin%d_rapbin%d_POWHEG_over_data",ipt,irap) << endl;
        // hratioAngCoefSF[irap][ipt]->Print();
      }
    }    
  }

  //------------------------------------------------------
  // retrieve boson pT SF
  //------------------------------------------------------
  TH1D* hZPtSF;
  if(usePtSF==0 && (sampleName.Contains("DYJetsMadSig") || sampleName.Contains("DYJetsPow"))) {
    TString filename=Form("../utils/Zpt_output_%s_Pos.root",sampleName.Data());
    cout << "hZPtSF_central = " << filename.Data() << endl;

    TFile* finZPtSF = new TFile(filename.Data());
    hZPtSF=(TH1D*) finZPtSF->Get("hWlikePos_ZpT_8_JetCut_pdf229800-0_eta0p9_91188"); hZPtSF->Sumw2();
    
  } else hZPtSF = new TH1D("hZPtSF","hZPtSF",10,0,1);

  //------------------------------------------------------
  // Polarization
  //------------------------------------------------------

  TH2D* hZPolSF;
  if(reweight_polarization==1 && (sampleName.Contains("DYJetsMadSig") || sampleName.Contains("DYJetsPow"))) {

    TString filename=Form("../utils/Zpol_output_%s_Pos.root",sampleName.Data());
    cout << "hZpolSF_central = " << filename.Data() << endl;

    TFile* finZPolSF = new TFile(filename.Data());
    hZPolSF=(TH2D*) finZPolSF->Get("hWlikePos_phi_vs_costh_CS_8_JetCut_pdf229800-0_eta0p9_91188"); hZPolSF->Sumw2();

  } else hZPolSF = new TH2D("hZPolSF","hZPolSF",10,0,1,10,0,1);

  // the following variables will get the recoil components from the corrector
  double u1_recoil = 0;
  double u2_recoil = 0;
  
  //------------------------------------------------------
  // Set initial and final number of entries
  //------------------------------------------------------
  // Long64_t first_entry = 0;
  // Long64_t nentries = fChain->GetEntriesFast();
  Long64_t first_entry = Entry_ini;
  Long64_t nentries = Entry_fin;
  // OBSOLETE setting
  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==1) first_entry=nentries/2; // in case of closure test, DATA runs from N/2 to N
  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==0) nentries=nentries/2; // in case of closure test, MC runs from 0 to N/2
  if(IS_MC_CLOSURE_TEST==1) lumi_scaling=lumi_scaling*2; // in case of closure test, scaling must be multiplied by 2
  
  //------------------------------------------------------
  // Initilize momentum scale corrections
  //------------------------------------------------------
  //To get the central value of the momentum correction
  // rochcor42X *rmcor42X = new rochcor42X();  // make the pointer of rochcor class
  //int random_seed_start=67525;
  //rochcor_44X_v3 *rmcor44X = new rochcor_44X_v3(random_seed_start);  // make the pointer of rochcor class
  //TString MuscleCard = (IS_MC_CLOSURE_TEST || isMCorDATA==0) ? "MuScleFit_2011_MC_44X" : "MuScleFit_2011_DATA_44X";
  //TString fitParametersFile = MuscleCard+".txt";
  //MuScleFitCorrector *corrector;
  //if(useMomentumCorr==2){
  //  cout << "using MuscleFit card " << fitParametersFile << endl;
  //  corrector = new MuScleFitCorrector(fitParametersFile);
  //}
  //KalmanCalibrator *corrector_Kalman;
  //if(useMomentumCorr==3){
  //  cout << "using Kalman Calibrator" << endl;
  //  corrector_Kalman = new KalmanCalibrator(isMCorDATA==0?false:true); // True for data , //False for MC
  //}
  KalmanCalibratorParam *corrector_KalmanParam;
  if(useMomentumCorr==4){
    cout << "using Kalman Calibrator Param" << endl;
    corrector_KalmanParam = new KalmanCalibratorParam(isMCorDATA==0?false:true); // True for data , //False for MC
    // corrector_KalmanParam = new KalmanCalibratorParam(true); // TEST, THIS IS A TEST!!!!!
  }

  //------------------------------------------------------
  // Closure test reweighting for recoil corrections
  //------------------------------------------------------
  TFile *f_u_reweighting = new TFile("/afs/cern.ch/work/p/perrozzi/private/git/v5_18_0/common_maria/CMSSW_5_3_22_patch1/src/CMGTools/WMass/analysis/utils/pre_unblinding5_recoil_ratiomc_div_data.root");
  TH1D* pre_unblinding5_recoil_ratiomc_div_data = (TH1D*)f_u_reweighting->Get("pre_unblinding5_recoil_ratiomc_div_data");
  
  //------------------------------------------------------
  // Initialize recoil corrections
  //------------------------------------------------------

  TString metSuffix;
  if(use_PForNoPUorTKmet==0) metSuffix="_pfmet"; // this doesn't work since we do no have the pfmet as of JAN25
  if(use_PForNoPUorTKmet==1) metSuffix="_pfnoPU";
  if(use_PForNoPUorTKmet==2) metSuffix="_tkmet";
  
  TString generatorSuffix="_powheg";
  if (sampleName.Contains("DYJetsMadSig"))
    generatorSuffix="_madgraph";

  RecoilCorrector* correctorRecoil_Z; // TYPE2
  bool doKeys= useRecoilCorr==3 ? true : false;
  int RapBinCorr;
  int DATAorMC;
  int U1orU2;
  int RecoilEigenNum;
  WMass::RecoilStatVariationToInternalVars(WMass::RecoilStatVariation, RapBinCorr, DATAorMC, U1orU2, RecoilEigenNum);
  if(useRecoilCorr>0){
    TString model_name[2]={"fitresult_Add","fitresult_model2D"};
    
    // TKMET type2
    int model_name_idx=0;
    string fileCorrectTo = /*POW */ Form("../RecoilCode/JAN31/recoilfit_JAN31_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X%s.root",generatorSuffix.Data());
    string fileZmmMC =     /*POW */ Form("../RecoilCode/JAN31/recoilfit_JAN31_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X%s.root",generatorSuffix.Data());
    // need to add the half stat
    string fileZmmData =   /*DATA*/ "../RecoilCode/JAN31/recoilfit_JAN31_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_triGauss_halfStat_UNBINNED_3G_53X.root";
    if(correctToMadgraph) fileZmmData = /*MAD */ "../RecoilCode/JAN31/recoilfit_JAN31_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root";

    // the sample are in the root://eoscms//eos/cms//store/group/phys_smp/Wmass/dalfonso/RecoilFiles/DEC6/; download from there
    string fileZmmKeysCorrectTo = Form("../RecoilCode/DEC6/keysrecoilfit_DEC6_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_absolute_UNBINNED_3G_53X%s.root",generatorSuffix.Data());
    string fileZmmKeysMC = Form("../RecoilCode/DEC6/keysrecoilfit_DEC6_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_absolute_UNBINNED_3G_53X%s.root",generatorSuffix.Data());
    // need to add the half stat
    string fileZmmKeysData = "../RecoilCode/DEC6/keysrecoilfit_DEC6_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_triGauss_halfStat_UNBINNED_3G_53X.root";
    if(correctToMadgraph) fileZmmKeysData = "../RecoilCode/DEC6/keysrecoilfit_DEC6_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_absolute_UNBINNED_3G_53X_madgraph.root";

    cout << "INITIALIZING RECOIL MC TARGET FILE" << endl;
    correctorRecoil_Z = new RecoilCorrector(doKeys, fileCorrectTo.c_str(),fileZmmKeysCorrectTo.c_str(),123456,model_name[0],"../RecoilCode/MAY25/nonClosureMAY25.root");
    cout << "INITIALIZING RECOIL Z DATA FILE" << endl;
    correctorRecoil_Z->addDataFile(fileZmmData.c_str(), fileZmmKeysData.c_str(), model_name[model_name_idx] );
    cout << "INITIALIZING RECOIL Z MC FILE" << endl;
    correctorRecoil_Z->addMCFile(fileZmmMC.c_str(), fileZmmKeysMC.c_str(), model_name[0]);

  }

  double ZWmassRatio = WMass::ZWmassRatio; // PDG
  
  GoToHXframe = new HTransformToHelicityFrame();
  GoToCSframe = new HTransformToCS();
  
  //------------------------------------------------------
  // start the event loop
  //------------------------------------------------------
  int cout_freq=TMath::Min(1+(nentries-first_entry)/10,(Long64_t) 25000);
  // cout_freq=1; // FOR DEBUGGING PURPOSE
  // nentries=100; // FOR DEBUGGING PURPOSE
  cout << "couts every " << cout_freq << " events" << endl;
  Long64_t nbytes = 0, nb = 0;
  for(Long64_t jentry=first_entry; jentry<=nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%cout_freq==0){
      time_t now = time(0);
      TString dt = ctime(&now); dt.ReplaceAll("\n"," ");
      cout << dt << "\t - \t Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    }
    
  
    //------------------------------------------------------------
    // Reject even events in DATA, ALSO in case of controlplots, since they were used for Recoil Corrections
    //------------------------------------------------------------
    // if(!controlplots && !(jentry%2) && sampleName.Contains("DATA")) continue;
    if(!(jentry%2) && sampleName.Contains("DATA")) continue;

    //------------------------------------------------------
    // initilize the mu+, mu-, met event variables
    //------------------------------------------------------

    double met_transv,metphi_transv;
    double met_transvCentral,metphi_transvCentral;
    
    if(use_PForNoPUorTKmet==0){
      met_transv = pfmet;
      metphi_transv = pfmet_phi;
    }else if(use_PForNoPUorTKmet==1){
      met_transv = nopumet;
      metphi_transv = nopumet_phi;
    }else if(use_PForNoPUorTKmet==2){
      met_transv = tkmet;
      metphi_transv = tkmet_phi;
    }else{
      cout << "MET TYPE OPTION use_PForNoPUorTKmet: " << use_PForNoPUorTKmet << " NOT DEFINED, USE 0:PF, 1:NOPU, 2:TK" << endl;
      return;
    }
    
    met_transvCentral = met_transv;
    metphi_transvCentral = metphi_transv;

    muPosNoCorr.SetPtEtaPhiM(MuPos_pt,MuPos_eta,MuPos_phi,MuPos_mass);
    muNegNoCorr.SetPtEtaPhiM(MuNeg_pt,MuNeg_eta,MuNeg_phi,MuNeg_mass);
    ZNocorr = muPosNoCorr + muNegNoCorr;

    muPosGen_status3.SetPtEtaPhiM(MuPosGen_pt,MuPosGen_eta,MuPosGen_phi,MuPosGen_mass);
    muNegGen_status3.SetPtEtaPhiM(MuNegGen_pt,MuNegGen_eta,MuNegGen_phi,MuNegGen_mass);
    ZGen_status3 = muPosGen_status3 + muNegGen_status3;

    muPosCorr = muPosNoCorr;
    muPosCorrCentral = muPosNoCorr;
    muNegCorr = muNegNoCorr;
    muNegCorrCentral = muNegNoCorr;

    //------------------------------------------------------
    // compute event weight
    //------------------------------------------------------

    //---------------- MUON weight
    double TRG_TIGHT_ISO_muons_SF = 1;
    double eff_TIGHT_SF = 1;
    double eff_TIGHT_subleading_SF = 1;
    double eff_ISO_SF = 1;
    double eff_TRG_SF = 1;

    costh_HX = -1e10;     phi_HX = -1e10;
    costh_HX_gen = -1e10; phi_HX_gen = -1e10;
    costh_CS = -1e10;     phi_CS = -1e10;
    costh_CS_gen = -1e10; phi_CS_gen = -1e10;

    if(MuPosGen_pt>0 && MuNegGen_pt>0) ComputeHXVarAndPhiStarEta(muPosGen_status3,muNegGen_status3,true);
    if(MuPos_pt>0 && MuNeg_pt>0) ComputeHXVarAndPhiStarEta(muPosNoCorr,muNegNoCorr,false);

    // cout << "CHECKING EFFICIENCIES" << endl;
    if(((useEffSF>=2 && useEffSF<=6) || (useEffSF>=13 && useEffSF<=16)) && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
      if(useEffSF==2 || useEffSF==13 || useEffSF!=3){
        // === leading
        eff_TIGHT_SF            = SF_TIGHT_ISO->GetBinContent(SF_TIGHT_ISO->FindBin(isChargePos?MuPos_eta:MuNeg_eta,isChargePos?MuPos_pt:MuNeg_pt));
        // cout << "eff_TIGHT_SF no smear= "<< eff_TIGHT_SF << endl;
        if(useEffSF==13){
          random_->SetSeed(UInt_t(TMath::Abs(isChargePos?MuPos_phi:MuNeg_phi)*1e9 + TMath::Abs(isChargePos?MuPos_eta:MuNeg_eta)*1e6 + TMath::Abs(isChargePos?MuPos_pt:MuNeg_pt)*1e3));
          eff_TIGHT_SF += random_->Gaus(0,TMath::Hypot(0.02,SF_TIGHT_ISO->GetBinError(SF_TIGHT_ISO->FindBin(isChargePos?MuPos_eta:MuNeg_eta,isChargePos?MuPos_pt:MuNeg_pt))));
          // cout << "eff_TIGHT_SF smear= "<< eff_TIGHT_SF << endl;
        }
        TRG_TIGHT_ISO_muons_SF  *= eff_TIGHT_SF;
      }
      if(useEffSF==2 || useEffSF==14 || useEffSF!=4){
        // === subleading
        // cout << "eff_ISO_SF"<<endl;
        eff_ISO_SF              = SF_ISO05_PT10->GetBinContent(SF_ISO05_PT10->FindBin(costh_HX,TMath::Abs(phi_HX),ZNocorr.Pt()));
        // cout << "eff_ISO_SF no smear= "<< eff_ISO_SF << endl;
        if(useEffSF==14){
          random_->SetSeed(UInt_t(TMath::Abs(costh_HX)*1e9 + TMath::Abs(TMath::Abs(phi_HX))*1e6 + TMath::Abs(ZNocorr.Pt())*1e3));
          eff_ISO_SF += random_->Gaus(0,TMath::Hypot(0.02,SF_ISO05_PT10->GetBinError(SF_ISO05_PT10->FindBin(costh_HX,TMath::Abs(phi_HX),ZNocorr.Pt()))));
          // cout << "eff_ISO_SF smear= "<< eff_ISO_SF << endl;
        }
        TRG_TIGHT_ISO_muons_SF  *= eff_ISO_SF;
      }
      if(useEffSF==2 || useEffSF==15 || useEffSF!=5){
        // === subleading
        // cout << "eff_TIGHT_subleading_SF"<<endl;
        eff_TIGHT_subleading_SF = SF_TIGHT_PT10->GetBinContent(SF_TIGHT_PT10->FindBin(isChargePos?MuNeg_eta:MuPos_eta,isChargePos?MuNeg_pt:MuPos_pt));
        // cout << "eff_TIGHT_subleading_SF no smear= "<< eff_TIGHT_subleading_SF << endl;
        if(useEffSF==15){
          random_->SetSeed(UInt_t(TMath::Abs(costh_HX)*1e9 + TMath::Abs(TMath::Abs(phi_HX))*1e6 + TMath::Abs(ZNocorr.Pt())*1e3));
          eff_TIGHT_subleading_SF += random_->Gaus(0,TMath::Hypot(0.02,SF_TIGHT_PT10->GetBinContent(SF_TIGHT_PT10->FindBin(isChargePos?MuNeg_eta:MuPos_eta,isChargePos?MuNeg_pt:MuPos_pt))));
          // cout << "eff_TIGHT_subleading_SF smear= "<< eff_TIGHT_subleading_SF << endl;
        }
        TRG_TIGHT_ISO_muons_SF  *= eff_TIGHT_subleading_SF;
      }
      if(useEffSF==2 || useEffSF==16 || useEffSF!=6){
        // === leading
        // cout << "eff_TRG_SF"<<endl;
        eff_TRG_SF              = SF_HLT->GetBinContent(SF_HLT->FindBin(isChargePos?1:-1,isChargePos?MuPos_eta:MuNeg_eta,isChargePos?MuPos_pt:MuNeg_pt));
        // cout << "eff_TRG_SF no smear= "<< eff_TRG_SF << endl;
        if(useEffSF==16){
          random_->SetSeed(UInt_t(TMath::Abs(isChargePos?1:-1)*1e9 + TMath::Abs(TMath::Abs(isChargePos?MuPos_eta:MuNeg_eta))*1e6 + TMath::Abs(isChargePos?MuPos_pt:MuNeg_pt)*1e3));
          eff_TRG_SF += random_->Gaus(0,TMath::Hypot(0.02,SF_HLT->GetBinContent(SF_HLT->FindBin(isChargePos?1:-1,isChargePos?MuPos_eta:MuNeg_eta,isChargePos?MuPos_pt:MuNeg_pt))));
          // cout << "eff_TRG_SF smear= "<< eff_TRG_SF << endl;
        }
        TRG_TIGHT_ISO_muons_SF  *= eff_TRG_SF;
      }
    }else if(useEffSF==7){
      // cout << "flat SF at 0.98"<<endl;
      TRG_TIGHT_ISO_muons_SF=0.98;
    }
    // cout << "TRG_TIGHT_ISO_muons_SF= " << TRG_TIGHT_ISO_muons_SF << endl;

    //---------------- LUMI weight
    double evt_weight = lumi_scaling;
    // cout << "evt_weight_original " << evt_weight_original << endl;

    //---------------- VTX weight
    if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && npu>0)
      evt_weight *= hPileupSF->GetBinContent(hPileupSF->GetXaxis()->FindBin(npu));

    if((IS_MC_CLOSURE_TEST || isMCorDATA==0) && controlplots) 
      common_stuff::plot1D("hPileUp_Fall11",npu, 1, h_1d, 50,0,50);

    // THIS IS OBSOLETE
    //---------------- Angular coefficients weight
    // cout << "reweight_polarization= " << reweight_polarization << endl;
    if(reweight_polarization==2 && sampleName.Contains("DYJetsPow")){
      common_stuff::ComputeAllVarPietro(muPosGen_status3,muNegGen_status3, costh_CS_gen_pietro, phi_CS_gen_pietro, costh_HX_gen_pietro, phi_HX_gen_pietro);
      // cout
      // << " ZGen_status3.Rapidity()= " << ZGen_status3.Rapidity()
      // << " ZGen_status3.Pt()= " << ZGen_status3.Pt()
      // << " hrapbins->GetXaxis()->FindBin(ZGen_status3.Rapidity())= " << hrapbins->GetXaxis()->FindBin(ZGen_status3.Rapidity())
      // << " hptbins->GetXaxis()->FindBin(ZGen_status3.Pt())= " << hptbins->GetXaxis()->FindBin(ZGen_status3.Pt())
      // << " costh_CS_gen_pietro= " << costh_CS_gen_pietro
      // << " phi_CS_gen_pietro= " << phi_CS_gen_pietro
      // << endl;
      double AngCoef_sf = 1;
      if( TMath::Abs(ZGen_status3.Rapidity())<2.1 && ZGen_status3.Pt()<1000 ){
        int i_rapbin = hrapbins->GetXaxis()->FindBin(TMath::Abs(ZGen_status3.Rapidity()))-1 ;
        // cout << "i_rapbin= " << i_rapbin<< endl;;
        int i_ptbin = hptbins->GetXaxis()->FindBin(ZGen_status3.Pt())>0 ? hptbins->GetXaxis()->FindBin(ZGen_status3.Pt())-1 : 0 ;
        // cout << " i_ptbin= " << i_ptbin<< endl;;
        // hratioAngCoefSF[i_rapbin][i_ptbin]->Print();
        int costh_bin = hratioAngCoefSF[i_rapbin][i_ptbin]->GetXaxis()->FindBin(costh_CS_gen_pietro);
        // cout << " costh_bin= " << costh_bin<< endl;;
        int phi_bin = hratioAngCoefSF[i_rapbin][i_ptbin]->GetYaxis()->FindBin(TMath::Abs(phi_CS_gen_pietro));
        // cout << " phi_bin= " << phi_bin<< endl;;
        AngCoef_sf = hratioAngCoefSF[i_rapbin][i_ptbin]->GetBinContent(costh_bin,phi_bin);
      }
      // cout << "ang corr weight= " << AngCoef_sf 
           // << endl;
      evt_weight *= AngCoef_sf!=0 ? AngCoef_sf : 1;
    }

    //---------------- Invariant Mass weight
    // cout << "mass= " << ZGen_PostFSR_mass << " use_syst_ewk_Alcaraz = "<< use_syst_ewk_Alcaraz << endl;
    if(use_syst_ewk_Alcaraz>-1 && use_syst_ewk_Alcaraz!=100 && sampleName.Contains("DYJetsPow")){
      double ewk_weight_central=ZGen_PostFSR_mass>50&&ZGen_PostFSR_mass<200?hZmassSF_central->Interpolate(ZGen_PostFSR_mass):1;
      // cout << "ewk_weight_central = ewk_weight_central ("<<ewk_weight_central<<")" << endl;
      evt_weight *= ewk_weight_central;
      if(use_syst_ewk_Alcaraz>0){
        double ewk_weight_syst=ZGen_PostFSR_mass>50&&ZGen_PostFSR_mass<200?hZmassSF_syst->Interpolate(ZGen_PostFSR_mass):1;
        // cout << "evt_weight_original *= ewk_weight_syst ("<<ewk_weight_syst<<")" << endl;
          evt_weight *= ewk_weight_syst;
        if(use_syst_ewk_Alcaraz>30){ // 3 times the corrections (3 sigmas)
          // cout << "evt_weight_original *= (ewk_weight_syst*ewk_weight_syst)" << endl;
          evt_weight *= (ewk_weight_syst*ewk_weight_syst);
      
        // hZmassSF_central->Print();
        // cout << "mass= " << ZGen_PostFSR_mass << " ewk weight= " << (ZGen_PostFSR_mass>50&&ZGen_PostFSR_mass<200?hZmassSF_central->Interpolate(ZGen_PostFSR_mass):1) << endl;
        }
      }
    }
    //---------------- PT weight (usePtSF = -1; # Boson pT reweighting: -1=none, 0=data, 1...=other options)
    // OBSOLETE setting
    if(usePtSF!=-1 && usePtSF!=0 && usePtSF!=1 &&usePtSF!=2 /* && ZGen_pt<ZPt_cut */ && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && hZPtSF && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig")))
      // evt_weight_original*=hZPtSF->GetBinContent(hZPtSF->GetXaxis()->FindBin(/* Z_pt>0?Z_pt: */ZGen_pt));
      evt_weight *= hZPtSF->Interpolate(ZGen_pt)>0?hZPtSF->Interpolate(ZGen_pt):1;
    else if(usePtSF==1 && ZGen_pt<ZPt_cut && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && hZPtSF && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){
      evt_weight *= TMath::Min(1+0.1*(25-ZGen_pt)/25,0.5);
      common_stuff::plot2D(Form("weight1_vs_genpT"),
                                      ZGen_pt,(1+0.1*(25-ZGen_pt)/25), 1, 
                                          h_2d, 50,0,50,50,0,2 );
    }
    else if(usePtSF==2 && ZGen_pt<ZPt_cut && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && hZPtSF && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){
      evt_weight *= (1+0.1*(ZGen_pt-25)/25);
      common_stuff::plot2D(Form("weight2_vs_genpT"),
                                      ZGen_pt,(1+0.1*(25-ZGen_pt)/25), 1, 
                                          h_2d, 50,0,50,50,0,2 );
    }
    
    //---------------- EWK weight
    if(use_syst_ewk_Alcaraz && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){
      if(use_syst_ewk_Alcaraz==100 && FSRWeight>-99){ 
        evt_weight *= FSRWeight;
        // cout << "FSRWeight= " << FSRWeight << endl;
      }else if(use_syst_ewk_Alcaraz==0){
      }        
    }
    
    //------------------------------------------------------
    // define eta_max cut
    // WMass::etaMaxMuons = 0.9
    //------------------------------------------------------    
    TString eta_str = Form("%.1f",WMass::etaMaxMuons); eta_str.ReplaceAll(".","p");

    //------------------------------------------------------
    // retrieve and use gen-level info
    //------------------------------------------------------
    // SELECT EVENT WITH Z/W SCALED CUTS
    // gen stuff to be used only when required
    if(useGenVar){
      if(ZGen_mass>0){
        double WlikeGen_var_NotScaled[WMass::NFitVar] = {muGen_status3.Pt(), ZGen_mt, neutrinoGen_status3.Pt()};
        
        for(int k=0;k<WMass::NFitVar;k++)
          common_stuff::plot1D(Form("hWlike%s_%s_1_Gen_eta%s_%.0f",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                WlikeGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
          
          // common_stuff::plot1D(Form("hWlike%s_MGenNonScaled_1_GenBW_eta%s_%d",WCharge_str.Data(),eta_str.Data(),jZmass_MeV),
                                // ZGen_mass, bweight_i, h_1d, 50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio );
          // common_stuff::plot1D(Form("hWlike%s_MGenNonScaled_1_GenLHE_eta%s_%d",WCharge_str.Data(),eta_str.Data(),jZmass_MeV),
                                // ZGen_mass, lheweight_i, h_1d, 50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio );

        if(ZGen_mass>50){
          for(int k=0;k<WMass::NFitVar;k++)
            common_stuff::plot1D(Form("hWlike%s_%s_2_ZGenMassCut_eta%s_%.0f",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                  WlikeGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
          
          if(TMath::Abs(muGen_status3.Eta())<WMass::etaMaxMuons){
            for(int k=0;k<WMass::NFitVar;k++)
              common_stuff::plot1D(Form("hWlike%s_%s_3_Mu1GenCut_eta%s_%.0f",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                    WlikeGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
            
            if(TMath::Abs(neutrinoGen_status3.Eta())<WMass::etaMaxMuons2){
              for(int k=0;k<WMass::NFitVar;k++)
                common_stuff::plot1D(Form("hWlike%s_%s_4_Mu2GenCut_eta%s_%.0f",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                      WlikeGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
            }
          }
        }
      }
      if (controlplots) {
        common_stuff::plot1D(Form("z_mass_gen"), ZGen_status3.M(), 1, h_1d, 200, 0, 200 );
        common_stuff::plot1D(Form("z_rap_gen"), ZGen_status3.Rapidity(), 1, h_1d, 200, -5, 5 );
        
        common_stuff::plot1D(Form("mupos_pt_gen"), muPosGen_status3.Pt(), 1, h_1d, 100, 0, 100 );
        common_stuff::plot1D(Form("muneg_pt_gen"), muNegGen_status3.Pt(), 1, h_1d, 100, 0, 100 );
        common_stuff::plot1D(Form("mupos_eta_gen"), muPosGen_status3.Eta(), 1, h_1d, 50, -5, 5 );
        common_stuff::plot1D(Form("muneg_eta_gen"), muNegGen_status3.Eta(), 1, h_1d, 50, -5, 5 );
      }
      if(controlplots || preUnblinding) {
        // put polarization plots here:
        common_stuff::plot2D(Form("phi_vs_costh_CS"),
                                      costh_CS_gen,TMath::Abs(phi_CS_gen), 1, 
                                      h_2d, 12,-1,1,12,0,TMath::Pi() );
        common_stuff::plot2D(Form("phi_vs_costh_HX"),
                                      costh_HX_gen,TMath::Abs(phi_HX_gen), 1, 
                                      h_2d, 12,-1,1,12,0,TMath::Pi() );
      }
      // TO BE FITTED WITH
      // [1] The fit function for binning in cos_theta and phi:
      // [0]*0.5*(1-3*x*x)+[1]*sin(2*acos(x))*cos(y)+[2]*0.5*sin(acos(x))*sin(acos(x))*cos(2*y)+[3]*sin(acos(x))*cos(y)+[4]*x + [5]*(1+x*x)

    } // end gen stuff 

    if(!useGenVar || Z_mass>0){ // dummy thing to separate signal and background in DY+Jets (useless)
      // cout <<"WMass::RecoilCorrIniVarDiagoParU1orU2fromDATAorMC_["<<RecoilCorrVarDiagoParU1orU2fromDATAorMC<<"]= "
      // << WMass::RecoilCorrIniVarDiagoParU1orU2fromDATAorMC_[RecoilCorrVarDiagoParU1orU2fromDATAorMC]
      // << " WMass::RecoilCorrNVarDiagoParU1orU2fromDATAorMC_["<<RecoilCorrVarDiagoParU1orU2fromDATAorMC<<"]= " 
      // << WMass::RecoilCorrNVarDiagoParU1orU2fromDATAorMC_[RecoilCorrVarDiagoParU1orU2fromDATAorMC]
      // <<endl;

      //------------------------------------------------------
      // start reco event selection
      //------------------------------------------------------
      if( evtHasGoodVtx && evtHasTrg && muNoCorr.Pt()>0
          // CUTS ADDED TO SPEED UP THE CODE
          && TMath::Abs(muCorrCentral.Eta())<WMass::etaMaxMuons
          && TMath::Abs(neutrinoCorrCentral.Eta())<WMass::etaMaxMuons2 // CHANGED TO 2.1 DURING PLOTS PRE-UNBLINDING
          && MuPos_charge != MuNeg_charge
          && muTrg
          && MuPosIsTight && MuPos_dxy<0.02
          && MuNegIsTight && MuNeg_dxy<0.02
          && muRelIso<0.12 && neutrinoRelIso<0.5 ) { // good reco event
        
        //------------------------------------------------------------------------------------------------
        // Apply muon corrections
        //------------------------------------------------------------------------------------------------
        if (useMomentumCorr<4) {
          cout << "useMomentumCorr < 4 was deprecated many moons ago..." << endl;
          continue;
        }
        if (useMomentumCorr==4) { // use Momentum scale corrections from KalmanParam calibrator if required
          
          // Central correction
          corrector_KalmanParam->reset();
          corrector_KalmanParam->getCorrectedPt(muPosCorrCentral,MuPos_charge); //returns the corrected pt 
          corrector_KalmanParam->getCorrectedPt(muNegCorrCentral,MuNeg_charge); //returns the corrected pt 
          corrector_KalmanParam->smear(muPosCorrCentral);
          corrector_KalmanParam->smear(muNegCorrCentral);
          
          // Variation selection (if applicable)
          if (WMass::MuonScaleVariation){ // vary muon scale
            corrector_KalmanParam->varyClosure(WMass::MuonVariationSigmas);
          }
          if (WMass::MuonKalmanVariation != 0) { // vary n-th fit eigen
            corrector_KalmanParam->vary(WMass::MuonKalmanVariation, WMass::MuonVariationSigmas);
          }
          
          // Apply correction (shifted, if applicable)
          corrector_KalmanParam->getCorrectedPt(muPosCorr,MuPos_charge); //returns the corrected pt 
          corrector_KalmanParam->getCorrectedPt(muNegCorr,MuNeg_charge); //returns the corrected pt 
          corrector_KalmanParam->smear(muPosCorr);
          corrector_KalmanParam->smear(muNegCorr);
        }

        Zcorr = muPosCorr + muNegCorr;
        ZcorrCentral = muPosCorrCentral + muNegCorrCentral;

        //------------------------------------------------------------------------------------------------
        // Apply PT and Pol weight based on RECO
        //------------------------------------------------------------------------------------------------

        if (usePtSF!=-1 && usePtSF!=1 && usePtSF!=2
            /* && ZGen_pt<ZPt_cut */
            && (IS_MC_CLOSURE_TEST || isMCorDATA==0)
            && hZPtSF
            && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))
           )
          evt_weight*=hZPtSF->Interpolate(ZcorrCentral.Pt())>0?hZPtSF->Interpolate(ZcorrCentral.Pt()):1;

        // Boson Polarization
        common_stuff::ComputeAllVarPietro(muPosCorr,muNegCorr, costh_CS, phi_CS, costh_HX, phi_HX);

        if(reweight_polarization==1 && (sampleName.Contains("DYJetsMadSig") || sampleName.Contains("DYJetsPow"))) {
          // evt_weight*=hZPolSF->GetBinContent(hZPolSF->FindBin(costh_CS,TMath::Abs(phi_CS)))>0?hZPolSF->GetBinContent(hZPolSF->FindBin(costh_CS,TMath::Abs(phi_CS))):1;
          evt_weight *= hZPolSF->Interpolate(costh_CS,TMath::Abs(phi_CS))>0 ? hZPolSF->Interpolate(costh_CS,TMath::Abs(phi_CS)) : 1;
        }
        
        //------------------------------------------------------------------------------------------------
        // Apply recoil corrections
        //------------------------------------------------------------------------------------------------

        int rapBin=1;
        if(fabs(ZGen_rap) >= 1) rapBin=2;

        if(use_PForNoPUorTKmet<3 && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig")))
        { // use MET corrections if required
          
          if(use_PForNoPUorTKmet==0){
            met_transv = pfmet;
            metphi_transv = pfmet_phi;
          }
          else if(use_PForNoPUorTKmet==1){
            met_transv = nopumet;
            metphi_transv = nopumet_phi;
          }
          else if(use_PForNoPUorTKmet==2){
            met_transv = tkmet;
            metphi_transv = tkmet_phi;
          }

          if(useRecoilCorr==2 || useRecoilCorr==3) {
            // Central correction
            correctorRecoil_Z->Set_variation(RapBinCorr, DATAorMC, U1orU2, RecoilEigenNum, 0);
            correctorRecoil_Z->CorrectMET3gaus(
                                met_transvCentral, metphi_transvCentral,
                                ZGen_pt, ZGen_phi, ZNocorr.Pt(), ZNocorr.Phi(),
                                u1_recoil, u2_recoil, 1, rapBin);
                      
            if(WMass::RecoilVariationSigmas != 0 && rapBin==RapBinCorr){
              correctorRecoil_Z->Set_variation(RapBinCorr, DATAorMC, U1orU2, RecoilEigenNum, WMass::RecoilVariationSigmas);
              correctorRecoil_Z->CorrectMET3gaus(
                                  met_transv,metphi_transv,
                                  ZGen_pt, ZGen_phi, ZNocorr.Pt(), ZNocorr.Phi(),
                                  u1_recoil, u2_recoil, 1, rapBin);
            }
            else {
              met_transv    = met_transvCentral;
              metphi_transv = metphi_transvCentral;
            }
          }
          // cout
          // << "met_transv after=" << met_transv
          // << " metphi_transv after=" << metphi_transv
          // << " met_transvCentral after=" << met_transvCentral
          // << " metphi_transvCentral after=" << metphi_transvCentral
          // << endl;
        }


        //------------------------------------------------------
        // Apply met phi corrections
        //------------------------------------------------------
        if(usePhiMETCorr==1) { // use MET Phi correction if required
          pair<double, double> met_phicorr = common_stuff::getPhiCorrMET( met_transv, metphi_transv, nvtx, !sampleName.Contains("DATA"));
          met_transv = met_phicorr.first;
          metphi_transv = met_phicorr.second;
        }
        //------------------------------------------------------
        // Define mu+, mu-, Z
        //------------------------------------------------------
        TLorentzVector mu_transv,mu_transvCentral,mu_transvNoCorr,neutrino_transv,neutrino_transvCentral;
        
        Z_met.SetPtEtaPhiM(met_transv,0,metphi_transv,0);
        Z_metCentral.SetPtEtaPhiM(met_transvCentral,0,metphi_transvCentral,0);
        neutrino_transv.SetPtEtaPhiM(neutrinoNoCorr.Pt(),0,neutrinoNoCorr.Phi(),0); // correction only one one muon for Wlike
        neutrino_transvCentral.SetPtEtaPhiM(neutrinoNoCorr.Pt(),0,neutrinoNoCorr.Phi(),0); // correction only one one muon for Wlike
        mu_transv.SetPtEtaPhiM(muCorr.Pt(),0,muCorr.Phi(),0);
        mu_transvCentral.SetPtEtaPhiM(muCorrCentral.Pt(),0,muCorrCentral.Phi(),0);
        mu_transvNoCorr.SetPtEtaPhiM(muNoCorr.Pt(),0,muNoCorr.Phi(),0);

        Wlike_met = neutrino_transv + Z_met + mu_transvNoCorr - mu_transv; // taking into account muon correction into W_met
        Wlike_metCentral = neutrino_transvCentral + Z_metCentral + mu_transvNoCorr - mu_transvCentral;  // taking into account muon correction into W_met
        Wlike_met.SetPtEtaPhiM(Wlike_met.Pt(),0,Wlike_met.Phi(),0); // just to be sure
        Wlike_metCentral.SetPtEtaPhiM(Wlike_metCentral.Pt(),0,Wlike_metCentral.Phi(),0); // just to be sure
        Wlike = mu_transv + Wlike_met;
        WlikeCentral = mu_transvCentral + Wlike_metCentral;
      
        // MtLin definition
        // double coeff=2;
        // double MTFirstOrder_central = common_stuff::getMTFirstOrder(muCorrCentral.Pt(), muCorrCentral.Phi(), Wlike_metCentral.Pt() ,Wlike_metCentral.Phi(), coeff);
        // double MTFirstOrder = common_stuff::getMTFirstOrder(muCorr.Pt(), muCorr.Phi(), Wlike_met.Pt(),Wlike_met.Phi(), coeff);
        
        //------------------------------------------------------
        // Variables to fill the histos (pT, mT, MET)
        //------------------------------------------------------
        // double Wlike_var_jacobian[WMass::NFitVar] = {2*muCorr.Pt()/WMass::ZMassCentral_MeV*1e3,Wlike.Mt()/WMass::ZMassCentral_MeV*1e3,2*Wlike_met.Pt()/WMass::ZMassCentral_MeV*1e3,MTFirstOrder/WMass::ZMassCentral_MeV*1e3};
        double Wlike_vars[WMass::NFitVar] = {muCorr.Pt(),Wlike.Mt(),Wlike_met.Pt()};

        //------------------------------------------------------
        // Variables to define the box cut (pT, mT, MET)
        //------------------------------------------------------
        double Wlike_varsCentral[WMass::NFitVar] = {muCorrCentral.Pt(),WlikeCentral.Mt(),Wlike_metCentral.Pt()}; // Zcorr would be TEMP !!!!
        
        //------------------------------------------------------
        // good pair within acceptance cuts for both muons
        //------------------------------------------------------
        if( ZcorrCentral.M()>50
            && TMath::Abs(muCorrCentral.Eta())<WMass::etaMaxMuons
            // && TMath::Abs(neutrinoCorrCentral.Eta())<2.4 
            && TMath::Abs(neutrinoCorrCentral.Eta())<WMass::etaMaxMuons2 // CHANGED TO 2.1 DURING PLOTS PRE-UNBLINDING 
            && MuPos_charge != MuNeg_charge
            && muTrg
            && muCorrCentral.Pt()>WMass::sel_xmin[0]*ZWmassRatio 
            && neutrinoCorrCentral.Pt()>10 
            // && noTrgExtraMuonsLeadingPt<10 // REMOVED BECAUSE OF MARKUS
          ){
          //------------------------------------------------------
          // full ID and tight requirements on the muons as defined by Heiner for the efficiencies
          //------------------------------------------------------
          if(
              MuPosIsTight && MuPos_dxy<0.02
              && MuNegIsTight && MuNeg_dxy<0.02
              && muRelIso<0.12 && neutrinoRelIso<0.5
            ){
            for(int k=0;k<WMass::NFitVar;k++)
              common_stuff::plot1D(Form("hWlike%s_%s_5_RecoCut_eta%s_%.0f",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                Wlike_vars[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
  
            //------------------------------------------------------
            // cut on MET
            //------------------------------------------------------
            if(Wlike_metCentral.Pt()>WMass::sel_xmin[2]*ZWmassRatio){
              for(int k=0;k<WMass::NFitVar;k++)
                common_stuff::plot1D(Form("hWlike%s_%s_6_METCut_eta%s_%.0f",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                  Wlike_vars[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
              
              if(controlplots)
                common_stuff::plot1D(Form("hZ_pt_%s_eta%s_%.0f",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                  Zcorr.Pt(),evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 1000,0,250 );

              //------------------------------------------------------
              // cut on W recoil (BY DEFAULT IS 15)
              //------------------------------------------------------
              if(WlikeCentral.Pt()<WMass::WpTcut*ZWmassRatio
                  && ZcorrCentral.Pt() < ZPt_cut // ADDED DURING PLOTS PRE-UNBLINDING
                 ){
                for(int k=0;k<WMass::NFitVar;k++)
                  common_stuff::plot1D(Form("hWlike%s_%s_7_RecoilCut_eta%s_%.0f",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                              Wlike_vars[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                
                //------------------------------------------------------
                // Apply the "box" cuts on pT, mT, MET -- based on central correction
                //------------------------------------------------------
                if(    Wlike_varsCentral[0] > WMass::sel_xmin[0]*ZWmassRatio
                    && Wlike_varsCentral[0] < WMass::sel_xmax[0]*ZWmassRatio
                    && Wlike_varsCentral[1] > WMass::sel_xmin[1]*ZWmassRatio
                    && Wlike_varsCentral[1] < WMass::sel_xmax[1]*ZWmassRatio // Mt
                    && Wlike_varsCentral[2] > WMass::sel_xmin[2]*ZWmassRatio
                    && Wlike_varsCentral[2] < WMass::sel_xmax[2]*ZWmassRatio // MET
                  ){
                
                  //------------------------------------------------------
                  // Compute/retrieve PDF weights with LHAPDF or LHE
                  //------------------------------------------------------
                  double lha_weight = 1;
                  // double lha_weight = LHAPDF::xfx(0,x1,Q,fl1)*LHAPDF::xfx(0,x2,Q,fl2) / (LHAPDF::xfx(1,x1,Q,fl1)*LHAPDF::xfx(1,x2,Q,fl2));
                  double weight_old = 1;
                  #ifdef LHAPDF_ON
                    if(parton1_x>1e-7 && parton2_x>1e-7 && parton1_x<1 && parton2_x<1)
                      weight_old = !sampleName.Contains("DATA") ? (LHAPDF::xfx(1,parton1_x,scalePDF,parton1_pdgId)*LHAPDF::xfx(1,parton2_x,scalePDF,parton2_pdgId)) : 1;
                  #endif
                  if(controlplots){
                    common_stuff::plot1D("hPDF_x1",TMath::Log10(parton1_x), 1, h_1d, 1000,-4,0 );
                    common_stuff::plot1D("hPDF_x1unweighted",TMath::Log10(parton1_x), 1/weight_old, h_1d, 1000,-4,0 );
                    common_stuff::plot1D("hPDF_x2",TMath::Log10(parton2_x), 1, h_1d, 1000,-4,0 );
                    common_stuff::plot1D("hPDF_x2unweighted",TMath::Log10(parton2_x), 1/weight_old, h_1d, 1000,-4,0 );
                  }
                  
                  for(int h=0; h<WMass::PDF_members; h++){
                    if( !contains_LHE_weights || (!sampleName.Contains("DATA") && WMass::PDF_sets>0 && WMass::PDF_sets!=generated_PDF_set && WMass::PDF_members!=generated_PDF_member)){
                      double weight_new = 1;
                      #ifdef LHAPDF_ON
                        LHAPDF::usePDFMember(0,h);
                        if(parton1_x>1e-7 && parton2_x>1e-7 && parton1_x<1 && parton2_x<1)
                          weight_new = (LHAPDF::xfx(0,parton1_x,scalePDF,parton1_pdgId)*LHAPDF::xfx(0,parton2_x,scalePDF,parton2_pdgId));
                      #endif
                      lha_weight = weight_new/weight_old;
                      if(controlplots) 
                        common_stuff::plot1D(Form("hPDF_weights_%d_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h),
                                      lha_weight, 1, h_1d, 1000,0,2);
                    }
                    else if(contains_LHE_weights){
                      // common_stuff::plot2D(Form("hCorrPDFweights_pdf%d-%d_eta%s_%.0f",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                      // lha_weight,LHE_weight[PDF_reweighting_central_Index+h], 1, 
                                      // h_2d, 200,0.9,1.1,200,0.9,1.1);
                      
                      lha_weight = LHE_weight[PDF_reweighting_central_Index+h];
                    }
                    
                    // WEIGHT DEFINITION
                    double weight = evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight;

                    // RECOIL PRE-UNBLINDING REWEIGHTING
                    bool reweight_recoil_pre_unblinding = false; // CHANGE THIS IF NOT NEEDED!!!!!!!!!!!
                    if(reweight_recoil_pre_unblinding && sampleName.Contains("DYJetsPow")){
                      // double weight_pre_unblinding_rew = weight;
                      weight *= pre_unblinding5_recoil_ratiomc_div_data->Interpolate(Wlike.Pt())>0?pre_unblinding5_recoil_ratiomc_div_data->Interpolate(Wlike.Pt()):1;
                      // if(Wlike.Pt()<2) {
                        // cout << "Wlike.Pt()= " << Wlike.Pt();
                        // cout << " histo_ratio= " << pre_unblinding5_recoil_ratiomc_div_data->Interpolate(Wlike.Pt());
                        // cout << " weight before= " << weight_pre_unblinding_rew;
                        // cout << " weight after/before= " << weight/weight_pre_unblinding_rew;
                        // cout << endl;
                      // }
                    }
                    
                    //------------------------------------------------------
                    // start loop on mass hypotheses
                    //------------------------------------------------------
                    for(int j=0; j<WMass::NMassValues; j++){

                      double Zmass_MeV = WMass::Zmass_values[j];
                      double Zmass_GeV = WMass::Zmass_values[j]/1e3;

                      if(!(sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig")) && Zmass_MeV != WMass::ZMassCentral_MeV) continue;
                      
                      //------------------------------------------------------
                      // compute weight for mass hypotheses wiht Breit-Wigner reweighting or LHE
                      //------------------------------------------------------
                      double weight_i=1; // bweight_i=1,lheweight_i=1;
                      if(useGenVar){
                        if(!contains_LHE_weights){
                          double gamma=2.141; /*HARD CODED TO PDG VALUE*/
                          weight_i = common_stuff::BWweight(ZGen_mass, Zmass_GeV, gen_mass_value_MeV, gamma);
                        }
                        else{
                          weight_i = LHE_weight [ (WMass::LHE_mass_central_index + (j-WMass::NMassValues/2)*WMass::WMassSkipNSteps) ];
                        }
                      }
                      double weight_mass = weight * weight_i;

                      
                      //------------------------------------------------------
                      // "MONEY" PLOTS OF FIT VARIABLES WITHIN THE FIT RANGE
                      //------------------------------------------------------
                      
                      for(int k=0;k<WMass::NFitVar;k++){
                        // cout << Form("hWlike%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV) 
                             // << " " << Wlike_var_NotScaled[k] 
                             // << " " << weight
                             // // << " " << h_1d 
                             // << " " << 50
                             // << " " << WMass::fit_xmin[k]*ZWmassRatio 
                             // << " " <<  WMass::fit_xmax[k]*ZWmassRatio 
                             // << endl;
                        TString plotname = Form("hWlike%s_%s_8_JetCut_pdf%d-%d_eta%s", WCharge_str.Data(), WMass::FitVar_str[k].Data(), WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets, h, eta_str.Data())
                        if (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))
                          plotname = Form("%s_.0f", plotname.Data(), Zmass_MeV);
                        common_stuff::plot1D(plotname, Wlike_vars[k], weight_mass, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                        
                        // 2D plot obs_i vs obs_j (no MtLin)
                        if (false) {
                          for(int k2=k+1;k2<WMass::NFitVar-1;k2++){
                            // cout << "k= " << k << " k2= " << k2 << endl;
                            common_stuff::plot2D(Form("hWlike%s_%svs%s_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::FitVar_str[k2].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),Zmass_MeV),
                                   Wlike_vars[k2],Wlike_vars[k], weight_mass,
                                   h_2d, 50, WMass::fit_xmin[k2]*ZWmassRatio, WMass::fit_xmax[k2]*ZWmassRatio,
                                   50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                          }
                        }
                      }
                    }

                    //------------------------------------------------------------------------------------------------
                    // EXTRA PLOTS - these are inclusive distributions used to derive and verify weights
                    //------------------------------------------------------------------------------------------------

                    if(controlplots || preUnblinding){

                      // Leptons Efficiencies
                      common_stuff::plot1D(Form("h_eff_TIGHT_SF%s_%s_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::FitVar_str[1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                                           eff_TIGHT_SF, weight, h_1d, 200, -0.5, 1.5 );

                      common_stuff::plot1D(Form("h_eff_ISO_SF%s_%s_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::FitVar_str[1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                                           eff_ISO_SF, weight, h_1d, 200, -0.5, 1.5 );

                      common_stuff::plot1D(Form("h_eff_TIGHT_subleading_SF%s_%s_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::FitVar_str[1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                                           eff_TIGHT_subleading_SF, weight, h_1d, 200, -0.5, 1.5 );

                      common_stuff::plot1D(Form("h_eff_TRG_SF%s_%sNonScaled_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::FitVar_str[1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                                           eff_TRG_SF, weight, h_1d, 200, -0.5, 1.5 );


                      // Boson Kinematics Zpt, Zmass, Zrecoil, Zrapidity
                      common_stuff::plot1D(Form("hWlike%s_ZpT_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                                        Zcorr.Pt(), weight, h_1d, 100, 0, 50 );
                      common_stuff::plot1D(Form("hWlike%s_Zmass_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                                        Zcorr.M(), weight, h_1d, 150, 75., 105. );

                      common_stuff::plot1D(Form("hWlike%s_Recoil_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                                        Wlike.Pt(), weight, h_1d, 50, 0, 25 );

                      common_stuff::plot1D(Form("hWlike%s_Zrap_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                                        Zcorr.Rapidity(), weight, h_1d, 100,-3,3 );


                      if(controlplots) {
                        common_stuff::plot2D(Form("hWlike%s_MtVsGenZPt_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                           ZGen_pt,Wlike_vars[1], weight, h_2d, 40, 0,20,
                           50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio );
                      }

                      // Boson polarization
                      common_stuff::plot2D(Form("hWlike%s_phi_vs_costh_CS_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                        costh_CS,TMath::Abs(phi_CS), weight, h_2d, 12,-1,1, 12,0,TMath::Pi() );

                      common_stuff::plot1D(Form("hWlike%s_costh_CS_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                        costh_CS, weight, h_1d, 40,-1,1 );

                      common_stuff::plot1D(Form("hWlike%s_phi_CS_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                        TMath::Abs(phi_CS), weight, h_1d, 12, 0, TMath::Pi() );

                      if(controlplots) {
                        if( TMath::Abs(ZcorrCentral.Rapidity())<2.1 && ZcorrCentral.Pt()<1000 ){
                          int i_rapbin = hrapbins->GetXaxis()->FindBin(TMath::Abs(ZcorrCentral.Rapidity()))-1 ;
                          // cout << "i_rapbin= " << i_rapbin<< endl;;
                          int i_ptbin = hptbins->GetXaxis()->FindBin(ZcorrCentral.Pt())>0 ? hptbins->GetXaxis()->FindBin(ZcorrCentral.Pt())-1 : 0 ;
                          // cout << " i_ptbin= " << i_ptbin<< endl;;
                          // hratioAngCoefSF[i_rapbin][i_ptbin]->Print();
                          // int costh_bin = hratioAngCoefSF[i_rapbin][i_ptbin]->GetXaxis()->FindBin(costh_CS);
                          // cout << " costh_bin= " << costh_bin<< endl;;
                          // int phi_bin = hratioAngCoefSF[i_rapbin][i_ptbin]->GetYaxis()->FindBin(TMath::Abs(phi_CS));
                          // cout << " phi_bin= " << phi_bin<< endl;;
                          common_stuff::plot1D(Form("costh_CS_rapbin%d_ptbin%d_Wlike%s_8_JetCut_pdf%d-%d_eta%s_%.0f",i_rapbin,i_ptbin,WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                                   costh_CS, weight, h_1d, 12, -1, 1);
                          common_stuff::plot1D(Form("phi_CS_rapbin%d_ptbin%d_Wlike%s_8_JetCut_pdf%d-%d_eta%s_%.0f",i_rapbin,i_ptbin,WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                                   TMath::Abs(phi_CS), weight, h_1d, 12, 0, TMath::Pi());
                        }
                      }
                    }

                    //------------------------------------------------------------------------------------------------
                    // BELOW PLOTS for CLOSURE TEST - Various binned plots - for recoil plots - Zcentral info to avoid randomization
                    //------------------------------------------------------------------------------------------------

                    if(Wlike_met.Pt()>0 && controlplots) {

                      string tag_zPtcut;
                      if ( Zcorr.Pt()<2 ) tag_zPtcut = "_Zpt02";
                      else if  ( ZcorrCentral.Pt()>=2 && ZcorrCentral.Pt()<4 ) tag_zPtcut = "_Zpt24";
                      else if  ( ZcorrCentral.Pt()>=4 && ZcorrCentral.Pt()<6 ) tag_zPtcut = "_Zpt46";
                      else if  ( ZcorrCentral.Pt()>=6 && ZcorrCentral.Pt()<8 ) tag_zPtcut = "_Zpt68";
                      else if  ( ZcorrCentral.Pt()>=8 && ZcorrCentral.Pt()<10 ) tag_zPtcut = "_Zpt810";
                      else if  ( ZcorrCentral.Pt()>=10 && ZcorrCentral.Pt()<12 ) tag_zPtcut = "_Zpt1012";
                      else if  ( ZcorrCentral.Pt()>=12 && ZcorrCentral.Pt()<14 ) tag_zPtcut = "_Zpt1214";
                      else if  ( ZcorrCentral.Pt()>=14 && ZcorrCentral.Pt()<16 ) tag_zPtcut = "_Zpt1416";
                      else if  ( ZcorrCentral.Pt()>=16 && ZcorrCentral.Pt()<18 ) tag_zPtcut = "_Zpt1618";
                      else if  ( ZcorrCentral.Pt()>=18 && ZcorrCentral.Pt()<20 ) tag_zPtcut = "_Zpt1820";
                      else if  ( ZcorrCentral.Pt()>=20 && ZcorrCentral.Pt()<30 ) tag_zPtcut = "_Zpt2030";
                      else if  ( ZcorrCentral.Pt()>=30 && ZcorrCentral.Pt()<50 ) tag_zPtcut = "_Zpt3050";
                      else if  ( ZcorrCentral.Pt()>=50 ) tag_zPtcut = "_Zpt50";
                      else tag_zPtcut = "_ignore";

                      double Zy=ZcorrCentral.Rapidity();
                      string tag_y;
                      if ( Zy>=0 && Zy<0.5 ) tag_y = "_Zy0005";
                      else if  ( Zy>=0.5 && Zy<1.0 ) tag_y = "_Zy0510";
                      else if  ( Zy>=1.0 && Zy<1.5 ) tag_y = "_Zy1015";
                      else if  ( Zy>=1.5 && Zy<2.0 ) tag_y = "_Zy1520";
                      else if  ( Zy>=2.0 ) tag_y = "_Zy20inf";
                      else if  ( Zy>=(-0.5) && Zy<0.0 ) tag_y = "_Zy0500";
                      else if  ( Zy>=(-1.0) && Zy<(-0.5) ) tag_y = "_Zy1005";
                      else if  ( Zy>=(-1.5) && Zy<(-1.0) ) tag_y = "_Zy1510";
                      else if  ( Zy>=(-2.0) && Zy<(-1.5) ) tag_y = "_Zy2015";
                      else if  ( Zy<(-2.0) ) tag_y = "_Zyinf20";
                      else tag_y = "_ignore";

                      string tag_VTX="";
                      int n_vtx_max = 20; // 7 TeV
                      // int n_vtx_max = 35; // 8 TeV
                      if(nvtx==0) tag_VTX="_VTX1";
                      else if(nvtx>=1 && nvtx<=n_vtx_max) tag_VTX=Form("_VTX%d",nvtx);
                      else if(nvtx>n_vtx_max) tag_VTX=Form("_VTX%d",n_vtx_max);
                      
                      TLorentzVector VisPt;
                      VisPt.SetPtEtaPhiM(ZcorrCentral.Pt(),0,ZcorrCentral.Phi(),0);

                      TLorentzVector Zgen;
                      Zgen.SetPtEtaPhiM(ZGen_pt,0,ZGen_phi,0);

                      string mettype="_tk";
                      
                      // double u1_scale=0;
                      // plotVariables( Z_met, VisPt,  ZcorrCentral, Zgen, u1_scale, "closure", tag_zPtcut.c_str(), mettype.c_str() , false, false, h_1d, h_2d, weight, 1, WMass::ZMassCentral_MeV);
                      // plotVariables( Z_met, VisPt,  ZcorrCentral, Zgen, u1_scale, "closure", tag_VTX.c_str(), mettype.c_str() , false, false, h_1d, h_2d, weight, 1, WMass::ZMassCentral_MeV);
                      // plotVariables( Z_met, VisPt,  ZcorrCentral, Zgen, u1_scale, "closure", tag_y.c_str(), mettype.c_str() , false, false, h_1d, h_2d, weight, 1, WMass::ZMassCentral_MeV);
                    }

                    //---------------------------------------------------------------------
                    // Recoil plots: u1, u2, u1vsZpt, u2vsZpt, u1vsZptvsZrap, u2vsZptvsZrap (for recoil plots - Zcentral info to avoid randomization)
                    //---------------------------------------------------------------------
                    if(correctToMadgraph || controlplots){

                      common_stuff::calculateU1U2(met_transv, metphi_transv,  ZGen_pt, ZGen_phi,
                        ZNocorr.Pt(), ZNocorr.Phi(),  u1_recoil, u2_recoil);

                      double u_recoil = sqrt(u2_recoil*u2_recoil+u1_recoil*u1_recoil);

                      common_stuff::plot1D(Form("hWlike%s_u1_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                        u1_recoil, weight, h_1d, 60, -20, 20 );
                      common_stuff::plot1D(Form("hWlike%s_u2_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                        u2_recoil, weight, h_1d, 60, -20, 20 );
                      common_stuff::plot1D(Form("hWlike%s_u_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                         u_recoil, weight, h_1d, 60, +00, 20 );

                      common_stuff::plot2D(Form("hWlike%s_u1vsZpt_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                        ZcorrCentral.Pt(), u1_recoil, weight, h_2d, 60, 0, ZPt_cut, 60, -20, 20 );
                      common_stuff::plot2D(Form("hWlike%s_u2vsZpt_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                        ZcorrCentral.Pt(), u2_recoil, weight, h_2d, 60, 0, ZPt_cut, 60, -20, 20 );
                      common_stuff::plot2D(Form("hWlike%s_uvsZpt_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                        ZcorrCentral.Pt(),  u_recoil, weight, h_2d, 60, 0, ZPt_cut, 60, +00, 20 );

                      common_stuff::plot2D(Form("hWlike%s_u1vsZrap_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                        ZGen_status3.Rapidity(), u1_recoil, weight, h_2d, 60, -4, +4, 60, -20, 20 );
                      common_stuff::plot2D(Form("hWlike%s_u2vsZrap_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                        ZGen_status3.Rapidity(), u2_recoil, weight, h_2d, 60, -4, +4, 60, -20, 20 );

                      common_stuff::plot3D(Form("hWlike%s_u1vsZptvsZrap_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                        ZcorrCentral.Pt(), ZGen_status3.Rapidity(), u1_recoil, weight, h_3d, 60, 0, ZPt_cut, 60, -4, +4, 60, -20, 20 );
                      common_stuff::plot3D(Form("hWlike%s_u2vsZptvsZrap_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                        ZcorrCentral.Pt(), ZGen_status3.Rapidity(), u2_recoil, weight, h_3d, 60, 0, ZPt_cut, 60, -4, +4, 60, -20, 20 );

                    }

                    if (controlplots) {
                      //------------------------------------------------------------------------------------------------
                      // PLOTS FOR GIGI's TEST see 11 apr 2014 (CMG presentations)
                      //------------------------------------------------------------------------------------------------
                      //      cout << "filling control plot RecoilVar=" << RecoilVar_str.Data() << endl;
                      common_stuff::plot1D(Form("deltaMT_Wlike%s_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                              Wlike.Mt() - WlikeCentral.Mt(), weight, h_1d, 200, -0.1, 0.1);

                      common_stuff::plot1D(Form("deltaMET_Wlike%s_8_JetCut_pdf%d-%d_eta%s_%.0f",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),WMass::ZMassCentral_MeV),
                              Wlike_met.Pt() - Wlike_metCentral.Pt(), weight, h_1d, 200, -0.1, 0.1);
                    }

                  } // end loop PDF
                  
                  //------------------------------------------------------------------------------------------------
                  // control plots for different etas but only for central W mass
                  //------------------------------------------------------------------------------------------------ 
                  if(controlplots)
                    fillControlPlots(Zcorr, Z_met, muPosCorr, muNegCorr, h_1d, h_2d, evt_weight*TRG_TIGHT_ISO_muons_SF, WMass::ZMassCentral_MeV, eta_str, WMass::nSigOrQCD_str[0],Form("Wlike%s_8_JetCut",WCharge_str.Data()));
                }
              }
            }
          }
          else if(controlplots && isChargePos){
            // NOTE: ONLY DONE FOR WLIKEPOS !!!
            
            // Iso and dxy for muons which fail either tight requirement, isolation or dxy cut
            // if(pfmetWlikePos>25 && WlikePos_pt<20){
              // if( (TMath::Abs(zmass1 - WMass::WMassCentral_MeV) > 1)) continue;
            // }

            //------------------------------------------------------
            // Control plots in the QCD enriched region
            //------------------------------------------------------
            // invert cuts to select QCD enriched region, no cuts on met or jet
            if(
              MuPosRelIso>0.12 // single muon cuts (inverted iso (is <0.5 for signal) , no tight requirement)
              && MuPos_dxy>0.02 // single muon cuts (MuPosIsTight contains dxy < 0.2 cut)
              && WlikePos_pt<20/* *WMass::ZMassCentral_MeV/iZmass_GeV */ 
              ){

              // for(int k=0;k<WMass::NFitVar;k++)
                // common_stuff::plot1D(Form("hWlikePos_%sScaled_QCD_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                    // MuPos_var_jacobian[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, nbins, bins_scaled[k] );

              //------------------------------------------------------
              // QCD distributions for central W mass
              //------------------------------------------------------

              fillControlPlots(Zcorr, Z_met, muPosCorr, muNegCorr, h_1d, h_2d, evt_weight*TRG_TIGHT_ISO_muons_SF, WMass::ZMassCentral_MeV, eta_str, WMass::nSigOrQCD_str[1],Form("Wlike%s_8_JetCut",WCharge_str.Data()));

            } // end if for qcd enriched
          } // end else for muon cuts (sig or qcd enriched)
        } // end if for good pair within acceptance cuts for both muons
      } // end if for good reco event
    } // end if for dummy signal/fake separation
  } // end event loop

  TFile*fout = new TFile(Form("%s/Zanalysis%s.root",outputdir.Data(),chunk_str.Data()),"RECREATE");
  fout->cd();

  if (controlplots) hZPtSF->Write();

  common_stuff::writeOutHistos(fout, h_1d, h_2d, h_3d);
    
  fout->Write();
  fout->Close();
  
  cout << "file correctly written" << endl;

}

//----------------------------------------------------
void Zanalysis::ComputeHXVarAndPhiStarEta(TLorentzVector muPos,TLorentzVector muNeg, bool isGen){
  
  // RECO
  if(muPos.Pt()<0 || muNeg.Pt()<0) return;
  
  if(muPos.Pt()>muNeg.Pt()){
    GoToHXframe->TransformToHelicityFrame(muPos,muNeg);
  }else{
    GoToHXframe->TransformToHelicityFrame(muNeg,muPos);
  }

  if(muPos.Pt()>muNeg.Pt()){
    GoToCSframe->TransformToCS(muPos,muNeg);
  }else{
    GoToCSframe->TransformToCS(muNeg,muPos);
  }
  
  if(isGen){
    costh_HX_gen = GoToHXframe->GetCosTheta();
    phi_HX_gen = GoToHXframe->GetPhi();
    costh_CS_gen = GoToCSframe->GetCosTheta();
    phi_CS_gen = GoToCSframe->GetPhi();
  }else{
    costh_HX = GoToHXframe->GetCosTheta();
    phi_HX = GoToHXframe->GetPhi();
    costh_CS = GoToCSframe->GetCosTheta();
    phi_CS = GoToCSframe->GetPhi();
  }
}
