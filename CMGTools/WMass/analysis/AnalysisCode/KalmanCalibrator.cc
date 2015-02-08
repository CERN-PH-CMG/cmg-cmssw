#include "KalmanCalibrator.h"
#include <math.h>
#include <string.h>
#include <iostream>

//-----------------------------------------------------------------------------------//
KalmanCalibrator::KalmanCalibrator(bool isData) {

  random_ = new TRandom3(10101982);

  if (isData) {
    std::string path("../utils/kalmanCalibration_data_06022015.root");
    file_ = new TFile(path.c_str());
    magnetic = (TH2F*)file_->Get("magnetic");
  }
  else {
    std::string path("../utils/kalmanCalibration_mc_06022015.root");
    file_ = new TFile(path.c_str());
  }
  // file_->ls();
  isData_ = isData;

  scale_A1 =(TH3F*)file_->Get("A1"); 
  scale_A2 =(TH3F*)file_->Get("A2"); 
  scale_e = (TH3F*)file_->Get("e") ; 
  scale_B = (TH3F*)file_->Get("B") ; 

  shifted_A1 =(TH3F*)file_->Get("A1"); 
  shifted_A2 =(TH3F*)file_->Get("A2"); 
  shifted_e = (TH3F*)file_->Get("e") ; 
  // shifted_e->Print();
  shifted_B = (TH3F*)file_->Get("B") ; 

  sigma_A_target = (TH3F*)file_->Get("sigma_A_target"); 
  sigma_B_target = (TH3F*)file_->Get("sigma_B_target"); 
  sigma_C_target = (TH3F*)file_->Get("sigma_C_target"); 

  sigma_A_ref = (TH3F*)file_->Get("sigma_A_ref"); 
  sigma_B_ref = (TH3F*)file_->Get("sigma_B_ref"); 
  sigma_C_ref = (TH3F*)file_->Get("sigma_C_ref"); 

  ebe_A = (TH3F*)file_->Get("ebe_A"); 
  ebe_B = (TH3F*)file_->Get("ebe_B"); 
  ebe_C = (TH3F*)file_->Get("ebe_C"); 

}

//-----------------------------------------------------------------------------------//
void KalmanCalibrator::reset() {
  resetHisto(shifted_A1,scale_A1);
  resetHisto(shifted_A2,scale_A2);
  resetHisto(shifted_e,scale_e);
  resetHisto(shifted_B,scale_B);

}

//-----------------------------------------------------------------------------------//
void KalmanCalibrator::randomize() {
  reset();
  randomizeHisto(shifted_A1);
  randomizeHisto(shifted_A2);
  randomizeHisto(shifted_e);
  randomizeHisto(shifted_B);
}


//-----------------------------------------------------------------------------------//
void KalmanCalibrator::randomizeHisto(TH1* histo) {
  for (int i=0;i<histo->GetNbinsX()+1;++i)
  for (int j=0;j<histo->GetNbinsY()+1;++j)
  for (int k=0;k<histo->GetNbinsZ()+1;++k) {
    int bin = histo->GetBin(i,j,k);
    histo->SetBinContent(bin,random_->Gaus(histo->GetBinContent(bin),histo->GetBinError(bin)));
  }
}

//-----------------------------------------------------------------------------------//
void KalmanCalibrator::resetHisto(TH1* histo,const TH1* ref) {
  for (int i=0;i<histo->GetNbinsX()+1;++i)
  for (int j=0;j<histo->GetNbinsY()+1;++j)
  for (int k=0;k<histo->GetNbinsZ()+1;++k) {
    int bin = histo->GetBin(i,j,k);
    histo->SetBinContent(bin,ref->GetBinContent(bin));
  }
}

//-----------------------------------------------------------------------------------//
KalmanCalibrator::~KalmanCalibrator() {

  if(shifted_A1) delete shifted_A1;
  if(shifted_A2) delete shifted_A2;
  if(shifted_e) delete shifted_e;
  if(shifted_B) delete shifted_B;
  file_->Close();
}


//-----------------------------------------------------------------------------------//
// double KalmanCalibrator::getCorrectedPt(double pt,double eta,double phi,int charge) {
void KalmanCalibrator::getCorrectedPt(TLorentzVector &muon,int charge) {
  double pt=muon.Pt(); double eta=muon.Eta(); double phi=muon.Phi(); double mass=muon.M(); 
  // std::cout << "initial pt= " << pt << " eta= " << eta << " phi= " << phi << " mass= " << mass << std::endl; 
  
  double magneticMapFactor=1.0;
  if (isData_)
    magneticMapFactor = magnetic->GetBinContent(magnetic->GetBin(
                                                                magnetic->GetXaxis()->FindBin(phi),
                                                                magnetic->GetYaxis()->FindBin(eta)
                                                                )
                                                );
  // std::cout << "magneticMapFactor= " << magneticMapFactor << std::endl;
  double curvature = magneticMapFactor/pt;
  // std::cout << "initial curvature= " << curvature << std::endl;
  double sinTheta  = sin(2*atan(exp(-eta))); 
  double e = shifted_e->GetBinContent(scale_e->GetBin(1,
                                                      scale_e->GetYaxis()->FindBin(eta),
                                                      scale_e->GetZaxis()->FindBin(phi)
                                                      )
                                      );
  // std::cout << "e= " << e 
            // << " scale_e->GetYaxis()->FindBin("<<eta<<")= " << scale_e->GetYaxis()->FindBin(eta)
            // << " scale_e->GetZaxis()->FindBin("<<phi<<")= " << scale_e->GetZaxis()->FindBin(phi)
            // << std::endl;
  double A1 = shifted_A1->GetBinContent(scale_A1->GetBin(1,1,1));
  double A2 = shifted_A2->GetBinContent(scale_A2->GetBin(1,1,1));
  double B = shifted_B->GetBinContent(scale_B->GetBin(1,
                                                      scale_B->GetYaxis()->FindBin(eta),
                                                      scale_B->GetZaxis()->FindBin(phi)
                                                      )
                                      );
  // std::cout << "A1= " << A1 << " A2= " << A2 << " B= " << B << std::endl;
  curvature = (A2*eta*eta+A1-1)*curvature +curvature/(1+sinTheta*e*curvature) +charge*B;
  // std::cout << "final curvature= " << curvature << std::endl;
  
  // return 1.0/curvature;
  pt = 1.0/curvature;
  // std::cout << "final pt= " << pt << " eta= " << eta << " phi= " << phi << " mass= " << mass << std::endl; 

  muon.SetPtEtaPhiM(pt,eta,phi,mass);
  // std::cout << "muon inside KalmanCalibrator::getCorrectedPt()" << std::endl;
  // muon.Print();
}

