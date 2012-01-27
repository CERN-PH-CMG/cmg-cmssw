#include "Analysis/Statistics/interface/JPMCCalculator.h"

#include "RooStats/ModelConfig.h"
#include "RooStats/RooStatsUtils.h"

#include "RooGenericPdf.h"
#include "RooProdPdf.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooAbsFunc.h"

#include "Math/Functor.h"
#include "Math/BrentMinimizer1D.h"
#include "RooBrentRootFinder.h"
#include "RooFunctor.h"
#include "RooFunctor1DBinding.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////

JPMCCalculator::JPMCCalculator() :
  fData(0),
  fPdf(0),
  fPriorPOI(0),
  fNumIterations(100),
  fPosterior(0),
  fPosteriorHist(0),
  fPosteriorPdf(0),
  fLower(0), fUpper(0),
  fSize(0.05), fLeftSideFraction(0.5), 
  fBrfPrecision(0.00005), 
  fValidInterval(false)
{
   // default constructor
}

JPMCCalculator::JPMCCalculator( /* const char* name,  const char* title, */						   
						    RooAbsData& data,
                                                    RooAbsPdf& pdf,
						    const RooArgSet& POI,
						    RooAbsPdf& priorPOI,
						    const RooArgSet* nuisanceParameters ) :
   //TNamed( TString(name), TString(title) ),
  fData(&data),
  fPdf(&pdf),
  fPOI(POI),
  fPriorPOI(&priorPOI),
  fNumIterations(100),
  fPosterior(0),
  fPosteriorHist(0),
  fPosteriorPdf(0),
  fLower(0), fUpper(0), 
  fSize(0.05), fLeftSideFraction(0.5), 
  fBrfPrecision(0.00005), 
  fValidInterval(false)
{
   // constructor
   if (nuisanceParameters) fNuisanceParameters.add(*nuisanceParameters); 
}

JPMCCalculator::JPMCCalculator( RooAbsData& data,
				RooStats::ModelConfig & model) : 
  fData(&data), 
  fPdf(model.GetPdf()),
  fPriorPOI( model.GetPriorPdf()),
  fNumIterations(100),
  fPosterior(0),
  fPosteriorHist(0),
  fPosteriorPdf(0),
  fLower(0), fUpper(0), 
  fSize(0.05), fLeftSideFraction(0.5), 
  fBrfPrecision(0.00005), 
  fValidInterval(false)
{
  // constructor from Model Config
  SetModel(model);
}

JPMCCalculator::~JPMCCalculator()
{
  // destructor
  ClearAll(); 
}

void JPMCCalculator::ClearAll() const { 
  // clear cached pdf objects
  if (fPosterior) delete fPosterior;
  if (fPosteriorHist) delete fPosteriorHist;
  if (fPosteriorPdf) delete fPosteriorPdf;
  fLower = 0;
  fUpper = 0;
  fValidInterval = false;
}

void JPMCCalculator::SetModel(const RooStats::ModelConfig & model) {
  // set the model
  fPdf = model.GetPdf();
  fPriorPOI =  model.GetPriorPdf(); 
  // assignment operator = does not do a real copy the sets (must use add method) 
  fPOI.removeAll();
  fNuisanceParameters.removeAll();
  if(model.GetParametersOfInterest()) fPOI.add( *(model.GetParametersOfInterest()) );
  if(model.GetNuisanceParameters())  fNuisanceParameters.add( *(model.GetNuisanceParameters() ) );
  
  // invalidate the cached pointers
  ClearAll(); 
}

TH1D* JPMCCalculator::GetPosteriorHistTestUpper(double poiVal, int nbins) const
{
  return GetPosteriorHistInternal(1, poiVal, nbins, false);
}

TH1D* JPMCCalculator::GetPosteriorHist(int nbins) const
{
  return GetPosteriorHistInternal(0, 0.0, nbins, false);
}

TH1D* JPMCCalculator::GetPosteriorHistForce(int nbins) const
{
  return GetPosteriorHistInternal(0, 0.0, nbins, true);
}

