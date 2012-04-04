#define NSAMP 7

void qcdRatio(long njet=0){
  
  float lumi=4700;//in 1/pb
  Sample * qcd[NSAMP];


//   //Cant add this sample because does not have trigger isolation required
//   ////////////////////////////////////////////////////////////////////////
//   ///2.966E8 	2.855E-4 	-1 	QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6  
//   qcd[0] = new Sample("QCD_Pt_20_MuEnrichedPt_15","output/QCDmuPt15_Mar27");
//   qcd[0]->setCrossection(2.966e8);//in pb
//   qcd[0]->setSampleGenEvents(2.027e7/2.855e-4);
//   /////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
  //	5.792E8 	0.00288 	-1 	QCD_Pt-15to20_MuPt5Enriched_TuneZ2_7TeV-pythia6
  //	2.363E8 	0.00568 	-1 	QCD_Pt-20to30_MuPt5Enriched_TuneZ2_7TeV-pythia6
  //	5.307E7 	0.01124 	-1 	QCD_Pt-30to50_MuPt5Enriched_TuneZ2_7TeV-pythia6
  //	6351000.0 	0.02205 	-1 	QCD_Pt-50to80_MuPt5Enriched_TuneZ2_7TeV-pythia6
  // 	785100.0 	0.03636 	-1 	QCD_Pt-80to120_MuPt5Enriched_TuneZ2_7TeV-pythia6
  //	92950.0 	0.04866 	-1 	QCD_Pt-120to150_MuPt5Enriched_TuneZ2_7TeV-pythia6
  //	47580.0 	0.06033 	-1 	QCD_Pt-150_MuPt5Enriched_TuneZ2_7TeV-pythia6

  TString samplespath="output/QCD_Mar30";
  qcd[0] =new Sample("QCDPt15to20",samplespath.Data());
  qcd[1] =new Sample("QCDPt20to30",samplespath.Data());
  qcd[2] =new Sample("QCDPt30to50",samplespath.Data());
  qcd[3] =new Sample("QCDPt50to80",samplespath.Data());
  qcd[4] =new Sample("QCDPt80to120",samplespath.Data());
  qcd[5] =new Sample("QCDPt120to150",samplespath.Data());
  qcd[6] =new Sample("QCDPt150",samplespath.Data());
  
  qcd[0]->setCrossection(5.792E8);//in pb
  qcd[1]->setCrossection(2.363E8);
  qcd[2]->setCrossection(5.307E7);
  qcd[3]->setCrossection(6351000);
  qcd[4]->setCrossection(785100);
  qcd[5]->setCrossection(92950);
  qcd[6]->setCrossection(47580);

  qcd[0]->setSampleGenEvents(2524156*0.961411/0.00288);//events in PD * processEff / genFilterEff
  qcd[1]->setSampleGenEvents(10076800*0.978300/0.00568);
  qcd[2]->setSampleGenEvents(10874052*0.972078/0.01124);
  qcd[3]->setSampleGenEvents(10830209*0.964526/0.02205);
  qcd[4]->setSampleGenEvents(8313422*0.976163/0.03636);
  qcd[5]->setSampleGenEvents(8013763*0.968981/0.04866);
  qcd[6]->setSampleGenEvents(4045565*0.952323/0.06033);


  



  TString eventweight="eventweight";  

  TString vartau="taujetpt";
  int nbinstau=16; float xmintau=20; float xmaxtau=100;

  TString varmu="mujetpt";
  int nbinsmu=16; float xminmu=17; float xmaxmu=97;


  TH1F HTauJetPtI("HTauJetPtI","HTauJetPtI",nbinstau,xmintau,xmaxtau);HTauJetPtI.Sumw2();HTauJetPtI.GetXaxis()->SetTitle(vartau);
  TH1F HTauJetPtA("HTauJetPtA","HTauJetPtA",nbinstau,xmintau,xmaxtau);HTauJetPtA.Sumw2();HTauJetPtA.GetXaxis()->SetTitle(vartau);

  TH1F HTauJetPtISS("HTauJetPtISS","HTauJetPtISS",nbinstau,xmintau,xmaxtau);HTauJetPtISS.Sumw2();HTauJetPtISS.GetXaxis()->SetTitle(vartau);
  TH1F HTauJetPtASS("HTauJetPtASS","HTauJetPtASS",nbinstau,xmintau,xmaxtau);HTauJetPtASS.Sumw2();HTauJetPtASS.GetXaxis()->SetTitle(vartau);

  TH1F HMuJetPtI("HMuJetPtI","HMuJetPtI",nbinsmu,xminmu,xmaxmu);HMuJetPtI.Sumw2();HMuJetPtI.GetXaxis()->SetTitle(varmu);
  TH1F HMuJetPtA("HMuJetPtA","HMuJetPtA",nbinsmu,xminmu,xmaxmu);HMuJetPtA.Sumw2();HMuJetPtA.GetXaxis()->SetTitle(varmu);

  TH1F HMuJetPtISS("HMuJetPtISS","HMuJetPtISS",nbinsmu,xminmu,xmaxmu);HMuJetPtISS.Sumw2();HMuJetPtISS.GetXaxis()->SetTitle(varmu);
  TH1F HMuJetPtASS("HMuJetPtASS","HMuJetPtASS",nbinsmu,xminmu,xmaxmu);HMuJetPtASS.Sumw2();HMuJetPtASS.GetXaxis()->SetTitle(varmu);

  //
  TH1F HmuIso("HmuIso","mu relIso",100,0,2); HmuIso.Sumw2();HmuIso.GetXaxis()->SetTitle("muiso");
  TH1F HtauIso("HtauIso","tau relIso",100,0,2); HtauIso.Sumw2();HtauIso.GetXaxis()->SetTitle("tauiso");
  TH1F HtauIsoDisc("HtauIsoDisc","tau Iso Disc.",6,-.5,5.5); HtauIsoDisc.Sumw2();HtauIsoDisc.GetXaxis()->SetTitle("tauisodisc");

  //
  int nbinsmT=10; float xminmT=0.; float xmaxmT=100.;
  TH1F HmTI("HmTI","mT",nbinsmT,xminmT,xmaxmT);HmTI.Sumw2();HmTI.GetXaxis()->SetTitle("m_{T}");
  TH1F HmTATau("HmTATau","mT",nbinsmT,xminmT,xmaxmT);HmTATau.Sumw2();HmTATau.GetXaxis()->SetTitle("m_{T}");
  TH1F HmTAMu("HmTAMu","mT",nbinsmT,xminmT,xmaxmT);HmTAMu.Sumw2();HmTAMu.GetXaxis()->SetTitle("m_{T}");
  TH1F HmTATauMu("HmTATauMu","mT",nbinsmT,xminmT,xmaxmT);HmTATauMu.Sumw2();HmTATauMu.GetXaxis()->SetTitle("m_{T}");
   
  //Plots with the ratio applied (ratio computed inclusively and OS+SS)
  TH1F HmTARTau("HmTARTau","mT",nbinsmT,xminmT,xmaxmT);HmTARTau.Sumw2();HmTARTau.GetXaxis()->SetTitle("m_{T}");
  TH1F HmTARTauP("HmTARTauP","mT",nbinsmT,xminmT,xmaxmT);HmTARTauP.Sumw2();HmTARTauP.GetXaxis()->SetTitle("m_{T}");
  TH1F HmTARTauM("HmTARTauM","mT",nbinsmT,xminmT,xmaxmT);HmTARTauM.Sumw2();HmTARTauM.GetXaxis()->SetTitle("m_{T}");

  TH1F HmTARMu("HmTARMu","mT",nbinsmT,xminmT,xmaxmT);HmTARMu.Sumw2();HmTARMu.GetXaxis()->SetTitle("m_{T}");
  TH1F HmTARMuP("HmTARMuP","mT",nbinsmT,xminmT,xmaxmT);HmTARMuP.Sumw2();HmTARMuP.GetXaxis()->SetTitle("m_{T}");
  TH1F HmTARMuM("HmTARMuM","mT",nbinsmT,xminmT,xmaxmT);HmTARMuM.Sumw2();HmTARMuM.GetXaxis()->SetTitle("m_{T}");

  TH1F HmTARTauMu("HmTARTauMu","mT",nbinsmT,xminmT,xmaxmT);HmTARTauMu.Sumw2();HmTARTauMu.GetXaxis()->SetTitle("m_{T}");
  TH1F HmTARTauMuP("HmTARTauMuP","mT",nbinsmT,xminmT,xmaxmT);HmTARTauMuP.Sumw2();HmTARTauMuP.GetXaxis()->SetTitle("m_{T}");
  TH1F HmTARTauMuM("HmTARTauMuM","mT",nbinsmT,xminmT,xmaxmT);HmTARTauMuM.Sumw2();HmTARTauMuM.GetXaxis()->SetTitle("m_{T}");




  //Ratio calculated from Fall11 QCD samples and with other leg iso removed
  TString qcdRatioTau="*((20<taujetpt&&taujetpt<=25)*6.37+(25<taujetpt&&taujetpt<=30)*0.937+(30<taujetpt&&taujetpt<=35)*0.313+(35<taujetpt&&taujetpt<=40)*0.105+(40<taujetpt&&taujetpt<=45)*0.0571+(45<taujetpt&&taujetpt<=50)*0.0776+(50<taujetpt&&taujetpt<=55)*0.0468+(55<taujetpt&&taujetpt<=60)*0.0462+(60<taujetpt&&taujetpt<=65)*0.0119+(65<taujetpt&&taujetpt<=70)*0.0157+(70<taujetpt&&taujetpt<=75)*0.0188+(75<taujetpt&&taujetpt<=80)*0.0292+(80<taujetpt&&taujetpt<=85)*0.0105+(85<taujetpt&&taujetpt<=90)*0.00248+(90<taujetpt&&taujetpt<=95)*0.00143+(95<taujetpt&&taujetpt<=100)*0.0199)";
  TString qcdRatioTauP="*((20<taujetpt&&taujetpt<=25)*13.1+(25<taujetpt&&taujetpt<=30)*1.3+(30<taujetpt&&taujetpt<=35)*0.382+(35<taujetpt&&taujetpt<=40)*0.126+(40<taujetpt&&taujetpt<=45)*0.0725+(45<taujetpt&&taujetpt<=50)*0.095+(50<taujetpt&&taujetpt<=55)*0.0598+(55<taujetpt&&taujetpt<=60)*0.0589+(60<taujetpt&&taujetpt<=65)*0.0164+(65<taujetpt&&taujetpt<=70)*0.0213+(70<taujetpt&&taujetpt<=75)*0.0261+(75<taujetpt&&taujetpt<=80)*0.0458+(80<taujetpt&&taujetpt<=85)*0.017+(85<taujetpt&&taujetpt<=90)*0.00439+(90<taujetpt&&taujetpt<=95)*0.00265+(95<taujetpt&&taujetpt<=100)*0.0341)";
  TString qcdRatioTauM="*((20<taujetpt&&taujetpt<=25)*-0.356+(25<taujetpt&&taujetpt<=30)*0.576+(30<taujetpt&&taujetpt<=35)*0.244+(35<taujetpt&&taujetpt<=40)*0.0833+(40<taujetpt&&taujetpt<=45)*0.0418+(45<taujetpt&&taujetpt<=50)*0.0602+(50<taujetpt&&taujetpt<=55)*0.0338+(55<taujetpt&&taujetpt<=60)*0.0335+(60<taujetpt&&taujetpt<=65)*0.00744+(65<taujetpt&&taujetpt<=70)*0.0102+(70<taujetpt&&taujetpt<=75)*0.0116+(75<taujetpt&&taujetpt<=80)*0.0125+(80<taujetpt&&taujetpt<=85)*0.00399+(85<taujetpt&&taujetpt<=90)*0.000581+(90<taujetpt&&taujetpt<=95)*0.000211+(95<taujetpt&&taujetpt<=100)*0.00575)";


  TString qcdRatioMu="*((17<mujetpt&&mujetpt<=22)*2.13+(22<mujetpt&&mujetpt<=27)*0.451+(27<mujetpt&&mujetpt<=32)*0.181+(32<mujetpt&&mujetpt<=37)*0.0824+(37<mujetpt&&mujetpt<=42)*0.0754+(42<mujetpt&&mujetpt<=47)*0.0539+(47<mujetpt&&mujetpt<=52)*0.0504+(52<mujetpt&&mujetpt<=57)*0.0603+(57<mujetpt&&mujetpt<=62)*0.0778+(62<mujetpt&&mujetpt<=67)*0.092+(67<mujetpt&&mujetpt<=72)*0.0351+(72<mujetpt&&mujetpt<=77)*0.114+(77<mujetpt&&mujetpt<=82)*0.186+(82<mujetpt&&mujetpt<=87)*0.0596+(87<mujetpt&&mujetpt<=92)*0.086+(92<mujetpt&&mujetpt<=97)*0.0697)";
  TString qcdRatioMuP="*((17<mujetpt&&mujetpt<=22)*3.02+(22<mujetpt&&mujetpt<=27)*0.509+(27<mujetpt&&mujetpt<=32)*0.202+(32<mujetpt&&mujetpt<=37)*0.0946+(37<mujetpt&&mujetpt<=42)*0.0893+(42<mujetpt&&mujetpt<=47)*0.0655+(47<mujetpt&&mujetpt<=52)*0.0619+(52<mujetpt&&mujetpt<=57)*0.0858+(57<mujetpt&&mujetpt<=62)*0.0994+(62<mujetpt&&mujetpt<=67)*0.122+(67<mujetpt&&mujetpt<=72)*0.0495+(72<mujetpt&&mujetpt<=77)*0.172+(77<mujetpt&&mujetpt<=82)*0.29+(82<mujetpt&&mujetpt<=87)*0.092+(87<mujetpt&&mujetpt<=92)*0.144+(92<mujetpt&&mujetpt<=97)*0.117)";
  TString qcdRatioMuM="*((17<mujetpt&&mujetpt<=22)*1.25+(22<mujetpt&&mujetpt<=27)*0.394+(27<mujetpt&&mujetpt<=32)*0.16+(32<mujetpt&&mujetpt<=37)*0.0702+(37<mujetpt&&mujetpt<=42)*0.0615+(42<mujetpt&&mujetpt<=47)*0.0423+(47<mujetpt&&mujetpt<=52)*0.0389+(52<mujetpt&&mujetpt<=57)*0.0348+(57<mujetpt&&mujetpt<=62)*0.0562+(62<mujetpt&&mujetpt<=67)*0.0616+(67<mujetpt&&mujetpt<=72)*0.0206+(72<mujetpt&&mujetpt<=77)*0.0566+(77<mujetpt&&mujetpt<=82)*0.0824+(82<mujetpt&&mujetpt<=87)*0.0272+(87<mujetpt&&mujetpt<=92)*0.028+(92<mujetpt&&mujetpt<=97)*0.0222)";

  TString qcdRatioTauMu=qcdRatioTau+qcdRatioMu;
  TString qcdRatioTauMuP=qcdRatioTauP+qcdRatioMuP;
  TString qcdRatioTauMuM=qcdRatioTauM+qcdRatioMuM;




  for(Int_t i=0;i<NSAMP;i++){
    if(qcd[i]->getTreeEntries()<500)continue;
    qcd[i]->setDataType("MC");
    cout<<qcd[i]->GetName()<<" scale "<<lumi/(qcd[i]->getLumi())<<endl;
    qcd[i]->scale(lumi/(qcd[i]->getLumi()));


    //tau jet
    TH1F*HTauI=qcd[i]->getHistoNtpFile(vartau,nbinstau,xmintau,xmaxtau,eventweight+"*(categoryCh==1)*(tauisodisc>=2)");
    HTauJetPtI.Add(HTauI,qcd[i]->getNorm());
    delete HTauI;

    TH1F*HTauA=qcd[i]->getHistoNtpFile(vartau,nbinstau,xmintau,xmaxtau,eventweight+"*(categoryCh==1)*(tauisodisc<2)");
    HTauJetPtA.Add(HTauA,qcd[i]->getNorm());
    delete HTauA;
 
    TH1F*HTauISS=qcd[i]->getHistoNtpFile(vartau,nbinstau,xmintau,xmaxtau,eventweight+"*(categoryCh==2)*(tauisodisc>=2)");
    HTauJetPtISS.Add(HTauISS,qcd[i]->getNorm()); 
    delete HTauISS;

    TH1F*HTauASS=qcd[i]->getHistoNtpFile(vartau,nbinstau,xmintau,xmaxtau,eventweight+"*(categoryCh==2)*(tauisodisc<2)");
    HTauJetPtASS.Add(HTauASS,qcd[i]->getNorm());
    delete HTauASS;

    //Muon jet
    TH1F*HMuI=qcd[i]->getHistoNtpFile(varmu,nbinsmu,xminmu,xmaxmu,eventweight+"*(categoryCh==1)*(muiso<0.1)");
    HMuJetPtI.Add(HMuI,qcd[i]->getNorm());
    delete HMuI;
    
    TH1F*HMuA=qcd[i]->getHistoNtpFile(varmu,nbinsmu,xminmu,xmaxmu,eventweight+"*(categoryCh==1)*(muiso>=0.1)");
    HMuJetPtA.Add(HMuA,qcd[i]->getNorm());
    delete HMuA;

    TH1F*HMuISS=qcd[i]->getHistoNtpFile(varmu,nbinsmu,xminmu,xmaxmu,eventweight+"*(categoryCh==2)*(muiso<0.1)");
    HMuJetPtISS.Add(HMuISS,qcd[i]->getNorm()); 
    delete HMuISS;

    TH1F*HMuASS=qcd[i]->getHistoNtpFile(varmu,nbinsmu,xminmu,xmaxmu,eventweight+"*(categoryCh==2)*(muiso>=0.1)");
    HMuJetPtASS.Add(HMuASS,qcd[i]->getNorm());
    delete HMuASS;

    //Transverse mass
    TH1F*HmTI_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(categoryIso==1)");
    HmTI.Add(HmTI_,qcd[i]->getNorm()); 
    delete HmTI_;
    TH1F*HmTATau_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(muiso<0.1&&tauisodisc<2)");
    HmTATau.Add(HmTATau_,qcd[i]->getNorm()); 
    delete HmTATau_;
    TH1F*HmTAMu_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(muiso>=0.1&&tauisodisc>=2)");
    HmTAMu.Add(HmTAMu_,qcd[i]->getNorm()); 
    delete HmTAMu_;
    TH1F*HmTATauMu_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(muiso>=0.1&&tauisodisc<2)");
    HmTATauMu.Add(HmTATauMu_,qcd[i]->getNorm()); 
    delete HmTATauMu_;
    
    //apply the ratio to predict the QCD in the isolated region
    TH1F*HmTARTau_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(muiso<0.1&&tauisodisc<2)"+qcdRatioTau);
    HmTARTau.Add(HmTARTau_,qcd[i]->getNorm()); 
    delete HmTARTau_;    
    TH1F*HmTARTauP_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(muiso<0.1&&tauisodisc<2)"+qcdRatioTauP);
    HmTARTauP.Add(HmTARTauP_,qcd[i]->getNorm()); 
    delete HmTARTauP_;    
    TH1F*HmTARTauM_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(muiso<0.1&&tauisodisc<2)"+qcdRatioTauM);
    HmTARTauM.Add(HmTARTauM_,qcd[i]->getNorm()); 
    delete HmTARTauM_;    

    TH1F*HmTARMu_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(muiso>=0.1&&tauisodisc>=2)"+qcdRatioMu);
    HmTARMu.Add(HmTARMu_,qcd[i]->getNorm()); 
    delete HmTARMu_;    
    TH1F*HmTARMuP_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(muiso>=0.1&&tauisodisc>=2)"+qcdRatioMuP);
    HmTARMuP.Add(HmTARMuP_,qcd[i]->getNorm()); 
    delete HmTARMuP_;    
    TH1F*HmTARMuM_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(muiso>=0.1&&tauisodisc>=2)"+qcdRatioMuM);
    HmTARMuM.Add(HmTARMuM_,qcd[i]->getNorm()); 
    delete HmTARMuM_;    

    TH1F*HmTARTauMu_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(muiso>=0.1&&tauisodisc<2)"+qcdRatioTauMu);
    HmTARTauMu.Add(HmTARTauMu_,qcd[i]->getNorm()); 
    delete HmTARTauMu_;    
    TH1F*HmTARTauMuP_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(muiso>=0.1&&tauisodisc<2)"+qcdRatioTauMuP);
    HmTARTauMuP.Add(HmTARTauMuP_,qcd[i]->getNorm()); 
    delete HmTARTauMuP_;    
    TH1F*HmTARTauMuM_=qcd[i]->getHistoNtpFile("transversemass",nbinsmT,xminmT,xmaxmT,eventweight+"*(muiso>=0.1&&tauisodisc<2)"+qcdRatioTauMuM);
    HmTARTauMuM.Add(HmTARTauMuM_,qcd[i]->getNorm()); 
    delete HmTARTauMuM_;    


    //isolation distributions
    TH1F*HmuIso_=qcd[i]->getHistoNtpFile("muiso",100,0,2,eventweight);//&&tauisodisc>=2
    HmuIso.Add(HmuIso_,qcd[i]->getNorm()); 
    delete HmuIso_;

    TH1F*HtauIso_=qcd[i]->getHistoNtpFile("tauiso",100,0,2,eventweight);//&&muiso<0.1
    HtauIso.Add(HtauIso_,qcd[i]->getNorm()); 
    delete HtauIso_;

    TH1F*HtauIsoDisc_=qcd[i]->getHistoNtpFile("tauisodisc",6,-.5,5.5,eventweight);//&&muiso<0.1
    HtauIsoDisc.Add(HtauIsoDisc_,qcd[i]->getNorm()); 
    delete HtauIsoDisc_;

  }

  for(Int_t i=0;i<NSAMP;i++)
    delete qcd[i];


  //Compute the ratios here ;
  TH1F*HTauJetPtR=(TH1F*)HTauJetPtI.Clone("HTauJetPtR");
  HTauJetPtR->Divide(&HTauJetPtA);
  TH1F* HTauJetPtRSS=(TH1F*)HTauJetPtISS.Clone("HTauJetPtRSS");
  HTauJetPtRSS->Divide(&HTauJetPtASS);

  TH1F*HMuJetPtR=(TH1F*)HMuJetPtI.Clone("HMuJetPtR");
  HMuJetPtR->Divide(&HMuJetPtA);
  TH1F* HMuJetPtRSS=(TH1F*)HMuJetPtISS.Clone("HMuJetPtRSS");
  HMuJetPtRSS->Divide(&HMuJetPtASS);

  TH1F* HTauJetPtIOSSS=(TH1F*)HTauJetPtI.Clone("HTauJetPtIOSSS");
  HTauJetPtIOSSS->Add(&HTauJetPtISS);
  TH1F* HTauJetPtAOSSS=(TH1F*)HTauJetPtA.Clone("HTauJetPtAOSSS");
  HTauJetPtAOSSS->Add(&HTauJetPtASS);
  TH1F* HTauJetPtROSSS=(TH1F*)HTauJetPtIOSSS->Clone("HTauJetPtROSSS");
  HTauJetPtROSSS->Divide(HTauJetPtAOSSS);

  TH1F* HMuJetPtIOSSS=(TH1F*)HMuJetPtI.Clone("HMuJetPtIOSSS");
  HMuJetPtIOSSS->Add(&HMuJetPtISS);
  TH1F* HMuJetPtAOSSS=(TH1F*)HMuJetPtA.Clone("HMuJetPtAOSSS");
  HMuJetPtAOSSS->Add(&HMuJetPtASS);
  TH1F* HMuJetPtROSSS=(TH1F*)HMuJetPtIOSSS->Clone("HMuJetPtROSSS");
  HMuJetPtROSSS->Divide(HMuJetPtAOSSS);

  

  //Save the histograms in ROOT file
  TFile RootFile("qcdRatio.root","recreate");

  HTauJetPtI.Write();
  HTauJetPtA.Write();

  HTauJetPtISS.Write();
  HTauJetPtASS.Write();
    
  HMuJetPtI.Write();
  HMuJetPtA.Write();

  HMuJetPtISS.Write();
  HMuJetPtASS.Write();

  HTauJetPtR->Write();
  HTauJetPtRSS->Write();

  HMuJetPtR->Write();
  HMuJetPtRSS->Write();

  HTauJetPtROSSS->Write();
  HMuJetPtROSSS->Write();

  HmTI.Write();

  HmTATau.Write();
  HmTARTau.Write();
  HmTARTauP.Write();
  HmTARTauM.Write();

  HmTAMu.Write();
  HmTARMu.Write();
  HmTARMuP.Write();
  HmTARMuM.Write();

  HmTATauMu.Write();
  HmTARTauMu.Write();
  HmTARTauMuP.Write();
  HmTARTauMuM.Write();

  HmuIso.Write();
  HtauIso.Write();
  HtauIsoDisc.Write();

  RootFile.ls();
  RootFile.Close();



}


