{
  //combine -M MaxLikelihoodFit  --saveNLL --saveNorm --saveWorkspace ZZ_Datacard.dat

  using namespace RooFit;
  using namespace RooStats;
  gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so");
  gSystem->Load("libRooFit.so");
  gSystem->Load("libRooStats.so");
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1,0);
 
  //retrieve all from workspace 
  TString url[]={"MaxLikelihoodFitResult_ee.root","MaxLikelihoodFitResult_mumu.root","MaxLikelihoodFitResult_combined.root"};
  RooAbsReal* eenll,*mumunll,*combll;
  RooRealVar *mu;
  for(int i=0; i<3; i++)
    {
      TFile *f=TFile::Open(url[i]);
      RooWorkspace *w=(RooWorkspace *)f->Get("MaxLikelihoodFitResult");
      //w->Print();
      RooAbsData*	data=w->data("data_obs");
      if(i==0)
	{
	  RooAbsPdf*	eepdf=w->pdf("pdf_bineeeq0jets");
	  eenll = eepdf.createNLL(*data) ;
	  RooMinuit(*eenll).migrad() ;
	}
      else if(i==1)
	{
	  RooAbsPdf*	mumupdf=w->pdf("pdf_binmumueq0jets");
	  mumunll = mumupdf.createNLL(*data) ;
	  RooMinuit(*mumunll).migrad() ;
	}
      else
	{
	  RooAbsPdf*    eepdf=w->pdf("pdf_bineeeq0jets");
	  RooAbsPdf*	mumupdf=w->pdf("pdf_binmumueq0jets");
	  RooArgSet llSet; llSet.add((RooNLLVar &)(*eenll)); llSet.add((RooNLLVar &)(*mumunll));
	  combll = new RooAddition("combll","combll",llSet);
	  RooMinuit(*combll).migrad();
	      
	  mu=w->var("r");
	  mu->setRange(0.5,2.5);
	}
      f->Close();
    }

  //draw the likelihoods
  TCanvas *c=new TCanvas("c","c",600,600);
  c->cd();
  RooPlot *frame = mu->frame(Title("CMS preliminary, #sqrt{s}=7 TeV, #int L = 5.1 fb^{-1}"),Bins(1000)) ;
  eenll->plotOn(frame,LineColor(kGreen),LineStyle(kDashed),ShiftToZero(),Name("ee"));
  mumunll->plotOn(frame,LineColor(kRed),LineStyle(kDashed),ShiftToZero(),Name("mumu"));
  combll->plotOn(frame,LineWidth(3),ShiftToZero(),Name("combined"));

  frame->Draw(); 
  frame->GetXaxis()->SetTitle("#sigma / #sigma_{th.}^{NLO}"); frame->GetXaxis()->SetTitleSize(0.05); frame->GetXaxis()->SetTitleOffset(1.0);
  frame->GetYaxis()->SetTitle("- ln (L/L_{max})");      frame->GetYaxis()->SetTitleSize(0.05); frame->GetYaxis()->SetTitleOffset(1.0);
  
  TLegend *leg=new TLegend(0.2,0.8,0.8,0.9);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetNColumns(3);
  leg->SetHeader("CMS preliminary,#sqrt{s}=7 TeV, #int L = 5.1 fb^{-1}");
  leg->AddEntry("ee","ee","l");
  leg->AddEntry("mumu","#mu#mu","l");
  leg->AddEntry("combined","Combined","l");
  leg->Draw("same");

  c->SaveAs("LikelihoodFit.C");
  //  c->SaveAs("LikelihoodFit.png");
  // c->SaveAs("LikelihoodFit.pdf");
}
