#include "TauAnalysis/CandidateTools/interface/MarkovChainIntegrator.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include <TMath.h>

#include <iomanip>
#include <limits>
#include <assert.h>

enum { kMetropolis, kHybrid };

enum { kUniform, kGaus, kNone };

double square(double x)
{
  return x*x;
}

MarkovChainIntegrator::MarkovChainIntegrator(const edm::ParameterSet& cfg)
  : name_(""),
    integrand_(0),
    startPosition_and_MomentumFinder_(0),
    x_(0),
    numIntegrationCalls_(0),
    numMovesTotal_accepted_(0),
    numMovesTotal_rejected_(0),
    monitorFile_(0),
    monitorTree_(0)
{
  //std::cout << "<MarkovChainIntegrator::MarkovChainIntegrator>:" << std::endl;

  if ( cfg.exists("name") ) 
    name_ = cfg.getParameter<std::string>("name");
  //std::cout << " name = " << name_ << std::endl;

  std::string moveMode_string = cfg.getParameter<std::string>("mode");
  if      ( moveMode_string == "Metropolis" ) moveMode_ = kMetropolis;
  else if ( moveMode_string == "Hybrid"     ) moveMode_ = kHybrid;
  else throw cms::Exception("MarkovChainIntegrator")
    << "Invalid Configuration Parameter 'mode' = " << moveMode_string << ","
    << " expected to be either \"Metropolis\" or \"Hybrid\" !!\n";

  std::string initMode_string = cfg.getParameter<std::string>("initMode");
  if      ( initMode_string == "uniform" ) initMode_ = kUniform;
  else if ( initMode_string == "Gaus"    ) initMode_ = kGaus;
  else if ( initMode_string == "none"    ) initMode_ = kNone;
  else throw cms::Exception("MarkovChainIntegrator")
    << "Invalid Configuration Parameter 'initMode' = " << initMode_string << ","
    << " expected to be either \"uniform\", \"Gaus\" or \"none\" !!\n";

//--- get parameters defining number of "stochastic moves" performed per integration
  numIterBurnin_ = cfg.getParameter<unsigned>("numIterBurnin");
  numIterSampling_ = cfg.getParameter<unsigned>("numIterSampling");

//--- get parameters defining maximum number of attempts to find a valid starting-position for the Markov Chain
  maxCallsStartingPos_ = ( cfg.exists("maxCallsStartingPos") ) ?
    cfg.getParameter<unsigned>("maxCallsStartingPos") : 1000000;

//--- get parameters defining "simulated annealing" stage at beginning of integration
  numIterSimAnnealingPhase1_ = cfg.getParameter<unsigned>("numIterSimAnnealingPhase1");
  numIterSimAnnealingPhase2_ = cfg.getParameter<unsigned>("numIterSimAnnealingPhase2");
  numIterSimAnnealingPhase1plus2_ = numIterSimAnnealingPhase1_ + numIterSimAnnealingPhase2_;
  if ( numIterSimAnnealingPhase1plus2_ > numIterBurnin_ ) 
    throw cms::Exception("MarkovChainIntegrator")
      << "Invalid Configuration Parameters 'numIterSimAnnealingPhase1' = " << numIterSimAnnealingPhase1_ << ","
      << " 'numIterSimAnnealingPhase2' = " << numIterSimAnnealingPhase2_ << ","
      << " sim. Annealing and Sampling stages must not overlap !!\n";
  T0_ = cfg.getParameter<double>("T0");
  sqrtT0_ = TMath::Sqrt(T0_);
  alpha_ = cfg.getParameter<double>("alpha");
  if ( !(alpha_ > 0. && alpha_ < 1.) )
    throw cms::Exception("MarkovChainIntegrator")
      << "Invalid Configuration Parameter 'alpha' = " << alpha_ << "," 
      << " value within interval ]0..1[ expected !!\n";
  alpha2_ = square(alpha_);
  
//--- get parameter specifying how many Markov Chains are run in parallel
  numChains_ = cfg.getParameter<unsigned>("numChains");
  if ( numChains_ == 0 )
    throw cms::Exception("MarkovChainIntegrator")
      << "Invalid Configuration Parameter 'numChains' = " << numChains_ << "," 
      << " value greater 0 expected !!\n";

  numBatches_ = cfg.getParameter<unsigned>("numBatches");
  if ( numBatches_ == 0 )
    throw cms::Exception("MarkovChainIntegrator")
      << "Invalid Configuration Parameter 'numBatches' = " << numBatches_ << "," 
      << " value greater 0 expected !!\n";
  if ( (numIterSampling_ % numBatches_) != 0 )
    throw cms::Exception("MarkovChainIntegrator")
      << "Invalid Configuration Parameter 'numBatches' = " << numBatches_ << "," 
      << " factor of numIterSampling = " << numIterSampling_ << " expected !!\n";
  
//--- get parameters specific to "dynamic moves" 
  L_ = cfg.getParameter<unsigned>("L");
  if ( cfg.existsAs<double>("epsilon0") ) {
    epsilon0_  = cfg.getParameter<double>("epsilon0");
    useVariableEpsilon0_ = false;
  } else if ( cfg.existsAs<vdouble >("epsilon0") ) {
    epsilon0s_ = cfg.getParameter<vdouble>("epsilon0");
    useVariableEpsilon0_ = true;
  } else throw cms::Exception("MarkovChainIntegrator")
      << "Configuration Parameter 'epsilon0' undefined !!\n";
  nu_ = cfg.getParameter<double>("nu");

  verbosity_ = ( cfg.exists("verbosity") ) ?
    cfg.getParameter<int>("verbosity") : 0;
  //std::cout << " verbosity = " << verbosity_ << std::endl;
}

