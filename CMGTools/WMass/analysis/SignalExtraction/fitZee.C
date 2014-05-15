//================================================================================================
// 
// Perform fit to extract Z->ee signal and efficiency simultaneously
//
//  * outputs plots and fit results summary
//
//________________________________________________________________________________________________

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                        // access to gROOT, entry point to ROOT system
#include <TSystem.h>                      // interface to OS
#include <TStyle.h>                       // class to handle ROOT plotting styles
#include <TFile.h>                        // file handle class
#include <TTree.h>                        // class to access ntuples
#include <TBenchmark.h>                   // class to track macro running statistics
#include <TH1D.h>                         // histogram class
#include <vector>                         // STL vector class
#include <iostream>                       // standard I/O
#include <iomanip>                        // functions to format standard I/O
#include <fstream>                        // functions for file I/O
#include <string>                         // C++ string class
#include <sstream>                        // class for parsing strings
#include "Math/LorentzVector.h"           // 4-vector class

#include "Utils/MyTools.hh"	          // various helper functions
#include "Utils/CPlot.hh"	          // helper class for plots
#include "Utils/MitStyleRemix.hh"      // style settings for drawing

#include "Utils/ZSignals.hh"           // define models for Z signal PDFs
#include "Utils/ZBackgrounds.hh"       // define models for background PDFs

// RooFit headers
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooCategory.h"
#include "RooArgList.h"
#include "RooDataHist.h"
#include "RooFormulaVar.h"
#include "RooHistPdf.h"
#include "RooGenericPdf.h"
#include "RooAddPdf.h"
#include "RooExtendPdf.h"
#include "RooSimultaneous.h"
#include "RooGaussian.h"
#include "RooPoisson.h"
#include "RooNLLVar.h"
#include "RooConstVar.h"
#include "RooMinuit.h"
#include "RooFitResult.h"
#endif

typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > LorentzVector;

enum { eCount, eBWxCB, eMCxGaus };
enum { eNone, eExp, eErfcExp, eDblExp, eLinExp, eQuadExp };


//=== FUNCTION DECLARATIONS ======================================================================================

// make data-fit difference plots
TH1D* makeDiffHist(TH1D* hData, TH1D* hFit, const TString name);

// make webpage
void makeHTML(const TString outDir);

// obtain a signal+background PDF
RooAbsPdf* getModel(Int_t sigType, Int_t bkgType, const char *label, TH1D *histTemplate,
                    RooRealVar &m, RooFormulaVar &NfitSig, RooRealVar &NfitBkg);


// print correlations of fitted parameters
void printCorrelations(ostream& os, RooFitResult *res);

// print chi2 test and KS test results
void printChi2AndKSResults(ostream& os, 
                           const Double_t chi2prob, const Double_t chi2ndf, 
                           const Double_t ksprob, const Double_t ksprobpe);


//=== MAIN MACRO ================================================================================================= 