void plotQcdRatio(){
  TFile F("qcdRatio.root");
  gROOT->cd();

  TH1F* HTauJetPtI=(TH1F*)F.Get("HTauJetPtI");
  TH1F* HTauJetPtA=(TH1F*)F.Get("HTauJetPtA");

  TH1F* HTauJetPtISS=(TH1F*)F.Get("HTauJetPtISS");
  TH1F* HTauJetPtASS=(TH1F*)F.Get("HTauJetPtASS");

  TH1F* HMuJetPtI=(TH1F*)F.Get("HMuJetPtI");
  TH1F* HMuJetPtA=(TH1F*)F.Get("HMuJetPtA");

  TH1F* HMuJetPtISS=(TH1F*)F.Get("HMuJetPtISS");
  TH1F* HMuJetPtASS=(TH1F*)F.Get("HMuJetPtASS");

  TH1F *HTauJetPtR=(TH1F*)F.Get("HTauJetPtR");
  TH1F *HTauJetPtRSS=(TH1F*)F.Get("HTauJetPtRSS");
  TH1F *HMuJetPtR=(TH1F*)F.Get("HMuJetPtR");
  TH1F *HMuJetPtRSS=(TH1F*)F.Get("HMuJetPtRSS");
  TH1F *HTauJetPtROSSS=(TH1F*)F.Get("HTauJetPtROSSS");
  TH1F *HMuJetPtROSSS=(TH1F*)F.Get("HMuJetPtROSSS");
  


  TString file=TString("qcdRatio.ps");
  TCanvas C(file);
  TLine line;

  C.Print(file+"[");
  
  //Tau ratio SS vs OS
  C.Clear();
  C.SetLogy(1);
  HTauJetPtR->GetYaxis()->SetRangeUser(1e-3,50);
  HTauJetPtR->Draw("histpe");
  HTauJetPtRSS->SetMarkerColor(2);
  HTauJetPtRSS->SetLineColor(2);
  HTauJetPtRSS->Draw("histpesame");
  C.Print(file);
  C.SetLogy(0);
  C.Clear();
  TH1F*HTauJetPtRSSC=(TH1F*)HTauJetPtRSS->Clone("HTauJetPtRSSC");
  HTauJetPtRSSC->Divide(HTauJetPtR);
  HTauJetPtRSSC->GetYaxis()->SetRangeUser(0,2);
  HTauJetPtRSSC->Draw("histpe");
  line.DrawLine(HTauJetPtRSSC->GetXaxis()->GetXmin(),1,HTauJetPtRSSC->GetXaxis()->GetXmax(),1);
  C.Print(file);

  //Mu ratio SS vs OS
  C.Clear();
  C.SetLogy(1);
  HMuJetPtR->GetYaxis()->SetRangeUser(1e-3,50);
  HMuJetPtR->Draw("histpe");
  HMuJetPtRSS->SetMarkerColor(2);
  HMuJetPtRSS->SetLineColor(2);
  HMuJetPtRSS->Draw("histpesame");
  C.Print(file);
  C.SetLogy(0);
  C.Clear();
  TH1F*HMuJetPtRSSC=(TH1F*)HMuJetPtRSS->Clone("HMuJetPtRSSC");
  HMuJetPtRSSC->Divide(HMuJetPtR);
  HMuJetPtRSSC->GetYaxis()->SetRangeUser(0,2);
  HMuJetPtRSSC->Draw("histpe");
  line.DrawLine(HMuJetPtRSSC->GetXaxis()->GetXmin(),1,HMuJetPtRSSC->GetXaxis()->GetXmax(),1);
  C.Print(file);



  //Tau Jet Ratio for OS+SS 
//   C.Clear();
//   HTauJetPtAOSSS->Draw("histpe");
//   HTauJetPtIOSSS->Draw("histpesame");
//   C.Print(file);
  C.Clear();
  C.SetLogy(1);
  HTauJetPtROSSS->GetYaxis()->SetRangeUser(1e-3,50);
  HTauJetPtROSSS->Draw("histpe");
  C.Print(file);
  C.SetLogy(0);

  cout<<"OS+SS ratio"<<endl;
  cout.precision(3);
  cout<<"(";
  for(Int_t b=1;b<=HTauJetPtROSSS->GetNbinsX();b++){
    if(HTauJetPtROSSS->GetBinContent(b)>0){
      cout<<"("<<HTauJetPtROSSS->GetBinLowEdge(b)<<"<"<<HTauJetPtROSSS->GetXaxis()->GetTitle()<<"&&"<<HTauJetPtROSSS->GetXaxis()->GetTitle()<<"<="<<HTauJetPtROSSS->GetBinLowEdge(b)+HTauJetPtROSSS->GetBinWidth(b)<<")*";
      cout<<HTauJetPtROSSS->GetBinContent(b)<<"+";
    }
  }
  cout<<")"<<endl;  
  cout<<"(";
  for(Int_t b=1;b<=HTauJetPtROSSS->GetNbinsX();b++){
    if(HTauJetPtROSSS->GetBinContent(b)>0){
      cout<<"("<<HTauJetPtROSSS->GetBinLowEdge(b)<<"<"<<HTauJetPtROSSS->GetXaxis()->GetTitle()<<"&&"<<HTauJetPtROSSS->GetXaxis()->GetTitle()<<"<="<<HTauJetPtROSSS->GetBinLowEdge(b)+HTauJetPtROSSS->GetBinWidth(b)<<")*";
      cout<<HTauJetPtROSSS->GetBinContent(b)+HTauJetPtROSSS->GetBinError(b)<<"+";//+1sigma
    }
  }
  cout<<")"<<endl;  
  cout<<"(";
  for(Int_t b=1;b<=HTauJetPtROSSS->GetNbinsX();b++){
    if(HTauJetPtROSSS->GetBinContent(b)>0){
      cout<<"("<<HTauJetPtROSSS->GetBinLowEdge(b)<<"<"<<HTauJetPtROSSS->GetXaxis()->GetTitle()<<"&&"<<HTauJetPtROSSS->GetXaxis()->GetTitle()<<"<="<<HTauJetPtROSSS->GetBinLowEdge(b)+HTauJetPtROSSS->GetBinWidth(b)<<")*";
      cout<<HTauJetPtROSSS->GetBinContent(b)-HTauJetPtROSSS->GetBinError(b)<<"+";//-1sigma
    }
  }
  cout<<")"<<endl;  


  //Mu Jet Ratio for OS+SS 
//   C.Clear();
//   HMuJetPtAOSSS->Draw("histpe");
//   HMuJetPtIOSSS->Draw("histpesame");
//  C.Print(file);
  C.Clear();
  C.SetLogy(1);
  HMuJetPtROSSS->GetYaxis()->SetRangeUser(1e-3,50);
  HMuJetPtROSSS->Draw("histpe");
  C.Print(file);
  C.SetLogy(0);

  cout<<"Muon OS+SS ratio"<<endl;
  cout.precision(3);
  cout<<"(";
  for(Int_t b=1;b<=HMuJetPtROSSS->GetNbinsX();b++){
    if(HMuJetPtROSSS->GetBinContent(b)>0){
      cout<<"("<<HMuJetPtROSSS->GetBinLowEdge(b)<<"<"<<HMuJetPtROSSS->GetXaxis()->GetTitle()<<"&&"<<HMuJetPtROSSS->GetXaxis()->GetTitle()<<"<="<<HMuJetPtROSSS->GetBinLowEdge(b)+HMuJetPtROSSS->GetBinWidth(b)<<")*";
      cout<<HMuJetPtROSSS->GetBinContent(b)<<"+";
    }
  }
  cout<<")"<<endl;  
  cout<<"(";
  for(Int_t b=1;b<=HMuJetPtROSSS->GetNbinsX();b++){
    if(HMuJetPtROSSS->GetBinContent(b)>0){
      cout<<"("<<HMuJetPtROSSS->GetBinLowEdge(b)<<"<"<<HMuJetPtROSSS->GetXaxis()->GetTitle()<<"&&"<<HMuJetPtROSSS->GetXaxis()->GetTitle()<<"<="<<HMuJetPtROSSS->GetBinLowEdge(b)+HMuJetPtROSSS->GetBinWidth(b)<<")*";
      cout<<HMuJetPtROSSS->GetBinContent(b)+HMuJetPtROSSS->GetBinError(b)<<"+";//+1sigma
    }
  }
  cout<<")"<<endl;  
  cout<<"(";
  for(Int_t b=1;b<=HMuJetPtROSSS->GetNbinsX();b++){
    if(HMuJetPtROSSS->GetBinContent(b)>0){
      cout<<"("<<HMuJetPtROSSS->GetBinLowEdge(b)<<"<"<<HMuJetPtROSSS->GetXaxis()->GetTitle()<<"&&"<<HMuJetPtROSSS->GetXaxis()->GetTitle()<<"<="<<HMuJetPtROSSS->GetBinLowEdge(b)+HMuJetPtROSSS->GetBinWidth(b)<<")*";
      cout<<HMuJetPtROSSS->GetBinContent(b)-HMuJetPtROSSS->GetBinError(b)<<"+";//-1sigma
    }
  }
  cout<<")"<<endl;  



  C.Print(file+"]");



  F.Close();

}





