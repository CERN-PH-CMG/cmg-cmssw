#define Zanalysis_cxx
#include "Zanalysis.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLorentzVector.h>

void Zanalysis::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L Zanalysis.C
//      Root > Zanalysis t
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

  TFile*fout = new TFile("ZanalysisOnMC.root","RECREATE");
  double ZMass = 91.1876;
  int WMassCentral = 80385;
  int WMassStep = 15;
  static const int WMassNSteps = 60;
  static const int etaMuonNSteps = 5;
  double etaMaxMuons[etaMuonNSteps] = {0.6, 0.8, 1.2, 1.6, 2.1};
  // std::vector<TH1D*> hWlike_PtScaled;
  TH1D *hWlikePos_PtScaled_1_Gen[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_2_ZGenMassCut[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_3_Mu1RecoCut[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_4_Mu2GenCut[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_5_RecoCut[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_6_METCut[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_7_RecoilCut[etaMuonNSteps][2*WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_8_JetCut[etaMuonNSteps][2*WMassNSteps+1];

  for(int i=0; i<etaMuonNSteps; i++){
    TString eta_str = Form("%.1f",etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
    for(int j=0; j<2*WMassNSteps+1; j++){
      int jWmass = WMassCentral-(WMassNSteps-j)*WMassStep;
      hWlikePos_PtScaled_1_Gen[i][j]=new TH1D(Form("hWlikePos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),Form("Wlikemu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      hWlikePos_PtScaled_1_Gen[i][j]->Sumw2();
      hWlikePos_PtScaled_2_ZGenMassCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wlikemu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      hWlikePos_PtScaled_2_ZGenMassCut[i][j]->Sumw2();
      hWlikePos_PtScaled_3_Mu1RecoCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_3_Mu1RecoCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wlikemu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      hWlikePos_PtScaled_3_Mu1RecoCut[i][j]->Sumw2();
      hWlikePos_PtScaled_4_Mu2GenCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wlikemu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      hWlikePos_PtScaled_4_Mu2GenCut[i][j]->Sumw2();
      hWlikePos_PtScaled_5_RecoCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wlikemu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      hWlikePos_PtScaled_5_RecoCut[i][j]->Sumw2();
      hWlikePos_PtScaled_6_METCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wlikemu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      hWlikePos_PtScaled_6_METCut[i][j]->Sumw2();
      hWlikePos_PtScaled_7_RecoilCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wlikemu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      hWlikePos_PtScaled_7_RecoilCut[i][j]->Sumw2();
      hWlikePos_PtScaled_8_JetCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),Form("Wlikemu_pt_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      hWlikePos_PtScaled_8_JetCut[i][j]->Sumw2();
    }
  }

  TH1D *Zmass=new TH1D("Zmass","Zmass",200,0,200);
  
  TH1D *ZlepDeltaEta=new TH1D("ZlepDeltaEta","ZlepDeltaEta",200,-5,5);
  TH2D *ZlepEta1VsEta2=new TH2D("ZlepEta1VsEta2","ZlepEta1VsEta2",50,-2.5,2.5,50,-2.5,2.5);
  TH2D *ZlepDeltaEtaVsEta1=new TH2D("ZlepDeltaEtaVsEta1","ZlepDeltaEtaVsEta1",50,-2.5,2.5,100,-5,5);

  TH1D *hpfMET=new TH1D("hpfMET","hpfMET",200,0,200);
  TH1D *hpfMETphi=new TH1D("hpfMETphi","hpfMETphi",100,-TMath::Pi(),TMath::Pi());
  TH1D *hZproperpt=new TH1D("hZproperpt","hZproperpt",100,0,25);
  TH1D *hZproperphi=new TH1D("hZproperphi","hZproperphi",100,-TMath::Pi(),TMath::Pi());
  TH1D *hZpropermt=new TH1D("hZpropermt","hZpropermt",150,50,200);
  TH1D *hWlikept=new TH1D("hWlikept","hWlikept",100,0,25);
  TH1D *hWlikephi=new TH1D("hWlikephi","hWlikephi",100,-TMath::Pi(),TMath::Pi());
  TH1D *hWlikemt=new TH1D("hWlikemt","hWlikemt",150,50,200);
  TH1D *hmuWlikept=new TH1D("hmuWlikept","hmuWlikept",200,0,200);
  TH1D *hmuWlikeeta=new TH1D("hmuWlikeeta","hmuWlikeeta",100,-2.5,2.5);
  TH1D *hmuWlikephi=new TH1D("hmuWlikephi","hmuWlikephi",100,-TMath::Pi(),TMath::Pi());
  TH1D *hmuMETpt=new TH1D("hmuMETpt","hmuMETpt",200,0,200);
  TH1D *hmuMETeta=new TH1D("hmuMETeta","hmuMETeta",100,-2.5,2.5);
  TH1D *hmuMETphi=new TH1D("hmuMETphi","hmuMETphi",100,-TMath::Pi(),TMath::Pi());
  TH1D *hjetpt=new TH1D("hjetpt","hjetpt",100,0,50);
  TH1D *hjeteta=new TH1D("hjeteta","hjeteta",100,-2.5,2.5);
  TH1D *hjetphi=new TH1D("hjetphi","hjetphi",100,-TMath::Pi(),TMath::Pi());

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

    // TLorentzVector mu1,mu2, vZ;
    // mu1.SetPtEtaPhiM(mu1_pt,mu1_eta,mu1_phi,mu1_mass);
    // mu2.SetPtEtaPhiM(METmu2_pt,METmu2_eta,METmu2_phi,METmu2_mass);
    // vZ = mu1+mu2;
    
    for(int i=0; i<etaMuonNSteps; i++){
      // for(int j=0; j<2*WMassNSteps+1; j++){
      for(int j=60; j<61; j++){
      
        double iWmass = (WMassCentral-(WMassNSteps-j)*WMassStep)/1e3;
        // SELECT EVENT WITH Z/W SCALED CUTS
        // ORIGINAL CUTS:
        // muon pt > 30,
        // pfmetcut > 25,
        // Boson pt (recoilcut) < 20,
        
        double MuPosGen_pt_jacobian = 2*MuPosGen_pt/ZMass;
        
        if(ZGen_mass>0){
          hWlikePos_PtScaled_1_Gen[i][j]->Fill(MuPosGen_pt_jacobian);
        
          if(ZGen_mass>70 && ZGen_mass<110){
            hWlikePos_PtScaled_2_ZGenMassCut[i][j]->Fill(MuPosGen_pt_jacobian);
            
            if(TMath::Abs(MuPosGen_eta)<etaMaxMuons[i]){
              hWlikePos_PtScaled_3_Mu1RecoCut[i][j]->Fill(MuPosGen_pt_jacobian);
              
              if(TMath::Abs(MuNegGen_eta)<2.4){
                hWlikePos_PtScaled_4_Mu2GenCut[i][j]->Fill(MuPosGen_pt_jacobian);
              }
            }
          }
                
          double MuPos_pt_jacobian = 2*MuPos_pt/ZMass;
          if( evtHasGoodVtx && evtHasTrg
              // && Z_mass>70 && Z_mass<120
              && Z_mass>50
              && TMath::Abs(MuPos_eta)<etaMaxMuons[i] && MuPos_pt>30*ZMass/iWmass && MuPosIsTight && MuPosTrg
              && TMath::Abs(MuNeg_eta)<2.4 && MuNeg_pt>30*ZMass/iWmass
              // && TMath::Abs(MuNeg_eta)<2.4
            ){
            hWlikePos_PtScaled_5_RecoCut[i][j]->Fill(MuPos_pt_jacobian);

            if(pfmetWlikePos>25*ZMass/iWmass){
              hWlikePos_PtScaled_6_METCut[i][j]->Fill(MuPos_pt_jacobian);
              
              if(WlikePos_pt<20*ZMass/iWmass){
                hWlikePos_PtScaled_7_RecoilCut[i][j]->Fill(MuPos_pt_jacobian);
                
                if(Jet_leading_pt<30){
                  hWlikePos_PtScaled_8_JetCut[i][j]->Fill(MuPos_pt_jacobian);
                }
              }
            }
          }
        }
        // if(TMath::Abs(mu1_eta)>etaMaxMuons[i]) continue;
        
        // if( !( mu1_pt>30*ZMass/iWmass && pfmet > 25*ZMass/iWmass && W_pt<20*ZMass/iWmass) )
          // continue; // ONLY SELECT EVENTS WITH Z/W SCALED CUTS
            
        // // cout << "j " << j << " iWmass= " << iWmass << endl;
        // // cout << "mu1_pt/80.385 = " << mu1_pt<<"/80.385 = " << (mu1_pt/80.385) << endl;
        // hWlike_PtScaled[i][j]->Fill(2*mu1_pt/ZMass);
        // // hWlike_PtScaled[j]->Print();
      }
    }
    // cout << endl;
    
    // if(vZ.M()<50) continue;
    
    // Zmass->Fill(vZ.M());
    // ZlepDeltaEta->Fill(mu1_eta-METmu2_eta);
    // ZlepEta1VsEta2->Fill(mu1_eta,METmu2_eta);
    // ZlepDeltaEtaVsEta1->Fill(mu1_eta,mu1_eta-METmu2_eta);
    // hpfMET->Fill(pfmet);
    // hpfMETphi->Fill(pfmet_phi);
    // hZproperpt->Fill(vZ.Pt());
    // hZproperphi->Fill(vZ.Phi());
    // hZpropermt->Fill(vZ.Mt());
    // hWlikept->Fill(W_pt);
    // hWlikephi->Fill(W_phi);
    // hWlikemt->Fill(W_mt);
    // hmuWlikept->Fill(mu1_pt);
    // hmuWlikeeta->Fill(mu1_eta);
    // hmuWlikephi->Fill(mu1_phi);
    // hmuMETpt->Fill(METmu2_pt);
    // hmuMETeta->Fill(METmu2_eta);
    // hmuMETphi->Fill(METmu2_phi);
    // hjetpt->Fill(jet_pt);
    // hjeteta->Fill(jet_eta);
    // hjetphi->Fill(jet_phi);

    
  }
  
  fout->cd();
  Zmass->Write();
  ZlepDeltaEta->Write();
  ZlepEta1VsEta2->Write();
  ZlepDeltaEtaVsEta1->Write();
  hpfMET->Write();
  hpfMETphi->Write();
  hZproperpt->Write();
  hZproperphi->Write();
  hZpropermt->Write();
  hWlikept->Write();
  hWlikephi->Write();
  hWlikemt->Write();
  hmuWlikept->Write();
  hmuWlikeeta->Write();
  hmuWlikephi->Write();
  hmuMETpt->Write();
  hmuMETeta->Write();
  hmuMETphi->Write();
  hjetpt->Write();
  hjeteta->Write();
  hjetphi->Write();
  for(int i=0; i<etaMuonNSteps; i++){
    // for(int j=0; j<2*WMassNSteps+1; j++){
    for(int j=60; j<61; j++){
      hWlikePos_PtScaled_1_Gen[i][j]->Write();
      hWlikePos_PtScaled_2_ZGenMassCut[i][j]->Write();
      hWlikePos_PtScaled_3_Mu1RecoCut[i][j]->Write();
      hWlikePos_PtScaled_4_Mu2GenCut[i][j]->Write();
      hWlikePos_PtScaled_5_RecoCut[i][j]->Write();
      hWlikePos_PtScaled_6_METCut[i][j]->Write();
      hWlikePos_PtScaled_7_RecoilCut[i][j]->Write();
      hWlikePos_PtScaled_8_JetCut[i][j]->Write();
    }
  }
  
  fout->Write();
  fout->Close();

}
