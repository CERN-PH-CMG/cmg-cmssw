#include <string>
using namespace std;
int drawMergeSystPlot(const char *eta="centr",const char* dataset="DiJet_SystDatabase")
{
TFile *f1=TFile::Open( (string("checkSystPlots_")+dataset+"/syst_effqglJet_"+eta+".eps.root").c_str());
TFile *f2=TFile::Open( (string("checkSystPlots_")+dataset+"_Hpp/syst_effqglJet_"+eta+".eps.root").c_str());

TCanvas *c1=f1->Get("c1");
TCanvas *c2=f2->Get("c1");


TH1D* hg=c2->FindObject     ( Form("effNum_%s%s",eta,"_gluon_thresh1"     ) )->Clone("eff_gluon_hpp");
TH1D* hg_syst=c2->FindObject( Form("effNum_%s%s",eta,"_gluon_syst_thresh1") )->Clone("eff_gluon_syst_hpp");
TH1D* hq=c2->FindObject     ( Form("effNum_%s%s",eta,"_quark_thresh1"     ) )->Clone("eff_quark_hpp");
TH1D* hq_syst=c2->FindObject( Form("effNum_%s%s",eta,"_quark_syst_thresh1") )->Clone("eff_quark_syst_hpp");

c1->Draw();
hg->SetMarkerColor(kRed+2);
hg_syst->SetMarkerColor(kRed+2);

hq->SetMarkerColor(kBlue+2);
hq_syst->SetMarkerColor(kBlue+2);

hg->Draw("P SAME");
hg_syst->Draw("P SAME");
hq->Draw("P SAME");
hq_syst->Draw("P SAME");
//Exisist Legends
//TLegend* legend_quark = new TLegend( 0.17, 0.79, 0.45, 0.91 );
//TLegend* legend_gluon = new TLegend( 0.55, 0.79, 0.83, 0.91 );
TLegend* legend_quark_hpp;// = new TLegend( 0.17, 0.48, 0.45, 0.64 );
TLegend* legend_gluon_hpp;// = new TLegend( 0.55, 0.48, 0.83, 0.64 );
if(string(eta)=="centr"){
legend_quark_hpp = new TLegend( 0.17, 0.48, 0.45, 0.64 );
legend_gluon_hpp = new TLegend( 0.55, 0.48, 0.83, 0.64 );
}
else {
legend_quark_hpp = new TLegend( 0.17, 0.50, 0.45, 0.62 );
legend_gluon_hpp = new TLegend( 0.55, 0.50, 0.83, 0.62 );
}

legend_quark_hpp->SetBorderSize(0);legend_quark_hpp->SetFillStyle(0);
legend_gluon_hpp->SetBorderSize(0);legend_gluon_hpp->SetFillStyle(0);

legend_quark_hpp->AddEntry(hq,"quark (H++)","P");
legend_quark_hpp->AddEntry(hq_syst,"quark (H++ smeared)","P");
legend_gluon_hpp->AddEntry(hg,"gluon (H++)","P");
legend_gluon_hpp->AddEntry(hg_syst,"gluon (H++ smeared)","P");

legend_quark_hpp->Draw();
legend_gluon_hpp->Draw();

c1->SaveAs((string("checkSystPlots_")+dataset+"/syst_effqglJet_"+eta+"_MERGED.pdf").c_str());
return 0;
}
