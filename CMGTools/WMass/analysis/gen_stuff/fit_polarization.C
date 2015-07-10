{
  gROOT->SetBatch(1);
  
  gStyle->SetOptFit(1);
  gStyle->SetOptStat(0);
  
  TFile *fout=new TFile("polarization_data_powheg.root","RECREATE");
  TFile *fin=new TFile("fout_mass_allGenEvts.root");
  const int fitcoeff = 6;
  
  const int nrapbins = 2;
  const int nptbins = 8;
  double rapbins[nrapbins+1] = {0,1,2.1};
  TH1D *hrapbins = new TH1D("hrapbins","hrapbins",nrapbins,rapbins);
  double ptbins[nptbins+1] = {0.1,10,20,35,55,80,120,200,1000};
  TH1D *hptbins = new TH1D("hptbins","hptbins",nptbins,ptbins);

  TH1D *haibins_rap[nrapbins][fitcoeff],*haibins_data_rap[nrapbins][fitcoeff];
  for(int i=0; i<fitcoeff; i++){
    for(int j=0; j<nrapbins; j++){
      haibins_rap[j][i]= new TH1D(Form("ha%dbins_rap%d",i,j),Form("ha%dbins_rap%d",i,j),nptbins,ptbins);
      haibins_rap[j][i]->SetMarkerStyle(20);
      haibins_rap[j][i]->SetMarkerColor(4);
      haibins_rap[j][i]->SetLineColor(4);
      haibins_data_rap[j][i]= new TH1D(Form("ha%dbins_data_rap%d",i,j),Form("ha%dbins_data_rap%d",i,j),nptbins,ptbins);
      haibins_data_rap[j][i]->SetMarkerStyle(20);
      haibins_data_rap[j][i]->SetMarkerColor(2);
      haibins_data_rap[j][i]->SetLineColor(2);
    }
  }
  
  double a0bins_data_rap[nrapbins][nptbins]    ={
                                                { 0.018,  0.068,  0.179,  0.357,  0.563,  0.716,   0.834,  0.928},
                                                {0.032,  0.077,  0.179,  0.385,  0.554,  0.737,  0.860,  0.876}
                                                };
  double a1bins_data_rap[nrapbins][nptbins]    ={
                                                { -0.008, -0.006, 0.014, 0.033, 0.031, 0.029, 0.002, -0.02},
                                                {0.002, 0.018, 0.038, 0.063, 0.066, 0.059, 0.064, 0.04}
                                                };
  double a2bins_data_rap[nrapbins][nptbins]    ={
                                                {0.007, 0.037, 0.136, 0.278, 0.447, 0.583, 0.741, 0.689},
                                                {0.019, 0.038, 0.129, 0.26, 0.448, 0.587, 0.758, 0.864}
                                                };
  double a3bins_data_rap[nrapbins][nptbins]    ={
                                                {0.007, 0.003, 0.006, 0.005, 0.009, 0.033, 0.008, 0.101},
                                                {0.009, 0.003, 0.012, 0.012, 0.036, 0.074, 0.121, 0.181}
                                                };
  double a4bins_data_rap[nrapbins][nptbins]    ={
                                                {0.02, 0.013, 0.015, 0.021, 0.002, 0.019, 0.01, 0.029},
                                                {0.076, 0.072, 0.044, 0.052, 0.052, 0.074, 0.056, 0.005}
                                                };
  double a5bins_data_rap[nrapbins][nptbins]    ={
                                                {0., 0., 0., 0., 0., 0., 0., 0.},
                                                {0., 0., 0., 0., 0., 0., 0., 0.}
                                                };
  
  double a0bins_err_data_rap[nrapbins][nptbins]={
                                                { 0.009, 0.011, 0.014, 0.014, 0.012, 0.013, 0.021, 0.038},
                                                {0.011, 0.011, 0.015, 0.020, 0.020, 0.021, 0.023, 0.049}
                                                };
  double a1bins_err_data_rap[nrapbins][nptbins]={
                                                {0.005, 0.006, 0.009, 0.015, 0.018, 0.020, 0.020, 0.034},
                                                {0.008, 0.007, 0.009, 0.013, 0.019, 0.024, 0.028, 0.048}
                                                };
  double a2bins_err_data_rap[nrapbins][nptbins]={
                                                {0.005, 0.006, 0.015, 0.023, 0.025, 0.041, 0.052, 0.076},
                                                {0.008, 0.009, 0.017, 0.026, 0.025, 0.037, 0.049, 0.096}
                                                };
  double a3bins_err_data_rap[nrapbins][nptbins]={
                                                {0.004, 0.004, 0.004, 0.006, 0.008, 0.013, 0.017, 0.035},
                                                {0.005, 0.004, 0.007, 0.009, 0.019, 0.030, 0.034, 0.049}
                                                };
  double a4bins_err_data_rap[nrapbins][nptbins]={
                                                {0.003, 0.004, 0.004, 0.006, 0.007, 0.009, 0.014, 0.028},
                                                {0.005, 0.006, 0.009, 0.011, 0.012, 0.018, 0.023, 0.038}
                                                };
  double a5bins_err_data_rap[nrapbins][nptbins]={
                                                {0., 0., 0., 0., 0., 0., 0., 0.},
                                                {0., 0., 0., 0., 0., 0., 0., 0.}
                                                };
  
  // TF2 *fpol = new TF2("fpol","[5]*0.5*(1-3*x*x)+[0]*sin(2*acos(x))*cos(y)+[1]*0.5*sin(acos(x))*sin(acos(x))*cos(2*y)+[2]*sin(acos(x))*cos(y)+[3]*x + [4]*(1+x*x)",-1,1,0,TMath::Pi());
  TF2 *fpol = new TF2("fpol","[6]*([0]*0.5*(1-3*x*x)+[1]*sin(2*acos(x))*cos(y)+[2]*0.5*sin(acos(x))*sin(acos(x))*cos(2*y)+[3]*sin(acos(x))*cos(y)+[4]*x + 1*(1+x*x))",-1,1,0,TMath::Pi());
  fpol->SetParameter(6,100);
  
  for(int irap=0; irap<nrapbins; irap++){
    for(int ipt=1; ipt<nptbins+1; ipt++){
      TF2*fpol2 = (TF2*) fpol->Clone(Form("fpol_pt%d_rap%d",ipt,irap));
      fpol2->SetName(Form("fpol_pt%d_rap%d",ipt,irap));
      fpol2->SetTitle(Form("fpol_pt%d_rap%d",ipt,irap));
      TH2D*h=(TH2D*)fin->Get(Form("phi_vs_costh_CS_pietro_ptbin%d_rapbin%d",ipt,irap+1));
      TCanvas *c1 = new TCanvas(Form("ptbin_%d_rap%d",ipt,irap),Form("ptbin_%d_rap%d",ipt,irap));
      h->Fit(Form("fpol_pt%d_rap%d",ipt,irap));
      h->Draw("colz");
      fout->cd();
      h->Write();
      c1->Write();
      c1->SaveAs(Form("ptbin_%d_rap%d.pdf",ipt,irap));
      
      TF2*fpol_data = (TF2*) fpol2->Clone(Form("fpol_data_pt%d_rap%d",ipt,irap));
      fpol_data->SetName(Form("fpol_data_pt%d_rap%d",ipt,irap));
      fpol_data->SetTitle(Form("fpol_data_pt%d_rap%d",ipt,irap));
      fpol_data->SetParameter(0,a0bins_data_rap[irap][ipt]);
      fpol_data->SetParameter(1,a1bins_data_rap[irap][ipt]);
      fpol_data->SetParameter(2,a2bins_data_rap[irap][ipt]);
      fpol_data->SetParameter(3,a3bins_data_rap[irap][ipt]);
      fpol_data->SetParameter(4,a4bins_data_rap[irap][ipt]);
      TH2D*h_data=(TH2D*)h->Clone(Form("phi_vs_costh_CS_pietro_ptbin%d_rapbin%d_data",ipt,irap+1));
      h_data->SetName(Form("phi_vs_costh_CS_pietro_ptbin%d_rapbin%d_data",ipt,irap+1));
      h_data->SetTitle(Form("phi_vs_costh_CS_pietro_ptbin%d_rapbin%d_data",ipt,irap+1));
      h_data->Reset();
      for(int icosth=1;icosth<h_data->GetXaxis()->GetNbins()+1;icosth++){
        for(int iphi=1;iphi<h_data->GetYaxis()->GetNbins()+1;iphi++){
          cout << icosth << " " << iphi << " " 
          << " " << fpol_data->GetXaxis()->GetBinCenter(icosth) << " " << fpol_data->GetYaxis()->GetBinCenter(iphi)
          << " " << fpol_data->Eval(fpol_data->GetXaxis()->GetBinCenter(icosth),fpol_data->GetYaxis()->GetBinCenter(iphi)) 
          << endl;
          h_data->SetBinContent(icosth,iphi,fpol_data->Eval(fpol_data->GetXaxis()->GetBinCenter(icosth),fpol_data->GetYaxis()->GetBinCenter(iphi)));
        }
      }
      h_data->Write();
      
      for(int i=0; i<fitcoeff; i++){
        haibins_rap[irap][i]->SetBinContent(ipt,fpol2->GetParameter(i));
        haibins_rap[irap][i]->SetBinError(ipt,fpol2->GetParError(i));
        double abin=0, abin_err=0;
        if(i==0){
          abin=a0bins_data_rap[irap][ipt];
          abin_err=a0bins_err_data_rap[irap][ipt];
        } else if(i==1){
          abin=a1bins_data_rap[irap][ipt];
          abin_err=a1bins_err_data_rap[irap][ipt];
        } else if(i==2){
          abin=a2bins_data_rap[irap][ipt];
          abin_err=a2bins_err_data_rap[irap][ipt];
        } else if(i==3){
          abin=a3bins_data_rap[irap][ipt];
          abin_err=a3bins_err_data_rap[irap][ipt];
        } else if(i==4){
          abin=a4bins_data_rap[irap][ipt];
          abin_err=a4bins_err_data_rap[irap][ipt];
        } else if(i==5){
          abin=a5bins_data_rap[irap][ipt];
          abin_err=a5bins_err_data_rap[irap][ipt];
        }
        haibins_data_rap[irap][i]->SetBinContent(ipt,abin);
        haibins_data_rap[irap][i]->SetBinError(ipt,abin_err);
      }
    }
  
    TCanvas *cai[nrapbins][fitcoeff];
    TH1D hratio[nrapbins][fitcoeff];
    for(int i=0; i<fitcoeff; i++){
      cai[irap][i] = new TCanvas(Form("ca%d_rap%d",i,irap),Form("ca%d_rap%d",i,irap));
      cai[irap][i]->SetLogx();
      haibins_rap[irap][i]->GetXaxis()->SetRangeUser(0.1,haibins_rap[irap][i]->GetXaxis()->GetXmax());
      haibins_rap[irap][i]->GetYaxis()->SetRangeUser(
                                                TMath::Min(haibins_rap[irap][i]->GetMinimum(),haibins_data_rap[irap][i]->GetMinimum())*1.2,
                                                TMath::Max(haibins_rap[irap][i]->GetMaximum(),haibins_data_rap[irap][i]->GetMaximum())*1.2
                                                );
      haibins_rap[irap][i]->Draw();
      haibins_data_rap[irap][i]->Draw("same");
      fout->cd();
      cai[irap][i]->Write();
      cai[irap][i]->SaveAs(Form("ca%d_rap%d.pdf",i,irap));
      hratio[irap][i] = (*haibins_data_rap[irap][i])/(*haibins_rap[irap][i]);
      hratio[irap][i]->SetName(Form("hratio_a%d_rap%d",i,irap));
      hratio[irap][i]->SetTitle(Form("hratio_a%d_rap%d",i,irap));
      fout->cd();
      hratio[irap][i]->Write();
      
    }
  }
  
  fout->Write();
  fout->Close();
  
}

