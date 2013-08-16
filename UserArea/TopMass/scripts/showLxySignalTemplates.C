#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLine.h"

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

struct TemplateQuality_t
{
  TString tag;
  double chi2,dmean,dq025,dq05,dq075;
};

bool byTag (TemplateQuality_t i,TemplateQuality_t j) { return (i.tag<j.tag); }


void showLxySignalTemplates(TString url="LxyTemplates.root")
{
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  

  std::map<TString, std::vector<TemplateQuality_t> > results;


  TFile *inF=TFile::Open(url);
  inF->cd();
  TIter next(inF->GetListOfKeys());
  TObject *obj=0;
  while ((obj = next()))
    {
      inF->cd(obj->GetName());
      TIter nnext(gDirectory->GetListOfKeys());
      TObject *oobj=0;
      while ((oobj = nnext()))
	{
	  //
	  TString name=oobj->GetName();
	  TObjArray *tkns=name.Tokenize("_");
	  TString ch=tkns->At(0)->GetName();
	  TString proc=tkns->At(2)->GetName();
	  TString mass=tkns->At(3)->GetName();

	  //save canvas
	  TObject *o =(TObject *)gDirectory->Get(oobj->GetName());
	  if(TString(o->ClassName())!="TCanvas") continue;
	  TCanvas *c=(TCanvas *) o;
	  c->Draw();
	  c->IsBatch();
	  c->SetWindowSize(400,400);
	  c->SetCanvasSize(400,400);
	  c->cd(1)->Delete();
	  TPad *p=(TPad *)c->cd(2);
	  p->SetPad(0,0,1,1);
	  c->Modified();
	  c->Update();
	  c->SaveAs( ch+"_"+proc+"_"+mass+".png" );
	  c->SaveAs( ch+"_"+proc+"_"+mass+".pdf" );
	  

	  //make a summary
	  TH1F *target=(TH1F *) gDirectory->Get(ch+"_bjetoriglxy_"+proc+"_"+mass);
	  TH1F *templ =(TH1F *) gDirectory->Get(ch+"_bjetlxy_"+proc+"_"+mass);
	  TemplateQuality_t res;
	  res.chi2=target->Chi2Test(templ,"WWCHI2/NDF");
	  res.dmean=templ->GetMean()-target->GetMean();
	  const Int_t nq = 4;
	  Double_t xq[nq];  
	  for (Int_t i=0;i<nq;i++) xq[i] = Float_t(i+1)/nq;
	  Double_t templq[nq],targetq[nq];  // array to contain the quantiles
	  target->GetQuantiles(nq,targetq,xq);
	  templ->GetQuantiles(nq,templq,xq);
	  res.dq025 = templq[0]-targetq[0];
	  res.dq05  = templq[1]-targetq[1];
	  res.dq075 = templq[2]-targetq[2];
	  res.tag=mass;
	  if(proc.Contains("mepsup"))   res.tag="ME-PS up";
	  if(proc.Contains("mepsdown")) res.tag="ME-PS down";
	  if(proc.Contains("q2up"))     res.tag="Q^{2} up";
	  if(proc.Contains("q2down"))   res.tag="Q^{2} down";
	  results[ch].push_back(res);
	}
    }

  TCanvas *c = new TCanvas("c","c",800,1000);  
  for(std::map<TString, std::vector<TemplateQuality_t> >::iterator it=results.begin();
      it != results.end();
      it++)
    {
      std::vector<TemplateQuality_t> res=it->second;
      sort(res.begin(),res.end(),byTag);

      size_t nresults=it->second.size();
      TH1F *chi2=new TH1F("chi2h",";;#chi^{2}/ndf",nresults,0,nresults);
      chi2->GetYaxis()->SetNdivisions(10);
      chi2->GetYaxis()->SetTitleOffset(0.5); 
      chi2->GetYaxis()->SetTitleSize(0.1);
      chi2->GetYaxis()->SetLabelSize(0.08);
      chi2->GetXaxis()->SetLabelSize(0.09);
      chi2->GetYaxis()->SetRangeUser(0,5);
      for(size_t i=0; i<nresults; i++) chi2->GetXaxis()->SetBinLabel(i+1, res[i].tag );
      TH1F *dmean=(TH1F *) chi2->Clone("dmeanh");  dmean->GetYaxis()->SetTitle("#Delta <L_{xy}> [cm]");      dmean->GetYaxis()->SetRangeUser(-0.023,0.023);
      TH1F *dq025=(TH1F *) dmean->Clone("dq025h"); dq025->GetYaxis()->SetTitle("#Delta q_{0.25} <L_{xy}> [cm]");
      TH1F *dq05 =(TH1F *) dmean->Clone("dq05h");  dq05->GetYaxis()->SetTitle("#Delta q_{0.5} <L_{xy}> [cm]");
      TH1F *dq075=(TH1F *) dmean->Clone("dq075h"); dq075->GetYaxis()->SetTitle("#Delta q_{0.75} <L_{xy}> [cm]");
      for(size_t i=0; i<nresults; i++)
	{
	  chi2 ->SetBinContent(i+1,res[i].chi2);   chi2->SetBinError(i+1,0); 
	  dmean->SetBinContent(i+1,res[i].dmean);  dmean->SetBinError(i+1,0);
	  dq025->SetBinContent(i+1,res[i].dq025);  dq025->SetBinError(i+1,0);
	  dq05 ->SetBinContent(i+1,res[i].dq05);   dq05->SetBinError(i+1,0);
	  dq075->SetBinContent(i+1,res[i].dq075);  dq075->SetBinError(i+1,0);
	}

      c->Clear();
      c->Divide(1,5);

      c->cd(1); 
      chi2->Draw("e1");
      TLine *cen=new TLine(0,1,nresults,1); cen->SetLineStyle(kGray); cen->Draw();
      c->cd(2); 
      dmean->Draw("e1"); 
      TLine *up=new TLine(0,0.005,nresults,0.005); up->SetLineColor(kGray); up->SetLineStyle(9); up->Draw();
      TLine *down=new TLine(0,-0.005,nresults,-0.005); down->SetLineColor(kGray); down->SetLineStyle(9); down->Draw();
      cen=new TLine(0,0,nresults,0); cen->SetLineColor(kGray); cen->Draw();
      c->cd(3); 
      dq025->Draw("e1");
      cen->Draw();
      c->cd(4); 
      dq05->Draw("e1");
      cen->Draw();
      c->cd(5); 
      dq075->Draw("e1");
      cen->Draw();
      c->SaveAs(it->first + "_templatequality.png");
      c->SaveAs(it->first + "_templatequality.pdf");
    }

  inF->Close();
}
