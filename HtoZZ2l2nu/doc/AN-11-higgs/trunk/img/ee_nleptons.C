{
//=========Macro generated from canvas: recolevelc/recolevelc
//=========  (Sun Sep 11 15:15:58 2011) by ROOT version5.24/00
   TCanvas *recolevelc = new TCanvas("recolevelc", "recolevelc",67,47,804,804);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   recolevelc->SetHighLightColor(2);
   recolevelc->Range(0,0,1,1);
   recolevelc->SetFillColor(0);
   recolevelc->SetBorderMode(0);
   recolevelc->SetBorderSize(2);
   recolevelc->SetTickx(1);
   recolevelc->SetTicky(1);
   recolevelc->SetLeftMargin(0.15);
   recolevelc->SetRightMargin(0.05);
   recolevelc->SetTopMargin(0.05);
   recolevelc->SetFrameFillStyle(0);
   recolevelc->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: recolevelc_1
   TPad *recolevelc_1 = new TPad("recolevelc_1", "recolevelc_1",0,0.3,1,1);
   recolevelc_1->Draw();
   recolevelc_1->cd();
   recolevelc_1->Range(-0.6486486,-4.426829,3.405405,6.548781);
   recolevelc_1->SetFillColor(0);
   recolevelc_1->SetBorderMode(0);
   recolevelc_1->SetBorderSize(2);
   recolevelc_1->SetLogy();
   recolevelc_1->SetGridx();
   recolevelc_1->SetGridy();
   recolevelc_1->SetTickx(1);
   recolevelc_1->SetTicky(1);
   recolevelc_1->SetLeftMargin(0.16);
   recolevelc_1->SetTopMargin(0.05);
   recolevelc_1->SetBottomMargin(0.13);
   recolevelc_1->SetFrameFillStyle(0);
   recolevelc_1->SetFrameBorderMode(0);
   recolevelc_1->SetFrameFillStyle(0);
   recolevelc_1->SetFrameBorderMode(0);
   
   TH1 *recolevelc_1_frame__775 = new TH1F("recolevelc_1_frame__775","recolevelc_1_stackcontour",3,0,3);
   recolevelc_1_frame__775->SetBinContent(1,468539.3);
   recolevelc_1_frame__775->SetBinContent(2,594.7979);
   recolevelc_1_frame__775->SetBinContent(3,9.267117);
   recolevelc_1_frame__775->SetBinError(1,369.261);
   recolevelc_1_frame__775->SetBinError(2,12.38713);
   recolevelc_1_frame__775->SetBinError(3,0.4285862);
   recolevelc_1_frame__775->SetMinimum(0.001);
   recolevelc_1_frame__775->SetMaximum(1000000);
   recolevelc_1_frame__775->SetEntries(5104181);
   recolevelc_1_frame__775->SetDirectory(0);
   recolevelc_1_frame__775->SetFillColor(1);
   recolevelc_1_frame__775->SetFillStyle(0);
   recolevelc_1_frame__775->SetMarkerStyle(0);
   recolevelc_1_frame__775->GetXaxis()->SetTitle("Leptons");
   recolevelc_1_frame__775->GetXaxis()->SetBinLabel(1,"=2 leptons");
   recolevelc_1_frame__775->GetXaxis()->SetBinLabel(2,"=3 leptons");
   recolevelc_1_frame__775->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   recolevelc_1_frame__775->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__775->GetYaxis()->SetTitle("Events");
   recolevelc_1_frame__775->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__775->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__775->Draw("hist");
   
   THStack *stack = new THStack();
   stack->SetName("stack");
   stack->SetTitle("recolevelc_1");
   
   TH1 *stack388__7761 = new TH1F("stack388__7761","recolevelc_1",3,0,3);
   stack388__7761->SetMinimum(2.471393);
   stack388__7761->SetMaximum(913610.3);
   stack388__7761->SetDirectory(0);
   stack388__7761->SetStats(0);
   stack388__7761->SetLineStyle(0);
   stack388__7761->SetMarkerStyle(20);
   stack388__7761->GetXaxis()->SetBinLabel(1,"=2 leptons");
   stack388__7761->GetXaxis()->SetBinLabel(2,"=3 leptons");
   stack388__7761->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   stack388__7761->GetXaxis()->SetLabelFont(42);
   stack388__7761->GetXaxis()->SetLabelOffset(0.007);
   stack388__7761->GetXaxis()->SetTitleSize(0.05);
   stack388__7761->GetXaxis()->SetTitleOffset(0.8);
   stack388__7761->GetXaxis()->SetTitleFont(42);
   stack388__7761->GetYaxis()->SetLabelFont(42);
   stack388__7761->GetYaxis()->SetLabelOffset(0.007);
   stack388__7761->GetYaxis()->SetTitleSize(0.05);
   stack388__7761->GetYaxis()->SetTitleFont(42);
   stack388__7761->GetZaxis()->SetLabelFont(42);
   stack388__7761->GetZaxis()->SetLabelOffset(0.007);
   stack388__7761->GetZaxis()->SetTitleSize(0.05);
   stack388__7761->GetZaxis()->SetTitleFont(42);
   stack->SetHistogram(stack388__7761);
   
   
   TH1 *h13 = new TH1F("h13","ZZ",3,0,3);
   h13->SetBinContent(1,213.1301);
   h13->SetBinContent(2,8.345931);
   h13->SetBinContent(3,8.296422);
   h13->SetBinError(1,0.9308755);
   h13->SetBinError(2,0.1841613);
   h13->SetBinError(3,0.1848086);
   h13->SetEntries(164798);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#ccccff");
   h13->SetFillColor(ci);

   ci = TColor::GetColor("#ccccff");
   h13->SetMarkerColor(ci);
   h13->GetXaxis()->SetTitle("Leptons");
   h13->GetXaxis()->SetBinLabel(1,"=2 leptons");
   h13->GetXaxis()->SetBinLabel(2,"=3 leptons");
   h13->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   h13->GetXaxis()->SetLabelFont(42);
   h13->GetYaxis()->SetTitle("Events");
   h13->GetYaxis()->SetLabelFont(42);
   h13->GetZaxis()->SetLabelFont(42);
   stack->Add(h13,"");
   
   TH1 *h12 = new TH1F("h12","WW",3,0,3);
   h12->SetBinContent(1,77.30776);
   h12->SetBinContent(2,0.08431666);
   h12->SetBinError(1,1.485465);
   h12->SetBinError(2,0.04766246);
   h12->SetEntries(8004);

   ci = TColor::GetColor("#9999cc");
   h12->SetFillColor(ci);

   ci = TColor::GetColor("#9999cc");
   h12->SetMarkerColor(ci);
   h12->GetXaxis()->SetTitle("Leptons");
   h12->GetXaxis()->SetBinLabel(1,"=2 leptons");
   h12->GetXaxis()->SetBinLabel(2,"=3 leptons");
   h12->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   h12->GetXaxis()->SetLabelFont(42);
   h12->GetYaxis()->SetTitle("Events");
   h12->GetYaxis()->SetLabelFont(42);
   h12->GetZaxis()->SetLabelFont(42);
   stack->Add(h12,"");
   
   TH1 *h11 = new TH1F("h11","WZ",3,0,3);
   h11->SetBinContent(1,304.9013);
   h11->SetBinContent(2,55.44631);
   h11->SetBinContent(3,0.05451922);
   h11->SetBinError(1,1.919972);
   h11->SetBinError(2,0.8205388);
   h11->SetBinError(3,0.02486003);
   h11->SetEntries(86848);

   ci = TColor::GetColor("#3333ff");
   h11->SetFillColor(ci);

   ci = TColor::GetColor("#3333ff");
   h11->SetMarkerColor(ci);
   h11->GetXaxis()->SetTitle("Leptons");
   h11->GetXaxis()->SetBinLabel(1,"=2 leptons");
   h11->GetXaxis()->SetBinLabel(2,"=3 leptons");
   h11->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   h11->GetXaxis()->SetLabelFont(42);
   h11->GetYaxis()->SetTitle("Events");
   h11->GetYaxis()->SetLabelFont(42);
   h11->GetZaxis()->SetLabelFont(42);
   stack->Add(h11,"");
   
   TH1 *h10 = new TH1F("h10","Single top",3,0,3);
   h10->SetBinContent(1,34.56814);
   h10->SetBinContent(2,0.683694);
   h10->SetBinError(1,1.04681);
   h10->SetBinError(2,0.1492493);
   h10->SetEntries(2383);

   ci = TColor::GetColor("#669900");
   h10->SetFillColor(ci);

   ci = TColor::GetColor("#669900");
   h10->SetMarkerColor(ci);
   h10->GetXaxis()->SetTitle("Leptons");
   h10->GetXaxis()->SetBinLabel(1,"=2 leptons");
   h10->GetXaxis()->SetBinLabel(2,"=3 leptons");
   h10->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   h10->GetXaxis()->SetLabelFont(42);
   h10->GetYaxis()->SetTitle("Events");
   h10->GetYaxis()->SetLabelFont(42);
   h10->GetZaxis()->SetLabelFont(42);
   stack->Add(h10,"");
   
   TH1 *h9 = new TH1F("h9","t#bar{t}",3,0,3);
   h9->SetBinContent(1,364.7296);
   h9->SetBinContent(2,7.739503);
   h9->SetBinError(1,7.287806);
   h9->SetBinError(2,1.064869);
   h9->SetEntries(7628);
   h9->SetFillColor(8);
   h9->SetMarkerColor(8);
   h9->SetMarkerStyle(20);
   h9->GetXaxis()->SetTitle("Leptons");
   h9->GetXaxis()->SetBinLabel(1,"=2 leptons");
   h9->GetXaxis()->SetBinLabel(2,"=3 leptons");
   h9->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   h9->GetXaxis()->SetLabelFont(42);
   h9->GetYaxis()->SetTitle("Events");
   h9->GetYaxis()->SetLabelFont(42);
   h9->GetZaxis()->SetLabelFont(42);
   stack->Add(h9,"");
   
   TH1 *h8 = new TH1F("h8","W+jets",3,0,3);
   h8->SetBinContent(1,38.86523);
   h8->SetBinError(1,7.365324);
   h8->SetEntries(92);

   ci = TColor::GetColor("#cc3300");
   h8->SetFillColor(ci);

   ci = TColor::GetColor("#cc3300");
   h8->SetMarkerColor(ci);
   h8->SetMarkerStyle(20);
   h8->GetXaxis()->SetTitle("Leptons");
   h8->GetXaxis()->SetBinLabel(1,"=2 leptons");
   h8->GetXaxis()->SetBinLabel(2,"=3 leptons");
   h8->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   h8->GetXaxis()->SetLabelFont(42);
   h8->GetYaxis()->SetTitle("Events");
   h8->GetYaxis()->SetLabelFont(42);
   h8->GetZaxis()->SetLabelFont(42);
   stack->Add(h8,"");
   
   TH1 *h7 = new TH1F("h7","Z/#gamma^{*}+jets#rightarrow ll",3,0,3);
   h7->SetBinContent(1,467505.8);
   h7->SetBinContent(2,522.4982);
   h7->SetBinContent(3,0.9161746);
   h7->SetBinError(1,369.105);
   h7->SetBinError(2,12.31159);
   h7->SetBinError(3,0.3858936);
   h7->SetEntries(4669630);

   ci = TColor::GetColor("#99ffcc");
   h7->SetFillColor(ci);

   ci = TColor::GetColor("#99ffcc");
   h7->SetMarkerColor(ci);
   h7->SetMarkerStyle(20);
   h7->GetXaxis()->SetTitle("Leptons");
   h7->GetXaxis()->SetBinLabel(1,"=2 leptons");
   h7->GetXaxis()->SetBinLabel(2,"=3 leptons");
   h7->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   h7->GetXaxis()->SetLabelFont(42);
   h7->GetYaxis()->SetTitle("Events");
   h7->GetYaxis()->SetLabelFont(42);
   h7->GetZaxis()->SetLabelFont(42);
   stack->Add(h7,"");
   stack->Draw("histsame");
   
   TH1 *recolevelc_1_frame__775 = new TH1F("recolevelc_1_frame__775","recolevelc_1_stackcontour",3,0,3);
   recolevelc_1_frame__775->SetBinContent(1,468539.3);
   recolevelc_1_frame__775->SetBinContent(2,594.7979);
   recolevelc_1_frame__775->SetBinContent(3,9.267117);
   recolevelc_1_frame__775->SetBinError(1,369.261);
   recolevelc_1_frame__775->SetBinError(2,12.38713);
   recolevelc_1_frame__775->SetBinError(3,0.4285862);
   recolevelc_1_frame__775->SetMinimum(0.001);
   recolevelc_1_frame__775->SetMaximum(1000000);
   recolevelc_1_frame__775->SetEntries(5104181);
   recolevelc_1_frame__775->SetDirectory(0);
   recolevelc_1_frame__775->SetFillColor(1);
   recolevelc_1_frame__775->SetFillStyle(0);
   recolevelc_1_frame__775->SetMarkerStyle(0);
   recolevelc_1_frame__775->GetXaxis()->SetTitle("Leptons");
   recolevelc_1_frame__775->GetXaxis()->SetBinLabel(1,"=2 leptons");
   recolevelc_1_frame__775->GetXaxis()->SetBinLabel(2,"=3 leptons");
   recolevelc_1_frame__775->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   recolevelc_1_frame__775->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__775->GetYaxis()->SetTitle("Events");
   recolevelc_1_frame__775->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__775->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__775->Draw("sameaxis");
   
   TH1 *gr6 = new TH1F("gr6","H(600)#rightarrow VV",3,0,3);
   gr6->SetBinContent(1,1.461672);
   gr6->SetBinContent(2,0.002452441);
   gr6->SetBinError(1,0.01551832);
   gr6->SetBinError(2,0.0004953832);
   gr6->SetEntries(42659);

   ci = TColor::GetColor("#ff6600");
   gr6->SetFillColor(ci);
   gr6->SetFillStyle(0);

   ci = TColor::GetColor("#ff6600");
   gr6->SetLineColor(ci);
   gr6->SetLineWidth(2);

   ci = TColor::GetColor("#ff6600");
   gr6->SetMarkerColor(ci);
   gr6->GetXaxis()->SetTitle("Leptons");
   gr6->GetXaxis()->SetBinLabel(1,"=2 leptons");
   gr6->GetXaxis()->SetBinLabel(2,"=3 leptons");
   gr6->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   gr6->GetXaxis()->SetLabelFont(42);
   gr6->GetYaxis()->SetTitle("Events");
   gr6->GetYaxis()->SetLabelFont(42);
   gr6->GetZaxis()->SetLabelFont(42);
   gr6->Draw("histsame");
   
   TH1 *gr5 = new TH1F("gr5","H(500)#rightarrow VV",3,0,3);
   gr5->SetBinContent(1,3.534648);
   gr5->SetBinContent(2,0.01254913);
   gr5->SetBinError(1,0.1123018);
   gr5->SetBinError(2,0.0119515);
   gr5->SetEntries(10636);

   ci = TColor::GetColor("#ff9900");
   gr5->SetFillColor(ci);
   gr5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9900");
   gr5->SetLineColor(ci);
   gr5->SetLineWidth(2);

   ci = TColor::GetColor("#ff9900");
   gr5->SetMarkerColor(ci);
   gr5->GetXaxis()->SetTitle("Leptons");
   gr5->GetXaxis()->SetBinLabel(1,"=2 leptons");
   gr5->GetXaxis()->SetBinLabel(2,"=3 leptons");
   gr5->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   gr5->GetXaxis()->SetLabelFont(42);
   gr5->GetYaxis()->SetTitle("Events");
   gr5->GetYaxis()->SetLabelFont(42);
   gr5->GetZaxis()->SetLabelFont(42);
   gr5->Draw("histsame");
   
   TH1 *gr4 = new TH1F("gr4","H(400)#rightarrow VV",3,0,3);
   gr4->SetBinContent(1,9.024673);
   gr4->SetBinContent(2,0.01909734);
   gr4->SetBinError(1,0.08294766);
   gr4->SetBinError(2,0.003594955);
   gr4->SetEntries(40319);

   ci = TColor::GetColor("#cc6633");
   gr4->SetFillColor(ci);
   gr4->SetFillStyle(0);

   ci = TColor::GetColor("#cc6633");
   gr4->SetLineColor(ci);
   gr4->SetLineWidth(2);

   ci = TColor::GetColor("#cc6633");
   gr4->SetMarkerColor(ci);
   gr4->GetXaxis()->SetTitle("Leptons");
   gr4->GetXaxis()->SetBinLabel(1,"=2 leptons");
   gr4->GetXaxis()->SetBinLabel(2,"=3 leptons");
   gr4->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   gr4->GetXaxis()->SetLabelFont(42);
   gr4->GetYaxis()->SetTitle("Events");
   gr4->GetYaxis()->SetLabelFont(42);
   gr4->GetZaxis()->SetLabelFont(42);
   gr4->Draw("histsame");
   
   TH1 *gr3 = new TH1F("gr3","H(300)#rightarrow VV",3,0,3);
   gr3->SetBinContent(1,14.01329);
   gr3->SetBinContent(2,0.02352446);
   gr3->SetBinError(1,0.1332469);
   gr3->SetBinError(2,0.005039822);
   gr3->SetEntries(38989);

   ci = TColor::GetColor("#663300");
   gr3->SetFillColor(ci);
   gr3->SetFillStyle(0);

   ci = TColor::GetColor("#663300");
   gr3->SetLineColor(ci);
   gr3->SetLineWidth(2);

   ci = TColor::GetColor("#663300");
   gr3->SetMarkerColor(ci);
   gr3->GetXaxis()->SetTitle("Leptons");
   gr3->GetXaxis()->SetBinLabel(1,"=2 leptons");
   gr3->GetXaxis()->SetBinLabel(2,"=3 leptons");
   gr3->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   gr3->GetXaxis()->SetLabelFont(42);
   gr3->GetYaxis()->SetTitle("Events");
   gr3->GetYaxis()->SetLabelFont(42);
   gr3->GetZaxis()->SetLabelFont(42);
   gr3->Draw("histsame");
   
   TH1 *gr2 = new TH1F("gr2","H(200)#rightarrow VV",3,0,3);
   gr2->SetBinContent(1,29.57684);
   gr2->SetBinContent(2,0.02276825);
   gr2->SetBinError(1,0.7794395);
   gr2->SetBinError(2,0.00507786);
   gr2->SetEntries(35033);

   ci = TColor::GetColor("#ffcc00");
   gr2->SetFillColor(ci);
   gr2->SetFillStyle(0);

   ci = TColor::GetColor("#ffcc00");
   gr2->SetLineColor(ci);
   gr2->SetLineWidth(2);

   ci = TColor::GetColor("#ffcc00");
   gr2->SetMarkerColor(ci);
   gr2->GetXaxis()->SetTitle("Leptons");
   gr2->GetXaxis()->SetBinLabel(1,"=2 leptons");
   gr2->GetXaxis()->SetBinLabel(2,"=3 leptons");
   gr2->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   gr2->GetXaxis()->SetLabelFont(42);
   gr2->GetYaxis()->SetTitle("Events");
   gr2->GetYaxis()->SetLabelFont(42);
   gr2->GetZaxis()->SetLabelFont(42);
   gr2->Draw("histsame");
   
   TH1 *data1 = new TH1F("data1","data",3,0,3);
   data1->SetBinContent(1,473019);
   data1->SetBinContent(2,752);
   data1->SetBinContent(3,12);
   data1->SetBinError(1,687.7638);
   data1->SetBinError(2,27.42262);
   data1->SetBinError(3,3.464102);
   data1->SetEntries(473851);
   data1->SetFillColor(1);
   data1->SetFillStyle(0);
   data1->SetMarkerStyle(20);
   data1->SetMarkerSize(1.5);
   data1->GetXaxis()->SetTitle("Leptons");
   data1->GetXaxis()->SetBinLabel(1,"=2 leptons");
   data1->GetXaxis()->SetBinLabel(2,"=3 leptons");
   data1->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   data1->GetXaxis()->SetLabelFont(42);
   data1->GetYaxis()->SetTitle("Events");
   data1->GetYaxis()->SetLabelFont(42);
   data1->GetZaxis()->SetLabelFont(42);
   data1->Draw("ESAME");
   
   TPaveText *pt = new TPaveText(0.5,0.96,0.94,0.99,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(19);
   pt->SetFillStyle(0);
   pt->SetTextAlign(32);
   pt->SetTextFont(42);
   TText *text = pt->AddText("CMS preliminary,#sqrt{s}=7 TeV, #int L=2007.0 pb^{-1}");
   pt->Draw();
   recolevelc_1->Modified();
   recolevelc->cd();
  
// ------------>Primitives in pad: recolevelc_2
   recolevelc_2 = new TPad("recolevelc_2", "recolevelc_2",0,0,1,0.28);
   recolevelc_2->Draw();
   recolevelc_2->cd();
   recolevelc_2->Range(-0.6486486,-2.2,3.405405,2.2);
   recolevelc_2->SetFillColor(0);
   recolevelc_2->SetBorderMode(0);
   recolevelc_2->SetBorderSize(2);
   recolevelc_2->SetGridx();
   recolevelc_2->SetGridy();
   recolevelc_2->SetTickx(1);
   recolevelc_2->SetTicky(1);
   recolevelc_2->SetLeftMargin(0.16);
   recolevelc_2->SetTopMargin(0);
   recolevelc_2->SetBottomMargin(0.5);
   recolevelc_2->SetFrameFillStyle(0);
   recolevelc_2->SetFrameBorderMode(0);
   recolevelc_2->SetFrameFillStyle(0);
   recolevelc_2->SetFrameBorderMode(0);
   
   TH1 *ee_nleptons_tomc = new TH1F("ee_nleptons_tomc","data",3,0,3);
   ee_nleptons_tomc->SetBinContent(1,1.009561);
   ee_nleptons_tomc->SetBinContent(2,1.264295);
   ee_nleptons_tomc->SetBinContent(3,1.294901);
   ee_nleptons_tomc->SetBinError(1,0.001669656);
   ee_nleptons_tomc->SetBinError(2,0.05309286);
   ee_nleptons_tomc->SetBinError(3,0.3785725);
   ee_nleptons_tomc->SetMinimum(0);
   ee_nleptons_tomc->SetMaximum(2.2);
   ee_nleptons_tomc->SetEntries(87.15021);
   ee_nleptons_tomc->SetFillColor(1);
   ee_nleptons_tomc->SetFillStyle(0);
   ee_nleptons_tomc->SetMarkerStyle(20);
   ee_nleptons_tomc->SetMarkerSize(1.2);
   ee_nleptons_tomc->GetXaxis()->SetTitle("Leptons");
   ee_nleptons_tomc->GetXaxis()->SetBinLabel(1,"=2 leptons");
   ee_nleptons_tomc->GetXaxis()->SetBinLabel(2,"=3 leptons");
   ee_nleptons_tomc->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   ee_nleptons_tomc->GetXaxis()->SetLabelFont(42);
   ee_nleptons_tomc->GetXaxis()->SetLabelSize(0.1);
   ee_nleptons_tomc->GetXaxis()->SetTitleSize(0.125);
   ee_nleptons_tomc->GetXaxis()->SetTickLength(0.075);
   ee_nleptons_tomc->GetXaxis()->SetTitleOffset(0.85);
   ee_nleptons_tomc->GetYaxis()->SetTitle("Obs/Ref");
   ee_nleptons_tomc->GetYaxis()->SetNdivisions(509);
   ee_nleptons_tomc->GetYaxis()->SetLabelFont(42);
   ee_nleptons_tomc->GetYaxis()->SetLabelSize(0.1);
   ee_nleptons_tomc->GetYaxis()->SetTitleSize(0.1);
   ee_nleptons_tomc->GetYaxis()->SetTitleOffset(0.5);
   ee_nleptons_tomc->GetZaxis()->SetLabelFont(42);
   ee_nleptons_tomc->Draw("E");
   recolevelc_2->Modified();
   recolevelc->cd();
   recolevelc->Modified();
   recolevelc->cd();
   recolevelc->SetSelected(recolevelc);
}
