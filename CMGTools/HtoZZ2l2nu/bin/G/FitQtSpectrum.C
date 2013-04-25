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
  smoothWgtGr->SetName(wgtGr->GetName());
  smoothWgtGr->SetTitle(wgtGr->GetTitle());
  smoothWgtGr->SetFillColor(wgtGr->GetFillColor());
  smoothWgtGr->SetFillStyle(wgtGr->GetFillStyle());
  smoothWgtGr->SetLineColor(wgtGr->GetLineColor());
  smoothWgtGr->SetLineStyle(wgtGr->GetLineStyle());
  smoothWgtGr->SetMarkerColor(wgtGr->GetMarkerColor());
  smoothWgtGr->SetMarkerStyle(wgtGr->GetMarkerStyle());
  wgtGr->SetName(wgtGr->GetName()+TString("raw"));
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
      //      p->SetLogy();
      target->Draw(); 
      target->GetXaxis()->SetRangeUser(50,1000);
      target->GetXaxis()->SetTitleSize(0.07);
      target->GetXaxis()->SetTitleOffset(0.8);
      target->GetXaxis()->CenterTitle(true);
      target->GetXaxis()->SetLabelSize(0.06);
      target->GetYaxis()->SetTitleSize(0.06);
      target->GetYaxis()->SetTitleOffset(1.0);
      target->GetYaxis()->SetLabelSize(0.06);

      /*
      targetParamHighPt->SetLineColor(kGray);
      targetParamHighPt->Draw("lsame"); 
      targetParamLowPt->SetLineStyle(2);
      targetParamLowPt->SetLineColor(kGray);
      targetParamLowPt->Draw("lsame");  
      regParam->Draw("l");  
      */
      TPaveText *pave = new TPaveText(0.6,0.9,0.9,0.65,"brNDC");
      pave->SetFillStyle(0);
      pave->SetBorderSize(0);
      pave->AddText(target->GetTitle());
      pave->SetTextFont(42);
      pave->SetTextAlign(11);
      pave->SetTextSize(0.08);
      /*
      sprintf(buf,"#chi^{2}/ndf: %3.0f / %3.0f",chi2,ndf);
      pave->AddText(buf);
      sprintf(buf,"Prob: %3.2f",TMath::Prob(chi2,ndf));
      pave->AddText(buf);
      */
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
enum KinMode     { PT,    ET };
enum MCMode      { ALL,   PUREG };
void FitQtSpectrum(TString url="plotter.root", TString gUrl="plotter_gamma.root", int mode=NOFIT,int kinMode=PT, int mcMode=PUREG)
{
  std::vector<TString> categs,titles,mcg;
  categs.push_back("mjjq016"); titles.push_back("M_{jj}<250");
  categs.push_back("mjjq033"); titles.push_back("250<M_{jj}<350");
  categs.push_back("mjjq049"); titles.push_back("350<M_{jj}<450");
  categs.push_back("mjjq066"); titles.push_back("450<M_{jj}<550");
  categs.push_back("mjjq083"); titles.push_back("550<M_{jj}<750");
  categs.push_back("mjjq092"); titles.push_back("M_{jj}>750");
  /*
  categs.push_back("eq0jets");  titles.push_back("=0 jets");
  categs.push_back("eq1jets");  titles.push_back("=1 jets");
  categs.push_back("eq2jets");  titles.push_back("=2 jets");
  categs.push_back("geq3jets"); titles.push_back("#geq 3 jets");
  categs.push_back("vbf");      titles.push_back("VBF");
  */
  const size_t ncategs=categs.size();
  
  //mc for closure
  if(mcMode!=PUREG)
    {
      mcg.push_back("V#gamma");
      mcg.push_back("Multijets");
    }
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
      TH1F *eemass=(TH1F *) fIn->Get(llDir+"/ee"+categs[icat]+"_qmass");      
      if(eemass){
	eemass->SetName("ee"+categs[icat]+"_zmass");
	eemass->SetDirectory(0);
	toSave.Add(eemass);
      }

      //mass distributions
      TH1F *mmmass=(TH1F *) fIn->Get(llDir+"/mumu"+categs[icat]+"_qmass");      
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
	  if(mmqt) mmqt->Add( (TH1F *)fIn->Get(llDir+"/mumumjjq100_"+llVar) );
	  if(eeqt) eeqt->Add( (TH1F *)fIn->Get(llDir+"/eemjjq100_"+llVar) );
	  if(gqt) gqt->Add(  (TH1F *)gIn->Get("data (#gamma)/mumumjjq100_qt") );
	}

      if(mmqt)
	{
	  mmqt->SetDirectory(0); 
	  mmqt->SetName("mm"+categs[icat]);   
	  mmqt->SetTitle("#mu#mu,"+titles[icat]);
	  mmqt->Rebin();
	}
      if(eeqt)
	{
	  eeqt->SetDirectory(0); 
	  eeqt->SetName("ee"+categs[icat]);   
	  eeqt->SetTitle("ee,"+titles[icat]);
	  eeqt->Rebin();
	}
      if(gqt)
	{
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
		mcgqt->Add(  (TH1F *)gIn->Get(mcg[imcg]+"/mumumjjq100_qt") );
	    }
	}
      if(mcmmqt)
	{
	  mcmmqt->SetDirectory(0);
	  mcmmqt->Rebin();
	}
      if(mceeqt)
	{
	  mceeqt->SetDirectory(0);
	  mceeqt->Rebin();
	}
      if(mcgqt)
	{
	  mcgqt->SetDirectory(0);
	  mcgqt->Rebin();
	}
      
      fIn->Close();
      gIn->Close();
      
      //
      // PHOTON DATA -> REWEIGHT TO DILEPTON DATA
      //
      if(gqt)
	{      
	  c->cd();
	  TPad *p=(TPad *)c->cd(icat+1);      
	  p->Divide(1,3);

	  TGraph *regmmqt=0,*regeeqt=0,*reggqt=0;
	  if(mmqt) {p->cd(); regmmqt = parametrizeSpectrum(mmqt,(TPad *)p->cd(1));}
	  if(eeqt) {p->cd(); regeeqt = parametrizeSpectrum(eeqt,(TPad *)p->cd(2));}
	  if(gqt)  {p->cd(); reggqt  = parametrizeSpectrum(gqt,(TPad *)p->cd(3));}
	  
	  TGraph *eewgtGr=0,*mmwgtGr=0;
	  TGraph *eerawwgtGr=0,*mmrawwgtGr=0;
	  if(mode==NOFIT || mode ==NOFITSMOOTH)
	    {
	      if(eeqt) eewgtGr = getWeightsRatio(eeqt,gqt,"ee"  +categs[icat]+"_qt_datafitwgts");
	      if(mmqt) mmwgtGr = getWeightsRatio(mmqt,gqt,"mumu"+categs[icat]+"_qt_datafitwgts");
	      if(mode==NOFITSMOOTH)
		{
		  if(eeqt) { eerawwgtGr=eewgtGr; eewgtGr=getWeightsSmooth(eewgtGr); }
		  if(mmqt) { mmrawwgtGr=mmwgtGr; mmwgtGr=getWeightsSmooth(mmwgtGr); }
		}
	    }
	  else if(mode==FITDILEPTON)
	    {
	      if(regeeqt) eewgtGr = getWeightsTH1(regeeqt,gqt,"ee"  +categs[icat]+"_qt_datafitwgts");
	      if(regmmqt) mmwgtGr = getWeightsTH1(regmmqt,gqt,"mumu"+categs[icat]+"_qt_datafitwgts");
	    }
	  else
	    {
	      if(regeeqt) eewgtGr = getWeights(regeeqt,reggqt,"ee"  +categs[icat]+"_qt_datafitwgts");
	      if(regmmqt) mmwgtGr = getWeights(regmmqt,reggqt,"mumu"+categs[icat]+"_qt_datafitwgts");
	    }

	  //check if it will close
	  TH1F *eeClosure=0,*mmClosure=0;
	  if(eeqt)eeClosure=checkClosure(eeqt,gqt,eewgtGr);
	  if(mmqt)mmClosure=checkClosure(mmqt,gqt,mmwgtGr);


	  wc->cd();
	  p=(TPad *) wc->cd(icat+1); 
	  //  p->SetLogy();
	  p->SetLogx();
	  bool fill(false);
	  TGraph *frame=mmwgtGr;
	  if(eewgtGr)    { eewgtGr->Draw("al"); frame=eewgtGr; fill=true;}
	  if(mmwgtGr)    { mmwgtGr->Draw(fill ? "l" : "al"); fill=true; }
	  //	  if(eerawwgtGr) { eerawwgtGr->Draw("p"); eerawwgtGr->SetMarkerStyle(24); }
	  //	  if(mmrawwgtGr) { mmrawwgtGr->Draw("p"); mmrawwgtGr->SetMarkerStyle(24); }
	  if(fill)
	    {
	      frame->GetXaxis()->SetTitle("q_{T} [GeV]");
	      frame->GetYaxis()->SetTitle("Weight");
	      frame->GetXaxis()->SetRangeUser(50,1000);
	      frame->GetYaxis()->SetRangeUser(1e-3,0.1);
	      TLegend *leg=p->BuildLegend(0.2,0.68,0.45,0.88,titles[icat]);
	      leg->SetFillStyle(0);
	      leg->SetBorderSize(0);
	      leg->SetTextFont(42);
	    }

	  ctc->cd();
	  p=(TPad *) ctc->cd(icat+1); 
	  p->SetLogx();
	  fill=false;
	  TH1F *frameH=mmClosure;
	  if(eeClosure)        { eeClosure->Draw("e2"); frameH=eeClosure; fill=true; }
	  else if(mmClosure)   { mmClosure->Draw(fill?"e2same":"e2"); fill=true; }
	  if(fill)
	    {
	      frameH->GetXaxis()->SetTitle("q_{T} [GeV]");
	      frameH->GetYaxis()->SetTitle("Bias");
	      frameH->GetXaxis()->SetRangeUser(50,1000);
	      frameH->GetYaxis()->SetRangeUser(0,2);
	      TLegend *leg=p->BuildLegend(0.2,0.68,0.45,0.88,titles[icat]);
	      leg->SetFillStyle(0);
	      leg->SetBorderSize(0);
	      leg->SetTextFont(42);
	    }

	  if(eewgtGr) toSave.Add(eewgtGr);
	  if(mmwgtGr) toSave.Add(mmwgtGr);
	}

      //
      // MC PHOTON SAMPLES -> REWEIGHT TO DATA (fix me for mc closure test)
      //
      //mceeqt=(TH1F *) eeqt->Clone(TString("mc")+eeqt->GetName());
      //mcmmqt=(TH1F *) mmqt->Clone(TString("mc")+mmqt->GetName());
      if(mcgqt)
	{
	  mcc->cd();
	  TPad *p=(TPad *) mcc->cd(icat+1);
	  p->Divide(1,3);
	  TGraph *regmcmmqt=0,*regmceeqt=0,*regmcgqt=0;
	  
	  if(mcmmqt) regmcmmqt = parametrizeSpectrum(mcmmqt,(TPad *)p->cd(1));
	  if(mceeqt) regmceeqt = parametrizeSpectrum(mceeqt,(TPad *)p->cd(2));
	  if(mcgqt)  regmcgqt  = parametrizeSpectrum(mcgqt,(TPad *)p->cd(3));

	  
	  TGraph *mceewgtGr=0,*mcmmwgtGr=0;
	  TGraph *mceerawwgtGr=0,*mcmmrawwgtGr=0;
	  if(mode==NOFIT || mode==NOFITSMOOTH)
	    {
	      if(mceeqt) mceewgtGr = getWeightsRatio(mceeqt,mcgqt,"ee"  +categs[icat]+"_qt_mcfitwgts");
	      if(mcmmqt) mcmmwgtGr = getWeightsRatio(mcmmqt,mcgqt,"mumu"  +categs[icat]+"_qt_mcfitwgts");
	      if(mode==NOFITSMOOTH)
		{
		  if(mceeqt) { mceerawwgtGr=mceewgtGr; mceewgtGr=getWeightsSmooth(mceewgtGr); }
		  if(mcmmqt) { mcmmrawwgtGr=mcmmwgtGr; mcmmwgtGr=getWeightsSmooth(mcmmwgtGr); }
		}
	    }
	  else if(mode==FITDILEPTON)
	    {
	      if(mceeqt) mceewgtGr = getWeightsTH1(regmceeqt,mcgqt,"ee"  +categs[icat]+"_qt_mcfitwgts");
	      if(mcmmqt) mcmmwgtGr = getWeightsTH1(regmcmmqt,mcgqt,"mumu"  +categs[icat]+"_qt_mcfitwgts");
	    }
	  else
	    {
	      if(mceeqt) mceewgtGr = getWeights(regmceeqt,regmcgqt,"ee"  +categs[icat]+"_qt_mcfitwgts");
	      if(mcmmqt) mcmmwgtGr = getWeights(regmcmmqt,regmcgqt,"mumu"  +categs[icat]+"_qt_mcfitwgts");
	    }
	  
	  TH1F *mceeClosure=0,*mcmmClosure=0;
	  if(mceeClosure)mceeClosure=checkClosure(mceeqt,mcgqt,mceewgtGr);
	  if(mcmmClosure)mcmmClosure=checkClosure(mcmmqt,mcgqt,mcmmwgtGr);

	  mcwc->cd();
	  p=(TPad *) mcwc->cd(icat+1);	  
	  //	  p->SetLogy();
	  p->SetLogx();
	  bool fill(false);
	  TGraph *frame=mcmmwgtGr;
	  if(mceewgtGr) { fill=true; mceewgtGr->Draw("al"); frame=mceewgtGr; }
	  if(mcmmwgtGr) { mcmmwgtGr->Draw(fill ? "l" : "al");  fill=true; }
	  //	  if(mceerawwgtGr) { mceerawwgtGr->Draw("p"); mceerawwgtGr->SetMarkerStyle(24); }
	  //  if(mcmmrawwgtGr) { mcmmrawwgtGr->Draw("p"); mcmmrawwgtGr->SetMarkerStyle(24); }
	  if(fill)
	    {
	      frame->GetXaxis()->SetTitle("q_{T} [GeV]");
	      frame->GetYaxis()->SetTitle("Weight");
	      frame->GetXaxis()->SetRangeUser(50,1000);
	      frame->GetYaxis()->SetRangeUser(1e-3,10);
	      TLegend *leg=p->BuildLegend(0.2,0.68,0.45,0.88,titles[icat]);
	      leg->SetFillStyle(0);
	      leg->SetBorderSize(0);
	      leg->SetTextFont(42);
	    }

	  mcctc->cd();
	  p=(TPad *) mcctc->cd(icat+1); 
	  p->SetLogx();
	  fill=false;
	  TH1F *frameH=mcmmClosure;
	  if(mceeClosure) { mceeClosure->Draw("e2"); frameH=mceeClosure; fill=true; }
	  if(mcmmClosure) { mcmmClosure->Draw(fill?"e2same":"e2");       fill=true; }
	  if(fill){
	    frameH->GetXaxis()->SetTitle("q_{T} [GeV]");
	    frameH->GetYaxis()->SetTitle("Bias");
	    frameH->GetXaxis()->SetRangeUser(50,1000);
	    frameH->GetYaxis()->SetRangeUser(0,2);
	    TLegend *leg=p->BuildLegend(0.2,0.68,0.45,0.88,titles[icat]);
	    leg->SetFillStyle(0);
	    leg->SetBorderSize(0);
	    leg->SetTextFont(42);
	  }

	  if(mceewgtGr) toSave.Add(mceewgtGr);
	  if(mcmmwgtGr) toSave.Add(mcmmwgtGr);
	}
    }
  
  c->SaveAs("qtFit.png");
  c->SaveAs("qtFit.pdf");
  c->SaveAs("qtFit.C");
  mcc->SaveAs("qtFit_mc.png");
  mcc->SaveAs("qtFit_mc.pdf");
  mcc->SaveAs("qtFit_mc.C");
  wc->SaveAs("qtWeights.png");
  wc->SaveAs("qtWeights.pdf");
  wc->SaveAs("qtWeights.C");
  mcwc->SaveAs("qtWeights_mc.png");
  mcwc->SaveAs("qtWeights_mc.pdf");
  mcwc->SaveAs("qtWeights_mc.C");

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
