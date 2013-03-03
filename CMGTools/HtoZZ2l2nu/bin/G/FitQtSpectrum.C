#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TObjArray.h"
#include "TMath.h"
#include "TGraphSmooth.h"

#include <iostream>
#include <vector>
#include <map>

using namespace std;

TGraph *parametrizeSpectrum(TH1F *target, TPad *p);
TGraph *getWeights(TGraph *target, TGraph *ctrl,TString name);
TGraph *getWeightsTH1(TGraph *target, TH1F *ctrl,TString name);
TGraph *getWeightsRatio(TH1F *target, TH1F *ctrl,TString name);
TGraph *getWeightsSmooth(TGraph *wgtGr){
  TGraphSmooth *gs = new TGraphSmooth(wgtGr->GetName()+TString("smooth"));
  //TGraph *smoothWgtGr=gs->Approx(wgtGr,"linear");
  // TGraph *smoothWgtGr=gs->SmoothKern(wgtGr,"normal",2.0);
  //TGraph *smoothWgtGr=gs->SmoothKern(wgtGr,"normal",5.0);
  //TGraph *smoothWgtGr=gs->SmoothLowess(wgtGr,"",0.67);
  //TGraph *smoothWgtGr=gs->SmoothLowess(wgtGr,"",0.2);
  //TGraph *smoothWgtGr=gs->SmoothSuper(wgtGr,"",0,0);
  TGraph *smoothWgtGr=gs->SmoothSuper(wgtGr,"",3);
  smoothWgtGr->SetTitle(wgtGr->GetTitle());
  smoothWgtGr->SetFillColor(wgtGr->GetFillColor());
  smoothWgtGr->SetFillStyle(wgtGr->GetFillStyle());
  smoothWgtGr->SetLineColor(wgtGr->GetLineColor());
  smoothWgtGr->SetLineStyle(wgtGr->GetLineStyle());
  smoothWgtGr->SetMarkerColor(wgtGr->GetMarkerColor());
  smoothWgtGr->SetMarkerStyle(wgtGr->GetMarkerStyle());
  return smoothWgtGr;
}

TH1F *checkClosure(TH1F *target, TH1F *ctrl, TGraph *wgtGr);


