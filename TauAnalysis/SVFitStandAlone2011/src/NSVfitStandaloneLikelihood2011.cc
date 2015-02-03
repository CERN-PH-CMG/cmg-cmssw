#include "TauAnalysis/SVFitStandAlone2011/interface/svFitAuxFunctions.h"
#include "TauAnalysis/SVFitStandAlone2011/interface/LikelihoodFunctions.h"
#include "TauAnalysis/SVFitStandAlone2011/interface/NSVfitStandaloneLikelihood2011.h"

using namespace NSVfitStandalone2011;

const NSVfitStandaloneLikelihood2011* NSVfitStandaloneLikelihood2011::gNSVfitStandaloneLikelihood2011 = 0;

///Jose: made TMatrixD a const pointer, it was being modified inside !
NSVfitStandaloneLikelihood2011::NSVfitStandaloneLikelihood2011(std::vector<NSVfitStandalone2011::MeasuredTauLepton2011> measuredTauLeptons,  Vector measuredMET, const TMatrixD * covMET, bool verbose) : 
  metPower_(1.), addLogM_(true), verbose_(verbose), idxObjFunctionCall_(0), invCovMET_(2, 2)
{
  if(verbose_){
    std::cout << "<NSVfitStandaloneLikelihood2011::constructor>" << std::endl;
  }
  measuredMET_ = measuredMET;
  measuredTauLeptons_ = measuredTauLeptons;
  if(measuredTauLeptons_.size()!=2){
    std::cout << " >> ERROR : the numer of measured leptons must be 2 but is found to be: " << measuredTauLeptons_.size() << std::endl;
    assert(0);
  }

//   // determine transfer matrix for MET
//   covDet_=covMET.Determinant();
//   if(covDet_!=0){ 
//     invCovMET_ = covMET.Invert(); 
//   }

  //Jose: Why is the above code modifying the imput matrix? should copy first then invert 
  if(!covMET){ 
    std::cout << " >> ERROR:  MET covariance Matrix is NULL" << std::endl;
    assert(0);
  }
  invCovMET_[0][0]=(*covMET)[0][0];
  invCovMET_[0][1]=(*covMET)[0][1];
  invCovMET_[1][0]=(*covMET)[1][0];
  invCovMET_[1][1]=(*covMET)[1][1];
  covDet_=invCovMET_.Determinant();
  if(covDet_!=0){ 
    invCovMET_.Invert(); 
  }
  else{
    std::cout << " >> ERROR: cannot invert MET covariance Matrix (det=0)." << std::endl;
    assert(0);
  }
  // set global function pointer to this
  gNSVfitStandaloneLikelihood2011 = this;
}

double 
NSVfitStandaloneLikelihood2011::nll(const double* xPrime, double phiPenalty) const
{
  if( verbose_ ){
    std::cout << "<NSVfitStandaloneLikelihood2011:nll(const double*, double)> ..." << std::endl;
  }
  // map decay-wise parameters for simplicity
  std::vector<double> nunuMass  ; nunuMass  .push_back(xPrime[kNuNuMass1  ]); nunuMass  .push_back(xPrime[kNuNuMass2  ]);
  std::vector<double> visMass   ; visMass   .push_back(xPrime[kVisMass1   ]); visMass   .push_back(xPrime[kVisMass2   ]); 
  std::vector<double> decayAngle; decayAngle.push_back(xPrime[kDecayAngle1]); decayAngle.push_back(xPrime[kDecayAngle2]);

  double nll=0;
  // start the combined likelihood construction from MET
  nll=nllMET(xPrime[kDMETx], xPrime[kDMETy], covDet_, invCovMET_, metPower_, verbose_);
  // add likelihoods for the decay branches
  for(unsigned int idx=0; idx<measuredTauLeptons_.size(); ++idx){
    switch(measuredTauLeptons_[idx].decayType()){
    case kHadDecay :
      nll+=nllTauToHadPhaseSpace(decayAngle[idx], verbose_); break;
    case kLepDecay :
      nll+=nllTauToLepPhaseSpace(decayAngle[idx], nunuMass[idx], visMass[idx], verbose_); break;
    }
  }
  // add additional logM term if configured such 
  if(addLogM_){
    nll+=TMath::Log(xPrime[kMTauTau]);
  }
  if( verbose_ ){
    if(addLogM_){
      std::cout << " >> [single likelihood term nllLogM    ] nll = " << TMath::Log(xPrime[kMTauTau]) << std::endl;
    }
    std::cout   << " >> [penalty term on phi               ] nll = " << phiPenalty << std::endl;
    std::cout   << " >> [combined nll term                 ] nll = " << nll << std::endl;
  }
  // add additional phiPenalty in case kPhi in the fit parameters 
  // (kFitParams) trespassed the physical boundaries from +/-pi 
  return TMath::IsNaN(nll) ? std::numeric_limits<float>::max() : (nll+phiPenalty);
}

