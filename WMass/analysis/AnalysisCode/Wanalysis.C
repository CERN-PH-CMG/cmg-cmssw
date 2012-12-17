#define Wanalysis_cxx
#include "Wanalysis.h"
#include "../includes/common.h"
// #include "rochcor_42X.h"
#include "rochcor_44X_v3.h"
#include "MuScleFitCorrector.h"
#include <TH2.h>
#include <TStyle.h>
#include <TMath.h>
#include <TCanvas.h>
#include <vector>
#include <TGraphAsymmErrors.h>

void Wanalysis::Loop(int IS_MC_CLOSURE_TEST, int isMCorDATA, TString outputdir, int useMomentumCorr, int smearRochCorrByNsigma, int useEffSF, int useVtxSF, TString sampleName)
{
  //   In a ROOT session, you can do:
  //      Root > .L Wanalysis.C
  //      Root > Wanalysis t
  //      Root > t.GetEntry(12); // Fill t data members with entry number 12
  //      Root > t.Show();       // Show values of entry 12
  //      Root > t.Show(16);     // Read and show values of entry 16
  //      Root > t.Loop();       // Loop on all entries
  //

  //     This is the loop skeleton where:
  //    jentry is the global entry number in the chain
  //    ientry is the entry number in the current Tree
  //  Note that the argument to GetEntry must be:
  //    jentry for TChain::GetEntry
  //    ientry for TTree::GetEntry and TBranch::GetEntry
  //
  //       To read only selected branches, Insert statements like:
  // METHOD1:
  //    fChain->SetBranchStatus("*",0);  // disable all branches
  //    fChain->SetBranchStatus("branchname",1);  // activate branchname
  // METHOD2: replace line
  //    fChain->GetEntry(jentry);       //read all branches
  //by  b_branchname->GetEntry(ientry); //read only this branch

  TRandom3 *r = new TRandom3(0);
  
  cout << "output filename= " << Form("%s/Wanalysis.root",outputdir.Data()) << endl;
  TFile*fout = new TFile(Form("%s/Wanalysis.root",outputdir.Data()),"RECREATE");
  
  TH1D *hPileUp_Fall11=new TH1D("hPileUp_Fall11","hPileUp_Fall11",50,0,50);
  TH1D *hWPos_PtScaled_1_Gen[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_2_ZGenMassCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_3_Mu1GenCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_4_Mu2GenCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_5_RecoCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_6_METCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_7_RecoilCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_8_JetCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtNonScaled_8_JetCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_QCD[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];

  TH1D *hnvtx[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1],*hnoTrgMuonsLeadingPt[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hpfMET_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hpfMETphi_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hWPos_pt[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_phi[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hWPos_mt[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hMupt_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hMueta_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hMuphi_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hMulogiso_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hJetpt_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hJeteta_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hJetphi_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hWPos_MuDRgen[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  // TH2D *hWPos_logiso_vs_logdxy[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  // TH2D *hWPos_iso_vs_dxy[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  
  TFile*finEffSF, *finPileupSF;
  TGraphAsymmErrors*hEffSF_MuId_eta_2011[2],*hEffSF_Iso_eta_2011[2],*hEffSF_HLT_eta_2011/* ,*hEffSF_Iso_vtx_2011A,*hEffSF_Iso_vtx_2011B*/;
  TH1D*hPileupSF;

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
  

  static const int nbins=75;
  float bins_scaled[nbins]={0.};
  float bins_Notscaled[nbins]={0.};
  float xmin=0.6,xmax=1.8, x=xmin;
  float binsize1=0.01,binsize2=0.04;
  float binsize=binsize1;
  for(int i=0;i<nbins;i++){
    bins_scaled[i]=x;
    bins_Notscaled[i]=x*80/2;
    if(x>1.2-binsize) binsize=binsize2;
    x+=binsize;
    // cout << bins_scaled[i] << endl;
  }
  bins_scaled[nbins]=xmax;
  bins_Notscaled[nbins]=xmax*80/2;
  // cout << xmax << endl;

  for(int i=0; i<WMass::etaMuonNSteps; i++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");

    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      int jWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV);
      hWPos_PtScaled_1_Gen[i][j]=new TH1D(Form("hWPos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),nbins,bins_scaled);
      hWPos_PtScaled_1_Gen[i][j]->Sumw2();
      hWPos_PtScaled_2_ZGenMassCut[i][j]=new TH1D(Form("hWPos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins_scaled);
      hWPos_PtScaled_2_ZGenMassCut[i][j]->Sumw2();
      hWPos_PtScaled_3_Mu1GenCut[i][j]=new TH1D(Form("hWPos_PtScaled_3_Mu1GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_3_Mu1GenCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins_scaled);
      hWPos_PtScaled_3_Mu1GenCut[i][j]->Sumw2();
      hWPos_PtScaled_4_Mu2GenCut[i][j]=new TH1D(Form("hWPos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins_scaled);
      hWPos_PtScaled_4_Mu2GenCut[i][j]->Sumw2();
      hWPos_PtScaled_5_RecoCut[i][j]=new TH1D(Form("hWPos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins_scaled);
      hWPos_PtScaled_5_RecoCut[i][j]->Sumw2();
      hWPos_PtScaled_6_METCut[i][j]=new TH1D(Form("hWPos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins_scaled);
      hWPos_PtScaled_6_METCut[i][j]->Sumw2();
      hWPos_PtScaled_7_RecoilCut[i][j]=new TH1D(Form("hWPos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins_scaled);
      hWPos_PtScaled_7_RecoilCut[i][j]->Sumw2();
      hWPos_PtScaled_8_JetCut[i][j]=new TH1D(Form("hWPos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins_scaled);
      hWPos_PtScaled_8_JetCut[i][j]->Sumw2();
      hWPos_PtNonScaled_8_JetCut[i][j]=new TH1D(Form("hWPos_PtNonScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtNonScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins_Notscaled);
      hWPos_PtNonScaled_8_JetCut[i][j]->Sumw2();
      hWPos_PtScaled_QCD[i][j]=new TH1D(Form("hWPos_PtScaled_QCD_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_QCD_eta%s_%d",eta_str.Data(),jWmass),nbins,bins_scaled);
      hWPos_PtScaled_QCD[i][j]->Sumw2();
    
      // hWPos_logiso_vs_logdxy[i][j]=new TH2D(Form("hWPos_logiso_vs_logdxy_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_logiso_vs_logdxy_eta%s_%d",eta_str.Data(),jWmass),1000,-5,1,1000,-5,1);
      // hWPos_iso_vs_dxy[i][j]=new TH2D(Form("hWPos_iso_vs_dxy_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_iso_vs_dxy_eta%s_%d",eta_str.Data(),jWmass),1000,-0.001,0.999,1000,0,1);
      
      for(int k=0; k<WMass::nSigOrQCD; k++){
        hnvtx[k][i][j]=new TH1D(Form("hnvtx_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hnvtx_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200);
        hnoTrgMuonsLeadingPt[k][i][j]=new TH1D(Form("hnoTrgMuonsLeadingPt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hnoTrgMuonsLeadingPt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),300,-100,200);
        hpfMET_WPos[k][i][j]=new TH1D(Form("hpfMET_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hpfMET_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200);
        hpfMETphi_WPos[k][i][j]=new TH1D(Form("hpfMETphi_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hpfMETphi_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
        hWPos_pt[k][i][j]=new TH1D(Form("hWPos_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,25);
        hWPos_phi[k][i][j]=new TH1D(Form("hWPos_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
        hWPos_mt[k][i][j]=new TH1D(Form("hWPos_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200);
        hMupt_WPos[k][i][j]=new TH1D(Form("hMupt_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMupt_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200);
        hMueta_WPos[k][i][j]=new TH1D(Form("hMueta_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMueta_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-2.5,2.5);
        hMuphi_WPos[k][i][j]=new TH1D(Form("hMuphi_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMuphi_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
        hMulogiso_WPos[k][i][j]=new TH1D(Form("hMulogiso_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMulogiso_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),1000,-5,3);
        hJetpt_WPos[k][i][j]=new TH1D(Form("hJetpt_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hJetpt_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,50);
        hJeteta_WPos[k][i][j]=new TH1D(Form("hJeteta_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hJeteta_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-2.5,2.5);
        hJetphi_WPos[k][i][j]=new TH1D(Form("hJetphi_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hJetphi_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
        hWPos_MuDRgen[k][i][j]=new TH1D(Form("hWPos_MuDRgen_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_MuDRgen_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),1000,-6,1);
      }
    
    
    
    
    }
  }

  if (fChain == 0) return;

  Long64_t first_entry = 0;
  Long64_t nentries = fChain->GetEntriesFast();
  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==1) first_entry=nentries/2; // in case of closure test, DATA runs from N/2 to N
  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==0) nentries=nentries/2; // in case of closure test, MC runs from 0 to N/2
  if(IS_MC_CLOSURE_TEST==1) lumi_scaling=lumi_scaling*2; // in case of closure test, scaling must be multiplied by 2

  //To get the central value of the momentum correction
  rochcor_44X_v3 *rmcor44X = new rochcor_44X_v3();  // make the pointer of rochcor class
  TString MuscleCard = (IS_MC_CLOSURE_TEST || isMCorDATA==0) ? "MuScleFit_2011_MC_44X" : "MuScleFit_2011_DATA_44X";
  TString fitParametersFile = MuscleCard+".txt";
  MuScleFitCorrector *corrector;
  if(useMomentumCorr==2){
    cout << "using MuscleFit card " << fitParametersFile << endl;
    corrector = new MuScleFitCorrector(fitParametersFile);
  }
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=first_entry; jentry<nentries;jentry++) {
  // for (Long64_t jentry=0; jentry<5e5;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%250000==0) cout <<"Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    
    double evt_weight = lumi_scaling;
    if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && npu>0) evt_weight=lumi_scaling*hPileupSF->GetBinContent(hPileupSF->GetXaxis()->FindBin(npu));
    
    int runopt = r->Rndm()<0.457451 ? 0 : 1;
    double MuPos_tight_muon_SF = 1;
    if(useEffSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
      MuPos_tight_muon_SF = hEffSF_MuId_eta_2011[runopt]->Eval(Mu_eta)*hEffSF_Iso_eta_2011[runopt]->Eval(Mu_eta)*hEffSF_HLT_eta_2011->Eval(Mu_eta);
    }
    // cout << "MuPos_tight_muon_SF= " << MuPos_tight_muon_SF << endl;
    // if(!(IS_MC_CLOSURE_TEST || isMCorDATA==0) && run<175832) continue; // TEMPORARY TO TEST ROCHESTER CORRECTIONS
    
    if(IS_MC_CLOSURE_TEST || isMCorDATA==0) hPileUp_Fall11->Fill(npu);
    
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        double iWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV)/1e3;
          
        // GEN STUFF IF REQUESTED
        if(useGenVar){
          
          if(WGen_m>0){
            if(MuGen_charge>0){
              double MuPosGen_pt_jacobian = 2*MuGen_pt/iWmass;
              // LUCA ADD TO AVOID OVERFLOW
              if(MuPosGen_pt_jacobian>=xmax) MuPosGen_pt_jacobian=xmax-binsize2/2;
              
              hWPos_PtScaled_1_Gen[i][j]->Fill(MuPosGen_pt_jacobian,evt_weight);
              
                hWPos_PtScaled_2_ZGenMassCut[i][j]->Fill(MuPosGen_pt_jacobian,evt_weight);
                
                if(TMath::Abs(MuGen_eta)<WMass::etaMaxMuons[i]){
                  hWPos_PtScaled_3_Mu1GenCut[i][j]->Fill(MuPosGen_pt_jacobian,evt_weight);
                  
                  // if(TMath::Abs(MuNegGen_eta)<2.4){
                    hWPos_PtScaled_4_Mu2GenCut[i][j]->Fill(MuPosGen_pt_jacobian,evt_weight);
                  // }
                }
              // }
            }
          }
        }
          
        if(!useGenVar || W_mt>0){ // dummy thing to separate between sig and bak in W+Jets (useless)
          
          // good reco event selection
          if(evtHasGoodVtx && evtHasTrg && Mu_charge>0){
            TLorentzVector mu; //TLorentzVector of the reconstructed muon
            //Set TLorentzVector of mu, and mubar
            mu.SetPtEtaPhiM(Mu_pt,Mu_eta,Mu_phi,Mu_mass);
            //use rochester correction if required
            if(useMomentumCorr){
              if(IS_MC_CLOSURE_TEST || isMCorDATA==0){
                // int runopt = r->Rndm()<0.457451 ? 0 : 1; // smear MC according to Run2011A and Run2011B statistics (if cut on pileup the 0.457... must be changed accordingly!!!
                rmcor44X->momcor_mc(mu, Mu_charge, smearRochCorrByNsigma/* , runopt */);
              }
              else{
                rmcor44X->momcor_data(mu, Mu_charge, smearRochCorrByNsigma, run<175832 ? 0 : 1 );
              }
            }else if(useMomentumCorr==2){ // use Momentum scale corrections if required
              corrector->applyPtCorrection(mu,Mu_charge);
            }

            double MuPos_pt_jacobian = 2*mu.Pt()/iWmass;
            // LUCA ADD TO AVOID OVERFLOW
            if(MuPos_pt_jacobian>=xmax) MuPos_pt_jacobian=xmax-binsize2/2;
            int wmass1 = iWmass*1e3;

            // good event with mu from W candidate within acceptance
            if( /* Z_mass>50  not possible to apply something similar to Z mass cut*/
                TMath::Abs(mu.Eta())<WMass::etaMaxMuons[i] && mu.Pt()>30
              ){
              // muon candidate is passing tight, iso, dxy requirements
              if(MuIsTightAndIso && MuRelIso<0.12 && Mu_dxy<0.02 && noTrgMuonsLeadingPt<10 ){
                hWPos_PtScaled_5_RecoCut[i][j]->Fill(MuPos_pt_jacobian,evt_weight*MuPos_tight_muon_SF);

                if(pfmet>25){
                  hWPos_PtScaled_6_METCut[i][j]->Fill(MuPos_pt_jacobian,evt_weight*MuPos_tight_muon_SF);
                  
                  if(W_pt<20){
                    hWPos_PtScaled_7_RecoilCut[i][j]->Fill(MuPos_pt_jacobian,evt_weight*MuPos_tight_muon_SF);
                    
                    if(true){ // no jet pt cut at the moment
                    // if(Jet_leading_pt<30){
                      hWPos_PtScaled_8_JetCut[i][j]->Fill(MuPos_pt_jacobian,evt_weight*MuPos_tight_muon_SF);
                      // cout << (mu.Pt()<xmax*80/2 ? mu.Pt() : (xmax-binsize2/2)*80/2 )<< endl;
                      hWPos_PtNonScaled_8_JetCut[i][j]->Fill(mu.Pt()*iWmass/(WMass::WMassCentral_MeV/1e3)<xmax*80/2 ? mu.Pt()*iWmass/(WMass::WMassCentral_MeV/1e3) : (xmax-binsize2/2)*80/2 ,evt_weight*MuPos_tight_muon_SF);
                    
                      // cout << wmass1 << " " << WMass::WMassCentral_MeV << " " << (wmass1 - WMass::WMassCentral_MeV) << endl;
                      // cout << WMass::etaMaxMuons[i]  << " " << 2.1 << " " << ((WMass::etaMaxMuons[i] - 2.1)) << endl;
                      // if( (TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1)
                        // // || (TMath::Abs(WMass::etaMaxMuons[i] - 2.1) > 1e-3 ) 
                        // ) 
                        // continue;
                        
                      // cout << "wmass1 " << wmass1 << " WMass::etaMaxMuons[i]= " << WMass::etaMaxMuons[i] << endl;

                      if(useGenVar) hWPos_MuDRgen[0][i][j]->Fill(TMath::Log10(MuDRGenP),evt_weight*MuPos_tight_muon_SF);
                      hnvtx[0][i][j]->Fill(nvtx,evt_weight); // TO FIT MET IN THE WHOLE RANGE!!!!
                      hnoTrgMuonsLeadingPt[0][i][j]->Fill(noTrgMuonsLeadingPt,evt_weight*MuPos_tight_muon_SF); // TO FIT MET IN THE WHOLE RANGE!!!!
                      hpfMETphi_WPos[0][i][j]->Fill(pfmet_phi,evt_weight*MuPos_tight_muon_SF);
                      hWPos_pt[0][i][j]->Fill(W_pt,evt_weight*MuPos_tight_muon_SF);
                      hWPos_phi[0][i][j]->Fill(W_phi,evt_weight*MuPos_tight_muon_SF);
                      hWPos_mt[0][i][j]->Fill(W_mt,evt_weight*MuPos_tight_muon_SF);
                      hMupt_WPos[0][i][j]->Fill(mu.Pt(),evt_weight*MuPos_tight_muon_SF);
                      hMueta_WPos[0][i][j]->Fill(mu.Eta(),evt_weight*MuPos_tight_muon_SF);
                      hMuphi_WPos[0][i][j]->Fill(mu.Phi(),evt_weight*MuPos_tight_muon_SF);
                      hMulogiso_WPos[0][i][j]->Fill(TMath::Log10(MuRelIso),evt_weight*MuPos_tight_muon_SF);
                      hJetpt_WPos[0][i][j]->Fill(Jet_leading_pt,evt_weight*MuPos_tight_muon_SF);
                      hJeteta_WPos[0][i][j]->Fill(Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                      hJetphi_WPos[0][i][j]->Fill(Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                    
                    }
                  }
                }
                if(W_pt<20){ // do not cut on MET to let the fit have an handle
                        
                  // if(TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1) continue;
                    
                  hpfMET_WPos[0][i][j]->Fill(pfmet,evt_weight*MuPos_tight_muon_SF); // TO FIT MET IN THE WHOLE RANGE!!!!
                    
                }
              }else{ // muon candidate is failing either tight ID, iso or dxy: QCD enriched region
                
                if(pfmet>25 && W_pt<20){
                  // if( (TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1)) continue;
                  // hWPos_logiso_vs_logdxy[i][j]->Fill(TMath::Log10(Mu_dxy),TMath::Log10(MuRelIso),evt_weight*MuPos_tight_muon_SF);
                  // hWPos_iso_vs_dxy[i][j]->Fill(Mu_dxy,MuRelIso,evt_weight*MuPos_tight_muon_SF);
                }
                  
                if(
                  MuRelIso>0.12 // single muon cuts (inverted iso (is <0.5 for signal) , no tight requirement)
                  && Mu_dxy>0.02 // single muon cuts (inverted iso (is <0.5 for signal) , no tight requirement)
                  && W_pt<20
                ){
                
                  hpfMET_WPos[1][i][j]->Fill(pfmet,evt_weight*MuPos_tight_muon_SF);  // TO FIT MET IN THE WHOLE RANGE!!!!  
                  
                  if(pfmet>25){
                    hWPos_PtScaled_QCD[i][j]->Fill(MuPos_pt_jacobian,evt_weight*MuPos_tight_muon_SF);
                    
                    // if( (TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1)
                      // )
                      // continue;
                    
                    if(useGenVar) hWPos_MuDRgen[1][i][j]->Fill(TMath::Log10(MuDRGenP),evt_weight*MuPos_tight_muon_SF);
                    hnvtx[1][i][j]->Fill(nvtx,evt_weight);  // TO FIT MET IN THE WHOLE RANGE!!!!  
                    hnoTrgMuonsLeadingPt[1][i][j]->Fill(noTrgMuonsLeadingPt,evt_weight);  // TO FIT MET IN THE WHOLE RANGE!!!!  
                    hpfMETphi_WPos[1][i][j]->Fill(pfmet_phi,evt_weight*MuPos_tight_muon_SF);
                    hWPos_pt[1][i][j]->Fill(W_pt,evt_weight*MuPos_tight_muon_SF);
                    hWPos_phi[1][i][j]->Fill(W_phi,evt_weight*MuPos_tight_muon_SF);
                    hWPos_mt[1][i][j]->Fill(W_mt,evt_weight*MuPos_tight_muon_SF);
                    hMupt_WPos[1][i][j]->Fill(mu.Pt(),evt_weight*MuPos_tight_muon_SF);
                    hMueta_WPos[1][i][j]->Fill(mu.Eta(),evt_weight*MuPos_tight_muon_SF);
                    hMuphi_WPos[1][i][j]->Fill(mu.Phi(),evt_weight*MuPos_tight_muon_SF);
                    hMulogiso_WPos[1][i][j]->Fill(TMath::Log10(MuRelIso),evt_weight*MuPos_tight_muon_SF);
                    hJetpt_WPos[1][i][j]->Fill(Jet_leading_pt,evt_weight*MuPos_tight_muon_SF);
                    hJeteta_WPos[1][i][j]->Fill(Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                    hJetphi_WPos[1][i][j]->Fill(Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                  }
                }
              }
            } // end if for good muon in acceptance
          } // end good reco event
        } // end dummy separation sig/bkg
      } // end W mass loop
    } // end muon eta loop
  } // end event loop
    
  // }
  
  fout->cd();
  hPileUp_Fall11->Write();
  for(int i=0; i<WMass::etaMuonNSteps; i++){
  
    // hWPos_logiso_vs_logdxy[i]->Write();
    // hWPos_iso_vs_dxy[i]->Write();
    
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      for(int k=0; k<WMass::nSigOrQCD; k++){
        hWPos_MuDRgen[k][i][j]->Write();
        hnvtx[k][i][j]->Write();
        hnoTrgMuonsLeadingPt[k][i][j]->Write();
        hpfMET_WPos[k][i][j]->Write();
        hpfMETphi_WPos[k][i][j]->Write();
        hWPos_pt[k][i][j]->Write();
        hWPos_phi[k][i][j]->Write();
        hWPos_mt[k][i][j]->Write();
        hMupt_WPos[k][i][j]->Write();
        hMueta_WPos[k][i][j]->Write();
        hMuphi_WPos[k][i][j]->Write();
        hMulogiso_WPos[k][i][j]->Write();
        hJetpt_WPos[k][i][j]->Write();
        hJeteta_WPos[k][i][j]->Write();
        hJetphi_WPos[k][i][j]->Write();
      }
      hWPos_PtScaled_1_Gen[i][j]->Write();
      hWPos_PtScaled_2_ZGenMassCut[i][j]->Write();
      hWPos_PtScaled_3_Mu1GenCut[i][j]->Write();
      hWPos_PtScaled_4_Mu2GenCut[i][j]->Write();
      hWPos_PtScaled_5_RecoCut[i][j]->Write();
      hWPos_PtScaled_6_METCut[i][j]->Write();
      hWPos_PtScaled_7_RecoilCut[i][j]->Write();
      hWPos_PtScaled_8_JetCut[i][j]->Write();
      hWPos_PtNonScaled_8_JetCut[i][j]->Write();
      hWPos_PtScaled_QCD[i][j]->Write();
    }
  }

  fout->Write();
  fout->Close();
  
}
