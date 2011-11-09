#ifndef TauAnalysis_CandidateTools_NSVfitStandaloneLikelihood_h
#define TauAnalysis_CandidateTools_NSVfitStandaloneLikelihood_h

#include <vector>

#include "TMath.h"
#include "TMatrixD.h"
#include "Math/Minimizer.h"

#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"


namespace NSVfitStandalone{
  /**
     \typedef NSVfitStandalone::Vector
     \brief   spacial momentum vector (equivalent to reco::Candidate::Vector)
  */
  typedef math::XYZVector Vector;
  /**
     \typedef NSVfitStandalone::LorentzVector
     \brief   lorentz vector (equivalent to reco::Candidate::LorentzVector)
  */
  typedef math::XYZTLorentzVector LorentzVector;
  /**
     \enum    NSVfitStandalone::kDecayType
     \brief   enumeration of all tau decay types
  */
  enum kDecayType {
    kHadDecay,       /* < hadronic tau lepton decay                                                        */ 
    kLepDecay        /* < leptonic tau lepton decay                                                        */
  };
  /**
     \enum    NSVfitStandalone::kFitParams
     \brief   enumeration of all parameters used by the NSVfitAlgorithm to reconstruct the di-tau system
  */
  enum kFitParams {
    kXFrac,          /* < relative fraction of the visible energy on the total energy of the tau lepton    */
    kMNuNu,          /* < invariant mass of the neutrino system                                            */
    kPhi,            /* < phi angle of the tau lepton (this is parameter is not constraint by measurement) */
    kMaxFitParams    /* < maximum number of fit parameters per resonance decay branch                      */
  };
  /**
     \enum    NSVfitStandalone::kNLLParams
     \brief   enumeration of all parameters used to construct the combined likelihood including the logM term
  */
  enum kNLLParams {
    kNuNuMass1,      /* < mass of the neutrino system for the first decay branch                           */
    kVisMass1,       /* < mass of the visible parts of the first tau decay branch                          */
    kDecayAngle1,    /* < decay angle for the first decay branch (in restframe of the tau lelpton decay)   */  
    kNuNuMass2,      /* < mass of the neutrino system for the second decay branch                          */
    kVisMass2,       /* < mass of the visible parts of the second tau decay branch                         */
    kDecayAngle2,    /* < decay angle for the second decay branch (in restframe of the tau lelpton decay)  */  
    kDMETx,          /* < difference between the sum of the fitted neutrino px and px of the MET           */ 
    kDMETy,          /* < difference between the sum of the fitted neutrino py and py of the MET           */
    kMTauTau,        /* < invariant mass of the fitted di-tau system (used for the logM penalty term)      */
    kMaxNLLParams    /* < max number of parameters used for for the combined likelihood                    */
  };
  /**
     \class   MeasuredTauLepton NSVfitStandaloneLikelihood.h "TauAnalysis/CandidateTools/interface/NSVfitStandaloneLikelihood.h"
     
     \brief   Helper class to simplify the configuration of the NSVfitStandaloneLikelihood class. 
     
     This is a helper class to facilitate the configuration of the NSVfitStandaloneLikelihood class. It keeps the spacial momentum
     energy and information about the type of tau lepton decay. All information is stored in the lab-frame. A few getter functions 
     facilitate access to the information.
  */
  class MeasuredTauLepton
  {
  public:
    /// constructor from the measured quantities per decay branch
    MeasuredTauLepton(kDecayType type, LorentzVector p4) : decayType_(type), p4_(p4) {};
    /// default destructor
    ~MeasuredTauLepton(){};

    /// return px of the measured tau lepton in labframe
    double px() const { return p4_.px(); };
    /// return py of the measured tau lepton in labframe
    double py() const { return p4_.py(); };
    /// return visible mass in labframe
    double mass() const { return p4_.mass(); };
    /// return visible energy in labframe
    double energy() const { return p4_.energy(); };
    /// return visible momenumt in labframe
    double momentum() const { return p4_.P(); };
    /// return decay type of the reconstructed tau lepton
    unsigned int decayType() const { return decayType_; };
    /// return the spacial momentum vector in the labframe
    Vector p() const { return p4_.Vect(); };
    /// return the lorentz vector in the labframe
    LorentzVector p4() const { return p4_; };
    /// return the direction of the visible 
    Vector direction() const { return p4_.Vect().unit(); };
    
  private:
    /// decay type
    kDecayType decayType_;
    /// visible momentum in labframe 
    LorentzVector p4_;
  };

