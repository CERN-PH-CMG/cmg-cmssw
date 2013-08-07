#ifndef SAKLooseLepton_h
#define SAKLooseLepton_h

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "TMath.h"
#include "Math/VectorUtil.h"

bool isLooseElectron   (const reco::GsfElectron & electron, const math::XYZPoint & primaryVertex, edm::Handle<reco::PFCandidateCollection> pfCandidates);
bool isLooseElectron   (const reco::GsfElectron & electron, const math::XYZPoint & primaryVertex, const reco::PFCandidateCollection & pfCandidates);
bool isLooseElectronID (const reco::GsfElectron & electron, const math::XYZPoint & primaryVertex);
bool isLooseElectronIso(const reco::GsfElectron & electron, const reco::PFCandidateCollection & pfCandidates);
bool isLooseMuon   (const reco::Muon & muon, const math::XYZPoint & primaryVertex, edm::Handle<reco::PFCandidateCollection> pfCandidates);
bool isLooseMuon   (const reco::Muon & muon, const math::XYZPoint & primaryVertex, const reco::PFCandidateCollection& pfCandidates);
bool isLooseMuonID (const reco::Muon & muon, const math::XYZPoint & primaryVertex);
bool isLooseMuonIso(const reco::Muon & muon, const reco::PFCandidateCollection& pfCandidates);

bool passCut(double parameter, double variable, bool discardAbove, int numBins, const double* parameterBins, const double* variableCuts);

/**
  If the directional parameter is set, computes directional isolation, i.e. an
  (angular-)disperson weighted sum of pT of the provided isoParticles. Otherwise
  computes a simple scalar sum of isoParticles. In both cases the pT of the
  particles involved can be weighted or not by a gaussian fall-off instead of
  a strict use-it-or-not cone.
  @note   The probe particle is always omitted from the considered isoParticles
          (if they are from the same collection, of course).
*/
template<typename Particle, typename IsoParticle>
static double customIsolation(reco::PFCandidate::ParticleType lepton, const Particle& particle,
                              const std::vector<IsoParticle> & isoParticles, double coneDR, bool directional, bool falloff,
                              bool includePhoton = true, bool includeCharged = true, bool includeNeutralHadron = true,
                              bool normalizeEtaPhi = true, double minNeutralPT = 0.5)
{
  const double                              maxDR         = ( falloff ? 5*coneDR : coneDR );
  double                                    isoSum        = 0;
  math::XYZVector              isoAngleSum;
  std::vector<math::XYZVector> coneParticles;
  bool matched = false;
  for (unsigned int iPtcl = 0; iPtcl < isoParticles.size(); ++iPtcl) {
    if (lepton == reco::PFCandidate::mu && isoParticles[iPtcl].muonRef().isNonnull() && isoParticles[iPtcl].trackRef() == particle.track()) {
      matched = true; continue; }
    if (lepton == reco::PFCandidate::e  && isoParticles[iPtcl].gsfTrackRef().isNonnull() && isoParticles[iPtcl].gsfTrackRef() == particle.gsfTrack()) {
      matched = true; continue; }
    const IsoParticle&          isoParticle   = isoParticles[iPtcl];
    if (isoParticle.pt() <= 0)  continue;     // Ignore weird stuff
    //.. Vetoes ...............................................................
    if (isoParticle.charge()) {
      if (!includeCharged)                    continue;
    }
    else if (isoParticle.particleId() == reco::PFCandidate::gamma) {
      if (!includePhoton)                     continue;
      if (isoParticle.pt() < minNeutralPT)    continue;
    }
    else if (isoParticle.particleId() == reco::PFCandidate::h0) {
      if (!includeNeutralHadron)              continue;
      if (isoParticle.pt() < minNeutralPT)    continue;
    }
    //.. Computation ..........................................................
    const double                dR            = reco::deltaR(isoParticle, particle);
    if (dR > maxDR)             continue;
    const double                weight        = falloff
                                              ? TMath::Gaus(dR, 0, coneDR, true)
                                              : 1
                                              ;
    if (directional) {
      math::XYZVector           transverse( isoParticle.eta() - particle.eta()
                                          , reco::deltaPhi(isoParticle.phi(), particle.phi())
                                          , 0);
      if (normalizeEtaPhi)
        transverse             /= transverse.rho();
      transverse               *= weight * isoParticle.pt();
      if (transverse.rho() > 0) {
        isoAngleSum            += transverse;
        coneParticles.push_back(transverse);
      } // else if (lepton == reco::PFCandidate::e) std::cout << "ZERO! " << isoParticles[iPtcl].muonRef().isNonnull() << " "
        //                                                   << isoParticles[iPtcl].gsfTrackRef().id() << " " << particle.gsfTrack().id()
        //                                                   << " " << isoParticles[iPtcl].gsfTrackRef().key() << " " << particle.gsfTrack().key() << std::endl;
    } else { // if not directional
      isoSum                   += weight * isoParticle.pt();
    }
  } // end loop over PF candidates

  // if (!matched) std::cout << "AAAH a non-matched lepton! " << (int) (lepton == reco::PFCandidate::mu) << " " << (int) (lepton == reco::PFCandidate::e) << " " << isoSum << " " << particle.pt() << std::endl;

  if (directional) {
    double                        directionalPT = 0;
    for (unsigned int iPtcl = 0; iPtcl < coneParticles.size(); ++iPtcl)
      directionalPT              += pow(ROOT::Math::VectorUtil::DeltaPhi(coneParticles[iPtcl], isoAngleSum), 2)
                                  * coneParticles[iPtcl].rho()
                                        ;
    return directionalPT;
  }
  return isoSum;

}

#endif