void compareQCDRatioPrediction(){

  TFile MC("qcdRatio.root");

  TH1F*HI=(TH1F*)MC.Get("HmTI");
  TH1F*HATau=(TH1F*)MC.Get("HmTATau");
  TH1F*HAMu=(TH1F*)MC.Get("HmTAMu");
  TH1F*HATauMu=(TH1F*)MC.Get("HmTATauMu");
 

  TH1F*HARTau=(TH1F*)MC.Get("HmTARTau");
  TH1F*HARTauP=(TH1F*)MC.Get("HmTARTauP");
  TH1F*HARTauM=(TH1F*)MC.Get("HmTARTauM");
  //add error on ratio
  for(Int_t b=1;b<=HARTau->GetNbinsX();b++){
    float deltaP=fabs(HARTauP->GetBinContent(b)-HARTau->GetBinContent(b));
    float deltaM=fabs(HARTau->GetBinContent(b)-HARTauM->GetBinContent(b));
    HARTau->SetBinError(b,sqrt(HARTau->GetBinError(b)*HARTau->GetBinError(b) + pow(TMath::Max(deltaP,deltaM),2)));
  }

  TH1F*HARMu=(TH1F*)MC.Get("HmTARMu");
  TH1F*HARMuP=(TH1F*)MC.Get("HmTARMuP");
  TH1F*HARMuM=(TH1F*)MC.Get("HmTARMuM");
  //add error on ratio
  for(Int_t b=1;b<=HARMu->GetNbinsX();b++){
    float deltaP=fabs(HARMuP->GetBinContent(b)-HARMu->GetBinContent(b));
    float deltaM=fabs(HARMu->GetBinContent(b)-HARMuM->GetBinContent(b));
    HARMu->SetBinError(b,sqrt(HARMu->GetBinError(b)*HARMu->GetBinError(b) + pow(TMath::Max(deltaP,deltaM),2)));
  }

  TH1F*HARTauMu=(TH1F*)MC.Get("HmTARTauMu");
  TH1F*HARTauMuP=(TH1F*)MC.Get("HmTARTauMuP");
  TH1F*HARTauMuM=(TH1F*)MC.Get("HmTARTauMuM");
  //add error on ratio
  for(Int_t b=1;b<=HARTauMu->GetNbinsX();b++){
    float deltaP=fabs(HARTauMuP->GetBinContent(b)-HARTauMu->GetBinContent(b));
    float deltaM=fabs(HARTauMu->GetBinContent(b)-HARTauMuM->GetBinContent(b));
    HARTauMu->SetBinError(b,sqrt(HARTauMu->GetBinError(b)*HARTauMu->GetBinError(b) + pow(TMath::Max(deltaP,deltaM),2)));
  }


  TString file="compareQCDRatioPrediction.ps";
  TCanvas C(file);
  TLine line;

  C.Print(file+"[");

  //from anti-iso taus
  C.Clear();
  C.SetLogy(1);
  HATau->GetYaxis()->SetRangeUser(1,1000000);
  HATau->Draw("hist");
  HARTau->Draw("histpesame");
  HI->SetMarkerColor(2);
  HI->SetLineColor(2);
  HI->Draw("histpesame");
  C.Print(file);
  Float_t HIYieldEr=0;
  for(Int_t b=1;b<=HI->GetNbinsX();b++)
    HIYieldEr+=HI->GetBinError(b)*HI->GetBinError(b);
  Float_t HIYieldAREr=0;
  for(Int_t b=1;b<=HI->GetNbinsX();b++)
    HIYieldAREr+=HARTau->GetBinError(b)*HARTau->GetBinError(b);
  cout<<"Isolated = "<<HI->Integral()<<" +- "<<sqrt(HIYieldEr)<<"  Anti-Iso-Tau*R = "<<HARTau->Integral()<<" +- "<<sqrt(HIYieldAREr)<<endl;
  
  //from anti-iso muons
  C.Clear();
  C.SetLogy(1);
  HAMu->GetYaxis()->SetRangeUser(1,1000000);
  HAMu->Draw("hist");
  HARMu->Draw("histpesame");
  HI->SetMarkerColor(2);
  HI->SetLineColor(2);
  HI->Draw("histpesame");
  C.Print(file);
  HIYieldAREr=0;
  for(Int_t b=1;b<=HI->GetNbinsX();b++)
    HIYieldAREr+=HARMu->GetBinError(b)*HARMu->GetBinError(b);
  cout<<"Isolated = "<<HI->Integral()<<" +- "<<sqrt(HIYieldEr)<<"  Anti-Iso-Mu*R = "<<HARMu->Integral()<<" +- "<<sqrt(HIYieldAREr)<<endl;

  //from anti-iso-Tau and anti-iso-Mu
  C.Clear();
  C.SetLogy(1);
  HATauMu->GetYaxis()->SetRangeUser(1,1000000);
  HATauMu->Draw("hist");
  HARTauMu->Draw("histpesame");
  HI->SetMarkerColor(2);
  HI->SetLineColor(2);
  HI->Draw("histpesame");
  C.Print(file);
  HIYieldAREr=0;
  for(Int_t b=1;b<=HI->GetNbinsX();b++)
    HIYieldAREr+=HARTauMu->GetBinError(b)*HARTauMu->GetBinError(b);
  cout<<"Isolated = "<<HI->Integral()<<" +- "<<sqrt(HIYieldEr)<<"  Anti-Iso-TauMu*R = "<<HARTauMu->Integral()<<" +- "<<sqrt(HIYieldAREr)<<endl;
  
  C.Print(file+"]");

}



