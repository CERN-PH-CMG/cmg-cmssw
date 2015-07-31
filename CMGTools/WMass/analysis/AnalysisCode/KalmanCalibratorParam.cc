#include "KalmanCalibratorParam.h"
#include <math.h>
#include <string.h>
#include <iostream>

//-----------------------------------------------------------------------------------//
KalmanCalibratorParam::KalmanCalibratorParam(bool isData) {

  random_ = new TRandom3(10101982);

  if (isData) {
    std::string path("../utils/kalmanCalibration_data_31072015.root");
    file_ = new TFile(path.c_str());
    magnetic = (TH2F*)file_->Get("magnetic");
  }
  else {
    std::string path("../utils/kalmanCalibration_mc_31072015.root");
    file_ = new TFile(path.c_str());
  }
  // file_->ls();
  isData_ = isData;


  scale_A1 =(TH3F*)file_->Get("A1"); 
  scale_A2 =(TH3F*)file_->Get("A2"); 
  scale_L =(TH3F*)file_->Get("L"); 
  scale_e = (TH3F*)file_->Get("e") ; 
  scale_B0 = (TH3F*)file_->Get("B0") ; 
  scale_B1 = (TH3F*)file_->Get("B1") ; 
  scale_B2 = (TH3F*)file_->Get("B2") ; 
  scale_C1 = (TH3F*)file_->Get("C1") ; 
  scale_C2 = (TH3F*)file_->Get("C2") ; 


  shifted_A1 =(TH3F*)scale_A1->Clone();
  shifted_A1->SetName("shifted_A1");

  shifted_A2 =(TH3F*)scale_A2->Clone();
  shifted_A2->SetName("shifted_A2");

  shifted_L =(TH3F*)scale_L->Clone();
  shifted_L->SetName("shifted_L");

  shifted_e = (TH3F*)scale_e->Clone();
  shifted_e->SetName("shifted_e");

  shifted_B0 = (TH3F*)scale_B0->Clone();
  shifted_B0->SetName("shifted_B0"); 
  shifted_B1 = (TH3F*)scale_B1->Clone();
  shifted_B1->SetName("shifted_B1"); 
  shifted_B2 = (TH3F*)scale_B2->Clone();
  shifted_B2->SetName("shifted_B2"); 
  shifted_C1 = (TH3F*)scale_C1->Clone();
  shifted_C1->SetName("shifted_C1"); 
  shifted_C2 = (TH3F*)scale_C2->Clone();
  shifted_C2->SetName("shifted_C2"); 



  a2_ = (TH1F*)file_->Get("a2"); 
  b2_ = (TH1F*)file_->Get("b2");

  smearing_ = (TH3F*)file_->Get("smearing");
  


  closure_ = (TH3F*)file_->Get("closure"); 

  cholesky_ = (TMatrixDSym*)file_->Get("cholesky");
  covHistoMap_ = (TH1I*)file_->Get("covHistoMap");
  covBinMap_ = (TH1I*)file_->Get("covBinMap");

  eigenvalues_ = (TVectorD*)file_->Get("eigenvalues");
  eigenvectors_ = (TMatrixD*)file_->Get("eigenvectors");

  varyClosure_=0;
}

//-----------------------------------------------------------------------------------//
void KalmanCalibratorParam::reset() {
  varyClosure_=0;
  resetHisto(shifted_A1,scale_A1);
  resetHisto(shifted_A2,scale_A2);
  resetHisto(shifted_L,scale_L);
  resetHisto(shifted_e,scale_e);
  resetHisto(shifted_B0,scale_B0);
  resetHisto(shifted_B1,scale_B2);
  resetHisto(shifted_B2,scale_B2);
  resetHisto(shifted_C1,scale_C1);
  resetHisto(shifted_C2,scale_C2);
}

//-----------------------------------------------------------------------------------//
void KalmanCalibratorParam::randomize() {
  reset();
  //create random gaussian vector
  int N = cholesky_->GetNrows();
  TVectorD vec(N);
  for (int i=0;i<N;++i)
  vec[i] = random_->Gaus(0,1);

  TVectorD correlated = (*cholesky_)*vec;
  for (int i=0;i<N;++i) {
    int histo = covHistoMap_->GetBinContent(i+1);
    int bin = covBinMap_->GetBinContent(i+1);
    float value = correlated[i];
    switch (histo) {
    case 1:
      shifted_A1->SetBinContent(bin,scale_A1->GetBinContent(bin)+value);
      break;
    case 2:
      shifted_A2->SetBinContent(bin,scale_A2->GetBinContent(bin)+value);
      break;
    case 3:
      shifted_L->SetBinContent(bin,scale_L->GetBinContent(bin)+value);
      break;
    case 4:
      shifted_e->SetBinContent(bin,scale_e->GetBinContent(bin)+value);
      break;

    case 5:
      shifted_B0->SetBinContent(bin,scale_B0->GetBinContent(bin)+value);
      break;
    case 6:
      shifted_B1->SetBinContent(bin,scale_B1->GetBinContent(bin)+value);
      break;
    case 7:
      shifted_B2->SetBinContent(bin,scale_B2->GetBinContent(bin)+value);
      break;
    case 8:
      shifted_C1->SetBinContent(bin,scale_C1->GetBinContent(bin)+value);
      break;
    case 9:
      shifted_C2->SetBinContent(bin,scale_C2->GetBinContent(bin)+value);
      break;

    default:
      printf("UNKNOWN HISTO-> problem while varying (That is important\n");
    }
  }

}

