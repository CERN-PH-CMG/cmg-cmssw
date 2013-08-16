#ifndef PATMUONHELPER_H
#define PATMUONHELPER_H
//-----------------------------------------------------------------------------
// Subsystem:   Ntuples
// Package:     TNMc1
// Description: TheNtupleMaker helper class for pat::Muon
// Created:     Tue May  8 18:07:16 2012
// Author:      Sezen Sekmen      
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include "PhysicsTools/TheNtupleMaker/interface/HelperFor.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Math/interface/Point3D.h"
//-----------------------------------------------------------------------------
// Definitions:
//   helper:        object of class MuonHelper
//   helped object: object of class pat::Muon
//
//
// The following variables are automatically defined and available to
//       all methods:
//
//         blockname          name of config. buffer object (config block) 
//         buffername         name of buffer in config block
//         labelname          name of label in config block (for getByLabel)
//         parameter          parameter (as key, value pairs)
//                            accessed as in the following example:
//
//                            string param = parameter("label");
//
//         0. hltconfig       pointer to HLTConfigProvider
//                            Note: this will be zero if HLTConfigProvider
//                                  has not been properly initialized
//
//         1. config          pointer to global ParameterSet object
//         2. event           pointer to the current event
//         3. object          pointer to the current helped object
//         4. oindex          index of current helped object
//
//         5. index           index of item(s) returned by helper.
//                            Note 1: an item is associated with all
//                                    helper methods (think of it as an
//                                    extension of the helped object)
//                                  
//                            Note 2: index may differ from oindex if,
//                                    for a given helped object, the count
//                                    variable (see below) differs from 1.
//
//         6. count           number of items per helped object (default=1)
//                            Note:
//                                  count = 0  ==> current helped object is
//                                                 to be skipped
//
//                                  count = 1  ==> current helped object is
//                                                 to be kept
//
//                                  count > 1  ==> current helped object is
//                                                 associated with "count"
//                                                 items, where each item
//                                                 is associated with all the
//                                                 helper methods
//
//       variables 0-6 are initialized by TheNtupleMaker.
//       variables 0-5 should not be changed.
//       variable    6 can be changed by the helper to control whether a
//                     helped object should be kept or generates more items
//-----------------------------------------------------------------------------

namespace pat
{
  /// A helper class for pat::Muon.
  class MuonHelper : public HelperFor<pat::Muon>
  {
  public:
	///
	MuonHelper();

	virtual ~MuonHelper();

	// Uncomment if this class does some event-level analysis
	virtual void analyzeEvent();
	 
	// Uncomment if this class does some object-level analysis
	// virtual void analyzeObject();

	// ---------------------------------------------------------
	// -- User access methods go here
	// ---------------------------------------------------------

	
  private:
    // -- User internals
    // PV exists
    bool _PVexists;
    // PV position
    math::XYZPoint _PVposition;
    

  public:
    // ---------------------------------------------------------
    // -- Access Methods
    // ---------------------------------------------------------

    // Extra:

    // dxy wrt primary vertex
    double dxywrtPV() const;
    // dz wrt primary vertex
    double dzwrtPV() const;

    // Standard:

    int numberOfMatchedStations() const
    { return object->numberOfMatchedStations(); }

    double dB() const
    { return object->dB(); }

    reco::Track* bestTrack() const
    { return (reco::Track*)object->bestTrack(); }

    reco::TrackBaseRef bestTrackRef() const { return object->bestTrackRef(); }

    reco::RecoCandidate::TrackType bestTrackType() const
    { return object->bestTrackType(); }

    math::XYZVector boostToCM() const { return object->boostToCM(); }

    reco::MuonEnergy calEnergy() const { return object->calEnergy(); }

    float caloCompatibility() const { return object->caloCompatibility(); }

    float caloIso() const { return object->caloIso(); }

    reco::MuonMETCorrectionData caloMETMuonCorrs() const
    { return object->caloMETMuonCorrs(); }

    CaloTowerRef caloTower() const { return object->caloTower(); }

    int charge() const { return object->charge(); }

    float chargedHadronIso() const { return object->chargedHadronIso(); }

    reco::TrackRef combinedMuon() const { return object->combinedMuon(); }

    reco::MuonQuality combinedQuality() const
    { return object->combinedQuality(); }

    reco::Candidate* daughter(std::string s) const
    { return (reco::Candidate*)object->daughter(s); }

