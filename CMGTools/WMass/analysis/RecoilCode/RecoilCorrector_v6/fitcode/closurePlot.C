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

//TFile fpow_corr("TREE/output_mad0_POWasMAD_onlyU20_onlyU10_iter1_feb12.root");
TFile fpow_corr("TREE/output_mad0_POWasMAD_onlyU20_onlyU10_iter1_feb15.root");
TFile fmad_corr("TREE/output_mad1_POWasMAD_onlyU20_onlyU10_iter1_feb12.root");

TFile fpow_scale("recoilfits/recoilfit_JAN25_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_powheg.root");
TFile fmad_scale("recoilfits/recoilfit_JAN25_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root");

void calculateU1U2(double &iPar, bool iType, double fMet, double fMPhi, double fPt1, double fPhi1) {

  double fZPhi = fPhi1;

  //  cout << " inside calculateU1U2: fMet = " << fMet << " fMPhi = " << fMPhi << " fPt1 " << fPt1 << endl;
  // reset U1 U2                                                                                                                                                                                        
  double fU1 = -9999;
  double fU2 = -9999;

  double lUX  = fMet*cos(fMPhi) + fPt1*cos(fPhi1);
  double lUY  = fMet*sin(fMPhi) + fPt1*sin(fPhi1);
  double lU   = sqrt(lUX*lUX+lUY*lUY);

  // rotate of -180 the X and Y component
  double lCos = - (lUX*cos(fZPhi) + lUY*sin(fZPhi))/lU;
  double lSin =   (lUX*sin(fZPhi) - lUY*cos(fZPhi))/lU;
  fU1 = lU*lCos;
  fU2 = lU*lSin;
  if(iType)  iPar = fU1;
  if(!iType) iPar = fU2;

}

void DrawUcorr() {

  //  TFile *fileC_ = TFile::Open(fpow_corr);
  //  TFile *fileO_ = TFile::Open(fpow_or);

  //  cout << " filename" << fileName.Data() << "histoname" << histoName.Data() << endl;

  TTree *MyTreeC = (TTree*)  fpow_corr.Get("ZTreeProducer");

  //  MyTreeC->Print();
  MyTreeC->Draw("tkmet_phi-tkmet_phi_ori","Z_pt<10 && Z_pt>5");

}

