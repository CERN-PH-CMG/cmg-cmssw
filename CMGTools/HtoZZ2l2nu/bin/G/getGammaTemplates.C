/*
  gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
  .L getGammaTemplates.C+
 */
#include <iostream>


#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"

#include "TSystem.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TPaveText.h"

string cats[]      ={"eq0jets", "eq1jets", "eq2jets",  "geq3jets"};
TString catLabels[]={"=0 jets", "=1 jets", "=2 jets", "#geq 3 jets"};
const size_t nCats=sizeof(cats)/sizeof(string);

string dilCats[]       = {"ee","mumu"};
TString dilCatLabels[] = {"ee","#mu#mu"};
const size_t nDilCats=sizeof(dilCats)/sizeof(string);

string dymcDir="Z#rightarrow ll";
string dyDir="data (ll)";
string gmcDirs[] = { "Z#gamma#rightarrow#nu#nu#gamma",
		     "W#gamma#rightarrowl#nu#gamma",
		     "W#rightarrow l#nu",
		     "QCD",
		     "#gamma+jets" };
bool gmcPure[] = { false,
		   false,
		   false,
		   true,
		   true };
const size_t nGmcDirs=sizeof(gmcDirs)/sizeof(string);
string gDir = "data (#gamma)";
string sigDir ="H(300)#rightarrow ZZ";

Int_t avgPu[]={5,8,12,15};


TGraphErrors *getEfficiencyCurve(TH1 *bckg, TH1 *sig=0);
std::vector<TH1D *> getPUReweighted(TH2F *h);
TObject* getObjectFromPath(TDirectory* File, std::string Path, bool GetACopy=false);
void show(std::vector<TH1F *> &plots,TString outName);

//
TObject* getObjectFromPath(TDirectory* File, std::string Path, bool GetACopy)
{
   TObject* toReturn = NULL;
   size_t pos = Path.find("/");
   if(pos < std::string::npos){
      std::string firstPart = Path.substr(0,pos);
      std::string endPart   = Path.substr(pos+1,Path.length());
      TDirectory* TMP = (TDirectory*)File->Get(firstPart.c_str());
      toReturn =  getObjectFromPath(TMP,endPart,GetACopy);
   }else{
      toReturn = File->Get(Path.c_str());
   }

   if(!toReturn)       printf("BUG: %s\n",Path.c_str());
   else if(GetACopy){ toReturn = toReturn->Clone();  }
   return toReturn;
}

//
void getGammaWeights(TString inputFile="plotter.root",TString varname="qt")
{

  //get plots from file for each category
  TFile *fin = TFile::Open(inputFile);
  std::vector<TH1F *> dataWgts,     mcWgts;
  std::vector<TH1F *> mcMassShapes, dataMassShapes;
  for(size_t id=0; id<nDilCats; id++)
    {
      for(size_t ic=0; ic<nCats; ic++)
	{
	  //mass shape
	  string pName=dilCats[id]+"_zmass_"+cats[ic];
	  string newPname=dilCats[id]+"_datazmass_"+cats[ic];
	  TH1F *hdymass=(TH1F *) getObjectFromPath(fin,dyDir+"/"+pName,true);      hdymass->SetName(newPname.c_str());   hdymass->SetDirectory(0);   dataMassShapes.push_back( hdymass );
	  newPname=dilCats[id]+"_mczmass_"+cats[ic];
	  TH1F * hmcdymass=(TH1F *) getObjectFromPath(fin,dymcDir+"/"+pName,true); hmcdymass->SetName(newPname.c_str()); hmcdymass->SetDirectory(0); mcMassShapes.push_back( hmcdymass );
	  
	  //variable
	  pName=dilCats[id]+"_"; pName+=varname.Data(); pName+="_"+cats[ic];
	  TH1F *hdy   = (TH1F *) getObjectFromPath(fin,dyDir+"/"+pName,true);
	  TH1F *hmcdy = (TH1F *) getObjectFromPath(fin,dymcDir+"/"+pName,true);
	  TH1F *hg    = (TH1F *) getObjectFromPath(fin,gDir+"/"+pName,true);
	  TH1F *hmcg  = 0;
	  for(size_t imc=0; imc<nGmcDirs; imc++)
	    {
	      if(hmcg==0) hmcg =     (TH1F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+pName,true);
	      else        hmcg->Add( (TH1F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+pName,false) ); 
	    }
	  TH1F *hMCwgts   = (TH1F *) hmcdy->Clone((pName+"_mcwgts").c_str()); hMCwgts->SetDirectory(0);   hMCwgts->Divide(hmcg);    mcWgts.push_back(hMCwgts);
	  TH1F *hDataWgts = (TH1F *) hdy->Clone((pName+"_datawgts").c_str()); hDataWgts->SetDirectory(0); hDataWgts->Divide(hg);    dataWgts.push_back(hDataWgts);
	}
    }
  fin->Close();
  
  //show weights
  setStyle();
  show(dataWgts,"DataGammaWeights");
  show(mcWgts,"MCGammaWeights");
  show(dataMassShapes,"DataMassShapes");
  show(mcMassShapes,"MCMassShapes");

  //save histograms to file
  TString foutName="gamma"+varname+"weights.root";
  TFile *fout = TFile::Open(foutName,"RECREATE");   fout->cd();
  for(size_t ip=0; ip<dataWgts.size(); ip++)        dataWgts[ip]->Write();
  for(size_t ip=0; ip<mcWgts.size(); ip++)          mcWgts[ip]->Write();
  for(size_t ip=0; ip<dataMassShapes.size(); ip++)  dataMassShapes[ip]->Write();
  for(size_t ip=0; ip<mcMassShapes.size(); ip++)    mcMassShapes[ip]->Write();
  fout->Close();
  
  cout << "*** Gamma pT weights (inclusive/differential) available @ " << foutName << endl;
}