void fitZee(const TString  outputDir,   // output directory
            const Double_t lumi,        // integrated luminosity (/fb)
	    const Int_t    Ecm,         // center-of-mass energy
	    const Int_t    doPU         // option for PU-reweighting
) {
  gBenchmark->Start("fitZee");

  //--------------------------------------------------------------------------------------------------------------
  // Settings 
  //==============================================================================================================   
  
  //
  // input ntuple file names
  //
  enum { eData, eZee, eEWK };  // data type enum
  vector<TString> fnamev;
  vector<Int_t>   typev;
  
  fnamev.push_back("../Selection/Zee/ntuples/data_m23_select.root");   typev.push_back(eData);
  fnamev.push_back("../Selection/Zee/ntuples/zee_select.root");        typev.push_back(eZee);
  fnamev.push_back("../Selection/Zee/ntuples/ewk_select.root");        typev.push_back(eEWK);
  fnamev.push_back("../Selection/Zee/ntuples/top_select.root");        typev.push_back(eEWK);

  //
  // Fit options
  //
  const Bool_t doBinned = kTRUE;
  
  const Int_t    NBINS     = 30;
  const Double_t MASS_LOW  = 60;
  const Double_t MASS_HIGH = 120;  
  const Double_t PT_CUT    = 25;
  const Double_t ETA_CUT   = 2.5;
  
  const Int_t typeEleEleNoSelSig = eMCxGaus;
  const Int_t typeEleEleNoSelBkg = eLinExp;
  const Int_t typeEleSCSig       = eMCxGaus;
  const Int_t typeEleSCBkg       = eErfcExp;
  
  // plot output file format
  const TString format("png");
  
  // pile-up re-weight file
  const TString pufname("Utils/PileupReweighting.Summer11DYmm_To_Run2011A.root");
 
   
  //--------------------------------------------------------------------------------------------------------------
  // Main analysis code 
  //==============================================================================================================  

  // event category enumeration
  enum { eEleEle2HLT=1, eEleEle1HLT, eEleEleNoSel, eEleSC };
  
  // Create output directory
  gSystem->mkdir(outputDir,kTRUE);
  CPlot::sOutDir = outputDir;  
  
  // Get pile-up weights
  TFile *pufile    = 0;
  TH1D  *puWeights = 0;
  if(doPU) {
    pufile = new TFile(pufname);
    assert(pufile);
    puWeights = (TH1D*)pufile->Get("puWeights");
    assert(puWeights);
  }
    
  //
  // Trees/histograms to store events in each category for
  // unbinned/binned fits
  //
  Float_t mass;
  
  TTree *treeEleEle2HLT = new TTree("treeEleEle2HLT","treeEleEle2HLT");
  treeEleEle2HLT->Branch("m",&mass,"m/F");
  treeEleEle2HLT->SetDirectory(0);
  TH1D *hEleEle2HLT = new TH1D("hEleEle2HLT","",NBINS,MASS_LOW,MASS_HIGH);
  hEleEle2HLT->Sumw2();
  
  TTree *treeEleEle1HLT = new TTree("treeEleEle1HLT","treeEleEle1HLT");
  treeEleEle1HLT->Branch("m",&mass,"m/F");
  treeEleEle1HLT->SetDirectory(0);
  TH1D *hEleEle1HLT = new TH1D("hEleEle1HLT","",NBINS,MASS_LOW,MASS_HIGH);
  hEleEle1HLT->Sumw2();
  
  TTree *treeEleEleNoSel = new TTree("treeEleEleNoSel","treeEleEleNoSel");
  treeEleEleNoSel->Branch("m",&mass,"m/F");
  treeEleEleNoSel->SetDirectory(0);
  TH1D *hEleEleNoSel = new TH1D("hEleEleNoSel","",NBINS,MASS_LOW,MASS_HIGH);
  hEleEleNoSel->Sumw2();
  
  TTree *treeEleSC = new TTree("treeEleSC","treeEleSC");
  treeEleSC->Branch("m",&mass,"m/F");
  treeEleSC->SetDirectory(0);
  TH1D *hEleSC = new TH1D("hEleSC","",NBINS,MASS_LOW,MASS_HIGH);
  hEleSC->Sumw2();

  // histogram for MC-based templates
  TH1D *tmpltEleEle2HLT  = (TH1D*)hEleEle2HLT->Clone("tmpltEleEle2HLT");
  TH1D *tmpltEleEle1HLT  = (TH1D*)hEleEle1HLT->Clone("tmpltEleEle1HLT");
  TH1D *tmpltEleEleNoSel = (TH1D*)hEleEleNoSel->Clone("tmpltEleEleNoSel");
  TH1D *tmpltEleSC       = (TH1D*)hEleSC->Clone("tmpltEleSC");
  
  // histograms for full selection (EleEle2HLT + EleEle1HLT)
  TH1D *hData = new TH1D("hData","",NBINS,MASS_LOW,MASS_HIGH); hData->Sumw2();
  TH1D *hZee  = new TH1D("hZee", "",NBINS,MASS_LOW,MASS_HIGH); hZee->Sumw2();
  TH1D *hEWK  = new TH1D("hEWK", "",NBINS,MASS_LOW,MASS_HIGH); hEWK->Sumw2();
  TH1D *hMC   = new TH1D("hMC",  "",NBINS,MASS_LOW,MASS_HIGH); hMC->Sumw2();
    
  //
  // Declare variables to read in ntuple
  //
  UInt_t  runNum, lumiSec, evtNum;
  UInt_t  matchGen;
  UInt_t  category;
  UInt_t  npv, npu;
  Float_t genZPt, genZPhi;
  Float_t scale1fb;
  Float_t met, metPhi, sumEt, u1, u2;
  Int_t   q1, q2;
  LorentzVector *dilep=0, *lep1=0, *lep2=0;
  LorentzVector *sc1=0, *sc2=0;

  TFile *infile=0;
  TTree *intree=0;

  for(UInt_t ifile=0; ifile<fnamev.size(); ifile++) {
  
    // Read input file and get the TTrees
    cout << "Processing " << fnamev[ifile] << "..." << endl;
    infile = new TFile(fnamev[ifile]);	    assert(infile);
    intree = (TTree*)infile->Get("Events"); assert(intree);

    intree->SetBranchAddress("runNum",   &runNum);     // event run number
    intree->SetBranchAddress("lumiSec",  &lumiSec);    // event lumi section
    intree->SetBranchAddress("evtNum",   &evtNum);     // event number
    intree->SetBranchAddress("matchGen", &matchGen);   // event has both leptons matched to MC Z->ll
    intree->SetBranchAddress("category", &category);   // dilepton category
    intree->SetBranchAddress("npv",      &npv);	       // number of primary vertices
    intree->SetBranchAddress("npu",      &npu);	       // number of in-time PU events (MC)
    intree->SetBranchAddress("genZPt",   &genZPt);     // GEN Z boson pT (signal MC)
    intree->SetBranchAddress("genZPhi",  &genZPhi);    // GEN Z boson phi (signal MC)
    intree->SetBranchAddress("scale1fb", &scale1fb);   // event weight per 1/fb (MC)
    intree->SetBranchAddress("met",      &met);	       // MET
    intree->SetBranchAddress("metPhi",   &metPhi);     // phi(MET)
    intree->SetBranchAddress("sumEt",    &sumEt);      // Sum ET
    intree->SetBranchAddress("u1",       &u1);	       // parallel component of recoil
    intree->SetBranchAddress("u2",       &u2);	       // perpendicular component of recoil
    intree->SetBranchAddress("q1",       &q1);	       // charge of tag lepton
    intree->SetBranchAddress("q2",       &q2);	       // charge of probe lepton
    intree->SetBranchAddress("dilep",    &dilep);      // dilepton 4-vector
    intree->SetBranchAddress("lep1",     &lep1);       // tag lepton 4-vector
    intree->SetBranchAddress("lep2",     &lep2);       // probe lepton 4-vector
    intree->SetBranchAddress("sc1",      &sc1);        // tag Supercluster 4-vector
    intree->SetBranchAddress("sc2",      &sc2);        // probe Supercluster 4-vector 
  
    //
    // loop over events
    //
    for(UInt_t ientry=0; ientry<intree->GetEntries(); ientry++) {
      intree->GetEntry(ientry);

      if(dilep->M()       < MASS_LOW)  continue;
      if(dilep->M()       > MASS_HIGH) continue;
      if(sc1->Pt()        < PT_CUT)    continue;
      if(sc2->Pt()        < PT_CUT)    continue;
      if(fabs(sc1->Eta()) > ETA_CUT)   continue;      
      if(fabs(sc2->Eta()) > ETA_CUT)   continue;
   
      mass = dilep->M();
      
      Double_t weight=1;
      if(typev[ifile]!=eData) {
	weight *= scale1fb*lumi;
	if(doPU>0) weight *= puWeights->GetBinContent(npu+1);
      }
    
      // fill data events for each category
      if(typev[ifile]==eData) {
        if     (category == eEleEle2HLT)  { treeEleEle2HLT->Fill();  hEleEle2HLT->Fill(mass); }
        else if(category == eEleEle1HLT)  { treeEleEle1HLT->Fill();  hEleEle1HLT->Fill(mass); }
        else if(category == eEleEleNoSel) { treeEleEleNoSel->Fill(); hEleEleNoSel->Fill(mass); }
        else if(category == eEleSC)       { treeEleSC->Fill();       hEleSC->Fill(mass); }
      }
      
      // fill gen-matched Z events each category for MC templates and to compute MC efficiencies
      if(typev[ifile]==eZee && matchGen) {
        Double_t w=1;
	if(doPU>0) w *= puWeights->GetBinContent(npu+1);
	if     (category == eEleEle2HLT)  { tmpltEleEle2HLT->Fill(mass,w); }
        else if(category == eEleEle1HLT)  { tmpltEleEle1HLT->Fill(mass,w); }
	else if(category == eEleEleNoSel) { tmpltEleEleNoSel->Fill(mass,w); }
        else if(category == eEleSC)       { tmpltEleSC->Fill(mass,w); }
      }
      
      // fill Z events passing selection (EleEle2HLT + EleEle1HLT)
      if((category==eEleEle2HLT) || (category==eEleEle1HLT)) {
        if(typev[ifile]==eData) { 
	  hData->Fill(mass); 
	
	} else {
	  hMC->Fill(mass,weight);
	  if(typev[ifile]==eZee) { hZee->Fill(mass,weight); }
	  if(typev[ifile]==eEWK) { hEWK->Fill(mass,weight); }
	}
      }
    }
    
    delete infile;
    infile=0, intree=0;
  }
  
  
  //
  // Set up parameters and PDFs for fitting
  //
       
  RooRealVar m("m","m",MASS_LOW,MASS_HIGH);
  m.setBins(NBINS);
  
  // Combine low purity categories into one dataset for simultaneous fitting
  // The golden categories 2HLT and 1HLT are not fit: they enter minimization
  // via the constraint terms.
  RooCategory rooCat("rooCat","rooCat");
  rooCat.defineType("EleEleNoSel");
  rooCat.defineType("EleSC");
    
  RooAbsData *dataEleEleNoSel=0;
  RooAbsData *dataEleSC=0;
  RooAbsData *dataNonGolden=0;
  
  if(doBinned) {
    dataEleEleNoSel = new RooDataHist("dataEleEleNoSel","dataEleEleNoSel",RooArgSet(m),hEleEleNoSel);
    dataEleSC	    = new RooDataHist("dataEleSC",      "dataEleSC",      RooArgSet(m),hEleSC);
    dataNonGolden   = new RooDataHist("dataNonGolden",  "dataNonGolden", RooArgList(m), Index(rooCat),
                                    Import("EleEleNoSel", *((RooDataHist*)dataEleEleNoSel)),
				    Import("EleSC",       *((RooDataHist*)dataEleSC)));
  } else {  
    dataEleEleNoSel = new RooDataSet("dataEleEleNoSel","dataEleEleNoSel",treeEleEleNoSel,RooArgSet(m));
    dataEleSC	    = new RooDataSet("dataEleSC",      "dataEleSC",	 treeEleSC,      RooArgSet(m));
    dataNonGolden   = new RooDataSet("dataNonGolden",  "dataNonGolden", RooArgList(m), Index(rooCat),
                                   Import("EleEleNoSel", *((RooDataSet*)dataEleEleNoSel)),
				   Import("EleSC",       *((RooDataSet*)dataEleSC)));
  }
  
  // Primary parameters
  UInt_t nEleEle2HLT  = treeEleEle2HLT->GetEntries();
  UInt_t nEleEle1HLT  = treeEleEle1HLT->GetEntries();
  UInt_t nEleEleNoSel = treeEleEleNoSel->GetEntries();
  UInt_t nEleSC     = treeEleSC->GetEntries();
  UInt_t NzMax = 2*(nEleEle2HLT + nEleEle1HLT + nEleEleNoSel + nEleSC);
  RooRealVar Nz("Nz","Nz",nEleEle2HLT+nEleEle1HLT,0,NzMax);
  RooRealVar effHLT("effHLT","effHLT",0.95,0.80,1.0);
  RooRealVar effSel("effSel","effSel",0.75,0.60,1.0);
  RooRealVar effGsf("effGsf","effGsf",0.98,0.90,1.0); 
  
  // The expected background yields in the non-golden samples for extended likelihood
  RooRealVar NfitBkgEleEleNoSel("NfitBkgEleEleNoSel","NfitBkgEleEleNoSel",0.2*nEleEleNoSel,0,nEleEleNoSel);
  RooRealVar NfitBkgEleSC    ("NfitBkgEleSC",    "NfitBkgEleSC",    0.2*nEleSC,    0,nEleSC);
  
  // The expected numbers of signal events in each sample
  RooFormulaVar NfitEleEle2HLT ("NfitEleEle2HLT", "NfitEleEle2HLT", "Nz*effHLT*effHLT*effGsf*effGsf*effSel*effSel",      RooArgList(Nz,effHLT,effGsf,effSel));
  RooFormulaVar NfitEleEle1HLT ("NfitEleEle1HLT", "NfitEleEle1HLT", "2*Nz*effHLT*(1-effHLT)*effGsf*effGsf*effSel*effSel",RooArgList(Nz,effHLT,effGsf,effSel));
  RooFormulaVar NfitEleEleNoSel("NfitEleEleNoSel","NfitEleEleNoSel","2*Nz*effHLT*effGsf*effGsf*effSel*(1-effSel)",       RooArgList(Nz,effHLT,effGsf,effSel)); 
  RooFormulaVar NfitEleSC      ("NfitEleSC",      "NfitEleSC",      "2*Nz*effHLT*effGsf*(1-effGsf)*effSel",              RooArgList(Nz,effHLT,effGsf,effSel));
  
  //
  // Put together total PDFs
  //
  RooAbsPdf *pdfEleEleNoSel = getModel(typeEleEleNoSelSig, typeEleEleNoSelBkg, "EleEleNoSel", tmpltEleEleNoSel, m, NfitEleEleNoSel, NfitBkgEleEleNoSel);
  RooAbsPdf *pdfEleSC       = getModel(typeEleSCSig,       typeEleSCBkg,       "EleSC",       tmpltEleSC,       m, NfitEleSC,       NfitBkgEleSC); 

  // PDF for simultaneous fit
  RooSimultaneous pdfTotal("pdfTotal","pdfTotal",rooCat);
  pdfTotal.addPdf(*pdfEleEleNoSel,"EleEleNoSel");
  pdfTotal.addPdf(*pdfEleSC,      "EleSC");			     
                             
  //
  // Define likelihood, add constraints, and run the fit
  //
  
  // Extra terms to likelihood
  RooGaussian constraintEleEle2HLT("constraintEleEle2HLT","constraintEleEle2HLT", NfitEleEle2HLT, RooConst(nEleEle2HLT), RooConst(sqrt(nEleEle2HLT)));
  RooGaussian constraintEleEle1HLT("constraintEleEle1HLT","constraintEleEle1HLT", NfitEleEle1HLT, RooConst(nEleEle1HLT), RooConst(sqrt(nEleEle1HLT)));
  RooGaussian constraintEleEleNoSel("constraintEleEleNoSel","constraintEleEleNoSel", NfitEleEleNoSel, RooConst(nEleEleNoSel), RooConst(sqrt(nEleEleNoSel)));
  RooGaussian constraintEleSC("constraintEleSC","constraintEleSC", NfitEleSC, RooConst(nEleSC), RooConst(sqrt(nEleSC)));
/*  
  RooPoisson constraintEleEle2HLT("constraintEleEle2HLT","constraintEleEle2HLT", NfitEleEle2HLT, RooConst(nEleEle2HLT));
  RooPoisson constraintEleEle1HLT("constraintEleEle1HLT","constraintEleEle1HLT", NfitEleEle1HLT, RooConst(nEleEle1HLT));
  RooPoisson constraintEleEleNoSel("constraintEleEleNoSel","constraintEleEleNoSel", NfitEleEleNoSel, RooConst(nEleEleNoSel));
  RooPoisson constraintEleSC("constraintEleSC","constraintEleSC", NfitEleSC, RooConst(nEleSC));
*/
  // Define goodness of fit including the constraints
  RooArgList fitConstraints;
  fitConstraints.add(constraintEleEle2HLT);
  fitConstraints.add(constraintEleEle1HLT);
  if(typeEleEleNoSelSig==eCount) fitConstraints.add(constraintEleEleNoSel);
  if(typeEleSCSig==eCount)     fitConstraints.add(constraintEleSC);

  RooFitResult *result = pdfTotal.fitTo(*dataNonGolden,
                                        Extended(kTRUE),
					Strategy(1),
					ExternalConstraints(fitConstraints),
					Save(kTRUE));

  //
  // Use histogram version of fitted PDFs to make ratio plots
  // (Will also use PDF histograms later for Chi^2 and KS tests)
  //
  TH1D *hPdfEleEleNoSel = (TH1D*)(pdfEleEleNoSel->createHistogram("hPdfEleEleNoSel", m));
  hPdfEleEleNoSel->Scale((NfitEleEleNoSel.getVal() + NfitBkgEleEleNoSel.getVal())/hPdfEleEleNoSel->Integral());
  TH1D *hEleEleNoSelDiff = makeDiffHist(hEleEleNoSel,hPdfEleEleNoSel,"hEleEleNoSelDiff");
  hEleEleNoSelDiff->SetMarkerStyle(kFullCircle);
  hEleEleNoSelDiff->SetMarkerSize(0.9);
   
  TH1D *hPdfEleSC = (TH1D*)(pdfEleSC->createHistogram("hPdfEleSC", m));
  hPdfEleSC->Scale((NfitEleSC.getVal() + NfitBkgEleSC.getVal())/hPdfEleSC->Integral());
  TH1D *hEleSCDiff = makeDiffHist(hEleSC,hPdfEleSC,"hEleSCDiff");
  hEleSCDiff->SetMarkerStyle(kFullCircle);
  hEleSCDiff->SetMarkerSize(0.9);

Double_t crapscale = hData->Integral()/hMC->Integral();
hMC->Scale(crapscale);

  TH1D *hZeeDiff = makeDiffHist(hData,hMC,"hZeeDiff");
  hZeeDiff->SetMarkerStyle(kFullCircle); 
  hZeeDiff->SetMarkerSize(0.9);
  
  //
  // Solve simultaneous equations for signal MC
  // Exact solution:
  //   effHLT = 2*n1/(2*n1+n2)
  //   effSel = 1 - 0.5*n3/(n1+n2)*(1-(1-effHLT)*(1-effHLT))
  //   effGsf = 1 - 0.5*n4/(n1+n2)*(1-(1-effHLT)*(1-effHLT))*effSel
  //
  // Below, we fit the MC events to a histogram template of itself
  // to effectively "count" the yields in each category
  //
  RooCategory rooCat2("rooCat2","rooCat2");
  rooCat2.defineType("EleEle2HLT2");
  rooCat2.defineType("EleEle1HLT2");
  rooCat2.defineType("EleEleNoSel2");
  rooCat2.defineType("EleSC2");
    
  // Primary parameters
  Double_t NzeeMax = 1.5*(tmpltEleEle2HLT->Integral() + tmpltEleEle1HLT->Integral() + tmpltEleEleNoSel->Integral() + tmpltEleSC->Integral());
  RooRealVar Nzee("Nzee","Nzee",0.65*NzeeMax,0,NzeeMax);
  RooRealVar effHLT_Zee("effHLT_Zee","effHLT_Zee",0.95,0.80,1.0);
  RooRealVar effSel_Zee("effSel_Zee","effSel_Zee",0.75,0.60,1.0);
  RooRealVar effGsf_Zee("effGsf_Zee","effGsf_Zee",0.98,0.90,1.0);
  
  // The expected numbers of signal events in each sample
  RooFormulaVar NfitEleEle2HLT_Zee("NfitEleEle2HLT_Zee","NfitEleEle2HLT_Zee",
				 "Nzee*effHLT_Zee*effHLT_Zee*effGsf_Zee*effGsf_Zee*effSel_Zee*effSel_Zee",
                                 RooArgList(Nzee,effHLT_Zee,effGsf_Zee,effSel_Zee));
  RooFormulaVar NfitEleEle1HLT_Zee("NfitEleEle1HLT_Zee","NfitEleEle1HLT_Zee",
                                 "2*Nzee*effHLT_Zee*(1-effHLT_Zee)*effGsf_Zee*effGsf_Zee*effSel_Zee*effSel_Zee",
                                 RooArgList(Nzee,effHLT_Zee,effGsf_Zee,effSel_Zee));
  RooFormulaVar NfitEleEleNoSel_Zee("NfitEleEleNoSel_Zee","NfitEleEleNoSel_Zee", 
                                  "2*Nzee*effHLT_Zee*effGsf_Zee*effGsf_Zee*effSel_Zee*(1-effSel_Zee)",
                                  RooArgList(Nzee,effHLT_Zee,effGsf_Zee,effSel_Zee)); 
  RooFormulaVar NfitEleSC_Zee("NfitEleSC_Zee","NfitEleSC_Zee",
                              "2*Nzee*effHLT_Zee*effGsf_Zee*(1-effGsf_Zee)*effSel_Zee",
                              RooArgList(Nzee,effHLT_Zee,effGsf_Zee,effSel_Zee)); 
  
  RooRealVar NfitBkgDueey("NfitBkgDueey","NfitBkgDueey",10,0,100); 
  
  RooDataHist zeeEleEle2HLT ("zeeEleEle2HLT", "zeeEleEle2HLT", RooArgSet(m),tmpltEleEle2HLT);
  RooDataHist zeeEleEle1HLT ("zeeEleEle1HLT", "zeeEleEle1HLT", RooArgSet(m),tmpltEleEle1HLT);
  RooDataHist zeeEleEleNoSel("zeeEleEleNoSel","zeeEleEleNoSel",RooArgSet(m),tmpltEleEleNoSel);
  RooDataHist zeeEleSC	    ("zeeEleSC",      "zeeEleSC",      RooArgSet(m),tmpltEleSC);
  RooDataHist zeeData("zeeData","zeeData", RooArgList(m), Index(rooCat2),
                      Import("EleEle2HLT2",  zeeEleEle2HLT),
		      Import("EleEle1HLT2",  zeeEleEle1HLT),
		      Import("EleEleNoSel2", zeeEleEleNoSel),
		      Import("EleSC2",       zeeEleSC));
  
  RooAbsPdf *pdfZeeEleEle2HLT  = getModel(eCount, eNone, "ZeeEleEle2HLT",  tmpltEleEle2HLT,  m, NfitEleEle2HLT_Zee,  NfitBkgDueey);
  RooAbsPdf *pdfZeeEleEle1HLT  = getModel(eCount, eNone, "ZeeEleEle1HLT",  tmpltEleEle1HLT,  m, NfitEleEle1HLT_Zee,  NfitBkgDueey);
  RooAbsPdf *pdfZeeEleEleNoSel = getModel(eCount, eNone, "ZeeEleEleNoSel", tmpltEleEleNoSel, m, NfitEleEleNoSel_Zee, NfitBkgDueey);
  RooAbsPdf *pdfZeeEleSC     = getModel(eCount, eNone, "ZeeEleSC",     tmpltEleSC,     m, NfitEleSC_Zee,     NfitBkgDueey);			   			  
  
  RooSimultaneous pdfZee("pdfZee","pdfZee",rooCat2);
  pdfZee.addPdf(*pdfZeeEleEle2HLT, "EleEle2HLT2");
  pdfZee.addPdf(*pdfZeeEleEle1HLT, "EleEle1HLT2");  
  pdfZee.addPdf(*pdfZeeEleEleNoSel,"EleEleNoSel2");
  pdfZee.addPdf(*pdfZeeEleSC,    "EleSC2");  

  RooFitResult *zeeResult = pdfZee.fitTo(zeeData, Extended(kTRUE), Save(kTRUE));    


  //--------------------------------------------------------------------------------------------------------------
  // Make plots 
  //==============================================================================================================  

  char ylabel[100];     // string buffer for y-axis label
  char cattext[100];    // string buffer for category label
  char yieldtext[100];  // string buffer for category yield
  char nsigtext[100];   // string buffer for Nsig text
  char nbkgtext[100];   // string buffer for Nbkg text
  
  // label for lumi
  char lumitext[100];
  if(lumi<0.1) sprintf(lumitext,"%.1f pb^{-1}  at  #sqrt{s} = %i TeV",lumi*1000.,Ecm);
  else         sprintf(lumitext,"%.2f fb^{-1}  at  #sqrt{s} = %i TeV",lumi,Ecm);  
  
  // plot colors
  Int_t linecolorZ   = kOrange-3;
  Int_t fillcolorZ   = kOrange-2;
  Int_t linecolorEWK = kOrange+10;
  Int_t fillcolorEWK = kOrange+7;
  Int_t ratioColor   = kGray+2;
  
  //
  // Dummy histograms for TLegend
  // (I can't figure out how to properly pass RooFit objects...)
  //
  TH1D *hDummyData = new TH1D("hDummyData","",0,0,10);
  hDummyData->SetMarkerStyle(kFullCircle);
  hDummyData->SetMarkerSize(0.9);
  
  TH1D *hDummyZ = new TH1D("hDummyZ","",0,0,10);
  hDummyZ->SetLineColor(linecolorZ);
  hDummyZ->SetFillColor(fillcolorZ);
  hDummyZ->SetFillStyle(1001);
  
  TH1D *hDummyEWK = new TH1D("hDummyEWK","",0,0,10);
  hDummyEWK->SetLineColor(linecolorEWK);
  hDummyEWK->SetFillColor(fillcolorEWK);
  hDummyEWK->SetFillStyle(1001);
  
  
  TCanvas *c0 = MakeCanvas("c0","c0",800,800);
  c0->SetTickx(1);
  c0->SetTicky(1);
  
  //
  // EleEle2HLT category
  //
  sprintf(ylabel,"Events / %.1f GeV/c^{2}",hEleEle2HLT->GetBinWidth(1));
  sprintf(cattext,"e + e (2 HLT)");
  sprintf(yieldtext,"%i events",(Int_t)hEleEle2HLT->Integral());
  hEleEle2HLT->GetXaxis()->CenterTitle();
  CPlot plotEleEle2HLT("eleele2hlt","","mass [GeV/c^{2}]",ylabel);
  plotEleEle2HLT.AddHist1D(hEleEle2HLT,"histE");
  plotEleEle2HLT.AddTextBox(cattext,0.23,0.77,0.43,0.83,0);
  plotEleEle2HLT.AddTextBox(yieldtext,0.23,0.68,0.43,0.74,0);
  plotEleEle2HLT.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotEleEle2HLT.Draw(c0,kTRUE,format);
  
  sprintf(ylabel,"Events / %.1f GeV/c^{2}",tmpltEleEle2HLT->GetBinWidth(1));
  sprintf(cattext,"e + e (2 HLT)");
  sprintf(yieldtext,"%i events",(Int_t)tmpltEleEle2HLT->Integral());
  tmpltEleEle2HLT->GetXaxis()->CenterTitle();
  CPlot plotEleEle2HLT_Zee("eleele2hlt_zee","","mass [GeV/c^{2}]",ylabel);
  plotEleEle2HLT_Zee.AddHist1D(tmpltEleEle2HLT,"histE");
  plotEleEle2HLT_Zee.AddTextBox(cattext,0.23,0.77,0.43,0.83,0);
  plotEleEle2HLT_Zee.AddTextBox(yieldtext,0.23,0.68,0.43,0.74,0);
  plotEleEle2HLT_Zee.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotEleEle2HLT_Zee.Draw(c0,kTRUE,format);

  //
  // EleEle1HLT category
  //
  sprintf(ylabel,"Events / %.1f GeV/c^{2}",hEleEle1HLT->GetBinWidth(1));
  sprintf(cattext,"e + e (1 HLT)");
  sprintf(yieldtext,"%i events",(Int_t)hEleEle1HLT->Integral());
  hEleEle1HLT->GetXaxis()->CenterTitle();
  CPlot plotEleEle1HLT("eleele1hlt","","mass [GeV/c^{2}]",ylabel);
  plotEleEle1HLT.AddHist1D(hEleEle1HLT,"histE");
  plotEleEle1HLT.AddTextBox(cattext,0.23,0.77,0.43,0.83,0);
  plotEleEle1HLT.AddTextBox(yieldtext,0.23,0.68,0.43,0.74,0);
  plotEleEle1HLT.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotEleEle1HLT.Draw(c0,kTRUE,format);
  
  sprintf(ylabel,"Events / %.1f GeV/c^{2}",tmpltEleEle1HLT->GetBinWidth(1));
  sprintf(cattext,"e + e (1 HLT)");
  sprintf(yieldtext,"%i events",(Int_t)tmpltEleEle1HLT->Integral());
  tmpltEleEle1HLT->GetXaxis()->CenterTitle();
  CPlot plotEleEle1HLT_Zee("eleele1hlt_zee","","mass [GeV/c^{2}]",ylabel);
  plotEleEle1HLT_Zee.AddHist1D(tmpltEleEle1HLT,"histE");
  plotEleEle1HLT_Zee.AddTextBox(cattext,0.23,0.77,0.43,0.83,0);
  plotEleEle1HLT_Zee.AddTextBox(yieldtext,0.23,0.68,0.43,0.74,0);
  plotEleEle1HLT_Zee.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotEleEle1HLT_Zee.Draw(c0,kTRUE,format);
  
  //
  // EleEleNoSel category
  //
  sprintf(ylabel,"Events / %.1f GeV/c^{2}",tmpltEleEleNoSel->GetBinWidth(1));
  sprintf(cattext,"e + e_{No Sel}");
  sprintf(yieldtext,"%i events",(Int_t)tmpltEleEleNoSel->Integral());
  tmpltEleEleNoSel->GetXaxis()->CenterTitle();
  CPlot plotEleEleNoSel_Zee("eleelenosel_zee","","mass [GeV/c^{2}]",ylabel);
  plotEleEleNoSel_Zee.AddHist1D(tmpltEleEleNoSel,"histE");
  plotEleEleNoSel_Zee.AddTextBox(cattext,0.23,0.77,0.43,0.83,0);
  plotEleEleNoSel_Zee.AddTextBox(yieldtext,0.23,0.68,0.43,0.74,0);
  plotEleEleNoSel_Zee.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotEleEleNoSel_Zee.Draw(c0,kTRUE,format);
    
  //
  // EleSC category
  //
  sprintf(ylabel,"Events / %.1f GeV/c^{2}",tmpltEleSC->GetBinWidth(1));
  sprintf(cattext,"e + SC");
  sprintf(yieldtext,"%i events",(Int_t)tmpltEleSC->Integral());
  tmpltEleSC->GetXaxis()->CenterTitle();
  CPlot plotEleSC_Zee("elesc_zee","","mass [GeV/c^{2}]",ylabel);
  plotEleSC_Zee.AddHist1D(tmpltEleSC,"histE");
  plotEleSC_Zee.AddTextBox(cattext,0.23,0.77,0.43,0.83,0);
  plotEleSC_Zee.AddTextBox(yieldtext,0.23,0.68,0.43,0.74,0);
  plotEleSC_Zee.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotEleSC_Zee.Draw(c0,kTRUE,format);
    
  
  TCanvas *c = MakeCanvas("c","c",800,800);
  c->Divide(1,2,0,0);
  c->cd(1)->SetPad(0,0.3,1.0,1.0);
  c->cd(1)->SetTopMargin(0.1);
  c->cd(1)->SetBottomMargin(0.01);
  c->cd(1)->SetLeftMargin(0.18);  
  c->cd(1)->SetRightMargin(0.07);  
  c->cd(1)->SetTickx(1);
  c->cd(1)->SetTicky(1);  
  c->cd(2)->SetPad(0,0,1.0,0.3);
  c->cd(2)->SetTopMargin(0.05);
  c->cd(2)->SetBottomMargin(0.45);
  c->cd(2)->SetLeftMargin(0.18);
  c->cd(2)->SetRightMargin(0.07);
  c->cd(2)->SetTickx(1);
  c->cd(2)->SetTicky(1);
  gStyle->SetTitleOffset(1.400,"Y");
  
  //
  // EleEleNoSel category
  //
  RooPlot *frame3 = m.frame(Bins(NBINS));    
  dataEleEleNoSel->plotOn(frame3,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  pdfEleEleNoSel->plotOn(frame3,LineColor(kBlue));
  if(typeEleEleNoSelBkg != eNone) 
    pdfEleEleNoSel->plotOn(frame3,Components("bkgEleEleNoSel"),LineColor(kRed),LineStyle(7));
  dataEleEleNoSel->plotOn(frame3,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));  
  
  sprintf(ylabel,"Events / %.1f GeV/c^{2}",hEleEleNoSel->GetBinWidth(1));
  sprintf(cattext,"e + e_{No Sel}");
  sprintf(yieldtext,"%i events",(Int_t)hEleEleNoSel->Integral());
  sprintf(nsigtext,"N_{sig} = %.1f #pm %.1f",NfitEleEleNoSel.getVal(),NfitEleEleNoSel.getPropagatedError(*result));
  sprintf(nbkgtext,"N_{bkg} = %.1f #pm %.1f",NfitBkgEleEleNoSel.getVal(),NfitBkgEleEleNoSel.getPropagatedError(*result));
  CPlot plotEleEleNoSel("eenosel",frame3,"","",ylabel);
  plotEleEleNoSel.AddTextBox(cattext,0.23,0.77,0.43,0.83,0);
  plotEleEleNoSel.AddTextBox(yieldtext,0.23,0.68,0.43,0.74,0);
  plotEleEleNoSel.AddTextBox(nsigtext,0.63,0.77,0.87,0.83,0);
  plotEleEleNoSel.AddTextBox(nbkgtext,0.63,0.71,0.87,0.77,0);
  plotEleEleNoSel.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotEleEleNoSel.SetYRange(0.01,1.1*(hEleEleNoSel->GetMaximum() + sqrt(hEleEleNoSel->GetMaximum())));
  plotEleEleNoSel.Draw(c,kFALSE,format,1);

  CPlot plotEleEleNoSelDiff("eleelenosel","","mass [GeV/c^{2}]","#chi");
  plotEleEleNoSelDiff.AddHist1D(hEleEleNoSelDiff,"EX0",ratioColor);
  plotEleEleNoSelDiff.SetYRange(-8,8);
  plotEleEleNoSelDiff.AddLine(MASS_LOW, 0,MASS_HIGH, 0,kBlack,1);
  plotEleEleNoSelDiff.AddLine(MASS_LOW, 5,MASS_HIGH, 5,kBlack,3);
  plotEleEleNoSelDiff.AddLine(MASS_LOW,-5,MASS_HIGH,-5,kBlack,3);
  plotEleEleNoSelDiff.Draw(c,kTRUE,format,2);
  
  //
  // EleSC category
  //
  RooPlot *frame4 = m.frame(Bins(NBINS));    
  dataEleSC->plotOn(frame4,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  pdfEleSC->plotOn(frame4,LineColor(kBlue));
  if(typeEleSCBkg != eNone) 
    pdfEleSC->plotOn(frame4,Components("bkgEleSC"),LineColor(kRed),LineStyle(7));
  dataEleSC->plotOn(frame4,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));  
  
  sprintf(ylabel,"Events / %.1f GeV/c^{2}",hEleSC->GetBinWidth(1));
  sprintf(cattext,"e + SC");
  sprintf(yieldtext,"%i events",(Int_t)hEleSC->Integral());
  sprintf(nsigtext,"N_{sig} = %.1f #pm %.1f",NfitEleSC.getVal(),NfitEleSC.getPropagatedError(*result));
  sprintf(nbkgtext,"N_{bkg} = %.1f #pm %.1f",NfitBkgEleSC.getVal(),NfitBkgEleSC.getPropagatedError(*result));
  CPlot plotEleSC("elesc",frame4,"","",ylabel);
  plotEleSC.AddTextBox(cattext,0.23,0.77,0.43,0.83,0);
  plotEleSC.AddTextBox(yieldtext,0.23,0.68,0.43,0.74,0);
  plotEleSC.AddTextBox(nsigtext,0.63,0.77,0.87,0.83,0);
  plotEleSC.AddTextBox(nbkgtext,0.63,0.71,0.87,0.77,0);
  plotEleSC.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotEleSC.SetYRange(0.01,1.1*(hEleSC->GetMaximum() + sqrt(hEleSC->GetMaximum())));
  plotEleSC.Draw(c,kFALSE,format,1);

  CPlot plotEleSCDiff("elesc","","mass [GeV/c^{2}]","#chi");
  plotEleSCDiff.AddHist1D(hEleSCDiff,"EX0",ratioColor);
  plotEleSCDiff.SetYRange(-8,8);
  plotEleSCDiff.AddLine(MASS_LOW, 0,MASS_HIGH, 0,kBlack,1);
  plotEleSCDiff.AddLine(MASS_LOW, 5,MASS_HIGH, 5,kBlack,3);
  plotEleSCDiff.AddLine(MASS_LOW,-5,MASS_HIGH,-5,kBlack,3);
  plotEleSCDiff.Draw(c,kTRUE,format,2);  
  
  //
  // EleEle2HLT + EleEle1HLT categories
  //   
  sprintf(ylabel,"Events / %.1f GeV/c^{2}",hData->GetBinWidth(1));
  CPlot plotZee("zee","","",ylabel);
  plotZee.AddHist1D(hData,"data","E");
hZee->Scale(crapscale);
  plotZee.AddToStack(hZee,"Z#rightarrowee",fillcolorZ,linecolorZ);
  plotZee.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotZee.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotZee.SetYRange(0.01,1.2*(hData->GetMaximum() + sqrt(hData->GetMaximum())));
  plotZee.TransLegend(-0.35,-0.15);
  plotZee.Draw(c,kFALSE,format,1);

  CPlot plotZeeDiff("zee","","M(e^{+}e^{-}) [GeV/c^{2}]","#chi");
  plotZeeDiff.AddHist1D(hZeeDiff,"EX0",ratioColor);
  plotZeeDiff.SetYRange(-8,8);
  plotZeeDiff.AddLine(MASS_LOW, 0,MASS_HIGH, 0,kBlack,1);
  plotZeeDiff.AddLine(MASS_LOW, 5,MASS_HIGH, 5,kBlack,3);
  plotZeeDiff.AddLine(MASS_LOW,-5,MASS_HIGH,-5,kBlack,3);
  plotZeeDiff.Draw(c,kTRUE,format,2);
  
  CPlot plotZee2("zeelog","","",ylabel);
  plotZee2.AddHist1D(hData,"data","E");
  plotZee2.AddToStack(hEWK,"EWK",fillcolorEWK,linecolorEWK);
  plotZee2.AddToStack(hZee,"Z#rightarrowee",fillcolorZ,linecolorZ);
  plotZee2.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);plotZee2.SetName("zeelog");
  plotZee2.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotZee2.SetLogy();
  plotZee2.SetYRange(1e-4*(hData->GetMaximum()),10*(hData->GetMaximum()));
  plotZee2.TransLegend(-0.35,-0.15);
  plotZee2.Draw(c,kTRUE,format,1);

    
  //--------------------------------------------------------------------------------------------------------------
  // Output
  //==============================================================================================================
   
  cout << "*" << endl;
  cout << "* SUMMARY" << endl;
  cout << "*--------------------------------------------------" << endl;  
  cout << endl;
  
  //
  // Write fit results
  //
  ofstream txtfile;
  char txtfname[100];    
  
  sprintf(txtfname,"%s/fitresZee.txt",CPlot::sOutDir.Data());
  txtfile.open(txtfname);
  assert(txtfile.is_open());
  
  txtfile << setw(20) << "" << "[Data]" << setw(25) << "" << "[MC]" << endl;
  txtfile << "   EleEle2HLT: " << setw(8) << nEleEle2HLT  << " events" << setw(15) << "" << setw(8) << tmpltEleEle2HLT->Integral()  << " events" << endl; 
  txtfile << "   EleEle1HLT: " << setw(8) << nEleEle1HLT  << " events" << setw(15) << "" << setw(8) << tmpltEleEle1HLT->Integral()  << " events" << endl; 
  txtfile << "  EleEleNoSel: " << setw(8) << nEleEleNoSel << " events" << setw(15) << "" << setw(8) << tmpltEleEleNoSel->Integral() << " events" << endl;
  txtfile << "        EleSC: " << setw(8) << nEleSC       << " events" << setw(15) << "" << setw(8) << tmpltEleSC->Integral()       << " events" << endl;    
  txtfile << endl;
  txtfile << "       N_Z: " << setw(10) << Nz.getVal()   << " +/- " << setw(10) << Nz.getPropagatedError(*result);
  txtfile << setw(5)  << "" << setw(10) << Nzee.getVal() << " +/- " << setw(10) << Nzee.getPropagatedError(*zeeResult) << endl;  

  txtfile << "  eff(HLT): " << setw(10) << effHLT.getVal()     << " +/- " << setw(10) << effHLT.getPropagatedError(*result);  
  txtfile << setw(5)  << "" << setw(10) << effHLT_Zee.getVal() << " +/- " << setw(10) << effHLT_Zee.getPropagatedError(*zeeResult);
  txtfile << setw(5)  << "" << setw(5)  << "  ||  " << effHLT.getVal()/effHLT_Zee.getVal() << endl;
  
  txtfile << "  eff(Sel): " << setw(10) << effSel.getVal()     << " +/- " << setw(10) << effSel.getPropagatedError(*result);  
  txtfile << setw(5)  << "" << setw(10) << effSel_Zee.getVal() << " +/- " << setw(10) << effSel_Zee.getPropagatedError(*zeeResult);
  txtfile << setw(5)  << "" << setw(5)  << "  ||  " << effSel.getVal()/effSel_Zee.getVal() << endl;
  
  txtfile << "  eff(GSF): " << setw(10) << effGsf.getVal()     << " +/- " << setw(10) << effGsf.getPropagatedError(*result);
  txtfile << setw(5)  << "" << setw(10) << effGsf_Zee.getVal() << " +/- " << setw(10) << effGsf_Zee.getPropagatedError(*zeeResult);
  txtfile << setw(5)  << "" << setw(5)  << "  ||  " << effGsf.getVal()/effGsf_Zee.getVal() << endl;  
  
  Double_t effTotData    = (Double_t)(nEleEle2HLT+nEleEle1HLT)/Nz.getVal();
  Double_t effTotDataErr = sqrt(effTotData*(1.-effTotData)/Nz.getVal());
  Double_t effTotZee     = (tmpltEleEle2HLT->Integral()+tmpltEleEle1HLT->Integral())/Nzee.getVal();
  Double_t effTotZeeErr  = sqrt(effTotZee*(1.-effTotZee)/Nzee.getVal());
  
  txtfile << "    eff(Z): " << setw(10) << effTotData << " +/- " << setw(10) << effTotDataErr;
  txtfile << setw(5)  << "" << setw(10) << effTotZee  << " +/- " << setw(10) << effTotZeeErr;
  txtfile << setw(5)  << "" << setw(5)  << "  ||  " << effTotData/effTotZee << endl; 
  txtfile << endl;  
    
  Double_t chi2prob, chi2ndf;
  Double_t ksprob, ksprobpe;  
   
  txtfile << "*" << endl;
  txtfile << "* EleEleNoSel" << endl;
  txtfile << "*==================================================" << endl;  
  txtfile << endl;
  chi2prob = hEleEleNoSel->Chi2Test(hPdfEleEleNoSel,"PUW");
  chi2ndf  = hEleEleNoSel->Chi2Test(hPdfEleEleNoSel,"CHI2/NDFUW");
  ksprob   = hEleEleNoSel->KolmogorovTest(hPdfEleEleNoSel);
  ksprobpe = hEleEleNoSel->KolmogorovTest(hPdfEleEleNoSel,"DX");
  printChi2AndKSResults(txtfile, chi2prob, chi2ndf, ksprob, ksprobpe);  
   
  txtfile << "*" << endl;
  txtfile << "* EleSC" << endl;
  txtfile << "*==================================================" << endl;  
  txtfile << endl;
  chi2prob = hEleSC->Chi2Test(hPdfEleSC,"PUW");
  chi2ndf  = hEleSC->Chi2Test(hPdfEleSC,"CHI2/NDFUW");
  ksprob   = hEleSC->KolmogorovTest(hPdfEleSC);
  ksprobpe = hEleSC->KolmogorovTest(hPdfEleSC,"DX");
  printChi2AndKSResults(txtfile, chi2prob, chi2ndf, ksprob, ksprobpe);  
  
  result->printStream(txtfile,RooPrintable::kValue,RooPrintable::kVerbose);
  txtfile << endl;
  
  printCorrelations(txtfile, result);
  txtfile << endl;
  
  txtfile.close();

  makeHTML(outputDir);

