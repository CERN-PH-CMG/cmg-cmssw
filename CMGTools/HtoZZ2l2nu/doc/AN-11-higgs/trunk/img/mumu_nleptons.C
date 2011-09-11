{
//=========Macro generated from canvas: recolevelc/recolevelc
//=========  (Sun Sep 11 15:17:33 2011) by ROOT version5.24/00
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
   
   TH1 *recolevelc_1_frame__39 = new TH1F("recolevelc_1_frame__39","recolevelc_1_stackcontour",3,0,3);
   recolevelc_1_frame__39->SetBinContent(1,532895.9);
   recolevelc_1_frame__39->SetBinContent(2,623.9906);
   recolevelc_1_frame__39->SetBinContent(3,6.089367);
   recolevelc_1_frame__39->SetBinError(1,392.6509);
   recolevelc_1_frame__39->SetBinError(2,12.99147);
   recolevelc_1_frame__39->SetBinError(3,0.9081102);
   recolevelc_1_frame__39->SetMinimum(0.001);
   recolevelc_1_frame__39->SetMaximum(1000000);
   recolevelc_1_frame__39->SetEntries(5765935);
   recolevelc_1_frame__39->SetDirectory(0);
   recolevelc_1_frame__39->SetFillColor(1);
   recolevelc_1_frame__39->SetFillStyle(0);
   recolevelc_1_frame__39->SetMarkerStyle(0);
   recolevelc_1_frame__39->GetXaxis()->SetTitle("Leptons");
   recolevelc_1_frame__39->GetXaxis()->SetBinLabel(1,"=2 leptons");
   recolevelc_1_frame__39->GetXaxis()->SetBinLabel(2,"=3 leptons");
   recolevelc_1_frame__39->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   recolevelc_1_frame__39->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__39->GetYaxis()->SetTitle("Events");
   recolevelc_1_frame__39->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__39->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__39->Draw("hist");
   
   THStack *stack = new THStack();
   stack->SetName("stack");
   stack->SetTitle("recolevelc_1");
   
   TH1 *stack20__402 = new TH1F("stack20__402","recolevelc_1",3,0,3);
   stack20__402->SetMinimum(1.568565);
   stack20__402->SetMaximum(1064495);
   stack20__402->SetDirectory(0);
   stack20__402->SetStats(0);
   stack20__402->SetLineStyle(0);
   stack20__402->SetMarkerStyle(20);
   stack20__402->GetXaxis()->SetBinLabel(1,"=2 leptons");
   stack20__402->GetXaxis()->SetBinLabel(2,"=3 leptons");
   stack20__402->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   stack20__402->GetXaxis()->SetLabelFont(42);
   stack20__402->GetXaxis()->SetLabelOffset(0.007);
   stack20__402->GetXaxis()->SetTitleSize(0.05);
   stack20__402->GetXaxis()->SetTitleOffset(0.8);
   stack20__402->GetXaxis()->SetTitleFont(42);
   stack20__402->GetYaxis()->SetLabelFont(42);
   stack20__402->GetYaxis()->SetLabelOffset(0.007);
   stack20__402->GetYaxis()->SetTitleSize(0.05);
   stack20__402->GetYaxis()->SetTitleFont(42);
   stack20__402->GetZaxis()->SetLabelFont(42);
   stack20__402->GetZaxis()->SetLabelOffset(0.007);
   stack20__402->GetZaxis()->SetTitleSize(0.05);
   stack20__402->GetZaxis()->SetTitleFont(42);
   stack->SetHistogram(stack20__402);
   
   
   TH1 *h13 = new TH1F("h13","ZZ",3,0,3);
   h13->SetBinContent(1,234.3919);
   h13->SetBinContent(2,6.300274);
   h13->SetBinContent(3,3.261138);
   h13->SetBinError(1,0.9752767);
   h13->SetBinError(2,0.1593139);
   h13->SetBinError(3,0.114568);
   h13->SetEntries(175300);

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
   h12->SetBinContent(1,87.72525);
   h12->SetBinContent(2,0.07126159);
   h12->SetBinError(1,1.581461);
   h12->SetBinError(2,0.0390869);
   h12->SetEntries(8974);

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
   h11->SetBinContent(1,330.0424);
   h11->SetBinContent(2,36.24757);
   h11->SetBinContent(3,0.04685186);
   h11->SetBinError(1,1.994114);
   h11->SetBinError(2,0.6639088);
   h11->SetBinError(3,0.02377911);
   h11->SetEntries(88626);

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
   h10->SetBinContent(1,34.38184);
   h10->SetBinContent(2,0.7304939);
   h10->SetBinError(1,1.042816);
   h10->SetBinError(2,0.1592969);
   h10->SetEntries(2373);

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
   h9->SetBinContent(1,389.0144);
   h9->SetBinContent(2,6.681576);
   h9->SetBinContent(3,0.007992575);
   h9->SetBinError(1,7.585512);
   h9->SetBinError(2,0.9454335);
   h9->SetBinError(3,0.007992575);
   h9->SetEntries(7762);
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
   h8->SetBinContent(1,0.3099518);
   h8->SetBinError(1,0.3099518);
   h8->SetEntries(10);

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
   h7->SetBinContent(1,531820.1);
   h7->SetBinContent(2,573.9594);
   h7->SetBinContent(3,2.773385);
   h7->SetBinError(1,392.5667);
   h7->SetBinError(2,12.93799);
   h7->SetBinError(3,0.9005048);
   h7->SetEntries(5307590);

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
   
   TH1 *recolevelc_1_frame__39 = new TH1F("recolevelc_1_frame__39","recolevelc_1_stackcontour",3,0,3);
   recolevelc_1_frame__39->SetBinContent(1,532895.9);
   recolevelc_1_frame__39->SetBinContent(2,623.9906);
   recolevelc_1_frame__39->SetBinContent(3,6.089367);
   recolevelc_1_frame__39->SetBinError(1,392.6509);
   recolevelc_1_frame__39->SetBinError(2,12.99147);
   recolevelc_1_frame__39->SetBinError(3,0.9081102);
   recolevelc_1_frame__39->SetMinimum(0.001);
   recolevelc_1_frame__39->SetMaximum(1000000);
   recolevelc_1_frame__39->SetEntries(5765935);
   recolevelc_1_frame__39->SetDirectory(0);
   recolevelc_1_frame__39->SetFillColor(1);
   recolevelc_1_frame__39->SetFillStyle(0);
   recolevelc_1_frame__39->SetMarkerStyle(0);
   recolevelc_1_frame__39->GetXaxis()->SetTitle("Leptons");
   recolevelc_1_frame__39->GetXaxis()->SetBinLabel(1,"=2 leptons");
   recolevelc_1_frame__39->GetXaxis()->SetBinLabel(2,"=3 leptons");
   recolevelc_1_frame__39->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   recolevelc_1_frame__39->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__39->GetYaxis()->SetTitle("Events");
   recolevelc_1_frame__39->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__39->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__39->Draw("sameaxis");
   
   TH1 *gr6 = new TH1F("gr6","H(600)#rightarrow VV",3,0,3);
   gr6->SetBinContent(1,1.380122);
   gr6->SetBinContent(2,0.002818796);
   gr6->SetBinError(1,0.01820689);
   gr6->SetBinError(2,0.0005431246);
   gr6->SetEntries(39407);

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
   gr5->SetBinContent(1,3.212802);
   gr5->SetBinContent(2,0.0006473449);
   gr5->SetBinError(1,0.1145658);
   gr5->SetBinError(2,0.0002502416);
   gr5->SetEntries(10050);

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
   gr4->SetBinContent(1,8.92665);
   gr4->SetBinContent(2,0.0089751);
   gr4->SetBinContent(3,0.001010423);
   gr4->SetBinError(1,0.08293782);
   gr4->SetBinError(2,0.002336332);
   gr4->SetBinError(3,0.0008570272);
   gr4->SetEntries(39289);

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
   gr3->SetBinContent(1,14.45472);
   gr3->SetBinContent(2,0.01724674);
   gr3->SetBinError(1,0.1357577);
   gr3->SetBinError(2,0.00455924);
   gr3->SetEntries(39802);

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
   gr2->SetBinContent(1,31.15044);
   gr2->SetBinContent(2,0.05734671);
   gr2->SetBinError(1,0.7640852);
   gr2->SetBinError(2,0.02792808);
   gr2->SetEntries(37714);

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
   data1->SetBinContent(1,552963);
   data1->SetBinContent(2,822);
   data1->SetBinContent(3,14);
   data1->SetBinError(1,743.6148);
   data1->SetBinError(2,28.67054);
   data1->SetBinError(3,3.741657);
   data1->SetEntries(609473);
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
   
   TLegend *leg = new TLegend(0.66125,0.517673,0.89125,0.9392489,NULL,"brNDC");
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
   recolevelc_2->Range(-0.6486486,-3.2,3.405405,3.2);
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
   
   TH1 *mumu_nleptons_tomc = new TH1F("mumu_nleptons_tomc","data",3,0,3);
   mumu_nleptons_tomc->SetBinContent(1,1.037657);
   mumu_nleptons_tomc->SetBinContent(2,1.317327);
   mumu_nleptons_tomc->SetBinContent(3,2.29909);
   mumu_nleptons_tomc->SetBinError(1,0.001591154);
   mumu_nleptons_tomc->SetBinError(2,0.05351037);
   mumu_nleptons_tomc->SetBinError(3,0.7036434);
   mumu_nleptons_tomc->SetMinimum(0);
   mumu_nleptons_tomc->SetMaximum(3.2);
   mumu_nleptons_tomc->SetEntries(43.49654);
   mumu_nleptons_tomc->SetFillColor(1);
   mumu_nleptons_tomc->SetFillStyle(0);
   mumu_nleptons_tomc->SetMarkerStyle(20);
   mumu_nleptons_tomc->SetMarkerSize(1.2);
   mumu_nleptons_tomc->GetXaxis()->SetTitle("Leptons");
   mumu_nleptons_tomc->GetXaxis()->SetBinLabel(1,"=2 leptons");
   mumu_nleptons_tomc->GetXaxis()->SetBinLabel(2,"=3 leptons");
   mumu_nleptons_tomc->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   mumu_nleptons_tomc->GetXaxis()->SetLabelFont(42);
   mumu_nleptons_tomc->GetXaxis()->SetLabelSize(0.1);
   mumu_nleptons_tomc->GetXaxis()->SetTitleSize(0.125);
   mumu_nleptons_tomc->GetXaxis()->SetTickLength(0.075);
   mumu_nleptons_tomc->GetXaxis()->SetTitleOffset(0.85);
   mumu_nleptons_tomc->GetYaxis()->SetTitle("Obs/Ref");
   mumu_nleptons_tomc->GetYaxis()->SetNdivisions(509);
   mumu_nleptons_tomc->GetYaxis()->SetLabelFont(42);
   mumu_nleptons_tomc->GetYaxis()->SetLabelSize(0.1);
   mumu_nleptons_tomc->GetYaxis()->SetTitleSize(0.1);
   mumu_nleptons_tomc->GetYaxis()->SetTitleOffset(0.5);
   mumu_nleptons_tomc->GetZaxis()->SetLabelFont(42);
   mumu_nleptons_tomc->Draw("E");
   recolevelc_2->Modified();
   recolevelc->cd();
   recolevelc->Modified();
   recolevelc->cd();
   recolevelc->SetSelected(recolevelc);
}
