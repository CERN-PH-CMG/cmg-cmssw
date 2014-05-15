#define Templates_from_ZanalysisOnDATA_cxx
#include "Templates_from_ZanalysisOnDATA.h"
#include "../includes/common.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLorentzVector.h>

void Templates_from_ZanalysisOnDATA::Loop(int IS_MC_CLOSURE_TEST)
{
//   In a ROOT session, you can do:
//      Root > .L Templates_from_ZanalysisOnDATA.C
//      Root > Templates_from_ZanalysisOnDATA t
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

  TFile*fin = new TFile(Form("%s/R_WdivZ_OnMC.root",outputdir.Data()));
  TFile*fout = new TFile("Templates_from_ZanalysisOnDATA.root","RECREATE");

  TH1D *hWlikePos_PtScaled_RWeighted_Templates[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D*hWlikePos_R_WdivZ[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];

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
      hWlikePos_PtScaled_RWeighted_Templates[i][j]=new TH1D(Form("hWlikePos_PtScaled_RWeighted_Templates_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_RWeighted_Templates_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_RWeighted_Templates[i][j]->Sumw2();
      cout << Form("hR_WdivZ_WlikePos_8_JetCut_eta%s_%d",eta_str.Data(),jWmass) << endl;
      hWlikePos_R_WdivZ[i][j]=(TH1D*)fin->Get(Form("hR_WdivZ_WlikePos_8_JetCut_eta%s_%d",eta_str.Data(),jWmass));
      hWlikePos_R_WdivZ[i][j]->Print();
    }
  }

   if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t first_entry = 0;
  if(IS_MC_CLOSURE_TEST==1) first_entry=nentries/2;

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=first_entry; jentry<nentries;jentry++) {
  // for (Long64_t jentry=0; jentry<5e5;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%250000==0) cout <<"Analyzed entry "<<jentry<<"/"<<nentries<<endl;

    for(int i=0; i<WMass::etaMuonNSteps; i++){
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      
        double iWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV)/1e3;
        // SELECT EVENT WITH Z/W SCALED CUTS
        // ORIGINAL CUTS:
        // muon pt > 30,
        // pfmetcut > 25,
        // Boson pt (recoilcut) < 20,
                      
        if( evtHasGoodVtx && evtHasTrg){
            double MuPos_pt_jacobian = 2*MuPos_pt/WMass::ZMass;
          if( Z_mass>50
              && TMath::Abs(MuPos_eta)<WMass::etaMaxMuons[i] && MuPos_pt>30*WMass::ZMass/iWmass && MuPosIsTightAndIso && MuPosTrg
              && TMath::Abs(MuNeg_eta)<2.4
              // && TMath::Abs(MuNeg_eta)<2.4 && MuNeg_pt>30*WMass::ZMass/iWmass // 2nd muon pt cut added
          ){

            if(pfmetWlikePos>25*WMass::ZMass/iWmass){
              
              if(WlikePos_pt<20*WMass::ZMass/iWmass){
                
                if(Jet_leading_pt<30){
                  double weight=hWlikePos_R_WdivZ[i][j]->GetBinContent(hWlikePos_R_WdivZ[i][j]->GetXaxis()->FindBin(MuPos_pt_jacobian));
                  // cout << "etamax= " << WMass::etaMaxMuons[i] << " mass= " << iWmass << " MuPos_pt_jacobian= " << MuPos_pt_jacobian << " weight= " << weight << endl;
                  hWlikePos_PtScaled_RWeighted_Templates[i][j]->Fill(MuPos_pt_jacobian,weight*lumi_scaling);
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
  for(int i=0; i<WMass::etaMuonNSteps; i++){
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      hWlikePos_PtScaled_RWeighted_Templates[i][j]->Write();
    }
  }
  
  fout->Write();
  fout->Close();

}
