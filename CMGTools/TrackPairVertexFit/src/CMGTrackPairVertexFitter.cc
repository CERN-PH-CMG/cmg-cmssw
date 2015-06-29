#include "CMGTools/TrackPairVertexFit/interface/CMGTrackPairVertexFitter.h"


CMGTrackPairVertexFitter::CMGTrackPairVertexFitter() {
  t1= new TLorentzVector(0.0,0.0,1e-12,1e-12);
  t2= new TLorentzVector(0.0,0.0,1e-12,1e-12);

}


CMGTrackPairVertexFitter::~CMGTrackPairVertexFitter() {
  if (t1)
  delete t1;
  if (t2)
  delete t2;
}




bool CMGTrackPairVertexFitter::fit(const reco::TrackRef& pos, const reco::TrackRef& neg,const edm::EventSetup& iSetup) {
  using namespace reco;

  edm::ESHandle<TransientTrackBuilder> theB;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
  
  TransientTrack plusTrack = theB->build(pos);
  TransientTrack minusTrack = theB->build(neg);

  KinematicParticleFactoryFromTransientTrack pFactory;
  ParticleMass muon_mass = 0.1056583;
  float muon_sigma = 0.0000001;
  std::vector<RefCountedKinematicParticle> muonParticles;
  float chi = 0.;
  float ndf = 0.;

  muonParticles.push_back(pFactory.particle (plusTrack,muon_mass,chi,ndf,muon_sigma));
  muonParticles.push_back(pFactory.particle (minusTrack,muon_mass,chi,ndf,muon_sigma));
  
  KinematicParticleVertexFitter fitter;
  RefCountedKinematicTree vertexFitTree = fitter.fit(muonParticles);

  if (!vertexFitTree->isValid())
  return false;

  vertexFitTree->movePointerToTheTop();

  std::vector< RefCountedKinematicParticle > children = vertexFitTree->finalStateParticles();
  for (unsigned int i=0;i<children.size();++i) {
    const RefCountedKinematicParticle& particle = children[i];
    if (particle->currentState().particleCharge()>0) {
      t1->SetPxPyPzE(particle->currentState().globalMomentum().x(),
      particle->currentState().globalMomentum().y(),
      particle->currentState().globalMomentum().z(),
      sqrt(particle->currentState().globalMomentum().mag2()+muon_mass*muon_mass));
    }
    else {
      t2->SetPxPyPzE(particle->currentState().globalMomentum().x(),
      particle->currentState().globalMomentum().y(),
      particle->currentState().globalMomentum().z(),
      sqrt(particle->currentState().globalMomentum().mag2()+muon_mass*muon_mass));
    }


  }      


  return true;
}


