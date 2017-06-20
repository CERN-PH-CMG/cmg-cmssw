#include "EgammaAnalysis/ElectronTools/interface/ElectronEnergyCalibratorRun2.h"
#include <CLHEP/Random/RandGaussQ.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/Exception.h"

ElectronEnergyCalibratorRun2::ElectronEnergyCalibratorRun2(bool isMC, 
							   bool synchronization, 
							   std::string correctionFile
							   ) :
  isMC_(isMC), synchronization_(synchronization),
  rng_(0),
  doEpCombination_(false),
  _correctionRetriever(correctionFile) // here is opening the files and reading the corrections
{
  if(isMC_) {
    _correctionRetriever.doScale = false; 
    _correctionRetriever.doSmearings = true;
  } else {
    _correctionRetriever.doScale = true; 
    _correctionRetriever.doSmearings = false;
  }
}

ElectronEnergyCalibratorRun2::ElectronEnergyCalibratorRun2(EpCombinationTool &combinator, 
							   bool isMC, 
							   bool synchronization, 
							   std::string correctionFile
							   ) :
  epCombinationTool_(&combinator),
  isMC_(isMC), synchronization_(synchronization),
  rng_(0),
  doEpCombination_(true),
  _correctionRetriever(correctionFile) // here is opening the files and reading the corrections
{
  if(isMC_) {
    _correctionRetriever.doScale = false; 
    _correctionRetriever.doSmearings = true;
  } else {
    _correctionRetriever.doScale = true; 
    _correctionRetriever.doSmearings = false;
  }
}

ElectronEnergyCalibratorRun2::~ElectronEnergyCalibratorRun2()
{}

void ElectronEnergyCalibratorRun2::initPrivateRng(TRandom *rnd) 
{
  rng_ = rnd;   
}

void ElectronEnergyCalibratorRun2::calibrate(SimpleElectron &electron) const 
{
  assert(isMC_ == electron.isMC());
  float smear = 0.0, scale = 1.0;
  float aeta = std::abs(electron.getEta()); //, r9 = electron.getR9();
  float et = electron.getNewEnergy()/cosh(aeta);
  
  scale = _correctionRetriever.ScaleCorrection(electron.getRunNumber(), electron.isEB(), electron.getR9(), aeta, et);
  smear = _correctionRetriever.getSmearingSigma(electron.getRunNumber(), electron.isEB(), electron.getR9(), aeta, et, 0., 0.); 
  
  double newEcalEnergy, newEcalEnergyError;
  if (isMC_) {
    double corr = 1.0 + smear * gauss();
    newEcalEnergy      = electron.getNewEnergy() * corr;
    newEcalEnergyError = std::hypot(electron.getNewEnergyError() * corr, smear * newEcalEnergy);
  } else {
    newEcalEnergy      = electron.getNewEnergy() * scale;
    newEcalEnergyError = std::hypot(electron.getNewEnergyError() * scale, smear * newEcalEnergy);
  }
  electron.setNewEnergy(newEcalEnergy); 
  electron.setNewEnergyError(newEcalEnergyError);
  if(doEpCombination_) epCombinationTool_->combine(electron);
}

double ElectronEnergyCalibratorRun2::gauss() const 
{
  if (synchronization_) return 1.0;

  if (rng_) {
    return rng_->Gaus();
  } else {
    edm::Service<edm::RandomNumberGenerator> rng;
    if ( !rng.isAvailable() ) {
      throw cms::Exception("Configuration")
        << "XXXXXXX requires the RandomNumberGeneratorService\n"
        "which is not present in the configuration file.  You must add the service\n"
        "in the configuration file or remove the modules that require it.";
    }
    CLHEP::RandGaussQ gaussDistribution(rng->getEngine(), 0.0, 1.0);
    return gaussDistribution.fire();
  }
}

