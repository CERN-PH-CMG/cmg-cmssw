#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include "TauElePlotter.h"
#include "configTauEle2011.C"

void plotTauEle2011(){  

  //TString path="/data/benitezj/Samples/TauEleV541June29_FlatNtpJune29";
  //TString path="/afs/cern.ch/user/b/benitezj/output/TauEle2011FlatNtpJuly23";
  //TString path="/data/benitezj/Samples/TauEle2011FlatNtpJuly25";
  //TString path="/data/benitezj/Samples/TauEle2011FlatNtpJuly26";
  TString path="/data/benitezj/Samples/TauEle2011FlatNtpJuly26svfit";

  TauElePlotter*analysis=configTauEle2011("analysis",path);
  analysis->setQCDColor(kMagenta-10);
  analysis->setWJetsColor(kRed+2);
  analysis->setTTJetsColor(kBlue-8);
  analysis->setZMuMuColor(kBlue);
  analysis->setZTauTauColor(kOrange-4);
  analysis->setPlotTitle("CMS Preliminary 2011,   4.9 fb^{-1},     #sqrt{s} = 7 TeV,        #tau_{e}#tau_{h}");
  analysis->setZTTType(2);

  //raw yields to load samples now
  //analysis->printRawYields("(categoryIso==1&&abs(ditaucharge)==0)");

  //
  Float_t c[4]={.6,.94,.52,.87};//default legend coordinates
  Float_t c2[4]={.24,.47,.54,.9};//on the right side
  Float_t xbinsValues[14]={0,20,40,60,80,100,120,140,160,180,200,250,300,350};
  analysis->setVariableBinning(13,xbinsValues);


  analysis->plotInc("nvtx",50,-.5,49.5,1,1,1,"","",0,0,"# of reco. vertices ","",c,0,"inclusive");

  analysis->plotInc("mupt",50,0,100,1,1,1,"","",0,0," electron p_{T}   (GeV)","",c,0,"inclusive");
  analysis->plotInc("mueta",25,-2.5,2.5,1,1,1,"","",0,0," electron #eta","",c,0,"inclusive"); 
  analysis->plotInc("muiso",100,0,1,1,0,1,"(tauisodiscmva>=1)","",0,0," electron relIso","",c,0,"inclusive");
  analysis->plotInc("mudz",100,-.05,.05,1,1,1,"","",0,0," electron dz","",c,0,"inclusive");
  analysis->plotInc("mudxy",100,-.03,.03,1,1,1,"","",0,0," electron dxy","",c,0,"inclusive");

  analysis->plotInc("taupt",50,0,100,1,1,1,"","",0,0," tau p_{T}   (GeV)","",c,0,"inclusive");
  analysis->plotInc("taueta",25,-2.5,2.5,1,1,1,"","",0,0," tau  #eta","",c2,0,"inclusive"); 
  analysis->plotInc("tauisomva",100,-1.0,1.01,1,0,1,"(muiso<0.1)","",0,0,"tau iso mva","",c,0,"inclusive");
  analysis->plotInc("taudz",100,-.05,.05,1,1,1,"","",0,0," tau dz","",c,0,"inclusive");
  analysis->plotInc("taudxy",100,-.03,.03,1,1,1,"","",0,0," tau dxy","",c,0,"inclusive");

  analysis->plotInc("metpt",50,0,150,1,1,1,"","",0,0," MET   (GeV)","",c,0,"inclusive");    
  analysis->plotInc("metphi",35,-3.5,3.5,1,1,1,"","",0,0," MET  phi ","",c,0,"inclusive");    

  analysis->plotInc("transversemass",20,0,200,1,1,0,"","",0,0,"M_{T}   [GeV]","",c,0,"inclusive");
  analysis->plotInc("ditaumass",50,0,200,1,1,1,"","",0,0,"m_{vis}   (GeV)","",c,0,"inclusive")
  analysis->plotInc("svfitmass",60,0,300,1,1,1,"","",0,0," m(#tau#tau)   (GeV)","",c,0,"inclusive");
  analysis->plotInc("njet",5,-.5,4.5,1,1,1,"","",0,0," njet ","",c,0,"inclusive");
  
  analysis->plotInc("transversemass",20,0,200,1,1,0,"(metpt>30)","",0,0,"M_{T}   [GeV]","",c,0,"inclusiveHighMET");
  analysis->plotInc("ditaumass",50,0,200,1,1,1,"(metpt>30)","",0,0,"m_{vis}   (GeV)","",c,0,"inclusiveHighMET");
  analysis->plotInc("svfitmass",60,0,300,1,1,1,"(metpt>30)","",0,0," m(#tau#tau)   (GeV)","",c,0,"inclusiveHighMET");

  analysis->plotInc("transversemass",20,0,200,1,1,0,"(njet>=1)","",0,0,"m_{T}   (GeV)","",c,0,"inclusive1Jet");
  analysis->plotInc("leadJetPt",20,0,200,1,1,1,"(njet>=1)","",0,0,"lead jet p_{T}   (GeV)","",c,0,"inclusive1Jet");
  analysis->plotInc("leadJetEta",20,-5,5,1,1,1,"(njet>=1)","",0,0,"lead jet #eta","",c,0,"inclusive1Jet");
  analysis->plotInc("svfitmass",60,0,300,1,1,1,"(njet>=1)","",0,0," m(#tau#tau)   (GeV)","",c,0,"inclusive1Jet");

  analysis->plotInc("transversemass",30,0,300,1,1,0,"(njet>=2)","",3,12,"m_{T}   (GeV)","",c,0,"inclusive2Jet");
  analysis->plotInc("leadJetPt",20,0,200,1,1,1,"(njet>=2)","",3,12,"lead jet p_{T}   (GeV)","",c,0,"inclusive2Jet");
  analysis->plotInc("leadJetEta",20,-5,5,1,1,1,"(njet>=2)","",3,12,"lead jet #eta","",c,0,"inclusive2Jet");
  analysis->plotInc("subleadJetPt",20,0,200,1,1,1,"(njet>=2)","",3,12,"sublead jet p_{T}   (GeV)","",c,0,"inclusive2Jet");
  analysis->plotInc("subleadJetEta",20,-5,5,1,1,1,"(njet>=2)","",3,12,"sublead jet #eta","",c,0,"inclusive2Jet"); 
  analysis->plotInc("diJetMass",20,0,1000,1,1,1,"(njet>=2)","",3,12,"M(jj)","",c,0,"inclusive2Jet"); 
  analysis->plotInc("abs(diJetDeltaEta)",20,0,10,1,1,1,"(njet>=2)","",3,12,"#Delta#eta","",c,0,"inclusive2Jet"); 
  analysis->plotInc("svfitmass",18,0,360,1,1,1,"","",3,12," m(#tau#tau)   (GeV)","",c,0,"inclusive2Jet");

  analysis->plotInc("vbfvars0",20,0,1000,1,1,1,"(njet>=2)","",3,12,"M(jj)","",c,0,"inclusive2Jet");
  analysis->plotInc("vbfvars1",20,0,10,1,1,1,"(njet>=2)","",3,12,"#Delta#eta(jj)","",c,0,"inclusive2Jet");
  analysis->plotInc("vbfvars2",20,0,3.4,1,1,1,"(njet>=2)","",3,12,"#Delta#phi(jj)","",c2,0,"inclusive2Jet");
  analysis->plotInc("vbfvars3",20,0,200,1,1,1,"(njet>=2)","",3,12,"#tau#tau   p_{T}","",c,0,"inclusive2Jet");
  analysis->plotInc("vbfvars4",20,0,200,1,1,1,"(njet>=2)","",3,12,"di-jet p_{T}","",c,0,"inclusive2Jet");
  analysis->plotInc("vbfvars5",20,0,3.4,1,1,1,"(njet>=2)","",3,12,"#Delta#phi(#tau#tau-jj)","",c2,0,"inclusive2Jet");
  analysis->plotInc("vbfvars6",20,0,10,1,1,1,"(njet>=2)","",3,12,"#Delta#eta(#mu#tau-j)","",c,0,"inclusive2Jet");
  analysis->plotInc("vbfvars7",20,0,200,1,1,1,"(njet>=2)","",3,12,"#mu#tau p_{T}","",c,0,"inclusive2Jet");
  analysis->plotInc("vbfmva",20,-1.,1.001,1,1,1,"(njet>=2)","(vbfmva<0.5)",3,12,"VBF MVA output","",c,0,"inclusive2Jet");
  analysis->plotInc("vbfmva",10,0.0,1.001,1,1,1,"(njet>=2&&njetingap==0&&abs(svfitmass-125)<25.)","(vbfmva<0.5)",3,12,"VBF MVA output","",c,0,"inclusive2JetSignal");


  /////------------------SM 2011------------------------------------------
  
  analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMcut(0),"",2,0,"m_{T} ","",c,0,"0JetLow");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(0),"(svfitmass<100||160<svfitmass)",2,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,10,"0JetLow");
  
  analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMcut(1),"",2,0,"m_{T} ","",c,0,"0JetHigh");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(1),"(svfitmass<100||160<svfitmass)",2,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,10,"0JetHigh");
  
  analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMcut(2),"",2,0,"m_{T} ","",c,0,"BoostedLow");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(2),"(svfitmass<100||160<svfitmass)",2,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"BoostedLow");  
  
  analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMcut(3),"",2,0,"m_{T} ","",c,0,"BoostedHigh");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(3),"(svfitmass<100||160<svfitmass)",2,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"BoostedHigh");  
  
  analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMcut(4),"",3,12,"m_{T} ","",c,0,"VBF");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(4),"(svfitmass<100||160<svfitmass)",3,12,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"VBF");



