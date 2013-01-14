#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TObjArray.h"

#include <iostream>
#include <vector>
#include <map>

using namespace std;

TF1 *getQtParam(std::vector<int> &thr,TString fname, TH1 *qtH);
TGraph *getRegularizedQtParam(TF1 *qtParam,TH1F *qt, std::vector<int> &thr);

//
TF1 *getQtParam(std::vector<int> &thr,TString fname, TH1 *qtH)
{
  TString qtParamForm("");
  const size_t nthr=thr.size();
  for(size_t i=0; i<nthr; i++)
    {
      TString form("");
      if(i==0)            { form += "(x<";   form += thr[i+1]; form += ")"; }
      else if (i<nthr-1)  { form += "+(x>="; form += thr[i]; form += " && x<"; form += thr[i+1]; form += ")"; }
      else                { form += "+(x>="; form += thr[i]; form += ")"; }
      char buf[500];
      sprintf(buf,"*[%lu]*pow(x/[%lu],[%lu])*exp(-x/[%lu])/(1+exp((%d-x)/[%lu]))",i*4,i*4+1,i*4+2,i*4+1,thr[i],i*4+3);
      form += buf;
      qtParamForm += form;
    }

  TF1 *qtParam=new TF1(fname,qtParamForm.Data(),0,1e6);
  for(size_t i=0; i<nthr; i++)
    {
      float maxBinValue=qtH->GetBinContent(qtH->GetXaxis()->FindBin(thr[i+1])); 
      qtParam->SetParLimits(i*4,   maxBinValue/100,          100000*maxBinValue);
      qtParam->SetParLimits(i*4+1, thr[i]*0.9,thr[i]*1.1);
      qtParam->SetParLimits(i*4+2, -10,        0);
      qtParam->SetParLimits(i*4+3, 0.1,        3);
    } 
  return qtParam;
}
//
TGraph *getRegularizedQtParam(TF1 *qtParam,TH1F *qt, std::vector<int> &thr)
{
  std::map<int,float> binsForThr;
  for(size_t i=0; i<thr.size(); i++) 
    {
      int bin0=qt->FindBin(thr[i])-2;  float x0=qt->GetBinCenter(bin0);  float y0=qtParam->Eval(x0);
      int bin1=qt->FindBin(thr[i])-1; float x1=qt->GetBinCenter(bin1);  float y1=qt->GetBinContent(bin1);
      int bin2=qt->FindBin(thr[i]);   float x2=qt->GetBinCenter(bin2);  float y2=qtParam->Eval(x2);

      binsForThr[bin0]   = qtParam->Eval(x0);
      binsForThr[bin1]   = qt->GetBinContent(bin1);
      binsForThr[bin2]   = qtParam->Eval(x1);

      float a=((y2-y0)*(x1-x0)-(y1-y0)*(x2-x0))/( (x2*x2-x0*x0)*(x1-x0)-(x1*x1-x0*x0)*(x2-x0) );
      float b=(-a*(x1*x1-x0*x0)+(y1-y0))/(x1-x0);
      float c=y0-a*x0*x0-b*x0;
      binsForThr[bin0]=a*x0*x0+b*x0+c;
      binsForThr[bin1]=a*x1*x1+b*x1+c;
      binsForThr[bin2]=a*x2*x2+b*x2+c;

    }

  TGraph *gr = new TGraph;
  gr->SetName(TString("reg")+qtParam->GetName());
  for(int ibin=1; ibin<=qt->GetXaxis()->GetNbins(); ibin++)
    {
      Double_t x=qt->GetBinCenter(ibin);
      Double_t y=qtParam->Eval(x); //GetBinContent(ibin);
      if(binsForThr.find(ibin)!=binsForThr.end()) y=binsForThr[ibin];
      gr->SetPoint(gr->GetN(),x,y);
    }
  gr->SetLineWidth(2);
  gr->SetLineColor(kBlue);
  return gr;
}

//
TGraph *steerQtParameterization(TH1F *qt,std::vector<int> &thr,TPad *p)
{
  TF1 *qtParam=getQtParam(thr,p->GetName()+TString("qtparam"),qt);
  qt->Fit(qtParam,"RQ0","",36,1000);
  TGraph *regqt=getRegularizedQtParam(qtParam,qt,thr);
  
  //show in pad
  if(p)
    {
      p->cd();
      p->SetLogx();
      p->SetLogy();
      qt->Draw(); 
      qt->GetXaxis()->SetRangeUser(50,1000);
      regqt->Draw("l");  
      TPaveText *pave = new TPaveText(0.6,0.9,0.9,0.75,"brNDC");
      pave->SetFillStyle(0);
      pave->SetBorderSize(0);
      pave->AddText(qt->GetTitle());
      pave->SetTextFont(42);
      pave->Draw("same");
    }
  
  return regqt;
}


