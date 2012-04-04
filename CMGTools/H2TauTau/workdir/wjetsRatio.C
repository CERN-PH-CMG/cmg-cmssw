void wjetsRatio(long njets=0){
  Sample  wjets("WJetsToLNu","/data/benitezj/Samples/Mar19MuJet");
  //wjets.setDataType("MC");
  //wjets.setCrossection(31314);//in pb
  //wjets.setSampleGenEvents(24923318);
  //float lumi=4700;//in 1/pb
  //wjets.scale(lumi/(wjets.getLumi()));


  TString eventweight="eventweight";  

  int nbins=16; float xmin=20; float xmax=100;
  TString jetpt="taujetpt";
  TString select=TString("*(muiso<0.1&&njet==")+njets+")";

//   TString jetpt="mujetpt";
//   TString select="*(tauisodisc>=2)";
  ///--> there is very little anti-Iso muons ~ 10%


  
  //low MT
  TH1F*HAMT1=wjets.getHistoNtpFile(jetpt,nbins,xmin,xmax,eventweight+"*(categoryCh==1&&categoryMT==1)*(nditau==1&&categoryIso==2)"+select);
  HAMT1->Scale(wjets.getNorm());
  HAMT1->SetName("HAMT1");
  
  TH1F*HIMT1=wjets.getHistoNtpFile(jetpt,nbins,xmin,xmax,eventweight+"*(categoryCh==1&&categoryMT==1)*(categoryIso==1)"+select);
  HIMT1->Scale(wjets.getNorm());
  HIMT1->SetName("HIMT1");
  
//   TH1F*HASSMT1=wjets.getHistoNtpFile(jetpt,nbins,xmin,xmax,eventweight+"*(categoryCh==2&&categoryMT==1)*(nditau==1&&categoryIso==2)"+select);
//   HASSMT1->Scale(wjets.getNorm());
//   HASSMT1->SetName("HASSMT1");
 
//   TH1F*HISSMT1=wjets.getHistoNtpFile(jetpt,nbins,xmin,xmax,eventweight+"*(categoryCh==2&&categoryMT==1)*(categoryIso==1)"+select);
//   HISSMT1->Scale(wjets.getNorm());
//   HISSMT1->SetName("HISSMT1");

  //high MT
  TH1F*HAMT3=wjets.getHistoNtpFile(jetpt,nbins,xmin,xmax,eventweight+"*(categoryCh==1&&categoryMT==3)*(nditau==1&&categoryIso==2)"+select);
  HAMT3->Scale(wjets.getNorm());
  HAMT3->SetName("HAMT3");
  
  TH1F*HIMT3=wjets.getHistoNtpFile(jetpt,nbins,xmin,xmax,eventweight+"*(categoryCh==1&&categoryMT==3)*(categoryIso==1)"+select);
  HIMT3->Scale(wjets.getNorm());
  HIMT3->SetName("HIMT3");
  
//   TH1F*HASSMT3=wjets.getHistoNtpFile(jetpt,nbins,xmin,xmax,eventweight+"*(categoryCh==2&&categoryMT==3)*(nditau==1&&categoryIso==2)"+select);
//   HASSMT3->Scale(wjets.getNorm());
//   HASSMT3->SetName("HASSMT3");
 
//   TH1F*HISSMT3=wjets.getHistoNtpFile(jetpt,nbins,xmin,xmax,eventweight+"*(categoryCh==2&&categoryMT==3)*(categoryIso==1)"+select);
//   HISSMT3->Scale(wjets.getNorm());
//   HISSMT3->SetName("HISSMT3");


  


  TCanvas C("wjetsRatio");
  TString file=TString("wjetsRatio_")+njets+".ps";
  TLine line;

  C.Print(file+"[");


  //Compare Low and High MT in OS
  TH1F HJetPtRMT1("HJetPtRMT1","HJetPtRMT1",nbins,xmin,xmax);
  HJetPtRMT1.Add(HIMT1);
  HJetPtRMT1.Divide(HAMT1);
  TH1F HJetPtRMT3("HJetPtRMT3","HJetPtRMT3",nbins,xmin,xmax);
  HJetPtRMT3.Add(HIMT3);
  HJetPtRMT3.Divide(HAMT3);
  HJetPtRMT1.GetYaxis()->SetRangeUser(1e-2,50);
  C.Clear();
  C.SetLogy(1);
  HJetPtRMT1.Draw("histpe");
  HJetPtRMT3.SetMarkerColor(2);
  HJetPtRMT3.SetLineColor(2);
  HJetPtRMT3.Draw("histpesame");
  C.Print(file);
  C.SetLogy(0);

  TH1F HJetPtRCompare("HJetPtRCompare","HJetPtRCompare",nbins,xmin,xmax);
  HJetPtRCompare.Add(&HJetPtRMT3);
  HJetPtRCompare.Divide(&HJetPtRMT1);
  C.Clear();
  HJetPtRCompare.GetYaxis()->SetRangeUser(0,2);
  HJetPtRCompare.Draw("pe");
  line.DrawLine(xmin,1,xmax,1);
  C.Print(file);

//   //Compare Low and High MT in SS

//   TH1F HJetPtRSSMT1("HJetPtRSSMT1","HJetPtRSSMT1",nbins,xmin,xmax);
//   HJetPtRSSMT1.Add(HISSMT1);
//   HJetPtRSSMT1.Divide(HASSMT1);
//   TH1F HJetPtRSSMT3("HJetPtRSSMT3","HJetPtRSSMT3",nbins,xmin,xmax);
//   HJetPtRSSMT3.Add(HISSMT3);
//   HJetPtRSSMT3.Divide(HASSMT3);
//   C.Clear();
//   C.SetLogy(1);
//   HJetPtRSSMT1.GetYaxis()->SetRangeUser(1e-2,50);
//   HJetPtRSSMT1.Draw("histpe");
//   HJetPtRSSMT3.SetMarkerColor(2);
//   HJetPtRSSMT3.SetLineColor(2);
//   HJetPtRSSMT3.Draw("histpesame");
//   C.Print(file);
//   C.SetLogy(0);

//   TH1F HJetPtRCompareSS("HJetPtRCompareSS","HJetPtRCompareSS",nbins,xmin,xmax);
//   HJetPtRCompareSS.Add(&HJetPtRSSMT3);
//   HJetPtRCompareSS.Divide(&HJetPtRSSMT1);
//   C.Clear();
//   HJetPtRCompareSS.GetYaxis()->SetRangeUser(0,2);
//   HJetPtRCompareSS.Draw("pe");
//   line.DrawLine(xmin,1,xmax,1);
//   C.Print(file);


  //Save the histograms in ROOT file
  TFile RootFile(TString("wjetsRatio_")+njets+".root","recreate");
  HJetPtRMT1.Write();
  HJetPtRMT3.Write();
//  HJetPtRSSMT1.Write();
//   HJetPtRSSMT3.Write();
  RootFile.ls();
  RootFile.Close();



  C.Print(file+"]");
}