void compareQCDRatioData(){

  TFile MC("qcdRatio.root");

  //TFile DataTau("/data/benitezj/Samples/Mar19MuJet/TauMuPlotterIsoFakeRate_taujetptMTcat1_SMcat-11X_muisoL0.1_.root");  
  TFile DataTau("/data/benitezj/Samples/Mar19MuJet/TauMuPlotterIsoFakeRate_taujetptMTcat1_SMcat-11.root");   

  //TFile DataMu("/data/benitezj/Samples/Mar19MuJet/TauMuPlotterIsoFakeRate_mujetptMTcat1_SMcat-11X_tauisodiscMQ2_.root");
  TFile DataMu("/data/benitezj/Samples/Mar19MuJet/TauMuPlotterIsoFakeRate_mujetptMTcat1_SMcat-11.root");

  TH1F*HMCTau=(TH1F*)MC.Get("HTauJetPtROSSS");
  TH1F*HMCMu=(TH1F*)MC.Get("HMuJetPtROSSS");
  TH1F*HDataTau=(TH1F*)DataTau.Get("hRatio");
  TH1F*HDataMu=(TH1F*)DataMu.Get("hRatio");


  TString file="compareQCDRatioData.ps";
  TCanvas C(file);
  TLine line;

  C.Print(file+"[");

  C.Clear();
  C.SetLogy(1);
  HDataTau->GetYaxis()->SetRangeUser(1e-3,50);
  HDataTau->Draw("histpe");
  HMCTau->SetMarkerColor(2);
  HMCTau->SetLineColor(2);
  HMCTau->Draw("histpesame");
  C.Print(file);

  TH1F*HMCTauC=(TH1F*)HMCTau->Clone("HMCTauC");
  HMCTauC->Divide(HDataTau);
  C.Clear();
  C.SetLogy(0);
  HMCTauC->GetYaxis()->SetRangeUser(0,2);
  HMCTauC->Draw("histpe");
  line.DrawLine(HMCTauC->GetXaxis()->GetXmin(),1,HMCTauC->GetXaxis()->GetXmax(),1);
  C.Print(file);


  C.Clear();
  C.SetLogy(1);
  HDataMu->GetYaxis()->SetRangeUser(1e-3,50);
  HDataMu->Draw("histpe");
  HMCMu->SetMarkerColor(2);
  HMCMu->SetLineColor(2);
  HMCMu->Draw("histpesame");
  C.Print(file);

  TH1F*HMCMuC=(TH1F*)HMCMu->Clone("HMCMuC");
  HMCMuC->Divide(HDataMu);
  C.Clear();
  C.SetLogy(0);
  HMCMuC->GetYaxis()->SetRangeUser(0,2);
  HMCMuC->Draw("histpe");
  line.DrawLine(HMCMuC->GetXaxis()->GetXmin(),1,HMCMuC->GetXaxis()->GetXmax(),1);
  C.Print(file);
  
  C.Print(file+"]");

}


