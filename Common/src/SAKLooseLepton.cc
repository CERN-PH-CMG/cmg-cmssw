#include "CMGTools/Common/interface/SAKLooseLepton.h"

//_____________________________________________________________________________
/**
  Uses:
    const math::XYZPoint&               primaryVertex;  ///< First vertex in vertices list
    Handle<reco::PFCandidateCollection> pfCandidates;   ///< Particle flow candidates after PFchs
*/
bool isLooseElectron(const reco::GsfElectron & electron, const math::XYZPoint & primaryVertex, edm::Handle<reco::PFCandidateCollection> pfCandidates)
{
  const reco::PFCandidateCollection & pfCands = *pfCandidates;
  return isLooseElectron(electron, primaryVertex, pfCands);
}


bool isLooseElectron(const reco::GsfElectron & electron, const math::XYZPoint & primaryVertex, const reco::PFCandidateCollection & pfCandidates)
{
  return isLooseElectronID(electron, primaryVertex) && isLooseElectronIso(electron, pfCandidates);
}


bool isLooseElectronID(const reco::GsfElectron & electron, const math::XYZPoint & primaryVertex)
{
  static const int      numLeptonPTs                = 8;
  static const double   LEPTON_PTS[]                = {0, 5, 10, 15, 20, 40, 80, 140, 200};

  static const double   MAX_DZ    [][numLeptonPTs]  = { {0.03, 0.05, 0.05, 0.05, 0.2, 1, 1, 1}
                                                      , {0.09, 0.09, 0.09, 0.11, 1  , 1, 1, 1}
                                                      };
//  static const double   MAX_HOE   [][numLeptonPTs]  = { {0.74, 0.43, 0.33, 0.3, 0.37, 0.49, 0.76, 0.95}
//                                                     };
  //...........................................................................
  const reco::GsfTrackRef & track = electron.gsfTrack();
  if (electron.pt() < 20 && track->trackerExpectedHitsInner().numberOfLostHits() > 0                        )                  return false;
  if (electron.pt() < 10 && track->hitPattern().numberOfValidPixelHits()         < (electron.isEB() ? 2 : 1))                  return false;
  if (!passCut(electron.pt(), std::abs(track->dz (primaryVertex)), true, numLeptonPTs, LEPTON_PTS, MAX_DZ [!electron.isEB()])) return false;
// S.L. 21/6
// switched off after problems modelling this in PF embedding
// seems to incur an additional ~3% inefficiency on signal
//  if (electron.isEB()) {
//    if (!passCut(electron.pt(), electron.hadronicOverEm()        , true, numLeptonPTs, LEPTON_PTS, MAX_HOE[!electron.isEB()])) return false;
//  }
  return true;
}


bool isLooseElectronIso(const reco::GsfElectron & electron, const reco::PFCandidateCollection & pfCandidates)
{
  static const int      numLeptonPTs                = 8;
  static const double   LEPTON_PTS[]                = {0, 5, 10, 15, 20, 40, 80, 140, 200};
  static const double   MAX_ISO1  [][numLeptonPTs]  = { {0.5, 1.5, 4.5, 7.5, 10  , 18.5, 44  , 81.5}
                                                      , {0.5, 2.5, 6.5, 9  , 10.5, 18.5, 66.5, 70  }
                                                      };
  static const double   MAX_ISO2  [][numLeptonPTs]  = { {2, 4.25, 8.75, 11, 20.8, 200, 200, 200}
                                                      };
  //...........................................................................
  double isolation = electron.isEB() 
              ? customIsolation(reco::PFCandidate::e, electron, pfCandidates, 0.2, true, false)
              : customIsolation(reco::PFCandidate::e, electron, pfCandidates, 0.3, true, false, false, true, false)
              ;
  if   (!passCut(electron.pt(), isolation, true, numLeptonPTs, LEPTON_PTS, MAX_ISO1[!electron.isEB()])) return false;
  if (electron.isEB()) {
    isolation = customIsolation(reco::PFCandidate::e, electron, pfCandidates, 0.3, true, false, false, true, false);
    if (!passCut(electron.pt(), isolation, true, numLeptonPTs, LEPTON_PTS, MAX_ISO2[!electron.isEB()])) return false;
  }
  return true;
}


