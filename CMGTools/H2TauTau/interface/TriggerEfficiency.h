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
  // HLT_IsoMu12_LooseIsoPFTau10_v4 	163269 - 163869 	168.6 	L1_SingleMu10 	 
  // HLT_IsoMu15_LooseIsoPFTau15_v2 	165088 - 165633 	139.0 	L1_SingleMu10 	 
  // HLT_IsoMu15_LooseIsoPFTau15_v4 	165970 - 167043 	545.1 	L1_SingleMu10 	w/o run 166346
  // HLT_IsoMu15_LooseIsoPFTau15_v5 	166346          	4.3 	L1_SingleMu10 	one run only
  // HLT_IsoMu15_LooseIsoPFTau15_v6 	167078 - 167913 	245.6 	L1_SingleMu10 	 
  // HLT_IsoMu15_LooseIsoPFTau15_v8 	170249 - 173198 	785.7 	L1_SingleMu10 	 
  // HLT_IsoMu15_LooseIsoPFTau15_v9 	173236 - 178380 	1945 	L1_SingleMu10 	 (Note! this trigger got prescaled in 2011B)
  // HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5 	178420 - 179889 	706.7 	L1_SingleMu14_Eta2p1 	ET(tau)>20 GeV, |eta(mu)|<2.1
  // HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6 	179959 - 180252 	120.7 	L1_SingleMu14_Eta2p1 	end of 2011 run 


  double effTau2011A(double pt, double eta){
    return ((168.6)*effLooseTau10(pt,eta)
	    +(139.0+545.1+4.3+245.6+785.7+246.5)*effLooseTau15(pt,eta))/(168.6+139.0+545.1+4.3+245.6+785.7+246.527);
    //last number 246.5 obtained from runs 172620-->173692 in Oct3ReReco(=PromtReco-v6)
  }
  double effTau2011B(double pt, double eta){
    return ((0.1)*effLooseTau15(pt,eta)
	    +((1945-246.5-0.1)+706.7+120.7)*effLooseTau20(pt,eta))/(0.1+1945-246.5-0.1+706.7+120.7);
    //first number 0.1 is an approximation because this trigger got prescaled and most of data is actually with LooseTau20
  }
  double effTau2011AB(double pt, double eta){
    return ((168.6)*effLooseTau10(pt,eta)
	    +(139.0+545.1+4.3+245.6+785.7+246.5+0.1)*effLooseTau15(pt,eta)
	    +((1945-246.5-0.1)+706.7+120.7)*effLooseTau20(pt,eta))/(168.6+139.0+545.1+4.3+245.6+785.7+246.5+0.1+(1945-246.5-0.1)+706.7+120.7);
  }
  
  double effMu2011A(double pt, double eta){
    return ((168.6)*effIsoMu12(pt,eta)
	    +(139.0+545.1+4.3+245.6+785.7+246.527)*effIsoMu15(pt,eta))/(168.6+139.0+545.1+4.3+245.6+785.7+246.527);
  }
  double effMu2011B(double pt, double eta){
    return ((1698)*effIsoMu15(pt,eta)
	    +(706.7+120.7)*effIsoMu15eta2p1(pt,eta))/(1698+706.7+120.7);
  }
  double effMu2011AB(double pt, double eta){
    return ((168.6)*effIsoMu12(pt,eta)
	    +(139.0+545.1+4.3+245.6+785.7+1945.)*effIsoMu15(pt,eta)
	    +(706.7+120.7)*effIsoMu15eta2p1(pt,eta))/(168.6+139.0+545.1+4.3+245.6+785.7+1945+706.7+120.7);
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

private:

  //function definition taken from AN-11-390 v4
  double efficiency(double m, double m0, double sigma, double alpha,double n, double norm);
} ;
#endif 


