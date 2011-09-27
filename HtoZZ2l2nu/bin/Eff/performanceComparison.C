/**
   gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
 */

#include <iostream>


#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"

#include "TSystem.h"
#include "TFile.h"

using namespace std;

TObject* GetObjectFromPath(TDirectory* File, std::string Path, bool GetACopy=false)
{
   TObject* toReturn = NULL;

   size_t pos = Path.find("/");
   if(pos < std::string::npos){
      std::string firstPart = Path.substr(0,pos);
      std::string endPart   = Path.substr(pos+1,Path.length());
      TDirectory* TMP = (TDirectory*)File->Get(firstPart.c_str());
      toReturn =  GetObjectFromPath(TMP,endPart,GetACopy);
   }else{
      toReturn = File->Get(Path.c_str());
   }

   if(!toReturn)printf("BUG: %s\n",Path.c_str());

   if(GetACopy){ toReturn = toReturn->Clone();  }
   return toReturn;
}


//
TGraphAsymmErrors *getEfficiencyCurve(TH1 *signalProc, TH1 *bckgProc, bool Reverse=false)
{ 
  TGraphAsymmErrors *effGr = new TGraphAsymmErrors;
	      
  //compute efficiency
  const int nbins=bckgProc->GetXaxis()->GetNbins();
  int start = 1; int end=nbins; int step=1;
//  if(Reverse){start=nbins; end=0; step=-1;}

  double bckgTotalError(0), signalTotalError(0);
  double bckgTotal=bckgProc->IntegralAndError(1,nbins,bckgTotalError,"width");
  double signalTotal=signalProc->IntegralAndError(1,nbins,signalTotalError,"width");

  for(int xbin=start; xbin<=end; xbin+=step)
    {
      Double_t signalPassError(0), bckgPassError(0);
      float signalPass, bckgPass;     

      if(!Reverse){
         signalPass = signalProc->IntegralAndError(xbin,nbins,signalPassError,"width");
         bckgPass   = bckgProc->IntegralAndError(xbin,nbins,bckgPassError,"width"); 
      }else{
         signalPass = signalProc->IntegralAndError(1, xbin,signalPassError,"width");
         bckgPass   = bckgProc->IntegralAndError(1, xbin,bckgPassError,"width");  
      }
		  
      //efficiency curve
      double effb=float(bckgPass/bckgTotal);
      double effbError=sqrt(pow(bckgPassError*bckgTotal,2)+pow(bckgPass*bckgTotalError,2))/pow(bckgTotal,2);
      double effsig=float(signalPass/signalTotal);
      double effsigError=sqrt(pow(signalPassError*signalTotal,2)+pow(signalPass*signalTotalError,2))/pow(signalTotal,2);

      int Bin = xbin-1;
      effGr->SetPoint(Bin,effsig,effb);
      effGr->SetPointError(Bin,effsigError,effsigError,effbError,effbError);
    }

  return effGr;
}


TGraph* Inverse(TGraph* graph){
   TGraph* toReturn = new TGraph(graph->GetN());
   for(int i=0;i<graph->GetN();i++){
      double x, y;
      graph->GetPoint(i,x,y);
      toReturn->SetPoint(i,y,x);
   }
   return toReturn;
}