//   ////----------------MSSM 2011 ----------------------------------------

//   Float_t c4[4]={.65,.98,.52,.87};
//   Float_t xbinsValuesMSSM[20]={0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,450,500,600,700,800};
//   analysis->setVariableBinning(19,xbinsValuesMSSM);
//   analysis->setMSSMFlag(1);

//   analysis->plotInc("pZeta-1.5*pZetaVis",50,-150,100,1,1,0,"","",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","Events / 5 GeV ",c2,0,"MSSMinclusive");
//   analysis->plotInc("ditaumass",50,0,200,1,1,101,"","",0,0,"m_{vis}   (GeV)","",c,0,"MSSMinclusive");
//   analysis->plotInc("nbjet",5,-.5,4.5,1,1,101,"","",0,0," nbjet ","",c,0,"MSSMinclusive");

//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,1,0,"(nbjet>=1&&njet<2)","",4,22,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSMinclusiveBJet");
//   analysis->plotInc("leadBJetPt",20,0,200,1,1,101,"(nbjet>=1&&njet<2)","",4,22," b jet p_{T}   (GeV)","",c,0,"MSSMinclusiveBJet");
//   analysis->plotInc("leadBJetEta",20,-5,5,1,1,101,"(nbjet>=1&&njet<2)","",4,22," b jet #eta","",c,0,"MSSMinclusiveBJet"); 
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,"(nbjet>=1&&njet<2)","",4,22,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMinclusiveBJet");
//   analysis->plotInc("taupt",15,0,150,1,1,101,"(nbjet>=1&&njet<2)","",4,22,"tau pt (GeV)","",c,0,"MSSMinclusiveBJet"); 
  