//-----------------------------------------------------------------------------------//
double KalmanCalibratorParam::closure(double pt,double eta) {

  if (fabs(eta)>1.4)
  return 0.0;

  Int_t bin = closure_->GetBin(
                              closure_->GetXaxis()->FindBin(pt),	       
                              closure_->GetYaxis()->FindBin(fabs(eta)),
                              1);
  return closure_->GetBinContent(bin)-1.0;
}

//-----------------------------------------------------------------------------------//
void KalmanCalibratorParam::resetHisto(TH1* histo,const TH1* ref) {
  for (int i=0;i<histo->GetNbinsX()+1;++i)
  for (int j=0;j<histo->GetNbinsY()+1;++j)
  for (int k=0;k<histo->GetNbinsZ()+1;++k) {
    int bin = histo->GetBin(i,j,k);
    histo->SetBinContent(bin,ref->GetBinContent(bin));
  }
}

//-----------------------------------------------------------------------------------//
KalmanCalibratorParam::~KalmanCalibratorParam() {

  if(shifted_A1) delete shifted_A1;
  if(shifted_A2) delete shifted_A2;
  if(shifted_L) delete shifted_L;
  if(shifted_e) delete shifted_e;
  if(shifted_B0) delete shifted_B0;
  if(shifted_B1) delete shifted_B1;
  if(shifted_B2) delete shifted_B2;
  if(shifted_C1) delete shifted_C1;
  if(shifted_C2) delete shifted_C2;
  file_->Close();
}

//-----------------------------------------------------------------------------------//
void KalmanCalibratorParam::getCorrectedPt(TLorentzVector &muon,int charge) {
  double pt=muon.Pt(); double eta=muon.Eta(); double phi=muon.Phi(); double mass=muon.M();

  double magneticMapFactor=1.0;
  if (isData_)
  magneticMapFactor = magnetic->GetBinContent(magnetic->GetBin(
                                              magnetic->GetXaxis()->FindBin(phi),
                                              magnetic->GetYaxis()->FindBin(eta)
                                              )
                                              );
  
  double curvature = (magneticMapFactor)/pt;

  ///// 1.4 ETA PROTECTION

  if (fabs(eta)>1.4){
    pt = 1.0/curvature;
  }else{
    double e = shifted_e->GetBinContent(scale_e->GetBin(1,scale_e->GetYaxis()->FindBin(eta),1));
    double sinTheta  = sin(2*atan(exp(-eta))); 
    double A1 = shifted_A1->GetBinContent(13);
    double A2 = shifted_A2->GetBinContent(13);
    //    double L = fabs(shifted_L->GetBinContent(13));

    double B0 = shifted_B0->GetBinContent(scale_B0->GetBin(1,scale_B0->GetYaxis()->FindBin(eta),1));
    double B1 = shifted_B1->GetBinContent(scale_B1->GetBin(1,scale_B1->GetYaxis()->FindBin(eta),1));
    double B2 = shifted_B2->GetBinContent(scale_B2->GetBin(1,scale_B2->GetYaxis()->FindBin(eta),1));
    double C1 = shifted_C1->GetBinContent(scale_C1->GetBin(1,scale_C1->GetYaxis()->FindBin(eta),1));
    double C2 = shifted_C2->GetBinContent(scale_C2->GetBin(1,scale_C2->GetYaxis()->FindBin(eta),1));
    
    double B = B0+B1*sin(phi)+B2*sin(2*phi)+C1*cos(phi)+C2*cos(2*phi);
    double mag=A1+A2*eta*eta;
    // if (fabs(eta)<L)
    //   mag=A1;
    // else if (eta>L)
    //   mag=A1+A2*(eta-L)*(eta-L);
    // else if (eta<-L)
    //   mag=A1+A2*(eta+L)*(eta+L);
    curvature = mag*curvature -e*sinTheta*curvature*curvature+charge*B;
    
    // return (1.0/curvature)*(1.0+varyClosure_*closure(pt,eta));
    pt = (1.0/curvature)*(1.0+varyClosure_*closure(pt,eta));
    // std::cout << "final pt= " << pt << " eta= " << eta << " phi= " << phi << " mass= " << mass << std::endl; 

    // std::cout << "muon inside KalmanCalibratorParam::getCorrectedPt()" << std::endl;
    // muon.Print();
  }
  
  muon.SetPtEtaPhiM(pt,eta,phi,mass);

}