cout << "crapscale = " << crapscale << endl;
  
  cout << endl;
  cout << "  <> Output saved in " << outputDir << "/" << endl;    
  cout << endl;     
  
  gBenchmark->Show("fitZee");
}


//=== FUNCTION DEFINITIONS ======================================================================================

//--------------------------------------------------------------------------------------------------
TH1D *makeDiffHist(TH1D* hData, TH1D* hFit, const TString name)
{
  TH1D *hDiff = new TH1D(name,"",hData->GetNbinsX(),hData->GetXaxis()->GetXmin(),hData->GetXaxis()->GetXmax());
  for(Int_t ibin=1; ibin<=hData->GetNbinsX(); ibin++) {
    
    Double_t diff = (hData->GetBinContent(ibin)-hFit->GetBinContent(ibin));
    
    Double_t err = sqrt(hData->GetBinContent(ibin));
    if(err==0) err= sqrt(hFit->GetBinContent(ibin));
    
    if(err>0) hDiff->SetBinContent(ibin,diff/err);
    else      hDiff->SetBinContent(ibin,0);
    hDiff->SetBinError(ibin,1);   
  }
  
  hDiff->GetYaxis()->SetTitleOffset(0.48);
  hDiff->GetYaxis()->SetTitleSize(0.13);
  hDiff->GetYaxis()->SetLabelSize(0.10);
  hDiff->GetYaxis()->SetNdivisions(104);
  hDiff->GetYaxis()->CenterTitle();
  hDiff->GetXaxis()->SetTitleOffset(1.2);
  hDiff->GetXaxis()->SetTitleSize(0.13);
  hDiff->GetXaxis()->SetLabelSize(0.12);
  hDiff->GetXaxis()->CenterTitle();
  
  return hDiff;
}

