#include "TString.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"




void plotTriggerEffFits(TString effFile,float xmin=0,float xmax=0){
  cout<<effFile<<" "<<xmin<<" "<<xmax<<endl;

  TriggerEfficiency eff;
  eff.fitEfficiency(effFile.Data(),xmin,xmax);
  //TFile FitResultsFile(TString("TrigEffFitResult_")+effFile.Data(),"read");
  TFile FitResultsFile(effFile+"_Fit.root","read");
  gROOT->cd();


  TCanvas C;
  ((TH1F*)(FitResultsFile.Get("efficiency")))->GetYaxis()->SetRangeUser(0.,1);
  ((TH1F*)(FitResultsFile.Get("efficiency")))->GetYaxis()->SetTitle("Efficiency");
  ((TH1F*)(FitResultsFile.Get("efficiency")))->Draw("histpe");
  TGraph*GFitResultsFile=((TGraph*)(FitResultsFile.Get("Fit")));
  GFitResultsFile=(TGraph*)GFitResultsFile->Clone("GFitResultsFile");
  GFitResultsFile->SetLineColor(2);
  GFitResultsFile->Draw("lsame");
  ((TH1F*)(FitResultsFile.Get("efficiency")))->Draw("histpesame");
  //C.Print(TString("TrigEffFitPlot_")+effFile.Data()+".ps");  

  TH1F*HFitPars=((TH1F*)(FitResultsFile.Get("FitParameters")));

  TLatex title;
  title.SetTextSize(0.025);
  title.DrawTextNDC(0.21,.97,effFile.Data());

  char pars[100];
  sprintf(pars,"m0=%.2f, sigma=%.6f, alpha=%.6f, n=%.6f, norm=%.4f\n",HFitPars->GetBinContent(1),HFitPars->GetBinContent(2),HFitPars->GetBinContent(3),HFitPars->GetBinContent(4),HFitPars->GetBinContent(5));
  title.DrawTextNDC(0.21,.94,pars);
  //C.Print(TString("TrigEffFitPlot_")+effFile.Data()+".png");  
  C.Print(effFile+"_Plot.png");  

 
  std::cout<<endl<<effFile.Data()<<" : "<<endl;
  //printf("m0        sigma    alpha    n        norm\n");
  printf("%.6f,    %.6f,    %.6f,    %.6f,    %.6f\n",HFitPars->GetBinContent(1),HFitPars->GetBinContent(2),HFitPars->GetBinContent(3),HFitPars->GetBinContent(4),HFitPars->GetBinContent(5));
  std::cout<<endl;

}


