#ifndef reducedmetfitter_hh
#define reducedmetfitter_hh

#if !defined(__CINT__) || defined(__MAKECINT__)

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TSystem.h"

#include "RooGaussian.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooProdPdf.h"
#include "RooDataHist.h"
#include "RooAddition.h"
#include "RooPlot.h"
#include "RooMinuit.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooTFnPdfBinding.h"

#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

#endif

//
class ReducedMETFitter
{
 public:

  ReducedMETFitter(const edm::ParameterSet &iConfig, size_t maxJets=10);
  ~ReducedMETFitter() { }
  void compute(const LorentzVector &lep1, float sigmaPt1,
	       const LorentzVector &lep2, float sigmaPt2,
	       const LorentzVectorCollection &jets,
	       const LorentzVector &met);
 private:

  JetResolution *stdJetPtResol_, *stdJetPhiResol_;
  RooRealVar *balance_long, *balance_perp;
  RooDataSet *dataset;
};


#endif
