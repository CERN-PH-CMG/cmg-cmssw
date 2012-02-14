#ifndef CMGTools_H2TauTau_TriggerEfficiency_H
#define CMGTools_H2TauTau_TriggerEfficiency_H

#include <math.h> 
#include "TMath.h" 
#include <limits>


class TriggerEfficiency {
public:
  TriggerEfficiency(){} ;
  

  //trigger lumi weighting done according to this table from:
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/HToTauTauPlusTwoJets
  // HLT_IsoMu12_LooseIsoPFTau10_v4 	        163269 - 163869 	168.6 	L1_SingleMu10 	 
  // HLT_IsoMu15_LooseIsoPFTau15_v2 	        165088 - 165633 	139.0 	L1_SingleMu10 	 
  // HLT_IsoMu15_LooseIsoPFTau15_v4 	        165970 - 167043 	545.1 	L1_SingleMu10 	w/o run 166346
  // HLT_IsoMu15_LooseIsoPFTau15_v5 	        166346          	4.3 	L1_SingleMu10 	one run only
  // HLT_IsoMu15_LooseIsoPFTau15_v6 	        167078 - 167913 	245.6 	L1_SingleMu10 	 
  // HLT_IsoMu15_LooseIsoPFTau15_v8 	        170249 - 173198 	785.7 	L1_SingleMu10 	 
  // HLT_IsoMu15_LooseIsoPFTau15_v9 	        173236 - 178380 	1945 	L1_SingleMu10 	 (Note! this trigger got prescaled in 2011B)    6.95  ?? 
  // and in 2011A, it seems off for the first part of the spill..
  // HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1 	173236 - 178380 	1945 	L1_SingleMu14_Eta2p1 	ET(tau)>20 GeV, |eta(mu)|<2.1   1.692
  // HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5 	178420 - 179889 	706.7 	L1_SingleMu14_Eta2p1 	ET(tau)>20 GeV, |eta(mu)|<2.1   0.695
  // HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6 	179959 - 180252 	120.7 	L1_SingleMu14_Eta2p1 	end of 2011 run                 0.118

  
  //Below, measurement from Colin (we're now using these numbers to weight our efficiencies)
 
  //                                            2011A                        2011B
  // HLT_IsoMu12_LooseIsoPFTau10_v4 	        168.6
  // HLT_IsoMu15_LooseIsoPFTau15_v2 	        139.1
  // HLT_IsoMu15_LooseIsoPFTau15_v4 	        543.3
  // HLT_IsoMu15_LooseIsoPFTau15_v5 	          4.3
  // HLT_IsoMu15_LooseIsoPFTau15_v6 	        243.1
  // HLT_IsoMu15_LooseIsoPFTau15_v8 	        780.4 (368.04 + 412.36) 
  // HLT_IsoMu15_LooseIsoPFTau15_v9 	        246.527                      
  // HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1                                   1698           
  // HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5                                    694.8     
  // HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6                                    117.6     


  double effTau2011A(double pt, double eta){
    float tau10w = 168.6;
    float tau15w = 139.1 + 543.3 + 4.3 + 243.1 + 780.4 + 246.527;
    return ( tau10w * effLooseTau10(pt,eta) + 
	     tau15w * effLooseTau15(pt,eta) ) / ( tau10w + tau15w);
      
    /*     return ((168.6)*effLooseTau10(pt,eta) */
    /* 	    //COLIN I think the 246.5 should be multiplied by effLooseTau20? or maybe not...  */
    /* 	    +(139.0+545.1+4.3+245.6+785.7+246.5)*effLooseTau15(pt,eta))/(168.6+139.0+545.1+4.3+245.6+785.7+246.527); */
    //last number 246.5 obtained from runs 172620-->173692 in Oct3ReReco(=PromtReco-v6)
  }
  

  double effTau2011B(double pt, double eta){
    return effLooseTau20(pt,eta);

      /*     return ((0.1)*effLooseTau15(pt,eta) */
      /* 	    +((1945-246.5-0.1)+706.7+120.7)*effLooseTau20(pt,eta))/(0.1+1945-246.5-0.1+706.7+120.7); */
      /*     //first number 0.1 is an approximation because this trigger got prescaled and most of data is actually with LooseTau20 */
  }