void runTriggerEffFits(){

  //need the TriggerEfficiency class loaded for the fit
  gSystem->Load("libCMGToolsH2TauTau.so");


//   ///Run2012A
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_Data_A.root");
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_Data_A.root");

//   ///Run2012B
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_Data_B.root");
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_Data_B.root");

//   ////Run2012C
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_Data_C.root");
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_Data_C.root");

//   ///Run2012D
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_Data_D.root");
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_Data_D.root");

//   ///Run2012ABC
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_Data_ABC.root");
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_Data_ABC.root");

//   ///Run2012ABCD
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_Data_ABCD.root");
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_Data_ABCD.root");

//   //53X MC
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_MC_AB.root");
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_MC_AB.root");

//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_MC_ABC.root");
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_MC_ABC.root");

//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_MC_D.root");
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_MC_D.root");

 


//   ///////////////////////////mu-Tau Dec21
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_AB_Data.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_AB_Data.root",19,50);

//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_ABC_Data.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_ABC_Data.root",19,50);

//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_D_Data.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_D_Data.root",19,35);


//   //53X MC
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_AB_MC.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_AB_MC.root",19,40);

//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_ABC_MC.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_ABC_MC.root",19,45);

//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_D_MC.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_D_MC.root",19,50);


//   ///////////////////////////e-Tau Dec21
//   ///
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_Barrel_AB_Data.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_EndCap_AB_Data.root",19,45);

//   plotTriggerEffFits("TauTriggerEfficiency_eTau_Barrel_ABC_Data.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_EndCap_ABC_Data.root",19,50);

//   plotTriggerEffFits("TauTriggerEfficiency_eTau_Barrel_D_Data.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_EndCap_D_Data.root",19,35);


//   //53X MC
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_Barrel_AB_MC.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_EndCap_AB_MC.root",19,40);

//   plotTriggerEffFits("TauTriggerEfficiency_eTau_Barrel_ABC_MC.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_EndCap_ABC_MC.root",19,45);

//   plotTriggerEffFits("TauTriggerEfficiency_eTau_Barrel_D_MC.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_EndCap_D_MC.root",19,50);



  ///////////////////////////mu-Tau Dec25
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_ABCD_Data.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_ABCD_Data.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_ABCD_MC.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_ABCD_MC.root",19,50);

//   plotTriggerEffFits("TauTriggerEfficiency_eTau_Barrel_ABCD_Data.root",19,40);
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_EndCap_ABCD_Data.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_Barrel_ABCD_MC.root",19,50);
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_EndCap_ABCD_MC.root",19,50);


  ///////////////////////////mu-Tau Jan11
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_ABCD_Data.root",18,50);
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_ABCD_Data.root",18,50);
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_Barrel_ABCD_MC.root",18,50);
//   plotTriggerEffFits("TauTriggerEfficiency_muTau_EndCap_ABCD_MC.root",18,50);

//   plotTriggerEffFits("TauTriggerEfficiency_eTau_Barrel_ABCD_Data.root",18,40);
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_EndCap_ABCD_Data.root",18,50);
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_Barrel_ABCD_MC.root",18,50);
//   plotTriggerEffFits("TauTriggerEfficiency_eTau_EndCap_ABCD_MC.root",18,50);


  ///Summer13 ReReco mu-Tau
  //TString path="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/muTau";
  //TString path="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/muTauABC";
  //TString path="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/muTauD";
  //TString path="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/muTau_highTauPt";
  //TString path="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/muTauABCD_June28";
  //TString path="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/muTauABCD_June30";
  TString path="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/muTauABCD_June30_decaymode0";
//   plotTriggerEffFits(path+"/TauTriggerEfficiency_muTau_Barrel_Summer13ReReco_Data.root",18,60);
//   plotTriggerEffFits(path+"/TauTriggerEfficiency_muTau_EndCap_Summer13ReReco_Data.root",18,60);
//   plotTriggerEffFits(path+"/TauTriggerEfficiency_muTau_Barrel_Summer13ReReco_MC.root",18,60);
//   plotTriggerEffFits(path+"/TauTriggerEfficiency_muTau_EndCap_Summer13ReReco_MC.root",18,60);

  //TString pathETau="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/eTau_antiETight";
  //TString pathETau="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/eTau_antiEMedium";
  //TString pathETau="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/eTauABC_antiEMedium";
  //TString pathETau="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/eTauD_antiEMedium";
  //TString pathETau="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/eTauABCD_June28";
//   TString pathETau="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/eTauABCD_June30";
  TString pathETau="/afs/cern.ch/user/b/benitezj/www/Summer13Studies/TauTrigger/eTauABCD_June30_decaymode0";
//   plotTriggerEffFits(pathETau+"/TauTriggerEfficiency_eTau_Barrel_Summer13ReReco_Data.root",18,40);
//   plotTriggerEffFits(pathETau+"/TauTriggerEfficiency_eTau_EndCap_Summer13ReReco_Data.root",18,40);
//   plotTriggerEffFits(pathETau+"/TauTriggerEfficiency_eTau_Barrel_Summer13ReReco_MC.root",18,40);
//   plotTriggerEffFits(pathETau+"/TauTriggerEfficiency_eTau_EndCap_Summer13ReReco_MC.root",18,40);



  gROOT->ProcessLine(".q");
}

// void plotTriggerEffFits(){
  
