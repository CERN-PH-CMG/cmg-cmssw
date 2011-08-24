{
//=========Macro generated from canvas: recolevelc/recolevelc
//=========  (Wed Aug 24 17:10:04 2011) by ROOT version5.27/06b
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
   TPad *recolevelc_1 = new TPad("recolevelc_1", "recolevelc_1",0,0.3002577,1,1);
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
   
   TH1F *recolevelc_1_frame__75__5 = new TH1F("recolevelc_1_frame__75__5","recolevelc_1_stackcontour",4,0,4);
   recolevelc_1_frame__75__5->SetBinContent(1,281099.7);
   recolevelc_1_frame__75__5->SetBinContent(2,120946.8);
   recolevelc_1_frame__75__5->SetBinContent(3,42796.86);
   recolevelc_1_frame__75__5->SetBinContent(4,19167.57);
   recolevelc_1_frame__75__5->SetBinError(1,272.3778);
   recolevelc_1_frame__75__5->SetBinError(2,177.7388);
   recolevelc_1_frame__75__5->SetBinError(3,105.0539);
   recolevelc_1_frame__75__5->SetBinError(4,69.63149);
   recolevelc_1_frame__75__5->SetMinimum(0.1);
   recolevelc_1_frame__75__5->SetMaximum(1000000);
   recolevelc_1_frame__75__5->SetEntries(5544171);
   recolevelc_1_frame__75__5->SetDirectory(0);
   recolevelc_1_frame__75__5->SetFillColor(1);
   recolevelc_1_frame__75__5->SetFillStyle(0);
   recolevelc_1_frame__75__5->SetMarkerStyle(0);
   recolevelc_1_frame__75__5->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   recolevelc_1_frame__75__5->GetXaxis()->SetBinLabel(1,"0");
   recolevelc_1_frame__75__5->GetXaxis()->SetBinLabel(2,"1");
   recolevelc_1_frame__75__5->GetXaxis()->SetBinLabel(3,"2");
   recolevelc_1_frame__75__5->GetXaxis()->SetBinLabel(4,"#geq 3");
   recolevelc_1_frame__75__5->GetXaxis()->SetLabelFont(42);
   recolevelc_1_frame__75__5->GetYaxis()->SetTitle("Events");
   recolevelc_1_frame__75__5->GetYaxis()->SetLabelFont(42);
   recolevelc_1_frame__75__5->GetZaxis()->SetLabelFont(42);
   recolevelc_1_frame__75__5->Draw("hist");
   
   THStack *stack = new THStack();
   stack->SetName("stack");
   stack->SetTitle("recolevelc_1");
   
   TH1F *stack38__763__6 = new TH1F("stack38__763__6","recolevelc_1",4,0,4);
   stack38__763__6->SetMinimum(0.1129217);
   stack38__763__6->SetMaximum(607766.8);
   stack38__763__6->SetDirectory(0);
   stack38__763__6->SetStats(0);
   stack38__763__6->SetLineStyle(0);
   stack38__763__6->SetMarkerStyle(20);
   stack38__763__6->GetXaxis()->SetBinLabel(1,"0");
   stack38__763__6->GetXaxis()->SetBinLabel(2,"1");
   stack38__763__6->GetXaxis()->SetBinLabel(3,"2");
   stack38__763__6->GetXaxis()->SetBinLabel(4,"#geq 3");
   stack38__763__6->GetXaxis()->SetLabelFont(42);
   stack38__763__6->GetXaxis()->SetLabelOffset(0.007);
   stack38__763__6->GetXaxis()->SetTitleSize(0.05);
   stack38__763__6->GetXaxis()->SetTitleOffset(0.8);
   stack38__763__6->GetXaxis()->SetTitleFont(42);
   stack38__763__6->GetYaxis()->SetLabelFont(42);
   stack38__763__6->GetYaxis()->SetLabelOffset(0.007);
   stack38__763__6->GetYaxis()->SetTitleSize(0.05);
   stack38__763__6->GetYaxis()->SetTitleFont(42);
   stack38__763__6->GetZaxis()->SetLabelFont(42);
   stack38__763__6->GetZaxis()->SetLabelOffset(0.007);
   stack38__763__6->GetZaxis()->SetTitleSize(0.05);
   stack38__763__6->GetZaxis()->SetTitleFont(42);
   stack->SetHistogram(stack38__763);
   
   
   TH1F *h13 = new TH1F("h13","ZZ",4,0,4);
   h13->SetBinContent(1,31.46254);
   h13->SetBinContent(2,43.94589);
   h13->SetBinContent(3,73.10079);
   h13->SetBinContent(4,63.35622);
   h13->SetBinError(1,0.35782);
   h13->SetBinError(2,0.4237014);
   h13->SetBinError(3,0.5461465);
   h13->SetBinError(4,0.5055951);
   h13->SetEntries(152032);

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
   h12->SetBinContent(1,38.66668);
   h12->SetBinContent(2,25.11087);
   h12->SetBinContent(3,9.376366);
   h12->SetBinContent(4,3.586171);
   h12->SetBinError(1,1.05009);
   h12->SetBinError(2,0.8455874);
   h12->SetBinError(3,0.519207);
   h12->SetBinError(4,0.319732);
   h12->SetEntries(7944);

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
   h11->SetBinContent(1,34.34269);
   h11->SetBinContent(2,74.21064);
   h11->SetBinContent(3,104.9016);
   h11->SetBinContent(4,90.1166);
   h11->SetBinError(1,0.6434257);
   h11->SetBinError(2,0.9508182);
   h11->SetBinError(3,1.12825);
   h11->SetBinError(4,1.039007);
   h11->SetEntries(73162);

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
   h10->SetBinContent(1,0.9419286);
   h10->SetBinContent(2,9.462705);
   h10->SetBinContent(3,12.08055);
   h10->SetBinContent(4,11.80092);
   h10->SetBinError(1,0.1718896);
   h10->SetBinError(2,0.5474849);
   h10->SetBinError(3,0.6280604);
   h10->SetBinError(4,0.6005134);
   h10->SetEntries(2325);

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
   h9->SetBinContent(1,0.9813212);
   h9->SetBinContent(2,19.91696);
   h9->SetBinContent(3,108.3715);
   h9->SetBinContent(4,233.5856);
   h9->SetBinError(1,0.3927636);
   h9->SetBinError(2,1.690659);
   h9->SetBinError(3,3.991073);
   h9->SetBinError(4,5.822153);
   h9->SetEntries(7426);
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
   h8->SetBinContent(1,21.38827);
   h8->SetBinContent(2,7.322577);
   h8->SetBinContent(3,1.740728);
   h8->SetBinContent(4,5.35113);
   h8->SetBinError(1,5.539827);
   h8->SetBinError(2,2.888964);
   h8->SetBinError(3,1.251089);
   h8->SetBinError(4,2.623186);
   h8->SetEntries(90);

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
   h7->SetBinContent(1,280971.9);
   h7->SetBinContent(2,120766.8);
   h7->SetBinContent(3,42487.29);
   h7->SetBinContent(4,18759.77);
   h7->SetBinError(1,272.3181);
   h7->SetBinError(2,177.7013);
   h7->SetBinError(3,104.96);
   h7->SetBinError(4,69.32509);
   h7->SetEntries(5149160);

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
   gr6->SetBinContent(1,0.2815674);
   gr6->SetBinContent(2,0.4945358);
   gr6->SetBinContent(3,0.3555002);
   gr6->SetBinContent(4,0.3202931);
   gr6->SetBinError(1,0.006555987);
   gr6->SetBinError(2,0.01011609);
   gr6->SetBinError(3,0.00623764);
   gr6->SetBinError(4,0.007448003);
   gr6->SetEntries(42307);

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
   gr5->SetBinContent(1,0.7173248);
   gr5->SetBinContent(2,1.157485);
   gr5->SetBinContent(3,0.8199369);
   gr5->SetBinContent(4,0.7348121);
   gr5->SetBinError(1,0.05140945);
   gr5->SetBinError(2,0.06113551);
   gr5->SetBinError(3,0.05429411);
   gr5->SetBinError(4,0.05163373);
   gr5->SetEntries(10552);

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
   gr4->SetBinContent(1,1.984194);
   gr4->SetBinContent(2,2.879407);
   gr4->SetBinContent(3,2.022154);
   gr4->SetBinContent(4,1.671011);
   gr4->SetBinError(1,0.03832877);
   gr4->SetBinError(2,0.04521902);
   gr4->SetBinError(3,0.03832122);
   gr4->SetBinError(4,0.03512445);
   gr4->SetEntries(39937);

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
   gr3->SetBinContent(1,3.393839);
   gr3->SetBinContent(2,4.669456);
   gr3->SetBinContent(3,3.030974);
   gr3->SetBinContent(4,2.136846);
   gr3->SetBinError(1,0.06568025);
   gr3->SetBinError(2,0.07615273);
   gr3->SetBinError(3,0.06054495);
   gr3->SetBinError(4,0.04990745);
   gr3->SetEntries(38683);

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
   gr2->SetBinContent(1,8.655963);
   gr2->SetBinContent(2,9.533116);
   gr2->SetBinContent(3,5.21841);
   gr2->SetBinContent(4,4.275489);
   gr2->SetBinError(1,0.4620216);
   gr2->SetBinError(2,0.4348852);
   gr2->SetBinError(3,0.2945124);
   gr2->SetBinError(4,0.3304464);
   gr2->SetEntries(34748);

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
   data1->SetBinContent(1,248514);
   data1->SetBinContent(2,107454);
   data1->SetBinContent(3,38864);
   data1->SetBinContent(4,18392);
   data1->SetBinError(1,498.5118);
   data1->SetBinError(2,327.8018);
   data1->SetBinError(3,197.1395);
   data1->SetBinError(4,135.6171);
   data1->SetEntries(413292);
   data1->SetFillColor(1);
   data1->SetFillStyle(0);
   data1->SetMarkerStyle(20);
   data1->SetMarkerSize(1.5);
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
   
   TLegend *leg = new TLegend(0.17125,0.5009208,0.37125,0.8637201,NULL,"brNDC");
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
   recolevelc_2->Range(-0.8648648,-0.411118,4.540541,1.51706);
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
   
   TH1F *ee_njets_tomc = new TH1F("ee_njets_tomc","data",4,0,4);
   ee_njets_tomc->SetBinContent(1,0.8840779);
   ee_njets_tomc->SetBinContent(2,0.8884404);
   ee_njets_tomc->SetBinContent(3,0.9081039);
   ee_njets_tomc->SetBinContent(4,0.9595374);
   ee_njets_tomc->SetBinError(1,0.001969496);
   ee_njets_tomc->SetBinError(2,0.00300838);
   ee_njets_tomc->SetBinError(3,0.005117418);
   ee_njets_tomc->SetBinError(4,0.007887406);
   ee_njets_tomc->SetMinimum(0.5529708);
   ee_njets_tomc->SetMaximum(1.51706);
   ee_njets_tomc->SetEntries(130770.5);
   ee_njets_tomc->SetFillColor(1);
   ee_njets_tomc->SetFillStyle(0);
   ee_njets_tomc->SetMarkerStyle(20);
   ee_njets_tomc->GetXaxis()->SetTitle("Jet multiplicity (p_{T}>15 GeV/c #wedge |#eta|<2.5)");
   ee_njets_tomc->GetXaxis()->SetBinLabel(1,"0");
   ee_njets_tomc->GetXaxis()->SetBinLabel(2,"1");
   ee_njets_tomc->GetXaxis()->SetBinLabel(3,"2");
   ee_njets_tomc->GetXaxis()->SetBinLabel(4,"#geq 3");
   ee_njets_tomc->GetXaxis()->SetLabelFont(42);
   ee_njets_tomc->GetXaxis()->SetLabelSize(0.1);
   ee_njets_tomc->GetXaxis()->SetTitleSize(0.125);
   ee_njets_tomc->GetXaxis()->SetTickLength(0.075);
   ee_njets_tomc->GetXaxis()->SetTitleOffset(0.85);
   ee_njets_tomc->GetYaxis()->SetTitle("Obs/Ref");
   ee_njets_tomc->GetYaxis()->SetNdivisions(508);
   ee_njets_tomc->GetYaxis()->SetLabelFont(42);
   ee_njets_tomc->GetYaxis()->SetLabelSize(0.1);
   ee_njets_tomc->GetYaxis()->SetTitleSize(0.1);
   ee_njets_tomc->GetYaxis()->SetTitleOffset(0.5);
   ee_njets_tomc->GetZaxis()->SetLabelFont(42);
   ee_njets_tomc->Draw("E");
   recolevelc_2->Modified();
   recolevelc->cd();
   recolevelc->Modified();
   recolevelc->cd();
   recolevelc->SetSelected(recolevelc);
}
