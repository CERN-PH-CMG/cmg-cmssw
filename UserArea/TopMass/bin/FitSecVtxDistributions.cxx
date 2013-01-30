#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooFitResult.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooGenericPdf.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooWorkspace.h"
#include "RooCategory.h"
#include "RooPlot.h"
#include "RooFit.h"
#include "Riostream.h"
#include "RooArgList.h"
#include "RooRandom.h"
#include "RooMinuit.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"

#include "TProfile.h"
#include "TPaveText.h"
#include "TH1F.h"
#include "TSystem.h"
#include "TFile.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TAxis.h"
#include "TIterator.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TLegend.h"

#include <strstream>
#include <iostream>
#include <vector>
#include <map>

using namespace std;
using namespace RooFit;

strstream report;

class SecVtxShape_t
{
public:
  
  SecVtxShape_t() { m_tag=""; }
  
  SecVtxShape_t(TString tag) { m_tag=tag; }
  
  SecVtxShape_t(const SecVtxShape_t & other) 
  {
    m_tag       = other.m_tag;
    lxy_data    = other.lxy_data;
    mass_data   = other.mass_data;
    lxy_signal  = other.lxy_signal;
    mass_signal = other.mass_signal;
    lxy_signalSysts  = other.lxy_signalSysts;
    mass_signalSysts = other.mass_signalSysts;
    lxy_bckg    = other.lxy_bckg;
    mass_bckg   = other.mass_bckg;
  }
    
  TString m_tag;
  TH1F *lxy_data, *mass_data;
  std::map<Float_t, TH1F *> lxy_signal, mass_signal;
  std::map<Float_t, std::map<TString, TH1F *> > lxy_signalSysts, mass_signalSysts;
  std::map<TString, TH1F *> lxy_bckg;
  std::map<TString, TH1F *> mass_bckg;
};

void showSecVtxShapeCollection(RooWorkspace *,std::vector<SecVtxShape_t> &);
RooWorkspace *defineWorkspace(std::vector<SecVtxShape_t> &chShapes);
RooWorkspace *readWorkspace(TString url) { return 0; } 

