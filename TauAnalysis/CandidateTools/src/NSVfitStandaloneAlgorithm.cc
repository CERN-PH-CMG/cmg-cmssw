#include "Math/Factory.h"
#include "Math/Functor.h"
#include "Math/GSLMCIntegrator.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"

namespace NSVfitStandalone
{
  void map_x(const double* x, int nDim, double* x_mapped)
  {
    if(nDim == 4){
      x_mapped[kXFrac]                 = x[0];
      x_mapped[kMNuNu]                 = 0.;
      x_mapped[kPhi]                   = x[1];
      x_mapped[kMaxFitParams + kXFrac] = x[2];
      x_mapped[kMaxFitParams + kMNuNu] = 0.;
      x_mapped[kMaxFitParams + kPhi]   = x[3];
    } else if(nDim == 5){
      x_mapped[kXFrac]                 = x[0];
      x_mapped[kMNuNu]                 = x[1];
      x_mapped[kPhi]                   = x[2];
      x_mapped[kMaxFitParams + kXFrac] = x[3];
      x_mapped[kMaxFitParams + kMNuNu] = 0.;
      x_mapped[kMaxFitParams + kPhi]   = x[4];
    } else if(nDim == 6){
      x_mapped[kXFrac]                 = x[0];
      x_mapped[kMNuNu]                 = x[1];
      x_mapped[kPhi]                   = x[2];
      x_mapped[kMaxFitParams + kXFrac] = x[3];
      x_mapped[kMaxFitParams + kMNuNu] = x[4];
      x_mapped[kMaxFitParams + kPhi]   = x[5];
    } else assert(0);
    //std::cout << "<map_x>:" << std::endl;
    //for ( int i = 0; i < 6; ++i ) {
    //  std::cout << " x_mapped[" << i << "] = " << x_mapped[i] << std::endl;
    //}
  }
}

NSVfitStandaloneAlgorithm::NSVfitStandaloneAlgorithm(std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons, NSVfitStandalone::Vector measuredMET , const TMatrixD& covMET, unsigned int verbosity) : 
  fitStatus_(-1), 
  verbosity_(verbosity), 
  maxObjFunctionCalls_(5000),
  mcObjectiveFunctionAdapter_(0),
  mcPtEtaPhiMassAdapter_(0),
  integrator2_(0),
  integrator2_nDim_(0),
  isInitialized2_(false),
  maxObjFunctionCalls2_(100000)
{ 
  // instantiate minuit, the arguments might turn into configurables once
  minimizer_ = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
  // instantiate the combined likelihood
  nll_ = new NSVfitStandalone::NSVfitStandaloneLikelihood(measuredTauLeptons, measuredMET, covMET, (verbosity_ > 2));
  nllStatus_ = nll_->error();
}

NSVfitStandaloneAlgorithm::~NSVfitStandaloneAlgorithm() 
{
  delete nll_;
  delete minimizer_;
  delete mcObjectiveFunctionAdapter_;
  delete mcPtEtaPhiMassAdapter_;
  delete integrator2_;
}

void
NSVfitStandaloneAlgorithm::setup()
{
  using namespace NSVfitStandalone;

  if(verbosity_>0){
    std::cout << "<NSVfitStandaloneAlgorithm::setup()>" << std::endl;
  }
  for(unsigned int idx=0; idx<nll_->measuredTauLeptons().size(); ++idx){
    if(verbosity_>0){
      std::cout << " --> upper limit of leg1::mNuNu will be set to "; 
      if(nll_->measuredTauLeptons()[idx].decayType() == kHadDecay){ 
	std::cout << "0";
      } 
      else{
	std::cout << (SVfit_namespace::tauLeptonMass - TMath::Min(nll_->measuredTauLeptons()[idx].mass(), 1.5));
      } 
      std::cout << std::endl;
    }
    // start values for xFrac
    minimizer_->SetLimitedVariable(idx*kMaxFitParams + kXFrac, std::string(TString::Format("leg%d::xFrac", idx + 1)).c_str(), 0.5, 0.1, 0., 1.);
    // start values for nunuMass
    if(nll_->measuredTauLeptons()[idx].decayType() == kHadDecay){ 
      minimizer_->SetFixedVariable(idx*kMaxFitParams + kMNuNu, std::string(TString::Format("leg%d::mNuNu", idx + 1)).c_str(), 0.); 
    } 
    else{ 
      minimizer_->SetLimitedVariable(idx*kMaxFitParams + kMNuNu, std::string(TString::Format("leg%d::mNuNu", idx + 1)).c_str(), 0.8, 0.10, 0., SVfit_namespace::tauLeptonMass - TMath::Min(nll_->measuredTauLeptons()[idx].mass(), 1.5)); 
    }
    // start values for phi
    minimizer_->SetVariable(idx*kMaxFitParams + kPhi, std::string(TString::Format("leg%d::phi", idx + 1)).c_str(), 0.0, 0.25);
  }
}