void closurePlot(bool doOriginal, bool doMad) {

  TH1F *htkmet = new TH1F("tkmet","tkmet", 500,0,500);
  TH1F *hmt = new TH1F("tkmt","hmt", 150,0,150);
  TH1F *hZ = new TH1F("Z","Z", 200,0,200);

  TH1F *hdeltaU1 = new TH1F("hdeltaU1","hdeltaU1", 200,-10.,10.);
  TH1F *hdeltaU2 = new TH1F("hdeltaU2","hdeltaU2", 200,-10.,10.);

  TH1F *hdeltaU1pullCorr = new TH1F("hdeltaU1pullCorr","hdeltaU1pullCorr", 200,-20.,20.);
  TH1F *hdeltaU2pullCorr = new TH1F("hdeltaU2pullCorr","hdeltaU2pullCorr", 200,-20.,20.);

  TH2F *hdeltaU2pull2D = new TH2F("hdeltaU2pull2D","hdeltaU2pull2D", 200,-20.,20.,200,-20.,20.);
  TH2F *hdeltaU1pull2D = new TH2F("hdeltaU1pull2D","hdeltaU1pull2D", 200,-20.,20.,200,-20.,20.);

  //  TH1F *hdeltaU1 = new TH1F("hdeltaU1","hdeltaU1", 200,-10.,10.);
  TH1F *hdeltaU1_05 = new TH1F("hdeltaU1_05","hdeltaU1_05", 200,-10.,10.);
  TH1F *hdeltaU1_510 = new TH1F("hdeltaU1_510","hdeltaU1_510", 200,-10.,10.);
  TH1F *hdeltaU1_1015 = new TH1F("hdeltaU1_1015","hdeltaU1_1015", 200,-10.,10.);
  TH1F *hdeltaU1_1520 = new TH1F("hdeltaU1_1520","hdeltaU1_1520", 200,-10.,10.);

  static Double_t tkmet,tkmet_phi,evt;
  static Double_t tkmet_ori,tkmet_phi_ori;
  static Double_t Z_pt,Z_phi;
  static Double_t MuPos_pt,MuPos_eta,MuPos_phi,MuPos_mass;
  static Double_t MuNeg_pt,MuNeg_eta,MuNeg_phi,MuNeg_mass;
  //  TFile *fmad = new TFile("/scratch3/munir/mini/run09666tree2.root");

  TTree *trktree = 0 ;
  if(doOriginal) trktree = (TTree *) fmad_or.Get("ZTreeProducer");
  if(!doOriginal && !doMad) trktree = (TTree *) fpow_corr.Get("ZTreeProducer");
  if(!doOriginal && doMad) trktree = (TTree *) fmad_corr.Get("ZTreeProducer");

  //  fmad_scale.ls();

  // readScale
  TF1 * scaleMad = (TF1*) fmad_scale.Get("PFu1Mean_1");
  TF1 * scalePow = (TF1*) fpow_scale.Get("PFu1Mean_1");

  TF1 * meanRMSMad = (TF1*) fmad_scale.Get("PFu1MeanRMS_1");
  TF1 * meanRMSPow = (TF1*) fpow_scale.Get("PFu1MeanRMS_1");

  TF1 * meanRMSu2Mad = (TF1*) fmad_scale.Get("PFu2MeanRMS_1");
  TF1 * meanRMSu2Pow = (TF1*) fpow_scale.Get("PFu2MeanRMS_1");

  if(doOriginal) hmt->SetName("hMtMad");
  if(!doOriginal) hmt->SetName("hMtPOW");

  trktree->SetBranchAddress("evt", &evt);
  trktree->SetBranchAddress("tkmet", &tkmet);
  trktree->SetBranchAddress("tkmet_phi", &tkmet_phi);

  trktree->SetBranchAddress("tkmet_ori", &tkmet_ori);
  trktree->SetBranchAddress("tkmet_phi_ori", &tkmet_phi_ori);

  trktree->SetBranchAddress("Z_pt", &Z_pt);
  trktree->SetBranchAddress("Z_phi", &Z_phi);

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

    double u1=0;
    double u2=0;
    double u1_ori=0;
    double u2_ori=0;
    
    calculateU1U2(u1, true, tkmet, tkmet_phi, Z_pt, Z_phi);
    calculateU1U2(u2, false, tkmet, tkmet_phi, Z_pt, Z_phi);

    calculateU1U2(u1_ori, true, tkmet_ori, tkmet_phi_ori, Z_pt, Z_phi);
    calculateU1U2(u2_ori, false, tkmet_ori, tkmet_phi_ori, Z_pt, Z_phi);

    //    cout << "scaleMad->Eval(Z_pt)" << scaleMad->Eval(Z_pt)<< endl;
    //    hdeltaU1->Fill((u1-scaleMad->Eval(Z_pt))-(u1_ori-scalePow->Eval(Z_pt)));
    hdeltaU1->Fill(u1-u1_ori);
    hdeltaU2->Fill(u2-u2_ori);

    hdeltaU1pullCorr->Fill((u1-scaleMad->Eval(Z_pt))/meanRMSMad->Eval(Z_pt));
    hdeltaU2pullCorr->Fill((u2-scaleMad->Eval(Z_pt))/meanRMSMad->Eval(Z_pt));

    //    hdeltaU2pull2D->Fill((u2)/meanRMSu2Mad->Eval(Z_pt),(u2_ori)/meanRMSu2Pow->Eval(Z_pt));
    //    hdeltaU1pull2D->Fill((u1-scaleMad->Eval(Z_pt))/meanRMSMad->Eval(Z_pt),(u1_ori-scalePow->Eval(Z_pt))/meanRMSPow->Eval(Z_pt));

    hdeltaU2pull2D->Fill(u2,u2_ori);
    hdeltaU1pull2D->Fill(u1-scaleMad->Eval(Z_pt),u1_ori-scalePow->Eval(Z_pt));

    if(Z_pt>0 && Z_pt<5) hdeltaU1_05->Fill(u1-u1_ori);
    if(Z_pt>5 && Z_pt<10) hdeltaU1_510->Fill(u1-u1_ori);
    if(Z_pt>10 && Z_pt<15) hdeltaU1_1015->Fill(u1-u1_ori);


  }

  TFile f3("mt.root","UPDATE");
  hmt->Write();
  hdeltaU1->Write();
  hdeltaU2->Write();

  hdeltaU1pullCorr->Write();
  hdeltaU2pullCorr->Write();

  hdeltaU2pull2D->Write();
  hdeltaU1pull2D->Write();

  hdeltaU1_05->Write();
  hdeltaU1_510->Write();
  hdeltaU1_1015->Write();

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