//
void showSecVtxShapeCollection(RooWorkspace *w, std::vector<SecVtxShape_t> &shapes)
{
  if(shapes.size()==0) return;

  TCanvas *c=new TCanvas("c","c",400*shapes.size(),800);
  c->Divide(shapes.size(),2);

  for(size_t is=0; is<shapes.size(); is++)
    {
      const SecVtxShape_t &m_shape=shapes[is];

      TString ch(""), chTitle("");
      if(m_shape.m_tag.BeginsWith("ee"))       { ch="ee";   chTitle="ee"; }
      else if(m_shape.m_tag.BeginsWith("emu")) { ch="emu";  chTitle="e#mu"; }
      else if(m_shape.m_tag.BeginsWith("mumu")){ ch="mumu"; chTitle="#mu#mu"; }
      else if(m_shape.m_tag.BeginsWith("e"))   { ch="e";    chTitle="e"; }
      else if(m_shape.m_tag.BeginsWith("mu"))  { ch="mu";   chTitle="#mu"; }
      chTitle += " events";

      //lxy
      c->cd(is+1);
      RooRealVar *x=w->var("lxy");
      RooPlot *frame = x->frame();
      RooDataHist *lxydata=(RooDataHist *)w->data(ch+"data");
      lxydata->plotOn(frame,Name("data"),Name("data"));
      w->pdf(ch+"model")->plotOn(frame, RooFit::FillStyle(0), RooFit::LineWidth(2), MoveToBack(),
				 ProjWData(*x,*lxydata), DrawOption("lf"),   Name("b"));
      w->pdf(ch+"model")->plotOn(frame, RooFit::Components(* w->pdf( ch+"flxy_bkg")),
				 RooFit::FillStyle(1001), RooFit::FillColor(kAzure+9), MoveToBack(), 
				 ProjWData(*x,*lxydata), DrawOption("lf"), Name("udscg"));

      frame->SetTitleOffset(1.0,"Y");
      frame->SetTitleSize(0.05,"Y");
      frame->SetYTitle("Jets");
      frame->SetTitleSize(0.05,"X");
      frame->SetXTitle("SecVtx L_{xy} [cm]");
      frame->SetMinimum(0.01);
      frame->Draw();

      if(is==0)
	{
	  TLegend *leg=new TLegend(0.4,0.75,0.9,0.9);
	  leg->SetNColumns(3);
	  leg->SetBorderSize(0);
	  leg->SetFillStyle(0);
	  leg->SetTextFont(42);
	  leg->AddEntry("data","data","p");
	  leg->AddEntry("b","b","F");
	  leg->AddEntry("udscg","udsg+c","F");
	  leg->Draw();

	  TPaveText *pave = new TPaveText(0.15,0.9,0.4,0.95,"NDC");
	  pave->SetBorderSize(0);
	  pave->SetFillStyle(0);
	  pave->SetTextAlign(32);
	  pave->SetTextFont(42);
	  pave->AddText("CMS preliminary");
	  pave->Draw();
	}
      
      TPaveText *pave = new TPaveText(0.65,0.9,0.9,0.95,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->SetTextAlign(32);
      pave->SetTextFont(42);
      pave->AddText(chTitle);
      pave->Draw();
      
      //sec vtx mass
      c->cd(is+1+shapes.size());
      x=w->var("secvtxmass");
      frame = x->frame();
      RooDataHist *data=(RooDataHist *)w->data(ch+"mass_data");
      data->plotOn(frame,Name("data"),Name("data"));

      // FIXME: changed flavormodel to flavormodelunc
      w->pdf(ch+"flavormodelunc")->plotOn(frame, RooFit::FillStyle(0), RooFit::LineWidth(2), 
      				       ProjWData(*x,*data), DrawOption("lf"),   Name("b"));
      w->pdf(ch+"flavormodelunc")->plotOn(frame, RooFit::Components( RooArgSet(* w->pdf( ch+"_cvtxmasspdf"), * w->pdf( ch+"_udsgvtxmasspdf")) ),  
      				       RooFit::FillStyle(1001), RooFit::FillColor(kAzure+9),   
      				       ProjWData(*x,*data), DrawOption("lf"),   Name("udsg"));
      w->pdf(ch+"flavormodelunc")->plotOn(frame, RooFit::Components( * w->pdf( ch+"_cvtxmasspdf") ),  
      				       RooFit::FillStyle(1001), RooFit::FillColor(kOrange-2),  
      				       ProjWData(*x,*data), DrawOption("lf"),   Name("c"));
    

      frame->SetTitleOffset(1.0,"Y");
      frame->SetTitleSize(0.05,"Y");
      frame->SetYTitle("Jets");
      frame->SetTitleSize(0.05,"SecVtx Mass [GeV]");
      frame->SetMinimum(0.01);
      frame->Draw();

      TPaveText *T = new TPaveText(0.1,0.92,0.9,0.98, "NDC");
      T->SetFillColor(0);  T->SetFillStyle(0);  T->SetLineColor(0); T->SetTextAlign(22); T->SetTextFont(42);
      char buf[200];
      sprintf(buf,"f_{c}=%3.3f #pm %3.3f    f_{b}=%3.3f #pm %3.3f",
	      w->var(ch+"cyields")->getVal(), w->var(ch+"cyields")->getError(),
	      w->var(ch+"byields")->getVal(), w->var(ch+"byields")->getError());
      T->AddText(buf);
      T->Draw("same");

      if(is==0)
	{
	  TLegend *leg=new TLegend(0.3,0.75,0.9,0.9);
	  leg->SetNColumns(4);
	  leg->SetBorderSize(0);
	  leg->SetFillStyle(0);
	  leg->SetTextFont(42);
	  leg->AddEntry("data","data","p");
	  leg->AddEntry("b","b","F");
	  leg->AddEntry("udsg","udsg","F");
	  leg->AddEntry("c","c","F");
	  leg->Draw();
	}
    }

  c->Modified();
  c->Update();
  c->SaveAs("SecVtx.png");
}

//
RooWorkspace *defineWorkspace(std::vector<SecVtxShape_t> &chShapes)
{
  RooWorkspace* w = new RooWorkspace("w") ; 

  //the main incognitas
  RooRealVar mtop("mtop","m_{top} [GeV]",173.5,100,200);
  RooRealVar xsec("xsec","#sigma_{t#bar{t}} [pb]",220,0,1000);

  //the observables
  RooRealVar lxy("lxy","L_{xy} [cm]",0,5) ;
  RooRealVar secvtxmass("secvtxmass","SecVtx Mass [GeV]",0,10) ;
 
  TCanvas *cqt=new TCanvas("cqt","cqt",chShapes.size()*400,400);
  cqt->Divide(chShapes.size(),1);

  //iterate over the different channels and build the individual templates
  for(size_t is=0; is<chShapes.size(); is++)
    {
      TString ch=chShapes[is].m_tag;
      RooCategory templateCategories(ch+"categs",ch+"categs") ; 
      std::map<std::string,TH1 *> templates;
      std::map<std::string,RooDataHist *> rooTemplates;
      std::map<std::string,float> masses;


      //
      // FLAVOR FRACTIONS FROM SECVtx MASS
      // 
      RooArgSet flavorFractions,flavorPdfs,flavorConstraints;
      int smoothOrder=0;

      //data
      RooDataHist *mass_data = new RooDataHist(ch+"mass_data",ch+"mass_data",secvtxmass,chShapes[is].mass_data);
      w->import(*mass_data);
      float totalObs=chShapes[is].mass_data->Integral();

      //expected composition from simulation
      float totalExp(0),udsgExp(0),cExp(0),bExp(0);
      TH1F *h_signal= (TH1F *)chShapes[is].mass_signal[172.5]->Clone(ch+"bvtxmassh");
      //h_signal->Add(chShapes[is].mass_bckg["b"]);
      totalExp += h_signal->Integral();
      bExp     += h_signal->Integral();
      for(map<TString, TH1F *>::iterator it=chShapes[is].mass_bckg.begin(); it != chShapes[is].mass_bckg.end(); it++)
	{
	  totalExp += it->second->Integral();
       	  if(it->first.BeginsWith("c"))    cExp += it->second->Integral();
       	  if(it->first.BeginsWith("udsg")) udsgExp += it->second->Integral();
	}

      //b's from t->Wb and other residual b's
      RooDataHist *vtxMassSignalData = new RooDataHist(ch+"_bvtxmass",ch+"_bvtxmass",RooArgList(secvtxmass),Import(*h_signal));
      RooHistPdf *vtxMassSignalPdf   = new RooHistPdf(ch+"_bvtxmasspdf",ch+"_bvtxmasspdf",RooArgSet(secvtxmass),*vtxMassSignalData,smoothOrder);
      flavorPdfs.add(*vtxMassSignalPdf);
      RooRealVar *bfrac = new RooRealVar(ch+"byields",ch+"byields",bExp/totalExp,0,1.0);    
      flavorFractions.add(*bfrac);
      
      //other flavors
      int icount(0),maxCount(chShapes[is].mass_bckg.size()-1);
      for(map<TString, TH1F *>::iterator it=chShapes[is].mass_bckg.begin(); it != chShapes[is].mass_bckg.end(); it++)
	{
	  TString flav("udsg");
       	  float expFrac(udsgExp/totalExp);
	  if(it->first.BeginsWith("c")) { flav="c"; expFrac=cExp/totalExp; }
       	  if(it->first.BeginsWith("b")) continue;
	  
	  RooRealVar *flavfrac = new RooRealVar(ch+flav+"yields",ch+flav+"yields",expFrac,0.5*expFrac,3*expFrac);
	  if(icount<maxCount-1) { flavorFractions.add(*flavfrac); }

	  // RooGaussian *flavConstraint=new RooGaussian(ch+flav+"yieldsconstr",ch+flav+"yieldsconstr",flavfrac,RooConst(flavfrac->getVal()),RooConst(flavfrac->getVal()/totalObs));
       	  RooDataHist *vtxMassData = new RooDataHist(ch+"_"+flav+"vtxmass", ch+"_"+flav+"vtxmass", RooArgList(secvtxmass),Import(*it->second));
       	  RooHistPdf *vtxMassPdf   = new RooHistPdf(ch+"_"+flav+"vtxmasspdf",ch+"_"+flav+"vtxmasspdf", RooArgSet(secvtxmass),*vtxMassData,smoothOrder);
	  flavorPdfs.add(*vtxMassPdf);
	  icount++;
	}

      //fit flavor fractions to data
      RooAddPdf flavorShapeModelunc(ch+"flavormodelunc", ch+"flavormodelunc", flavorPdfs, flavorFractions);
      flavorShapeModelunc.Print();
      RooFitResult *fitRes=flavorShapeModelunc.fitTo(*mass_data,SumW2Error(kFALSE),Save(kTRUE));
      w->import(flavorShapeModelunc);
      
      float cFrac(w->var(ch+"cyields")->getVal()),cFracErr(w->var(ch+"cyields")->getError()); 
      float bFrac(w->var(ch+"byields")->getVal()),bFracErr(w->var(ch+"byields")->getError()); 
      float rhobc=fitRes->correlation(ch+"cyields",ch+"byields"); 
      float udsgFrac(1-cFrac-bFrac), udsgFracErr(sqrt(pow(cFracErr,2)+pow(bFracErr,2)+2*rhobc*(cFracErr*bFracErr)));

      report << "[" << ch << " flavor fit] observed: " << totalObs << " expected: " << totalExp << endl
	     << "\tc yields: "    << cFrac << "+/-" << cFracErr
	     << "\tSF_c "         << cFrac/(cExp/totalExp) << "+/-" << cFracErr/(cExp/totalExp) <<  endl 
	     << "\tudsg yields: " << udsgFrac << "+/-" << udsgFracErr  
	     << "\tSF_udsg "      << udsgFrac/(udsgExp/totalExp) << "+/-" << udsgFracErr/(udsgExp/totalExp) <<  endl 
	     << "\tb yields: "    << bFrac << "+/-" << bFracErr  
	     << "\tSF_b "         << bFrac/(bExp/totalExp) << "+/-" << bFracErr/(bExp/totalExp) <<  endl
	     << "\t rho(b,c):" << rhobc << endl;
      
      //
      // DATA
      //

      //lxy templates
      RooDataHist *lxy_data = new RooDataHist(ch+"data",ch+"data",lxy,chShapes[is].lxy_data);
      w->import(*lxy_data);
      //lxy.setBins( chShapes[is].lxy_signal.begin()->second->GetXaxis()->GetNbins()/2 );

      // 
      // MC SIGNAL
      //

      //base parameterization of the lxy
      RooRealVar alpha1(ch+"alpha1","#alpha_{1}",0.0, 0.,   1.0); 
      RooRealVar alpha2(ch+"alpha2","#alpha_{2}",0.02,  0.,   0.1);
      RooRealVar alpha3(ch+"alpha3","#alpha_{3}",0.5, 0.,   1.0);
      RooRealVar alpha4(ch+"alpha4","#alpha_{4}",0.02,  0.,   0.1);
      RooFormulaVar pfunc(ch+"pfunc","p","@0+@1*@2",RooArgSet(alpha1,alpha2,mtop)); 
      RooFormulaVar qfunc(ch+"qfunc","q","@0+@1*@2",RooArgSet(alpha3,alpha4,mtop));
      RooRealVar thr(ch+"thr","#lambda",0.11,0.09,0.12);
      RooRealVar wid(ch+"wid","#sigma",0.1,0.05,0.15);
      TString F_form(  "(1.0/qfunc)*(1.0/TMath::Gamma(1+pfunc,thr/qfunc))*pow(var/qfunc,pfunc)*(exp(-var/qfunc)/(1+exp((thr-var)/wid)))"); 
      F_form.ReplaceAll("thr",   "@0");
      F_form.ReplaceAll("wid",   "@1");
      F_form.ReplaceAll("var",   "@2");
      F_form.ReplaceAll("pfunc", "@3");
      F_form.ReplaceAll("qfunc", "@4");
      RooGenericPdf Flxy_pdf(ch+"flxy","",F_form.Data(),RooArgSet(thr,wid,lxy,pfunc,qfunc)); 
      w->import(Flxy_pdf);

      w->factory("EDIT::"+ch+"flxy_base("+ch+"flxy,"+ch+"pfunc="+ch+"p_base[0.02,0.,10],"+ch+"qfunc="+ch+"q_base[0.6,0.,10],"+ch+"thr="+ch+"thr_base[0.11,0.09,0.12],"+ch+"wid="+ch+"wid_base[0.035,0,0.1])");

      const Int_t nq = 4;
      Double_t xq[nq];  // position where to compute the quantiles in [0,1]
      Double_t yq[nq];  // array to contain the quantiles
      for (Int_t i=0;i<nq;i++) xq[i] = Float_t(i+1)/nq;
      TGraph *gr25 = new TGraph;   gr25->SetName(ch+"q25");    gr25->SetTitle("1^{st} quart.");  gr25->SetMarkerStyle(24);    gr25->SetFillStyle(0);
      TGraph *gr50 = new TGraph;   gr50->SetName(ch+"q50");    gr50->SetTitle("median");         gr50->SetMarkerStyle(25);    gr50->SetFillStyle(0);
      TGraph *grmean = new TGraph; grmean->SetName(ch+"mean"); grmean->SetTitle("average");      grmean->SetMarkerStyle(20);  grmean->SetFillStyle(0);
      TGraphErrors *pEvol = new TGraphErrors;
      TGraphErrors *qEvol = new TGraphErrors;
      TGraphErrors *thrEvol = new TGraphErrors;
      TGraphErrors *widEvol = new TGraphErrors;
      for(std::map<Float_t,TH1F *>::iterator it=chShapes[is].lxy_signal.begin();
	  it!=chShapes[is].lxy_signal.end();
	  it++)
	{
	  TString catName(ch); catName += (int)(it->first*10);
	  templateCategories.defineType(catName);
	  templateCategories.setLabel(catName.Data());
	  
	  // add the b-jets from background to the signal!
	  TH1F *h_signal=(TH1F *)it->second->Clone(ch+"blxyh");
	  //h_signal->Add(chShapes[is].lxy_bckg["b"]);
 
	  // store the hitogram pdf in the workspace in order to throw the toy experiments from them and not from the fit model
	  RooDataHist *lxySignalData = new RooDataHist(ch+"lxy_"+catName,    ch+"lxy_"+catName,     RooArgList(lxy), Import(*h_signal));
	  RooHistPdf *lxySignalPdf   = new RooHistPdf(lxySignalData->GetName(), lxySignalData->GetName(), RooArgSet(lxy), *lxySignalData);
	  w->import(*lxySignalPdf);

	  masses[catName.Data()]    = it->first;
	  templates[catName.Data()] = h_signal;
	  rooTemplates[catName.Data()] = new RooDataHist(catName+"templ",catName+"templ",lxy,h_signal);
	  h_signal->GetQuantiles(nq,yq,xq);
	  gr25->SetPoint(gr25->GetN(),it->first,yq[0]);
	  gr50->SetPoint(gr50->GetN(),it->first,yq[1]);
	  grmean->SetPoint(grmean->GetN(),it->first,h_signal->GetMean());

	  w->pdf(ch+"flxy_base")->fitTo(*(rooTemplates[catName.Data()]),SumW2Error(kTRUE));
	  int np=pEvol->GetN();
	  pEvol->SetPoint(np,it->first,w->var(ch+"p_base")->getVal()); pEvol->SetPointError(np,0,w->var(ch+"p_base")->getError());
	  qEvol->SetPoint(np,it->first,w->var(ch+"q_base")->getVal()); qEvol->SetPointError(np,0,w->var(ch+"q_base")->getError());
	  thrEvol->SetPoint(np,it->first,w->var(ch+"thr_base")->getVal());  thrEvol->SetPointError(np,0,w->var(ch+"thr_base")->getError());
	  widEvol->SetPoint(np,it->first,w->var(ch+"wid_base")->getVal());  widEvol->SetPointError(np,0,w->var(ch+"wid_base")->getError());
	}

      RooDataHist combTempl("combTempl","combTempl",RooArgList(lxy),templateCategories,rooTemplates);  
      w->import(RooArgSet(mtop,templateCategories),RenameConflictNodes(ch));

      //clone for each sub-mass and fix mass 
      w->factory("SIMCLONE::flxy_"+ch+"sim( "+ch+"flxy, $SplitParam({mtop},"+ch+"categs) )") ;
      for(std::map<string,float>::iterator mIt = masses.begin(); mIt != masses.end(); mIt++)  
	{
	  string ikey="mtop_"+mIt->first; 
	  RooRealVar *imtop=w->var(ikey.c_str());
	  if(imtop==0) continue;
	  float imass=mIt->second;
	  imtop->setVal(imass);
	  imtop->setConstant(kTRUE);
	}

      
      //now fit the template parameters and plot the results
      float fitrange = 5.0;
      RooFitResult *fitres = w->pdf("flxy_"+ch+"sim")->fitTo(combTempl,Range(0.,fitrange),Save(kTRUE),SumW2Error(kTRUE)); 

      {
	cout <<__LINE__ << endl;
	TCanvas *cpar = new TCanvas("cpar","cpar",600,600);
	cpar->Divide(2,2);
	cpar->cd(1);
	pEvol->Draw("ap");
	pEvol->Fit("pol1"); TF1 *pol1=pEvol->GetFunction("pol1");
  	char buf[200];
  	sprintf(buf,"#alpha_{1}: %f (%f)  #alpha_{2}: %f (%f)",
  		pol1->GetParameter(0),w->var(ch+"alpha1")->getVal(),
  		pol1->GetParameter(1),w->var(ch+"alpha2")->getVal());
  	TPaveText *pave=new TPaveText(0.5,0.5,0.88,0.88,"NDC");
  	pave->AddText(buf);
  	pave->Draw();
	cpar->cd(2);
	qEvol->Draw("ap"); 
	//qEvol->Fit("pol1"); pol1=qEvol->GetFunction("pol1");
//  	sprintf(buf,"#alpha_{3}: %f (%f)  #alpha_{4}: %f (%f)",
//  		pol1->GetParameter(0),w->var(ch+"alpha3")->getVal(),
//  		pol1->GetParameter(1),w->var(ch+"alpha4")->getVal());
//  	pave=new TPaveText(0.5,0.5,0.88,0.88,"NDC");
//  	pave->AddText(buf);
//  	pave->Draw();
	cpar->cd(3);
	thrEvol->Draw("ap");
	// thrEvol->Fit("pol0"); TF1 *pol0=thrEvol->GetFunction("pol0");
//  	pave=new TPaveText(0.5,0.5,0.88,0.88,"NDC");
//  	pave->AddText(buf);
//  	sprintf(buf,"thr: %f (%f)", pol0->GetParameter(0),w->var(ch+"thr")->getVal());
//  	pave->AddText(buf);
//  	pave->Draw();
	cpar->cd(4);
	widEvol->Draw("ap"); widEvol->Fit("pol0");
	// pol0=widEvol->GetFunction("pol0");
// 	pave=new TPaveText(0.5,0.5,0.88,0.88,"NDC");
//  	pave->AddText(buf);
//  	sprintf(buf,"thr: %f (%f)", pol0->GetParameter(0),w->var(ch+"wid")->getVal());
//  	pave->AddText(buf);
//  	pave->Draw();
	cpar->cd();
	cpar->Modified();
	cpar->Update();
	cpar->SaveAs(ch+"cpar.png");
      }

      int ncats=templates.size();
      int npadsx=ncats/2+1;
      int npadsy=ncats/2+1;
      while(npadsx*npadsy>ncats+2) npadsy--;
      TCanvas *c=new TCanvas("c","c",npadsx*400,npadsy*400);
      c->Divide(npadsx,npadsy);
      int ipad(1);
      int npar(0);
      for(std::map<string,float>::iterator mIt=masses.begin(); mIt != masses.end(); mIt++,ipad++)
	{
	  c->cd(ipad);
	  string key=mIt->first;
	  //float imass=mIt->second;

	  RooPlot* frame = lxy.frame();

	  //Project the dataset for a given category
	  TString cut=ch+"categs=="+ch+"categs::"+key;
	  RooDataHist* dataslice = (RooDataHist *)combTempl.reduce(cut.Data());
	  dataslice->plotOn(frame,DataError(RooAbsData::SumW2));

	  //get the PDF corresponding to the required category
	  RooCategory theCategory(key.c_str(),key.c_str());
	  // w->pdf("flxy_"+ch+"sim")->plotOn(frame,ProjWData(lxy,*dataslice),MoveToBack(),Range(0.,fitrange));
	  w->pdf("flxy_"+ch+"sim")->plotOn(frame,ProjWData(lxy,*dataslice),MoveToBack());

	  frame->Draw();
	  frame->GetXaxis()->SetTitle("SecVtx L_{xy} [cm]");
	  frame->GetYaxis()->SetTitle("Jets");
	  frame->SetMinimum(0.001);
	  
	  if(ipad==1)
	    {
	      fitres->Print("v");
	      TPaveText *pave = new TPaveText(0.5,0.5,0.88,0.88,"NDC");
	      pave->SetBorderSize(0);
	      pave->SetFillStyle(0);
	      pave->SetTextAlign(12);
	      pave->SetTextFont(42);
	      pave->AddText("Fit results");
	      RooArgList varSet=fitres->floatParsFinal();
	      //RooArgSet varSet=w->allVars();
	      TIterator *iter=varSet.createIterator();
	      RooRealVar *var;
	      while((var=(RooRealVar*)iter->Next())) {
		if(var->getError()==0) continue;
		TString varName(var->GetName()); 
		if(!varName.BeginsWith(ch)) continue;
		if(varName.Contains("mtop")) continue;
		char line[100];
		sprintf(line,"%s=%3.4f #pm %3.4f", var->GetTitle(),var->getVal(),var->getError());
		pave->AddText(line); 
		npar++;
	      }
	      pave->Draw();
	    }
	  
	  TPaveText *pave = new TPaveText(0.1,0.9,0.4,0.95,"NDC");
	  pave->SetBorderSize(0);
	  pave->SetFillStyle(0);
	  pave->SetTextAlign(12);
	  pave->SetTextFont(42);
	  char buf[200]; sprintf(buf,"m_{t}^{gen}=%3.1f",mIt->second);
	  pave->AddText(buf);
	  pave->Draw();

	  pave = new TPaveText(0.65,0.9,0.9,0.95,"NDC");
	  pave->SetBorderSize(0);
	  pave->SetFillStyle(0);
	  pave->SetTextAlign(32);
	  pave->SetTextFont(42);
	  float chi2=frame->chiSquare(0); //npar);
	  //int myndof = frame->GetNbinsX() - npar;
	  sprintf(buf,"#chi^{2} %3.2f",chi2);
	  pave->AddText(buf);
	  pave->Draw();
	  
	  pave->SetBorderSize(0);
	  pave->SetFillStyle(0);
	  pave->SetTextFont(42);
	  pave->Draw();
	}

      c->SaveAs("SignalPDFs_"+ch+".png");  
      
      //draw the evolution of the momenta of the Lxy distribution
      //create the inversion formulae based on the momenta
      cqt->cd(is+1);
      TGraph *grframe=new TGraph;
      grframe->SetPoint(0,160,0.2); grframe->SetPoint(1,185,1.8); grframe->Draw("ap");
      grframe->GetXaxis()->SetTitle("m_{top} [GeV]");
      grframe->GetYaxis()->SetTitle("SecVtx L_{xy} momentum");

      grmean->Draw("p");  grmean->Fit("pol1");
      RooFormulaVar *invForm=new RooFormulaVar(ch+"meanInv",
					       "(@0-@1)/@2",
					       RooArgSet(lxy,RooConst(grmean->GetFunction("pol1")->GetParameter(0)),RooConst(grmean->GetFunction("pol1")->GetParameter(1))));
      w->import(*invForm);

      gr25->Draw("p");    gr25->Fit("pol1");  
      invForm = new RooFormulaVar(ch+"25qInv",
				  "(@0-@1)/@2",
				  RooArgSet(lxy,RooConst(gr25->GetFunction("pol1")->GetParameter(0)),RooConst(gr25->GetFunction("pol1")->GetParameter(1))));
      w->import(*invForm);

      gr50->Draw("p");    gr50->Fit("pol1");
      invForm = new RooFormulaVar(ch+"50qInv",
				  "(@0-@1)/@2",
				  RooArgSet(lxy,RooConst(gr50->GetFunction("pol1")->GetParameter(0)),RooConst(gr50->GetFunction("pol1")->GetParameter(1))));
      w->import(*invForm);

      TPaveText *pave = new TPaveText(0.65,0.9,0.9,0.95,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->SetTextAlign(32);
      pave->SetTextFont(42);
      TString title(ch); title.ReplaceAll("mu","#mu");
      pave->AddText(title);
      pave->Draw();
      if(is==0)
	{
	  TLegend *leg=new TLegend(0.5,0.8,0.9,0.9);
	  leg->AddEntry(grmean,grmean->GetTitle(),"p");
	  leg->AddEntry(gr25,gr25->GetTitle(),"p");  
	  leg->AddEntry(gr50,gr50->GetTitle(),"p");    
	  leg->SetBorderSize(0);
	  leg->SetFillStyle(0);
	  leg->SetTextFont(42);
	  leg->SetNColumns(3);
	  leg->Draw();
	}
      
      //
      // BACKGROUND
      //
      
      for(map<TString, TH1F *>::iterator it=chShapes[is].lxy_bckg.begin(); it != chShapes[is].lxy_bckg.end(); it++)
	{
	  if(it->first.BeginsWith("b")) continue;

	  RooDataHist *lxyData = new RooDataHist(ch+"lxy_"+it->first,    ch+"lxy_"+it->first,     RooArgList(lxy), Import(*it->second));
	  RooHistPdf *lxyPdf   = new RooHistPdf(lxyData->GetName()+TString("bkg"), lxyData->GetName()+TString("bkg"), RooArgSet(lxy), *lxyData);
	  w->import(*lxyPdf);
	}
      
      // sum the backgrounds: SF_c x c + SF_udsg x udsg
      TH1F *h_bkg=(TH1F *)chShapes[is].lxy_bckg["c"]->Clone(ch+"lxy_bkgh");
      h_bkg->Scale( cFrac/(cExp/totalExp) );
      h_bkg->Add(chShapes[is].lxy_bckg["udsg"], udsgFrac/(bExp/totalExp));
      RooDataHist *lxyData = new RooDataHist(ch+"lxy_bkg",    ch+"lxy_bkg",     RooArgList(lxy), Import(*h_bkg));
      RooHistPdf *lxyPdf   = new RooHistPdf(lxyData->GetName(), lxyData->GetName(), RooArgSet(lxy), *lxyData);
      w->import(*lxyPdf);

      w->factory("EDIT::"+ch+"flxy_bkg("+ch+"flxy,"+ch+"pfunc="+ch+"beta1[0.02,0.,10],"+ch+"qfunc="+ch+"beta2[0.6,0.,10],"+ch+"thr="+ch+"thr_bckg[0.11],"+ch+"wid="+ch+"wid_bkg[0.035,0,0.1])");
      w->var(ch+"beta1")->SetTitle("p");
      w->var(ch+"beta2")->SetTitle("q");
      w->var(ch+"thr_bckg")->SetTitle("#lambda");
      w->var(ch+"wid_bkg")->SetTitle("#sigma");
      fitRes=w->pdf(ch+"flxy_bkg")->fitTo(*lxyData,Save(kTRUE),SumW2Error(kTRUE));
      
      TCanvas *cbkg=new TCanvas("cbkg","cbkg",500,500);
      RooPlot* frame = lxy.frame();
      lxyData->plotOn(frame,DataError(RooAbsData::SumW2));
      w->pdf(ch+"flxy_bkg")->plotOn(frame,ProjWData(lxy,*lxyData),MoveToBack());
      frame->Draw();
      frame->SetMinimum(0.001);
      TPaveText *pavebkg = new TPaveText(0.2,0.9,0.9,0.95,"NDC");
      pavebkg->SetBorderSize(0);
      pavebkg->SetFillStyle(0);
      pavebkg->SetTextFont(42);
      TString chTitle(ch); chTitle.ReplaceAll("mu","#mu"); chTitle += " background";
      pavebkg->AddText(chTitle);
      pavebkg->Draw();

      pavebkg = new TPaveText(0.65,0.9,0.9,0.95,"NDC");
      pavebkg->SetBorderSize(0);
      pavebkg->SetFillStyle(0);
      pavebkg->SetTextAlign(32);
      pavebkg->SetTextFont(42);
      float chi2=frame->chiSquare(0);
      char buf[200];
      sprintf(buf,"#chi^{2} %3.2f",chi2);
      pavebkg->AddText(buf);
      pavebkg->Draw();
      
      fitres->Print("v");
      pavebkg = new TPaveText(0.5,0.5,0.88,0.88,"NDC");
      pavebkg->AddText("Fit results");
      pavebkg->SetBorderSize(0);
      pavebkg->SetFillStyle(0);
      pavebkg->SetTextAlign(12);
      pavebkg->SetTextFont(42);
      RooArgList varSet=fitRes->floatParsFinal();
      TIterator *iter=varSet.createIterator();
      RooRealVar *var;
      while((var=(RooRealVar*)iter->Next())) {
	if(var->getError()==0) continue;
	TString varName(var->GetName()); 
	if(!varName.BeginsWith(ch)) continue;
	if(varName.Contains("mtop")) continue;
	char line[100];
	sprintf(line,"%s=%3.4f #pm %3.4f", var->GetTitle(),var->getVal(),var->getError());
	pavebkg->AddText(line); 
	npar++;
      }
      pavebkg->Draw();

      cqt->SaveAs("SignalPDFsMomenta.png");
      cqt->SaveAs("SignalPDFsMomenta.C");
      cbkg->SaveAs("SecVtxBckg_"+ch+".png");

      //construct the model according to the fractions from the mass fit
      w->var(ch+"byields")->setConstant(kTRUE);
      RooAddPdf *model = new RooAddPdf(ch+"model",ch+"model",RooArgList(*w->pdf(ch+"flxy"),*w->pdf(ch+"flxy_bkg")),RooArgList(*w->var(ch+"byields")));
      w->import(*model);
      
      //
      // SYSTEMATICS
      //
      for(std::map<Float_t, std::map<TString, TH1F *> >::iterator it=chShapes[is].lxy_signalSysts.begin(); it != chShapes[is].lxy_signalSysts.end(); it++)
	{
	  for(map<TString, TH1F *>::iterator iit=it->second.begin(); iit != it->second.end(); iit++)
	    {
	      // add the b-jets from background to the signal!
	      TH1F *h_signal=(TH1F *) iit->second->Clone(ch+iit->first+"bh");
	      //h_signal->Add(chShapes[is].lxy_bckg["b"]);
	      RooDataHist *lxySystData = new RooDataHist(ch+"lxy_"+iit->first,    ch+"lxy_"+iit->first,     RooArgList(lxy), Import(*h_signal));
	      RooHistPdf *lxySystPdf   = new RooHistPdf(lxySystData->GetName()+TString("syst"), lxySystData->GetName()+TString("syst"), RooArgSet(lxy), *lxySystData);
	      w->import(*lxySystPdf);      
	    }
	}
    }


  //all done here  
  w->Print("v");
  return w;
}


//
int main(int argc, char *argv[])
{
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

  // load framework libraries 
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  //keep RooFit quiet
  RooMsgService::instance().setSilentMode(true);
  RooMsgService::instance().getStream(0).removeTopic(Minimization);
  RooMsgService::instance().getStream(1).removeTopic(Minimization);
  RooMsgService::instance().getStream(1).removeTopic(Fitting);
  RooMsgService::instance().getStream(1).removeTopic(Eval);
  RooMsgService::instance().getStream(1).removeTopic(Integration);
  RooMsgService::instance().getStream(1).removeTopic(NumIntegration);
  
  // initialize command line parser                  
  optutl::CommandLineParser parser ("[FitSecVtxDistrutions]");
  parser.addOption("templ",   optutl::CommandLineParser::kString, "input file with templates");
  parser.addOption("ws",      optutl::CommandLineParser::kString, "input file with pre-defined workspace");
  parser.stringValue("ws")="";
  parser.parseArguments (argc, argv);
  std::string templateInputFile = parser.stringValue("templ");
  std::string wsInUrl           = parser.stringValue("ws");

  //
  // READ HISTOGRAMS FROM INPUT FILE AND DEFINE CATEGORIES
  //
  cout << "[FitSecVtxDistributions] templates will be retrieved from " << templateInputFile << endl;
  std::vector<SecVtxShape_t> chShapes;
  TFile *fIn=TFile::Open(templateInputFile.c_str()); 
  TIter nextCh(fIn->GetListOfKeys()); 
  TObject *chDir=nextCh();
  while(chDir!=0) 
    { 
      //the channel is the directory name
      TString ch(chDir->GetName());
      SecVtxShape_t m_shape(ch);
      
      //parse the templates for this channel
      TDirectory *dir = (TDirectory *)fIn->Get(ch);
      TIter nextTempl(dir->GetListOfKeys());
      TObject *nextTemplKey=nextTempl();
      while(nextTemplKey!=0)
	{
	  TString keyStr(nextTemplKey->GetName());
	  TH1F *h=(TH1F *) fIn->Get(ch+"/"+keyStr);
	  h->SetDirectory(0);

	  //force minimal counts in all bins (RooFit bug)
	  for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
	    {
	      if(h->GetBinContent(ibin)>0) continue;
	      h->SetBinContent(ibin,0.00001);
	    }
	  
	  //parse the name of the histo
	  TObjArray *tkns=keyStr.Tokenize("_");
	  TString ch      = tkns->At(0)->GetName();
	  TString var     = tkns->At(1)->GetName();
	  TString jetFlav("");
	  if(var.BeginsWith("b"))     { jetFlav="b";      h->SetLineColor(1); h->SetMarkerColor(596); h->SetFillColor(596); h->SetFillStyle(1001); }
	  if(var.BeginsWith("c"))     { jetFlav="c";      h->SetLineColor(1); h->SetMarkerColor(592); h->SetFillColor(592); h->SetFillStyle(1001); }
	  if(var.BeginsWith("udsg"))  { jetFlav="udsg";   h->SetLineColor(1); h->SetMarkerColor(590); h->SetFillColor(590); h->SetFillStyle(1001); }
	  TString proc    = tkns->At(2)->GetName();
	  TString syst("");
	  TObjArray *systTkns=proc.Tokenize("syst");
	  if(systTkns->GetEntriesFast()>1) syst=systTkns->At(1)->GetName();
	  if(systTkns->GetEntriesFast()>2) syst=syst+systTkns->At(2)->GetName();
	  TString massStr = tkns->At(3)->GetName(); 
	  float mass      = massStr.Atof()/10.;
	  if(mass != 0 ) { h->SetLineColor(1); h->SetMarkerColor(1); h->SetFillColor(804); h->SetFillStyle(1001); } 

	  //save in shape
	  if(proc=="data")
	    {
	      if(var.Contains("lxy"))  m_shape.lxy_data=h;
	      if(var.Contains("mass")) m_shape.mass_data=h;
	    }
	  else if(mass!=0 && jetFlav=="b")
	    {
	      if(syst=="")
	  	{
	  	  if(var.Contains("lxy"))  m_shape.lxy_signal[mass]=h;
	  	  if(var.Contains("mass")) m_shape.mass_signal[mass]=h;
	  	}
	      else
	  	{
		  std::cout << syst << std::endl;
		  if(var.Contains("lxy"))  m_shape.lxy_signalSysts[mass][syst]=h;
	  	  if(var.Contains("mass")) m_shape.mass_signalSysts[mass][syst]=h;
	  	}
	    }
	  else if((mass==0 || (mass==172.5 && syst=="") ) && jetFlav!="")
	    {
	      if(var.Contains("lxy"))
		{
		  if(m_shape.lxy_bckg.find(jetFlav)==m_shape.lxy_bckg.end()) m_shape.lxy_bckg[jetFlav]=h;
		  else                                                       m_shape.lxy_bckg[jetFlav]->Add(h);
		}
	      if(var.Contains("mass"))
		{
		  if(m_shape.mass_bckg.find(jetFlav)==m_shape.mass_bckg.end()) m_shape.mass_bckg[jetFlav]=h, std::cout << jetFlav << "/" << var << std::endl;
		  else                                                         m_shape.mass_bckg[jetFlav]->Add(h), std::cout << jetFlav<< ":" << h->Integral() << std::endl;
		}
	    }


	
	  nextTemplKey=nextTempl();
	}

      chShapes.push_back(m_shape);
      chDir=nextCh();
    }
  fIn->Close();

  cout << "[FitSecVtxDistributions] defining workspace" << endl;
  RooWorkspace *w=0;
  if(wsInUrl=="")
    {
      w=defineWorkspace(chShapes);
      w->writeToFile("FitSecVtxDistributionsWS.root",kTRUE);
    }
  else
    {
      TFile *inF=TFile::Open(wsInUrl.c_str());
      w = new RooWorkspace( *((RooWorkspace *)inF->Get("w") ) );
      inF->Close();
    }

  cout << "[FitSecVtxDistributions] saving pre-fit distributions" << endl;
  showSecVtxShapeCollection(w,chShapes);

  cout << report.str() << endl;
}
