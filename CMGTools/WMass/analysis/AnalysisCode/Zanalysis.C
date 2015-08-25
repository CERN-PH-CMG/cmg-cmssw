// UNCOMMENT TO USE PDF REWEIGHTING
//#define LHAPDF_ON

#ifdef LHAPDF_ON
  #include "LHAPDF/LHAPDF.h"
#endif 

#define Zanalysis_cxx
#include "Zanalysis.h"
#include "common.h"
// #include "rochcor_42X.h"
//#include "common_stuff.h"
//#include "rochcor_44X_v3.h"
#include "KalmanCalibrator.h"
#include "KalmanCalibratorParam.h"
#include "MuScleFitCorrector.h"
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
#include <TGraphAsymmErrors.h>
#include <ctime>
#include <string>

TLorentzVector muPosNoCorr,muNegNoCorr,ZNocorr;
TLorentzVector muPosGen_status3,muNegGen_status3,ZGen_status3;
TLorentzVector muPosCorr,muNegCorr,muPosCorrCentral,muNegCorrCentral;
TLorentzVector Zcorr, ZcorrCentral; //TLorentzVector of the reconstructed muon
TLorentzVector Z_met,Z_metCentral,Wlike_met,Wlike_metCentral;
TLorentzVector Wlike,WlikeCentral;

HTransformToHelicityFrame *GoToHXframe;
double costh_HX = -1e10, phi_HX = -1e10;
double costh_HX_gen = -1e10, phi_HX_gen = -1e10;
double costh_HX_gen_pietro = -1e10, phi_HX_gen_pietro = -1e10;
HTransformToCS *GoToCSframe;
double costh_CS = -1e10, phi_CS = -1e10;
double costh_CS_gen_pietro = -1e10, phi_CS_gen_pietro = -1e10;
double costh_CS_gen = -1e10, phi_CS_gen = -1e10;

