#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include "TauMuPlotter.h"
#include "configTauMu2011.C"


void plotTauMu2011(){  

  //TString path="/data/benitezj/Samples/TauMuV541June2_TrigEff";
  //TString path="/afs/cern.ch/user/b/benitezj/output/TauMu2011FlatNtpJuly23";
  //TString path="/data/benitezj/Samples/TauMu2011FlatNtpJuly25";
  //TString path="/data/benitezj/Samples/TauMu2011FlatNtpJuly26";//fixed b-jets 
  TString path="/data/benitezj/Samples/TauMu2011FlatNtpJuly26svfit";

  TauMuPlotter*analysis=configTauMu2011("analysis",path);
  analysis->setQCDColor(kMagenta-10);
  analysis->setWJetsColor(kRed+2);
  analysis->setTTJetsColor(kBlue-8);
  analysis->setZMuMuColor(kRed);
  analysis->setZTauTauColor(kOrange-4);
  analysis->setSmearHistoRes(0.);
  analysis->setZTTType(2);
  analysis->setPlotTitle("CMS Preliminary 2011,   4.9 fb^{-1},     #sqrt{s} = 7 TeV,        #tau_{#mu}#tau_{h}");

  //raw yields to load samples now
  //analysis->printRawYields("(categoryIso==1&&abs(ditaucharge)==0)");

  //determine the SS-->OS QCD ratio
  //analysis->plotQCDSSOSRatio();

  //
  Float_t c[4]={.6,.96,.57,.87};//default legend coordinates
  Float_t c2[4]={.22,.47,.54,.9};//on the right side
  Float_t c3[4]={.6,.96,.22,.57};//on the right side bottom
  Float_t xbinsValues[14]={0,20,40,60,80,100,120,140,160,180,200,250,300,350};
  analysis->setVariableBinning(13,xbinsValues);

//   analysis->plotInc("nvtx",50,-.5,49.5,1,1,1,"","",0,0,"# of reco. vertices ","",c,0,"inclusive");

//   analysis->plotInc("mupt",50,0,100,1,1,1,"","",0,0," muon  pt   (GeV)","",c,0,"inclusive");
//   analysis->plotInc("mueta",50,-2.5,2.5,1,1,1,"","",0,0," muon  #eta","",c2,0,"inclusive");
//   analysis->plotInc("muphi",20,-3.5,3.5,1,1,1,"","",0,0," muon  phi ","",c3,0,"inclusive");     
//   analysis->plotInc("muiso",100,0,1,1,0,1,"(tauisodiscmva>=1)","",0,0," muon relIso","",c,0,"inclusive");
//   analysis->plotInc("mudz",100,-.05,.05,1,1,1,"","",0,0," mu dz","",c,0,"inclusive");
//   analysis->plotInc("mudxy",100,-.03,.03,1,1,1,"","",0,0," mu dxy","",c,0,"inclusive");

  analysis->plotInc("taupt",50,0,100,1,1,1,"","",0,0," tau pt   (GeV)","",c,0,"inclusive");
//   analysis->plotInc("taueta",50,-2.5,2.5,1,1,1,"","",0,0," tau  #eta","",c2,0,"inclusive"); 
//   analysis->plotInc("tauisomva",100,-1.0,1.01,1,0,1,"(muiso<0.1)","",0,0,"tau iso mva","",c,0,"inclusive");
//   analysis->plotInc("taudz",100,-.05,.05,1,1,1,"","",0,0," tau dz","",c,0,"inclusive");
//   analysis->plotInc("taudxy",100,-.03,.03,1,1,1,"","",0,0," tau dxy","",c,0,"inclusive");

//   analysis->plotInc("metpt",50,0,150,1,1,0,"","",0,0," MET   (GeV)","",c,0,"inclusive");    
//   analysis->plotInc("metphi",20,-3.5,3.5,1,1,0,"","",0,0," MET  phi ","",c,0,"inclusive");  
//   analysis->plotInc("1-cos(metphi-muphi)",40,0,2.001,1,1,0,"","",0,0," 1 - cos(#Delta#phi) ","",c,0,"inclusive");  
//   analysis->plotInc("metpt*cos(metphi)",30,-150,150,1,1,0,"","",0,0," MET  X (GeV)","",c2,0,"inclusive");    
//   analysis->plotInc("metpt*sin(metphi)",30,-150,150,1,1,0,"","",0,0," MET  Y (GeV)","",c2,0,"inclusive");    
  
//  analysis->plotInc("transversemass",20,0,200,1,1,0,"","",0,0,"M_{T}   [GeV]","",c,0,"inclusive");
//  analysis->plotInc("ditaumass",40,0,200,1,1,1,"","",0,0,"m(#mu#tau)   (GeV)","Events / 5 GeV",c,0,"inclusive");
//   analysis->plotInc("svfitmass",60,0,300,1,1,1,"","",0,0," m(#tau#tau)   (GeV)","Events / 5 GeV",c,0,"inclusive");
//   analysis->plotInc("njet",5,-.5,4.5,1,1,1,"","",0,0," njet ","",c,0,"inclusive");

//   //   analysis->plotInc("ditaumass",40,0,200,2,1,1,"(taudecaymode==0)","",0,0,"m(#mu#tau)   (GeV)","Events / 5 GeV",c,0,"taudecaymode==0");
//   //   analysis->plotInc("ditaumass",40,0,200,2,1,1,"(taudecaymode==1)","",0,0,"m(#mu#tau)   (GeV)","Events / 5 GeV",c,0,"taudecaymode==1");
//   //   analysis->plotInc("ditaumass",40,0,200,2,1,1,"(taudecaymode==2)","",0,0,"m(#mu#tau)   (GeV)","Events / 5 GeV",c,0,"taudecaymode==2");
//   //   analysis->plotInc("ditaumass",40,0,200,2,1,1,"(taudecaymode==10)","",0,0,"m(#mu#tau)   (GeV)","Events / 5 GeV",c,0,"taudecaymode==10");

//   analysis->plotInc("transversemass",30,0,300,1,1,0,"(njet>=1)","",2,0,"m_{T}   (GeV)","",c,0,"inclusive1Jet");
//   analysis->plotInc("leadJetPt",20,0,200,1,1,1,"(njet>=1)","",2,0,"lead jet p_{T}   (GeV)","",c,0,"inclusive1Jet");
//   analysis->plotInc("leadJetEta",20,-5,5,1,1,1,"(njet>=1)","",2,0,"lead jet #eta","",c,0,"inclusive1Jet");
//   analysis->plotInc("svfitmass",30,0,300,1,1,1,"(njet>=1)","",2,0," m(#tau#tau)   (GeV)","",c,0,"inclusive1Jet");
//   analysis->plotInc("taupt",50,0,100,1,1,1,"(njet>=1)","",2,0," tau pt   (GeV)","",c,0,"inclusive1Jet");

//   analysis->plotInc("transversemass",30,0,300,1,1,0,"(njet>=2)","",3,12,"m_{T}   (GeV)","",c,0,"inclusive2Jet");
//   analysis->plotInc("leadJetPt",20,0,200,1,1,1,"(njet>=2)","",3,12,"lead jet p_{T}   (GeV)","",c,0,"inclusive2Jet");
//   analysis->plotInc("leadJetEta",20,-5,5,1,1,1,"(njet>=2)","",3,12,"lead jet #eta","",c2,0,"inclusive2Jet");
//   analysis->plotInc("subleadJetPt",20,0,200,1,1,1,"(njet>=2)","",3,12,"sublead jet p_{T}   (GeV)","",c,0,"inclusive2Jet");
//   analysis->plotInc("subleadJetEta",20,-5,5,1,1,1,"(njet>=2)","",3,12,"sublead jet #eta","",c2,0,"inclusive2Jet"); 
//   analysis->plotInc("diJetMass",20,0,1000,1,1,1,"(njet>=2)","",3,12,"M(jj)","",c,0,"inclusive2Jet"); 
//   analysis->plotInc("abs(diJetDeltaEta)",20,0,10,1,1,1,"(njet>=2)","",3,12,"#Delta#eta","",c,0,"nomina2Jet"); 
//   analysis->plotInc("svfitmass",30,0,300,1,1,1,"(njet>=2)","",3,12," m(#tau#tau)   (GeV)","",c,0,"inclusive2Jet");
//   analysis->plotInc("taupt",50,0,100,1,1,1,"(njet>=2)","",3,12," tau pt   (GeV)","",c,0,"inclusive2Jet");

//   ////Plot the vbf mva variables
//   analysis->plotInc("vbfvars0",20,0,1000,1,1,1,"(njet>1)","",3,12,"M(jj)","",c,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars1",20,0,10,1,1,1,"(njet>1)","",3,12,"#Delta#eta(jj)","",c,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars2",20,0,3.4,1,1,1,"(njet>1)","",3,12,"#Delta#phi(jj)","",c2,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars3",20,0,200,1,1,1,"(njet>1)","",3,12,"#tau#tau   p_{T}","",c,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars4",20,0,200,1,1,1,"(njet>1)","",3,12,"di-jet p_{T}","",c,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars5",20,0,3.4,1,1,1,"(njet>1)","",3,12,"#Delta#phi(#tau#tau-jj)","",c2,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars6",20,0,10,1,1,1,"(njet>1)","",3,12,"#Delta#eta(#mu#tau-j)","",c,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars7",20,0,200,1,1,1,"(njet>1)","",3,12,"#mu#tau p_{T}","",c,0,"inclusive2Jet");
//   analysis->plotInc("vbfmva",20,-1.,1.001,1,1,1,"(njet>1)","(vbfmva<0.5)",3,12,"VBF MVA output","",c,10,"inclusive2Jet");
//   analysis->plotInc("vbfmva",10,0.0,1.001,1,1,1,"(njet>1&&njetingap==0&&abs(svfitmass-125)<25.)","(vbfmva<0.5)",3,12,"VBF MVA output","",c,5,"inclusive2JetSignal");


//   //   //////////2012 categories 
  
//   analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMcut(0),"",0,0,"m_{T} ","",c,0,"0JetLow");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(0),"(svfitmass<100||160<svfitmass)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,10,"0JetLow");
  
//   analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMcut(1),"",2,0,"m_{T} ","",c,0,"0JetHigh");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(1),"(svfitmass<100||160<svfitmass)",2,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,10,"0JetHigh");
  
//   analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMcut(2),"",2,0,"m_{T} ","",c,0,"BoostedLow");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(2),"(svfitmass<100||160<svfitmass)",2,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,10,"BoostedLow");  

//   analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMcut(3),"",2,0,"m_{T} ","",c,0,"BoostedHigh");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(3),"(svfitmass<100||160<svfitmass)",2,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"BoostedHigh");     

//   analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMcut(4),"",3,12,"m_{T} ","",c,0,"VBF");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(4),"(svfitmass<100||160<svfitmass)",3,12,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"VBF");
//   analysis->plotInc("ditaumass",0,0,260,1,1,1,analysis->getSMcut(4),"(ditaumass<100||160<ditaumass)",3,12,"m(#mu#tau)   (GeV)","",c,5,"VBF");
  



// // /* ----------------MSSM ----------------------------------------*/

//   //Float_t c4[4]={.65,.98,.52,.87};
//   Float_t xbinsValuesMSSM[20]={0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,450,500,600,700,800};
//   analysis->setVariableBinning(19,xbinsValuesMSSM);
//   analysis->setMSSMFlag(1);

//   analysis->plotInc("pZeta-1.5*pZetaVis",100,-150,100,1,1,0,"","",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSMinclusive");
//   analysis->plotInc("ditaumass",60,0,300,1,1,101,"","",0,0,"m(#mu#tau)  (GeV)","",c,0,"MSSMinclusive"); 
//   analysis->plotInc("svfitmass",70,0,350,1,1,101,"","",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMinclusive");
//   analysis->plotInc("nbjet",5,-.5,4.5,1,1,101,"","",0,0," njet ","",c,0,"MSSMinclusive");

//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,1,0,"(nbjet>=1&&njet<2)","",4,22,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSMinclusiveBJet");
//   analysis->plotInc("leadBJetPt",20,0,200,1,1,101,"(nbjet>=1&&njet<2)","",4,22," b jet p_{T}   (GeV)","",c,0,"MSSMinclusiveBJet");
//   analysis->plotInc("leadBJetEta",20,-5,5,1,1,101,"(nbjet>=1&&njet<2)","",4,22," b jet #eta","",c,0,"MSSMinclusiveBJet"); 
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,"(nbjet>=1&&njet<2)","",4,22,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMinclusiveBJet");
//   analysis->plotInc("taupt",15,0,150,1,1,101,"(nbjet>=1&&njet<2)","",4,22,"tau pt (GeV)","",c,0,"MSSMinclusiveBJet"); 

//   analysis->plotInc("pZeta-1.5*pZetaVis",100,-150,100,1,1,0,analysis->getSMcut(0),"",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSM0JetLow");
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,analysis->getSMcut(0),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSM0JetLow");
  
//   analysis->plotInc("pZeta-1.5*pZetaVis",50,-150,100,1,1,0,analysis->getSMcut(1),"",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSM0JetHigh");
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,analysis->getSMcut(1),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSM0JetHigh");

//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,1,0,analysis->getSMcut(2),"",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSMBoostedLow");
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,analysis->getSMcut(2),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMBoostedLow");  

//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,1,0,analysis->getSMcut(3),"",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSMBoostedHigh");
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,analysis->getSMcut(3),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMBoostedHigh");     

//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,1,0,analysis->getSMcut(5),"",4,22,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSMBJetLow");
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,analysis->getSMcut(5),"(svfitmass<100)",4,22,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMBJetLow"); 

//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,1,0,analysis->getSMcut(6),"",4,22,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSMBJetHigh");
//   analysis->plotInc("svfitmass",0,0,350,1,1,101,analysis->getSMcut(6),"(svfitmass<100)",4,22,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMBJetHigh");
  

  gROOT->ProcessLine(".q");
}





  // ////////////////////////////********************************************************************************************
  
  //misc checks
  //svfitmass at high mT
  //analysis->plotInc("svfitmass",30,0,300,1,3,analysis->getSMcut(1),"",0,0," m(#tau#tau)   (GeV)","Events / 5 GeV",c,0,"Check");

  
  ////////////Simple Tau fake rate ratios
  //analysis->plotTauFakeRate("taujetpt",16,20,100,"(njet==0)",50,1);//*(transversemass<15.)//*(15<=transversemass&&transversemass<30.)//*(30<=transversemass)
  //analysis->plotTauFakeRate("mupt",4,20,60,0);
  //analysis->plotTauFakeRate("taupt",4,20,60,2);
  //analysis->plotTauFakeRate("abs(taueta)",6,0,3.0);
  //analysis->plotTauFakeRate("abs(taujeteta)",6,0,3.0,0);
  //analysis->plotTauFakeRate("njet",4,-0.5,3.5);
  //analysis->plotTauFakeRate("transversemass",15,0,150,0);
  //analysis->plotTauFakeRate("cos(metphi-muphi)",10,-1,1.0001,1);

  /////////////Ratio for both mu and tau anti-Iso
  //AverageRatio values as a function of njets:
  //n=0--> 0.0246023, n=1-->0.0187078, n=2-->0.0125766, n=3-->0.0148136, n=4 -->0.0175073 +- 0.00520148

  /////there are 4 regions: 1=[tauIso,muIso] 2=[tauAntiIso,muIso]  3=[tauIso,muAntiIso] 4=[tauAntiIso,muAntiIso]
  //analysis->plotIsoFakeRate("taujetpt",16,20,100,"(muiso<0.1)","(tauisodisc>=2)",50,1);//region 1/2 
  //analysis->plotIsoFakeRate("mujetpt",16,17,97,"(tauisodisc>=2)","(muiso<0.1)",50,1);//region 1/3
  //analysis->plotIsoFakeRate("taujetpt",16,20,100,"","(tauisodisc>=2)",50,1);//region 4 
  //analysis->plotIsoFakeRate("mujetpt",16,17,97,"","(muiso<0.1)",50,1);//region 4

  //for LooseMVA tau isolation
  //analysis->plotIsoFakeRate("taujetpt",16,20,100,"(pftransversemass<40.&&muiso<0.1)","(tauisodiscmva>=1)",50,1);//region 1/2 
  //analysis->plotIsoFakeRate("mujetpt",16,17,97,"(pftransversemass<40.&&tauisodiscmva>=1)","(muiso<0.1)",50,1);//region 1/3

  ///Ratio for W+jets (mT>60)
  //analysis->plotTauFakeRateWJets("taujetpt",16,20,100,"(pftransversemass>60.&&muiso<0.1)",50,1);
  


