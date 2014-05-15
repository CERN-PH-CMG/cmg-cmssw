#include <vector>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TLine.h>
#include <TH1D.h>
#include <TKey.h>
#include <TROOT.h>
#include <iostream>

void test_plot_comparison(){

  gStyle->SetOptStat(0);
  TFile *file_temp;
  
  std::vector <TFile*> files;
  std::vector <TString> labels;
  std::vector <int> colors;

  // TString outputfolder="scaleW/";  
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_recoilOK_tkmet_RochCorr_RecoilCorr_EffHeinerSFCorr_PileupSFCorr/test_numbers_WJetsSig/WanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC corr"); colors.push_back(1);
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_recoilOK_tkmet_RochCorr_RecoilCorr_Scale1_EffHeinerSFCorr_PileupSFCorr/test_numbers_WJetsSig/WanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC corr scale+1"); colors.push_back(2);
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_recoilOK_tkmet_RochCorr_RecoilCorr_Scale-1_EffHeinerSFCorr_PileupSFCorr/test_numbers_WJetsSig/WanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC corr scale-1"); colors.push_back(4);
  
  // TString outputfolder="scaleWlike/";  
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_recoilOK_tkmet_RochCorr_RecoilCorr_EffHeinerSFCorr_PileupSFCorr/test_numbers_DYJetsSig/ZanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC corr"); colors.push_back(1);
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_recoilOK_tkmet_RochCorr_RecoilCorr_Scale1_EffHeinerSFCorr_PileupSFCorr/test_numbers_DYJetsSig/ZanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC corr scale+1"); colors.push_back(2);
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_recoilOK_tkmet_RochCorr_RecoilCorr_Scale-1_EffHeinerSFCorr_PileupSFCorr/test_numbers_DYJetsSig/ZanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC corr scale-1"); colors.push_back(4);
  
  // TString outputfolder="CorrNOcorr/";  
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_recoilOK_tkmet_RochCorr_RecoilCorr_EffHeinerSFCorr_PileupSFCorr/test_numbers_WJetsSig/WanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC corr"); colors.push_back(1);
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_recoilOK_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/test_numbers_WJetsSig/WanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC NO corr"); colors.push_back(4);
  
  // TString outputfolder="CDFrangeSelLarge/";  
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_CDFrangeLargeSel_tkmet_RochCorr_RecoilCorr_EffHeinerSFCorr_PileupSFCorr/test_numbers_WJetsSig/WanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC corr"); colors.push_back(1);
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_CDFrangeLargeSel_tkmet_RochCorr_RecoilCorr_Scale1_EffHeinerSFCorr_PileupSFCorr/test_numbers_WJetsSig/WanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC corr scale+1"); colors.push_back(2);
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_CDFrangeLargeSel_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/test_numbers_WJetsSig/WanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC NO corr "); colors.push_back(4);
  
  TString outputfolder="CDFrangeSelLargeWlike/";  
  /////
  file_temp = new TFile("../JobOutputs/test_MR1p1_CDFrangeLargeSel_tkmet_RochCorr_RecoilCorr_EffHeinerSFCorr_PileupSFCorr/test_numbers_DYJetsSig/ZanalysisOnDATA.root");
  files.push_back(file_temp);labels.push_back("MC corr"); colors.push_back(1);
  /////
  file_temp = new TFile("../JobOutputs/test_MR1p1_CDFrangeLargeSel_tkmet_RochCorr_RecoilCorr_Scale1_EffHeinerSFCorr_PileupSFCorr/test_numbers_DYJetsSig/ZanalysisOnDATA.root");
  files.push_back(file_temp);labels.push_back("MC corr scale+1"); colors.push_back(2);
  /////
  file_temp = new TFile("../JobOutputs/test_MR1p1_CDFrangeLargeSel_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/test_numbers_DYJetsSig/ZanalysisOnDATA.root");
  files.push_back(file_temp);labels.push_back("MC NO corr "); colors.push_back(4);
  
  // TString outputfolder="CDFrange/";  
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_CDFrange_tkmet_RochCorr_RecoilCorr_EffHeinerSFCorr_PileupSFCorr/test_numbers_WJetsSig/WanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC corr"); colors.push_back(1);
  // /////
  // file_temp = new TFile("../JobOutputs/test_MR1p1_CDFrange_tkmet_RochCorr_RecoilCorr_Scale1_EffHeinerSFCorr_PileupSFCorr/test_numbers_WJetsSig/WanalysisOnDATA.root");
  // files.push_back(file_temp);labels.push_back("MC corr scale+1"); colors.push_back(2);
  // /////
  // // file_temp = new TFile("../JobOutputs/test_MR1p1_CDFrange_tkmet_RochCorr_EffHeinerSFCorr_PileupSFCorr/test_numbers_WJetsSig/WanalysisOnDATA.root");
  // // files.push_back(file_temp);labels.push_back("MC NO corr "); colors.push_back(4);
  
  
  bool useUpDown = false;

  gROOT->ProcessLine(Form(".! mkdir %s",outputfolder.Data()));
  TIter nextkey(files[0]->GetListOfKeys());
  TKey *key;
  while (key = (TKey*)nextkey()) {
    if(!key->ReadObj()->InheritsFrom("TH1D")) continue;
    TString hname = key->ReadObj()->GetName();
    
    // if(hname.Contains("hPileUp_Fall11_noWeights")) continue;
    if(hname.Contains("QCD")) continue;
    if(hname.Contains("METScaled")) continue;
    if(hname.Contains("MtScaled")) continue;
    if(hname.Contains("PtScaled")) continue;
    if(hname.Contains("_80") && !hname.Contains("_80419")) continue;
    if(!hname.Contains("_8_")) continue;
    // if(!hname.Contains("NonScaled_8_")) continue;
    cout << "processing histo " << hname << endl;
    
    TCanvas*c1=new TCanvas("c1","c1",1200,800);
    double bottom = (files.size()>1?0.25:0.);
    TPad *pad1 = new TPad("pad1", "",0,bottom,1,1);
    // pad1->SetLogx(logx);
    // pad1->SetLogy(logy);
    // pad1->SetLogz(logz);
    pad1->SetTickx(1);
    pad1->SetTicky(1);
    bottom = (files.size()>1?0.:0.1);
    pad1->SetBottomMargin(bottom);
    pad1->SetTopMargin(0.075);
    pad1->Draw();
    TPad *pad2 = new TPad("pad2", "",0,0,1,0.25);
    pad2->SetTickx(1);
    pad2->SetTicky(1);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.35);
    if(files.size()>1) pad2->Draw();

    pad1->cd();  

    TH1D *h0=(TH1D*)files[0]->Get(key->ReadObj()->GetName());
    h0->Scale(1/h0->Integral());
    h0->SetLineColor(colors[0]);
    h0->SetMarkerColor(colors[0]);
    h0->DrawNormalized("histo");
    TLatex *t = new TLatex();
    t->SetNDC();
    t->SetTextFont(63);
    t->SetTextSizePixels(17);  
    t->SetTextColor(colors[0]);
    t->DrawLatex(0.55,0.85,Form("%s : Mean= %.2f RMS= %.2f",labels[0].Data(),h0->GetMean(),h0->GetRMS()));

    TH1D *h1,*h2;
    TH1D hmcdataratio,hmcdataratio2;
    TLine*l0=new TLine(h0->GetXaxis()->GetBinLowEdge(1),1,h0->GetXaxis()->GetBinCenter(h0->GetNbinsX()),1);l0->SetLineColor(kGray);
    TLine*lp5=new TLine(h0->GetXaxis()->GetBinLowEdge(1),1.025,h0->GetXaxis()->GetBinCenter(h0->GetNbinsX()),1.025);lp5->SetLineColor(kGray);
    TLine*lm5=new TLine(h0->GetXaxis()->GetBinLowEdge(1),0.975,h0->GetXaxis()->GetBinCenter(h0->GetNbinsX()),0.975);lm5->SetLineColor(kGray);
    
    if(files.size()>1){

      for(int ifile = 1; ifile<2/* files.size() */; ifile++){
    
      pad1->cd();
      
      h1=(TH1D*)files[ifile]->Get(key->ReadObj()->GetName());
      h1->Scale(1/h1->Integral());
      h1->SetLineColor(colors[ifile]);
      h1->SetMarkerColor(colors[ifile]);
      h1->DrawNormalized("same histo");
      
      t->SetTextColor(colors[ifile]);
      t->DrawLatex(0.55,0.85-0.025*ifile, Form("%s : Mean= %.2f RMS= %.2f",labels[ifile].Data(),h1->GetMean(),h1->GetRMS()));

      if(files.size()>2){
        
        h2=(TH1D*)files[ifile+1]->Get(key->ReadObj()->GetName());
        h2->Scale(1/h2->Integral());
        h2->SetLineColor(colors[ifile+1]);
        h2->SetMarkerColor(colors[ifile+1]);
        h2->DrawNormalized("same histo");
        t->SetTextColor(colors[ifile+1]);
        t->DrawLatex(0.55,0.85-0.025*(ifile+1), Form("%s : Mean= %.2f RMS= %.2f",labels[ifile+1].Data(),h2->GetMean(),h2->GetRMS()));
      }
      
      pad2->cd();

      hmcdataratio=(*h1)/(*h0);
      hmcdataratio.SetLineColor(colors[ifile]);
      hmcdataratio.SetMarkerColor(colors[ifile]);
      hmcdataratio.GetYaxis()->SetRangeUser(0.95,1.05);
      hmcdataratio.GetYaxis()->SetLabelSize(0.075);
      hmcdataratio.GetXaxis()->SetLabelSize(0.1);
      hmcdataratio.Draw(ifile==1?"":" same");
      
      l0->Draw("same");
      lp5->Draw("same");
      lm5->Draw("same");
      hmcdataratio.Draw("same");
        
      if(files.size()>2){
        
        pad2->cd();

        hmcdataratio2=(*h2)/(*h0);
        hmcdataratio2.SetLineColor(colors[ifile+1]);
        hmcdataratio2.SetMarkerColor(colors[ifile+1]);
        hmcdataratio2.GetYaxis()->SetRangeUser(0.9,1.1);
        hmcdataratio2.GetYaxis()->SetLabelSize(0.075);
        hmcdataratio2.GetXaxis()->SetLabelSize(0.1);
        hmcdataratio2.Draw(" same");
        
        hmcdataratio2.Draw("same");
      
        }
        
      }
    
    }
    
    c1->SaveAs(Form("%s/%s.png",outputfolder.Data(),key->ReadObj()->GetName()));
   
  }

}