void Zanalysis::Loop(int chunk, int Entry_ini, int Entry_fin, int IS_MC_CLOSURE_TEST, int isMCorDATA, TString outputdir, /* unused*/ int buildTemplates, int useMomentumCorr, int varyMuonCorrNsigma, int useEffSF, int usePtSF, int useVtxSF, int controlplots, TString sampleName, int generated_PDF_set, int generated_PDF_member, int contains_PDF_reweight, int usePhiMETCorr, int useRecoilCorr, /* unused*/ int RecoilCorrNonClosure, int RecoilCorrVarDiagoParSigmas, int RecoilCorrVarDiagoParU1orU2fromDATAorMC, int use_PForNoPUorTKmet, int use_syst_ewk_Alcaraz, int gen_mass_value_MeV, int contains_LHE_weights)
{

  if (fChain == 0) return;

  using namespace std;

  // maps needed for common_stuff histos
  map<string, TH1D*> h_1d;
  map<string, TH2D*> h_2d;
  map<string, TH3D*> h_3d;

  cout << "generated_PDF_set= "<<generated_PDF_set
       << " generated_PDF_member= " << generated_PDF_member
       << " contains_PDF_reweight= " << contains_PDF_reweight
       << " WMass::WlikeCharge= " << WMass::WlikeCharge
       << endl;

  TString chunk_str = chunk>0? Form("_chunk%d",chunk) : "";
  // ofstream outTXTfile;
  // outTXTfile.open(Form("%s/Zanalysis_EVlog%s.log",outputdir.Data(),chunk_str.Data()));
  // if(!outputdir.Contains("../")) outputdir = "../"+outputdir;
  cout << "output filename= " << Form("%s/Zanalysis%s.root",outputdir.Data(),chunk_str.Data()) << endl;

  //------------------------------------------------------
  // Set variables for charge
  //------------------------------------------------------
  
  bool isChargePos = WMass::WlikeCharge==1;
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
  // TFile*feffSF = new TFile(Form("../Zanalysis.root",outputdir.Data()),"RECREATE");
  
  //------------------------------------------------------
  // retrieve efficiencies SF
  //------------------------------------------------------
  // TGraphAsymmErrors*hEffSF_MuId_eta_2011[2],*hEffSF_Iso_eta_2011[2],*hEffSF_HLT_eta_2011/* ,*hEffSF_Iso_vtx_2011A,*hEffSF_Iso_vtx_2011B*/;
  TH3F *SF_HLT;
  TH2F *SF_TIGHT_ISO;
  TH2F *SF_TIGHT_PT10;
  TH3F *SF_ISO05_PT10;
  
  if(useEffSF==2 && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    
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
    // finPileupSF = new TFile(Form("../utils/pileup_reweighting_%s.root",vtx_str.Data())); // used only to build templates
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
  TH1D*hZmassSF;

  if(use_syst_ewk_Alcaraz>-1 && use_syst_ewk_Alcaraz!=100){
    TFile* finZmassSF = new TFile("../utils/fout_invariant_mass_qed_higstat.root"); // used only to build templates
    if(!finZmassSF){
      cout << "file " << Form("../utils/fout_invariant_mass_qed_higstat.root") << " is missing, impossible to retrieve invariant mass SF (for EWK)" << endl;
      return;
    }
    if(use_syst_ewk_Alcaraz==0){
      hZmassSF=(TH1D*)finZmassSF->Get("ewk_p8std_div_ewk_p8bad"); hZmassSF->Sumw2();
    }else if(use_syst_ewk_Alcaraz==1){
      hZmassSF=(TH1D*)finZmassSF->Get("ewk_p8photos_div_ewk_p8std"); hZmassSF->Sumw2();
    }else if(use_syst_ewk_Alcaraz==2){
      hZmassSF=(TH1D*)finZmassSF->Get("noewk_p8std_div_ewk_p8std"); hZmassSF->Sumw2();
    }else if(use_syst_ewk_Alcaraz==3){
      hZmassSF=(TH1D*)finZmassSF->Get("ewk_p8photos_div_ewk_p8bad"); hZmassSF->Sumw2();
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
  // TH1D*hZPtSFbase;
  // if(usePtSF!=-1 && usePtSF!=0 && usePtSF!=1 &&usePtSF!=2 && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){
    // cout << "REWEIGHTING Z PT" << endl;
    // // TFile* finZPtSF = new TFile(Form("../utils/Zpt_reweighting.root")); // used only to build templates
    // // cout << Form("hzpt_atlas_mc_ren%d_fac%d_0p5GeVsteps",/* usePtSF<1000?30: */1030,/* usePtSF<1000?30: */1030) << endl;
    // TFile* finZPtSFbase = new TFile(Form("/afs/cern.ch/work/p/perrozzi/private/git/CMSSW_5_3_22/src/BosonPT/serious_stuff/resbos/dyres/ZNLOpt_scales_%d.root",/* usePtSF<1000?30: */1030)); // used only to build templates
    // // cout << Form("hzpt_atlas_mc_ren%d_fac%d_0p5GeVsteps",usePtSF,usePtSF) << endl;
    // TFile* finZPtSF = new TFile(Form("/afs/cern.ch/work/p/perrozzi/private/git/CMSSW_5_3_22/src/BosonPT/serious_stuff/resbos/dyres/ZNLOpt_scales_%d.root",usePtSF)); // used only to build templates
    // // finZPtSF->ls();
    // if(!finZPtSF){
      // cout << "file " << Form("../utils/Zpt_reweighting.root") << " is missing, impossible to retrieve ZPt reweighting factors" << endl;
      // return;
    // }else{
      // // hZPtSF=(TH1D*)finZPtSF->Get("hZ_pt_Sig_eta0p6");
      // hZPtSFbase=(TH1D*)finZPtSFbase->Get(Form("hzpt_atlas_mc_ren%d_fac%d",/* usePtSF<1000?30: */1030,/* usePtSF<1000?30: */1030)); hZPtSFbase->Sumw2();
      // hZPtSF=(TH1D*)finZPtSF->Get(Form("hzpt_atlas_mc_ren%d_fac%d",usePtSF,usePtSF)); hZPtSF->Sumw2();
      // hZPtSF->Divide(hZPtSFbase);
    // }
  // } else 
  if(usePtSF==0){
    TFile* finZPtSF = new TFile("../utils/fptout_ZdataMC.root"); // used only to build templates
    // hZPtSF=(TH1D*)finZPtSF->Get(Form("hWlike%s_ZpT_8_JetCut_pdf229800-0_eta0p9_91188_smooth", WCharge_str.Data())); hZPtSF->Sumw2();
    // TEMP !!! WE HAVE ONLY THE POS HISTO, BUT SHOULD NOT MATTER MUCH
    hZPtSF=(TH1D*)finZPtSF->Get("hWlikePos_ZpT_8_JetCut_pdf229800-0_eta0p9_91188_smooth"); hZPtSF->Sumw2();
    
  } else hZPtSF = new TH1D("hZPtSF","hZPtSF",10,0,1);

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
  TString MuscleCard = (IS_MC_CLOSURE_TEST || isMCorDATA==0) ? "MuScleFit_2011_MC_44X" : "MuScleFit_2011_DATA_44X";
  TString fitParametersFile = MuscleCard+".txt";
  MuScleFitCorrector *corrector;
  if(useMomentumCorr==2){
    cout << "using MuscleFit card " << fitParametersFile << endl;
    corrector = new MuScleFitCorrector(fitParametersFile);
  }
  KalmanCalibrator *corrector_Kalman;
  if(useMomentumCorr==3){
    cout << "using Kalman Calibrator" << endl;
    corrector_Kalman = new KalmanCalibrator(isMCorDATA==0?false:true); // True for data , //False for MC
  }
  KalmanCalibratorParam *corrector_KalmanParam;
  if(useMomentumCorr==4){
    cout << "using Kalman Calibrator Param" << endl;
    corrector_KalmanParam = new KalmanCalibratorParam(isMCorDATA==0?false:true); // True for data , //False for MC
  }

  //------------------------------------------------------
  // Initialize recoil corrections
  //------------------------------------------------------

  int m_start = WMass::RecoilCorrIniVarDiagoParU1orU2fromDATAorMC_[RecoilCorrVarDiagoParU1orU2fromDATAorMC];
  int m_end = WMass::RecoilCorrNVarDiagoParU1orU2fromDATAorMC_[RecoilCorrVarDiagoParU1orU2fromDATAorMC];

  // int use_PForNoPUorTKmet = 0; // 0:PF, 1:NOPU, 2:TK // TO BE CHANGED BY HAND FOR THE MOMENT!!!
  // bool use_InclusiveNVTX_RecoilCorr = true; // TO BE CHANGED BY HAND FOR THE MOMENT!!!
  // TString InclusiveNVTXSuffix="";
  // if(use_InclusiveNVTX_RecoilCorr) InclusiveNVTXSuffix="_inclusiveNvtx";

  TString metSuffix;
  if(use_PForNoPUorTKmet==0) metSuffix="_pfmet"; // this doesn't work since we do no have the pfmet as of JAN25
  if(use_PForNoPUorTKmet==1) metSuffix="_pfnoPU";
  if(use_PForNoPUorTKmet==2) metSuffix="_tkmet";
  
  TString generatorSuffix="_powheg";
  if (sampleName.Contains("DYJetsMadSig"))
    generatorSuffix="_madgraph";

  RecoilCorrector*  correctorRecoil_Z; // TYPE2
  if(useRecoilCorr>0){
    TString model_name[2]={"fitresult_Add","fitresult_model2D"};
    
    // TKMET type2
    int model_name_idx=0;
    string fileCorrectTo = /*POW */ Form("../RecoilCode/AUG19/recoilfit_AUG19_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_powheg.root");
    string fileZmmMC =     /*POW */ Form("../RecoilCode/AUG19/recoilfit_AUG19_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_powheg.root");
    string fileZmmData =   /*DATA*/ Form("../RecoilCode/AUG19/recoilfit_AUG19_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_triGauss_halfStat_UNBINNED_3G_53X.root");
    // string fileZmmData =   /*POW */ Form("../RecoilCode/AUG19/recoilfit_AUG19_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_powheg.root");
    // string fileZmmData =   /*MAD */ Form("../RecoilCode/AUG19/recoilfit_AUG19_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root");
    
    cout << "INITIALIZING RECOIL MC TARGET FILE" << endl;
    correctorRecoil_Z = new RecoilCorrector(fileCorrectTo.c_str(),123456,model_name[0],"../RecoilCode/MAY25/nonClosureMAY25.root");
    cout << "INITIALIZING RECOIL Z DATA FILE" << endl;
    correctorRecoil_Z->addDataFile(fileZmmData.c_str(),model_name[model_name_idx]);
    cout << "INITIALIZING RECOIL Z MC FILE" << endl;
    correctorRecoil_Z->addMCFile(fileZmmMC.c_str(),model_name[0]);
  }
  bool doSingleGauss=false;

  double ZWmassRatio = ((double)WMass::ZMassCentral_MeV)/((double)WMass::WMassCentral_MeV);
  
  GoToHXframe = new HTransformToHelicityFrame();
  GoToCSframe = new HTransformToCS();
  
  //------------------------------------------------------
  // start the event loop
  //------------------------------------------------------
  int cout_freq=TMath::Min(1+(nentries-first_entry)/10,(Long64_t) 25000);
  // cout_freq=1;
  // nentries=100;
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
      // outTXTfile << dt << "\t - \t Analyzed entry "<<jentry<<"/"<<nentries<<endl;
      cout << dt << "\t - \t Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    }
    
  
    //------------------------------------------------------------
    // Reject even events in DATA, if not controlplots, since where used for Recoil Corrections
    //------------------------------------------------------------
    if(!controlplots && !(jentry%2) && sampleName.Contains("DATA")) continue;

    //------------------------------------------------------
    // initilize the mu+, mu-, met event variables
    //------------------------------------------------------

    bool first_time_in_the_event = true;
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

    // if(!(IS_MC_CLOSURE_TEST || isMCorDATA==0) && run>175832) continue; // TO TEST ROCHESTER CORRECTIONS ONLY ON RUN2011A
    // if(!(IS_MC_CLOSURE_TEST || isMCorDATA==0) && run<175832) continue; // TO TEST ROCHESTER CORRECTIONS ONLY ON RUN2011B
    
    //------------------------------------------------------
    // compute event weight
    //------------------------------------------------------

    //---------------- MUON weight
    // int runopt = r->Rndm()<0.457451 ? 0 : 1;
    double TRG_TIGHT_ISO_muons_SF = 1;
    // double MuNeg_tight_muon_SF = 1;

    costh_HX = -1e10;     phi_HX = -1e10;
    costh_HX_gen = -1e10; phi_HX_gen = -1e10;
    costh_CS = -1e10;     phi_CS = -1e10;
    costh_CS_gen = -1e10; phi_CS_gen = -1e10;

    if(MuPosGen_pt>0 && MuNegGen_pt>0) ComputeHXVarAndPhiStarEta(muPosGen_status3,muNegGen_status3,true);
    if(MuPos_pt>0 && MuNeg_pt>0) ComputeHXVarAndPhiStarEta(muPosNoCorr,muNegNoCorr,false);

    if(useEffSF==2 && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
      // TRG_TIGHT_ISO_muons_SF = hEffSF_MuId_eta_2011[runopt]->Eval(MuPos_eta)*hEffSF_Iso_eta_2011[runopt]->Eval(MuPos_eta)*hEffSF_HLT_eta_2011->Eval(MuPos_eta);
      // MuNeg_tight_muon_SF = hEffSF_MuId_eta_2011[runopt]->Eval(MuNeg_eta)*hEffSF_Iso_eta_2011[runopt]->Eval(MuNeg_eta)*hEffSF_HLT_eta_2011->Eval(MuPos_eta);
      TRG_TIGHT_ISO_muons_SF = SF_HLT->GetBinContent(SF_HLT->FindBin(1,MuPos_eta,MuPos_pt))
                           *SF_TIGHT_ISO->GetBinContent(SF_TIGHT_ISO->FindBin(MuPos_eta,MuPos_pt))
                           *SF_TIGHT_PT10->GetBinContent(SF_TIGHT_PT10->FindBin(MuNeg_eta,MuNeg_pt))
                           *SF_ISO05_PT10->GetBinContent(SF_ISO05_PT10->FindBin(costh_HX,TMath::Abs(phi_HX),ZNocorr.Pt()));
    }
    // cout << "TRG_TIGHT_ISO_muons_SF= " << TRG_TIGHT_ISO_muons_SF << endl;

    //---------------- LUMI weight
    double evt_weight_original = lumi_scaling;
    // cout << "evt_weight_original " << evt_weight_original << endl;

    //---------------- VTX weight
    if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && npu>0)
      evt_weight_original=lumi_scaling*hPileupSF->GetBinContent(hPileupSF->GetXaxis()->FindBin(npu));

    if((IS_MC_CLOSURE_TEST || isMCorDATA==0) && controlplots) 
      common_stuff::plot1D("hPileUp_Fall11",npu, 1, h_1d, 50,0,50);

    //---------------- Angular coefficients weight
    if(false && sampleName.Contains("DYJetsPow")){
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
      evt_weight_original*= AngCoef_sf!=0 ? AngCoef_sf : 1;
      // hZmassSF->Print();
    }
    
    //---------------- Invariant Mass weight
    if(use_syst_ewk_Alcaraz>-1 && use_syst_ewk_Alcaraz!=100 && sampleName.Contains("DYJetsPow")){
      evt_weight_original*=ZGen_PostFSR_mass>50&&ZGen_PostFSR_mass<200?hZmassSF->Interpolate(ZGen_PostFSR_mass):1;
      // hZmassSF->Print();
      // cout << "mass= " << ZGen_PostFSR_mass << " ewk weight= " << (ZGen_PostFSR_mass>50&&ZGen_PostFSR_mass<200?hZmassSF->Interpolate(ZGen_PostFSR_mass):1) << endl;
    }
    //---------------- PT weight
    if(usePtSF!=-1  && usePtSF!=1 &&usePtSF!=2 /* && ZGen_pt<30 */ && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && hZPtSF && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig")))
      // evt_weight_original*=hZPtSF->GetBinContent(hZPtSF->GetXaxis()->FindBin(/* Z_pt>0?Z_pt: */ZGen_pt));
      evt_weight_original*=hZPtSF->Interpolate(ZGen_pt)>0?hZPtSF->Interpolate(ZGen_pt):1;
    else if(usePtSF==1 && ZGen_pt<30 && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && hZPtSF && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){
      evt_weight_original*=TMath::Min(1+0.1*(25-ZGen_pt)/25,0.5);
      common_stuff::plot2D(Form("weight1_vs_genpT"),
                                      ZGen_pt,(1+0.1*(25-ZGen_pt)/25), 1, 
                                          h_2d, 50,0,50,50,0,2 );
    }
    else if(usePtSF==2 && ZGen_pt<30 && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && hZPtSF && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){
      evt_weight_original*=(1+0.1*(ZGen_pt-25)/25);
      common_stuff::plot2D(Form("weight2_vs_genpT"),
                                      ZGen_pt,(1+0.1*(25-ZGen_pt)/25), 1, 
                                          h_2d, 50,0,50,50,0,2 );
    }
    
    //---------------- EWK weight
    if(use_syst_ewk_Alcaraz && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){
      if(use_syst_ewk_Alcaraz==100 && FSRWeight>-99){ 
        evt_weight_original *= FSRWeight;
        // cout << "FSRWeight= " << FSRWeight << endl;
      }else if(use_syst_ewk_Alcaraz==0){
      }        
    }
    
    //------------------------------------------------------
    // start loop on eta_max cuts
    //------------------------------------------------------    
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      // cout << "eta_str " << eta_str << endl;

      //------------------------------------------------------
      // start loop on mass hypotheses
      //------------------------------------------------------
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){

        if(!(sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig")) && WMass::WMassNSteps!=j) continue;
        // int jZmass_MeV = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMass_SkipNSteps;
        int jZmass_MeV = WMass::Zmass_values_array[j];
        // double iZmass_GeV = (WMass::ZMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMass_SkipNSteps)/1e3;
        double iZmass_GeV = WMass::Zmass_values_array[j]/1e3;
        // cout << "iZmass_GeV " << iZmass_GeV << endl;
        
        //------------------------------------------------------
        // compute weight for mass hypotheses wiht Breit-Wigner reweighting or LHE
        //------------------------------------------------------
        double weight_i=1; // bweight_i=1,lheweight_i=1;
        if(useGenVar){
          if(!contains_LHE_weights){

            double gamma=2.141; /*HARD CODED TO PDG VALUE*/
            weight_i = common_stuff::BWweight(ZGen_mass, iZmass_GeV, gen_mass_value_MeV, gamma);
            // bweight_i = weight_i;

          }else{
            // cout << "LHE weight= " << (LHE_weight [ (WMass::LHE_mass_central_index + ( -WMass::WMassNSteps + j)*WMass::WMassSkipNSteps) ]) << " weight_i= " << weight_i << endl;
            // common_stuff::plot2D(Form("hCorrMassweights_eta%s_%d",eta_str.Data(),jZmass_MeV),
            // weight_i,LHE_weight [ (WMass::LHE_mass_central_index + ( -WMass::WMassNSteps + j)*WMass::WMassSkipNSteps) ], 1,
            // h_2d, 200,0.9,1.1,200,0.9,1.1 );

            weight_i = LHE_weight [ (WMass::LHE_mass_central_index + ( -WMass::WMassNSteps + j)*WMass::WMassSkipNSteps) ];
            // lheweight_i = weight_i;
            // cout << "jZmass_MeV= " << jZmass_MeV << " j= " << j << " index= " << (WMass::LHE_mass_central_index + ( -WMass::WMassNSteps + j)*WMass::WMassSkipNSteps) << endl;
          }
        }
        double evt_weight=evt_weight_original*weight_i;
        // cout << "evt_weight B-W " << evt_weight << endl;
                
        //------------------------------------------------------
        // retrieve and use gen-level info
        //------------------------------------------------------
        // SELECT EVENT WITH Z/W SCALED CUTS
        // gen stuff to be used only when required
        if(useGenVar){
          if(ZGen_mass>0 && WMass::WMassNSteps==j ){
            double WlikeGen_var_NotScaled[WMass::NFitVar-1] = {muGen_status3.Pt(),ZGen_mt,neutrinoGen_status3.Pt()};
            
            for(int k=0;k<WMass::NFitVar-1;k++)
              common_stuff::plot1D(Form("hWlike%s_%sNonScaled_1_Gen_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                    WlikeGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
              
              // common_stuff::plot1D(Form("hWlike%s_MGenNonScaled_1_GenBW_eta%s_%d",WCharge_str.Data(),eta_str.Data(),jZmass_MeV),
                                    // ZGen_mass, bweight_i, h_1d, 50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio );
              // common_stuff::plot1D(Form("hWlike%s_MGenNonScaled_1_GenLHE_eta%s_%d",WCharge_str.Data(),eta_str.Data(),jZmass_MeV),
                                    // ZGen_mass, lheweight_i, h_1d, 50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio );

            if(ZGen_mass>50){
              for(int k=0;k<WMass::NFitVar-1;k++)
                common_stuff::plot1D(Form("hWlike%s_%sNonScaled_2_ZGenMassCut_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                      WlikeGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
              
              if(TMath::Abs(muGen_status3.Eta())<WMass::etaMaxMuons[i]){
                for(int k=0;k<WMass::NFitVar-1;k++)
                  common_stuff::plot1D(Form("hWlike%s_%sNonScaled_3_Mu1GenCut_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                        WlikeGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                
                if(TMath::Abs(neutrinoGen_status3.Eta())<2.4){
                  for(int k=0;k<WMass::NFitVar-1;k++)
                    common_stuff::plot1D(Form("hWlike%s_%sNonScaled_4_Mu2GenCut_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                          WlikeGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                }
              }
            }
          }
          common_stuff::plot1D(Form("z_mass_gen"), ZGen_status3.M(), 1, h_1d, 200, 0, 200 );
          common_stuff::plot1D(Form("z_rap_gen"), ZGen_status3.Rapidity(), 1, h_1d, 200, 0, 200 );
          
          common_stuff::plot1D(Form("mupos_pt_gen"), muPosGen_status3.Pt(), 1, h_1d, 100, 0, 100 );
          common_stuff::plot1D(Form("muneg_pt_gen"), muNegGen_status3.Pt(), 1, h_1d, 100, 0, 100 );
          common_stuff::plot1D(Form("mupos_eta_gen"), muPosGen_status3.Eta(), 1, h_1d, 50, -5, 5 );
          common_stuff::plot1D(Form("muneg_eta_gen"), muNegGen_status3.Eta(), 1, h_1d, 50, -5, 5 );
          
          // put polarization plots here:
          common_stuff::plot2D(Form("phi_vs_costh_CS"),
                                          costh_CS_gen,TMath::Abs(phi_CS_gen), 1, 
                                          h_2d, 12,-1,1,12,0,TMath::Pi() );
          common_stuff::plot2D(Form("phi_vs_costh_HX"),
                                          costh_HX_gen,TMath::Abs(phi_HX_gen), 1, 
                                          h_2d, 12,-1,1,12,0,TMath::Pi() );

          // continue;
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
              && TMath::Abs(muCorrCentral.Eta())<WMass::etaMaxMuons[i]
              && TMath::Abs(neutrinoCorrCentral.Eta())<2.4
              && MuPos_charge != MuNeg_charge
              && muTrg
              && MuPosIsTight && MuPos_dxy<0.02
              && MuNegIsTight && MuNeg_dxy<0.02
              && muRelIso<0.12 && neutrinoRelIso<0.5
            ){ // good reco event

            for(int m=m_start; m<m_end; m++){

              TString RecoilVar_str = "";
              if(RecoilCorrVarDiagoParU1orU2fromDATAorMC>0) RecoilVar_str = Form("_RecoilCorrVar%d",m);
              // cout << RecoilVar_str << endl;

              for(int n=0; n<WMass::KalmanNvariations; n++){

                TString KalmanVars_str = "";
                if(WMass::KalmanNvariations>1) KalmanVars_str = Form("_KalmanVar%d",n);

                //------------------------------------------------------------------------------------------------
                // Apply muon corrections
                //------------------------------------------------------------------------------------------------
                if(first_time_in_the_event && m==m_start){ // use rochester corrections if required
                  //if(useMomentumCorr==1){ // use rochester corrections if required
                    //if(IS_MC_CLOSURE_TEST || isMCorDATA==0){
                      //rmcor44X->momcor_mc(muPosCorr, MuPos_charge, varyMuonCorrNsigma/* , runopt */);
                      //rmcor44X->momcor_mc(muNegCorr, MuNeg_charge, varyMuonCorrNsigma/* , runopt */);
                      //rmcor44X->momcor_mc(muPosCorrCentral, MuPos_charge, 0/* , runopt */);
                      //rmcor44X->momcor_mc(muNegCorrCentral, MuNeg_charge, 0/* , runopt */);
                    //}
                    //else{
                      //rmcor44X->momcor_data(muPosCorr, MuPos_charge, varyMuonCorrNsigma , run<175832 ? 0 : 1 );
                      //rmcor44X->momcor_data(muNegCorr, MuNeg_charge, varyMuonCorrNsigma , run<175832 ? 0 : 1 );
                      //rmcor44X->momcor_data(muPosCorrCentral, MuPos_charge, 0 , run<175832 ? 0 : 1 );
                      //rmcor44X->momcor_data(muNegCorrCentral, MuNeg_charge, 0 , run<175832 ? 0 : 1 );
                    //}
                  //}
                  if(useMomentumCorr==2){ // use Momentum scale corrections if required
                    corrector->applyPtCorrection(muPosCorr,MuPos_charge);
                    corrector->applyPtCorrection(muNegCorr,MuNeg_charge);
                    corrector->applyPtCorrection(muPosCorrCentral,MuPos_charge);
                    corrector->applyPtCorrection(muNegCorrCentral,MuNeg_charge);
                  }else if(useMomentumCorr==3){ // use Momentum scale corrections from Kalman calibrator if required
                    
                    // corrector_Kalman->getCorrectedPt(pt,eta,phi,charge); //returns the corrected pt 
                    // if(isMCorDATA==0) corrector_Kalman->smear(pt,eta); // Applies smearing to the MC to match the data-returns the smeared pt -> Only for MC . 
                    
                    // if(TMath::Abs(muPosCorr.Eta())<0.9){
                    
                      // cout << "muPosCorr before muon correction" << endl; 
                      // cout << "muPosCorr.Pt()= " << muPosCorr.Pt() << " muPosCorr.Eta()= " << muPosCorr.Eta() << " muPosCorr.Phi()= " << muPosCorr.Phi() << endl;
                      // cout << "muPosCorrCentral.Pt()= " << muPosCorrCentral.Pt() << " muPosCorrCentral.Eta()= " << muPosCorrCentral.Eta() << " muPosCorrCentral.Phi()= " << muPosCorrCentral.Phi() << endl;

                      if(n==0){
                        corrector_Kalman->getCorrectedPt(muPosCorrCentral,MuPos_charge); //returns the corrected pt 
                        corrector_Kalman->getCorrectedPt(muNegCorrCentral,MuNeg_charge); //returns the corrected pt 
                        corrector_Kalman->smear(muPosCorrCentral);
                        corrector_Kalman->smear(muNegCorrCentral);
                      }
                      if(varyMuonCorrNsigma!=0){
                        if(WMass::KalmanNvariations==1){
                          corrector_Kalman->varyClosure(varyMuonCorrNsigma);
                        }else{
                          muPosCorr = muPosNoCorr;
                          muNegCorr = muNegNoCorr;
                          corrector_Kalman->reset(); 
                          corrector_Kalman->vary(n,varyMuonCorrNsigma);
                        }
                      }
                      corrector_Kalman->getCorrectedPt(muPosCorr,MuPos_charge); //returns the corrected pt 
                      corrector_Kalman->getCorrectedPt(muNegCorr,MuNeg_charge); //returns the corrected pt 
                      corrector_Kalman->smear(muPosCorr);
                      corrector_Kalman->smear(muNegCorr);
                      // corrector_Kalman->getCorrectedPt(muNegCorrCentral,MuNeg_charge); //returns the corrected pt 
                        
                      // cout << "muPosCorr after scale correction, varyMuonCorrNsigma=" << varyMuonCorrNsigma << endl;
                      // cout << "muPosCorr.Pt()= " << muPosCorr.Pt() << " muPosCorr.Eta()= " << muPosCorr.Eta() << " muPosCorr.Phi()= " << muPosCorr.Phi() << endl;
                      // cout << "muPosCorrCentral.Pt()= " << muPosCorrCentral.Pt() << " muPosCorrCentral.Eta()= " << muPosCorrCentral.Eta() << " muPosCorrCentral.Phi()= " << muPosCorrCentral.Phi() << endl;
                      // muPosCorr.Print();
                      
                      // if(isMCorDATA==0){ // Applies smearing to the MC to match the data-returns the smeared pt -> Only for MC .
                        // corrector_Kalman->smear(muPosCorrCentral);
                        // corrector_Kalman->smear(muNegCorrCentral);
                      // }
                      // cout << "corrector_Kalman->getN()= " << corrector_Kalman->getN() << endl;
                      
                      // cout << "muPosCorr after smear correction, varyMuonCorrNsigma=" << varyMuonCorrNsigma << endl;
                      // cout << "muPosCorr.Pt()= " << muPosCorr.Pt() << " muPosCorr.Eta()= " << muPosCorr.Eta() << " muPosCorr.Phi()= " << muPosCorr.Phi() << endl;
                      // cout << "muPosCorrCentral.Pt()= " << muPosCorrCentral.Pt() << " muPosCorrCentral.Eta()= " << muPosCorrCentral.Eta() << " muPosCorrCentral.Phi()= " << muPosCorrCentral.Phi() << endl;
                      // if(varyMuonCorrNsigma!=0){
                        // corrector_Kalman->applyPtBias(muPosCorr,1e-3*varyMuonCorrNsigma); //returns the corrected pt
                        // // corrector_Kalman->applyPtBias(muNegCorr,1e-3*varyMuonCorrNsigma); //returns the corrected pt
                      // }
                      // cout << "muPosCorr after muon correction" << endl; 
                      // cout << "muPosCorr.Pt()= " << muPosCorr.Pt() << " muPosCorr.Eta()= " << muPosCorr.Eta() << " muPosCorr.Phi()= " << muPosCorr.Phi() << endl;
                      // cout << "muPosCorrCentral.Pt()= " << muPosCorrCentral.Pt() << " muPosCorrCentral.Eta()= " << muPosCorrCentral.Eta() << " muPosCorrCentral.Phi()= " << muPosCorrCentral.Phi() << endl;
                    // }
                  }else if(useMomentumCorr==4){ // use Momentum scale corrections from KalmanParam calibrator if required
                    
                      // cout << "muPosCorr before muon correction" << endl; 
                      // cout << "muPosCorr.Pt()= " << muPosCorr.Pt() << " muPosCorr.Eta()= " << muPosCorr.Eta() << " muPosCorr.Phi()= " << muPosCorr.Phi() << endl;
                      // cout << "muPosCorrCentral.Pt()= " << muPosCorrCentral.Pt() << " muPosCorrCentral.Eta()= " << muPosCorrCentral.Eta() << " muPosCorrCentral.Phi()= " << muPosCorrCentral.Phi() << endl;

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
                      // corrector_KalmanParam->getCorrectedPt(muNegCorrCentral,MuNeg_charge); //returns the corrected pt 
                        
                      // cout << "muPosCorr after scale correction, varyMuonCorrNsigma=" << varyMuonCorrNsigma << endl;
                      // cout << "muPosCorr.Pt()= " << muPosCorr.Pt() << " muPosCorr.Eta()= " << muPosCorr.Eta() << " muPosCorr.Phi()= " << muPosCorr.Phi() << endl;
                      // cout << "muPosCorrCentral.Pt()= " << muPosCorrCentral.Pt() << " muPosCorrCentral.Eta()= " << muPosCorrCentral.Eta() << " muPosCorrCentral.Phi()= " << muPosCorrCentral.Phi() << endl;
                      // muPosCorr.Print();
                      
                      // if(isMCorDATA==0){ // Applies smearing to the MC to match the data-returns the smeared pt -> Only for MC .
                        // corrector_KalmanParam->smear(muPosCorrCentral);
                        // corrector_KalmanParam->smear(muNegCorrCentral);
                      // }
                      // cout << "corrector_KalmanParam->getN()= " << corrector_KalmanParam->getN() << endl;
                      
                      // cout << "muPosCorr after smear correction, varyMuonCorrNsigma=" << varyMuonCorrNsigma << endl;
                      // cout << "muPosCorr.Pt()= " << muPosCorr.Pt() << " muPosCorr.Eta()= " << muPosCorr.Eta() << " muPosCorr.Phi()= " << muPosCorr.Phi() << endl;
                      // cout << "muPosCorrCentral.Pt()= " << muPosCorrCentral.Pt() << " muPosCorrCentral.Eta()= " << muPosCorrCentral.Eta() << " muPosCorrCentral.Phi()= " << muPosCorrCentral.Phi() << endl;
                      // if(varyMuonCorrNsigma!=0){
                        // corrector_KalmanParam->applyPtBias(muPosCorr,1e-3*varyMuonCorrNsigma); //returns the corrected pt
                        // // corrector_KalmanParam->applyPtBias(muNegCorr,1e-3*varyMuonCorrNsigma); //returns the corrected pt
                      // }
                      // cout << "muPosCorr after muon correction" << endl; 
                      // cout << "muPosCorr.Pt()= " << muPosCorr.Pt() << " muPosCorr.Eta()= " << muPosCorr.Eta() << " muPosCorr.Phi()= " << muPosCorr.Phi() << endl;
                      // cout << "muPosCorrCentral.Pt()= " << muPosCorrCentral.Pt() << " muPosCorrCentral.Eta()= " << muPosCorrCentral.Eta() << " muPosCorrCentral.Phi()= " << muPosCorrCentral.Phi() << endl;
                    // }
                  }

                }

                Zcorr = muPosCorr + muNegCorr;
                ZcorrCentral = muPosCorrCentral + muNegCorrCentral;
                //------------------------------------------------------------------------------------------------
                // Apply recoil corrections
                //------------------------------------------------------------------------------------------------

                int rapBin=1;
                if(fabs(ZGen_rap)>=1 ) rapBin=2;
                // if(fabs(ZGen_rap)<1) rapBin=1;
                // if(fabs(ZGen_rap)>=1 && fabs(ZGen_rap)<=1.25) rapBin=125;
                // if(fabs(ZGen_rap)>=1.25 && fabs(ZGen_rap)<=1.5) rapBin=150;
                // if(fabs(ZGen_rap)>=1.5 && fabs(ZGen_rap)<=1.75) rapBin=175;
                // if(fabs(ZGen_rap)>=1.75 && fabs(ZGen_rap)<=2.0) rapBin=200;
                // if(fabs(ZGen_rap)>2.0) rapBin=201;

                // int vtxBin=1;
                int vtxBin=rapBin;

                if( ((first_time_in_the_event || RecoilCorrVarDiagoParU1orU2fromDATAorMC>0) && n==0)
                    && use_PForNoPUorTKmet<3 && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){ // use MET corrections if required
                  
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
                  
                  // cout
                  // << "m= " << m 
                  // << " u1u2 " << WMass::RecoilCorrVarDiagoParU1orU2fromDATAorMC_[m]
                  // << " par " << WMass::RecoilCorrVarDiagoParN_[m] 
                  // << " first_time_in_the_event " << first_time_in_the_event << endl;
                  // cout
                  // << " met_trasv before=" << met_trasv
                  // << " metphi_trasv before=" << metphi_trasv
                  // << " met_trasvCentral before=" << met_trasvCentral
                  // << " metphi_trasvCentral before=" << metphi_trasvCentral
                  // << endl;
                  //if(useRecoilCorr==1){}
                  if(useRecoilCorr==2){
                    // cout
                    // << "ZGen_pt=" << ZGen_pt
                    // << " ZGen_phi=" << ZGen_phi
                    // << " ZNocorr.Pt()=" << ZNocorr.Pt()
                    // << " ZNocorr.Phi()=" << ZNocorr.Phi()
                    // << endl
                    // << " first_time_in_the_event=" << first_time_in_the_event
                    // << " RecoilCorrVarDiagoParU1orU2fromDATAorMC=" << RecoilCorrVarDiagoParU1orU2fromDATAorMC
                    // << " m=" << m
                    // << " RecoilCorrVarDiagoParSigmas=" << RecoilCorrVarDiagoParSigmas
                    // << " vtxBin=" << vtxBin
                    // << Form("hWlike%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[0].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,0,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV)
                    // << endl;
                    // cout << "Before correction:" << m << " - " << met_trasv << " - " << metphi_trasv << endl;
                    correctorRecoil_Z->CorrectMET3gaus(met_trasv,metphi_trasv,
                            ZGen_pt,ZGen_phi,
                            // Zcorr.Pt(),Zcorr.Phi(),
                            ZNocorr.Pt(),ZNocorr.Phi(),
                            u1_recoil, u2_recoil,
                            RecoilCorrVarDiagoParU1orU2fromDATAorMC, m, RecoilCorrVarDiagoParSigmas,
                            vtxBin,doSingleGauss,1);

                    if(first_time_in_the_event && m==m_start && n==0){
                      if(RecoilCorrVarDiagoParU1orU2fromDATAorMC!=0 || m!=0 || RecoilCorrVarDiagoParSigmas!=0){
                      // cout << "correcting met_Central= " << met_trasvCentral ;
                        correctorRecoil_Z->CorrectMET3gaus(met_trasvCentral,metphi_trasvCentral,
                                ZGen_pt,ZGen_phi,
                                ZNocorr.Pt(),ZNocorr.Phi(),
                                u1_recoil, u2_recoil,
                                0, 0, 0,
                                vtxBin,doSingleGauss,1);
                      }else{
                        met_trasvCentral    = met_trasv;
                        metphi_trasvCentral = metphi_trasv;
                      }
                      // correctorRecoil_Z->NonClosure_scale(met_trasv,metphi_trasv,
                      //        ZGen_pt,ZGen_phi,ZGen_rap,
                      //        Zcorr.Pt(),Zcorr.Phi());
                    }
                  }
                  // cout
                  // << "met_trasv after=" << met_trasv
                  // << " metphi_trasv after=" << metphi_trasv
                  // << " met_trasvCentral after=" << met_trasvCentral
                  // << " metphi_trasvCentral after=" << metphi_trasvCentral
                  // << endl;
                }


                //------------------------------------------------------
                // Apply met phi corrections
                //------------------------------------------------------
                if(first_time_in_the_event && usePhiMETCorr==1 && m==m_start && n==0){ // use MET Phi correction if required
                  pair<double, double> met_phicorr = common_stuff::getPhiCorrMET( met_trasv, metphi_trasv, nvtx, !sampleName.Contains("DATA"));
                  met_trasv = met_phicorr.first;
                  metphi_trasv = met_phicorr.second;
                }
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
              
                double coeff=2;
                double MTFirstOrder_central = common_stuff::getMTFirstOrder(muCorrCentral.Pt(), muCorrCentral.Phi(), Wlike_metCentral.Pt() ,Wlike_metCentral.Phi(), coeff);
                double MTFirstOrder = common_stuff::getMTFirstOrder(muCorr.Pt(), muCorr.Phi(), Wlike_met.Pt(),Wlike_met.Phi(), coeff);
                //------------------------------------------------------
                // Variables to fill the histos (pT, mT, MET)
                //------------------------------------------------------
                // double Wlike_var_jacobian[WMass::NFitVar] = {2*muCorr.Pt()/WMass::ZMassCentral_MeV*1e3,Wlike.Mt()/WMass::ZMassCentral_MeV*1e3,2*Wlike_met.Pt()/WMass::ZMassCentral_MeV*1e3,MTFirstOrder/WMass::ZMassCentral_MeV*1e3};
                double Wlike_var_NotScaled[WMass::NFitVar] = {muCorr.Pt(),Wlike.Mt(),Wlike_met.Pt(),MTFirstOrder};

                //------------------------------------------------------
                // Variables to define the box cut (pT, mT, MET)
                //------------------------------------------------------
                double Wlike_var_NotScaledCentral[WMass::NFitVar] = {muCorrCentral.Pt(),WlikeCentral.Mt(),Wlike_metCentral.Pt(),MTFirstOrder_central}; // Zcorr would be TEMP !!!!
                
                // LUCA ADD TO AVOID OVERFLOW
                // for(int k=0;k<WMass::NFitVar;k++)
                  // if(Wlike_var_jacobian[k]>=xmax) Wlike_var_jacobian[k]=xmax-binsize2/2;

                //------------------------------------------------------
                // good pair within acceptance cuts for both muons
                //------------------------------------------------------
                if( ZcorrCentral.M()>50
                    && TMath::Abs(muCorrCentral.Eta())<WMass::etaMaxMuons[i] 
                    && TMath::Abs(neutrinoCorrCentral.Eta())<2.4 
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
                      if(m==m_start && n==0 && WMass::WMassNSteps==j) common_stuff::plot1D(Form("hWlike%s_%sNonScaled_5_RecoCut_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                       Wlike_var_NotScaled[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
          
                    //------------------------------------------------------------------------------------------------
                    // BELOW PLOTS for CLOSURE TEST
                    //------------------------------------------------------------------------------------------------ 
                    
                    if(Wlike_met.Pt()>0 && m==m_start && n==0 && controlplots && WMass::WMassNSteps==j) {

                      string tag_zPtcut;
                      if ( Zcorr.Pt()<2 ) tag_zPtcut = "_Zpt02";
                      else if  ( Zcorr.Pt()>=2 && Zcorr.Pt()<4 ) tag_zPtcut = "_Zpt24";
                      else if  ( Zcorr.Pt()>=4 && Zcorr.Pt()<6 ) tag_zPtcut = "_Zpt46";
                      else if  ( Zcorr.Pt()>=6 && Zcorr.Pt()<8 ) tag_zPtcut = "_Zpt68";
                      else if  ( Zcorr.Pt()>=8 && Zcorr.Pt()<10 ) tag_zPtcut = "_Zpt810";
                      else if  ( Zcorr.Pt()>=10 && Zcorr.Pt()<12 ) tag_zPtcut = "_Zpt1012";
                      else if  ( Zcorr.Pt()>=12 && Zcorr.Pt()<14 ) tag_zPtcut = "_Zpt1214";
                      else if  ( Zcorr.Pt()>=14 && Zcorr.Pt()<16 ) tag_zPtcut = "_Zpt1416";
                      else if  ( Zcorr.Pt()>=16 && Zcorr.Pt()<18 ) tag_zPtcut = "_Zpt1618";
                      else if  ( Zcorr.Pt()>=18 && Zcorr.Pt()<20 ) tag_zPtcut = "_Zpt1820";
                      else if  ( Zcorr.Pt()>=20 && Zcorr.Pt()<30 ) tag_zPtcut = "_Zpt2030";
                      else if  ( Zcorr.Pt()>=30 && Zcorr.Pt()<50 ) tag_zPtcut = "_Zpt3050";
                      else if  ( Zcorr.Pt()>=50 ) tag_zPtcut = "_Zpt50";
                      else tag_zPtcut = "_ignore";

                      double Zy=Zcorr.Rapidity();
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
                      VisPt.SetPtEtaPhiM(Zcorr.Pt(),0,Zcorr.Phi(),0);

                      TLorentzVector Zgen;
                      Zgen.SetPtEtaPhiM(ZGen_pt,0,ZGen_phi,0);

                      string mettype="_tk";
                      
                      double u1_scale=0;
                      plotVariables( Z_met, VisPt,  Zcorr, Zgen, u1_scale, tag_zPtcut.c_str(), mettype.c_str() , false, false, h_1d, h_2d, evt_weight*TRG_TIGHT_ISO_muons_SF, WMass::WMassNSteps, jZmass_MeV);
                      plotVariables( Z_met, VisPt,  Zcorr, Zgen, u1_scale, tag_VTX.c_str(), mettype.c_str() , false, false, h_1d, h_2d, evt_weight*TRG_TIGHT_ISO_muons_SF, WMass::WMassNSteps, jZmass_MeV);
                      plotVariables( Z_met, VisPt,  Zcorr, Zgen, u1_scale, tag_y.c_str(), mettype.c_str() , false, false, h_1d, h_2d, evt_weight*TRG_TIGHT_ISO_muons_SF, WMass::WMassNSteps , jZmass_MeV);
                    }

                    //------------------------------------------------------
                    // cut on MET
                    //------------------------------------------------------
                    if(Wlike_metCentral.Pt()>WMass::sel_xmin[2]*ZWmassRatio){
                      for(int k=0;k<WMass::NFitVar;k++)
                        if(m==m_start && n==0 && WMass::WMassNSteps==j) common_stuff::plot1D(Form("hWlike%s_%sNonScaled_6_METCut_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                         Wlike_var_NotScaled[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                      
                      if(controlplots && m==m_start && n==0 && WMass::WMassNSteps==j) common_stuff::plot1D(Form("hZ_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                      Zcorr.Pt(),evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 1000,0,250 );

                      //------------------------------------------------------
                      // cut on W recoil (BY DEFAULT IS 15)
                      //------------------------------------------------------
                      if(WlikeCentral.Pt()<WMass::WpTcut*ZWmassRatio){
                        for(int k=0;k<WMass::NFitVar;k++)
                          if(m==m_start && n==0 && WMass::WMassNSteps==j) common_stuff::plot1D(Form("hWlike%s_%sNonScaled_7_RecoilCut_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                            Wlike_var_NotScaled[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                        
                        //------------------------------------------------------
                        // Apply the "box" cuts on pT, mT, MET -- based on central correction
                        //------------------------------------------------------
                        if(
                            Wlike_var_NotScaledCentral[0] > WMass::sel_xmin[0]*ZWmassRatio && Wlike_var_NotScaledCentral[0] < WMass::sel_xmax[0]*ZWmassRatio
                            && Wlike_var_NotScaledCentral[1] > WMass::sel_xmin[1]*ZWmassRatio && Wlike_var_NotScaledCentral[1] < WMass::sel_xmax[1]*ZWmassRatio // Mt
                            // && Wlike_var_NotScaledCentral[3] > WMass::sel_xmin[3]*ZWmassRatio && Wlike_var_NotScaledCentral[3] < WMass::sel_xmax[3]*ZWmassRatio // MtLin
                            && Wlike_var_NotScaledCentral[2] > WMass::sel_xmin[2]*ZWmassRatio && Wlike_var_NotScaledCentral[2] < WMass::sel_xmax[2]*ZWmassRatio
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
                          if(m==m_start && n==0 && controlplots && WMass::WMassNSteps==j){
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
                              if(m==m_start && n==0 && controlplots && WMass::WMassNSteps==j) 
                                common_stuff::plot1D(Form("hPDF_weights_%d_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h),
                                              lha_weight, 1, h_1d, 1000,0,2
                                            );
                            } else if(contains_LHE_weights){
                              // if(m==m_start && n==0 && WMass::WMassNSteps==j)
                                // common_stuff::plot2D(Form("hCorrPDFweights_pdf%d-%d%s%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                              // lha_weight,LHE_weight[PDF_reweighting_central_Index+h], 1, 
                                              // h_2d, 200,0.9,1.1,200,0.9,1.1);
                              
                              lha_weight = LHE_weight[PDF_reweighting_central_Index+h];
                            }
                            
                            // WEIGHT DEFINITION
                            double weight = evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight;
                            
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
                              common_stuff::plot1D(Form("hWlike%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                                    Wlike_var_NotScaled[k], weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                            }

                            //---------------------------------------------------------------------
                            // Recoil plots: u1, u2, u1vsZpt, u2vsZpt, u1vsZptvsZrap, u2vsZptvsZrap
                            //---------------------------------------------------------------------
                            common_stuff::calculateU1U2(met_trasv, metphi_trasv,  ZGen_pt, ZGen_phi,
                                                        ZNocorr.Pt(), ZNocorr.Phi(),  u1_recoil, u2_recoil);

                            common_stuff::plot1D(Form("hWlike%s_u1_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                              u1_recoil, weight, h_1d, 80, -20, 20 );
                            common_stuff::plot1D(Form("hWlike%s_u2_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                              u2_recoil, weight, h_1d, 80, -20, 20 );

                            common_stuff::plot2D(Form("hWlike%s_u1vsZpt_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                              ZNocorr.Pt(), u1_recoil, weight, h_2d, 80, 0, 80, 80, -20, 20 );
                            common_stuff::plot2D(Form("hWlike%s_u2vsZpt_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                              ZNocorr.Pt(), u2_recoil, weight, h_2d, 80, 0, 80, 80, -20, 20 );
                            
                            common_stuff::plot3D(Form("hWlike%s_u1vsZptvsZrap_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                              ZNocorr.Pt(), ZGen_status3.Rapidity(), u1_recoil, weight, h_3d, 80, 0, 80, 80, -5, +5, 80, -20, 20 );
                            common_stuff::plot3D(Form("hWlike%s_u2vsZptvsZrap_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                              ZNocorr.Pt(), ZGen_status3.Rapidity(), u2_recoil, weight, h_3d, 80, 0, 80, 80, -5, +5, 80, -20, 20 );


                            //------------------------------------------------------------------------------------------------
                            // PLOTS FOR GIGI's TEST see 11 apr 2014 (CMG presentations)
                            //------------------------------------------------------------------------------------------------

                            if(controlplots){
                              //      cout << "filling control plot RecoilVar=" << RecoilVar_str.Data() << endl;
                              common_stuff::plot1D(Form("deltaMT_Wlike%s_8_JetCut_pdf%d-%d%s_kalman%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                      Wlike.Mt() - WlikeCentral.Mt(), weight, h_1d, 200, -0.1, 0.1);

                              common_stuff::plot1D(Form("deltaMET_Wlike%s_8_JetCut_pdf%d-%d%s_kalman%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                      Wlike_met.Pt() - Wlike_metCentral.Pt(), weight, h_1d, 200, -0.1, 0.1);
                            }

                            //------------------------------------------------------------------------------------------------
                            // EXTRA PLOTS
                            //------------------------------------------------------------------------------------------------ 
                            if(m==m_start && n==0 && controlplots && WMass::WMassNSteps==j){
                              common_stuff::plot1D(Form("hWlike%s_ZpT_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                                ZcorrCentral.Pt(), weight, h_1d, 200, 0, 100 );
                              common_stuff::plot1D(Form("hWlike%s_Zmass_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                                ZcorrCentral.M(), weight, h_1d, 200, 50, 150 );

                              common_stuff::plot1D(Form("hWlike%s_Recoil_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                                Wlike.Pt(), evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, 0, 25 );

                              common_stuff::plot2D(Form("hWlike%s_MtLinVsMt_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                                Wlike_var_NotScaled[1],Wlike_var_NotScaled[3], 1,
                                                h_2d, 50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio,
                                                50, WMass::fit_xmin[3]*ZWmassRatio, WMass::fit_xmax[3]*ZWmassRatio );
                              common_stuff::plot2D(Form("hWlike%s_MtLinVsGenZPt_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                                ZGen_pt,Wlike_var_NotScaled[3], 1,
                                                h_2d, 40, 0,20,
                                                50, WMass::fit_xmin[3]*ZWmassRatio, WMass::fit_xmax[3]*ZWmassRatio );
                              common_stuff::plot2D(Form("hWlike%s_MtVsGenZPt_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV),
                                                ZGen_pt,Wlike_var_NotScaled[1], 1,
                                                h_2d, 40, 0,20,
                                                50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio );
                            }
                          } // end loop PDF
                          
                          //------------------------------------------------------------------------------------------------
                          // control plots for different etas but only for central W mass
                          //------------------------------------------------------------------------------------------------ 
                          if(TMath::Abs(jZmass_MeV - WMass::ZMassCentral_MeV) > 1)  continue;

                          if(controlplots && m==m_start && n==0) fillControlPlots(Zcorr, Z_met, muPosCorr, muNegCorr, h_1d, h_2d, evt_weight*TRG_TIGHT_ISO_muons_SF, jZmass_MeV , eta_str, WMass::nSigOrQCD_str[0]);

                        }
                      }
                    }
                  }else if(controlplots && m==m_start && n==0 && isChargePos){
                    // NOTE: ONLY DONE FOR WLIKEPOS !!!
                    
                    // Iso and dxy for muons which fail either tight requirement, isolation or dxy cut
                    if(pfmetWlikePos>25 && WlikePos_pt<20){
                      // if( (TMath::Abs(zmass1 - WMass::WMassCentral_MeV) > 1)) continue;
                    }

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

                      fillControlPlots(Zcorr, Z_met, muPosCorr, muNegCorr, h_1d, h_2d, evt_weight*TRG_TIGHT_ISO_muons_SF, jZmass_MeV , eta_str, WMass::nSigOrQCD_str[1]);

                    } // end if for qcd enriched
                  } // end else for muon cuts (sig or qcd enriched)
                } // end if for good pair within acceptance cuts for both muons
              } // end KalmanVars loop
            } // end RecoilCorr params loop
          } // end if for good reco event
        } // end if for dummy signal/fake separation
        first_time_in_the_event=false;
      } // end W mass loop
    } // end muon eta loop
    
  } // end event loop

  // outTXTfile.close();
  
  TFile*fout = new TFile(Form("%s/Zanalysis%s.root",outputdir.Data(),chunk_str.Data()),"RECREATE");
  fout->cd();

  hZPtSF->Write();

  if(!(sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
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
                  
                    common_stuff::cloneHisto1D(Form("hWlike%s_%sScaled_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV), 
                                                Form("hWlike%s_%sScaled_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV), 
                                                h_1d);
                    common_stuff::cloneHisto1D(Form("hWlike%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),WMass::ZMassCentral_MeV), 
                                                Form("hWlike%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s_eta%s_%d",WCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilVar_str.Data(),KalmanVars_str.Data(),eta_str.Data(),jZmass_MeV), 
                                                h_1d);
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