void
NSVfitStandaloneAlgorithm::fit()
{
  if(verbosity_>0){
    std::cout << "<NSVfitStandaloneAlgorithm::fit()>" << std::endl
	      << "--> dimension of fit    : " << nll_->measuredTauLeptons().size()*NSVfitStandalone::kMaxFitParams << std::endl
	      << "--> maxObjFunctionCalls : " << maxObjFunctionCalls_ << std::endl; 
  }
  // clear minimizer
  minimizer_->Clear();
  // set verbosity level of minimizer
  minimizer_->SetPrintLevel(-1);
  // setup the function to be called and the dimension of the fit
  ROOT::Math::Functor toMinimize(standaloneObjectiveFunctionAdapter_, nll_->measuredTauLeptons().size()*NSVfitStandalone::kMaxFitParams);
  minimizer_->SetFunction(toMinimize); 
  setup();
  minimizer_->SetMaxFunctionCalls(maxObjFunctionCalls_);
  // set Minuit strategy = 2, in order to get reliable error estimates:
  // http://www-cdf.fnal.gov/physics/statistics/recommendations/minuit.html
  minimizer_->SetStrategy(2);
  // compute uncertainties for increase of objective function by 0.5 wrt. 
  // minimum (objective function is log-likelihood function)
  minimizer_->SetErrorDef(0.5);
  if(verbosity_>0){
    std::cout << "--> starting ROOT::Math::Minimizer::Minimize..." << std::endl;
    std::cout << "--> #freeParameters = " << minimizer_->NFree() << ","
  	      << "--> #constrainedParameters = " << (minimizer_->NDim() - minimizer_->NFree()) << std::endl;
  }
  // do the minimization
  nll_->addDelta(false);
  nll_->addSinTheta(true);
  minimizer_->Minimize();
  if(verbosity_>1){ 
    minimizer_->PrintResults(); 
  };
  /* get Minimizer status code, check if solution is valid:
    
     0: Valid solution
     1: Covariance matrix was made positive definite
     2: Hesse matrix is invalid
     3: Estimated distance to minimum (EDM) is above maximum
     4: Reached maximum number of function calls before reaching convergence
     5: Any other failure
  */
  fitStatus_ = minimizer_->Status();
  if(verbosity_>0){ 
    std::cout << "--> fitStatus = " << fitStatus_ << std::endl; 
  }
  
  // and write out the result
  using NSVfitStandalone::kXFrac;
  using NSVfitStandalone::kMNuNu;
  using NSVfitStandalone::kPhi;
  using NSVfitStandalone::kMaxFitParams;
  // update di-tau system with final fit results
  nll_->results(fittedTauLeptons_, minimizer_->X());
  // determine uncertainty of the fitted di-tau mass
  double x1RelErr = minimizer_->Errors()[kXFrac]/minimizer_->X()[kXFrac];
  double x2RelErr = minimizer_->Errors()[kMaxFitParams + kXFrac]/minimizer_->X()[kMaxFitParams + kXFrac];
  // this gives a unified treatment for retrieving the result for integration mode and fit mode
  fittedDiTauSystem_ = fittedTauLeptons_[0] + fittedTauLeptons_[1];
  mass_ = fittedDiTauSystem().mass();
  massUncert_ = TMath::Sqrt(0.25*x1RelErr*x1RelErr + 0.25*x2RelErr*x2RelErr)*fittedDiTauSystem().mass();
  if(verbosity_>1){
    std::cout << ">> -------------------------------------------------------------" << std::endl;
    std::cout << ">> Resonance Record: " << std::endl;
    std::cout << ">> -------------------------------------------------------------" << std::endl;
    std::cout << ">> pt  (di-tau)    = " << fittedDiTauSystem().pt  () << std::endl;
    std::cout << ">> eta (di-tau)    = " << fittedDiTauSystem().eta () << std::endl;
    std::cout << ">> phi (di-tau)    = " << fittedDiTauSystem().phi () << std::endl;
    std::cout << ">> mass(di-tau)    = " << fittedDiTauSystem().mass() << std::endl;  
    std::cout << ">> massUncert      = " << massUncert_ << std::endl
	      << "   error[xFrac1]   = " << minimizer_->Errors()[kXFrac] << std::endl
	      << "   value[xFrac1]   = " << minimizer_->X()[kXFrac]      << std::endl
	      << "   error[xFrac2]   = " << minimizer_->Errors()[kMaxFitParams+kXFrac] << std::endl
	      << "   value[xFrac2]   = " << minimizer_->X()[kMaxFitParams+kXFrac]      << std::endl;
    for(unsigned int leg=0; leg<2; ++leg){
      std::cout << ">> -------------------------------------------------------------" << std::endl;
      std::cout << ">> Leg " << leg+1 << " Record: " << std::endl;
      std::cout << ">> -------------------------------------------------------------" << std::endl;
      std::cout << ">> pt  (meas)      = " << nll_->measuredTauLeptons()[leg].p4().pt () << std::endl;
      std::cout << ">> eta (meas)      = " << nll_->measuredTauLeptons()[leg].p4().eta() << std::endl;
      std::cout << ">> phi (meas)      = " << nll_->measuredTauLeptons()[leg].p4().phi() << std::endl; 
      std::cout << ">> pt  (fit )      = " << fittedTauLeptons()[leg].pt () << std::endl;
      std::cout << ">> eta (fit )      = " << fittedTauLeptons()[leg].eta() << std::endl;
      std::cout << ">> phi (fit )      = " << fittedTauLeptons()[leg].phi() << std::endl; 
    }
  }
}

