{
  //combine -M MaxLikelihoodFit  --saveNLL --saveNorm --saveWorkspace ZZ_Datacard.dat
  TString url="MaxLikelihoodFitResult.root";

  using namespace RooFit;
  gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so");
  gSystem->Load("libRooFit.so");
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1,0);
 
  //retrieve all from workspace 
  TFile *f=TFile::Open(url);
  RooWorkspace *w=(RooWorkspace *)f->Get("MaxLikelihoodFitResult");
  //  w->Print();
  RooAbsData*	data=w->data("data_obs");
  RooAbsPdf*	eepdf=w->pdf("pdf_bineeeq0jets");
  RooAbsPdf*	mumupdf=w->pdf("pdf_binmumueq0jets");
  RooRealVar *mu=w->var("r");
  mu->setRange(0.5,2.5);
  f->Close();


  //draw the likelihoods
  TCanvas *c=new TCanvas("c","c",600,600);
  c->cd();
  RooPlot *frame = mu->frame(Title("LL and profileLL in signal strength"),Bins(1000)) ;
  
  //build the likelihoods
  RooAbsReal* eenll = eepdf.createNLL(*data) ;
  RooMinuit(*eenll).migrad() ;
  eenll->plotOn(frame,LineColor(kGreen),LineStyle(kDashed),ShiftToZero(),Name("ee"));

  RooAbsReal* mumunll = mumupdf.createNLL(*data) ;
  RooMinuit(*mumunll).migrad() ;
  mumunll->plotOn(frame,LineColor(kRed),LineStyle(kDashed),ShiftToZero(),Name("mumu"));

  RooArgSet llSet; llSet.add((RooNLLVar &)(*eenll)); llSet.add((RooNLLVar &)(*mumunll));
  RooAddition *combll = new RooAddition("combll","combll",llSet);        
  RooMinuit(*combll).migrad();
  combll->plotOn(frame,LineWidth(3),ShiftToZero(),Name("combined"));

  frame->Draw(); 
  frame->GetXaxis()->SetTitle("#sigma / #sigma_{th.}^{NLO}"); frame->GetXaxis()->SetTitleSize(0.05); frame->GetXaxis()->SetTitleOffset(1.0);
  frame->GetYaxis()->SetTitle("- ln (L/L_{max})");      frame->GetYaxis()->SetTitleSize(0.05); frame->GetYaxis()->SetTitleOffset(1.0);
  
  TLegend *leg=new TLegend(0.2,0.8,0.8,0.9);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetNColumns(3);
  leg->SetHeader("CMS preliminary");
  leg->AddEntry("ee","ee","l");
  leg->AddEntry("mumu","#mu#mu","l");
  leg->AddEntry("combined","Combined","l");
  leg->Draw("same");

  c->SaveAs("LikelihoodFit.png");
  c->SaveAs("LikelihoodFit.pdf");
}
