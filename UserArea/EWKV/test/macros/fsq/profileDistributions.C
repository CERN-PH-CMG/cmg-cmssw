#include "TFile.h"
#include "TString.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TPaveText.h"

#include <iostream>

using namespace std;

//
void showCMSHeader(TObject *data=0, TObject *mc=0)
{
  TPaveText *pt=new TPaveText(0.1,0.91,0.8,0.99,"brNDC");
  pt->SetBorderSize(0);
  pt->SetFillStyle(0);
  pt->SetTextAlign(12);
  pt->SetTextSize(0.08);
  pt->AddText("CMS preliminary, #sqrt{s}=8 TeV, #scale[0.5]{#int}L=19.7 fb^{-1}");
  pt->Draw();
  
  if(data==0 && mc==0) return;
  TLegend *leg=new TLegend(0.15,0.15,0.7,0.2);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetNColumns(2);
  leg->SetTextSize(0.08);
  if(data) leg->AddEntry(data,"data","p");
  if(mc) leg->AddEntry(mc,"simulation","p");
  leg->Draw();
}


void profileDistributions()
{
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPalette(1);

  TFile *llF=TFile::Open("~/work/ewkzp2j_539/plotter.root");
  TFile *gF=TFile::Open("~/work/ewkzp2j_539/plotter_g_raw.root");
  
  TString profs[]={//"recoilbalancevseta",
		   "recoilbalancevseta30to50",
		   "recoilbalancevseta50toInf"};
  TString cats[]={//"inclusive",
		  "30<p_{T}/GeV<50",
		  "p_{T}>50 GeV"};
  const size_t nprofs=sizeof(profs)/sizeof(TString);

  TString ch[]={"ee","mumu"};
  const size_t nch=sizeof(ch)/sizeof(TString);
  
  TGraph *frame=new TGraph; 
  frame->SetName("frame");
  frame->SetPoint(0,0,0.4);
  frame->SetPoint(1,5,1.8);
  frame->SetMarkerStyle(1);
  TGraph *sfFrame=0;
  TObjArray toSave;
  for(size_t ich=0; ich<nch; ich++)
    {
      TCanvas *c=new TCanvas("c"+ch[ich],"c"+ch[ich],600,600); c->Divide(1,3);
      TCanvas *cg=new TCanvas("cg","cg",600,600);              cg->Divide(1,3);

      TCanvas *cr=new TCanvas("cr"+ch[ich],"cr"+ch[ich],600,600); cr->Divide(1,3);
      TCanvas *cgr=new TCanvas("cgr","cgr",600,600);              cgr->Divide(1,3);

      for(size_t i=0; i<nprofs; i++)
	{
	  TH2 *dyMC   = (TH2 *) llF->Get("Z#rightarrow ll/"+ch[ich]+"_"+profs[i]); 
	  TH2 *dyData = (TH2 *) llF->Get("data/"+ch[ich]+"_"+profs[i]);            
	  cout << dyMC << " " << dyData << endl;
	  dyMC->SetDirectory(0);
	  dyData->SetDirectory(0);

	  TH2 *gMC   = (TH2 *) gF->Get("#gamma+jets/"+ch[ich]+"_"+profs[i]); 
	  cout << gMC << endl;
	  gMC->Add(    (TH2 *) gF->Get("Multijets/"+ch[ich]+"_"+profs[i]) ); 
	  TH2 *gData = (TH2 *) gF->Get("data (#gamma)/"+ch[ich]+"_"+profs[i]);            
	  cout << gData << endl;
	  gMC->SetDirectory(0);
	  gData->SetDirectory(0);
	  
	  TH1D *dyMCProfH   = dyMC->ProfileX("dymcprof");
	  TGraphErrors *dyMCProf   = new TGraphErrors(dyMCProfH);
	  dyMCProf->SetMarkerStyle(24);  
	  dyMCProf->SetName(ch[ich]+profs[i]+"dymc");
	  
	  TH1D *dyDataProfH = dyData->ProfileX("dydataprof");	  
	  TGraphErrors *dyDataProf = new TGraphErrors(dyDataProfH);  
	  dyDataProf->SetMarkerStyle(20); 
	  dyDataProf->SetName(ch[ich]+profs[i]+"dydata");
	  
	  dyDataProfH->Divide(dyMCProfH);
	  TGraph *dyRatio = new TGraph(dyDataProfH); 
	  dyRatio->SetName(ch[ich]+profs[i]+"dydata2mc");

	  TH1D *gMCProfH = gMC->ProfileX("gmcprof");
	  TGraphErrors *gMCProf    = new TGraphErrors(gMCProfH);    
	  gMCProf->SetMarkerStyle(24);  
	  gMCProf->SetName(ch[ich]+profs[i]+"gmc");
	 
	  TH1D *gDataProfH = gData->ProfileX("gdataprof");
	  TGraphErrors *gDataProf  = new TGraphErrors(gDataProfH);   
	  gDataProf->SetMarkerStyle(20); 
	  gDataProf->SetName(ch[ich]+profs[i]+"gdata");

	  gDataProfH->Divide(gMCProfH);
	  TGraph *gRatio = new TGraph(gDataProfH);
	  gRatio->SetName(ch[ich]+profs[i]+"gdata2mc");
	  
	  toSave.Add(dyRatio);
	  toSave.Add(gRatio);

	  //
	  TPad *p=(TPad *)c->cd(i+1);
	  if(i<nprofs-1) p->SetBottomMargin(0.01);
	  if(i>0) p->SetTopMargin(0);
	  frame->Draw("ap");
	  frame->GetYaxis()->SetRangeUser(0.34,1.66);
	  frame->GetYaxis()->SetTitle("Jet response");
	  frame->GetXaxis()->SetRangeUser(0,4.75);
	  frame->GetXaxis()->SetTitle("Pseudo-rapidity");
	  frame->GetYaxis()->SetLabelSize(0.06);
	  frame->GetYaxis()->SetTitleSize(0.08);
	  frame->GetYaxis()->SetTitleOffset(0.5);
	  frame->GetXaxis()->SetLabelSize(0.06);
	  frame->GetXaxis()->SetTitleSize(0.08);
	  frame->GetXaxis()->SetTitleOffset(0.6);
	  dyDataProf->Draw("p");
	  dyMCProf->Draw("p");
	  if(i==0) showCMSHeader(dyDataProf,dyMCProf);

	  TPaveText *pt=new TPaveText(0.1,0.8,0.8,0.85,"brNDC");
	  pt->SetBorderSize(0);
	  pt->SetFillStyle(0);
	  pt->SetTextAlign(12);
	  pt->SetTextFont(42);
	  pt->SetTextSize(0.08);
	  TString label("Z#rightarrow "+ch[ich]+ ", " + cats[i]);
	  label.ReplaceAll("mumu","#mu#mu");
	  pt->AddText(label);
	  pt->Draw();


	  //
	  p=(TPad *)cr->cd(i+1);
	  if(i<nprofs-1) p->SetBottomMargin(0.01);
	  if(i>0) p->SetTopMargin(0);
	  if(sfFrame==0) {
	    sfFrame =(TGraph *) frame->Clone("sfframe");
	    sfFrame->GetYaxis()->SetTitle("Data/MC jet response");
	    sfFrame->GetYaxis()->SetRangeUser(0.86,1.14);
	  }
	  sfFrame->Draw("ap");
	  dyRatio->Draw("l");
	  if(i==0) showCMSHeader();
	  pt->Clone()->Draw();

	  //
	  p=(TPad *)cg->cd(i+1);
	  if(i<nprofs-1) p->SetBottomMargin(0.01);
	  if(i>0) p->SetTopMargin(0);
	  frame->Draw("ap");
	  gDataProf->Draw("p");
	  gMCProf->Draw("p");

	  if(i==0) showCMSHeader(gDataProf,gMCProf);

	  pt=new TPaveText(0.1,0.8,0.8,0.85,"brNDC");
	  pt->SetBorderSize(0);
	  pt->SetFillStyle(0);
	  pt->SetTextAlign(12);
	  pt->SetTextFont(42);
	  pt->SetTextSize(0.08);
	  pt->AddText("#gamma, " + cats[i]);
	  pt->Draw();


	  //
	  p=(TPad *)cgr->cd(i+1);
	  if(i<nprofs-1) p->SetBottomMargin(0.01);
	  if(i>0) p->SetTopMargin(0);
	  sfFrame->Draw("ap");
	  gRatio->Draw("l");
	  if(i==0) showCMSHeader();
	  pt->Clone()->Draw();
	}

      c->SaveAs(ch[ich]+"jetresponse.pdf");
      cg->SaveAs("gjetresponse.pdf");
      cr->SaveAs(ch[ich]+"jetresponseSF.pdf");
      cgr->SaveAs("gjetresponseSF.pdf");
    }
  llF->Close();
  gF->Close();
  

  //save the results
  TFile *fOut = TFile::Open("jecResponse.root","RECREATE");
  for(int i=0; i<toSave.GetEntriesFast(); i++)
      toSave.At(i)->Write();
  fOut->Close();
}
