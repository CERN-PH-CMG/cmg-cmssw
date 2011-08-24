{
//=========Macro generated from canvas: recolevelc/recolevelc
//=========  (Wed Aug 24 17:11:33 2011) by ROOT version5.27/06b
   TCanvas *recolevelc = new TCanvas("recolevelc", "recolevelc",1,23,804,804);
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
   recolevelc_1->Range(-0.8648648,-2.109756,4.540541,6.426829);
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
   
   TH1F *recolevelc_1_frame__255__7 = new TH1F("recolevelc_1_frame__255__7","recolevelc_1_stackcontour",4,0,4);
   recolevelc_1_frame__255__7->SetBinContent(1,323650.4);
   recolevelc_1_frame__255__7->SetBinContent(2,137065.2);
   recolevelc_1_frame__255__7->SetBinContent(3,48582.89);
   recolevelc_1_frame__255__7->SetBinContent(4,21552.95);
   recolevelc_1_frame__255__7->SetBinError(1,291.8399);
   recolevelc_1_frame__255__7->SetBinError(2,188.6536);
   recolevelc_1_frame__255__7->SetBinError(3,111.5498);
   recolevelc_1_frame__255__7->SetBinError(4,73.50692);
   recolevelc_1_frame__255__7->SetMinimum(0.1);
   recolevelc_1_frame__255__7->SetMaximum(1000000);
   recolevelc_1_frame__255__7->SetEntries(6330828);
   recolevelc_1_frame__255__7->SetDirectory(0);
   recolevelc_1_frame__255__7->SetFillColor(1);
   recolevelc_1_frame__255__7->SetFillStyle(0);
   recolevelc_1_frame__255__7->SetMarkerStyle(0);
   recolevelc_1_frame__255__7->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   recolevelc_1_frame__255__7->GetXaxis()->SetBinLabel(1,"0");
   recolevelc_1_frame__255__7->GetXaxis()->SetBinLabel(2,"1");
   recolevelc_1_frame__255__7->GetXaxis()->SetBinLabel(3,"2");
   recolevelc_1_frame__255__7->GetXaxis()->SetBinLabel(4,"#geq 3");
   recolevelc_1_frame__255__7->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__255__7->GetYaxis()->SetTitle("Events");
   recolevelc_1_frame__255__7->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__255__7->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__255__7->Draw("hist");
   
   THStack *stack = new THStack();
   stack->SetName("stack");
   stack->SetTitle("recolevelc_1");
   
   TH1F *stack128__2564__8 = new TH1F("stack128__2564__8","recolevelc_1",4,0,4);
   stack128__2564__8->SetMinimum(129.5281);
   stack128__2564__8->SetMaximum(518112.4);
   stack128__2564__8->SetDirectory(0);
   stack128__2564__8->SetStats(0);
   stack128__2564__8->SetLineStyle(0);
   stack128__2564__8->SetMarkerStyle(20);
   stack128__2564__8->GetXaxis()->SetBinLabel(1,"0");
   stack128__2564__8->GetXaxis()->SetBinLabel(2,"1");
   stack128__2564__8->GetXaxis()->SetBinLabel(3,"2");
   stack128__2564__8->GetXaxis()->SetBinLabel(4,"#geq 3");
   stack128__2564__8->GetXaxis()->SetLabelFont(42);
   stack128__2564__8->GetXaxis()->SetLabelOffset(0.007);
   stack128__2564__8->GetXaxis()->SetTitleSize(0.05);
   stack128__2564__8->GetXaxis()->SetTitleOffset(0.8);
   stack128__2564__8->GetXaxis()->SetTitleFont(42);
   stack128__2564__8->GetYaxis()->SetLabelFont(42);
   stack128__2564__8->GetYaxis()->SetLabelOffset(0.007);
   stack128__2564__8->GetYaxis()->SetTitleSize(0.05);
   stack128__2564__8->GetYaxis()->SetTitleFont(42);
   stack128__2564__8->GetZaxis()->SetLabelFont(42);
   stack128__2564__8->GetZaxis()->SetLabelOffset(0.007);
   stack128__2564__8->GetZaxis()->SetTitleSize(0.05);
   stack128__2564__8->GetZaxis()->SetTitleFont(42);
   stack->SetHistogram(stack128__2564);
   
   
   TH1F *h13 = new TH1F("h13","ZZ",4,0,4);
   h13->SetBinContent(1,35.48905);
   h13->SetBinContent(2,48.82242);
   h13->SetBinContent(3,79.55842);
   h13->SetBinContent(4,70.58963);
   h13->SetBinError(1,0.3805232);
   h13->SetBinError(2,0.4459542);
   h13->SetBinError(3,0.56846);
   h13->SetBinError(4,0.5335557);
   h13->SetEntries(168350);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#ccccff");
   h13->SetFillColor(ci);

   ci = TColor::GetColor("#ccccff");
   h13->SetMarkerColor(ci);
   h13->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   h13->GetXaxis()->SetBinLabel(1,"0");
   h13->GetXaxis()->SetBinLabel(2,"1");
   h13->GetXaxis()->SetBinLabel(3,"2");
   h13->GetXaxis()->SetBinLabel(4,"#geq 3");
   h13->GetXaxis()->SetLabelFont(42);
   h13->GetYaxis()->SetTitle("Events");
   h13->GetYaxis()->SetLabelFont(42);
   h13->GetZaxis()->SetLabelFont(42);
   stack->Add(h13,"");
   
   TH1F *h12 = new TH1F("h12","WW",4,0,4);
   h12->SetBinContent(1,46.18895);
   h12->SetBinContent(2,27.66687);
   h12->SetBinContent(3,9.838639);
   h12->SetBinContent(4,4.030839);
   h12->SetBinError(1,1.153426);
   h12->SetBinError(2,0.883148);
   h12->SetBinError(3,0.5297974);
   h12->SetBinError(4,0.3316533);
   h12->SetEntries(8960);

   ci = TColor::GetColor("#9999cc");
   h12->SetFillColor(ci);

   ci = TColor::GetColor("#9999cc");
   h12->SetMarkerColor(ci);
   h12->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   h12->GetXaxis()->SetBinLabel(1,"0");
   h12->GetXaxis()->SetBinLabel(2,"1");
   h12->GetXaxis()->SetBinLabel(3,"2");
   h12->GetXaxis()->SetBinLabel(4,"#geq 3");
   h12->GetXaxis()->SetLabelFont(42);
   h12->GetYaxis()->SetTitle("Events");
   h12->GetYaxis()->SetLabelFont(42);
   h12->GetZaxis()->SetLabelFont(42);
   stack->Add(h12,"");
   
   TH1F *h11 = new TH1F("h11","WZ",4,0,4);
   h11->SetBinContent(1,38.01952);
   h11->SetBinContent(2,80.77493);
   h11->SetBinContent(3,113.9279);
   h11->SetBinContent(4,97.77344);
   h11->SetBinError(1,0.6793801);
   h11->SetBinError(2,0.9892205);
   h11->SetBinError(3,1.171627);
   h11->SetBinError(4,1.081187);
   h11->SetEntries(80014);

   ci = TColor::GetColor("#3333ff");
   h11->SetFillColor(ci);

   ci = TColor::GetColor("#3333ff");
   h11->SetMarkerColor(ci);
   h11->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   h11->GetXaxis()->SetBinLabel(1,"0");
   h11->GetXaxis()->SetBinLabel(2,"1");
   h11->GetXaxis()->SetBinLabel(3,"2");
   h11->GetXaxis()->SetBinLabel(4,"#geq 3");
   h11->GetXaxis()->SetLabelFont(42);
   h11->GetYaxis()->SetTitle("Events");
   h11->GetYaxis()->SetLabelFont(42);
   h11->GetZaxis()->SetLabelFont(42);
   stack->Add(h11,"");
   
   TH1F *h10 = new TH1F("h10","Single top",4,0,4);
   h10->SetBinContent(1,1.07516);
   h10->SetBinContent(2,11.43559);
   h10->SetBinContent(3,11.55369);
   h10->SetBinContent(4,10.31751);
   h10->SetBinError(1,0.1783429);
   h10->SetBinError(2,0.6068685);
   h10->SetBinError(3,0.6071965);
   h10->SetBinError(4,0.5645191);
   h10->SetEntries(2336);

   ci = TColor::GetColor("#669900");
   h10->SetFillColor(ci);

   ci = TColor::GetColor("#669900");
   h10->SetMarkerColor(ci);
   h10->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   h10->GetXaxis()->SetBinLabel(1,"0");
   h10->GetXaxis()->SetBinLabel(2,"1");
   h10->GetXaxis()->SetBinLabel(3,"2");
   h10->GetXaxis()->SetBinLabel(4,"#geq 3");
   h10->GetXaxis()->SetLabelFont(42);
   h10->GetYaxis()->SetTitle("Events");
   h10->GetYaxis()->SetLabelFont(42);
   h10->GetZaxis()->SetLabelFont(42);
   stack->Add(h10,"");
   
   TH1F *h9 = new TH1F("h9","t#bar{t}",4,0,4);
   h9->SetBinContent(1,1.214149);
   h9->SetBinContent(2,24.40037);
   h9->SetBinContent(3,118.6167);
   h9->SetBinContent(4,244.8579);
   h9->SetBinError(1,0.4190346);
   h9->SetBinError(2,1.88462);
   h9->SetBinError(3,4.20275);
   h9->SetBinError(4,6.012898);
   h9->SetEntries(7612);
   h9->SetFillColor(8);
   h9->SetMarkerColor(8);
   h9->SetMarkerStyle(20);
   h9->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   h9->GetXaxis()->SetBinLabel(1,"0");
   h9->GetXaxis()->SetBinLabel(2,"1");
   h9->GetXaxis()->SetBinLabel(3,"2");
   h9->GetXaxis()->SetBinLabel(4,"#geq 3");
   h9->GetXaxis()->SetLabelFont(42);
   h9->GetYaxis()->SetTitle("Events");
   h9->GetYaxis()->SetLabelFont(42);
   h9->GetZaxis()->SetLabelFont(42);
   stack->Add(h9,"");
   
   TH1F *h8 = new TH1F("h8","W+jets",4,0,4);
   h8->SetBinContent(2,0.3003715);
   h8->SetBinError(2,0.3003715);
   h8->SetEntries(10);

   ci = TColor::GetColor("#cc3300");
   h8->SetFillColor(ci);

   ci = TColor::GetColor("#cc3300");
   h8->SetMarkerColor(ci);
   h8->SetMarkerStyle(20);
   h8->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   h8->GetXaxis()->SetBinLabel(1,"0");
   h8->GetXaxis()->SetBinLabel(2,"1");
   h8->GetXaxis()->SetBinLabel(3,"2");
   h8->GetXaxis()->SetBinLabel(4,"#geq 3");
   h8->GetXaxis()->SetLabelFont(42);
   h8->GetYaxis()->SetTitle("Events");
   h8->GetYaxis()->SetLabelFont(42);
   h8->GetZaxis()->SetLabelFont(42);
   stack->Add(h8,"");
   
   TH1F *h7 = new TH1F("h7","Z/#gamma^{*}+jets#rightarrow ll",4,0,4);
   h7->SetBinContent(1,323528.4);
   h7->SetBinContent(2,136871.8);
   h7->SetBinContent(3,48249.39);
   h7->SetBinContent(4,21125.38);
   h7->SetBinError(1,291.8362);
   h7->SetBinError(2,188.6378);
   h7->SetBinError(3,111.4601);
   h7->SetBinError(4,73.24773);
   h7->SetEntries(5895196);

   ci = TColor::GetColor("#99ffcc");
   h7->SetFillColor(ci);

   ci = TColor::GetColor("#99ffcc");
   h7->SetMarkerColor(ci);
   h7->SetMarkerStyle(20);
   h7->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   h7->GetXaxis()->SetBinLabel(1,"0");
   h7->GetXaxis()->SetBinLabel(2,"1");
   h7->GetXaxis()->SetBinLabel(3,"2");
   h7->GetXaxis()->SetBinLabel(4,"#geq 3");
   h7->GetXaxis()->SetLabelFont(42);
   h7->GetYaxis()->SetTitle("Events");
   h7->GetYaxis()->SetLabelFont(42);
   h7->GetZaxis()->SetLabelFont(42);
   stack->Add(h7,"");
   stack->Draw("histsame");
   
   TH1F *gr6 = new TH1F("gr6","H(600)#rightarrow VV",4,0,4);
   gr6->SetBinContent(1,0.2821243);
   gr6->SetBinContent(2,0.4660567);
   gr6->SetBinContent(3,0.3354748);
   gr6->SetBinContent(4,0.296426);
   gr6->SetBinError(1,0.009048404);
   gr6->SetBinError(2,0.01141128);
   gr6->SetBinError(3,0.007955937);
   gr6->SetBinError(4,0.007490187);
   gr6->SetEntries(39332);

   ci = TColor::GetColor("#ff6600");
   gr6->SetFillColor(ci);
   gr6->SetFillStyle(0);

   ci = TColor::GetColor("#ff6600");
   gr6->SetLineColor(ci);
   gr6->SetLineWidth(2);

   ci = TColor::GetColor("#ff6600");
   gr6->SetMarkerColor(ci);
   gr6->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   gr6->GetXaxis()->SetBinLabel(1,"0");
   gr6->GetXaxis()->SetBinLabel(2,"1");
   gr6->GetXaxis()->SetBinLabel(3,"2");
   gr6->GetXaxis()->SetBinLabel(4,"#geq 3");
   gr6->GetXaxis()->SetLabelFont(42);
   gr6->GetYaxis()->SetTitle("Events");
   gr6->GetYaxis()->SetLabelFont(42);
   gr6->GetZaxis()->SetLabelFont(42);
   gr6->Draw("histsame");
   
   TH1F *gr5 = new TH1F("gr5","H(500)#rightarrow VV",4,0,4);
   gr5->SetBinContent(1,0.5740826);
   gr5->SetBinContent(2,1.031559);
   gr5->SetBinContent(3,0.8307697);
   gr5->SetBinContent(4,0.6988874);
   gr5->SetBinError(1,0.04877529);
   gr5->SetBinError(2,0.05701528);
   gr5->SetBinError(3,0.05736671);
   gr5->SetBinError(4,0.06131928);
   gr5->SetEntries(10041);

   ci = TColor::GetColor("#ff9900");
   gr5->SetFillColor(ci);
   gr5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9900");
   gr5->SetLineColor(ci);
   gr5->SetLineWidth(2);

   ci = TColor::GetColor("#ff9900");
   gr5->SetMarkerColor(ci);
   gr5->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   gr5->GetXaxis()->SetBinLabel(1,"0");
   gr5->GetXaxis()->SetBinLabel(2,"1");
   gr5->GetXaxis()->SetBinLabel(3,"2");
   gr5->GetXaxis()->SetBinLabel(4,"#geq 3");
   gr5->GetXaxis()->SetLabelFont(42);
   gr5->GetYaxis()->SetTitle("Events");
   gr5->GetYaxis()->SetLabelFont(42);
   gr5->GetZaxis()->SetLabelFont(42);
   gr5->Draw("histsame");
   
   TH1F *gr4 = new TH1F("gr4","H(400)#rightarrow VV",4,0,4);
   gr4->SetBinContent(1,1.991957);
   gr4->SetBinContent(2,2.874293);
   gr4->SetBinContent(3,2.001115);
   gr4->SetBinContent(4,1.672271);
   gr4->SetBinError(1,0.03842191);
   gr4->SetBinError(2,0.04546358);
   gr4->SetBinError(3,0.03826569);
   gr4->SetBinError(4,0.03580208);
   gr4->SetEntries(39238);

   ci = TColor::GetColor("#cc6633");
   gr4->SetFillColor(ci);
   gr4->SetFillStyle(0);

   ci = TColor::GetColor("#cc6633");
   gr4->SetLineColor(ci);
   gr4->SetLineWidth(2);

   ci = TColor::GetColor("#cc6633");
   gr4->SetMarkerColor(ci);
   gr4->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   gr4->GetXaxis()->SetBinLabel(1,"0");
   gr4->GetXaxis()->SetBinLabel(2,"1");
   gr4->GetXaxis()->SetBinLabel(3,"2");
   gr4->GetXaxis()->SetBinLabel(4,"#geq 3");
   gr4->GetXaxis()->SetLabelFont(42);
   gr4->GetYaxis()->SetTitle("Events");
   gr4->GetYaxis()->SetLabelFont(42);
   gr4->GetZaxis()->SetLabelFont(42);
   gr4->Draw("histsame");
   
   TH1F *gr3 = new TH1F("gr3","H(300)#rightarrow VV",4,0,4);
   gr3->SetBinContent(1,3.532589);
   gr3->SetBinContent(2,4.871394);
   gr3->SetBinContent(3,3.074064);
   gr3->SetBinContent(4,2.226928);
   gr3->SetBinError(1,0.06698285);
   gr3->SetBinError(2,0.07673349);
   gr3->SetBinError(3,0.06181114);
   gr3->SetBinError(4,0.05248997);
   gr3->SetEntries(39747);

   ci = TColor::GetColor("#663300");
   gr3->SetFillColor(ci);
   gr3->SetFillStyle(0);

   ci = TColor::GetColor("#663300");
   gr3->SetLineColor(ci);
   gr3->SetLineWidth(2);

   ci = TColor::GetColor("#663300");
   gr3->SetMarkerColor(ci);
   gr3->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   gr3->GetXaxis()->SetBinLabel(1,"0");
   gr3->GetXaxis()->SetBinLabel(2,"1");
   gr3->GetXaxis()->SetBinLabel(3,"2");
   gr3->GetXaxis()->SetBinLabel(4,"#geq 3");
   gr3->GetXaxis()->SetLabelFont(42);
   gr3->GetYaxis()->SetTitle("Events");
   gr3->GetYaxis()->SetLabelFont(42);
   gr3->GetZaxis()->SetLabelFont(42);
   gr3->Draw("histsame");
   
   TH1F *gr2 = new TH1F("gr2","H(200)#rightarrow VV",4,0,4);
   gr2->SetBinContent(1,9.040913);
   gr2->SetBinContent(2,10.51147);
   gr2->SetBinContent(3,5.802763);
   gr2->SetBinContent(4,3.872753);
   gr2->SetBinError(1,0.4386581);
   gr2->SetBinError(2,0.4438287);
   gr2->SetBinError(3,0.3325475);
   gr2->SetBinError(4,0.2764756);
   gr2->SetEntries(37659);

   ci = TColor::GetColor("#ffcc00");
   gr2->SetFillColor(ci);
   gr2->SetFillStyle(0);

   ci = TColor::GetColor("#ffcc00");
   gr2->SetLineColor(ci);
   gr2->SetLineWidth(2);

   ci = TColor::GetColor("#ffcc00");
   gr2->SetMarkerColor(ci);
   gr2->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   gr2->GetXaxis()->SetBinLabel(1,"0");
   gr2->GetXaxis()->SetBinLabel(2,"1");
   gr2->GetXaxis()->SetBinLabel(3,"2");
   gr2->GetXaxis()->SetBinLabel(4,"#geq 3");
   gr2->GetXaxis()->SetLabelFont(42);
   gr2->GetYaxis()->SetTitle("Events");
   gr2->GetYaxis()->SetLabelFont(42);
   gr2->GetZaxis()->SetLabelFont(42);
   gr2->Draw("histsame");
   
   TH1F *data1 = new TH1F("data1","data",4,0,4);
   data1->SetBinContent(1,366709);
   data1->SetBinContent(2,155394);
   data1->SetBinContent(3,55986);
   data1->SetBinContent(4,26331);
   data1->SetBinError(1,605.565);
   data1->SetBinError(2,394.2005);
   data1->SetBinError(3,236.6136);
   data1->SetBinError(4,162.2683);
   data1->SetEntries(660010);
   data1->SetFillColor(1);
   data1->SetFillStyle(0);
   data1->SetMarkerStyle(20);
   data1->SetMarkerSize(1.3);
   data1->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   data1->GetXaxis()->SetBinLabel(1,"0");
   data1->GetXaxis()->SetBinLabel(2,"1");
   data1->GetXaxis()->SetBinLabel(3,"2");
   data1->GetXaxis()->SetBinLabel(4,"#geq 3");
   data1->GetXaxis()->SetLabelFont(42);
   data1->GetYaxis()->SetTitle("Events");
   data1->GetYaxis()->SetLabelFont(42);
   data1->GetZaxis()->SetLabelFont(42);
   data1->Draw("ESAME");
   
   TLegend *leg = new TLegend(0.17375,0.4992636,0.37375,0.8784978,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("NULL","data","lpf");
   entry->SetFillColor(1);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("NULL","H(200)#rightarrow VV","lpf");

   ci = TColor::GetColor("#ffcc00");
   entry->SetFillColor(ci);

   ci = TColor::GetColor("#ffcc00");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);

   ci = TColor::GetColor("#ffcc00");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("NULL","H(300)#rightarrow VV","lpf");

   ci = TColor::GetColor("#663300");
   entry->SetFillColor(ci);

   ci = TColor::GetColor("#663300");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);

   ci = TColor::GetColor("#663300");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("NULL","H(400)#rightarrow VV","lpf");

   ci = TColor::GetColor("#cc6633");
   entry->SetFillColor(ci);

   ci = TColor::GetColor("#cc6633");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);

   ci = TColor::GetColor("#cc6633");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("NULL","H(500)#rightarrow VV","lpf");

   ci = TColor::GetColor("#ff9900");
   entry->SetFillColor(ci);

   ci = TColor::GetColor("#ff9900");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);

   ci = TColor::GetColor("#ff9900");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("NULL","H(600)#rightarrow VV","lpf");

   ci = TColor::GetColor("#ff6600");
   entry->SetFillColor(ci);

   ci = TColor::GetColor("#ff6600");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);

   ci = TColor::GetColor("#ff6600");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("NULL","Z/#gamma^{*}+jets#rightarrow ll","lpf");

   ci = TColor::GetColor("#99ffcc");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#99ffcc");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("NULL","W+jets","lpf");

   ci = TColor::GetColor("#cc3300");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#cc3300");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("NULL","t#bar{t}","lpf");
   entry->SetFillColor(8);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(8);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("NULL","Single top","lpf");

   ci = TColor::GetColor("#669900");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#669900");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("NULL","WZ","lpf");

   ci = TColor::GetColor("#3333ff");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#3333ff");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("NULL","WW","lpf");

   ci = TColor::GetColor("#9999cc");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#9999cc");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("NULL","ZZ","lpf");

   ci = TColor::GetColor("#ccccff");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#ccccff");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   leg->Draw();
   
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
   recolevelc_2->Range(-0.8648648,-1.1,4.540541,1.7);
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
   
   TH1F *mumu_njets_tomc = new TH1F("mumu_njets_tomc","data",4,0,4);
   mumu_njets_tomc->SetBinContent(1,1.133041);
   mumu_njets_tomc->SetBinContent(2,1.133723);
   mumu_njets_tomc->SetBinContent(3,1.152381);
   mumu_njets_tomc->SetBinContent(4,1.221689);
   mumu_njets_tomc->SetBinError(1,0.002131817);
   mumu_njets_tomc->SetBinError(2,0.003272057);
   mumu_njets_tomc->SetBinError(3,0.005542648);
   mumu_njets_tomc->SetBinError(4,0.008604865);
   mumu_njets_tomc->SetMinimum(0.3);
   mumu_njets_tomc->SetMaximum(1.7);
   mumu_njets_tomc->SetEntries(179454.4);
   mumu_njets_tomc->SetFillColor(1);
   mumu_njets_tomc->SetFillStyle(0);
   mumu_njets_tomc->SetMarkerStyle(20);
   mumu_njets_tomc->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   mumu_njets_tomc->GetXaxis()->SetBinLabel(1,"0");
   mumu_njets_tomc->GetXaxis()->SetBinLabel(2,"1");
   mumu_njets_tomc->GetXaxis()->SetBinLabel(3,"2");
   mumu_njets_tomc->GetXaxis()->SetBinLabel(4,"#geq 3");
   mumu_njets_tomc->GetXaxis()->SetLabelFont(42);
   mumu_njets_tomc->GetXaxis()->SetLabelSize(0.1);
   mumu_njets_tomc->GetXaxis()->SetTitleSize(0.125);
   mumu_njets_tomc->GetXaxis()->SetTickLength(0.075);
   mumu_njets_tomc->GetXaxis()->SetTitleOffset(0.85);
   mumu_njets_tomc->GetYaxis()->SetTitle("Obs/Ref");
   mumu_njets_tomc->GetYaxis()->SetNdivisions(505);
   mumu_njets_tomc->GetYaxis()->SetLabelFont(42);
   mumu_njets_tomc->GetYaxis()->SetLabelSize(0.1);
   mumu_njets_tomc->GetYaxis()->SetTitleSize(0.1);
   mumu_njets_tomc->GetYaxis()->SetTitleOffset(0.5);
   mumu_njets_tomc->GetZaxis()->SetLabelFont(42);
   mumu_njets_tomc->Draw("E");
   recolevelc_2->Modified();
   recolevelc->cd();
   recolevelc->Modified();
   recolevelc->cd();
   recolevelc->SetSelected(recolevelc);
}
