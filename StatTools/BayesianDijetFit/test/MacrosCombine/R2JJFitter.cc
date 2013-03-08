/** \macro H2GGFitter.cc
 *
 * $Id$
 *
 * Software developed for the CMS Detector at LHC
 *
 *
 *  \author Serguei Ganjour - CEA/IRFU/SPP, Saclay
 *  \modified by Maxime Gouzevitch for the Dijet Bump Search - IPNL, Lyon 
 *
 * Macro is implementing the unbinned maximum-likelihood model for 
 * the Higgs to gamma gamma analysis. PDF model and RooDataSets 
 * are stored in the workspace which is feeded to  HiggsAnalysis/CombinedLimit tools:
 * 
 * http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HiggsAnalysis/CombinedLimit
 * http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HiggsAnalysis/CombinedLimit/data/lhc-hcg/cms-hgg-1fb/
 * 
 * The analysis root trees produced in a simple format 
 *
 *     TFile file(filename,"RECREATE", "X->jj input tree for unbinned maximum-likelihood fit");
 *     TTree* outTree  = new TTree("XTojj","X->jj input tree for unbinned maximum-likelihood fit");
 *     Float_t mass;
 *     Int_t CAT3;
 *     Float_t weight;
 *
 *     outTree->Branch("mass",&mass,"mass/F");
 *     outTree->Branch("weight",&weight,"weight/F");
 *     outTree->Branch("CAT4",&CAT4,"CAT4/I");
 *     {
 *       .............
 *       outTree->Fill();
 *     }
 *
 *     file.Write();
 *     file.Close();
 *     delete outTree;
 *
 * are used as input files. They have to be produced for 
 * data and Monte Carlo signal and background data sets 
 * after all analysis selections to be applied. It is recommended to put   
 * loose kinematical cuts on pt1 and pt2 (20 GeV) since further selections 
 * are possible based on RooDataSets. 
 * It is recommended to use Root 5.28/00 (CMSSW_4_1_3).
 *
 *
 */
// Loading:  .L H2GGFitter.cc
// Running:  runfits("hgg120-shapes-combined-Unbinned.root")  
//                

/*
#include <cstring>
#include <cerrno>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <unistd.h>
#include <errno.h>
#include <iomanip>
// ROOT headers
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TIterator.h"

#include "TLatex.h"
#include "TString.h"
#include "TLegend.h"
#include "TGraphAsymmErrors.h"


// RooFit headers
#include "RooAbsPdf.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooHistPdf.h"
#include "RooMsgService.h"
#include "RooNLLVar.h"
#include "RooPlot.h"
#include "RooRandom.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"
#include "TStyle.h"

// RooStats headers
#include "RooStats/HLFactory.h"

#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooAbsData.h"
#include "RooPlot.h"
#include "RooGaussian.h"
#include "RooProduct.h"
#include "RooExtendPdf.h"
#include "RooBernstein.h"
#include "RooFitResult.h"
#include "RooMinimizer.h"
#include "RooCmdArg.h"
#include "RooConstVar.h"
#include "RooRealVar.h"
*/
#include "HiggsCSandWidth.h"
#include "HiggsCSandWidth.cc"
//#include "RooPower.h"

using namespace RooFit;
using namespace RooStats ;

static const Int_t NCAT = 6;

void AddSigData(RooWorkspace*, Float_t);
void AddBkgData(RooWorkspace*);
void SigModelFit(RooWorkspace*, Float_t);
RooFitResult*  BkgModelFitBernstein(RooWorkspace*, Bool_t);
void MakePlots(RooWorkspace*, Float_t, RooFitResult* );
void MakeSigWS(RooWorkspace* w, const char* filename);
void MakeBkgWS(RooWorkspace* w, const char* filename);
void MakeDataCard(RooWorkspace* w, const char* filename);
void SetParamNames(RooWorkspace*);
void SetConstantParams(const RooArgSet* params);


RooArgSet* defineVariables()
{
  // define variables of the input ntuple
  RooRealVar* mgg  = new RooRealVar("mgg","M(jet-jet)",1000,5000,"GeV");
  mgg->setBins(2000);
  RooRealVar* evWeight   = new RooRealVar("evWeight","Reweightings",0,100,"");
  RooRealVar* normWeight  = new RooRealVar("normWeight","Additionnal Weight",0,10000000,"");
  RooCategory* categories = new RooCategory("categories","event category 6") ;
  categories->defineType("dijet_mass_2mtag_0mdtag",0);
  categories->defineType("dijet_mass_2mtag_1mdtag",1);
  categories->defineType("dijet_mass_2mtag_2mdtag",2);
  categories->defineType("dijet_mass_1mtag_0mdtag",3);
  categories->defineType("dijet_mass_1mtag_1mdtag",4);
  categories->defineType("dijet_mass_0mtag",5);

  RooArgSet* ntplVars = new RooArgSet(*mgg, *categories, *evWeight, *normWeight);
 
  return ntplVars;
}



void runfits(const Float_t mass=2000, Bool_t dobands = false)
{

//******************************************************************//
//  Running mode  corresponds to the following cases
//         - full run set:
//         - create signal and background data sets 
//         - make and fit signal and background  models 
//         - write signal and background workspaces in root files
//         - write data card

//*******************************************************************//


  TString fileBaseName(TString::Format("Xvv.mX%.1f_8TeV", mass));

  TString fileBkgName(TString::Format("Xvv.inputbkg_8TeV", mass));
  TString card_name("Xvv_models_Bkg_8TeV_test.rs");
  HLFactory hlf("HLFactory", card_name, false);
  RooWorkspace* w = hlf.GetWs();

  RooFitResult* fitresults;

// Add data to the workspace
    AddSigData(w, mass);
    AddBkgData(w);
    
// Add the signal and background models to the workspace.
// Inside this function you will find a discription our model.
// Fit data with models
    SigModelFit(w, mass);
    
    fitresults = BkgModelFitBernstein(w, dobands);
      
// Make statistical treatment
// Setup the limit on Higgs production

    MakeSigWS(w, fileBaseName);
    MakeBkgWS(w, fileBkgName);
    MakeDataCard(w, fileBaseName, fileBkgName);

    int ncat = NCAT;
    for (int c = 0; c < ncat; c++) MakeDataCard_1Channel(w, fileBaseName, fileBkgName, c);

// Make plots for data and fit results
    MakePlots(w, mass, fitresults);
  return;

}



