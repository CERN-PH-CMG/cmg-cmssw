#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TLorentzVector.h"

#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooArgList.h"
#include "RooProdPdf.h"
#include "RooVoigtian.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooAddition.h"
#include "RooCustomizer.h"
#include "RooWorkspace.h"

#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"
#include "TVectorD.h"
#include "TMatrixD.h"

using namespace RooFit;

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

//$$$$$$$$$$$$$$$$$$$$$$$$
//$$   begin code from runRecoilFit3G.C
//$$$$$$$$$$$$$$$$$$$$$$$$

class PdfDiagonalizer {
public:
  PdfDiagonalizer(const char *name, RooWorkspace *w, RooFitResult &result);

  RooAbsPdf *diagonalize(RooAbsPdf &pdf) ;
  const RooArgList & originalParams() { return parameters_; }
  const RooArgList & diagonalParams() { return eigenVars_; }
private:
  std::string name_;
  RooArgList  parameters_;
  RooArgList  eigenVars_;
  RooArgList  replacements_;
};

//std::string name_;
//RooArgList  * parameters_; // these are the original paramters
//RooArgList  * eigenVars_; // these are the diagonal parameters

//RooArgList  replacements_;

//void PdfDiagonalizer(const char *name, RooWorkspace *w, RooFitResult &result) {
PdfDiagonalizer::PdfDiagonalizer(const char *name, RooWorkspace *w, RooFitResult &result) :
  name_(name),
  parameters_(result.floatParsFinal())
{

  //  name_.append(name);/// MARIA hard coded
  //  parameters_= new RooArgList(result.floatParsFinal());
  //  int n = parameters_->getSize();

  int n = parameters_.getSize();
  //  int n = result.floatParsFinal().getSize();
  //  cout << " parameters_.getSize() " << n  << " " << result.floatParsFinal() << endl;

  TMatrixDSym cov(result.covarianceMatrix());
  TMatrixDSymEigen eigen(cov);

  const TMatrixD& vectors = eigen.GetEigenVectors();
  const TVectorD& values  = eigen.GetEigenValues();

  //  cout << " --> print the eigenvalues " << endl;
  //  values.Print();

  char buff[10240];

  RooArgList  eigenVars;
  //
  // create unit gaussians per eigen-vector
  for (int i = 0; i < n; ++i) {
    //    snprintf(buff,sizeof(buff),"%s_eig%s[-5,5]", name, parameters_->at(i)->GetName());
    snprintf(buff,sizeof(buff),"%s_eig%d[-5,5]", name, i);
    // w->factory(buff);
    eigenVars_.add(*(w->factory(buff)));
  }

  //  cout << "got here " << endl;
  //  eigenVars_= new RooArgList(eigenVars);

  // put them in a list, with a one at the end to set the mean
  //  RooArgList eigvVarsPlusOne(*eigenVars_);
  RooArgList eigvVarsPlusOne(eigenVars_);
  if (w->var("_one_") == 0) w->factory("_one_[1]");
  eigvVarsPlusOne.add(*w->var("_one_"));

  // then go create the linear combinations
  // each is equal to the transpose matrx times the square root of the eigenvalue (so that we get unit gaussians)
  for (int i = 0; i < n; ++i) {
    RooArgList coeffs;
    for (int j = 0; j < n; ++j) {
      snprintf(buff,sizeof(buff),"%s_eigCoeff_%d_%d[%g]", name, i, j, vectors(i,j)*sqrt(values(j)));
      //      snprintf(buff,sizeof(buff),"%s_eigCoeff_%d_%d[%g]", name, parameters_->at(i)->GetName(), j, vectors(i,j)*sqrt(values(j)));
      coeffs.add(*w->factory(buff));
    }
    snprintf(buff,sizeof(buff),"%s_eigBase_%d[%g]", name, i, (dynamic_cast<RooAbsReal*>(parameters_.at(i)))->getVal());
    //    snprintf(buff,sizeof(buff),"%s_eigBase_%d[%g]", name, i, (dynamic_cast<RooAbsReal*>(parameters_->at(i)))->getVal());
    //    snprintf(buff,sizeof(buff),"%s_eigBase_%s[%g]", name, parameters_->at(i)->GetName(), (dynamic_cast<RooAbsReal*>(parameters_->at(i)))->getVal());
    coeffs.add(*w->factory(buff));
    snprintf(buff,sizeof(buff),"%s_eigLin_%d", name, i);
    //    snprintf(buff,sizeof(buff),"%s_eigLin_%s", name, parameters_->at(i)->GetName());
    RooAddition *add = new RooAddition(buff,buff,coeffs,eigvVarsPlusOne);
    w->import(*add,Silence());
    replacements_.add(*add);
  }

  //  cout << "---------------------- NEW WORKSPACE --------------------- "<< endl;
  //  w->Print();

}