  /**
     \class   NSVfitStandaloneLikelihood NSVfitStandaloneLikelihood.h "TauAnalysis/CandidateTools/interface/NSVfitStandaloneLikelihood.h"
     
     \brief   Negative log likelihood for a resonance decay into two tau leptons that may themselves decay hadronically or leptonically
     
     Negative log likelihood for a resonance decay into two tau leptons that may themselves decay hadronically or leptonically 
     Depending on the configuration during object creation it will be a combination of MET, TauToHad, TauToLep and an additional
     penalty term to suppress tails in m(tau,tau) (logM). Configurables during creation time are:
     
     \var measuredTauLeptons : the vector of the two reconstructed tau leptons
     \var measuredMET : the spacial vector of the measured MET
     \var covMET : the covariance matrix of the MET (as determined from the MEt significance for instance)
     \verbose : indicating the verbosity level

     Additional optional values may be set before the actual fit is performed. During construction the class is initialized with 
     default values as indicated in braces in the following:

     \var metPower : indicating an additional power to enhance the MET likelihood (default is 1.)
     \var addLogM : specifying whether to use the LogM penalty term or not (default is true)     

     A typical way to obtain the covariance matrix of the MET is to follow the MET significance algorithm as provided by RecoMET.
     The NSVfitStandaloneLikelihood class is for internal use only. It is interfaced from the NSVfitStandaloneAlgorithm class as 
     defined in interface/NSVfitStandaloneAlgorithm.h in the same package. It keeps all necessary information to calculate the 
     combined likelihood but does not perform any fit. It is interfaced to the ROOT minuit minimization package via the global
     function pointer gNSVfitStandaloneLikelihood as defined in src/NSVfitStandaloneLikelihood.cc in the same package and 
     initialized in the function NSVfitStandaloneLikelihood::fnc(const double*). Per default the LogM term is added.
  */
  class NSVfitStandaloneLikelihood {
  public:
    /// constructor with a minimla set of configurables 
    NSVfitStandaloneLikelihood(std::vector<MeasuredTauLepton> measuredTauLeptons, Vector measuredMET, TMatrixD& covMET, bool verbose);
    /// default destructor
    ~NSVfitStandaloneLikelihood() {};
    /// static pointer to this (needed for the minuit function calls)
    static const NSVfitStandaloneLikelihood* gNSVfitStandaloneLikelihood;

    /// add an additional logM(tau,tau) term to the nll to suppress tails on M(tau,tau) (default is true)
    void addLogM(bool value) { addLogM_=value; };
    /// modify the MET term in the nll by an additional power (default is 1.)
    void metPower(double value) { metPower_=value; };    

    /// return vector of measured MET
    Vector measuredMET() const { return measuredMET_; };
    /// return vector of measured tau leptons
    std::vector<MeasuredTauLepton> measuredTauLeptons() const { return measuredTauLeptons_; };
    /// return vector of fitted tau leptons, which will be the actual fit result. This function is a subset of transform.
    /// It needs to be factored out though as transform has to be const to be usable by minuit and therefore is not allowed 
    /// changfe the class memebers.  
    void results(std::vector<LorentzVector>& fittedTauLeptons, const double* x) const;
    /// fit function to be called from outside (has to be const to be usable by minuit). This function will call the actual 
    /// functions transform and nll internally 
    double nll(const double* x) const;    

  private:
    /// transformation from x to xPrime, x are the actual fit parameters, xPrime are the transformed parameters that go into 
    /// the nll (has to be const to be usable by minuit)
    const double* transform(double* xPrime, const double* x) const;
    /// combined likelihood function (has to be const to be usable by minuit). The additional boolean phiPenalty is added to 
    /// prevent singularities at the +/-pi boundaries of kPhi within the fit parameters (kFitParams) 
    double nll(const double* xPrime, double phiPenalty) const;

  private:
    /// additional power to enhance MET term in the nll (default is 1.)
    double metPower_;
    /// add a logM penalty term in the nll
    bool addLogM_;
    /// verbosity level
    bool verbose_;
    /// monitor the number of function calls
    mutable unsigned int idxObjFunctionCall_;

    /// measured tau leptons
    std::vector<MeasuredTauLepton> measuredTauLeptons_;
    /// measured MET
    Vector measuredMET_;
    /// transfer matrix for MET in (inverse covariance matrix) 
    TMatrixD invCovMET_;
    /// determinant of the covariance matrix of MET
    double covDet_;
  };
}

#endif
