#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"
#include "TLine.h"

#include <iostream>
#include <vector>

using namespace std;

std::vector<TGraphAsymmErrors *> getEfficiencyCurves(TH1 *b,TH1 *other);

//
void buildSelectionReport(TString url="plotter.root")
{
  TFile *_file0=TFile::Open(url);

  ///
  /// LEPTON EFFICIENCY
  ///
  //TString proc="Z#rightarrow ll";
  TString proc("ggH(600)#rightarrow ZZ");
  bool rebin=false;
  TString stage[]={"","iso"};
  for(int istage=0; istage<2; istage++)
    {
      TString cat(stage[istage]);

      TCanvas *c = new TCanvas("c"+cat,"c"+cat);
      c->SetWindowSize(1500,1000);
      c->Divide(3,2);
      
      for(size_t ilep=0; ilep<2; ilep++)
	{
	  TString lepton(ilep==0?"mu":"e");
	  
	  TPad *p=(TPad *)c->cd(1+ilep*3);
	  TH1 *hmu=(TH1 *)_file0->Get(proc+"/"+lepton+"genpt");
	  if(rebin) hmu->Rebin();
	  TString muids[]={"tight","2011","soft/veto"};
	  if(ilep==1) muids[0]="medium";
	  for(int i=1; i<=3; i++)
	    {
	      TString idStr(""); idStr += i;
	      TH1 *hmuid=(TH1 *)_file0->Get(proc+"/"+lepton+idStr+cat+"pt");
	      if(rebin) hmuid->Rebin();
	      TGraphAsymmErrors *mueff = new TGraphAsymmErrors;
	      mueff->BayesDivide(hmuid,hmu);
	      mueff->SetTitle(muids[i-1]);
	      mueff->SetName(idStr);
	      mueff->SetMarkerStyle(20+i);
	      mueff->SetMarkerColor(i); mueff->SetLineColor(i);
	      mueff->SetFillStyle(0);
	      mueff->Draw(i==1? "ap":"p");
	      mueff->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	      mueff->GetYaxis()->SetTitle("Efficiency");
	      mueff->GetYaxis()->SetRangeUser(0.45,1.05);
	    }
	  TLegend *leg=p->BuildLegend();
	  if(istage==0) leg->SetHeader(ilep==0 ? "Muon id" : "Electron id");
	  if(istage==1) leg->SetHeader(ilep==0 ? "Muon id+iso" : "Electron id+iso");
	  leg->SetFillStyle(0);
	  leg->SetBorderSize(0);
	  leg->SetTextFont(42);
	  
	  c->cd(2+ilep*3);
	  hmu=(TH1 *)_file0->Get(proc+"/"+lepton+"geneta");
	  if(rebin) hmu->Rebin();
	  for(int i=1; i<=3; i++)
	    {
	      TString idStr(""); idStr += i;
	      TH1 *hmuid=(TH1 *)_file0->Get(proc+"/"+lepton+idStr+cat+"eta");
	      if(rebin) hmuid->Rebin();
	      TGraphAsymmErrors *mueff = new TGraphAsymmErrors;
	      mueff->BayesDivide(hmuid,hmu);
	      mueff->SetTitle(muids[i-1]);
	      mueff->SetName(idStr);
	      mueff->SetMarkerStyle(20+i);
	      mueff->SetMarkerColor(i); mueff->SetLineColor(i);
	      mueff->Draw(i==1? "ap":"p");
	      mueff->GetXaxis()->SetTitle("#eta");
	      mueff->GetYaxis()->SetTitle("Efficiency");
	      mueff->GetYaxis()->SetRangeUser(0.45,1.05);
	    }
	  
	  c->cd(3+ilep*3);
	  hmu=(TH1 *)_file0->Get(proc+"/"+lepton+"genpu");
	  if(rebin) hmu->Rebin();
	  for(int i=1; i<=3; i++)
	    {
	      TString idStr(""); idStr += i;
	      TH1 *hmuid=(TH1 *)_file0->Get(proc+"/"+lepton+idStr+cat+"pu");
	      if(rebin) hmuid->Rebin();
	      TGraphAsymmErrors *mueff = new TGraphAsymmErrors;
	      mueff->BayesDivide(hmuid,hmu);
	      mueff->SetTitle(muids[i-1]);
	      mueff->SetName(idStr);
	      mueff->SetMarkerStyle(20+i);
	      mueff->SetMarkerColor(i); mueff->SetLineColor(i);
	      mueff->Draw(i==1? "ap":"p");
	      mueff->GetXaxis()->SetTitle("Pileup interactions");
	      mueff->GetYaxis()->SetTitle("Efficiency");
	      mueff->GetYaxis()->SetRangeUser(0.45,1.05);
	    }
	  	  
	}
    }

  //
  // B-VETO
  //
  TCanvas *c= new TCanvas("cbtag","cbtag",1200,600);
  c->Divide(2,1);
  TString algos[]={"TCHE","JP","CSV"};
  Double_t btagCuts[]={2.0,   0.275, 0.244};
  TString procsToIntegrate[]={"t#bar{t}","Single top",
			      "Z#rightarrow ll", "Z#rightarrow #tau#tau", "W#rightarrow l#nu",
			      "ZZ","WW#rightarrow 2l2#nu","WZ#rightarrow 3l#nu"};
  Bool_t measureAgainstSignal[]={true,true,
				 false,false,false,
				 false,false,false};
  TString signal("ggH(200)#rightarrow ZZ");
  const size_t nProcsToIntegrate=sizeof(procsToIntegrate)/sizeof(TString);
  for(int ialgo=0; ialgo<3; ialgo++)
    {
      Double_t totalBckg(0), totalBckgErr(0);

      TH1 *b=0; 
      TH1*other=0;
      TH1 *inclusive=0;
      for(size_t iproc=0; iproc<nProcsToIntegrate; iproc++)
	{
	  TH1 *ictr=(TH1F *) _file0->Get(procsToIntegrate[iproc]+"/npfjetsbtags"+algos[ialgo]);     
	  TH1 *ib=(TH1F *) _file0->Get(procsToIntegrate[iproc]+"/"+algos[ialgo]+"bpfjetstags");           
	  TH1 *iother=(TH1F *) _file0->Get(procsToIntegrate[iproc]+"/"+algos[ialgo]+"otherpfjetstags");   
	  TH1 *iinclusive=(TH1F *) _file0->Get(procsToIntegrate[iproc]+"/"+algos[ialgo]+"pfjetstags");    
	  if(b==0)
	    {
	      b=(TH1 *)ib->Clone(algos[ialgo]+"b");               b->SetDirectory(0);
	      other=(TH1 *)iother->Clone(algos[ialgo]+"other");   other->SetDirectory(0);
	    }
	  else
	    {
	      b->Add(ib);
	      other->Add(iother);
	    }
	  if(measureAgainstSignal[iproc])
	    {
	      totalBckg     += ictr->GetBinContent(1);
	      totalBckgErr += pow(ictr->GetBinError(1),2);
	      if(inclusive==0) { inclusive=(TH1 *)iinclusive->Clone(algos[ialgo]+"inc"); inclusive->SetDirectory(0); }
	      else             inclusive->Add(iinclusive);
	    }
	}

      c->cd(1);
      std::vector<TGraphAsymmErrors *> effCurves=getEfficiencyCurves(b,other);
      if(ialgo<3)
	{
	  effCurves[0]->Draw(ialgo==0?"ap":"p");
	  effCurves[0]->GetXaxis()->SetTitle("b efficiency");
	  effCurves[0]->GetYaxis()->SetTitle("udscg efficiency");
	  effCurves[0]->SetMarkerStyle(20+ialgo);
	  effCurves[0]->SetMarkerColor(ialgo+1); 
	  effCurves[0]->SetLineColor(ialgo+1);
	  effCurves[0]->SetFillStyle(0);
	  effCurves[0]->SetTitle(algos[ialgo]);
	}

      Double_t mistagrate=effCurves[2]->Eval(btagCuts[ialgo]);
      TLine *l = new TLine(0,mistagrate,1,mistagrate);
      l->SetLineColor(ialgo+1);
      l->Draw();

      c->cd(2);
      TH1 *signalctr=(TH1F *) _file0->Get(signal+"/"+"npfjetsbtags"+algos[ialgo]);
      Double_t totalSig=signalctr->GetBinContent(1);
      Double_t totalSigErr=signalctr->GetBinError(1);
      totalBckgErr = sqrt(totalBckgErr);
      cout << algos[ialgo] << " | " 
	   << totalBckg << " +/- " << totalBckgErr << " | "
	   << totalSig << " +/- " << totalSigErr << " | "
	   << totalSig/totalBckg << " +/- " << sqrt(pow(totalSig*totalBckgErr,2)+pow(totalBckg*totalSigErr,2))/pow(totalBckg,2) << endl;
      TH1 *signalinclusive=(TH1F *) _file0->Get(signal+"/"+algos[ialgo]+"pfjetstags");
      effCurves=getEfficiencyCurves(signalinclusive,inclusive);
      if(ialgo<3)
	{
	  effCurves[0]->Draw(ialgo==0?"ap":"p");
	  effCurves[0]->GetXaxis()->SetTitle(signal+" mistag");
	  effCurves[0]->GetYaxis()->SetTitle("Top background efficiency");
	  effCurves[0]->SetMarkerStyle(20+ialgo);
	  effCurves[0]->SetMarkerColor(ialgo+1); 
	  effCurves[0]->SetLineColor(ialgo+1);
	  effCurves[0]->SetFillStyle(0);
	  effCurves[0]->SetTitle(algos[ialgo]);
	}

      mistagrate=effCurves[2]->Eval(btagCuts[ialgo]);
      l = new TLine(0,mistagrate,1,mistagrate);
      l->SetLineColor(ialgo+1);
      l->Draw();
    }
  TPad *p=(TPad *)c->cd(1);
  TLegend *leg=p->BuildLegend();
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  




  //
  // 3^rd LEPTON VETO
  //


  //
  // JET COUNTING STABILITY
  //



  _file0->Close();
}


