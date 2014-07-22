void std_pileupreweighting(){

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

  // In-time (Poisson-smeared) Distribution for Fall 2011
  // double Fall2011_InTime[55] = {
  double Fall2011_InTime[50] = {
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
  1.96001E-05
  // 1.59001E-05,
  // 1.01001E-05,
  // 8.50006E-06,
  // 6.60004E-06,
  // 2.70002E-06
  };
  
  TH1D*hpileup_Fall11=new TH1D("hpileup_Fall11","hpileup_Fall11",50,0,50);
  
  for(int i=0;i<50;i++){
    hpileup_Fall11->Fill(i,Fall2011[i]);
  }
  
  TFile*fout=new TFile("pileup_Fall11.root","RECREATE");
  hpileup_Fall11->Scale(1/hpileup_Fall11->Integral());
  hpileup_Fall11->Write();
  fout->Close();
  
  // TFile*f2011=new TFile("pileup_data.root");
  // TH1D*hpileup_DATA11=(TH1D*)f2011->Get("pileup7TeV");
  TFile*f2011=new TFile("pileupTruth_Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.root");
  TH1D*hpileup_DATA11=(TH1D*)f2011->Get("pileup");
  hpileup_DATA11->Scale(1/hpileup_DATA11->Integral());
  
  TH1D pileup_reweighting_Fall11 = (*hpileup_DATA11)/(*hpileup_Fall11);
  pileup_reweighting_Fall11.SetBinContent(50,0);
  pileup_reweighting_Fall11.SetName("hpileup_reweighting_Fall11");
  pileup_reweighting_Fall11.SetTitle("hpileup_reweighting_Fall11");
  
  TFile*f2011=new TFile("pileup_reweighting_Fall11.root","RECREATE");
  pileup_reweighting_Fall11.Write();
  f2011->Write();
  f2011->Close();
  
}