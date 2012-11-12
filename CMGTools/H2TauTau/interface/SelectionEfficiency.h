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
  // 2012 53X
  // pT / eta bin 	                  MC Efficiency 	Data Efficiency 	Scale Factor 

  //From Rebecca
// 20 < pT < 30, eta < 0.8      	0.9649 ± 0.0004 	0.9524 ± 0.0005 	0.9870 ± 0.0006
// 20 < pT < 30, 0.8 < eta < 1.2 	0.9586 ± 0.0016 	0.9430 ± 0.0008 	0.9837 ± 0.0018
// 20 < pT < 30, 1.2 < eta < 2.1 	0.9512 ± 0.0005 	0.9430 ± 0.0005 	0.9914 ± 0.0007
// pT > 30, eta < 0.8          	        0.9632 ± 0.0001 	0.9508 ± 0.0001 	0.9871 ± 0.0001
// pT > 30, 0.8 < eta < 1.2      	0.9581 ± 0.0002 	0.9406 ± 0.0001 	0.9817 ± 0.0001
// pT > 30, 1.2 < eta < 2.1     	0.9491 ± 0.0001 	0.9508 ± 0.0001 	1.0018 ± 0.0001
// Iso
// 20 < pT < 30, eta < 0.8 	        0.7866 ± 0.0010 	0.7642 ± 0.0009 	0.9715 ± 0.0017
// 20 < pT < 30, 0.8 < eta < 1.2 	0.8222 ± 0.0012 	0.8080 ± 0.0013 	0.9826 ± 0.0022
// 20 < pT < 30, 1.2 < eta < 2.1 	0.8531 ± 0.0008 	0.8497 ± 0.0008 	0.9960 ± 0.0013
// pT > 30, eta < 0.8           	0.9151 ± 0.0002 	0.9041 ± 0.0003 	0.9880 ± 0.0004
// pT > 30, 0.8 < eta < 1.2     	0.9361 ± 0.0003 	0.9239 ± 0.0002 	0.9870 ± 0.0004
// pT > 30, 1.2 < eta < 2.1     	0.9389 ± 0.0003 	0.9397 ± 0.0002 	1.0008 ± 0.0004

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



  ///factors from Rebecca
// pT / eta bin 	MC Efficiency 	Data Efficiency 	Scale Factor
// ID
// 24 < pT < 30 Barrel 	0.8249 ± 0.0011 	0.7532 ± 0.0015 	0.9130 ± 0.0022
// 24 < pT < 30 Endcap 	0.4267 ± 0.0038 	0.3631 ± 0.0022 	0.8509 ± 0.0092
// pT > 30 Barrel 	0.8975 ± 0.0002 	0.8587 ± 0.0001 	0.9567 ± 0.0002
// pT > 30 Endcap 	0.6074 ± 0.0001 	0.5611 ± 0.0001 	0.9239 ± 0.0001
// Iso
// 24 < pT < 30 Barrel 	0.7806 ± 0.0011 	0.7496 ± 0.0012 	0.9602 ± 0.0021
// 24 < pT < 30 Endcap 	0.8533 ± 0.0023 	0.8244 ± 0.0030 	0.9661 ± 0.0044
// pT > 30 Barrel 	0.9084 ± 0.0002 	0.8956 ± 0.0001 	0.9858 ± 0.0002
// pT > 30 Endcap 	0.9240 ± 0.0001 	0.9187 ± 0.0003 	0.9942 ± 0.0003

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


private:
  
} ;
#endif 