void AddSigData(RooWorkspace* w, Float_t mass) {

  Int_t ncat = NCAT;
  TString inDir   = "./MiniTrees/Signal_VV/";

//********************************************//
// Compute weights for the given luminosity:
//********************************************//
//          Use HiggsCSandWidth tools which return
//          cross section and branching fraction 
//          for given mass and production channel
//          (M.Snowball)
//********************************************//

  Float_t MASS(mass);
  Float_t sqrts(8);




  HiggsCSandWidth *myCSW = new HiggsCSandWidth();

  Float_t xs_ggqq  = 0.05;
  Float_t br       = 1.0;


  cout << "++++++++++++++++++++++++++++++++" << endl;
  cout << "sigma_X x BR (mX = " << MASS << ") = "  << xs_ggqq*br << endl;
  cout << "++++++++++++++++++++++++++++++++" << endl;


// number of generated events in each signal dataset

  Float_t n_ggqq(30000);

// Luminosity:

  Float_t Lum = 19400.0;
  RooRealVar lumi("lumi","lumi",Lum);
  w->import(lumi); 

  // Scale factor for the signal

  Float_t scaleX  = lumi.getVal()*xs_ggqq * br /n_ggqq;



//****************************//
// Signal Data Set
//****************************//

  // Variables
  RooArgSet* ntplVars = defineVariables();
  RooRealVar weightVar("weightVar","",1,0,1000);

//signal300_tree_radcut.root

  TFile sigFile1(inDir+TString("dijetWtag_Moriond_ZZ2000_miniTree.root"));  

  TTree* sigTree1 = (TTree*) sigFile1.Get("TCVARS");


// add weight var into the list of ntuple variables
  weightVar.setVal(1.);
  ntplVars->add(RooArgList(weightVar));

// common preselection cut
  TString mainCut("1");


//****************************//
// Signal  Data Set
//****************************//
// Create non scaled signal dataset composed with  different productions 
// according to their cross sections

  RooDataSet sigScaled("sigScaled","dataset",sigTree1,*ntplVars,mainCut,"evWeight");

  RooRealVar *scaleWeightVar1 = (RooRealVar*) (*ntplVars)["evWeight"] ;
  RooRealVar *scaleWeightVar2 = (RooRealVar*) (*ntplVars)["normWeight"] ;
  RooFormulaVar *scaleWeightVar3 = new RooFormulaVar( "scaleWeight3", "", "@0*@1", 
						      RooArgList(*scaleWeightVar1, *scaleWeightVar2));

  sigScaled.Print("v");


  RooDataSet* sigToFit[NCAT];
  for (int c = 0; c < ncat; ++c) {
    sigToFit[c] =  (RooDataSet*) sigScaled.reduce(*w->var("mgg"),mainCut+TString::Format(" && categories==%d",c));
    w->import(*sigToFit[c],Rename(TString::Format("Sig_cat%d",c)));
  }

          
// Create full signal data set without categorization
  RooDataSet* sigToFitAll  = (RooDataSet*) sigScaled.reduce(*w->var("mgg"),mainCut);
  w->import(*sigToFitAll,Rename("Sig"));

// Create weighted signal dataset composed with  different 
// production processes according to their cross sections
// no common preselection cut applied yet 

  RooRealVar *weightVar1 = (RooRealVar*) (*ntplVars)["evWeight"] ;
  RooRealVar *weightVar2 = (RooRealVar*) (*ntplVars)["normWeight"] ;
  RooFormulaVar *weightVar3 = new RooFormulaVar( "weight3", "", "@0*@1", RooArgList(*weightVar1, *weightVar2));

  weightVar.setVal(1.);
  ntplVars->setRealValue("normWeight", scaleX);
  RooDataSet sigWeightedTmp1("sigData","dataset",sigTree1,*ntplVars,mainCut,"weightVar");
  RooRealVar *weightX = (RooRealVar*) sigWeightedTmp1.addColumn(*weightVar3) ;
  RooDataSet sigWeighted("sigData","dataset",
			 RooArgList((*ntplVars)["mgg"],
				    (*ntplVars)["categories"],*weightX),
			 Import(sigWeightedTmp1),WeightVar(*weightX));

  cout << "---- nX:  " << sigWeighted.sumEntries() << endl; 
  for (int c = 0; c < ncat; ++c) {
    Float_t nExpEvt = sigWeighted.reduce(*w->var("mgg"),TString::Format("categories==%d",c))->sumEntries();
    cout << TString::Format("nEvt exp.  cat%d : ",c) << nExpEvt 
	 << TString::Format("   eff x Acc  cat%d : ",c) << nExpEvt*100/scaleX/100000. << "%" 
	 << endl; 
  }

  sigWeighted.Print("v");


// apply a common preselection cut;
// split into 4  categories;

  RooDataSet* signal[NCAT];
  for (int c = 0; c < ncat; ++c) {
    signal[c] =  (RooDataSet*) sigWeighted.reduce(*w->var("mgg"),mainCut+TString::Format(" && categories==%d",c));
    w->import(*signal[c],Rename(TString::Format("SigWeight_cat%d",c)));
  }
// Create full weighted signal data set without categorization
  RooDataSet* signalAll  = (RooDataSet*) sigWeighted.reduce(*w->var("mgg"),mainCut);
  w->import(*signalAll, Rename("SigWeight"));

}


void AddBkgData(RooWorkspace* w) {

  Int_t ncat = NCAT;
  TString inDir   = "./MiniTrees/Data_VV/";

// common preselection cut
  TString mainCut("1");

  Float_t minMassFit(1000),maxMassFit(5000); 


//****************************//
// CMS Data Set
//****************************//
// retrieve the data tree;
// no common preselection cut applied yet; 

  TFile dataFile(inDir+"dijetWtag_Moriond_Mar6_miniTree.root");   
  TTree* dataTree     = (TTree*) dataFile.Get("TCVARS");

  // Variables
  RooArgSet* ntplVars = defineVariables();

  RooDataSet Data("Data","dataset",dataTree,*ntplVars,"","normWeight");

// apply a common preselection cut;
// split into NCAT  categories;

  
  RooDataSet* dataToFit[6];
  for (int c = 0; c < ncat; ++c) {
// Real data
    dataToFit[c]   = (RooDataSet*) Data.reduce(*w->var("mgg"),mainCut+TString::Format(" && categories==%d",c));
    w->import(*dataToFit[c],Rename(TString::Format("Data_cat%d",c)));
  }

// Create full data set without categorization
  RooDataSet* data    = (RooDataSet*) Data.reduce(*w->var("mgg"),mainCut);
  w->import(*data, Rename("Data"));
  data->Print("v");

}



void SigModelFit(RooWorkspace* w, Float_t mass) {

  Int_t ncat = NCAT;
  Float_t MASS(mass);

//******************************************//
// Fit signal with model pdfs
//******************************************//
// retrieve pdfs and datasets from workspace to fit with pdf models




  RooDataSet* sigToFit[NCAT];
  RooAbsPdf* MggSig[NCAT];

  Float_t minMassFit(1000),maxMassFit(5000); 


// Fit Signal 

  for (int c = 0; c < ncat; ++c) {
    cout << "---------- category = " << c << endl;
//    sigToFit[c]   = (RooDataSet*) w->data(TString::Format("Sig_cat%d",c));
    sigToFit[c]   = (RooDataSet*) w->data(TString::Format("SigWeight_cat%d",c));

    MggSig[c]     = (RooAbsPdf*)  w->pdf(TString::Format("MggSig_cat%d",c));

    RooRealVar* peak = w->var(TString::Format("mgg_sig_m0_cat%d",c));
    peak->setVal(MASS);
			      
    MggSig[c]     ->fitTo(*sigToFit[c],Range(minMassFit,maxMassFit),SumW2Error(kTRUE));
// IMPORTANT: fix all pdf parameters to constant
    w->defineSet(TString::Format("SigPdfParam_cat%d",c), RooArgSet(*w->var(TString::Format("mgg_sig_m0_cat%d",c)),
								   *w->var(TString::Format("mgg_sig_sigma_cat%d",c)),
								   *w->var(TString::Format("mgg_sig_alpha_cat%d",c)),
								   *w->var(TString::Format("mgg_sig_n_cat%d",c)), 
								   *w->var(TString::Format("mgg_sig_gsigma_cat%d",c)),
								   *w->var(TString::Format("mgg_sig_frac_cat%d",c))) );
    SetConstantParams(w->set(TString::Format("SigPdfParam_cat%d",c)));
  }

//  w->pdf("MggSig")->fitTo(*w->data("Sig"), Range(minMassFit,maxMassFit),SumW2Error(kTRUE));



}



