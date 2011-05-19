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

  ReducedMETFitter(const edm::ParameterSet &iConfig);
  ~ReducedMETFitter() { }
  std::auto_ptr<RooFitResult> compute(const LorentzVector &lep1, float sigmaPt1,
				      const LorentzVector &lep2, float sigmaPt2,
				      const LorentzVectorCollection &jets,
				      const LorentzVector &met);


  
  std::pair<double, double> reducedMET() {
    return std::make_pair(redMET_, redMETErr_);
  }

  std::pair<double, double> reducedMET_long() {
    return std::make_pair(redMET_long_, redMETErr_long_);
  }

  std::pair<double, double> reducedMET_perp() {
    return std::make_pair(redMET_perp_, redMETErr_perp_);
  }

  std::pair<double, double> jetRecoil_perp() {
    return std::make_pair(jetRecoil_perp_, jetRecoilErr_perp_);
  }

  std::pair<double, double> jetRecoil_long() {
    return std::make_pair(jetRecoil_long_, jetRecoilErr_long_);
  }
  


  class JetVariables {
   public:
    JetVariables(double pt, double px, double py, double phi, double eta,
		 const JetResolution* jetPtResolParam, const JetResolution* jetPhiResolParam,
		 int index);
    ~JetVariables();

    RooFormulaVar& jetPx() {
      return *px_jet;
    }
    
    RooFormulaVar& jetPy() {
      return *py_jet;
    }
    
    RooFormulaVar& jetPxErr() {
      return *px_err_jet;
    }
    
    RooFormulaVar& jetPyErr() {
      return *py_err_jet;
    }

    RooAbsPdf& ptResoModelPdf() {
      return *bindPtResolModel;
    }

    RooAbsPdf& phiResoModelPdf() {
      return *bindPhiResolModel;
    }

    

   private:
    
    TF1 *ptresolModel;
    RooRealVar *ptresol;
    RooAbsPdf *bindPtResolModel;
    RooRealVar *avgpt_jet;
    RooFormulaVar *pt_jet;
    RooRealVar *ptErr_jet;

    TF1 *phiresolModel;    
    RooRealVar *phiresol;
    RooAbsPdf *bindPhiResolModel;
    RooRealVar *avgphi_jet;
    RooFormulaVar *phi_jet;

    // the X/y projected variables
    RooFormulaVar *px_jet;
    RooFormulaVar *py_jet;
    RooFormulaVar *px_err_jet;
    RooFormulaVar *py_err_jet;
  };


 private:

  JetResolution *stdJetPtResol_, *stdJetPhiResol_;
  RooRealVar *balance_long, *balance_perp;
  RooDataSet *dataset;

  double redMET_long_;
  double redMETErr_long_;

  double redMET_perp_;
  double redMETErr_perp_;

  double redMET_;
  double redMETErr_;
  
  double jetRecoil_perp_;
  double jetRecoil_long_;

  double jetRecoilErr_perp_;
  double jetRecoilErr_long_;



};


#endif
