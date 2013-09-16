#include <iostream>

#include "TMath.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/LikelihoodFunctions.h"

using namespace SVfit_namespace;

double 
probMET(double dMETX, double dMETY, double covDet, const TMatrixD& covInv, double power, bool verbose)
{
  double nll = 0.;
  if( covDet != 0. ){
    nll = TMath::Log(2*TMath::Pi()) + 0.5*TMath::Log(TMath::Abs(covDet)) 
         + 0.5*(dMETX*(covInv(0,0)*dMETX + covInv(0,1)*dMETY) + dMETY*(covInv(1,0)*dMETX + covInv(1,1)*dMETY));
  } else {
    nll = std::numeric_limits<float>::max();
  }
  double prob = TMath::Exp(-power*nll);
  if ( verbose ) {
    std::cout << " >> [single likelihhod term probMET] prob = " << prob << " (power = " << power << ")" << std::endl;
  }
  return prob; 
}

double 
probTauToLepPhaseSpace(double decayAngle, double nunuMass, double visMass, double x, bool applySinTheta, bool verbose)
{
  double nuMass2 = nunuMass*nunuMass;
  // protect against rounding errors that may lead to negative masses
  if ( nunuMass < 0. ) nunuMass = 0.; 
  double prob = 0.;
  if ( nunuMass < TMath::Sqrt((1. - x)*tauLeptonMass2) ) { // LB: physical solution
    prob = (13./tauLeptonMass4)*(tauLeptonMass2 - nuMass2)*(tauLeptonMass2 + 2.*nuMass2)*nunuMass;
  } else {    
    double nuMass_limit  = TMath::Sqrt((1. - x)*tauLeptonMass2);
    double nuMass2_limit = nuMass_limit*nuMass_limit;
    prob = (13./tauLeptonMass4)*(tauLeptonMass2 - nuMass2_limit)*(tauLeptonMass2 + 2.*nuMass2_limit)*nuMass_limit;
    prob /= (1. + 1.e+6*TMath::Power(nunuMass - nuMass_limit, 2));
  }
  if ( applySinTheta ) prob *= (0.5*TMath::Sin(decayAngle));
  if ( verbose ) {
    std::cout << " >> [single likelihood term probTauToLep] prob = " << prob << std::endl;
  }
  return prob;
}

double 
probTauToHadPhaseSpace(double decayAngle, double nunuMass, double visMass, double x, bool applySinTheta, bool verbose)
{
  double Pvis_rf = SVfit_namespace::pVisRestFrame(visMass, nunuMass, SVfit_namespace::tauLeptonMass);
  double visMass2 = visMass*visMass;
  double prob = tauLeptonMass/(2.*Pvis_rf);
  if ( x < (visMass2/tauLeptonMass2) ) {
    double x_limit = visMass2/tauLeptonMass2;
    prob /= (1. + 1.e+6*TMath::Power(x - x_limit, 2));
  } else if ( x > 1. ) {
    double visEnFracX_limit = 1.;
    prob /= (1. + 1.e+6*TMath::Power(x - visEnFracX_limit, 2));
  }
  if ( applySinTheta ) prob *= (0.5*TMath::Sin(decayAngle));
  if ( verbose ) {
    std::cout << " >> [single likelihood term probTauToHad] prob = " << prob << std::endl;
  }
  return prob;
}
