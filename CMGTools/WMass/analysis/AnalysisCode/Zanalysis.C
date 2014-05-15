// UNCOMMENT TO USE PDF REWEIGHTING
//#define LHAPDF_ON

#ifdef LHAPDF_ON
  #include "LHAPDF/LHAPDF.h"
#endif 

#define Zanalysis_cxx
#include "Zanalysis.h"
#include "../includes/common.h"
// #include "rochcor_42X.h"
#include "common_stuff.h"
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

void Zanalysis::Loop(int chunk, int Entry_ini, int Entry_fin, int IS_MC_CLOSURE_TEST, int isMCorDATA, TString outputdir, int buildTemplates, int useMomentumCorr, int smearRochCorrByNsigma, int useEffSF, int usePtSF, int useVtxSF, int controlplots, TString sampleName, int generated_PDF_set, int generated_PDF_member, int contains_PDF_reweight, int usePhiMETCorr, int useRecoilCorr, int RecoilCorrResolutionNSigmaU1, int RecoilCorrScaleNSigmaU1, int RecoilCorrResolutionNSigmaU2, int use_PForNoPUorTKmet, int use_syst_ewk_Alcaraz)
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
  if(usePtSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && sampleName.Contains("DYJetsSig")){
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
      
      if(!sampleName.Contains("DYJetsSig") && WMass::WMassNSteps!=j) continue;
      
      int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;

      for(int k=0;k<3;k++){
        for(int h=0; h<WMass::PDF_members; h++){
          if(buildTemplates){
            cout << Form("hR_WdivZ_WlikePos_%s_8_JetCut_pdf%d-%d_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),jWmass) << endl;
            hWlikePos_R_WdivZ[h][k][i][j]=(TH1D*)finTemplates->Get(Form("hR_WdivZ_WlikePos_%s_8_JetCut_pdf%d-%d_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),jWmass));
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

  // TFile *ffpdf=new TFile("pdf_w.root","RECREATE");
  // TH1D*hpdf_w[100];
  // TH1D*hpdf_wlha[100];
  // TH2D*hpdf_wlha_vs_w[100];
  // TH1D*hpdf_w_mean = new TH1D("hpdf_w_mean","hpdf_w_mean",100,0,2);
  // TH1D*hpdf_wlha_mean = new TH1D("hpdf_wlha_mean","hpdf_wlha_mean",100,0,2);
  // // cout << "contains_PDF_reweight " << contains_PDF_reweight << endl;
  // if(contains_PDF_reweight){
    // for(int i_weight=0; i_weight< 100; i_weight++){
      // hpdf_w[i_weight]=new TH1D(Form("hpdf_w_%d",i_weight),Form("hpdf_w_%d",i_weight),100,0,2);
      // hpdf_wlha[i_weight]=new TH1D(Form("hpdf_wlha_%d",i_weight),Form("hpdf_wlha_%d",i_weight),100,0,2);
      // hpdf_wlha_vs_w[i_weight]=new TH2D(Form("hpdf_wlha_vs_w_%d",i_weight),Form("hpdf_wlha_vs_w_%d",i_weight),100,0,2,100,0,2);
    // }
  // }
  
  // double ZMassCentral_MeV = 91187.6;

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

  RecoilCorrector::RecoilCorrector*  correctorRecoil_Z; // TYPE2

  correctorRecoil_Z = new RecoilCorrector(fileCorrectTo.c_str(),123456);
  correctorRecoil_Z->addDataFile(fileZmmData.c_str());
  correctorRecoil_Z->addMCFile(fileZmmMC.c_str());

  bool doSingleGauss=false;

  double ZWmassRatio = WMass::ZMass/(WMass::WMassCentral_MeV/1e3);
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=first_entry; jentry<nentries;jentry++) {
    // for (Long64_t jentry=0; jentry<5e4;jentry++) {
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
    
    double lha_weight = 1;
    double weight_old = 1;
    #ifdef LHAPDF_ON
      weight_old = !sampleName.Contains("DATA") ? (LHAPDF::xfx(1,parton1_x,scalePDF,parton1_pdgId)*LHAPDF::xfx(1,parton2_x,scalePDF,parton2_pdgId)) : 1;
    #endif
    
    for(int h=0; h<WMass::PDF_members; h++){
      if(!sampleName.Contains("DATA") && WMass::PDF_sets>0 && WMass::PDF_sets!=generated_PDF_set && WMass::PDF_members!=generated_PDF_member){
        double weight_new = 1;
        #ifdef LHAPDF_ON
          LHAPDF::usePDFMember(0,h);
          weight_new = (LHAPDF::xfx(0,parton1_x,scalePDF,parton1_pdgId)*LHAPDF::xfx(0,parton2_x,scalePDF,parton2_pdgId));
        #endif
        lha_weight = weight_new/weight_old;
        // cout << lha_weight << endl;
        // if(contains_PDF_reweight){
          // hpdf_wlha[h]->Fill(lha_weight);
          // hpdf_wlha_vs_w[h]->Fill(LHE_weight[h+7],lha_weight);
        // }
      }    
    }
    // continue;
    
    // if(!(IS_MC_CLOSURE_TEST || isMCorDATA==0) && run>175832) continue; // TO TEST ROCHESTER CORRECTIONS ONLY ON RUN2011A
    // if(!(IS_MC_CLOSURE_TEST || isMCorDATA==0) && run<175832) continue; // TO TEST ROCHESTER CORRECTIONS ONLY ON RUN2011B
    
    if(nTrgMuons>2) continue; // NOT (YET) REQUIRED WHILE MAKING NTUPLES!!!! <<---------- it sohuld be already fixed in the ntuples now
    
    double evt_weight_original = lumi_scaling;
    // cout << "evt_weight_original " << evt_weight_original << endl;
    // TO BE CHECKED!!!
    if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && npu>0) evt_weight_original=lumi_scaling*hPileupSF->GetBinContent(hPileupSF->GetXaxis()->FindBin(npu));
    if(usePtSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && hZPtSF && sampleName.Contains("DYJetsSig")) evt_weight_original*=hZPtSF->GetBinContent(hZPtSF->GetXaxis()->FindBin(Z_pt>0?Z_pt:ZGen_pt));
    
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
      // hPileUp_Fall11->Fill(npu);
      common_stuff::plot1D("hPileUp_Fall11",
                                        npu, 1, h_1d, 50,0,50
                                      );

    if(use_syst_ewk_Alcaraz && sampleName.Contains("DYJetsSig")){
      if(FSRWeight>-99){ 
        evt_weight_original *= FSRWeight;
        // cout << "FSRWeight= " << FSRWeight << endl;
      }
    }

    
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");

      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        
        if(!sampleName.Contains("DYJetsSig") && WMass::WMassNSteps!=j) continue;
        int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;
        // double iWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV)/1e3;
        double iZmass = (WMass::ZMass*1000-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV)/1e3;
        
        // // BW REWEIGHTING
        double shat=0,gamma=2.141 /*HARD CODED TO PDG VALUE*/,mw0=0,mw_i=0,weight_i=1;
        if(useGenVar){
          shat=ZGen_mass*ZGen_mass;
          // mw0=WMass::WMassCentral_MeV/1e3;
          mw0=WMass::ZMass;
          mw_i=iZmass;
          // ((shat - mw0^2)^2 + gamma^2 mw0^2) / ((shat - mw_i^2)^2 + gamma^2 mw_i^2)
          weight_i=(TMath::Power(shat - mw0*mw0,2) + TMath::Power(gamma*mw0,2)) / (TMath::Power(shat - mw_i*mw_i,2) + TMath::Power(gamma*mw_i,2));
          // cout << "ZGen_mass = " << ZGen_mass << " mw0= " << mw0 << " mw_i= " << mw_i << " weight_i= " << weight_i << endl;
        }
        double evt_weight=evt_weight_original*weight_i;
        // cout << "evt_weight B-W " << evt_weight << endl;
        
        
        // SELECT EVENT WITH Z/W SCALED CUTS
        // ORIGINAL CUTS:
        // muon pt > 30,
        // pfmetcut > 25,
        // Boson pt (recoilcut) < 20,
        
        // gen stuff to be used only when required
        if(useGenVar){
          if(ZGen_mass>0){
            // double MuPosGen_pt_jacobian = 2*MuPosGen_pt/WMass::ZMass;
            double MuPosGen_var_jacobian[3] = {2*MuPosGen_pt/WMass::ZMass,ZGen_mt/iZmass,2*MuNegGen_pt/iZmass};
            double MuPosGen_var_NotScaled[3] = {MuPosGen_pt,ZGen_mt,MuNegGen_pt};
            // LUCA ADD TO AVOID OVERFLOW
            for(int k=0;k<3;k++)
              if(MuPosGen_var_jacobian[k]>=xmax) MuPosGen_var_jacobian[k]=xmax-binsize2/2;
            
            for(int k=0;k<3;k++)
              // hWlikePos_VarScaled_1_Gen[k][i][j]->Fill(MuPosGen_var_jacobian[k],evt_weight);
              // common_stuff::plot1D(Form("hWlikePos_%sScaled_1_Gen_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                    // MuPosGen_var_jacobian[k], evt_weight, h_1d, nbins, bins_scaled[k] );
              common_stuff::plot1D(Form("hWlikePos_%sNonScaled_1_Gen_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                    MuPosGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k], WMass::fit_xmax[k] );

            
            if(ZGen_mass>50){
              for(int k=0;k<3;k++)
                // hWlikePos_VarScaled_2_ZGenMassCut[k][i][j]->Fill(MuPosGen_var_jacobian[k],evt_weight);
                // common_stuff::plot1D(Form("hWlikePos_%sScaled_2_ZGenMassCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                      // MuPosGen_var_jacobian[k], evt_weight, h_1d, nbins, bins_scaled[k] );
                common_stuff::plot1D(Form("hWlikePos_%sNonScaled_2_ZGenMassCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                      MuPosGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k], WMass::fit_xmax[k] );
              
              if(TMath::Abs(MuPosGen_eta)<WMass::etaMaxMuons[i]){
                for(int k=0;k<3;k++)
                  // hWlikePos_VarScaled_3_Mu1GenCut[k][i][j]->Fill(MuPosGen_var_jacobian[k],evt_weight);
                  // common_stuff::plot1D(Form("hWlikePos_%sScaled_3_Mu1GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                        // MuPosGen_var_jacobian[k], evt_weight, h_1d, nbins, bins_scaled[k] );
                  common_stuff::plot1D(Form("hWlikePos_%sNonScaled_3_Mu1GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                        MuPosGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k], WMass::fit_xmax[k] );
                
                if(TMath::Abs(MuNegGen_eta)<2.4){
                  for(int k=0;k<3;k++)
                    // hWlikePos_VarScaled_4_Mu2GenCut[k][i][j]->Fill(MuPosGen_var_jacobian[k],evt_weight);
                    // common_stuff::plot1D(Form("hWlikePos_%sScaled_4_Mu2GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                          // MuPosGen_var_jacobian[k], evt_weight, h_1d, nbins, bins_scaled[k] );
                    common_stuff::plot1D(Form("hWlikePos_%sNonScaled_4_Mu2GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                          MuPosGen_var_NotScaled[k], evt_weight, h_1d, 50, WMass::fit_xmin[k], WMass::fit_xmax[k] );
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
              // cout << Z_mass << " " << MuPos_pt << " " << MuPos_eta << " " << MuPos_phi << " " << MuPos_mass << endl;
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
              int vtxBin=nvtx;
              if(nvtx==0) vtxBin=1;
              if(nvtx>20) vtxBin=20;
              if(nvtx>18 && runopt==0) vtxBin=18;
              // if(j==0 && runopt==0 && nvtx>17) cout << "runopt= " << runopt << " nvtx= " << nvtx << " vtxBin= " << vtxBin << endl;
              // continue;
              if(use_InclusiveNVTX_RecoilCorr && use_PForNoPUorTKmet==2) vtxBin=1;

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
              if(useRecoilCorr==1 && use_PForNoPUorTKmet<3 && sampleName.Contains("DYJetsSig")){ // use Rochester Momentum scale corrections if required
                // if(Mu_charge>0){
                  // if(runopt==0){
                // }else{
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
                                 
                // }
              }
              // cout << " and after " << pfmet_bla << endl;

              if(usePhiMETCorr==1){ // use MET Phi correction if required
                pair<double, double> pfmet_phicorr = common_stuff::getPhiCorrMET( pfmet_bla, pfmetphi_bla, nvtx, !sampleName.Contains("DATA"));
                // cout << "0 pfmet_bla= " << pfmet_bla << " pfmetphi_bla= " << pfmetphi_bla << endl;
                pfmet_bla = pfmet_phicorr.first;
                pfmetphi_bla = pfmet_phicorr.second;
                // cout << "1 pfmet_bla= " << pfmet_bla << " pfmetphi_bla= " << pfmetphi_bla << endl;
              }

              if(useMomentumCorr==1){ // use rochester corrections if required
                if(IS_MC_CLOSURE_TEST || isMCorDATA==0){
                  // int runopt = r->Rndm()<0.457451 ? 0 : 1;
                  // int runopt = 1;
                  // rmcor42X->momcor_mc(muPosCorr, MuPos_charge, smearRochCorrByNsigma, runopt);
                  // rmcor42X->momcor_mc(muNegCorr, MuNeg_charge, smearRochCorrByNsigma, runopt);
                  rmcor44X->momcor_mc(muPosCorr, MuPos_charge, smearRochCorrByNsigma/* , runopt */);
                  rmcor44X->momcor_mc(muNegCorr, MuNeg_charge, smearRochCorrByNsigma/* , runopt */);
                  rmcor44X->momcor_mc(muPosCorrCentral, MuPos_charge, 0/* , runopt */);
                  rmcor44X->momcor_mc(muNegCorrCentral, MuNeg_charge, 0/* , runopt */);
                  // musclefit_data
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
              
              // double MuPos_pt_jacobian = 2*muPosCorr.Pt()/WMass::ZMass;
              double MuPos_var_jacobian[3] = {2*muPosCorr.Pt()/WMass::ZMass,WlikePos.Mt()/WMass::ZMass,2*WlikePos_met.Pt()/WMass::ZMass};
              // double MuPos_var_NotScaled[3] = {muPosCorr.Pt(),WlikePos.Mt(),WlikePos_met.Pt()};
              double MuPos_var_NotScaled[3] = {muPosCorr.Pt(),Zcorr.Mt(),WlikePos_met.Pt()}; // TEMP !!!!
              double MuPos_var_NotScaledCentral[3] = {muPosCorrCentral.Pt(),ZcorrCentral.Mt(),WlikePos_metCentral.Pt()}; // TEMP !!!!
              // LUCA ADD TO AVOID OVERFLOW
              for(int k=0;k<3;k++)
                if(MuPos_var_jacobian[k]>=xmax) MuPos_var_jacobian[k]=xmax-binsize2/2;

              int zmass1 = iZmass*1e3;
              
              // good pair within acceptance cuts for both muons
              if( /* Z_mass */ZcorrCentral.M()>50
                  && TMath::Abs(/* MuPos_eta */muPosCorrCentral.Eta())<WMass::etaMaxMuons[i] && muPosCorrCentral.Pt()>WMass::sel_xmin[0]/* WMass::fit_xmin[0] */*ZWmassRatio/* *WMass::ZMass/iZmass */ && MuPosTrg
                  && TMath::Abs(muNegCorrCentral.Eta())<2.4 && muNegCorrCentral.Pt()>10 && MuPos_charge != MuNeg_charge
                  // && noTrgExtraMuonsLeadingPt<10 // REMOVED BECAUSE OF MARKUS
                  ){
                // full ID and tight requirements on the muon
                if(MuPosIsTightAndIso && MuPosRelIso<0.12 && MuPos_dxy<0.02){
                  for(int k=0;k<3;k++)
                    // if(m==0) hWlikePos_VarScaled_5_RecoCut[k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);
                    // if(m==0) common_stuff::plot1D(Form("hWlikePos_%sScaled_5_RecoCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                          // MuPos_var_jacobian[k], evt_weight*MuPos_tight_muon_SF, h_1d, nbins, bins_scaled[k] );
                    if(m==0) common_stuff::plot1D(Form("hWlikePos_%sNonScaled_5_RecoCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                          MuPos_var_NotScaled[k]/ZWmassRatio, evt_weight*MuPos_tight_muon_SF, h_1d, 50, WMass::fit_xmin[k], WMass::fit_xmax[k] );

                  if(/* pfmetWlikePos */WlikePos_metCentral.Pt()>WMass::sel_xmin[2]/* WMass::fit_xmin[2] */*ZWmassRatio/* *WMass::ZMass/iZmass */){
                    for(int k=0;k<3;k++)
                      // if(m==0) hWlikePos_VarScaled_6_METCut[k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);
                      // if(m==0) common_stuff::plot1D(Form("hWlikePos_%sScaled_6_METCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                          // MuPos_var_jacobian[k], evt_weight*MuPos_tight_muon_SF, h_1d, nbins, bins_scaled[k] );
                      if(m==0) common_stuff::plot1D(Form("hWlikePos_%sNonScaled_6_METCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                          MuPos_var_NotScaled[k]/ZWmassRatio, evt_weight*MuPos_tight_muon_SF, h_1d, 50, WMass::fit_xmin[k], WMass::fit_xmax[k] );
                    
                      // if(controlplots && m==0) hZ_pt[0][i][j]->Fill(Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                      if(controlplots && m==0) common_stuff::plot1D(Form("hZ_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF, h_1d, 1000,0,250 );
                                        
                    if(WlikePosCentral.Pt()<15*ZWmassRatio/* *WMass::ZMass/iZmass */){ // DEFAULT IS 15
                      for(int k=0;k<3;k++)
                        // if(m==0) hWlikePos_VarScaled_7_RecoilCut[k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);
                        // if(m==0) common_stuff::plot1D(Form("hWlikePos_%sScaled_7_RecoilCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                          // MuPos_var_jacobian[k], evt_weight*MuPos_tight_muon_SF, h_1d, nbins, bins_scaled[k] );
                        if(m==0) common_stuff::plot1D(Form("hWlikePos_%sNonScaled_7_RecoilCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                          MuPos_var_NotScaled[k]/ZWmassRatio, evt_weight*MuPos_tight_muon_SF, h_1d, 50, WMass::fit_xmin[k], WMass::fit_xmax[k] );
                      
                        if(
                            // MuPos_var_NotScaled[0] > WMass::fit_xmin[0]*ZWmassRatio && MuPos_var_NotScaled[0] < WMass::fit_xmax[0]*ZWmassRatio
                            // && MuPos_var_NotScaled[1] > WMass::fit_xmin[1]*ZWmassRatio && MuPos_var_NotScaled[1] < WMass::fit_xmax[1]*ZWmassRatio
                            // && MuPos_var_NotScaled[2] > WMass::fit_xmin[2]*ZWmassRatio && MuPos_var_NotScaled[2] < WMass::fit_xmax[2]*ZWmassRatio
                            MuPos_var_NotScaledCentral[0] > WMass::sel_xmin[0]*ZWmassRatio && MuPos_var_NotScaledCentral[0] < WMass::sel_xmax[0]*ZWmassRatio
                            && MuPos_var_NotScaledCentral[1] > WMass::sel_xmin[1]*ZWmassRatio && MuPos_var_NotScaledCentral[1] < WMass::sel_xmax[1]*ZWmassRatio
                            && MuPos_var_NotScaledCentral[2] > WMass::sel_xmin[2]*ZWmassRatio && MuPos_var_NotScaledCentral[2] < WMass::sel_xmax[2]*ZWmassRatio
                           ){
                      // if(true){ // for the momet remove jet pt cut
                      // if(Jet_leading_pt<30){
                      
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
                        if(m==0){
                          // hPDF_x1->Fill(TMath::Log10(parton1_x));
                          common_stuff::plot1D("hPDF_x1",
                                          TMath::Log10(parton1_x), 1, h_1d, 1000,-4,0 );
                          // hPDF_x1unweighted->Fill(TMath::Log10(parton1_x),1/weight_old);
                          common_stuff::plot1D("hPDF_x1unweighted",
                                          TMath::Log10(parton1_x), 1/weight_old, h_1d, 1000,-4,0 );
                          // hPDF_x2->Fill(TMath::Log10(parton2_x));
                          common_stuff::plot1D("hPDF_x2",
                                          TMath::Log10(parton2_x), 1, h_1d, 1000,-4,0 );
                          // hPDF_x2unweighted->Fill(TMath::Log10(parton2_x),1/weight_old);
                          common_stuff::plot1D("hPDF_x2unweighted",
                                          TMath::Log10(parton2_x), 1/weight_old, h_1d, 1000,-4,0 );
                        }
                        
                        for(int h=0; h<WMass::PDF_members; h++){
                          if(!sampleName.Contains("DATA") && WMass::PDF_sets>0 && WMass::PDF_sets!=generated_PDF_set && WMass::PDF_members!=generated_PDF_member){
                            double weight_new = 1;
                            #ifdef LHAPDF_ON
                              LHAPDF::usePDFMember(0,h);
                              if(parton1_x>1e-7 && parton2_x>1e-7 && parton1_x<1 && parton2_x<1)
                                weight_new = (LHAPDF::xfx(0,parton1_x,scalePDF,parton1_pdgId)*LHAPDF::xfx(0,parton2_x,scalePDF,parton2_pdgId));
                            #endif
                            lha_weight = weight_new/weight_old;
                            // if(m==0) hPDF_weights[h]->Fill(lha_weight);
                            if(m==0) 
                              common_stuff::plot1D(Form("hPDF_weights_%d_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h),
                                            lha_weight, 1, h_1d, 1000,0,2
                                          );
                          }
                          for(int k=0;k<3;k++){
                            // hWlikePos_VarScaled_8_JetCut[m][h][k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);
                            // common_stuff::plot1D(Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass),
                                          // MuPos_var_jacobian[k], evt_weight*MuPos_tight_muon_SF, h_1d, nbins, bins_scaled[k] );
                            
                            // cout << ((muPosCorr.Pt()*iZmass/WMass::ZMass)<xmax*80/2 ? muPosCorr.Pt()*iZmass/WMass::ZMass : (xmax-binsize2/2)*80/2) << endl;
                            // hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][j]->Fill( (MuPos_var_NotScaled[k]*iZmass/WMass::ZMass)<xmax*80/(k==1 ? 1 : 2) ? MuPos_var_NotScaled[k]*iZmass/WMass::ZMass : (xmax-binsize2/2)*80/(k==1 ? 1 : 2) ,evt_weight*MuPos_tight_muon_SF);
                            common_stuff::plot1D(Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass),
                                          // (MuPos_var_NotScaled[k]/* *iZmass/WMass::ZMass */)<xmax*80/(k==1 ? 1 : 2) ? MuPos_var_NotScaled[k]/* *iZmass/WMass::ZMass */ : (xmax-binsize2/2)*80/(k==1 ? 1 : 2) ,
                                          // evt_weight*MuPos_tight_muon_SF, h_1d, nbins, bins_Notscaled[k] );
                                          MuPos_var_NotScaled[k]/ZWmassRatio, evt_weight*MuPos_tight_muon_SF, h_1d, 50, WMass::fit_xmin[k], WMass::fit_xmax[k] );
                            
                            common_stuff::plot1D(Form("hWlikePos_Recoil_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass),
                                          // (MuPos_var_NotScaled[k]/* *iZmass/WMass::ZMass */)<xmax*80/(k==1 ? 1 : 2) ? MuPos_var_NotScaled[k]/* *iZmass/WMass::ZMass */ : (xmax-binsize2/2)*80/(k==1 ? 1 : 2) ,
                                          // evt_weight*MuPos_tight_muon_SF, h_1d, nbins, bins_Notscaled[k] );
                                          WlikePos.Pt(), evt_weight*MuPos_tight_muon_SF, h_1d, 50, 0, 25 );
                                        
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
                        
                          // templates for "scaled observable method" as of Martina's thesis can be also built by multiplying histos, the result is the same (CHECKED!)
                          if(buildTemplates && m==0){
                            for(int k=0;k<3;k++){
                              double R_WdivZ_weight=hWlikePos_R_WdivZ[h][k][i][j]->GetBinContent(hWlikePos_R_WdivZ[h][k][i][j]->GetXaxis()->FindBin(MuPos_var_jacobian[k]));
                              // cout << "etamax= " << WMass::etaMaxMuons[i] << " mass= " << iZmass << " MuPos_var_jacobian[k]= " << MuPos_var_jacobian[k] << " R_WdivZ_weight= " << R_WdivZ_weight << endl;
                              // hWlikePos_VarScaled_RWeighted_Templates[h][k][i][j]->Fill(MuPos_var_jacobian[k],R_WdivZ_weight*evt_weight*MuPos_tight_muon_SF);
                              common_stuff::plot1D(Form("hWlikePos_%sScaled_RWeighted_Templates_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                          MuPos_var_jacobian[k],R_WdivZ_weight*evt_weight*MuPos_tight_muon_SF, h_1d, nbins, bins_scaled[k] );
                            }
                          }
                        }
                        
                        // control plots for different etas but only for central W mass
                        if(TMath::Abs(zmass1/1000 - WMass::ZMass) > 1)  continue;
                        if(controlplots && m==0){
                          // control distributions 
                          // hnvtx[0][i][j]->Fill(nvtx,evt_weight);
                          common_stuff::plot1D(Form("hnvtx_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          nvtx, evt_weight, h_1d, 50,0,50 );
                          // Zmass[0][i][j]->Fill(Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("Zmass_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          Zcorr.M(), evt_weight*MuPos_tight_muon_SF, h_1d, 300,50,200 );
                          
                          // Zmass_zoomed[0][i][j]->Fill(Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("Zmass_zoomed_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          Zcorr.M(), evt_weight*MuPos_tight_muon_SF, h_1d, 6000,60,120 );
                          // ZmassVsMuPosEta[0][i][j]->Fill(muPosCorr.Eta(),Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot2D(Form("ZmassVsMuPosEta_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta(),Zcorr.M(), evt_weight*MuPos_tight_muon_SF, 
                                          h_2d, 50,-2.5,2.5,200,90,92 );
                          // PosMuonPtCorrVsMuPosEta[0][i][j]->Fill(MuPos_eta,muPosCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          // common_stuff::plot2D(Form("PosMuonPtCorrVsMuPosEta_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          // muPosCorr.Eta(),Zcorr.M(),evt_weight*MuPos_tight_muon_SF,
                                          // h_2d, 50,-2.5,2.5,300,60,120
                                        // );
                          // PosMuonPtCorrVsMuPosPhi[0][i][j]->Fill(MuPos_phi,muPosCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          // common_stuff::plot2D(Form("PosMuonPtCorrVsMuPosPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          // muPosCorr.Phi(),Zcorr.M(),evt_weight*MuPos_tight_muon_SF,
                                          // h_2d, 50,-2*TMath::Pi(),2*TMath::Pi(),300,60,120
                                        // );
                          // ZlepDeltaEta[0][i][j]->Fill(muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("ZlepDeltaEta_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF, h_1d, 200,-5,5 );

                          // ZlepEta1VsEta2[0][i][j]->Fill(muPosCorr.Eta(),muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot2D(Form("ZlepEta1VsEta2_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta(),muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 50,-2.5,2.5,50,-2.5,2.5 );
                          // ZlepDeltaEtaVsEta1[0][i][j]->Fill(muPosCorr.Eta(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot2D(Form("ZlepDeltaEtaVsEta1_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 50,-2.5,2.5,100,-5,5 );
                          // ZlepDeltaEtaVsMass[0][i][j]->Fill(Zcorr.M(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                          // ZlepDeltaPt[0][i][j]->Fill(muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("ZlepDeltaPt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF, h_1d, 100,-50,50 );
                          // ZlepPt1VsPt2[0][i][j]->Fill(muPosCorr.Pt(),muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot2D(Form("ZlepPt1VsPt2_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Pt(),muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,0,200,200,0,200 );
                          // ZlepDeltaPtVsPt1[0][i][j]->Fill(muPosCorr.Pt(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot2D(Form("ZlepDeltaPtVsPt1_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Pt(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,0,200,100,-50,50 );
                          // ZlepDeltaPtVsMass[0][i][j]->Fill(Zcorr.M(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          // ZlepDeltaPhi[0][i][j]->Fill(muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("ZlepDeltaPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF, h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                          // ZlepPhi1VsPhi2[0][i][j]->Fill(muPosCorr.Phi(),muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot2D(Form("ZlepPhi1VsPhi2_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Phi(),muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
                          // ZlepDeltaPhiVsPhi1[0][i][j]->Fill(muPosCorr.Phi(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot2D(Form("ZlepDeltaPhiVsPhi1_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Phi(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
                          // ZlepDeltaPhiVsMass[0][i][j]->Fill(Zcorr.M(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          // ZlepDeltaR[0][i][j]->Fill(TMath::Hypot(muPosCorr.Phi()-muNegCorr.Phi(),muPosCorr.Eta()-muNegCorr.Eta()),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("ZlepDeltaR_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF, h_1d, 1000,0,10 );
                          // ZlepDeltaRVsMass[0][i][j]->Fill(Zcorr.M(),TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                          // ZDeltaEtaMuPosJet[0][i][j]->Fill(muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("ZDeltaEtaMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF, h_1d, 200,-5,5 );
                          // ZDeltaPhiMuPosJet[0][i][j]->Fill(muPosCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("ZDeltaPhiMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                          // ZDeltaRMuPosJet[0][i][j]->Fill(TMath::Hypot(muPosCorr.Phi()-Jet_leading_phi,muPosCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("ZDeltaRMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          TMath::Hypot(muPosCorr.Phi()-Jet_leading_phi,muPosCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 1000,0,10 );
                          // ZDeltaEtaMuNegJet[0][i][j]->Fill(muNegCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("ZDeltaEtaMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muNegCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,-5,5 );
                          // ZDeltaPhiMuNegJet[0][i][j]->Fill(muNegCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("ZDeltaPhiMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muNegCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                          // ZDeltaRMuNegJet[0][i][j]->Fill(TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("ZDeltaRMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 1000,0,10 );
                          // hpfMET_Z[0][i][j]->Fill(pfmet_bla,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hpfMET_Z_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          pfmet_bla,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                          // hpfMETphi_Z[0][i][j]->Fill(pfmetphi_bla,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hpfMETphi_Z_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          pfmetphi_bla,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                          // hZ_pt[0][i][j]->Fill(Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          // hZ_phi[0][i][j]->Fill(Zcorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hZ_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          Zcorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                          // hZ_mt[0][i][j]->Fill(Zcorr.Mt(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hZ_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          Zcorr.Mt(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                          // hZ_ptVsmt[0][i][j]->Fill(Zcorr.Mt(),Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot2D(Form("hZ_ptVsmt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          Zcorr.Mt(),Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 150,50,200,100,0,25 );
                          // hWlikePos_pt[0][i][j]->Fill(WlikePos_pt,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hWlikePos_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          WlikePos_pt,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,25 );
                          // hWlikePos_phi[0][i][j]->Fill(WlikePos_phi,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hWlikePos_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          WlikePos_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                          // hWlikePos_mt[0][i][j]->Fill(WlikePos_mt,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hWlikePos_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          WlikePos_mt,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,0,200 );
                          // hpfMET_WlikePos[0][i][j]->Fill(pfmetWlikePos,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hpfMET_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          pfmetWlikePos,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                          // hpfMETphi_WlikePos[0][i][j]->Fill(pfmetWlikePos_phi,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hpfMETphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          pfmetWlikePos_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                          // hMupt_WlikePos[0][i][j]->Fill( muPosCorr.Pt()<200 ? muPosCorr.Pt() : 199.5,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hMupt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Pt()<200 ? muPosCorr.Pt() : 199.5,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,0,200 );
                          // hMueta_WlikePos[0][i][j]->Fill(muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hMueta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-2.5,2.5 );
                          // hMuphi_WlikePos[0][i][j]->Fill(muPosCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hMuphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                          // hMuiso_WlikePos[0][i][j]->Fill(MuPosRelIso,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hMuiso_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          MuPosRelIso,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 1000,0,10 );
                          // hMuMETpt_WlikePos[0][i][j]->Fill(muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hMuMETpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                          // hMuMETeta_WlikePos[0][i][j]->Fill(muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-2.5,2.5 );
                          // hMuMETphi_WlikePos[0][i][j]->Fill(muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                          // hMuMETiso_WlikePos[0][i][j]->Fill(MuNegRelIso,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hMuMETiso_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          MuNegRelIso,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,10 );
                          // hJetpt_WlikePos[0][i][j]->Fill(Jet_leading_pt,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hJetpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          Jet_leading_pt,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,50 );
                          // hJeteta_WlikePos[0][i][j]->Fill(Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          Jet_leading_eta,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100, -2.5,2.5 );
                          // hJetphi_WlikePos[0][i][j]->Fill(Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          Jet_leading_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100, -TMath::Pi(),TMath::Pi() );
                          // hu1_WlikePos[0][i][j]->Fill(u1,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hu1_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          u1,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 360,-30,150 );
                          // hu1_Zpt_WlikePos[0][i][j]->Fill(u1-Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hu1_Zpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          u1-Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 360,-30,150 );
                          // hu2_WlikePos[0][i][j]->Fill(u2,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot1D(Form("hu2_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          u2,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 250,-50,50 );
                          // hu1vsZpt_WlikePos[0][i][j]->Fill(Zcorr.Pt(),u1,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
                                          Zcorr.Pt(),u1,evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 100,0,25,360,-30,150 );
                          // hu2vsZpt_WlikePos[0][i][j]->Fill(Zcorr.Pt(),u2,evt_weight*MuPos_tight_muon_SF);
                          common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jWmass),
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
                    // hWlikePos_logiso_vs_logdxy[i][j]->Fill(TMath::Log10(MuPos_dxy),TMath::Log10(MuPosRelIso),evt_weight*MuPos_tight_muon_SF);
                    // hWlikePos_iso_vs_dxy[i][j]->Fill(MuPos_dxy,MuPosRelIso,evt_weight*MuPos_tight_muon_SF);
                  }

                  // invert cuts to select QCD enriched region, no cuts on met or jet
                  if(
                    MuPosRelIso>0.12 // single muon cuts (inverted iso (is <0.5 for signal) , no tight requirement)
                    && MuPos_dxy>0.02 // single muon cuts (MuPosIsTightAndIso contains dxy < 0.2 cut)
                    && WlikePos_pt<20/* *WMass::ZMass/iZmass */ 
                    ){

                    for(int k=0;k<3;k++)
                      // hWlikePos_VarScaled_QCD[k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);
                      common_stuff::plot1D(Form("hWlikePos_%sScaled_QCD_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),
                                          MuPos_var_jacobian[k], evt_weight*MuPos_tight_muon_SF, h_1d, nbins, bins_scaled[k] );

                    
                    // if( (TMath::Abs(zmass1 - WMass::WMassCentral_MeV) > 1)
                        // ) 
                    // continue;

                    // QCD distributions for central W mass
                    // hnvtx[1][i][j]->Fill(nvtx,evt_weight);
                    // Zmass[1][i][j]->Fill(Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("Zmass_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          nvtx, evt_weight*MuPos_tight_muon_SF, h_1d, 300,50,200 );
                    // Zmass_zoomed[1][i][j]->Fill(Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("Zmass_zoomed_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          nvtx, evt_weight*MuPos_tight_muon_SF, h_1d, 6000,60,120 );
                    // ZmassVsMuPosEta[1][i][j]->Fill(muPosCorr.Eta(),Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot2D(Form("ZmassVsMuPosEta_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta(),Zcorr.M(), evt_weight*MuPos_tight_muon_SF, 
                                          h_2d, 50,-2.5,2.5,200,90,92 );
                    // ZlepDeltaEta[1][i][j]->Fill(muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                    // ZlepEta1VsEta2[1][i][j]->Fill(muPosCorr.Eta(),muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                    // ZlepDeltaEtaVsEta1[1][i][j]->Fill(muPosCorr.Eta(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                    // ZlepDeltaEtaVsMass[1][i][j]->Fill(Zcorr.M(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                    // ZlepDeltaPt[1][i][j]->Fill(muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("ZlepDeltaPt_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF, h_1d, 100,-50,50 );
                    // ZlepPt1VsPt2[1][i][j]->Fill(muPosCorr.Pt(),muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot2D(Form("ZlepPt1VsPt2_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Pt(),muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,0,200,200,0,200 );
                    // ZlepDeltaPtVsPt1[1][i][j]->Fill(muPosCorr.Pt(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot2D(Form("ZlepDeltaPtVsPt1_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Pt(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,0,200,100,-50,50 );
                    // ZlepDeltaPtVsMass[1][i][j]->Fill(Zcorr.M(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    // ZlepDeltaPhi[1][i][j]->Fill(muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("ZlepDeltaPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF, h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                    // ZlepPhi1VsPhi2[1][i][j]->Fill(muPosCorr.Phi(),muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot2D(Form("ZlepPhi1VsPhi2_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Phi(),muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
                    // ZlepDeltaPhiVsPhi1[1][i][j]->Fill(muPosCorr.Phi(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot2D(Form("ZlepDeltaPhiVsPhi1_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Phi(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
                    // ZlepDeltaPhiVsMass[1][i][j]->Fill(Zcorr.M(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    // ZlepDeltaR[1][i][j]->Fill(TMath::Hypot(muPosCorr.Phi()-muNegCorr.Phi(),muPosCorr.Eta()-muNegCorr.Eta()),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("ZlepDeltaR_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF, h_1d, 1000,0,10 );
                    // ZlepDeltaRVsMass[1][i][j]->Fill(Zcorr.M(),TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                    // ZDeltaEtaMuPosJet[1][i][j]->Fill(muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("ZDeltaEtaMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF, h_1d, 200,-5,5 );
                    // ZDeltaPhiMuPosJet[1][i][j]->Fill(muPosCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("ZDeltaPhiMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                    // ZDeltaRMuPosJet[1][i][j]->Fill(TMath::Hypot(muPosCorr.Phi()-Jet_leading_phi,muPosCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("ZDeltaRMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          TMath::Hypot(muPosCorr.Phi()-Jet_leading_phi,muPosCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 1000,0,10 );
                    // ZDeltaEtaMuNegJet[1][i][j]->Fill(muNegCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("ZDeltaEtaMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                    muNegCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF,
                                    h_1d, 200,-5,5 );
                    // ZDeltaPhiMuNegJet[1][i][j]->Fill(muNegCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("ZDeltaPhiMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muNegCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                    // ZDeltaRMuNegJet[1][i][j]->Fill(TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("ZDeltaRMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 1000,0,10 );
                    // hpfMET_Z[1][i][j]->Fill(pfmet_bla,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hpfMET_Z_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                    // hpfMETphi_Z[1][i][j]->Fill(pfmetphi_bla,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hpfMETphi_Z_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          pfmetphi_bla,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                    // hZ_pt[1][i][j]->Fill(Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hZ_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF, h_1d, 100,0,25 );
                    // hZ_phi[1][i][j]->Fill(Zcorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hZ_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          Zcorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                    // hZ_mt[1][i][j]->Fill(Zcorr.Mt(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hZ_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          Zcorr.Mt(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                    // hZ_ptVsmt[1][i][j]->Fill(Zcorr.Mt(),Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot2D(Form("hZ_ptVsmt_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          Zcorr.Mt(),Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 150,50,200,100,0,25 );
                    // hWlikePos_pt[1][i][j]->Fill(WlikePos_pt,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hWlikePos_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          WlikePos_pt,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,25 );
                    // hWlikePos_phi[1][i][j]->Fill(WlikePos_phi,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hWlikePos_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          WlikePos_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                    // hWlikePos_mt[1][i][j]->Fill(WlikePos_mt,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hWlikePos_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          WlikePos_mt,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,0,200 );
                    // hpfMET_WlikePos[1][i][j]->Fill(pfmetWlikePos,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hpfMET_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          pfmetWlikePos,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                    // hpfMETphi_WlikePos[1][i][j]->Fill(pfmetWlikePos_phi,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hpfMETphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          pfmetWlikePos_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                    // hMupt_WlikePos[1][i][j]->Fill(muPosCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hMupt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Pt()<200 ? muPosCorr.Pt() : 199.5,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 200,0,200 );
                    // hMueta_WlikePos[1][i][j]->Fill(muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hMueta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-2.5,2.5 );
                    // hMuphi_WlikePos[1][i][j]->Fill(muPosCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hMuphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                    // hMuiso_WlikePos[1][i][j]->Fill(MuPosRelIso,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hMuiso_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          MuPosRelIso,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 1000,0,10 );
                    // hMuMETpt_WlikePos[1][i][j]->Fill(muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hMuMETpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,200 );
                    // hMuMETeta_WlikePos[1][i][j]->Fill(muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-2.5,2.5 );
                    // hMuMETphi_WlikePos[1][i][j]->Fill(muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,-TMath::Pi(),TMath::Pi() );
                    // hMuMETiso_WlikePos[1][i][j]->Fill(MuNegRelIso,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hMuMETiso_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          MuNegRelIso,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,10 );
                    // hJetpt_WlikePos[1][i][j]->Fill(Jet_leading_pt,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hJetpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          MuNegRelIso,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100,0,50 );
                    // hJeteta_WlikePos[1][i][j]->Fill(Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          Jet_leading_eta,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100, -2.5,2.5 );
                    // hJetphi_WlikePos[1][i][j]->Fill(Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          Jet_leading_phi,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 100, -TMath::Pi(),TMath::Pi() );
                    // hu1_WlikePos[1][i][j]->Fill(u1-Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hu1_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          u1,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 360,-30,150 );
                    // hu1_Zpt_WlikePos[1][i][j]->Fill(u1-Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hu1_Zpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          u1-Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 360,-30,150 );
                    // hu2_WlikePos[1][i][j]->Fill(u2,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot1D(Form("hu2_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          u2,evt_weight*MuPos_tight_muon_SF,
                                          h_1d, 250,-50,50 );
                    // hu1vsZpt_WlikePos[1][i][j]->Fill(Zcorr.Pt(),u1,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
                                          Zcorr.Pt(),u1,evt_weight*MuPos_tight_muon_SF,
                                          h_2d, 100,0,25,360,-30,150 );
                    // hu2vsZpt_WlikePos[1][i][j]->Fill(Zcorr.Pt(),u2,evt_weight*MuPos_tight_muon_SF);
                    common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[1].Data(),eta_str.Data(),jWmass),
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


  if(!sampleName.Contains("DYJetsSig")){
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        if(WMass::WMassNSteps!=j){
          int jWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV);
          for(int k=0;k<3;k++){
            for(int h=0; h<WMass::PDF_members; h++){
              for(int m=0; m<WMass::NtoysMomCorr; m++){
                TString toys_str = "";
                if(WMass::NtoysMomCorr>1) toys_str = Form("_MomCorrToy%d",m);
                
                common_stuff::cloneHisto1D(Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),WMass::WMassCentral_MeV), 
                                            Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass), 
                                            h_1d);
                // hWlikePos_VarScaled_8_JetCut[m][h][k][i][j]=(TH1D*)hWlikePos_VarScaled_8_JetCut[m][h][k][i][WMass::WMassNSteps]->Clone(Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                // hWlikePos_VarScaled_8_JetCut[m][h][k][i][j]->SetName(Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                // hWlikePos_VarScaled_8_JetCut[m][h][k][i][j]->SetTitle(Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                // hWlikePos_VarScaled_8_JetCut[m][h][k][i][j]->Write();
                
                common_stuff::cloneHisto1D(Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),WMass::WMassCentral_MeV), 
                                            Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass), 
                                            h_1d);
                // hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][j]=(TH1D*)hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][WMass::WMassNSteps]->Clone(Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                // hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][j]->SetName(Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                // hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][j]->SetTitle(Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                // hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][j]->Write();
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
