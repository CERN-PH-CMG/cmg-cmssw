{
//=========Macro generated from canvas: recolevelc/recolevelc
//=========  (Sun Sep 11 15:36:57 2011) by ROOT version5.24/00
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
   recolevelc_1->Range(-1.081081,-2.109756,5.675676,6.426829);
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
   
   TH1 *recolevelc_1_frame__389 = new TH1F("recolevelc_1_frame__389","recolevelc_1_stackcontour",5,0,5);
   recolevelc_1_frame__389->SetBinContent(1,323698.6);
   recolevelc_1_frame__389->SetBinContent(2,136997.4);
   recolevelc_1_frame__389->SetBinContent(3,48596.48);
   recolevelc_1_frame__389->SetBinContent(4,15283.13);
   recolevelc_1_frame__389->SetBinContent(5,6293.404);
   recolevelc_1_frame__389->SetBinError(1,307.7871);
   recolevelc_1_frame__389->SetBinError(2,198.9637);
   recolevelc_1_frame__389->SetBinError(3,117.6628);
   recolevelc_1_frame__389->SetBinError(4,65.44722);
   recolevelc_1_frame__389->SetBinError(5,41.56193);
   recolevelc_1_frame__389->SetMinimum(0.1);
   recolevelc_1_frame__389->SetMaximum(1000000);
   recolevelc_1_frame__389->SetEntries(5737184);
   recolevelc_1_frame__389->SetDirectory(0);
   recolevelc_1_frame__389->SetFillColor(1);
   recolevelc_1_frame__389->SetFillStyle(0);
   recolevelc_1_frame__389->SetMarkerStyle(0);
   recolevelc_1_frame__389->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   recolevelc_1_frame__389->GetXaxis()->SetBinLabel(1,"=0");
   recolevelc_1_frame__389->GetXaxis()->SetBinLabel(2,"=1");
   recolevelc_1_frame__389->GetXaxis()->SetBinLabel(3,"=2");
   recolevelc_1_frame__389->GetXaxis()->SetBinLabel(4,"=3");
   recolevelc_1_frame__389->GetXaxis()->SetBinLabel(5,"#geq 4");
   recolevelc_1_frame__389->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__389->GetYaxis()->SetTitle("Events");
   recolevelc_1_frame__389->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__389->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__389->Draw("hist");
   
   THStack *stack = new THStack();
   stack->SetName("stack");
   stack->SetTitle("recolevelc_1");
   
   TH1 *stack195__3901 = new TH1F("stack195__3901","recolevelc_1",5,0,5);
   stack195__3901->SetMinimum(3015.195);
   stack195__3901->SetMaximum(437447);
   stack195__3901->SetDirectory(0);
   stack195__3901->SetStats(0);
   stack195__3901->SetLineStyle(0);
   stack195__3901->SetMarkerStyle(20);
   stack195__3901->GetXaxis()->SetBinLabel(1,"=0");
   stack195__3901->GetXaxis()->SetBinLabel(2,"=1");
   stack195__3901->GetXaxis()->SetBinLabel(3,"=2");
   stack195__3901->GetXaxis()->SetBinLabel(4,"=3");
   stack195__3901->GetXaxis()->SetBinLabel(5,"#geq 4");
   stack195__3901->GetXaxis()->SetLabelFont(42);
   stack195__3901->GetXaxis()->SetLabelOffset(0.007);
   stack195__3901->GetXaxis()->SetTitleSize(0.05);
   stack195__3901->GetXaxis()->SetTitleOffset(0.8);
   stack195__3901->GetXaxis()->SetTitleFont(42);
   stack195__3901->GetYaxis()->SetLabelFont(42);
   stack195__3901->GetYaxis()->SetLabelOffset(0.007);
   stack195__3901->GetYaxis()->SetTitleSize(0.05);
   stack195__3901->GetYaxis()->SetTitleFont(42);
   stack195__3901->GetZaxis()->SetLabelFont(42);
   stack195__3901->GetZaxis()->SetLabelOffset(0.007);
   stack195__3901->GetZaxis()->SetTitleSize(0.05);
   stack195__3901->GetZaxis()->SetTitleFont(42);
   stack->SetHistogram(stack195__3901);
   
   
   TH1 *h13 = new TH1F("h13","ZZ",5,0,5);
   h13->SetBinContent(1,35.46682);
   h13->SetBinContent(2,48.79505);
   h13->SetBinContent(3,79.54195);
   h13->SetBinContent(4,46.64185);
   h13->SetBinContent(5,23.93251);
   h13->SetBinError(1,0.3803938);
   h13->SetBinError(2,0.4458146);
   h13->SetBinError(3,0.5684114);
   h13->SetBinError(4,0.4343696);
   h13->SetBinError(5,0.3097517);
   h13->SetEntries(168298);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#ccccff");
   h13->SetFillColor(ci);

   ci = TColor::GetColor("#ccccff");
   h13->SetMarkerColor(ci);
   h13->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   h13->GetXaxis()->SetBinLabel(1,"=0");
   h13->GetXaxis()->SetBinLabel(2,"=1");
   h13->GetXaxis()->SetBinLabel(3,"=2");
   h13->GetXaxis()->SetBinLabel(4,"=3");
   h13->GetXaxis()->SetBinLabel(5,"#geq 4");
   h13->GetXaxis()->SetLabelFont(42);
   h13->GetYaxis()->SetTitle("Events");
   h13->GetYaxis()->SetLabelFont(42);
   h13->GetZaxis()->SetLabelFont(42);
   stack->Add(h13,"");
   
   TH1 *h12 = new TH1F("h12","WW",5,0,5);
   h12->SetBinContent(1,46.18895);
   h12->SetBinContent(2,27.66687);
   h12->SetBinContent(3,9.838639);
   h12->SetBinContent(4,2.954199);
   h12->SetBinContent(5,1.076641);
   h12->SetBinError(1,1.153426);
   h12->SetBinError(2,0.883148);
   h12->SetBinError(3,0.5297974);
   h12->SetBinError(4,0.2856168);
   h12->SetBinError(5,0.1685734);
   h12->SetEntries(8960);

   ci = TColor::GetColor("#9999cc");
   h12->SetFillColor(ci);

   ci = TColor::GetColor("#9999cc");
   h12->SetMarkerColor(ci);
   h12->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   h12->GetXaxis()->SetBinLabel(1,"=0");
   h12->GetXaxis()->SetBinLabel(2,"=1");
   h12->GetXaxis()->SetBinLabel(3,"=2");
   h12->GetXaxis()->SetBinLabel(4,"=3");
   h12->GetXaxis()->SetBinLabel(5,"#geq 4");
   h12->GetXaxis()->SetLabelFont(42);
   h12->GetYaxis()->SetTitle("Events");
   h12->GetYaxis()->SetLabelFont(42);
   h12->GetZaxis()->SetLabelFont(42);
   stack->Add(h12,"");
   
   TH1 *h11 = new TH1F("h11","WZ",5,0,5);
   h11->SetBinContent(1,37.78192);
   h11->SetBinContent(2,80.62827);
   h11->SetBinContent(3,113.8566);
   h11->SetBinContent(4,64.6155);
   h11->SetBinContent(5,33.15799);
   h11->SetBinError(1,0.6772645);
   h11->SetBinError(2,0.9883283);
   h11->SetBinError(3,1.171344);
   h11->SetBinError(4,0.8812816);
   h11->SetBinError(5,0.6263442);
   h11->SetEntries(79904);

   ci = TColor::GetColor("#3333ff");
   h11->SetFillColor(ci);

   ci = TColor::GetColor("#3333ff");
   h11->SetMarkerColor(ci);
   h11->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   h11->GetXaxis()->SetBinLabel(1,"=0");
   h11->GetXaxis()->SetBinLabel(2,"=1");
   h11->GetXaxis()->SetBinLabel(3,"=2");
   h11->GetXaxis()->SetBinLabel(4,"=3");
   h11->GetXaxis()->SetBinLabel(5,"#geq 4");
   h11->GetXaxis()->SetLabelFont(42);
   h11->GetYaxis()->SetTitle("Events");
   h11->GetYaxis()->SetLabelFont(42);
   h11->GetZaxis()->SetLabelFont(42);
   stack->Add(h11,"");
   
   TH1 *h10 = new TH1F("h10","Single top",5,0,5);
   h10->SetBinContent(1,1.07516);
   h10->SetBinContent(2,11.43559);
   h10->SetBinContent(3,11.55369);
   h10->SetBinContent(4,6.056849);
   h10->SetBinContent(5,4.260662);
   h10->SetBinError(1,0.1783429);
   h10->SetBinError(2,0.6068685);
   h10->SetBinError(3,0.6071965);
   h10->SetBinError(4,0.4323153);
   h10->SetBinError(5,0.3630225);
   h10->SetEntries(2336);

   ci = TColor::GetColor("#669900");
   h10->SetFillColor(ci);

   ci = TColor::GetColor("#669900");
   h10->SetMarkerColor(ci);
   h10->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   h10->GetXaxis()->SetBinLabel(1,"=0");
   h10->GetXaxis()->SetBinLabel(2,"=1");
   h10->GetXaxis()->SetBinLabel(3,"=2");
   h10->GetXaxis()->SetBinLabel(4,"=3");
   h10->GetXaxis()->SetBinLabel(5,"#geq 4");
   h10->GetXaxis()->SetLabelFont(42);
   h10->GetYaxis()->SetTitle("Events");
   h10->GetYaxis()->SetLabelFont(42);
   h10->GetZaxis()->SetLabelFont(42);
   stack->Add(h10,"");
   
   TH1 *h9 = new TH1F("h9","t#bar{t}",5,0,5);
   h9->SetBinContent(1,1.214149);
   h9->SetBinContent(2,24.40037);
   h9->SetBinContent(3,118.6167);
   h9->SetBinContent(4,114.6632);
   h9->SetBinContent(5,130.1212);
   h9->SetBinError(1,0.4190346);
   h9->SetBinError(2,1.88462);
   h9->SetBinError(3,4.20275);
   h9->SetBinError(4,4.115515);
   h9->SetBinError(5,4.383154);
   h9->SetEntries(7610);
   h9->SetFillColor(8);
   h9->SetMarkerColor(8);
   h9->SetMarkerStyle(20);
   h9->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   h9->GetXaxis()->SetBinLabel(1,"=0");
   h9->GetXaxis()->SetBinLabel(2,"=1");
   h9->GetXaxis()->SetBinLabel(3,"=2");
   h9->GetXaxis()->SetBinLabel(4,"=3");
   h9->GetXaxis()->SetBinLabel(5,"#geq 4");
   h9->GetXaxis()->SetLabelFont(42);
   h9->GetYaxis()->SetTitle("Events");
   h9->GetYaxis()->SetLabelFont(42);
   h9->GetZaxis()->SetLabelFont(42);
   stack->Add(h9,"");
   
   TH1 *h8 = new TH1F("h8","W+jets",5,0,5);
   h8->SetBinContent(2,0.3099518);
   h8->SetBinError(2,0.3099518);
   h8->SetEntries(10);

   ci = TColor::GetColor("#cc3300");
   h8->SetFillColor(ci);

   ci = TColor::GetColor("#cc3300");
   h8->SetMarkerColor(ci);
   h8->SetMarkerStyle(20);
   h8->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   h8->GetXaxis()->SetBinLabel(1,"=0");
   h8->GetXaxis()->SetBinLabel(2,"=1");
   h8->GetXaxis()->SetBinLabel(3,"=2");
   h8->GetXaxis()->SetBinLabel(4,"=3");
   h8->GetXaxis()->SetBinLabel(5,"#geq 4");
   h8->GetXaxis()->SetLabelFont(42);
   h8->GetYaxis()->SetTitle("Events");
   h8->GetYaxis()->SetLabelFont(42);
   h8->GetZaxis()->SetLabelFont(42);
   stack->Add(h8,"");
   
   TH1 *h7 = new TH1F("h7","Z/#gamma^{*}+jets#rightarrow ll",5,0,5);
   h7->SetBinContent(1,323576.8);
   h7->SetBinContent(2,136804.1);
   h7->SetBinContent(3,48263.07);
   h7->SetBinContent(4,15048.19);
   h7->SetBinContent(5,6100.855);
   h7->SetBinError(1,307.7837);
   h7->SetBinError(2,198.9487);
   h7->SetBinError(3,117.5777);
   h7->SetBinError(4,65.30825);
   h7->SetBinError(5,41.32232);
   h7->SetEntries(5301768);

   ci = TColor::GetColor("#99ffcc");
   h7->SetFillColor(ci);

   ci = TColor::GetColor("#99ffcc");
   h7->SetMarkerColor(ci);
   h7->SetMarkerStyle(20);
   h7->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   h7->GetXaxis()->SetBinLabel(1,"=0");
   h7->GetXaxis()->SetBinLabel(2,"=1");
   h7->GetXaxis()->SetBinLabel(3,"=2");
   h7->GetXaxis()->SetBinLabel(4,"=3");
   h7->GetXaxis()->SetBinLabel(5,"#geq 4");
   h7->GetXaxis()->SetLabelFont(42);
   h7->GetYaxis()->SetTitle("Events");
   h7->GetYaxis()->SetLabelFont(42);
   h7->GetZaxis()->SetLabelFont(42);
   stack->Add(h7,"");
   stack->Draw("histsame");
   
   TH1 *recolevelc_1_frame__389 = new TH1F("recolevelc_1_frame__389","recolevelc_1_stackcontour",5,0,5);
   recolevelc_1_frame__389->SetBinContent(1,323698.6);
   recolevelc_1_frame__389->SetBinContent(2,136997.4);
   recolevelc_1_frame__389->SetBinContent(3,48596.48);
   recolevelc_1_frame__389->SetBinContent(4,15283.13);
   recolevelc_1_frame__389->SetBinContent(5,6293.404);
   recolevelc_1_frame__389->SetBinError(1,307.7871);
   recolevelc_1_frame__389->SetBinError(2,198.9637);
   recolevelc_1_frame__389->SetBinError(3,117.6628);
   recolevelc_1_frame__389->SetBinError(4,65.44722);
   recolevelc_1_frame__389->SetBinError(5,41.56193);
   recolevelc_1_frame__389->SetMinimum(0.001);
   recolevelc_1_frame__389->SetMaximum(1000000);
   recolevelc_1_frame__389->SetEntries(5737184);
   recolevelc_1_frame__389->SetDirectory(0);
   recolevelc_1_frame__389->SetFillColor(1);
   recolevelc_1_frame__389->SetFillStyle(0);
   recolevelc_1_frame__389->SetMarkerStyle(0);
   recolevelc_1_frame__389->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   recolevelc_1_frame__389->GetXaxis()->SetBinLabel(1,"=0");
   recolevelc_1_frame__389->GetXaxis()->SetBinLabel(2,"=1");
   recolevelc_1_frame__389->GetXaxis()->SetBinLabel(3,"=2");
   recolevelc_1_frame__389->GetXaxis()->SetBinLabel(4,"=3");
   recolevelc_1_frame__389->GetXaxis()->SetBinLabel(5,"#geq 4");
   recolevelc_1_frame__389->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__389->GetYaxis()->SetTitle("Events");
   recolevelc_1_frame__389->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__389->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__389->Draw("sameaxis");
   
   TH1 *gr6 = new TH1F("gr6","H(600)#rightarrow VV",5,0,5);
   gr6->SetBinContent(1,0.2821243);
   gr6->SetBinContent(2,0.4660567);
   gr6->SetBinContent(3,0.3354748);
   gr6->SetBinContent(4,0.172667);
   gr6->SetBinContent(5,0.1237571);
   gr6->SetBinError(1,0.009048404);
   gr6->SetBinError(2,0.01141128);
   gr6->SetBinError(3,0.007955937);
   gr6->SetBinError(4,0.004985516);
   gr6->SetBinError(5,0.005589949);
   gr6->SetEntries(39331);

   ci = TColor::GetColor("#ff6600");
   gr6->SetFillColor(ci);
   gr6->SetFillStyle(0);

   ci = TColor::GetColor("#ff6600");
   gr6->SetLineColor(ci);
   gr6->SetLineWidth(2);

   ci = TColor::GetColor("#ff6600");
   gr6->SetMarkerColor(ci);
   gr6->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   gr6->GetXaxis()->SetBinLabel(1,"=0");
   gr6->GetXaxis()->SetBinLabel(2,"=1");
   gr6->GetXaxis()->SetBinLabel(3,"=2");
   gr6->GetXaxis()->SetBinLabel(4,"=3");
   gr6->GetXaxis()->SetBinLabel(5,"#geq 4");
   gr6->GetXaxis()->SetLabelFont(42);
   gr6->GetYaxis()->SetTitle("Events");
   gr6->GetYaxis()->SetLabelFont(42);
   gr6->GetZaxis()->SetLabelFont(42);
   gr6->Draw("histsame");
   
   TH1 *gr5 = new TH1F("gr5","H(500)#rightarrow VV",5,0,5);
   gr5->SetBinContent(1,0.6144074);
   gr5->SetBinContent(2,1.064478);
   gr5->SetBinContent(3,0.8394668);
   gr5->SetBinContent(4,0.4039918);
   gr5->SetBinContent(5,0.2904571);
   gr5->SetBinError(1,0.05103855);
   gr5->SetBinError(2,0.05862363);
   gr5->SetBinError(3,0.05778938);
   gr5->SetBinError(4,0.03977989);
   gr5->SetBinError(5,0.04649303);
   gr5->SetEntries(10039);

   ci = TColor::GetColor("#ff9900");
   gr5->SetFillColor(ci);
   gr5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9900");
   gr5->SetLineColor(ci);
   gr5->SetLineWidth(2);

   ci = TColor::GetColor("#ff9900");
   gr5->SetMarkerColor(ci);
   gr5->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   gr5->GetXaxis()->SetBinLabel(1,"=0");
   gr5->GetXaxis()->SetBinLabel(2,"=1");
   gr5->GetXaxis()->SetBinLabel(3,"=2");
   gr5->GetXaxis()->SetBinLabel(4,"=3");
   gr5->GetXaxis()->SetBinLabel(5,"#geq 4");
   gr5->GetXaxis()->SetLabelFont(42);
   gr5->GetYaxis()->SetTitle("Events");
   gr5->GetYaxis()->SetLabelFont(42);
   gr5->GetZaxis()->SetLabelFont(42);
   gr5->Draw("histsame");
   
   TH1 *gr4 = new TH1F("gr4","H(400)#rightarrow VV",5,0,5);
   gr4->SetBinContent(1,2.218014);
   gr4->SetBinContent(2,3.01954);
   gr4->SetBinContent(3,2.041872);
   gr4->SetBinContent(4,1.066131);
   gr4->SetBinContent(5,0.5811562);
   gr4->SetBinError(1,0.04245814);
   gr4->SetBinError(2,0.04774439);
   gr4->SetBinError(3,0.03913333);
   gr4->SetBinError(4,0.02871004);
   gr4->SetBinError(5,0.0209947);
   gr4->SetEntries(39236);

   ci = TColor::GetColor("#cc6633");
   gr4->SetFillColor(ci);
   gr4->SetFillStyle(0);

   ci = TColor::GetColor("#cc6633");
   gr4->SetLineColor(ci);
   gr4->SetLineWidth(2);

   ci = TColor::GetColor("#cc6633");
   gr4->SetMarkerColor(ci);
   gr4->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   gr4->GetXaxis()->SetBinLabel(1,"=0");
   gr4->GetXaxis()->SetBinLabel(2,"=1");
   gr4->GetXaxis()->SetBinLabel(3,"=2");
   gr4->GetXaxis()->SetBinLabel(4,"=3");
   gr4->GetXaxis()->SetBinLabel(5,"#geq 4");
   gr4->GetXaxis()->SetLabelFont(42);
   gr4->GetYaxis()->SetTitle("Events");
   gr4->GetYaxis()->SetLabelFont(42);
   gr4->GetZaxis()->SetLabelFont(42);
   gr4->Draw("histsame");
   
   TH1 *gr3 = new TH1F("gr3","H(300)#rightarrow VV",5,0,5);
   gr3->SetBinContent(1,3.966162);
   gr3->SetBinContent(2,5.132463);
   gr3->SetBinContent(3,3.143744);
   gr3->SetBinContent(4,1.449077);
   gr3->SetBinContent(5,0.7631499);
   gr3->SetBinError(1,0.07301046);
   gr3->SetBinError(2,0.07992249);
   gr3->SetBinError(3,0.06292689);
   gr3->SetBinError(4,0.0432739);
   gr3->SetBinError(5,0.02965791);
   gr3->SetEntries(39742);

   ci = TColor::GetColor("#663300");
   gr3->SetFillColor(ci);
   gr3->SetFillStyle(0);

   ci = TColor::GetColor("#663300");
   gr3->SetLineColor(ci);
   gr3->SetLineWidth(2);

   ci = TColor::GetColor("#663300");
   gr3->SetMarkerColor(ci);
   gr3->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   gr3->GetXaxis()->SetBinLabel(1,"=0");
   gr3->GetXaxis()->SetBinLabel(2,"=1");
   gr3->GetXaxis()->SetBinLabel(3,"=2");
   gr3->GetXaxis()->SetBinLabel(4,"=3");
   gr3->GetXaxis()->SetBinLabel(5,"#geq 4");
   gr3->GetXaxis()->SetLabelFont(42);
   gr3->GetYaxis()->SetTitle("Events");
   gr3->GetYaxis()->SetLabelFont(42);
   gr3->GetZaxis()->SetLabelFont(42);
   gr3->Draw("histsame");
   
   TH1 *gr2 = new TH1F("gr2","H(200)#rightarrow VV",5,0,5);
   gr2->SetBinContent(1,10.16669);
   gr2->SetBinContent(2,11.12311);
   gr2->SetBinContent(3,5.981141);
   gr2->SetBinContent(4,2.745863);
   gr2->SetBinContent(5,1.133574);
   gr2->SetBinError(1,0.4436313);
   gr2->SetBinError(2,0.4463399);
   gr2->SetBinError(3,0.3335636);
   gr2->SetBinError(4,0.2414141);
   gr2->SetBinError(5,0.1351025);
   gr2->SetEntries(37657);

   ci = TColor::GetColor("#ffcc00");
   gr2->SetFillColor(ci);
   gr2->SetFillStyle(0);

   ci = TColor::GetColor("#ffcc00");
   gr2->SetLineColor(ci);
   gr2->SetLineWidth(2);

   ci = TColor::GetColor("#ffcc00");
   gr2->SetMarkerColor(ci);
   gr2->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   gr2->GetXaxis()->SetBinLabel(1,"=0");
   gr2->GetXaxis()->SetBinLabel(2,"=1");
   gr2->GetXaxis()->SetBinLabel(3,"=2");
   gr2->GetXaxis()->SetBinLabel(4,"=3");
   gr2->GetXaxis()->SetBinLabel(5,"#geq 4");
   gr2->GetXaxis()->SetLabelFont(42);
   gr2->GetYaxis()->SetTitle("Events");
   gr2->GetYaxis()->SetLabelFont(42);
   gr2->GetZaxis()->SetLabelFont(42);
   gr2->Draw("histsame");
   
   TH1 *data1 = new TH1F("data1","data",5,0,5);
   data1->SetBinContent(1,335208);
   data1->SetBinContent(2,142255);
   data1->SetBinContent(3,51338);
   data1->SetBinContent(4,16868);
   data1->SetBinContent(5,7294);
   data1->SetBinError(1,578.9715);
   data1->SetBinError(2,377.1671);
   data1->SetBinError(3,226.5789);
   data1->SetBinError(4,129.8769);
   data1->SetBinError(5,85.40492);
   data1->SetEntries(608553);
   data1->SetFillColor(1);
   data1->SetFillStyle(0);
   data1->SetMarkerStyle(20);
   data1->SetMarkerSize(1.5);
   data1->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   data1->GetXaxis()->SetBinLabel(1,"=0");
   data1->GetXaxis()->SetBinLabel(2,"=1");
   data1->GetXaxis()->SetBinLabel(3,"=2");
   data1->GetXaxis()->SetBinLabel(4,"=3");
   data1->GetXaxis()->SetBinLabel(5,"#geq 4");
   data1->GetXaxis()->SetLabelFont(42);
   data1->GetYaxis()->SetTitle("Events");
   data1->GetYaxis()->SetLabelFont(42);
   data1->GetZaxis()->SetLabelFont(42);
   data1->Draw("ESAME");
   
   TLegend *leg = new TLegend(0.17125,0.488218,0.40125,0.8803387,NULL,"brNDC");
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
   recolevelc_2->Range(-1.081081,-2.2,5.675676,2.2);
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
   
   TH1 *mumu_njets_tomc = new TH1F("mumu_njets_tomc","data",5,0,5);
   mumu_njets_tomc->SetBinContent(1,1.035556);
   mumu_njets_tomc->SetBinContent(2,1.038378);
   mumu_njets_tomc->SetBinContent(3,1.056414);
   mumu_njets_tomc->SetBinContent(4,1.103701);
   mumu_njets_tomc->SetBinContent(5,1.158991);
   mumu_njets_tomc->SetBinError(1,0.002041734);
   mumu_njets_tomc->SetBinError(2,0.003139072);
   mumu_njets_tomc->SetBinError(3,0.005317978);
   mumu_njets_tomc->SetBinError(4,0.009723981);
   mumu_njets_tomc->SetBinError(5,0.01558024);
   mumu_njets_tomc->SetMinimum(0);
   mumu_njets_tomc->SetMaximum(2.2);
   mumu_njets_tomc->SetEntries(76619.2);
   mumu_njets_tomc->SetFillColor(1);
   mumu_njets_tomc->SetFillStyle(0);
   mumu_njets_tomc->SetMarkerStyle(20);
   mumu_njets_tomc->SetMarkerSize(1.4);
   mumu_njets_tomc->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c)");
   mumu_njets_tomc->GetXaxis()->SetBinLabel(1,"=0");
   mumu_njets_tomc->GetXaxis()->SetBinLabel(2,"=1");
   mumu_njets_tomc->GetXaxis()->SetBinLabel(3,"=2");
   mumu_njets_tomc->GetXaxis()->SetBinLabel(4,"=3");
   mumu_njets_tomc->GetXaxis()->SetBinLabel(5,"#geq 4");
   mumu_njets_tomc->GetXaxis()->SetLabelFont(42);
   mumu_njets_tomc->GetXaxis()->SetLabelSize(0.1);
   mumu_njets_tomc->GetXaxis()->SetTitleSize(0.125);
   mumu_njets_tomc->GetXaxis()->SetTickLength(0.075);
   mumu_njets_tomc->GetXaxis()->SetTitleOffset(0.85);
   mumu_njets_tomc->GetYaxis()->SetTitle("Obs/Ref");
   mumu_njets_tomc->GetYaxis()->SetNdivisions(508);
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