//
void getGammaTemplates(TString inputFile="plotter.root",TString varname="met_met_")
{
  setStyle();

  //get plots from file for each category
  TFile *fin = TFile::Open(inputFile);
  for(size_t id=0; id<nDilCats; id++)
    {
      //variable
      string pName=dilCats[id]+"_"; pName+=varname.Data(); pName+="_vspu";
      TH2F *hmcsig= (TH2F *) getObjectFromPath(fin,sigDir+"/"+pName,true);
      TH2F *hdy   = (TH2F *) getObjectFromPath(fin,dyDir+"/"+pName,true);
      TH2F *hmcdy = (TH2F *) getObjectFromPath(fin,dymcDir+"/"+pName,true);
      TH2F *hg    = (TH2F *) getObjectFromPath(fin,gDir+"/"+pName,true);
      TH2F *hmcg  = 0, *hmcpureg=0, *hmccontg=0;
      for(size_t imc=0; imc<nGmcDirs; imc++)
	{
	  if(hmcg==0) hmcg =     (TH2F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+pName,true);
	  else        hmcg->Add( (TH2F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+pName,false) ); 
	  if( gmcPure[imc] )
	    {
	      if(hmcpureg==0) hmcpureg = (TH2F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+pName,true);
	      else            hmcpureg = (TH2F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+pName,false);
	    }
	  else
	    {
	      if(hmccontg==0) hmccontg = (TH2F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+pName,true);
	      else            hmccontg = (TH2F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+pName,false);
	    }
	}

      hmcsig->SetName(("signalmc_"+dilCats[id]).c_str());
      hdy->SetName(("dydata_"+dilCats[id]).c_str());
      hmcdy->SetName(("dymc_"+dilCats[id]).c_str());
      hg->SetName(("gdata_"+dilCats[id]).c_str());
      TH2F *hpureg = (TH2F *) hg->Clone(("puregdata_"+dilCats[id]).c_str()); hpureg->SetDirectory(0); hpureg->Add(hmccontg,-1);
      hmcg->SetName(("gmc_"+dilCats[id]).c_str());
      hmcpureg->SetName(("puregmc_"+dilCats[id]).c_str());
      hmccontg->SetName(("contgmc_"+dilCats[id]).c_str());

      //get templates for different pileup scenarios
      std::vector<TH1D *> hmcsigRwgt=getPUReweighted(hmcsig);
      std::vector<TH1D *> hmcdyRwgt=getPUReweighted(hmcdy);
      std::vector<TH1D *> hmcpuregRwgt=getPUReweighted(hmcpureg);
      std::vector<TH1D *> hpuregRwgt=getPUReweighted(hpureg);
      std::vector<TGraphErrors *> sfGr;
      std::vector<TGraphErrors *> sObGr;
      for(int igr=0; igr<2; igr++) 
	{
	  sfGr.push_back( new TGraphErrors );
	  sfGr[igr]->SetMarkerStyle(20+4*igr);
	  sfGr[igr]->SetFillStyle(0);

	  sObGr.push_back( new TGraphErrors );
	  sObGr[igr]->SetMarkerStyle(20+4*igr);
	  sObGr[igr]->SetFillStyle(0);
	}

      TCanvas *c= new TCanvas("c","c");
      c->SetWindowSize(800,400);
      c->SetCanvasSize(800,400);
      TGraph *frame=new TGraph;
      frame->SetMarkerStyle(1);
      frame->SetPoint(0,0.,0.0001);
      frame->SetPoint(1,500.,1.0);
      TGraph *frame2=new TGraph;
      frame2->SetMarkerStyle(1);
      frame2->SetPoint(0,0.6,0.0001);
      frame2->SetPoint(1,1.1,1.1);
      for(size_t iscenario=0; iscenario<hmcdyRwgt.size(); iscenario++)
	{
	  c->Clear();
	  c->Divide(2,1);
	  TPad *p=(TPad *)c->cd(1);
	  TLegend *leg = new TLegend(0.65,0.75,0.87,0.9);
	  leg->SetBorderSize(0);
	  leg->SetFillStyle(0);
	  leg->SetTextFont(42);
	  char scenarioTitle[100];
	  sprintf(scenarioTitle,"<N_{vtx}>=%d",avgPu[iscenario]); 
	  leg->SetHeader(scenarioTitle);
	  frame->Draw("ap");
	  frame->GetXaxis()->SetTitle(hmcdyRwgt[iscenario]->GetXaxis()->GetTitle());
	  frame->GetYaxis()->SetTitle("Events (normalized)");
	  leg->AddEntry(hmcdyRwgt[iscenario],"Z#rightarrow ll (MC)","l"); hmcdyRwgt[iscenario]->SetLineColor(kRed);      hmcdyRwgt[iscenario]->DrawNormalized("histsame");
	  leg->AddEntry(hmcpuregRwgt[iscenario],"#gamma+jets (MC)","l");  hmcpuregRwgt[iscenario]->SetLineColor(kGreen); hmcpuregRwgt[iscenario]->DrawNormalized("histsame");
	  leg->AddEntry(hpuregRwgt[iscenario],"#gamma+jets (data)","l"); 
	  hpuregRwgt[iscenario]->SetLineColor(kBlue);    
	  hpuregRwgt[iscenario]->SetMarkerColor(kBlue);    
	  hpuregRwgt[iscenario]->SetMarkerStyle(20);
	  hpuregRwgt[iscenario]->DrawNormalized("histsame");
	  leg->AddEntry(hmcsigRwgt[iscenario],sigDir.c_str(),"l");    	  hmcsigRwgt[iscenario]->SetLineWidth(2);        hmcsigRwgt[iscenario]->DrawNormalized("histsame");   
	  leg->Draw();
	  p->SetLogy();

	  p=(TPad *)c->cd(2);
	  frame2->Draw("ap");
	  frame2->GetXaxis()->SetTitle(hmcdyRwgt[iscenario]->GetXaxis()->GetTitle());
	  frame2->GetYaxis()->SetTitle("Instrumental background efficiency");
	  frame2->GetXaxis()->SetTitle("Signal efficiency");
	  TGraphErrors *mcdyEff    = getEfficiencyCurve(hmcdyRwgt[iscenario],hmcsigRwgt[iscenario]);      mcdyEff->Draw("l");
	  TGraphErrors *mcpuregEff = getEfficiencyCurve(hmcpuregRwgt[iscenario],hmcsigRwgt[iscenario]);   mcpuregEff->Draw("l");
	  TGraphErrors *puregEff   = getEfficiencyCurve(hpuregRwgt[iscenario],hmcsigRwgt[iscenario]);     puregEff->Draw("p");
	  p->SetLogy();


	  //Int_t ipVec[]={5,7};
	  Int_t ipVec[]={3,6};
	  for(size_t iip=0; iip<2; iip++)
	    {
	      Int_t ip=ipVec[iip];
	      if(iip==1 && iscenario>2) continue; 
	      Double_t x,ymc,ymcerr,ydata,ydataerr;
	      mcdyEff->GetPoint(ip,x,ymc);    ymcerr=mcdyEff->GetErrorY(ip);
	      puregEff->GetPoint(ip,x,ydata); ydataerr=puregEff->GetErrorY(ip);
	      
	      Double_t sf=fabs(ydata/ymc);
	      Double_t sferr=sqrt(pow(ydata*ymcerr,2)+pow(ymc*ydataerr,2))/pow(ymc,2);
	      Double_t ipgr=sfGr[iip]->GetN();
	      sfGr[iip]->SetPoint(ipgr,avgPu[iscenario],sf);
	      sfGr[iip]->SetPointError(ipgr,0,sferr);
	      char bufTit[100];
	      sprintf(bufTit,"#varepsilon_{signal}=%3.2lf",x);
	      sfGr[iip]->SetTitle(bufTit);

	      Double_t signal=hmcsigRwgt[iscenario]->Integral()*x;
	      Double_t bckg=hpuregRwgt[iscenario]->Integral()*ydata;
	      Double_t bckgErr=hpuregRwgt[iscenario]->Integral()*ydataerr;

	      sObGr[iip]->SetPoint(ipgr,avgPu[iscenario],signal/sqrt(bckg));
	      sObGr[iip]->SetPointError(ipgr,0,0.5*signal*bckgErr/pow(bckg,3.5));
	      sObGr[iip]->SetTitle(bufTit);
	    }

	  c->Modified();
	  c->Update();
	  TString cname("PuScenario_"); cname+= iscenario;
	  c->SaveAs(cname + ".png");
	}
      delete c;
      
      c= new TCanvas("c","c");
      c->SetWindowSize(400,400);
      c->SetCanvasSize(400,400);
      frame=new TGraph;
      frame->SetMarkerStyle(1);
      frame->SetPoint(0,0.,0.0);
      frame->SetPoint(1,25.,2.0);
      frame->Draw("ap");
      frame->GetYaxis()->SetTitle("Data/MC efficiency scale factor");
      frame->GetXaxis()->SetTitle("Number of vertices");
      sfGr[0]->Draw("pl");
      sfGr[1]->Draw("pl");
      TLegend *leg = new TLegend(0.65,0.75,0.87,0.9);
      leg->SetBorderSize(0);
      leg->SetFillStyle(0);
      leg->SetTextFont(42);
      leg->AddEntry(sfGr[0],sfGr[0]->GetTitle(),"p");
      leg->AddEntry(sfGr[1],sfGr[1]->GetTitle(),"p");
      leg->Draw();
    
      c->SaveAs("~/www/tmp/ScaleFactor.png");



      c= new TCanvas("c","c");
      c->SetWindowSize(400,400);
      c->SetCanvasSize(400,400);
      frame=new TGraph;
      frame->SetMarkerStyle(1);
      frame->SetPoint(0,0.,0.0);
      frame->SetPoint(1,25.,0.5);
      frame->Draw("ap");
      frame->GetYaxis()->SetTitle("S/#sqrt{Intrumental background}");
      frame->GetXaxis()->SetTitle("Number of vertices");
      sObGr[0]->Draw("pl");
      sObGr[1]->Draw("pl");
      leg = new TLegend(0.65,0.75,0.87,0.9);
      leg->SetBorderSize(0);
      leg->SetFillStyle(0);
      leg->SetTextFont(42);
      leg->AddEntry(sfGr[0],sObGr[0]->GetTitle(),"p");
      leg->AddEntry(sfGr[1],sObGr[1]->GetTitle(),"p");
      leg->Draw();
    
      c->SaveAs("~/www/tmp/Robustness.png");


    }
  fin->Close();
}

