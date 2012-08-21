
void fit_angular()
{

// histograms obtained with MyTestClassQstar.C

  int Detastart=1; // this is the bin number
  int Detacut1=5; // last bin for the 1st region
  int Detacut2=10; // last bin for the 2nd region
  int Detaend=13;
  double mass = 2000.;
  //string Signal("QStar"); string spin("");
  //string Signal("RSG");  string spin("_gg");
  string Signal("RSG");  string spin("_qq");
  double min = 0.6*mass, max = 1.3*mass;



  string File = "data/Signal/" +  Signal + "_D6T_" + Form("%.0f", mass) + ".root";

  cout << File.c_str() << endl;

  TFile *file1=TFile::Open(File.c_str());

  string hist("dijet_deta_vs_mass");
  hist = hist + "" + spin + ";1";

  TH2F* h_DEta_Mass =  (TH2F*) file1->Get(hist.c_str());
  h_DEta_Mass->RebinY(10);


TH1D* hist_mass1 = h_DEta_Mass->ProjectionY("hist_mass1",Detastart,Detacut1,"e");
TH1D* hist_mass2 = h_DEta_Mass->ProjectionY("hist_mass2",Detacut1+1,Detacut2,"e");
TH1D* hist_mass3 = h_DEta_Mass->ProjectionY("hist_mass3",Detacut2+1,Detaend,"e");
TH1D* hist_mass = h_DEta_Mass->ProjectionY("hist_mass",Detastart,Detaend,"e");

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
  TLegend* leg8 = new TLegend(0.60, 0.2, 0.9, 0.35);

  TH1F *hist_mass2_ratio = (TH1F*)hist_mass2->Clone("hist_mass2_Qstar_ratio");
  TCanvas *Canvas_9 = new TCanvas("Canvas_9","Canvas_9",11,51,700,500);
  hist_mass2_ratio->SetTitle("M2/M (q*)");
  hist_mass2_ratio->GetYaxis()->SetTitle("");
  TLegend* leg9 = new TLegend(0.60, 0.2, 0.9, 0.35);

  TH1F *hist_mass3_ratio = (TH1F*)hist_mass3->Clone("hist_mass3_Qstar_ratio");
  TCanvas *Canvas_10 = new TCanvas("Canvas_10","Canvas_10",11,51,700,500);
  hist_mass3_ratio->SetTitle("M3/M (q*)");
  hist_mass3_ratio->GetYaxis()->SetTitle("");
  TLegend* leg10 = new TLegend(0.60, 0.2, 0.9, 0.35);

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


  TF1 *f1= new TF1("func1", "[0]+[1]*(x-[2])");
 f1->SetParameters(1,0);
 f1->FixParameter(2,mass);

 TF1 *f2= new TF1("func2","[0]+[1]*(x-[2])");
 f2->SetParameters(1,0);
 f2->FixParameter(2,mass);

 TF1 *f3= new TF1("func3","[0]+[1]*(x-[2])");
 f3->SetParameters(1,0);
 f3->FixParameter(2,mass);

 gStyle->SetOptFit(111); 

 Canvas_8->cd(0);
 hist_mass1_ratio->Divide(hist_mass1_ratio,hist_mass,1,1,"B");
 hist_mass1_ratio->Fit("func1", "L", "", min, max);
 hist_mass1_ratio->Draw();
 leg8->AddEntry(hist_mass1_ratio,"mass ( 0<#Delta#eta<0.5 ) / mass ( 0<#Delta#eta<1.3 )","l");
 //leg8->AddEntry((TObject*)0,"mass ( 0<#Delta#eta<1.3 )","");
 leg8->AddEntry((TObject*)0,"","");
 leg8->AddEntry(func1,"linear fit (p0+p1#timesx)","l");
 leg8->Draw();
 //hist_mass1_ratio->Rebin();

 Canvas_9->cd(0);
 hist_mass2_ratio->Divide(hist_mass2_ratio,hist_mass,1,1,"B");
 hist_mass2_ratio->Fit("func2",  "L", "", min, max);
 hist_mass2_ratio->Draw();
 leg9->AddEntry(hist_mass2_ratio," mass ( 0.5<#Delta#eta<1 ) / mass ( 0<#Delta#eta<1.3 )","l");
 //leg9->AddEntry((TObject*)0,"mass ( 0<#Delta#eta<1.3 )","");
 leg9->AddEntry((TObject*)0,"","");
 leg9->AddEntry(func2,"linear fit (p0+p1#timesx)","l");
 leg9->Draw();
 //hist_mass2_ratio->Rebin();

 Canvas_10->cd(0);
 hist_mass3_ratio->Divide(hist_mass3_ratio,hist_mass,1,1,"B");
 hist_mass3_ratio->Fit("func3",  "L", "", min, max);
 hist_mass3_ratio->Draw();
 leg10->AddEntry(hist_mass3_ratio,"mass ( 1<#Delta#eta<1.3 ) / mass ( 0<#Delta#eta<1.3 )","l");
 //leg10->AddEntry((TObject*)0,"mass ( 0<#Delta#eta<1.3 )","");
 leg10->AddEntry((TObject*)0,"","");
 leg10->AddEntry(func3,"linear fit (p0+p1#timesx)","l");
 leg10->Draw();
 //hist_mass3_ratio->Rebin();



}