void compareIso(){

  TFile FMC("qcdRatio.root");
  TH1F* HMCmuIso=(TH1F*)FMC.Get("HmuIso");
  TH1F* HMCtauIso=(TH1F*)FMC.Get("HtauIso");
  TH1F* HMCtauIsoDisc=(TH1F*)FMC.Get("HtauIsoDisc");

  TFile FDataMu("/data/benitezj/Samples/Mar19MuJet/DiffFileIncSS_muiso_Iso-1_MT1_SM-1.root");
  TH1F* HDatamuIso=(TH1F*)FDataMu.Get("hDiff");
  TFile FDataTau("/data/benitezj/Samples/Mar19MuJet/DiffFileIncSS_tauiso_Iso-1_MT1_SM-1.root");
  TH1F* HDatatauIso=(TH1F*)FDataTau.Get("hDiff");
  TFile FDataTauDisc("/data/benitezj/Samples/Mar19MuJet/DiffFileIncSS_tauisodisc_Iso-1_MT1_SM-1.root");
  TH1F* HDatatauIsoDisc=(TH1F*)FDataTauDisc.Get("hDiff");

												    
  TString file="compareIso.ps";
  TCanvas C(file);
  
  C.Print(file+"[");
  
  C.Clear();
  HDatamuIso->Draw();
  HMCmuIso->Scale(HDatamuIso->Integral()/HMCmuIso->Integral());
  HMCmuIso->Draw("histsame");
  C.Print(file);

  C.Clear();
  HDatatauIso->Draw();
  HMCtauIso->Scale(HDatatauIso->Integral()/HMCtauIso->Integral());
  HMCtauIso->Draw("histsame");
  C.Print(file);
  
  C.Clear();
  HDatatauIsoDisc->Draw();
  HMCtauIsoDisc->Scale(HDatatauIsoDisc->Integral()/HMCtauIsoDisc->Integral());
  HMCtauIsoDisc->Draw("histsame");
  C.Print(file);
  
  C.Print(file+"]");
}




