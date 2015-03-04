//#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
//#include "DataFormats/Common/interface/Handle.h"
//#include "FWCore/Framework/interface/MakerMacros.h"
//#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
//#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
//#include "MagneticField/Engine/interface/MagneticField.h"
//#include "MagneticField/Records/interface/IdealMagneticField.h"
#include "RecoVertex/KinematicFitPrimitives/interface/ParticleMass.h"
 //#include "RecoVertex/KinematicFitPrimitives/interface/MultiTrackKinematicConstraint.h"
#include <RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h>
// #include "RecoVertex/KinematicFitPrimitives/interface/"
#include "RecoVertex/KinematicFit/interface/KinematicConstrainedVertexFitter.h"
//#include "RecoVertex/KinematicFit/interface/TwoTrackMassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
//#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
//#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"
#include "TLorentzVector.h"



class CMGTrackPairVertexFitter {
 public:
  CMGTrackPairVertexFitter();
  ~CMGTrackPairVertexFitter();

  
  bool fit(const reco::TrackRef& track1,const reco::TrackRef& track2,const edm::EventSetup&);
  
  const TLorentzVector * plus() const { return t1;}
  const TLorentzVector * minus() const { return t2;}

 private:
  TLorentzVector *t1; 
  TLorentzVector *t2; 


};
