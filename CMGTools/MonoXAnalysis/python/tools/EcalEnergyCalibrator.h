#ifndef EcalEnergyCalibrator_h
#define EcalEnergyCalibrator_h

#include <TRandom3.h>

class EcalEnergyCalibrator {
    public:
        // dummy constructor for persistence
        EcalEnergyCalibrator() {}

        // further configuration will be added when we will learn how it will work
        EcalEnergyCalibrator(bool isMC, bool synchronization) ;
        ~EcalEnergyCalibrator() ;
    
        /// Correct this electron. 
        double calibrate(double energy, double aeta, double r9, unsigned int seed) ;

    protected:    
        // whatever data will be needed
        bool isMC_, synchronization_;
        TRandom3 *rng_;
   
        /// If synchronization is set to true, it returns a fixed number (1.0)
        double gauss(unsigned int seed) const ;
};

#endif
