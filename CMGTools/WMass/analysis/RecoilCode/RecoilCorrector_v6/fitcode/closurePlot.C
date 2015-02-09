#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TLorentzVector.h"

float deltaPhi( float phi1 , float phi2 ) {
  float dphi = fabs( phi1 - phi2 );
  if( dphi > TMath::Pi() ) dphi = TMath::TwoPi() - dphi;
  return dphi;
}

float getMT( float pt1 , float phi1 , float pt2 , float phi2 ){

  float dphi = deltaPhi(phi1, phi2);
  return sqrt( 2 * ( pt1 * pt2 * (1 - cos( dphi ) ) ) );

}

float getPseudoMT(double pfmet_corr, double pfmet_phi_corr, double MuNeg_pt, double MuNeg_phi , double MuPos_pt, double MuPos_phi)
{

  float metx = pfmet_corr * cos( pfmet_phi_corr );
  float mety = pfmet_corr * sin( pfmet_phi_corr );

  float ptx = MuNeg_pt * cos( MuNeg_phi );
  float pty = MuNeg_pt * sin( MuNeg_phi );

  //recalculate the MET with the negative lepton
  metx += ptx;
  mety += pty;

  float metphicorr_lep    = sqrt(metx*metx + mety*mety);
  float metphicorrphi_lep = atan2( mety , metx );

  //recalculate the MT with the negative lepton                                                                                                                            
  float pseudoMT =  getMT(MuPos_pt, MuPos_phi, metphicorr_lep, metphicorrphi_lep);
  return pseudoMT;

}

TFile fmad_or("TREE/output_skimmedTree_mad1_Y1_iter0.root");
TFile fpow_or("TREE/output_skimmedTree_mad0_Y1_iter0.root");
TFile fpow_corr("TREE/output_mad0_POWasMAD_onlyU21_iter1.root");

void closurePlot(bool doOriginal) {

  TH1F *htkmet = new TH1F("tkmet","tkmet", 500,0,500);
  TH1F *hmt = new TH1F("tkmt","hmt", 150,0,150);
  TH1F *hZ = new TH1F("Z","Z", 200,0,200);

  static Double_t tkmet,tkmet_phi,evt;
  static Double_t MuPos_pt,MuPos_eta,MuPos_phi,MuPos_mass;
  static Double_t MuNeg_pt,MuNeg_eta,MuNeg_phi,MuNeg_mass;
  //  TFile *fmad = new TFile("/scratch3/munir/mini/run09666tree2.root");

  TTree *trktree = 0 ;
  if(doOriginal) trktree = (TTree *) fmad_or.Get("ZTreeProducer");
  if(!doOriginal) trktree = (TTree *) fpow_corr.Get("ZTreeProducer");

  if(doOriginal) hmt->SetName("hMtMad");
  if(!doOriginal) hmt->SetName("hMtPOW");

  trktree->SetBranchAddress("evt", &evt);
  trktree->SetBranchAddress("tkmet", &tkmet);
  trktree->SetBranchAddress("tkmet_phi", &tkmet_phi);

  trktree->SetBranchAddress("MuPos_pt", &MuPos_pt);
  trktree->SetBranchAddress("MuPos_eta", &MuPos_eta);
  trktree->SetBranchAddress("MuPos_phi", &MuPos_phi);
  trktree->SetBranchAddress("MuPos_mass", &MuPos_mass);

  trktree->SetBranchAddress("MuNeg_pt", &MuNeg_pt);
  trktree->SetBranchAddress("MuNeg_eta", &MuNeg_eta);
  trktree->SetBranchAddress("MuNeg_phi", &MuNeg_phi);
  trktree->SetBranchAddress("MuNeg_mass", &MuNeg_mass);


  Int_t nrow = trktree->GetEntries();
  for (Int_t i =0; i < nrow; i++) {
    trktree->GetEntry(i);
    //    cout << "i=" << i << " tkmet=" << tkmet << endl;
    TLorentzVector muNeg,muPos;
    muNeg.SetPtEtaPhiM(MuNeg_pt,MuNeg_eta,MuNeg_phi,MuNeg_mass);
    muPos.SetPtEtaPhiM(MuPos_pt,MuPos_eta,MuPos_phi,MuPos_mass);
    TLorentzVector WlikePos_met,WlikePos_nu,WlikePos, Z;
    Z = muPos + muNeg;

    //    WlikePos_met.SetPtEtaPhiM(tkmet,0,tkmet_phi,0);
    //    WlikePos_nu = muNeg + WlikePos_met;
    //    WlikePos_nu.SetPtEtaPhiM(tkmet,0,tkmet_phi,0);
    //    WlikePos = muPos + WlikePos_nu;

    float mt=getPseudoMT(tkmet, tkmet_phi, MuNeg_pt, MuNeg_phi , MuPos_pt, MuPos_phi);

    hZ->Fill(Z.M());
    hmt->Fill(mt);
    htkmet->Fill(tkmet);
  }

  TFile f3("mt.root","UPDATE");
  hmt->Write();

  f3.Write();
  f3.Close();

  // hmt->Draw();

}

void makeMoneyPlot() {


  TFile *file_ = TFile::Open("mt.root");

  //  cout << " filename" << fileName.Data() << "histoname" << histoName.Data() << endl;

  TH1 * hPOW_ = (TH1F*) file_->Get("hMtPOW");
  TH1 * htarget_ = (TH1F*) file_->Get("hMtMad");

  hPOW_->Scale(1./hPOW_->Integral(hPOW_->FindBin(70),hPOW_->FindBin(110)));
  htarget_->Scale(1./htarget_->Integral(htarget_->FindBin(70),htarget_->FindBin(110)));

  hPOW_->Divide(htarget_);
  hPOW_->GetXaxis()->SetRangeUser(70,110);
  hPOW_->Draw();

}
