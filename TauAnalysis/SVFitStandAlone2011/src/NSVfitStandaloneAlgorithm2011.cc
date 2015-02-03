#include "TauAnalysis/SVFitStandAlone2011/interface/NSVfitStandaloneAlgorithm2011.h"
#include "TauAnalysis/SVFitStandAlone2011/interface/svFitAuxFunctions.h"

#include "Math/Factory.h"
#include "Math/Functor.h"

///Jose: made TMatrixD a const pointer, it was being modified inside !
NSVfitStandaloneAlgorithm2011::NSVfitStandaloneAlgorithm2011(std::vector<NSVfitStandalone2011::MeasuredTauLepton2011> measuredTauLeptons, NSVfitStandalone2011::Vector measuredMET ,const TMatrixD * covMET, unsigned int verbosity) :
  fitStatus_(-1), verbosity_(verbosity), maxObjFunctionCalls_(5000)
{ 
  // instantiate minuit, the arguments might turn into configurables once
  minimizer_ = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
  // instantiate the combined likelihood
  nll_ = new NSVfitStandalone2011::NSVfitStandaloneLikelihood2011(measuredTauLeptons, measuredMET,covMET, (verbosity_>1));
}

NSVfitStandaloneAlgorithm2011::~NSVfitStandaloneAlgorithm2011() 
{
  delete nll_;
  delete minimizer_;
}

void
NSVfitStandaloneAlgorithm2011::setup()
{
  using namespace NSVfitStandalone2011;

  if( verbosity_>0 ){
    std::cout << "<NSVfitStandaloneAlgorithm2011::setup()>" << std::endl;
  }
  for(unsigned int idx=0; idx<nll_->measuredTauLeptons().size(); ++idx){
    if(verbosity_>0){
      std::cout << " >> upper limit of leg1::mNuNu will be set to "; 
      if( nll_->measuredTauLeptons()[idx].decayType() == kHadDecay) std::cout << 0; 
      else std::cout << SVfit_namespace::tauLeptonMass-TMath::Min(nll_->measuredTauLeptons()[idx].mass(), 1.5); 
      std::cout << std::endl;
    }
    // setup the first decay branch
    minimizer_->SetLimitedVariable(idx*kMaxFitParams+kXFrac, std::string(TString::Format("leg%d::xFrac", idx+1)).c_str(), 0.5, 0.10, 0., 1.);
    if( nll_->measuredTauLeptons()[idx].decayType() == kHadDecay ){ 
      minimizer_->SetFixedVariable(idx*kMaxFitParams+kMNuNu, std::string(TString::Format("leg%d::mNuNu", idx+1)).c_str(), 0.); 
    }
    else{ 
      minimizer_->SetLimitedVariable(idx*kMaxFitParams+kMNuNu, std::string(TString::Format("leg%d::mNuNu", idx+1)).c_str(), 0.8, 0.10, 0., SVfit_namespace::tauLeptonMass-TMath::Min(nll_->measuredTauLeptons()[idx].mass(), 1.5)); 
    }
    minimizer_->SetVariable(idx*kMaxFitParams+kPhi, std::string(TString::Format("leg%d::phi", idx+1)).c_str(),  0.0,  0.25);
  }
}