//
TGraph *parametrizeSpectrum(TH1F *target,TPad *p)
{
  char buf[1000];

  //instantiate fitting function (Tsallis)
  TString fname(target->GetName());
  //  TF1 *targetParam=new TF1(fname+"fit","[0]*(pow(1.0+[1]*(x-[2])/[3],[3])+[4]*pow(1.0+[5]*(x-[6])/[7],[7]))*x",0,1e3);
  TF1 *targetParamLowPt=new TF1(fname+"lowptfit","[0]*x*pow(1.0+[1]*(x-[3])/[2],[2])",0,1e3);
  targetParamLowPt->SetParLimits(0,0,target->GetMaximum());
  targetParamLowPt->SetParLimits(1,-1,0);
  targetParamLowPt->SetParLimits(2,-15,0);
  targetParamLowPt->SetParLimits(3,40,60);
  target->Fit(targetParamLowPt,"MRQ0","",0,250);

  TF1 *targetParamHighPt=(TF1 *) targetParamLowPt->Clone(fname+"highptfit");
  target->Fit(targetParamHighPt,"MRQ0","",200,1000);
  
  double dymin(1e3),xmin(150.);
  for(double x=50; x<=1000; x+=0.1)
    {
      double dy=fabs(targetParamLowPt->Eval(x)-targetParamHighPt->Eval(x))/targetParamHighPt->Eval(x);
      if(dy>dymin) continue;
      dymin=dy;
      xmin=x;
    }

  //compute the chi2 per range and choose the function which best minimizes it
  float chi2_perRange[]={0,0,0,0};
  float ndf_perRange[]={0,0,0,0};
  for(int ibin=1; ibin<=target->GetXaxis()->GetNbins(); ibin++)
    {
      float s2=pow(target->GetBinError(ibin),2);
      if(s2==0) continue;

      float x=target->GetBinCenter(ibin);
      size_t rangeBin(x<xmin);

      float dy2_lowPt  = pow(target->GetBinContent(ibin)-targetParamLowPt->Eval(x),2);
      float dy2_highPt = pow(target->GetBinContent(ibin)-targetParamHighPt->Eval(x),2);
      
      chi2_perRange[rangeBin]   += dy2_lowPt/s2;  ndf_perRange[rangeBin]   += 1.0; 
      chi2_perRange[rangeBin+2] += dy2_highPt/s2; ndf_perRange[rangeBin+2] += 1.0; 
    }

  double chi2(0.),ndf(0.);
  TF1 *fPerRange[]={0,0};
  for(size_t i=0; i<2; i++)
    {
      if(ndf_perRange[i]==0   || isnan(chi2_perRange[i]) )  { chi2_perRange[i]=1e9;   ndf_perRange[i]=1.0; }
      if(ndf_perRange[i+2]==0 || isnan(chi2_perRange[i+2])) { chi2_perRange[i+2]=1e9; ndf_perRange[i+2]=1.0; }
      
      float reducedChi2_loPt=chi2_perRange[i]/ndf_perRange[i];
      float reducedChi2_hiPt=chi2_perRange[i+2]/ndf_perRange[i+2];
     
      if(reducedChi2_loPt<reducedChi2_hiPt)
	{ 
	  fPerRange[i]=targetParamLowPt;  
	  chi2 += chi2_perRange[i];   
	  ndf += ndf_perRange[i];   
	}
      else                                                           
	{ 
	  fPerRange[i]=targetParamHighPt; 
	  chi2 += chi2_perRange[i+2]; 
	  ndf += ndf_perRange[i+2]; 
	}
    }
  
  //return as a graph
  TGraph *regParam=new TGraph;
  regParam->SetName(fname+TString("_fitreg"));
  for(double x=0; x<=1500; x+=0.5)
    {
      Double_t y= fPerRange[ x<xmin ]->Eval(x);
      regParam->SetPoint(regParam->GetN(),x,y);
    }
  regParam->SetLineWidth(2);
  regParam->SetLineColor(kBlue);


  //show if required
  if(p)
    {
      p->cd();
      p->Clear();
      p->SetLogx();
      p->SetLogy();
      target->Draw(); 
      target->GetXaxis()->SetRangeUser(50,1000);
      target->GetXaxis()->SetTitleSize(0.07);
      target->GetXaxis()->SetTitleOffset(0.8);
      target->GetXaxis()->CenterTitle(true);
      target->GetXaxis()->SetLabelSize(0.06);
      target->GetYaxis()->SetTitleSize(0.06);
      target->GetYaxis()->SetTitleOffset(1.0);
      target->GetYaxis()->SetLabelSize(0.06);

      targetParamHighPt->SetLineColor(kGray);
      targetParamHighPt->Draw("lsame"); 
      targetParamLowPt->SetLineStyle(2);
      targetParamLowPt->SetLineColor(kGray);
      targetParamLowPt->Draw("lsame");  
      regParam->Draw("l");  
      
      TPaveText *pave = new TPaveText(0.6,0.9,0.9,0.65,"brNDC");
      pave->SetFillStyle(0);
      pave->SetBorderSize(0);
      pave->AddText(target->GetTitle());
      pave->SetTextFont(42);
      pave->SetTextAlign(11);
      sprintf(buf,"#chi^{2}/ndf: %3.0f / %3.0f",chi2,ndf);
      pave->AddText(buf);
      sprintf(buf,"Prob: %3.2f",TMath::Prob(chi2,ndf));
      pave->AddText(buf);
      pave->Draw("same");
    }
  return regParam;
}


//
TGraph *getWeights(TGraph *target, TGraph *ctrl,TString name)
{
  TString title("ee");
  int color(1),lw(2),marker(1); 
 if(name.BeginsWith("mumu")) 
   {
     title="#mu#mu";
     color=kGreen;
   }
  
  TGraph *wgtGr=new TGraph; 
  wgtGr->SetTitle(title); 
  wgtGr->SetName(name);
  wgtGr->SetMarkerStyle(marker); 
  wgtGr->SetMarkerColor(color);
  wgtGr->SetLineColor(color);
  wgtGr->SetLineWidth(lw);
  wgtGr->SetFillStyle(0); 
  for(float x=50; x<=1500; x+=1)
    {
      float wgt(0.);
      if(ctrl->Eval(x)>0.001) wgt=target->Eval(x)/ctrl->Eval(x);
      wgtGr->SetPoint(wgtGr->GetN(),x,wgt);
    }

  return wgtGr;
}

