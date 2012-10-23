#define Zanalysis_cxx
#include "Zanalysis.h"
#include "../includes/common.h"
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
  // double ZMass = 91.1876;
  // int WMassCentral_MeV = 80385;
  // int WMassStep_MeV = 15;
  // static const int WMassNSteps = 60;
  // static const int etaMuonNSteps = 5;
  // double etaMaxMuons[etaMuonNSteps] = {0.6, 0.8, 1.2, 1.6, 2.1};
  // std::vector<TH1D*> hWlike_PtScaled;
  TH1D *hWlikePos_PtScaled_1_Gen[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_2_ZGenMassCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_3_Mu1GenCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_4_Mu2GenCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_5_RecoCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_6_METCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_7_RecoilCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_8_JetCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];

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
      int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;
      // hWlikePos_PtScaled_1_Gen[i][j]=new TH1D(Form("hWlikePos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      // hWlikePos_PtScaled_1_Gen[i][j]->Sumw2();
      // hWlikePos_PtScaled_2_ZGenMassCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      // hWlikePos_PtScaled_2_ZGenMassCut[i][j]->Sumw2();
      // hWlikePos_PtScaled_3_Mu1GenCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_3_Mu1GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_3_Mu1GenCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      // hWlikePos_PtScaled_3_Mu1GenCut[i][j]->Sumw2();
      // hWlikePos_PtScaled_4_Mu2GenCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      // hWlikePos_PtScaled_4_Mu2GenCut[i][j]->Sumw2();
      // hWlikePos_PtScaled_5_RecoCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      // hWlikePos_PtScaled_5_RecoCut[i][j]->Sumw2();
      // hWlikePos_PtScaled_6_METCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      // hWlikePos_PtScaled_6_METCut[i][j]->Sumw2();
      // hWlikePos_PtScaled_7_RecoilCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      // hWlikePos_PtScaled_7_RecoilCut[i][j]->Sumw2();
      // hWlikePos_PtScaled_8_JetCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),120.,0.6,1.8);
      // hWlikePos_PtScaled_8_JetCut[i][j]->Sumw2();
      hWlikePos_PtScaled_1_Gen[i][j]=new TH1D(Form("hWlikePos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_1_Gen[i][j]->Sumw2();
      hWlikePos_PtScaled_2_ZGenMassCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_2_ZGenMassCut[i][j]->Sumw2();
      hWlikePos_PtScaled_3_Mu1GenCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_3_Mu1GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_3_Mu1GenCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_3_Mu1GenCut[i][j]->Sumw2();
      hWlikePos_PtScaled_4_Mu2GenCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_4_Mu2GenCut[i][j]->Sumw2();
      hWlikePos_PtScaled_5_RecoCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_5_RecoCut[i][j]->Sumw2();
      hWlikePos_PtScaled_6_METCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_6_METCut[i][j]->Sumw2();
      hWlikePos_PtScaled_7_RecoilCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_7_RecoilCut[i][j]->Sumw2();
      hWlikePos_PtScaled_8_JetCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_8_JetCut[i][j]->Sumw2();
    }
  }

  TH1D *Zmass=new TH1D("Zmass","Zmass",200,0,200);
  
  TH1D *ZlepDeltaEta=new TH1D("ZlepDeltaEta","ZlepDeltaEta",200,-5,5);
  TH2D *ZlepEta1VsEta2=new TH2D("ZlepEta1VsEta2","ZlepEta1VsEta2",50,-2.5,2.5,50,-2.5,2.5);
  TH2D *ZlepDeltaEtaVsEta1=new TH2D("ZlepDeltaEtaVsEta1","ZlepDeltaEtaVsEta1",50,-2.5,2.5,100,-5,5);

  TH1D *hpfMET_Z=new TH1D("hpfMET_Z","hpfMET_Z",200,0,200);
  TH1D *hpfMETphi_Z=new TH1D("hpfMETphi_Z","hpfMETphi_Z",100,-TMath::Pi(),TMath::Pi());
  TH1D *hZ_pt=new TH1D("hZ_pt","hZ_pt",100,0,25);
  TH1D *hZ_phi=new TH1D("hZ_phi","hZ_phi",100,-TMath::Pi(),TMath::Pi());
  TH1D *hZ_mt=new TH1D("hZ_mt","hZ_mt",150,50,200);
  TH1D *hWlikePos_pt=new TH1D("hWlikePos_pt","hWlikePos_pt",100,0,25);
  TH1D *hWlikePos_phi=new TH1D("hWlikePos_phi","hWlikePos_phi",100,-TMath::Pi(),TMath::Pi());
  TH1D *hWlikePos_mt=new TH1D("hWlikePos_mt","hWlikePos_mt",150,50,200);
  TH1D *hMupt_WlikePos=new TH1D("hMupt_WlikePos","hMupt_WlikePos",200,0,200);
  TH1D *hMueta_WlikePos=new TH1D("hMueta_WlikePos","hMueta_WlikePos",100,-2.5,2.5);
  TH1D *hMuphi_WlikePos=new TH1D("hMuphi_WlikePos","hMuphi_WlikePos",100,-TMath::Pi(),TMath::Pi());
  TH1D *hMuMETpt_WlikePos=new TH1D("hMuMETpt_WlikePos","hMuMETpt_WlikePos",200,0,200);
  TH1D *hMuMETeta_WlikePos=new TH1D("hMuMETeta_WlikePos","hMuMETeta_WlikePos",100,-2.5,2.5);
  TH1D *hMuMETphi_WlikePos=new TH1D("hMuMETphi_WlikePos","hMuMETphi_WlikePos",100,-TMath::Pi(),TMath::Pi());
  TH1D *hJetpt=new TH1D("hJetpt","hJetpt",100,0,50);
  TH1D *hJeteta=new TH1D("hJeteta","hJeteta",100,-2.5,2.5);
  TH1D *hJetphi=new TH1D("hJetphi","hJetphi",100,-TMath::Pi(),TMath::Pi());

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

    // TLorentzVector mu1,mu2, vZ;
    // mu1.SetPtEtaPhiM(mu1_pt,mu1_eta,mu1_phi,mu1_mass);
    // mu2.SetPtEtaPhiM(METmu2_pt,METmu2_eta,METmu2_phi,METmu2_mass);
    // vZ = mu1+mu2;
    
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      
        double iWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV)/1e3;
        // SELECT EVENT WITH Z/W SCALED CUTS
        // ORIGINAL CUTS:
        // muon pt > 30,
        // pfmetcut > 25,
        // Boson pt (recoilcut) < 20,
        
        
        if(ZGen_mass>0){
          double MuPosGen_pt_jacobian = 2*MuPosGen_pt/WMass::ZMass;
          hWlikePos_PtScaled_1_Gen[i][j]->Fill(MuPosGen_pt_jacobian);
        
          if(ZGen_mass>50){
            hWlikePos_PtScaled_2_ZGenMassCut[i][j]->Fill(MuPosGen_pt_jacobian);
            
            if(TMath::Abs(MuPosGen_eta)<WMass::etaMaxMuons[i]){
              hWlikePos_PtScaled_3_Mu1GenCut[i][j]->Fill(MuPosGen_pt_jacobian);
              
              if(TMath::Abs(MuNegGen_eta)<2.4){
                hWlikePos_PtScaled_4_Mu2GenCut[i][j]->Fill(MuPosGen_pt_jacobian);
              }
            }
          }
                
          if( evtHasGoodVtx && evtHasTrg){
              double MuPos_pt_jacobian = 2*MuPos_pt/WMass::ZMass;
            if( Z_mass>50
                && TMath::Abs(MuPos_eta)<WMass::etaMaxMuons[i] && MuPos_pt>30*WMass::ZMass/iWmass && MuPosIsTightAndIso && MuPosTrg
                && TMath::Abs(MuNeg_eta)<2.4
                // && TMath::Abs(MuNeg_eta)<2.4 && MuNeg_pt>30*WMass::ZMass/iWmass // 2nd muon pt cut added
            ){
              hWlikePos_PtScaled_5_RecoCut[i][j]->Fill(MuPos_pt_jacobian);

              if(pfmetWlikePos>25*WMass::ZMass/iWmass){
                hWlikePos_PtScaled_6_METCut[i][j]->Fill(MuPos_pt_jacobian);
                
                if(WlikePos_pt<20*WMass::ZMass/iWmass){
                  hWlikePos_PtScaled_7_RecoilCut[i][j]->Fill(MuPos_pt_jacobian);
                  
                  if(Jet_leading_pt<30){
                    hWlikePos_PtScaled_8_JetCut[i][j]->Fill(MuPos_pt_jacobian);
                  }
                }
              }
            }
          }
        }
        // if(TMath::Abs(mu1_eta)>WMass::etaMaxMuons[i]) continue;
        
        // if( !( mu1_pt>30*WMass::ZMass/iWmass && pfmet > 25*WMass::ZMass/iWmass && W_pt<20*WMass::ZMass/iWmass) )
          // continue; // ONLY SELECT EVENTS WITH Z/W SCALED CUTS
            
        // // cout << "j " << j << " iWmass= " << iWmass << endl;
        // // cout << "mu1_pt/80.385 = " << mu1_pt<<"/80.385 = " << (mu1_pt/80.385) << endl;
        // hWlike_PtScaled[i][j]->Fill(2*mu1_pt/WMass::ZMass);
        // // hWlike_PtScaled[j]->Print();
      }
    }
    // cout << endl;
    
    // if(vZ.M()<50) continue;
    
    if(evtHasGoodVtx && evtHasTrg && Z_mass>50
        && TMath::Abs(MuPos_eta)<2.1 && MuPos_pt>30*WMass::ZMass/WMass::WMassCentral_MeV 
        && MuPosIsTightAndIso && MuPosTrg
        && TMath::Abs(MuNeg_eta)<2.4
      ){
      Zmass->Fill(Z_mass);
      ZlepDeltaEta->Fill(MuPos_eta-MuNeg_eta);
      ZlepEta1VsEta2->Fill(MuPos_eta,MuNeg_eta);
      ZlepDeltaEtaVsEta1->Fill(MuPos_eta,MuPos_eta-MuNeg_eta);
      hpfMET_Z->Fill(pfmet);
      hpfMETphi_Z->Fill(pfmet_phi);
      hZ_pt->Fill(Z_pt);
      hZ_phi->Fill(Z_phi);
      hZ_mt->Fill(Z_mt);
      hWlikePos_pt->Fill(WlikePos_pt);
      hWlikePos_phi->Fill(WlikePos_phi);
      hWlikePos_mt->Fill(WlikePos_mt);
      hMupt_WlikePos->Fill(MuPos_pt);
      hMueta_WlikePos->Fill(MuPos_eta);
      hMuphi_WlikePos->Fill(MuPos_phi);
      hMuMETpt_WlikePos->Fill(MuNeg_pt);
      hMuMETeta_WlikePos->Fill(MuNeg_eta);
      hMuMETphi_WlikePos->Fill(MuNeg_phi);
      hJetpt->Fill(Jet_leading_pt);
      hJeteta->Fill(Jet_leading_eta);
      hJetphi->Fill(Jet_leading_phi);
    }
    
  }
  
  fout->cd();
  Zmass->Write();
  ZlepDeltaEta->Write();
  ZlepEta1VsEta2->Write();
  ZlepDeltaEtaVsEta1->Write();
  hpfMET_Z->Write();
  hpfMETphi_Z->Write();
  hZ_pt->Write();
  hZ_phi->Write();
  hZ_mt->Write();
  hWlikePos_pt->Write();
  hWlikePos_phi->Write();
  hWlikePos_mt->Write();
  hMupt_WlikePos->Write();
  hMueta_WlikePos->Write();
  hMuphi_WlikePos->Write();
  hMuMETpt_WlikePos->Write();
  hMuMETeta_WlikePos->Write();
  hMuMETphi_WlikePos->Write();
  hJetpt->Write();
  hJeteta->Write();
  hJetphi->Write();
  for(int i=0; i<WMass::etaMuonNSteps; i++){
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      hWlikePos_PtScaled_1_Gen[i][j]->Write();
      hWlikePos_PtScaled_2_ZGenMassCut[i][j]->Write();
      hWlikePos_PtScaled_3_Mu1GenCut[i][j]->Write();
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