RooFitResult* BkgModelFitBernstein(RooWorkspace* w, Bool_t dobands) {

  Int_t ncat = NCAT;

  std::vector<TString> catdesc;
  catdesc.push_back("#scale[0.8]{dijet_mass_2mtag_0mdtag}");
  catdesc.push_back("#scale[0.8]{dijet_mass_2mtag_1mdtag}");
  catdesc.push_back("#scale[0.8]{dijet_mass_2mtag_2mdtag}");
  catdesc.push_back("#scale[0.8]{dijet_mass_1mtag_0mdtag}");
  catdesc.push_back("#scale[0.8]{dijet_mass_1mtag_1mdtag}");
  catdesc.push_back("#scale[0.8]{dijet_mass_0mtag}");


//******************************************//
// Fit background with model pdfs
//******************************************//

// retrieve pdfs and datasets from workspace to fit with pdf models


  RooDataSet* data[NCAT];
  RooBernstein* MggBkg[NCAT];
  RooFitResult* fitresult[NCAT];;
  RooPlot* plotMggBkg[NCAT];

// dobands and dosignal
  RooDataSet* signal[NCAT];
  RooAbsPdf*  MggSig[NCAT];


  Float_t minMassFit(1000),maxMassFit(5000); 

// Fit data with background pdf for data limit

  RooRealVar* mgg     = w->var("mgg");  
  mgg->setUnit("GeV");
  
  TLatex *text = new TLatex();
  text->SetNDC();
  text->SetTextSize(0.04);


  for (int c = 0; c < ncat; ++c) {
    data[c]   = (RooDataSet*) w->data(TString::Format("Data_cat%d",c));
    
    RooFormulaVar *p1mod = new RooFormulaVar(TString::Format("p1mod_cat%d",c),"","@0",*w->var(TString::Format("mgg_bkg_8TeV_slope1_cat%d",c)));
    RooFormulaVar *p2mod = new RooFormulaVar(TString::Format("p2mod_cat%d",c),"","@0",*w->var(TString::Format("mgg_bkg_8TeV_slope2_cat%d",c)));
    RooFormulaVar *p3mod = new RooFormulaVar(TString::Format("p3mod_cat%d",c),"","@0",*w->var(TString::Format("mgg_bkg_8TeV_slope3_cat%d",c)));
     

    RooFormulaVar *sqrtS = new RooFormulaVar(TString::Format("sqrtS_cat%d",c),"","@0*@0",*w->var("sqrtS"));
    RooFormulaVar *x = new RooFormulaVar(TString::Format("x_cat%d",c),"","@0/@1",RooArgList(*mgg, *sqrtS));


    
    RooAbsPdf* MggBkgTmp0 = new RooGenericPdf(TString::Format("DijetBackground_%d",c), "pow(1-@0, @1)/pow(@0, @2+@3*log(@0))", RooArgList(*x, *p1mod, *p2mod, *p3mod));
    
    
    w->factory(TString::Format("mgg_bkg_8TeV_norm_cat%d[4000.0,0.0,10000000]",c));
    
    RooExtendPdf MggBkgTmp(TString::Format("MggBkg_cat%d",c),"",*MggBkgTmp0,*w->var(TString::Format("mgg_bkg_8TeV_norm_cat%d",c)));
    
    
    fitresult[c] = MggBkgTmp.fitTo(*data[c], Strategy(1),Minos(kFALSE), Range(minMassFit,maxMassFit),SumW2Error(kTRUE), Save(kTRUE));
    w->import(MggBkgTmp);


//************************************************//
// Plot Mgg background fit results per categories 
//************************************************//
// Plot Background Categories 
//****************************//

    TCanvas* ctmp = new TCanvas("ctmp","Mgg Background Categories",0,0,500,500);
    Int_t nBinsMass(80);
    plotMggBkg[c] = mgg->frame(nBinsMass);
    data[c]->plotOn(plotMggBkg[c],LineColor(kWhite),MarkerColor(kWhite));    

    MggBkgTmp.plotOn(plotMggBkg[c],LineColor(kBlue),Range("fitrange"),NormRange("fitrange")); 
    data[c]->plotOn(plotMggBkg[c]);    

    plotMggBkg[c]->Draw();  
    cout << "!!!!!!!!!!!!!!!!!" << endl;

//********************************************************************************//

    if (dobands) {

      RooAbsPdf *cpdf; cpdf = MggBkgTmp0;
      TGraphAsymmErrors *onesigma = new TGraphAsymmErrors();
      TGraphAsymmErrors *twosigma = new TGraphAsymmErrors();
      
      RooRealVar *nlim = new RooRealVar(TString::Format("nlim%d",c),"",0.0,0.0,10.0);
      nlim->removeRange();
      
      RooCurve *nomcurve = dynamic_cast<RooCurve*>(plotMggBkg[c]->getObject(1));
      
      for (int i=1; i<(plotMggBkg[c]->GetXaxis()->GetNbins()+1); ++i) {
	double lowedge = plotMggBkg[c]->GetXaxis()->GetBinLowEdge(i);
	double upedge  = plotMggBkg[c]->GetXaxis()->GetBinUpEdge(i);
	double center  = plotMggBkg[c]->GetXaxis()->GetBinCenter(i);
	
	double nombkg = nomcurve->interpolate(center);
	nlim->setVal(nombkg);
	mgg->setRange("errRange",lowedge,upedge);
	RooAbsPdf *epdf = 0;
	epdf = new RooExtendPdf("epdf","",*cpdf,*nlim,"errRange");
	
	RooAbsReal *nll = epdf->createNLL(*(data[c]),Extended());
	RooMinimizer minim(*nll);
	minim.setStrategy(0);
	double clone = 1.0 - 2.0*RooStats::SignificanceToPValue(1.0);
	double cltwo = 1.0 - 2.0*RooStats::SignificanceToPValue(2.0);
	
	minim.migrad();
	minim.minos(*nlim);
	// printf("errlo = %5f, errhi = %5f\n",nlim->getErrorLo(),nlim->getErrorHi());
	
	onesigma->SetPoint(i-1,center,nombkg);
	onesigma->SetPointError(i-1,0.,0.,-nlim->getErrorLo(),nlim->getErrorHi());
	
	minim.setErrorLevel(0.5*pow(ROOT::Math::normal_quantile(1-0.5*(1-cltwo),1.0), 2)); // the 0.5 is because qmu is -2*NLL
	// eventually if cl = 0.95 this is the usual 1.92!      
	
	
	minim.migrad();
	minim.minos(*nlim);
	
	twosigma->SetPoint(i-1,center,nombkg);
	twosigma->SetPointError(i-1,0.,0.,-nlim->getErrorLo(),nlim->getErrorHi());
	
	
	delete nll;
	delete epdf;
	
      }
      mgg->setRange("errRange",minMassFit,maxMassFit);
      
      twosigma->SetLineColor(kGreen);
      twosigma->SetFillColor(kGreen);
      twosigma->SetMarkerColor(kGreen);
      twosigma->Draw("L3 SAME");
      
      onesigma->SetLineColor(kYellow);
      onesigma->SetFillColor(kYellow);
      onesigma->SetMarkerColor(kYellow);
      onesigma->Draw("L3 SAME");
      
      plotMggBkg[c]->Draw("SAME"); 
     
    }

//********************************************************************************//
/*
    cout << "!!!!!!!!!!!!!!!!!" << endl;
    signal[c]       = (RooDataSet*) w->data(TString::Format("SigWeight_cat%d",c));
    double norm     = 1.0*signal[c]->sumEntries();
    MggSig[c]       = (RooAbsPdf*)  w->pdf(TString::Format("MggSig_cat%d",c));
    MggSig[c]       ->plotOn(plotMggBkg[c],Normalization(norm,RooAbsPdf::NumEvent),DrawOption("F"),LineColor(kRed),FillStyle(1001),FillColor(19));
    MggSig[c]       ->plotOn(plotMggBkg[c],Normalization(norm,RooAbsPdf::NumEvent),LineColor(kRed));
    
    plotMggBkg[c]->SetTitle("");      
    plotMggBkg[c]->SetMinimum(0.0);
    plotMggBkg[c]->SetMaximum(1.40*plotMggBkg[c]->GetMaximum());
    plotMggBkg[c]->GetXaxis()->SetTitle("m_{#gamma#gamma} (GeV)");

    cout << "!!!!!!!!!!!!!!!!!" << endl;
    plotMggBkg[c]->Draw("SAME");  
    TLegend *legmc = new TLegend(0.62,0.75,0.92,0.9);
    legmc->AddEntry(plotMggBkg[c]->getObject(2),"Data (5.3/fb)","LPE");
    legmc->AddEntry(plotMggBkg[c]->getObject(1),"Bkg Model","L");
//    legmc->AddEntry(plotMggBkg[c]->getObject(2),"#pm1 #sigma","F");
//    legmc->AddEntry(plotMggBkg[c]->getObject(1),"#pm2 #sigma","F");
    if (dobands) {
      legmc->AddEntry(onesigma,"#pm1 #sigma","F");
      legmc->AddEntry(twosigma,"#pm2 #sigma","F");
      legmc->AddEntry(plotMggBkg[c]->getObject(3),"1xSM m_{H}=125 GeV","LF"); 
    }
    legmc->SetBorderSize(0);
    legmc->SetFillStyle(0);
    legmc->Draw();    

    TLatex *lat  = new TLatex(103.0,0.85*plotMggBkg[c]->GetMaximum(),"#splitline{#scale[1.0]{CMS Preliminary}}{#scale[0.8]{#sqrt{s} = 8 TeV L = 5.3 fb^{-1}}}");
    lat->Draw();
    TLatex *lat2 = new TLatex(103.0,0.7*plotMggBkg[c]->GetMaximum(),catdesc.at(c));
    lat2->Draw();
 
    ctmp->SaveAs(TString::Format("databkgoversig_cat%d.pdf",c));
    ctmp->SaveAs(TString::Format("databkgoversig_cat%d.eps",c));
    ctmp->SaveAs(TString::Format("databkgoversig_cat%d.C",c));
*/

  }
  return fitresult;


}


void SetConstantParams(const RooArgSet* params) {

  TIterator* iter(params->createIterator());
  for (TObject *a = iter->Next(); a != 0; a = iter->Next()) {
    RooRealVar *rrv = dynamic_cast<RooRealVar *>(a);
    if (rrv) { rrv->setConstant(true); std::cout << " " << rrv->GetName(); }
  }  

}