MarkovChainIntegrator::~MarkovChainIntegrator()
{
  if ( verbosity_ >= 0 ) {
    std::cout << "<MarkovChainIntegrator::~MarkovChainIntegrator>:" << std::endl;
    std::cout << " name = " << name_ << std::endl;
    std::cout << " integration calls = " << numIntegrationCalls_ << std::endl;
    std::cout << " moves: accepted = " << numMovesTotal_accepted_ << ", rejected = " << numMovesTotal_rejected_ 
	      << " (fraction = " << (double)numMovesTotal_accepted_/(numMovesTotal_accepted_ + numMovesTotal_rejected_)*100. 
	      << "%)" << std::endl;
  }

  delete [] x_;

  delete monitorTree_;
  delete monitorFile_;
}

void MarkovChainIntegrator::setIntegrand(const ROOT::Math::Functor& integrand)
{
  integrand_ = &integrand;
  numDimensions_ = integrand.NDim();

  delete [] x_;
  x_ = new double[numDimensions_];

  xMin_.resize(numDimensions_); 
  xMax_.resize(numDimensions_);  

  dqDerr_.resize(numDimensions_); 
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    dqDerr_[iDimension] = 1.e-6;
  }

  if ( useVariableEpsilon0_ ) {
    if ( epsilon0s_.size() != numDimensions_ ) 
      throw cms::Exception("MarkovChainIntegrator::setIntegrand")
	<< "Mismatch in dimensionality between integrand = " << numDimensions_
	<< " and Configuration Parameter 'epsilon0' = " << epsilon0s_.size() << " !!\n";
  } else {
    epsilon0s_.resize(numDimensions_); 
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
      epsilon0s_[iDimension] = epsilon0_;
    }
  }

  p_.resize(2*numDimensions_);   // first N entries = "significant" components, last N entries = "dummy" components
  q_.resize(numDimensions_);     // "potential energy" E(q) depends in the first N "significant" components only
  gradE_.resize(numDimensions_); 
  prob_ = 0.;

  u_.resize(2*numDimensions_);   // first N entries = "significant" components, last N entries = "dummy" components
  pProposal_.resize(numDimensions_);
  qProposal_.resize(numDimensions_);

  probSum_.resize(numChains_*numBatches_);  
  for ( vdouble::iterator probSum_i = probSum_.begin();
	probSum_i != probSum_.end(); ++probSum_i ) {
    (*probSum_i) = 0.;
  }
  integral_.resize(numChains_*numBatches_);  
}

