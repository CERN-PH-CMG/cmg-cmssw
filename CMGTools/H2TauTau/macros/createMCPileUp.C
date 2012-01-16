bool AddMCPUPHistos(){
  
  ///adding histos obtained by running over a sample

  TH1D MCPUPHisto("pileup","",50,-.5,49.5);//must include 0
  MCPUPHisto.Sumw2();

  
  TFile F1("./MCPUP1.root","read");
  MCPUPHisto.Add((TH1D*)F1.Get("pileupRaw"));

  TFile F2("./MCPUP2.root","read");
  MCPUPHisto.Add((TH1D*)F2.Get("pileupRaw"));

  TFile F3("./MCPUP3.root","read");
  MCPUPHisto.Add((TH1D*)F3.Get("pileupRaw"));

  TFile F4("./MCPUP4.root","read");
  MCPUPHisto.Add((TH1D*)F4.Get("pileupRaw"));

  TFile F5("./MCPUP5.root","read");
  MCPUPHisto.Add((TH1D*)F5.Get("pileupRaw"));

  TFile F6("./MCPUP6.root","read");
  MCPUPHisto.Add((TH1D*)F6.Get("pileupRaw"));

  MCPUPHisto.Scale(1./MCPUPHisto.Integral());
  TFile F("./MCPUP.root","recreate");
  if(F.IsZombie())return 0;
  MCPUPHisto.Write();
  F.Close();

  return 1;
}

bool CreateMCPileUpFall11(){

  TH1D MCPUPHisto("pileup","",50,-.5,49.5);//must include 0


  // In-time (Poisson-smeared) Distribution for Fall 2011

  double Fall2011_InTime[55] = {
    0.014583699,
    0.025682975,
    0.038460562,
    0.049414536,
    0.056931087,
    0.061182816,
    0.062534625,
    0.061476918,
    0.058677499,
    0.055449877,
    0.051549051,
    0.047621024,
    0.043923799,
    0.040569076,
    0.037414654,
    0.034227033,
    0.031437714,
    0.028825596,
    0.026218978,
    0.023727061,
    0.021365645,
    0.01918743,
    0.016972815,
    0.014920601,
    0.013038989,
    0.011293777,
    0.009612465,
    0.008193556,
    0.006888047,
    0.005715239,
    0.004711232,
    0.003869926,
    0.003154521,
    0.002547417,
    0.002024714,
    0.001574411,
    0.001245808,
    0.000955206,
    0.000735305,
    0.000557304,
    0.000412503,
    0.000305502,
    0.000231002,
    0.000165701,
    0.000121201,
    9.30006E-05,
    6.40004E-05,
    4.22003E-05,
    2.85002E-05,
    1.96001E-05,
    1.59001E-05,
    1.01001E-05,
    8.50006E-06,
    6.60004E-06,
    2.70002E-06
  };


  for(Int_t i=0;i<50;i++)
    MCPUPHisto.SetBinContent(i+1,Fall2011_InTime[i]);
  
  MCPUPHisto.Scale(1./MCPUPHisto.Integral());
  TFile F("./MCPUPFall.root","recreate");
  if(F.IsZombie())return 0;
  MCPUPHisto.Write();
  F.Close();

  return 1;
}




bool CreateMCPileUp3D(){

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



  for(Int_t i=0;i<25;i++)
    MCPUPHisto.SetBinContent(i+1,PoissonIntDist[i]);

  MCPUPHisto.Scale(1./MCPUPHisto.Integral());
  TFile F("./MCPUP3D.root","recreate");
  if(F.IsZombie())return 0;
  MCPUPHisto.Write();
  F.Close();

  return 1;
}



bool CreateMCPileUp3DFall(){

  TH1D MCPUPHisto("pileup","",50,-.5,49.5);//must include 0
  MCPUPHisto.Sumw2();

  //"true" distribution taken from here: 
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupMCReweightingUtilities#3D_Reweighting

  // Distribution used for Fall2011 MC.  
  Double_t Fall2011[50] = {
    0.003388501,
    0.010357558,
    0.024724258,
    0.042348605,
    0.058279812,
    0.068851751,
    0.072914824,
    0.071579609,
    0.066811668,
    0.060672356,
    0.054528356,
    0.04919354,
    0.044886042,
    0.041341896,
    0.0384679,
    0.035871463,
    0.03341952,
    0.030915649,
    0.028395374,
    0.025798107,
    0.023237445,
    0.020602754,
    0.0180688,
    0.015559693,
    0.013211063,
    0.010964293,
    0.008920993,
    0.007080504,
    0.005499239,
    0.004187022,
    0.003096474,
    0.002237361,
    0.001566428,
    0.001074149,
    0.000721755,
    0.000470838,
    0.00030268,
    0.000184665,
    0.000112883,
    6.74043E-05,
    3.82178E-05,
    2.22847E-05,
    1.20933E-05,
    6.96173E-06,
    3.4689E-06,
    1.96172E-06,
    8.49283E-07,
    5.02393E-07,
    2.15311E-07,
    9.56938E-08
  };


  for(Int_t i=0;i<50;i++)
    MCPUPHisto.SetBinContent(i+1,Fall2011[i]);

  MCPUPHisto.Scale(1./MCPUPHisto.Integral());
  TFile F("./MCPUP3DFall.root","recreate");
  if(F.IsZombie())return 0;
  MCPUPHisto.Write();
  F.Close();

  return 1;
}
