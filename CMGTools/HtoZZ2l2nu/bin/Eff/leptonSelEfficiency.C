
/**
   gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
   .L leptonSelEfficiency.C+
 */

#include <iostream>
#include <strstream>
#include <map>

#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"

#include "TString.h"
#include "TSystem.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TH2F.h"
#include "TF1.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TPaveText.h"


using namespace std;

Int_t markers[]={20, 25, 4, 23, 22};

//
void leptonSelEfficiency(TString url)
{
  std::strstream report;


  TFile *fin = TFile::Open(url);
  
  setStyle();
  gStyle->SetOptFit(0);

  TString ch[]={"ee","mumu"};
  for(size_t ich=0; ich<2; ich++)
    {

      report << ch[ich] << endl;
      
      std::vector<TH1 *> ptl[2]; 
      std::vector<TGraphAsymmErrors *> effl[2];
      std::vector<TProfile *> lIso[2];
      for(size_t id=0; id<=6; id++)
	{
	  TString idb(""); if(id) idb+=id;
	  

	  TString idname("no id");
	  if(ich==0)
	    {
	      if(id==1) idname="VBTF90";
	      if(id==2) idname="VBTF80";
	      if(id==3) idname="CIC medium";
	      if(id==4) idname="HEEP";
	    }
	  if(ich==1)
	    {
	      if(id==1) idname="GlobalMuonPromptTight";
	      if(id==2) idname="TMLastStationLoose";
	      if(id==3) idname="TMLastStationTight";
	      if(id==4) idname="TMLastStationAngTight";
	    }
	  
	  for(size_t ilep=0; ilep<=1; ilep++)
	    {
	      TString lid("_l"); lid += (ilep+1); 

	      TString idHisto("pt"); idHisto += idb;
	      TH1 *h = (TH1 *)fin->Get("localAnalysis/"+ch[ich]+"/"+ch[ich]+lid+idHisto);
	      if(h==0) continue;
	      if(h->Integral()==0) continue;
	      h->SetDirectory(0);
	      fixExtremities(h,true,true);
	      ptl[ilep].push_back(h);
	      
	      if(id)
		{
		  TGraphAsymmErrors *gr = new TGraphAsymmErrors;  
		  gr->SetName(ch[ich]+"eff"+lid+idHisto);
		  gr->BayesDivide(h,ptl[ilep][0]); 
		  gr->SetTitle(idname);
		  effl[ilep].push_back(gr); 
		  gr->Fit("pol0","0Q+");
		}
	      else
		{
		  TString isoHistos[]={"pfisovspu","pfisocorrvspu","detisovspu","detisocorrvspu"};
		  TString isoTitles[]={"PF","PF+#beta correction","detector","detector+#rho correction"};
		  for(int iisoHisto=0; iisoHisto<4; iisoHisto++)
		    {
		      TH2F *h2 = (TH2F *) fin->Get("localAnalysis/"+ch[ich]+"/"+ch[ich]+lid+isoHistos[iisoHisto]);
		      if(h2==0) continue;

		      TProfile *prof=h2->ProfileY(ch[ich]+lid+isoHistos[iisoHisto]+"_prof");
		      prof->SetDirectory(0);
		      prof->SetTitle(isoTitles[iisoHisto]);
		      lIso[ilep].push_back(prof);
		      prof->Fit("pol1","0Q+");
		    }
		}
	    }
	}
      TCanvas *c = new TCanvas("pt"+ch[ich],"pt"+ch[ich],1200,600);
      c->SetWindowSize(600,600);
      for(size_t ilep=0; ilep<2; ilep++)
	{
	  ptl[ilep][0]->SetLineColor(1+ilep);
	  ptl[ilep][0]->DrawNormalized(ilep==0?"hist":"histsame");
	  
	}
      c->SaveAs(ch[ich]+"pt.png");
      
      c = new TCanvas("effid"+ch[ich],"effid"+ch[ich],1200,600);
      c->SetWindowSize(1200,600);
      c->Divide(2,1);
      for(size_t ilep=0; ilep<2; ilep++)
	{
	  c->cd(ilep+1);
	  TLegend *leg = new TLegend(0.65,0.7,0.9,0.8);
	  cout << __LINE__ << endl;
	  for(size_t i=0; i<effl[ilep].size(); i++)
	    {
	      effl[ilep][i]->SetFillStyle(0);
	      effl[ilep][i]->SetMarkerStyle(markers[i]);
	      effl[ilep][i]->Draw(i==0?"ap":"p");
	      effl[ilep][i]->GetXaxis()->SetTitle( ptl[ilep][i]->GetXaxis()->GetTitle() );
	      effl[ilep][i]->GetYaxis()->SetTitle( "Efficiency" );
	      leg->AddEntry(effl[ilep][i],effl[ilep][i]->GetTitle(),"p");

	      TF1 *pol0 = (TF1 *)effl[ilep][i]->GetFunction("pol0");
	      pol0->SetLineColor(kBlue);
	      pol0->SetLineStyle(9);
	      pol0->SetLineWidth(1);
	      pol0->Draw("same");

	      report << ilep << " \t " << effl[ilep][i]->GetTitle() << "\t" << pol0->GetParameter(0) << " +/- " << pol0->GetParError(0) << endl;
	    }
	  if(ilep==0)
	    {
	      leg->SetFillStyle(0);
	      leg->SetFillColor(0);
	      leg->SetBorderSize(0);
	      leg->SetTextFont(42);
	      leg->Draw();
	    }
	  TPaveText *pave = new TPaveText(0.65,0.83,0.9,0.95,"brNDC");
	  pave->SetBorderSize(0);
	  pave->SetFillStyle(0);
	  pave->SetTextAlign(32);
	  pave->SetTextFont(42);
	  pave->AddText(ilep==0 ? "leading lepton" : "sub-leading lepton");
	  pave->Draw();

	  c->SaveAs(ch[ich]+"eff.png");
	}
    

      //isolation
      c = new TCanvas("iso"+ch[ich],"iso"+ch[ich],1200,600);
      c->SetWindowSize(1200,600);
      c->Divide(2,1);

      for(size_t ilep=0; ilep<2; ilep++)
	{
	  c->cd(ilep+1);
	  TLegend *leg = new TLegend(0.65,0.7,0.9,0.8);
	  for(size_t i=0; i<lIso[ilep].size(); i++)
	    {
	      lIso[ilep][i]->SetFillStyle(0);
	      lIso[ilep][i]->SetMarkerStyle(markers[i]);   
	      lIso[ilep][i]->Draw(i==0?"e1":"e1same");
	      lIso[ilep][i]->GetXaxis()->SetTitle( "Pileup");
	      lIso[ilep][i]->GetYaxis()->SetTitle( "<Isolation>" );
	      TF1 *pol1 = (TF1 *)lIso[ilep][i]->GetFunction("pol1");
	      pol1->SetLineColor(kBlue);
	      pol1->SetLineStyle(9);
	      pol1->SetLineWidth(1);
	      pol1->Draw("same");
	      leg->AddEntry(lIso[ilep][i],lIso[ilep][i]->GetTitle(),"p");
	    }
	  if(ilep==0)
	    {
	      leg->SetFillStyle(0);
	      leg->SetFillColor(0);
	      leg->SetBorderSize(0);
	      leg->SetTextFont(42);
	      leg->Draw();
	    }
	  TPaveText *pave = new TPaveText(0.65,0.83,0.9,0.95,"brNDC");
	  pave->SetBorderSize(0);
	  pave->SetFillStyle(0);
	  pave->SetTextAlign(32);
	  pave->SetTextFont(42);
	  pave->AddText(ilep==0 ? "leading lepton" : "sub-leading lepton");
	  pave->Draw();

	  c->SaveAs(ch[ich]+"iso.png");
	}
    }
  
  
  fin->Close();

  //
  cout << "Efficiencies measured" << endl
       << report.str()
       << endl;
}