//
TGraph *getWeightsTH1(TGraph *target, TH1F *ctrl,TString name)
{
  TString title("ee");
  int color(1),lw(2),marker(1); 
  if(name.BeginsWith("mumu")) 
   {
     title="#mu#mu";
     color=kGreen;
   }
  cout << target << " " << ctrl << " " << name << endl;
  TGraph *wgtGr=new TGraph; 
  wgtGr->SetTitle(title); 
  wgtGr->SetName(name);
  wgtGr->SetMarkerStyle(marker); 
  wgtGr->SetMarkerColor(color);
  wgtGr->SetLineColor(color);
  wgtGr->SetLineWidth(lw);
  wgtGr->SetFillStyle(0); 
  for(int ibin=1; ibin<=ctrl->GetXaxis()->GetNbins(); ibin++)
    {
      float wgt(0.);
      float x=ctrl->GetXaxis()->GetBinCenter(ibin);
      if(ctrl->GetBinContent(ibin)>0.001) wgt=target->Eval(x)/ctrl->GetBinContent(ibin);
      wgtGr->SetPoint(wgtGr->GetN(),x,wgt);
    }

  return wgtGr;
}


//
TGraph *getWeightsRatio(TH1F *target, TH1F *ctrl,TString name)
{
  TString title("ee");
  int color(1),lw(2),marker(1); 
  if(name.BeginsWith("mumu")) 
   {
     title="#mu#mu";
     color=kGreen;
   }

  TH1F *ratio=(TH1F *)target->Clone("ratio");
  ratio->Divide(ctrl);
  TGraph *wgtGr=new TGraph(ratio); 
  wgtGr->SetTitle(title); 
  wgtGr->SetName(name);
  wgtGr->SetMarkerStyle(marker); 
  wgtGr->SetMarkerColor(color);
  wgtGr->SetLineColor(color);
  wgtGr->SetLineWidth(lw);
  wgtGr->SetFillStyle(0); 
  delete ratio;

  return wgtGr;
}

TH1F *checkClosure(TH1F *target, TH1F *ctrl, TGraph *wgtGr)
{
  TString name(target->GetName()+TString("closure"));
  TString title(target->GetTitle());
  int color(1),lw(1),marker(1),fill(3003); 
  if(!title.BeginsWith("ee")) 
    {
     title="#mu#mu";
     color=kGreen;
     fill=3001;
   }
  else
    {
      title="ee";
    }

  TH1F *ctrlWgt=(TH1F *)ctrl->Clone(name);
  ctrlWgt->SetTitle(title);
  for(int ibin=1; ibin<=ctrl->GetXaxis()->GetNbins(); ibin++)
    {
      float x=ctrl->GetBinCenter(ibin);
      float y=ctrl->GetBinContent(ibin)*wgtGr->Eval(x);
      float yerr=ctrl->GetBinError(ibin)*wgtGr->Eval(x);
      ctrlWgt->SetBinContent(ibin,y);
      ctrlWgt->SetBinError(ibin,yerr);
    }
  ctrlWgt->Divide(target);
  ctrlWgt->SetDirectory(0);
  ctrlWgt->SetTitle(title); 
  ctrlWgt->SetName(name);
  ctrlWgt->SetMarkerStyle(marker); 
  ctrlWgt->SetMarkerColor(color);
  ctrlWgt->SetLineColor(color);
  ctrlWgt->SetLineWidth(lw);
  ctrlWgt->SetFillStyle(fill); 
  ctrlWgt->SetFillColor(color);

  return ctrlWgt;
}



