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

//
void showSecVtxShapeCollection(RooWorkspace *w, std::vector<SecVtxShape_t> &shapes)
{
  if(shapes.size()==0) return;

  TCanvas *c=new TCanvas("c","c",500*shapes.size(),500);
  c->Divide(shapes.size(),1);

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
      w->pdf(ch+"model")->plotOn(frame,                                               RooFit::FillStyle(0),    RooFit::LineWidth(2),        MoveToBack(), DrawOption("lf"), Name("signal"));
      w->pdf(ch+"model")->plotOn(frame, RooFit::Components(* w->pdf( ch+"lxy_bckg")), RooFit::FillStyle(1001), RooFit::FillColor(kAzure+9), MoveToBack(), DrawOption("lf"), Name("bckg"));

      frame->SetTitleOffset(1.0,"Y");
      frame->SetTitleSize(0.05,"Y");
      frame->SetYTitle("Jets");
      frame->SetTitleSize(0.05,"X");
      frame->SetXTitle("SecVtx L_{xy} [cm]");
      frame->SetMinimum(0.01);
      frame->Draw();

      TPaveText *pave = new TPaveText(0.15,0.9,0.4,0.94,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->SetTextAlign(12);
      pave->SetTextFont(42);
      pave->AddText(chTitle);
      pave->Draw();

      if(is==0)
	{
	  TLegend *leg=new TLegend(0.15,0.85,0.9,0.9);
	  leg->SetNColumns(3);
	  leg->SetBorderSize(0);
	  leg->SetFillStyle(0);
	  leg->SetTextFont(42);
	  leg->SetTextAlign(12);
	  leg->AddEntry("data","data","p");
	  leg->AddEntry("signal","t#bar{t}","F");
	  leg->AddEntry("bckg","#Sigma bckg", "F");
	  leg->Draw();

	  pave = new TPaveText(0.1,0.95,0.9,0.99,"NDC");
	  pave->SetBorderSize(0);
	  pave->SetFillStyle(0);
	  pave->SetTextAlign(12);
	  pave->AddText("CMS preliminary");
	  pave->Draw();
	}
    }

  c->Modified();
  c->Update();
  c->SaveAs("SecVtx.png");
  c->SaveAs("SecVtx.pdf");
}