RooAbsPdf* PdfDiagonalizer::diagonalize(RooAbsPdf &pdf)
{
  if (!pdf.dependsOn(parameters_)) return 0;

  /*
  if (!pdf.dependsOn(*parameters_)) return 0;
  cout << " original parameters " << endl;
  parameters_->Print();
  cout << " diagonalized parameters " << endl;
  eigenVars_->Print();
  replacements_.Print();
  */

  // now do the customization
  RooCustomizer custom(pdf, name_.c_str());
  for (int i = 0, n = parameters_.getSize(); i < n; ++i) {
    if (pdf.dependsOn(*parameters_.at(i))) {
      custom.replaceArg(*parameters_.at(i), *replacements_.at(i));
    }
  }

  RooAbsPdf *ret = dynamic_cast<RooAbsPdf *>(custom.build());
  ret->SetName((std::string(pdf.GetName()) + "_" + name_).c_str());
  return ret;


}


RooAbsReal* lpdfMCU1; RooAbsReal* lpdfMCU2;
RooAbsReal* lpdfDATAU1; RooAbsReal* lpdfDATAU2;
RooWorkspace *lwMCU1; RooWorkspace *lwMCU2;
RooWorkspace *lwDATAU1; RooWorkspace *lwDATAU2;


void readRecoil(RooAbsReal * & iPdfU1, RooWorkspace * & iwU1,RooAbsReal * & iPdfU2, RooWorkspace * & iwU2,std::string iFName = "recoilfit.root", bool isData) {

  TFile *lFile  = new TFile(iFName.c_str());

  int i0=1;
  int fId=1;

  cout << " ------- reading workspace 1 -------------------- " << endl;

  RooAddPdf* pdfU1 = (RooAddPdf*) lFile->Get(Form("AddU1Y%d",i0));
  iwU1 = new RooWorkspace("wU1","wU1");
  iwU1->import(*pdfU1,Silence());
  RooFitResult* frU1 = (RooFitResult*) lFile->Get(Form("fitresult_AddU1Y%d_Crapsky0_U1_2D",fId));
  
  PdfDiagonalizer * diagoU1 = new PdfDiagonalizer("eigU1", iwU1, *frU1);
  RooAbsPdf* diagPdfU1 = diagoU1->diagonalize(*pdfU1);
  //  //  iwU1->import(*diagPdfU1,Silence());
  //  iwU1->Print("tV");
  
  RooRealVar* myX1=iwU1->var("XVar");
  iPdfU1 = diagPdfU1->createCdf(*myX1,ScanNoCdf());
  //  iPdfU1 = pdfU1->createCdf(*myX1);
  delete pdfU1;
  delete frU1;
  //    delete diagPdfU1;
  //    delete myX1;

  cout << " ------- reading workspace 2 -------------------- " << endl;
    
  RooAddPdf* pdfU2 = (RooAddPdf*) lFile->Get(Form("AddU2Y%d",i0));
  iwU2 = new RooWorkspace("wU2","wU2");
  iwU2->import(*pdfU2,Silence());
  RooFitResult* frU2 = (RooFitResult*) lFile->Get(Form("fitresult_AddU2Y%d_Crapsky0_U2_2D",fId));
    
  PdfDiagonalizer * diagoU2 = new PdfDiagonalizer("eigU2", iwU2, *frU2);
  RooAbsPdf* diagPdfU2 = diagoU2->diagonalize(*pdfU2);
  //  //  iwU2->import(*diagPdfU2,Silence());
  
  RooRealVar* myX2=iwU2->var("XVar");
  if(!isData) iPdfU2 = diagPdfU2->createCdf(*myX2,ScanNoCdf());
  if(isData) iPdfU2 = diagPdfU2->createCdf(*myX2,ScanAllCdf());
  //  iPdfU2 = diagPdfU2->createCdf(*myX2);
  delete pdfU2;
  delete frU2;
  //  delete diagPdfU2;
  //  delete myX2;

}