const double*
NSVfitStandaloneLikelihood2011::transform(double* xPrime, const double* x) const
{
  if( verbose_ ){
    std::cout << "<NSVfitStandaloneLikelihood2011:transform(double*, const double*)>" << std::endl;
  }
  LorentzVector fittedDiTauSystem;
  for(unsigned int idx=0; idx<measuredTauLeptons_.size(); ++idx){
    // map to local variables to be more clear on the meaning of the individual parameters. The fit parameters are ayered 
    // for each tau decay
    double nunuMass       = x[ idx*kMaxFitParams + kMNuNu ];       // nunu inv mass (can be const 0 for had tau decays) 
    double labframeXFrac  = x[ idx*kMaxFitParams + kXFrac ];       // visible energy fraction x in labframe
    double labframePhi    = x[ idx*kMaxFitParams + kPhi   ];       // phi in labframe 
    double labframeVisMom = measuredTauLeptons_[ idx ].momentum(); // visible momentum in lab-frame
    double labframeVisEn  = measuredTauLeptons_[ idx ].energy();   // visible energy in lab-frame
    double visMass        = measuredTauLeptons_[ idx ].mass();     // vis mass

    // add protection against zero mass for visMass. If visMass is lower than the electron mass, set it
    // to the electron mass
    if(visMass<5.1e-4) { visMass = 5.1e-4; }    
    // momentum of visible decay products in tau lepton restframe
    double restframeVisMom     = SVfit_namespace::pVisRestFrame(visMass, nunuMass);
    // tau lepton decay angle in tau lepton restframe (as function of the energy ratio of visible decay products/tau lepton energy)
    double restframeDecayAngle = SVfit_namespace::gjAngleFromX(labframeXFrac, visMass, restframeVisMom, labframeVisEn);
    // tau lepton decay angle in labframe
    double labframeDecayAngle  = SVfit_namespace::gjAngleToLabFrame(restframeVisMom, restframeDecayAngle, labframeVisMom);
    // tau lepton momentum in labframe
    double labframeTauMom      = SVfit_namespace::tauMomentumLabFrame(visMass, restframeVisMom, restframeDecayAngle, labframeVisMom);
    Vector labframeTauDir      = SVfit_namespace::tauDirection(measuredTauLeptons_[idx].direction(), labframeDecayAngle, labframePhi).unit();
    // tau lepton four vector in labframe
    fittedDiTauSystem += SVfit_namespace::tauP4(labframeTauDir, labframeTauMom);
    // fill branch-wise nll parameters
    xPrime[ idx==0 ? kNuNuMass1   : kNuNuMass2   ] = nunuMass;
    xPrime[ idx==0 ? kVisMass1    : kVisMass2    ] = visMass;
    xPrime[ idx==0 ? kDecayAngle1 : kDecayAngle2 ] = restframeDecayAngle;
  }
  // determine fittedMET
  // subtract the visible part from it. The remainder is the pure neutrino part. Minus the 
  // the remainder is the estimate of the fittedMET
  //Vector fittedMom(fittedDiTauSystem.px(), fittedDiTauSystem.py(), fittedDiTauSystem.pz());
  //Vector fittedMET = -(fittedMom - (measuredTauLeptons_[0].p()+measuredTauLeptons_[1].p())); 
  /*
    I believe that the following line contains a sign bug in the official version of SVfit:
    http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h?revision=1.11&view=markup
    reco::Candidate::LorentzVector dp4MEt_fitted() const { return (p4MEt_ - dp4_); } // should be +

    Reasoning:
    from 
    reco::Candidate::LorentzVector p4_fitted() const { return (p4_ + dp4_); }
    dp4_ should be the neutrino momentum --> the neutrino MET would by -dp4_ --> the residual of the measured MET and the fitted MET shyould be 
    p4MEt - fittedMEt = p4MEt + dp4_
  */
  Vector fittedMET = fittedDiTauSystem.Vect() - (measuredTauLeptons_[0].p()+measuredTauLeptons_[1].p()); 
  // fill event-wise nll parameters
  xPrime[ kDMETx   ] = measuredMET_.x() - fittedMET.x(); 
  xPrime[ kDMETy   ] = measuredMET_.y() - fittedMET.y();
  xPrime[ kMTauTau ] = fittedDiTauSystem.mass();

  if( verbose_ ){
    std::cout << " >> input values for transformed variables: " << std::endl;
    std::cout << "    MET[x] = " <<  fittedMET.x() << " (fitted)  " << measuredMET_.x() << " (measured) " << std::endl; 
    std::cout << "    MET[y] = " <<  fittedMET.y() << " (fitted)  " << measuredMET_.y() << " (measured) " << std::endl; 
    std::cout << "    fittedDiTauSystem: [" 
	      << " px = " << fittedDiTauSystem.px() 
	      << " py = " << fittedDiTauSystem.py() 
	      << " pz = " << fittedDiTauSystem.pz() 
	      << " En = " << fittedDiTauSystem.energy() 
	      << " ]" << std::endl; 
    std::cout << " >> nll parameters after transformation: " << std::endl;
    std::cout << "    x[kNuNuMass1  ] = " << xPrime[kNuNuMass1  ] << std::endl;
    std::cout << "    x[kVisMass1   ] = " << xPrime[kVisMass1   ] << std::endl;
    std::cout << "    x[kDecayAngle1] = " << xPrime[kDecayAngle1] << std::endl;
    std::cout << "    x[kNuNuMass2  ] = " << xPrime[kNuNuMass2  ] << std::endl;
    std::cout << "    x[kVisMass2   ] = " << xPrime[kVisMass2   ] << std::endl;
    std::cout << "    x[kDecayAngle2] = " << xPrime[kDecayAngle2] << std::endl;
    std::cout << "    x[kDMETx      ] = " << xPrime[kDMETx      ] << std::endl;
    std::cout << "    x[kDMETy      ] = " << xPrime[kDMETy      ] << std::endl;
    std::cout << "    x[kMTauTau    ] = " << xPrime[kMTauTau    ] << std::endl;
  }
  return xPrime;
}