  double effTau2011AB(double pt, double eta){
    float tau10w = 168.6;
    float tau15w = 139.1 + 543.3 + 4.3 + 243.1 + 780.4 + 246.527;
    float tau20w = 1698 + 694.8 + 117.6;
    return ( tau10w * effLooseTau10(pt,eta) + 
	     tau15w * effLooseTau15(pt,eta) +
	     tau20w * effLooseTau20(pt,eta) ) / ( tau10w + tau15w + tau20w );

    /*     return ((168.6)*effLooseTau10(pt,eta) */
    /* 	    +(139.0+545.1+4.3+245.6+785.7+246.5+0.1)*effLooseTau15(pt,eta) */
    /* 	    +((1945-246.5-0.1)+706.7+120.7)*effLooseTau20(pt,eta))/(168.6+139.0+545.1+4.3+245.6+785.7+246.5+0.1+(1945-246.5-0.1)+706.7+120.7); */
  }
  
  double effMu2011A(double pt, double eta){
    float mu12w = 168.6;
    float mu15w = 139.1 + 543.3 + 4.3 + 243.1 + 780.4 + 246.527;
    return ( mu12w * effIsoMu12(pt,eta) + 
	     mu15w * effIsoMu15(pt,eta) ) / ( mu12w + mu15w);

/*     return ((168.6)*effIsoMu12(pt,eta) */
/* 	    +(139.0+545.1+4.3+245.6+785.7+246.527)*effIsoMu15(pt,eta))/(168.6+139.0+545.1+4.3+245.6+785.7+246.527); */
  }

  double effMu2011B(double pt, double eta){
    return effIsoMu15eta2p1(pt,eta);
/*     return ((1698)*effIsoMu15(pt,eta) */
/* 	    +(706.7+120.7)*effIsoMu15eta2p1(pt,eta))/(1698+706.7+120.7); */
  }

  double effMu2011AB(double pt, double eta){
    float mu12w = 168.6;
    float mu15w = 139.1 + 543.3 + 4.3 + 243.1 + 780.4 + 246.527;
    float mu15eta2p1 = 1698 + 694.8 + 117.6;
    return ( mu12w * effIsoMu12(pt,eta) + 
	     mu15w * effIsoMu15(pt,eta) + 
	     mu15eta2p1 * effIsoMu15eta2p1(pt,eta) ) / ( mu12w + mu15w + mu15eta2p1);

/*     return ((168.6)*effIsoMu12(pt,eta) */
/* 	    +(139.0+545.1+4.3+245.6+785.7+1945.)*effIsoMu15(pt,eta) */
/* 	    +(706.7+120.7)*effIsoMu15eta2p1(pt,eta))/(168.6+139.0+545.1+4.3+245.6+785.7+1945+706.7+120.7); */
  }

  /* HLT_Ele15_ _LooseIsoPFTau15_v1 	160404 - 161176 	6.7 	L1_SingleEG12 	buggy, lower eff */
  /* HLT_Ele15_ _LooseIsoPFTau15_v2 	161216 - 163261 	40.9 	L1_SingleEG12 	buggy, lower eff */
  /* HLT_Ele15_ _LooseIsoPFTau15_v4 	163269 - 163869 	168.6 	L1_SingleEG12 	  */
  /* HLT_Ele15_ _LooseIsoPFTau20_v6 	165088 - 165633 	139.0 	L1_SingleEG12 	  */
  /* HLT_Ele15_ _LooseIsoPFTau20_v8 	165970 - 166967 	526.7 	L1_SingleEG12 	  */
  /* HLT_Ele15_ _LooseIsoPFTau20_v9 	167039 - 167913 	268.3 	L1_SingleEG12 	  */
  /* HLT_Ele15_ _TightIsoPFTau20_v2 	170249 - 173198 	785.7 	L1_SingleEG12 	tight iso */
  /* HLT_Ele18_ _MediumIsoPFTau20_v1 	173236 - 178380 	1945 	L1_SingleEG15 	medium iso, ET(e)>18 GeV */
  /* HLT_Ele20_ _MediumIsoPFTau20_v5 	178420 - 179889 	706.7 	L1_SingleEG18 OR EG20 	medium iso, ET(e)>20 GeV */
  /* HLT_Ele20_ _MediumIsoPFTau20_v6 	179959 - 180252 	120.7 	L1_SingleEG18 OR EG20 	end of 2011 run  */