double triGausInvGraph(double iPVal, double Zpt, RooAbsReal *pdfMCcdf, RooAbsReal *pdfDATAcdf, RooWorkspace *wMC, RooWorkspace *wDATA) {

  RooRealVar* myptm=wMC->var("pt");
  RooRealVar* myptd=wDATA->var("pt");

  myptm->setVal(Zpt);
  myptd->setVal(Zpt);

  RooRealVar* myXm = wMC->var("XVar");
  RooRealVar* myXd = wDATA->var("XVar");

  myXm->setVal(iPVal);
  //  double pVal=pdfDATAcdf->findRoot(*myXd,myXd->getMin(),myXd->getMax(),pdfMCcdf->evaluate());
  double pVal=pdfDATAcdf->findRoot(*myXd,myXd->getMin(),myXd->getMax(),pdfMCcdf->getVal());
  //  cout << "ORIGINAL MC: " << iPVal ;
  //  cout << " from findRoot " << pVal << endl;

  // add protection for outlier since I tabulated up to 5
  if(pVal>=5) pVal=iPVal;
  if(pVal<=-5) pVal=iPVal;

  return pVal;

}

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

//$$$$$$$$$$$$$$$$$$$$$$$$
//$$   end code from runRecoilFit3G.C
//$$$$$$$$$$$$$$$$$$$$$$$$

void makeToys() {

  // readScale
  TF1 * scaleMad = (TF1*) fmad_scale.Get("PFu1Mean_1");
  TF1 * scalePow = (TF1*) fpow_scale.Get("PFu1Mean_1");

  TF1 * meanRMSMad = (TF1*) fmad_scale.Get("PFu1MeanRMS_1");
  TF1 * meanRMSPow = (TF1*) fpow_scale.Get("PFu1MeanRMS_1");

  TF1 * meanRMSu2Mad = (TF1*) fmad_scale.Get("PFu2MeanRMS_1");
  TF1 * meanRMSu2Pow = (TF1*) fpow_scale.Get("PFu2MeanRMS_1");

  readRecoil(lpdfMCU1, lwMCU1, lpdfMCU2, lwMCU2, fpow_scale.GetName(),true);
  //  readRecoil(lpdfDATAU1, lwDATAU1, lpdfDATAU2, lwDATAU2, fpow_scale.GetName(),false);
  readRecoil(lpdfDATAU1, lwDATAU1, lpdfDATAU2, lwDATAU2, fmad_scale.GetName(),true);

  ////

  RooAddPdf* pdfU1 = (RooAddPdf*) fpow_scale.Get(Form("AddU1Y%d",1));
  RooAddPdf* pdfU2 = (RooAddPdf*) fpow_scale.Get(Form("AddU2Y%d",1));

  RooAddPdf* pdfU1DATA = (RooAddPdf*) fmad_scale.Get(Form("AddU1Y%d",1));
  RooAddPdf* pdfU2DATA = (RooAddPdf*) fmad_scale.Get(Form("AddU2Y%d",1));

  ///

  RooRealVar* myX1data=lwDATAU2->var("XVar");
  RooRealVar* myX1=lwMCU2->var("XVar");

  RooRealVar lRXVar("XVarCorr","corrected",0,-50.,50.);
  RooRealVar* myX1Corr = new RooRealVar("","",-50.,50.);

  RooDataSet* toyMC = pdfU2->generate(*myX1,100000) ;
  RooDataSet lData("corrected","Corrected",RooArgSet(*myX1));

  double Z_pt=15;

  TH1F *hPrime = new TH1F("u2prime","u2prime", 200,-5.,5.);
  TH1F *hOri = new TH1F("u2ori","u2ori", 200,-5.,5.);

  for (int i=0; i<toyMC->numEntries(); i++ ) {

    RooRealVar* myX1toy=toyMC->get(i)->find(myX1->GetName());
    double u2_ori=myX1toy->getVal();
    //    cout << "uoriginal " << u2_ori ;
    //    double u2prime = meanRMSu2Mad->Eval(Z_pt) * triGausInvGraph(u2_ori/meanRMSu2Pow->Eval(Z_pt), Z_pt, lpdfMCU2, lpdfDATAU2, lwMCU2, lwDATAU2);
    double u2prime = triGausInvGraph(u2_ori, Z_pt, lpdfMCU2, lpdfDATAU2, lwMCU2, lwDATAU2);
    // double u2prime = triGausInvGraph(u2_ori, Z_pt, lpdfMCU2, lpdfMCU2, lwMCU2, lwMCU2);
    //    cout << "   u2prime= " << u2prime << endl;
    myX1Corr->setVal(u2prime);
    lData->add(*myX1Corr);

    hPrime->Fill(u2prime);
    hOri->Fill(u2_ori);

  }

  //  TH1* hh_target = pdfU2DATA->createHistogram("dataHisto",*myX1data,Binning(100));
  //  TH1* hh_target = pdfU2->createHistogram("dataHisto",*myX1data,Binning(100));
  //  TH1* hh_corrected = lData->createHistogram("dataHisto",*myX1Corr,Binning(100));

  /*
  hh_target->SetLineColor(2);
  hh_target->Draw("hist");
  hh_corrected->Draw("hist same");
  hPrime->SetLineColor(4);
  */
  hPrime->Divide(hOri);
  hPrime->Draw("hist"); 

}


