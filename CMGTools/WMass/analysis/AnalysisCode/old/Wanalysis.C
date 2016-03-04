// UNCOMMENT TO USE PDF REWEIGHTING
//#define LHAPDF_ON

#ifdef LHAPDF_ON
  #include "LHAPDF/LHAPDF.h"
#endif 

#define Wanalysis_cxx
#include "Wanalysis.h"
#include "common.h"
//#include "common_stuff.h"
#include "RecoilCorrector.h"
// #include "rochcor_42X.h"
#include "rochcor_44X_v3.h"
#include "MuScleFitCorrector.h"
#include <TH3.h>
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TMath.h>
#include <TCanvas.h>
#include <vector>
#include <TGraphAsymmErrors.h>
#include <ctime>
#include <time.h>

void Wanalysis::Loop(int chunk, int Entry_ini, int Entry_fin, int IS_MC_CLOSURE_TEST, int isMCorDATA, TString outputdir, int useMomentumCorr, int smearRochCorrByNsigma, int useEffSF, int usePtSF, int useVtxSF, int controlplots, TString sampleName, int generated_PDF_set, int generated_PDF_member, int contains_PDF_reweight, int usePhiMETCorr, int useRecoilCorr, int RecoilCorrResolutionNSigmaU1, int RecoilCorrScaleNSigmaU1, int RecoilCorrResolutionNSigmaU2, int use_PForNoPUorTKmet, int use_syst_ewk_Alcaraz, int gen_mass_value_MeV, int contains_LHE_weights)
{
  if (fChain == 0) return;

  std::map<std::string, TH1D*> h_1d;
  std::map<std::string, TH2D*> h_2d;

  cout << "generated_PDF_set= "<<generated_PDF_set
       << " generated_PDF_member= " << generated_PDF_member
       << " contains_PDF_reweight= " << contains_PDF_reweight
       << " WMass::NVarRecoilCorr= " << WMass::NVarRecoilCorr
       << endl;  

  int PDF_reweighting_central_Index = -1;
  if(contains_LHE_weights && WMass::PDF_sets==229800) PDF_reweighting_central_Index = WMass::LHE_NNPDF2p3_NLO_central_index;
  if(contains_LHE_weights && WMass::PDF_sets==11000) PDF_reweighting_central_Index = WMass::LHE_CT10_NLO_central_index;
  if(contains_LHE_weights && WMass::PDF_sets==232000) PDF_reweighting_central_Index = WMass::LHE_NNPDF2p3_NNLO_central_index;

  TRandom3 *r = new TRandom3(0);
      
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
      else if(WMass::PDF_sets==21200)
        // LHAPDF::initPDFSet(0,"MSTW2008nnlo68cl.LHgrid");
        LHAPDF::initPDFSet(0,21200,0);
      else if(WMass::PDF_sets==21241)
        LHAPDF::initPDFSet(0,21241,0);  // else if(WMass::PDF_sets<0)
        // LHAPDF::initPDFSet(0,generated_PDF_set,generated_PDF_member);
      else if(WMass::PDF_sets==11000)
        LHAPDF::initPDFSet(0,11000,0);  // else if(WMass::PDF_sets<0)
    }
    
  #endif 
  
  
  int starting_weight_for_generated_PDF_set = 0;
  if(contains_PDF_reweight){
    // cteq6ll.LHpdf=10042 CT10nnlo.LHgrid=11200, NNPDF23_nnlo_as_0118.LHgrid=232000, MSTW2008nnlo68cl.LHgrid=21200
    if(generated_PDF_set==11200) starting_weight_for_generated_PDF_set = 1;
    else if(generated_PDF_set==232000) starting_weight_for_generated_PDF_set = 1;
    else if(generated_PDF_set==21200) starting_weight_for_generated_PDF_set = 1;
  }
  
  TFile*finEffSF, *finPileupSF,*finPtSF;
  TGraphAsymmErrors*hEffSF_MuId_eta_2011[2],*hEffSF_Iso_eta_2011[2],*hEffSF_HLT_eta_2011/* ,*hEffSF_Iso_vtx_2011A,*hEffSF_Iso_vtx_2011B*/;
  TH1D*hPileupSF,*hWPtSFPos,*hWPtSFNeg;

  TH3F *SF_HLT;
  TH2F *SF_TIGHT_ISO;
  TH2F *SF_TIGHT_PT10;
  TH3F *SF_ISO05_PT10;

  //------------------------------------------------------
  // retrieve efficiencies SF
  //------------------------------------------------------
  if(useEffSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    
    finEffSF = new TFile("../utils/MuonEfficiencies_SF_2011_53X_DataMC_Heiner.root"); // used only to build templates
    if(!finEffSF){
      cout << "file MuonEfficiencies_SF_2011_53X_DataMC_Heiner.root is missing, impossible to retrieve efficiency scale factors" << endl;
      return;
    }
    SF_HLT=(TH3F*)finEffSF->Get("SF_2011_HLT_TisoMu24eta2p1_IsoMu24_eta2p1_charge_eta_pt_PLOT"); // x=charge, y=eta, z=pt
    SF_TIGHT_ISO=(TH2F*)finEffSF->Get("SF_2011_TIGHT_ISO_PT25_PtrgL_eta_pt_PLOT");
    SF_TIGHT_PT10=(TH2F*)finEffSF->Get("SF_2011_TIGHT_PT10_ETA2P4_Tid_iso_trg_eta_pt_PLOT");
    SF_ISO05_PT10=(TH3F*)finEffSF->Get("SF_2011_ISO05_PT10_ETA2P4_Tid_iso_trg_CosThetaStar_PhiStarAbs_pair_pt_PLOT");
    // hEffSF_MuId_eta_2011[0]=(TGraphAsymmErrors*)finEffSF->Get("SF_TIGHT_nL8_2011A_eta__pt>20");
    // hEffSF_MuId_eta_2011[1]=(TGraphAsymmErrors*)finEffSF->Get("SF_TIGHT_nL8_2011B_eta__pt>20");
    // hEffSF_Iso_eta_2011[0]=(TGraphAsymmErrors*)finEffSF->Get("combRelPFISO12_2011A_eta__pt>20");
    // hEffSF_Iso_eta_2011[1]=(TGraphAsymmErrors*)finEffSF->Get("combRelPFISO12_2011B_eta__pt>20");
    // hEffSF_HLT_eta_2011=(TGraphAsymmErrors*)finEffSF->Get("SF_HLT_MuIso24_2011_eta__pt>30");
  
  }else{
    cout << "NOT APPLYING EFFICIENCIES SF (not requested or analyzing data)"<< endl;
  }
  //------------------------------------------------------
  // retrieve pileup SF
  //------------------------------------------------------
  bool pileup_reweighting_npu = true;
  if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    TString vtx_str = sampleName; vtx_str.ReplaceAll("Sig",""); vtx_str.ReplaceAll("Fake","");
    // finPileupSF = new TFile(Form("../utils/pileup_reweighting_%s.root",vtx_str.Data())); // used only to build templates
    finPileupSF = new TFile(Form("../utils/pileup/pileup_reweighting_Fall11.root")); // used only to build templates
    // finPileupSF = new TFile(Form("../utils/pileup/pileup_reweighting_Fall11_radoW.root")); pileup_reweighting_npu=false; // used only to build templates
    // finPileupSF = new TFile(Form("../utils/pileup/pileup_reweighting_Fall11_afterRecoilCut.root")); // used only to build templates
    if(!finPileupSF){
      cout << "file " << Form("../utils/pileup/pileup_reweighting_Fall11_afterRecoilCut.root") << " is missing, impossible to retrieve pileup reweighting factors" << endl;
      return;
    }else{
      hPileupSF=(TH1D*)finPileupSF->Get("hpileup_reweighting_Fall11");
    }
  }
  //------------------------------------------------------
  // retrieve pileup SF
  //------------------------------------------------------
  if(usePtSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    // cout << "APPLYING W PT RESCALING" << endl;
    finPtSF = new TFile(Form("../utils/Wpt_reweighting.root")); // used only to build templates
    if(!finPtSF){
      cout << "file " << Form("../utils/Wpt_reweighting.root") << " is missing, impossible to retrieve W pt reweighting factors" << endl;
      // return;
    }else{
      hWPtSFPos=(TH1D*)finPtSF->Get("hWPos_pt_Sig_eta0p6");
      // hWPtSFNeg=(TH1D*)finPtSF->Get("hWNeg_pt_Sig_eta0p6");
    }
  }

  // static const int nbins=75;
  // double bins_scaled[3][nbins+1]={{0.}};
  // double bins_Notscaled[3][nbins+1]={{0.}};
  // double binsize1=0.01,binsize2=0.04;
  // double binsize;
  // double xmin=0.6,xmax=1.8, x;
  // for(int k=0;k<3;k++){
    // x=xmin;
    // binsize=binsize1;
    // for(int i=0;i<nbins;i++){
      // bins_scaled[k][i]=x;
      // bins_Notscaled[k][i]=x*80/(k==1 ? 1 : 2); // mT has double range wrt pt, met
      // if(x>1.2-binsize) binsize=binsize2;
      // x+=binsize;
    // }
    // bins_scaled[k][nbins]=xmax;
    // bins_Notscaled[k][nbins]=xmax*80/(k==1 ? 1 : 2);
  // }
  
  Long64_t first_entry = Entry_ini;
  Long64_t nentries = Entry_fin;
  TString chunk_str = chunk>0? Form("_chunk%d",chunk) : "";
  ofstream outTXTfile;
  outTXTfile.open(Form("%s/Wanalysis_EVlog%s.log",outputdir.Data(),chunk_str.Data()));
  if(!outputdir.Contains("../")) outputdir = "../"+outputdir;
  cout << "output filename= " << Form("%s/Wanalysis%s.root",outputdir.Data(),chunk_str.Data()) << endl;

  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==1) first_entry=nentries/2; // in case of closure test, DATA runs from N/2 to N
  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==0) nentries=nentries/2; // in case of closure test, MC runs from 0 to N/2
  if(IS_MC_CLOSURE_TEST==1) lumi_scaling=lumi_scaling*2; // in case of closure test, scaling must be multiplied by 2

  //To get the central value of the momentum correction
  int random_seed_start=67525;
  rochcor_44X_v3 *rmcor44X = WMass::NVarRecoilCorr>1? new rochcor_44X_v3(random_seed_start) : new rochcor_44X_v3();  // make the pointer of rochcor class
  TString MuscleCard = (IS_MC_CLOSURE_TEST || isMCorDATA==0) ? "MuScleFit_2011_MC_44X" : "MuScleFit_2011_DATA_44X";
  TString fitParametersFile = MuscleCard+".txt";
  MuScleFitCorrector *corrector;
  if(useMomentumCorr==2){
    cout << "using MuscleFit card " << fitParametersFile << endl;
    corrector = new MuScleFitCorrector(fitParametersFile);
  }

  //------------------------------------------------------------------------------------------------
  //    Initialize recoil corrections
  //------------------------------------------------------------------------------------------------
  
  // int use_PForNoPUorTKmet = 2; // 0:PF, 1:NOPU, 2:TK // TO BE CHANGED BY HAND FOR THE MOMENT!!!
  bool use_InclusiveAB_RecoilCorr = true; // TO BE CHANGED BY HAND FOR THE MOMENT!!!
  bool use_InclusiveNVTX_RecoilCorr = true; // TO BE CHANGED BY HAND FOR THE MOMENT!!!
  TString InclusiveNVTXSuffix="";
  if(use_InclusiveNVTX_RecoilCorr) InclusiveNVTXSuffix="_inclusiveNvtx";

  TString metSuffix=use_PForNoPUorTKmet==1?"_pfnoPU":"";
  if(use_PForNoPUorTKmet==2) metSuffix="_tkmet";
  if(use_PForNoPUorTKmet==0) metSuffix="_pfmet";

  TString generatorSuffix="_powheg";
  //  if(use_madgraph) generatorSuffix="_madgraph";    
   
  std::string fileCorrectToPos = Form("../RecoilCode/recoilfit_OCT6_Wpos%s_eta21_PDF-1_pol3_type2_doubleGauss_x2Stat_53X%s.root",metSuffix.Data(),generatorSuffix.Data());
  std::string fileCorrectToNeg = Form("../RecoilCode/recoilfit_OCT6_Wneg%s_eta21_PDF-1_pol3_type2_doubleGauss_x2Stat_53X%s.root",metSuffix.Data(),generatorSuffix.Data());
  std::string fileZmmMC = Form("../RecoilCode/recoilfit_OCT6_genZ%s_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_53X%s.root",metSuffix.Data(),generatorSuffix.Data());
  std::string fileZmmData = Form("../RecoilCode/recoilfit_OCT6_DATA%s_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat_53X.root",metSuffix.Data());
  std::string fileZmmMCMIX = Form("../RecoilCode/recoilfit_.root",metSuffix.Data());

  RecoilCorrector*  correctorRecoil_W_Pos_;  
  RecoilCorrector*  correctorRecoil_W_Neg_;
  RecoilCorrector*  correctorRecoil_W_Pos_MIX_; 
  RecoilCorrector*  correctorRecoil_W_Neg_MIX_;    

  correctorRecoil_W_Neg_ = new RecoilCorrector(fileCorrectToNeg.c_str(),123456);
  correctorRecoil_W_Neg_->addDataFile(fileZmmData.c_str());
  correctorRecoil_W_Neg_->addMCFile(fileZmmMC.c_str());

  correctorRecoil_W_Pos_ = new RecoilCorrector(fileCorrectToPos.c_str(),123456);
  correctorRecoil_W_Pos_->addDataFile(fileZmmData.c_str());
  correctorRecoil_W_Pos_->addMCFile(fileZmmMC.c_str());

  correctorRecoil_W_Neg_MIX_ = new RecoilCorrector(fileCorrectToNeg.c_str(),123456);
  correctorRecoil_W_Neg_MIX_->addDataFile(fileZmmData.c_str());
  correctorRecoil_W_Neg_MIX_->addMCFile(fileZmmMCMIX.c_str());

  correctorRecoil_W_Pos_MIX_ = new RecoilCorrector(fileCorrectToPos.c_str(),123456);
  correctorRecoil_W_Pos_MIX_->addDataFile(fileZmmData.c_str());
  correctorRecoil_W_Pos_MIX_->addMCFile(fileZmmMCMIX.c_str());

  cout << "RecoilCorrResolutionNSigmaU2 " << RecoilCorrResolutionNSigmaU2
       << "RecoilCorrResolutionNSigmaU1 " << RecoilCorrResolutionNSigmaU1
       << "RecoilCorrScaleNSigmaU1 " << RecoilCorrScaleNSigmaU1 << endl;

  bool doSingleGauss=false;

  // the following variables are dummy, but necessary to call the RecoilCorrector.
  double u1_dummy = 0;
  double u2_dummy = 0;
  double fluc_dummy = 0;
  double zero_dummy = 0;
  int jetMult = 0; // set to zero;
  //for the lepPt, lepPhi, 2: lepton is on leg2;
  
  //------------------------------------------------------
  // start the actual event loop
  //------------------------------------------------------
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=first_entry; jentry<=nentries; jentry++) {
  // for (Long64_t jentry=0; jentry<5e4;jentry++) { // for testing purposes
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%250000==0) cout <<"Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    // if(jentry%500==0) cout <<"Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    if(jentry%50000==0){
      time_t now = time(0);
      TString dt = ctime(&now); dt.ReplaceAll("\n"," ");
      outTXTfile << dt << "\t - \t Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    }
    
    double evt_weight_original = lumi_scaling;
    if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && npu>0) evt_weight_original=lumi_scaling*hPileupSF->GetBinContent(hPileupSF->GetXaxis()->FindBin(pileup_reweighting_npu?npu:nvtx));
    if(usePtSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && (sampleName.Contains("WJets") && !sampleName.Contains("Fake")) ){
      if(MuGen_charge>0||Mu_charge>0) evt_weight_original*=hWPtSFPos->GetBinContent(hWPtSFPos->GetXaxis()->FindBin(W_pt>0?W_pt:WGen_pt));
      // else if(MuGen_charge<0||Mu_charge<0) evt_weight_original*=hWPtSFNeg->GetBinContent(hWPtSFNeg->GetXaxis()->FindBin(W_pt>0?W_pt:WGen_pt));
    }
    
    int runopt = r->Rndm()<0.457451 ? 0 : 1; // divide MC according to Run2011A and Run2011B statistics (if cut on pileup the 0.457... must be changed accordingly!!!
    double TRG_TIGHT_ISO_muons_SF = 1;
    // THIS MUST BE CHECKED !!!!!
    
    if(useEffSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
      TRG_TIGHT_ISO_muons_SF = SF_HLT->GetBinContent(SF_HLT->FindBin(Mu_charge,Mu_eta,Mu_pt))
                              *SF_TIGHT_ISO->GetBinContent(SF_TIGHT_ISO->FindBin(Mu_eta,Mu_pt));
      // hEffSF_MuId_eta_2011[runopt]->Eval(Mu_eta)*hEffSF_Iso_eta_2011[runopt]->Eval(Mu_eta)*hEffSF_HLT_eta_2011->Eval(Mu_eta);
    }
    // cout << "TRG_TIGHT_ISO_muons_SF= " << TRG_TIGHT_ISO_muons_SF << endl;
    // if(!(IS_MC_CLOSURE_TEST || isMCorDATA==0) && run<175832) continue; // TEMPORARY TO TEST ROCHESTER CORRECTIONS
    
    if(controlplots){
      common_stuff::plot1D("hnvtx_noWeights", nvtx, 1, h_1d, 50,0,50 );
      if(IS_MC_CLOSURE_TEST || isMCorDATA==0)
          common_stuff::plot1D("hPileUp_Fall11_noWeights", npu, 1, h_1d, 50,0,50 );
    }
    
    if(use_syst_ewk_Alcaraz && (sampleName.Contains("WJets") && !sampleName.Contains("Fake"))){
      if(FSRWeight>-99){ 
        evt_weight_original *= FSRWeight;
        // cout << "FSRWeight= " << FSRWeight << endl;
      }
    }

    //------------------------------------------------------
    // start loop on eta_max cuts
    //------------------------------------------------------    
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      
      //------------------------------------------------------
      // start loop on mass hypotheses
      //------------------------------------------------------
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        // int jWmass_MeV = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassSkipNSteps);
        int jWmass_MeV = WMass::Wmass_values_array[j];
        if(!(sampleName.Contains("WJets") && !sampleName.Contains("Fake")) && WMass::WMassNSteps!=j) continue;
        // double iWmass_GeV = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassSkipNSteps)/1e3;
        double iWmass_GeV = jWmass_MeV/1e3;
        
        //------------------------------------------------------
        // compute weight for mass hypotheses wiht Breit-Wigner reweighting or LHE
        //------------------------------------------------------
        double weight_i=1;
        if(useGenVar){
          if(!contains_LHE_weights){

            double gamma=2.085; /*HARD CODED TO PDG VALUE*/
            weight_i = common_stuff::BWweight(WGen_m, iWmass_GeV, gen_mass_value_MeV, gamma);

          }else{
            weight_i = LHE_weight [ WMass::LHE_mass_central_index - (WMass::WMassNSteps + j)*WMass::WMassSkipNSteps ];
            // cout << "j= " << j << " index= " << (WMass::LHE_mass_central_index + (-WMass::WMassNSteps + j)*WMass::WMassSkipNSteps) << endl;
          }
        }
        double evt_weight=evt_weight_original*weight_i;
          
        //------------------------------------------------------
        // retrieve and use gen-level info
        //------------------------------------------------------
        if(useGenVar){
          if(WGen_m>0  && WMass::WMassNSteps==j ){ // only for signal event
            TString MuGenCharge_str = MuGen_charge>0? "Pos" : "Neg"; 
            
            double MuGen_var_jacobian[WMass::NFitVar-1] = {2*MuGen_pt/iWmass_GeV,WGen_mt/iWmass_GeV,2*NuGen_pt/iWmass_GeV};
            double MuGen_var_NotScaled[WMass::NFitVar-1] = {MuGen_pt,WGen_mt,NuGen_pt};
            // AVOID OVERFLOW BIN TO BE FILLED
              for(int k=0;k<WMass::NFitVar-1;k++){
              if(MuGen_var_jacobian[k]>=xmax) MuGen_var_jacobian[k]=xmax-binsize2/2;
            
            if(WMass::PDF_members<2){
              for(int k=0;k<WMass::NFitVar-1;k++){
                common_stuff::plot1D(Form("hW%s_%sNonScaled_1_Gen_eta%s_%d",MuGenCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass_MeV),
                                    MuGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k],WMass::fit_xmax[k] );
                // mass cut not meaningful for W case
                common_stuff::plot1D(Form("hW%s_%sNonScaled_2_ZGenMassCut_eta%s_%d",MuGenCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass_MeV),
                                    MuGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k],WMass::fit_xmax[k] );
              }
                
              if(TMath::Abs(MuGen_eta)<WMass::etaMaxMuons[i]){
                for(int k=0;k<WMass::NFitVar-1;k++){
                  common_stuff::plot1D(Form("hW%s_%sNonScaled_3_Mu1GenCut_eta%s_%d",MuGenCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass_MeV),
                                  MuGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k],WMass::fit_xmax[k] );
                  // second lepton (i.e. neutrino) not detected
                  // if(TMath::Abs(MuNegGen_eta)<2.4){
                    common_stuff::plot1D(Form("hW%s_%sNonScaled_4_Mu2GenCut_eta%s_%d",MuGenCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass_MeV),
                                  MuGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k],WMass::fit_xmax[k] );
                  // }
                }
              }
            }
          }
        }
          
        if(!useGenVar || W_mt>0){ // dummy thing to separate between sig and bkg in W+Jets (useless)
          TString MuCharge_str = Mu_charge>0? "Pos" : "Neg"; 
          
          for(int m=0; m<WMass::NVarRecoilCorr; m++){
            TString toys_str = "";
            if(WMass::NVarRecoilCorr>1) toys_str = Form("_RecoilCorrVar%d",m);
            
            //------------------------------------------------------
            // start reco event selection
            //------------------------------------------------------
            if(evtHasGoodVtx && evtHasTrg /* && Mu_charge>0 */ 
                && nTrgMuons==1 // LATEST ADD-ON
                ){
            
              if(controlplots && m==0 && i==0 && j==0 && WMass::PDF_members<2){
                common_stuff::plot1D("hnvtx_1_TrgAndGoodVtx", nvtx, evt_weight, h_1d, 50,0,50 );
                if(IS_MC_CLOSURE_TEST || isMCorDATA==0)
                  common_stuff::plot1D("hPileUp_Fall11_Sig_1_TrgAndGoodVtx", npu, evt_weight, h_1d, 50,0,50 );
              }
              
              TLorentzVector mu, muCentralCorr, nuCentralCorr; //TLorentzVector of the reconstructed muon
              //Set TLorentzVector of mu
              mu.SetPtEtaPhiM(Mu_pt,Mu_eta,Mu_phi,Mu_mass);
              muCentralCorr.SetPtEtaPhiM(Mu_pt,Mu_eta,Mu_phi,Mu_mass);
              
              //------------------------------------------------------------------------------------------------
              // Apply recoil corrections
              //------------------------------------------------------------------------------------------------
              int rapBin=1;
              if(fabs(WGen_rap)<1) rapBin=1;
              if(fabs(WGen_rap)>=1 && fabs(WGen_rap)<=1.25) rapBin=125;
              if(fabs(WGen_rap)>=1.25 && fabs(WGen_rap)<=1.5) rapBin=150;
              if(fabs(WGen_rap)>=1.5 && fabs(WGen_rap)<=1.75) rapBin=175;
              if(fabs(WGen_rap)>=1.75 && fabs(WGen_rap)<=2.0) rapBin=200;
              if(fabs(WGen_rap)>2.0) rapBin=201;

              int vtxBin=rapBin;
              
              double pfmet_bla,pfmetphi_bla,pfmet_blaCentralCorr,pfmetphi_blaCentralCorr;
              if(use_PForNoPUorTKmet==0){
                pfmet_bla = pfmet;
                pfmetphi_bla = pfmet_phi;
                pfmet_blaCentralCorr = pfmet;
                pfmetphi_blaCentralCorr = pfmet_phi;
              }else if(use_PForNoPUorTKmet==1){
                pfmet_bla = nopumet;
                pfmetphi_bla = nopumet_phi;
                pfmet_blaCentralCorr = nopumet;
                pfmetphi_blaCentralCorr = nopumet_phi;
              }else if(use_PForNoPUorTKmet==2){
                pfmet_bla = tkmet;
                pfmetphi_bla = tkmet_phi;
                pfmet_blaCentralCorr = tkmet;
                pfmetphi_blaCentralCorr = tkmet_phi;
              }else{
                cout << "MET TYPE OPTION use_PForNoPUorTKmet: " << use_PForNoPUorTKmet << " NOT DEFINED, USE 0:PF, 1:NOPU, 2:TK" << endl;
                return;
              }
                            
              if(useRecoilCorr==1 && use_PForNoPUorTKmet<3 && (sampleName.Contains("WJets") && !sampleName.Contains("Fake"))){ // use Rochester Recoil corrections if required

                if(Mu_charge>0){ // W plus corrections
                  correctorRecoil_W_Pos_->CorrectType2( pfmet_bla, pfmetphi_bla,
                                    WGen_pt, WGen_phi,
                                    mu.Pt(), mu.Phi(),
                                    u1_dummy, u2_dummy,
                                    RecoilCorrResolutionNSigmaU2, RecoilCorrResolutionNSigmaU1, RecoilCorrScaleNSigmaU1,
                                   vtxBin,doSingleGauss);                                      
                  correctorRecoil_W_Pos_->CorrectType2( pfmet_blaCentralCorr, pfmetphi_blaCentralCorr,
                                    WGen_pt, WGen_phi,
                                    muCentralCorr.Pt(), muCentralCorr.Phi(),
                                    u1_dummy, u2_dummy,
                                    0, 0, 0,
                                   vtxBin,doSingleGauss);                                      

                }else{ // W minus corrections
                  correctorRecoil_W_Neg_->CorrectType2( pfmet_bla, pfmetphi_bla,
                                    WGen_pt, WGen_phi,
                                    mu.Pt(), mu.Phi(),
                                    u1_dummy, u2_dummy,
                                    RecoilCorrResolutionNSigmaU2, RecoilCorrResolutionNSigmaU1, RecoilCorrScaleNSigmaU1,
                                    vtxBin,doSingleGauss);
                  correctorRecoil_W_Neg_->CorrectType2( pfmet_blaCentralCorr, pfmetphi_blaCentralCorr,
                                    WGen_pt, WGen_phi,
                                    muCentralCorr.Pt(), muCentralCorr.Phi(),
                                    u1_dummy, u2_dummy,
                                    0, 0, 0,
                                    vtxBin,doSingleGauss);
                }
              }
              
              if(usePhiMETCorr==1){ // use MET Phi correction if required
                pair<double, double> pfmet_phicorr = common_stuff::getPhiCorrMET( pfmet_bla, pfmetphi_bla, nvtx, !sampleName.Contains("DATA"));
                pfmet_bla = pfmet_phicorr.first;
                pfmetphi_bla = pfmet_phicorr.second;
              }
              
              //------------------------------------------------------------------------------------------------
              // Apply muon corrections
              //------------------------------------------------------------------------------------------------
              //use rochester correction if required
              if(useMomentumCorr==1){ // use Rochester Momentum scale corrections if required
                if(IS_MC_CLOSURE_TEST || isMCorDATA==0){
                  // IN THE LAST VERSION NO RUN DEPENDENCE IN MC (TO be CHECKED)
                  // int runopt = r->Rndm()<0.457451 ? 0 : 1; // smear MC according to Run2011A and Run2011B statistics (if cut on pileup the 0.457... must be changed accordingly!!!
                  rmcor44X->momcor_mc(mu, Mu_charge, smearRochCorrByNsigma/* , runopt */);
                  rmcor44X->momcor_mc(muCentralCorr, Mu_charge, 0/* , runopt */);
                }
                else{
                  rmcor44X->momcor_data(mu, Mu_charge, smearRochCorrByNsigma, run<175832 ? 0 : 1 );
                  rmcor44X->momcor_data(muCentralCorr, Mu_charge, 0, run<175832 ? 0 : 1 );
                }
              }else if(useMomentumCorr==2){ // use MuscleFit Momentum scale corrections if required
                corrector->applyPtCorrection(mu,Mu_charge);
                corrector->applyPtCorrection(muCentralCorr,Mu_charge);
              }
              
              TLorentzVector mubla,mublaCentralCorr,nu,W,WCentralCorr; //TLorentzVector of the reconstructed W
              nu.SetPtEtaPhiM(pfmet_bla,0,pfmetphi_bla,0);
              nuCentralCorr.SetPtEtaPhiM(pfmet_blaCentralCorr,0,pfmetphi_blaCentralCorr,0);
              mubla.SetPtEtaPhiM(mu.Pt(),0,mu.Phi(),0);
              mublaCentralCorr.SetPtEtaPhiM(muCentralCorr.Pt(),0,muCentralCorr.Phi(),0);
              W = mubla + nu;
              WCentralCorr = mublaCentralCorr + nuCentralCorr;
              // W = mu + nu;      // <<<<<<------ LUCA BUGGY FOR TEST

              // double Mu_var_jacobian[WMass::NFitVar] = {2*mu.Pt()/iWmass_GeV,W.Mt()/iWmass_GeV,2*nu.Pt()/iWmass_GeV,W.Mt()/iWmass_GeV}; // SCALED VARIABLE
              double Mu_var_NotScaled[WMass::NFitVar] = {mu.Pt(),W.Mt(),nu.Pt(),W.Mt()}; // SCALED VARIABLE 
              double Mu_var_NotScaledCentralCorr[WMass::NFitVar] = {muCentralCorr.Pt(),WCentralCorr.Mt(),nuCentralCorr.Pt(),WCentralCorr.Mt()}; // SCALED VARIABLE 
              // double Mu_var_NotScaled[WMass::NFitVar] = {mu.Pt(),W.Mt()*W.Mt(),nu.Pt()}; // SCALED VARIABLE LUCA TEMP!!!
              double dphiGen = MuGen_phi-NuGen_phi;
              if(dphiGen > TMath::Pi()) dphiGen -= 2*TMath::Pi();
              if(dphiGen < -TMath::Pi()) dphiGen += 2*TMath::Pi();
              // double Mu_var_NotScaledGen[WMass::NFitVar] = {MuGen_pt,WGen_mt,WGen_mt,dphiGen}; // SCALED VARIABLE
              // LUCA ADD TO AVOID OVERFLOW
                // for(int k=0;k<WMass::NFitVar;k++)
                // if(Mu_var_jacobian[k]>=xmax) Mu_var_jacobian[k]=xmax-binsize2/2;
              
              int wmass1 = iWmass_GeV*1e3;

              //------------------------------------------------------
              // good event with mu from W candidate within acceptance
              //------------------------------------------------------
              if( /* Z_mass>50  not possible to apply something similar to Z mass cut*/
                  TMath::Abs(muCentralCorr.Eta())<WMass::etaMaxMuons[i] && muCentralCorr.Pt()>WMass::sel_xmin[0]
                ){
                // muon candidate is passing tight, iso, dxy requirements
                if(MuIsTight 
                   && MuRelIso<0.12 // REL ISO
                   // && MuRelIso*mu.Pt()<1  // ABS ISO
                   && Mu_dxy<0.02 
                && noTrgMuonsLeadingPt<10
                ){
                  
                  if(controlplots && m==0 && i==0 && j==0 && WMass::PDF_members<2 && WMass::WMassNSteps==j ){
                    common_stuff::plot1D("hnvtx_5_RecoCut", nvtx, evt_weight, h_1d, 50,0,50 );
                    if(IS_MC_CLOSURE_TEST || isMCorDATA==0) 
                      common_stuff::plot1D("hPileUp_Fall11_5_RecoCut", npu, evt_weight, h_1d, 50,0,50 );
                  }

                    for(int k=0;k<WMass::NFitVar;k++)
                    if(m==0 && WMass::PDF_members<2) common_stuff::plot1D(Form("hW%s_%sNonScaled_5_RecoCut_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass_MeV),
									  Mu_var_NotScaled[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, WMass::fit_xmin[k],WMass::fit_xmax[k] );
		  
                  //------------------------------------------------------
                  // PLOT OF MET WITHIN THE WHOLE RANGE
                  //------------------------------------------------------
                  if(WCentralCorr.Pt()<WMass::WpTcut && m==0 && j==WMass::WMassNSteps && WMass::PDF_members<2){
                    if(
                       Mu_var_NotScaledCentralCorr[0] > WMass::sel_xmin[0] && Mu_var_NotScaledCentralCorr[0] < WMass::sel_xmax[0]
                       && Mu_var_NotScaledCentralCorr[1] > WMass::sel_xmin[1] && Mu_var_NotScaledCentralCorr[1] < WMass::sel_xmax[1]
                       // && Mu_var_NotScaled[2] > WMass::sel_xmin[2] && Mu_var_NotScaled[2] < WMass::sel_xmax[2]
                                   )
                      common_stuff::plot1D(Form("hW%s_%sNonScaled_SelRange_8_JetCut_pdf%d-0%s_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[2].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,toys_str.Data(),eta_str.Data(),jWmass_MeV),
                         Mu_var_NotScaled[2] ,evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 
                         50*(WMass::sel_xmax[2]-WMass::sel_xmin[2])/(WMass::fit_xmax[2]-WMass::fit_xmin[2]), WMass::sel_xmin[2],WMass::sel_xmax[2] );
                  }
		  
                  //------------------------------------------------------------------------------------------------
                  // APPLY MET CUT
                  //------------------------------------------------------------------------------------------------
                  if(pfmet_blaCentralCorr>WMass::sel_xmin[2]/* WMass::fit_xmin[2] *//* 25 */){
                  
                    if(controlplots && m==0 && i==0 && j==0 && WMass::PDF_members<2  && WMass::WMassNSteps==j ){
                      common_stuff::plot1D("hnvtx_6_METCut", nvtx, evt_weight, h_1d, 50,0,50 );
                      if(IS_MC_CLOSURE_TEST || isMCorDATA==0)
                        common_stuff::plot1D("hPileUp_Fall11_6_METCut", npu, evt_weight, h_1d, 50,0,50 );
                    }

                      for(int k=0;k<WMass::NFitVar;k++)
                      if(m==0 && WMass::PDF_members<2) common_stuff::plot1D(Form("hW%s_%sNonScaled_6_METCut_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass_MeV),
                                    Mu_var_NotScaled[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, WMass::fit_xmin[k],WMass::fit_xmax[k] );
                    
                    //------------------------------------------------------
                    // cut on W recoil (BY DEFAULT IS 15)
                    //------------------------------------------------------
                    if(WCentralCorr.Pt()<WMass::WpTcut){ // DEFAULT IS WMass::WpTcut
                        for(int k=0;k<WMass::NFitVar;k++)
                        if(m==0 && WMass::PDF_members<2 && WMass::WMassNSteps==j ) common_stuff::plot1D(Form("hW%s_%sNonScaled_7_RecoilCut_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass_MeV),
									      Mu_var_NotScaled[k], evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 50, WMass::fit_xmin[k],WMass::fit_xmax[k] );
                      
                      if(controlplots && m==0 && i==0 && j==0 && WMass::PDF_members<2){
                        common_stuff::plot1D("hnvtx_7_RecoilCut", nvtx, evt_weight, h_1d, 50,0,50 );
                        if(IS_MC_CLOSURE_TEST || isMCorDATA==0)
                          common_stuff::plot1D("hPileUp_Fall11_7_RecoilCut", npu, evt_weight, h_1d, 50,0,50 );
                      }

                      //------------------------------------------------------------------------------------------------
                      //  PLOTS OF FIT VARIABLES WITHIN THE WHOLE RANGE
                      //------------------------------------------------------------------------------------------------
                      if(m==0 && j==WMass::WMassNSteps && WMass::PDF_members<2){
                        if(
                            // Mu_var_NotScaled[0] > WMass::sel_xmin[0] && Mu_var_NotScaled[0] < WMass::sel_xmax[0] && 
                            Mu_var_NotScaledCentralCorr[1] > WMass::sel_xmin[1] && Mu_var_NotScaledCentralCorr[1] < WMass::sel_xmax[1]
                            && Mu_var_NotScaledCentralCorr[2] > WMass::sel_xmin[2] && Mu_var_NotScaledCentralCorr[2] < WMass::sel_xmax[2]
                           )
                          common_stuff::plot1D(Form("hW%s_%sNonScaled_SelRange_8_JetCut_pdf%d-0%s_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[0].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,toys_str.Data(),eta_str.Data(),jWmass_MeV),
                              Mu_var_NotScaled[0] ,evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 
                              50*(WMass::sel_xmax[0]-WMass::sel_xmin[0])/(WMass::fit_xmax[0]-WMass::fit_xmin[0]), WMass::sel_xmin[0],WMass::sel_xmax[0] );
                        if(
                            Mu_var_NotScaledCentralCorr[0] > WMass::sel_xmin[0] && Mu_var_NotScaledCentralCorr[0] < WMass::sel_xmax[0]
                            // && Mu_var_NotScaled[1] > WMass::sel_xmin[1] && Mu_var_NotScaled[1] < WMass::sel_xmax[1]
                            && Mu_var_NotScaledCentralCorr[2] > WMass::sel_xmin[2] && Mu_var_NotScaledCentralCorr[2] < WMass::sel_xmax[2]
                           )
                          common_stuff::plot1D(Form("hW%s_%sNonScaled_SelRange_8_JetCut_pdf%d-0%s_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,toys_str.Data(),eta_str.Data(),jWmass_MeV),
                              Mu_var_NotScaled[1] ,evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 
                              50*(WMass::sel_xmax[1]-WMass::sel_xmin[1])/(WMass::fit_xmax[1]-WMass::fit_xmin[1]), WMass::sel_xmin[1],WMass::sel_xmax[1] );
                      
                        if(
                            controlplots && 
                            m==0 && WMass::WMassNSteps==j){
                            double fMet;fMet=pfmet_bla;
                            double fMPhi;fMPhi=pfmetphi_bla;
                            double fU1;
                            double fU2;
                            double fWPt;fWPt=WGen_pt;
                            double fWPhi;fWPhi=WGen_phi;
                            double fPtVisual;fPtVisual=MuGen_pt;
                            double fPhiVisual;fPhiVisual=MuGen_phi;
                            common_stuff::calculateU1U2( fMet,  
                                                         fMPhi,  
                                                         fWPt,  
                                                         fWPhi,  
                                                         fPtVisual,  
                                                         fPhiVisual,  
                                                         fU1, 
                                                         fU2
                                                        );
                            common_stuff::plot2D(Form("U1_vs_Wpt_gen_eta%s_%d",eta_str.Data(),jWmass_MeV),
                                          fWPt,fU1, evt_weight*TRG_TIGHT_ISO_muons_SF, 
                                          h_2d, 100,0,20,100,-10,10 );
                            common_stuff::plot2D(Form("U2_vs_Wpt_gen_eta%s_%d",eta_str.Data(),jWmass_MeV),
                                          fWPt,fU2, evt_weight*TRG_TIGHT_ISO_muons_SF, 
                                          h_2d, 100,0,20,100,-10,10 );
                            fWPt=WGen_pt;
                            fWPhi=WGen_phi;
                            fPtVisual=Mu_pt;
                            fPhiVisual=Mu_phi;
                            common_stuff::calculateU1U2( fMet,  
                                                         fMPhi,  
                                                         fWPt,  
                                                         fWPhi,  
                                                         fPtVisual,  
                                                         fPhiVisual,  
                                                         fU1, 
                                                         fU2
                                                        );
                            common_stuff::plot2D(Form("U1_vs_Wpt_reco_eta%s_%d",eta_str.Data(),jWmass_MeV),
                                          fWPt,fU1, evt_weight*TRG_TIGHT_ISO_muons_SF, 
                                          h_2d, 100,0,20,100,-10,10 );
                            common_stuff::plot2D(Form("U2_vs_Wpt_reco_eta%s_%d",eta_str.Data(),jWmass_MeV),
                                          fWPt,fU2, evt_weight*TRG_TIGHT_ISO_muons_SF, 
                                          h_2d, 100,0,20,100,-10,10 );
                      
                        }
                      }
                      
                      //------------------------------------------------------------------------------------------------
                      // APPLY FIT RANGE BOX -- based on central correction 
                      //------------------------------------------------------------------------------------------------ 

                      if(
                         Mu_var_NotScaledCentralCorr[0] > WMass::sel_xmin[0] && Mu_var_NotScaledCentralCorr[0] < WMass::sel_xmax[0]
                         && Mu_var_NotScaledCentralCorr[1] > WMass::sel_xmin[1] && Mu_var_NotScaledCentralCorr[1] < WMass::sel_xmax[1]
                         && Mu_var_NotScaledCentralCorr[2] > WMass::sel_xmin[2] && Mu_var_NotScaledCentralCorr[2] < WMass::sel_xmax[2]
                         ){
                                          
                        if(controlplots && m==0 && i==0 && j==0 && WMass::PDF_members<2){
                          common_stuff::plot1D("hnvtx_8_JetCut", nvtx, evt_weight, h_1d, 50,0,50 );
                          if(IS_MC_CLOSURE_TEST || isMCorDATA==0)
                            common_stuff::plot1D("hPileUp_Fall11_8_JetCut", npu, evt_weight, h_1d, 50,0,50 );
                        }

                        //------------------------------------------------------------------------------------------------
                        // SET THE PDF WEIGHT
                        //------------------------------------------------------------------------------------------------     
			
                        // std::cout << "event= " << jentry << " mw0= " << mw0 << " iWmass_GeV= " << iWmass_GeV << " WGen_m= " << WGen_m << " weight_i= " << weight_i << std::endl;
                        double lha_weight = 1;
                        // double lha_weight = LHAPDF::xfx(0,x1,Q,fl1)*LHAPDF::xfx(0,x2,Q,fl2) / (LHAPDF::xfx(1,x1,Q,fl1)*LHAPDF::xfx(1,x2,Q,fl2));
                        double weight_old = 1;
                        #ifdef LHAPDF_ON
                          if(parton1_x>1e-7 && parton2_x>1e-7 && parton1_x<1 && parton2_x<1)
                            weight_old = !sampleName.Contains("DATA") ? (LHAPDF::xfx(1,parton1_x,scalePDF,parton1_pdgId)*LHAPDF::xfx(1,parton2_x,scalePDF,parton2_pdgId)) : 1;
                        #endif
                        if(m==0){
                          common_stuff::plot1D("hPDF_x1",
                                    TMath::Log10(parton1_x), 1, h_1d, 100,-4,0 );
                          common_stuff::plot1D("hPDF_x1unweighted",
                                    TMath::Log10(parton1_x),1/weight_old, h_1d, 100,-4,0 );
                          common_stuff::plot1D("hPDF_x2",
                                    TMath::Log10(parton2_x), 1, h_1d, 100,-4,0 );
                          common_stuff::plot1D("hPDF_x2unweighted",
                                    TMath::Log10(parton2_x),1/weight_old, h_1d, 100,-4,0 );
                        }
                        // cout << "scalePDF= " << scalePDF << " parton1_x= " << parton1_x << " parton1_pdgId= " << parton1_pdgId 
                        // << " parton2_x= " << parton2_x << " parton2_pdgId= " << parton2_pdgId << endl;
                        // cout << " LHAPDF::xfx(0,parton1_x,scalePDF,parton1_pdgId)= LHAPDF::xfx(0,"<<parton1_x<<","<<scalePDF<<","<<parton1_pdgId<<")= " << LHAPDF::xfx(0,parton1_x,scalePDF,parton1_pdgId) << endl;
                        // cout << " LHAPDF::xfx(0,parton2_x,scalePDF,parton2_pdgId)= LHAPDF::xfx(0,"<<parton2_x<<","<<scalePDF<<","<<parton2_pdgId<<")= " << LHAPDF::xfx(0,parton2_x,scalePDF,parton2_pdgId) << endl;
                        // cout << " LHAPDF::xfx(1,parton1_x,scalePDF,parton1_pdgId)= LHAPDF::xfx(1,"<<parton1_x<<","<<scalePDF<<","<<parton1_pdgId<<")= " << LHAPDF::xfx(1,parton1_x,scalePDF,parton1_pdgId) << endl;
                        // cout << " LHAPDF::xfx(1,parton2_x,scalePDF,parton2_pdgId)= LHAPDF::xfx(1,"<<parton2_x<<","<<scalePDF<<","<<parton2_pdgId<<")= " << LHAPDF::xfx(1,parton2_x,scalePDF,parton2_pdgId) << endl;

                        for(int h=0; h<WMass::PDF_members; h++){
                          if(!contains_LHE_weights || (!sampleName.Contains("DATA") && WMass::PDF_sets>0 && WMass::PDF_sets!=generated_PDF_set && WMass::PDF_members!=generated_PDF_member)){
                            double weight_new = 1;
                            #ifdef LHAPDF_ON
                              LHAPDF::usePDFMember(0,h);
                              if(parton1_x>1e-7 && parton2_x>1e-7 && parton1_x<1 && parton2_x<1)
                                weight_new = (LHAPDF::xfx(0,parton1_x,scalePDF,parton1_pdgId)*LHAPDF::xfx(0,parton2_x,scalePDF,parton2_pdgId));
                            #endif
                            lha_weight = weight_new/weight_old;
                            if(m==0) common_stuff::plot1D(Form("hPDF_weights_%d_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h),
                                    lha_weight, 1, h_1d, 100, 0, 2 );
                            // cout << " lha_weight= " << lha_weight << endl;
                          }else if(contains_LHE_weights){
                            lha_weight = LHE_weight[PDF_reweighting_central_Index+h];
                          }

                          //------------------------------------------------------------------------------------------------
                          // END SET PDF WEIGHT and THOSE SHOULD BE THE FINAL HISTOGRAMS TO FIT 
                          // PLOTS OF FIT VARIABLES WITHIN THE FIT RANGE
                          //------------------------------------------------------
                          for(int k=0;k<WMass::NFitVar;k++){
                            common_stuff::plot1D(Form("hW%s_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass_MeV),
                                    Mu_var_NotScaled[k] ,evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight, h_1d, 
                                    50, WMass::fit_xmin[k],WMass::fit_xmax[k] );
                          }


                          //------------------------------------------------------------------------------------------------
                          // PLOTS FOR GIGI's TEST
                          //------------------------------------------------------------------------------------------------
                          if(controlplots){
                            common_stuff::plot1D(Form("deltaMT_W%s_8_JetCut_pdf%d-%d%s_eta%s",MuCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                     W.Mt()-WCentralCorr.Mt(),
                                     evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight, h_1d,
                                     200,-0.1,0.1 );
                            
                            common_stuff::plot1D(Form("deltaMET_W%s_8_JetCut_pdf%d-%d%s_eta%s",MuCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                     nu.Pt() - nuCentralCorr.Pt() ,
                                     evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight, h_1d,
                                     200,-0.1,0.1 );
                            
                            common_stuff::plot2D(Form("deltaMT2D_W%s_8_JetCut_pdf%d-%d%s_eta%s",MuCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                     WCentralCorr.Mt(), W.Mt()-WCentralCorr.Mt(),
                                     evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight, h_2d,
                                     50, WMass::sel_xmin[1],WMass::sel_xmax[1],
                                     500,-2.5,2.5 );
                            
                            common_stuff::plot2D(Form("deltaMET2D_W%s_8_JetCut_pdf%d-%d%s_eta%s",MuCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                     nu.Pt(),  nu.Pt() - nuCentralCorr.Pt(),
                                     evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight, h_2d, 
                                     50, WMass::sel_xmin[2],WMass::sel_xmax[2],
                                     500,-2.5,2.5 );
          
                            // common_stuff::plot1D(Form("hW%s_GenMass_8_JetCut_pdf%d-%d%s_eta%s_%d",MuCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass_MeV),
                                    // WGen_m ,evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight, h_1d, 
                                    // 100, 50,250 );
                                    
                            // common_stuff::plot1D(Form("hW%s_Recoil_8_JetCut_pdf%d-%d%s_eta%s_%d",MuCharge_str.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass_MeV),
                                    // W.Pt() ,evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight, h_1d, 
                                    // 50, 0, 25 );
                          }

                          // // 2D CORRELATION HISTOS
                          // common_stuff::plot2D(Form("hW%s_%sVs%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[1].Data(),WMass::FitVar_str[0].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass_MeV),
                                  // Mu_var_NotScaled[0],Mu_var_NotScaled[1],evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight, h_2d, 
                                  // // nbins, bins_Notscaled[0], nbins, bins_Notscaled[1]  );
                                  // 50, WMass::fit_xmin[0], WMass::fit_xmax[0], 50, WMass::fit_xmin[1], WMass::fit_xmax[1] );
                          // common_stuff::plot2D(Form("hW%s_%sVs%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[2].Data(),WMass::FitVar_str[0].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass_MeV),
                                  // Mu_var_NotScaled[0], Mu_var_NotScaled[2],evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight, h_2d, 
                                  // // nbins, bins_Notscaled[0], nbins, bins_Notscaled[2]  );
                                  // 50, WMass::fit_xmin[0], WMass::fit_xmax[0], 50, WMass::fit_xmin[2], WMass::fit_xmax[2] );
                          // common_stuff::plot2D(Form("hW%s_%sVs%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[2].Data(),WMass::FitVar_str[1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass_MeV),
                                  // Mu_var_NotScaled[1],Mu_var_NotScaled[2],evt_weight*TRG_TIGHT_ISO_muons_SF*lha_weight, h_2d, 
                                  // // nbins, bins_Notscaled[1], nbins, bins_Notscaled[2]  );
                                  // 50, WMass::fit_xmin[1], WMass::fit_xmax[1], 50, WMass::fit_xmin[2], WMass::fit_xmax[2] );
                        }
                        
                      }
                    }
                  }
                }else if(controlplots && m==0 && j==WMass::WMassNSteps){ // muon candidate is failing either tight ID, iso or dxy: QCD enriched region
                  
                //------------------------------------------------------
                // Control plots in the QCD enriched region
                //------------------------------------------------------
                  if(
                    MuRelIso<0.12 // single muon cuts (inverted REL iso (is <0.12 for signal) , no tight requirement)
                    // MuRelIso*mu.Pt()<1 // single muon cuts (inverted ABD iso (is <0.12 for signal) , no tight requirement)
                    && Mu_dxy>0.02 // single muon cuts (inverted iso (is <0.02 for signal) , no tight requirement)
                    // && W_pt<20
                    && WCentralCorr.Pt()<WMass::WpTcut // TEMPORARY
                  ){
                  
                    if(
                      Mu_var_NotScaledCentralCorr[0] > WMass::sel_xmin[0] && Mu_var_NotScaledCentralCorr[0] < WMass::sel_xmax[0]
                      && Mu_var_NotScaledCentralCorr[1] > WMass::sel_xmin[1] && Mu_var_NotScaledCentralCorr[1] < WMass::sel_xmax[1]
                      // && Mu_var_NotScaled[2] > WMass::sel_xmin[2] && Mu_var_NotScaled[2] < WMass::sel_xmax[2]
                     )
                      common_stuff::plot1D(Form("hW%s_%sNonScaled_SelRangeQCD_8_JetCut_pdf%d-0%s_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[2].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,toys_str.Data(),eta_str.Data(),jWmass_MeV),
                        Mu_var_NotScaled[2] ,evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 
                        50*(WMass::sel_xmax[2]-WMass::sel_xmin[2])/(WMass::fit_xmax[2]-WMass::fit_xmin[2]), WMass::sel_xmin[2],WMass::sel_xmax[2] );
                  
                    if(
                        // Mu_var_NotScaled[0] > WMass::sel_xmin[0] && Mu_var_NotScaled[0] < WMass::sel_xmax[0] && 
                        Mu_var_NotScaledCentralCorr[1] > WMass::sel_xmin[1] && Mu_var_NotScaledCentralCorr[1] < WMass::sel_xmax[1]
                        && Mu_var_NotScaledCentralCorr[2] > WMass::sel_xmin[2] && Mu_var_NotScaledCentralCorr[2] < WMass::sel_xmax[2]
                       )
                      common_stuff::plot1D(Form("hW%s_%sNonScaled_SelRangeQCD_8_JetCut_pdf%d-0%s_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[0].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,toys_str.Data(),eta_str.Data(),jWmass_MeV),
                          Mu_var_NotScaled[0] ,evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 
                          50*(WMass::sel_xmax[0]-WMass::sel_xmin[0])/(WMass::fit_xmax[0]-WMass::fit_xmin[0]), WMass::sel_xmin[0],WMass::sel_xmax[0] );
                    if(
                        Mu_var_NotScaledCentralCorr[0] > WMass::sel_xmin[0] && Mu_var_NotScaledCentralCorr[0] < WMass::sel_xmax[0]
                        // && Mu_var_NotScaled[1] > WMass::sel_xmin[1] && Mu_var_NotScaled[1] < WMass::sel_xmax[1]
                        && Mu_var_NotScaledCentralCorr[2] > WMass::sel_xmin[2] && Mu_var_NotScaledCentralCorr[2] < WMass::sel_xmax[2]
                       )
                      common_stuff::plot1D(Form("hW%s_%sNonScaled_SelRangeQCD_8_JetCut_pdf%d-0%s_eta%s_%d",MuCharge_str.Data(),WMass::FitVar_str[1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,toys_str.Data(),eta_str.Data(),jWmass_MeV),
                          Mu_var_NotScaled[1] ,evt_weight*TRG_TIGHT_ISO_muons_SF, h_1d, 
                          50*(WMass::sel_xmax[1]-WMass::sel_xmin[1])/(WMass::fit_xmax[1]-WMass::fit_xmin[1]), WMass::sel_xmin[1],WMass::sel_xmax[1] );
                  }
                }
              } // end if for good muon in acceptance
            } // end good reco event
          } // end momentum correction toys
        } // end dummy separation sig/bkg
      } // end W mass loop
    } // end muon eta loop
  } // end event loop
  // }
  
  outTXTfile.close();
  
  TFile*fout = new TFile(Form("%s/Wanalysis%s.root",outputdir.Data(),chunk_str.Data()),"RECREATE");
  
  if(!(sampleName.Contains("WJets") && !sampleName.Contains("Fake"))){
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        // int jWmass_MeV = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassSkipNSteps);
        int jWmass_MeV = WMass::Wmass_values_array[j];
        for(int k=0;k<WMass::NFitVar;k++){
          for(int h=0; h<WMass::PDF_members; h++){
            for(int m=0; m<WMass::NVarRecoilCorr; m++){
              TString toys_str = "";
              if(WMass::NVarRecoilCorr>1) toys_str = Form("_RecoilCorrVar%d",m);
              TString MuCharge_str[]={"Pos","Neg"};
              for(int MuCharge = 0; MuCharge < 2; MuCharge++){
                common_stuff::cloneHisto1D(Form("hW%s_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",MuCharge_str[MuCharge].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),WMass::WMassCentral_MeV), 
                                          Form("hW%s_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",MuCharge_str[MuCharge].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass_MeV), 
                                          h_1d);
                
                common_stuff::cloneHisto1D(Form("hW%s_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",MuCharge_str[MuCharge].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),WMass::WMassCentral_MeV), 
                                          Form("hW%s_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",MuCharge_str[MuCharge].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass_MeV), 
                                          h_1d);
              }
            }
          }
        }
      }
    }
  }

  common_stuff::writeOutHistos( fout, h_1d, h_2d );
  
  fout->cd();
  
  fout->Write();
  fout->Close();
  
}