//_____________________________________________________________________________
/**
  Uses:
    const math::XYZPoint&               primaryVertex;  ///< First vertex in vertices list
    Handle<reco::PFCandidateCollection> pfCandidates;   ///< Particle flow candidates after PFchs
*/
bool isLooseMuon(const reco::Muon & muon, const math::XYZPoint & primaryVertex, edm::Handle<reco::PFCandidateCollection> pfCandidates)
{
  const reco::PFCandidateCollection & pfCands = *pfCandidates;
  return isLooseMuon(muon, primaryVertex, pfCands);
}


bool isLooseMuon(const reco::Muon & muon, const math::XYZPoint & primaryVertex, const reco::PFCandidateCollection & pfCandidates)
{
  return isLooseMuonID(muon, primaryVertex) && isLooseMuonIso(muon, pfCandidates);
}


bool isLooseMuonID(const reco::Muon & muon, const math::XYZPoint & primaryVertex)
{
  static const int      numLeptonPTs            = 8;
  static const double   LEPTON_PTS[]            = {0, 5, 10, 15, 20, 40, 80, 140, 200};

  static const double   MAX_DXY[][numLeptonPTs] = { {0.052, 0.041, 0.029, 0.014, 0.021, 0.04, 0.1, 0.1}
                                                  , {0.037, 0.018, 0.013, 0.015, 0.021, 0.2 , 0.2, 0.2}
                                                  };
  static const double   MAX_DZ [][numLeptonPTs] = { {0.054, 0.042, 0.028, 0.034, 1   , 1   , 1, 1}
                                                  , {0.076, 0.082, 0.098, 0.1  , 0.1 , 1   , 1, 1}
                                                  };
  //...........................................................................
  const bool inBarrel = ( std::abs(muon.eta()) < 1.5 );
  const reco::TrackRef & track = muon.innerTrack();

  muon::SelectionType glprt = muon::selectionTypeFromString("GlobalMuonPromptTight");
  bool muonidglprt = muon::isGoodMuon(muon, glprt);
  muon::SelectionType tmlst = muon::selectionTypeFromString("TMLastStationTight");
  bool muonidtmlst = muon::isGoodMuon(muon, tmlst);

  if (!( muonidglprt && muon.numberOfMatches() > 1 ) && !( muon.isTrackerMuon() && muonidtmlst )) return false;
  if (track->trackerExpectedHitsInner().numberOfLostHits() > (muon.pt() < 20 ? 1 : 4))            return false;
  if (!passCut(muon.pt(), std::abs(track->dxy(primaryVertex)), true, numLeptonPTs, LEPTON_PTS, MAX_DXY[!inBarrel])) return false;
  if (!passCut(muon.pt(), std::abs(track->dz (primaryVertex)), true, numLeptonPTs, LEPTON_PTS, MAX_DZ [!inBarrel])) return false;
  return true;
}


bool isLooseMuonIso(const reco::Muon & muon, const reco::PFCandidateCollection & pfCandidates)
{
  static const int      numLeptonPTs            = 8;
  static const double   LEPTON_PTS[]            = {0, 5, 10, 15, 20, 40, 80, 140, 200};
  static const double   MAX_ISO[][numLeptonPTs] = { {1.5, 3  , 7  , 10.5, 15.5, 32.5, 54.5, 87  }
                                                  , {2  , 2.5, 7.5, 9   , 13.5, 19  , 37  , 65.5}
                                                  };
  //...........................................................................
  const bool inBarrel = ( std::abs(muon.eta()) < 1.5 );
  double isolation = customIsolation(reco::PFCandidate::mu, muon, pfCandidates, 0.2, true, false);
  if (!passCut(muon.pt(), isolation, true, numLeptonPTs, LEPTON_PTS, MAX_ISO[!inBarrel])) return false;
  return true;
}


//_____________________________________________________________________________
bool passCut(double parameter, double variable, bool discardAbove, int numBins, const double* parameterBins, const double* variableCuts)
{
  double                threshold = variableCuts[0];
  for (int bin = 0; true; ++bin) {
    if (bin >= numBins) {
      threshold         = variableCuts[numBins - 1];
      break;
    }
    if (parameter < parameterBins[bin]) {
      threshold         = variableCuts[bin - 1];
      break;
    }
  } // end loop over bins

  return (discardAbove ? variable <= threshold : variable >= threshold);
}