////////////////////////////////////////////////////
//   Float_t sigY=0;
//   sigY+=analysis->getSample("HiggsGG120")->Integral();
//   sigY+=analysis->getSample("HiggsVBF120")->Integral();
//   sigY+=analysis->getSample("HiggsVH120")->Integral();
//   cout<<"Signal yield "<<sigY<<endl;

  /////////////inclusive SS
  //analysis->plotIncSS("ditaumass",1,1,-1,60,0,300,"m_{vis}   (GeV)","Events / 5 GeV",c,0);
  //analysis->plotIncSS("taupt",1,1,-1,50,0,100,""," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("muiso",-1,1,-1,100,0,2," #mu relIsoDB","",c);
  //analysis->plotIncSS("tauiso",-1,1,-1,100,0,2," #tau relIso","",c);
  //analysis->plotIncSS("tauisodisc",-1,1,-1,6,-0.5,5.5," #tau iso disc.","",c);
  //analysis->plotIncSS("pftransversemass",30,0,150,1,0,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("transversemass",1,0,-1,30,0,150,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("nditau",1,1,-1,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotIncSS("metpt",1,0,-1,50,0,200," MET   (GeV)","Events / 4 GeV",c);    
  //////////////inclusive anti
  //analysis->plotInc("nditau",2,1,-1,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotInc("taupt",2,1,-1,12,0,60," #tau p_{T}   (GeV)","Events / 5 GeV",c);  
  //analysis->plotInc("metpt",2,0,-1,50,0,200," MET   (GeV)","Events / 4 GeV",c);    
  //analysis->plotInc("transversemass",2,0,-1,50,0,200,"m_{T}   (GeV)","Events / 4 GeV",c);
  //////////////inclusive SS anti
  //analysis->plotIncSS("taupt",2,1,-1,8,20,100," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",2,1,-1,6,0,3," #eta_{#tau}","",c);   
  //analysis->plotIncSS("nditau",2,1,-1,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotIncSS("ditaumass",2,1,-1,60,0,300,"m_{vis}   (GeV)","Events / 5 GeV",c,0);
  //analysis->plotIncSS("transversemass",2,0,-1,30,0,150,"m_{T}   (GeV)","",c);

  

  //////////// //////////////SM0/////////////////////////
  //analysis->plotInc("ditaumass",1,1,0,60,0,300,"","m_{vis}   (GeV)","Events / 5 GeV",c);
  //analysis->plotInc("taupt",1,1,0,8,20,100," #tau p_{T}   (GeV)","",c);  
  //analysis->plotInc("taujetpt",1,1,-1,8,20,100,"(njet==0)"," #tau jet  p_{T}   (GeV)","",c);  
  //analysis->plotInc("transversemass",1,0,-1,20,0,200,"(njet==0)","m_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",1,1,-1,60,0,300,"(njet==0)","m_{vis}   (GeV)","",c);
  //analysis->plotInc("mupt",1,1,-1,20,0,100,"(njet==0)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",1,1,-1,20,0,100,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  //analysis->plotInc("metpt",1,1,-1,12,0,60,"(njet==0)"," MET   (GeV)","",c);    
  /////SS
  //analysis->plotIncSS("transversemass",1,0,-1,20,0,200,"(njet==0)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("taujetpt",1,1,-1,16,20,100,"(njet==0)"," #tau jet  p_{T}   (GeV)","",c);  
  //analysis->plotIncSS("taupt",1,1,-1,6,0,60,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  ////Anti
  //analysis->plotInc("transversemass",2,0,-1,20,0,200,"(njet==0&&muiso<0.1)","m_{T}   (GeV)","",c);
  //analysis->plotInc("transversemass",2,0,-1,20,0,200,"(njet==0&&tauisodisc>=2)","m_{T}   (GeV)","",c);
  //analysis->plotInc("transversemass",2,0,-1,20,0,200,"(njet==0&&muiso>0.1&&tauisodisc<2)","m_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",2,1,-1,6,0,60,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  ////SS anti
  //analysis->plotIncSS("transversemass",2,0,-1,20,0,200,"(njet==0)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("metpt",2,1,-1,12,0,60,"(njet==0)"," MET   (GeV)","",c);  
  //analysis->plotIncSS("taupt",2,1,-1,6,0,60,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  //analysis->plotIncSS("taujetpt",2,1,-1,16,20,100,"(njet==0)"," #tau jet  p_{T}   (GeV)","",c);  

  ////////////////////////////SM1//////////////////////////
  //analysis->plotInc("transversemass",1,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",1,1,1,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotInc("transversemass",1,0,-1,20,0,200,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotInc("mupt",1,1,-1,20,0,100,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",1,1,-1,20,0,100,"(njet==1)"," #tau p_{T}   (GeV)","",c);  
  //analysis->plotInc("ditaumass",1,1,-1,30,0,300,"(njet==1)","m_{vis}   (GeV)","",c);
  //analysis->plotInc("metpt",1,1,-1,15,0,150,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotInc("taujetpt",1,1,-1,8,20,100,"(njet==1)"," #tau jet  p_{T}   (GeV)","",c);  
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotInc("(1-cos(muphi-metphi))",1,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);
  //analysis->plotInc("cos(tauphi-muphi)",1,1,-1,20,-1,1.001,"(njet==1)","cos(#mu_#phi-#tau_#phi)","",c);
  //analysis->plotInc("transversemass",1,0,-1,20,0,200,"(njet>=1)","m_{T}   (GeV)","",c);
  ////////////////////////////SS
  //analysis->plotIncSS("transversemass",1,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("taupt",1,1,1,20,0,200," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("ditaumass",1,1,1,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotIncSS("transversemass",1,0,-1,20,0,200,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("mupt",1,1,-1,8,10,50,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",1,0,10,6,0,3," |#eta_{#tau}| ","",c); 
  //analysis->plotIncSS("metpt",1,1,-1,12,0,60,"(njet==1)"," MET   (GeV)","",c);    
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotIncSS("(1-cos(muphi-metphi))",1,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);
  //analysis->plotIncSS("taujetpt",1,1,-1,16,20,100,"(njet==1)"," #tau jet  p_{T}   (GeV)","",c);  
  //analysis->plotIncSS("transversemass",1,0,-1,20,0,200,"(njet>=1)","m_{T}   (GeV)","",c);
  //////////////////////////////anti
  //analysis->plotInc("transversemass",2,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotInc("tauisodisc",2,1,1,6,-.5,5.5,"tau isolation discriminator ","",c,0);
  //analysis->plotInc("taupt",2,1,1,20,0,200," #tau p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotInc("taujetpt",2,1,1,10,0,100,""," #tau jet  p_{T}   (GeV)","",c);
  //analysis->plotInc("mupt",2,1,1,10,0,100,""," #mu p_{T}   (GeV)","",c);  
  //analysis->plotInc("ditaumass",2,1,1,15,0,300,"","m_{vis}   (GeV)","Events / 20 GeV",c);
  //analysis->plotInc("transversemass",2,0,-1,20,0,200,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotInc("metpt",2,1,-1,12,0,60,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotInc("metpt",2,3,-1,40,0,200,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotInc("mupt",2,1,-1,10,0,50,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotInc("(1-cos(muphi-metphi))",2,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);
  ////////////////////////////SS anti
  //analysis->plotIncSS("taupt",2,1,1,20,0,200," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("ditaumass",2,1,1,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotIncSS("transversemass",2,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",2,0,10,6,0,3," |#eta_{#tau}| ","",c); 
  //analysis->plotIncSS("transversemass",2,0,-1,15,0,150,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("metpt",2,1,-1,12,0,60,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotIncSS("mupt",2,1,-1,10,0,50,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotIncSS("(1-cos(muphi-metphi))",2,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);


  ////////////////////////////SM2
  //analysis->plotInc("transversemass",1,0,2,15,0,150,"","m_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",1,1,2,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotInc("svfitmass",1,1,2,14,0,350,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotInc("taupt",1,1,2,10,0,100," #tau p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotInc("mupt",1,1,2,10,0,100," #mu p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotInc("nditau",1,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotInc("transversemass",1,0,-1,20,0,200,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotInc("mupt",1,1,-1,10,0,100,"(njet==2)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",1,1,-1,10,0,100,"(njet==2)","#tau  p_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",1,1,-1,15,0,300,"(njet==2)","m_{vis}   (GeV)","",c);
  //analysis->plotInc("leadJetPt",1,1,-1,20,0,400,"(njet>=2)"," jet p_{T}   (GeV)","",c);
  //analysis->plotInc("leadJetEta",1,1,-1,18,-4.5,4.5,"(njet>=2)"," jet #eta","",c); 

  ///////////////////////////SS
  //analysis->plotIncSS("transversemass",1,0,2,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("ditaumass",1,1,2,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotIncSS("nditau",1,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotIncSS("transversemass",1,0,-1,15,0,150,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",1,0,20,6,0,3," #eta_{#tau} ","",c); 
  //analysis->plotIncSS("metpt",1,0,20,20,0,200," MET   (GeV)","Events / 5 GeV",c);    
  //analysis->plotIncSS("taujetpt",1,1,-1,8,20,100,"(njet==2)"," #tau jet  p_{T}   (GeV)","",c);  
  ///////////////////////////anti
  //analysis->plotInc("transversemass",2,0,2,15,0,150,"","m_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",2,1,2,10,0,100," #tau p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotInc("ditaumass",2,1,2,15,0,300,"","m_{vis}   (GeV)","Events / 20 GeV",c);
  //analysis->plotInc("nditau",2,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotInc("transversemass",2,0,-1,15,0,150,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",2,1,20,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);
  ///////////////////////////SS anti
  //analysis->plotIncSS("transversemass",2,0,2,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("nditau",2,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotIncSS("ditaumass",2,1,2,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotIncSS("taupt",2,1,2,8,20,100," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",2,1,2,6,0,3," #eta_{#tau} ","",c); 
  //analysis->plotIncSS("transversemass",2,0,-1,20,0,200,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",2,0,20,6,0,3," #eta_{#tau} ","",c); 
  //analysis->plotIncSS("metpt",2,0,20,20,0,200," MET   (GeV)","Events / 5 GeV",c);    



  //////Tau Iso studies: LooseMVA=0.795, MediumMVA=0.884, TightMVA=0.921
  //inclusive
  //analysis->plotIncSS("transversemass",1,0,-1,30,0,150,"","m_{T}   (GeV)","",c);//Reference Loose deltaBetaCorr
  //analysis->plotIncSS("transversemass",0,0,-1,30,0,150,"(muiso<0.1&&tauisomva>0.795)","m_{T}   (GeV)","",c);//Loose MVA
  //analysis->plotIncSS("transversemass",0,0,-1,30,0,150,"(muiso<0.1&&tauisomva>0.884)","m_{T}   (GeV)","",c);//Medium MVA
  //analysis->plotIncSS("transversemass",0,0,-1,30,0,150,"(muiso<0.1&&tauisomva>0.921)","m_{T}   (GeV)","",c);//Tight MVA
  //analysis->plotInc("ditaumass",0,1,-1,40,0,200,"(muiso<0.1&&tauisomva>0.884)","m_{vis}   (GeV)","Events / 5 GeV",c);
  //////Muon Iso studies
  //analysis->plotInc("ditaumass",0,1,-1,40,0,200,"(muiso<0.1&&tauisodisc>=2)","m_{vis}   (GeV)","Events / 5 GeV",c);
  //analysis->plotInc("ditaumass",0,1,-1,40,0,200,"(((abs(mueta)<1.479)*(muisomva>0.96)+(abs(mueta)>1.479)*(muisomva>0.95))&&tauisodisc>=2)","m_{vis}   (GeV)","Events / 5 GeV",c);
  