TH1D* JPMCCalculator::GetPosteriorHistInternal(int testtype, double poiVal, int nbins, bool force) const
{
  if(!force && fPosteriorHist) return fPosteriorHist;

  // run some sanity checks
  if(!fPdf ) {
    coutE(InputArguments) << "JPMCCalculator::GetPosterior - missing pdf model" << std::endl;
    return 0;
  }
  if(!fPriorPOI) { 
    coutE(InputArguments) << "JPMCCalculator::GetPosterior - missing prior pdf" << std::endl;
    return 0;
  }
  if(fPOI.getSize() == 0) {
    coutE(InputArguments) << "JPMCCalculator::GetPosterior - missing parameter of interest" << std::endl;
    return 0;
  }
  if(fPOI.getSize() > 1) { 
    coutE(InputArguments) << "JPMCCalculator::GetPosterior - current implementation works only on 1D intervals" << std::endl;
    return 0; 
  }
  
  // take the product of the model with the POI prior
  TString prodName = TString("product_") + TString(fPdf->GetName()) + TString("_") + TString(fPriorPOI->GetName() );
  RooAbsPdf* prodPdf = new RooProdPdf(prodName,"",RooArgList(*fPdf,*fPriorPOI));
  if(prodPdf==0) {
    coutE(InputArguments) << "JPMCCalculator::GetPosterior - failed to take the product of the model with the POI prior." << std::endl;
    return 0;
  }

  // generate N=fNumIterations samples of the nuisance parameters
  RooDataSet* genParams = fPriorPOI->generate(fNuisanceParameters, fNumIterations);
  if(genParams==0) {
    coutE(InputArguments) << "JPMCCalculator::GetPosterior - failed to generate nuisance parameters" << std::endl;
    return 0;
  }

  // if there are 0 nuisance parameters, don't iterate, but do it only once
  int actualNumIterations=fNumIterations;
  if(fNuisanceParameters.getSize()==0) actualNumIterations=1;

  // intialize the posterior
  fPosterior=0;

  // get the poi
  RooRealVar* poi = dynamic_cast<RooRealVar*>( fPOI.first() ); 
  assert(poi);

  // if we're doing a test, create a range
  if(testtype==1) poi->setRange("range",poiVal, poi->getMax());

  // create the histogram
  TString histName = TString("posthist_")+TString(fPdf->GetName()) + TString("_")+this->GetName();
  fPosteriorHist=new TH1D(histName, "", nbins, poi->getMin(), poi->getMax());

  // loop over all of the iterations
  for(int iter=0; iter<actualNumIterations; ++iter) {

    // get the set of generated parameters and set the nuisance parameters to the generated values
    if(fNuisanceParameters.getSize()>0) {
      const RooArgSet* genset=genParams->get(iter);
      TIterator* it=genset->createIterator();
      RooAbsReal* arg;
      while((arg=(RooAbsReal*)it->Next())) {
	//	std::cout << arg->GetName() << " " << arg->getVal() << std::endl;
	fNuisanceParameters.setRealValue(arg->GetName(), arg->getVal());
      }
    }

    // turn the pdf into an NLL
    RooAbsReal* theNLL = prodPdf->createNLL(*fData);
    if(theNLL==0) {
      coutE(InputArguments) << "JPMCCalculator::GetPosterior - failed to compute NLL" << std::endl;
      return 0;
    }
    if(theNLL->getVal() > std::numeric_limits<double>::max()) {
      coutE(Eval) <<  "JPMCCalculator::GetPosteriorFunction : " 
		  << " Negative log likelihood evaluates to infinity " << std::endl;
      return 0;
    }

    // need do find minimum of log-likelihood in the range to shift function 
    // to avoid numerical errors when we compute the likelihood (overflows in the exponent)
    // N.B.: this works for only 1 parameter of interest otherwise Minuit should be used for finding the minimum
    double nllMin=0;
    RooFunctor *nllFunc = theNLL->functor(fPOI);
    ROOT::Math::Functor1D wnllFunc(*nllFunc);
    ROOT::Math::BrentMinimizer1D minim; 
    minim.SetFunction(wnllFunc,poi->getMin(),poi->getMax() );
    bool ret  = minim.Minimize(100,1.E-3,1.E-3);
    if (ret) nllMin = minim.FValMinimum();
    delete nllFunc;

    // compute the unnormalized posterior
    TString formula; 
    formula.Form("exp(-@0+%f)",nllMin);
    RooFormulaVar *unnormPost=new RooFormulaVar(TString("unnormPost")+this->GetName(),formula,RooArgList(*theNLL));

    // normalize the posterior
    RooAbsReal* theIntegral = unnormPost->createIntegral(RooArgSet(*poi));
    double integralVal = theIntegral->getVal();
    formula.Form("@0/%f",integralVal);
    RooFormulaVar *normPost=new RooFormulaVar(TString("normPost")+TString(iter)+this->GetName(),formula,RooArgList(*unnormPost));
    delete theIntegral;

    // do some tests on the posterior
    if(testtype==1) {
      RooAbsReal* unnormPostInt=unnormPost->createIntegral(RooArgSet(*poi), "range");
      double unnormPostIntVal = unnormPostInt->getVal(RooArgSet(*poi));
      TIterator* it=fNuisanceParameters.createIterator();
      RooAbsReal* arg;
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "normalized integral of posterior in [" << poiVal << ", " << poi->getMax() << "] = " << unnormPostIntVal/integralVal << std::endl;
      while((arg=(RooAbsReal*)it->Next())) {
	std::cout << arg->GetName() << " " << arg->getVal() << std::endl;
      }
      std::cout << "------------------------------------------------" << std::endl;
      delete unnormPostInt;
    }



    // add the posterior
    for(int bin=1; bin<=fPosteriorHist->GetNbinsX(); ++bin) {
      double center=fPosteriorHist->GetBinCenter(bin);
      poi->setVal(center);
      RooArgSet poiset(*poi);
      double eval=normPost->getVal(&poiset);
      fPosteriorHist->AddBinContent(bin, eval);
    }

    delete normPost;
    delete unnormPost;
    delete theNLL;
  }

  delete prodPdf;
  delete genParams;
  
  return fPosteriorHist;
}