void MarkovChainIntegrator::setStartPosition_and_MomentumFinder(const ROOT::Math::Functor& startPosition_and_MomentumFinder)
{
  startPosition_and_MomentumFinder_ = &startPosition_and_MomentumFinder;
}

void MarkovChainIntegrator::registerCallBackFunction(const ROOT::Math::Functor& function)
{
  callBackFunctions_.push_back(&function);
}

void MarkovChainIntegrator::setF(const ROOT::Math::Functor& f, const std::string& branchName)
{
  extraMonitorBranches_.push_back(monitorElementType(&f, branchName));
}

void MarkovChainIntegrator::integrate(const std::vector<double>& xMin, const std::vector<double>& xMax, 
				      double& integral, double& integralErr, int& errorFlag,
				      const std::string& monitorFileName)
{
  if ( verbosity_ >= 2 ) {
    std::cout << "<MarkovChainIntegrator::integrate>:" << std::endl;
    std::cout << " name = " << name_ << std::endl;
    std::cout << " numDimensions = " << numDimensions_ << std::endl;
  }

  if ( monitorFileName != "" ) openMonitorFile(monitorFileName);

  if ( !integrand_ )
    throw cms::Exception("MarkovChainIntegrator::integrate")
      << "No integrand function has been set yet !!\n";

  if ( !(xMin.size() == numDimensions_ && xMax.size() == numDimensions_) )
    throw cms::Exception("MarkovChainIntegrator::integrate")
      << "Mismatch in dimensionality between integrand = " << numDimensions_
      << " and integration limits = " << xMin.size() << "/" << xMax.size() << " !!\n";
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    xMin_[iDimension] = xMin[iDimension];
    xMax_[iDimension] = xMax[iDimension];
    if ( verbosity_ >= 2 ) {
      std::cout << "dimension #" << iDimension << ": min = " << xMin_[iDimension] << ", max = " << xMax_[iDimension] << std::endl;
    }
  }
  
//--- CV: set random number generator used to initialize starting-position
//        for each integration, in order to make integration results independent of processing history
  rnd_.SetSeed(12345);

  numMoves_accepted_ = 0;
  numMoves_rejected_ = 0;

  unsigned k = numChains_*numBatches_;  
  unsigned m = numIterSampling_/numBatches_;

  numChainsRun_ = 0; 

  for ( unsigned iChain = 0; iChain < numChains_; ++iChain ) {
    bool isValidStartPos = false;
    if ( initMode_ == kNone ) {
      prob_ = evalProb(q_);
      if ( prob_ > 0. ) {
	bool isWithinBounds = true;
	for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
	  double q_i = q_[iDimension];
	  if ( !(q_i > 0. && q_i < 1.) ) isWithinBounds = false;
	}
	if ( isWithinBounds ) {
	  isValidStartPos = true;
	} else {
	  edm::LogWarning ("MarkovChainIntegrator::integrate")
	    << "Requested start-position = " << format_vdouble(q_) << " not within interval ]0..1[ --> searching for valid alternative !!";
	}
      } else {
	edm::LogWarning ("MarkovChainIntegrator::integrate")
	  << "Requested start-position = " << format_vdouble(q_) << " returned probability zero --> searching for valid alternative !!";
      }
    }    
    unsigned iTry = 0;
    while ( !isValidStartPos && iTry < maxCallsStartingPos_ ) {
      initializeStartPosition_and_Momentum();
//--- CV: check if start-position is within "valid" (physically allowed) region 
      bool isWithinPhysicalRegion = true;
      if ( startPosition_and_MomentumFinder_ ) {
	updateX(q_);
	isWithinPhysicalRegion = ((*startPosition_and_MomentumFinder_)(x_) > 0.5);
      }
      if ( isWithinPhysicalRegion ) {
	prob_ = evalProb(q_);
	if ( prob_ > 0. ) {
	  isValidStartPos = true;
	} else {
	  if ( iTry > 0 && (iTry % 100000) == 0 ) {
	    if ( iTry == 100000 ) std::cout << "<MarkovChainIntegrator::integrate (name = " << name_ << ")>:" << std::endl;
	    std::cout << "try #" << iTry << ": did not find valid start-position yet." << std::endl;
	    //std::cout << "(q = " << format_vdouble(q_) << ", prob = " << prob_ << ")" << std::endl;
	  }
	}
      }
      ++iTry;
    }
    if ( !isValidStartPos ) continue;

    for ( unsigned iMove = 0; iMove < numIterBurnin_; ++iMove ) {
//--- propose Markov Chain transition to new, randomly chosen, point
      if ( verbosity_ >= 2 ) std::cout << "burn-in move #" << iMove << ":" << std::endl;
      bool isAccepted = false;
      bool isValid = true;
      do {
	makeStochasticMove(iMove, isAccepted, isValid);
      } while ( !isValid );
    }

    unsigned idxBatch = iChain*numBatches_;

    for ( unsigned iMove = 0; iMove < numIterSampling_; ++iMove ) {
//--- propose Markov Chain transition to new, randomly chosen, point;
//    evaluate "call-back" functions at this point
      if ( verbosity_ >= 2 ) std::cout << "sampling move #" << iMove << ":" << std::endl;
      bool isAccepted = false;
      bool isValid = true;
      do {
	makeStochasticMove(numIterBurnin_ + iMove, isAccepted, isValid);
      } while ( !isValid );
      if ( isAccepted ) {
	++numMoves_accepted_;
      } else {
	++numMoves_rejected_;
      }

      updateX(q_);
      for ( std::vector<const ROOT::Math::Functor*>::const_iterator callBackFunction = callBackFunctions_.begin();
	    callBackFunction != callBackFunctions_.end(); ++callBackFunction ) {
	(**callBackFunction)(x_);
      }

      if ( iMove > 0 && (iMove % m) == 0 ) ++idxBatch;
      probSum_[idxBatch] += prob_;

      if ( monitorFile_ ) updateMonitorFile();
    }

    ++numChainsRun_;
  }

  for ( unsigned idxBatch = 0; idxBatch < probSum_.size(); ++idxBatch ) {  
    integral_[idxBatch] = probSum_[idxBatch]/m;
    //if ( verbosity_ >= 1 ) std::cout << "integral[" << idxBatch << "] = " << integral_[idxBatch] << std::endl;
  }

  //if ( verbosity_ >= 1 ) print(std::cout);

