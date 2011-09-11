{
//=========Macro generated from canvas: recolevelc/recolevelc
//=========  (Sun Sep 11 15:40:15 2011) by ROOT version5.24/00
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
   
   TH1 *recolevelc_1_frame__331 = new TH1F("recolevelc_1_frame__331","recolevelc_1_stackcontour",3,0,3);
   recolevelc_1_frame__331->SetBinContent(1,456585.4);
   recolevelc_1_frame__331->SetBinContent(2,10994.44);
   recolevelc_1_frame__331->SetBinContent(3,751.0133);
   recolevelc_1_frame__331->SetBinError(1,364.7086);
   recolevelc_1_frame__331->SetBinError(2,56.13395);
   recolevelc_1_frame__331->SetBinError(3,13.79639);
   recolevelc_1_frame__331->SetMinimum(0.1);
   recolevelc_1_frame__331->SetMaximum(1000000);
   recolevelc_1_frame__331->SetEntries(5061503);
   recolevelc_1_frame__331->SetDirectory(0);
   recolevelc_1_frame__331->SetFillColor(1);
   recolevelc_1_frame__331->SetFillStyle(0);
   recolevelc_1_frame__331->SetMarkerStyle(0);
   recolevelc_1_frame__331->GetXaxis()->SetTitle("b-tag multiplicity");
   recolevelc_1_frame__331->GetXaxis()->SetBinLabel(1,"0 b-tags");
   recolevelc_1_frame__331->GetXaxis()->SetBinLabel(2,"1 b-tags");
   recolevelc_1_frame__331->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   recolevelc_1_frame__331->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__331->GetYaxis()->SetTitle(" Events");
   recolevelc_1_frame__331->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__331->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__331->Draw("hist");
   
   THStack *stack = new THStack();
   stack->SetName("stack");
   stack->SetTitle("recolevelc_1");
   
   TH1 *stack166__3321 = new TH1F("stack166__3321","recolevelc_1",3,0,3);
   stack166__3321->SetMinimum(279.902);
   stack166__3321->SetMaximum(714979.1);
   stack166__3321->SetDirectory(0);
   stack166__3321->SetStats(0);
   stack166__3321->SetLineStyle(0);
   stack166__3321->SetMarkerStyle(20);
   stack166__3321->GetXaxis()->SetBinLabel(1,"0 b-tags");
   stack166__3321->GetXaxis()->SetBinLabel(2,"1 b-tags");
   stack166__3321->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   stack166__3321->GetXaxis()->SetLabelFont(42);
   stack166__3321->GetXaxis()->SetLabelOffset(0.007);
   stack166__3321->GetXaxis()->SetTitleSize(0.05);
   stack166__3321->GetXaxis()->SetTitleOffset(0.8);
   stack166__3321->GetXaxis()->SetTitleFont(42);
   stack166__3321->GetYaxis()->SetLabelFont(42);
   stack166__3321->GetYaxis()->SetLabelOffset(0.007);
   stack166__3321->GetYaxis()->SetTitleSize(0.05);
   stack166__3321->GetYaxis()->SetTitleFont(42);
   stack166__3321->GetZaxis()->SetLabelFont(42);
   stack166__3321->GetZaxis()->SetLabelOffset(0.007);
   stack166__3321->GetZaxis()->SetTitleSize(0.05);
   stack166__3321->GetZaxis()->SetTitleFont(42);
   stack->SetHistogram(stack166__3321);
   
   
   TH1 *h13 = new TH1F("h13","ZZ",3,0,3);
   h13->SetBinContent(1,164.1263);
   h13->SetBinContent(2,40.60442);
   h13->SetBinContent(3,8.394141);
   h13->SetBinError(1,0.8166953);
   h13->SetBinError(2,0.4071652);
   h13->SetBinError(3,0.1837236);
   h13->SetEntries(152940);

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
   h12->SetBinContent(1,75.04272);
   h12->SetBinContent(2,2.173837);
   h12->SetBinContent(3,0.09122697);
   h12->SetBinError(1,1.463995);
   h12->SetBinError(2,0.2457445);
   h12->SetBinError(3,0.054183);
   h12->SetEntries(7996);

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
   h11->SetBinContent(1,256.4287);
   h11->SetBinContent(2,45.51732);
   h11->SetBinContent(3,2.951296);
   h11->SetBinError(1,1.760587);
   h11->SetBinError(2,0.743071);
   h11->SetBinError(3,0.1856642);
   h11->SetEntries(73484);

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
   h10->SetBinContent(1,10.62484);
   h10->SetBinContent(2,18.95136);
   h10->SetBinContent(3,4.992037);
   h10->SetBinError(1,0.5799317);
   h10->SetBinError(2,0.7737466);
   h10->SetBinError(3,0.4010071);
   h10->SetEntries(2339);

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
   h9->SetBinContent(1,39.73034);
   h9->SetBinContent(2,164.0269);
   h9->SetBinContent(3,160.9737);
   h9->SetBinError(1,2.398023);
   h9->SetBinError(2,4.921777);
   h9->SetBinError(3,4.810168);
   h9->SetEntries(7466);
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
   h8->SetBinContent(1,36.94463);
   h8->SetBinContent(2,1.9206);
   h8->SetBinError(1,7.110506);
   h8->SetBinError(2,1.9206);
   h8->SetEntries(92);

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
   h7->SetBinContent(1,456002.5);
   h7->SetBinContent(2,10721.25);
   h7->SetBinContent(3,573.6108);
   h7->SetBinError(1,364.6228);
   h7->SetBinError(2,55.87245);
   h7->SetBinError(3,12.92171);
   h7->SetEntries(4664246);

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
   
   TH1 *recolevelc_1_frame__331 = new TH1F("recolevelc_1_frame__331","recolevelc_1_stackcontour",3,0,3);
   recolevelc_1_frame__331->SetBinContent(1,456585.4);
   recolevelc_1_frame__331->SetBinContent(2,10994.44);
   recolevelc_1_frame__331->SetBinContent(3,751.0133);
   recolevelc_1_frame__331->SetBinError(1,364.7086);
   recolevelc_1_frame__331->SetBinError(2,56.13395);
   recolevelc_1_frame__331->SetBinError(3,13.79639);
   recolevelc_1_frame__331->SetMinimum(0.001);
   recolevelc_1_frame__331->SetMaximum(1000000);
   recolevelc_1_frame__331->SetEntries(5061503);
   recolevelc_1_frame__331->SetDirectory(0);
   recolevelc_1_frame__331->SetFillColor(1);
   recolevelc_1_frame__331->SetFillStyle(0);
   recolevelc_1_frame__331->SetMarkerStyle(0);
   recolevelc_1_frame__331->GetXaxis()->SetTitle("b-tag multiplicity");
   recolevelc_1_frame__331->GetXaxis()->SetBinLabel(1,"0 b-tags");
   recolevelc_1_frame__331->GetXaxis()->SetBinLabel(2,"1 b-tags");
   recolevelc_1_frame__331->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   recolevelc_1_frame__331->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__331->GetYaxis()->SetTitle(" Events");
   recolevelc_1_frame__331->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__331->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__331->Draw("sameaxis");
   
   TH1 *gr6 = new TH1F("gr6","H(600)#rightarrow VV",3,0,3);
   gr6->SetBinContent(1,1.281044);
   gr6->SetBinContent(2,0.1650743);
   gr6->SetBinContent(3,0.01554898);
   gr6->SetBinError(1,0.01473417);
   gr6->SetBinError(2,0.004714075);
   gr6->SetBinError(3,0.001224838);
   gr6->SetEntries(42591);

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
   gr5->SetBinContent(1,3.153282);
   gr5->SetBinContent(2,0.355259);
   gr5->SetBinContent(3,0.02610426);
   gr5->SetBinError(1,0.1072512);
   gr5->SetBinError(2,0.03238361);
   gr5->SetBinError(3,0.007757463);
   gr5->SetEntries(10621);

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
   gr4->SetBinContent(1,8.091846);
   gr4->SetBinContent(2,0.8805132);
   gr4->SetBinContent(3,0.05233525);
   gr4->SetBinError(1,0.07870221);
   gr4->SetBinError(2,0.02553406);
   gr4->SetBinError(3,0.005855645);
   gr4->SetEntries(40226);

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
   gr3->SetBinContent(1,12.8097);
   gr3->SetBinContent(2,1.137733);
   gr3->SetBinContent(3,0.06584017);
   gr3->SetBinError(1,0.1276982);
   gr3->SetBinError(2,0.03695757);
   gr3->SetBinError(3,0.009056991);
   gr3->SetEntries(38916);

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
   gr2->SetBinContent(1,26.98326);
   gr2->SetBinContent(2,2.440157);
   gr2->SetBinContent(3,0.1534209);
   gr2->SetBinError(1,0.737387);
   gr2->SetBinError(2,0.2445388);
   gr2->SetBinError(3,0.06314339);
   gr2->SetEntries(34976);

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
   data1->SetBinContent(1,460096);
   data1->SetBinContent(2,12023);
   data1->SetBinContent(3,900);
   data1->SetBinError(1,678.3038);
   data1->SetBinError(2,109.6494);
   data1->SetBinError(3,30);
   data1->SetEntries(473087);
   data1->SetFillColor(1);
   data1->SetFillStyle(0);
   data1->SetMarkerStyle(20);
   data1->SetMarkerSize(1.6);
   data1->GetXaxis()->SetTitle("b-tag multiplicity");
   data1->GetXaxis()->SetBinLabel(1,"0 b-tags");
   data1->GetXaxis()->SetBinLabel(2,"1 b-tags");
   data1->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   data1->GetXaxis()->SetLabelFont(42);
   data1->GetYaxis()->SetTitle(" Events");
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
   
   TH1 *ee_nbtags_tomc = new TH1F("ee_nbtags_tomc","data",3,0,3);
   ee_nbtags_tomc->SetBinContent(1,1.007689);
   ee_nbtags_tomc->SetBinContent(2,1.093552);
   ee_nbtags_tomc->SetBinContent(3,1.198381);
   ee_nbtags_tomc->SetBinError(1,0.001689645);
   ee_nbtags_tomc->SetBinError(2,0.01142967);
   ee_nbtags_tomc->SetBinError(3,0.04561066);
   ee_nbtags_tomc->SetMinimum(0);
   ee_nbtags_tomc->SetMaximum(2.2);
   ee_nbtags_tomc->SetEntries(4917.963);
   ee_nbtags_tomc->SetFillColor(1);
   ee_nbtags_tomc->SetFillStyle(0);
   ee_nbtags_tomc->SetMarkerStyle(20);
   ee_nbtags_tomc->SetMarkerSize(1.3);
   ee_nbtags_tomc->GetXaxis()->SetTitle("b-tag multiplicity");
   ee_nbtags_tomc->GetXaxis()->SetBinLabel(1,"0 b-tags");
   ee_nbtags_tomc->GetXaxis()->SetBinLabel(2,"1 b-tags");
   ee_nbtags_tomc->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
   ee_nbtags_tomc->GetXaxis()->SetLabelFont(42);
   ee_nbtags_tomc->GetXaxis()->SetLabelSize(0.1);
   ee_nbtags_tomc->GetXaxis()->SetTitleSize(0.125);
   ee_nbtags_tomc->GetXaxis()->SetTickLength(0.075);
   ee_nbtags_tomc->GetXaxis()->SetTitleOffset(0.85);
   ee_nbtags_tomc->GetYaxis()->SetTitle("Obs/Ref");
   ee_nbtags_tomc->GetYaxis()->SetNdivisions(509);
   ee_nbtags_tomc->GetYaxis()->SetLabelFont(42);
   ee_nbtags_tomc->GetYaxis()->SetLabelSize(0.1);
   ee_nbtags_tomc->GetYaxis()->SetTitleSize(0.1);
   ee_nbtags_tomc->GetYaxis()->SetTitleOffset(0.5);
   ee_nbtags_tomc->GetZaxis()->SetLabelFont(42);
   ee_nbtags_tomc->Draw("E");
   recolevelc_2->Modified();
   recolevelc->cd();
   recolevelc->Modified();
   recolevelc->cd();
   recolevelc->SetSelected(recolevelc);
}