//
enum AnalysisMode {STANDARD, ZZ, VBFZ};
void FitQtSpectrum(TString url="plotter.root", TString gUrl="plotter_gamma.root", int mode=STANDARD,bool is2011=false)
{
  std::vector<TString> categs,titles,mcg;
  if(mode==ZZ){
    categs.push_back("eq0jets");
    titles.push_back("=0jets");
  }
  else{
    categs.push_back("eq0jets"); categs.push_back("eq1jets"); categs.push_back("eq2jets"); categs.push_back("geq3jets");  if(mode==STANDARD) categs.push_back("vbf");
    titles.push_back("=0jets");  titles.push_back("=1jets");  titles.push_back("=2jets");  titles.push_back("#geq3jets"); if(mode==STANDARD) titles.push_back("VBF");
  }
  mcg.push_back("Fakes");
  mcg.push_back("Prompt #gamma");
  TString mcdy("Z#rightarrow ll");


  const size_t ncategs=categs.size();
  TObjArray toSave;

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptTitle(0);

  TCanvas *c= new TCanvas("c","c",ncategs*400,600);
  c->Divide(ncategs,1);

  TCanvas *mcc= new TCanvas("mcc","mcc",ncategs*400,600);
  mcc->Divide(ncategs,1);

  TCanvas *wc=new TCanvas("wc","wc",ncategs*400,400);
  wc->Divide(ncategs,1);

  TCanvas *mcwc=new TCanvas("mcwc","mcwc",ncategs*400,400);
  mcwc->Divide(ncategs,1);

  for(size_t icat=0; icat<ncategs; icat++)
    {
      //get histos from file
      TFile *fIn=TFile::Open(url);
      TFile *gIn=TFile::Open(gUrl);

      //mass distributions
      TH1F *eemass=(TH1F *) fIn->Get("data/ee_zmass");      
      if(mode!=ZZ && eemass){
	eemass->SetName("ee"+categs[icat]+"_zmass");
	eemass->SetDirectory(0);
	toSave.Add(eemass);
      }

      //mass distributions
      TH1F *mmmass=(TH1F *) fIn->Get("data/mumu_zmass");      
      if(mode!=ZZ && mmmass){
	mmmass->SetName("mumu"+categs[icat]+"_zmass");
	mmmass->SetDirectory(0);
	toSave.Add(mmmass);
      }

      //qt distributions
      TH1F *mmqt = (TH1F *)fIn->Get("data/mumu"+categs[icat]+"_qt");    
      TH1F *eeqt = (TH1F *)fIn->Get("data/ee"+categs[icat]+"_qt");  
      TH1F *gqt  = (TH1F *)gIn->Get("data (#gamma)/mumu"+categs[icat]+"_qt");
      if(mmqt && eeqt && gqt)
	{
	  mmqt->SetDirectory(0); 
	  mmqt->SetName("mm"+categs[icat]);   
	  mmqt->SetTitle("#mu#mu,"+titles[icat]);
	 
	  eeqt->SetDirectory(0); 
	  eeqt->SetName("ee"+categs[icat]);   
	  eeqt->SetTitle("ee,"+titles[icat]);
	  
	  gqt->SetDirectory(0);  
	  gqt->SetName("gamma"+categs[icat]); 
	  gqt->SetTitle("#gamma,"+titles[icat]);
	}
      
      TH1F *mcgqt=0,*mcmmqt=0,*mceeqt=0;
      for(size_t imcg=0; imcg<mcg.size(); imcg++)
	{
	  TH1F *ih=(TH1F *)fIn->Get(mcdy+"/mumu"+categs[icat]+"_qt");
	  if(ih!=0)
	    {
	      mcmmqt=ih;
	      mcmmqt->SetDirectory(0);
	      mcmmqt->SetName("mcmm"+categs[icat]);
	      mcmmqt->SetTitle("#mu#mu,"+titles[icat]);
	    }
	  ih=(TH1F *)fIn->Get(mcdy+"/ee"+categs[icat]+"_qt");
	  if(ih!=0)
	    {
	      mceeqt=ih;
	      mceeqt->SetDirectory(0);
	      mceeqt->SetName("mcee"+categs[icat]);
	      mceeqt->SetTitle("ee,"+titles[icat]);
	    }
	  ih  = (TH1F *)gIn->Get(mcg[imcg]+"/mumu"+categs[icat]+"_qt");
	  if(ih==0) continue;
	  mcgqt=ih;
	  mcgqt->SetDirectory(0);  
	  mcgqt->SetName("mcgamma"+categs[icat]); 
	  mcgqt->SetTitle("#gamma,"+titles[icat]);
	}
      
      fIn->Close();
      gIn->Close();

      //perform fits and show results on subpads
      if(mmqt && eeqt && gqt)
	{      
	  c->cd();
	  TPad *p=(TPad *)c->cd(icat+1);      
	  p->Divide(1,3);

	  std::vector<int> thr;	  
	  if(is2011){
	    thr.push_back(40);
	    thr.push_back(50);
	    thr.push_back(75);
	    thr.push_back(90);
	  }
	  else{
	    thr.push_back(50); 
	    thr.push_back(75);
	    if(!categs[icat].Contains("vbf"))
	      {      
		thr.push_back(90); 
		//thr.push_back(135);
		thr.push_back(150); 
		thr.push_back(170); 
		if(!categs[icat].Contains("eq0jets"))
		  {
		    //thr.push_back(250); 
		    thr.push_back(300); 
		  }
	      }
	  }
	  TGraph *regmmqt = steerQtParameterization(mmqt,thr,(TPad *)p->cd(1)); 
	  TGraph *regeeqt = steerQtParameterization(eeqt,thr,(TPad *)p->cd(2));
	  
	  thr.clear();
	  if(is2011){
	    thr.push_back(40);
	    thr.push_back(50);
	    thr.push_back(75);
	    thr.push_back(90);
	    thr.push_back(125);
	    thr.push_back(135);
	    thr.push_back(200);
	  }
	  else{
	    thr.push_back(50); 
	    thr.push_back(80);
	    thr.push_back(90); 
	    thr.push_back(150);
	    thr.push_back(170);
	    if(!categs[icat].Contains("eq0jets") /*&& !categs[icat].Contains("vbf")*/)
	      {
		if(categs[icat].Contains("eq1jets"))  thr.push_back(250); 
		thr.push_back(300); 
	      }
	  }
	  TGraph *reggqt = steerQtParameterization(gqt,thr,(TPad *)p->cd(3));
	  
	  TGraph *eewgtGr=new TGraph; eewgtGr->SetTitle("ee");     eewgtGr->SetName("ee"+categs[icat]+"_qt_datafitwgts");
	  TGraph *mmwgtGr=new TGraph; mmwgtGr->SetTitle("#mu#mu"); mmwgtGr->SetName("mumu"+categs[icat]+"_qt_datafitwgts");
	  for(int ibin=1; ibin<=gqt->GetXaxis()->GetNbins(); ibin++)
	    {
	      float x=gqt->GetBinCenter(ibin);
	      if(reggqt->Eval(x)<0.01) continue;
	      if(x<50) continue;
	      float wgt=regeeqt->Eval(x)/reggqt->Eval(x);
	      eewgtGr->SetPoint(eewgtGr->GetN(),x,wgt);
	      wgt=regmmqt->Eval(x)/reggqt->Eval(x);
	      mmwgtGr->SetPoint(eewgtGr->GetN(),x,wgt);
	    }
	  
	  wc->cd();
	  p=(TPad *) wc->cd(icat+1); 
	  eewgtGr->Draw("al"); eewgtGr->SetMarkerColor(1); eewgtGr->SetFillStyle(0); eewgtGr->SetLineColor(1);      eewgtGr->SetLineWidth(2);
	  mmwgtGr->Draw("l");  mmwgtGr->SetMarkerColor(kGreen); mmwgtGr->SetFillStyle(0); mmwgtGr->SetLineColor(kGreen); mmwgtGr->SetLineWidth(2);
	  eewgtGr->GetXaxis()->SetTitle("q_{T} [GeV]");
	  eewgtGr->GetYaxis()->SetTitle("Weight");
	  eewgtGr->GetXaxis()->SetRangeUser(50,1000);
	  eewgtGr->GetYaxis()->SetRangeUser(0,0.1);
	  TLegend *leg=p->BuildLegend();
	  leg->SetFillStyle(0);
	  leg->SetBorderSize(0);
	  leg->SetTextFont(42);
	  leg->SetHeader(titles[icat]);

	  toSave.Add(eewgtGr);
	  toSave.Add(mmwgtGr);
	}

      //mc based
      if(mcgqt && mceeqt && mcmmqt)
	{
	  mcc->cd();
	  TPad *p=(TPad *) mcc->cd(icat+1);
	  p->Divide(1,3);

	  std::vector<int> thr;
	  thr.clear();
	  thr.push_back(50); 
	  //thr.push_back(80);
	  //thr.push_back(90); 
	  thr.push_back(150);
	  // if( !categs[icat].Contains("eq0jets") ) thr.push_back(250);
	  p->cd(1); mcmmqt->Draw("e1");
	  //TGraph *regmcmmqt = steerQtParameterization(mcmmqt,thr,(TPad *)p->cd(1)); 
	  p->cd(2); mceeqt->Draw("e1");
	  //TGraph *regmceeqt = steerQtParameterization(mceeqt,thr,(TPad *)p->cd(2));
	  thr.clear();
	  thr.push_back(50); 
	  //	  thr.push_back(80);
	  // thr.push_back(90); 
	   thr.push_back(150);
	  //if( categs[icat].Contains("eq0jets") ) thr.push_back(500);
	  //else                                   thr.push_back(300); 
	   p->cd(3); mcgqt->Draw("e1");
	   //TGraph *regmcgqt  = steerQtParameterization(mcgqt,thr,(TPad *)p->cd(3));
	   
	   p=(TPad *) mcwc->cd(icat+1);
	   TH1 *mceewgts=(TH1 *) mceeqt->Clone(); mceewgts->Divide(mcgqt);
	   TGraph *mceewgtGr=new TGraph(mceewgts); mceewgtGr->SetTitle("ee");     mceewgtGr->SetName("ee"+categs[icat]+"_qt_mcfitwgts");
	   TH1 *mcmmwgts=(TH1 *) mcmmqt->Clone(); mcmmwgts->Divide(mcgqt); 
	   TGraph *mcmmwgtGr=new TGraph(mcmmwgts); mcmmwgtGr->SetTitle("#mu#mu"); mcmmwgtGr->SetName("mumu"+categs[icat]+"_qt_mcfitwgts");

	   //TGraph *mceewgtGr=new TGraph; mceewgtGr->SetTitle("ee");     mceewgtGr->SetName("ee"+categs[icat]+"_qt_mcfitwgts");
	   //TGraph *mcmmwgtGr=new TGraph; mcmmwgtGr->SetTitle("#mu#mu"); mcmmwgtGr->SetName("mumu"+categs[icat]+"_qt_mcfitwgts");
	   // for(int ibin=1; ibin<=mcgqt->GetXaxis()->GetNbins(); ibin++)
	   // 	     {
	   // 	       float x=mcgqt->GetBinCenter(ibin);
	   // 	       if(regmcgqt->Eval(x)<0.01) continue;
	   // 	       if(x<50) continue;
	   // 	       float wgt=regmceeqt->Eval(x)/regmcgqt->Eval(x);
	   // 	       mceewgtGr->SetPoint(mceewgtGr->GetN(),x,wgt);
	   // 	       wgt=regmcmmqt->Eval(x)/regmcgqt->Eval(x);
	   // 	       mcmmwgtGr->SetPoint(mcmmwgtGr->GetN(),x,wgt);
	   //}
	   mceewgtGr->Draw("al"); mceewgtGr->SetMarkerColor(1);      mceewgtGr->SetFillStyle(0); mceewgtGr->SetLineColor(1);      mceewgtGr->SetLineWidth(2);
	   mcmmwgtGr->Draw("l");  mcmmwgtGr->SetMarkerColor(kGreen); mcmmwgtGr->SetFillStyle(0); mcmmwgtGr->SetLineColor(kGreen); mcmmwgtGr->SetLineWidth(2);
	   mceewgtGr->GetXaxis()->SetTitle("q_{T} [GeV]");
	   mceewgtGr->GetYaxis()->SetTitle("Weight");
	   mceewgtGr->GetXaxis()->SetRangeUser(50,1000);
	   mceewgtGr->GetYaxis()->SetRangeUser(0,0.1);
	   TLegend *leg=p->BuildLegend();
	   leg->SetFillStyle(0);
	   leg->SetBorderSize(0);
	   leg->SetTextFont(42);
	   leg->SetHeader(titles[icat]);
	   
	   toSave.Add(mceewgtGr);
	   toSave.Add(mcmmwgtGr);
	}
    }
  
  c->SaveAs("qtFit.png");
  c->SaveAs("qtFit.pdf");
  mcc->SaveAs("qtFit_mc.png");
  mcc->SaveAs("qtFit_mc.pdf");
  wc->SaveAs("qtWeights.png");
  wc->SaveAs("qtWeights.pdf");
  mcwc->SaveAs("qtWeights_mc.png");
  mcwc->SaveAs("qtWeights_mc.pdf");


  //save results
  TFile *fOut=TFile::Open("gammawgts.root","RECREATE");
  fOut->cd();
  for(int i=0; i<toSave.GetEntriesFast(); i++)
    {
      if(toSave.At(i)==0) continue;
      toSave.At(i)->Write();
    }
  fOut->Close();


}
