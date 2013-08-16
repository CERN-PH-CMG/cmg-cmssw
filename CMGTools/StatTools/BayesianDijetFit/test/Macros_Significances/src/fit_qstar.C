
void fit_qstar()
{

// histograms obtained with MyTestClassQstar.C

TFile *file1=TFile::Open("signalQstar.root");
TH1F* hist_mass = (TH1F*) file1->Get("hist_mass_Qstar;1");
TH1F* hist_DeltaEta = (TH1F*) file1->Get("hist_DeltaEta_Qstar;1");
TH1F* hist_ExpDeltaEta = (TH1F*) file1->Get("hist_ExpDeltaEta_Qstar;1");
TH1F* hist_CosThetaStar = (TH1F*) file1->Get("hist_CosThetaStar_Qstar;1");
TH1F* hist_mass1 = (TH1F*) file1->Get("hist_mass1_Qstar;1");
TH1F* hist_mass2 = (TH1F*) file1->Get("hist_mass2_Qstar;1");
TH1F* hist_mass3 = (TH1F*) file1->Get("hist_mass3_Qstar;1");

Double_t Ntot=hist_mass->GetEntries();
Double_t N1=hist_mass1->GetEntries();
Double_t N2=hist_mass2->GetEntries();
Double_t N3=hist_mass3->GetEntries();

cout<<"for 0<DeltaEta<0.5: "<<N1/Ntot*100<<" %"<<endl;
cout<<"for 0.5<DeltaEta<1: "<<N2/Ntot*100<<" %"<<endl;
cout<<"for 1<DeltaEta<1.3: "<<N3/Ntot*100<<" %"<<endl;



  TCanvas *Canvas_1 = new TCanvas("Canvas_1","Canvas_1",11,51,700,500);
  TCanvas *Canvas_2 = new TCanvas("Canvas_2","Canvas_2",11,51,700,500);
  TCanvas *Canvas_3 = new TCanvas("Canvas_3","Canvas_3",11,51,700,500);
  TCanvas *Canvas_4 = new TCanvas("Canvas_4","Canvas_4",11,51,700,500);
  TCanvas *Canvas_5 = new TCanvas("Canvas_5","Canvas_5",11,51,700,500);
  TCanvas *Canvas_6 = new TCanvas("Canvas_6","Canvas_6",11,51,700,500);
  TCanvas *Canvas_7 = new TCanvas("Canvas_7","Canvas_7",11,51,700,500);

  TLegend* leg1 = new TLegend(0.4, 0.7, 0.5, 0.75);
  TLegend* leg2 = new TLegend(0.2, 0.55, 0.3, 0.6);
  TLegend* leg3 = new TLegend(0.45, 0.45, 0.55, 0.5);
  TLegend* leg4 = new TLegend(0.2, 0.55, 0.3, 0.6);
  TLegend* leg5 = new TLegend(0.3, 0.7, 0.5, 0.8);
  TLegend* leg6 = new TLegend(0.3, 0.7, 0.5, 0.8);
  TLegend* leg7 = new TLegend(0.3, 0.7, 0.5, 0.8);



 Canvas_1->cd(0);
   hist_mass->Draw();
   leg1->AddEntry(hist_mass,"  q*","l");
   leg1->Draw();


TLine * line1=new TLine(0.5,0,0.5,1100);
TLine * line2=new TLine(1,0,1,900);
TLine * line3=new TLine(1.3,0,1.3,800);
 Canvas_2->cd(0);
   hist_DeltaEta->Draw();
   leg2->AddEntry(hist_DeltaEta,"  q*","l");
   leg2->Draw();
line1->SetLineColor(kRed);
line1->SetLineWidth(2);
line2->SetLineColor(kRed);
line2->SetLineWidth(2);
line3->SetLineColor(kRed);
line3->SetLineWidth(2);
line1->Draw("same");
line2->Draw("same");
line3->Draw("same");

 Canvas_3->cd(0);
   hist_ExpDeltaEta->Draw();
   leg3->AddEntry(hist_ExpDeltaEta,"  q*","l");
   leg3->Draw();

 Canvas_4->cd(0);
   hist_CosThetaStar->Draw();
   leg4->AddEntry(hist_CosThetaStar,"  q*","l");
   leg4->Draw();

 Canvas_5->cd(0);
 hist_mass1->Draw();
 leg5->AddEntry(hist_mass1,"q*   0<#Delta#eta<0.5","l");
 leg5->Draw();

 Canvas_6->cd(0);
 hist_mass2->Draw();
 leg6->AddEntry(hist_mass3,"q*   0.5<#Delta#eta<1","l");
 leg6->Draw();

 Canvas_7->cd(0);
 hist_mass3->Draw();
 leg7->AddEntry(hist_mass3,"q*   1<#Delta#eta<1.3","l");
 leg7->Draw();


  TH1F *hist_mass1_ratio = (TH1F*)hist_mass1->Clone("hist_mass1_Qstar_ratio"); 
  TCanvas *Canvas_8 = new TCanvas("Canvas_8","Canvas_8",11,51,700,500);
  hist_mass1_ratio->SetTitle("M1/M (q*)");
  hist_mass1_ratio->GetYaxis()->SetTitle("");
  TLegend* leg8 = new TLegend(0.15, 0.2, 0.7, 0.35);

  TH1F *hist_mass2_ratio = (TH1F*)hist_mass2->Clone("hist_mass2_Qstar_ratio");
  TCanvas *Canvas_9 = new TCanvas("Canvas_9","Canvas_9",11,51,700,500);
  hist_mass2_ratio->SetTitle("M2/M (q*)");
  hist_mass2_ratio->GetYaxis()->SetTitle("");
  TLegend* leg9 = new TLegend(0.15, 0.2, 0.7, 0.35);

  TH1F *hist_mass3_ratio = (TH1F*)hist_mass3->Clone("hist_mass3_Qstar_ratio");
  TCanvas *Canvas_10 = new TCanvas("Canvas_10","Canvas_10",11,51,700,500);
  hist_mass3_ratio->SetTitle("M3/M (q*)");
  hist_mass3_ratio->GetYaxis()->SetTitle("");
  TLegend* leg10 = new TLegend(0.15, 0.2, 0.7, 0.35);

/*
  double Integral1=hist_mass1_ratio->Integral();
  hist_mass1_ratio->Scale(1/Integral1);
 
  double Integral2=hist_mass2_ratio->Integral();
  hist_mass2_ratio->Scale(1/Integral2);

  double Integral3=hist_mass3_ratio->Integral();
  hist_mass3_ratio->Scale(1/Integral3);
*/
  cout<<"integral1: "<<hist_mass1_ratio->Integral()<<endl;
  cout<<"integral2: "<<hist_mass2_ratio->Integral()<<endl;
  cout<<"integral3: "<<hist_mass3_ratio->Integral()<<endl;


 TF1 *f1= new TF1("func1","[0]+[1]*x");
 f1->SetParameters(1,0);

 TF1 *f2= new TF1("func2","[0]+[1]*x");
 f2->SetParameters(1,0);

 TF1 *f3= new TF1("func3","[0]+[1]*x");
 f3->SetParameters(1,0);

 gStyle->SetOptFit(111); 

 Canvas_8->cd(0);
 hist_mass1_ratio->Divide(hist_mass1_ratio,hist_mass,1,1,"B");
 hist_mass1_ratio->Fit("func1");
 hist_mass1_ratio->Draw();
 leg8->AddEntry(hist_mass1_ratio,"mass ( 0<#Delta#eta<0.5 ) / mass ( 0<#Delta#eta<1.3 )","l");
 //leg8->AddEntry((TObject*)0,"mass ( 0<#Delta#eta<1.3 )","");
 leg8->AddEntry((TObject*)0,"","");
 leg8->AddEntry(func1,"linear fit (p0+p1#timesx)","l");
 leg8->Draw();
 //hist_mass1_ratio->Rebin();

 Canvas_9->cd(0);
 hist_mass2_ratio->Divide(hist_mass2_ratio,hist_mass,1,1,"B");
 hist_mass2_ratio->Fit("func2");
 hist_mass2_ratio->Draw();
 leg9->AddEntry(hist_mass2_ratio," mass ( 0.5<#Delta#eta<1 ) / mass ( 0<#Delta#eta<1.3 )","l");
 //leg9->AddEntry((TObject*)0,"mass ( 0<#Delta#eta<1.3 )","");
 leg9->AddEntry((TObject*)0,"","");
 leg9->AddEntry(func2,"linear fit (p0+p1#timesx)","l");
 leg9->Draw();
 //hist_mass2_ratio->Rebin();

 Canvas_10->cd(0);
 hist_mass3_ratio->Divide(hist_mass3_ratio,hist_mass,1,1,"B");
 hist_mass3_ratio->Fit("func3");
 hist_mass3_ratio->Draw();
 leg10->AddEntry(hist_mass3_ratio,"mass ( 1<#Delta#eta<1.3 ) / mass ( 0<#Delta#eta<1.3 )","l");
 //leg10->AddEntry((TObject*)0,"mass ( 0<#Delta#eta<1.3 )","");
 leg10->AddEntry((TObject*)0,"","");
 leg10->AddEntry(func3,"linear fit (p0+p1#timesx)","l");
 leg10->Draw();
 //hist_mass3_ratio->Rebin();



}
