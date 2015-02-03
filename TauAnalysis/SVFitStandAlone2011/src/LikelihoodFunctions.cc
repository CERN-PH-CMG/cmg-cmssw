#include <iostream>

#include "TauAnalysis/SVFitStandAlone2011/interface/svFitAuxFunctions.h"
#include "TauAnalysis/SVFitStandAlone2011/interface/LikelihoodFunctions.h"


double 
nllMET(double dMETX, double dMETY, double covDet, const TMatrixD& covInv, double power, bool verbose)
{
  double nll = 0.;
  if( covDet != 0. ){
    nll = TMath::Log(2*TMath::Pi()) + 0.5*TMath::Log(TMath::Abs(covDet)) + 0.5*(dMETX*(covInv(0,0)*dMETX + covInv(0,1)*dMETY)+dMETY*(covInv(1,0)*dMETX + covInv(1,1)*dMETY));
  } 
  else{
    nll = std::numeric_limits<float>::max();
  }
  if( verbose ){
    std::cout << " >> [single likelihhod term nllMET     ] nll = " << nll << " (power = " << power << ")" << std::endl;
  }
  return power*nll; 
}

double 
nllTauToLepPhaseSpace(double decayAngle, double nunuMass, double visMass, bool verbose)
{
  // protect against rounding errors that may lead to negative masses
  if( nunuMass < 0. ){ nunuMass = 0.; }

  double prob = 0.5*nunuMass*TMath::Sqrt((SVfit_namespace::tauLeptonMass2 - SVfit_namespace::square(nunuMass + visMass))*(SVfit_namespace::tauLeptonMass2 - SVfit_namespace::square(nunuMass - visMass)))/(2*SVfit_namespace::tauLeptonMass)*TMath::Sin(decayAngle);
  double nll = 0.;
  if( prob > 0. ){
    nll = -TMath::Log(prob);
  } 
  else{
    nll = std::numeric_limits<float>::max();
  }
  if( verbose ){
    std::cout << " >> [single likelihood term nllTauToLep] nll = " << nll << std::endl;
  }
  return nll;
}

double 
nllTauToHadPhaseSpace(double decayAngle, bool verbose)
{
  double prob = TMath::Sin(decayAngle);
  double nll = 0.;
  if( prob > 0. ){
    nll = -TMath::Log(prob);
  } 
  else{
    nll = std::numeric_limits<float>::max();
  }
  if( verbose ){
    std::cout << " >> [single likelihood term nllTauToHad] nll = " << nll << std::endl;
  }
  return nll;
}
