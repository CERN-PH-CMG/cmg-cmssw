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
  // pT / eta bin 	MC Efficiency 	Data Efficiency 	Scale Factor 
  /////-------------muTau 2012
  // 15 < pT < 20 Barrel 	0.9610 ± 0.0008 	0.9480 ± 0.0010 	0.9864 ± 0.0013
  // 15 < pT < 20 Endcap 	0.9473 ± 0.0013 	0.9341 ± 0.0017 	0.9861 ± 0.0023
  // 20 < pT < 30 Barrel 	0.9617 ± 0.0003 	0.9510 ± 0.0003 	0.9889 ± 0.0004
  // 20 < pT < 30 Endcap 	0.9454 ± 0.0006 	0.9324 ± 0.0006 	0.9862 ± 0.0009
  // pT > 30 Barrel 	0.9610 ± 0.0001 	0.9457 ± 0.0001 	0.9841 ± 0.0001
  // pT > 30 Endcap 	0.9397 ± 0.0001 	0.9291 ± 0.0001 	0.9887 ± 0.0001
  // Iso
  // 15 < pT < 20 Barrel 	0.7150 ± 0.0020 	0.6875 ± 0.0020 	0.9615 ± 0.0038
  // 15 < pT < 20 Endcap 	0.7730 ± 0.0026 	0.7681 ± 0.0029 	0.9936 ± 0.0051
  // 20 < pT < 30 Barrel 	0.8030 ± 0.0006 	0.7945 ± 0.0007 	0.9894 ± 0.0011
  // 20 < pT < 30 Endcap 	0.8574 ± 0.0010 	0.8589 ± 0.0011 	1.0018 ± 0.0018
  // pT > 30 Barrel 	0.9228 ± 0.0001 	0.9153 ± 0.0001 	0.9918 ± 0.0002
  // pT > 30 Endcap 	0.9398 ± 0.0003 	0.9435 ± 0.0003 	1.0040 ± 0.0004
  
  double effCorrMu2012ABC(double pt,double eta){
    if(fabs(eta)<1.6) {//Barrel
      if(15.0<pt&&pt<=20.0) return 0.9864*0.9615;
      if(20.0<pt&&pt<=30.0) return 0.9889*0.9894;
      if(30.0<pt) return 0.9841*0.9918;
      return 0;
    }else if(1.6<fabs(eta)&&fabs(eta)<2.1){//Endcap
      if(15.0<pt&&pt<=20.0) return 0.9861*0.9936;
      if(20.0<pt&&pt<=30.0) return 0.9862*1.0018;
      if(30.0<pt) return 0.9887*1.0040;
      return 0;
    }
    return 0;
  }

//   pT / eta bin	 MC Efficiency	 Data Efficiency	 Scale Factor
// ID
// 20 < pT < 30 Barrel	 0.8097 ± 0.0030	 0.7389 ± 0.0015	 0.9126 ± 0.0037
// 20 < pT < 30 Endcap	 0.4023 ± 0.0017	 0.3423 ± 0.0020	 0.8507 ± 0.0062
// pT > 30 Barrel	 0.8975 ± 0.0002	 0.8587 ± 0.0001	 0.9567 ± 0.0002
// pT > 30 Endcap	 0.6074 ± 0.0001	 0.5611 ± 0.0001	 0.9239 ± 0.0001
// Iso
// 20 < pT < 30 Barrel	 0.7683 ± 0.0010	 0.7376 ± 0.0011	 0.9600 ± 0.0019
// 20 < pT < 30 Endcap	 0.8439 ± 0.0018	 0.8167 ± 0.0032	 0.9677 ± 0.0043
// pT > 30 Barrel	 0.9084 ± 0.0002	 0.8956 ± 0.0001	 0.9858 ± 0.0002
// pT > 30 Endcap	 0.9240 ± 0.0001	 0.9187 ± 0.0003	 0.9942 ± 0.0003
  double effCorrEle2012ABC(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(20.0<pt&&pt<=30.0) return 0.9126*0.9600;
      if(30.0<pt) return 0.9567*0.9858;
      return 0;
    }else if(1.479<fabs(eta)&&fabs(eta)<2.1){//Endcap
      if(20.0<pt&&pt<=30.0) return 0.8507*0.9677;
      if(30.0<pt) return 0.9239*0.9942;
      return 0;
    }
    return 0;
  }


private:
  
} ;
#endif 


