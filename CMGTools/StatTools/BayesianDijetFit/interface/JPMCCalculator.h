#ifndef ROOSTATS_JPMCCalculator
#define ROOSTATS_JPMCCalculator

#include "TNamed.h"

#include "Math/IFunctionfwd.h"

#include "TH1F.h"

#include "RooArgSet.h"

#include "RooStats/IntervalCalculator.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/SimpleInterval.h"

class RooAbsData;
class RooAbsPdf;
class RooPlot;
class RooAbsReal;

class JPMCCalculator : public RooStats::IntervalCalculator, public TNamed {
  
 public:
  
  // constructor
  JPMCCalculator( );
  
  JPMCCalculator( RooAbsData& data,
		  RooAbsPdf& pdf,
		  const RooArgSet& POI,
		  RooAbsPdf& priorPOI,
		  const RooArgSet* nuisanceParameters = 0 );
  
  JPMCCalculator( RooAbsData& data,
		  RooStats::ModelConfig& model );
  
  // destructor
  virtual ~JPMCCalculator();
  
  TH1D* GetPosteriorHist(int nbins=1000) const;
  TH1D* GetPosteriorHistForce(int nbins=1000) const;
  TH1D* GetPosteriorHistTestUpper(double poiVal, int nbins=1000) const;
  
  RooAbsReal* GetPosterior(void) const;
  RooAbsPdf* GetPosteriorPdf(void) const;
  
  RooPlot* GetPosteriorPlot(double precision=0.01);
  
  // compute the interval. By Default a central interval is computed 
  // By using SetLeftTileFraction can control if central/ upper/lower interval
  // For shortest interval use SetShortestInterval(true)
  virtual RooStats::SimpleInterval* GetInterval(void) const; 
  
  virtual void SetData( RooAbsData & data ) {
    fData = &data;
    ClearAll();
  }
  
  
  // set the model via the ModelConfig
  virtual void SetModel( const RooStats::ModelConfig& model ); 
  
  // set the size of the test (rate of Type I error) ( Eg. 0.05 for a 95% Confidence Interval)
  virtual void SetTestSize( Double_t size ) {
    fSize = size;
    fValidInterval = false; 
  }
  // set the confidence level for the interval (eg. 0.95 for a 95% Confidence Interval)
  virtual void SetConfidenceLevel( Double_t cl ) { SetTestSize(1.-cl); }
  // Get the size of the test (eg. rate of Type I error)
  virtual Double_t Size() const { return fSize; }
  // Get the Confidence level for the test
  virtual Double_t ConfidenceLevel() const { return 1.-fSize; }
  // Set the number of iterations to sample the integral
  virtual void SetNumIterations( Int_t n ) { fNumIterations = n; }
  
  // set the fraction of probability content on the left tail
  // Central limits use 0.5 (default case)  
  // for upper limits it is 0 and 1 for lower limit
  // For shortest intervals a negative value (i.e. -1) must be given
  void SetLeftSideTailFraction(Double_t leftSideFraction )  {fLeftSideFraction = leftSideFraction;} 
  
  // set the precision of the Root Finder 
  void SetBrfPrecision( double precision ) { fBrfPrecision = precision; }
  
 protected:
  
  void ClearAll() const; 
  
  void ComputeIntervalFromHist(double c1, double c2) const;
  void ComputeIntervalFromPdf(double c1, double c2) const;
  
  TH1D* GetPosteriorHistInternal(int testtype, double poiVal, int nbins, bool force) const;
    
 private:
    
    // plan to replace the above: return a SimpleInterval integrating 
    // over all other parameters except the one specified as argument
    //virtual SimpleInterval* GetInterval( RooRealVar* parameter  ) const { return 0; }
    
    RooAbsData* fData;
    RooAbsPdf* fPdf;
    RooArgSet fPOI;
    RooAbsPdf* fPriorPOI;
    mutable RooArgSet fNuisanceParameters;
    
    int fNumIterations;
    
    mutable RooAbsReal* fPosterior;
    mutable TH1D* fPosteriorHist;
    mutable RooAbsPdf* fPosteriorPdf;


    ///    mutable RooAbsPdf* fProductPdf;                         // internal pointer to model * prior
    //    mutable std::vector<RooAbsReal*> fLogLike;              // internal pointer to log likelihood functions
    //    mutable std::vector<RooAbsReal*> fLikelihood;           // internal pointer to likelihood functions
    //    mutable std::vector<Double_t> fNLLMin;                  // minimum value of Nll
    //    mutable std::vector<TH1F*> fPosterior;                  // posterior as a histogram

    //    mutable TH1F* fAvgPosterior;

    mutable Double_t  fLower;    // computer lower interval bound
    mutable Double_t  fUpper;    // upper interval bound

    double fSize;  // size used for getting the interval
    double fLeftSideFraction;    // fraction of probability content on left side of interval
    double fBrfPrecision;     // root finder precision

    mutable Bool_t    fValidInterval; 
    
    
    
    TString fIntegrationType; 
    
    ClassDef(JPMCCalculator, 1)

  };

#endif