    reco::TrackRef dytTrack() const { return object->dytTrack(); }

    float ecalIso() const { return object->ecalIso(); }

    pat::IsoDeposit* ecalIsoDeposit() const
    { return (pat::IsoDeposit*)object->ecalIsoDeposit(); }

    std::vector<std::pair<std::string,pat::LookupTableRecord> >
    efficiencies() const
    { return object->efficiencies(); }

    const pat::LookupTableRecord efficiency(std::string name) const
    { return object->efficiency(name); }

    const std::vector<std::string> efficiencyNames() const
    { return object->efficiencyNames(); }

    const std::vector<pat::LookupTableRecord> efficiencyValues() const
    { return object->efficiencyValues(); }

    double energy() const { return object->energy(); }

    double et() const { return object->et(); }

    double eta() const { return object->eta(); }

    reco::GenParticle* genLepton() const
    { return (reco::GenParticle*)object->genLepton(); }

    std::vector<reco::GenParticleRef> genParticleRefs() const
    { return object->genParticleRefs(); }

    size_t genParticlesSize() const { return object->genParticlesSize(); }

    const pat::CandKinResolution getKinResolution(std::string label="") const
    { return object->getKinResolution(label); }

    reco::TrackRef globalTrack() const { return object->globalTrack(); }

    reco::GsfTrackRef gsfTrack() const { return object->gsfTrack(); }

    bool hasKinResolution(std::string label="") const
    { return object->hasKinResolution(label); }

    bool hasOverlaps(std::string label) const
    { return object->hasOverlaps(label); }

    bool hasUserCand(std::string key) const
    { return object->hasUserCand(key); }

    bool hasUserData(std::string key) const
    { return object->hasUserData(key); }

    bool hasUserFloat(char* key) const { return object->hasUserFloat(key); }

    bool hasUserFloat(std::string key) const
    { return object->hasUserFloat(key); }

    bool hasUserInt(std::string key) const { return object->hasUserInt(key); }

    float hcalIso() const { return object->hcalIso(); }

    pat::IsoDeposit* hcalIsoDeposit() const
    { return (pat::IsoDeposit*)object->hcalIsoDeposit(); }

    reco::TrackRef innerTrack() const { return object->innerTrack(); }

    bool isCaloCompatibilityValid() const
    { return object->isCaloCompatibilityValid(); }

    bool isCaloMuon() const { return object->isCaloMuon(); }

    bool isConvertedPhoton() const { return object->isConvertedPhoton(); }

    bool isElectron() const { return object->isElectron(); }

    bool isEnergyValid() const { return object->isEnergyValid(); }

    bool isGlobalMuon() const { return object->isGlobalMuon(); }

    bool isGood(std::string name) const { return object->isGood(name); }

    bool isIsolationValid() const { return object->isIsolationValid(); }

    bool isJet() const { return object->isJet(); }

    bool isMatchesValid() const { return object->isMatchesValid(); }

    bool isMuon() const { return object->isMuon(); }

    const reco::MuonIsolation isolationR03() const
    { return object->isolationR03(); }

    const reco::MuonIsolation isolationR05() const
    { return object->isolationR05(); }

    bool isPFIsolationValid() const { return object->isPFIsolationValid(); }

    bool isPFMuon() const { return object->isPFMuon(); }

    bool isPhoton() const { return object->isPhoton(); }

    bool isQualityValid() const { return object->isQualityValid(); }

    bool isStandAloneMuon() const { return object->isStandAloneMuon(); }

    bool isTimeValid() const { return object->isTimeValid(); }

    bool isTrackerMuon() const { return object->isTrackerMuon(); }

    bool longLived() const { return object->longLived(); }

    double mass() const { return object->mass(); }

    bool massConstraint() const { return object->massConstraint(); }

    double massSqr() const { return object->massSqr(); }

    std::vector<reco::MuonChamberMatch> matches() const
    { return object->matches(); }

    math::XYZVector momentum() const { return object->momentum(); }

    double mt() const { return object->mt(); }

    double mtSqr() const { return object->mtSqr(); }

    reco::TrackRef muonBestTrack() const { return object->muonBestTrack(); }

    reco::Muon::MuonTrackType muonBestTrackType() const
    { return object->muonBestTrackType(); }

    bool muonID(std::string name) const { return object->muonID(name); }

    float neutralHadronIso() const { return object->neutralHadronIso(); }

