

void CrystalBall(){

  TF1* fCBall = new TF1("fCBall", CBall, 400, 4600., 8);
  fCBall->SetParameters(989.3, 71., 0.8, 0.3, 137., 58., 0.64, 100.);

  double integral = fCBall->Integral(400., 4600.);
  double parameter = fCBall->GetParameter(7);
  fCBall->SetParameter(7, parameter/integral);

  cout << "Integral = " << fCBall->Integral(400., 4600.) << endl;

  fCBall->Draw();

  TFile* _shapes = new TFile("/afs/cern.ch/user/m/mgouzevi/scratch0/CMGTools/CMSSW_4_2_8/src/StatTools/BayesianDijetFit/Results/Resonance_Shapes_Qstar_2012_D6T_ak5_fat30_save.root");
  int i = 0;
  TH1F* shape = (TH1F*) _shapes->Get(Form("h_qstar_%d;1", 1000+i*100));
  
  double integral = shape->Integral();
  shape->Scale(1./integral);

  for (int j = 1; j < shape->GetNbinsX()+1; j++){
    double weight = shape->GetBinContent(j);
    double binWidth = shape->GetBinWidth(j);
    shape->SetBinContent(j, weight/binWidth);
  }
  shape->Draw("same");


}

Double_t CBall(Double_t *x, Double_t *p)
{
  double mean = p[0], sigma = p[1], alphaHigh = p[2], alphaLow = p[3], nHigh = p[4], nLow = p[5], fact = p[6], norm = p[7];
  
  double pull = (x[0]-mean)/sigma;

  if (pull > -alphaLow && pull < alphaHigh) {
    return norm*TMath::Exp(-pull*pull/2);
  } else if ((x[0]-mean)/sigma <= -alphaLow) {

    double A = pow(nLow/fabs(alphaLow),nLow)*TMath::Exp(-alphaLow*alphaLow/2);
    double B = nLow/fabs(alphaLow)-fabs(alphaLow);

    return fact*norm*A*TMath::Power((B-pull),-nLow);

  } else if(pull >=  alphaHigh) {

    double A = pow(nHigh/fabs(alphaHigh),nHigh)*TMath::Exp(-alphaHigh*alphaHigh/2);
    double B = nHigh/fabs(alphaHigh)-fabs(alphaHigh);

    return (1-fact)*norm*A*TMath::Power((B+pull),-nHigh);

  }
  return 0;

}