//--- compute integral value and uncertainty
//   (eqs. (6.39) and (6.40) in [1])   
  integral = 0.;
  for ( unsigned i = 0; i < k; ++i ) {    
    integral += integral_[i];
  }
  integral /= k;

  integralErr = 0.;
  for ( unsigned i = 0; i < k; ++i ) {
    integralErr += square(integral_[i] - integral);
  }
  if ( k >= 2 ) integralErr /= (k*(k - 1));
  integralErr = TMath::Sqrt(integralErr);

  //if ( verbosity_ >= 1 ) std::cout << "--> returning integral = " << integral << " +/- " << integralErr << std::endl;

  errorFlag = ( numChainsRun_ >= 0.5*numChains_ ) ?
    0 : 1;

  ++numIntegrationCalls_;
  numMovesTotal_accepted_ += numMoves_accepted_;
  numMovesTotal_rejected_ += numMoves_rejected_;

  if ( monitorFile_ ) closeMonitorFile();
}

void MarkovChainIntegrator::print(std::ostream& stream) const
{
  stream << "<MarkovChainIntegrator::print>:" << std::endl;
  for ( unsigned iChain = 0; iChain < numChains_; ++iChain ) {
    double integral = 0.;
    for ( unsigned iBatch = 0; iBatch < numBatches_; ++iBatch ) {    
      double integral_i = integral_[iChain*numBatches_ + iBatch];
      //std::cout << "batch #" << iBatch << ": integral = " << integral_i << std::endl;
      integral += integral_i;
    }
    integral /= numBatches_;
    //std::cout << "<integral> = " << integral << std::endl;
    
    double integralErr = 0.;
    for ( unsigned iBatch = 0; iBatch < numBatches_; ++iBatch ) { 
      double integral_i = integral_[iChain*numBatches_ + iBatch];
      integralErr += square(integral_i - integral);
    }
    if ( numBatches_ >= 2 ) integralErr /= (numBatches_*(numBatches_ - 1));
    integralErr = TMath::Sqrt(integralErr);

    std::cout << " chain #" << iChain << ": integral = " << integral << " +/- " << integralErr << std::endl;
  }
  std::cout << "moves: accepted = " << numMoves_accepted_ << ", rejected = " << numMoves_rejected_ 
	    << " (fraction = " << (double)numMoves_accepted_/(numMoves_accepted_ + numMoves_rejected_)*100. 
	    << "%)" << std::endl;
}

