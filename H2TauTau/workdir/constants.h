
/////SM
#define NMASS 12
long massValues[NMASS]={90,95,100,105,110,115,120,125,130,135,140,145};

///Crossections taken from here:
///https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt8TeV
///Higgs --> tau tau branching ratios taken from http://arxiv.org/abs/1101.0593 v3
Float_t HiggsTauTauBF[NMASS]={1,1,1,1,8.03e-2, 7.65e-2,  7.11e-2,  6.37e-2,  5.49e-2,  4.52e-2, 3.54e-2,2.62e-2};
Float_t HiggsGGcross[NMASS]={1,1,1,1,25.04,22.96,21.13,19.52,18.07,16.79,15.63,14.59};
Float_t HiggsVBFcross[NMASS]={1,1,1,1,1.809,1.729,1.649,1.578,1.511,1.448,1.389,1.333 };
Float_t HiggsWHcross[NMASS]= {1,1,1,1,1.0600,0.9165,0.7966,0.6966,0.6095,0.5351,0.47130,0.41640};
Float_t HiggsZHcross[NMASS]= {1,1,1,1,0.5869,0.5117,0.4483,0.3943,0.3473,0.3074,0.27280,0.24240};
Float_t HiggsttHcross[NMASS]={1,1,1,1,0.1887,0.1663,0.1470,0.1302,0.1157,0.1031,0.09207,0.08246 };

#define NCAT 5
TString catdirname[NCAT]={
 "0jet_low",
 "0jet_high",
 "boost_low",
 "boost_high",
 "vbf"};

#define NXBINS 26
Float_t xbinsValues[NXBINS+1]={0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350};
#define NXBINSVBF 13
Float_t xbinsValuesVBF[NXBINSVBF+1]={0,20,40,60,80,100,120,140,160,180,200,250,300,350};

/////SUSY
#define NSUSY 16
//Int_t SUSYMass[NSUSY]={90,100,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000};  
Int_t  SUSYMass[NSUSY]={90,100,110,120,130,140,160,180,250,300,400,450,500,600,800,1000};  