//
RooWorkspace *defineWorkspace(std::vector<SecVtxShape_t> &chShapes)
{
  RooWorkspace* w = new RooWorkspace("w") ; 

  //the main incognitas
  RooRealVar mtop("mtop","m_{top} [GeV]",173.5,100,200);
  w->import(mtop);
  RooRealVar xsec("xsec","#sigma_{t#bar{t}} [pb]",220,0,1000);

  //the observables
  RooRealVar lxy("lxy","L_{xy} [cm]",0,5) ;
  lxy.setBins( chShapes[0].lxy_data->GetXaxis()->GetNbins() );
 
  TCanvas *cqt=new TCanvas("cqt","cqt",chShapes.size()*400,400);
  cqt->Divide(chShapes.size(),1);
  
  //iterate over the different channels and build the individual lxy templates
  for(size_t is=0; is<chShapes.size(); is++)
    {
      TString ch=chShapes[is].m_tag;
      
      //
      // DATA
      //
      RooDataHist *dataHist = new RooDataHist(ch+"data",ch+"data",lxy,chShapes[is].lxy_data);
      w->import(*dataHist);
      Double_t nData=chShapes[is].lxy_data->Integral(0,chShapes[is].lxy_data->GetNbinsX()+1);
      Double_t dataBinWidth(chShapes[is].lxy_data->GetBinWidth(1));

      //
      // BACKGROUND
      //
      TH1F *bckgH=0;
      for(map<TString, TH1F *>::iterator it=chShapes[is].lxy_bckg.begin(); it != chShapes[is].lxy_bckg.end(); it++)
	{
	  Double_t bckgBinWidth(it->second->GetBinWidth(1));
	  if(bckgBinWidth!=dataBinWidth) it->second->Rebin(int(dataBinWidth/bckgBinWidth));

	  if(bckgH==0) bckgH=(TH1F *)it->second->Clone(ch+"lxy_bckg");
	  else         bckgH->Add(it->second);
	}
      Double_t bckgExp=bckgH->Integral(0,bckgH->GetNbinsX()+1);
      RooDataHist *lxyBckgHist = new RooDataHist(ch+"lxy_bckgtempl", ch+"lxy_bckgtempl", RooArgList(lxy), Import(*bckgH) );
      RooHistPdf *lxyBckgPdf   = new RooHistPdf(ch+"lxy_bckg",       ch +"lxy_bckg",     RooArgSet(lxy), *lxyBckgHist);
      w->import(*lxyBckgPdf);

        
      // 
      // MC SIGNAL
      //
      //TH1F *nominalSignalH = chShapes[is].lxy_signal[172.5];
      Double_t signalExp   = nData-bckgExp; //nominalSignalH->Integral(0,nominalSignalH->GetNbinsX()+1);
      cout << ch << " " << signalExp << " " << bckgExp << " " << nData << endl;
      const Int_t nq = 4;
      Double_t xq[nq];  // position where to compute the quantiles in [0,1]
      Double_t yq[nq];  // array to contain the quantiles
      for (Int_t i=0;i<nq;i++) xq[i] = Float_t(i+1)/nq;
      TGraph *gr25 = new TGraph;   gr25->SetName(ch+"q25");    gr25->SetTitle("1^{st} quart.");  gr25->SetMarkerStyle(24);    gr25->SetFillStyle(0);
      TGraph *gr50 = new TGraph;   gr50->SetName(ch+"q50");    gr50->SetTitle("median");         gr50->SetMarkerStyle(25);    gr50->SetFillStyle(0);
      TGraph *grmean = new TGraph; grmean->SetName(ch+"mean"); grmean->SetTitle("average");      grmean->SetMarkerStyle(20);  grmean->SetFillStyle(0);
      for(std::map<Float_t,TH1F *>::iterator it=chShapes[is].lxy_signal.begin(); it!=chShapes[is].lxy_signal.end(); it++)
	{
	  Double_t sigBinWidth(it->second->GetBinWidth(1));
	  if(sigBinWidth!=dataBinWidth) it->second->Rebin(int(dataBinWidth/sigBinWidth));
	  it->second->Scale(signalExp/it->second->Integral(0,it->second->GetNbinsX()+1));
	  
	  // store the hitogram pdf in the workspace in order to throw the toy experiments from them and not from the fit model
	  TString name(ch+"lxy_"); name += (int)(it->first*10);
	  RooDataHist *lxySignalHist = new RooDataHist(name +"templ", name+"templ", RooArgList(lxy), Import(*(it->second)) );
	  RooHistPdf *lxySignalPdf   = new RooHistPdf(name, name, RooArgSet(lxy), *lxySignalHist);
	  w->import(*lxySignalPdf);
	  
	  // Add the background to the signal for the calibration  
	  TH1F *h_signalPlusBkg=(TH1F *)it->second->Clone(ch+"spblxy");
	  for(map<TString, TH1F *>::iterator bit=chShapes[is].lxy_bckg.begin(); bit != chShapes[is].lxy_bckg.end(); bit++)
	    h_signalPlusBkg->Add( bit->second); 

	  //compute the quantiles and fill the calibration curves
	  h_signalPlusBkg->GetQuantiles(nq,yq,xq);
	  Int_t np=gr25->GetN();
	  gr25->SetPoint(np,  it->first,yq[0]);
	  gr50->SetPoint(np,  it->first,yq[1]);
	  grmean->SetPoint(np,it->first,h_signalPlusBkg->GetMean());
	}
     

      //
      // COMBINED MODEL : NEEDS WORK HERE TO MORPH THE SIGNAL, FOR NOW USE FIXED MASS
      //	
      RooRealVar *sigfrac =new RooRealVar(ch+"sigfrac",ch+"sigfrac",signalExp/(signalExp+bckgExp),0,1.0);
      RooAddPdf *model = new RooAddPdf(ch+"model",ch+"model",RooArgList(*w->pdf(ch+"lxy_1725"),*w->pdf(ch+"lxy_bckg")),*sigfrac);
      w->import(*model);

      //draw the evolution of the momenta of the Lxy distribution
      //create the inversion formulae based on the momenta
      cqt->cd(is+1);
      TGraph *grframe=new TGraph;
      grframe->SetPoint(0,160,0.2); grframe->SetPoint(1,185,1.4); grframe->Draw("ap");
      grframe->GetXaxis()->SetTitle("m_{top} [GeV]");
      grframe->GetYaxis()->SetTitle("SecVtx L_{xy} momentum [cm]");

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
      // SYSTEMATICS
      //
      for(std::map<Float_t, std::map<TString, TH1F *> >::iterator it=chShapes[is].lxy_signalSysts.begin(); it != chShapes[is].lxy_signalSysts.end(); it++)
	{
	  for(map<TString, TH1F *>::iterator iit=it->second.begin(); iit != it->second.end(); iit++)
	    {
	      // add the b-jets from background to the signal!
	      iit->second->Scale(signalExp/iit->second->Integral(0,iit->second->GetNbinsX()));
	      RooDataHist *lxySystData = new RooDataHist(ch+"lxy_"+iit->first,    ch+"lxy_"+iit->first,     RooArgList(lxy), Import(*(iit->second)) );
	      RooHistPdf *lxySystPdf   = new RooHistPdf(lxySystData->GetName()+TString("syst"), lxySystData->GetName()+TString("syst"), RooArgSet(lxy), *lxySystData);
	      w->import(*lxySystPdf);      
	    }
	}

    }
  cqt->SaveAs("SignalPDFsMomenta.png");
  cqt->SaveAs("SignalPDFsMomenta.pdf");

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

	  //force minimal counts in all bins (RooFit limitation)
	  TH1F *h=(TH1F *) fIn->Get(ch+"/"+keyStr);
	  h->SetDirectory(0);
	  for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
	    {
	      if(h->GetBinContent(ibin)>0) continue;
	      h->SetBinContent(ibin,0.00001);
	    }
	  
	  //parse the name of the histo
	  TObjArray *tkns = keyStr.Tokenize("_");
	  TString ch      = tkns->At(0)->GetName();
	  TString var     = tkns->At(1)->GetName();
	  TString jetFlav("");
	  if(var.BeginsWith("b"))     { jetFlav="b";      h->SetLineColor(1); h->SetMarkerColor(596); h->SetFillColor(596); h->SetFillStyle(1001); }
	  if(var.BeginsWith("c"))     { jetFlav="c";      h->SetLineColor(1); h->SetMarkerColor(592); h->SetFillColor(592); h->SetFillStyle(1001); }
	  if(var.BeginsWith("udsg"))  { jetFlav="udsg";   h->SetLineColor(1); h->SetMarkerColor(590); h->SetFillColor(590); h->SetFillStyle(1001); }
	  TString proc    = tkns->At(2)->GetName();
	  TString syst("");
	  if(proc.Contains("syst")){
	    proc=proc.ReplaceAll("syst"," ");
	    Ssiz_t idx=proc.First(" ");
	    syst = proc(idx+1,proc.Length());
	    proc = proc(0,idx);
	  }
	  TString massStr = tkns->At(3)->GetName(); 
	  float mass      = massStr.Atof()/10.;
	  if(proc=="ttbarv" || proc=="otherttbar") mass=0;
	  if(mass != 0 )              { h->SetLineColor(1); h->SetMarkerColor(1); h->SetFillColor(804); h->SetFillStyle(1001); } 
	  
	  //some are not interesting for us
	  if((var.Contains("eta") || var.Contains("pt") || var.Contains("presel") || var.Contains("Count")|| var.Contains("PV") || var.Contains("phi") || var.Contains("njets") || var.Contains("1jet") )
	     || (var.Contains("jet") && !var.Contains("lxy") && !var.Contains("mass")) ){
	    nextTemplKey=nextTempl();
	    continue;
	  }

	  //save to shape
	  if(proc=="data")
	    {
	      if(var.Contains("lxy"))  m_shape.lxy_data=h, std::cout << "data hist(" << var <<"): " << h->Integral(0,h->GetNbinsX()+1) << std::endl;
	      if(var.Contains("mass")) m_shape.mass_data=h;
	    }
	  else if(mass!=0 && jetFlav=="")
	    {
	      if(syst=="")
	  	{
	  	  if(var.Contains("lxy"))  m_shape.lxy_signal[mass]=h; //, std::cout << mass << " " << h->GetName() << " " << h->GetBinWidth(1) << std::endl; //, std::cout << "signal (" << mass << ") hist: " << h->Integral(0,h->GetNbinsX()+1) << std::endl;
	  	  if(var.Contains("mass")) m_shape.mass_signal[mass]=h;
	  	}
	      else
	  	{
		  if(var.Contains("lxy"))  m_shape.lxy_signalSysts[mass][syst]=h;
	  	  if(var.Contains("mass")) m_shape.mass_signalSysts[mass][syst]=h;
	  	}
	    }
	  else if(mass==0 && jetFlav=="" && syst=="")
	    {
	      if(var.Contains("lxy"))
		{
		  if(m_shape.lxy_bckg.find(jetFlav)==m_shape.lxy_bckg.end()) m_shape.lxy_bckg[jetFlav]=h; //, std::cout << "bkg hist: " << h->Integral(0,h->GetNbinsX()+1) << std::endl;
		  else                                                       m_shape.lxy_bckg[jetFlav]->Add(h);
		}
	      if(var.Contains("mass"))
		{
		  if(m_shape.mass_bckg.find(jetFlav)==m_shape.mass_bckg.end()) m_shape.mass_bckg[jetFlav]=h; //, std::cout << jetFlav << "/" << var << std::endl;
		  else                                                         m_shape.mass_bckg[jetFlav]->Add(h); //, std::cout << jetFlav<< ":" << h->Integral() << std::endl;
		}
	    }
	  
	  //move to the next one
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
  showSecVtxShapeCollection(w,chShapes);
}
