#ifndef CMGTools_H2TauTau_SelectionEfficiency_H
#define CMGTools_H2TauTau_SelectionEfficiency_H

#include <math.h> 
#include "TMath.h" 
#include <limits>



class SelectionEfficiency {
public:
  SelectionEfficiency(){} ;

  /*
    Factors correcting for id and isolation differences between MC and Data
  */  
  //****************
  //parameters taken from AN-11-390 v8, should be for Fall11 MC 
  //*****************  
  double effCorrMu2011AB(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(10.0<pt&&pt<=15.0) return 0.92;
      if(15.0<pt&&pt<=20.0) return 0.948;
      if(20.0<pt) return 0.9933;
      return 0;
    }else if(1.479<fabs(eta)&&fabs(eta)<2.1){//Endcap
      if(10.0<pt&&pt<=15.0) return 0.98;
      if(15.0<pt&&pt<=20.0) return 0.962;
      if(20.0<pt) return 0.9982;
      return 0;
    }
    return 0;
  }


  ///for e-Tau channel the electron id+iso corrections taken from twiki:
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorking2012
//   ID
// 20 < pT < 30 Barrel 	0.955 ± 0.002
// 20 < pT < 30 Endcap 	0.938 ± 0.007
// pT > 30 Barrel 	1.044 ± 0.001
// pT > 30 Endcap 	0.977 ± 0.001
// Iso
// 20 < pT < 30 Barrel 	0.980 ± 0.003
// 20 < pT < 30 Endcap 	0.967 ± 0.006
// pT > 30 Barrel 	0.984 ± 0.001
// pT > 30 Endcap 	0.989 ± 0.001 
  double effCorrEle2011AB(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(20.0<pt&&pt<=30.0) return 0.955*0.980;
      if(30.0<pt) return 1.044*0.984;
      return 0;
    }else if(1.479<fabs(eta)&&fabs(eta)<2.1){//Endcap
      if(20.0<pt&&pt<=30.0) return 0.938*0.967;
      if(30.0<pt) return 0.977*0.989;
      return 0;
    }
    return 0;
  }


  //muTau 2012
// pT /  eta bin 	MC Efficiency 	Data Efficiency 	Scale Factor
// ID
// 15 < pT < 20 Barrel 	0.979 ± 0.003 	0.968 ± 0.002 	0.989 ± 0.004
// 15 < pT < 20 Endcap 	0.982 ± 0.005 	0.959 ± 0.004 	0.977 ± 0.007
// 20 < pT < 30 Barrel 	0.970 ± 0.002 	0.961 ± 0.001 	0.991 ± 0.002
// 20 < pT < 30 Endcap 	0.976 ± 0.003 	0.951 ± 0.002 	0.974 ± 0.004
// pT > 30 Barrel 	0.973 ± 0.001 	0.962 ± 0.001 	0.989 ± 0.001
// pT > 30 Endcap 	0.966 ± 0.001 	0.955 ± 0.001 	0.989 ± 0.001
// Iso
// 15 < pT < 20 Barrel 	0.728 ± 0.011 	0.688 ± 0.006 	0.945 ± 0.017
// 15 < pT < 20 Endcap 	0.716 ± 0.018 	0.750 ± 0.010 	1.047 ± 0.030
// 20 < pT < 30 Barrel 	0.771 ± 0.004 	0.775 ± 0.002 	1.005 ± 0.006
// 20 < pT < 30 Endcap 	0.841 ± 0.007 	0.834 ± 0.004 	0.992 ± 0.009
// pT > 30 Barrel 	0.921 ± 0.001 	0.915 ± 0.001 	0.993 ± 0.001
// pT > 30 Endcap 	0.930 ± 0.002 	0.935 ± 0.001 	1.005 ± 0.002 
  double effCorrMu2012AB(double pt,double eta){
    if(fabs(eta)<1.6) {//Barrel
      if(15.0<pt&&pt<=20.0) return 0.989*0.945;
      if(20.0<pt&&pt<=30.0) return 0.991*1.005;
      if(30.0<pt) return 0.989*0.993;
      return 0;
    }else if(1.6<fabs(eta)&&fabs(eta)<2.1){//Endcap
      if(15.0<pt&&pt<=20.0) return 0.977*1.047;
      if(20.0<pt&&pt<=30.0) return 0.974*0.992;
      if(30.0<pt) return 0.989*1.005;
      return 0;
    }
    return 0;
  }

  //eTau 2012 //Upto ICHEP