// NUMBERS FROM HEINER

  // double a0bins_data_rap[nrapbins][nptbins]    ={
                                                // { 285.202071,  606.7457184,  1017.791781,  1056.878023,  782.3261628,  510.6218603,  228.032288,  50.31849562},
                                                // {535.1159964,  705.1489889,  1026.432892,  1122.117515,  750.6617558,  490.1655831,  211.6938541,  35.20349993}
                                                // };
  // double a1bins_data_rap[nrapbins][nptbins]    ={
                                                // { -128.1900354,  -52.72970708,  77.65385864,  97.79899245,  43.21879098,  20.47571257,  0.681356066,  -1.073095305},
                                                // {33.15732116,  164.4133432,  216.2403414,  184.6026156,  88.78599067,  39.30264518,  15.72229295,  1.617476623}
                                                // };
  // double a2bins_data_rap[nrapbins][nptbins]    ={
                                                // { 113.0218833,  326.6409422,  772.1828014,  823.6557657,  620.7403998,  415.6941492,  202.6764417,  37.3438942},
                                                // {317.0650874,  348.1992801,  737.0298167,  759.2817113,  606.9004159,  390.1691124,  186.5729526,  34.70567631}
                                                // };
  // double a3bins_data_rap[nrapbins][nptbins]    ={
                                                // { 109.1240772,  24.75831852,  34.69666569,  13.80249962,  12.01734187,  23.22302043,  2.281316856,  5.451310405},
                                                // {140.9300838,  23.88691996,  69.19757133,  33.6441154,  49.291562,  49.47321355,  29.72966158,  7.273695639}
                                                // };
  // double a4bins_data_rap[nrapbins][nptbins]    ={
                                                // { 311.9186761,  111.1125895,  84.21502812,  63.61456855,  3.179488012,  13.82662612,  2.78602491,  1.585823018},
                                                // {1252.442929,  655.0694276,  251.9987083,  153.0130141,  70.48794593,  49.43103088,  13.88528054,  0.182414114}
                                                // };
  // double a5bins_data_rap[nrapbins][nptbins]    ={
                                                // { 15703.52683,  8879.308884,  5670.593304,  2962.034996,  1390.182475,  713.2553324,  273.5611537,  54.23006615},
                                                // {16540.83663,  9121.612768,  5721.605888,  2916.134187,  1355.231794,  664.8343634,  246.0310182,  40.17182786}
                                                // };
  
  // double a0bins_err_data_rap[nrapbins][nptbins]={
                                                // { 45.50998192,  31.48609646,  24.59025135,  16.36893882,  10.28755358,  7.128077227,  4.403035055,  2.027744385},
                                                // {77.67119215,  56.4926129,  43.97703173,  29.35974496,  16.31287109,  9.073230928,  4.914988073,  1.912351453}
                                                // };
  // double a1bins_err_data_rap[nrapbins][nptbins]={
                                                // { 33.25762289,  22.46804867,  18.26509996,  13.71532187,  9.755356625,  6.911865295,  4.087908236,  1.720663355},
                                                // {51.69958236,  32.9905886,  26.68878701,  20.21591026,  14.60351359,  9.764995189,  5.215201764,  1.752159268}
                                                // };
  // double a2bins_err_data_rap[nrapbins][nptbins]={
                                                // { 58.6889881,  39.28458579,  31.67851985,  23.70310281,  16.93260107,  12.55907781,  8.116045661,  3.767258527},
                                                // {75.08255874,  46.57669956,  37.20236512,  27.81391588,  19.83780837,  14.39019217,  9.010108199,  3.639165065}
                                                // };
  // double a3bins_err_data_rap[nrapbins][nptbins]={
                                                // { 25.92165363,  17.64525719,  14.49993954,  10.98578963,  7.927099412,  5.874143023,  3.745562619,  1.682407761},
                                                // {33.93431423,  21.57746859,  17.55536672,  13.29527979,  9.610048553,  6.93815821,  4.261938454,  1.636458894}
                                                // };
  // double a4bins_err_data_rap[nrapbins][nptbins]={
                                                // { 33.37354736,  23.70914589,  18.63493712,  12.81232945,  8.182287102,  5.530706643,  3.254063377,  1.407384152},
                                                // {52.47665811,  38.07744865,  30.0274519,  20.78761833,  12.8693914,  7.540026696,  3.855704701,  1.37919107}
                                                // };
  // double a5bins_err_data_rap[nrapbins][nptbins]={
                                                // { 13.36350353,  9.328569138,  7.300111127,  5.044406292,  3.346621624,  2.363270641,  1.45439739,  0.652809884},
                                                // {25.39119821,  18.295581,  14.06592987,  9.311734964,  5.377997516,  3.124709998,  1.700279087,  0.635688178}
                                                // };