    double normChi2() const { return object->normChi2(); }

    int numberOfChambers() const { return object->numberOfChambers(); }

    int numberOfChambersNoRPC() const
    { return object->numberOfChambersNoRPC(); }

    size_t numberOfDaughters() const { return object->numberOfDaughters(); }

    size_t numberOfMothers() const { return object->numberOfMothers(); }

    size_t numberOfSourceCandidatePtrs() const
    { return object->numberOfSourceCandidatePtrs(); }

    size_t numberOfTracks() const { return object->numberOfTracks(); }

    unsigned int numberOfValidHits() const
    { return object->numberOfValidHits(); }

    reco::Candidate* originalObject() const
    { return (reco::Candidate*)object->originalObject(); }

    const edm::Ptr<reco::Candidate> originalObjectRef() const
    { return object->originalObjectRef(); }

    reco::TrackRef outerTrack() const { return object->outerTrack(); }

    const std::vector<std::string> overlapLabels() const
    { return object->overlapLabels(); }

    const edm::PtrVector<reco::Candidate> overlaps(std::string label) const
    { return object->overlaps(label); }

    double p() const { return object->p(); }

    const math::XYZTLorentzVector p4() const { return object->p4(); }

    float particleIso() const { return object->particleIso(); }

    int pdgId() const { return object->pdgId(); }

    reco::PFCandidateRef pfCandidateRef() const
    { return object->pfCandidateRef(); }

    const reco::MuonPFIsolation pfIsolationR03() const
    { return object->pfIsolationR03(); }

    const reco::MuonPFIsolation pfIsolationR04() const
    { return object->pfIsolationR04(); }

    math::XYZTLorentzVector pfP4() const { return object->pfP4(); }

    double phi() const { return object->phi(); }

    float photonIso() const { return object->photonIso(); }

    reco::TrackRef pickyMuon() const { return object->pickyMuon(); }

    reco::TrackRef pickyTrack() const { return object->pickyTrack(); }

    const math::PtEtaPhiMLorentzVector polarP4() const
    { return object->polarP4(); }

    double pt() const { return object->pt(); }

    float puChargedHadronIso() const { return object->puChargedHadronIso(); }

    double px() const { return object->px(); }

    double py() const { return object->py(); }

    double pz() const { return object->pz(); }

    double rapidity() const { return object->rapidity(); }

    double resolE(std::string label="") const { return object->resolE(label); }

    double resolEt(std::string label="") const
    { return object->resolEt(label); }

    double resolEta(std::string label="") const
    { return object->resolEta(label); }

    double resolM(std::string label="") const { return object->resolM(label); }

    double resolP(std::string label="") const { return object->resolP(label); }

    double resolPhi(std::string label="") const
    { return object->resolPhi(label); }

    double resolPInv(std::string label="") const
    { return object->resolPInv(label); }

    double resolPt(std::string label="") const
    { return object->resolPt(label); }

    double resolPx(std::string label="") const
    { return object->resolPx(label); }

    double resolPy(std::string label="") const
    { return object->resolPy(label); }

    double resolPz(std::string label="") const
    { return object->resolPz(label); }

    double resolTheta(std::string label="") const
    { return object->resolTheta(label); }

    reco::TrackRef standAloneMuon() const { return object->standAloneMuon(); }

    unsigned int stationGapMaskDistance(float distanceCut=1.0e+1) const
    { return object->stationGapMaskDistance(distanceCut); }

    unsigned int stationGapMaskPull(float sigmaCut=3.0e+0) const
    { return object->stationGapMaskPull(sigmaCut); }

    int status() const { return object->status(); }

    reco::SuperClusterRef superCluster() const
    { return object->superCluster(); }

    //float t0(int n=0) const { return object->t0(n); }

    reco::MuonMETCorrectionData tcMETMuonCorrs() const
    { return object->tcMETMuonCorrs(); }

    double theta() const { return object->theta(); }

    int threeCharge() const { return object->threeCharge(); }

    reco::MuonTime time() const { return object->time(); }

    reco::TrackRef tpfmsMuon() const { return object->tpfmsMuon(); }

    reco::TrackRef tpfmsTrack() const { return object->tpfmsTrack(); }

    reco::TrackRef track() const { return object->track(); }

    float trackIso() const { return object->trackIso(); }

