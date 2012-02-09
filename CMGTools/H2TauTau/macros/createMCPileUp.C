
bool createMCPUPHisto(){

  //create a histogram with the MC PUP distribution
  //this must be done with unselected MC events
  TH1D MCPUPHisto("pileup","",36,-.5,35.5);//must include 0
  MCPUPHisto.Sumw2();

  std::vector<std::string> list;
  list.push_back("/tmp/tree_CMG_0.root");
  list.push_back("/tmp/tree_CMG_1.root");
  list.push_back("/tmp/tree_CMG_2.root");
  list.push_back("/tmp/tree_CMG_3.root");
  list.push_back("/tmp/tree_CMG_4.root");
  list.push_back("/tmp/tree_CMG_5.root");
  list.push_back("/tmp/tree_CMG_6.root");
  list.push_back("/tmp/tree_CMG_7.root");
  list.push_back("/tmp/tree_CMG_8.root");
  list.push_back("/tmp/tree_CMG_9.root");
  list.push_back("/tmp/tree_CMG_10.root");
  list.push_back("/tmp/tree_CMG_11.root");
  list.push_back("/tmp/tree_CMG_12.root");
  list.push_back("/tmp/tree_CMG_13.root");
  list.push_back("/tmp/tree_CMG_14.root");
  list.push_back("/tmp/tree_CMG_15.root");
  list.push_back("/tmp/tree_CMG_16.root");
  list.push_back("/tmp/tree_CMG_17.root");
  list.push_back("/tmp/tree_CMG_18.root");
  list.push_back("/tmp/tree_CMG_19.root");
  list.push_back("/tmp/tree_CMG_20.root");
  list.push_back("/tmp/tree_CMG_21.root");
  list.push_back("/tmp/tree_CMG_22.root");
  list.push_back("/tmp/tree_CMG_23.root");
  list.push_back("/tmp/tree_CMG_24.root");
  list.push_back("/tmp/tree_CMG_25.root");
  list.push_back("/tmp/tree_CMG_26.root");
  list.push_back("/tmp/tree_CMG_27.root");
  list.push_back("/tmp/tree_CMG_28.root");
  list.push_back("/tmp/tree_CMG_29.root");
  list.push_back("/tmp/tree_CMG_30.root");
  fwlite::ChainEvent chain(list);


  Int_t ievt=0;
  for(chain.toBegin(); !chain.atEnd() ; ++chain, ++ievt){
    if(ievt%printFreq_==0)cout<<ievt<<" done"<<endl;
    const fwlite::Event * event = chain.event();

    //get the number of pile up vertexes
    edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    event->getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
    std::vector<PileupSummaryInfo>::const_iterator PVI;  
    int npv=-1;
    for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {    
      int BX = PVI->getBunchCrossing();    
      if(BX == 0) { 
        npv = PVI->getPU_NumInteractions();
        continue;
      }    
    }
    MCPUPHisto.Fill(npv);
    
  }

  MCPUPHisto.Scale(1./MCPUPHisto.Integral());
  TFile F("./MCPUP.root","recreate");
  if(F.IsZombie())return 0;
  MCPUPHisto.Write();
  F.Close();

  return 1;
}



bool createMCPUP3DHisto(){

  TH1D MCPUPHisto("pileup","",25,-.5,24.5);//must include 0
  MCPUPHisto.Sumw2();

  //"true" distribution taken from here: 
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupMCReweightingUtilities#3D_Reweighting

  // Summer11 PU_S4, distribution obtained by averaging the number of interactions
  // in each beam crossing to estimate the true mean.  
  Double_t PoissonIntDist[25] = {
    0.104109,
    0.0703573,
    0.0698445,
    0.0698254,
    0.0697054,
    0.0697907,
    0.0696751,
    0.0694486,
    0.0680332,
    0.0651044,
    0.0598036,
    0.0527395,
    0.0439513,
    0.0352202,
    0.0266714,
    0.019411,
    0.0133974,
    0.00898536,
    0.0057516,
    0.00351493,
    0.00212087,
    0.00122891,
    0.00070592,
    0.000384744,
    0.000219377
  };


//   // I think this corresponds to the "true" distribution, check the number of bins
//   // Distribution used for Fall2011 MC.  
//   Double_t Fall2011[50] = {
//     0.003388501,
//     0.010357558,
//     0.024724258,
//     0.042348605,
//     0.058279812,
//     0.068851751,
//     0.072914824,
//     0.071579609,
//     0.066811668,
//     0.060672356,
//     0.054528356,
//     0.04919354,
//     0.044886042,
//     0.041341896,
//     0.0384679,
//     0.035871463,
//     0.03341952,
//     0.030915649,
//     0.028395374,
//     0.025798107,
//     0.023237445,
//     0.020602754,
//     0.0180688,
//     0.015559693,
//     0.013211063,
//     0.010964293,
//     0.008920993,
//     0.007080504,
//     0.005499239,
//     0.004187022,
//     0.003096474,
//     0.002237361,
//     0.001566428,
//     0.001074149,
//     0.000721755,
//     0.000470838,
//     0.00030268,
//     0.000184665,
//     0.000112883,
//     6.74043E-05,
//     3.82178E-05,
//     2.22847E-05,
//     1.20933E-05,
//     6.96173E-06,
//     3.4689E-06,
//     1.96172E-06,
//     8.49283E-07,
//     5.02393E-07,
//     2.15311E-07,
//     9.56938E-08
//   };




  for(Int_t b=0;b<25;b++)
    MCPUPHisto.SetBinContent(b,PoissonIntDist[b]);

  MCPUPHisto.Scale(1./MCPUPHisto.Integral());
  TFile F("./MCPUP3D.root","recreate");
  if(F.IsZombie())return 0;
  MCPUPHisto.Write();
  F.Close();

  return 1;
}