//
void performanceComparison(string evcat="mumu", string signal="H(200)", string background="Z-#gamma^{*}+jets#rightarrow ll",  string fname="../../test/plotter.root")
{
  setStyle();
  gStyle->SetOptFit(0);
  gStyle->SetPadTopMargin   (0.05);
  gStyle->SetPadBottomMargin(0.10);
  gStyle->SetPadRightMargin (0.05);
  gStyle->SetPadLeftMargin  (0.15);
  system("mkdir -p Img");

  int colors[]  = {1,  kGreen-3, kGreen+3, kBlue, kBlue, kRed+3, kRed, kRed};
  int styles[]  = {1,  2,        9,        9,     1,     1,      1,    1};
  int markers[] = {20, 24,       21,       25,    22,    24,     20,   24};

  //configure 
  TString meanFitFunc("pol3");
  TString rmsFitFunc("pol3");
  
  // TString names[]={"metvspu",       "minMetvspu",                       "minClusteredMetvspu",     "unclusteredMetvspu",     "superMinMetvspu"};
//   TString titles[]={"E_{T}^{miss}", "min assoc. E_{T}^{miss}(charged)", "min assoc. E_{T}^{miss}", "min clean E_{T}^{miss}", "minimized E_{T}^{miss}"};
  
//  string names[]  ={"met_metvspu",      "met_assocMetvspu",       "met_cleanMetvspu", "met_minAssocChargedMetvspu", "met_minAssocMetvspu", "met_minCleanMetvspu", "met_superMinMetvspu", "met_redMetvspu"};
//  TString titles[] ={"E_{T}^{miss}", "assoc-E_{T}^{miss}", "clean E_{T}^{miss}", "min{E_{T}^{miss},assoc E_{T}^{miss} (charged)}", "min{E_{T}^miss,assoc-E_{T}^{miss})", "min{E_{T}^{miss},clean-E_{T}^{miss}}", "min{E_{T}^{miss},assoc-E_{T}^{miss},clean-E_{T}^{miss},central-E_{T}^{miss}}", "red-E_{T}^{miss}"};

    string names[]  ={"metL_metvspu",      "metL_assocMetvspu",       "metL_cleanMetvspu", "metL_minAssocChargedMetvspu", "metL_minAssocMetvspu", "metL_minCleanMetvspu", "metL_superMinMetvspu", "met_redMetvspu"};
  TString titles[] ={"E_{T}^{miss}", "assoc-E_{T}^{miss}", "clean E_{T}^{miss}", "min{E_{T}^{miss},assoc E_{T}^{miss} (charged)}", "min{E_{T}^miss,assoc-E_{T}^{miss})", "min{E_{T}^{miss},clean-E_{T}^{miss}}", "min{E_{T}^{miss},assoc-E_{T}^{miss},clean-E_{T}^{miss},central-E_{T}^{miss}}", "red-E_{T}^{miss}"};



  //   TString names[]={"njetsvspu","njetsincvspu"};
  //   TString titles[]={"Central jets","All jets"};
  
  int ntouse=sizeof(names)/sizeof(string);

  TFile *f=TFile::Open(fname.c_str());
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
      string name=names[i];
      TString title=titles[i];
      TString idxStr("");
      idxStr += i;

      TH2D *signalProc= (TH2D*) GetObjectFromPath(f, signal    +"#rightarrow VV" + "/"+evcat+"_"+name, true);    
      TH2D *bckgProc=NULL;
      char* BckgSamples = new char [background.size()+1];  strcpy (BckgSamples, background.c_str());
      char* BckgSubSample = strtok (BckgSamples,"|");
      while (BckgSubSample != NULL){
         if(!bckgProc){bckgProc =    (TH2D*)GetObjectFromPath(f, string(BckgSubSample)+"/"+evcat+"_"+name, true);
         }else{        bckgProc->Add((TH2D*)GetObjectFromPath(f, string(BckgSubSample)+"/"+evcat+"_"+name, true));}
         BckgSubSample = strtok (NULL, "|");
      }

      //inclusive efficiency
      int nxbins=bckgProc->GetXaxis()->GetNbins();
      TH1*signalProj = ((TH2 *)signalProc)->ProjectionY("sig_py",1,nxbins);
      fixExtremities(signalProj,true,true);
      TH1*bckgProj   = ((TH2 *)bckgProc)->ProjectionY("bckg_py",1,nxbins);
      fixExtremities(bckgProj,true,true);
      TGraphAsymmErrors *gr = getEfficiencyCurve(signalProj, bckgProj, names[i].find("metL")!=std::string::npos );
      delete signalProj; delete bckgProj;
      gr->SetName((names[i]+"_eff").c_str());
      gr->SetTitle(titles[i]);
      gr->SetLineColor(colors[i]);
      gr->SetLineStyle(styles[i]);
      gr->SetFillStyle(0);
      gr->SetMarkerColor(colors[i]);
      gr->SetMarkerStyle(markers[i]);
      gr->SetLineWidth(2);
      incEffGrList.push_back(gr);

      TGraph* tmpgr = Inverse(gr);
      printf("%35s : Beff=%6.2E --> Seff=%6.2E\n",names[i].c_str(), 1E-2, tmpgr->Eval(1E-2));
      printf("%35s : Beff=%6.2E --> Seff=%6.2E\n",names[i].c_str(), 1E-3, tmpgr->Eval(1E-3));
      printf("%35s : Beff=%6.2E --> Seff=%6.2E\n",names[i].c_str(), 1E-4, tmpgr->Eval(1E-4));
      delete tmpgr;
      



      //efficiency per bins
      std::vector<TGraphAsymmErrors *> ieffGrList;
      for(size_t ibin=0; ibin<effBins.size(); ibin++){ 
	  signalProj = ((TH2 *)signalProc)->ProjectionY("sig_py",effBins[ibin].first,effBins[ibin].second);
	  fixExtremities(signalProj,true,true);
	  bckgProj   = ((TH2 *)bckgProc)->ProjectionY("bckg_py",effBins[ibin].first,effBins[ibin].second);
	  fixExtremities(bckgProj,true,true);

	  //compute efficiency
	  gr = getEfficiencyCurve(signalProj, bckgProj, names[i].find("metL")!=std::string::npos );
         delete signalProj; delete bckgProj;
	  gr->SetName((names[i]+"_eff").c_str());
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
      bckgMean->SetName((names[i]+"_mean").c_str());
      bckgMean->SetTitle(titles[i]);
      bckgMean->SetLineColor(colors[i]);
      bckgMean->SetLineStyle(styles[i]);
      bckgMean->SetFillStyle(0);
      bckgMean->SetMarkerColor(colors[i]);
      bckgMean->SetMarkerStyle(markers[i]);
      bckgMean->SetLineWidth(2);
      bckgMeanList.push_back(bckgMean);

      TGraphAsymmErrors *bckgRMS = new TGraphAsymmErrors;
      bckgRMS->SetName((names[i]+"_rms").c_str());
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
          delete signalProj; delete bckgProj;
	}
      bckgMean->Fit(meanFitFunc,"Q+","",2,nxbins);
      bckgRMS->Fit(rmsFitFunc,"Q+","",2,nxbins);
    }

  //pileup robustness
  TCanvas *puC = new TCanvas("puC", "Pileup robustness", 1000,500);
