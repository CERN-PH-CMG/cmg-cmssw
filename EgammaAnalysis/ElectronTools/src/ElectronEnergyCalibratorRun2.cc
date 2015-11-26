#include "EgammaAnalysis/ElectronTools/interface/ElectronEnergyCalibratorRun2.h"
#include <CLHEP/Random/RandGaussQ.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/Exception.h"

ElectronEnergyCalibratorRun2::ElectronEnergyCalibratorRun2(EpCombinationTool &combinator, bool isMC, bool synchronization) :
    epCombinationTool_(&combinator),
    isMC_(isMC), synchronization_(synchronization),
    rng_(0)
{
}

ElectronEnergyCalibratorRun2::~ElectronEnergyCalibratorRun2()
{
}

void ElectronEnergyCalibratorRun2::initPrivateRng(TRandom *rnd) 
{
     rng_ = rnd;   
}

void ElectronEnergyCalibratorRun2::calibrate(reco::GsfElectron &electron, unsigned int runNumber, edm::StreamID const &id) const
{
    SimpleElectron simple(electron, runNumber, isMC_);
    calibrate(simple, id);
    simple.writeTo(electron);
}
void ElectronEnergyCalibratorRun2::calibrate(SimpleElectron &electron, edm::StreamID const & id) const 
{
    assert(isMC_ == electron.isMC());
    float smear = 0.0, scale = 1.0;
    float aeta = std::abs(electron.getEta()), r9 = electron.getR9();
    bool bad = (r9 < 0.94), gold = !bad;
    // FIXME replace hard-coded numbers below with something dependent on a real payload
    if      (0.0    <= aeta && aeta < 1.0    && bad ) { smear = 0.013654; scale = 0.99544; }
    else if (0.0    <= aeta && aeta < 1.0    && gold) { smear = 0.014142; scale = 0.99882; }
    else if (1.0    <= aeta && aeta < 1.4442 && bad ) { smear = 0.020859; scale = 0.99662; }
    else if (1.0    <= aeta && aeta < 1.4442 && gold) { smear = 0.017120; scale = 1.0065;  }
    else if (1.566  <= aeta && aeta < 2.0    && bad ) { smear = 0.028083; scale = 0.98633; }
    else if (1.566  <= aeta && aeta < 2.0    && gold) { smear = 0.027289; scale = 0.99536; }
    else if (2.0    <= aeta && aeta < 2.5    && bad ) { smear = 0.031793; scale = 0.97859; }
    else if (2.0    <= aeta && aeta < 2.5    && gold) { smear = 0.030831; scale = 0.98567; }
    /// use the 1.566-2.0 also for the 1.4442-1.566 area, in the lack of a new correction
    else if (1.4442 <= aeta && aeta < 1.566  && bad ) { smear = 0.028083; scale = 0.98633; } 
    else if (1.4442 <= aeta && aeta < 1.566  && gold) { smear = 0.027289; scale = 0.99536; } 
    double newEcalEnergy, newEcalEnergyError;
    if (isMC_) {
        double corr = 1.0 + smear * gauss(id);
        newEcalEnergy      = electron.getNewEnergy() * corr;
        newEcalEnergyError = std::hypot(electron.getNewEnergyError() * corr, smear * newEcalEnergy);
    } else {
        newEcalEnergy      = electron.getNewEnergy() / scale;
        newEcalEnergyError = std::hypot(electron.getNewEnergyError() / scale, smear * newEcalEnergy);
    }
    electron.setNewEnergy(newEcalEnergy); 
    electron.setNewEnergyError(newEcalEnergyError); 
    epCombinationTool_->combine(electron);
}

double ElectronEnergyCalibratorRun2::gauss(edm::StreamID const& id) const 
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
        CLHEP::RandGaussQ gaussDistribution(rng->getEngine(id), 0.0, 1.0);
        return gaussDistribution.fire();
    }
}

