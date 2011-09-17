/**
   gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
 */

#include <iostream>


#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"

#include "TSystem.h"
#include "TFile.h"

using namespace std;

//
TGraphAsymmErrors *getEfficiencyCurve(TH1 *signalProc, TH1 *bckgProc)
{ 
  TGraphAsymmErrors *effGr = new TGraphAsymmErrors;
	      
  //compute efficiency
  const int nbins=bckgProc->GetXaxis()->GetNbins();
  double bckgTotalError(0), signalTotalError(0);
  double bckgTotal=bckgProc->IntegralAndError(1,nbins,bckgTotalError,"width");
  double signalTotal=signalProc->IntegralAndError(1,nbins,signalTotalError,"width");
  for(int xbin=1; xbin<=nbins; xbin++)
    {
      Double_t signalPassError(0), bckgPassError(0);
      float signalPass = signalProc->IntegralAndError(xbin,nbins,signalPassError,"width");
      float bckgPass   = bckgProc->IntegralAndError(xbin,nbins,bckgPassError,"width"); 
		  
      //efficiency curve
      double effb=float(bckgPass/bckgTotal);
      double effbError=sqrt(pow(bckgPassError*bckgTotal,2)+pow(bckgPass*bckgTotalError,2))/pow(bckgTotal,2);
      double effsig=float(signalPass/signalTotal);
      double effsigError=sqrt(pow(signalPassError*signalTotal,2)+pow(signalPass*signalTotalError,2))/pow(signalTotal,2);
      effGr->SetPoint(xbin-1,effsig,effb);
      effGr->SetPointError(xbin-1,effsigError,effsigError,effbError,effbError);
    }

  return effGr;
}