//-----------------------------------------------------------------------------------//
double KalmanCalibratorParam::getCorrectedPtMag(double pt,double eta,double phi) {
  double magneticMapFactor=1.0;
  if (isData_)
    magneticMapFactor = magnetic->GetBinContent(magnetic->GetBin(
                                  magnetic->GetXaxis()->FindBin(phi),
                                  magnetic->GetYaxis()->FindBin(eta)));

  //double sinTheta  = sin(2*atan(exp(-eta))); 
  //    double e = shifted_e->GetBinContent(scale_e->GetXaxis()->FindBin(eta));
  //    double curvature = (magneticMapFactor-1.0)/pt +1.0/(pt-sinTheta*e);
  double curvature = (magneticMapFactor)/pt;
  return 1.0/curvature;
}

//-----------------------------------------------------------------------------------//
double KalmanCalibratorParam::getCorrectedError(double pt,double eta,double error) {

  int bin = a2_->GetXaxis()->FindBin(eta); 
  float a2 = a2_->GetBinContent(bin)/4.0;
  bin = b2_->GetXaxis()->FindBin(eta); 
  float b2 = b2_->GetBinContent(bin)/4.0;


  float diff = error*error +b2+a2*pt*pt;
  if (diff<0.0) {
    printf("Negative square root pt=%f eta=%f error2=%f residual==%f \n",pt,eta,error*error,diff);
    return error;
  }
  return sqrt(diff);
}

//-----------------------------------------------------------------------------------//
int KalmanCalibratorParam::getN() {
  return eigenvalues_->GetNoElements();
}

//-----------------------------------------------------------------------------------//
void KalmanCalibratorParam::varyClosure(int sigmas) {
  varyClosure_ = sigmas;

}

//-----------------------------------------------------------------------------------//
void KalmanCalibratorParam::vary(int ii,int sigmas) {
  reset();
  int N = getN();

  TVectorD * v = new TVectorD(N);
  v->Zero();

  if (ii>N) {
    printf("Hey you are trying to vary outside the number of elements,which is pretty stupid \n");
    printf("You ask for element %d out of %d\n",ii,N);   
  }

  (*v)[ii] = sqrt((*eigenvalues_)[ii])*sigmas;


  TVectorD correlated = (*eigenvectors_)*(*v);

  for (int i=0;i<N;++i) {
    int histo = covHistoMap_->GetBinContent(i+1);
    int bin = covBinMap_->GetBinContent(i+1);
    float value = correlated[i];
    switch (histo) {
    case 1:
      shifted_A1->SetBinContent(bin,scale_A1->GetBinContent(bin)+value);
      break;
    case 2:
      shifted_A2->SetBinContent(bin,scale_A2->GetBinContent(bin)+value);
      break;
    case 3:
      shifted_L->SetBinContent(bin,scale_L->GetBinContent(bin)+value);
      break;
    case 4:
      shifted_e->SetBinContent(bin,scale_e->GetBinContent(bin)+value);
      break;

    case 5:
      shifted_B0->SetBinContent(bin,scale_B0->GetBinContent(bin)+value);
      break;
    case 6:
      shifted_B1->SetBinContent(bin,scale_B1->GetBinContent(bin)+value);
      break;
    case 7:
      shifted_B2->SetBinContent(bin,scale_B2->GetBinContent(bin)+value);
      break;
    case 8:
      shifted_C1->SetBinContent(bin,scale_C1->GetBinContent(bin)+value);
      break;
    case 9:
      shifted_C2->SetBinContent(bin,scale_C2->GetBinContent(bin)+value);
      break;

    default:
      printf("UNKNOWN HISTO-> problem while varying (That is important\n");
    }

  }

  delete v;

}

//-----------------------------------------------------------------------------------//
void KalmanCalibratorParam::smear(TLorentzVector &muon) {
  double pt=muon.Pt(); double eta=muon.Eta(); double phi=muon.Phi(); double mass=muon.M(); 

  Int_t binx = smearing_->GetXaxis()->FindBin(pt);
  if (binx==0){
    binx=1;
  }
  if (binx==smearing_->GetNbinsX()+1){
    binx=smearing_->GetNbinsX();
  }

  Int_t biny = smearing_->GetYaxis()->FindBin(fabs(eta));
  Int_t bin = smearing_->GetBin(binx,biny,1);
  Double_t factor = smearing_->GetBinContent(bin);
  if ((isData_ && factor>0) || ((!isData_) && factor<0))
    pt=pt; 
  }else{
    factor = sqrt(fabs(factor));
    pt+=random_->Gaus(0.0,factor*pt);
  }
  muon.SetPtEtaPhiM(pt,eta,phi,mass);
}