void MakePlots(RooWorkspace* w, Float_t Mass, RooFitResult* fitresults) {

  Int_t ncat = NCAT;

  std::vector<TString> catdesc;
  catdesc.push_back("#scale[0.8]{dijet_mass_2mtag_0mdtag}");
  catdesc.push_back("#scale[0.8]{dijet_mass_2mtag_1mdtag}");
  catdesc.push_back("#scale[0.8]{dijet_mass_2mtag_2mdtag}");
  catdesc.push_back("#scale[0.8]{dijet_mass_1mtag_0mdtag}");
  catdesc.push_back("#scale[0.8]{dijet_mass_1mtag_1mdtag}");
  catdesc.push_back("#scale[0.8]{dijet_mass_0mtag}");


// retrieve data sets from the workspace
  RooDataSet* dataAll         = (RooDataSet*) w->data("Data");
  RooDataSet* signalAll       = (RooDataSet*) w->data("Sig");

  RooDataSet* data[9];  
  RooDataSet* signal[9];
  RooAbsPdf*  MggGaussSig[9];
  RooAbsPdf*  MggCBSig[9];
  RooAbsPdf*  MggSig[9];
  RooAbsPdf*  MggBkg[9];  
//  RooAbsPdf*  MggBkg2[9];  

  for (int c = 0; c < ncat; ++c) {
    data[c]         = (RooDataSet*) w->data(TString::Format("Data_cat%d",c));
//    signal[c]       = (RooDataSet*) w->data(TString::Format("Sig_cat%d",c));
    signal[c]       = (RooDataSet*) w->data(TString::Format("SigWeight_cat%d",c));
    MggGaussSig[c]  = (RooAbsPdf*)  w->pdf(TString::Format("MggGaussSig_cat%d",c));
    MggCBSig[c]     = (RooAbsPdf*)  w->pdf(TString::Format("MggCBSig_cat%d",c));
    MggSig[c]       = (RooAbsPdf*)  w->pdf(TString::Format("MggSig_cat%d",c));
    MggBkg[c]       = (RooAbsPdf*)  w->pdf(TString::Format("MggBkg_cat%d",c));
//    MggBkg2[c]      = (RooAbsPdf*)  w->pdf(TString::Format("MggBkg2_cat%d",c));
  }

// retrieve mass observable from the workspace
  RooRealVar* mgg     = w->var("mgg");  
  mgg->setUnit("GeV");

// retrieve pdfs after the fits
// Signal Model

  RooAbsPdf* MggGaussSigAll  = w->pdf("MggGaussSig");
  RooAbsPdf* MggCBSigAll     = w->pdf("MggCBSig");
  RooAbsPdf* MggSigAll       = w->pdf("MggSig");

//  RooAbsPdf* MggBkgAll       = w->pdf("MggBkg");
  RooAbsPdf* MggBkgAll       = w->pdf("MggBkgAll");



//****************************//
// Plot Mgg Fit results
//****************************//
// Set P.D.F. parameter names
// WARNING: Do not use it if Workspaces are created
//  SetParamNames(w);


  Float_t minMassFit(1000),maxMassFit(5000); 
  Float_t MASS(Mass);

  Int_t nBinsMass(50);
  
  RooPlot* plotMggAll = mgg->frame(Range(minMassFit,maxMassFit),Bins(nBinsMass));
  signalAll->plotOn(plotMggAll);
 
  gStyle->SetOptTitle(0);
  MggSigAll->plotOn(plotMggAll);
  MggSigAll->plotOn(plotMggAll,Components("MggGaussSig"),LineStyle(kDashed),LineColor(kGreen));
  MggSigAll->plotOn(plotMggAll,Components("MggCBSig"),LineStyle(kDashed),LineColor(kRed));

  MggSigAll->paramOn(plotMggAll, ShowConstants(true), Layout(0.15,0.55,0.9), Format("NEU",AutoPrecision(2)));
  plotMggAll->getAttText()->SetTextSize(0.03);

  TCanvas* c1 = new TCanvas("c1","Mgg",0,0,500,500);
  c1->cd(1);
  plotMggAll->Draw();  


//********************************************//
// Plot Mgg signal fit results per categories 
//********************************************//
// Plot Signal Categories 
//****************************//

  TLatex *text = new TLatex();
  text->SetNDC();
  text->SetTextSize(0.04);

//  TCanvas* c2 = new TCanvas("c2","Mgg Categories",0,0,1000,1000);

//  c2->Divide(3,3);
  RooPlot* plotMgg[9];
  for (int c = 0; c < ncat; ++c) {
    plotMgg[c] = mgg->frame(Range(minMassFit,maxMassFit),Bins(nBinsMass));
    signal[c]->plotOn(plotMgg[c],LineColor(kWhite),MarkerColor(kWhite));    

    MggSig[c]  ->plotOn(plotMgg[c]);
    MggSig[c]  ->plotOn(plotMgg[c],Components(TString::Format("MggGaussSig_cat%d",c)),LineStyle(kDashed),LineColor(kGreen));
    MggSig[c]  ->plotOn(plotMgg[c],Components(TString::Format("MggCBSig_cat%d",c)),LineStyle(kDashed),LineColor(kRed));
    

    MggSig[c]  ->paramOn(plotMgg[c]);
    signal[c]  ->plotOn(plotMgg[c]);

        
    TCanvas* dummy = new TCanvas("dummy", "dummy",0, 0, 400, 400);
    TH1F *hist = new TH1F("hist", "hist", 400, minMassFit, maxMassFit);
 
    plotMgg[c]->SetTitle("");      
    plotMgg[c]->SetMinimum(0.0);
    plotMgg[c]->SetMaximum(1.40*plotMgg[c]->GetMaximum());
    plotMgg[c]->GetXaxis()->SetTitle("m_{#gamma#gamma} (GeV)");

    TCanvas* ctmp = new TCanvas("ctmp","Mgg Background Categories",0,0,500,500);
    plotMgg[c]->Draw();  
//    hist->Draw("same");
    
    plotMgg[c]->Draw("SAME");  
    TLegend *legmc = new TLegend(0.62,0.75,0.92,0.9);
    legmc->AddEntry(plotMgg[c]->getObject(5),"Simulation","LPE");
    legmc->AddEntry(plotMgg[c]->getObject(1),"Parametric Model","L");
    legmc->AddEntry(plotMgg[c]->getObject(3),"Crystal Ball component","L");
    legmc->AddEntry(plotMgg[c]->getObject(2),"Gaussian Outliers","L");
    
    legmc->SetBorderSize(0);
    legmc->SetFillStyle(0);
    legmc->Draw();    
    
    float effS = effSigma(hist);
//    text->DrawLatex(0.65,0.4, TString::Format("#sigma_{eff} = %.2f GeV",effS));
//    cout<<"effective sigma [" << c << "] = " << effS <<endl;
    
    TLatex *lat  = new TLatex(minMassFit+1.5,0.85*plotMgg[c]->GetMaximum(),"#scale[1.0]{CMS Preliminary}");
    lat->Draw();
    TLatex *lat2 = new TLatex(minMassFit+1.5,0.75*plotMgg[c]->GetMaximum(),catdesc.at(c));
    lat2->Draw();
    TLatex *lat3 = new TLatex(minMassFit+1.5,0.55*plotMgg[c]->GetMaximum(),TString::Format("#scale[0.8]{#sigma_{eff} = %.2f GeV}",effS));
    lat3->Draw();

 
    ctmp->SaveAs(TString::Format("sigmodel_cat%d.png",c));
    ctmp->SaveAs(TString::Format("sigmodel_cat%d.pdf",c));
    ctmp->SaveAs(TString::Format("sigmodel_cat%d.eps",c));
    ctmp->SaveAs(TString::Format("sigmodel_cat%d.C",c));

  }


//****************************//
// Plot Mgg Background 
//****************************//
/*
  nBinsMass=80;
  RooPlot* plotMggBkgAll = mgg->frame(nBinsMass);
  dataAll->plotOn(plotMggBkgAll,LineColor(kWhite),MarkerColor(kWhite)); 
  fitresults->Print();

  MggBkgAll->plotOn(plotMggBkgAll,FillColor(kYellow),Range("fitrange"),NormRange("fitrange"), VisualizeError(*fitresults, 2.0,kFALSE));
  MggBkgAll->plotOn(plotMggBkgAll,FillColor(kGreen),Range("fitrange"),NormRange("fitrange"), VisualizeError(*fitresults, 1.0,kFALSE));
  MggBkgAll->plotOn(plotMggBkgAll,LineColor(kBlue),Range("fitrange"),NormRange("fitrange"));
  MggBkgAll->plotOn(plotMggBkgAll,Components("MggBkgAll2"),LineStyle(kDashed),LineColor(kRed));
  dataAll->plotOn(plotMggBkgAll); 
//  MggBkgAll->paramOn(plotMggBkgAll, ShowConstants(true), Layout(0.65,0.9,0.9), Format("NEU",AutoPrecision(4)));
//  plotMggBkgAll->getAttText()->SetTextSize(0.03);



  TCanvas* c3 = new TCanvas("c3","Mgg Background",0,0,500,500);
  plotMggBkgAll->Draw();  
  TLegend *legmc = new TLegend(0.62,0.75,0.92,0.9);
  legmc->AddEntry(plotMggBkgAll->getObject(4),"Data (5.3/fb)","LPE");
  legmc->AddEntry(plotMggBkgAll->getObject(3),"Bkg Model","L");
  legmc->AddEntry(plotMggBkgAll->getObject(2),"#pm1 #sigma","F");
  legmc->AddEntry(plotMggBkgAll->getObject(1),"#pm2 #sigma","F");
  legmc->SetBorderSize(0);
  legmc->SetFillStyle(0);
  legmc->Draw();

  text->DrawLatex(0.2,0.85,"CMS Preliminary");
  text->DrawLatex(0.2,0.78,"#sqrt{s} = 8 TeV L =5.3 fb^{-1}");

*/
//************************************************//
// Plot Mgg background fit results per categories 
//************************************************//
// Plot Background Categories 
//****************************//

  TCanvas* c4 = new TCanvas("c4","Mgg Background Categories",0,0,1000,1000);
  c4->Divide(2,3);

  RooPlot* plotMggBkg[9];
  for (int c = 0; c < ncat; ++c) {
    plotMggBkg[c] = mgg->frame(nBinsMass);
    data[c]->plotOn(plotMggBkg[c],LineColor(kWhite),MarkerColor(kWhite));    
    MggBkg[c]->plotOn(plotMggBkg[c],LineColor(kBlue),Range("fitrange"),NormRange("fitrange")); 
    data[c]->plotOn(plotMggBkg[c]);    
    MggBkg[c]->paramOn(plotMggBkg[c], ShowConstants(true), Layout(0.65,0.9,0.9), Format("NEU",AutoPrecision(4)));
    plotMggBkg[c]->getAttText()->SetTextSize(0.03);
    c4->cd(c+1);
    plotMggBkg[c]->Draw();  
    gPad->SetLogy(1);
  }

  c4->SaveAs(TString::Format("backgrounds_log.png",c));
  c4->SaveAs(TString::Format("backgrounds_log.pdf",c));
  c4->SaveAs(TString::Format("backgrounds_log.eps",c));
  c4->SaveAs(TString::Format("backgrounds_log.C",c));



  TCanvas* c5 = new TCanvas("c5","Mgg Background Categories",0,0,1000,1000);
  c5->Divide(2,3);

  RooPlot* plotMggBkg[9];
  for (int c = 0; c < ncat; ++c) {
    plotMggBkg[c] = mgg->frame(nBinsMass);
    data[c]->plotOn(plotMggBkg[c],LineColor(kWhite),MarkerColor(kWhite));    
    MggBkg[c]->plotOn(plotMggBkg[c],LineColor(kBlue),Range("fitrange"),NormRange("fitrange")); 
    data[c]->plotOn(plotMggBkg[c]);    
    MggBkg[c]->paramOn(plotMggBkg[c], ShowConstants(true), Layout(0.65,0.9,0.9), Format("NEU",AutoPrecision(4)));
    plotMggBkg[c]->getAttText()->SetTextSize(0.03);
    c5->cd(c+1);
    plotMggBkg[c]->Draw();  
  }

  c5->SaveAs(TString::Format("backgrounds_log.png",c));
  c5->SaveAs(TString::Format("backgrounds_log.pdf",c));
  c5->SaveAs(TString::Format("backgrounds_log.eps",c));
  c5->SaveAs(TString::Format("backgrounds_log.C",c));

}