RooAbsReal* JPMCCalculator::GetPosterior(void) const
{
  /// build and return the posterior pdf (i.e posterior function normalized to all range of poi
  /// NOTE: user must delete the returned object 
  // the posterior is obtained from the product of the likelihood function and the

  if(fPosterior) return fPosterior;
  
  // run some sanity checks
  if(!fPdf ) {
    coutE(InputArguments) << "JPMCCalculator::GetPosterior - missing pdf model" << std::endl;
    return 0;
  }
  if(!fPriorPOI) { 
    coutE(InputArguments) << "JPMCCalculator::GetPosterior - missing prior pdf" << std::endl;
    return 0;
  }
  if(fPOI.getSize() == 0) {
    coutE(InputArguments) << "JPMCCalculator::GetPosterior - missing parameter of interest" << std::endl;
    return 0;
  }
  if(fPOI.getSize() > 1) { 
    coutE(InputArguments) << "JPMCCalculator::GetPosterior - current implementation works only on 1D intervals" << std::endl;
    return 0; 
  }
  
  // take the product of the model with the POI prior
  TString prodName = TString("product_") + TString(fPdf->GetName()) + TString("_") + TString(fPriorPOI->GetName() );
  RooAbsPdf* prodPdf = new RooProdPdf(prodName,"",RooArgList(*fPdf,*fPriorPOI));
  if(prodPdf==0) {
    coutE(InputArguments) << "JPMCCalculator::GetPosterior - failed to take the product of the model with the POI prior." << std::endl;
    return 0;
  }

  // generate N=fNumIterations samples of the nuisance parameters
  RooDataSet* genParams = fPdf->generate(fNuisanceParameters, fNumIterations);
  if(genParams==0) {
    coutE(InputArguments) << "JPMCCalculator::GetPosterior - failed to generate nuisance parameters" << std::endl;
    return 0;
  }

  // if there are 0 nuisance parameters, don't iterate, but do it only once
  int actualNumIterations=fNumIterations;
  if(fNuisanceParameters.getSize()==0) actualNumIterations=1;

  // intialize the posterior
  fPosterior=0;

  // loop over all of the iterations
  for(int iter=0; iter<actualNumIterations; ++iter) {

    // get the set of generated parameters and set the nuisance parameters to the generated values
    if(fNuisanceParameters.getSize()>0) {
      const RooArgSet* genset=genParams->get(iter);
      TIterator* it=genset->createIterator();
      RooAbsReal* arg;
      while((arg=(RooAbsReal*)it->Next())) {
	std::cout << arg->GetName() << " " << arg->getVal() << std::endl;
	fNuisanceParameters.setRealValue(arg->GetName(), arg->getVal());
      }
    }

    // turn the pdf into an NLL
    RooAbsReal* theNLL = prodPdf->createNLL(*fData);
    if(theNLL==0) {
      coutE(InputArguments) << "JPMCCalculator::GetPosterior - failed to compute NLL" << std::endl;
      return 0;
    }
    if(theNLL->getVal() > std::numeric_limits<double>::max()) {
      coutE(Eval) <<  "JPMCCalculator::GetPosteriorFunction : " 
		  << " Negative log likelihood evaluates to infinity " << std::endl;
      return 0;
    }

    // need do find minimum of log-likelihood in the range to shift function 
    // to avoid numerical errors when we compute the likelihood (overflows in the exponent)
    // N.B.: this works for only 1 parameter of interest otherwise Minuit should be used for finding the minimum
    double nllMin=0;
    RooFunctor *nllFunc = theNLL->functor(fPOI);
    ROOT::Math::Functor1D wnllFunc(*nllFunc);
    RooRealVar* poi = dynamic_cast<RooRealVar*>( fPOI.first() ); 
    assert(poi);
    ROOT::Math::BrentMinimizer1D minim; 
    minim.SetFunction(wnllFunc,poi->getMin(),poi->getMax() );
    bool ret  = minim.Minimize(100,1.E-3,1.E-3);
    if (ret) nllMin = minim.FValMinimum();
    delete nllFunc;

    // compute the unnormalized posterior
    TString formula; 
    formula.Form("exp(-@0+%f)",nllMin);
    RooFormulaVar *unnormPost=new RooFormulaVar(TString("unnormPost")+this->GetName(),formula,RooArgList(*theNLL));

    // normalize the posterior
    RooAbsReal* theIntegral = unnormPost->createIntegral(RooArgSet(*poi));
    double integralVal = theIntegral->getVal();
    delete theIntegral;
    formula.Form("@0/%f",integralVal);
    RooFormulaVar *normPost=new RooFormulaVar(TString("normPost")+TString(iter)+this->GetName(),formula,RooArgList(*unnormPost));
    //    delete unnormPost;
    //    RooFormulaVar *normPost=unnormPost;

    // sum the posteriors together
    if(fPosterior==0) {
      fPosterior=normPost;
    } else {
      RooFormulaVar *sum=new RooFormulaVar("sum","@0+@1",RooArgList(*fPosterior,*normPost));
      //      delete fPosterior;
      //      delete normPost;
      fPosterior=sum;
    }
    
    //    delete theNLL;
  }

  delete prodPdf;
  delete genParams;

  return fPosterior;
}

  RooAbsPdf* JPMCCalculator::GetPosteriorPdf(void) const
{
  if(fPosteriorPdf) return fPosteriorPdf;
  if(!fPosterior) GetPosterior();
  if(!fPosterior) return 0;
  
  // create a unique name on the posterior from the names of the components
  TString posteriorName = this->GetName() + TString("_posteriorPdf_") + fPosterior->GetName(); 
  fPosteriorPdf = new RooGenericPdf(posteriorName,"@0",*fPosterior);
  
  return fPosteriorPdf;
}