    pat::IsoDeposit* trackIsoDeposit() const
    { return (pat::IsoDeposit*)object->trackIsoDeposit(); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatches() const
    { return object->triggerObjectMatches(); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByAlgorithm(char* nameAlgorithm, bool algoCondAccepted=true) const
    { return object->triggerObjectMatchesByAlgorithm(nameAlgorithm, algoCondAccepted); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByAlgorithm(char* nameAlgorithm, unsigned int algoCondAccepted) const
    { return object->triggerObjectMatchesByAlgorithm(nameAlgorithm, algoCondAccepted); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByAlgorithm(std::string nameAlgorithm, bool algoCondAccepted=true) const
    { return object->triggerObjectMatchesByAlgorithm(nameAlgorithm, algoCondAccepted); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByAlgorithm(std::string nameAlgorithm, unsigned int algoCondAccepted) const
    { return object->triggerObjectMatchesByAlgorithm(nameAlgorithm, algoCondAccepted); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByCollection(char* coll) const
    { return object->triggerObjectMatchesByCollection(coll); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByCollection(std::string coll) const
    { return object->triggerObjectMatchesByCollection(coll); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByCondition(char* nameCondition) const
    { return object->triggerObjectMatchesByCondition(nameCondition); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByCondition(std::string nameCondition) const
    { return object->triggerObjectMatchesByCondition(nameCondition); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByFilter(char* labelFilter) const
    { return object->triggerObjectMatchesByFilter(labelFilter); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByFilter(std::string labelFilter) const
    { return object->triggerObjectMatchesByFilter(labelFilter); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByFilterID(unsigned int triggerObjectType) const
    { return object->triggerObjectMatchesByFilterID(triggerObjectType); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByPath(char* namePath, bool pathLastFilterAccepted=false, bool pathL3FilterAccepted=true) const
    { return object->triggerObjectMatchesByPath(namePath, pathLastFilterAccepted, pathL3FilterAccepted); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByPath(char* namePath, unsigned int pathLastFilterAccepted, unsigned int pathL3FilterAccepted=1) const
    { return object->triggerObjectMatchesByPath(namePath, pathLastFilterAccepted, pathL3FilterAccepted); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByPath(std::string namePath, bool pathLastFilterAccepted=false, bool pathL3FilterAccepted=true) const
    { return object->triggerObjectMatchesByPath(namePath, pathLastFilterAccepted, pathL3FilterAccepted); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByPath(std::string namePath, unsigned int pathLastFilterAccepted, unsigned int pathL3FilterAccepted=1) const
    { return object->triggerObjectMatchesByPath(namePath, pathLastFilterAccepted, pathL3FilterAccepted); }

    const std::vector<pat::TriggerObjectStandAlone>
    triggerObjectMatchesByType(unsigned int triggerObjectType) const
    { return object->triggerObjectMatchesByType(triggerObjectType); }

    unsigned int type() const { return object->type(); }

    edm::Ptr<reco::Candidate> userCand(std::string key) const
    { return object->userCand(key); }

    const std::vector<std::string> userCandNames() const
    { return object->userCandNames(); }

    const std::vector<std::string> userDataNames() const
    { return object->userDataNames(); }

    const std::string userDataObjectType(std::string key) const
    { return object->userDataObjectType(key); }

    float userFloat(char* key) const { return object->userFloat(key); }

    float userFloat(std::string key) const { return object->userFloat(key); }

    const std::vector<std::string> userFloatNames() const
    { return object->userFloatNames(); }

    int userInt(std::string key) const { return object->userInt(key); }

    const std::vector<std::string> userIntNames() const
    { return object->userIntNames(); }

    float userIsolation(std::string key) const
    { return object->userIsolation(key); }

    const math::XYZPoint vertex() const { return object->vertex(); }

    double vertexChi2() const { return object->vertexChi2(); }

    ROOT::Math::SMatrix<double,3,3,ROOT::Math::MatRepSym<double,3> >
    vertexCovariance() const
    { return object->vertexCovariance(); }

    double vertexCovariance(int i, int j) const
    { return object->vertexCovariance(i, j); }

    double vertexNdof() const { return object->vertexNdof(); }

    double vertexNormalizedChi2() const
    { return object->vertexNormalizedChi2(); }

    double vx() const { return object->vx(); }

    double vy() const { return object->vy(); }

    double vz() const { return object->vz(); }

    double y() const { return object->y(); }
  };
}
#endif