void SetParamNames(RooWorkspace* w) {
  
  Int_t ncat = NCAT;

//****************************//
// Mgg signal all categories
//****************************//

  RooRealVar* mgg_sig_m0     = w->var("mgg_sig_m0");  
  RooRealVar* mgg_sig_sigma  = w->var("mgg_sig_sigma");
  RooRealVar* mgg_sig_alpha  = w->var("mgg_sig_alpha"); 
  RooRealVar* mgg_sig_n      = w->var("mgg_sig_n"); 
  RooRealVar* mgg_sig_gsigma = w->var("mgg_sig_gsigma");
  RooRealVar* mgg_sig_frac   = w->var("mgg_sig_frac");

  mgg_sig_m0    ->SetName("m_{0}");
  mgg_sig_sigma ->SetName("#sigma_{CB}");
  mgg_sig_alpha ->SetName("#alpha");
  mgg_sig_n     ->SetName("n");
  mgg_sig_gsigma->SetName("#sigma_G");  
  mgg_sig_frac  ->SetName("f_G");  

  mgg_sig_m0    ->setUnit("GeV");
  mgg_sig_sigma ->setUnit("GeV");
  mgg_sig_gsigma->setUnit("GeV"); 

//****************************//
// Mgg background  
//****************************//

  RooRealVar* mgg_bkg_8TeV_slope1  = w->var("mgg_bkg_8TeV_slope1");
  mgg_bkg_8TeV_slope1              ->SetName("a_{B}");
  mgg_bkg_8TeV_slope1              ->setUnit("1/GeV");

  RooRealVar* mgg_bkg_8TeV_slope2  = w->var("mgg_bkg_8TeV_slope2");
  mgg_bkg_8TeV_slope2              ->SetName("a_{B}");
  mgg_bkg_8TeV_slope2              ->setUnit("1/GeV");



//****************************//
// Mgg per category  
//****************************//

  for (int c = 0; c < ncat; ++c) {


    mgg_sig_m0     = (RooRealVar*) w->var(TString::Format("mgg_sig_m0_cat%d",c));
    mgg_sig_sigma  = (RooRealVar*) w->var(TString::Format("mgg_sig_sigma_cat%d",c));
    mgg_sig_alpha  = (RooRealVar*) w->var(TString::Format("mgg_sig_alpha_cat%d",c));
    mgg_sig_n      = (RooRealVar*) w->var(TString::Format("mgg_sig_n_cat%d",c));
    mgg_sig_gsigma = (RooRealVar*) w->var(TString::Format("mgg_sig_gsigma_cat%d",c));
    mgg_sig_frac   = (RooRealVar*) w->var(TString::Format("mgg_sig_frac_cat%d",c));

    mgg_sig_m0     ->SetName("m_{0}"); 
    mgg_sig_sigma  ->SetName("#sigma_{CB}");
    mgg_sig_alpha  ->SetName("#alpha");
    mgg_sig_n      ->SetName("n"); 
    mgg_sig_gsigma ->SetName("#sigma_{G}");
    mgg_sig_frac   ->SetName("f_{G}");


    mgg_sig_m0     ->setUnit("GeV");
    mgg_sig_sigma  ->setUnit("GeV");
    mgg_sig_gsigma ->setUnit("GeV");


    mgg_bkg_8TeV_slope1 = w->var(TString::Format("mgg_bkg_8TeV_slope1_cat%d",c));
    mgg_bkg_8TeV_slope1 ->SetName("p_{B}^{1}");
    mgg_bkg_8TeV_slope1 ->setUnit("1/GeV");

    mgg_bkg_8TeV_slope2 = w->var(TString::Format("mgg_bkg_8TeV_slope2_cat%d",c));
    mgg_bkg_8TeV_slope2 ->SetName("p_{B}^{2}");
    mgg_bkg_8TeV_slope2 ->setUnit("1/GeV^{2}");

//    RooRealVar* mgg_bkg_8TeV_frac = w->var(TString::Format("mgg_bkg_8TeV_frac_cat%d",c));
//    mgg_bkg_8TeV_frac ->SetName("f");

  }

}



