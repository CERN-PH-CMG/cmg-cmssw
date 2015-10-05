// imported from RecoEgamma/ElectronIdentification/interface/ElectronMVAEstimatorRun2Spring15NonTrig.h and renamed, with modifications to run in FWLite

#ifndef EGammaMvaEleEstimatorSpring15NonTrig_H
#define EGammaMvaEleEstimatorSpring15NonTrig_H

using namespace std;

#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/EventBase.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "EgammaAnalysis/ElectronTools/interface/ElectronEffectiveArea.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "DataFormats/PatCandidates/interface/Electron.h"

using namespace reco;

#include "CondFormats/EgammaObjects/interface/GBRForest.h"

#include <vector>
#include <string>
#include <TROOT.h>
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

class EGammaMvaEleEstimatorSpring15NonTrig {
  
 public:

  // Define here the number and the meaning of the categories
  // for this specific MVA
  const int nCategories = 6;
  enum mvaCategories {
    UNDEFINED = -1,
    CAT_EB1_PT5to10  = 0,
    CAT_EB2_PT5to10  = 1,
    CAT_EE_PT5to10   = 2,
    CAT_EB1_PT10plus = 3,
    CAT_EB2_PT10plus = 4,
    CAT_EE_PT10plus  = 5
  };

  // Define the struct that contains all necessary for MVA variables
  // Note: all variables have to be floats for TMVA Reader, even if 
  // the training was done with ints.
  struct AllVariables {
    // Pure ECAL -> shower shapes
    float see; // 0
    float spp; // 1
    float OneMinusE1x5E5x5; // 2
    float R9; // 3
    float etawidth; // 4
    float phiwidth; // 5
    float HoE; // 6
    // Endcap only variables
    float PreShowerOverRaw; // 7
    //Pure tracking variables
    float kfhits; // 8
    float kfchi2; // 9
    float gsfchi2; // 10
    // Energy matching
    float fbrem; // 11

    float gsfhits; // 12
    float expectedMissingInnerHits; // 13
    float convVtxFitProbability; // 14

    float EoP; // 15
    float eleEoPout; // 16
    float IoEmIoP; // 17
    // Geometrical matchings
    float deta; // 18
    float dphi; // 19
    float detacalo; // 20 
    // Spectator variables  
    float pt; // 21
    float isBarrel; // 22
    float isEndcap; // 23
    float SCeta; // 24
    //
    float eClass; // 25          
    float pfRelIso; // 26         
    float expectedInnerHits; // 27
    float vtxconv; // 28
    float mcEventWeight; // 29
    float mcCBmatchingCategory; // 30
    
  };
  
  // Constructor and destructor
  EGammaMvaEleEstimatorSpring15NonTrig();
  virtual ~EGammaMvaEleEstimatorSpring15NonTrig();
  void initialize(std::string methodName, std::vector<std::string> weightFileNames);

  // Calculation of the MVA value
  // provide fwlite event to pick conversions from reducedEgamma:reducedConversions and beamspot
  float mvaValue( const pat::Electron& particle, const fwlite::EventBase& eventbase, bool printDebug=false) const;
 
  // Utility functions
  std::unique_ptr<const GBRForest> createSingleReader(const int iCategory, 
                                                      const std::string weightFile);

  virtual int getNCategories() const { return nCategories; }
  bool isEndcapCategory( int category ) const;

  // Functions that should work on both pat and reco electrons
  // (use the fact that pat::Electron inherits from reco::GsfElectron)
  std::vector<float> fillMVAVariables(const pat::Electron& particle, const reco::BeamSpot& theBeamSpot, const edm::Handle<reco::ConversionCollection>& conversions) const;
  int findCategory( const pat::Electron& particle) const;
  // The function below ensures that the variables passed to MVA are 
  // within reasonable bounds
  void constrainMVAVariables(AllVariables&) const;

 private:

  // Data members
  std::vector< std::unique_ptr<const GBRForest> > _gbrForests;

  // All variables needed by this MVA
  std::string _MethodName;
  AllVariables _allMVAVars;
  
};

#endif