//-----------------------------------------------------------------------------------//
double KalmanCalibrator::getCorrectedPtMag(double pt,double eta,double phi) {
  double magneticMapFactor=1.0;
  if (isData_)
  magneticMapFactor = magnetic->GetBinContent(magnetic->GetBin(
  magnetic->GetXaxis()->FindBin(phi),
  magnetic->GetYaxis()->FindBin(eta)));
  double curvature = magneticMapFactor/pt;
  return 1.0/curvature;
}

//-----------------------------------------------------------------------------------//
double KalmanCalibrator::getCorrectedError(double pt,double eta,double error) {
  double eA  = ebe_A->GetBinContent(ebe_A->GetBin(1,ebe_A->GetYaxis()->FindBin(eta),1));
  double eB  = ebe_B->GetBinContent(ebe_B->GetBin(1,ebe_B->GetYaxis()->FindBin(eta),1));
  double eC  = ebe_C->GetBinContent(ebe_C->GetBin(1,ebe_C->GetYaxis()->FindBin(eta),1));

  double diff = error*error+eA+eB*pt+eC*pt*pt;

  if (diff<0.0) {
    printf("Negative square root !!Ignoring\n");
    return error;
  }
  return sqrt(diff);
}


//-----------------------------------------------------------------------------------//
double KalmanCalibrator::smearGEN(double pt,double eta) {
  double A  = sigma_A_target->GetBinContent(sigma_A_target->GetBin(1,sigma_A_target->GetYaxis()->FindBin(eta),1));
  double B  = sigma_B_target->GetBinContent(sigma_B_target->GetBin(1,sigma_B_target->GetYaxis()->FindBin(eta),1));
  double C  = sigma_C_target->GetBinContent(sigma_C_target->GetBin(1,sigma_C_target->GetYaxis()->FindBin(eta),1));
  
  double resolution = sqrt( 4*A +4*B*pt+4*C*pt*pt )*pt;

  return pt+random_->Gaus(0.0,resolution);
}

//-----------------------------------------------------------------------------------//
// double KalmanCalibrator::smear(double pt,double eta,bool reverse) {
void KalmanCalibrator::smear(TLorentzVector &muon,bool reverse) {
  double pt=muon.Pt(); double eta=muon.Eta(); double phi=muon.Phi(); double mass=muon.M(); 
  
  double Aref  = sigma_A_ref->GetBinContent(sigma_A_ref->GetBin(1,sigma_A_ref->GetYaxis()->FindBin(eta),1));
  double Bref  = sigma_B_ref->GetBinContent(sigma_B_ref->GetBin(1,sigma_B_ref->GetYaxis()->FindBin(eta),1));
  double Cref  = sigma_C_ref->GetBinContent(sigma_C_ref->GetBin(1,sigma_C_ref->GetYaxis()->FindBin(eta),1));
  
  double resolutionRef2 =  4*Aref +4*Bref*pt+4*Cref*pt*pt ;

  double Atarget  = sigma_A_target->GetBinContent(sigma_A_target->GetBin(1,sigma_A_target->GetYaxis()->FindBin(eta),1));
  double Btarget  = sigma_B_target->GetBinContent(sigma_B_target->GetBin(1,sigma_B_target->GetYaxis()->FindBin(eta),1));
  double Ctarget  = sigma_C_target->GetBinContent(sigma_C_target->GetBin(1,sigma_C_target->GetYaxis()->FindBin(eta),1));

  double resolutionTarget2 = 4*Atarget +4*Btarget*pt+4*Ctarget*pt*pt;

  if (resolutionTarget2<=resolutionRef2) {
    //assume we smear MC
    if(!reverse){
      double factor = sqrt(resolutionRef2-resolutionTarget2)*pt;
      // return pt+random_->Gaus(0,factor);
      // muon.SetPtEtaPhiM(pt+random_->Gaus(0,factor),eta,phi,mass);
      pt+=random_->Gaus(0,factor);
      // else
      // // return pt;
      // muon.SetPtEtaPhiM(pt,eta,phi,mass);
    }
  } else {
    //assume we smear Data
    // printf("You are smearing something strange!.. \n");
    // std::cout << "You are smearing something strange!.."
         // << " pt= " << pt 
         // << " eta= " << eta 
         // << " phi= " << phi
    // << std::endl;
    if(reverse){
      double factor = sqrt(resolutionTarget2-resolutionRef2)*pt;
      // // return pt+random_->Gaus(0,factor);
      // muon.SetPtEtaPhiM(pt+random_->Gaus(0,factor),eta,phi,mass);
      pt+=random_->Gaus(0,factor);
      // else
      // // return pt;
      // muon.SetPtEtaPhiM(pt,eta,phi,mass);
    }

  }
  
    muon.SetPtEtaPhiM(pt,eta,phi,mass);
  
}
