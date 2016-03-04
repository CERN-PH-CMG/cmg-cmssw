// UNCOMMENT TO USE PDF REWEIGHTING
#define LHAPDF_ON

#ifdef LHAPDF_ON
#include "LHAPDF/LHAPDF.h"
#endif 

#define PhiStarEtaAnalysis_cxx
#include "PhiStarEtaAnalysis.h"
#include "../includes/common.h"
// #include "rochcor_42X.h"
#include "common_stuff.h"
#include "rochcor_44X_v3.h"
#include "MuScleFitCorrector.h"
#include "RecoilCorrector.h"
#include "HTransformToHelicityFrame.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TGraphAsymmErrors.h>
#include <ctime>
#include <time.h>


double MuLeadingPt_TRG_Tight_Iso0p12_SF = 1;
double MuSubLeadingPt_Tight_Iso0p5_SF = 1;
int muLeadingPt_charge, muSubLeadingPt_charge, muLeadingPt_trg, muLeadingPt_tight, muSubLeadingPt_tight, muLeadingPt_dxy, muLeadingPt_dz, muSubLeadingPt_dxy, muSubLeadingPt_dz;
double muLeadingPtIso, muSubLeadingPtIso;

TLorentzVector muLeadingPtCorr,muSubLeadingPtCorr, muLeadingPtNoCorr,muSubLeadingPtNoCorr, Zcorr, ZNocorr; //TLorentzVector of the reconstructed muon

TLorentzVector muGenLeadingPt,muGenSubLeadingPt,ZGen; //TLorentzVector of the generated muon
double costh_HX_Gen = -1e10;
double phi_HX_Gen = -1e10;
double costh_HX = -1e10;
double phi_HX = -1e10;
double phistar_gen=-1, logphistar_gen=-1;
double phistar=-1, logphistar=-1;


HTransformToHelicityFrame *GoToHXframe;

rochcor_44X_v3 *rmcor44X;
MuScleFitCorrector *corrector;