//--------------------------------------------------------------------------------------------------
void printCorrelations(ostream& os, RooFitResult *res)
{
  ios_base::fmtflags flags = os.flags();
  const RooArgList parlist = res->floatParsFinal();
  
  os << "  Correlation Matrix" << endl;
  os << " --------------------" << endl;
  for(Int_t i=0; i<parlist.getSize(); i++) {
    for(Int_t j=0; j<parlist.getSize(); j++) 
      os << "  " << setw(7) << setprecision(4) << fixed << res->correlationMatrix()(i,j);    
    os << endl;
  }
  os.flags(flags);
}

//--------------------------------------------------------------------------------------------------
void printChi2AndKSResults(ostream& os, 
                           const Double_t chi2prob, const Double_t chi2ndf, 
                           const Double_t ksprob, const Double_t ksprobpe)
{
  ios_base::fmtflags flags = os.flags();
  
  os << "  Chi2 Test" << endl;
  os << " -----------" << endl;
  os << "       prob = " << chi2prob << endl;
  os << "   chi2/ndf = " << chi2ndf << endl;
  os << endl;
  os << "  KS Test" << endl;
  os << " ---------" << endl;
  os << "   prob = " << ksprob << endl;
  os << "   prob = " << ksprobpe << " with 1000 pseudo-experiments" << endl;
  os << endl;
 
  os.flags(flags);
}

