#ifndef TauAnalysis_SVFitStandAlone_LikelihoodFunctions_h
#define TauAnalysis_SVFitStandAlone_LikelihoodFunctions_h

#include "TMatrixD.h"

/**
   \class   nllMET LikelihoodFunctions.h "TauAnalysis/SVFitStandAlone/interface/LikelihoodFunctions.h"
   
   \brief   Negative log likelihood for MET

   Negative log likelihood for MET. Input parameters are:

    pMETX  : difference between reconstructed MET and fitted MET in x
    pMETY  : difference between reconstructed MET and fitted MET in y
    covDet : determinant of the covariance matrix of the measured MET
    covInv : 2-dim inverted covariance matrix for measured MET (to be 
             determined from MET significance algorithm)
    power  : additional power to enhance the nll term
*/
double nllMET(double dMETX, double dMETY, double covDet, const TMatrixD& covInv, double power=1., bool verbose=false);

/**
   \class   nllTauToLepPhaseSpace LikelihoodFunctions.h "TauAnalysis/SVFitStandAlone/interface/LikelihoodFunctions.h"
   
   \brief   Negative log likelihood for a three body tau decay into lepton, neutrino, neutrino.

   Negative log likelihood for a three body tau decay into lepton, neutrino, neutrino. Input parameters are:

   \var decayAngle : decay angle in the restframe of the tau lepton decay
   \var visMass : measured visible mass
   \var nunuMass : fitted neutrino mass

   The parameter tauLeptonMass2 is the mass of the tau lepton squared as defined in svFitAuxFunctions.h   
*/
double nllTauToLepPhaseSpace(double decayAngle, double nunuMass, double visMass, bool verbose=false);

/**
   \class   nllTauToHadPhaseSpace LikelihoodFunctions.h "TauAnalysis/SVFitStandAlone/interface/LikelihoodFunctions.h"
   
   \brief   Negative log likelihood for a two body tau decay into two hadrons

   Negative log likelihood for a two body tau decay into two hadrons. Input parameters is:

    decayAngle : decay angle in the restframe of the tau lepton decay
*/
double nllTauToHadPhaseSpace(double decayAngle, bool verbose=false);

#endif