double
NSVfitStandaloneLikelihood2011::nll(const double* x) const 
{
  if( verbose_ ){
    std::cout << "<NSVfitStandaloneLikelihood2011:nll(const double*)>" << std::endl;
  }
  ++idxObjFunctionCall_;
  if( verbose_ ){
    std::cout << " >> ixdObjFunctionCall : " << idxObjFunctionCall_ << std::endl;  
    std::cout << " >> fit parameters before transformation: " << std::endl;
    std::cout << "    x[kXFrac1] = " << x[                kXFrac] << std::endl;
    std::cout << "    x[kMNuNu1] = " << x[                kMNuNu] << std::endl;
    std::cout << "    x[kPhi1  ] = " << x[                kPhi  ] << std::endl;
    std::cout << "    x[kXFrac2] = " << x[kMaxFitParams + kXFrac] << std::endl;
    std::cout << "    x[kMNuNu2] = " << x[kMaxFitParams + kMNuNu] << std::endl;
    std::cout << "    x[kPhi2  ] = " << x[kMaxFitParams + kPhi  ] << std::endl;
  }
  // prevent kPhi in the fit parameters (kFitParams) from trespassing the 
  // +/-pi boundaries
  double phiPenalty=0;
  for(unsigned int idx=0; idx<measuredTauLeptons_.size(); ++idx){
    if( TMath::Abs(idx*kMaxFitParams+x[kPhi]) > TMath::Pi() ){
      phiPenalty += (TMath::Abs(x[kPhi])-TMath::Pi())*(TMath::Abs(x[kPhi])-TMath::Pi());
    }
  }
  // xPrime are the transformed variables from which to construct the nll
  // transform performs the transformation from the fit parameters x to the 
  // nll parameters xPrime. nllPrime is the actual combined log likelihood
  // phiPenalty prevents the fit to converge to unphysical values beyond
  // +/-phi 
  double xPrime[ kMaxNLLParams ];
  return nll(transform(xPrime, x), phiPenalty);
}

void
NSVfitStandaloneLikelihood2011::results(std::vector<LorentzVector>& fittedTauLeptons, const double* x) const
{
  if( verbose_ ){
    std::cout << "<NSVfitStandaloneLikelihood2011:results(std::vector<LorentzVector>&, const double*)>" << std::endl;
  }
  for(unsigned int idx=0; idx<measuredTauLeptons_.size(); ++idx){
    // map to local variables to be more clear on the meaning of the individual parameters. The fit parameters are ayered 
    // for each tau decay
    double nunuMass       = x[ idx*kMaxFitParams + kMNuNu ];       // nunu inv mass (can be const 0 for had tau decays) 
    double labframeXFrac  = x[ idx*kMaxFitParams + kXFrac ];       // visible energy fraction x in labframe
    double labframePhi    = x[ idx*kMaxFitParams + kPhi   ];       // phi in labframe 
    double labframeVisMom = measuredTauLeptons_[ idx ].momentum(); // visible momentum in lab-frame
    double labframeVisEn  = measuredTauLeptons_[ idx ].energy();   // visible energy in lab-frame
    double visMass        = measuredTauLeptons_[ idx ].mass();     // vis mass
  
    // momentum of visible decay products in tau lepton restframe
    double restframeVisMom     = SVfit_namespace::pVisRestFrame(visMass, nunuMass);
    // tau lepton decay angle in tau lepton restframe (as function of the energy ratio of visible decay products/tau lepton energy)
    double restframeDecayAngle = SVfit_namespace::gjAngleFromX(labframeXFrac, visMass, restframeVisMom, labframeVisEn);
    // tau lepton decay angle in labframe
    double labframeDecayAngle  = SVfit_namespace::gjAngleToLabFrame(restframeVisMom, restframeDecayAngle, labframeVisMom);
    // tau lepton momentum in labframe
    double labframeTauMom      = SVfit_namespace::tauMomentumLabFrame(visMass, restframeVisMom, restframeDecayAngle, labframeVisMom);
    Vector labframeTauDir      = SVfit_namespace::tauDirection(measuredTauLeptons_[idx].direction(), labframeDecayAngle, labframePhi).unit();
    // tau lepton four vector in labframe
    fittedTauLeptons.push_back(SVfit_namespace::tauP4(labframeTauDir, labframeTauMom));
  }
  return;
}