//
//-------------------------------------------------------------------------------
//

void MarkovChainIntegrator::initializeStartPosition_and_Momentum(const std::vector<double>& q)
{
//--- set start position of Markov Chain in N-dimensional space to given values
  if ( q.size() == numDimensions_ ) {
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
      double q_i = q[iDimension];
      if ( q_i > 0. && q_i < 1. ) {
	q_[iDimension] = q_i;
      } else {
	throw cms::Exception("MarkovChainIntegrator")
	  << "Invalid start-position coordinates = " << format_vdouble(q) << " !!\n";
      }
    }
  } else { 
    throw cms::Exception("MarkovChainIntegrator")
      << "Mismatch in dimensionality between integrand = " << numDimensions_
      << " and vector of start-position coordinates = " << q.size() << " !!\n";
  }
}

void MarkovChainIntegrator::initializeStartPosition_and_Momentum()
{
//--- randomly choose start position of Markov Chain in N-dimensional space
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    bool isInitialized = false;
    while ( !isInitialized ) {
      double q0 = 0.;
      if ( initMode_ == kGaus ) q0 = rnd_.Gaus(0.5, 0.5);
      else q0 = rnd_.Uniform(0., 1.);
      if ( q0 > 0. && q0 < 1. ) {
	q_[iDimension] = q0;
	isInitialized = true;
      }
    }
  }

  if ( verbosity_ >= 1 ) {
    std::cout << "<MarkovChainIntegrator::initializeStartPosition_and_Momentum>:" << std::endl;
    std::cout << " q = " << format_vdouble(q_) << std::endl;
  }
}

void MarkovChainIntegrator::sampleSphericallyRandom()
{
//--- compute vector of unit length
//    pointing in random direction in N-dimensional space
//
//    NOTE: the algorithm implemented in this function 
//          uses the fact that a N-dimensional Gaussian is spherically symmetric
//         (u is uniformly distributed over the surface of an N-dimensional hypersphere)
//
  double uMag2 = 0.;
  for ( unsigned iDimension = 0; iDimension < 2*numDimensions_; ++iDimension ) {
    double u_i = rnd_.Gaus(0., 1.);
    u_[iDimension] = u_i;
    uMag2 += (u_i*u_i);
  }
  double uMag = TMath::Sqrt(uMag2);
  for ( unsigned iDimension = 0; iDimension < 2*numDimensions_; ++iDimension ) {
    u_[iDimension] /= uMag;
  }
}

