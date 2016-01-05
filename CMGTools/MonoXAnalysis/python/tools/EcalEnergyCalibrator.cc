#include "EcalEnergyCalibrator.h"
#include <iostream>

EcalEnergyCalibrator::EcalEnergyCalibrator(bool isMC, bool synchronization) :
    isMC_(isMC), synchronization_(synchronization)
{
  rng_ = new TRandom3();
}

EcalEnergyCalibrator::~EcalEnergyCalibrator()
{
}

double EcalEnergyCalibrator::calibrate(double energy, double eta, double r9, unsigned int seed)
{
    float smear = 0.0, scale = 1.0;
    bool bad = (r9 < 0.94), gold = !bad;
    double aeta=fabs(eta);
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
    double newEcalEnergy;
    if (isMC_) {
        double corr = 1.0 + smear * gauss(seed);
        newEcalEnergy      = energy * corr;
    } else {
        newEcalEnergy      = energy / scale;
    }
    return newEcalEnergy;
}

double EcalEnergyCalibrator::gauss(unsigned int seed) const 
{
    if (synchronization_) return 1.0;
    rng_->SetSeed(seed);
    return rng_->Gaus();
}

