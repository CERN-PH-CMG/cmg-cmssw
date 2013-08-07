{
  //  TFile *fIn=TFile::Open("~/work/vbfz/gamma/2012/nvtx/plotter.root");
  TFile *fIn=TFile::Open("~/work/vbfz/gamma/2011/nvtx/plotter.root");
  
  bool is2011(TString(fIn->GetName()).Contains("2011"));

  gStyle->SetOptStat(0);	
  TCanvas *c1=new TCanvas("c","c",600,600);
  c->Divide(1,2);
  for(size_t i=0; i<=2; i++)
    {
      c->cd(i+1);
      TString ch(i==0?"mumu":"ee");
      TH2 *hdy=(TH2 *) fIn->Get("Z#rightarrow ll/"+ch+"_dijet_mass_shapes");
      TH2 *hga=(TH2 *) fIn->Get("data (#gamma)/"+ch+"_dijet_mass_shapes");

      TH1 *projdy=hdy->ProjectionX(ch+"dy"); projdy->Draw();
      TH1 *projga=hga->ProjectionX(ch+"ga"); projga->Draw("hist same");
      TH1 *ratio=(TH1 *)projga->Clone("ration_"+ch);
      ratio->Divide(projdy);
      ratio->SetDirectory(0);
      ratio->Draw();
      ratio->GetYaxis()->SetTitle("Data-MC scale factor");
      ratio->GetXaxis()->SetRangeUser(0,1000);
      TString tit=ch.ReplaceAll("mu","#mu");
      tit += " events";
      ratio->SetTitle(tit);

      TLegend* legA  = new TLegend(0.8,0.8,0.9,0.9);
      legA->AddEntry(ratio,ratio->GetTitle(),"");
      legA->SetFillColor(0); legA->SetFillStyle(0); legA->SetLineColor(0);
      legA->Draw("same");
      legA->SetTextFont(42);

      TLine *l=new TLine(0,1,ratio->GetXaxis()->GetXmax(),1);
      l->SetLineColor(kGray+1);
      l->Draw("same");

      if(i==0)
	{
	  TPaveText* T = new TPaveText(0.1,0.995,0.9,0.96, "NDC");
	  T->SetFillColor(0);
	  T->SetFillStyle(0);  T->SetLineColor(0);
	  T->SetTextAlign(22);
	  char Buffer[1024];
	  if(is2011)
	    sprintf(Buffer, "CMS preliminary, #sqrt{s}=7 TeV, #scale[0.5]{#int} L=%.1f fb^{-1}", 5051./1000);
	  else
	    sprintf(Buffer, "CMS preliminary, #sqrt{s}=8 TeV, #scale[0.5]{#int} L=%.1f fb^{-1}", 5041./1000);
	  T->AddText(Buffer);
	  T->Draw("same");
	}
      
    }
  

}
