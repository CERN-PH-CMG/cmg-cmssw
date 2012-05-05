#ifndef COMPOUNDTYPES_H_
#define COMPOUNDTYPES_H_

#include "AnalysisDataFormats/CMGTools/interface/FatJet.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/DiObject.h"
#include "AnalysisDataFormats/CMGTools/interface/DiObjectPair.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/Hemisphere.h"
#include "AnalysisDataFormats/CMGTools/interface/Photon.h"

#include "AnalysisDataFormats/CMGTools/interface/GenericTypes.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

namespace cmg {

  typedef cmg::DiObject<cmg::PFCandidate, cmg::PFCandidate> DiPFCandidate;
  typedef cmg::DiObject<cmg::Hemisphere, cmg::Hemisphere> DiHemisphere;
  typedef cmg::DiObject<cmg::FatJet, cmg::FatJet> DiFatJet;
  typedef cmg::DiObject<cmg::Photon, cmg::Photon> DiPhoton;
  typedef cmg::DiObject<cmg::Electron,cmg::Electron> DiElectron;
  typedef cmg::DiObject<cmg::BaseJet,cmg::BaseJet> DiJet;
  //  typedef cmg::DiObject< edm::Ptr<cmg::Muon>, edm::Ptr< cmg::Muon> > DiMuon;
  typedef cmg::DiObject<cmg::Muon, cmg::Muon > DiMuon;
  typedef cmg::DiObject<cmg::PFJet,cmg::PFJet> DiPFJet;
  typedef cmg::DiObject<cmg::Tau,cmg::Tau> DiTau;
  typedef cmg::DiObject<cmg::Tau,cmg::Electron> TauEle;
  typedef cmg::DiObject<cmg::Tau,cmg::Muon> TauMu;
  typedef cmg::DiObject<cmg::Muon,cmg::Electron> MuEle;
  typedef cmg::DiObject<cmg::Electron,cmg::BaseMET> WENu;
  typedef cmg::DiObject<cmg::Muon,cmg::BaseMET> WMuNu;
  typedef cmg::DiObject<cmg::Tau,cmg::BaseMET> WTauNu;

  // Mainly for testing rather than Higgs discovery!
  //  typedef cmg::DiObject<cmg::DiMuon,cmg::DiMuon> QuadMuon;

  //Four Lepton Types (by Michalis)
  typedef cmg::DiObjectPair<cmg::Muon, cmg::Muon,cmg::Muon,cmg::Muon > QuadMuon;
  typedef cmg::DiObjectPair<cmg::Muon, cmg::Muon,cmg::Electron,cmg::Electron > DiMuonDiElectron;
  typedef cmg::DiObjectPair<cmg::Electron, cmg::Electron,cmg::Electron,cmg::Electron > QuadElectron;
  typedef cmg::DiObjectPair<cmg::Muon, cmg::Muon,cmg::Muon,cmg::BaseMET > TriMuonMET;


}

#endif /*COMPOUNDTYPES_H_*/
