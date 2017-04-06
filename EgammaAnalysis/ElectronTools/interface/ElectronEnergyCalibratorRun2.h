#ifndef ElectronEnergyCalibratorRun2_h
#define ElectronEnergyCalibratorRun2_h

#include <TRandom.h>
#include "EgammaAnalysis/ElectronTools/interface/EnergyScaleCorrection_class.h"
#include "EgammaAnalysis/ElectronTools/interface/SimpleElectron.h"
#include "EgammaAnalysis/ElectronTools/interface/EpCombinationTool.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"

#include <vector>

class ElectronEnergyCalibratorRun2 {
 public:
  // dummy constructor for persistence
  ElectronEnergyCalibratorRun2() {}
  
  // constructor w/o combinator: do not combine E-p: intented for residual corrections
  ElectronEnergyCalibratorRun2(bool isMC, bool synchronization, std::string); 
  // constructor with combinator: do E-p combination after E corrections
  ElectronEnergyCalibratorRun2(EpCombinationTool &combinator, bool isMC, bool synchronization, std::string); 
  ~ElectronEnergyCalibratorRun2() ;
  
  /// Initialize with a random number generator (if not done, it will use the CMSSW service)
  /// Caller code owns the TRandom.
  void initPrivateRng(TRandom *rnd) ;
  
  /// Correct this electron. 
  void calibrate(SimpleElectron &electron) const ;
  
 protected:    
  // whatever data will be needed
  EpCombinationTool *epCombinationTool_;
  bool isMC_, synchronization_;
  TRandom *rng_;

  /// Return a number distributed as a unit gaussian, drawn from the private RNG if initPrivateRng was called, 
  /// or from the CMSSW RandomNumberGenerator service
  /// If synchronization is set to true, it returns a fixed number (1.0)
  double gauss() const ;
  bool doEpCombination_;
  EnergyScaleCorrection_class _correctionRetriever;
};

#endif