void MakeSigWS(RooWorkspace* w, const char* fileBaseName) {

  TString wsDir   = "workspaces/";
  Int_t ncat = NCAT;

//**********************************************************************//
// Write pdfs and datasets into the workspace 
// for statistical tests. 
// 
// Suffix to attribute belonging to given the category
// "catN"    - N shower shape, pseudo-rapidity, pt or Njets categories 
//  N=0-3    - default shower shape, pseudo-rapidity 4 categories
//  N=0-3    - Pt<40 GeV or 0-jet (Central Jet Veto)
//  N=4-7    - Pt>40 GeV or 1-jet (one-jet tagging)
//**********************************************************************//



//********************************//
// Retrieve P.D.F.s
//********************************//

  RooAbsPdf* MggSigPdf[6];

// (1) import signal P.D.F.s

  RooWorkspace *wAll = new RooWorkspace("w_all","w_all");


  for (int c = 0; c < ncat; ++c) {
    MggSigPdf[c] = (RooAbsPdf*)  w->pdf(TString::Format("MggSig_cat%d",c));
    wAll->import(*w->pdf(TString::Format("MggSig_cat%d",c)));
  }

// (2) Systematics on energy scale and resolution

  wAll->factory("CMS_hgg_sig_m0_absShift_cat0[1,0.9875,1.0125]");
  wAll->factory("CMS_hgg_sig_m0_absShift_cat1[1,0.9875,1.0125]");
  wAll->factory("CMS_hgg_sig_m0_absShift_cat2[1,0.9875,1.0125]");
  wAll->factory("CMS_hgg_sig_m0_absShift_cat3[1,0.9875,1.0125]");
  wAll->factory("CMS_hgg_sig_m0_absShift_cat4[1,0.9875,1.0125]");
  wAll->factory("CMS_hgg_sig_m0_absShift_cat5[1,0.9875,1.0125]");
  wAll->factory("prod::CMS_hgg_sig_m0_cat0(mgg_sig_m0_cat0, CMS_hgg_sig_m0_absShift_cat0)");
  wAll->factory("prod::CMS_hgg_sig_m0_cat1(mgg_sig_m0_cat1, CMS_hgg_sig_m0_absShift_cat1)");
  wAll->factory("prod::CMS_hgg_sig_m0_cat2(mgg_sig_m0_cat2, CMS_hgg_sig_m0_absShift_cat2)");
  wAll->factory("prod::CMS_hgg_sig_m0_cat3(mgg_sig_m0_cat3, CMS_hgg_sig_m0_absShift_cat3)");
  wAll->factory("prod::CMS_hgg_sig_m0_cat4(mgg_sig_m0_cat4, CMS_hgg_sig_m0_absShift_cat4)");
  wAll->factory("prod::CMS_hgg_sig_m0_cat5(mgg_sig_m0_cat5, CMS_hgg_sig_m0_absShift_cat5)");
// (3) Systematics on resolution: create new sigmas


  wAll->factory("CMS_hgg_sig_sigmaScale_cat0[1,0.9,1.1]");
  wAll->factory("CMS_hgg_sig_sigmaScale_cat1[1,0.9,1.1]");
  wAll->factory("CMS_hgg_sig_sigmaScale_cat2[1,0.9,1.1]");
  wAll->factory("CMS_hgg_sig_sigmaScale_cat3[1,0.9,1.1]");
  wAll->factory("CMS_hgg_sig_sigmaScale_cat4[1,0.9,1.1]");
  wAll->factory("CMS_hgg_sig_sigmaScale_cat5[1,0.9,1.1]");


  wAll->factory("prod::CMS_hgg_sig_sigma_cat0(mgg_sig_sigma_cat0, CMS_hgg_sig_sigmaScale_cat0)");
  wAll->factory("prod::CMS_hgg_sig_sigma_cat1(mgg_sig_sigma_cat1, CMS_hgg_sig_sigmaScale_cat1)");
  wAll->factory("prod::CMS_hgg_sig_sigma_cat2(mgg_sig_sigma_cat2, CMS_hgg_sig_sigmaScale_cat2)");
  wAll->factory("prod::CMS_hgg_sig_sigma_cat3(mgg_sig_sigma_cat3, CMS_hgg_sig_sigmaScale_cat3)");
  wAll->factory("prod::CMS_hgg_sig_sigma_cat4(mgg_sig_sigma_cat4, CMS_hgg_sig_sigmaScale_cat4)");
  wAll->factory("prod::CMS_hgg_sig_sigma_cat5(mgg_sig_sigma_cat5, CMS_hgg_sig_sigmaScale_cat5)");


  wAll->factory("prod::CMS_hgg_sig_gsigma_cat0(mgg_sig_gsigma_cat0, CMS_hgg_sig_sigmaScale_cat0)");
  wAll->factory("prod::CMS_hgg_sig_gsigma_cat1(mgg_sig_gsigma_cat1, CMS_hgg_sig_sigmaScale_cat1)");
  wAll->factory("prod::CMS_hgg_sig_gsigma_cat2(mgg_sig_gsigma_cat2, CMS_hgg_sig_sigmaScale_cat2)");
  wAll->factory("prod::CMS_hgg_sig_gsigma_cat3(mgg_sig_gsigma_cat3, CMS_hgg_sig_sigmaScale_cat3)");
  wAll->factory("prod::CMS_hgg_sig_gsigma_cat4(mgg_sig_gsigma_cat4, CMS_hgg_sig_sigmaScale_cat4)");
  wAll->factory("prod::CMS_hgg_sig_gsigma_cat5(mgg_sig_gsigma_cat5, CMS_hgg_sig_sigmaScale_cat5)");

// (4) do reparametrization of signal
  for (int c = 0; c < ncat; ++c) {
    wAll->factory(
		  TString::Format("EDIT::CMS_hgg_sig_cat%d(MggSig_cat%d,",c,c) +
		  TString::Format(" mgg_sig_m0_cat%d=CMS_hgg_sig_m0_cat%d, ", c,c) +
		  TString::Format(" mgg_sig_sigma_cat%d=CMS_hgg_sig_sigma_cat%d, ", c,c) +
		  TString::Format(" mgg_sig_gsigma_cat%d=CMS_hgg_sig_gsigma_cat%d)", c,c)
		  );
  }

  TString filename(wsDir+TString(fileBaseName)+".inputsig.root");
  wAll->writeToFile(filename);
  cout << "Write signal workspace in: " << filename << " file" << endl;

  return;
}


void MakeBkgWS(RooWorkspace* w, const char* fileBaseName) {

  TString wsDir   = "workspaces/";
  Int_t ncat = NCAT;  

//**********************************************************************//
// Write pdfs and datasets into the workspace 
// for statistical tests. 
// 
// Suffix to attribute belonging to given the category
// "catN"    - N shower shape, pseudo-rapidity, pt or Njets categories 
//  N=0-3    - default shower shape, pseudo-rapidity 4 categories
//  N=0-3    - Pt<40 GeV or 0-jet (Central Jet Veto)
//  N=4-7    - Pt>40 GeV or 1-jet (one-jet tagging)
//**********************************************************************//



//********************************//
// Retrieve the datasets and PDFs
//********************************//

  RooDataSet* data[NCAT];
  RooExtendPdf* MggBkgPdf[NCAT];

// (1) import everything

  cout << "Start importing everything" << endl;

  RooWorkspace *wAll = new RooWorkspace("w_all","w_all");

  for (int c = 0; c < ncat; ++c) {
 
    cout << "For category " << c << endl;
    data[c]      = (RooDataSet*) w->data(TString::Format("Data_cat%d",c));
    ((RooRealVar*) data[c]->get()->find("mgg"))->setBins(400) ;
    RooDataHist* dataBinned = data[c]->binnedClone();
    MggBkgPdf[c] = (RooExtendPdf*)  w->pdf(TString::Format("MggBkg_cat%d",c));
    //   wAll->import(*data[c], Rename(TString::Format("data_obs_cat%d",c)));
    wAll->import(*dataBinned, Rename(TString::Format("data_obs_cat%d",c)));
   wAll->import(*w->pdf(TString::Format("MggBkg_cat%d",c)));
    wAll->factory(TString::Format("CMS_hgg_bkg_8TeV_cat%d_norm[%g,0.0,1000000.0]", c, wAll->var(TString::Format("mgg_bkg_8TeV_norm_cat%d",c))->getVal()));
    wAll->factory(TString::Format("CMS_hgg_bkg_8TeV_slope1_cat%d[%g,0,100]", c, wAll->var(TString::Format("mgg_bkg_8TeV_slope1_cat%d",c))->getVal()));
    wAll->factory(TString::Format("CMS_hgg_bkg_8TeV_slope2_cat%d[%g,0,100]", c, wAll->var(TString::Format("mgg_bkg_8TeV_slope2_cat%d",c))->getVal()));
    wAll->factory(TString::Format("CMS_hgg_bkg_8TeV_slope3_cat%d[%g,-10,10]", c, wAll->var(TString::Format("mgg_bkg_8TeV_slope3_cat%d",c))->getVal()));  
    //    wAll->factory(TString::Format("sqrtS_cat%d[%g,%g,%g]", c, wAll->var(TString::Format("sqrtS_cat%d",c))->getVal()));  

    cout << "Done For category " << c << endl;    
  }
  
  
  cout << "Imported" << endl;

// (2) do reparametrization of background

  for (int c = 0; c < ncat; ++c) {
      wAll->factory(
		    TString::Format("EDIT::CMS_hgg_bkg_8TeV_cat%d(MggBkg_cat%d,",c,c) +
		    TString::Format(" mgg_bkg_8TeV_norm_cat%d=CMS_hgg_bkg_8TeV_cat%d_norm,", c,c)+
		    TString::Format(" mgg_bkg_8TeV_slope1_cat%d=CMS_hgg_bkg_8TeV_slope1_cat%d,", c,c)+
		    TString::Format(" mgg_bkg_8TeV_slope2_cat%d=CMS_hgg_bkg_8TeV_slope2_cat%d,", c,c)+
		    TString::Format(" mgg_bkg_8TeV_slope3_cat%d=CMS_hgg_bkg_8TeV_slope3_cat%d)", c,c)
		    );
  } 


  TString filename(wsDir+TString(fileBaseName)+".root");
  wAll->writeToFile(filename);
  cout << "Write background workspace in: " << filename << " file" << endl;

  std::cout << "observation ";
  for (int c = 0; c < ncat; ++c) {
    std::cout << "  " << wAll->data(TString::Format("data_obs_cat%d",c))->sumEntries();
    wAll->data(TString::Format("data_obs_cat%d",c))->Print();
  }
  std::cout << std::endl;
  
  for (int c = 0; c < ncat; ++c) {
    printf("CMS_hgg_bkg_8TeV_slope1_cat%d  param  %.4f  %.3f   # Mean and absolute uncertainty on background slope\n",
	   c, wAll->var(TString::Format("CMS_hgg_bkg_8TeV_slope1_cat%d",c))->getVal(), 10);
  }

  return;
}