void
NSVfitStandaloneAlgorithm::integrateVEGAS()
{
  using namespace NSVfitStandalone;
  
  if(verbosity_>0){
    std::cout << "<NSVfitStandaloneAlgorithm::integrateVEGAS()>:" << std::endl;
  }

  double pi = 3.14159265;
  // number of hadrponic decays
  int khad = 0;
  for(unsigned int idx=0; idx<nll_->measuredTauLeptons().size(); ++idx){
    if(nll_->measuredTauLeptons()[idx].decayType() == kHadDecay){ 
      khad++; 
    }
  }
  // number of parameters for fit
  int par = nll_->measuredTauLeptons().size()*NSVfitStandalone::kMaxFitParams - (khad + 1);
  /* --------------------------------------------------------------------------------------
     lower and upper bounds for integration. Boundaries are deefined for each decay channel
     separately. The order is: 
     
     - 3dim : fully hadronic {xFrax, phihad1, phihad2}
     - 4dim : semi  leptonic {xFrac, nunuMass, philep, phihad}
     - 5dim : fully leptonic {xFrac, nunuMass1, philep1, nunuMass2, philep2}
     
     xl* defines the lower integation bound, xu* defines the upper integration bound in 
     the following definitions. 
     ATTENTION: order matters here! In the semi-leptonic decay the lepton must go first in 
     the parametrization, as it is first in the definition of integral boundaries. This is
     the reason why the measuredLeptons are eventually re-ordered in the constructor of 
     this class before passing them on to NSVfitStandaloneLikelihood.
  */
  double xl3[3] = { 0.0, -pi, -pi };
  double xu3[3] = { 1.0,  pi,  pi };
  double xl4[4] = { 0.0, 0.0, -pi, -pi };
  double xu4[4] = { 1.0, SVfit_namespace::tauLeptonMass, pi, pi };
  double xl5[5] = { 0.0, 0.0, -pi, 0.0, -pi };
  double xu5[5] = { 1.0, SVfit_namespace::tauLeptonMass, pi, SVfit_namespace::tauLeptonMass, pi };

  // integrator instance
  //ROOT::Math::IntegratorMultiDim ig2(ROOT::Math::IntegrationMultiDim::kVEGAS, 1.e-12, 1.e-5);
  ROOT::Math::GSLMCIntegrator ig2("vegas", 1.e-12, 1.e-5, 2000);
  ROOT::Math::Functor toIntegrate(&standaloneObjectiveFunctionAdapter_, &ObjectiveFunctionAdapter::Eval, par); 
  standaloneObjectiveFunctionAdapter_.SetPar(par);
  ig2.SetFunction(toIntegrate);
  nll_->addDelta(true);
  nll_->addSinTheta(false);
  nll_->addPhiPenalty(false);
  int count = 0;
  double pMax = 0.;
  double mtest = measuredDiTauSystem().mass();
  bool skiphighmasstail = false;
  for(int i=0; i<100 && (!skiphighmasstail); ++i){
    standaloneObjectiveFunctionAdapter_.SetM(mtest);
    double p = -1.;
    if(par == 4){
      p = ig2.Integral(xl4, xu4);
    } else if(par == 5){
      p = ig2.Integral(xl5, xu5);
    } else if(par == 3){
      p = ig2.Integral(xl3, xu3);
    } else{
      std::cout << " >> ERROR : the nubmer of measured leptons must be 2" << std::endl;
      assert(0);
    }
    if(verbosity_>1){
      std::cout << "--> scan idx = " << i << "  mtest = " << mtest << "  p = " << p << "  pmax = " << pMax << std::endl;
    }
    if(p>pMax){
      mass_ = mtest;
      pMax  = p;
      count = 0;
    } 
    else{
      if(p<(1.e-3*pMax)){
	++count;
	if(count>= 5){
	  skiphighmasstail=true;
	}
      } 
      else {
	count=0;
      }
    }
    mtest += TMath::Max(2.5, 0.025*mtest);
  }
  if ( verbosity_ > 0 ) {
    std::cout << "--> mass  = " << mass_  << std::endl;
    std::cout << "--> pmax  = " << pMax   << std::endl;
    std::cout << "--> count = " << count  << std::endl;
  }
}