//
enum WeightsMode { NOFIT, FITDILEPTON, FITBOTH, NOFITSMOOTH };
enum KinMode     { PT, ET };
void FitQtSpectrum(TString url="plotter.root", TString gUrl="plotter_gamma.root", int mode=NOFIT,int kinMode=PT)
{
  std::vector<TString> categs,titles,mcg;
  categs.push_back("mjjq016"); titles.push_back("<250");
  categs.push_back("mjjq033"); titles.push_back("250-350");
  categs.push_back("mjjq049"); titles.push_back("350-450");
  categs.push_back("mjjq066"); titles.push_back("450-550");
  categs.push_back("mjjq083"); titles.push_back("550-750");
  categs.push_back("mjjq092"); titles.push_back("#geq 750");
  const size_t ncategs=categs.size();
  
  //mc for closure
  //  mcg.push_back("V#gamma");
  //  mcg.push_back("Multijets");
  mcg.push_back("#gamma+jets");
  TString mcdy("Z#rightarrow ll");

  //plots to save in file
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

  TCanvas *ctc=new TCanvas("ctc","ctc",ncategs*400,400);
  ctc->Divide(ncategs,1);

  TCanvas *mcctc=new TCanvas("mcctc","mcctc",ncategs*400,400);
  mcctc->Divide(ncategs,1);
  

  TString llDir("data");
  if(url==gUrl) llDir="data (ll)";
  TString llVar("qt");
  if(kinMode==ET) llVar="et";

  for(size_t icat=0; icat<ncategs; icat++)
    {
      //get histos from file
      TFile *fIn=TFile::Open(url);
      TFile *gIn=TFile::Open(gUrl);

      //mass distribution
      TH1F *eemass=(TH1F *) fIn->Get(llDir+"/ee_qmass");      
      if(eemass){
	eemass->SetName("ee"+categs[icat]+"_zmass");
	eemass->SetDirectory(0);
	toSave.Add(eemass);
      }

      //mass distributions
      TH1F *mmmass=(TH1F *) fIn->Get(llDir+"/mumu_qmass");      
      if(mmmass){
	mmmass->SetName("mumu"+categs[icat]+"_zmass");
	mmmass->SetDirectory(0);
	toSave.Add(mmmass);
      }

      //qt distributions
      TH1F *mmqt = (TH1F *)fIn->Get(llDir+"/mumu"+categs[icat]+"_"+llVar);
      TH1F *eeqt = (TH1F *)fIn->Get(llDir+"/ee"+categs[icat]+"_"+llVar);  
      TH1F *gqt  = (TH1F *)gIn->Get("data (#gamma)/mumu"+categs[icat]+"_qt");
      if(categs[icat]=="mjjq092")
	{
	  mmqt->Add( (TH1F *)fIn->Get(llDir+"/mumumjjq100_"+llVar) );
	  eeqt->Add( (TH1F *)fIn->Get(llDir+"/eemjjq100_"+llVar) );
	  gqt->Add(  (TH1F *)gIn->Get("data (#gamma)/mumumjjq100_qt") );
	}

      if(mmqt && eeqt && gqt)
	{
	  mmqt->SetDirectory(0); 
	  mmqt->SetName("mm"+categs[icat]);   
	  mmqt->SetTitle("#mu#mu,"+titles[icat]);
	  mmqt->Rebin();

	  eeqt->SetDirectory(0); 
	  eeqt->SetName("ee"+categs[icat]);   
	  eeqt->SetTitle("ee,"+titles[icat]);
	  eeqt->Rebin();

	  gqt->SetDirectory(0);  
	  gqt->SetName("gamma"+categs[icat]); 
	  gqt->SetTitle("#gamma,"+titles[icat]);
	  gqt->Rebin();
	}
      
      TH1F *mcgqt=0,*mcmmqt=0,*mceeqt=0;
      for(size_t imcg=0; imcg<mcg.size(); imcg++)
	{
	  TH1F *ih=(TH1F *)fIn->Get(mcdy+"/mumu"+categs[icat]+"_"+llVar);
	  if(ih!=0)
	    {
	      mcmmqt=ih;
	      mcmmqt->SetDirectory(0);
	      mcmmqt->SetName("mcmm"+categs[icat]);
	      mcmmqt->SetTitle("#mu#mu,"+titles[icat]);
	      if(categs[icat]=="mjjq092")
		mcmmqt->Add( (TH1F *)fIn->Get(mcdy+"/mumumjjq100_"+llVar) );
	    }

	  ih=(TH1F *)fIn->Get(mcdy+"/ee"+categs[icat]+"_"+llVar);
	  if(ih!=0)
	    {
	      mceeqt=(TH1F *)ih->Clone("mcee"+categs[icat]);
	      mceeqt->SetDirectory(0);
	      mceeqt->SetTitle("ee,"+titles[icat]);
	      if(categs[icat]=="mjjq092")
		mceeqt->Add( (TH1F *)fIn->Get(mcdy+"/eemjjq100_"+llVar) );
	    }

	  ih  = (TH1F *)gIn->Get(mcg[imcg]+"/mumu"+categs[icat]+"_qt");
	  if(ih!=0)
	    {
	      if(mcgqt==0)
		{
		  mcgqt=ih;
		  mcgqt->SetDirectory(0);  
		  mcgqt->SetName("mcgamma"+categs[icat]); 
		  mcgqt->SetTitle("#gamma,"+titles[icat]);
		}
	      else
		mcgqt->Add(ih);
	      if(categs[icat]=="mjjq092")
		mcgqt->Add(  (TH1F *)gIn->Get("data (#gamma)/mumumjjq100_qt") );
	    }
	}
      if(mcgqt && mcmmqt && mceeqt)
	{
	  mcmmqt->SetDirectory(0);
	  mcmmqt->Rebin();

	  mceeqt->SetDirectory(0);
	  mceeqt->Rebin();

	  mcgqt->SetDirectory(0);
	  mcgqt->Rebin();
	}
      
      fIn->Close();
      gIn->Close();
      
      //
      // PHOTON DATA -> REWEIGHT TO DILEPTON DATA
      //
      if(mmqt && eeqt && gqt)
	{      
	  c->cd();
	  TPad *p=(TPad *)c->cd(icat+1);      
	  p->Divide(1,3);

	  p->cd(); TGraph *regmmqt = parametrizeSpectrum(mmqt,(TPad *)p->cd(1));
	  p->cd(); TGraph *regeeqt = parametrizeSpectrum(eeqt,(TPad *)p->cd(2));
	  p->cd(); TGraph *reggqt  = parametrizeSpectrum(gqt,(TPad *)p->cd(3));
    
	  TGraph *eewgtGr=0,*mmwgtGr=0;
	  if(mode==NOFIT || mode ==NOFITSMOOTH)
	    {
	      eewgtGr = getWeightsRatio(eeqt,gqt,"ee"  +categs[icat]+"_qt_datafitwgts");
	      mmwgtGr = getWeightsRatio(mmqt,gqt,"mumu"+categs[icat]+"_qt_datafitwgts");
	      if(mode==NOFITSMOOTH)
		{
		  eewgtGr=getWeightsSmooth(eewgtGr);
		  mmwgtGr=getWeightsSmooth(mmwgtGr);
		}
	    }
	  else if(mode==FITDILEPTON)
	    {
	      eewgtGr = getWeightsTH1(regeeqt,gqt,"ee"  +categs[icat]+"_qt_datafitwgts");
	      mmwgtGr = getWeightsTH1(regmmqt,gqt,"mumu"+categs[icat]+"_qt_datafitwgts");
	    }
	  else
	    {
	      eewgtGr = getWeights(regeeqt,reggqt,"ee"  +categs[icat]+"_qt_datafitwgts");
	      mmwgtGr = getWeights(regmmqt,reggqt,"mumu"+categs[icat]+"_qt_datafitwgts");
	    }

	  TH1F *eeClosure=checkClosure(eeqt,gqt,eewgtGr);
	  TH1F *mmClosure=checkClosure(mmqt,gqt,mmwgtGr);


	  wc->cd();
	  p=(TPad *) wc->cd(icat+1); 
	  p->SetLogy();
	  p->SetLogx();
	  eewgtGr->Draw("al");
	  mmwgtGr->Draw("l");
	  eewgtGr->GetXaxis()->SetTitle("q_{T} [GeV]");
	  eewgtGr->GetYaxis()->SetTitle("Weight");
	  eewgtGr->GetXaxis()->SetRangeUser(50,1000);
	  eewgtGr->GetYaxis()->SetRangeUser(1e-3,10);
	  TLegend *leg=p->BuildLegend(0.2,0.68,0.45,0.88,titles[icat]);
	  leg->SetFillStyle(0);
	  leg->SetBorderSize(0);
	  leg->SetTextFont(42);
	  toSave.Add(eewgtGr);
	  toSave.Add(mmwgtGr);

	  ctc->cd();
	  p=(TPad *) ctc->cd(icat+1); 
	  p->SetLogx();
	  eeClosure->Draw("e2");      eeClosure->Fit("pol1","QM"); 
	  mmClosure->Fit("pol1","QM","same");
	  eeClosure->GetXaxis()->SetTitle("q_{T} [GeV]");
	  eeClosure->GetYaxis()->SetTitle("Bias");
	  eeClosure->GetXaxis()->SetRangeUser(50,1000);
	  eeClosure->GetYaxis()->SetRangeUser(0,2);
	  leg=p->BuildLegend(0.2,0.68,0.45,0.88,titles[icat]);
	  leg->SetFillStyle(0);
	  leg->SetBorderSize(0);
	  leg->SetTextFont(42);
	}

      //
      // MC PHOTON SAMPLES -> REWEIGHT TO DATA (fix me for mc closure test)
      //
      //mceeqt=(TH1F *) eeqt->Clone(TString("mc")+eeqt->GetName());
      //mcmmqt=(TH1F *) mmqt->Clone(TString("mc")+mmqt->GetName());
      if(mcgqt && mceeqt && mcmmqt)
	{
	  mcc->cd();
	  TPad *p=(TPad *) mcc->cd(icat+1);
	  p->Divide(1,3);

	  TGraph *regmcmmqt = parametrizeSpectrum(mcmmqt,(TPad *)p->cd(1));
	  TGraph *regmceeqt = parametrizeSpectrum(mceeqt,(TPad *)p->cd(2));
	  TGraph *regmcgqt  = parametrizeSpectrum(mcgqt,(TPad *)p->cd(3));
	  
	  TGraph *mceewgtGr=0,*mcmmwgtGr=0;
	  if(mode==NOFIT || mode==NOFITSMOOTH)
	    {
	      mceewgtGr = getWeightsRatio(mceeqt,mcgqt,"ee"  +categs[icat]+"_qt_mcfitwgts");
	      mcmmwgtGr = getWeightsRatio(mcmmqt,mcgqt,"mumu"  +categs[icat]+"_qt_mcfitwgts");
	      if(mode==NOFITSMOOTH)
		{
		  mceewgtGr=getWeightsSmooth(mceewgtGr);
		  mcmmwgtGr=getWeightsSmooth(mcmmwgtGr);
		}
	    }
	  else if(mode==FITDILEPTON)
	    {
	      mceewgtGr = getWeightsTH1(regmceeqt,mcgqt,"ee"  +categs[icat]+"_qt_mcfitwgts");
	      mcmmwgtGr = getWeightsTH1(regmcmmqt,mcgqt,"mumu"  +categs[icat]+"_qt_mcfitwgts");
	    }
	  else
	    {
	      mceewgtGr = getWeights(regmceeqt,regmcgqt,"ee"  +categs[icat]+"_qt_mcfitwgts");
	      mcmmwgtGr = getWeights(regmcmmqt,regmcgqt,"mumu"  +categs[icat]+"_qt_mcfitwgts");
	    }
	  
	  TH1F *mceeClosure=checkClosure(mceeqt,mcgqt,mceewgtGr);
	  TH1F *mcmmClosure=checkClosure(mcmmqt,mcgqt,mcmmwgtGr);

	  mcwc->cd();
	  p=(TPad *) mcwc->cd(icat+1);	  
	  p->SetLogy();
	  p->SetLogx();
	  mceewgtGr->Draw("al");
	  mcmmwgtGr->Draw("l"); 
	  mceewgtGr->GetXaxis()->SetTitle("q_{T} [GeV]");
	  mceewgtGr->GetYaxis()->SetTitle("Weight");
	  mceewgtGr->GetXaxis()->SetRangeUser(50,1000);
	  mceewgtGr->GetYaxis()->SetRangeUser(1e-3,10);
	  TLegend *leg=p->BuildLegend(0.2,0.68,0.45,0.88,titles[icat]);
	  leg->SetFillStyle(0);
	  leg->SetBorderSize(0);
	  leg->SetTextFont(42);
	  toSave.Add(mceewgtGr);
	  toSave.Add(mcmmwgtGr);
	  
	  mcctc->cd();
	  p=(TPad *) mcctc->cd(icat+1); 
	  p->SetLogx();
	  mceeClosure->Draw("e2");      mceeClosure->Fit("pol1","QM");
	  mcmmClosure->Fit("pol1","QM","same"); 
	  mceeClosure->GetXaxis()->SetTitle("q_{T} [GeV]");
	  mceeClosure->GetYaxis()->SetTitle("Bias");
	  mceeClosure->GetXaxis()->SetRangeUser(50,1000);
	  mceeClosure->GetYaxis()->SetRangeUser(0,2);
	  leg=p->BuildLegend(0.2,0.68,0.45,0.88,titles[icat]);
	  leg->SetFillStyle(0);
	  leg->SetBorderSize(0);
	  leg->SetTextFont(42);
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
