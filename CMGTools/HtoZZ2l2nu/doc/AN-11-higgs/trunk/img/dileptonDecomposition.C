{
//=========Macro generated from canvas: c1/c1
//=========  (Tue Sep 13 16:45:34 2011) by ROOT version5.27/06b
   TCanvas *c1 = new TCanvas("c1", "c1",395,68,500,530);
   c1->Range(0,0,1,1);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   TCurlyLine *curlyline = new TCurlyLine(0.3407258,0.1652542,0.4012097,0.4597458,0.027,0.016);
   curlyline->SetLineWidth(2);
   curlyline->Draw();
   TArrow *arrow = new TArrow(0.4012097,0.4449153,0.2116935,0.6885593,0.04,"|>");
   arrow->SetFillColor(2);
   arrow->SetFillStyle(1001);
   arrow->SetLineColor(2);
   arrow->SetLineWidth(3);
   arrow->Draw();
   arrow = new TArrow(0.3991935,0.4491525,0.6350806,0.5466102,0.03,"|>");
   arrow->SetFillColor(2);
   arrow->SetFillStyle(1001);
   arrow->SetLineColor(2);
   arrow->SetLineWidth(3);
   arrow->Draw();
   TLine *line = new TLine(0.6108871,0.3771186,0.1794355,0.5190678);
   line->SetLineColor(17);
   line->SetLineStyle(9);
   line->SetLineWidth(3);
   line->Draw();
   arrow = new TArrow(0.3991935,0.4512712,0.4435484,0.7966102,0.03,"|>");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);
   arrow->SetLineWidth(3);
   arrow->Draw();
   
   TPaveText *pt = new TPaveText(0.4133065,0.7394068,0.6068548,0.8199153,"br");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   TText *text = pt->AddText("p_{T}^{ll}");
   pt->Draw();
   
   pt = new TPaveText(0.1370968,0.7055085,0.266129,0.7775424,"br");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetLineColor(0);
   pt->SetTextColor(2);
   pt->SetTextFont(42);
   text = pt->AddText("p_{T}^{(1)}");
   pt->Draw();
   
   pt = new TPaveText(0.6330645,0.5317797,0.7620968,0.6038136,"br");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetLineColor(0);
   pt->SetTextColor(2);
   pt->SetTextFont(42);
   text = pt->AddText("p_{T}^{(2)}");
   pt->Draw();
   
   TArc *arc = new TArc(0.4112903,0.4894068,0.05040323,-15,175);
   arc->SetFillStyle(0);
   arc->SetLineColor(9);
   arc->SetLineStyle(7);
   arc->SetLineWidth(2);
   arc->SetNoEdges();
   arc->Draw();
   
   pt = new TPaveText(0.3185484,0.5381356,0.4153226,0.595339,"br");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetLineColor(0);
   pt->SetTextColor(9);
   pt->SetTextFont(42);
   text = pt->AddText("#delta#phi^{ll}");
   pt->Draw();
   
   pt = new TPaveText(0.3951613,0.184322,0.7076613,0.2690678,"br");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetLineColor(0);
   pt->SetTextFont(52);
   text = pt->AddText("hadronic recoil");
   pt->Draw();
   arrow = new TArrow(0.3951613,0.4491525,0.2842742,0.4872881,0.05,">");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);
   arrow->SetLineColor(9);
   arrow->SetLineWidth(3);
   arrow->Draw();
   arrow = new TArrow(0.3951613,0.4491525,0.4274194,0.5550847,0.05,">");
   arrow->SetFillColor(1);
   arrow->SetFillStyle(1001);
   arrow->SetLineColor(9);
   arrow->SetLineWidth(3);
   arrow->Draw();
   
   pt = new TPaveText(0.4334677,0.5254237,0.5181452,0.5911017,"br");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetLineColor(0);
   pt->SetTextColor(9);
   pt->SetTextFont(42);
   text = pt->AddText("#vec{L}");
   pt->Draw();
   
   pt = new TPaveText(0.25,0.3919492,0.3346774,0.4576271,"br");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetLineColor(0);
   pt->SetTextColor(9);
   pt->SetTextFont(42);
   text = pt->AddText("#vec{T}");
   pt->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
   c1->ToggleToolBar();
}
