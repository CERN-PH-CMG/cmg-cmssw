#include <iostream>
#include <vector>

#include "TStyle.h"
#include "TFile.h"
#include "TH1.h"
#include "THStack.h"
#include "TMath.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TSystem.h"

void plotAndSaveHisto1D_bands(TString LegendEvTypeTeX, TFile*fMCsig, TFile*fMCEWKTT, TFile*fDATA, TString HistoName_st, int logx, int logy, int logz, int scaleMCtoDATA, TString title, double xmin, double xmax, int rebinfactor, std::vector<TFile*> systfiles_MCsum, std::vector<TString> systnames){

  std::cout << "retrieving hMCsig  = " << HistoName_st.Data() << std::endl;
  TH1D*hMCsig=(TH1D*)fMCsig->Get(HistoName_st.Data());
  hMCsig->Rebin(rebinfactor);
  hMCsig->SetLineColor(kOrange-3);
  TH1D*hMCdummy=new TH1D("hMCdummy","hMCdummy",hMCsig->GetNbinsX(),hMCsig->GetXaxis()->GetXmin(),hMCsig->GetXaxis()->GetXmax());
  
  std::cout << "retrieving hMCsig2 = " << HistoName_st.Data() << std::endl;
  TH1D*hMCsig2=(TH1D*)hMCsig->Clone("hMCsig2");
  hMCsig2->SetFillColor(kOrange-2);
  hMCsig2->SetLineStyle(2);
  hMCsig2->SetLineWidth(2);
  
  std::cout << "retrieving hMCEWKTT  = " << HistoName_st.Data() << std::endl;
  TH1D*hMCEWKTT=(TH1D*)fMCEWKTT->Get(HistoName_st.Data());
  hMCEWKTT->SetFillColor(kOrange+7);
  hMCEWKTT->SetLineColor(kOrange+10);
  hMCEWKTT->Rebin(rebinfactor);
  
  std::cout << "retrieving hDATA   = " << HistoName_st.Data() << std::endl;
  TH1D*hDATA=(TH1D*)fDATA->Get(HistoName_st.Data());
  hDATA->SetMarkerColor(1);
  hDATA->SetLineColor(1);
  hDATA->SetMarkerStyle(20);
  hDATA->SetMarkerSize(0.9);
  hDATA->Rebin(rebinfactor);

  TH1D MCsum=(*hMCsig)+(*hMCEWKTT);
  
  std::cout << "Total DATA/MC scaling factor = " << hDATA->Integral()/MCsum.Integral() << std::endl;
  if(scaleMCtoDATA){
    hMCsig2->Scale(hDATA->Integral()/MCsum.Integral());
    hMCsig->Scale(hDATA->Integral()/MCsum.Integral());
    hMCEWKTT->Scale(hDATA->Integral()/MCsum.Integral());
    MCsum.Scale(hDATA->Integral()/MCsum.Integral());
  }
  
  // compute % fractions
  double fsig = hMCsig->Integral()/MCsum.Integral()*100;
  double fewktt = hMCEWKTT->Integral()/MCsum.Integral()*100;

  THStack *hs= new THStack("hs","test stacked histograms");
  hs->Add(hMCEWKTT);
  hs->Add(hMCsig);

  std::cout << "Integrals: hDATA= " << hDATA->Integral()
            << " hMCsig= " << hMCsig->Integral()
            << " hMCEWKTT= " << hMCEWKTT->Integral()
            << " MCsum= "  << MCsum.Integral() << std::endl;
  // std::cout << "Entries hDATA= " << hDATA->GetEntries() << " hMCsig= " << hMCsig->GetEntries() << " hMCEWKTT= " << hMCEWKTT->GetEntries() << " MCsum= " << MCsum.GetEntries() << std::endl;
  
  // Double_t TH1::Chi2TestX(const TH1* h2,  Double_t &chi2, Int_t &ndf, Int_t &igood, Option_t *option,  Double_t *res) const
  
  TLatex *t = new TLatex();
  t->SetNDC();
  // t->SetTextAlign(22);
  t->SetTextSize(0.05);
  //  t->SetTextFont(63);
  t->SetTextSizePixels(17);

  // h1->SetLineColor(2);
  
  const double ydiv = 0.25;

  TCanvas *c1 = new TCanvas("c"+HistoName_st,"c"+HistoName_st,800,800);
  //  c1->SetLeftMargin();
  //  c1->SetRightMargin(0.2);
  TPad *pad1 = new TPad("pad1", "",0.04,ydiv,1,1);
  pad1->SetFillStyle(4000); //will be transparent
  pad1->SetLogx(logx);
  pad1->SetLogy(logy);
  pad1->SetLogz(logz);
  pad1->SetTickx(1);
  pad1->SetTicky(1);
  pad1->SetBottomMargin(0);
  //  pad1->SetLeftMargin(0.1);
  //  pad1->SetRightMargin(0.2);
  pad1->SetTopMargin(0.075);
  pad1->Draw();
  

  // 0.03
  TPad *pad2 = new TPad("pad2", "",0.04,0,1,ydiv);
  pad2->SetFillStyle(4000); //will be transparent
  pad2->SetLogx(logx);
  pad2->SetTickx(1);
  pad2->SetTicky(1);
  pad2->SetTopMargin(0);
  //  pad2->SetLeftMargin(-1);
  // pad2->SetRightMargin(0.2);
  pad2->SetBottomMargin(0.35);
  pad2->Draw();

  // Draw pad1
  pad1->cd();  
  if(logy!=1)
    hDATA->GetYaxis()->SetRangeUser(0,hDATA->GetMaximum()*1.2);
  else
    hDATA->GetYaxis()->SetRangeUser(1,hDATA->GetMaximum()*10);
  
  hDATA->GetXaxis()->SetLabelSize(0.05);
  hDATA->GetXaxis()->SetTitleOffset(1);
  hDATA->GetXaxis()->SetTitleSize(0.05);
  hDATA->GetYaxis()->SetLabelSize(0.05);
  hDATA->GetYaxis()->SetTitleOffset(1.5);
  hDATA->GetYaxis()->SetTitleSize(0.05);
  hDATA->GetYaxis()->SetNoExponent(kFALSE);
  hDATA->GetYaxis()->SetNdivisions(410);

  hDATA->SetTitle(title.Data());
  hDATA->GetXaxis()->SetRangeUser(xmin==-1?hDATA->GetXaxis()->GetBinLowEdge(1):xmin,
				  xmax==-1?hDATA->GetXaxis()->GetBinCenter(hDATA->GetNbinsX()):xmax);
  
  hDATA->GetYaxis()->SetTitle(Form("Counts / %.2f",hDATA->GetBinWidth(1)));

  //  hDATA->SetMaximum(1.12*hDATA->GetMaximum());
  //  hDATA->GetYaxis()->SetRangeUser(-300,1.12*hDATA->GetMaximum());
  hDATA->GetYaxis()->SetRangeUser(-hDATA->GetMaximum()*0.06,1.12*hDATA->GetMaximum());

  hDATA->Draw("pe");
  hs->Draw("hist same");
  hMCsig2->Draw("hist same");
  hMCEWKTT->Draw("hist same");
  hDATA->Draw("same pe");
  pad1->RedrawAxis();
  
  //  TLegend *leg = new TLegend(0.63,0.6,0.87,0.9,"4.75 fb^{-1} at #sqrt{7} TeV","brNDC");
  TLegend *leg = new TLegend(0.55,0.7,0.84,0.92,"","brNDC");
  leg->SetBorderSize(0);
  leg->SetTextFont(62);
  leg->SetTextSize(0.038);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->AddEntry(hDATA,"DATA","ple");
  leg->AddEntry(hMCsig2,Form("%s (%.1f\%%)",LegendEvTypeTeX.Data(), fsig),"f");
  leg->AddEntry(hMCEWKTT,Form("Background (%.1f\%%)",fewktt),"f");
  leg->Draw();

  // t->DrawLatex(0.15,0.85,Form("stat chi2/ndf: %.2f, prob: %.3e",(chi2/ndf),TMath::Prob(chi2,ndf)));

  t->DrawLatex(0.22,0.94,"#bf{#it{Preliminary}}"); // need unbold
  t->DrawLatex(0.6,0.94,"#sqrt{s}=7 TeV (4.7 fb^{-1})");

  t->SetTextSize(0.07);
  t->DrawLatex(0.1,0.94,"CMS");

  
  // Draw pad2
  pad2->cd();
  TH1D* hDATApull = (TH1D*)hDATA->Clone("hDATApull");
  TH1D* hMCsum = &MCsum;
  TH1D* hERRsum = (TH1D*)hMCsum->Clone("hERRsum");
  for(int i=1;i<hERRsum->GetNbinsX()+1; i++){
    hERRsum->SetBinError(i, 0);
  }
  hDATApull->Add(hERRsum,-1);
  for(int i=1;i<hERRsum->GetNbinsX()+1; i++){
    hERRsum->SetBinContent(i, TMath::Hypot(hERRsum->GetBinError(i),hDATApull->GetBinError(i)));
  }

  int systtotnum = systfiles_MCsum.size();
  std::vector<TH1D*> herrors_stacked; // works only with single syst files !!!
  
  for (int systnum = 0; systnum < systtotnum; ++systnum) {
    herrors_stacked.push_back((TH1D*)systfiles_MCsum[systnum]->Get(HistoName_st.Data()));
    herrors_stacked[systnum]->Scale(hDATA->Integral()/herrors_stacked[systnum]->Integral());
    herrors_stacked[systnum]->Add(hMCsum,-1);
    for(int bin=1; bin<herrors_stacked[systnum]->GetNbinsX()+1; bin++){
      double error_curr = herrors_stacked[systnum]->GetBinContent(bin);
      // double error_pred;
      // if (systnum > 0) error_pred = herrors_stacked[systnum-1]->GetBinError(bin);
      // else error_pred = hERRsum->GetBinError(bin);
      // herrors_stacked[systnum]->SetBinError(bin, sqrt(error_pred*error_pred + error_curr*error_curr));
      hERRsum->SetBinContent(bin, TMath::Hypot(hERRsum->GetBinContent(bin),error_curr));
    }
  }

  hDATApull->GetXaxis()->SetLabelSize(0.05*0.75/0.25);
  hDATApull->GetXaxis()->SetTitleOffset(1);
  hDATApull->GetXaxis()->SetTitleSize(0.05*0.75/0.25);
  hDATApull->GetYaxis()->SetTitle("Pull");
  hDATApull->GetYaxis()->SetLabelSize(0.05*0.75/0.25);
  hDATApull->GetYaxis()->SetTitleOffset(1.);
  hDATApull->GetYaxis()->SetTitleSize(0.05*0.75/0.25);
  hDATApull->GetYaxis()->SetNdivisions(4);
  hDATApull->GetYaxis()->SetRangeUser(-5,5);
  
  hDATApull->Divide(hERRsum);
  
  double chi2_all = 0;
  for(int i=1;i<hDATApull->GetNbinsX()+1; i++){
    hDATApull->SetBinError(i,0);
    chi2_all+= hDATApull->GetBinContent(i)*hDATApull->GetBinContent(i);
  }
  
  Double_t chi2; Int_t ndf; Int_t igood;
  std::cout << "DATA-MC chi2: " << hDATA->Chi2TestX((TH1D*)&MCsum,chi2,ndf,igood,"") << std::endl;
  std::cout << "chi2= " << chi2 << " ndf= " << ndf << " norm chi2= " << (chi2/ndf) << " prob= " << TMath::Prob(chi2,ndf) << " igood= " << igood << std::endl;
  std::cout << "chi2 including systematics= " << (chi2_all/ndf) << std::endl << std::endl;
  
  hDATApull->Draw("P");

  TLine*l0=new TLine(xmin==-1?hDATA->GetXaxis()->GetBinLowEdge(1):xmin, 0, xmax==-1?hDATA->GetXaxis()->GetBinCenter(hDATA->GetNbinsX()):xmax, 0); l0->SetLineColor(kBlack);
  TLine*lp5=new TLine(xmin==-1?hDATA->GetXaxis()->GetBinLowEdge(1):xmin, 1, xmax==-1?hDATA->GetXaxis()->GetBinCenter(hDATA->GetNbinsX()):xmax,1); lp5->SetLineColor(kGray);
  TLine*lm5=new TLine(xmin==-1?hDATA->GetXaxis()->GetBinLowEdge(1):xmin, -1, xmax==-1?hDATA->GetXaxis()->GetBinCenter(hDATA->GetNbinsX()):xmax, -1); lm5->SetLineColor(kGray);
  l0->Draw("histo same");
  lp5->Draw("histo same");
  lm5->Draw("histo same");
  
  // Output files
  TString extra_ouput_str = "";
  if(logx)extra_ouput_str+="_logx";
  if(logy)extra_ouput_str+="_logy";
  if(logz)extra_ouput_str+="_logz";
  if(scaleMCtoDATA)extra_ouput_str+="_norm";
  if(xmin!=-1)extra_ouput_str+=Form("_xmin%.f",xmin);
  if(xmax!=-1)extra_ouput_str+=Form("_xmax%.f",xmax);
  pad1->Update();
  pad2->Update();
  c1->Update();
  c1->SaveAs(HistoName_st+extra_ouput_str+".pdf");
  c1->SaveAs(HistoName_st+extra_ouput_str+".png");
  c1->SaveAs(HistoName_st+extra_ouput_str+".root");
}