//
void comparePerformance(TString evcat="mumu", TString mpoint="H(200)",  TString fname="plotter.root")
{
  setStyle();
  gStyle->SetOptFit(0);

  int colors[]  = {1,  kGreen-3, kGreen+3, kBlue, kBlue, kRed+3, kRed};
  int styles[]  = {1,  2,        9,        9,     1,     1,      1};
  int markers[] = {20, 24,       21,       25,    22,    24,     20};

  //configure 
  TString bckgName("#gamma");  
  TString meanFitFunc("pol3");
  TString rmsFitFunc("pol3");
  
  // TString names[]={"metvspu",       "minMetvspu",                       "minClusteredMetvspu",     "unclusteredMetvspu",     "superMinMetvspu"};
//   TString titles[]={"E_{T}^{miss}", "min assoc. E_{T}^{miss}(charged)", "min assoc. E_{T}^{miss}", "min clean E_{T}^{miss}", "minimized E_{T}^{miss}"};
  
  TString names[]  ={"metvspu",      "assocMetvspu",       "assocOtherVertexMetvspu",        "cleanMetvspu"};
  TString titles[] ={"E_{T}^{miss}", "assoc-E_{T}^{miss}", "assoc-E_{T}^{miss} (other vtx)", "clean E_{T}^{miss}"};

  //   TString names[]={"njetsvspu","njetsincvspu"};
  //   TString titles[]={"Central jets","All jets"};
  
  int ntouse=sizeof(names)/sizeof(TString);


  TFile *f=TFile::Open(fname);
  std::vector<TGraphAsymmErrors*> bckgMeanList, bckgRMSList;
  std::vector<TGraphAsymmErrors *> incEffGrList;
  std::vector< std::vector<TGraphAsymmErrors *> > effGrList; 
  std::vector<std::pair<int,int> > effBins;
  effBins.push_back(std::pair<int,int>(1,3));
  effBins.push_back(std::pair<int,int>(4,6));  
  effBins.push_back(std::pair<int,int>(7,10));
  effBins.push_back(std::pair<int,int>(10,25));
  for(int i=0; i<ntouse; i++)
    {
      TString name=names[i];
      TString title=titles[i];
      TString idxStr("");
      idxStr += i;
      
      TH2D *bckgProc=0;
      TH2D *signalProc=0;
      TIter next(f->GetListOfKeys());
      TObject *obj=0;
      int idir(0);
      while(1)
	{
          idir++;
          obj=next();
          if(obj==0) break;
          TString dirname=obj->GetName();
          TString hname=dirname +"/"+evcat+"_" + name;
          TH1 *h = (TH1 *) f->Get(hname);
          TString tit=h->GetTitle();
          if(tit.Contains(bckgName)) 
	    {
	      bckgProc = (TH2D *) h->Clone("bckg_"+idxStr);
	      bckgProc->SetDirectory(0);
	      cout << " -bckg: " << tit << endl; 
	    }
	   else if(tit.Contains(mpoint))
	    {
	      if(signalProc==0) 
		{
		  signalProc = (TH2D *) h->Clone("signal_"+idxStr);
		  signalProc->SetDirectory(0);
		}
	      else 
		signalProc->Add(h);
	      cout << " -signal: " << tit << endl;  
	    }
	}

      //inclusive efficiency
      int nxbins=bckgProc->GetXaxis()->GetNbins();
      TH1*signalProj = ((TH2 *)signalProc)->ProjectionY("sig_py",1,nxbins);
      fixExtremities(signalProj,true,true);
      TH1*bckgProj   = ((TH2 *)bckgProc)->ProjectionY("bckg_py",1,nxbins);
      fixExtremities(bckgProj,true,true);
      TGraphAsymmErrors *gr = getEfficiencyCurve(signalProj, bckgProj );
      gr->SetName(names[i]+"_eff");
      gr->SetTitle(titles[i]);
      gr->SetLineColor(colors[i]);
      gr->SetLineStyle(styles[i]);
      gr->SetFillStyle(0);
      gr->SetMarkerColor(colors[i]);
      gr->SetMarkerStyle(markers[i]);
      gr->SetLineWidth(2);
      incEffGrList.push_back(gr);

      //efficiency per bins
      std::vector<TGraphAsymmErrors *> ieffGrList;
      for(size_t ibin=0; ibin<effBins.size(); ibin++) 
	{ 
	  signalProj = ((TH2 *)signalProc)->ProjectionY("sig_py",effBins[ibin].first,effBins[ibin].second);
	  fixExtremities(signalProj,true,true);
	  bckgProj   = ((TH2 *)bckgProc)->ProjectionY("bckg_py",effBins[ibin].first,effBins[ibin].second);
	  fixExtremities(bckgProj,true,true);

	  //compute efficiency
	  gr = getEfficiencyCurve(signalProj, bckgProj );
	  gr->SetName(names[i]+"_eff");
	  gr->SetTitle(titles[i]);
	  gr->SetLineColor(colors[i]);
	  gr->SetLineStyle(styles[i]);
	  gr->SetFillStyle(0);
	  gr->SetMarkerColor(colors[i]);
	  gr->SetMarkerStyle(markers[i]);
	  gr->SetLineWidth(2);
	  ieffGrList.push_back( gr );
	}
      effGrList.push_back( ieffGrList );

      //built the performance curves 
      TGraphAsymmErrors *bckgMean = new TGraphAsymmErrors;
      bckgMean->SetName(names[i]+"_mean");
      bckgMean->SetTitle(titles[i]);
      bckgMean->SetLineColor(colors[i]);
      bckgMean->SetLineStyle(styles[i]);
      bckgMean->SetFillStyle(0);
      bckgMean->SetMarkerColor(colors[i]);
      bckgMean->SetMarkerStyle(markers[i]);
      bckgMean->SetLineWidth(2);
      bckgMeanList.push_back(bckgMean);

      TGraphAsymmErrors *bckgRMS = new TGraphAsymmErrors;
      bckgRMS->SetName(names[i]+"_rms");
      bckgRMS->SetTitle(titles[i]);
      bckgRMS->SetLineColor(colors[i]);
      bckgRMS->SetLineStyle(styles[i]);
      bckgRMS->SetFillStyle(0);
      bckgRMS->SetMarkerColor(colors[i]);
      bckgRMS->SetMarkerStyle(markers[i]);
      bckgRMS->SetLineWidth(2);
      bckgRMSList.push_back(bckgRMS);
      for(int ibin=1; ibin<=nxbins; ibin++) 
	{ 
	  signalProj = ((TH2 *)signalProc)->ProjectionY("sig_py",ibin,ibin); 
	  fixExtremities(signalProj,true,true);
	  bckgProj   = ((TH2 *)bckgProc)->ProjectionY("bckg_py",ibin,ibin); 
	  fixExtremities(bckgProj,true,true);

	  //check the evolution for pileup
	  bckgMean->SetPoint(ibin-1,ibin-1,bckgProj->GetMean()); 
	  bckgMean->SetPointError(ibin-1,0,0,bckgProj->GetMeanError(),bckgProj->GetMeanError());
	  
	  bckgRMS->SetPoint(ibin-1,ibin-1,bckgProj->GetRMS()); 
	  bckgRMS->SetPointError(ibin-1,0,0,bckgProj->GetRMSError(),bckgProj->GetRMSError()); 
	}
      bckgMean->Fit(meanFitFunc,"Q+","",2,nxbins);
      bckgRMS->Fit(rmsFitFunc,"Q+","",2,nxbins);
    }

  

  //pileup robustness
  TCanvas *puC = getNewCanvas("puC","Pileup robustness",false);
  puC->SetWindowSize(1000,500);
  puC->Divide(2,1);  

  TPad *p=(TPad *)puC->cd(1);
  for(size_t ip=0; ip<bckgMeanList.size(); ip++)    bckgMeanList[ip]->Draw(ip==0 ? "ap" : "p" );
  bckgMeanList[0]->GetXaxis()->SetTitle("Pileup");
  bckgMeanList[0]->GetYaxis()->SetTitle("Average");
  TLegend *leg=p->BuildLegend();
  formatForCmsPublic(p,leg,"CMS simulation, Z#rightarrow ll",ntouse);

  p=(TPad *)puC->cd(2);
  for(size_t ip=0; ip<bckgRMSList.size(); ip++)    bckgRMSList[ip]->Draw(ip==0 ? "ap" : "p" );
  bckgRMSList[0]->GetXaxis()->SetTitle("Pileup");
  bckgRMSList[0]->GetYaxis()->SetTitle("RMS");


  TString legTit="CMS simulation\\"+mpoint;
  legTit.ReplaceAll("H(","m_{H^{0}}=");
  legTit.ReplaceAll(")"," GeV/c^{2}");
  
  //inclusive efficiency
  TCanvas *effC = getNewCanvas("effC","Inclusive efficiency",false);
  effC->SetWindowSize(500,500);
  effC->cd();
  effC->SetLogy();
  TGraph *effFrame=new TGraph;
  effFrame->SetPoint(0,1e-5,1e-5);
  effFrame->SetPoint(1,1e-5,1.01);
  effFrame->SetPoint(2,1.01,1.01);
  effFrame->SetPoint(3,1.01,1e-5);
  effFrame->SetMarkerStyle(1);
  effFrame->Draw("ap");
  effFrame->GetXaxis()->SetTitle("#varepsilon(signal)");
  effFrame->GetYaxis()->SetTitle("#varepsilon(instr. background)");

  for(size_t ip=0; ip<incEffGrList.size(); ip++)   incEffGrList[ip]->Draw("pl");
  leg=effC->BuildLegend();
  formatForCmsPublic(effC,leg,legTit,ntouse);


  //efficiency degradation  
  effC = getNewCanvas("effpuC","Differential efficiency",false);
  effC->SetWindowSize(400*effBins.size(),400);
  effC->Divide(effBins.size(),1);
  for(size_t ip=0; ip<effBins.size(); ip++) 
    {
      p=(TPad *)effC->cd(ip+1);
      p->SetLogy();
      effFrame->Draw("ap");
      
      TString ybinTitle("");
      ybinTitle += effBins[ip].first;
      ybinTitle += "#leq PU #leq";
      ybinTitle += effBins[ip].second;
      TPaveText *pave = new TPaveText(0.05,0.96,0.5,0.99,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->AddText(ybinTitle);
      pave->Draw("same");

      for(int i=0; i<ntouse; i++) effGrList[i][ip]->Draw("pl");

      if(ip>0) continue;
      leg=p->BuildLegend();
      formatForCmsPublic(p,leg,legTit,ntouse);
    }

  //another view of efficiency degradation
  effC = getNewCanvas("effpertypeC","Differential efficiency per type",false);
  effC->SetWindowSize(400*ntouse,400);
  effC->Divide(ntouse,1);
  for(int ip=0; ip<ntouse; ip++) 
    {
      p=(TPad *)effC->cd(ip+1);
      p->SetLogy();
      effFrame->Draw("ap");
      
      TPaveText *pave = new TPaveText(0.05,0.96,0.5,0.99,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->AddText(effGrList[ip][0]->GetTitle());
      pave->Draw("same");

      //
      for(int np=0; np<effGrList[ip][0]->GetN(); np++)
	{
	  TGraphAsymmErrors *dif_gr=new TGraphAsymmErrors;
	  for(size_t i=0; i<effBins.size(); i++) 
	    {
	      Double_t x(0),y(0);
	      effGrList[ip][i]->GetPoint(np,x,y);
	      dif_gr->SetPoint(i,x,y);
	    }
	  dif_gr->SetLineColor(6);
	  dif_gr->SetLineWidth(3);
	  dif_gr->Draw("l");
	}
      
      incEffGrList[ip]->Draw("pl");
    }

//       if(ip>0) continue;
//       leg=p->BuildLegend();
//       formatForCmsPublic(p,leg,legTit,ntouse);
}

