{
//=========Macro generated from canvas: recolevelc/recolevelc
//=========  (Wed Aug 24 17:07:59 2011) by ROOT version5.27/06b
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
   
   TH1F *recolevelc_1_frame__3__1 = new TH1F("recolevelc_1_frame__3__1","recolevelc_1_stackcontour",3,0,3);
   recolevelc_1_frame__3__1->SetBinContent(1,533535.6);
   recolevelc_1_frame__3__1->SetBinContent(2,607.219);
   recolevelc_1_frame__3__1->SetBinContent(3,5.784005);
   recolevelc_1_frame__3__1->SetBinError(1,372.3);
   recolevelc_1_frame__3__1->SetBinError(2,12.15544);
   recolevelc_1_frame__3__1->SetBinError(3,0.8181721);
   recolevelc_1_frame__3__1->SetMinimum(0.001);
   recolevelc_1_frame__3__1->SetMaximum(1000000);
   recolevelc_1_frame__3__1->SetEntries(6359883);
   recolevelc_1_frame__3__1->SetDirectory(0);
   recolevelc_1_frame__3__1->SetFillColor(1);
   recolevelc_1_frame__3__1->SetFillStyle(0);
   recolevelc_1_frame__3__1->SetMarkerStyle(0);
   recolevelc_1_frame__3__1->GetXaxis()->SetTitle("Leptons");
   recolevelc_1_frame__3__1->GetXaxis()->SetBinLabel(1,"=2 leptons");
   recolevelc_1_frame__3__1->GetXaxis()->SetBinLabel(2,"=3 leptons");
   recolevelc_1_frame__3__1->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   recolevelc_1_frame__3__1->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__3__1->GetYaxis()->SetTitle("Events");
   recolevelc_1_frame__3__1->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__3__1->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__3__1->Draw("hist");
   
   THStack *stack = new THStack();
   stack->SetName("stack");
   stack->SetTitle("recolevelc_1");
   
   TH1F *stack2__41__2 = new TH1F("stack2__41__2","recolevelc_1",3,0,3);
   stack2__41__2->SetMinimum(213.4142);
   stack2__41__2->SetMaximum(853656.9);
   stack2__41__2->SetDirectory(0);
   stack2__41__2->SetStats(0);
   stack2__41__2->SetLineStyle(0);
   stack2__41__2->SetMarkerStyle(20);
   stack2__41__2->GetXaxis()->SetBinLabel(1,"=2 leptons");
   stack2__41__2->GetXaxis()->SetBinLabel(2,"=3 leptons");
   stack2__41__2->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
   stack2__41__2->GetXaxis()->SetLabelFont(42);
   stack2__41__2->GetXaxis()->SetLabelOffset(0.007);
   stack2__41__2->GetXaxis()->SetTitleSize(0.05);
   stack2__41__2->GetXaxis()->SetTitleOffset(0.8);
   stack2__41__2->GetXaxis()->SetTitleFont(42);
   stack2__41__2->GetYaxis()->SetLabelFont(42);
   stack2__41__2->GetYaxis()->SetLabelOffset(0.007);
   stack2__41__2->GetYaxis()->SetTitleSize(0.05);
   stack2__41__2->GetYaxis()->SetTitleFont(42);
   stack2__41__2->GetZaxis()->SetLabelFont(42);
   stack2__41__2->GetZaxis()->SetLabelOffset(0.007);
   stack2__41__2->GetZaxis()->SetTitleSize(0.05);
   stack2__41__2->GetZaxis()->SetTitleFont(42);
   stack->SetHistogram(stack2__41);
   
   
   TH1F *h13 = new TH1F("h13","ZZ",3,0,3);
   h13->SetBinContent(1,234.473);
   h13->SetBinContent(2,6.275317);
   h13->SetBinContent(3,3.235132);
   h13->SetBinError(1,0.9754494);
   h13->SetBinError(2,0.1590782);
   h13->SetBinError(3,0.114024);
   h13->SetEntries(175320);

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
   
   TH1F *h12 = new TH1F("h12","WW",3,0,3);
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
   
   TH1F *h11 = new TH1F("h11","WZ",3,0,3);
   h11->SetBinContent(1,330.4981);
   h11->SetBinContent(2,35.95355);
   h11->SetBinContent(3,0.04685186);
   h11->SetBinError(1,1.995442);
   h11->SetBinError(2,0.6613491);
   h11->SetBinError(3,0.02377911);
   h11->SetEntries(88660);

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
   
   TH1F *h10 = new TH1F("h10","Single top",3,0,3);
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
   
   TH1F *h9 = new TH1F("h9","t#bar{t}",3,0,3);
   h9->SetBinContent(1,389.0882);
   h9->SetBinContent(2,6.60782);
   h9->SetBinContent(3,0.007992575);
   h9->SetBinError(1,7.585871);
   h9->SetBinError(2,0.942552);
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
   
   TH1F *h8 = new TH1F("h8","W+jets",3,0,3);
   h8->SetBinContent(1,0.3003715);
   h8->SetBinError(1,0.3003715);
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
   
   TH1F *h7 = new TH1F("h7","Z/#gamma^{*}+jets#rightarrow ll",3,0,3);
   h7->SetBinContent(1,532459.1);
   h7->SetBinContent(2,557.5806);
   h7->SetBinContent(3,2.494029);
   h7->SetBinError(1,372.2112);
   h7->SetBinError(2,12.09862);
   h7->SetBinError(3,0.8097992);
   h7->SetEntries(5901464);

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
   
   TH1F *gr6 = new TH1F("gr6","H(600)#rightarrow VV",3,0,3);
   gr6->SetBinContent(1,1.380124);
   gr6->SetBinContent(2,0.002816832);
   gr6->SetBinError(1,0.01820689);
   gr6->SetBinError(2,0.0005431211);
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
   
   TH1F *gr5 = new TH1F("gr5","H(500)#rightarrow VV",3,0,3);
   gr5->SetBinContent(1,3.135286);
   gr5->SetBinContent(2,0.0004587372);
   gr5->SetBinError(1,0.1126089);
   gr5->SetBinError(2,0.0001829822);
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
   
   TH1F *gr4 = new TH1F("gr4","H(400)#rightarrow VV",3,0,3);
   gr4->SetBinContent(1,8.539824);
   gr4->SetBinContent(2,0.0085397);
   gr4->SetBinContent(3,0.0009575333);
   gr4->SetBinError(1,0.07930468);
   gr4->SetBinError(2,0.002220122);
   gr4->SetBinError(3,0.0008246765);
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
   
   TH1F *gr3 = new TH1F("gr3","H(300)#rightarrow VV",3,0,3);
   gr3->SetBinContent(1,13.70528);
   gr3->SetBinContent(2,0.01507325);
   gr3->SetBinError(1,0.1301943);
   gr3->SetBinError(2,0.004079566);
   gr3->SetEntries(39803);

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
   
   TH1F *gr2 = new TH1F("gr2","H(200)#rightarrow VV",3,0,3);
   gr2->SetBinContent(1,29.22848);
   gr2->SetBinContent(2,0.05507332);
   gr2->SetBinError(1,0.7592309);
   gr2->SetBinError(2,0.02778541);
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
   
   TH1F *data1 = new TH1F("data1","data",3,0,3);
   data1->SetBinContent(1,604420);
   data1->SetBinContent(2,879);
   data1->SetBinContent(3,15);
   data1->SetBinError(1,777.4445);
   data1->SetBinError(2,29.64793);
   data1->SetBinError(3,3.872983);
   data1->SetEntries(660988);
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
   
   TLegend *leg = new TLegend(0.65375,0.519514,0.85375,0.9466127,NULL,"brNDC");
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
   recolevelc_2->Range(-0.6486486,-5.3,3.405405,5.3);
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
   
   TH1F *mumu_nleptons_tomc = new TH1F("mumu_nleptons_tomc","data",3,0,3);
   mumu_nleptons_tomc->SetBinContent(1,1.132858);
   mumu_nleptons_tomc->SetBinContent(2,1.447583);
   mumu_nleptons_tomc->SetBinContent(3,2.593359);
   mumu_nleptons_tomc->SetBinError(1,0.001657771);
   mumu_nleptons_tomc->SetBinError(2,0.05677748);
   mumu_nleptons_tomc->SetBinError(3,0.7635051);
   mumu_nleptons_tomc->SetMinimum(0);
   mumu_nleptons_tomc->SetMaximum(5.3);
   mumu_nleptons_tomc->SetEntries(45.66656);
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
