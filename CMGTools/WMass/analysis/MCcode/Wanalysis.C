#define Wanalysis_cxx
#include "Wanalysis.h"
#include "../includes/common.h"
#include <TH2.h>
#include <TStyle.h>
#include <TMath.h>
#include <TCanvas.h>
#include <vector>

void Wanalysis::Loop()
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

  TFile*fout = new TFile("WanalysisOnMC.root","RECREATE");
  // int WMassStep_MeV = 80385;
  // int WMassStep_MeV = 15;
  // static const int WMassNSteps = 60;
  // static const int WMass::etaMuonNSteps = 5;
  // double WMass::etaMaxMuons[WMass::etaMuonNSteps] = {0.6, 0.8, 1.2, 1.6, 2.1};
  // std::vector<TH1D*> hWPos_PtScaled_1_Gen;
  TH1D *hpfMET_WPos=new TH1D("hpfMET_WPos","hpfMET_WPos",200,0,200);
  TH1D *hpfMETphi_WPos=new TH1D("hpfMETphi_WPos","hpfMETphi_WPos",100,-TMath::Pi(),TMath::Pi());
  TH1D *hWPos_pt=new TH1D("hWPos_pt","hWPos_pt",100,0,25);
  TH1D *hWPos_phi=new TH1D("hWPos_phi","hWPos_phi",100,-TMath::Pi(),TMath::Pi());
  TH1D *hWPos_mt=new TH1D("hWPos_mt","hWPos_mt",150,50,200);
  TH1D *hMupt_WPos=new TH1D("hMupt_WPos","hMupt_WPos",200,0,200);
  TH1D *hMueta_WPos=new TH1D("hMueta_WPos","hMueta_WPos",100,-2.5,2.5);
  TH1D *hMuphi_WPos=new TH1D("hMuphi_WPos","hMuphi_WPos",100,-TMath::Pi(),TMath::Pi());
  TH1D *hJetpt_WPos=new TH1D("hJetpt_WPos","hJetpt_WPos",100,0,50);
  TH1D *hJeteta_WPos=new TH1D("hJeteta_WPos","hJeteta_WPos",100,-2.5,2.5);
  TH1D *hJetphi_WPos=new TH1D("hJetphi_WPos","hJetphi_WPos",100,-TMath::Pi(),TMath::Pi());
  
  TH1D *hWPos_PtScaled_1_Gen[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_2_ZGenMassCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_3_Mu1GenCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_4_Mu2GenCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_5_RecoCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_6_METCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_7_RecoilCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_PtScaled_8_JetCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];

  static const int nbins=75;
  float bins[nbins]={0.};
  float xmin=0.6,xmax=1.8, x=xmin;
  float binsize1=0.01,binsize2=0.04;
  float binsize=binsize1;
  for(int i=0;i<nbins;i++){
    bins[i]=x;
    if(x>1.2-binsize) binsize=binsize2;
    x+=binsize;
    // cout << bins[i] << endl;
  }
  bins[nbins]=xmax;
  // cout << xmax << endl;

  for(int i=0; i<WMass::etaMuonNSteps; i++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
    int jWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV);
    // hWPos_PtScaled_1_Gen[i][j]=new TH1D(Form("hWPos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    // hWPos_PtScaled_1_Gen[i][j]->Sumw2();
    // hWPos_PtScaled_2_ZGenMassCut[i][j]=new TH1D(Form("hWPos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    // hWPos_PtScaled_2_ZGenMassCut[i][j]->Sumw2();
    // hWPos_PtScaled_3_Mu1GenCut[i][j]=new TH1D(Form("hWPos_PtScaled_3_Mu1GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_3_Mu1GenCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    // hWPos_PtScaled_3_Mu1GenCut[i][j]->Sumw2();
    // hWPos_PtScaled_4_Mu2GenCut[i][j]=new TH1D(Form("hWPos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    // hWPos_PtScaled_4_Mu2GenCut[i][j]->Sumw2();
    // hWPos_PtScaled_5_RecoCut[i][j]=new TH1D(Form("hWPos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    // hWPos_PtScaled_5_RecoCut[i][j]->Sumw2();
    // hWPos_PtScaled_6_METCut[i][j]=new TH1D(Form("hWPos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    // hWPos_PtScaled_6_METCut[i][j]->Sumw2();
    // hWPos_PtScaled_7_RecoilCut[i][j]=new TH1D(Form("hWPos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    // hWPos_PtScaled_7_RecoilCut[i][j]->Sumw2();
    // hWPos_PtScaled_8_JetCut[i][j]=new TH1D(Form("hWPos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    // hWPos_PtScaled_8_JetCut[i][j]->Sumw2();
    hWPos_PtScaled_1_Gen[i][j]=new TH1D(Form("hWPos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
    hWPos_PtScaled_1_Gen[i][j]->Sumw2();
    hWPos_PtScaled_2_ZGenMassCut[i][j]=new TH1D(Form("hWPos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
    hWPos_PtScaled_2_ZGenMassCut[i][j]->Sumw2();
    hWPos_PtScaled_3_Mu1GenCut[i][j]=new TH1D(Form("hWPos_PtScaled_3_Mu1GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_3_Mu1GenCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
    hWPos_PtScaled_3_Mu1GenCut[i][j]->Sumw2();
    hWPos_PtScaled_4_Mu2GenCut[i][j]=new TH1D(Form("hWPos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
    hWPos_PtScaled_4_Mu2GenCut[i][j]->Sumw2();
    hWPos_PtScaled_5_RecoCut[i][j]=new TH1D(Form("hWPos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
    hWPos_PtScaled_5_RecoCut[i][j]->Sumw2();
    hWPos_PtScaled_6_METCut[i][j]=new TH1D(Form("hWPos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
    hWPos_PtScaled_6_METCut[i][j]->Sumw2();
    hWPos_PtScaled_7_RecoilCut[i][j]=new TH1D(Form("hWPos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
    hWPos_PtScaled_7_RecoilCut[i][j]->Sumw2();
    hWPos_PtScaled_8_JetCut[i][j]=new TH1D(Form("hWPos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
    hWPos_PtScaled_8_JetCut[i][j]->Sumw2();
    }
  }
  // return;

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
  // for (Long64_t jentry=0; jentry<5e5;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%250000==0) cout <<"Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    
    if(evtWSel>0 && Mu_charge>0 && MuIsTightAndIso && TMath::Abs(Mu_eta)<2.1 && Mu_pt>30 && Jet_leading_pt<30 && W_pt<20){
      hpfMET_WPos->Fill(pfmet);
      hpfMETphi_WPos->Fill(pfmet_phi);
      hWPos_pt->Fill(W_pt);
      hWPos_phi->Fill(W_phi);
      hWPos_mt->Fill(W_mt); // TO BE CORRECTED!!!!
      hMupt_WPos->Fill(Mu_pt);
      hMueta_WPos->Fill(Mu_eta);
      hMuphi_WPos->Fill(Mu_phi);
      hJetpt_WPos->Fill(Jet_leading_pt);
      hJeteta_WPos->Fill(Jet_leading_eta);
      hJetphi_WPos->Fill(Jet_leading_phi);
    }
    
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      // if(TMath::Abs(mu1_eta)>WMass::etaMaxMuons[i]) continue;
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        double iWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV)/1e3;
        // cout << "j " << j << " iWmass= " << iWmass << endl;
        // cout << "mu1_pt/80.385 = " << mu1_pt<<"/80.385 = " << (mu1_pt/80.385) << endl;
        // hWPos_PtScaled_1_Gen[i][j]->Fill(2*mu1_pt/iWmass);
        // hWPos_PtScaled_1_Gen[j]->Print();
        if(WGen_m>0){
          if(MuGen_charge>0){
            double MuPosGen_pt_jacobian = 2*MuGen_pt/iWmass;
            hWPos_PtScaled_1_Gen[i][j]->Fill(MuPosGen_pt_jacobian);
            
            // if(ZGen_mass>70 && ZGen_mass<110){
              hWPos_PtScaled_2_ZGenMassCut[i][j]->Fill(MuPosGen_pt_jacobian);
              
              if(TMath::Abs(MuGen_eta)<WMass::etaMaxMuons[i]){
                hWPos_PtScaled_3_Mu1GenCut[i][j]->Fill(MuPosGen_pt_jacobian);
                
                // if(TMath::Abs(MuNegGen_eta)<2.4){
                  hWPos_PtScaled_4_Mu2GenCut[i][j]->Fill(MuPosGen_pt_jacobian);
                // }
              }
            // }
          }
          if(evtHasGoodVtx && evtHasTrg && Mu_charge>0 && MuIsTightAndIso>0){
            double MuPos_pt_jacobian = 2*Mu_pt/iWmass;
            if( /* Z_mass>70 && Z_mass<120 && */
                TMath::Abs(Mu_eta)<WMass::etaMaxMuons[i] && Mu_pt>30 && MuIsTightAndIso /* && MuTrg */
              ){
              hWPos_PtScaled_5_RecoCut[i][j]->Fill(MuPos_pt_jacobian);

              if(pfmet>25){
                hWPos_PtScaled_6_METCut[i][j]->Fill(MuPos_pt_jacobian);
                
                if(W_pt<20){
                  hWPos_PtScaled_7_RecoilCut[i][j]->Fill(MuPos_pt_jacobian);
                  
                  if(Jet_leading_pt<30){
                    hWPos_PtScaled_8_JetCut[i][j]->Fill(MuPos_pt_jacobian);
                  }
                }
              }
            }
          }
        }
      }
    }
    // cout << endl;
    
  }
  
  fout->cd();
  hpfMET_WPos->Write();
  hpfMETphi_WPos->Write();
  hWPos_pt->Write();
  hWPos_phi->Write();
  hWPos_mt->Write();
  hMupt_WPos->Write();
  hMueta_WPos->Write();
  hMuphi_WPos->Write();
  hJetpt_WPos->Write();
  hJeteta_WPos->Write();
  hJetphi_WPos->Write();
  for(int i=0; i<WMass::etaMuonNSteps; i++){
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      hWPos_PtScaled_1_Gen[i][j]->Write();
      hWPos_PtScaled_2_ZGenMassCut[i][j]->Write();
      hWPos_PtScaled_3_Mu1GenCut[i][j]->Write();
      hWPos_PtScaled_4_Mu2GenCut[i][j]->Write();
      hWPos_PtScaled_5_RecoCut[i][j]->Write();
      hWPos_PtScaled_6_METCut[i][j]->Write();
      hWPos_PtScaled_7_RecoilCut[i][j]->Write();
      hWPos_PtScaled_8_JetCut[i][j]->Write();
    }
  }

  fout->Write();
  fout->Close();
  
}