void PhiStarEtaAnalysis::Loop(int chunk, int Entry_ini, int Entry_fin, int IS_MC_CLOSURE_TEST, int isMCorDATA, TString outputdir, int buildTemplates, int useMomentumCorr, int smearRochCorrByNsigma, int useEffSF, int usePtSF, int useVtxSF, int controlplots, TString sampleName, int generated_PDF_set, int generated_PDF_member, int contains_PDF_reweight)
{

  if (fChain == 0) return;

  std::map<std::string, TH1D*> h_1d;
  std::map<std::string, TH2D*> h_2d;
  int counter=0;

  cout << "generated_PDF_set= "<<generated_PDF_set
  << " generated_PDF_member= " << generated_PDF_member
  << " contains_PDF_reweight= " << contains_PDF_reweight
  << " WMass::NVarRecoilCorr= " << WMass::NVarRecoilCorr
  << endl;

  // ofstream outTXTfile;

  TRandom3 *r = new TRandom3(0);

  if(!outputdir.Contains("../")) outputdir = "../"+outputdir;
  // cout << "output filename= " << Form("%s/PhiStarEtaAnalysis.root",outputdir.Data()) << endl;
  // outTXTfile.open(Form("%s/PhiStarEtaAnalysis_EVlog.log",outputdir.Data()));
  
  #ifdef LHAPDF_ON
  // LHAPDF::initPDFSet(1,"CT10nnlo.LHgrid");
  if(!sampleName.Contains("DATA")){
    cout << "inizializing LHAPDF::initPDFSet(1)" << endl;
    LHAPDF::initPDFSet(1,generated_PDF_set,generated_PDF_member); // CMSSW DEFAULT
    cout << "finished inizializing LHAPDF" << endl;

    cout << "inizializing LHAPDF::initPDFSet(0)" << endl;
    // LHAPDF::initPDFSet();
    if(WMass::PDF_sets==11200) // LHAPDF::initPDFSet(0,"CT10nnlo.LHgrid");
    LHAPDF::initPDFSet(0,11200,0);
    else if(WMass::PDF_sets==232000) // LHAPDF::initPDFSet(0,"NNPDF23_nnlo_as_0118.LHgrid");
    LHAPDF::initPDFSet(0,232000,0);
    else if(WMass::PDF_sets==21200) // LHAPDF::initPDFSet(0,"MSTW2008nnlo68cl.LHgrid");
    LHAPDF::initPDFSet(0,21200,0);
    else if(WMass::PDF_sets==21241)
    LHAPDF::initPDFSet(0,21241,0);  // else if(WMass::PDF_sets<0) // LHAPDF::initPDFSet(0,generated_PDF_set,generated_PDF_member);
    
  }
  #endif
  
  TH1D*hWlikePos_R_WdivZ[WMass::PDF_members][WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1]; // used only to build templates
  TFile*finTemplates, *finEffSF, *finPileupSF, *finZPtSF;
  TGraphAsymmErrors*hEffSF_MuId_eta_2011[2],*hEffSF_Iso_eta_2011[2],*hEffSF_HLT_eta_2011/* ,*hEffSF_Iso_vtx_2011A,*hEffSF_Iso_vtx_2011B*/;
  TH2F *hEffSF_DATA_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[2],
       *hEffSF_DATA_2011_TIGHT_ISO_PT6_Ptrg_eta_pt[2],
       *hEffSF_DATA_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT,
       *hEffSF_DATA_TIGHT_ISO05_Tid_iso_trg_Ppt_Peta_PLOT,
       *hEff_DATA_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[2],
       *hEff_DATA_2011_TIGHT_ISO_PT6_Ptrg_eta_pt[2],
       *hEff_DATA_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT,
       *hEff_DATA_TIGHT_ISO05_Tid_iso_trg_Ppt_Peta_PLOT,
       *hEff_MC_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[2],
       *hEff_MC_2011_TIGHT_ISO_PT6_Ptrg_eta_pt[2],
       *hEff_MC_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT,
       *hEff_MC_TIGHT_ISO05_Tid_iso_trg_Ppt_Peta_PLOT;
  TH1D *hPileupSF,*hZPtSF;
  
  if(useEffSF==1 && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    finEffSF = new TFile("../utils/MuonEfficiencies_SF_2011_44X_DataMC.root"); // used only to build templates
    hEffSF_MuId_eta_2011[0]=(TGraphAsymmErrors*)finEffSF->Get("SF_TIGHT_nL8_2011A_eta__pt>20");
    hEffSF_MuId_eta_2011[1]=(TGraphAsymmErrors*)finEffSF->Get("SF_TIGHT_nL8_2011B_eta__pt>20");
    hEffSF_Iso_eta_2011[0]=(TGraphAsymmErrors*)finEffSF->Get("combRelPFISO12_2011A_eta__pt>20");
    hEffSF_Iso_eta_2011[1]=(TGraphAsymmErrors*)finEffSF->Get("combRelPFISO12_2011B_eta__pt>20");
    hEffSF_HLT_eta_2011=(TGraphAsymmErrors*)finEffSF->Get("SF_HLT_MuIso24_2011_eta__pt>30");
  }else if(useEffSF==2 /* && !(IS_MC_CLOSURE_TEST || isMCorDATA==0) */){

    finEffSF = new TFile("../utils/MuonEfficiencies_SF_2011_44X_DataMC_Heiner.root"); // used only to build templates
    
    hEff_MC_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[0]=(TH2F*)finEffSF->Get("MC_2011A_HLT_IsoMu24_eta2p1_eta_pair_pt_PLOT_pt_bin0");
    hEff_MC_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[1]=(TH2F*)finEffSF->Get("MC_2011B_HLT_IsoMu24_eta2p1_eta_pair_pt_PLOT_pt_bin0");
    hEff_MC_2011_TIGHT_ISO_PT6_Ptrg_eta_pt[0]=(TH2F*)finEffSF->Get("MC_2011A_TIGHT_ISO_PT6_Ptrg_eta_pt_PLOT");
    hEff_MC_2011_TIGHT_ISO_PT6_Ptrg_eta_pt[1]=(TH2F*)finEffSF->Get("MC_2011B_TIGHT_ISO_PT6_Ptrg_eta_pt_PLOT");
    hEff_MC_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT=(TH2F*)finEffSF->Get("MC_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT");
    hEff_MC_TIGHT_ISO05_Tid_iso_trg_Ppt_Peta_PLOT=(TH2F*)finEffSF->Get("MC_TIGHT_ISO05_Tid_iso_trg_Ppt_Peta_PLOT");

    hEff_DATA_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[0]=(TH2F*)finEffSF->Get("DATA_2011A_HLT_IsoMu24_eta2p1_eta_pair_pt_PLOT_pt_bin0");
    hEff_DATA_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[1]=(TH2F*)finEffSF->Get("DATA_2011B_HLT_IsoMu24_eta2p1_eta_pair_pt_PLOT_pt_bin0");
    hEff_DATA_2011_TIGHT_ISO_PT6_Ptrg_eta_pt[0]=(TH2F*)finEffSF->Get("DATA_2011A_TIGHT_ISO_PT6_Ptrg_eta_pt_PLOT");
    hEff_DATA_2011_TIGHT_ISO_PT6_Ptrg_eta_pt[1]=(TH2F*)finEffSF->Get("DATA_2011B_TIGHT_ISO_PT6_Ptrg_eta_pt_PLOT");
    hEff_DATA_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT=(TH2F*)finEffSF->Get("DATA_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT");
    hEff_DATA_TIGHT_ISO05_Tid_iso_trg_Ppt_Peta_PLOT=(TH2F*)finEffSF->Get("DATA_TIGHT_ISO05_Tid_iso_Ppt_Peta_pt_PLOT");
    
    hEffSF_DATA_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[0]=(TH2F*)finEffSF->Get("SF_DATA_2011A_HLT_IsoMu24_eta2p1_eta_pair_pt_PLOT_pt_bin0");
    hEffSF_DATA_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[1]=(TH2F*)finEffSF->Get("SF_DATA_2011B_HLT_IsoMu24_eta2p1_eta_pair_pt_PLOT_pt_bin0");
    hEffSF_DATA_2011_TIGHT_ISO_PT6_Ptrg_eta_pt[0]=(TH2F*)finEffSF->Get("SF_DATA_2011A_TIGHT_ISO_PT6_Ptrg_eta_pt_PLOT");
    hEffSF_DATA_2011_TIGHT_ISO_PT6_Ptrg_eta_pt[1]=(TH2F*)finEffSF->Get("SF_DATA_2011B_TIGHT_ISO_PT6_Ptrg_eta_pt_PLOT");
    hEffSF_DATA_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT=(TH2F*)finEffSF->Get("SF_DATA_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT");
    hEffSF_DATA_TIGHT_ISO05_Tid_iso_trg_Ppt_Peta_PLOT=(TH2F*)finEffSF->Get("SF_DATA_TIGHT_ISO05_Tid_iso_trg_Ppt_Peta_PLOT");
    
  }
  if(useEffSF!=0 && !finEffSF){
    cout << "file MuonEfficiencies_SF_2011_44X_DataMC.root is missing, impossible to retrieve efficiency scale factors" << endl;
    return;
  }

  if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    // TString vtx_str = sampleName; vtx_str.ReplaceAll("Sig",""); vtx_str.ReplaceAll("Fake","");
    // finPileupSF = new TFile(Form("../utils/pileup_reweighting_%s.root",vtx_str.Data())); // used only to build templates
    finPileupSF = new TFile(Form("../utils/pileup/pileup_reweighting_Fall11_7TeV_Markus.root")); // used only to build templates
    if(!finPileupSF){
      cout << "file " << Form("../utils/pileup/pileup_reweighting_Fall11_7TeV_Markus.root") << " is missing, impossible to retrieve pileup reweighting factors" << endl;
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

  static const int nbins_phistar_ATLAS=34;
  double bins_phistar_ATLAS[nbins_phistar_ATLAS+1]={0, 0.004, 0.008, 0.012, 0.016, 0.02, 0.024, 0.029, 0.034, 0.039, 0.045, 0.051, 
   0.057, 0.064, 0.072, 0.081, 0.091, 0.102, 0.114, 0.128, 0.145, 0.165, 0.189, 0.219, 
   0.258, 0.312, 0.391, 0.524, 0.695, 0.918, 1.153, 1.496, 1.947, 2.522, 3.277};
  double bins_phistar_ATLAS_log[nbins_phistar_ATLAS+1]={0.};
  bins_phistar_ATLAS_log[0]=TMath::Log10(1e-3);
  for(int i=1;i<nbins_phistar_ATLAS+1; i++){
    bins_phistar_ATLAS_log[i]=TMath::Log10(bins_phistar_ATLAS[i]);
  }
  
  static const int nbins_zpt_CMS=18;
  double bins_zpt_CMS[nbins_zpt_CMS+1]={0, 2.5, 5, 7.5, 10, 12.5, 15, 17.5, 20, 30, 40, 50, 70, 90, 110, 150, 190, 250, 600};
  double bins_zpt_CMS_log[nbins_zpt_CMS+1]={0.};
  bins_zpt_CMS_log[0]=TMath::Log10(1e-1);
  for(int i=1;i<nbins_zpt_CMS+1; i++){
    bins_zpt_CMS_log[i]=TMath::Log10(bins_zpt_CMS[i]);
  }

  // the following variables are dummy, but necessary to call the corrector.
  double u1_dummy = 0;
  double u2_dummy = 0;
  double fluc_dummy = 0;
  double zero_dummy = 0;
  int jetMult = 0; // set to zero;
  
  // Long64_t first_entry = 0;
  // Long64_t nentries = fChain->GetEntriesFast();
  Long64_t first_entry = Entry_ini;
  Long64_t nentries = Entry_fin;
  // int chunk = nentries != fChain->GetEntries() && first_entry>0 ? (int)nentries/1e6:0;
  TString chunk_str = chunk>0? Form("_chunk%d",chunk) : "";
  ofstream outTXTfile;
  outTXTfile.open(Form("%s/PhiStarEtaAnalysis_EVlog%s.log",outputdir.Data(),chunk_str.Data()));
  if(!outputdir.Contains("../")) outputdir = "../"+outputdir;
  cout << "output filename= " << Form("%s/PhiStarEtaAnalysis%s.root",outputdir.Data(),chunk_str.Data()) << endl;

  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==1) first_entry=nentries/2; // in case of closure test, DATA runs from N/2 to N
  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==0) nentries=nentries/2; // in case of closure test, MC runs from 0 to N/2
  if(IS_MC_CLOSURE_TEST==1) lumi_scaling=lumi_scaling*2; // in case of closure test, scaling must be multiplied by 2
  
  //To get the central value of the momentum correction
  int random_seed_start=67525;
  rmcor44X = WMass::NVarRecoilCorr>1? new rochcor_44X_v3(random_seed_start) : new rochcor_44X_v3();  // make the pointer of rochcor class
  TString MuscleCard = (IS_MC_CLOSURE_TEST || isMCorDATA==0) ? "MuScleFit_2011_MC_44X" : "MuScleFit_2011_DATA_44X";
  TString fitParametersFile = MuscleCard+".txt";
  if(useMomentumCorr==2){
    cout << "using MuscleFit card " << fitParametersFile << endl;
    corrector = new MuScleFitCorrector(fitParametersFile);
  }

  GoToHXframe = new HTransformToHelicityFrame();
  
  Long64_t nbytes = 0, nb = 0;
  // BEGINNING OF THE EVENT LOOP
  for (Long64_t jentry=first_entry; jentry<nentries;jentry++) {
  // for (Long64_t jentry=0; jentry<5e5+1;jentry++) {
  
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;

    if(jentry%50000==0){
      time_t now = time(0);
      TString dt = ctime(&now); dt.ReplaceAll("\n"," ");
      if(jentry%250000==0) cout << dt << "\t - \t Analyzed entry "<<jentry<<"/"<<nentries<<endl;
      // cout << dt << "\t - \t Analyzed entry "<<jentry<<"/"<<nentries<<endl;
      outTXTfile << dt << "\t - \t Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    }

    for(int i=0; i<WMass::etaMuonNSteps; i++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");

      int runopt_MC = r->Rndm()<0.457451 ? 0 : 1;

      double MuGenLeadingPt_Tight_Iso0p12 = 1;
      double MuGenLeadingPt_TRG_Tight_Iso0p12 = 1;
      double MuGenSubLeadingPt_Tight_Iso0p5 = 1;
      double weightGen = 1;
      double gen_evt_weight_NoEff = lumi_scaling;
      if(useVtxSF && npu>0){
        gen_evt_weight_NoEff *= hPileupSF->GetBinContent(hPileupSF->GetXaxis()->FindBin(npu));
      }
      double gen_evt_weight_Eff = gen_evt_weight_NoEff;

      // // gen stuff to be used only when required
      if(useGenVar){
        
        SelectBestMuPosNegPair();
        AssignRecoLeadingAndSubleadingMuonLabels(useMomentumCorr, IS_MC_CLOSURE_TEST, isMCorDATA, smearRochCorrByNsigma);
        ComputeHXVarAndPhiStarEta();
      
        common_stuff::plot1D("gencounting",1, 1, h_1d, 10, 0.5, 10.5 );
          
        if(ZGen.M()>70 && ZGen.M()<110){
          
          common_stuff::plot1D("gencounting",2, 1, h_1d, 10, 0.5, 10.5 );
          
          if(TMath::Abs(muGenLeadingPt.Eta())<WMass::etaMaxMuons[i]){
            
          common_stuff::plot1D("gencounting",3, 1, h_1d, 10, 0.5, 10.5 );
          
            if(MuPos_pt>0 && MuNeg_pt>0 // TEST TO CHECK TRACKING EFFICIENCY FACTORIZATION
              ){

              common_stuff::plot1D("gencounting",4, 1, h_1d, 10, 0.5, 10.5 );

              if(
                TMath::Abs(muGenSubLeadingPt.Eta())<WMass::etaMaxMuons[i]
                && muGenLeadingPt.Pt()>25 && muGenSubLeadingPt.Pt()>6
                ){
                
                // I'm within acceptance
                common_stuff::plot1D("gencounting",5, 1, h_1d, 10, 0.5, 10.5 );
                                                
                common_stuff::plot1D(Form("logPhiStarEta_gen_InAcceptance_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                       logphistar_gen, gen_evt_weight_NoEff, h_1d, 
                                       nbins_phistar_ATLAS, bins_phistar_ATLAS_log );

                // cout << "lumi_scaling= " << lumi_scaling << " gen_evt_weight_Eff= " << gen_evt_weight_Eff << endl;
                
                if(controlplots){
                common_stuff::plot1D(Form("CosTh_HX_gen_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     costh_HX_Gen, gen_evt_weight_NoEff, h_1d, 
                                     100, -1, 1 );
                common_stuff::plot1D(Form("Phi_HX_gen_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     phi_HX_Gen, gen_evt_weight_NoEff, h_1d, 
                                     100, 0, TMath::Pi() );

                common_stuff::plot1D(Form("CosTh_HX_gen_MCEffWeighted_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     costh_HX_Gen, gen_evt_weight_Eff, h_1d, 
                                     100, -1, 1 );
                common_stuff::plot1D(Form("Phi_HX_gen_MCEffWeighted_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     phi_HX_Gen, gen_evt_weight_Eff, h_1d, 
                                     100, 0, TMath::Pi() );

                common_stuff::plot1D(Form("PhiStarEta_gen_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     phistar_gen, gen_evt_weight_NoEff, h_1d, 
                                     100, 0, 3.277 );
                common_stuff::plot1D(Form("PhiStarEta_gen_MCEffWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     phistar_gen, gen_evt_weight_Eff, h_1d, 
                                     nbins_phistar_ATLAS, bins_phistar_ATLAS );
                if( evtHasGoodVtx /* && evtHasTrg */){ // compare only good reco event
                  common_stuff::plot1D(Form("logPhiStarEta_gen_MCEffWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                       logphistar_gen, gen_evt_weight_Eff, h_1d, 
                                       nbins_phistar_ATLAS, bins_phistar_ATLAS_log );
                }
                common_stuff::plot1D(Form("PhiStarEta_gen_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     phistar_gen, gen_evt_weight_NoEff, h_1d, 
                                     nbins_phistar_ATLAS, bins_phistar_ATLAS );
                common_stuff::plot1D(Form("logPhiStarEta_gen_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     logphistar_gen, gen_evt_weight_NoEff, h_1d, 
                                     nbins_phistar_ATLAS, bins_phistar_ATLAS_log );
                common_stuff::plot1D(Form("Zpt_gen_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     ZGen.Pt(), gen_evt_weight_NoEff, h_1d, 
                                     100, 0, 600 );
                common_stuff::plot1D(Form("Zpt_gen_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     ZGen.Pt(), gen_evt_weight_NoEff, h_1d, 
                                     nbins_zpt_CMS, bins_zpt_CMS );
                common_stuff::plot1D(Form("Zpt_gen_MCEffWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     ZGen.Pt(), gen_evt_weight_Eff, h_1d, 
                                     nbins_zpt_CMS, bins_zpt_CMS );
                common_stuff::plot2D(Form("Zpt_vs_PhiStarEta_gen_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     phistar_gen, ZGen.Pt(), gen_evt_weight_NoEff, h_2d, 
                                     100, 0, 3.277, 100, 0, 600 );
                common_stuff::plot2D(Form("Zpt_vs_PhiStarEta_gen_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     phistar_gen, ZGen.Pt(), gen_evt_weight_NoEff, h_2d, 
                                     nbins_phistar_ATLAS, bins_phistar_ATLAS , nbins_zpt_CMS, bins_zpt_CMS );
                
                common_stuff::plot1D(Form("logPhiStarEta_gen_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     logphistar_gen, 1, h_1d, nbins_phistar_ATLAS, bins_phistar_ATLAS_log );
                common_stuff::plot1D(Form("logZpt_gen_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     TMath::Log10(ZGen.Pt()), 1, h_1d, nbins_zpt_CMS, bins_zpt_CMS_log );
                common_stuff::plot2D(Form("logZpt_vs_logPhiStarEta_gen_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                     logphistar_gen, TMath::Log10(ZGen.Pt()), 1, h_2d, nbins_phistar_ATLAS, bins_phistar_ATLAS_log , nbins_zpt_CMS, bins_zpt_CMS_log );
                }                       
                                       
                MuGenLeadingPt_Tight_Iso0p12 = hEff_MC_2011_TIGHT_ISO_PT6_Ptrg_eta_pt[runopt_MC]->GetBinContent(
                                                  hEff_MC_2011_TIGHT_ISO_PT6_Ptrg_eta_pt[runopt_MC]->FindBin(
                                                  muGenLeadingPt.Eta(), muGenLeadingPt.Pt()
                                                    )
                                                   );

                gen_evt_weight_Eff *= MuGenLeadingPt_Tight_Iso0p12;
                
                common_stuff::plot1D(Form("logPhiStarEta_gen_MCEffLeadTightIsoWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                       logphistar_gen, gen_evt_weight_Eff, h_1d, 
                                       nbins_phistar_ATLAS, bins_phistar_ATLAS_log );

                MuGenLeadingPt_TRG_Tight_Iso0p12 = hEff_MC_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[runopt_MC]->GetBinContent(
                                                  hEff_MC_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[runopt_MC]->FindBin(
                                                  muGenLeadingPt.Eta(), ZGen.Pt()
                                                    )
                                                   );
                         
                gen_evt_weight_Eff *= MuGenLeadingPt_TRG_Tight_Iso0p12;

                common_stuff::plot1D(Form("logPhiStarEta_gen_MCEffLeadTightIsoTRGWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                       logphistar_gen, gen_evt_weight_Eff, h_1d, 
                                       nbins_phistar_ATLAS, bins_phistar_ATLAS_log );
                                       
                // MuGenSubLeadingPt_Tight_Iso0p5 = hEff_MC_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT->GetBinContent(
                                                  // hEff_MC_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT->FindBin(
                                                  // costh_HX_Gen, TMath::Abs(phi_HX_Gen), ZGen.Pt()
                                                    // )
                                                   // );

                MuGenSubLeadingPt_Tight_Iso0p5 = hEff_MC_TIGHT_ISO05_Tid_iso_trg_Ppt_Peta_PLOT->GetBinContent(
                                                  hEff_MC_TIGHT_ISO05_Tid_iso_trg_Ppt_Peta_PLOT->FindBin(
                                                  muGenSubLeadingPt.Pt(), muGenSubLeadingPt.Eta()
                                                    )
                                                   );
                
                gen_evt_weight_Eff *= MuGenSubLeadingPt_Tight_Iso0p5;
                
                common_stuff::plot1D(Form("logPhiStarEta_gen_MCEffLeadTightIsoTRGSubTightIsoWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                       logphistar_gen, gen_evt_weight_Eff, h_1d, 
                                       nbins_phistar_ATLAS, bins_phistar_ATLAS_log );
                common_stuff::plot1D(Form("ZMass_gen_MCEffLeadTightIsoTRGSubTightIsoWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                       ZGen.M(), gen_evt_weight_Eff, h_1d, 
                                       300,50,200 );

                                       
                if(evtHasGoodVtx && TMath::Abs(muLeadingPtCorr.Eta())<WMass::etaMaxMuons[i] 
                    && muLeadingPtCorr.Pt()>25 && muLeadingPt_trg && muLeadingPtIso<0.12 && muLeadingPt_tight
                    && muLeadingPt_dxy < 0.2
                    ){

                  common_stuff::plot1D(Form("logPhiStarEta_gen_MCEffLeadTightIsoPass_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                       logphistar_gen, gen_evt_weight_NoEff, h_1d, 
                                       nbins_phistar_ATLAS, bins_phistar_ATLAS_log );
                                       
                    
                  if(evtHasTrg && muLeadingPt_trg){

                    common_stuff::plot1D(Form("logPhiStarEta_gen_MCEffLeadTightIsoTRGPass_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                         logphistar_gen, gen_evt_weight_NoEff, h_1d, 
                                         nbins_phistar_ATLAS, bins_phistar_ATLAS_log );
                                         
                    if(TMath::Abs(muSubLeadingPtCorr.Eta())<WMass::etaMaxMuons[i] 
                        && muSubLeadingPtCorr.Pt()>6 && muSubLeadingPtIso<0.5 && muSubLeadingPt_tight
                        && muSubLeadingPt_dxy < 0.2
                        ){

                      common_stuff::plot1D(Form("logPhiStarEta_gen_MCEffLeadTightIsoTRGSubTightIsoPass_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                           logphistar_gen, gen_evt_weight_NoEff, h_1d, 
                                           nbins_phistar_ATLAS, bins_phistar_ATLAS_log );

                      // if((gen_evt_weight_Eff/gen_evt_weight_NoEff)<5e-1){ // evt_weight_NoEff/gen_evt_weight_Eff
                        // cout << "gen_evt_weight_NoEff= " << gen_evt_weight_NoEff
                             // << " gen_evt_weight_Eff/gen_evt_weight_NoEff= " << (gen_evt_weight_Eff/gen_evt_weight_NoEff)
                             // << " MuGenLeadingPt: Tight_Iso0p12= " << MuGenLeadingPt_Tight_Iso0p12 
                             // << " TRG_Tight_Iso0p12= " << MuGenLeadingPt_TRG_Tight_Iso0p12 
                             // << " MuGenSubLeadingPt: Tight_Iso0p5= " << MuGenSubLeadingPt_Tight_Iso0p5 
                             // << endl;
                        // cout << "muGenLeadingPt= " << muGenLeadingPt.Pt() << " " << muGenLeadingPt.Eta() << " " << muGenLeadingPt.Phi();
                        // cout << " \t muLeadingPtCorr= " << muLeadingPtCorr.Pt() << " " << muLeadingPtCorr.Eta() << " " << muLeadingPtCorr.Phi() << endl;
                        // cout << "muGenSubLeadingPt= " << muGenSubLeadingPt.Pt() << " " << muGenSubLeadingPt.Eta() << " " << muGenSubLeadingPt.Phi();
                        // cout << " \t muSubLeadingPtCorr= " << muSubLeadingPtCorr.Pt() << " " << muSubLeadingPtCorr.Eta() << " " << muSubLeadingPtCorr.Phi() << endl;
                        // cout << " costh_HX_Gen= " << costh_HX_Gen << " phi_HX_Gen= " << phi_HX_Gen << endl;
                        // cout << " Zcorr= " << Zcorr.Pt() << " " << Zcorr.Rapidity() << " " << Zcorr.Phi() << endl;
                      // }

                    }
                  }
                }
              } 
            }
          }
        }
      } // end gen stuff 
      
      if(!useGenVar || Z_mass>0){ // dummy thing to separate signal and background in DY+Jets (useless)
        for(int m=0; m<WMass::NVarRecoilCorr; m++){
          
          TString toys_str = "";
          if(WMass::NVarRecoilCorr>1) toys_str = Form("_RecoilCorrVar%d",m);
          
          if( evtHasGoodVtx && evtHasTrg){ // good reco event
          
            if(nTrgMuons>2) continue;
            
            double evt_weight_NoEff = lumi_scaling;
            // TO BE CHECKED!!!
            if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && npu>0) evt_weight_NoEff*=hPileupSF->GetBinContent(hPileupSF->GetXaxis()->FindBin(npu));
            if(usePtSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && hZPtSF && sampleName.Contains("DYJetsSig")) evt_weight_NoEff*=hZPtSF->GetBinContent(hZPtSF->GetXaxis()->FindBin(Zcorr.Pt()>0?Zcorr.Pt():ZGen.Pt()));
            
            double evt_weight_Eff = evt_weight_NoEff;
            if(!useGenVar){
              SelectBestMuPosNegPair();
              AssignRecoLeadingAndSubleadingMuonLabels(useMomentumCorr, IS_MC_CLOSURE_TEST, isMCorDATA, smearRochCorrByNsigma);
              ComputeHXVarAndPhiStarEta();
            }

            if(muLeadingPtNoCorr.Pt()>0){
              if(useEffSF==1 && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
                MuLeadingPt_TRG_Tight_Iso0p12_SF = hEffSF_MuId_eta_2011[runopt_MC]->Eval(MuPos_eta)*hEffSF_Iso_eta_2011[runopt_MC]->Eval(MuPos_eta)*hEffSF_HLT_eta_2011->Eval(MuPos_eta);
                MuSubLeadingPt_Tight_Iso0p5_SF = hEffSF_MuId_eta_2011[runopt_MC]->Eval(MuNeg_eta)*hEffSF_Iso_eta_2011[runopt_MC]->Eval(MuNeg_eta)*hEffSF_HLT_eta_2011->Eval(MuPos_eta);
              
              }else if(useEffSF==2 && !(IS_MC_CLOSURE_TEST || isMCorDATA==0)){
              
                MuLeadingPt_TRG_Tight_Iso0p12_SF = hEffSF_DATA_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[run<175832 ? 0 : 1]->GetBinContent(
                                                    hEffSF_DATA_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[run<175832 ? 0 : 1]->FindBin(
                                                      muLeadingPtNoCorr.Eta(), ZNocorr.Pt()
                                                    )
                                                   );
                MuLeadingPt_TRG_Tight_Iso0p12_SF *= hEffSF_DATA_2011_TIGHT_ISO_PT6_Ptrg_eta_pt[run<175832 ? 0 : 1]->GetBinContent(
                                                      hEffSF_DATA_2011_HLT_IsoMu24_eta2p1_eta_pair_pt[run<175832 ? 0 : 1]->FindBin(
                                                        muLeadingPtNoCorr.Eta(), muLeadingPtNoCorr.Pt()
                                                      )
                                                    );
                MuSubLeadingPt_Tight_Iso0p5_SF = hEffSF_DATA_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT->GetBinContent(
                                                   hEffSF_DATA_TIGHT_ISO05_Tid_iso_trg_CosThetaStar_PhiStarAbs_Zpt_PLOT->FindBin(
                                                     costh_HX, TMath::Abs(phi_HX), Zcorr.Pt()
                                                   )
                                                 );
                // cout << "I'm USING THE EFFICIENCIES FOR THE RECO EVENTS AS WELL" << endl;
              }
            }
            
            evt_weight_Eff*=MuLeadingPt_TRG_Tight_Iso0p12_SF*MuSubLeadingPt_Tight_Iso0p5_SF;
            
            if((IS_MC_CLOSURE_TEST || isMCorDATA==0) && controlplots) 
            common_stuff::plot1D("hPileUp_Fall11",
            npu, 1, h_1d, 50,0,50
            );
                        
            TLorentzVector WlikePos_met,WlikePos_nu,WlikePos;
            WlikePos_met.SetPtEtaPhiM(pfmet,0,pfmet_phi,0);
            // WlikePos_nu = muSubLeadingPtCorr + WlikePos_met;
            // WlikePos = muLeadingPtCorr + WlikePos_nu;
            
            // good pair within acceptance cuts for both muons
            if(Zcorr.M()>70 && Zcorr.M()<110
                && TMath::Abs(muLeadingPtCorr.Eta())<WMass::etaMaxMuons[i] 
                && muLeadingPtCorr.Pt()>25 && muLeadingPt_trg && muLeadingPtIso<0.12 && muLeadingPt_tight
                && TMath::Abs(muSubLeadingPtCorr.Eta())<WMass::etaMaxMuons[i] 
                && muSubLeadingPtCorr.Pt()>6 && muSubLeadingPtIso<0.5 && muSubLeadingPt_tight
                && muLeadingPt_charge != muSubLeadingPt_charge
                && muLeadingPt_dxy < 0.2 && muSubLeadingPt_dxy < 0.2
                && muLeadingPt_dz < 0.5 && muSubLeadingPt_dz < 0.5
                // && noTrgExtraMuonsLeadingPt<10
                ){
              // full ID and tight requirements on the muon
              // if(MuPosIsTightAndIso && MuPosRelIso<0.12 && MuPos_dxy<0.02){
                if(true){ // for the momet remove jet pt cut
                  // if(Jet_leading_pt<30){
                  
                  double lha_weight = 1;
                  // double lha_weight = LHAPDF::xfx(0,x1,Q,fl1)*LHAPDF::xfx(0,x2,Q,fl2) / (LHAPDF::xfx(1,x1,Q,fl1)*LHAPDF::xfx(1,x2,Q,fl2));
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
                      if(m==0) 
                        common_stuff::plot1D(Form("hPDF_weights_%d_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h),
                        lha_weight, 1, h_1d, 1000,0,2
                        );
                    }
                    common_stuff::plot1D(Form("log_evt_gen_evt_weight_NoEff_over_gen_evt_weight_Eff_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         TMath::Log10(gen_evt_weight_NoEff/gen_evt_weight_Eff), 1, h_1d, 
                                         200, -10, 10 );
                                         
                    common_stuff::plot1D(Form("PhiStarEta_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         phistar, evt_weight_Eff, h_1d, 
                                         100, 0, 3.277 );
                    common_stuff::plot1D(Form("PhiStarEta_ATLASbin_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         phistar, evt_weight_Eff, h_1d, 
                                         nbins_phistar_ATLAS, bins_phistar_ATLAS );
                    common_stuff::plot1D(Form("logPhiStarEta_ATLASbin_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         logphistar, evt_weight_Eff, h_1d, 
                                         nbins_phistar_ATLAS, bins_phistar_ATLAS_log );
                    common_stuff::plot1D(Form("ZMass_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         Zcorr.M(), evt_weight_Eff, h_1d, 
                                         300,50,200 );
                    common_stuff::plot1D(Form("Zpt_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         Zcorr.Pt(), evt_weight_Eff, h_1d, 
                                         100, 0, 600 );
                                         // nbins_zpt_CMS, bins_zpt_CMS );
                    common_stuff::plot2D(Form("Zpt_vs_PhiStarEta_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         phistar, Zcorr.Pt(), evt_weight_Eff, h_2d, 
                                         100, 0, 3.277, 100, 0, 600 );
                                         // nbins_phistar_ATLAS, bins_phistar_ATLAS , nbins_zpt_CMS, bins_zpt_CMS );
                    common_stuff::plot1D(Form("PhiStarEta_NoEffWeights_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         phistar, evt_weight_NoEff, h_1d, 
                                         100, 0, 3.277 );
                    common_stuff::plot1D(Form("PhiStarEta_NoEffWeights_ATLASbin_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         phistar, evt_weight_NoEff, h_1d, 
                                         nbins_phistar_ATLAS, bins_phistar_ATLAS );
                    common_stuff::plot1D(Form("logPhiStarEta_NoEffWeights_ATLASbin_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         logphistar, evt_weight_NoEff, h_1d, 
                                         nbins_phistar_ATLAS, bins_phistar_ATLAS_log );
                    common_stuff::plot1D(Form("logPhiStarEta_EffUnWeighted_ATLASbin_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         logphistar, (lumi_scaling*gen_evt_weight_NoEff/gen_evt_weight_Eff), h_1d, 
                                         nbins_phistar_ATLAS, bins_phistar_ATLAS_log );
                    common_stuff::plot1D(Form("Zpt_NoEffWeights_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         Zcorr.Pt(), evt_weight_NoEff, h_1d, 
                                         100, 0, 600 );
                                         // nbins_zpt_CMS, bins_zpt_CMS );
                    if(phistar_gen>0){
                      common_stuff::plot2D(Form("PhiStarEta_reco_vs_gen_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                           phistar, phistar_gen, evt_weight_Eff, h_2d, 
                                           // 100, 0, 3.277, 100, 0, 3.277 );
                                           nbins_phistar_ATLAS, bins_phistar_ATLAS , nbins_phistar_ATLAS, bins_phistar_ATLAS );
                      common_stuff::plot2D(Form("zpt_reco_vs_gen_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                           Zcorr.Pt(), ZGen.Pt(), evt_weight_Eff, h_2d, 
                                           // 100, 0, 600, 100, 0, 600 );
                                           nbins_zpt_CMS, bins_zpt_CMS, nbins_zpt_CMS, bins_zpt_CMS );
                    }
                    // common_stuff::plot1D(Form("logPhiStarEta_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         // logphistar, evt_weight_Eff, h_1d, nbins_phistar_ATLAS, bins_phistar_ATLAS_log );
                    // common_stuff::plot1D(Form("logZpt_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         // TMath::Log10(Zcorr.Pt()), evt_weight_Eff, h_1d, nbins_zpt_CMS, bins_zpt_CMS_log );
                    // common_stuff::plot2D(Form("logZpt_vs_logPhiStarEta_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         // logphistar, TMath::Log10(Zcorr.Pt()), evt_weight_Eff, h_2d, nbins_phistar_ATLAS, bins_phistar_ATLAS_log , nbins_zpt_CMS,  bins_zpt_CMS_log );
                    
                    
                  }
                  
                }
                
                // control plots for different etas but only for central W mass
                if(controlplots && m==0){
                  // control distributions 
                  common_stuff::plot1D(Form("hnvtx_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  nvtx, evt_weight_Eff, h_1d, 50,0,50 );
                  common_stuff::plot1D(Form("Zmass_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  Zcorr.M(), evt_weight_Eff, h_1d, 300,50,200 );
                  
                  common_stuff::plot1D(Form("Zmass_zoomed_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  Zcorr.M(), evt_weight_Eff, h_1d, 6000,60,120 );
                  common_stuff::plot2D(Form("ZmassVsMuPosEta_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Eta(),Zcorr.M(), evt_weight_Eff, 
                  h_2d, 50,-2.5,2.5,200,90,92 );
                  common_stuff::plot1D(Form("ZlepDeltaEta_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Eta()-muSubLeadingPtCorr.Eta(),evt_weight_Eff, h_1d, 200,-5,5 );

                  common_stuff::plot2D(Form("ZlepEta1VsEta2_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Eta(),muSubLeadingPtCorr.Eta(),evt_weight_Eff,
                  h_2d, 50,-2.5,2.5,50,-2.5,2.5 );
                  common_stuff::plot2D(Form("ZlepDeltaEtaVsEta1_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Eta(),muLeadingPtCorr.Eta()-muSubLeadingPtCorr.Eta(),evt_weight_Eff,
                  h_2d, 50,-2.5,2.5,100,-5,5 );
                  common_stuff::plot1D(Form("ZlepDeltaPt_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Pt()-muSubLeadingPtCorr.Pt(),evt_weight_Eff, h_1d, 100,-50,50 );
                  common_stuff::plot2D(Form("ZlepPt1VsPt2_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Pt(),muSubLeadingPtCorr.Pt(),evt_weight_Eff,
                  h_2d, 200,0,200,200,0,200 );
                  common_stuff::plot2D(Form("ZlepDeltaPtVsPt1_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Pt(),muLeadingPtCorr.Pt()-muSubLeadingPtCorr.Pt(),evt_weight_Eff,
                  h_2d, 200,0,200,100,-50,50 );
                  common_stuff::plot1D(Form("ZlepDeltaPhi_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Phi()-muSubLeadingPtCorr.Phi(),evt_weight_Eff, h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                  common_stuff::plot2D(Form("ZlepPhi1VsPhi2_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Phi(),muSubLeadingPtCorr.Phi(),evt_weight_Eff,
                  h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
                  common_stuff::plot2D(Form("ZlepDeltaPhiVsPhi1_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Phi(),muLeadingPtCorr.Phi()-muSubLeadingPtCorr.Phi(),evt_weight_Eff,
                  h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
                  common_stuff::plot1D(Form("ZlepDeltaR_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Eta()-Jet_leading_eta,evt_weight_Eff, h_1d, 1000,0,10 );
                  common_stuff::plot1D(Form("ZDeltaEtaMuPosJet_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Eta()-Jet_leading_eta,evt_weight_Eff, h_1d, 200,-5,5 );
                  common_stuff::plot1D(Form("ZDeltaPhiMuPosJet_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Phi()-Jet_leading_phi,evt_weight_Eff,
                  h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                  common_stuff::plot1D(Form("ZDeltaRMuPosJet_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  TMath::Hypot(muLeadingPtCorr.Phi()-Jet_leading_phi,muLeadingPtCorr.Eta()-Jet_leading_eta),evt_weight_Eff,
                  h_1d, 1000,0,10 );
                  common_stuff::plot1D(Form("ZDeltaEtaMuNegJet_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muSubLeadingPtCorr.Eta()-Jet_leading_eta,evt_weight_Eff,
                  h_1d, 200,-5,5 );
                  common_stuff::plot1D(Form("ZDeltaPhiMuNegJet_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muSubLeadingPtCorr.Phi()-Jet_leading_phi,evt_weight_Eff,
                  h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                  common_stuff::plot1D(Form("ZDeltaRMuNegJet_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  TMath::Hypot(muSubLeadingPtCorr.Phi()-Jet_leading_phi,muSubLeadingPtCorr.Eta()-Jet_leading_eta),evt_weight_Eff,
                  h_1d, 1000,0,10 );
                  common_stuff::plot1D(Form("hpfMET_Z_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  pfmet,evt_weight_Eff,
                  h_1d, 100,0,200 );
                  common_stuff::plot1D(Form("hpfMETphi_Z_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  pfmet_phi,evt_weight_Eff,
                  h_1d, 100,-TMath::Pi(),TMath::Pi() );
                  common_stuff::plot1D(Form("hZ_phi_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  Zcorr.Phi(),evt_weight_Eff,
                  h_1d, 100,-TMath::Pi(),TMath::Pi() );
                  common_stuff::plot1D(Form("hZ_mt_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  Zcorr.Mt(),evt_weight_Eff,
                  h_1d, 100,0,200 );
                  common_stuff::plot2D(Form("hZ_ptVsmt_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  Zcorr.Mt(),Zcorr.Pt(),evt_weight_Eff,
                  h_2d, 150,50,200,100,0,25 );
                  common_stuff::plot1D(Form("hWlikePos_pt_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  WlikePos_pt,evt_weight_Eff,
                  h_1d, 100,0,25 );
                  common_stuff::plot1D(Form("hWlikePos_phi_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  WlikePos_phi,evt_weight_Eff,
                  h_1d, 100,-TMath::Pi(),TMath::Pi() );
                  common_stuff::plot1D(Form("hWlikePos_mt_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  WlikePos_mt,evt_weight_Eff,
                  h_1d, 200,0,200 );
                  common_stuff::plot1D(Form("hpfMET_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  pfmetWlikePos,evt_weight_Eff,
                  h_1d, 100,0,200 );
                  common_stuff::plot1D(Form("hpfMETphi_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  pfmetWlikePos_phi,evt_weight_Eff,
                  h_1d, 100,-TMath::Pi(),TMath::Pi() );
                  common_stuff::plot1D(Form("hMupt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Pt()<200 ? muLeadingPtCorr.Pt() : 199.5,evt_weight_Eff,
                  h_1d, 200,0,200 );
                  common_stuff::plot1D(Form("hMueta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Eta(),evt_weight_Eff,
                  h_1d, 100,-2.5,2.5 );
                  common_stuff::plot1D(Form("hMuphi_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muLeadingPtCorr.Eta(),evt_weight_Eff,
                  h_1d, 100,-TMath::Pi(),TMath::Pi() );
                  common_stuff::plot1D(Form("hMuiso_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  MuPosRelIso,evt_weight_Eff,
                  h_1d, 1000,0,10 );
                  common_stuff::plot1D(Form("hMuMETpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muSubLeadingPtCorr.Pt(),evt_weight_Eff,
                  h_1d, 100,0,200 );
                  common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muSubLeadingPtCorr.Eta(),evt_weight_Eff,
                  h_1d, 100,-2.5,2.5 );
                  common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  muSubLeadingPtCorr.Phi(),evt_weight_Eff,
                  h_1d, 100,-TMath::Pi(),TMath::Pi() );
                  common_stuff::plot1D(Form("hMuMETiso_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  MuNegRelIso,evt_weight_Eff,
                  h_1d, 100,0,10 );
                  common_stuff::plot1D(Form("hJetpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  Jet_leading_pt,evt_weight_Eff,
                  h_1d, 100,0,50 );
                  common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  Jet_leading_eta,evt_weight_Eff,
                  h_1d, 100, -2.5,2.5 );
                  common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  Jet_leading_phi,evt_weight_Eff,
                  h_1d, 100, -TMath::Pi(),TMath::Pi() );
                  common_stuff::plot1D(Form("hu1_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  u1,evt_weight_Eff,
                  h_1d, 360,-30,150 );
                  common_stuff::plot1D(Form("hu1_Zpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  u1-Zcorr.Pt(),evt_weight_Eff,
                  h_1d, 360,-30,150 );
                  common_stuff::plot1D(Form("hu2_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  u2,evt_weight_Eff,
                  h_1d, 250,-50,50 );
                  common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  Zcorr.Pt(),u1,evt_weight_Eff,
                  h_2d, 100,0,25,360,-30,150 );
                  common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[0].Data(),eta_str.Data()),
                  Zcorr.Pt(),u2,evt_weight_Eff,
                  h_2d, 100,0,25,250,-50,50 );
                }
            }else{
            
              // // Iso and dxy for muons which fail either tight requirement, isolation or dxy cut
              // if(pfmetWlikePos>25 && WlikePos_pt<20){
                // // if( (TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1)) continue;
                // // hWlikePos_logiso_vs_logdxy[i][j]->Fill(TMath::Log10(MuPos_dxy),TMath::Log10(MuPosRelIso),evt_weight_Eff);
                // // hWlikePos_iso_vs_dxy[i][j]->Fill(MuPos_dxy,MuPosRelIso,evt_weight_Eff);
              // }

              // // invert cuts to select QCD enriched region, no cuts on met or jet
              // if(controlplots && m==0
                  // && MuPosRelIso>0.12 // single muon cuts (inverted iso (is <0.5 for signal) , no tight requirement)
                  // && MuPos_dxy>0.02 // single muon cuts (MuPosIsTightAndIso contains dxy < 0.2 cut)
                  // && WlikePos_pt<20
                  // ){

                // // QCD distributions for central W mass
                // common_stuff::plot1D(Form("Zmass_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // nvtx, evt_weight_Eff, h_1d, 300,50,200 );
                // common_stuff::plot1D(Form("Zmass_zoomed_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // nvtx, evt_weight_Eff, h_1d, 6000,60,120 );
                // common_stuff::plot2D(Form("ZmassVsMuPosEta_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Eta(),Zcorr.M(), evt_weight_Eff, 
                // h_2d, 50,-2.5,2.5,200,90,92 );
                // common_stuff::plot1D(Form("ZlepDeltaPt_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Pt()-muSubLeadingPtCorr.Pt(),evt_weight_Eff, h_1d, 100,-50,50 );
                // common_stuff::plot2D(Form("ZlepPt1VsPt2_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Pt(),muSubLeadingPtCorr.Pt(),evt_weight_Eff,
                // h_2d, 200,0,200,200,0,200 );
                // common_stuff::plot2D(Form("ZlepDeltaPtVsPt1_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Pt(),muLeadingPtCorr.Pt()-muSubLeadingPtCorr.Pt(),evt_weight_Eff,
                // h_2d, 200,0,200,100,-50,50 );
                // common_stuff::plot1D(Form("ZlepDeltaPhi_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Phi()-muSubLeadingPtCorr.Phi(),evt_weight_Eff, h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                // common_stuff::plot2D(Form("ZlepPhi1VsPhi2_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Phi(),muSubLeadingPtCorr.Phi(),evt_weight_Eff,
                // h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
                // common_stuff::plot2D(Form("ZlepDeltaPhiVsPhi1_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Phi(),muLeadingPtCorr.Phi()-muSubLeadingPtCorr.Phi(),evt_weight_Eff,
                // h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
                // common_stuff::plot1D(Form("ZlepDeltaR_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Eta()-Jet_leading_eta,evt_weight_Eff, h_1d, 1000,0,10 );
                // common_stuff::plot1D(Form("ZDeltaEtaMuPosJet_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Eta()-Jet_leading_eta,evt_weight_Eff, h_1d, 200,-5,5 );
                // common_stuff::plot1D(Form("ZDeltaPhiMuPosJet_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Phi()-Jet_leading_phi,evt_weight_Eff,
                // h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                // common_stuff::plot1D(Form("ZDeltaRMuPosJet_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // TMath::Hypot(muLeadingPtCorr.Phi()-Jet_leading_phi,muLeadingPtCorr.Eta()-Jet_leading_eta),evt_weight_Eff,
                // h_1d, 1000,0,10 );
                // common_stuff::plot1D(Form("ZDeltaEtaMuNegJet_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muSubLeadingPtCorr.Eta()-Jet_leading_eta,evt_weight_Eff,
                // h_1d, 200,-5,5 );
                // common_stuff::plot1D(Form("ZDeltaPhiMuNegJet_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muSubLeadingPtCorr.Phi()-Jet_leading_phi,evt_weight_Eff,
                // h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
                // common_stuff::plot1D(Form("ZDeltaRMuNegJet_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // TMath::Hypot(muSubLeadingPtCorr.Phi()-Jet_leading_phi,muSubLeadingPtCorr.Eta()-Jet_leading_eta),evt_weight_Eff,
                // h_1d, 1000,0,10 );
                // common_stuff::plot1D(Form("hpfMET_Z_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // TMath::Hypot(muSubLeadingPtCorr.Phi()-Jet_leading_phi,muSubLeadingPtCorr.Eta()-Jet_leading_eta),evt_weight_Eff,
                // h_1d, 100,0,200 );
                // common_stuff::plot1D(Form("hpfMETphi_Z_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // pfmet_phi,evt_weight_Eff,
                // h_1d, 100,-TMath::Pi(),TMath::Pi() );
                // common_stuff::plot1D(Form("hZ_pt_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // Zcorr.Pt(),evt_weight_Eff, h_1d, 100,0,25 );
                // common_stuff::plot1D(Form("hZ_phi_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // Zcorr.Phi(),evt_weight_Eff,
                // h_1d, 100,-TMath::Pi(),TMath::Pi() );
                // common_stuff::plot1D(Form("hZ_mt_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // Zcorr.Mt(),evt_weight_Eff,
                // h_1d, 100,0,200 );
                // common_stuff::plot2D(Form("hZ_ptVsmt_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // Zcorr.Mt(),Zcorr.Pt(),evt_weight_Eff,
                // h_2d, 150,50,200,100,0,25 );
                // common_stuff::plot1D(Form("hWlikePos_pt_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // WlikePos_pt,evt_weight_Eff,
                // h_1d, 100,0,25 );
                // common_stuff::plot1D(Form("hWlikePos_phi_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // WlikePos_phi,evt_weight_Eff,
                // h_1d, 100,-TMath::Pi(),TMath::Pi() );
                // common_stuff::plot1D(Form("hWlikePos_mt_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // WlikePos_mt,evt_weight_Eff,
                // h_1d, 200,0,200 );
                // common_stuff::plot1D(Form("hpfMET_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // pfmetWlikePos,evt_weight_Eff,
                // h_1d, 100,0,200 );
                // common_stuff::plot1D(Form("hpfMETphi_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // pfmetWlikePos_phi,evt_weight_Eff,
                // h_1d, 100,-TMath::Pi(),TMath::Pi() );
                // common_stuff::plot1D(Form("hMupt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Pt()<200 ? muLeadingPtCorr.Pt() : 199.5,evt_weight_Eff,
                // h_1d, 200,0,200 );
                // common_stuff::plot1D(Form("hMueta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Eta(),evt_weight_Eff,
                // h_1d, 100,-2.5,2.5 );
                // common_stuff::plot1D(Form("hMuphi_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muLeadingPtCorr.Eta(),evt_weight_Eff,
                // h_1d, 100,-TMath::Pi(),TMath::Pi() );
                // common_stuff::plot1D(Form("hMuiso_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // MuPosRelIso,evt_weight_Eff,
                // h_1d, 1000,0,10 );
                // common_stuff::plot1D(Form("hMuMETpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muSubLeadingPtCorr.Pt(),evt_weight_Eff,
                // h_1d, 100,0,200 );
                // common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muSubLeadingPtCorr.Eta(),evt_weight_Eff,
                // h_1d, 100,-2.5,2.5 );
                // common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // muSubLeadingPtCorr.Phi(),evt_weight_Eff,
                // h_1d, 100,-TMath::Pi(),TMath::Pi() );
                // common_stuff::plot1D(Form("hMuMETiso_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // MuNegRelIso,evt_weight_Eff,
                // h_1d, 100,0,10 );
                // common_stuff::plot1D(Form("hJetpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // MuNegRelIso,evt_weight_Eff,
                // h_1d, 100,0,50 );
                // common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // Jet_leading_eta,evt_weight_Eff,
                // h_1d, 100, -2.5,2.5 );
                // common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // Jet_leading_phi,evt_weight_Eff,
                // h_1d, 100, -TMath::Pi(),TMath::Pi() );
                // common_stuff::plot1D(Form("hu1_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // u1,evt_weight_Eff,
                // h_1d, 360,-30,150 );
                // common_stuff::plot1D(Form("hu1_Zpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // u1-Zcorr.Pt(),evt_weight_Eff,
                // h_1d, 360,-30,150 );
                // common_stuff::plot1D(Form("hu2_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // u2,evt_weight_Eff,
                // h_1d, 250,-50,50 );
                // common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // Zcorr.Pt(),u1,evt_weight_Eff,
                // h_2d, 100,0,25,360,-30,150 );
                // common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[1].Data(),eta_str.Data()),
                // Zcorr.Pt(),u2,evt_weight_Eff,
                // h_2d, 100,0,25,250,-50,50 );
                
              // } // end if for qcd enriched
              
              
              // START E-MU SELECTION
              if(SelectBestElMuPair()){
                AssignRecoLeadingAndSubleadingMuonLabels(useMomentumCorr, IS_MC_CLOSURE_TEST, isMCorDATA, smearRochCorrByNsigma);
                ComputeHXVarAndPhiStarEta();
                common_stuff::plot1D(Form("logPhiStarEta_emu_ATLASbin_eta%s",eta_str.Data()),
                                     logphistar, evt_weight_Eff, h_1d, 
                                     nbins_phistar_ATLAS, bins_phistar_ATLAS_log );     
                                     
                common_stuff::plot1D(Form("ZMass_emu_eta%s",eta_str.Data()),
                       Zcorr.M(), evt_weight_Eff, h_1d, 
                       300,50,200 );
              }
              
              
            } // end else for muon cuts (sig or qcd enriched)
            // } // end if for good pair within acceptance cuts for both muons
          } // end if for good reco event
        } // end toys loop
      } // end if for dummy signal/fake separation
      // } // end W mass loop
    } // end muon eta loop
    
  } // end event loop
  
  outTXTfile.close();
  
  TFile*fout = new TFile(Form("%s/PhiStarEtaAnalysis%s.root",outputdir.Data(),chunk_str.Data()),"RECREATE");

    for(int i=0; i<WMass::etaMuonNSteps; i++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      for(int h=0; h<WMass::PDF_members; h++){
        for(int m=0; m<WMass::NVarRecoilCorr; m++){
          TString toys_str = "";
          if(WMass::NVarRecoilCorr>1) toys_str = Form("_RecoilCorrVar%d",m);

          // common_stuff::makeRatioHisto1D(Form("logPhiStarEta_gen_MCEffWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                         // Form("logPhiStarEta_NoEffWeights_ATLASbin_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         // "test_logratio_PhiStarEta", h_1d);
          
          common_stuff::makeRatioHisto1D(Form("logPhiStarEta_gen_MCEffLeadTightIsoWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                         Form("logPhiStarEta_gen_MCEffLeadTightIsoPass_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                        "ratio_logPhiStarEta_gen_LeadTightIsoMuEff_WeightOverPass", h_1d);

          common_stuff::makeRatioHisto1D(Form("logPhiStarEta_gen_MCEffLeadTightIsoTRGWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                         Form("logPhiStarEta_gen_MCEffLeadTightIsoTRGPass_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                        "ratio_logPhiStarEta_gen_LeadTightIsoTRGMuEff_WeightOverPass", h_1d);
          
          common_stuff::makeRatioHisto1D(Form("logPhiStarEta_gen_MCEffLeadTightIsoTRGSubTightIsoWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                         Form("logPhiStarEta_gen_MCEffLeadTightIsoTRGSubTightIsoPass_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                        "ratio_logPhiStarEta_gen_LeadTightIsoTRGSubTightIsoMuEff_WeightOverPass", h_1d);

          common_stuff::makeRatioHisto1D(Form("logPhiStarEta_EffUnWeighted_ATLASbin_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()),
                                         Form("logPhiStarEta_gen_InAcceptance_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()),
                                        "ratio_logPhiStarEta_gen_LeadTightIsoTRGSubTightIsoMuEff_UnWeightOverGen", h_1d);
          
          // std::map<std::string, TH1D*>::iterator numerator= h_1d.find(Form("logPhiStarEta_gen_MCEffWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()));
          // std::map<std::string, TH1D*>::iterator denominator= h_1d.find(Form("logPhiStarEta_NoEffWeights_ATLASbin_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()));  
          // TH1D currentHistolog= (*(*numerator).second)/(*(*denominator).second);
          // currentHistolog.SetName("test_logratio_PhiStarEta");
          // currentHistolog.SetTitle("test_logratio_PhiStarEta");
          
          // numerator= h_1d.find(Form("PhiStarEta_gen_MCEffWeighted_ATLASbin_pdf%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data()));
          // denominator= h_1d.find(Form("PhiStarEta_NoEffWeights_ATLASbin_pdf%d_%d%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data()));  
          // TH1D currentHisto= (*(*numerator).second)/(*(*denominator).second);
          // currentHisto.SetName("test_ratio_PhiStarEta");
          // currentHisto.SetTitle("test_ratio_PhiStarEta");
          
          // fout->cd();
          // currentHistolog.Write();
          // currentHisto.Write();
        
        }
      }
    }

  common_stuff::writeOutHistos( fout, h_1d, h_2d );
  
  fout->cd();
  
  fout->Write();
  fout->Close();

}


//--------------------------------------------------------------------------------------------

void PhiStarEtaAnalysis::AssignRecoLeadingAndSubleadingMuonLabels(int useMomentumCorr, int IS_MC_CLOSURE_TEST, int isMCorDATA, int smearRochCorrByNsigma){

  if(useGenVar){
    if(MuPosGenStatus1_pt>MuNegGenStatus1_pt){
      muGenLeadingPt.SetPtEtaPhiM(MuPosGenStatus1_pt,MuPosGenStatus1_eta,MuPosGenStatus1_phi,MuPos_mass);
      muGenSubLeadingPt.SetPtEtaPhiM(MuNegGenStatus1_pt,MuNegGenStatus1_eta,MuNegGenStatus1_phi,MuNeg_mass);
    }else{
      muGenLeadingPt.SetPtEtaPhiM(MuNegGenStatus1_pt,MuNegGenStatus1_eta,MuNegGenStatus1_phi,MuNeg_mass);
      muGenSubLeadingPt.SetPtEtaPhiM(MuPosGenStatus1_pt,MuPosGenStatus1_eta,MuPosGenStatus1_phi,MuPos_mass);
    }
    ZGen = muGenLeadingPt + muGenSubLeadingPt;
  }
  
  //Set TLorentzVector of mu, and mubar
  if(MuNeg_pt<0 || MuPos_pt<0) return;
  // cout << MuPos_pt << " " << MuPos_eta << " " << MuPos_phi << " " << MuPos_mass << endl;
  // cout << MuNeg_pt << " " << MuNeg_eta << " " << MuNeg_phi << " " << MuNeg_mass << endl;
  if(MuPos_pt>MuNeg_pt){
    muLeadingPtCorr.SetPtEtaPhiM(MuPos_pt,MuPos_eta,MuPos_phi,MuPos_mass);
    muLeadingPt_trg = MuPosTrg;
    muLeadingPt_tight = MuPosIsTight;
    muLeadingPt_charge = MuPos_charge;
    muLeadingPtIso = MuPosRelIso;
    muLeadingPt_dxy = MuPos_dxy;
    muLeadingPt_dz = MuPos_dz;
    muSubLeadingPtCorr.SetPtEtaPhiM(MuNeg_pt,MuNeg_eta,MuNeg_phi,MuNeg_mass);
    muSubLeadingPt_charge = MuNeg_charge;
    muSubLeadingPtIso = MuNegRelIso;
    muSubLeadingPt_tight = MuNegIsTight;
    muSubLeadingPt_dxy = MuNeg_dxy;
  }else{
    muLeadingPtCorr.SetPtEtaPhiM(MuNeg_pt,MuNeg_eta,MuNeg_phi,MuNeg_mass);
    muLeadingPt_trg = MuNegTrg;
    muLeadingPt_tight = MuNegIsTight;
    muLeadingPt_charge = MuNeg_charge;
    muLeadingPtIso = MuNegRelIso;
    muLeadingPt_dxy = TMath::Abs(MuNeg_dxy);
    muLeadingPt_dz = TMath::Abs(MuNeg_dz);
    muSubLeadingPtCorr.SetPtEtaPhiM(MuPos_pt,MuPos_eta,MuPos_phi,MuPos_mass);
    muSubLeadingPt_charge = MuPos_charge;
    muSubLeadingPtIso = MuPosRelIso;
    muSubLeadingPt_tight = MuPosIsTight;
    muSubLeadingPt_dxy = MuPos_dxy;
  }
  muLeadingPtNoCorr = muLeadingPtCorr;
  muSubLeadingPtNoCorr = muSubLeadingPtCorr;
  ZNocorr = muLeadingPtNoCorr + muSubLeadingPtNoCorr;

  // cout << "debug before, useMomentumCorr=" << useMomentumCorr << endl;
  if(useMomentumCorr==1){ // use rochester corrections if required
    if(IS_MC_CLOSURE_TEST || isMCorDATA==0){
      // cout << "debug in useMomentumCorr==1 mc" << endl;
      rmcor44X->momcor_mc(muLeadingPtCorr, muLeadingPt_charge, smearRochCorrByNsigma);
      rmcor44X->momcor_mc(muSubLeadingPtCorr, muSubLeadingPt_charge, smearRochCorrByNsigma);
      // cout << "debug end useMomentumCorr==1 mc" << endl;
      // musclefit_data
    }
    else{
      // cout << "debug in useMomentumCorr==1 data" << endl;
      // muLeadingPtCorr.Print();
      // muSubLeadingPtCorr.Print();
      rmcor44X->momcor_data(muLeadingPtCorr, muLeadingPt_charge, smearRochCorrByNsigma , run<175832 ? 0 : 1 );
      rmcor44X->momcor_data(muSubLeadingPtCorr, muSubLeadingPt_charge, smearRochCorrByNsigma , run<175832 ? 0 : 1 );
      // cout << "debug end useMomentumCorr==1 data" << endl;
    }
  }else if(useMomentumCorr==2){ // use Momentum scale corrections if required
    corrector->applyPtCorrection(muLeadingPtCorr,muLeadingPt_charge);
    corrector->applyPtCorrection(muSubLeadingPtCorr,muSubLeadingPt_charge);
  }

  Zcorr = muLeadingPtCorr + muSubLeadingPtCorr;

}

//--------------------------------------------------------------------------------------------

void PhiStarEtaAnalysis::ComputeHXVarAndPhiStarEta(){

  // GEN
  if(useGenVar){
    if(MuPosGenStatus1_pt>MuNegGenStatus1_pt){
      GoToHXframe->TransformToHelicityFrame(muGenLeadingPt,muGenSubLeadingPt);
    }else{
      GoToHXframe->TransformToHelicityFrame(muGenSubLeadingPt,muGenLeadingPt);
    }

    costh_HX_Gen = GoToHXframe->GetCosTheta();
    phi_HX_Gen = GoToHXframe->GetPhi();

    double D_phi_gen = MuNegGenStatus1_phi-MuPosGenStatus1_phi;
    if ( D_phi_gen > TMath::Pi() ) {
      D_phi_gen -= 2.0*TMath::Pi();
    } else if ( D_phi_gen <= -TMath::Pi() ) {
      D_phi_gen += 2.0*TMath::Pi();
    }
    D_phi_gen=TMath::Abs(D_phi_gen);

    if(D_phi_gen>TMath::Pi()) D_phi_gen-=TMath::Pi();
    phistar_gen=TMath::Tan((TMath::Pi()- D_phi_gen)/2.)*TMath::Sqrt(1.-TMath::Power(TMath::TanH(TMath::Abs(MuNegGenStatus1_eta-MuPosGenStatus1_eta)/2.0),2));
    logphistar_gen=TMath::Log10(phistar_gen);

  }
  
  // RECO
  if(MuPos_pt<0 || MuNeg_pt<0) return;
  
  if(MuPos_pt>MuNeg_pt){
    GoToHXframe->TransformToHelicityFrame(muLeadingPtCorr,muSubLeadingPtCorr);
  }else{
    GoToHXframe->TransformToHelicityFrame(muSubLeadingPtCorr,muLeadingPtCorr);
  }
  
  costh_HX = GoToHXframe->GetCosTheta();
  phi_HX = GoToHXframe->GetPhi();

  // M. Guzzi
  // char hname0[]="phistar", vexp0[]="tan((3.1415- D_phi)/2.)*sqrt(1.-pow(tanh((y_d2-y_d1)/2.0),2))";
  double D_phi = muSubLeadingPtCorr.Phi()-muLeadingPtCorr.Phi();
  if ( D_phi > TMath::Pi() ) {
    D_phi -= 2.0*TMath::Pi();
  } else if ( D_phi <= -TMath::Pi() ) {
    D_phi += 2.0*TMath::Pi();
  }
  D_phi=TMath::Abs(D_phi);

  if(D_phi>TMath::Pi()) D_phi-=TMath::Pi();
  phistar=TMath::Tan((TMath::Pi()- D_phi)/2.)*TMath::Sqrt(1.-TMath::Power(TMath::TanH(TMath::Abs(muSubLeadingPtCorr.Eta()-muLeadingPtCorr.Eta())/2.0),2));
  logphistar=TMath::Log10(phistar);



}

//--------------------------------------------------------------------------------------------

void PhiStarEtaAnalysis::SetMuPosNegZ(const Int_t& iPos, const Int_t& iNeg)
{

  Z_pt = HZ_lv.Pt();
  Z_rap = HZ_lv.Rapidity();
  Z_phi = HZ_lv.Phi();
  Z_mass = HZ_lv.M();
  Z_mt = sqrt(HMuPos_lv.M()*HMuPos_lv.M() + HMuNeg_lv.M()*HMuNeg_lv.M() 
	      + 2.0*(sqrt(HMuPos_lv.M()*HMuPos_lv.M()+HMuPos_lv.Pt()*HMuPos_lv.Pt())*sqrt(HMuNeg_lv.M()*HMuNeg_lv.M()+HMuNeg_lv.Pt()*HMuNeg_lv.Pt())
		     -HMuPos_lv.Px()*HMuNeg_lv.Px() - HMuPos_lv.Py()*HMuNeg_lv.Py()));
  
  MuPos_pt = HMuPos_lv.Pt();
  MuPos_eta = HMuPos_lv.PseudoRapidity();
  MuPos_phi = HMuPos_lv.Phi();

  MuNeg_pt = HMuNeg_lv.Pt();
  MuNeg_eta = HMuNeg_lv.PseudoRapidity();
  MuNeg_phi = HMuNeg_lv.Phi();

  if(iPos>=0) {
    MuPosIsTight = cmgmuons_ID[iPos];
    MuPosRelIso = cmgmuons_Iso[iPos];
    MuPosTrg = cmgmuons_IsTrig[iPos];
    HMuPosIsPrompt = cmgmuons_IsPromt[iPos];
  }
  if(iNeg>=0) {
    MuNegIsTight = cmgmuons_ID[iNeg];
    MuNegRelIso = cmgmuons_Iso[iNeg];
    MuNegTrg = cmgmuons_IsTrig[iNeg];
    HMuNegIsPrompt = cmgmuons_IsPromt[iNeg];
  }

  return;
}


//--------------------------------------------------------------------------------------------

bool PhiStarEtaAnalysis::LeadMuIdTrg(const Int_t& i)
{
  return LeadMuIdTrg(cmgmuons_pt[i],cmgmuons_eta[i],cmgmuons_IsTrig[i],cmgmuons_Iso[i],cmgmuons_ID[i]);
}
//--------------------------------------------------------------------------------------------

bool PhiStarEtaAnalysis::LeadMuIdTrg(const Double_t& pt, const Double_t& eta, const Int_t& trg, 
			   const Double_t& iso, const Int_t& id)
{
#warning hard coded selection cuts in LeadMuIdTrg
  if(pt<25||fabs(eta)>2.1) return false;
  if(trg==0||iso>0.12||id==0) return false;
  return true;
}


//--------------------------------------------------------------------------------------------

bool PhiStarEtaAnalysis::SecondMuId(const Int_t& i)
{
 return SecondMuId(cmgmuons_pt[i],cmgmuons_eta[i],cmgmuons_Iso[i],cmgmuons_ID[i]);
}

//--------------------------------------------------------------------------------------------

bool PhiStarEtaAnalysis::SecondMuId(const Double_t& pt, const Double_t& eta,const Double_t& iso, const Int_t& id)
{
#warning hard coded selection cuts SecondMuId
  if(pt<6||fabs(eta)>2.1) return false;
  if(iso>0.5||id==0) return false;
  return true;
}

//--------------------------------------------------------------------------------------------

void PhiStarEtaAnalysis::SelectBestMuPosNegPair()
{
  if(cmgmuons_number<=2) return;

  if(MuPos_pt > MuNeg_pt) {
    if(LeadMuIdTrg(MuPos_pt,MuPos_eta,MuPosTrg,MuPosRelIso,MuPosIsTight) && SecondMuId(MuNeg_pt,MuNeg_eta,MuNegRelIso,MuNegIsTight))
      return;
  }
  else {
    if(LeadMuIdTrg(MuNeg_pt,MuNeg_eta,MuNegTrg,MuNegRelIso,MuNegIsTight) && SecondMuId(MuPos_pt,MuPos_eta,MuPosRelIso,MuPosIsTight))
      return;
  }
  //the existing muon pair with best Zmass would not pass the selection cuts
  //lets check if there would be a pair, with a worse mass but good muon selections
  
  Int_t muIndex1=-1;
  Int_t muIndex2=-1;
  Int_t Index = cmgmuons_number;
   
  if (cmgmuons_number>8)  Index=9;

  //to avoid all the outliers
#warning another hard coded cut in SelectBestMuPosNegPair
  Double_t dZmass_ref = 50.0;
      
  // search for opposite sign el mu pairs, for which both survive selection
  
  for(Int_t i=0;i<Index;i++)
    {
      //            cout << "HSW:DEBUG  i: "  <<  i << "  " << cmgmuons_pt[i] << "  " << cmgmuons_eta[i] << "  " << cmgmuons_phi[i] << endl; 
      if(LeadMuIdTrg(i))// selected leading muons
	{
	  //the muons are sorted in pt
	  for(Int_t j=i+1;j<Index;j++)
	    {
	      if(cmgmuons_charge[j]!=cmgmuons_charge[i]) {
		if(SecondMuId(j))
		  {
		    if(cmgmuons_charge[j]==1) {
		      if(fabs(MuPos_pt - cmgmuons_pt[j])<0.0001 && fabs(MuNeg_pt - cmgmuons_pt[i])<0.001) continue;
		      HMuPos_lv.SetPtEtaPhiM(cmgmuons_pt[j],cmgmuons_eta[j],cmgmuons_phi[j],0.10566);
		      HMuNeg_lv.SetPtEtaPhiM(cmgmuons_pt[i],cmgmuons_eta[i],cmgmuons_phi[i],0.10566);
		    }
		    else {
		      if(fabs(MuPos_pt - cmgmuons_pt[i])<0.0001 && fabs(MuNeg_pt - cmgmuons_pt[j])<0.001) continue;
		      HMuPos_lv.SetPtEtaPhiM(cmgmuons_pt[i],cmgmuons_eta[i],cmgmuons_phi[i],0.10566);
		      HMuNeg_lv.SetPtEtaPhiM(cmgmuons_pt[j],cmgmuons_eta[j],cmgmuons_phi[j],0.10566);
		    }
		    HZ_lv = HMuPos_lv + HMuNeg_lv;
		    if(fabs(HZ_lv.M()-91.1876) < dZmass_ref) {
		      dZmass_ref = fabs(HZ_lv.M()-91.1876);
		      muIndex1 = i;
		      muIndex2 = j;
		    }
		  }
	      }
	    }
	}
    }

  if (muIndex1==-1) return;

  if(cmgmuons_charge[muIndex1]==1) {
    HMuPos_lv.SetPtEtaPhiM(cmgmuons_pt[muIndex1],cmgmuons_eta[muIndex1],cmgmuons_phi[muIndex1],0.10566);
    HMuNeg_lv.SetPtEtaPhiM(cmgmuons_pt[muIndex2],cmgmuons_eta[muIndex2],cmgmuons_phi[muIndex2],0.10566);
    MuPosMatchCMGmuon = muIndex1;
    MuNegMatchCMGmuon = muIndex2;
  }
  else {
    HMuPos_lv.SetPtEtaPhiM(cmgmuons_pt[muIndex2],cmgmuons_eta[muIndex2],cmgmuons_phi[muIndex2],0.10566);
    HMuNeg_lv.SetPtEtaPhiM(cmgmuons_pt[muIndex1],cmgmuons_eta[muIndex1],cmgmuons_phi[muIndex1],0.10566);
    MuPosMatchCMGmuon = muIndex2;
    MuNegMatchCMGmuon = muIndex1;
  }
  HZ_lv = HMuPos_lv + HMuNeg_lv;

  SetMuPosNegZ(MuPosMatchCMGmuon,MuNegMatchCMGmuon);

}


//--------------------------------------------------------------------------------------------

bool PhiStarEtaAnalysis::ElectronTest(const Int_t& i)
{

  if(cmgelectrons_pt[i]<20||fabs(cmgelectrons_eta[i])>2.1||cmgelectrons_TightID[i]==0) return false;
  return true;
}

//--------------------------------------------------------------------------------------------

bool PhiStarEtaAnalysis::SelectBestElMuPair()
{
  // cout << "cmgelectrons_number: "  <<  cmgelectrons_number << " cmgmuons_number: "  <<  cmgmuons_number << endl; 

  if(cmgmuons_number<1||cmgelectrons_number<1) return false;
  
  Int_t muIndex=-1;
  Int_t elIndex=-1;
  Int_t Index = cmgmuons_number;
   
  if (cmgmuons_number>8)  Index=9;

  //to avoid all the outliers
#warning another hard coded cut in SelectBestElMuPair
  Double_t dZmass_ref = 50.0;
      
  // search for opposite sign el mu pairs, for which both survive selection
  
  int IndexEl =  cmgelectrons_number;
  if(cmgelectrons_number>8) IndexEl=9;
  // cout << "cmgelectrons_number: "  <<  cmgelectrons_number << endl; 


  for(Int_t i=0;i<Index;i++)
  {
    // cout << "HSW:DEBUG  muon_i: "  <<  i << "  " << cmgmuons_pt[i] << "  " << cmgmuons_eta[i] << "  " << cmgmuons_phi[i] << endl; 
    if(LeadMuIdTrg(i))// selected leading muons
    {
      //the muons are sorted in pt
      for(Int_t j=0;j<IndexEl;j++){
      
        // cout << "HSW:DEBUG  phi electron:  " << cmgelectrons_phi[j] << endl;
        if(cmgelectrons_charge[j]!=cmgmuons_charge[i]) {
          if(ElectronTest(j))
          {

            if(cmgelectrons_charge[j]==1) {
              // if(fabs(MuPos_pt - cmgmuons_pt[j])<0.0001 && fabs(MuNeg_pt - cmgmuons_pt[i])<0.001) continue;
              HMuPos_lv.SetPtEtaPhiM(cmgelectrons_pt[j],cmgelectrons_eta[j],cmgelectrons_phi[j],0.0005);
              HMuNeg_lv.SetPtEtaPhiM(cmgmuons_pt[i],cmgmuons_eta[i],cmgmuons_phi[i],0.10566);
            }
            else {
              //  if(fabs(MuPos_pt - cmgmuons_pt[i])<0.0001 && fabs(MuNeg_pt - cmgmuons_pt[j])<0.001) continue;
              HMuPos_lv.SetPtEtaPhiM(cmgmuons_pt[i],cmgmuons_eta[i],cmgmuons_phi[i],0.10566);
              HMuNeg_lv.SetPtEtaPhiM(cmgelectrons_pt[j],cmgelectrons_eta[j],cmgelectrons_phi[j],0.0005);
            }
            HZ_lv = HMuPos_lv + HMuNeg_lv;
            if(fabs(HZ_lv.M()-91.1876) < dZmass_ref) {
              dZmass_ref = fabs(HZ_lv.M()-91.1876);
              muIndex = i;
              elIndex = j;
            }
          }
        }
      }
    }
  }

  if (muIndex==-1) return false;

  if(cmgmuons_charge[muIndex]==1) {
    HMuPos_lv.SetPtEtaPhiM(cmgmuons_pt[muIndex],cmgmuons_eta[muIndex],cmgmuons_phi[muIndex],0.10566);
    HMuNeg_lv.SetPtEtaPhiM(cmgelectrons_pt[elIndex],cmgelectrons_eta[elIndex],cmgelectrons_phi[elIndex],0.0005);
    MuPosMatchCMGmuon = muIndex;
    MuNegMatchCMGmuon = -1;
  }
  else {
    HMuPos_lv.SetPtEtaPhiM(cmgelectrons_pt[elIndex],cmgelectrons_eta[elIndex],cmgelectrons_phi[elIndex],0.0005);
    HMuNeg_lv.SetPtEtaPhiM(cmgmuons_pt[muIndex],cmgmuons_eta[muIndex],cmgmuons_phi[muIndex],0.10566);
    MuPosMatchCMGmuon = -1;
    MuNegMatchCMGmuon = muIndex;
  }
  HZ_lv = HMuPos_lv + HMuNeg_lv;

  SetMuPosNegZ(MuPosMatchCMGmuon,MuNegMatchCMGmuon);
  return true;
}