void PlotZpre_unblinding(){

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  
  TString folderMCsig = "output_DYJetsPow/";
  TString folderMCEWKTT = "output_EWKTT/";
  TString folderDATA  = "output_DATA/";
  TString folderMCsum = "output_MCDATALIKEPOW/";
  
  TString basefolder = "/afs/cern.ch/work/a/astacchi/CMSSW_5_3_22_patch1_Wmass/src/CMGTools/WMass/analysis/JobOutputs/"; // Supposed to be run from PlottingCode/
  // TString posNeg_str = "Neg";
   TString posNeg_str = "Pos";
  const int mass = 91188;

  TString prefix = "moriond_stable_mu" + posNeg_str;

  TString destinationfolder = "../JobOutputs/"+prefix+"_pre_unblinding_plots";
  
  TString templatefolder = basefolder+prefix+"_tkmet_ewk0_polariz1_KalmanCorrParam_RecoilCorr2_EffHeinerSFCorr_PtSFCorr0_PileupSFCorr/";
  
  TString systfolders[] = {
    basefolder+prefix+"_tkmet_ewk0_polariz1_KalmanCorrParam_globalScaleSigma1_RecoilCorr2_EffHeinerSFCorr_PtSFCorr0_PileupSFCorr/",
    basefolder+prefix+"_tkmet_ewk0_polariz1_KalmanCorrParam_globalScaleSigma-1_RecoilCorr2_EffHeinerSFCorr_PtSFCorr0_PileupSFCorr/",
    basefolder+prefix+"_tkmet_ewk0_polariz1_KalmanCorrParam_RecoilCorr3_EffHeinerSFCorr_PtSFCorr0_PileupSFCorr/",
  };
  TString systnames_IHATECXX98[] = {
    "Lepton scale +1",
    "Lepton scale -1",
    "Recoil alt model",
  };
  
  const int systtotnum = sizeof(systfolders)/sizeof(TString);
  std::vector<TString> systnames(systnames_IHATECXX98, systnames_IHATECXX98 + systtotnum);
  
  TFile *fMCsig= new TFile(Form("%s/%s/ZanalysisOnDATA.root", templatefolder.Data(), folderMCsig.Data()));
  TFile *fMCEWKTT= new TFile(Form("%s/%s/ZanalysisOnDATA.root", templatefolder.Data(), folderMCEWKTT.Data()));
  TFile *fDATA = new TFile(Form("%s/%s/ZanalysisOnDATA.root", templatefolder.Data(), folderDATA.Data() ));

  std::vector<TFile*> systfiles_MCsum;
  for (int systnum = 0; systnum < systtotnum; ++systnum) {
    TString filepath = Form("%s/%s/ZanalysisOnDATA.root", systfolders[systnum].Data(), folderMCsum.Data());
    systfiles_MCsum.push_back(new TFile(filepath));
  }

  // Go into destfolder
  gSystem->mkdir(destinationfolder);
  gSystem->cd(destinationfolder);
  
  TString LegendEvTypeTeX=Form("W_{like}%s#rightarrow#mu#nu",posNeg_str.Data());
  //  if(posNeg_str.Contains("Neg")) LegendEvTypeTeX=Form("W_{like}^{-}#rightarrow#mu^{-}#nu");
  //  if(posNeg_str.Contains("Pos")) LegendEvTypeTeX=Form("W_{like}^{+}#rightarrow#mu^{+}#nu");
  if(posNeg_str.Contains("Neg")) LegendEvTypeTeX=Form("Z(W_{like}^{-})#rightarrow#mu^{-}#nu");
  if(posNeg_str.Contains("Pos")) LegendEvTypeTeX=Form("Z(W_{like}^{+})#rightarrow#mu^{+}#nu");
  
  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_Recoil_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,0,0,1,Form(";Recoil [GeV];Counts"),-1,16.99,1,systfiles_MCsum,systnames);
  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_Zmass_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,0,0,1,Form(";Dimuon mass [GeV];Counts"),-1,-1,1,systfiles_MCsum,systnames);
  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_ZpT_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,0,0,1,Form(";Z pT [GeV];Counts"),-1,29.99,1,systfiles_MCsum,systnames);
  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_Zrap_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,0,0,1,Form(";Dimuon rapidity;Counts"),-1.6,1.6,1,systfiles_MCsum,systnames);
  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_costh_CS_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,0,0,1,Form(";cos #theta^{*}_{CS};Counts"),-1,-1,1,systfiles_MCsum,systnames);
  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_phi_CS_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,0,0,1,Form(";#phi^{*}_{CS} [rad];Counts"),-1,-1,1,systfiles_MCsum,systnames);

  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_METNonScaled_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,0,0,1,Form("; E_{T}^{miss} [GeV];Counts"),-1,-1,1,systfiles_MCsum,systnames);
  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_MtNonScaled_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,0,0,1,Form("; M_{T} [GeV];Counts"),-1,-1,1,systfiles_MCsum,systnames);
  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_PtNonScaled_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,0,0,1,Form("; P_{T}^{#mu} [GeV];Counts"),-1,-1,1,systfiles_MCsum,systnames);

  /*
  bool logy=1;
  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_U1Z_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,logy,0,1,Form(";U_{#parallel}^{Z} [GeV];Counts"),-1,-1,1,systfiles_MCsum,systnames);
  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_U2Z_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,logy,0,1,Form(";U_{#perp}^{Z} [GeV];Counts"),-1,-1,1,systfiles_MCsum,systnames);
  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_U1lep_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,logy,0,1,Form(";U_{#parallel}^{#mu} [GeV];Counts"),-1,-1,1,systfiles_MCsum,systnames);
  plotAndSaveHisto1D_bands(LegendEvTypeTeX,fMCsig,fMCEWKTT,fDATA,Form("hWlike%s_U2lep_8_JetCut_pdf229800-0_eta0p9_%d",posNeg_str.Data(),mass),0,logy,0,1,Form(";U_{#perp}^{#mu} [GeV];Counts"),-1,-1,1,systfiles_MCsum,systnames);
  */

}

int main()
{
  PlotZpre_unblinding();
  return 0;
}