//
TGraphErrors *getEfficiencyCurve(TH1 *bckg, TH1 *sig)
{
  if(bckg==0) return 0;

  //format the efficiency curve according to the background
  TGraphErrors *gr = new TGraphErrors;
  gr->SetName(bckg->GetName()+TString("_eff"));
  gr->SetMarkerStyle(bckg->GetMarkerStyle());
  gr->SetMarkerColor(bckg->GetMarkerColor());
  gr->SetLineStyle(bckg->GetLineStyle());
  gr->SetLineColor(bckg->GetLineColor());
  gr->SetLineWidth(bckg->GetLineWidth());

  //compute the efficiency as function of the cut
  Double_t totalBckg(0),totalBckgErr(0);
  totalBckg=bckg->IntegralAndError(0,bckg->GetXaxis()->GetNbins()+1,totalBckgErr);
  Double_t totalSig(0),totalSigErr(0);
  if(sig) totalSig=sig->IntegralAndError(0,sig->GetXaxis()->GetNbins()+1,totalSigErr);
  for(int ibin=1; ibin<=bckg->GetXaxis()->GetNbins(); ibin++)
    {
      Int_t ip=gr->GetN();
      Double_t xval=bckg->GetXaxis()->GetBinCenter(ibin);
      Double_t xvalErr(0);
      if(sig) 
	{
	  Double_t iTotalSig(0),iTotalSigErr(0);
	  iTotalSig=sig->IntegralAndError(ibin,sig->GetXaxis()->GetNbins()+1,iTotalSigErr);
	  xval=iTotalSig/totalSig;
	  xvalErr=sqrt(pow(iTotalSig*totalSigErr,2)+pow(iTotalSigErr*totalSig,2))/pow(totalSig,2);
	}
      Double_t iTotalBckg(0),iTotalBckgErr(0);
      iTotalBckg=bckg->IntegralAndError(ibin,bckg->GetXaxis()->GetNbins()+1,iTotalBckgErr);
      Double_t yval=fabs(iTotalBckg/totalBckg);
      Double_t yvalErr=sqrt(pow(iTotalBckg*totalBckgErr,2)+pow(iTotalBckgErr*totalBckg,2))/pow(totalBckg,2);

      gr->SetPoint(ip,xval,yval);
      gr->SetPointError(ip,xvalErr,yvalErr);
    }

  return gr;
}

