
// Program to fit an histogram with a lognormal function

#include <iostream>
#include <sstream>

#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TChain.h"
#include "TF1.h"
#include "TKey.h"
#include "Riostream.h"
#include "TMath.h"
#include "TVirtualFitter.h"

#include <stdio.h>
#include <math.h>


void fitLogNormal () {


// Default fit with 4 parameters
TF1* logNorm = new TF1("logNorm", "[0]/x*TMath::Gaus(log(x), [1], [2])");



// fit of the lumi histogram

TF1* logNorm2 = new TF1("logNorm2", "[0]/([2]*sqrt(2*TMath::Pi())*x)*TMath::Gaus(log(x), [1], [2])");
// Approximative value of parameters
logNorm2->SetParameters(200000,8.5,0.02);

// Input Files  
TFile *inputFile2 = TFile::Open("/afs/cern.ch/user/h/hprager/private/CMSSW_4_4_2_patch8/src/StatTools/BayesianDijetFit/bin/lumi.root", "READ");

// Histogram
TH1F* h2 = (TH1F*) inputFile2->Get("h2;1");

//TF1* Fit = (TF1*) 
h2->Fit("logNorm2");



/*
// fit of the JES histogram

TF1* logNorm3 = new TF1("logNorm3", "[0]/x*TMath::Gaus(log(x), [1], [2])");

logNorm3->SetParameters(40,-0.0002,0.02);

TFile *inputFile3 = TFile::Open("/afs/cern.ch/user/h/hprager/private/CMSSW_4_4_2_patch8/src/StatTools/BayesianDijetFit/bin/sigMassDelta.root", "READ");

TH1F* h3 = (TH1F*) inputFile3->Get("h3;1");

h3->Fit("logNorm3");




// fit of the JER histogram

TF1* logNorm4 = new TF1("logNorm4", "[0]/x*TMath::Gaus(log(x), [1], [2])");

logNorm4->SetParameters(17,-0.008,0.11);

TFile *inputFile4 = TFile::Open("/afs/cern.ch/user/h/hprager/private/CMSSW_4_4_2_patch8/src/StatTools/BayesianDijetFit/bin/sigWidthDelta.root", "READ");

TH1F* h4 = (TH1F*) inputFile4->Get("h4;1");

h4->Fit("logNorm4");
*/

}

