void SignalBackground(){

  TCanvas* C = new TCanvas("C", "Gaussian fits", 1400, 1100);
  TCanvas* C1 = new TCanvas("C1", "Gaussian fits", 1400, 1100);
  C->Divide(1,2);

  string source("QstarToJJ_M-500_TuneD6T_ak5");
  TH1D* h_Mass_data_fat = new TH1D();

  TGraphErrors* Qstar_fat = new TGraphErrors(5); 
  TGraphErrors* RSgg_fat = new TGraphErrors(5); 
  TGraphErrors* RSqq_fat = new TGraphErrors(5); 

  TGraphErrors* Qstar_pf = new TGraphErrors(5); 
  TGraphErrors* RSgg_pf = new TGraphErrors(5); 
  TGraphErrors* RSqq_pf = new TGraphErrors(5); 

  TGraphErrors* Qstar_calo = new TGraphErrors(5); 
  TGraphErrors* RSgg_calo = new TGraphErrors(5); 
  TGraphErrors* RSqq_calo = new TGraphErrors(5); 

  TGraphErrors* Qstar;
  TGraphErrors* RSgg;
  TGraphErrors* RSqq;

    TF1* Bkg = new TF1("Bkg", "[0]*TMath::Power(1-x/7000.,[1])/TMath::Power(x/7000.,[2]+[3]*log(x/7000.))");
  // TF1* Bkg = new TF1("Bkg", "[0]*TMath::Power(1 - x/7000. - [3] * x * x /7000./7000.,[1])/TMath::Power(x/7000.,[2])");

  string obs("h_DijetMass_data_fat;1");

  for (int j = 0; j < 1; j++){

    switch(j){
      //    case 0: source = "data_HT_Run2011B_Begin_Fat_short_v2_ak5"; break;
    case 0: source = "data_HT_340fb_Fat_ak5"; break;  
//    case 0: source = "summer11_mc_All_ak5"; break;  
    default: break;
    }

    C->cd(j+1);

    string file = "out/res/histograms_"+ source + ".root";
    cout << source.c_str() << endl << endl;


    TFile *_file0 = TFile::Open(file.c_str());
    TH1D* h_Mass_data_fat = (TH1D*) _file0->Get(obs.c_str());
    /* 
    string obs("h_DijetMass_data_fat;1");
    h_Mass_data_fat->Add((TH1D*) _file0->Get(obs.c_str()));
    */

    Bkg->SetLineWidth(1);
    Bkg->SetLineColor(kRed);

    //   Bkg->SetParameters(3.91550e+16/1.6e+19, 2.75715e+00, 5.61080e+00, -1.15812e+00);
    Bkg->SetParameters(6.7e-6, 5.8, 6.8, 0.27);

    ndf = 0;
    h_Mass_data_fat->Sumw2();
    for (int i = 1; i <= h_Mass_data_fat->GetNbinsX(); i++ ){
      if (h_Mass_data_fat->GetBinCenter(i) > 837) ndf++;
      double error = h_Mass_data_fat->GetBinError(i)/h_Mass_data_fat->GetBinWidth(i)/928.;
      double content = h_Mass_data_fat->GetBinContent(i)/h_Mass_data_fat->GetBinWidth(i)/928.;
      if(content < 1e-10) error = 1.8/h_Mass_data_fat->GetBinWidth(i)/928., content = 1e-10;
      h_Mass_data_fat->SetBinError(i, error);      
      h_Mass_data_fat->SetBinContent(i, content);
      //      cout << " is the error right? " <<  h_Mass_data_fat->GetBinContent(i)*h_Mass_data_fat->GetBinWidth(i)*928. - h_Mass_data_fat->GetBinError(i)* h_Mass_data_fat->GetBinError(i)*h_Mass_data_fat->GetBinWidth(i)*928*h_Mass_data_fat->GetBinWidth(i)*928 << endl; 
      if (ndf > 32) h_Mass_data_fat->SetBinError(i, 0); //cout << "bin val maximum " << h_Mass_data_fat->GetBinLowEdge(i+1) << " is the error right? " <<  h_Mass_data_fat->GetBinContent(i) - h_Mass_data_fat->GetBinError(i)* h_Mass_data_fat->GetBinError(i) << endl; 
    }

    h_Mass_data_fat->Fit(Bkg, "", "", 838, 3854);
    //    h_Mass_data_fat->Fit(Bkg, "", "", 1000, 3854);

    TH1D* PLOTTER = new TH1D("PLOTTER", "", 1, 700, 4000);
    PLOTTER->SetMaximum(h_Mass_data_fat->GetMaximum()*1.2);
    PLOTTER->SetMinimum(1e-6);
    PLOTTER->SetLabelSize(0.05, "X");
    PLOTTER->SetLabelSize(0.05, "Y");
    PLOTTER->SetTitle("Mass Shape; M (GeV); pb/GeV");

    PLOTTER->DrawCopy();
    h_Mass_data_fat->Draw("SAME");

    gPad->SetLogy(1);
    gPad->Update();
    
    C->cd(2);
    PLOTTER->SetMaximum(3);
    PLOTTER->SetMinimum(-3);
    TH1D* h_Mass_data_fat_ratio = new TH1D();

    h_Mass_data_fat->Copy(*h_Mass_data_fat_ratio);
    h_Mass_data_fat_ratio->Sumw2();

    for (int i = 1; i <= h_Mass_data_fat->GetNbinsX()-2; i++ ){
      double signif = (h_Mass_data_fat->GetBinContent(i)-Bkg->Eval(h_Mass_data_fat->GetBinCenter(i)))/(h_Mass_data_fat->GetBinError(i)+1e-100);
      if (fabs(signif) > 100) signif = 0;
      cout << "bin " << i << " center = " << h_Mass_data_fat->GetBinCenter(i) << " value = " <<  h_Mass_data_fat->GetBinContent(i) << " signif = " << signif << endl;
      
      h_Mass_data_fat_ratio->SetBinContent(i, signif);
      h_Mass_data_fat_ratio->SetBinError(i, 1e-10); //h_Mass_data_fat->GetBinError(i)/Bkg->Eval(h_Mass_data_fat->GetBinCenter(i)));
    }
   
    PLOTTER->SetTitle("Pull; M (GeV); Pull");
    PLOTTER->DrawCopy();
    PLOTTER->SetStats(0);
    h_Mass_data_fat_ratio->DrawCopy("SAMEH");
    gPad->Update();

  }

  C->SaveAs("DEtator/MassFit_34fb_fat.png");

  C1->Clear();
  C1->Divide(5,2);
  
  for (int j = 1; j < 6; j++){

    string obs("h_Chi_Mass_data_fat;1");

    TH2D* h_Chi_Mass_data_fat = (TH2D*) _file0->Get(obs.c_str());

    /*
    string obs("h_Chi_Mass_data_fat;1");
    h_Chi_Mass_data_fat->Add((TH2D*) _file0->Get(obs.c_str()));
    */

    TH1D* h_Chi_Mass_data = h_Chi_Mass_data_fat->ProjectionY("h_Chi_Mass_data", j, j);

    C1->cd(j);
    
    TF1* Bkg1 = new TF1("Bkg1", "[0]*TMath::Power(1-x/7000.,[1])/TMath::Power(x/7000.,[2]+[3]*log(x/7000.))", 700, 4000);
    //  TF1* Bkg1 = new TF1("Bkg1", "[0]*TMath::Power(1 - x/7000. - [3] * x * x /7000./7000.,[1])/TMath::Power(x/7000.,[2])", 838, 3854);
    double norm = Bkg->GetParameter(0)*h_Chi_Mass_data->Integral()/h_Chi_Mass_data_fat->Integral();
    cout << "norm = " << norm << endl;
    Bkg1->SetParameters(norm, Bkg->GetParameter(1), Bkg->GetParameter(2), Bkg->GetParameter(3));
    
    
    ndf = 0;
    h_Chi_Mass_data->Sumw2();
    for (int i = 1; i <= h_Chi_Mass_data->GetNbinsX(); i++ ){
      if (h_Chi_Mass_data->GetBinCenter(i) > 837) ndf++;
      h_Chi_Mass_data->SetBinError(i, h_Chi_Mass_data->GetBinError(i)/h_Chi_Mass_data->GetBinWidth(i)/928.);      
      h_Chi_Mass_data->SetBinContent(i, h_Chi_Mass_data->GetBinContent(i)/h_Chi_Mass_data->GetBinWidth(i)/928.);
      if (ndf == 32) cout << "bin val maximum " << h_Chi_Mass_data->GetBinLowEdge(i+1) << endl; 
    }
    
    
    PLOTTER->SetMaximum(h_Chi_Mass_data->GetMaximum()*1.2);
    PLOTTER->SetMinimum(1e-6);

    PLOTTER->SetTitle("Mass Shape; M (GeV);  pb/GeV");
    PLOTTER->DrawCopy();  
    h_Chi_Mass_data->DrawCopy("SAMEH");
    


    Bkg1->Draw("SAME");

    cout << "Bkg1 = " << Bkg1->Eval(1000.) << endl;

    gPad->SetLogy(1);
    gPad->Update();




    C1->cd(j+5);
    PLOTTER->SetMaximum(3);
    PLOTTER->SetMinimum(-3);
    TH1D* h_Chi_Mass_data_ratio = new TH1D();

    h_Chi_Mass_data->Copy(*h_Chi_Mass_data_ratio);
    h_Chi_Mass_data_ratio->Sumw2();

    for (int i = 1; i <= h_Chi_Mass_data->GetNbinsX()-10; i++ ){
      double signif = (h_Chi_Mass_data->GetBinContent(i)-Bkg1->Eval(h_Chi_Mass_data->GetBinCenter(i)))/(h_Chi_Mass_data->GetBinError(i)+1e-100);
      if (fabs(signif) > 100) signif = 0;
      cout << "signif = " << signif << endl;
      
      h_Chi_Mass_data_ratio->SetBinContent(i, signif);
      h_Chi_Mass_data_ratio->SetBinError(i, 1e-10); //h_Chi_Mass_data->GetBinError(i)/Bkg->Eval(h_Chi_Mass_data->GetBinCenter(i)));
    }
    PLOTTER->SetTitle("Pull; M (GeV); Pull");
    PLOTTER->DrawCopy();
    PLOTTER->SetStats(0);
    h_Chi_Mass_data_ratio->DrawCopy("SAMEH");

    gPad->Update();





  }


  C1->SaveAs("DEtator/Significance_34fb_fat.png");





}