//
std::vector<TGraphAsymmErrors *> getEfficiencyCurves(TH1 *b,TH1 *other)
{
  TGraphAsymmErrors *roc=new TGraphAsymmErrors;
  TGraphAsymmErrors *bgr=new TGraphAsymmErrors;
  TGraphAsymmErrors *othergr=new TGraphAsymmErrors;
  
  Int_t nbins(b->GetXaxis()->GetNbins());

  //normalization of the curves
  Double_t totalberr(0);
  Double_t totalbint=b->IntegralAndError(1,nbins,totalberr);
  Double_t totalothererr(0);
  Double_t totalotherint=other->IntegralAndError(1,nbins,totalothererr);

  //underflows and overflows are already taken into account the histograms from plotter
  for(int ibin=1; ibin<=nbins; ibin++)
    {
      Double_t cut(b->GetXaxis()->GetBinLowEdge(ibin));
      
      //b-efficiency
      Double_t berr(0);
      Double_t bint=b->IntegralAndError(ibin,nbins,berr);
      Double_t effb=bint/totalbint;
      Double_t effberr=sqrt(pow(bint*totalberr,2)+pow(berr*totalbint,2))/pow(totalbint,2);
      if(effb>0)
	{
	  bgr->SetPoint(bgr->GetN(),cut,effb);
	  bgr->SetPointError(bgr->GetN(),0,0,effberr,effberr);
	}

      //other efficiency
      Double_t othererr(0);
      Double_t otherint=other->IntegralAndError(ibin,nbins,othererr);
      Double_t effother=otherint/totalotherint;
      Double_t effothererr=sqrt(pow(bint*totalothererr,2)+pow(berr*totalotherint,2))/pow(totalotherint,2);
      if(effother>0)
	{
	  othergr->SetPoint(othergr->GetN(),cut,effother);
	  othergr->SetPointError(othergr->GetN(),0,0,effothererr,effothererr);
	}
      //ROC curve
      if(effb>0 && effother>0)
	{
	  roc->SetPoint(roc->GetN(),effb,effother);
	  roc->SetPointError(roc->GetN(),effberr,effberr,effothererr,effothererr);
	}
    }
  
  //all done
  std::vector<TGraphAsymmErrors *> res;
  res.push_back(roc);
  res.push_back(bgr);
  res.push_back(othergr);
  return res;
}