//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,1,0,analysis->getSMcut(0),"",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSM0JetLow");
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,analysis->getSMcut(0),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSM0JetLow");
  
//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,1,0,analysis->getSMcut(1),"",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV] ","",c2,0,"MSSM0JetHigh");
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,analysis->getSMcut(1),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSM0JetHigh");
  
//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,1,0,analysis->getSMcut(2),"",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV] ","",c2,0,"MSSMBoostedLow");
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,analysis->getSMcut(2),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMBoostedLow");  
  
//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,1,0,analysis->getSMcut(3),"",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV] ","",c2,0,"MSSMBoostedHigh");
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,analysis->getSMcut(3),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMBoostedHigh");     
  
//   analysis->plotInc("pZeta-1.5*pZetaVis",10,-150,100,1,1,0,analysis->getSMcut(5),"",4,22,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSBJetLow");
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,analysis->getSMcut(5),"(svfitmass<100)",4,22,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMBJetLow"); 
  
//   analysis->plotInc("pZeta-1.5*pZetaVis",10,-150,100,1,1,0,analysis->getSMcut(6),"",4,22,"pZeta-1.5*pZetaVis","",c2,0,"MSSBJetHigh");
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,analysis->getSMcut(6),"(svfitmass<100)",4,22,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMBJetHigh");


  gROOT->ProcessLine(".q");
}