//   TCanvas C;
//   C.Print("plotTriggerEffFits.ps[");

//   TFile FitBarrelData("TrigEffFitResult_TauTriggerEfficiency_muTau_Barrel_Data.root","read");
//   TFile FitBarrelMC("TrigEffFitResult_TauTriggerEfficiency_muTau_Barrel_MC.root","read");
//   TFile FitEndCapData("TrigEffFitResult_TauTriggerEfficiency_muTau_EndCap_Data.root","read");
//   TFile FitEndCapMC("TrigEffFitResult_TauTriggerEfficiency_muTau_EndCap_MC.root","read");
//   gROOT->cd();

//   ///Barrel
//   C.Clear();
//   ((TH1F*)(FitBarrelData.Get("efficiency")))->GetYaxis()->SetRangeUser(0.5,1);
//   ((TH1F*)(FitBarrelData.Get("efficiency")))->Draw("histpe");
//   TGraph*GFitBarrelData=((TGraph*)(FitBarrelData.Get("Fit")));
//   GFitBarrelData=(TGraph*)GFitBarrelData->Clone("GFitBarrelData");
//   GFitBarrelData->SetLineColor(2);
//   GFitBarrelData->Draw("lsame");
//   ((TH1F*)(FitBarrelData.Get("efficiency")))->Draw("histpesame");

//   ///comment this 3 lines to plot Data and MC on same page
//   C.Print("plotTriggerEffFits.ps");  
//   ((TH1F*)(FitBarrelMC.Get("efficiency")))->GetYaxis()->SetRangeUser(0.5,1);
//   ((TH1F*)(FitBarrelMC.Get("efficiency")))->GetYaxis()->SetTitle("Efficiency");
//   ((TH1F*)(FitBarrelMC.Get("efficiency")))->Draw("histpe");

//   TGraph*GFitBarrelMC=((TGraph*)(FitBarrelMC.Get("Fit")));
//   GFitBarrelMC=(TGraph*)GFitBarrelMC->Clone("GFitBarrelMC");
//   GFitBarrelMC->SetLineColor(2);
//   GFitBarrelMC->Draw("lsame");
//   ((TH1F*)(FitBarrelMC.Get("efficiency")))->Draw("histpesame");

//   C.Print("plotTriggerEffFits.ps");  

//   //EndCap
//   C.Clear();
//   ((TH1F*)(FitEndCapData.Get("efficiency")))->GetYaxis()->SetRangeUser(0.5,1);
//   ((TH1F*)(FitEndCapData.Get("efficiency")))->Draw("histpe");
//   TGraph*GFitEndCapData=((TGraph*)(FitEndCapData.Get("Fit")));
//   GFitEndCapData=(TGraph*)GFitEndCapData->Clone("GFitEndCapData");
//   GFitEndCapData->SetLineColor(2);
//   GFitEndCapData->Draw("lsame");
//   ((TH1F*)(FitEndCapData.Get("efficiency")))->Draw("histpesame");

//   ///comment this 3 lines to plot Data and MC on same page
//   C.Print("plotTriggerEffFits.ps");  
//   ((TH1F*)(FitEndCapMC.Get("efficiency")))->GetYaxis()->SetRangeUser(0.5,1);
//   ((TH1F*)(FitEndCapMC.Get("efficiency")))->GetYaxis()->SetTitle("Efficiency");
//   ((TH1F*)(FitEndCapMC.Get("efficiency")))->Draw("histpe");


//   TGraph*GFitEndCapMC=((TGraph*)(FitEndCapMC.Get("Fit")));
//   GFitEndCapMC=(TGraph*)GFitEndCapMC->Clone("GFitEndCapMC");
//   GFitEndCapMC->SetLineColor(2);
//   GFitEndCapMC->Draw("lsame");
//   ((TH1F*)(FitEndCapMC.Get("efficiency")))->Draw("histpesame");

//   C.Print("plotTriggerEffFits.ps");  


  
//   C.Print("plotTriggerEffFits.ps]");

// }