void compareWJetsRatioNjets(){

  TFile FNjet0("wjetsRatio_0.root");  
  TFile FNjet1("wjetsRatio_1.root"); 
  TFile FNjet2("wjetsRatio_2.root");  
  TH1F*HNjet0=(TH1F*)FNjet0.Get("HJetPtRMT3");
  TH1F*HNjet1=(TH1F*)FNjet1.Get("HJetPtRMT3");
  TH1F*HNjet2=(TH1F*)FNjet2.Get("HJetPtRMT3");

  TString file="compareWJetsRatioNjets.ps";
  TCanvas C(file);
  C.Print(file+"[");

  //compare Njet==1/Njet==0
  C.Clear();
  C.SetLogy(1);
  HNjet1->GetYaxis()->SetRangeUser(1e-2,50);
  HNjet1->Draw("histpe");
  HNjet0->SetMarkerColor(1);
  HNjet0->SetLineColor(1);
  HNjet0->Draw("histpesame");
  C.Print(file);
  C.SetLogy(0);

  TH1F * HJetPtRCompare=(TH1F*)HNjet1->Clone("HJetPtRCompare");
  HJetPtRCompare->Divide(HNjet0);
  TLine line;
  C.Clear();
  delete HJetPtRCompare->FindObject("stats");
  HJetPtRCompare->SetStats(0);
  HJetPtRCompare->SetTitle("");
  HJetPtRCompare->GetYaxis()->SetRangeUser(0,2);
  //HJetPtRCompare->Draw("pe");
  HJetPtRCompare->Fit("pol0","","pe",HJetPtRCompare->GetXaxis()->GetXmin(),HJetPtRCompare->GetXaxis()->GetXmax());
  line.DrawLine(HJetPtRCompare->GetXaxis()->GetXmin(),1,HJetPtRCompare->GetXaxis()->GetXmax(),1);  
  C.Print(file);

  //compare Njet==2/Njet==0
  C.Clear();
  C.SetLogy(1);
  HNjet2->GetYaxis()->SetRangeUser(1e-2,50);
  HNjet2->Draw("histpe");
  HNjet0->SetMarkerColor(1);
  HNjet0->SetLineColor(1);
  HNjet0->Draw("histpesame");
  C.Print(file);
  C.SetLogy(0);

  TH1F * HJetPtRCompare2=(TH1F*)HNjet2->Clone("HJetPtRCompare2");
  HJetPtRCompare2->Divide(HNjet0);
  TLine line;
  C.Clear();
  delete HJetPtRCompare2->FindObject("stats");
  HJetPtRCompare2->SetStats(0);
  HJetPtRCompare2->SetTitle("");
  HJetPtRCompare2->GetYaxis()->SetRangeUser(0,2);
  //HJetPtRCompare2->Draw("pe");
  //HJetPtRCompare2->Fit("pol0","","pe",HJetPtRCompare2->GetXaxis()->GetXmin(),HJetPtRCompare2->GetXaxis()->GetXmax());
  HJetPtRCompare2->Fit("pol0","","pe",30,100);
  line.DrawLine(HJetPtRCompare2->GetXaxis()->GetXmin(),1,HJetPtRCompare2->GetXaxis()->GetXmax(),1);  
  C.Print(file);



  C.Print(file+"]");

}


