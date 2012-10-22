#define Wanalysis_cxx
#include "Wanalysis.h"
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
  int WMassCentral = 80385;
  int WMassStep = 15;
  static const int WMassNSteps = 60;
  static const int etaMuonNSteps = 5;
  double etaMaxMuons[etaMuonNSteps] = {0.6, 0.8, 1.2, 1.6, 2.1};
  // std::vector<TH1D*> hWPos_PtScaled_1_Gen;
  TH1D *hpfMET=new TH1D("hpfMET","hpfMET",200,0,200);
  TH1D *hpfMETphi=new TH1D("hpfMETphi","hpfMETphi",100,-TMath::Pi(),TMath::Pi());
  TH1D *hWpt=new TH1D("hWpt","hWpt",100,0,25);
  TH1D *hWphi=new TH1D("hWphi","hWphi",100,-TMath::Pi(),TMath::Pi());
  TH1D *hWmt=new TH1D("hWmt","hWmt",150,50,200);
  TH1D *hmuWpt=new TH1D("hmuWpt","hmuWpt",200,0,200);
  TH1D *hmuWeta=new TH1D("hmuWeta","hmuWeta",100,-2.5,2.5);
  TH1D *hmuWphi=new TH1D("hmuWphi","hmuWphi",100,-TMath::Pi(),TMath::Pi());
  TH1D *hjetpt=new TH1D("hjetpt","hjetpt",100,0,50);
  TH1D *hjeteta=new TH1D("hjeteta","hjeteta",100,-2.5,2.5);
  TH1D *hjetphi=new TH1D("hjetphi","hjetphi",100,-TMath::Pi(),TMath::Pi());
  
  TH1D *hWPos_PtScaled_1_Gen[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWPos_PtScaled_2_ZGenMassCut[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWPos_PtScaled_3_Mu1RecoCut[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWPos_PtScaled_4_Mu2GenCut[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWPos_PtScaled_5_RecoCut[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWPos_PtScaled_6_METCut[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWPos_PtScaled_7_RecoilCut[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWPos_PtScaled_8_JetCut[etaMuonNSteps][2*WMassNSteps+1];

  for(int i=0; i<etaMuonNSteps; i++){
    TString eta_str = Form("%.1f",etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
    for(int j=0; j<2*WMassNSteps+1; j++){
    int jWmass = WMassCentral-(WMassNSteps-j)*WMassStep;
    hWPos_PtScaled_1_Gen[i][j]=new TH1D(Form("hWPos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),Form("Wmu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    hWPos_PtScaled_1_Gen[i][j]->Sumw2();
    hWPos_PtScaled_2_ZGenMassCut[i][j]=new TH1D(Form("hWPos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wmu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    hWPos_PtScaled_2_ZGenMassCut[i][j]->Sumw2();
    hWPos_PtScaled_3_Mu1RecoCut[i][j]=new TH1D(Form("hWPos_PtScaled_3_Mu1RecoCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wmu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    hWPos_PtScaled_3_Mu1RecoCut[i][j]->Sumw2();
    hWPos_PtScaled_4_Mu2GenCut[i][j]=new TH1D(Form("hWPos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wmu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    hWPos_PtScaled_4_Mu2GenCut[i][j]->Sumw2();
    hWPos_PtScaled_5_RecoCut[i][j]=new TH1D(Form("hWPos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wmu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    hWPos_PtScaled_5_RecoCut[i][j]->Sumw2();
    hWPos_PtScaled_6_METCut[i][j]=new TH1D(Form("hWPos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wmu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    hWPos_PtScaled_6_METCut[i][j]->Sumw2();
    hWPos_PtScaled_7_RecoilCut[i][j]=new TH1D(Form("hWPos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wmu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
    hWPos_PtScaled_7_RecoilCut[i][j]->Sumw2();
    hWPos_PtScaled_8_JetCut[i][j]=new TH1D(Form("hWPos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wmu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
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
    if(jentry%50000==0) cout <<"Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    
    // hpfMET->Fill(pfmet);
    // hpfMETphi->Fill(pfmet_phi);
    // hWpt->Fill(W_pt);
    // hWphi->Fill(W_phi);
    // hWmt->Fill(W_mt);
    // hmuWpt->Fill(mu1_pt);
    // hmuWeta->Fill(mu1_eta);
    // hmuWphi->Fill(mu1_phi);
    // hjetpt->Fill(jet_pt);
    // hjeteta->Fill(jet_eta);
    // hjetphi->Fill(jet_phi);
    
    for(int i=0; i<etaMuonNSteps; i++){
      // if(TMath::Abs(mu1_eta)>etaMaxMuons[i]) continue;
      // for(int j=0; j<2*WMassNSteps+1; j++){
      for(int j=60; j<61; j++){
        double iWmass = (WMassCentral-(WMassNSteps-j)*WMassStep)/1e3;
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
              
              if(TMath::Abs(MuGen_eta)<etaMaxMuons[i]){
                hWPos_PtScaled_3_Mu1RecoCut[i][j]->Fill(MuPosGen_pt_jacobian);
                
                // if(TMath::Abs(MuNegGen_eta)<2.4){
                  hWPos_PtScaled_4_Mu2GenCut[i][j]->Fill(MuPosGen_pt_jacobian);
                // }
              }
            // }
          }
          if(evtHasGoodVtx && evtHasTrg && Mu_charge>0){
            double MuPos_pt_jacobian = 2*Mu_pt/iWmass;
            if( /* Z_mass>70 && Z_mass<120 && */
                TMath::Abs(Mu_eta)<etaMaxMuons[i] && Mu_pt>30 && MuIsTight /* && MuTrg */
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
  hpfMET->Write();
  hpfMETphi->Write();
  hWpt->Write();
  hWphi->Write();
  hWmt->Write();
  hmuWpt->Write();
  hmuWeta->Write();
  hmuWphi->Write();
  hjetpt->Write();
  hjeteta->Write();
  hjetphi->Write();
  for(int i=0; i<etaMuonNSteps; i++){
    // for(int j=0; j<2*WMassNSteps+1; j++){
    for(int j=60; j<61; j++){
      hWPos_PtScaled_1_Gen[i][j]->Write();
      hWPos_PtScaled_2_ZGenMassCut[i][j]->Write();
      hWPos_PtScaled_3_Mu1RecoCut[i][j]->Write();
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