  //                                       2011A                                       2011B
  // HLT_Ele15_ _LooseIsoPFTau15_v1        not used - bugged	
  // HLT_Ele15_ _LooseIsoPFTau15_v2 	   not used - bugged
  // HLT_Ele15_ _LooseIsoPFTau15_v4 	   168.6    (May10ReReco)
  // HLT_Ele15_ _LooseIsoPFTau20_v6 	   139.1    (V4)
  // HLT_Ele15_ _LooseIsoPFTau20_v8 	   524.9    (V4)
  // HLT_Ele15_ _LooseIsoPFTau20_v9 	   265.7    (V4)
  // HLT_Ele15_ _TightIsoPFTau20_v2 	   368.04   (Aug5) + 412.36 (Oct3) = 780.4
  // HLT_Ele18_ _MediumIsoPFTau20_v1 	   246.527                                    1698
  // HLT_Ele20_ _MediumIsoPFTau20_v5 	                                               694.8
  // HLT_Ele20_ _MediumIsoPFTau20_v6 	                                               117.6


  double effEle2011A( double pt, double eta) {
    double ele15Weight = 168.6 + 139.1 + 524.9 + 265.7 + 780.4;
    double ele18Weight = 246.5; // warning overlap!
    return (ele15Weight * effEle15(pt, eta) + 
	    ele18Weight * effEle18(pt,eta)) / (ele15Weight+ele18Weight);
  }

  double effEle2011B( double pt, double eta) {
    double ele18Weight = 1698; 
    double ele20Weight = 694.8 + 117.6;
    return ( ele18Weight * effEle18(pt,eta) + 
	     ele20Weight * effEle20(pt,eta)) / (ele18Weight + ele20Weight);
  } 

  double effEle2011AB( double pt, double eta) {
    double ele15Weight = 168.6 + 139.1 + 524.9 + 265.7 + 780.4;
    double ele18Weight = 246.5 + 1698; 
    double ele20Weight = 694.8 + 117.6; 
    return ( ele15Weight * effEle15(pt, eta) + 
	     ele18Weight * effEle18(pt,eta) + 
	     ele20Weight * effEle20(pt,eta)) / (ele15Weight + ele18Weight + ele20Weight);    
  }

  double effTau2011A_TauEle( double pt, double eta) {
    double tau15Weight = 168.6;
    double tau20Weight = 139.1 + 524.9 + 265.7;
    double tightIsoTau20Weight = 780.4;
    double mediumIsoTau20Weight = 246.5; 

    return (tau15Weight * effLooseTau15(pt, eta) + 
	    tau20Weight * effLooseTau20(pt,eta) + 
	    tightIsoTau20Weight * effTightIsoTau20(pt,eta) + 
	    mediumIsoTau20Weight * effMediumIsoTau20(pt,eta)) 
      / ( tau15Weight + tau20Weight + tightIsoTau20Weight + mediumIsoTau20Weight);
  }

  double effTau2011B_TauEle( double pt, double eta) {
    return effMediumIsoTau20(pt,eta);
  }

  double effTau2011AB_TauEle( double pt, double eta) {
    double tau15Weight = 168.6;
    double tau20Weight = 139.1 + 524.9 + 265.7;
    double tightIsoTau20Weight = 780.4;
    double mediumIsoTau20Weight = 246.5 + 1698 + 694.8 + 117.6;

    return (tau15Weight * effLooseTau15(pt, eta) + 
	    tau20Weight * effLooseTau20(pt,eta) + 
	    tightIsoTau20Weight * effTightIsoTau20(pt,eta) + 
	    mediumIsoTau20Weight * effMediumIsoTau20(pt,eta)) 
      / ( tau15Weight + tau20Weight + tightIsoTau20Weight + mediumIsoTau20Weight);
  }