RooPlot* JPMCCalculator::GetPosteriorPlot(double precision)
{
  /// return a RooPlot with the posterior and the credibility region
  RooAbsReal* post = GetPosterior();
  if(!post) return 0;

  //  if(!fValidInterval) GetInterval();

  RooAbsRealLValue* poi = dynamic_cast<RooAbsRealLValue*>( fPOI.first() );
  assert(poi);

  RooPlot* plot = poi->frame();

  // try to reduce some error messages
  post->setEvalErrorLoggingMode(RooAbsReal::CountErrors);

  plot->SetTitle(TString("Posterior probability of parameter \"")+TString(poi->GetName())+TString("\""));

  post->plotOn(plot,RooFit::Range(fLower,fUpper,kFALSE),RooFit::VLines(),RooFit::DrawOption("F"),RooFit::MoveToBack(),RooFit::FillColor(kGray),RooFit::Precision(precision));
  //  post->plotOn(plot);

  plot->GetYaxis()->SetTitle("posterior function");
   
  return plot;
}

RooStats::SimpleInterval* JPMCCalculator::GetInterval(void) const
{
  /// returns a SimpleInterval with lower and upper bounds on the
  /// parameter of interest specified in the constructor. 
  /// Using the method (to be called before SetInterval) SetLeftSideTailFraction the user can choose the type of interval.
  /// By default the returned interval is a central interval with the confidence level specified 
  /// previously in the constructor ( LeftSideTailFraction = 0.5). 
  ///  For lower limit use SetLeftSideTailFraction = 1
  ///  For upper limit use SetLeftSideTailFraction = 0
  ///  for shortest intervals use SetLeftSideTailFraction = -1 or call the method SetShortestInterval()
  /// NOTE: The BayesianCaluclator covers only the case with one
  /// single parameter of interest

  if(fValidInterval) 
    coutW(Eval) << "JPMCCalculator::GetInterval - recomputing interval for the same CL and same model" << std::endl;

  RooRealVar* poi = dynamic_cast<RooRealVar*>( fPOI.first() );
  if (!poi) { 
    coutE(Eval) << "JPMCCalculator::GetInterval - no parameter of interest is set " << std::endl;
    return 0; 
  } 


  // compute the other intervals

  double lowerCutOff = fLeftSideFraction * fSize; 
  double upperCutOff = 1. - (1.- fLeftSideFraction) * fSize; 

  ComputeIntervalFromHist(lowerCutOff, upperCutOff);      

  if (!fValidInterval) { 
    fLower = 1; fUpper = 0;
    coutE(Eval) << "JPMCCalculator::GetInterval - cannot compute a valid interval - return a dummy [1,0] interval"
		<<  std::endl;
  }
  else {
    coutI(Eval) << "JPMCCalculator::GetInterval - found a valid interval : [" << fLower << " , " 
		<< fUpper << " ]" << std::endl;
  }
  
  TString interval_name = TString("BayesianInterval_a") + TString(this->GetName());
  RooStats::SimpleInterval * interval = new RooStats::SimpleInterval(interval_name,*poi,fLower,fUpper,ConfidenceLevel());
  interval->SetTitle("SimpleInterval from JPMCCalculator");
  
  return interval;
}