//--------------------------------------------------------------------------------------------------
void makeHTML(const TString outDir)
{
  ofstream htmlfile;
  char htmlfname[100];
  sprintf(htmlfname,"%s/ZeeFitPlots.html",outDir.Data());
  htmlfile.open(htmlfname);
  htmlfile << "<!DOCTYPE html" << endl;
  htmlfile << "    PUBLIC \"-//W3C//DTD HTML 3.2//EN\">" << endl;
  htmlfile << "<html>" << endl;
  htmlfile << "<head><title>Zee</title></head>" << endl;
  htmlfile << "<body bgcolor=\"EEEEEE\">" << endl;
  
  htmlfile << "<table border=\"0\" cellspacing=\"5\" width=\"100%\">" << endl; 
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"zee.png\"><img src=\"zee.png\" alt=\"zee.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"zeelog.png\"><img src=\"zeelog.png\" alt=\"zeelog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "</table>" << endl;
  htmlfile << "<table border=\"0\" cellspacing=\"5\" width=\"100%\">" << endl; 
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"eleelel2hlt.png\"><img src=\"eleele2hlt.png\" alt=\"eleele2hlt.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"eleelel1hlt.png\"><img src=\"eleele1hlt.png\" alt=\"eleele1hlt.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"eleelelnosel.png\"><img src=\"eleelenosel.png\" alt=\"eleelenosel.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"elesc.png\"><img src=\"elesc.png\" alt=\"elesc.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"eleele2hlt_zee.png\"><img src=\"eleele2hlt_zee.png\" alt=\"eleele2hlt_zee.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"eleele1hlt_zee.png\"><img src=\"eleele1hlt_zee.png\" alt=\"eleele1hlt_zee.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"eleelenosel_zee.png\"><img src=\"eleelenosel_zee.png\" alt=\"eleelenosel_zee.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"elesc_zee.png\"><img src=\"elesc_zee.png\" alt=\"elesc_zee.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "</table>" << endl;
  htmlfile << "<hr />" << endl;
  
  htmlfile << "</body>" << endl;
  htmlfile << "</html>" << endl;
  htmlfile.close();
}


