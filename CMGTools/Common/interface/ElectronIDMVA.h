//--------------------------------------------------------------------------------------------------
// $Id $
//
// ElectronIDMVA
//
// Helper Class for applying MVA electron ID selection
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef HIGGSANALYSIS_ElectronIDMVA_H
#define HIGGSANALYSIS_ElectronIDMVA_H

#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

class ElectronIDMVA {
  public:
    ElectronIDMVA();
    ~ElectronIDMVA(); 

    enum MVAType {
      kBaseline = 0,      // SigmaIEtaIEta, DEtaIn, DPhiIn, FBrem, SigmaIPhiIPhi, NBrem, 
                          // OneOverEMinusOneOverP
      kNoIPInfo,          // kBaseline + EOverP, ESeedClusterOverPout, ESeedClusterOverPIn
      kWithIPInfo         // kV2 + d0 , IP3d, IP3dSig
    };

    void   Initialize(std::string methodName,
                      std::string Subdet0Pt10To20Weights , 
                      std::string Subdet1Pt10To20Weights , 
                      std::string Subdet2Pt10To20Weights,
                      std::string Subdet0Pt20ToInfWeights, 
                      std::string Subdet1Pt20ToInfWeights, 
                      std::string Subdet2Pt20ToInfWeights,
                      ElectronIDMVA::MVAType type );
    Bool_t IsInitialized() const { return fIsInitialized; }
    
    double MVAValue(const reco::GsfElectron *ele, const reco::Vertex vertex, 
                    EcalClusterLazyTools myEcalCluster,
                    const TransientTrackBuilder *transientTrackBuilder);
    double MVAValue(const reco::GsfElectron *ele, 
                    EcalClusterLazyTools myEcalCluster);
    double MVAValue(double ElePt , double EleSCEta,
                    double EleSigmaIEtaIEta,
                    double EleDEtaIn,
                    double EleDPhiIn,
                    double EleHoverE,
                    double EleD0,
                    double EleFBrem,
                    double EleEOverP,
                    double EleESeedClusterOverPout,
                    double EleSigmaIPhiIPhi,
                    double EleNBrem,
                    double EleOneOverEMinusOneOverP,
                    double EleESeedClusterOverPIn,
                    double EleIP3d,
                    double EleIP3dSig );



  protected:
    TMVA::Reader             *fTMVAReader[6];
    std::string               fMethodname;
    MVAType                   fMVAType;
    
    Bool_t                    fIsInitialized;
    
    Float_t                   fMVAVar_EleSigmaIEtaIEta; 
    Float_t                   fMVAVar_EleDEtaIn; 
    Float_t                   fMVAVar_EleDPhiIn; 
    Float_t                   fMVAVar_EleHoverE; 
    Float_t                   fMVAVar_EleD0; 
    Float_t                   fMVAVar_EleFBrem; 
    Float_t                   fMVAVar_EleEOverP; 
    Float_t                   fMVAVar_EleESeedClusterOverPout; 
    Float_t                   fMVAVar_EleSigmaIPhiIPhi; 
    Float_t                   fMVAVar_EleNBrem; 
    Float_t                   fMVAVar_EleOneOverEMinusOneOverP; 
    Float_t                   fMVAVar_EleESeedClusterOverPIn; 
    Float_t                   fMVAVar_EleIP3d; 
    Float_t                   fMVAVar_EleIP3dSig; 
    
};

#endif
