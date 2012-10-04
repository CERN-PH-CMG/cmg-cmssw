void SimpleDiJetFitV1()
{

  gROOT->Reset();

  gStyle->SetOptFit(1111); 

  //### Modify this part
  // Run2012B - only dijet, razoe, alfaT
  char input_root_file[500] = "data/DataScouting_V00-01-03_Run2012B_runrange_193752-197044_dijet_alfaT_razor.root";
  // Run2012B - all analyses (and 1-2% more events)
  //char input_root_file[500] = "data/DataScouting_V00-01-05_Run2012B_runrange_193752-197044_dijet_alfaT_razor_dijetpairs_trijetpairs.root";
  // Run2012C - all analyses
  //char input_root_file[500] = "data/DataScouting_V00-01-05_Run2012C_runrange_197885-203755_dijet_alfaT_razor_dijetpairs_trijetpairs.root";
  // Run2012B+Run2012C - all analyses
  //char input_root_file[500] = "data/DataScouting_V00-01-05_Run2012B_Run2012C_runrange_193752-203755_dijet_alfaT_razor_dijetpairs_trijetpairs.root";

  char input_directory[500] = "DQMData_Merged Runs_DataScouting_Run summary_DiJet;1";
  char input_2Dhistogram[500] = "h2_DetajjVsMjjWide;1";
  double minY_deta = 0.;
  double maxY_deta = 1.3;
  //   double minX_mass = 890.;
  //   double maxX_mass = 4337.;
  double minX_mass = 270.;
  double maxX_mass = 4337.;
  //   double minX_mass = 270.;
  //   double maxX_mass = 1530.;
  const int nPar=6; //number of fit parameters -  4 = default  , 5 = intermediate , 6 = fullCTEQ inspired

  int number_of_variableWidth_bins = 88 - 1;
  Double_t massBins[88] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649,  693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509, 4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7000, 7250,7500,7750,8000}; 

  char output_root_file[500] = "fit_results.root";
  

  //================================================================================================================

  //### input file and 2D histo
  TFile *file0=TFile::Open( input_root_file );
  TDirectoryFile* DQMData_Merged_Runs_DataScouting_Run_summary_DiJet = (TDirectoryFile*) file0->Get( input_directory );
  //  DQMData_Merged_Runs_DataScouting_Run_summary_DiJet->cd();
  TH2D* h_DEta_Mass = (TH2D*) DQMData_Merged_Runs_DataScouting_Run_summary_DiJet->Get( input_2Dhistogram );  
  //h_DEta_Mass->Draw("colz");

  //### 1D histo   
  TH1D* hist_mass = new TH1D();
  hist_mass->Sumw2();

  //### find bin width  
  float bin_width_X = float ( h_DEta_Mass->GetXaxis()->GetXmax() - h_DEta_Mass->GetXaxis()->GetXmin() ) / float ( h_DEta_Mass->GetNbinsX() ) ;
  float bin_width_Y = float ( h_DEta_Mass->GetYaxis()->GetXmax() - h_DEta_Mass->GetYaxis()->GetXmin() ) / float ( h_DEta_Mass->GetNbinsY() ) ;
  //   cout << "bin_width_X: " << bin_width_X << endl;
  //   cout << "bin_width_Y:" << bin_width_Y << endl;

  //### find bin range for projection
  float minY_deta_bin_tmp = (minY_deta / bin_width_Y) + 1;
  float maxY_deta_bin_tmp = maxY_deta / bin_width_Y    ;
  int minY_deta_bin = int(minY_deta_bin_tmp);
  int maxY_deta_bin = int(maxY_deta_bin_tmp);
  //   cout << "minY_deta_bin: " << minY_deta_bin << endl;
  //   cout << "minY_deta_bin:" << maxY_deta_bin << endl;

  //### project 2D histogram on X axix in the y range specified
  hist_mass = h_DEta_Mass->ProjectionX("hist_mass",minY_deta_bin,maxY_deta_bin,"e");
  hist_mass->GetXaxis()->SetTitle("M_{jj} WideJets [GeV]");
  //hist_mass->Draw();

  //### fit mass histogram with background function
  TF1 *M1Bkg;

  if(nPar==4)
    {
      M1Bkg = new TF1("M1Bkg","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]+[3]*log(x/8000)))",minX_mass,maxX_mass);
      M1Bkg->SetParameter(0,0.05);
      M1Bkg->SetParameter(1,7.1);
      M1Bkg->SetParameter(2,5.9);
      M1Bkg->SetParameter(3,0.2);
      //M1Bkg->SetParLimits(3,0,0.4);
    }

  if(nPar==5)
    {
      M1Bkg = new TF1("M1Bkg","( [0]*TMath::Power(1-x/8000,[1])*(1+[4]*x/8000) ) / ( TMath::Power(x/8000,[2]+[3]*log(x/8000)) )",minX_mass,maxX_mass);

      M1Bkg->SetParameter(0,0.005);
      //      M1Bkg->SetParLimits(0,-1,1);
      M1Bkg->SetParameter(1,9.3);
      //      M1Bkg->SetParLimits(1,-5,15);
      M1Bkg->SetParameter(2,7.2);
      //      M1Bkg->SetParLimits(2,-5,15);
      M1Bkg->SetParameter(3,0.4);
      //      M1Bkg->SetParLimits(3,-1,1);
      M1Bkg->SetParameter(4,3.1);
      //      M1Bkg->SetParLimits(4,-5,5);
    }

   if(nPar==6)
    {
      M1Bkg = new TF1("M1Bkg","( [0]*TMath::Power(1-x/8000,[1])*(1+[4]*x/8000+[5]*pow(x/8000,2)) ) / ( TMath::Power(x/8000,[2]+[3]*log(x/8000)) )",minX_mass,maxX_mass);

      M1Bkg->SetParameter(0,0.005);
      //      M1Bkg->SetParLimits(0,-1,1);
      M1Bkg->SetParameter(1,9.3);
      //      M1Bkg->SetParLimits(1,-5,15);
      M1Bkg->SetParameter(2,7.2);
      //      M1Bkg->SetParLimits(2,-5,15);
      M1Bkg->SetParameter(3,0.4);
      //      M1Bkg->SetParLimits(3,-1,1);
      M1Bkg->SetParameter(4,3.1);
      //      M1Bkg->SetParLimits(4,-5,5);
      M1Bkg->SetParameter(5,25.6);
      //      M1Bkg->SetParLimits(5,10,50);
    }

  TFitResultPtr r;
  int stopProgram=1;
  for( int loop=0; loop<10; loop++)
    {
      r = hist_mass->Fit("M1Bkg","LS","",minX_mass,maxX_mass);      
      Int_t fitStatus = r;
      if(fitStatus==0)
	{
	  stopProgram=0;
	  r->Print("V");  
	  break;
	}
    }

  if(stopProgram==1)
    {
      cout << "######################" << endl;
      cout << "######################" << endl;
      cout << "ERROR : Fit failed!!!!" << endl;
      cout << "######################" << endl;
      cout << "######################" << endl;
      break;
    }

  //### make histogram with variable binning
  hist_mass->Rebin( number_of_variableWidth_bins, "hist_mass_varbin", massBins);
  //hist_mass_varbin->Draw();
  for(int bin=1; bin<hist_mass_varbin->GetNbinsX()+1; bin++)
    {
      double current_bin_content = hist_mass_varbin->GetBinContent(bin);
      double current_bin_error = hist_mass_varbin->GetBinError(bin);
      double current_bin_width = hist_mass_varbin->GetBinWidth(bin);
      hist_mass_varbin->SetBinContent( bin, double (current_bin_content / current_bin_width) );      
      hist_mass_varbin->SetBinError( bin, double (current_bin_error / current_bin_width) );      
      //       cout << double (current_bin_content / current_bin_width) << endl;
      //       cout << double (current_bin_error / current_bin_width) << endl;
      //       cout << endl;
    }
  //hist_mass_varbin->Draw();


  // fit residuals
  TH1D* hist_fit_residual_vsMass = new TH1D("hist_fit_residual_vsMass","hist_fit_residual_vsMass",number_of_variableWidth_bins,massBins);
  TH1D* hist_fit_residual = new TH1D("hist_fit_residual","hist_fit_residual",10,-5,5);
 
  //cout << hist_mass_varbin->GetNbinsX() << endl;  
  for(int bin=1; bin<number_of_variableWidth_bins; bin++)
    {
      if( hist_mass_varbin->GetXaxis()->GetBinLowEdge(bin)>=minX_mass 
 	  && hist_mass_varbin->GetXaxis()->GetBinUpEdge(bin)<=maxX_mass )
 	{	  
 	  //cout << hist_mass_varbin->GetXaxis()->GetBinLowEdge(bin) << endl;
	  double data = hist_mass_varbin->GetBinContent(bin);
	  double err_data = hist_mass_varbin->GetBinError(bin);
	  //double fit = M1Bkg->Eval(hist_mass_varbin->GetBinCenter(bin));
 	  double fit = M1Bkg->Integral(hist_mass_varbin->GetXaxis()->GetBinLowEdge(bin) 
 	  			       , hist_mass_varbin->GetXaxis()->GetBinUpEdge(bin) ); 
	  fit = fit / ( hist_mass_varbin->GetBinWidth(bin) );

	  double err_tot = err_data;	  
	  double fit_residual = (data - fit) / err_tot;
	  double err_fit_residual = 1;
	  if (data == 0)
	    {
	      fit_residual = (data - fit) / 1.29;
	    }
	    	  
	  // 	  cout << "data, err_data, fit: " << data << ", " << err_data << ", " << fit << endl;
	  // 	  cout << "bin, fit residual : " << bin << ", " <<fit_residual << endl;	  
	  hist_fit_residual_vsMass->SetBinContent(bin,fit_residual);
	  hist_fit_residual_vsMass->SetBinError(bin,err_fit_residual);
	  hist_fit_residual->Fill(fit_residual);
 	}
    }

  
  //### Draw plots

  TCanvas *Canvas0 = new TCanvas("Canvas0","Canvas0",11,51,700,500);
  Canvas0->cd();
  Canvas0->SetLogy();
  hist_mass->GetYaxis()->SetTitle("Events");
  hist_mass->Draw();  
  M1Bkg->SetLineColor(1);
  M1Bkg->Draw("same");     

  TCanvas *Canvas1 = new TCanvas("Canvas1","Canvas1",11,51,700,500);
  Canvas1->cd();
  Canvas1->SetLogy();
  hist_mass_varbin->GetYaxis()->SetTitle("Events / bin width");
  hist_mass_varbin->Draw();  
  M1Bkg->SetLineColor(1);
  M1Bkg->Draw("same");     
 
  TCanvas *Canvas2 = new TCanvas("Canvas2","Canvas2",11,51,700,500);
  Canvas2->cd();
  Canvas2->SetGridx();
  Canvas2->SetGridy();
  Canvas2->SetLogx();
  hist_fit_residual_vsMass->GetYaxis()->SetLimits(-5,5);
  hist_fit_residual_vsMass->GetYaxis()->SetRangeUser(-5,5);
  hist_fit_residual_vsMass->GetYaxis()->SetTitle("(data - fit) / #sqrt{data}");
  hist_fit_residual_vsMass->GetXaxis()->SetRangeUser(minX_mass,maxX_mass);
  hist_fit_residual_vsMass->GetXaxis()->SetTitle("M_{jj} WideJets [GeV]");
  hist_fit_residual_vsMass->Draw();

  TCanvas *Canvas3 = new TCanvas("Canvas3","Canvas3",11,51,700,500);
  Canvas3->cd();
  hist_fit_residual->GetXaxis()->SetTitle("(data - fit) / #sqrt{data}");
  hist_fit_residual->GetYaxis()->SetTitle("Number of bins");
  hist_fit_residual->GetYaxis()->SetRangeUser(0,number_of_variableWidth_bins/3);
  hist_fit_residual->Draw();
  hist_fit_residual->Fit("gaus","L","",-3,3);

  //### Output files
  TFile f_output(output_root_file,"RECREATE");
  f_output.cd();
  Canvas0->Write();
  Canvas1->Write();
  Canvas2->Write();
  Canvas3->Write();
  f_output.Close();

  //### Save figures from canvas
  char c0_fileName[200];
  sprintf(c0_fileName,"dijetmass_nParFit%d.png",nPar);
  char c1_fileName[200];
  sprintf(c1_fileName,"dijetmass_varbin_nParFit%d.png",nPar);
  char c2_fileName[200];
  sprintf(c2_fileName,"fitresiduals_vs_mass_nParFit%d.png",nPar);
  char c3_fileName[200];
  sprintf(c3_fileName,"fitresiduals_nParFit%d.png",nPar);

  Canvas0->SaveAs(c0_fileName);
  Canvas1->SaveAs(c1_fileName);
  Canvas2->SaveAs(c2_fileName);
  Canvas3->SaveAs(c3_fileName);
}
