// UNCOMMENT TO USE PDF REWEIGHTING
//#define LHAPDF_ON

#ifdef LHAPDF_ON
  #include "LHAPDF/LHAPDF.h"
#endif 

#define Zanalysis_cxx
#include "Zanalysis.h"
#include "common.h"
#include "common_stuff.h"
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
const double submuon_eta_cut = 2.1;
const bool preUnblinding = true; // this is meant to save only the inclusive plots the controlplots variable is used for the various binned plots
const bool polarization_checks = false; // this is meant to save only the inclusive plots the controlplots variable is used for the various binned plots
const bool useAlternateEventXweights = false; // this is meant to save only the inclusive plots the controlplots variable is used for the various binned plots
const bool doRecoilMassVariations = false; // this is meant to save Recoil distribution for different mass hypotheses
const bool testRecoilOnData = false; // this does the recoil component plot data/MC comparison

void Zanalysis::Loop(int chunk, int Entry_ini, int Entry_fin, int IS_MC_CLOSURE_TEST, int isMCorDATA, TString outputdir, int useMomentumCorr, int varyMuonCorrNsigma, int MuonCorrAsDATA, int useEffSF, int usePtSF, int useVtxSF, int controlplots, TString sampleName, int generated_PDF_set, int generated_PDF_member, int contains_PDF_reweight, int useRecoilCorr, int correctToMadgraph, int RecoilCorrVarDiagoParSigmas, int RecoilCorrVarDiagoParU1orU2fromDATAorMC, int use_PForNoPUorTKmet, int use_syst_ewk_Alcaraz, int gen_mass_value_MeV, int contains_LHE_weights, int reweight_polarization)
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
  
  const bool isChargePos = WMass::WlikeCharge==1;
  TString WCharge_str = isChargePos?"Pos":"Neg";
  TLorentzVector& muNoCorr            = isChargePos?muPosNoCorr     :muNegNoCorr;
  TLorentzVector& neutrinoNoCorr      = isChargePos?muNegNoCorr     :muPosNoCorr;
  TLorentzVector& muCorr              = isChargePos?muPosCorr       :muNegCorr;
  TLorentzVector& neutrinoCorr        = isChargePos?muNegCorr       :muPosCorr;
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
  
  // Booleans for MC signal samples, we use these a lot:
  bool isPowheg   = sampleName.Contains("DYJetsPow");
  bool isMadgraph = sampleName.Contains("DYJetsMadSig");
  bool isPowOrMad = isPowheg || isMadgraph;
  
  //------------------------------------------------------
  // retrieve efficiencies SF
  //------------------------------------------------------
  TH3F *SF_HLT;
  TH2F *SF_TIGHT_ISO;
  TH2F *SF_TIGHT_PT10;
  TH3F *SF_ISO05_PT10;
  
  cout << "CHECKING EFFICIENCIES, useEffSF= " << useEffSF << endl;
  if(((useEffSF>=2 && useEffSF<=6) || (useEffSF>=13 && useEffSF<=16)) && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    
    TFile* finEffSF = new TFile("../utils/MuonEfficiencies_SF_2011_53X_DataMC_Heiner.root"); // used only to build templates
    if(!finEffSF){
      cout << "ERROR: file MuonEfficiencies_SF_2011_53X_DataMC_Heiner.root is missing, impossible to retrieve efficiency scale factors" << endl;
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
    TFile* finPileupSF = new TFile("../utils/pileup/pileup_reweighting_Fall11.root"); // used only to build templates
    if(!finPileupSF){
      cout << "ERROR: file ../utils/pileup/pileup_reweighting_Fall11.root is missing, impossible to retrieve pileup reweighting factors" << endl;
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
      cout << "ERROR: file ../utils/fout_invariant_mass_qed_higstat.root is missing, impossible to retrieve invariant mass SF (for EWK)" << endl;
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
  // Define binning for rap and pt
  //------------------------------------------------------
  const int nrapbins = 2;
  const int nptbins = 8;
  double rapbins[nrapbins+1] = {0,1,2.1};
  TH1D *hrapbins = new TH1D("hrapbins","hrapbins",nrapbins,rapbins);
  double ptbins[nptbins+1] = {0.1,10,20,35,55,80,120,200,1000};
  TH1D *hptbins = new TH1D("hptbins","hptbins",nptbins,ptbins);

  //------------------------------------------------------
  // retrieve boson pT SF
  //------------------------------------------------------
  TH1D* hZPtSF;
  if(usePtSF==0 && isPowOrMad) {

    TString filename=Form("../utils/Zpt_%soutput_%s_%s.root",useAlternateEventXweights?"altern_":"",sampleName.Data(),WCharge_str.Data());
    cout << "hZPtSF_central = " << filename.Data() << endl;

    TFile* finZPtSF = new TFile(filename.Data());
    if(!finZPtSF){
      cout << "ERROR: file " << filename << " is missing, impossible to retrieve Zpt reweighting factors" << endl;
      return;
    }
    hZPtSF=(TH1D*) finZPtSF->Get(Form("hWlike%s_ZpT_8_JetCut_pdf229800-0_eta0p9_91188",WCharge_str.Data())); hZPtSF->Sumw2();
    
  } else hZPtSF = new TH1D("hZPtSF","hZPtSF",10,0,1);


  //------------------------------------------------------
  // Polarization reweighting
  //------------------------------------------------------

  TH2D* hZPolSF;
  if(reweight_polarization==1 && isPowheg) {

    TString filename = Form("../utils/Zpol_Zrap_cosTheta_%soutput_%s_%s_PtSFCorr0.root",useAlternateEventXweights?"altern_":"",sampleName.Data(),WCharge_str.Data());
    cout << "hZpolSF_central = " << filename.Data() << endl;

    TFile* finZPolSF = new TFile(filename.Data());
    if(!finZPolSF){
      cout << "ERROR: file " << filename << " is missing, impossible to retrieve Zpol reweighting factors" << endl;
      return;
    }
    hZPolSF=(TH2D*) finZPolSF->Get(Form("hWlike%s_Zrap_vs_costh_CS_8_JetCut_pdf229800-0_eta0p9_91188",WCharge_str.Data())); hZPolSF->Sumw2();

  }
  else hZPolSF = new TH2D("hZPolSF","hZPolSF",10,0,1,10,0,1);
  
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
  // Define eta_max cut
  // WMass::etaMaxMuons = 0.9
  //------------------------------------------------------
  TString eta_str = Form("%.1f",WMass::etaMaxMuons); eta_str.ReplaceAll(".","p");

  //------------------------------------------------------
  // Initialize momentum scale corrections
  //------------------------------------------------------
  // To get the central value of the momentum correction
  KalmanCalibratorParam *corrector_KalmanParam;
  if(useMomentumCorr==4){
    cout << "using Kalman Calibrator Param" << endl;
    bool correctAsDATA = MuonCorrAsDATA || isMCorDATA!=0;
    corrector_KalmanParam = new KalmanCalibratorParam(correctAsDATA); // True for data , //False for MC
  }

  //------------------------------------------------------
  // Initialize recoil corrections
  //------------------------------------------------------

  const int m_start = WMass::RecoilCorrIniVarDiagoParU1orU2fromDATAorMC_[RecoilCorrVarDiagoParU1orU2fromDATAorMC];
  const int m_end = WMass::RecoilCorrNVarDiagoParU1orU2fromDATAorMC_[RecoilCorrVarDiagoParU1orU2fromDATAorMC];
  const bool varyRecoilCorr = (0<RecoilCorrVarDiagoParU1orU2fromDATAorMC && RecoilCorrVarDiagoParU1orU2fromDATAorMC<13);
  const bool useRecoilToys = (RecoilCorrVarDiagoParU1orU2fromDATAorMC==13);
  const int rapBinRecoilVariation = RecoilCorrVarDiagoParU1orU2fromDATAorMC <= 6 ? 0 : 1;
  const int recoilCorrVariationParReduced = RecoilCorrVarDiagoParU1orU2fromDATAorMC>6 ? RecoilCorrVarDiagoParU1orU2fromDATAorMC-6 : RecoilCorrVarDiagoParU1orU2fromDATAorMC;
  const double recoilSmearing = 0.01;

  TString generatorSuffix="_powheg";
  if (isMadgraph)
    generatorSuffix="_madgraph";

  RecoilCorrector*  correctorRecoil_Z;
  const bool correctWithKeys = (useRecoilCorr==3);
  const int recoilCorrScale = 1;
  if(useRecoilCorr>0){
    string RCset = "JAN31";
    string fileCorrectTo = /*POW */ Form("../RecoilCode/%s/recoilfit_%s_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X%s.root", RCset.c_str(), RCset.c_str(), generatorSuffix.Data());
    string fileZmmMC =     /*POW */ Form("../RecoilCode/%s/recoilfit_%s_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X%s.root", RCset.c_str(), RCset.c_str(), generatorSuffix.Data());
    string fileZmmData =   /*DATA*/ Form("../RecoilCode/%s/recoilfit_%s_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_triGauss_halfStat_UNBINNED_3G_53X.root", RCset.c_str(), RCset.c_str());
    if(correctToMadgraph)
           fileZmmData =   /*MAD */ Form("../RecoilCode/%s/recoilfit_%s_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root", RCset.c_str(), RCset.c_str());

    // ROOKEYS
    string basepath = "root://eoscms//eos/cms//store/group/phys_smp/Wmass/dalfonso/RecoilFiles/DEC6/";
    string fileZmmKeysCorrectTo = Form("%s/keysrecoilfit_DEC6_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_absolute_UNBINNED_3G_53X%s.root", basepath.c_str(), generatorSuffix.Data());
    string fileZmmKeysMC = Form("%s/keysrecoilfit_DEC6_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_absolute_UNBINNED_3G_53X%s.root", basepath.c_str(), generatorSuffix.Data());
    string fileZmmKeysData = basepath + "keysrecoilfit_DEC6_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_triGauss_halfStat_UNBINNED_3G_53X.root";
    if(correctToMadgraph) 
           fileZmmKeysData = basepath + "keysrecoilfit_DEC6_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_absolute_UNBINNED_3G_53X_madgraph.root";

    TString model_name = "fitresult_Add";
    cout << "INITIALIZING RECOIL MC TARGET FILE" << endl;
    correctorRecoil_Z = new RecoilCorrector(correctWithKeys, fileCorrectTo, fileZmmKeysCorrectTo, model_name);
    cout << "INITIALIZING RECOIL Z DATA FILE" << endl;
    correctorRecoil_Z->addDataFile(fileZmmData, fileZmmKeysData, model_name);
    cout << "INITIALIZING RECOIL Z MC FILE" << endl;
    correctorRecoil_Z->addMCFile(fileZmmMC, fileZmmKeysMC, model_name);
  }

  const double ZWmassRatio = ((double)WMass::ZMassCentral_MeV)/((double)WMass::WMassCentral_MeV);
  
  GoToHXframe = new HTransformToHelicityFrame();
  GoToCSframe = new HTransformToCS();
  
  //------------------------------------------------------
  // start the event loop
  //------------------------------------------------------
  int cout_freq=TMath::Min(1+(nentries-first_entry)/10,(Long64_t) 25000);
  // cout_freq=1; // FOR DEBUGGING PURPOSE
  // nentries=100; // FOR DEBUGGING PURPOSE
  cout << "couts every " << cout_freq << " events" << endl;
  for(Long64_t jentry=first_entry; jentry<=nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    fChain->GetEntry(jentry);

    if(jentry%cout_freq==0){
      time_t now = time(0);
      TString dt = ctime(&now); dt.ReplaceAll("\n"," ");
      cout << dt << "\t - \t Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    }
    
    //------------------------------------------------------------
    // Reject even events in DATA, ALSO in case of controlplots, since they were used for Recoil Corrections
    //------------------------------------------------------------
    if(jentry%2==0 && sampleName.Contains("DATA")) continue;

    // The following variables will get the recoil components from the corrector
    double u1_recoil = 0;
    double u2_recoil = 0;

    //------------------------------------------------------
    // initilize the mu+, mu-, met event variables
    //------------------------------------------------------

    double met_trasv,metphi_trasv;
    double met_trasvCentral,metphi_trasvCentral;
    
    if(use_PForNoPUorTKmet==0){
      met_trasv = pfmet;
      metphi_trasv = pfmet_phi;
    }else if(use_PForNoPUorTKmet==1){
      met_trasv = nopumet;
      metphi_trasv = nopumet_phi;
    }else if(use_PForNoPUorTKmet==2){
      met_trasv = tkmet;
      metphi_trasv = tkmet_phi;
    }else{
      cout << "MET TYPE OPTION use_PForNoPUorTKmet: " << use_PForNoPUorTKmet << " NOT DEFINED, USE 0:PF, 1:NOPU, 2:TK" << endl;
      return;
    }
    
    met_trasvCentral = met_trasv;
    metphi_trasvCentral = metphi_trasv;

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

    //---------------- LUMI weight
    double evt_weight = lumi_scaling;
    // cout << "evt_weight_original " << evt_weight_original << endl;

    //---------------- VTX weight
    if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && npu>0)
      evt_weight *= hPileupSF->GetBinContent(hPileupSF->GetXaxis()->FindBin(npu));

    if((IS_MC_CLOSURE_TEST || isMCorDATA==0) && controlplots) 
      common_stuff::plot1D("hPileUp_Fall11",npu, 1, h_1d, 50,0,50);

    //---------------- Invariant Mass weight
    // cout << "mass= " << ZGen_PostFSR_mass << " use_syst_ewk_Alcaraz = "<< use_syst_ewk_Alcaraz << endl;
    if(use_syst_ewk_Alcaraz>-1 && use_syst_ewk_Alcaraz!=100 && isPowheg){
      double ewk_weight_central = (ZGen_PostFSR_mass>50&&ZGen_PostFSR_mass<200)?hZmassSF_central->Interpolate(ZGen_PostFSR_mass):1;
      // cout << "ewk_weight_central = ewk_weight_central ("<<ewk_weight_central<<")" << endl;
      evt_weight *= ewk_weight_central;
      if(use_syst_ewk_Alcaraz>0){
        double ewk_weight_syst = (ZGen_PostFSR_mass>50&&ZGen_PostFSR_mass<200)?hZmassSF_syst->Interpolate(ZGen_PostFSR_mass):1;
        // cout << "evt_weight_original *= ewk_weight_syst ("<<ewk_weight_syst<<")" << endl;
        evt_weight *= ewk_weight_syst;
        if(use_syst_ewk_Alcaraz>30){ // 3 times the corrections (3 sigmas)
          // cout << "evt_weight_original *= (ewk_weight_syst*ewk_weight_syst)" << endl;
          evt_weight *= (ewk_weight_syst*ewk_weight_syst);
      
        // cout << "mass= " << ZGen_PostFSR_mass << " ewk weight= " << ewk_weight_central << endl;
        }
      }
    }
    
    //---------------- EWK weight
    if(isPowOrMad){
      if(use_syst_ewk_Alcaraz==100 && FSRWeight>-99){ 
        evt_weight *= FSRWeight;
        // cout << "FSRWeight= " << FSRWeight << endl;
      }        
    }

    //------------------------------------------------------
    // retrieve and use gen-level info
    //------------------------------------------------------
    // SELECT EVENT WITH Z/W SCALED CUTS
    // gen stuff to be used only when required
    if(useGenVar){
      if(ZGen_mass>0){
        double WlikeGen_var_NotScaled[] = {muGen_status3.Pt(),ZGen_mt,neutrinoGen_status3.Pt()};
        
        for(int k=0;k<WMass::NFitVar;k++)
          common_stuff::plot1D(Form("hWlike%s_%sNonScaled_1_Gen_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                WlikeGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
          
          // common_stuff::plot1D(Form("hWlike%s_MGenNonScaled_1_GenBW_eta%s_%d",WCharge_str.Data(),eta_str.Data(),jZmass_MeV),
                                // ZGen_mass, bweight_i, h_1d, 50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio );
          // common_stuff::plot1D(Form("hWlike%s_MGenNonScaled_1_GenLHE_eta%s_%d",WCharge_str.Data(),eta_str.Data(),jZmass_MeV),
                                // ZGen_mass, lheweight_i, h_1d, 50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio );

        if(ZGen_mass>50){
          for(int k=0;k<WMass::NFitVar;k++)
            common_stuff::plot1D(Form("hWlike%s_%sNonScaled_2_ZGenMassCut_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                  WlikeGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
          
          if(TMath::Abs(muGen_status3.Eta())<WMass::etaMaxMuons){
            for(int k=0;k<WMass::NFitVar;k++)
              common_stuff::plot1D(Form("hWlike%s_%sNonScaled_3_Mu1GenCut_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                    WlikeGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
            
            if(TMath::Abs(neutrinoGen_status3.Eta())<submuon_eta_cut){
              for(int k=0;k<WMass::NFitVar;k++)
                common_stuff::plot1D(Form("hWlike%s_%sNonScaled_4_Mu2GenCut_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
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
      // continue;
      // TO BE FITTED WITH
      // [1] The fit function for binning in cos_theta and phi:
      // [0]*0.5*(1-3*x*x)+[1]*sin(2*acos(x))*cos(y)+[2]*0.5*sin(acos(x))*sin(acos(x))*cos(2*y)+[3]*sin(acos(x))*cos(y)+[4]*x + [5]*(1+x*x)

    } // end gen stuff 

    if(!useGenVar || Z_mass>0){ // dummy thing to separate signal and background in DY+Jets (useless)

      //------------------------------------------------------
      // start reco event selection
      //------------------------------------------------------
      
      //------------------------------------------------------
      // good muon pair within acceptance cuts
      //------------------------------------------------------
      if( evtHasGoodVtx && evtHasTrg && muNoCorr.Pt()>0
          && TMath::Abs(muCorrCentral.Eta())<WMass::etaMaxMuons
          // && TMath::Abs(neutrinoCorrCentral.Eta())<2.4
          && TMath::Abs(neutrinoCorrCentral.Eta())<submuon_eta_cut // CHANGED TO 2.1 DURING PLOTS PRE-UNBLINDING
          && MuPos_charge != MuNeg_charge
          && muTrg
          //-----------------------------------------------------------------
          // full ID and tight requirements on the muons as defined by Heiner for the efficiencies
          //-----------------------------------------------------------------
          && MuPosIsTight && MuPos_dxy<0.02
          && MuNegIsTight && MuNeg_dxy<0.02
          && muRelIso<0.12 && neutrinoRelIso<0.5
        ){ // good reco event

        // Polarization variables
        costh_HX = -1e10;     phi_HX = -1e10;
        costh_HX_gen = -1e10; phi_HX_gen = -1e10;
        costh_CS = -1e10;     phi_CS = -1e10;
        costh_CS_gen = -1e10; phi_CS_gen = -1e10;

        if(MuPosGen_pt>0 && MuNegGen_pt>0) ComputeHXVarAndPhiStarEta(muPosGen_status3,muNegGen_status3,true);
        if(MuPos_pt>0 && MuNeg_pt>0) ComputeHXVarAndPhiStarEta(muPosNoCorr,muNegNoCorr,false);
        
        // charge invariant variables
        double& Leading_Mu_pt  = MuPos_pt>MuNeg_pt ? MuPos_pt  : MuNeg_pt;
        double& Leading_Mu_eta = MuPos_pt>MuNeg_pt ? MuPos_eta : MuNeg_eta;
        double& Leading_Mu_phi = MuPos_pt>MuNeg_pt ? MuPos_phi : MuNeg_phi;
        double& SubLeading_Mu_pt  = MuPos_pt>MuNeg_pt ? MuNeg_pt  : MuPos_pt;
        double& SubLeading_Mu_eta = MuPos_pt>MuNeg_pt ? MuNeg_eta : MuPos_eta;
        double& SubLeading_Mu_phi = MuPos_pt>MuNeg_pt ? MuNeg_phi : MuPos_phi;
        
        //---------------- MUON weight
        double TRG_TIGHT_ISO_muons_SF = 1;
        double eff_TIGHT_SF = 1;
        double eff_TIGHT_subleading_SF = 1;
        double eff_ISO_SF = 1;
        double eff_TRG_SF = 1;

        // Build variables for Recoil corrections
        int rapBin=0;
        if(fabs(ZGen_rap)>=1) rapBin=1;

        for(int m=m_start; m<m_end; m++){

          TString RecoilVar_str = "";
          if(varyRecoilCorr || useRecoilToys) RecoilVar_str = Form("_RecoilCorrVar%d",m);
          // cout << RecoilVar_str << endl;

          //------------------------------------------------------------------------------------------------
          // Apply recoil corrections
          //------------------------------------------------------------------------------------------------
          
          if(use_PForNoPUorTKmet<3 && isPowOrMad){ // use MET corrections if required
            
            if(use_PForNoPUorTKmet==0){
              met_trasv = pfmet;
              metphi_trasv = pfmet_phi;
            }else if(use_PForNoPUorTKmet==1){
              met_trasv = nopumet;
              metphi_trasv = nopumet_phi;
            }else if(use_PForNoPUorTKmet==2){
              met_trasv = tkmet;
              metphi_trasv = tkmet_phi;
            }
            
            // cout << "m= " << m  << " u1u2 " << WMass::RecoilCorrVarDiagoParU1orU2fromDATAorMC_[m] << " par " << WMass::RecoilCorrVarDiagoParN_[m]  << endl;
            // cout << " met_trasv before=" << met_trasv << " metphi_trasv before=" << metphi_trasv << " met_trasvCentral before=" << met_trasvCentral << " metphi_trasvCentral before=" << metphi_trasvCentral << endl;
            // cout << "met_trasvCentral before=" << met_trasvCentral << endl;
            // cout << "met_trasv before=" << met_trasv << endl;

            if(useRecoilCorr==2 || useRecoilCorr==3){
              // cout << "ZGen_pt=" << ZGen_pt << " ZGen_phi=" << ZGen_phi << " ZNocorr.Pt()=" << ZNocorr.Pt() << " ZNocorr.Phi()=" << ZNocorr.Phi() << endl;
              // cout << " RecoilCorrVarDiagoParU1orU2fromDATAorMC=" << RecoilCorrVarDiagoParU1orU2fromDATAorMC << " m=" << m << " RecoilCorrVarDiagoParSigmas=" << RecoilCorrVarDiagoParSigmas << " rapBin=" << rapBin << endl;
              // cout << "Before correction:" << m << " - " << met_trasv << " - " << metphi_trasv << endl;
              correctorRecoil_Z->reset(WMass::RecoilCorrNVarDiagoParU1orU2fromDATAorMC_[2],
                                       WMass::RecoilCorrNVarDiagoParU1orU2fromDATAorMC_[3],
                                       rapBinRecoilVariation);
              if(varyRecoilCorr && rapBin==rapBinRecoilVariation){
                // cout << "correcting met_trasv to eigen par " << recoilCorrVariationParReduced << ", m= " << m << endl;
                correctorRecoil_Z->CorrectMET(
                        met_trasv,metphi_trasv,
                        ZGen_pt,ZGen_phi,
                        ZNocorr.Pt(),ZNocorr.Phi(),
                        u1_recoil, u2_recoil,
                        recoilCorrVariationParReduced, m, RecoilCorrVarDiagoParSigmas,
                        rapBinRecoilVariation,recoilCorrScale,false);
              }
              else if (useRecoilToys) {
                common_stuff::calculateU1U2(met_trasv, metphi_trasv,  ZGen_pt, ZGen_phi,ZNocorr.Pt(), ZNocorr.Phi(),  u1_recoil, u2_recoil);
                random_->SetSeed(uint(abs(u1_recoil)*1e9 + abs(u2_recoil)*1e6 + m));
                u1_recoil *= random_->Gaus(1, recoilSmearing);
                u2_recoil *= random_->Gaus(1, recoilSmearing);
                met_trasv    = correctorRecoil_Z->calculate(0,ZNocorr.Pt(),ZNocorr.Phi(),ZGen_phi,u1_recoil,u2_recoil);
                metphi_trasv = correctorRecoil_Z->calculate(1,ZNocorr.Pt(),ZNocorr.Phi(),ZGen_phi,u1_recoil,u2_recoil);
              }
              else{
                // cout << "correcting met_trasv to default eigen par 0, m= " << m << endl;
                correctorRecoil_Z->CorrectMET(
                        met_trasv,metphi_trasv,
                        ZGen_pt,ZGen_phi,
                        ZNocorr.Pt(),ZNocorr.Phi(),
                        u1_recoil, u2_recoil,
                        0, 0, 0,
                        rapBin,recoilCorrScale,correctWithKeys);
              }
              if(m==m_start){
                // cout << "before setting met_trasvCentral "<< RecoilCorrVarDiagoParU1orU2fromDATAorMC<< " " << m << " " << RecoilCorrVarDiagoParSigmas << endl;
                if(correctWithKeys || (varyRecoilCorr && rapBin==rapBinRecoilVariation) || useRecoilToys) {
                  // cout << " setting met_trasvCentral to central" << endl;
                  correctorRecoil_Z->reset(WMass::RecoilCorrNVarDiagoParU1orU2fromDATAorMC_[2],
                                           WMass::RecoilCorrNVarDiagoParU1orU2fromDATAorMC_[3],
                                           rapBinRecoilVariation);
                  // cout << "correcting met_trasvCentral to eigen par " << recoilCorrVariationParReduced << ", m= " << m << endl;
                  correctorRecoil_Z->CorrectMET(
                          met_trasvCentral,metphi_trasvCentral,
                          ZGen_pt,ZGen_phi,
                          ZNocorr.Pt(),ZNocorr.Phi(),
                          u1_recoil, u2_recoil,
                          0, 0, 0,
                          rapBin,recoilCorrScale,false);
                }else{
                  // cout << "correcting met_trasvCentral to default met_trasv, m= " << m << endl;
                  // cout << " met_trasvCentral = met_trasv" << endl;
                  met_trasvCentral    = met_trasv;
                  metphi_trasvCentral = metphi_trasv;
                }
              }
            }
            // cout << "met_trasv after=" << met_trasv << " metphi_trasv after=" << metphi_trasv << " met_trasvCentral after=" << met_trasvCentral << " metphi_trasvCentral after=" << metphi_trasvCentral << endl;
          }
          // cout << "met_trasvCentral after=" << met_trasvCentral << endl;
          // cout << "met_trasv after=" << met_trasv << endl;

          for(int n=0; n<WMass::KalmanNvariations; n++){

            TString KalmanVars_str = "";
            if(WMass::KalmanNvariations>1) KalmanVars_str = Form("_KalmanVar%d",n);

            //------------------------------------------------------------------------------------------------
            // Apply muon corrections
            //------------------------------------------------------------------------------------------------
            if(m==m_start){ // use rochester corrections if required
              if(useMomentumCorr<=3){
                cout << "ERROR: useMomentumCorr<=3 unsupported" << endl;
                return;
              }
              else if(useMomentumCorr==4){ // use Momentum scale corrections from KalmanParam calibrator if required
                
                if(n==0){
                  corrector_KalmanParam->getCorrectedPt(muPosCorrCentral,MuPos_charge); //returns the corrected pt 
                  corrector_KalmanParam->getCorrectedPt(muNegCorrCentral,MuNeg_charge); //returns the corrected pt 
                  corrector_KalmanParam->smear(muPosCorrCentral);
                  corrector_KalmanParam->smear(muNegCorrCentral);
                }
                if(varyMuonCorrNsigma!=0){
                  if(WMass::KalmanNvariations==1){ // vary global scale (just once)
                    corrector_KalmanParam->varyClosure(varyMuonCorrNsigma);
                  }else{ // vary n-th fit eigen
                    muPosCorr = muPosNoCorr;
                    muNegCorr = muNegNoCorr;
                    corrector_KalmanParam->reset(); 
                    corrector_KalmanParam->vary(n,varyMuonCorrNsigma);
                  }
                }
                corrector_KalmanParam->getCorrectedPt(muPosCorr,MuPos_charge); //returns the corrected pt 
                corrector_KalmanParam->getCorrectedPt(muNegCorr,MuNeg_charge); //returns the corrected pt 
                corrector_KalmanParam->smear(muPosCorr);
                corrector_KalmanParam->smear(muNegCorr);
              }

              Zcorr = muPosCorr + muNegCorr;
              ZcorrCentral = muPosCorrCentral + muNegCorrCentral;
            }

            //------------------------------------------------------------------------------------------------
            // Apply PT and Pol weight based on RECO
            //------------------------------------------------------------------------------------------------
            
            if(m==m_start && n==0) {
              if(usePtSF!=-1  && usePtSF!=1 &&usePtSF!=2 && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && isPowOrMad)
                evt_weight *= hZPtSF->Interpolate(ZcorrCentral.Pt())>0?hZPtSF->Interpolate(ZcorrCentral.Pt()):1;

              // Boson Polarization
              common_stuff::ComputeAllVarPietro(muPosCorrCentral, muNegCorrCentral, costh_CS, phi_CS, costh_HX, phi_HX);

              // cout << " ZcorrCentral.Rapidity()= " << ZcorrCentral.Rapidity() << " ZcorrCentral.Pt()= " << ZcorrCentral.Pt() << " hrapbins->GetXaxis()->FindBin(ZcorrCentral.Rapidity())= " << hrapbins->GetXaxis()->FindBin(ZcorrCentral.Rapidity()) << " hptbins->GetXaxis()->FindBin(ZcorrCentral.Pt())= " << hptbins->GetXaxis()->FindBin(ZcorrCentral.Pt()) << " costh_CS= " << costh_CS << " phi_CS= " << phi_CS << endl;

              if(reweight_polarization==1 && isPowOrMad){
                double interpolated_weight = hZPolSF->Interpolate(costh_CS, TMath::Abs(Zcorr.Rapidity()));
                evt_weight *= interpolated_weight>0 ? interpolated_weight : 1;
              }
            }

            //-----------------------------
            // Throw toys for efficiency (i)
            //------------------------------
            TString effToy_str = "";
            for (int i=0; i<max(1, WMass::efficiency_toys); ++i) {
              
              if(WMass::efficiency_toys>0) effToy_str = Form("_effToy%d", i);
              
              if( (m==m_start && n==0) 
                  && ((useEffSF>=2 && useEffSF<=6) || (useEffSF>=13 && useEffSF<=16)) 
                  && (IS_MC_CLOSURE_TEST || isMCorDATA==0))
              {
                
                TRG_TIGHT_ISO_muons_SF = 1;
                eff_TIGHT_SF = 1;
                eff_TIGHT_subleading_SF = 1;
                eff_ISO_SF = 1;
                eff_TRG_SF = 1;
                
                if(useEffSF==2 || useEffSF==13 || useEffSF!=3){
                  // === leading
                  eff_TIGHT_SF = SF_TIGHT_ISO->GetBinContent(SF_TIGHT_ISO->FindBin(Leading_Mu_eta,Leading_Mu_pt));
                  if(useEffSF==13){
                    random_->SetSeed(UInt_t(TMath::Abs(Leading_Mu_phi)*1e9 + TMath::Abs(Leading_Mu_eta)*1e6 + TMath::Abs(Leading_Mu_pt)*1e3 + i));
                    eff_TIGHT_SF += random_->Gaus(0, TMath::Hypot(0.01, SF_TIGHT_ISO->GetBinError(SF_TIGHT_ISO->FindBin(Leading_Mu_eta, Leading_Mu_pt))));
                    // cout << "SF_TIGHT_ISO->GetBinError(SF_TIGHT_ISO->FindBin(Leading_Mu_eta, Leading_Mu_pt)= " << SF_TIGHT_ISO->GetBinError(SF_TIGHT_ISO->FindBin(Leading_Mu_eta, Leading_Mu_pt)) << endl;
                  }
                  TRG_TIGHT_ISO_muons_SF  *= eff_TIGHT_SF;
                }
                if(useEffSF==2 || useEffSF==14 || useEffSF!=4){
                  // === subleading
                  eff_ISO_SF   = SF_ISO05_PT10->GetBinContent(SF_ISO05_PT10->FindBin(costh_HX,TMath::Abs(phi_HX),ZNocorr.Pt()));
                  if(useEffSF==14){
                    random_->SetSeed(UInt_t(TMath::Abs(costh_HX)*1e9 + TMath::Abs(phi_HX)*1e6 + TMath::Abs(ZNocorr.Pt())*1e3 + i));
                    eff_ISO_SF += random_->Gaus(0, TMath::Hypot(0.01, SF_ISO05_PT10->GetBinError(SF_ISO05_PT10->FindBin(costh_HX, TMath::Abs(phi_HX),ZNocorr.Pt()))));
                    // cout << "SF_ISO05_PT10->GetBinError(SF_ISO05_PT10->FindBin(costh_HX, TMath::Abs(phi_HX),ZNocorr.Pt()))= " << SF_ISO05_PT10->GetBinError(SF_ISO05_PT10->FindBin(costh_HX, TMath::Abs(phi_HX),ZNocorr.Pt())) << endl;
                  }
                  TRG_TIGHT_ISO_muons_SF  *= eff_ISO_SF;
                }
                if(useEffSF==2 || useEffSF==15 || useEffSF!=5){
                  // === subleading
                  eff_TIGHT_subleading_SF = SF_TIGHT_PT10->GetBinContent(SF_TIGHT_PT10->FindBin(SubLeading_Mu_eta, SubLeading_Mu_pt));
                  if(useEffSF==15){
                    random_->SetSeed(UInt_t(TMath::Abs(costh_HX)*1e9 + TMath::Abs(phi_HX)*1e6 + TMath::Abs(ZNocorr.Pt())*1e3 + i));
                    eff_TIGHT_subleading_SF += random_->Gaus(0,TMath::Hypot(0.01, SF_TIGHT_PT10->GetBinError(SF_TIGHT_PT10->FindBin(SubLeading_Mu_eta, SubLeading_Mu_pt))));
                    // cout << "SF_TIGHT_PT10->GetBinError(SF_TIGHT_PT10->FindBin(SubLeading_Mu_eta, SubLeading_Mu_pt))= " << SF_TIGHT_PT10->GetBinError(SF_TIGHT_PT10->FindBin(SubLeading_Mu_eta, SubLeading_Mu_pt)) << endl;
                  }
                  TRG_TIGHT_ISO_muons_SF  *= eff_TIGHT_subleading_SF;
                }
                if(useEffSF==2 || useEffSF==16 || useEffSF!=6){
                  // === leading
                  eff_TRG_SF = SF_HLT->GetBinContent(SF_HLT->FindBin(WMass::WlikeCharge, Leading_Mu_eta, Leading_Mu_pt));
                  if(useEffSF==16){
                    random_->SetSeed(UInt_t((isChargePos?1:2)*1e9 + TMath::Abs(Leading_Mu_eta)*1e6 + TMath::Abs(Leading_Mu_pt)*1e3 + i));
                    eff_TRG_SF += random_->Gaus(0,TMath::Hypot(0.01,SF_HLT->GetBinError(SF_HLT->FindBin(WMass::WlikeCharge, Leading_Mu_eta, Leading_Mu_pt))));
                    // cout << "SF_HLT->GetBinError(SF_HLT->FindBin(WMass::WlikeCharge, Leading_Mu_eta, Leading_Mu_pt))= " << SF_HLT->GetBinError(SF_HLT->FindBin(WMass::WlikeCharge, Leading_Mu_eta, Leading_Mu_pt)) << endl;
                  }
                  TRG_TIGHT_ISO_muons_SF *= eff_TRG_SF;
                }
              }

              // cout 
              // << "TRG_TIGHT_ISO_muons_SF= " << TRG_TIGHT_ISO_muons_SF
              // << " eff_TIGHT_SF= " << eff_TIGHT_SF
              // << " eff_TIGHT_subleading_SF= " << eff_TIGHT_subleading_SF
              // << " eff_ISO_SF= " << eff_ISO_SF
              // << " eff_TRG_SF= " << eff_TRG_SF
              // << endl;
              
              //------------------------------------------------------
              // Define mu+, mu-, Z
              //------------------------------------------------------
              TLorentzVector mu_trasv,mu_trasvCentral,mu_trasvNoCorr,neutrino_trasv,neutrino_trasvCentral;
              
              Z_met.SetPtEtaPhiM(met_trasv,0,metphi_trasv,0);
              Z_metCentral.SetPtEtaPhiM(met_trasvCentral,0,metphi_trasvCentral,0);
              neutrino_trasv.SetPtEtaPhiM(neutrinoNoCorr.Pt(),0,neutrinoNoCorr.Phi(),0); // correction only one one muon for Wlike
              neutrino_trasvCentral.SetPtEtaPhiM(neutrinoNoCorr.Pt(),0,neutrinoNoCorr.Phi(),0); // correction only one one muon for Wlike
              mu_trasv.SetPtEtaPhiM(muCorr.Pt(),0,muCorr.Phi(),0);
              mu_trasvCentral.SetPtEtaPhiM(muCorrCentral.Pt(),0,muCorrCentral.Phi(),0);
              mu_trasvNoCorr.SetPtEtaPhiM(muNoCorr.Pt(),0,muNoCorr.Phi(),0);

              Wlike_met = neutrino_trasv + Z_met + mu_trasvNoCorr - mu_trasv; // taking into account muon correction into W_met
              Wlike_metCentral = neutrino_trasvCentral + Z_metCentral + mu_trasvNoCorr - mu_trasvCentral;  // taking into account muon correction into W_met
              Wlike_met.SetPtEtaPhiM(Wlike_met.Pt(),0,Wlike_met.Phi(),0); // just to be sure
              Wlike_metCentral.SetPtEtaPhiM(Wlike_metCentral.Pt(),0,Wlike_metCentral.Phi(),0); // just to be sure
              Wlike = mu_trasv + Wlike_met;
              WlikeCentral = mu_trasvCentral + Wlike_metCentral;
            
              // Define MtLin
              double coeff=2;
              double MTFirstOrder_central = common_stuff::getMTFirstOrder(muCorrCentral.Pt(), muCorrCentral.Phi(), Wlike_metCentral.Pt() ,Wlike_metCentral.Phi(), coeff);
              double MTFirstOrder = common_stuff::getMTFirstOrder(muCorr.Pt(), muCorr.Phi(), Wlike_met.Pt(),Wlike_met.Phi(), coeff);
              
              //------------------------------------------------------
              // Variables to fill the histos (pT, mT, MET)
              //------------------------------------------------------
              // double Wlike_var_jacobian[] = {2*muCorr.Pt()/WMass::ZMassCentral_MeV*1e3,Wlike.Mt()/WMass::ZMassCentral_MeV*1e3,2*Wlike_met.Pt()/WMass::ZMassCentral_MeV*1e3,MTFirstOrder/WMass::ZMassCentral_MeV*1e3};
              double Wlike_var_NotScaled[] = {muCorr.Pt(),Wlike.Mt(),Wlike_met.Pt(),MTFirstOrder};
              // cout << "Wlike_var_NotScaled[2]= " << Wlike_var_NotScaled[2] << endl;

              //------------------------------------------------------
              // Variables to define the box cut (pT, mT, MET)
              //------------------------------------------------------
              double Wlike_var_NotScaledCentral[] = {muCorrCentral.Pt(),WlikeCentral.Mt(),Wlike_metCentral.Pt(),MTFirstOrder_central}; // Zcorr would be TEMP !!!!
              
              //------------------------------------------------------
              // cuts for both muons
              //------------------------------------------------------
              if( ZcorrCentral.M()>50
                  && muCorrCentral.Pt()>WMass::sel_xmin[0]*ZWmassRatio 
                  && neutrinoCorrCentral.Pt()>10 
                ){
                if(m==m_start && n==0 && i==0)
                  for(int k=0;k<WMass::NFitVar;k++)
                    common_stuff::plot1D(Form("hWlike%s_%sNonScaled_5_RecoCut_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                      Wlike_var_NotScaled[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
      
                //------------------------------------------------------
                // cut on MET
                //------------------------------------------------------
                if(Wlike_metCentral.Pt()>WMass::sel_xmin[2]*ZWmassRatio){
                  if(m==m_start && n==0 && i==0)
                    for(int k=0;k<WMass::NFitVar;k++)
                      common_stuff::plot1D(Form("hWlike%s_%sNonScaled_6_METCut_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                        Wlike_var_NotScaled[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                  
                  if(controlplots && m==m_start && n==0 && i==0)
                    common_stuff::plot1D(Form("hZ_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                      Zcorr.Pt(),evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 1000,0,250 );

                  //------------------------------------------------------
                  // cut on W recoil (BY DEFAULT IS 15)
                  //------------------------------------------------------
                  if(WlikeCentral.Pt()<WMass::WpTcut*ZWmassRatio
                      && ZcorrCentral.Pt() < ZPt_cut // ADDED DURING PLOTS PRE-UNBLINDING
                     ){
                    if(m==m_start && n==0 && i==0)
                      for(int k=0;k<WMass::NFitVar;k++)
                        common_stuff::plot1D(Form("hWlike%s_%sNonScaled_7_RecoilCut_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                        Wlike_var_NotScaled[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                    
                    //------------------------------------------------------
                    // Apply the "box" cuts on pT, mT, MET -- based on central correction
                    //------------------------------------------------------
                    if(
                        Wlike_var_NotScaledCentral[0] > WMass::sel_xmin[0]*ZWmassRatio && Wlike_var_NotScaledCentral[0] < WMass::sel_xmax[0]*ZWmassRatio  // Pt
                        && Wlike_var_NotScaledCentral[1] > WMass::sel_xmin[1]*ZWmassRatio && Wlike_var_NotScaledCentral[1] < WMass::sel_xmax[1]*ZWmassRatio // Mt
                        && Wlike_var_NotScaledCentral[2] > WMass::sel_xmin[2]*ZWmassRatio && Wlike_var_NotScaledCentral[2] < WMass::sel_xmax[2]*ZWmassRatio // MET
                        // && Wlike_var_NotScaledCentral[3] > WMass::sel_xmin[3]*ZWmassRatio && Wlike_var_NotScaledCentral[3] < WMass::sel_xmax[3]*ZWmassRatio // MtLin
                      ){
                    
                      //------------------------------------------------------
                      // Compute/retrieve PDF weights with LHAPDF or LHE
                      //------------------------------------------------------
                      double lha_weight = 1;
                      double weight_old = 1;
                      #ifdef LHAPDF_ON
                        if(parton1_x>1e-7 && parton2_x>1e-7 && parton1_x<1 && parton2_x<1)
                          if (!sampleName.Contains("DATA"))
                            weight_old = LHAPDF::xfx(1,parton1_x,scalePDF,parton1_pdgId)*LHAPDF::xfx(1,parton2_x,scalePDF,parton2_pdgId);
                        if(m==m_start && n==0 && i==0 && controlplots){
                          common_stuff::plot1D("hPDF_x1",TMath::Log10(parton1_x), 1, h_1d, 1000,-4,0 );
                          common_stuff::plot1D("hPDF_x1unweighted",TMath::Log10(parton1_x), 1/weight_old, h_1d, 1000,-4,0 );
                          common_stuff::plot1D("hPDF_x2",TMath::Log10(parton2_x), 1, h_1d, 1000,-4,0 );
                          common_stuff::plot1D("hPDF_x2unweighted",TMath::Log10(parton2_x), 1/weight_old, h_1d, 1000,-4,0 );
                        }
                      #endif
                      
                      for(int h=0; h<WMass::PDF_members; h++){
                        if( !contains_LHE_weights || (!sampleName.Contains("DATA") && WMass::PDF_sets>0 && WMass::PDF_sets!=generated_PDF_set && WMass::PDF_members!=generated_PDF_member)){
                          double weight_new = 1;
                          #ifdef LHAPDF_ON
                            LHAPDF::usePDFMember(0,h);
                            if(parton1_x>1e-7 && parton2_x>1e-7 && parton1_x<1 && parton2_x<1)
                              weight_new = (LHAPDF::xfx(0,parton1_x,scalePDF,parton1_pdgId)*LHAPDF::xfx(0,parton2_x,scalePDF,parton2_pdgId));
                          #endif
                          lha_weight = weight_new/weight_old;
                          if(m==m_start && n==0 && i==0 && controlplots)
                            common_stuff::plot1D(Form("hPDF_weights_%d_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h),
                                          lha_weight, 1, h_1d, 1000,0,2
                                        );
                        } else if(contains_LHE_weights){
                          lha_weight = LHE_weight[PDF_reweighting_central_Index+h];
                        }
                        
                        // WEIGHT DEFINITION
                        double weight = evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight;

                        //------------------------------------------------------
                        // start loop on mass hypotheses
                        //------------------------------------------------------
                        for(int j=0; j<2*WMass::WMassNSteps+1; j++){

                          if(!isPowOrMad && WMass::WMassNSteps!=j) continue;
                          int jZmass_MeV = WMass::Zmass_values_array[j];
                          double iZmass_GeV = WMass::Zmass_values_array[j]/1e3;
                          
                          //------------------------------------------------------
                          // compute weight for mass hypotheses wiht Breit-Wigner reweighting or LHE
                          //------------------------------------------------------
                          double weight_i=1; // bweight_i=1,lheweight_i=1;
                          if(useGenVar){
                            if(!contains_LHE_weights){
                              double gamma=2.141; /*HARD CODED TO PDG VALUE*/
                              weight_i = common_stuff::BWweight(ZGen_mass, iZmass_GeV, gen_mass_value_MeV, gamma);
                            }
                            else{
                              weight_i = LHE_weight [ (WMass::LHE_mass_central_index + ( -WMass::WMassNSteps + j)*WMass::WMassSkipNSteps) ];
                            }
                          }
                          double weight_mass = weight * weight_i;

                          
                          //------------------------------------------------------
                          // "MONEY" PLOTS OF FIT VARIABLES WITHIN THE FIT RANGE
                          //------------------------------------------------------

                          for(int k=0;k<WMass::NFitVar;k++){
                            common_stuff::plot1D(Form("hWlike%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                                  Wlike_var_NotScaled[k], weight_mass, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                            
                            // 2D plot obs_i vs obs_j (no MtLin)
                            if (false) {
                              for(int k2=k+1;k2<WMass::NFitVar;k2++){
                                // cout << "k= " << k << " k2= " << k2 << endl;
                                common_stuff::plot2D(Form("hWlike%s_%svs%s_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::FitVar_str[k2].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                       Wlike_var_NotScaled[k2],Wlike_var_NotScaled[k], weight_mass,
                                       h_2d, 50, WMass::fit_xmin[k2]*ZWmassRatio, WMass::fit_xmax[k2]*ZWmassRatio,
                                       50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                              }
                            }
                          }

                          if (false) {
                            // 3D plot
                            common_stuff::plot3D(Form("hWlike%s_%svs%svs%s_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[2].Data(),WMass::FitVar_str[1].Data(),WMass::FitVar_str[0].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                               Wlike_var_NotScaled[0],Wlike_var_NotScaled[1],Wlike_var_NotScaled[2], weight_mass,
                               h_3d, 50, WMass::fit_xmin[0]*ZWmassRatio, WMass::fit_xmax[0]*ZWmassRatio,
                               50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio,
                               50, WMass::fit_xmin[2]*ZWmassRatio, WMass::fit_xmax[2]*ZWmassRatio );
                          }
                          if(doRecoilMassVariations){
                            common_stuff::plot1D(Form("hWlike%s_RecoilNonScaled_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                              Wlike.Pt(), weight_mass, h_1d, 50, 0, 18 );
                          }

                        }
                      
                        //------------------------------------------------------------------------------------------------
                        // EXTRA PLOTS - these are inclusive distributions used to derive and verify weights
                        //------------------------------------------------------------------------------------------------

                        if(m==m_start && n==0 && (controlplots || preUnblinding)){

                          // Leptons Efficiencies
                          common_stuff::plot1D(Form("h_eff_TIGHT_SF%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                               eff_TIGHT_SF, weight, h_1d, 200, -0.5, 1.5 );

                          common_stuff::plot1D(Form("h_eff_ISO_SF%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                               eff_ISO_SF, weight, h_1d, 200, -0.5, 1.5 );

                          common_stuff::plot1D(Form("h_eff_TIGHT_subleading_SF%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                               eff_TIGHT_subleading_SF, weight, h_1d, 200, -0.5, 1.5 );

                          common_stuff::plot1D(Form("h_eff_TRG_SF%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                               eff_TRG_SF, weight, h_1d, 200, -0.5, 1.5 );

                          // Lepton rapidity
                          common_stuff::plot1D(Form("hWlike%s_MuEta_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                            muCorr.Rapidity(), weight, h_1d, 60, -2.4, 2.4 );
                          common_stuff::plot1D(Form("hWlike%s_NuEta_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                            neutrinoCorr.Rapidity(), weight, h_1d, 60, -2.4, 2.4 );
                          
                          // Boson Kinematics Zpt, Zmass, Zrecoil, Zrapidity
                          common_stuff::plot1D(Form("hWlike%s_ZpT_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                            Zcorr.Pt(), weight, h_1d, 80, 0, 40 );
                          common_stuff::plot1D(Form("hWlike%s_Zmass_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                            Zcorr.M(), weight, h_1d, 80, 80, 100 );

                          common_stuff::plot1D(Form("hWlike%s_Recoil_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                            Wlike.Pt(), weight, h_1d, 40, 0, 20 );

                          common_stuff::plot1D(Form("hWlike%s_Zrap_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                            Zcorr.Rapidity(), weight, h_1d, 80, -2, 2 );


                          if(controlplots) {
                            common_stuff::plot2D(Form("hWlike%s_MtLinVsGenZPt_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                               ZGen_pt,Wlike_var_NotScaled[3], weight,
                               h_2d, 40, 0,20,
                               50, WMass::fit_xmin[3]*ZWmassRatio, WMass::fit_xmax[3]*ZWmassRatio );
                            common_stuff::plot2D(Form("hWlike%s_MtVsGenZPt_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                               ZGen_pt,Wlike_var_NotScaled[1], weight,
                               h_2d, 40, 0,20,
                               50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio );
                          }

                          // Boson polarization
                          common_stuff::plot2D(Form("hWlike%s_Zrap_vs_costh_CS_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                               costh_CS,TMath::Abs(Zcorr.Rapidity()), weight,
                               h_2d, 40,-1,1,
                               9,0,1.8 );

                          common_stuff::plot2D(Form("hWlike%s_phi_CS_vs_costh_CS_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                               costh_CS,TMath::Abs(phi_CS), weight,
                               h_2d, 20,-1,1,
                               8,0,TMath::Pi() );

                          common_stuff::plot1D(Form("hWlike%s_costh_CS_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                               costh_CS, weight,
                               h_1d, 40,-1,1 );

                          common_stuff::plot1D(Form("hWlike%s_phi_CS_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                               TMath::Abs(phi_CS), weight,
                               h_1d, 16, 0, TMath::Pi() );
                          
                          if(polarization_checks){
                            
                            common_stuff::plot1D(Form("hWlike%s_Eta_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d_all",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                                  muCorr.Pt(), weight, h_1d, 50, -2.5, 2.5 );
                            
                            if(TMath::Abs(costh_CS)<0.2){
                              
                              common_stuff::plot1D(Form("hWlike%s_costh_CS_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d_central",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                   costh_CS, weight,
                                   h_1d, 40,-1,1 );
                              common_stuff::plot1D(Form("hWlike%s_phi_CS_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d_central",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                   TMath::Abs(phi_CS), weight,
                                   h_1d, 16, 0, TMath::Pi() );

                                common_stuff::plot1D(Form("hWlike%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d_central",WCharge_str.Data(),WMass::FitVar_str[0].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                                      Wlike_var_NotScaled[0], weight, h_1d, 50, WMass::fit_xmin[0]*ZWmassRatio, WMass::fit_xmax[0]*ZWmassRatio );
                                common_stuff::plot1D(Form("hWlike%s_Eta_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d_central",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                                      Wlike_var_NotScaled[0], weight, h_1d, 50, -2.5, 2.5 );
                                
                            }
                          }

                          if(controlplots || polarization_checks) {
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
                              common_stuff::plot1D(Form("costh_CS_rapbin%d_ptbin%d_Wlike%s_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",i_rapbin,i_ptbin,WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                       costh_CS, weight, h_1d, 12, -1, 1);
                              common_stuff::plot1D(Form("phi_CS_rapbin%d_ptbin%d_Wlike%s_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",i_rapbin,i_ptbin,WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                       TMath::Abs(phi_CS), weight, h_1d, 12, 0, TMath::Pi());
                            }
                          }
                        }

                        //------------------------------------------------------------------------------------------------
                        // BELOW PLOTS for CLOSURE TEST - Various binned plots - for recoil plots - Zcentral info to avoid randomization
                        //------------------------------------------------------------------------------------------------

			//i loop on toys, n loop on kalman
                        if(Wlike_met.Pt()>0 && m==m_start && n==0 && i==0 && testRecoilOnData) {

			  string tag_zPtcut;
			  if ( ZcorrCentral.Pt()<2 ) tag_zPtcut = "_Zpt02";
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
                          
			  /*
			  bool plot_vtx_binned_Wlike_var_NotScaled = true;
			  if(plot_vtx_binned_Wlike_var_NotScaled){
			    for(int k=0;k<WMass::NFitVar;k++)
			      common_stuff::plot1D(Form("hWlike%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s_eta%s_%d%s",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV,tag_VTX.c_str()),
						   Wlike_var_NotScaled[k], weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
			  }
			  */
                          

			  double u1_scale=0;
			  plotVariables( Z_met, VisPt,  ZcorrCentral, u1_scale, "closure", tag_zPtcut.c_str(), mettype.c_str() , false, h_1d, h_2d, weight, WMass::WMassNSteps, WMass::ZMassCentral_MeV);
			  plotVariables( Z_met, VisPt,  ZcorrCentral, u1_scale, "closure", tag_VTX.c_str(), mettype.c_str() , false, h_1d, h_2d, weight, WMass::WMassNSteps, WMass::ZMassCentral_MeV);
			  plotVariables( Z_met, VisPt,  ZcorrCentral, u1_scale, "closure", tag_y.c_str(), mettype.c_str() , false, h_1d, h_2d, weight, WMass::WMassNSteps , WMass::ZMassCentral_MeV);
			}

			//i loop on toys, n loop on kalman
			if(testRecoilOnData && m==m_start && n==0 && i==0){

			    double u1Z_recoil=0;
			    double u2Z_recoil=0;
			    double u1lep_recoil=0;
			    double u2lep_recoil=0;

			    common_stuff::calculateU1U2(met_trasv, metphi_trasv,  ZcorrCentral.Pt(), ZcorrCentral.Phi(),
							ZNocorr.Pt(), ZNocorr.Phi(),  u1Z_recoil, u2Z_recoil);

			    common_stuff::calculateU1U2(met_trasv, metphi_trasv,  muCorrCentral.Pt(), muCorrCentral.Phi(),
							ZNocorr.Pt(), ZNocorr.Phi(),  u1lep_recoil, u2lep_recoil);

			    common_stuff::plot1D(Form("hWlike%s_U1Z_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
						 u1Z_recoil, weight, h_1d, 40, -20., 20. );

			    common_stuff::plot1D(Form("hWlike%s_U2Z_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
						 u2Z_recoil, weight, h_1d, 40, -20., 20. );

			    common_stuff::plot1D(Form("hWlike%s_U1lep_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
						 u1lep_recoil, weight, h_1d, 40, -20., 20. );

			    common_stuff::plot1D(Form("hWlike%s_U2lep_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
						 u2lep_recoil, weight, h_1d, 40, -20., 20. );

			    common_stuff::plot1D(Form("hWlike%s_RecoilPhi_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
						 Wlike.Phi(), weight, h_1d, 18,-TMath::Pi(),TMath::Pi() );

			  }

                        //---------------------------------------------------------------------
                        // Recoil plots: u1, u2, u1vsZpt, u2vsZpt, u1vsZptvsZrap, u2vsZptvsZrap (for recoil plots - Zcentral info to avoid randomization)
                        //---------------------------------------------------------------------
                        if(correctToMadgraph || controlplots){

                          common_stuff::calculateU1U2(met_trasv, metphi_trasv,  ZGen_pt, ZGen_phi,
                            ZNocorr.Pt(), ZNocorr.Phi(),  u1_recoil, u2_recoil);

                          double u_recoil = sqrt(u2_recoil*u2_recoil+u1_recoil*u1_recoil);

                          common_stuff::plot1D(Form("hWlike%s_u1_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                            u1_recoil, weight, h_1d, 60, -20, 20 );
                          common_stuff::plot1D(Form("hWlike%s_u2_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                            u2_recoil, weight, h_1d, 60, -20, 20 );
                          common_stuff::plot1D(Form("hWlike%s_u_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                            u_recoil, weight, h_1d, 60, +00, 20 );

                          common_stuff::plot2D(Form("hWlike%s_u1vsZpt_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                            ZcorrCentral.Pt(), u1_recoil, weight, h_2d, 60, 0, ZPt_cut, 60, -20, 20 );
                          common_stuff::plot2D(Form("hWlike%s_u2vsZpt_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                            ZcorrCentral.Pt(), u2_recoil, weight, h_2d, 60, 0, ZPt_cut, 60, -20, 20 );
                          common_stuff::plot2D(Form("hWlike%s_uvsZpt_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                            ZcorrCentral.Pt(),  u_recoil, weight, h_2d, 60, 0, ZPt_cut, 60, +00, 20 );
                        }

                        if (controlplots) {
                          common_stuff::plot2D(Form("hWlike%s_u1vsZrap_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                            ZGen_status3.Rapidity(), u1_recoil, weight, h_2d, 60, -4, +4, 60, -20, 20 );
                          common_stuff::plot2D(Form("hWlike%s_u2vsZrap_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                            ZGen_status3.Rapidity(), u2_recoil, weight, h_2d, 60, -4, +4, 60, -20, 20 );

                          common_stuff::plot3D(Form("hWlike%s_u1vsZptvsZrap_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                            ZcorrCentral.Pt(), ZGen_status3.Rapidity(), u1_recoil, weight, h_3d, 60, 0, ZPt_cut, 60, -4, +4, 60, -20, 20 );
                          common_stuff::plot3D(Form("hWlike%s_u2vsZptvsZrap_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                            ZcorrCentral.Pt(), ZGen_status3.Rapidity(), u2_recoil, weight, h_3d, 60, 0, ZPt_cut, 60, -4, +4, 60, -20, 20 );
                        }

                        if (controlplots) {
                          //------------------------------------------------------------------------------------------------
                          // PLOTS FOR GIGI's TEST see 11 apr 2014 (CMG presentations)
                          //------------------------------------------------------------------------------------------------
                          //      cout << "filling control plot RecoilVar=" << RecoilVar_str.Data() << endl;
                          common_stuff::plot1D(Form("deltaMT_Wlike%s_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                  Wlike.Mt() - WlikeCentral.Mt(), weight, h_1d, 200, -0.1, 0.1);

                          common_stuff::plot1D(Form("deltaMET_Wlike%s_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV),
                                  Wlike_met.Pt() - Wlike_metCentral.Pt(), weight, h_1d, 200, -0.1, 0.1);
                        }

                      } // end loop PDF
                      
                      //------------------------------------------------------------------------------------------------
                      // control plots for different etas but only for central W mass
                      //------------------------------------------------------------------------------------------------ 
                      if(controlplots && m==m_start && n==0 && i==0) fillControlPlots(Zcorr, Z_met, muPosCorr, muNegCorr, h_1d, h_2d, evt_weight*TRG_TIGHT_ISO_muons_SF, WMass::ZMassCentral_MeV, eta_str, WMass::nSigOrQCD_str[0],Form("Wlike%s_8_JetCut",WCharge_str.Data()));

                    } // end if for box cuts
                  } // end if for recoil
                } // end if for MET cuts
              } // end if for muon cuts
            } // end efficiency toys
          } // end KalmanVars loop
        } // end RecoilCorr params loop
      } // end if for good reco event
    } // end if for dummy signal/fake separation
  } // end event loop

  TFile*fout = new TFile(Form("%s/Zanalysis%s.root",outputdir.Data(),chunk_str.Data()),"RECREATE");
  fout->cd();

  hZPtSF->Write();

  if(!isPowOrMad){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons); eta_str.ReplaceAll(".","p");
    for(int i=0; i<max(1, WMass::efficiency_toys); i++){
      TString effToy_str = "";
      if(WMass::efficiency_toys>0) effToy_str = Form("_effToy%d", i);
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        if(WMass::WMassNSteps!=j){
          // int jZmass_MeV = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMass_SkipNSteps);
          int jZmass_MeV = WMass::Zmass_values_array[j];
          for(int k=0;k<WMass::NFitVar;k++){
            for(int h=0; h<WMass::PDF_members; h++){
              for(int m=m_start; m<m_end; m++){
                TString RecoilVar_str = "";
                if(RecoilCorrVarDiagoParU1orU2fromDATAorMC>0) RecoilVar_str = Form("_RecoilCorrVar%d",m);

                for(int n=0; n<WMass::KalmanNvariations; n++){
                
                  TString KalmanVars_str = "";
                  if(WMass::KalmanNvariations>1) KalmanVars_str = Form("_KalmanVar%d",n);
                  
                    common_stuff::cloneHisto1D(Form("hWlike%s_%sScaled_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV), 
                                                Form("hWlike%s_%sScaled_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV), 
                                                h_1d);

                    common_stuff::cloneHisto1D(Form("hWlike%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV), 
                                                Form("hWlike%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV), 
                                                h_1d);
                  for(int k2=1;k2<WMass::NFitVar;k2++){
                    if(k==k2 || k==3) continue;
                    
                    common_stuff::cloneHisto2D(Form("hWlike%s_%svs%s_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::FitVar_str[k2].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV), 
                                               Form("hWlike%s_%svs%s_8_JetCut_pdf%d-%d%s%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::FitVar_str[k2].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,effToy_str.Data(),RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV), 
                                               h_2d);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  
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
  
  // // M. Guzzi
  // // char hname0[]="phistar", vexp0[]="tan((3.1415- D_phi)/2.)*sqrt(1.-pow(tanh((y_d2-y_d1)/2.0),2))";
  // double D_phi = muSubLeadingPtCorr.Phi()-muLeadingPtCorr.Phi();
  // if ( D_phi > TMath::Pi() ) {
    // D_phi -= 2.0*TMath::Pi();
  // } else if ( D_phi <= -TMath::Pi() ) {
    // D_phi += 2.0*TMath::Pi();
  // }
  // D_phi=TMath::Abs(D_phi);

  // if(D_phi>TMath::Pi()) D_phi-=TMath::Pi();
  // phistar=TMath::Tan((TMath::Pi()- D_phi)/2.)*TMath::Sqrt(1.-TMath::Power(TMath::TanH(TMath::Abs(muSubLeadingPtCorr.Eta()-muLeadingPtCorr.Eta())/2.0),2));
  // logphistar=TMath::Log10(phistar);

}