void
NSVfitStandaloneAlgorithm2011::fit(){
  if( verbosity_>0 ){
    std::cout << "<NSVfitStandaloneAlgorithm2011::fit()>" << std::endl
	      << ">> dimension of fit    : " << nll_->measuredTauLeptons().size()*NSVfitStandalone2011::kMaxFitParams << std::endl
	      << ">> maxObjFunctionCalls : " << maxObjFunctionCalls_ << std::endl; 
  }

  // clear minimizer
  minimizer_->Clear();
  // set verbosity level of minimizer
  minimizer_->SetPrintLevel(verbosity_);
  // setup the function to be called and the dimension of the fit
  ROOT::Math::Functor toMinimize(standaloneObjectiveFunctionAdapter_, nll_->measuredTauLeptons().size()*NSVfitStandalone2011::kMaxFitParams);
  minimizer_->SetFunction(toMinimize);
  setup();
  minimizer_->SetMaxFunctionCalls(maxObjFunctionCalls_);
  // set Minuit strategy = 2, in order to get reliable error estimates:
  // http://www-cdf.fnal.gov/physics/statistics/recommendations/minuit.html
  minimizer_->SetStrategy(2);
  // compute uncertainties for increase of objective function by 0.5 wrt. 
  // minimum (objective function is log-likelihood function)
  minimizer_->SetErrorDef(0.5);
  
  //Jose: increase tolerance to avoid error messages
  minimizer_->SetTolerance(1.);
  
  if( verbosity_>0 ){
    std::cout << ">> starting ROOT::Math::Minimizer::Minimize..." << std::endl;
    std::cout << ">> #freeParameters = " << minimizer_->NFree() << ","
  	      << ">> #constrainedParameters = " << (minimizer_->NDim() - minimizer_->NFree()) << std::endl;
  }
  // do the minimization
  minimizer_->Minimize();

  // evaluate Minuit status code:
  //
  //  0: error matrix not calculated at all
  //  1: diagonal approximation only, not accurate
  //  2: full matrix, but forced positive-definite
  //  3: full accurate covariance matrix 
  //    (after MIGRAD, this is the indication of normal convergence.)
  //
  // NOTE: meaning of error codes taken from http://lmu.web.psi.ch/facilities/software/minuit_doc.html
  //

  /////Jose: The above is not right, 0 is good status look here: http://root.cern.ch/lxr/source/math/minuit2/src/Minuit2Minimizer.cxx

  fitStatus_ = minimizer_->Status();  
  finalEDM_ = minimizer_->Edm();
  if( verbosity_>0 ){ std::cout << ">> fitStatus = " << fitStatus_ << " , edm = "<<finalEDM_ << std::endl; }

  if( verbosity_>1 ){ minimizer_->PrintResults(); };  
  // and write out the result
  using NSVfitStandalone2011::kXFrac;
  using NSVfitStandalone2011::kMNuNu;
  using NSVfitStandalone2011::kPhi;
  using NSVfitStandalone2011::kMaxFitParams;
  // update di-tau system with final fit results
  nll_->results(fittedTauLeptons_, minimizer_->X());
  // determine uncertainty of the fitted di-tau mass
  massUncert_ = TMath::Sqrt( 0.25*minimizer_->Errors()[kXFrac]/minimizer_->X()[kXFrac]*minimizer_->Errors()[kXFrac]/minimizer_->X()[kXFrac]+0.25*minimizer_->Errors()[kMaxFitParams+kXFrac]/minimizer_->X()[kMaxFitParams+kXFrac]*minimizer_->Errors()[kMaxFitParams+kXFrac]/minimizer_->X()[kMaxFitParams+kXFrac])*fittedDiTauSystem().mass();
  
  if(verbosity_>1){
    std::cout << ">> Resonance : " << std::endl;
    std::cout << ">> p4: pt = " << fittedDiTauSystem().pt() << " eta = " << fittedDiTauSystem().eta() << " phi = " << fittedDiTauSystem().phi() << std::endl;
    std::cout << ">> mass          = " << fittedDiTauSystem().mass() << std::endl;  
    std::cout << ">> massUncert    = " << massUncert_ << std::endl
	      << "   error[xFrac1 ]=" << minimizer_->Errors()[kXFrac] << std::endl
	      << "   value[xFrac1 ]=" << minimizer_->X()[kXFrac] << std::endl
	      << "   error[xFrac2 ]=" << minimizer_->Errors()[kMaxFitParams+kXFrac] << std::endl
	      << "   value[xFrac2 ]=" << minimizer_->X()[kMaxFitParams+kXFrac] << std::endl;
    std::cout << ">> Leg1 : " << std::endl;
    std::cout << ">> p4: pt = " << nll_->measuredTauLeptons()[0].p4().pt() << " eta = " << nll_->measuredTauLeptons()[0].p4().eta() << " phi = " <<  nll_->measuredTauLeptons()[0].p4().phi() << std::endl; 
    std::cout << ">> p4: Pt = " << fittedTauLeptons()[0].pt() << " eta = " << fittedTauLeptons()[0].eta() << " phi = " << fittedTauLeptons()[0].phi() << std::endl; 
    std::cout << ">> Leg2 : " << std::endl;
    std::cout << ">> p4: pt = " << nll_->measuredTauLeptons()[1].p4().pt() << " eta = " << nll_->measuredTauLeptons()[1].p4().eta() << " phi = " <<  nll_->measuredTauLeptons()[1].p4().phi() << std::endl;
    std::cout << ">> p4: pt = " << fittedTauLeptons()[1].pt() << " eta = " << fittedTauLeptons()[1].eta() << " phi = " << fittedTauLeptons()[1].phi() << std::endl;  
  }
}