void MarkovChainIntegrator::makeStochasticMove(unsigned idxMove, bool& isAccepted, bool& isValid)
{
//--- perform "stochastic" move
//   (eq. 24 in [2])
  if ( verbosity_ >= 2 ) {
    std::cout << "<MarkovChainIntegrator::makeStochasticMove>:" << std::endl;
    std::cout << " idx = " << idxMove << std::endl;
    std::cout << " q = " << format_vdouble(q_) << std::endl;
    std::cout << " prob = " << prob_ << std::endl;
    //std::cout << " Ks = " << evalK(p_, 0, numDimensions_) << std::endl;
  }
  //if ( (idxMove % 1000) == 0 ) std::cout << "computing move #" << idxMove << "..." << std::endl;

//--- perform random updates of momentum components
  if ( idxMove < numIterSimAnnealingPhase1_ ) {
    //std::cout << "case 1" << std::endl;
    for ( unsigned iDimension = 0; iDimension < 2*numDimensions_; ++iDimension ) {
      p_[iDimension] = sqrtT0_*rnd_.Gaus(0., 1.);
    }
  } else if ( idxMove < numIterSimAnnealingPhase1plus2_ ) {
    //std::cout << "case 2" << std::endl;
    double pMag2 = 0.;
    for ( unsigned iDimension = 0; iDimension < 2*numDimensions_; ++iDimension ) {
      double p_i = p_[iDimension];
      pMag2 += p_i*p_i;
    }
    double pMag = TMath::Sqrt(pMag2);
    sampleSphericallyRandom();
    for ( unsigned iDimension = 0; iDimension < 2*numDimensions_; ++iDimension ) {
      p_[iDimension] = alpha_*pMag*u_[iDimension] + (1. - alpha2_)*rnd_.Gaus(0., 1.);
    }
  } else {
    //std::cout << "case 3" << std::endl;
    for ( unsigned iDimension = 0; iDimension < 2*numDimensions_; ++iDimension ) {
      p_[iDimension] = rnd_.Gaus(0., 1.);
    }
  }

  if ( verbosity_ >= 2 ) {
    std::cout << "p(updated) = " << format_vdouble(p_) << std::endl;
    //std::cout << " Ks = " << evalK(p_, 0, numDimensions_) << std::endl;
    //std::cout << " Kd = " << evalK(p_, numDimensions_, 2*numDimensions_) << std::endl;
  }

//--- choose random step size 
  double exp_nu_times_C = 0.;
  do {
    double C = rnd_.BreitWigner(0., 1.);
    exp_nu_times_C = TMath::Exp(nu_*C);
  } while ( TMath::IsNaN(exp_nu_times_C) || !TMath::Finite(exp_nu_times_C) || exp_nu_times_C > 1.e+6 );
  vdouble epsilon(numDimensions_);
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    epsilon[iDimension] = epsilon0s_[iDimension]*exp_nu_times_C;
  }
  if ( verbosity_ >= 2 ) std::cout << "epsilon = " << format_vdouble(epsilon) << std::endl;

  if        ( moveMode_ == kMetropolis ) { // Metropolis algorithm: move according to eq. (27) in [2]
//--- update position components
//    by single step of chosen size in direction of the momentum components
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {    
      qProposal_[iDimension] = q_[iDimension] + epsilon[iDimension]*p_[iDimension];
    }
  } else if ( moveMode_ == kHybrid     ) { // Hybrid algorithm: move according to eqs. (20)-(23) in [2]
//--- initialize position components
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
      qProposal_[iDimension] = q_[iDimension];
    }
//--- evolve momentum and position components
//    according to discretized Hamiltonian mechanics 
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
      pProposal_[iDimension] = p_[iDimension];
    }
    makeDynamicMoves(epsilon);
  } else assert(0);

  if ( verbosity_ >= 2 ) std::cout << "q(proposed) = " << format_vdouble(qProposal_) << std::endl;

//--- ensure that proposed new point is within integration region
//   (take integration region to be "cyclic")
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {         
    double q_i = qProposal_[iDimension];
    q_i = q_i - TMath::Floor(q_i);
    assert(q_i >= 0. && q_i <= 1.);
    qProposal_[iDimension] = q_i;
  }

