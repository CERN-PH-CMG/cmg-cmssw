#include <utility>

void TemplateFit(TString fDATA_str=0, TString fMC_str=0, double xmin=0.75, double xmax=1.4 ){

  int WMassCentral = 80385;
  int WMassStep = 15;
  static const int WMassNSteps = 60;
  static const int etaMuonNSteps = 5;
  double etaMaxMuons[etaMuonNSteps] = {0.6, 0.8, 1.2, 1.6, 2.1};
  
  static const int FitCombinations = 16;
  double lowerFit[FitCombinations]={0.75,0.85,0.95,1.05,0.75,0.85,0.95,1.05,0.75,0.85,0.95,1.05,0.75,0.85,0.95,1.05};
  double upperFit[FitCombinations]={1.4 ,1.4 ,1.4 ,1.4 ,1.3 ,1.3 ,1.3 ,1.3, 1.2 ,1.2 ,1.2 ,1.2 ,1.1 ,1.1 ,1.1 ,1.1 };

  TFile*fDATA=new TFile(Form("%s",fDATA_str.Data()));
  TFile*fMC=new TFile(Form("%s",fMC_str.Data()));

  TFile *fout=new TFile("FitResults.root","RECREATE");
  
  TH1D *data;                              // data histogram
  TH1D *mc0;                               // MC histogram
  double chi2_fitres=0;
  
  for(int fitlimits=0; fitlimits<FitCombinations; fitlimits++){
    TString fit_str = Form("fit_%.2f_%.2f",lowerFit[fitlimits],upperFit[fitlimits]); fit_str.ReplaceAll(".","p");

    TGraphErrors*chi2resVsEta=new TGraphErrors();
    chi2resVsEta->SetName(Form("chi2resVsEta_%s",fit_str.Data()));
    chi2resVsEta->SetTitle(Form("chi2resVsEta_%s",fit_str.Data()));
    chi2resVsEta->SetMarkerStyle(20);
    chi2resVsEta->SetMarkerSize(1);
    TGraphErrors*WMassVsEta=new TGraphErrors();
    WMassVsEta->SetName(Form("WMassVsEta_%s",fit_str.Data()));
    WMassVsEta->SetTitle(Form("WMassVsEta_%s",fit_str.Data()));
    WMassVsEta->SetMarkerStyle(20);
    WMassVsEta->SetMarkerSize(1);
    TGraphErrors*WMassErrorVsEta=new TGraphErrors();
    WMassErrorVsEta->SetName(Form("WMassErrorVsEta_%s",fit_str.Data()));
    WMassErrorVsEta->SetTitle(Form("WMassErrorVsEta_%s",fit_str.Data()));
    WMassErrorVsEta->SetMarkerStyle(20);
    WMassErrorVsEta->SetMarkerSize(1);

    for(int i=0; i<etaMuonNSteps; i++){

      TString eta_str = Form("%.1f",etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      TGraph*chi2res=new TGraph();
      chi2res->SetName(Form("chi2res_%s_eta%s",fit_str.Data(),eta_str.Data()));
      chi2res->SetTitle(Form("chi2res_%s_eta%s",fit_str.Data(),eta_str.Data()));
      for(int j=0; j<2*WMassNSteps+1; j++){
      // for(int j=0; j<1; j++){
      
        int jWmass = WMassCentral-(WMassNSteps-j)*WMassStep;

        data = (TH1D*)fDATA->Get(Form("Wmu_pt_eta%s_%d",eta_str.Data(),jWmass));
        mc0 = (TH1D*)fMC->Get(Form("hWtemplates_eta%s_%d",eta_str.Data(),jWmass));

        data->GetXaxis()->SetRangeUser(lowerFit[fitlimits],upperFit[fitlimits]);
        mc0->GetXaxis()->SetRangeUser(lowerFit[fitlimits],upperFit[fitlimits]);
        
        chi2_fitres=data->Chi2Test(mc0,"UU NORM CHI2/NDF");
        // cout << "chi2= " << chi2_fitres << endl;
        
        chi2res->SetPoint(j,jWmass,chi2_fitres);
      
      }
    
      TCanvas*c_chi2=new TCanvas(Form("c_chi2_%s_eta%s",fit_str.Data(),eta_str.Data()),Form("c_chi2_%s_eta%s",fit_str.Data(),eta_str.Data()));
      TF1*ffit=new TF1(Form("ffit_%s_eta%s",fit_str.Data(),eta_str.Data()),Form("[0]+TMath::Power((x-[1])/[2],2)"),70,100);
      ffit->SetParameter(0,chi2res->GetMinimum());
      ffit->SetParameter(1,80150);
      ffit->SetParameter(2,1e2);
      ffit->SetLineColor(2);
      ffit->SetLineWidth(1);
      chi2res->Fit(Form("ffit_%s_eta%s",fit_str.Data(),eta_str.Data()),"WEM");
      chi2res->SetMarkerStyle(20);
      chi2res->SetMarkerSize(1);
      chi2res->Draw("ap");
      TLatex *text,*text2;
      text = new TLatex(0.25,0.7,Form("Best M_{W} = %.0f +/- %.0f", ffit->GetParameter(1), ffit->GetParameter(2) ));
      text->SetNDC();
      text->Draw();
      text2 = new TLatex(0.25,0.6,Form("Best #chi^{2} = %.1f", ffit->GetParameter(0) ));
      text2->SetNDC();
      text2->Draw();
      cout << "Fit range = [" << lowerFit[fitlimits] <<" ; " << upperFit[fitlimits] <<" ]"<< endl;
      cout << "Best M_W value = " << ffit->GetParameter(1) << " +/- " << ffit->GetParameter(2) << endl;
      cout << "Best chi2 value = " << ffit->GetParameter(0) << endl;
      
      WMassVsEta->SetPoint(i,etaMaxMuons[i],ffit->GetParameter(1));
      WMassVsEta->SetPointError(i,0,ffit->GetParameter(2));
      WMassErrorVsEta->SetPoint(i,etaMaxMuons[i],ffit->GetParameter(2));
      chi2resVsEta->SetPoint(i,etaMaxMuons[i],ffit->GetParameter(0));
      
      fout->cd();
      c_chi2->Write();
      chi2res->Write();
    }
    
    fout->cd();
    WMassVsEta->Write();
    WMassErrorVsEta->Write();
    chi2resVsEta->Write();
  }
  fout->Close();
  
}