//
std::vector<TH1D *> getPUReweighted(TH2F *h)
{
  std::vector<TH1D *> result;
  if(h==0) return result;

  //get PU distribution observed
  TH1D *puObs = h->ProjectionX();
  puObs->Scale(1./puObs->Integral());
  TH1D *puDist=(TH1D *) puObs->Clone("pudist");
  TH1D *puWgts=(TH1D *) puObs->Clone("puwgts");

  Double_t totalBeforeProj(h->Integral());
  for(size_t i=0; i<sizeof(avgPu)/sizeof(Int_t); i++)
    {
      //generate a new PU distribution and weights
      puDist->Reset("ICE");
      puWgts->Reset("ICE");
      for(int ibin=1; ibin<=puDist->GetXaxis()->GetNbins(); ibin++)
	{
	  Float_t npu=puDist->GetXaxis()->GetBinLowEdge(ibin);
	  puDist->SetBinContent(ibin,TMath::Poisson(npu,avgPu[i]));
	}
      puDist->Scale(1./puDist->Integral());
      puWgts->Add(puDist);
      puWgts->Divide(puObs);

      //now generate a re-weighted projection
      TH1D *rwgtProj=0;
      TString name(h->GetName()); name += "_"; name += avgPu[i];
      for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
	{
	  Float_t wgt= puWgts->GetBinContent(ibin);
	  TH1D *varProj = h->ProjectionY("_py",ibin,ibin);
	  if(rwgtProj==0)  { rwgtProj = (TH1D *) varProj->Clone(name); rwgtProj->SetDirectory(0); rwgtProj->Reset("ICE"); }
	  rwgtProj->Add(varProj,wgt);
	  delete varProj;
	}
      Double_t sf=rwgtProj->Integral()/totalBeforeProj;
      rwgtProj->Scale(sf);
      result.push_back(rwgtProj);
    }

  //free unused results
  delete puObs;
  delete puDist;
  delete puWgts;

  //all done here
  return result;
}


//
void show(std::vector<TH1F *> &plots,TString outName)
{
  TCanvas *cnv = getNewCanvas("c","c",false);
  cnv->Clear();
  cnv->SetWindowSize(nDilCats*400,400);
  cnv->Divide(nDilCats,1);
  Int_t colors[]={kBlue,1,kRed,kGreen,kOrange};
  for(size_t ip=0; ip<plots.size(); ip++)
    {
      cnv->cd(ip/nCats+1);
      plots[ip]->SetLineColor(colors[ip%nCats]);
      plots[ip]->Draw(ip%nCats==0 ? "hist" : "histsame");
      plots[ip]->SetTitle(catLabels[ip%nCats]);
    }
  for(size_t id=0; id<nDilCats; id++)
    {
      cnv->cd(id+1);
      TPaveText *pave = new TPaveText(0.5,0.75,0.85,0.95,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->SetTextAlign(32);
      pave->SetTextFont(42);
      pave->AddText(dilCatLabels[id])->SetTextFont(42);
      pave->Draw();
    }
  cnv->SaveAs(outName+".png");
}



