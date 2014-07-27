// UNCOMMENT TO USE PDF REWEIGHTING
//#define LHAPDF_ON

#ifdef LHAPDF_ON
  #include "LHAPDF/LHAPDF.h"
#endif 

#define Zanalysis_cxx
#include "Zanalysis.h"
#include "../includes/common.h"
// #include "rochcor_42X.h"
//#include "common_stuff.h"
#include "rochcor_44X_v3.h"
#include "MuScleFitCorrector.h"
#include "RecoilCorrector.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TGraphAsymmErrors.h>
#include <ctime>
#include <time.h>

void Zanalysis::Loop(int chunk, int Entry_ini, int Entry_fin, int IS_MC_CLOSURE_TEST, int isMCorDATA, TString outputdir, int buildTemplates, int useMomentumCorr, int smearRochCorrByNsigma, int useEffSF, int usePtSF, int useVtxSF, int controlplots, TString sampleName, int generated_PDF_set, int generated_PDF_member, int contains_PDF_reweight, int usePhiMETCorr, int useRecoilCorr, int RecoilCorrResolutionNSigmaU1, int RecoilCorrScaleNSigmaU1, int RecoilCorrResolutionNSigmaU2, int use_PForNoPUorTKmet, int use_syst_ewk_Alcaraz, int gen_mass_value_MeV, int contains_LHE_weights)
{

  if (fChain == 0) return;

  std::map<std::string, TH1D*> h_1d;
  std::map<std::string, TH2D*> h_2d;
  int counter=0;

  cout << "generated_PDF_set= "<<generated_PDF_set
       << " generated_PDF_member= " << generated_PDF_member
       << " contains_PDF_reweight= " << contains_PDF_reweight
       << " WMass::NtoysMomCorr= " << WMass::NtoysMomCorr
       << endl;

  TRandom3 *r = new TRandom3(0);

  TString chunk_str = chunk>0? Form("_chunk%d",chunk) : "";
  ofstream outTXTfile;
  outTXTfile.open(Form("%s/Zanalysis_EVlog%s.log",outputdir.Data(),chunk_str.Data()));
  if(!outputdir.Contains("../")) outputdir = "../"+outputdir;
  cout << "output filename= " << Form("%s/Zanalysis%s.root",outputdir.Data(),chunk_str.Data()) << endl;
  
  int PDF_reweighting_central_Index = -1;
  if(contains_LHE_weights && WMass::PDF_sets==229800) PDF_reweighting_central_Index = WMass::LHE_NNPDF2p3_NLO_central_index;
  if(contains_LHE_weights && WMass::PDF_sets==11000) PDF_reweighting_central_Index = WMass::LHE_CT10_NLO_central_index;
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
  
  TH1D*hWlikePos_R_WdivZ[WMass::PDF_members][WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1]; // used only to build templates
  TFile*finTemplates, *finEffSF, *finPileupSF, *finZPtSF;
  TGraphAsymmErrors*hEffSF_MuId_eta_2011[2],*hEffSF_Iso_eta_2011[2],*hEffSF_HLT_eta_2011/* ,*hEffSF_Iso_vtx_2011A,*hEffSF_Iso_vtx_2011B*/;
  TH1D*hPileupSF,*hZPtSF;
  
  if(buildTemplates){
    finTemplates = new TFile(Form("%s/../R_WdivZ_OnMC.root",outputdir.Data())); // used only to build templates
    if(!finTemplates){
      cout << "file R_WdivZ_OnMC.root is missing, impossible to build templates" << endl;
      return;
    }
  }
  if(useEffSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    finEffSF = new TFile("../utils/MuonEfficiencies_SF_2011_44X_DataMC.root"); // used only to build templates
    hEffSF_MuId_eta_2011[0]=(TGraphAsymmErrors*)finEffSF->Get("SF_TIGHT_nL8_2011A_eta__pt>20");
    hEffSF_MuId_eta_2011[1]=(TGraphAsymmErrors*)finEffSF->Get("SF_TIGHT_nL8_2011B_eta__pt>20");
    hEffSF_Iso_eta_2011[0]=(TGraphAsymmErrors*)finEffSF->Get("combRelPFISO12_2011A_eta__pt>20");
    hEffSF_Iso_eta_2011[1]=(TGraphAsymmErrors*)finEffSF->Get("combRelPFISO12_2011B_eta__pt>20");
    hEffSF_HLT_eta_2011=(TGraphAsymmErrors*)finEffSF->Get("SF_HLT_MuIso24_2011_eta__pt>30");
    // hEffSF_Iso_vtx_2011A=(TH1D*)finEffSF->Get("combRelPFISO12_2011A_vtx__pt>20_abseta<2.4");
    // hEffSF_Iso_vtx_2011B=(TH1D*)finEffSF->Get("combRelPFISO12_2011B_vtx__pt>20_abseta<2.4");
    if(!finEffSF){
      cout << "file MuonEfficiencies_SF_2011_44X_DataMC.root is missing, impossible to retrieve efficiency scale factors" << endl;
      return;
    }
  }
  if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    TString vtx_str = sampleName; vtx_str.ReplaceAll("Sig",""); vtx_str.ReplaceAll("Fake","");
    // finPileupSF = new TFile(Form("../utils/pileup_reweighting_%s.root",vtx_str.Data())); // used only to build templates
    finPileupSF = new TFile(Form("../utils/pileup/pileup_reweighting_Fall11.root")); // used only to build templates
    if(!finPileupSF){
      cout << "file " << Form("../utils/pileup/pileup_reweighting_Fall11.root") << " is missing, impossible to retrieve pileup reweighting factors" << endl;
      return;
    }else{
      hPileupSF=(TH1D*)finPileupSF->Get("hpileup_reweighting_Fall11");
    }
  }
  if(usePtSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){
    cout << "REWEIGHTING Z PT" << endl;
    finZPtSF = new TFile(Form("../utils/Zpt_reweighting.root")); // used only to build templates
    if(!finZPtSF){
      cout << "file " << Form("../utils/Zpt_reweighting.root") << " is missing, impossible to retrieve ZPt reweighting factors" << endl;
      return;
    }else{
      hZPtSF=(TH1D*)finZPtSF->Get("hZ_pt_Sig_eta0p6");
    }
  }

  static const int nbins=75;
  double bins_scaled[3][nbins+1]={{0.}};
  double bins_Notscaled[3][nbins+1]={{0.}};
  double binsize1=0.01,binsize2=0.04;
  double binsize;
  double xmin=0.6,xmax=1.8, x;
  for(int k=0;k<3;k++){
    x=xmin;
    binsize=binsize1;
    for(int i=0;i<nbins;i++){
      bins_scaled[k][i]=x;
      bins_Notscaled[k][i]=x*80/(k==1 ? 1 : 2); // mT has double range wrt pt, met
      if(x>1.2-binsize) binsize=binsize2;
      x+=binsize;
      // cout << "bins_scaled["<<k<<"]["<<i<<"]= "<< bins_scaled[k][i] << endl;
      // cout << "bins_Notscaled["<<k<<"]["<<i<<"]= "<< bins_Notscaled[k][i] << endl;
    }
    bins_scaled[k][nbins]=xmax;
    bins_Notscaled[k][nbins]=xmax*80/(k==1 ? 1 : 2);
    // cout << "bins_scaled["<<k<<"]["<<nbins<<"]= " <<bins_scaled[k][nbins] << endl;
    // cout << "bins_Notscaled["<<k<<"]["<<nbins<<"]= " <<bins_Notscaled[k][nbins] << endl;
    // cout << endl;
  }
  
  for(int i=0; i<WMass::etaMuonNSteps; i++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
            
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      
      if(!(sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig")) && WMass::WMassNSteps!=j) continue;
      
      // int jZmass_MeV = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMass_SkipNSteps;
      int jZmass_MeV = WMass::Zmass_values_array[j];

      for(int k=0;k<3;k++){
        for(int h=0; h<WMass::PDF_members; h++){
          if(buildTemplates){
            cout << Form("hR_WdivZ_WlikePos_%s_8_JetCut_pdf%d-%d_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),jZmass_MeV) << endl;
            hWlikePos_R_WdivZ[h][k][i][j]=(TH1D*)finTemplates->Get(Form("hR_WdivZ_WlikePos_%s_8_JetCut_pdf%d-%d_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),jZmass_MeV));
            hWlikePos_R_WdivZ[h][k][i][j]->Print();
          }
        }
      }
    
    }
  }

  // the following variables are dummy, but necessary to call the corrector.
  double u1_dummy = 0;
  double u2_dummy = 0;
  double fluc_dummy = 0;
  double zero_dummy = 0;
  int jetMult = 0; // set to zero;
  //for the lepPt, lepPhi, 2: lepton is on leg2;
  
  // Long64_t first_entry = 0;
  // Long64_t nentries = fChain->GetEntriesFast();
  Long64_t first_entry = Entry_ini;
  Long64_t nentries = Entry_fin;
  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==1) first_entry=nentries/2; // in case of closure test, DATA runs from N/2 to N
  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==0) nentries=nentries/2; // in case of closure test, MC runs from 0 to N/2
  if(IS_MC_CLOSURE_TEST==1) lumi_scaling=lumi_scaling*2; // in case of closure test, scaling must be multiplied by 2
  
  //To get the central value of the momentum correction
  // rochcor42X *rmcor42X = new rochcor42X();  // make the pointer of rochcor class
  int random_seed_start=67525;
  rochcor_44X_v3 *rmcor44X = WMass::NtoysMomCorr>1? new rochcor_44X_v3(random_seed_start) : new rochcor_44X_v3();  // make the pointer of rochcor class
  TString MuscleCard = (IS_MC_CLOSURE_TEST || isMCorDATA==0) ? "MuScleFit_2011_MC_44X" : "MuScleFit_2011_DATA_44X";
  TString fitParametersFile = MuscleCard+".txt";
  MuScleFitCorrector *corrector;
  if(useMomentumCorr==2){
    cout << "using MuscleFit card " << fitParametersFile << endl;
    corrector = new MuScleFitCorrector(fitParametersFile);
  }
  
  // double ZMassCentral_GeV = 91187.6;

  //////////////
  ////////   Initialize recoil corrections
  ////////

  // int use_PForNoPUorTKmet = 0; // 0:PF, 1:NOPU, 2:TK // TO BE CHANGED BY HAND FOR THE MOMENT!!!
  bool use_InclusiveAB_RecoilCorr = true; // TO BE CHANGED BY HAND FOR THE MOMENT!!!
  bool use_InclusiveNVTX_RecoilCorr = true; // TO BE CHANGED BY HAND FOR THE MOMENT!!!
  TString InclusiveNVTXSuffix="";
  if(use_InclusiveNVTX_RecoilCorr) InclusiveNVTXSuffix="_inclusiveNvtx";

  TString metSuffix=use_PForNoPUorTKmet==1?"_pfnoPU":"";
  if(use_PForNoPUorTKmet==2) metSuffix="_tkmet";
  if(use_PForNoPUorTKmet==0) metSuffix="_pfmet";
  
  /// TKMET type2
  std::string fileCorrectTo = Form("../RecoilCode/recoilfit_genZ%s_inclusiveNvtx_eta11_pol3_type2_oneGauss_x2Stat_ErrorFix_madgraph.root",metSuffix.Data());
  std::string fileZmmMC = Form("../RecoilCode/recoilfit_genZ%s_inclusiveNvtx_eta11_pol3_type2_oneGauss_x2Stat_ErrorFix_madgraph.root",metSuffix.Data());
  std::string fileZmmData = Form("../RecoilCode/recoilfit_DATA%s_inclusiveNvtx_eta11_pol3_type2_oneGauss_x2Stat_ErrorFix.root",metSuffix.Data());

  RecoilCorrector*  correctorRecoil_Z; // TYPE2

  correctorRecoil_Z = new RecoilCorrector(fileCorrectTo.c_str(),123456);
  correctorRecoil_Z->addDataFile(fileZmmData.c_str());
  correctorRecoil_Z->addMCFile(fileZmmMC.c_str());

  bool doSingleGauss=false;

  double ZWmassRatio = WMass::ZMassCentral_MeV/WMass::WMassCentral_MeV;
  
  Long64_t nbytes = 0, nb = 0;
  // for(Long64_t jentry=first_entry; jentry<nentries;jentry++) {
    for (Long64_t jentry=0; jentry<1e5;jentry++) { // TEMP !!!
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    // if(jentry%250000==0) cout <<"Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    if(jentry%25000==0) cout <<"Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    if(jentry%50000==0){
      time_t now = time(0);
      TString dt = ctime(&now); dt.ReplaceAll("\n"," ");
      outTXTfile << dt << "\t - \t Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    }
            
    // if(Z_mass<60 || Z_mass>120 || MuPos_pt<20 || MuNeg_pt<20 || TMath::Abs(MuPos_eta)>2.1 || TMath::Abs(MuNeg_eta)>2.1) continue;
    
    // if(contains_PDF_reweight){
      // for(int i_weight=0; i_weight< 100; i_weight++){
        // // cout << " " 
        // // << i_weight << " "
        // // << LHE_weight[i_weight+7] << " "
        // // << LHE_ren[i_weight+7] << " "
        // // << LHE_fac[i_weight+7] << " "
        // // << LHE_pdf[i_weight+7] 
        // // << endl;
        
        // hpdf_w[i_weight]->Fill(LHE_weight[i_weight+7]);

      // }
    // }
    
    // if(!(IS_MC_CLOSURE_TEST || isMCorDATA==0) && run>175832) continue; // TO TEST ROCHESTER CORRECTIONS ONLY ON RUN2011A
    // if(!(IS_MC_CLOSURE_TEST || isMCorDATA==0) && run<175832) continue; // TO TEST ROCHESTER CORRECTIONS ONLY ON RUN2011B
    
    if(nTrgMuons>2) continue; // NOT (YET) REQUIRED WHILE MAKING NTUPLES!!!! <<---------- it sohuld be already fixed in the ntuples now
    
    double evt_weight_original = lumi_scaling;
    // cout << "evt_weight_original " << evt_weight_original << endl;
    // TO BE CHECKED!!!
    if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && npu>0) evt_weight_original=lumi_scaling*hPileupSF->GetBinContent(hPileupSF->GetXaxis()->FindBin(npu));
    if(usePtSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && hZPtSF && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))) evt_weight_original*=hZPtSF->GetBinContent(hZPtSF->GetXaxis()->FindBin(Z_pt>0?Z_pt:ZGen_pt));
    
    int runopt = r->Rndm()<0.457451 ? 0 : 1;
    double MuPos_tight_muon_SF = 1;
    double MuNeg_tight_muon_SF = 1;
    
    if(useEffSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    // TO BE CHECKED!!!
      MuPos_tight_muon_SF = hEffSF_MuId_eta_2011[runopt]->Eval(MuPos_eta)*hEffSF_Iso_eta_2011[runopt]->Eval(MuPos_eta)*hEffSF_HLT_eta_2011->Eval(MuPos_eta);
      MuNeg_tight_muon_SF = hEffSF_MuId_eta_2011[runopt]->Eval(MuNeg_eta)*hEffSF_Iso_eta_2011[runopt]->Eval(MuNeg_eta)*hEffSF_HLT_eta_2011->Eval(MuPos_eta);
    }
    // cout << "MuPos_tight_muon_SF= " << MuPos_tight_muon_SF << endl;
    if((IS_MC_CLOSURE_TEST || isMCorDATA==0) && controlplots) 
      common_stuff::plot1D("hPileUp_Fall11",npu, 1, h_1d, 50,0,50);

    if(use_syst_ewk_Alcaraz && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){
      if(FSRWeight>-99){ 
        evt_weight_original *= FSRWeight;
        // cout << "FSRWeight= " << FSRWeight << endl;
      }
    }

    
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");

      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        
        if(!(sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig")) && WMass::WMassNSteps!=j) continue;
        // int jZmass_MeV = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMass_SkipNSteps;
        int jZmass_MeV = WMass::Zmass_values_array[j];
        // double iZmass_GeV = (WMass::ZMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMass_SkipNSteps)/1e3;
        double iZmass_GeV = WMass::Zmass_values_array[j]/1e3;
        
        // // BW REWEIGHTING
        double weight_i=1,bweight_i=1,lheweight_i=1;
        if(useGenVar){
          if(!contains_LHE_weights){
          double shat=0,gamma=2.141 /*HARD CODED TO PDG VALUE*/,mw0=0,mw_i=0;
          shat=ZGen_mass*ZGen_mass;
          // mw0=WMass::ZMassCentral_MeV;
          mw0=gen_mass_value_MeV/1e3;
          mw_i=iZmass_GeV;
          // ((shat - mw0^2)^2 + gamma^2 mw0^2) / ((shat - mw_i^2)^2 + gamma^2 mw_i^2)
          weight_i=(TMath::Power(shat - mw0*mw0,2) + TMath::Power(gamma*mw0,2)) / (TMath::Power(shat - mw_i*mw_i,2) + TMath::Power(gamma*mw_i,2));
          bweight_i = weight_i;
          // cout << "ZGen_mass = " << ZGen_mass << " mw0= " << mw0 << " mw_i= " << mw_i << " weight_i= " << weight_i << endl;
          }else{
          // cout << "LHE weight= " << (LHE_weight [ (WMass::LHE_mass_central_index + ( -WMass::WMassNSteps + j)*WMass::WMassSkipNSteps) ]) << " weight_i= " << weight_i << endl;
          // common_stuff::plot2D(Form("hCorrMassweights_eta%s_%d",eta_str.Data(),jZmass_MeV),
              // weight_i,LHE_weight [ (WMass::LHE_mass_central_index + ( -WMass::WMassNSteps + j)*WMass::WMassSkipNSteps) ], 1, 
              // h_2d, 200,0.9,1.1,200,0.9,1.1 );

            weight_i = LHE_weight [ (WMass::LHE_mass_central_index + ( -WMass::WMassNSteps + j)*WMass::WMassSkipNSteps) ];
            lheweight_i = weight_i;
            // cout << "jZmass_MeV= " << jZmass_MeV << " j= " << j << " index= " << (WMass::LHE_mass_central_index + ( -WMass::WMassNSteps + j)*WMass::WMassSkipNSteps) << endl;
          }
        }
        double evt_weight=evt_weight_original*weight_i;
        // cout << "evt_weight B-W " << evt_weight << endl;
        
        
        // SELECT EVENT WITH Z/W SCALED CUTS
        // gen stuff to be used only when required
        if(useGenVar){
          if(ZGen_mass>0 && WMass::WMassNSteps==j ){
            double MuPosGen_var_jacobian[3] = {2*MuPosGen_pt/WMass::ZMassCentral_MeV*1e3,ZGen_mt/WMass::ZMassCentral_MeV*1e3,2*MuNegGen_pt/WMass::ZMassCentral_MeV*1e3};
            double MuPosGen_var_NotScaled[3] = {MuPosGen_pt,ZGen_mt,MuNegGen_pt};
            // LUCA ADD TO AVOID OVERFLOW
            for(int k=0;k<3;k++)
              if(MuPosGen_var_jacobian[k]>=xmax) MuPosGen_var_jacobian[k]=xmax-binsize2/2;
            
            for(int k=0;k<3;k++)
              common_stuff::plot1D(Form("hWlikePos_%sNonScaled_1_Gen_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                    MuPosGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
              
              // common_stuff::plot1D(Form("hWlikePos_MGenNonScaled_1_GenBW_eta%s_%d",eta_str.Data(),jZmass_MeV),
                                    // ZGen_mass, bweight_i, h_1d, 50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio );
              // common_stuff::plot1D(Form("hWlikePos_MGenNonScaled_1_GenLHE_eta%s_%d",eta_str.Data(),jZmass_MeV),
                                    // ZGen_mass, lheweight_i, h_1d, 50, WMass::fit_xmin[1]*ZWmassRatio, WMass::fit_xmax[1]*ZWmassRatio );

            if(ZGen_mass>50){
              for(int k=0;k<3;k++)
                common_stuff::plot1D(Form("hWlikePos_%sNonScaled_2_ZGenMassCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                      MuPosGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
              
              if(TMath::Abs(MuPosGen_eta)<WMass::etaMaxMuons[i]){
                for(int k=0;k<3;k++)
                  common_stuff::plot1D(Form("hWlikePos_%sNonScaled_3_Mu1GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                        MuPosGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                
                if(TMath::Abs(MuNegGen_eta)<2.4){
                  for(int k=0;k<3;k++)
                    common_stuff::plot1D(Form("hWlikePos_%sNonScaled_4_Mu2GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                          MuPosGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                }
              }
            }
          }
        } // end gen stuff 
        
        if(!useGenVar || Z_mass>0){ // dummy thing to separate signal and background in DY+Jets (useless)
          for(int m=0; m<WMass::NtoysMomCorr; m++){
            
            TString toys_str = "";
            if(WMass::NtoysMomCorr>1) toys_str = Form("_MomCorrToy%d",m);
            
            if( evtHasGoodVtx && evtHasTrg && MuPos_pt>0){ // good reco event

              TLorentzVector muPosCorr,muNegCorr,muPosCorrCentral,muNegCorrCentral, muPosNoCorr,muNegNoCorr, Zcorr, ZNocorr, ZcorrCentral; //TLorentzVector of the reconstructed muon
              //Set TLorentzVector of mu, and mubar
              muPosCorr.SetPtEtaPhiM(MuPos_pt,MuPos_eta,MuPos_phi,MuPos_mass);
              muPosCorrCentral = muPosCorr;
              muPosNoCorr = muPosCorr;
              muNegCorr.SetPtEtaPhiM(MuNeg_pt,MuNeg_eta,MuNeg_phi,MuNeg_mass);
              muNegCorrCentral = muNegCorr;
              muNegNoCorr = muNegCorr;

              ZNocorr = muPosNoCorr + muNegNoCorr;

              //------------------------------------------------------------------------------------------------
              // Apply recoil corrections
              //------------------------------------------------------------------------------------------------

	      int rapBin=1;
              if(fabs(ZGen_rap)<1) rapBin=1;
              if(fabs(ZGen_rap)>=1 && fabs(ZGen_rap)<=1.25) rapBin=125;
              if(fabs(ZGen_rap)>=1.25 && fabs(ZGen_rap)<=1.5) rapBin=150;
              if(fabs(ZGen_rap)>=1.5 && fabs(ZGen_rap)<=1.75) rapBin=175;
              if(fabs(ZGen_rap)>=1.75 && fabs(ZGen_rap)<=2.0) rapBin=200;
              if(fabs(ZGen_rap)>2.0) rapBin=201;

              int vtxBin=rapBin;

              double pfmet_bla,pfmetphi_bla,pfmet_blaCentral,pfmetphi_blaCentral;
              if(use_PForNoPUorTKmet==0){
                pfmet_bla = pfmet;
                pfmetphi_bla = pfmet_phi;
              }else if(use_PForNoPUorTKmet==1){
                pfmet_bla = nopumet;
                pfmetphi_bla = nopumet_phi;
              }else if(use_PForNoPUorTKmet==2){
                pfmet_bla = tkmet;
                pfmetphi_bla = tkmet_phi;
              }else{
                cout << "MET TYPE OPTION use_PForNoPUorTKmet: " << use_PForNoPUorTKmet << " NOT DEFINED, USE 0:PF, 1:NOPU, 2:TK" << endl;
                return;
              }
              
              pfmet_blaCentral = pfmet_bla;
              pfmetphi_blaCentral = pfmetphi_bla;
              
              // cout << "pfmet_bla before " << pfmet_bla;
              if(useRecoilCorr==1 && use_PForNoPUorTKmet<3 && (sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){ // use Rochester Momentum scale corrections if required
                correctorRecoil_Z->CorrectType2( pfmet_bla, pfmetphi_bla,
                                  ZGen_pt, ZGen_phi,
                                  ZNocorr.Pt(), ZNocorr.Phi(),
                                  u1_dummy, u2_dummy,
                                  RecoilCorrResolutionNSigmaU2, RecoilCorrResolutionNSigmaU1, RecoilCorrScaleNSigmaU1,
                                 vtxBin,doSingleGauss); 
                
                correctorRecoil_Z->CorrectType2( pfmet_blaCentral, pfmetphi_blaCentral,
                                  ZGen_pt, ZGen_phi,
                                  ZNocorr.Pt(), ZNocorr.Phi(),
                                  u1_dummy, u2_dummy,
                                  RecoilCorrResolutionNSigmaU2, RecoilCorrResolutionNSigmaU1, RecoilCorrScaleNSigmaU1,
                                 vtxBin,doSingleGauss); 
                                 
              }
              // cout << " and after " << pfmet_bla << endl;

              if(usePhiMETCorr==1){ // use MET Phi correction if required
                pair<double, double> pfmet_phicorr = common_stuff::getPhiCorrMET( pfmet_bla, pfmetphi_bla, nvtx, !sampleName.Contains("DATA"));
                pfmet_bla = pfmet_phicorr.first;
                pfmetphi_bla = pfmet_phicorr.second;
              }

              //------------------------------------------------------------------------------------------------
              // Apply muon corrections
              //------------------------------------------------------------------------------------------------

              if(useMomentumCorr==1){ // use rochester corrections if required
                if(IS_MC_CLOSURE_TEST || isMCorDATA==0){
                  rmcor44X->momcor_mc(muPosCorr, MuPos_charge, smearRochCorrByNsigma/* , runopt */);
                  rmcor44X->momcor_mc(muNegCorr, MuNeg_charge, smearRochCorrByNsigma/* , runopt */);
                  rmcor44X->momcor_mc(muPosCorrCentral, MuPos_charge, 0/* , runopt */);
                  rmcor44X->momcor_mc(muNegCorrCentral, MuNeg_charge, 0/* , runopt */);
                }
                else{
                  rmcor44X->momcor_data(muPosCorr, MuPos_charge, smearRochCorrByNsigma , run<175832 ? 0 : 1 );
                  rmcor44X->momcor_data(muNegCorr, MuNeg_charge, smearRochCorrByNsigma , run<175832 ? 0 : 1 );
                  rmcor44X->momcor_data(muPosCorrCentral, MuPos_charge, 0 , run<175832 ? 0 : 1 );
                  rmcor44X->momcor_data(muNegCorrCentral, MuNeg_charge, 0 , run<175832 ? 0 : 1 );
                }
              }else if(useMomentumCorr==2){ // use Momentum scale corrections if required
                corrector->applyPtCorrection(muPosCorr,MuPos_charge);
                corrector->applyPtCorrection(muNegCorr,MuNeg_charge);
                corrector->applyPtCorrection(muPosCorrCentral,MuPos_charge);
                corrector->applyPtCorrection(muNegCorrCentral,MuNeg_charge);
              }
              // cout << "toy= " << m 
                   // << " " << muPosCorr.Pt() << " " << muPosCorr.Eta() << " " << muPosCorr.Phi() 
                   // << " " << muNegCorr.Pt() << " " << muNegCorr.Eta() << " " << muNegCorr.Phi() 
                   // << " " << endl;
              
              Zcorr = muPosCorr + muNegCorr;
              ZcorrCentral = muPosCorrCentral + muNegCorrCentral;
              
              TLorentzVector Z_met,Z_metCentral,WlikePos_met,WlikePos_metCentral,WlikePos,WlikePosCentral,mupos_bla,mupos_blaCentral,muneg_bla,muneg_blaCentral;
              Z_met.SetPtEtaPhiM(pfmet_bla,0,pfmetphi_bla,0);
              Z_metCentral.SetPtEtaPhiM(pfmet_blaCentral,0,pfmetphi_blaCentral,0);
              muneg_bla.SetPtEtaPhiM(muNegCorr.Pt(),0,muNegCorr.Phi(),0);
              muneg_blaCentral.SetPtEtaPhiM(muNegCorrCentral.Pt(),0,muNegCorrCentral.Phi(),0);
              mupos_bla.SetPtEtaPhiM(muPosCorr.Pt(),0,muPosCorr.Phi(),0);
              mupos_blaCentral.SetPtEtaPhiM(muPosCorrCentral.Pt(),0,muPosCorrCentral.Phi(),0);
              WlikePos_met = muneg_bla + Z_met;
              WlikePos_metCentral = muneg_blaCentral + Z_metCentral;
              WlikePos_met.SetPtEtaPhiM(WlikePos_met.Pt(),0,WlikePos_met.Phi(),0);
              WlikePos_metCentral.SetPtEtaPhiM(WlikePos_metCentral.Pt(),0,WlikePos_metCentral.Phi(),0);
              WlikePos = mupos_bla + WlikePos_met;
              WlikePosCentral = mupos_blaCentral + WlikePos_metCentral;
              
              double MuPos_var_jacobian[3] = {2*muPosCorr.Pt()/WMass::ZMassCentral_MeV*1e3,WlikePos.Mt()/WMass::ZMassCentral_MeV*1e3,2*WlikePos_met.Pt()/WMass::ZMassCentral_MeV*1e3};
              double MuPos_var_NotScaled[3] = {muPosCorr.Pt(),WlikePos.Mt()/* Zcorr.Mt() */,WlikePos_met.Pt()}; // Zcorr would be TEMP !!!!
              double MuPos_var_NotScaledCentral[3] = {muPosCorrCentral.Pt(),WlikePosCentral.Mt()/* ZcorrCentral.Mt() */,WlikePos_metCentral.Pt()}; // Zcorr would be TEMP !!!!
              // LUCA ADD TO AVOID OVERFLOW
              for(int k=0;k<3;k++)
                if(MuPos_var_jacobian[k]>=xmax) MuPos_var_jacobian[k]=xmax-binsize2/2;

	      //------------------------------------------------------------------------------------------------
	      // APPLY Zmass selection and MUONpt CUT
	      //------------------------------------------------------------------------------------------------

              // good pair within acceptance cuts for both muons
              if( ZcorrCentral.M()>50
                  && TMath::Abs(muPosCorrCentral.Eta())<WMass::etaMaxMuons[i] && muPosCorrCentral.Pt()>WMass::sel_xmin[0]*ZWmassRatio && MuPosTrg
                  && TMath::Abs(muNegCorrCentral.Eta())<2.4 && muNegCorrCentral.Pt()>10 && MuPos_charge != MuNeg_charge
                  // && noTrgExtraMuonsLeadingPt<10 // REMOVED BECAUSE OF MARKUS
                  ){
                // full ID and tight requirements on the muon
                if(MuPosIsTight && MuPosRelIso<0.12 && MuPos_dxy<0.02){
                  for(int k=0;k<3;k++)
                    if(m==0 && WMass::WMassNSteps==j) common_stuff::plot1D(Form("hWlikePos_%sNonScaled_5_RecoCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                          MuPos_var_NotScaled[k], evt_weight*MuPos_tight_muon_SF, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );

		  //------------------------------------------------------------------------------------------------
		  // APPLY MET CUT
		  //------------------------------------------------------------------------------------------------

                  if(WlikePos_metCentral.Pt()>WMass::sel_xmin[2]*ZWmassRatio){
                    for(int k=0;k<3;k++)
                      if(m==0 && WMass::WMassNSteps==j) common_stuff::plot1D(Form("hWlikePos_%sNonScaled_6_METCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
									     MuPos_var_NotScaled[k], evt_weight*MuPos_tight_muon_SF, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                    
		    if(controlplots && m==0  && WMass::WMassNSteps==j) common_stuff::plot1D(Form("hZ_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
											    Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF, h_1d, 1000,0,250 );

		    //------------------------------------------------------------------------------------------------
		    // APPLY RECOIL CUT: 
		    //------------------------------------------------------------------------------------------------

                    if(WlikePosCentral.Pt()<WMass::WpTcut*ZWmassRatio){
                      for(int k=0;k<3;k++)
                        if(m==0 && WMass::WMassNSteps==j) common_stuff::plot1D(Form("hWlikePos_%sNonScaled_7_RecoilCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                          MuPos_var_NotScaled[k], evt_weight*MuPos_tight_muon_SF, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );
                      
                      //------------------------------------------------------------------------------------------------
                      // APPLY FIT RANGE BOX -- based on central correction
                      //------------------------------------------------------------------------------------------------

                        if(
                            MuPos_var_NotScaledCentral[0] > WMass::sel_xmin[0]*ZWmassRatio && MuPos_var_NotScaledCentral[0] < WMass::sel_xmax[0]*ZWmassRatio
                            && MuPos_var_NotScaledCentral[1] > WMass::sel_xmin[1]*ZWmassRatio && MuPos_var_NotScaledCentral[1] < WMass::sel_xmax[1]*ZWmassRatio
                            && MuPos_var_NotScaledCentral[2] > WMass::sel_xmin[2]*ZWmassRatio && MuPos_var_NotScaledCentral[2] < WMass::sel_xmax[2]*ZWmassRatio
                           ){
                      

                        //------------------------------------------------------------------------------------------------
                        // SET THE PDF WEIGHT 
                        //------------------------------------------------------------------------------------------------

                        double lha_weight = 1;
                        // double lha_weight = LHAPDF::xfx(0,x1,Q,fl1)*LHAPDF::xfx(0,x2,Q,fl2) / (LHAPDF::xfx(1,x1,Q,fl1)*LHAPDF::xfx(1,x2,Q,fl2));
                        double weight_old = 1;
                        #ifdef LHAPDF_ON
                          if(parton1_x>1e-7 && parton2_x>1e-7 && parton1_x<1 && parton2_x<1)
                            weight_old = !sampleName.Contains("DATA") ? (LHAPDF::xfx(1,parton1_x,scalePDF,parton1_pdgId)*LHAPDF::xfx(1,parton2_x,scalePDF,parton2_pdgId)) : 1;
                        #endif
                        // cout << "scalePDF= " << scalePDF << " parton1_x= " << parton1_x << " parton1_pdgId= " << parton1_pdgId 
                             // << "parton2_x= " << parton2_x << " parton2_pdgId= " << parton2_pdgId << endl;
                        // cout << " LHAPDF::xfx(0,parton1_x,scalePDF,parton1_pdgId)= LHAPDF::xfx(0,"<<parton1_x<<","<<scalePDF<<","<<parton1_pdgId<<")= " << LHAPDF::xfx(0,parton1_x,scalePDF,parton1_pdgId) << endl;
                        // cout << " LHAPDF::xfx(0,parton2_x,scalePDF,parton2_pdgId)= LHAPDF::xfx(0,"<<parton2_x<<","<<scalePDF<<","<<parton2_pdgId<<")= " << LHAPDF::xfx(0,parton2_x,scalePDF,parton2_pdgId) << endl;
                        // cout << " LHAPDF::xfx(1,parton1_x,scalePDF,parton1_pdgId)= LHAPDF::xfx(1,"<<parton1_x<<","<<scalePDF<<","<<parton1_pdgId<<")= " << LHAPDF::xfx(1,parton1_x,scalePDF,parton1_pdgId) << endl;
                        // cout << " LHAPDF::xfx(1,parton2_x,scalePDF,parton2_pdgId)= LHAPDF::xfx(1,"<<parton2_x<<","<<scalePDF<<","<<parton2_pdgId<<")= " << LHAPDF::xfx(1,parton2_x,scalePDF,parton2_pdgId) << endl;
                        // cout << " lha_weight= " << lha_weight << endl;
                        if(m==0 && controlplots && WMass::WMassNSteps==j){
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
                            if(m==0 && controlplots && WMass::WMassNSteps==j) 
                              common_stuff::plot1D(Form("hPDF_weights_%d_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h),
                                            lha_weight, 1, h_1d, 1000,0,2
                                          );
                          } else if(contains_LHE_weights){
                            // if(m==0 && WMass::WMassNSteps==j)
                              // common_stuff::plot2D(Form("hCorrPDFweights_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jZmass_MeV),
                                            // lha_weight,LHE_weight[PDF_reweighting_central_Index+h], 1, 
                                            // h_2d, 200,0.9,1.1,200,0.9,1.1);
                            
                            lha_weight = LHE_weight[PDF_reweighting_central_Index+h];
                          }
                          
			  
			  //------------------------------------------------------------------------------------------------
			  // END PDF WEIGHT  --- BELOW PLOT for CLOSURE TEST
			  //------------------------------------------------------------------------------------------------ 
			  
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
			  if(nvtx==1 || nvtx==0) tag_VTX="_VTX1";
			  if(nvtx==2) tag_VTX="_VTX2";
			  if(nvtx==3) tag_VTX="_VTX3";
			  if(nvtx==4) tag_VTX="_VTX4";
			  if(nvtx==5) tag_VTX="_VTX5";
			  if(nvtx==6) tag_VTX="_VTX6";
			  if(nvtx==7) tag_VTX="_VTX7";
			  if(nvtx==8) tag_VTX="_VTX8";
			  if(nvtx==9) tag_VTX="_VTX9";
			  if(nvtx==10) tag_VTX="_VTX10";
			  if(nvtx==11) tag_VTX="_VTX11";
			  if(nvtx==12) tag_VTX="_VTX12";
			  if(nvtx==13) tag_VTX="_VTX13";
			  if(nvtx==14) tag_VTX="_VTX14";
			  if(nvtx==15) tag_VTX="_VTX15";
			  if(nvtx==16) tag_VTX="_VTX16";
			  if(nvtx==17) tag_VTX="_VTX17";
			  if(nvtx==18) tag_VTX="_VTX18";
			  if(nvtx==19) tag_VTX="_VTX19";
			  if(nvtx>=20) tag_VTX="_VTX20";
			  
			  
			  TLorentzVector VisPt;
			  VisPt.SetPtEtaPhiM(Zcorr.Pt(),0,Zcorr.Phi(),0);
			  
			  string mettype="_tk";
			  
			  double u1_scale=0;
			  if(WlikePos_met.Pt()>0 ) {
			    plotVariables( Z_met, VisPt,  Zcorr, u1_scale, tag_zPtcut.c_str(), mettype.c_str() , false, false, h_1d, h_2d, evt_weight*MuPos_tight_muon_SF);
			    plotVariables( Z_met, VisPt,  Zcorr, u1_scale, tag_VTX.c_str(), mettype.c_str() , false, false, h_1d, h_2d, evt_weight*MuPos_tight_muon_SF);
			    plotVariables( Z_met, VisPt,  Zcorr, u1_scale, tag_y.c_str(), mettype.c_str() , false, false, h_1d, h_2d, evt_weight*MuPos_tight_muon_SF);
			  }
			  
			  
                          //------------------------------------------------------------------------------------------------
                          // THOSE SHOULD BE THE MONEY PLOTS: HISTOGRAMS FOR Wlike Mass FIT 
                          //------------------------------------------------------------------------------------------------ 
                          for(int k=0;k<3;k++){
			    
                            common_stuff::plot1D(Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jZmass_MeV),
						 MuPos_var_NotScaled[k], evt_weight*MuPos_tight_muon_SF*lha_weight, h_1d, 50, WMass::fit_xmin[k]*ZWmassRatio, WMass::fit_xmax[k]*ZWmassRatio );

                            // std::map<std::string, TH1D*>::iterator iter= h_1d.find(Form("TEST_hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));  
			    
                            
			    // if(counter==0){
			    // if(iter == h_1d.end()){ //no histo for this yet, so make a new  one
                            // cout << "NOT FOUND!!!" << endl;
			    // }else{     
			    // cout << "HISTO FOUND!!!" << endl;
			    // (*iter).second->Print(); 
			    // cout << "name " << (*iter).second->GetName() << endl;
			    // cout << "title " << (*iter).second->GetTitle() << endl;
			    // cout << "x axis title " << (*iter).second->GetXaxis()->GetTitle() << endl;
			    // cout << "y axis title " << (*iter).second->GetYaxis()->GetTitle() << endl; 
			    // } 
			    // // return; 
			    // counter++; 
                            // }                 			    
                            
                          }
                          
                          //------------------------------------------------------------------------------------------------
                          // EXTRA PLOTS
                          //------------------------------------------------------------------------------------------------ 

                          if(m==0 && controlplots && WMass::WMassNSteps==j)
                            common_stuff::plot1D(Form("hWlikePos_Recoil_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jZmass_MeV),
						 WlikePos.Pt(), evt_weight*MuPos_tight_muon_SF, h_1d, 50, 0, 25 );
                                      
                        
                          // templates for "scaled observable method" as of Martina's thesis can be also built by multiplying histos, the result is the same (CHECKED!)
                          if(buildTemplates && m==0){
                            for(int k=0;k<3;k++){
                              double R_WdivZ_weight=hWlikePos_R_WdivZ[h][k][i][j]->GetBinContent(hWlikePos_R_WdivZ[h][k][i][j]->GetXaxis()->FindBin(MuPos_var_jacobian[k]));
                              common_stuff::plot1D(Form("hWlikePos_%sScaled_RWeighted_Templates_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                          MuPos_var_jacobian[k],R_WdivZ_weight*evt_weight*MuPos_tight_muon_SF, h_1d, nbins, bins_scaled[k] );
                            }
                          }

                        } // end loop PDF
                        

			//------------------------------------------------------------------------------------------------
			// control plots for different etas but only for central W mass
			//------------------------------------------------------------------------------------------------ 

                        if(TMath::Abs(jZmass_MeV - WMass::ZMassCentral_MeV) > 1)  continue;
                        if(controlplots && m==0){
                          // control distributions 
                          common_stuff::plot1D(Form("hnvtx_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          nvtx, evt_weight, h_1d, 50,0,50 );
                          common_stuff::plot1D(Form("Zmass_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.M(), evt_weight*MuPos_tight_muon_SF, h_1d, 300,50,200 );
                          
                          common_stuff::plot1D(Form("Zmass_zoomed_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.M(), evt_weight*MuPos_tight_muon_SF, h_1d, 6000,60,120 );
                          common_stuff::plot2D(Form("ZmassVsMuPosEta_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta(),Zcorr.M(), evt_weight*MuPos_tight_muon_SF, 
                                          h_2d, 50,-2.5,2.5,200,90,92 );
                          // common_stuff::plot2D(Form("PosMuonPtCorrVsMuPosEta_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          // muPosCorr.Eta(),Zcorr.M(),evt_weight*MuPos_tight_muon_SF,
                                          // h_2d, 50,-2.5,2.5,300,60,120
                                        // );
                          // common_stuff::plot2D(Form("PosMuonPtCorrVsMuPosPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          // muPosCorr.Phi(),Zcorr.M(),evt_weight*MuPos_tight_muon_SF,
                                          // h_2d, 50,-2*TMath::Pi(),2*TMath::Pi(),300,60,120
                                        // );
                          common_stuff::plot1D(Form("ZlepDeltaEta_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF, h_1d, 200,-5,5 );

                          common_stuff::plot2D(Form("ZlepEta1VsEta2_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta(),muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 50,-2.5,2.5,50,-2.5,2.5 );
                          common_stuff::plot2D(Form("ZlepDeltaEtaVsEta1_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 50,-2.5,2.5,100,-5,5 );
                          common_stuff::plot1D(Form("ZlepDeltaPt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF, h_1d, 100,-50,50 );
                          common_stuff::plot2D(Form("ZlepPt1VsPt2_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Pt(),muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,0,200,200,0,200 );
                          common_stuff::plot2D(Form("ZlepDeltaPtVsPt1_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Pt(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,0,200,100,-50,50 );
                          common_stuff::plot1D(Form("ZlepDeltaPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF, h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                          common_stuff::plot2D(Form("ZlepPhi1VsPhi2_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Phi(),muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
                          common_stuff::plot2D(Form("ZlepDeltaPhiVsPhi1_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Phi(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
                          common_stuff::plot1D(Form("ZlepDeltaR_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF, h_1d, 1000,0,10 );
                          common_stuff::plot1D(Form("ZDeltaEtaMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF, h_1d, 200,-5,5 );
                          common_stuff::plot1D(Form("ZDeltaPhiMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                          common_stuff::plot1D(Form("ZDeltaRMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          TMath::Hypot(muPosCorr.Phi()-Jet_leading_phi,muPosCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 1000,0,10 );
                          common_stuff::plot1D(Form("ZDeltaEtaMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muNegCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,-5,5 );
                          common_stuff::plot1D(Form("ZDeltaPhiMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muNegCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                          common_stuff::plot1D(Form("ZDeltaRMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 1000,0,10 );
                          common_stuff::plot1D(Form("hpfMET_Z_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          pfmet_bla,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                          common_stuff::plot1D(Form("hpfMETphi_Z_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          pfmetphi_bla,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                          common_stuff::plot1D(Form("hZ_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                          common_stuff::plot1D(Form("hZ_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.Mt(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                          common_stuff::plot2D(Form("hZ_ptVsmt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.Mt(),Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 150,50,200,100,0,25 );
                          common_stuff::plot1D(Form("hWlikePos_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          WlikePos_pt,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,25 );
                          common_stuff::plot1D(Form("hWlikePos_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          WlikePos_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                          common_stuff::plot1D(Form("hWlikePos_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          WlikePos_mt,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,0,200 );
                          common_stuff::plot1D(Form("hpfMET_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          pfmetWlikePos,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                          common_stuff::plot1D(Form("hpfMETphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          pfmetWlikePos_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                          common_stuff::plot1D(Form("hMupt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Pt()<200 ? muPosCorr.Pt() : 199.5,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,0,200 );
                          common_stuff::plot1D(Form("hMueta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-2.5,2.5 );
                          common_stuff::plot1D(Form("hMuphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                          common_stuff::plot1D(Form("hMuiso_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          MuPosRelIso,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 1000,0,10 );
                          common_stuff::plot1D(Form("hMuMETpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                          common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-2.5,2.5 );
                          common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                          common_stuff::plot1D(Form("hMuMETiso_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          MuNegRelIso,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,10 );
                          common_stuff::plot1D(Form("hJetpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          Jet_leading_pt,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,50 );
                          common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          Jet_leading_eta,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100, -2.5,2.5 );
                          common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          Jet_leading_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100, -TMath::Pi(),TMath::Pi() );
                          common_stuff::plot1D(Form("hu1_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          u1,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 360,-30,150 );
                          common_stuff::plot1D(Form("hu1_Zpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          u1-Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 360,-30,150 );
                          common_stuff::plot1D(Form("hu2_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          u2,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 250,-50,50 );
                          common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.Pt(),u1,evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 100,0,25,360,-30,150 );
                          common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.Pt(),u2,evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 100,0,25,250,-50,50 );
                        }
                      }
                    }
                  }
                }else if(controlplots && m==0){
                  
                  // Iso and dxy for muons which fail either tight requirement, isolation or dxy cut
                  if(pfmetWlikePos>25 && WlikePos_pt<20){
                    // if( (TMath::Abs(zmass1 - WMass::WMassCentral_MeV) > 1)) continue;
                  }

                  // invert cuts to select QCD enriched region, no cuts on met or jet
                  if(
                    MuPosRelIso>0.12 // single muon cuts (inverted iso (is <0.5 for signal) , no tight requirement)
                    && MuPos_dxy>0.02 // single muon cuts (MuPosIsTight contains dxy < 0.2 cut)
                    && WlikePos_pt<20/* *WMass::ZMassCentral_MeV/iZmass_GeV */ 
                    ){

                    for(int k=0;k<3;k++)
                      common_stuff::plot1D(Form("hWlikePos_%sScaled_QCD_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jZmass_MeV),
                                          MuPos_var_jacobian[k], evt_weight*MuPos_tight_muon_SF, h_1d, nbins, bins_scaled[k] );

                    
                    // if( (TMath::Abs(zmass1 - WMass::WMassCentral_MeV) > 1)
                        // ) 
                    // continue;

                    // QCD distributions for central W mass
                    common_stuff::plot1D(Form("Zmass_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          nvtx, evt_weight*MuPos_tight_muon_SF, h_1d, 300,50,200 );
                    common_stuff::plot1D(Form("Zmass_zoomed_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          nvtx, evt_weight*MuPos_tight_muon_SF, h_1d, 6000,60,120 );
                    common_stuff::plot2D(Form("ZmassVsMuPosEta_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta(),Zcorr.M(), evt_weight*MuPos_tight_muon_SF, 
                                          h_2d, 50,-2.5,2.5,200,90,92 );
                    common_stuff::plot1D(Form("ZlepDeltaPt_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF, h_1d, 100,-50,50 );
                    common_stuff::plot2D(Form("ZlepPt1VsPt2_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Pt(),muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,0,200,200,0,200 );
                    common_stuff::plot2D(Form("ZlepDeltaPtVsPt1_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Pt(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,0,200,100,-50,50 );
                    common_stuff::plot1D(Form("ZlepDeltaPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF, h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                    common_stuff::plot2D(Form("ZlepPhi1VsPhi2_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Phi(),muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
                    common_stuff::plot2D(Form("ZlepDeltaPhiVsPhi1_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Phi(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
                    common_stuff::plot1D(Form("ZlepDeltaR_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF, h_1d, 1000,0,10 );
                    common_stuff::plot1D(Form("ZDeltaEtaMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF, h_1d, 200,-5,5 );
                    common_stuff::plot1D(Form("ZDeltaPhiMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                    common_stuff::plot1D(Form("ZDeltaRMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          TMath::Hypot(muPosCorr.Phi()-Jet_leading_phi,muPosCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 1000,0,10 );
                    common_stuff::plot1D(Form("ZDeltaEtaMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                    muNegCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF,
                                    h_1d, 200,-5,5 );
                    common_stuff::plot1D(Form("ZDeltaPhiMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muNegCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                    common_stuff::plot1D(Form("ZDeltaRMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 1000,0,10 );
                    common_stuff::plot1D(Form("hpfMET_Z_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                    common_stuff::plot1D(Form("hpfMETphi_Z_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          pfmetphi_bla,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                    common_stuff::plot1D(Form("hZ_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF, h_1d, 100,0,25 );
                    common_stuff::plot1D(Form("hZ_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                    common_stuff::plot1D(Form("hZ_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.Mt(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                    common_stuff::plot2D(Form("hZ_ptVsmt_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.Mt(),Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 150,50,200,100,0,25 );
                    common_stuff::plot1D(Form("hWlikePos_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          WlikePos_pt,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,25 );
                    common_stuff::plot1D(Form("hWlikePos_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          WlikePos_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                    common_stuff::plot1D(Form("hWlikePos_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          WlikePos_mt,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,0,200 );
                    common_stuff::plot1D(Form("hpfMET_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          pfmetWlikePos,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                    common_stuff::plot1D(Form("hpfMETphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          pfmetWlikePos_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                    common_stuff::plot1D(Form("hMupt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Pt()<200 ? muPosCorr.Pt() : 199.5,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,0,200 );
                    common_stuff::plot1D(Form("hMueta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-2.5,2.5 );
                    common_stuff::plot1D(Form("hMuphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                    common_stuff::plot1D(Form("hMuiso_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          MuPosRelIso,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 1000,0,10 );
                    common_stuff::plot1D(Form("hMuMETpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                    common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-2.5,2.5 );
                    common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                    common_stuff::plot1D(Form("hMuMETiso_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          MuNegRelIso,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,10 );
                    common_stuff::plot1D(Form("hJetpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          MuNegRelIso,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,50 );
                    common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          Jet_leading_eta,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100, -2.5,2.5 );
                    common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          Jet_leading_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100, -TMath::Pi(),TMath::Pi() );
                    common_stuff::plot1D(Form("hu1_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          u1,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 360,-30,150 );
                    common_stuff::plot1D(Form("hu1_Zpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          u1-Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 360,-30,150 );
                    common_stuff::plot1D(Form("hu2_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          u2,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 250,-50,50 );
                    common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.Pt(),u1,evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 100,0,25,360,-30,150 );
                    common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jZmass_MeV),
                                          Zcorr.Pt(),u2,evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 100,0,25,250,-50,50 );
                    
                  } // end if for qcd enriched
                } // end else for muon cuts (sig or qcd enriched)
              } // end if for good pair within acceptance cuts for both muons
            } // end if for good reco event
          } // end toys loop
        } // end if for dummy signal/fake separation
      } // end W mass loop
    } // end muon eta loop
        
  } // end event loop

  // TGraph *tigre = new TGraph();
  // tigre->SetName("tigre");
  // tigre->SetTitle("tigre");
  
  // TGraph *tigre2 = new TGraph();
  // tigre2->SetName("tigre2");
  // tigre2->SetTitle("tigre2");
  
  // if(contains_PDF_reweight){
    // for(int i_weight=0; i_weight< 100; i_weight++){
      // tigre->SetPoint(i_weight,i_weight,hpdf_wlha_vs_w[i_weight]->GetMean(1)/hpdf_wlha_vs_w[i_weight]->GetMean(2));
      // tigre2->SetPoint(i_weight,hpdf_wlha_vs_w[i_weight]->GetMean(1),hpdf_wlha_vs_w[i_weight]->GetMean(2));
      // hpdf_wlha_mean->Fill(hpdf_wlha[i_weight]->GetMean());
      // hpdf_w_mean->Fill(hpdf_w[i_weight]->GetMean());
    // }
    // ffpdf->cd();
    // tigre->Write();
    // tigre2->Write();
  // }
  
  // ffpdf->Write();
  // ffpdf->Close();
  
  outTXTfile.close();
  
  TFile*fout = new TFile(Form("%s/Zanalysis%s.root",outputdir.Data(),chunk_str.Data()),"RECREATE");
  fout->cd();


  if(!(sampleName.Contains("DYJetsPow") || sampleName.Contains("DYJetsMadSig"))){
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        if(WMass::WMassNSteps!=j){
          // int jZmass_MeV = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMass_SkipNSteps);
          int jZmass_MeV = WMass::Zmass_values_array[j];
          for(int k=0;k<3;k++){
            for(int h=0; h<WMass::PDF_members; h++){
              for(int m=0; m<WMass::NtoysMomCorr; m++){
                TString toys_str = "";
                if(WMass::NtoysMomCorr>1) toys_str = Form("_MomCorrToy%d",m);
                
                common_stuff::cloneHisto1D(Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),WMass::WMassCentral_MeV), 
                                            Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jZmass_MeV), 
                                            h_1d);
                common_stuff::cloneHisto1D(Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),WMass::WMassCentral_MeV), 
                                            Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jZmass_MeV), 
                                            h_1d);
              }
            }
          }
        }
      }
    }
  }
  
  common_stuff::writeOutHistos( fout, h_1d, h_2d );
    
  fout->Write();
  fout->Close();

}