void closurePlot(bool doOriginal, bool doMad) {

  TH1F *htkmet = new TH1F("tkmet","tkmet", 500,0,500);
  TH1F *hmt = new TH1F("tkmt","hmt", 150,0,150);
  TH1F *hZ = new TH1F("Z","Z", 200,0,200);

  //
  TH1F *hU1 = new TH1F("hU1","hU1", 200,-10.,10.);
  TH1F *hU2 = new TH1F("hU2","hU2", 200,-10.,10.);

  TH1F *hU1corr = new TH1F("hU1corr","hU1corr", 200,-10.,10.);
  TH1F *hU2corr = new TH1F("hU2corr","hU2corr", 200,-10.,10.);

  TH1F *hU1redone = new TH1F("hU1redone","hU1redone", 200,-10.,10.);
  TH1F *hU2redone = new TH1F("hU2redone","hU2redone", 200,-10.,10.);
  ///

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

  ///$$$$$$$
  ///$$$ BEGIN READ files
  ///$$$$$$$

  // readScale
  TF1 * scaleMad = (TF1*) fmad_scale.Get("PFu1Mean_1");
  TF1 * scalePow = (TF1*) fpow_scale.Get("PFu1Mean_1");

  TF1 * meanRMSMad = (TF1*) fmad_scale.Get("PFu1MeanRMS_1");
  TF1 * meanRMSPow = (TF1*) fpow_scale.Get("PFu1MeanRMS_1");

  TF1 * meanRMSu2Mad = (TF1*) fmad_scale.Get("PFu2MeanRMS_1");
  TF1 * meanRMSu2Pow = (TF1*) fpow_scale.Get("PFu2MeanRMS_1");

  readRecoil(lpdfMCU1, lwMCU1, lpdfMCU2, lwMCU2, fpow_scale.GetName());
  readRecoil(lpdfDATAU1, lwDATAU1, lpdfDATAU2, lwDATAU2, fmad_scale.GetName());

  ///$$$$$$$
  ///$$$ END READ files
  ///$$$$$$$

  if(doOriginal) hmt->SetName("hMtMad");
  if(!doOriginal) hmt->SetName("hMtPOW");

  trktree->SetBranchAddress("evt", &evt);
  trktree->SetBranchAddress("tkmet", &tkmet);
  trktree->SetBranchAddress("tkmet_phi", &tkmet_phi);

  trktree->SetBranchAddress("tkmet_ori", &tkmet_ori);
  trktree->SetBranchAddress("tkmet_phi_ori", &tkmet_phi_ori);

  trktree->SetBranchAddress("ZGen_pt", &Z_pt);
  trktree->SetBranchAddress("ZGen_phi", &Z_phi);

  trktree->SetBranchAddress("MuPos_pt", &MuPos_pt);
  trktree->SetBranchAddress("MuPos_eta", &MuPos_eta);
  trktree->SetBranchAddress("MuPos_phi", &MuPos_phi);
  trktree->SetBranchAddress("MuPos_mass", &MuPos_mass);

  trktree->SetBranchAddress("MuNeg_pt", &MuNeg_pt);
  trktree->SetBranchAddress("MuNeg_eta", &MuNeg_eta);
  trktree->SetBranchAddress("MuNeg_phi", &MuNeg_phi);
  trktree->SetBranchAddress("MuNeg_mass", &MuNeg_mass);

  //  Int_t nrow = trktree->GetEntries();
  Int_t nrow = 250000;
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

    /////// $$$$$$$$$$$$$
    /////// begin U closure plots
    /////// $$$$$$$$$$$$$

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
    hdeltaU2pullCorr->Fill(u2/meanRMSu2Mad->Eval(Z_pt));

    //    hdeltaU2pull2D->Fill((u2)/meanRMSu2Mad->Eval(Z_pt),(u2_ori)/meanRMSu2Pow->Eval(Z_pt));
    //    hdeltaU1pull2D->Fill((u1-scaleMad->Eval(Z_pt))/meanRMSMad->Eval(Z_pt),(u1_ori-scalePow->Eval(Z_pt))/meanRMSPow->Eval(Z_pt));

    hdeltaU2pull2D->Fill(u2,u2_ori);
    hdeltaU1pull2D->Fill(u1-scaleMad->Eval(Z_pt),u1_ori-scalePow->Eval(Z_pt));

    if(Z_pt>0 && Z_pt<5) hdeltaU1_05->Fill(u1-u1_ori);
    if(Z_pt>5 && Z_pt<10) hdeltaU1_510->Fill(u1-u1_ori);
    if(Z_pt>10 && Z_pt<15) hdeltaU1_1015->Fill(u1-u1_ori);

    /*
      RooAbsReal* lpdfMCU1; RooAbsReal* lpdfMCU2;
      RooAbsReal* lpdfDATAU1; RooAbsReal* lpdfDATAU2;
      RooWorkspace *lwMCU1; RooWorkspace *lwMCU2;
      RooWorkspace *lwDATAU1; RooWorkspace *lwDATAU2;
    */

    double u1prime = meanRMSMad->Eval(Z_pt) * triGausInvGraph(u1_ori/meanRMSPow->Eval(Z_pt), Z_pt, lpdfMCU1, lpdfDATAU1, lwMCU1, lwDATAU1);
    double u2prime = meanRMSu2Mad->Eval(Z_pt) * triGausInvGraph(u2_ori/meanRMSu2Pow->Eval(Z_pt), Z_pt, lpdfMCU2, lpdfDATAU2, lwMCU2, lwDATAU2);
    cout << "Zpt=" << Z_pt <<  "; u2_original=" << u2_ori << "; u2corr=" << u2 << "; u2prime " << u2prime << endl;
    //    cout << "--------------------------" << endl;

    hU1->Fill(u1_ori);
    hU1redone->Fill(u1prime);
    hU1corr->Fill(u1);

    hU2->Fill(u2_ori);
    hU2redone->Fill(u2prime);
    hU2corr->Fill(u2);

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

  hU1->Write();
  hU1redone->Write();
  hU1corr->Write();
  
  hU2->Write();
  hU2redone->Write();
  hU2corr->Write();
  
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