//getNewCanvas("puC","Pileup robustness",false);
//puC->SetWindowSize(1000,500);
  puC->Divide(2,1);  

  TPad *p=(TPad *)puC->cd(1);
  for(size_t ip=0; ip<bckgMeanList.size(); ip++)    bckgMeanList[ip]->Draw(ip==0 ? "ap" : "p" );
  bckgMeanList[0]->GetXaxis()->SetTitle("Pileup");
  bckgMeanList[0]->GetYaxis()->SetTitle("Average");
  TLegend *leg=p->BuildLegend(0.20, 0.60, 0.60, 0.90);
  formatForCmsPublic(p,leg,"CMS simulation, Z#rightarrow ll",ntouse);

  p=(TPad *)puC->cd(2);
  for(size_t ip=0; ip<bckgRMSList.size(); ip++)    bckgRMSList[ip]->Draw(ip==0 ? "ap" : "p" );
  bckgRMSList[0]->GetXaxis()->SetTitle("Pileup");
  bckgRMSList[0]->GetYaxis()->SetTitle("RMS");

  TString legTit="CMS simulation\\"+TString(signal.c_str());
  legTit.ReplaceAll("H(","m_{H^{0}}=");
  legTit.ReplaceAll(")"," GeV/c^{2}");
  puC->SaveAs("Img/puC.png");
  delete puC;

  //inclusive efficiency
  TCanvas *effC =  new TCanvas("effC", "Inclusive efficiency", 500,500);//getNewCanvas("effC","Inclusive efficiency",false);
//  effC->SetWindowSize(1000,1000);
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
  leg=effC->BuildLegend(0.20, 0.60, 0.60, 0.90);
  formatForCmsPublic(effC,leg,legTit,ntouse);
  effC->SaveAs("Img/effC.png");
  delete effC;

  //efficiency degradation  
  effC = new TCanvas("effpuC", "Differential efficiency", 500*effBins.size(),500);//getNewCanvas("effpuC","Differential efficiency",false);
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
      leg=p->BuildLegend(0.15, 0.60, 0.60, 0.95);
      formatForCmsPublic(p,leg,legTit,ntouse);
    }
    effC->SaveAs("Img/effpuC.png");
    delete effC;

  //another view of efficiency degradation
//  effC = getNewCanvas("effpertypeC","Differential efficiency per type",false);
//  effC->SetWindowSize(400*ntouse,400);
//  effC->Divide(ntouse,1);
  for(int ip=0; ip<ntouse; ip++) 
    {
       effC = new TCanvas("effpertypeC","Differential efficiency per type",500,500);
       effC->SetLogy();
//      p=(TPad *)effC->cd(ip+1);
//      p->SetLogy();
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
      effC->SaveAs((string("Img/effpertypeC_") + names[ip] + ".png").c_str());
      delete effC;
    }

//       if(ip>0) continue;
//       leg=p->BuildLegend();
//       formatForCmsPublic(p,leg,legTit,ntouse);

}