void compareWJetsRatioMCData(){

  TFile MC("wjetsRatio_0.root");  
  TH1F* HMC=(TH1F*)MC.Get("HJetPtRMT3");

  TFile Data("/data/benitezj/Samples/Mar19MuJet/TauMuPlotterTauFakeRateWJets_taujetptMTcat3_SMcat-11X_nditauQQ1AAmuisoL0.1_X_njetQQ0_.root");
  //TFile Data("/data/benitezj/Samples/Mar19MuJet/TauMuPlotterTauFakeRateWJets_taujetptMTcat3_SMcat-11X_nditauQQ1AAmuisoL0.1_X_muisoL0.1AAnjetQQ1_.root");
  //TFile Data("/data/benitezj/Samples/Mar19MuJet/TauMuPlotterTauFakeRateWJets_taujetptMTcat3_SMcat-11X_nditauQQ1_X_muisoL0.1AAnjetQQ2_.root");
  TH1F* HData=(TH1F*)Data.Get("hRatio");

  TString file="compareWJetsRatioMCData.ps";
  TCanvas C(file);
  C.Print(file+"[");

  C.Clear();
  C.SetLogy(1);
  HData->GetYaxis()->SetRangeUser(1e-2,50);
  HData->Draw("histpe");
  HMC->SetMarkerColor(2);
  HMC->SetLineColor(2);
  HMC->Draw("histpesame");
  C.Print(file);
  C.SetLogy(0);

  TH1F * HJetPtRCompare=(TH1F*)HData->Clone("HJetPtRCompare");
  HJetPtRCompare->Divide(HMC);
  TLine line;
  C.Clear();
  delete HJetPtRCompare->FindObject("stats");
  HJetPtRCompare->SetStats(0);
  HJetPtRCompare->SetTitle("");
  HJetPtRCompare->GetYaxis()->SetRangeUser(0,2);
  //HJetPtRCompare->Draw("pe");
  HJetPtRCompare->Fit("pol0","","pe",HJetPtRCompare->GetXaxis()->GetXmin(),HJetPtRCompare->GetXaxis()->GetXmax());
  line.DrawLine(HJetPtRCompare->GetXaxis()->GetXmin(),1,HJetPtRCompare->GetXaxis()->GetXmax(),1);  
  C.Print(file);



  C.Print(file+"]");

}