//--- check if proposed move of Markov Chain to new position is accepted or not:
//    compute change in phase-space volume for "dummy" momentum components
//   (eqs. 25 in [2])
  double probProposal = evalProb(qProposal_);
  if ( verbosity_ >= 2 ) std::cout << "prob(proposed) = " << probProposal << std::endl;

  double deltaE = 0.;
  if      ( probProposal > 0. && prob_ > 0. ) deltaE = -TMath::Log(probProposal/prob_);
  else if ( probProposal > 0.               ) deltaE = -std::numeric_limits<double>::max();
  else if (                      prob_ > 0. ) deltaE = +std::numeric_limits<double>::max();
  else assert(0);
  //if ( verbosity_ >= 1 ) std::cout << " deltaE = " << deltaE << std::endl;

  double deltaE_or_H = deltaE;
  if ( moveMode_ == kHybrid ) {
    double Ks = evalK(p_, 0, numDimensions_);
    double KsProposal = evalK(pProposal_, 0, numDimensions_);
    deltaE_or_H += (KsProposal - Ks);
    //if ( verbosity_ >= 1 ) std::cout << " deltaH = " << deltaE_or_H << std::endl;
  }
  
  double Kd = evalK(p_, numDimensions_, 2*numDimensions_);
  //if ( verbosity_ >= 1 ) std::cout << " Kd = " << Kd << std::endl;
  double base = 1. - deltaE_or_H/Kd;
  double rho = ( base > 0. ) ? 
    TMath::Power(base, 0.5*numDimensions_ - 1.) : 0.;
  if ( verbosity_ >= 2 ) std::cout << " rho = " << rho << std::endl;
  
  double u = rnd_.Uniform(0., 1.);
  if ( verbosity_ >= 2 ) std::cout << "u = " << u << std::endl;
  if ( u < rho ) {
    if ( verbosity_ >= 2 ) std::cout << "move accepted." << std::endl;
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {    
      q_[iDimension] = qProposal_[iDimension];
    }
    if ( moveMode_ == kHybrid ) {
      for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
	p_[iDimension] = pProposal_[iDimension];
      }
    }
    prob_ = evalProb(q_);
    isAccepted = true;
  } else {
    if ( verbosity_ >= 2 ) std::cout << "move rejected." << std::endl;
    isAccepted = false;
  }
}

void MarkovChainIntegrator::makeDynamicMoves(const std::vector<double>& epsilon)
{
//--- perform "dynamical move"
//   (execute series of L "leap-frog" steps, eqs. 20-23 in [2])
  for ( unsigned iLeapFrogStep = 0; iLeapFrogStep < L_; ++iLeapFrogStep ) {
    //if ( verbosity_ >= 1 ) std::cout << "leap-frog step #" << iLeapFrogStep << ":" << std::endl;    
    updateGradE(qProposal_);
    //if ( verbosity_ >= 1 ) std::cout << " gradE = " << format_vdouble(gradE_) << std::endl;
    double step_p = ( iLeapFrogStep == 0 ) ? 
      0.5 : 1.0;
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
      pProposal_[iDimension] -= step_p*epsilon[iDimension]*gradE_[iDimension];
      qProposal_[iDimension] += epsilon[iDimension]*pProposal_[iDimension];
    }
    //if ( verbosity_ >= 1 ) {
    //  std::cout << " p(" << (iLeapFrogStep + 0.5) << ") = " << format_vdouble(pProposal_) << std::endl;
    //  std::cout << " q(" << (iLeapFrogStep + 1) << ") = " << format_vdouble(qProposal_) << std::endl;
    //  std::cout << "(prob = " << evalProb(qProposal_) << ", E = " << evalE(qProposal_) << "," 
    //		  << " Ks = " << evalK(pProposal_, 0, numDimensions_) << ","
    //		  << " E + Ks = " << (evalE(qProposal_) + evalK(pProposal_, 0, numDimensions_)) << ")" << std::endl;
    //}
  }
  updateGradE(qProposal_);
  //if ( verbosity_ >= 1 ) std::cout << " gradE = " << format_vdouble(gradE_) << std::endl;
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    pProposal_[iDimension] -= 0.5*epsilon[iDimension]*gradE_[iDimension];
  }
  //if ( verbosity_ >= 1 ) {  
  //  std::cout << " p(" << L_ << ") = " << format_vdouble(pProposal_) << std::endl;
  //  std::cout << "(prob = " << evalProb(qProposal_) << ", E = " << evalE(qProposal_) << "," 
  //	        << " Ks = " << evalK(pProposal_, 0, numDimensions_) << ","
  //	        << " E + Ks = " << (evalE(qProposal_) + evalK(pProposal_, 0, numDimensions_)) << ")" << std::endl;
  //}
}

void MarkovChainIntegrator::updateX(const std::vector<double>& q)
{
  //std::cout << "<MarkovChainIntegrator::updateX>:" << std::endl;
  //std::cout << " q = " << format_vdouble(q) << std::endl;
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    double q_i = q[iDimension];
    x_[iDimension] = (1. - q_i)*xMin_[iDimension] + q_i*xMax_[iDimension];
    //std::cout << " x[" << iDimension << "] = " << x_[iDimension] << " ";
    //std::cout << "(xMin[" << iDimension << "] = " << xMin_[iDimension] << ","
    //          << " xMax[" << iDimension << "] = " << xMax_[iDimension] << ")";
    //std::cout << std::endl;
  }
}