//--------------------------------------------------------------------------------------------------
RooAbsPdf* getModel(Int_t sigType, Int_t bkgType, const char *label, TH1D *histTemplate,
                    RooRealVar &m, RooFormulaVar &NfitSig, RooRealVar &NfitBkg)
{
  char name[100];
  
  sprintf(name,"sig%s",label);
  CSignalModel *sigModel=0;  
  if(sigType==eCount) {    
    sprintf(name,"dataHist_%s",label); RooDataHist *dataHist = new RooDataHist(name,name,RooArgSet(m),histTemplate);
    sprintf(name,"histPdf_%s",label);  RooHistPdf  *histPdf  = new RooHistPdf(name,name,m,*dataHist,0);
    sprintf(name,"sig%s",label);       return new RooExtendPdf(name,name,*histPdf,NfitSig); 
  } 
  else if(sigType==eBWxCB)   { sigModel = new CBreitWignerConvCrystalBall(name, m); } 
  else if(sigType==eMCxGaus) { sigModel = new CMCTemplateConvGaussian(name, m, histTemplate); } 
  else { 
    cout << "Not valid background model choice for " << label << endl;
    assert(0);
  }
  
  sprintf(name,"bkg%s",label);  
  CBackgroundModel *bkgModel=0;
  if     (bkgType==eNone)    { NfitBkg.setVal(0); NfitBkg.setConstant(kTRUE); } 
  else if(bkgType==eExp)     { bkgModel = new CExponential(name, m); } 
  else if(bkgType==eErfcExp) { bkgModel = new CErfExpo(name, m); ((CErfExpo*)bkgModel)->beta->setVal(0.05); ((CErfExpo*)bkgModel)->beta->setMax(0.20); } 
  else if(bkgType==eDblExp)  { bkgModel = new CDoubleExp(name, m); } 
  else if(bkgType==eLinExp)  { bkgModel = new CLinearExp(name, m); } 
  else if(bkgType==eQuadExp) { bkgModel = new CQuadraticExp(name, m); } 
  else {    
    cout << "Not valid signal model choice for EleSC!" << endl;
    assert(0);
  }
  
  sprintf(name,"pdf%s",label);
  return new RooAddPdf(name,name,
                       (bkgType==eNone) ? RooArgList(*(sigModel->model)) : RooArgList(*(sigModel->model),*(bkgModel->model)),
		       (bkgType==eNone) ? RooArgList(NfitSig) : RooArgList(NfitSig,NfitBkg));  
}