void
NSVfitStandaloneAlgorithm::integrateMarkovChain()
{
  using namespace NSVfitStandalone;
  
  if(verbosity_>0){
    std::cout << "<NSVfitStandaloneAlgorithm::integrateMarkovChain()>:" << std::endl;
  }
  if(isInitialized2_){
    mcPtEtaPhiMassAdapter_->Reset();
  } 
  else{
    // initialize    
    edm::ParameterSet cfg;
    cfg.addParameter<std::string>("mode", "Metropolis");
    cfg.addParameter<std::string>("initMode", "none");
    cfg.addParameter<unsigned>("numIterBurnin", TMath::Nint(0.10*maxObjFunctionCalls2_));
    cfg.addParameter<unsigned>("numIterSampling", maxObjFunctionCalls2_);
    cfg.addParameter<unsigned>("numIterSimAnnealingPhase1", TMath::Nint(0.02*maxObjFunctionCalls2_));    
    cfg.addParameter<unsigned>("numIterSimAnnealingPhase2", TMath::Nint(0.06*maxObjFunctionCalls2_));
    cfg.addParameter<double>("T0", 15.);
    cfg.addParameter<double>("alpha", 1.0 - 1.e+2/maxObjFunctionCalls2_);
    cfg.addParameter<unsigned>("numChains", 1);
    cfg.addParameter<unsigned>("numBatches", 1);
    cfg.addParameter<unsigned>("L", 1);
    cfg.addParameter<double>("epsilon0", 1.e-2);
    cfg.addParameter<double>("nu", 0.71);
    cfg.addParameter<std::string>("name", "NSVfitStandaloneAlgorithm");
    cfg.addParameter<int>("verbosity", -1);
    //cfg.addParameter<int>("verbosity", 2);
    integrator2_ = new MarkovChainIntegrator(cfg);
    mcObjectiveFunctionAdapter_ = new MCObjectiveFunctionAdapter();
    integrator2_->setIntegrand(*mcObjectiveFunctionAdapter_);
    integrator2_nDim_ = 0;
    mcPtEtaPhiMassAdapter_ = new MCPtEtaPhiMassAdapter();
    integrator2_->registerCallBackFunction(*mcPtEtaPhiMassAdapter_);
    isInitialized2_= true;    
  }

  const double pi = 3.14159265;
  // number of hadronic decays
  int khad = 0;
  for(unsigned int idx=0; idx<nll_->measuredTauLeptons().size(); ++idx){
    if(nll_->measuredTauLeptons()[idx].decayType() == kHadDecay){ 
      ++khad; 
    }
  }
  // number of parameters for fit
  int nDim = nll_->measuredTauLeptons().size()*NSVfitStandalone::kMaxFitParams - khad;  
  if(nDim != integrator2_nDim_){
    mcObjectiveFunctionAdapter_->SetNDim(nDim);
    integrator2_->setIntegrand(*mcObjectiveFunctionAdapter_);
    mcPtEtaPhiMassAdapter_->SetNDim(nDim);
    integrator2_nDim_ = nDim;
  }
  /* --------------------------------------------------------------------------------------
     lower and upper bounds for integration. Boundaries are defined for each decay channel
     separately. The order is: 
     
     - 4dim : fully hadronic {xhad1, phihad1, xhad2, phihad2}
     - 5dim : semi  leptonic {xlep, nunuMass, philep, xhad, phihad}
     - 6dim : fully leptonic {xlep1, nunuMass1, philep1, xlep2, nunuMass2, philep2}
     
     x0* defines the start value for the integration, xl* defines the lower integation bound, 
     xu* defines the upper integration bound in the following definitions. 
     ATTENTION: order matters here! In the semi-leptonic decay the lepton must go first in 
     the parametrization, as it is first in the definition of integral boundaries. This is
     the reason why the measuredLeptons are eventually re-ordered in the constructor of 
     this class before passing them on to NSVfitStandaloneLikelihood.
  */
  double x04[4] = { 0.5, 0.0, 0.5, 0.0 };
  double xl4[4] = { 0.0, -pi, 0.0, -pi };
  double xu4[4] = { 1.0,  pi, 1.0,  pi };
  double x05[5] = { 0.5, 0.8, 0.0, 0.5, 0.0 };
  double xl5[5] = { 0.0, 0.0, -pi, 0.0, -pi };
  double xu5[5] = { 1.0, SVfit_namespace::tauLeptonMass, pi, 1.0, pi };
  xu5[1] = SVfit_namespace::tauLeptonMass - TMath::Min(nll_->measuredTauLeptons()[0].mass(), 1.6);
  double x06[6] = { 0.5, 0.8, 0.0, 0.5, 0.8, 0.0 };
  double xl6[6] = { 0.0, 0.0, -pi, 0.0, 0.0, -pi };
  double xu6[6] = { 1.0, SVfit_namespace::tauLeptonMass, pi, 1.0, SVfit_namespace::tauLeptonMass, pi };
  xu6[1] = SVfit_namespace::tauLeptonMass - TMath::Min(nll_->measuredTauLeptons()[0].mass(), 1.6);
  xu6[4] = SVfit_namespace::tauLeptonMass - TMath::Min(nll_->measuredTauLeptons()[1].mass(), 1.6);
  std::vector<double> x0(nDim);
  std::vector<double> xl(nDim);
  std::vector<double> xu(nDim);
  for(int i = 0; i < nDim; ++i){
    if(nDim == 4){
      x0[i] = x04[i];
      xl[i] = xl4[i];
      xu[i] = xu4[i];
    } else if(nDim == 5){
      x0[i] = x05[i];
      xl[i] = xl5[i];
      xu[i] = xu5[i];
    } else if(nDim == 6){
      x0[i] = x06[i];
      xl[i] = xl6[i];
      xu[i] = xu6[i];
    } else {
      std::cout << " >> ERROR : the number of measured leptons must be 2" << std::endl;
      assert(0);
    }
    // transform startPosition into interval ]0..1[
    // expected by MarkovChainIntegrator class
    x0[i] = (x0[i] - xl[i])/(xu[i] - xl[i]);
    //std::cout << "x0[" << i << "] = " << x0[i] << std::endl;
  }
  integrator2_->initializeStartPosition_and_Momentum(x0);
  nll_->addDelta(false);
  nll_->addSinTheta(false);
  nll_->addPhiPenalty(false);
  double integral = 0.;
  double integralErr = 0.;
  int errorFlag = 0;
  //integrator2_->integrate(xl, xu, integral, integralErr, errorFlag, "/data1/veelken/tmp/svFitStudies/svFitStandalone/debugMarkovChain.root");
  integrator2_->integrate(xl, xu, integral, integralErr, errorFlag);
  fitStatus_ = errorFlag;
  pt_ = mcPtEtaPhiMassAdapter_->getPt();
  ptUncert_ = mcPtEtaPhiMassAdapter_->getPtUncert();
  eta_ = mcPtEtaPhiMassAdapter_->getEta();
  etaUncert_ = mcPtEtaPhiMassAdapter_->getEtaUncert();
  phi_ = mcPtEtaPhiMassAdapter_->getPhi();
  phiUncert_ = mcPtEtaPhiMassAdapter_->getPhiUncert();
  mass_ = mcPtEtaPhiMassAdapter_->getMass();
  massUncert_ = mcPtEtaPhiMassAdapter_->getMassUncert();
  fittedDiTauSystem_ = math::PtEtaPhiMLorentzVector(pt_, eta_, phi_, mass_);
  if(verbosity_ > 0){
    std::cout << "--> Pt = " << pt_ << ", eta = " << eta_ << ", phi = " << phi_ << ", mass  = " << mass_  << std::endl;
  }
}
