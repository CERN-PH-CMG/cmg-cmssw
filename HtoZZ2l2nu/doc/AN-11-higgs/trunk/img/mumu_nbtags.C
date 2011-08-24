{
//=========Macro generated from canvas: recolevelc/recolevelc
//=========  (Wed Aug 24 22:49:52 2011) by ROOT version5.24/00
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
   recolevelc_1->Range(-0.6486486,-2.109756,3.405405,6.426829);
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
   
   TH1 *recolevelc_1_frame__347 = new TH1F("recolevelc_1_frame__347","recolevelc_1_stackcontour",3,0,3);
   recolevelc_1_frame__347->SetBinContent(1,520570);
   recolevelc_1_frame__347->SetBinContent(2,11921.15);
   recolevelc_1_frame__347->SetBinContent(3,800.8624);
   recolevelc_1_frame__347->SetBinError(1,367.9122);
   recolevelc_1_frame__347->SetBinError(2,55.34341);
   recolevelc_1_frame__347->SetBinError(3,13.6041);
   recolevelc_1_frame__347->SetMinimum(0.1);
   recolevelc_1_frame__347->SetMaximum(1000000);
   recolevelc_1_frame__347->SetEntries(6330828);
   recolevelc_1_frame__347->SetDirectory(0);
   recolevelc_1_frame__347->SetFillColor(1);
   recolevelc_1_frame__347->SetFillStyle(0);
   recolevelc_1_frame__347->SetMarkerStyle(0);
   recolevelc_1_frame__347->GetXaxis()->SetTitle("b-tag multiplicity");
   recolevelc_1_frame__347->GetXaxis()->SetBinLabel(1,"0 b-tags");
   recolevelc_1_frame__347->GetXaxis()->SetBinLabel(2,"1 b-tags");
   recolevelc_1_frame__347->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   recolevelc_1_frame__347->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__347->GetYaxis()->SetTitle(" Events");
   recolevelc_1_frame__347->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__347->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__347->Draw("hist");
   
   THStack *stack = new THStack();
   stack->SetName("stack");
   stack->SetTitle("recolevelc_1");
   
   TH1 *stack174__3481 = new TH1F("stack174__3481","recolevelc_1",3,0,3);
   stack174__3481->SetMinimum(296.6964);
   stack174__3481->SetMaximum(818198.5);
   stack174__3481->SetDirectory(0);
   stack174__3481->SetStats(0);
   stack174__3481->SetLineStyle(0);
   stack174__3481->SetMarkerStyle(20);
   stack174__3481->GetXaxis()->SetBinLabel(1,"0 b-tags");
   stack174__3481->GetXaxis()->SetBinLabel(2,"1 b-tags");
   stack174__3481->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   stack174__3481->GetXaxis()->SetLabelFont(42);
   stack174__3481->GetXaxis()->SetLabelOffset(0.007);
   stack174__3481->GetXaxis()->SetTitleSize(0.05);
   stack174__3481->GetXaxis()->SetTitleOffset(0.8);
   stack174__3481->GetXaxis()->SetTitleFont(42);
   stack174__3481->GetYaxis()->SetLabelFont(42);
   stack174__3481->GetYaxis()->SetLabelOffset(0.007);
   stack174__3481->GetYaxis()->SetTitleSize(0.05);
   stack174__3481->GetYaxis()->SetTitleFont(42);
   stack174__3481->GetZaxis()->SetLabelFont(42);
   stack174__3481->GetZaxis()->SetLabelOffset(0.007);
   stack174__3481->GetZaxis()->SetTitleSize(0.05);
   stack174__3481->GetZaxis()->SetTitleFont(42);
   stack->SetHistogram(stack174__3481);
   
   
   TH1 *h13 = new TH1F("h13","ZZ",3,0,3);
   h13->SetBinContent(1,180.9677);
   h13->SetBinContent(2,44.1226);
   h13->SetBinContent(3,9.380447);
   h13->SetBinError(1,0.8567623);
   h13->SetBinError(2,0.4235531);
   h13->SetBinError(3,0.1950966);
   h13->SetEntries(168350);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#ccccff");
   h13->SetFillColor(ci);

   ci = TColor::GetColor("#ccccff");
   h13->SetMarkerColor(ci);
   h13->GetXaxis()->SetTitle("b-tag multiplicity");
   h13->GetXaxis()->SetBinLabel(1,"0 b-tags");
   h13->GetXaxis()->SetBinLabel(2,"1 b-tags");
   h13->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   h13->GetXaxis()->SetLabelFont(42);
   h13->GetYaxis()->SetTitle(" Events");
   h13->GetYaxis()->SetLabelFont(42);
   h13->GetZaxis()->SetLabelFont(42);
   stack->Add(h13,"");
   
   TH1 *h12 = new TH1F("h12","WW",3,0,3);
   h12->SetBinContent(1,84.91834);
   h12->SetBinContent(2,2.729244);
   h12->SetBinContent(3,0.07762565);
   h12->SetBinError(1,1.556144);
   h12->SetBinError(2,0.2786741);
   h12->SetBinError(3,0.0421643);
   h12->SetEntries(8960);

   ci = TColor::GetColor("#9999cc");
   h12->SetFillColor(ci);

   ci = TColor::GetColor("#9999cc");
   h12->SetMarkerColor(ci);
   h12->GetXaxis()->SetTitle("b-tag multiplicity");
   h12->GetXaxis()->SetBinLabel(1,"0 b-tags");
   h12->GetXaxis()->SetBinLabel(2,"1 b-tags");
   h12->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   h12->GetXaxis()->SetLabelFont(42);
   h12->GetYaxis()->SetTitle(" Events");
   h12->GetYaxis()->SetLabelFont(42);
   h12->GetZaxis()->SetLabelFont(42);
   stack->Add(h12,"");
   
   TH1 *h11 = new TH1F("h11","WZ",3,0,3);
   h11->SetBinContent(1,279.256);
   h11->SetBinContent(2,47.94425);
   h11->SetBinContent(3,3.291695);
   h11->SetBinError(1,1.835482);
   h11->SetBinError(2,0.7568527);
   h11->SetBinError(3,0.1999201);
   h11->SetEntries(80014);

   ci = TColor::GetColor("#3333ff");
   h11->SetFillColor(ci);

   ci = TColor::GetColor("#3333ff");
   h11->SetMarkerColor(ci);
   h11->GetXaxis()->SetTitle("b-tag multiplicity");
   h11->GetXaxis()->SetBinLabel(1,"0 b-tags");
   h11->GetXaxis()->SetBinLabel(2,"1 b-tags");
   h11->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   h11->GetXaxis()->SetLabelFont(42);
   h11->GetYaxis()->SetTitle(" Events");
   h11->GetYaxis()->SetLabelFont(42);
   h11->GetZaxis()->SetLabelFont(42);
   stack->Add(h11,"");
   
   TH1 *h10 = new TH1F("h10","Single top",3,0,3);
   h10->SetBinContent(1,10.07477);
   h10->SetBinContent(2,20.24761);
   h10->SetBinContent(3,4.059556);
   h10->SetBinError(1,0.5603502);
   h10->SetBinError(2,0.8049916);
   h10->SetBinError(3,0.3542049);
   h10->SetEntries(2336);

   ci = TColor::GetColor("#669900");
   h10->SetFillColor(ci);

   ci = TColor::GetColor("#669900");
   h10->SetMarkerColor(ci);
   h10->GetXaxis()->SetTitle("b-tag multiplicity");
   h10->GetXaxis()->SetBinLabel(1,"0 b-tags");
   h10->GetXaxis()->SetBinLabel(2,"1 b-tags");
   h10->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   h10->GetXaxis()->SetLabelFont(42);
   h10->GetYaxis()->SetTitle(" Events");
   h10->GetYaxis()->SetLabelFont(42);
   h10->GetZaxis()->SetLabelFont(42);
   stack->Add(h10,"");
   
   TH1 *h9 = new TH1F("h9","t#bar{t}",3,0,3);
   h9->SetBinContent(1,41.82048);
   h9->SetBinContent(2,169.8588);
   h9->SetBinContent(3,177.4098);
   h9->SetBinError(1,2.459959);
   h9->SetBinError(2,4.995494);
   h9->SetBinError(3,5.151609);
   h9->SetEntries(7612);
   h9->SetFillColor(8);
   h9->SetMarkerColor(8);
   h9->SetMarkerStyle(20);
   h9->GetXaxis()->SetTitle("b-tag multiplicity");
   h9->GetXaxis()->SetBinLabel(1,"0 b-tags");
   h9->GetXaxis()->SetBinLabel(2,"1 b-tags");
   h9->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   h9->GetXaxis()->SetLabelFont(42);
   h9->GetYaxis()->SetTitle(" Events");
   h9->GetYaxis()->SetLabelFont(42);
   h9->GetZaxis()->SetLabelFont(42);
   stack->Add(h9,"");
   
   TH1 *h8 = new TH1F("h8","W+jets",3,0,3);
   h8->SetBinContent(1,0.3003715);
   h8->SetBinError(1,0.3003715);
   h8->SetEntries(10);

   ci = TColor::GetColor("#cc3300");
   h8->SetFillColor(ci);

   ci = TColor::GetColor("#cc3300");
   h8->SetMarkerColor(ci);
   h8->SetMarkerStyle(20);
   h8->GetXaxis()->SetTitle("b-tag multiplicity");
   h8->GetXaxis()->SetBinLabel(1,"0 b-tags");
   h8->GetXaxis()->SetBinLabel(2,"1 b-tags");
   h8->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   h8->GetXaxis()->SetLabelFont(42);
   h8->GetYaxis()->SetTitle(" Events");
   h8->GetYaxis()->SetLabelFont(42);
   h8->GetZaxis()->SetLabelFont(42);
   stack->Add(h8,"");
   
   TH1 *h7 = new TH1F("h7","Z/#gamma^{*}+jets#rightarrow ll",3,0,3);
   h7->SetBinContent(1,519972.7);
   h7->SetBinContent(2,11636.24);
   h7->SetBinContent(3,606.6433);
   h7->SetBinError(1,367.8945);
   h7->SetBinError(2,55.10408);
   h7->SetBinError(3,12.58281);
   h7->SetEntries(5895196);

   ci = TColor::GetColor("#99ffcc");
   h7->SetFillColor(ci);

   ci = TColor::GetColor("#99ffcc");
   h7->SetMarkerColor(ci);
   h7->SetMarkerStyle(20);
   h7->GetXaxis()->SetTitle("b-tag multiplicity");
   h7->GetXaxis()->SetBinLabel(1,"0 b-tags");
   h7->GetXaxis()->SetBinLabel(2,"1 b-tags");
   h7->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   h7->GetXaxis()->SetLabelFont(42);
   h7->GetYaxis()->SetTitle(" Events");
   h7->GetYaxis()->SetLabelFont(42);
   h7->GetZaxis()->SetLabelFont(42);
   stack->Add(h7,"");
   stack->Draw("histsame");
   
   TH1 *recolevelc_1_frame__347 = new TH1F("recolevelc_1_frame__347","recolevelc_1_stackcontour",3,0,3);
   recolevelc_1_frame__347->SetBinContent(1,520570);
   recolevelc_1_frame__347->SetBinContent(2,11921.15);
   recolevelc_1_frame__347->SetBinContent(3,800.8624);
   recolevelc_1_frame__347->SetBinError(1,367.9122);
   recolevelc_1_frame__347->SetBinError(2,55.34341);
   recolevelc_1_frame__347->SetBinError(3,13.6041);
   recolevelc_1_frame__347->SetMinimum(0.001);
   recolevelc_1_frame__347->SetMaximum(1000000);
   recolevelc_1_frame__347->SetEntries(6330828);
   recolevelc_1_frame__347->SetDirectory(0);
   recolevelc_1_frame__347->SetFillColor(1);
   recolevelc_1_frame__347->SetFillStyle(0);
   recolevelc_1_frame__347->SetMarkerStyle(0);
   recolevelc_1_frame__347->GetXaxis()->SetTitle("b-tag multiplicity");
   recolevelc_1_frame__347->GetXaxis()->SetBinLabel(1,"0 b-tags");
   recolevelc_1_frame__347->GetXaxis()->SetBinLabel(2,"1 b-tags");
   recolevelc_1_frame__347->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   recolevelc_1_frame__347->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__347->GetYaxis()->SetTitle(" Events");
   recolevelc_1_frame__347->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__347->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__347->Draw("sameaxis");
   
   TH1 *gr6 = new TH1F("gr6","H(600)#rightarrow VV",3,0,3);
   gr6->SetBinContent(1,1.219946);
   gr6->SetBinContent(2,0.148642);
   gr6->SetBinContent(3,0.01153209);
   gr6->SetBinError(1,0.01749797);
   gr6->SetBinError(2,0.004917427);
   gr6->SetBinError(3,0.001063381);
   gr6->SetEntries(39332);

   ci = TColor::GetColor("#ff6600");
   gr6->SetFillColor(ci);
   gr6->SetFillStyle(0);

   ci = TColor::GetColor("#ff6600");
   gr6->SetLineColor(ci);
   gr6->SetLineWidth(2);

   ci = TColor::GetColor("#ff6600");
   gr6->SetMarkerColor(ci);
   gr6->GetXaxis()->SetTitle("b-tag multiplicity");
   gr6->GetXaxis()->SetBinLabel(1,"0 b-tags");
   gr6->GetXaxis()->SetBinLabel(2,"1 b-tags");
   gr6->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   gr6->GetXaxis()->SetLabelFont(42);
   gr6->GetYaxis()->SetTitle(" Events");
   gr6->GetYaxis()->SetLabelFont(42);
   gr6->GetZaxis()->SetLabelFont(42);
   gr6->Draw("histsame");
   
   TH1 *gr5 = new TH1F("gr5","H(500)#rightarrow VV",3,0,3);
   gr5->SetBinContent(1,2.7766);
   gr5->SetBinContent(2,0.3483199);
   gr5->SetBinContent(3,0.0103686);
   gr5->SetBinError(1,0.103774);
   gr5->SetBinError(2,0.04348578);
   gr5->SetBinError(3,0.004551211);
   gr5->SetEntries(10041);

   ci = TColor::GetColor("#ff9900");
   gr5->SetFillColor(ci);
   gr5->SetFillStyle(0);

   ci = TColor::GetColor("#ff9900");
   gr5->SetLineColor(ci);
   gr5->SetLineWidth(2);

   ci = TColor::GetColor("#ff9900");
   gr5->SetMarkerColor(ci);
   gr5->GetXaxis()->SetTitle("b-tag multiplicity");
   gr5->GetXaxis()->SetBinLabel(1,"0 b-tags");
   gr5->GetXaxis()->SetBinLabel(2,"1 b-tags");
   gr5->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   gr5->GetXaxis()->SetLabelFont(42);
   gr5->GetYaxis()->SetTitle(" Events");
   gr5->GetYaxis()->SetLabelFont(42);
   gr5->GetZaxis()->SetLabelFont(42);
   gr5->Draw("histsame");
   
   TH1 *gr4 = new TH1F("gr4","H(400)#rightarrow VV",3,0,3);
   gr4->SetBinContent(1,7.623974);
   gr4->SetBinContent(2,0.8529629);
   gr4->SetBinContent(3,0.06285591);
   gr4->SetBinError(1,0.07482096);
   gr4->SetBinError(2,0.02534627);
   gr4->SetBinError(3,0.006973002);
   gr4->SetEntries(39238);

   ci = TColor::GetColor("#cc6633");
   gr4->SetFillColor(ci);
   gr4->SetFillStyle(0);

   ci = TColor::GetColor("#cc6633");
   gr4->SetLineColor(ci);
   gr4->SetLineWidth(2);

   ci = TColor::GetColor("#cc6633");
   gr4->SetMarkerColor(ci);
   gr4->GetXaxis()->SetTitle("b-tag multiplicity");
   gr4->GetXaxis()->SetBinLabel(1,"0 b-tags");
   gr4->GetXaxis()->SetBinLabel(2,"1 b-tags");
   gr4->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   gr4->GetXaxis()->SetLabelFont(42);
   gr4->GetYaxis()->SetTitle(" Events");
   gr4->GetYaxis()->SetLabelFont(42);
   gr4->GetZaxis()->SetLabelFont(42);
   gr4->Draw("histsame");
   
   TH1 *gr3 = new TH1F("gr3","H(300)#rightarrow VV",3,0,3);
   gr3->SetBinContent(1,12.4404);
   gr3->SetBinContent(2,1.195702);
   gr3->SetBinContent(3,0.06911365);
   gr3->SetBinError(1,0.1243189);
   gr3->SetBinError(2,0.03764126);
   gr3->SetBinError(3,0.008859276);
   gr3->SetEntries(39747);

   ci = TColor::GetColor("#663300");
   gr3->SetFillColor(ci);
   gr3->SetFillStyle(0);

   ci = TColor::GetColor("#663300");
   gr3->SetLineColor(ci);
   gr3->SetLineWidth(2);

   ci = TColor::GetColor("#663300");
   gr3->SetMarkerColor(ci);
   gr3->GetXaxis()->SetTitle("b-tag multiplicity");
   gr3->GetXaxis()->SetBinLabel(1,"0 b-tags");
   gr3->GetXaxis()->SetBinLabel(2,"1 b-tags");
   gr3->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   gr3->GetXaxis()->SetLabelFont(42);
   gr3->GetYaxis()->SetTitle(" Events");
   gr3->GetYaxis()->SetLabelFont(42);
   gr3->GetZaxis()->SetLabelFont(42);
   gr3->Draw("histsame");
   
   TH1 *gr2 = new TH1F("gr2","H(200)#rightarrow VV",3,0,3);
   gr2->SetBinContent(1,26.70447);
   gr2->SetBinContent(2,2.422449);
   gr2->SetBinContent(3,0.101494);
   gr2->SetBinError(1,0.7249109);
   gr2->SetBinError(2,0.2253096);
   gr2->SetBinError(3,0.01308957);
   gr2->SetEntries(37659);

   ci = TColor::GetColor("#ffcc00");
   gr2->SetFillColor(ci);
   gr2->SetFillStyle(0);

   ci = TColor::GetColor("#ffcc00");
   gr2->SetLineColor(ci);
   gr2->SetLineWidth(2);

   ci = TColor::GetColor("#ffcc00");
   gr2->SetMarkerColor(ci);
   gr2->GetXaxis()->SetTitle("b-tag multiplicity");
   gr2->GetXaxis()->SetBinLabel(1,"0 b-tags");
   gr2->GetXaxis()->SetBinLabel(2,"1 b-tags");
   gr2->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   gr2->GetXaxis()->SetLabelFont(42);
   gr2->GetYaxis()->SetTitle(" Events");
   gr2->GetYaxis()->SetLabelFont(42);
   gr2->GetZaxis()->SetLabelFont(42);
   gr2->Draw("histsame");
   
   TH1 *data1 = new TH1F("data1","data",3,0,3);
   data1->SetBinContent(1,588251);
   data1->SetBinContent(2,15058);
   data1->SetBinContent(3,1111);
   data1->SetBinError(1,766.9752);
   data1->SetBinError(2,122.711);
   data1->SetBinError(3,33.33167);
   data1->SetEntries(660010);
   data1->SetFillColor(1);
   data1->SetFillStyle(0);
   data1->SetMarkerStyle(20);
   data1->SetMarkerSize(1.5);
   data1->GetXaxis()->SetTitle("b-tag multiplicity");
   data1->GetXaxis()->SetBinLabel(1,"0 b-tags");
   data1->GetXaxis()->SetBinLabel(2,"1 b-tags");
   data1->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   data1->GetXaxis()->SetLabelFont(42);
   data1->GetYaxis()->SetTitle(" Events");
   data1->GetYaxis()->SetLabelFont(42);
   data1->GetZaxis()->SetLabelFont(42);
   data1->Draw("ESAME");
   
   TLegend *leg = new TLegend(0.65,0.6078792,0.85,0.9502946,NULL,"brNDC");
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
   recolevelc_2->Range(-0.6486486,-0.4066465,3.405405,1.648807);
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
   
   TH1 *mumu_nbtags_tomc = new TH1F("mumu_nbtags_tomc","data",3,0,3);
   mumu_nbtags_tomc->SetBinContent(1,1.130013);
   mumu_nbtags_tomc->SetBinContent(2,1.263134);
   mumu_nbtags_tomc->SetBinContent(3,1.387254);
   mumu_nbtags_tomc->SetBinError(1,0.00167587);
   mumu_nbtags_tomc->SetBinError(2,0.0118467);
   mumu_nbtags_tomc->SetBinError(3,0.04782793);
   mumu_nbtags_tomc->SetMinimum(0.6210801);
   mumu_nbtags_tomc->SetMaximum(1.648807);
   mumu_nbtags_tomc->SetEntries(5879.642);
   mumu_nbtags_tomc->SetFillColor(1);
   mumu_nbtags_tomc->SetFillStyle(0);
   mumu_nbtags_tomc->SetMarkerStyle(20);
   mumu_nbtags_tomc->SetMarkerSize(1.3);
   mumu_nbtags_tomc->GetXaxis()->SetTitle("b-tag multiplicity");
   mumu_nbtags_tomc->GetXaxis()->SetBinLabel(1,"0 b-tags");
   mumu_nbtags_tomc->GetXaxis()->SetBinLabel(2,"1 b-tags");
   mumu_nbtags_tomc->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   mumu_nbtags_tomc->GetXaxis()->SetLabelFont(42);
   mumu_nbtags_tomc->GetXaxis()->SetLabelSize(0.1);
   mumu_nbtags_tomc->GetXaxis()->SetTitleSize(0.125);
   mumu_nbtags_tomc->GetXaxis()->SetTickLength(0.075);
   mumu_nbtags_tomc->GetXaxis()->SetTitleOffset(0.85);
   mumu_nbtags_tomc->GetYaxis()->SetTitle("Obs/Ref");
   mumu_nbtags_tomc->GetYaxis()->SetNdivisions(506);
   mumu_nbtags_tomc->GetYaxis()->SetLabelFont(42);
   mumu_nbtags_tomc->GetYaxis()->SetLabelSize(0.1);
   mumu_nbtags_tomc->GetYaxis()->SetTitleSize(0.1);
   mumu_nbtags_tomc->GetYaxis()->SetTitleOffset(0.5);
   mumu_nbtags_tomc->GetZaxis()->SetLabelFont(42);
   mumu_nbtags_tomc->Draw("E");
   recolevelc_2->Modified();
   recolevelc->cd();
   recolevelc->Modified();
   recolevelc->cd();
   recolevelc->SetSelected(recolevelc);
}
