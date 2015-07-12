#include <utility>
#include "../includes/common.h"

void TemplateFit(TString fWDATA_str=0, TString fMCtemplates_str=0, TString fR_str=0, double xmin=0.75, double xmax=1.4 ){

  // int WMass::WMassCentral = 80385;
  // int WMass::WMassStep = 15;
  // static const int WMass::WMassNSteps = 60;
  // static const int WMass::etaMuonNSteps = 5;
  // double WMass::etaMaxMuons[etaMuonNSteps] = {0.6, 0.8, 1.2, 1.6, 2.1};
  
  static const int FitCombinations = 16;
  // double lowerFit[FitCombinations]={0.75,0.8, 0.9, 1.0, 0.75,0.8, 0.9, 1.0, 0.75,0.8, 0.9, 1.0, 0.75,0.8, 0.9, 1.0};
  // double upperFit[FitCombinations]={1.4 ,1.4 ,1.4 ,1.4 ,1.3 ,1.3 ,1.3 ,1.3, 1.2 ,1.2 ,1.2 ,1.2 ,1.1 ,1.1 ,1.1 ,1.1 };
  double lowerFit[FitCombinations]={0.75,0.75,0.75,0.75,0.8 ,0.8 ,0.8 ,0.8 ,0.9 ,0.9 ,0.9 ,0.9 ,1.0 ,1.0 ,1.0 ,1.0 };
  double upperFit[FitCombinations]={1.1 ,1.2 ,1.3 ,1.4 ,1.1 ,1.2 ,1.3 ,1.4 ,1.1 ,1.2 ,1.3 ,1.4 ,1.1 ,1.2 ,1.3 ,1.4 };

  TFile*fWDATA=new TFile(Form("%s",fWDATA_str.Data()));
  TFile*fMCtemplates=new TFile(Form("%s",fMCtemplates_str.Data()));
  TFile*fR=new TFile(Form("%s",fR_str.Data()));

  TFile *fout=new TFile("FitResults.root","RECREATE");
  
  TH1D *data;                              // data histogram
  TH1D *mc0;                               // MC histogram
  TH1D *hR;                               // MC histogram
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

    for(int i=0; i<WMass::etaMuonNSteps; i++){

      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      TGraph*chi2res=new TGraph();
      chi2res->SetName(Form("chi2res_%s_eta%s",fit_str.Data(),eta_str.Data()));
      chi2res->SetTitle(Form("chi2res_%s_eta%s",fit_str.Data(),eta_str.Data()));
      double chi2min=1e10, chi2max=0;
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      // for(int j=0; j<1; j++){
      
        int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;

        data = (TH1D*)fWDATA->Get(Form("hWPos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass));
        mc0 = (TH1D*)fMCtemplates->Get(Form("hWlikePos_PtScaled_RWeighted_Templates_eta%s_%d",eta_str.Data(),jWmass));
        hR = (TH1D*)fR->Get(Form("hR_WdivZ_WlikePos_8_JetCut_eta%s_%d",eta_str.Data(),jWmass));

        data->GetXaxis()->SetRangeUser(lowerFit[fitlimits],upperFit[fitlimits]);
        mc0->GetXaxis()->SetRangeUser(lowerFit[fitlimits],upperFit[fitlimits]);
        
        chi2_fitres=data->Chi2Test(mc0,"UU NORM CHI2/NDF");
        // chi2_fitres=CustomChi2Test(data,mc0,hR,xmin,xmax);
        cout << "chi2= " << chi2_fitres << endl;
        
        chi2res->SetPoint(j,jWmass,chi2_fitres);
        if(chi2_fitres<chi2min) chi2min=chi2_fitres;
        if(chi2_fitres>chi2max) chi2max=chi2_fitres;
      
      }
      // return;
    
      TCanvas*c_chi2=new TCanvas(Form("c_chi2_%s_eta%s",fit_str.Data(),eta_str.Data()),Form("c_chi2_%s_eta%s",fit_str.Data(),eta_str.Data()));
      TF1*ffit=new TF1(Form("ffit_%s_eta%s",fit_str.Data(),eta_str.Data()),Form("[0]+TMath::Power((x-[1])/[2],2)"),70,100);
      ffit->SetParameter(0,chi2res->GetMinimum());
      ffit->SetParameter(1,80150);
      ffit->SetParameter(2,500); // IF FIT DOES NOT CONVERGE, CHANGE THIS PARAMETER BY LOOKING AT THE CHI2 VS MASS DISTRIBUTION (~value for which Delta_chi2 = 1)
      // ffit->SetParameter(2,1e4); // IF FIT DOES NOT CONVERGE, CHANGE THIS PARAMETER BY LOOKING AT THE CHI2 VS MASS DISTRIBUTION (~value for which Delta_chi2 = 1)
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
      cout << "Measured mass points chi2 min = " << chi2min << " max = " << chi2max << endl;
      
      WMassVsEta->SetPoint(i,WMass::etaMaxMuons[i],ffit->GetParameter(1));
      WMassVsEta->SetPointError(i,0,ffit->GetParameter(2));
      WMassErrorVsEta->SetPoint(i,WMass::etaMaxMuons[i],ffit->GetParameter(2));
      chi2resVsEta->SetPoint(i,WMass::etaMaxMuons[i],ffit->GetParameter(0));
      
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




double CustomChi2Test(TH1D *Wdata, TH1D *mcZtemplates, TH1D *hR, double xmin, double xmax){

  Wdata->Print();
  mcZtemplates->Print();
  int xmin_bin=mcZtemplates->GetXaxis()->FindBin(xmin);
  int xmax_bin=mcZtemplates->GetXaxis()->FindBin(xmax);
  double ZdivW_norm = mcZtemplates->Integral(xmin_bin,xmax_bin)/Wdata->Integral(xmin_bin,xmax_bin);
  cout << xmin_bin << " " << xmax_bin << " " << ZdivW_norm << endl;
  
  double chi2=0;
  for(int i=xmin_bin; i<xmax_bin; i++){
    chi2+= TMath::Power( ( Wdata->GetBinContent(i)*ZdivW_norm - mcZtemplates->GetBinContent(i) ), 2 ) / ( mcZtemplates->GetBinContent(i)*hR->GetBinContent(i) + Wdata->GetBinContent(i)*Wdata->GetBinContent(i));
    // cout << Wdata->GetBinContent(i)*ZdivW_norm << " " << mcZtemplates->GetBinContent(i) << " " << hR->GetBinContent(i) << " " << chi2 << endl;
  }
  
  return chi2;

}