void compareQCDRatioNjet(TString jet="taujetpt"){

  TFile F0(TString("qcdRatio_")+jet+"_nJet0.root");
  TFile F1(TString("qcdRatio_")+jet+"_nJet1.root");
  TFile F2(TString("qcdRatio_")+jet+"_nJet2.root");


  TH1F*H0=(TH1F*)F0.Get("HTauJetPtROSSS");
  TH1F*H1=(TH1F*)F1.Get("HTauJetPtROSSS");
  TH1F*H2=(TH1F*)F2.Get("HTauJetPtROSSS");


  TH1F*HmTI0=(TH1F*)F0.Get("HmTI");
  TH1F*HmTA0=(TH1F*)F0.Get("HmTA");

  TH1F*HmTI1=(TH1F*)F1.Get("HmTI");
  TH1F*HmTA1=(TH1F*)F1.Get("HmTA");


  TString file=TString("compareQCDRatioNjet_")+jet+".ps";
  TCanvas C(file);
  C.Print(file+"[");
  TLine line;


  //njet1/njet0
  C.Clear();
  C.SetLogy(1);
  H0->GetYaxis()->SetRangeUser(1e-3,50);
  H0->Draw("histpe");
  H1->SetMarkerColor(2);
  H1->SetLineColor(2);
  H1->Draw("histpesame");
  C.Print(file);

  C.Clear();
  C.SetLogy(0);
  TH1F*H1C=(TH1F*)H1->Clone("H1C");
  H1C->Divide(H0);
  H1C->GetYaxis()->SetRangeUser(0,2);
  H1C->Draw("histpe");
  line.DrawLine(H1C->GetXaxis()->GetXmin(),1,H1C->GetXaxis()->GetXmax(),1);
  C.Print(file);

  

  //njet1/njet0
  C.Clear();
  C.SetLogy(1);
  H0->GetYaxis()->SetRangeUser(1e-3,50);
  H0->Draw("histpe");
  H2->SetMarkerColor(2);
  H2->SetLineColor(2);
  H2->Draw("histpesame");
  C.Print(file);

  C.Clear();
  C.SetLogy(0);
  TH1F*H2C=(TH1F*)H2->Clone("H2C");
  H2C->Divide(H0);
  H2C->GetYaxis()->SetRangeUser(0,2);
  H2C->Draw("histpe");
  line.DrawLine(H2C->GetXaxis()->GetXmin(),1,H2C->GetXaxis()->GetXmax(),1);
  C.Print(file);

  

  ///Compare ratio of mT between njet1/njet0
  TH1F*HmTR0=(TH1F*)HmTI0->Clone("HmTR0");
  HmTR0->Divide(HmTA0);
  TH1F*HmTR1=(TH1F*)HmTI1->Clone("HmTR1");
  HmTR1->Divide(HmTA1);
  C.Clear();
  HmTR0->GetYaxis()->SetRangeUser(0,0.2);
  HmTR0->Draw("histpe");
  HmTR1->SetMarkerColor(2);
  HmTR1->SetLineColor(2);
  HmTR1->Draw("histpesame");
  line.DrawLine(H2C->GetXaxis()->GetXmin(),1,H2C->GetXaxis()->GetXmax(),1);
  C.Print(file);
  


  C.Print(file+"]");

}