void JPMCCalculator::ComputeIntervalFromHist(double lowerCutOff, double upperCutOff) const
{
  // compute the interval using RooFit
  coutI(Eval) <<  "JPMCCalculator: Compute interval using RooFit:  posteriorPdf + createCdf + RooBrentRootFinder " << std::endl;

  if(!fPosteriorHist) GetPosteriorHist();
  if(!fPosteriorHist) return;

  TH1D* hCdf = (TH1D*)fPosteriorHist->Clone(fPosteriorHist->GetName()+TString("_cdf"));
  for(int bin=1; bin<=hCdf->GetNbinsX()+1; ++bin) {
    double last=hCdf->GetBinContent(bin-1);
    double next=fPosteriorHist->GetBinContent(bin);
    hCdf->SetBinContent(bin, last+next);
  }
  hCdf->Scale(1./fPosteriorHist->GetSumOfWeights());

  for(int bin=1; bin<=hCdf->GetNbinsX(); bin++) {
    if(lowerCutOff<=hCdf->GetBinContent(bin)) {
      fLower=hCdf->GetBinLowEdge(bin);
      break;
    }
  }

  for(int bin=hCdf->GetNbinsX(); bin>=1; --bin) {
    if(upperCutOff>=hCdf->GetBinContent(bin)) {
      fUpper=hCdf->GetBinLowEdge(bin+1);
      break;
    }
  }
  delete hCdf;
  fValidInterval = true;
}


void JPMCCalculator::ComputeIntervalFromPdf(double lowerCutOff, double upperCutOff) const
{
  // compute the interval using RooFit
  coutI(Eval) <<  "JPMCCalculator: Compute interval using RooFit:  posteriorPdf + createCdf + RooBrentRootFinder " << std::endl;

  RooRealVar* poi = dynamic_cast<RooRealVar*>( fPOI.first() ); 
  assert(poi);

  if(!fPosteriorPdf) GetPosteriorPdf();
  if(!fPosteriorPdf) return;

  RooAbsReal* cdf = fPosteriorPdf->createCdf(fPOI,RooFit::ScanNoCdf());
  RooAbsFunc* cdf_bind = cdf->bindVars(fPOI,&fPOI);
  RooBrentRootFinder brf(*cdf_bind);
  brf.setTol(fBrfPrecision); // set the brf precision
   
  double tmpVal = poi->getVal();  // patch used because findRoot changes the value of poi
   
  bool ret = true; 
  if (lowerCutOff > 0) { 
    double y = lowerCutOff;
    ret &= brf.findRoot(fLower,poi->getMin(),poi->getMax(),y);
  } 
  else 
    fLower = poi->getMin(); 
  
  if (upperCutOff < 1.0) { 
    double y=upperCutOff;
    ret &= brf.findRoot(fUpper,poi->getMin(),poi->getMax(),y);
  }
  else 
    fUpper = poi->getMax();
  if (!ret) coutE(Eval) << "JPMCCalculator::GetInterval "
			<< "Error returned from Root finder, estimated interval is not fully correct" 
			<< std::endl;
  
  poi->setVal(tmpVal); // patch: restore the original value of poi
   
  delete cdf_bind;
  delete cdf;
  fValidInterval = true;
}


ClassImp(JPMCCalculator)