void MakeDataCard(RooWorkspace* w, const char* fileBaseName, const char* fileBkgName) {

  TString cardDir = "datacards/";
  Int_t ncat = NCAT;

//**********************//
// Retrieve the datasets
//**********************//

  cout << "Start retrieving dataset" << endl;

  RooDataSet* data[6];
  RooDataSet* signal[6];
  for (int c = 0; c < ncat; ++c) {
    data[c]        = (RooDataSet*) w->data(TString::Format("Data_cat%d",c));
    signal[c]      = (RooDataSet*) w->data(TString::Format("SigWeight_cat%d",c));
  }

  RooRealVar*  lumi = w->var("lumi");

//*****************************//
// Print Expected event yields
//*****************************//

  cout << "======== Expected Events Number =====================" << endl;  
  cout << ".........Measured Data for L = " << lumi->getVal() << " pb-1 ............................" << endl;  
  cout << "#Events data:        " <<  w->data("Data")->sumEntries()  << endl;
  for (int c = 0; c < ncat; ++c) {
    cout << TString::Format("#Events data cat%d:   ",c) << data[c]->sumEntries()  << endl;
  }
  cout << ".........Expected Signal for L = " << lumi->getVal() << " pb-1 ............................" << endl;  
  cout << "#Events Signal:      " << w->data("SigWeight")->sumEntries()  << endl;
  Float_t siglikeErr[6];
  for (int c = 0; c < ncat; ++c) {
    cout << TString::Format("#Events Signal cat%d: ",c) << signal[c]->sumEntries() << endl;
    siglikeErr[c]=0.6*signal[c]->sumEntries();
  }
  cout << "====================================================" << endl;  


//*************************//
// Print Data Crd int file
//*************************//


  TString filename(cardDir+TString(fileBaseName)+".txt");
  ofstream outFile(filename);


  outFile << "#CMS-HGG DataCard for Unbinned Limit Setting, " << lumi->getVal() <<  " pb-1 " << endl;
  outFile << "#Run with: combine -d hgg.mH130.0.shapes-Unbinned.txt -U -m 130 -H ProfileLikelihood -M MarkovChainMC --rMin=0 --rMax=20.0  -b 3000 -i 50000 --optimizeSim=1 --tries 30" << endl;
  outFile << "# Lumi =  " << lumi->getVal() << " pb-1" << endl;
  outFile << "imax 6" << endl;
  outFile << "jmax 1" << endl;
  outFile << "kmax *" << endl;
  outFile << "---------------" << endl;

  
  outFile << "shapes *      * " << TString(fileBkgName)+".root" << " w_all:$PROCESS_$CHANNEL" << endl;
  outFile << "shapes MggBkg * "<<  TString(fileBkgName)+".root" << " w_all:CMS_hgg_bkg_8TeV_$CHANNEL" << endl;
  outFile << "shapes MggSig * " << TString(fileBaseName)+".inputsig.root" << " w_all:CMS_hgg_sig_$CHANNEL" << endl;

  outFile << "---------------" << endl;
  outFile << "bin          cat0   cat1   cat2   cat3   cat4   cat5" << endl;
  outFile <<  "observation   "  <<  Form("%.10lg",data[0]->sumEntries()) << "  " <<  Form("%.10lg",data[1]->sumEntries()) << "  " 
	   <<  data[2]->sumEntries() << "  " << Form("%.10lg",data[3]->sumEntries()) << "  " << Form("%.10lg",data[4]->sumEntries()) << "  " << Form("%.10lg",data[5]->sumEntries()) << endl;
  outFile << "------------------------------" << endl;
  outFile << "bin                      cat0       cat0       cat1      cat1       cat2       cat2      cat3     cat3        cat4         cat4        cat5      cat5" << endl;
  outFile << "process                 MggSig     MggBkg     MggSig    MggBkg     MggSig     MggBkg  MggSig     MggBkg MggSig     MggBkg  MggSig     MggBkg" << endl;
  outFile << "process                    0          1          0         1          0          1    0          1   0          1   0          1" << endl;
  outFile <<  "rate                      " 
	   << "  " << signal[0]->sumEntries() << "  " <<  1  << "  " << signal[1]->sumEntries() << "  " <<  1
	  << "  " << signal[2]->sumEntries() << "  " <<  1  << "  " << signal[3]->sumEntries() << "  " <<  1 
	  << "  " << signal[4]->sumEntries() << "  " <<  1  << "  " << signal[5]->sumEntries() << "  " <<  1 << endl;
  outFile << "--------------------------------" << endl;

  
  outFile << "lumi_8TeV       lnN  0.950/1.050      -    0.950/1.050     -    0.950/1.050      - 0.950/1.050      - 0.950/1.050      - 0.950/1.050      - " << endl;
  outFile << "CMS_VV_eff_g         lnN  1.020      -        1.020       -         1.032       -   1.020      -  1.020      - 1.020      - # Photon efficiency" << endl;
  outFile << "# Parametric shape uncertainties, entered by hand." << endl;
  outFile << "CMS_hgg_sig_m0_absShift_cat0    param   1   0.0025   # displacement of the mean w.r.t. nominal in EB*EB category, good R9" << endl;
  outFile << "CMS_hgg_sig_m0_absShift_cat1    param   1   0.0025   # as above, in low R9" << endl;
  outFile << "CMS_hgg_sig_m0_absShift_cat2    param   1   0.0050   # displacement of the mean w.r.t. nominal in EB*EX category, good R9" << endl;
  outFile << "CMS_hgg_sig_m0_absShift_cat3    param   1   0.0050   # displacement of the mean w.r.t. nominal in EB*EX category, good R9" << endl;
  outFile << "CMS_hgg_sig_m0_absShift_cat4    param   1   0.0050   # displacement of the mean w.r.t. nominal in EB*EX category, good R9" << endl;
  outFile << "CMS_hgg_sig_m0_absShift_cat5    param   1   0.0050   # displacement of the mean w.r.t. nominal in EB*EX category, good R9" << endl;
  outFile << "CMS_hgg_sig_sigmaScale_cat0     param   1   0.136   # multiplicative correction to sigmas in EB*EB category, good R9" << endl;
  outFile << "CMS_hgg_sig_sigmaScale_cat1     param   1   0.111   # as above, in low R9" << endl;
  outFile << "CMS_hgg_sig_sigmaScale_cat2     param   1   0.151   # multiplicative correction to sigmas in EB*EX category, good R9" << endl;
  outFile << "CMS_hgg_sig_sigmaScale_cat3     param   1   0.151   # multiplicative correction to sigmas in EB*EX category, good R9" << endl;
  outFile << "CMS_hgg_sig_sigmaScale_cat4     param   1   0.151   # multiplicative correction to sigmas in EB*EX category, good R9" << endl;
  outFile << "CMS_hgg_sig_sigmaScale_cat5     param   1   0.151   # multiplicative correction to sigmas in EB*EX category, good R9" << endl;

  outFile << "CMS_hgg_bkg_8TeV_cat0_norm           flatParam  # Normalization uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_cat1_norm           flatParam  # Normalization uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_cat2_norm           flatParam  # Normalization uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_cat3_norm           flatParam  # Normalization uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_cat4_norm           flatParam  # Normalization uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_cat5_norm           flatParam  # Normalization uncertainty on background slope" << endl;


  outFile << "CMS_hgg_bkg_8TeV_slope1_cat0         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope1_cat1         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope1_cat2         flatParam  # Mean and absolute uncertainty on background slope" << endl; 
  outFile << "CMS_hgg_bkg_8TeV_slope1_cat3         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope1_cat4         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope1_cat5         flatParam  # Mean and absolute uncertainty on background slope" << endl;

  outFile << "CMS_hgg_bkg_8TeV_slope2_cat0         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope2_cat1         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope2_cat2         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope2_cat3         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope2_cat4         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope2_cat5         flatParam  # Mean and absolute uncertainty on background slope" << endl;

  outFile << "CMS_hgg_bkg_8TeV_slope3_cat0         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope3_cat1         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope3_cat3         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope3_cat4         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile << "CMS_hgg_bkg_8TeV_slope3_cat5         flatParam  # Mean and absolute uncertainty on background slope" << endl;
  outFile.close();

  cout << "Write data card in: " << filename << " file" << endl;

  return;
}