// 2012
// pT /  eta bin 	MC Efficiency 	Data Efficiency 	Scale Factor
// ID
// 20 < pT < 30 Barrel 	0.795 ± 0.005 	0.733 ± 0.003 	0.922 ± 0.007
// 20 < pT < 30 Endcap 	0.433 ± 0.009 	0.389 ± 0.006 	0.944 ± 0.026
// pT > 30 Barrel 	0.908 ± 0.001 	0.876 ± 0.001 	0.964 ± 0.001
// pT > 30 Endcap 	0.617 ± 0.004 	0.592 ± 0.010 	0.958 ± 0.007
// Iso
// 20 < pT < 30 Barrel 	0.734 ± 0.006 	0.715 ± 0.001 	0.974 ± 0.008
// 20 < pT < 30 Endcap 	0.740 ± 0.013 	0.745 ± 0.008 	1.008 ± 0.021
// pT > 30 Barrel 	0.896 ± 0.001 	0.893 ± 0.001 	0.997 ± 0.001
// pT > 30 Endcap 	0.912 ± 0.003 	0.896 ± 0.001 	0.983 ± 0.004 

  double effCorrEle2012AB(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(20.0<pt&&pt<=30.0) return 0.922*0.974 ;
      if(30.0<pt) return 0.964*0.997;
      return 0;
    }else if(1.479<fabs(eta)&&fabs(eta)<2.1){//Endcap
      if(20.0<pt&&pt<=30.0) return 0.944*1.008;
      if(30.0<pt) return 0.958*0.983;
      return 0;
    }
    return 0;
  }


  /////------------------HCP: 2012A+B+Cv1+Cv2----------------------------
  //From Rebecca
  double effCorrMuID2012ABC(double pt,double eta){
    if(fabs(eta)<0.8) {
      if(20.0<pt&&pt<=30.0) return 0.9870;
      if(30.0<pt) return 0.9871;
      return 0;
    }else if(fabs(eta)<1.2) {
      if(20.0<pt&&pt<=30.0) return 0.9837;
      if(30.0<pt) return 0.9817;
      return 0;
    }else if(fabs(eta)<2.1){
      if(20.0<pt&&pt<=30.0) return 0.9914;
      if(30.0<pt) return 1.0018;
      return 0;
    }
    return 0;
  }

  double effCorrMuIso2012ABC(double pt,double eta){
    if(fabs(eta)<0.8) {
      if(20.0<pt&&pt<=30.0) return 0.9715;
      if(30.0<pt) return 0.9880;
      return 0;
    }else if(fabs(eta)<1.2) {
      if(20.0<pt&&pt<=30.0) return 0.9826;
      if(30.0<pt) return 0.9870;
      return 0;
    }else if(fabs(eta)<2.1){
      if(20.0<pt&&pt<=30.0) return 0.9960;
      if(30.0<pt) return 1.0008;
      return 0;
    }
    return 0;
  }

  double effCorrEleID2012ABC(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(24.0<pt&&pt<=30.0) return 0.9130;
      if(30.0<pt) return 0.9567;
      return 0;
    }else if(1.479<fabs(eta)&&fabs(eta)<2.1){//Endcap
      if(24.0<pt&&pt<=30.0) return 0.8509;
      if(30.0<pt) return 0.9239;
      return 0;
    }
    return 0;
  }

  double effCorrEleIso2012ABC(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(24.0<pt&&pt<=30.0) return 0.9602;
      if(30.0<pt) return 0.9858;
      return 0;
    }else if(1.479<fabs(eta)&&fabs(eta)<2.1){//Endcap
      if(24.0<pt&&pt<=30.0) return 0.9661;
      if(30.0<pt) return 0.9942;
      return 0;
    }
    return 0;
  }


  ///////////////Moriond Top-up/////////////////////////////////
  double effCorrMuID2012D(double pt,double eta){
    if(fabs(eta)<0.8) {
      if(20.0<pt&&pt<=30.0) return 0.9835;
      if(30.0<pt) return 0.9834;
      return 0;
    }else if(fabs(eta)<1.2) {
      if(20.0<pt&&pt<=30.0) return 0.9789;
      if(30.0<pt) return 0.9785;
      return 0;
    }else if(fabs(eta)<2.1){
      if(20.0<pt&&pt<=30.0) return 0.9876;
      if(30.0<pt) return 0.9884;
      return 0;
    }
    return 0;
  }
  double effCorrMuIso2012D(double pt,double eta){
    if(fabs(eta)<0.8) {
      if(20.0<pt&&pt<=30.0) return 0.9614;
      if(30.0<pt) return 0.9610;
      return 0;
    }else if(fabs(eta)<1.2) {
      if(20.0<pt&&pt<=30.0) return 0.9776;
      if(30.0<pt) return 0.9910;
      return 0;
    }else if(fabs(eta)<2.1){
      if(20.0<pt&&pt<=30.0) return 0.9994;
      if(30.0<pt) return 1.0018;
      return 0;
    }
    return 0;
  }

  double effCorrEleID2012D(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(24.0<pt&&pt<=30.0) return 0.9236;
      if(30.0<pt) return 0.9497;
      return 0;
    }else if(fabs(eta)<2.1){//Endcap
      if(24.0<pt&&pt<=30.0) return 0.8082;
      if(30.0<pt) return 0.9175;
      return 0;
    }
    return 0;
  }
  double effCorrEleIso2012D(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(24.0<pt&&pt<=30.0) return 0.9440;
      if(30.0<pt) return 0.9764;
      return 0;
    }else if(fabs(eta)<2.1){//Endcap
      if(24.0<pt&&pt<=30.0) return 0.9757;
      if(30.0<pt) return 0.9936;
      return 0;
    }
    return 0;
  }


  double effCorrMuID2012ABCD(double pt,double eta){
    if(fabs(eta)<0.8) {
      if(20.0<pt&&pt<=30.0) return 0.9853;
      if(30.0<pt) return 0.9857;
      return 0;
    }else if(fabs(eta)<1.2) {
      if(20.0<pt&&pt<=30.0) return 0.9818;
      if(30.0<pt) return 0.9805;
      return 0;
    }else if(fabs(eta)<2.1){
      if(20.0<pt&&pt<=30.0) return 0.9899;
      if(30.0<pt) return 0.9900;
      return 0;
    }
    return 0;
  }

  double effCorrMuIso2012ABCD(double pt,double eta){
    if(fabs(eta)<0.8) {
      if(20.0<pt&&pt<=30.0) return 0.9685;
      if(30.0<pt) return 0.9872;
      return 0;
    }else if(fabs(eta)<1.2) {
      if(20.0<pt&&pt<=30.0) return 0.9808;
      if(30.0<pt) return 0.9924;
      return 0;
    }else if(fabs(eta)<2.1){
      if(20.0<pt&&pt<=30.0) return 0.9972;
      if(30.0<pt) return 1.0012;
      return 0;
    }
    return 0;
  }

  double effCorrEleID2012ABCD(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(24.0<pt&&pt<=30.0) return 0.9100;
      if(30.0<pt) return 0.9493;
      return 0;
    }else if(fabs(eta)<2.1){//Endcap
      if(24.0<pt&&pt<=30.0) return 0.8244;
      if(30.0<pt) return 0.9260;
      return 0;
    }
    return 0;
  }
  double effCorrEleIso2012ABCD(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(24.0<pt&&pt<=30.0) return 0.9468;
      if(30.0<pt) return 0.9820;
      return 0;
    }else if(fabs(eta)<2.1){//Endcap
      if(24.0<pt&&pt<=30.0) return 0.9586;
      if(30.0<pt) return 0.9948;
      return 0;
    }
    return 0;
  }

  ///////////////Summere13///////////////////
  double effCorrMuID2012ABCDSummer13(double pt,double eta){
    if(fabs(eta)<0.8) {
      if(20.0<pt&&pt<=30.0) return 0.9818;
      if(30.0<pt) return 0.9852;
      return 0;
    }else if(fabs(eta)<1.2) {
      if(20.0<pt&&pt<=30.0) return 0.9829;
      if(30.0<pt) return 0.9852;
      return 0;
    }else if(fabs(eta)<2.1){
      if(20.0<pt&&pt<=30.0) return 0.9869;
      if(30.0<pt) return 0.9884;
      return 0;
    }
    return 0;
  }

  double effCorrMuIso2012ABCDSummer13(double pt,double eta){
    if(fabs(eta)<0.8) {
      if(20.0<pt&&pt<=30.0) return 0.9494;
      if(30.0<pt) return 0.9883;
      return 0;
    }else if(fabs(eta)<1.2) {
      if(20.0<pt&&pt<=30.0) return 0.9835;
      if(30.0<pt) return 0.9937;
      return 0;
    }else if(fabs(eta)<2.1){
      if(20.0<pt&&pt<=30.0) return 0.9923;
      if(30.0<pt) return 0.9996;
      return 0;
    }
    return 0;
  }

  double effCorrEleID2012ABCDSummer13(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(24.0<pt&&pt<=30.0) return 0.8999;
      if(30.0<pt) return 0.9486;
      return 0;
    }else if(fabs(eta)<2.1){//Endcap
      if(24.0<pt&&pt<=30.0) return 0.7945;
      if(30.0<pt) return 0.8866;
      return 0;
    }
    return 0;
  }
  double effCorrEleIso2012ABCDSummer13(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(24.0<pt&&pt<=30.0) return 0.9417;
      if(30.0<pt) return 0.9804;
      return 0;
    }else if(fabs(eta)<2.1){//Endcap
      if(24.0<pt&&pt<=30.0) return 0.9471;
      if(30.0<pt) return 0.9900;
      return 0;
    }
    return 0;
  }


  //this only applies to electrons in the rec-hit embedded sample for e-Tau, computed by Ivo
  // Tight e-Id as in e+tau, Pt>20, |super-cluster eta|<2.1
  Float_t elecEffSFTight(Float_t pt, Float_t eta){

    //Define histogram with weights
    Double_t xAxis1[10] = {10, 15, 20, 25, 30, 40, 55, 70, 100, 200}; 
    Double_t yAxis1[4] = {0, 0.8, 1.479, 2.5}; 
   
    TH2F hPtEtaSF("hPtEtaSF","",9, xAxis1,3, yAxis1);
    hPtEtaSF.SetBinContent(14,0.95);
    hPtEtaSF.SetBinContent(15,0.96);
    hPtEtaSF.SetBinContent(16,0.97);
    hPtEtaSF.SetBinContent(17,0.98);
    hPtEtaSF.SetBinContent(18,1.00);
    hPtEtaSF.SetBinContent(19,0.98);
    hPtEtaSF.SetBinContent(20,0.99);
    hPtEtaSF.SetBinContent(21,1.00);
    hPtEtaSF.SetBinContent(25,0.89);
    hPtEtaSF.SetBinContent(26,0.93);
    hPtEtaSF.SetBinContent(27,0.94);
    hPtEtaSF.SetBinContent(28,0.98);
    hPtEtaSF.SetBinContent(29,0.96);
    hPtEtaSF.SetBinContent(30,0.97);
    hPtEtaSF.SetBinContent(31,0.98);
    hPtEtaSF.SetBinContent(32,0.93);
    hPtEtaSF.SetBinContent(36,0.66);
    hPtEtaSF.SetBinContent(37,0.75);
    hPtEtaSF.SetBinContent(38,0.78);
    hPtEtaSF.SetBinContent(39,0.84);
    hPtEtaSF.SetBinContent(40,0.89);
    hPtEtaSF.SetBinContent(41,0.89);
    hPtEtaSF.SetBinContent(42,0.93);
    hPtEtaSF.SetBinContent(43,1.00);

    if(pt>199.99)
      pt=199.9;
    eta=fabs(eta);
    if(eta>2.49)
      eta=2.49;

    if(pt<20)
      return 0;

    Float_t eff=0;
    Int_t bin = hPtEtaSF.FindFixBin(pt,eta);
    eff = hPtEtaSF.GetBinContent(bin);

    return eff;
  }





private:
  
} ;
#endif 