double MarkovChainIntegrator::evalProb(const std::vector<double>& q)
{
  updateX(q);
  double prob = (*integrand_)(x_);
  return prob;
}

double MarkovChainIntegrator::evalE(const std::vector<double>& q)
{
  double prob = evalProb(q);
  double E = -TMath::Log(prob);
  return E;
}

double MarkovChainIntegrator::evalK(const std::vector<double>& p, unsigned idxFirst, unsigned idxLast)
{
//--- compute "kinetic energy"
//   (of either the "significant" or "dummy" momentum components) 
  assert(idxLast <= p.size());
  double K = 0.;
  for ( unsigned iDimension = idxFirst; iDimension < idxLast; ++iDimension ) {
    double p_i = p[iDimension];
    K += (p_i*p_i);
  }
  K *= 0.5;
  return K;
}

void MarkovChainIntegrator::updateGradE(std::vector<double>& q)
{
//--- numerically compute gradient of "potential energy" E = -log(P(q)) at point q
  //if ( verbosity_ >= 1 ) {
  //  std::cout << "<MarkovChainIntegrator::updateGradE>:" << std::endl;
  //  std::cout << " q(1) = " << format_vdouble(q) << std::endl;
  //}

  double prob_q = evalProb(q);  
  //if ( verbosity_ >= 1 ) std::cout << " prob(q) = " << prob_q << std::endl;

  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    double q_i = q[iDimension];
    double dqDerr_i = dqDerr_[iDimension];
    double dq = ( (q_i + dqDerr_i) < 1. ) ? +dqDerr_i : -dqDerr_i;
    double q_plus_dq = q_i + dq;
    q[iDimension] = q_plus_dq;
    double prob_q_plus_dq = evalProb(q);
    double gradE_i = -(prob_q_plus_dq - prob_q)/dq;
    if ( prob_q > 0. ) gradE_i /= prob_q;
    gradE_[iDimension] = gradE_i;
    q[iDimension] = q_i;
  }

  //if ( verbosity_ >= 1 ) {
  //  std::cout << " q(2) = " << format_vdouble(q) << std::endl;
  //  std::cout << "--> gradE = " << format_vdouble(gradE_) << std::endl;
  //}
}


//
//-------------------------------------------------------------------------------
//

void MarkovChainIntegrator::openMonitorFile(const std::string& monitorFileName)
{
  monitorFile_ = new TFile(monitorFileName.data(), "RECREATE");

  monitorTree_ = new TTree("monitorTree", "Markov Chain transitions");

  branchValues_.resize(numDimensions_ + 1);

  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    std::string branchName = Form("x%u", iDimension);
    std::string branchName_and_option = Form("%s/F", branchName.data());
    monitorTree_->Branch(branchName.data(), &branchValues_[iDimension], branchName_and_option.data());
  }
  
  for ( std::vector<monitorElementType>::iterator extraMonitorBranch = extraMonitorBranches_.begin();
	extraMonitorBranch != extraMonitorBranches_.end(); ++extraMonitorBranch ) {
    monitorTree_->Branch(extraMonitorBranch->branchName_.data(), &extraMonitorBranch->branchValue_, Form("%s/F", extraMonitorBranch->branchName_.data()));
  }
}

void MarkovChainIntegrator::updateMonitorFile()
{
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    double x_i = x_[iDimension];
    branchValues_[iDimension] = x_i;
  }
  
  for ( std::vector<monitorElementType>::iterator extraMonitorBranch = extraMonitorBranches_.begin();
	extraMonitorBranch != extraMonitorBranches_.end(); ++extraMonitorBranch ) {
    extraMonitorBranch->branchValue_ = (*extraMonitorBranch->f_)(x_);
  }

  monitorTree_->Fill();
}

void MarkovChainIntegrator::closeMonitorFile()
{
  monitorFile_->cd();

  monitorTree_->Write();

  delete monitorTree_;
  monitorTree_ = 0;

  delete monitorFile_;
  monitorFile_ = 0;
}