Double_t effSigma(TH1 *hist) {

  TAxis *xaxis = hist->GetXaxis();
  Int_t nb = xaxis->GetNbins();
  if(nb < 10) {
    std::cout << "effsigma: Not a valid histo. nbins = " << nb << std::endl;
    return 0.;
  }

  Double_t bwid = xaxis->GetBinWidth(1);
  if(bwid == 0) {
    std::cout << "effsigma: Not a valid histo. bwid = " << bwid << std::endl;
    return 0.;
  }
  Double_t xmax = xaxis->GetXmax();
  Double_t xmin = xaxis->GetXmin();
  Double_t ave = hist->GetMean();
  Double_t rms = hist->GetRMS();

  Double_t total=0.;
  for(Int_t i=0; i<nb+2; i++) {
    total+=hist->GetBinContent(i);
  }
  if(total < 100.) {
    std::cout << "effsigma: Too few entries " << total << std::endl;
    return 0.;
  }
  Int_t ierr=0;
  Int_t ismin=999;

  Double_t rlim=0.683*total;
  Int_t nrms=rms/(bwid);    // Set scan size to +/- rms
  if(nrms > nb/10) nrms=nb/10; // Could be tuned...

  Double_t widmin=9999999.;
  for(Int_t iscan=-nrms;iscan<nrms+1;iscan++) { // Scan window centre
    Int_t ibm=(ave-xmin)/bwid+1+iscan;
    Double_t x=(ibm-0.5)*bwid+xmin;
    Double_t xj=x;
    Double_t xk=x;
    Int_t jbm=ibm;
    Int_t kbm=ibm;
    Double_t bin=hist->GetBinContent(ibm);
    total=bin;
    for(Int_t j=1;j<nb;j++){
      if(jbm < nb) {
        jbm++;
        xj+=bwid;
        bin=hist->GetBinContent(jbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
      if(kbm > 0) {
        kbm--;
        xk-=bwid;
        bin=hist->GetBinContent(kbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
    }
    Double_t dxf=(total-rlim)*bwid/bin;
    Double_t wid=(xj-xk+bwid-dxf)*0.5;
    if(wid < widmin) {
      widmin=wid;
      ismin=iscan;
    }
  }
  if(ismin == nrms || ismin == -nrms) ierr=3;
  if(ierr != 0) std::cout << "effsigma: Error of type " << ierr << std::endl;

  return widmin;
}



void MakeDataCard_1Channel(RooWorkspace* w, const char* fileBaseName, const char* fileBkgName, int iChan) {

  TString cardDir = "datacards/";
  Int_t ncat = NCAT;

//**********************//
// Retrieve the datasets
//**********************//

  cout << "Start retrieving dataset" << endl;

  RooDataSet* data[6];
  RooDataSet* signal[6];
  for (int c = 0; c < 6; ++c) {
    data[c]        = (RooDataSet*) w->data(TString::Format("Data_cat%d",c));
    signal[c]      = (RooDataSet*) w->data(TString::Format("SigWeight_cat%d",c));
  }

  RooRealVar*  lumi = w->var("lumi");

//*****************************//
// Print Expected event yields
//*****************************//

  cout << "======== Expected Events Number =====================" << endl;  
  cout << ".........Measured Data for L = " << lumi->getVal() << " pb-1 ............................" << endl;  
  cout << "#Events data:        " <<  w->data("Data")->sumEntries()  << endl;
  for (int c = 0; c < ncat; ++c) {
    cout << TString::Format("#Events data cat%d:   ",c) << data[c]->sumEntries()  << endl;
  }
  cout << ".........Expected Signal for L = " << lumi->getVal() << " pb-1 ............................" << endl;  
  cout << "#Events Signal:      " << w->data("SigWeight")->sumEntries()  << endl;
  Float_t siglikeErr[6];
  for (int c = 0; c < ncat; ++c) {
    cout << TString::Format("#Events Signal cat%d: ",c) << signal[c]->sumEntries() << endl;
    siglikeErr[c]=0.6*signal[c]->sumEntries();
  }
  cout << "====================================================" << endl;  


//*************************//
// Print Data Crd int file
//*************************//


  TString filename(cardDir+TString(fileBaseName)+Form("_channel%d.txt",iChan));
  ofstream outFile(filename);


  outFile << "#CMS-HGG DataCard for Unbinned Limit Setting, " << lumi->getVal() <<  " pb-1 " << endl;
  outFile << "#Run with: combine -d hgg.mH130.0.shapes-Unbinned.txt -U -m 130 -H ProfileLikelihood -M MarkovChainMC --rMin=0 --rMax=20.0  -b 3000 -i 50000 --optimizeSim=1 --tries 30" << endl;
  outFile << "# Lumi =  " << lumi->getVal() << " pb-1" << endl;
  outFile << "imax 1" << endl;
  outFile << "jmax 1" << endl;
  outFile << "kmax *" << endl;
  outFile << "---------------" << endl;

  
  outFile << "shapes *      * " << TString(fileBkgName)+".root" << " w_all:$PROCESS_$CHANNEL" << endl;
  outFile << "shapes MggBkg * "<<  TString(fileBkgName)+".root" << " w_all:CMS_hgg_bkg_8TeV_$CHANNEL" << endl;
  outFile << "shapes MggSig * " << TString(fileBaseName)+".inputsig.root" << " w_all:CMS_hgg_sig_$CHANNEL" << endl;

  outFile << "---------------" << endl;
  outFile << Form("bin          cat%d", iChan) << endl;
  outFile <<  "observation   "  <<  Form("%.10lg",data[iChan]->sumEntries()) << endl;
  outFile << "------------------------------" << endl;
  outFile << "bin                      "<< Form("cat%d       cat%d      ", iChan, iChan) << endl;
  outFile << "process                 MggSig     MggBkg     " << endl;
  outFile << "process                    0          1          " << endl;
  outFile <<  "rate                      " 
	  << "  " << signal[iChan]->sumEntries()  << "  " << 1 << endl;
  outFile << "--------------------------------" << endl;

  
  outFile << "lumi_8TeV       lnN  0.950/1.050    - " << endl;
  outFile << "CMS_VV_eff_g         lnN  1.020      - # Photon efficiency" << endl;
  outFile << "# Parametric shape uncertainties, entered by hand." << endl;
  outFile << Form("CMS_hgg_sig_m0_absShift_cat%d    param   1   0.0050   # displacement of the mean w.r.t. nominal in EB*EX category, good R9",iChan) << endl;
  outFile << Form("CMS_hgg_sig_sigmaScale_cat%d     param   1   0.151   # multiplicative correction to sigmas in EB*EX category, good R9",iChan) << endl;
 
  outFile << Form("CMS_hgg_bkg_8TeV_cat%d_norm           flatParam  # Normalization uncertainty on background slope",iChan) << endl;

 outFile << Form("CMS_hgg_bkg_8TeV_slope1_cat%d         flatParam  # Mean and absolute uncertainty on background slope",iChan) << endl; 


  outFile << Form("CMS_hgg_bkg_8TeV_slope2_cat%d         flatParam  # Mean and absolute uncertainty on background slope",iChan) << endl;

  if (iChan != 2 )  outFile << Form("CMS_hgg_bkg_8TeV_slope3_cat%d         flatParam  # Mean and absolute uncertainty on background slope",iChan) << endl;

  outFile.close();

  cout << "Write data card in: " << filename << " file" << endl;

  return;
}


Double_t effSigma(TH1 *hist) {

  TAxis *xaxis = hist->GetXaxis();
  Int_t nb = xaxis->GetNbins();
  if(nb < 10) {
    std::cout << "effsigma: Not a valid histo. nbins = " << nb << std::endl;
    return 0.;
  }

  Double_t bwid = xaxis->GetBinWidth(1);
  if(bwid == 0) {
    std::cout << "effsigma: Not a valid histo. bwid = " << bwid << std::endl;
    return 0.;
  }
  Double_t xmax = xaxis->GetXmax();
  Double_t xmin = xaxis->GetXmin();
  Double_t ave = hist->GetMean();
  Double_t rms = hist->GetRMS();

  Double_t total=0.;
  for(Int_t i=0; i<nb+2; i++) {
    total+=hist->GetBinContent(i);
  }
  if(total < 100.) {
    std::cout << "effsigma: Too few entries " << total << std::endl;
    return 0.;
  }
  Int_t ierr=0;
  Int_t ismin=999;

  Double_t rlim=0.683*total;
  Int_t nrms=rms/(bwid);    // Set scan size to +/- rms
  if(nrms > nb/10) nrms=nb/10; // Could be tuned...

  Double_t widmin=9999999.;
  for(Int_t iscan=-nrms;iscan<nrms+1;iscan++) { // Scan window centre
    Int_t ibm=(ave-xmin)/bwid+1+iscan;
    Double_t x=(ibm-0.5)*bwid+xmin;
    Double_t xj=x;
    Double_t xk=x;
    Int_t jbm=ibm;
    Int_t kbm=ibm;
    Double_t bin=hist->GetBinContent(ibm);
    total=bin;
    for(Int_t j=1;j<nb;j++){
      if(jbm < nb) {
        jbm++;
        xj+=bwid;
        bin=hist->GetBinContent(jbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
      if(kbm > 0) {
        kbm--;
        xk-=bwid;
        bin=hist->GetBinContent(kbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
    }
    Double_t dxf=(total-rlim)*bwid/bin;
    Double_t wid=(xj-xk+bwid-dxf)*0.5;
    if(wid < widmin) {
      widmin=wid;
      ismin=iscan;
    }
  }
  if(ismin == nrms || ismin == -nrms) ierr=3;
  if(ierr != 0) std::cout << "effsigma: Error of type " << ierr << std::endl;

  return widmin;
}