  //****************
  //parameters taken from AN-11-390 v8
  //*****************
  double effLooseTau10(double pt, double eta){
    if(fabs(eta)<1.479) efficiency(pt,13.6046,1.66291,1.71551,141.929,0.910686);
    return efficiency(pt,-0.392211,7.90467,5.48228,134.599,0.925858);
  }
  double effLooseTau15(double pt, double eta){
    if(fabs(eta)<1.479) efficiency(pt,13.9694,0.084835,0.057743,1.50674,0.984976);
    return efficiency(pt,14.435,1.34952,2.43996,1.03631,1.79081);
  }
  double effLooseTau20(double pt, double eta){
    if(fabs(eta)<1.479) efficiency(pt,19.2102,1.26519,2.48994,1.04699,1.3492);
    return efficiency(pt,19.2438,1.37298,1.76448,1.73935,0.901291);
  }

  double effLooseTau15MC(double pt, double eta){//should correspond to Fall11 MC
    if(fabs(eta)<1.479) efficiency(pt,14.4601,0.0485272,0.03849,1.48324,0.965257);
    return efficiency(pt,14.4451,0.0790573,0.0732472,1.47046,0.942028);
  }


  double effTightIsoTau20(double pt, double eta){
    if(fabs(eta)<1.479) efficiency(pt, 19.6013, 0.987317, 1.08015, 1.88592, 0.776894);
    return efficiency(pt, 18.8859, 0.271301, 0.128008, 1.50993, 0.825122);
  }
  double effMediumIsoTau20(double pt, double eta){
    if(fabs(eta)<1.479) efficiency(pt, 19.5667, 1.15203, 1.68126, 1.40025, 0.848033);
    return efficiency(pt, 18.8476, 0.528963, 0.16717, 3.65814, 0.749759);
  }


  //****************
  //parameters taken from AN-11-390 v8
  //*****************
  double effIsoMu12(double pt, double eta){
    if(fabs(eta)<1.479)return 0.901; //Barrel: eta < 1.479 
    return 0.863; //EndCap
  }
  double effIsoMu15(double pt, double eta){
    if(fabs(eta)<1.479)return 0.901; 
    return 0.863;
  }
  double effIsoMu15eta2p1(double pt, double eta){
    if(fabs(eta)<1.479)return efficiency(pt,15.06,0.55278,1.34236,1.003,3.36767);
    return efficiency(pt,15.03175,0.866114,1.25009,1.63711,0.844906);
  }
  double effIsoMu15MC(double pt, double eta){//should correspond to Fall11 MC
    if(fabs(eta)<1.479)return 0.917;
    return 0.836;
  }


  //****************
  //parameters taken from AN-11-390 v8
  //*****************

  double effEle15(double pt, double eta) {
    if(fabs(eta)<1.479) 
      return efficiency(pt, 14.8772, 0.311255, 0.221021, 1.87734, 0.986665);
    else
      return efficiency(pt, 15.6629, 0.759192, 0.47756, 2.02154, 0.998816);
  }

  double effEle18(double pt, double eta) {
    if(fabs(eta)<1.479) 
      return efficiency(pt, 18.3193, 0.443703, 0.385554, 1.86523, 0.986514 );
    else
      return efficiency(pt, 19.6586, 0.682633, 0.279486, 2.66423, 0.973455 );
  }

  double effEle20(double pt, double eta) {
    if(fabs(eta)<1.479) 
      return efficiency(pt, 20.554, 0.683776, 0.855573, 1.45917, 1.03957 );
    else
      return efficiency(pt, 23.6386, 1.60775, 1.72093, 1.4131, 1.13962  );
  }

  double effEle18MC(double pt, double eta) {
    if(fabs(eta)<1.479) 
      return efficiency(pt, 15.1804, 2.43126, 3.85048, 1.72284, 0.998507 );
    else
      return efficiency(pt, 16.993, 0.0693958, 0.00695096, 1.9566, 1.00632 );
  }



private:

  //function definition taken from AN-11-390 v4
  double efficiency(double m, double m0, double sigma, double alpha,double n, double norm);
} ;
#endif 


