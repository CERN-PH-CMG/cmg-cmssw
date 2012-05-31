#ifndef PATELECTRONHELPER_H
#define PATELECTRONHELPER_H
//-----------------------------------------------------------------------------
// Subsystem:   Ntuples
// Package:     TNMc1
// Description: TheNtupleMaker helper class for pat::Electron
// Created:     Wed May  9 15:36:04 2012
// Author:      Sezen Sekmen      
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include "PhysicsTools/TheNtupleMaker/interface/HelperFor.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
//-----------------------------------------------------------------------------
// Definitions:
//   helper:        object of class ElectronHelper
//   helped object: object of class pat::Electron
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
  /// A helper class for pat::Electron.
  class ElectronHelper : public HelperFor<pat::Electron>
  {
  public:
	///
	ElectronHelper();

	virtual ~ElectronHelper();

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

    bool ambiguous() const { return object->ambiguous(); }

    edm::RefVectorIterator<std::vector<reco::GsfTrack>,reco::GsfTrack,edm::refhelper::FindUsingAdvance<std::vector<reco::GsfTrack>,reco::GsfTrack> >
    ambiguousGsfTracksBegin() const
    { return object->ambiguousGsfTracksBegin(); }

    edm::RefVectorIterator<std::vector<reco::GsfTrack>,reco::GsfTrack,edm::refhelper::FindUsingAdvance<std::vector<reco::GsfTrack>,reco::GsfTrack> >
    ambiguousGsfTracksEnd() const
    { return object->ambiguousGsfTracksEnd(); }

    std::size_t ambiguousGsfTracksSize() const
    { return object->ambiguousGsfTracksSize(); }

    int basicClustersSize() const { return object->basicClustersSize(); }

    reco::Track* bestTrack() const
    { return (reco::Track*)object->bestTrack(); }

    reco::TrackBaseRef bestTrackRef() const { return object->bestTrackRef(); }

    reco::RecoCandidate::TrackType bestTrackType() const
    { return object->bestTrackType(); }

    math::XYZVector boostToCM() const { return object->boostToCM(); }

    float caloEnergy() const { return object->caloEnergy(); }

    float caloIso() const { return object->caloIso(); }

    math::XYZPoint caloPosition() const { return object->caloPosition(); }

    CaloTowerRef caloTower() const { return object->caloTower(); }

    reco::GsfElectron::P4Kind candidateP4Kind() const
    { return object->candidateP4Kind(); }

    int charge() const { return object->charge(); }

    float chargedHadronIso() const { return object->chargedHadronIso(); }

    const reco::GsfElectron::ChargeInfo chargeInfo() const
    { return object->chargeInfo(); }

    reco::GsfElectron::Classification classification() const
    { return object->classification(); }

    const reco::GsfElectron::ClassificationVariables
    classificationVariables() const
    { return object->classificationVariables(); }

    reco::GsfElectron::ClosestCtfTrack closestCtfTrack() const
    { return object->closestCtfTrack(); }

    reco::TrackRef closestCtfTrackRef() const
    { return object->closestCtfTrackRef(); }

    reco::TrackRef closestTrack() const { return object->closestTrack(); }

    reco::TrackRef combinedMuon() const { return object->combinedMuon(); }

    float convDcot() const { return object->convDcot(); }

    float convDist() const { return object->convDist(); }

    const reco::GsfElectron::ConversionRejection
    conversionRejectionVariables() const
    { return object->conversionRejectionVariables(); }

    int convFlags() const { return object->convFlags(); }

    reco::TrackBaseRef convPartner() const { return object->convPartner(); }

    float convRadius() const { return object->convRadius(); }

    reco::GsfElectronCoreRef core() const { return object->core(); }

    float correctedEcalEnergy() const { return object->correctedEcalEnergy(); }

    float correctedEcalEnergyError() const
    { return object->correctedEcalEnergyError(); }

    const reco::GsfElectron::Corrections corrections() const
    { return object->corrections(); }

    float ctfGsfOverlap() const { return object->ctfGsfOverlap(); }

    reco::Candidate* daughter(std::string s) const
    { return (reco::Candidate*)object->daughter(s); }

    float deltaEtaEleClusterTrackAtCalo() const
    { return object->deltaEtaEleClusterTrackAtCalo(); }

    float deltaEtaSeedClusterTrackAtCalo() const
    { return object->deltaEtaSeedClusterTrackAtCalo(); }

    float deltaEtaSuperClusterTrackAtVtx() const
    { return object->deltaEtaSuperClusterTrackAtVtx(); }

    float deltaPhiEleClusterTrackAtCalo() const
    { return object->deltaPhiEleClusterTrackAtCalo(); }

    float deltaPhiSeedClusterTrackAtCalo() const
    { return object->deltaPhiSeedClusterTrackAtCalo(); }

    float deltaPhiSuperClusterTrackAtVtx() const
    { return object->deltaPhiSuperClusterTrackAtVtx(); }

    float dr03EcalRecHitSumEt() const { return object->dr03EcalRecHitSumEt(); }

    float dr03HcalDepth1TowerSumEt() const
    { return object->dr03HcalDepth1TowerSumEt(); }

    float dr03HcalDepth1TowerSumEtBc() const
    { return object->dr03HcalDepth1TowerSumEtBc(); }

    float dr03HcalDepth2TowerSumEt() const
    { return object->dr03HcalDepth2TowerSumEt(); }

    float dr03HcalDepth2TowerSumEtBc() const
    { return object->dr03HcalDepth2TowerSumEtBc(); }

    float dr03HcalTowerSumEt() const { return object->dr03HcalTowerSumEt(); }

    float dr03HcalTowerSumEtBc() const
    { return object->dr03HcalTowerSumEtBc(); }

    const reco::GsfElectron::IsolationVariables dr03IsolationVariables() const
    { return object->dr03IsolationVariables(); }

    float dr03TkSumPt() const { return object->dr03TkSumPt(); }

    float dr04EcalRecHitSumEt() const { return object->dr04EcalRecHitSumEt(); }

    float dr04HcalDepth1TowerSumEt() const
    { return object->dr04HcalDepth1TowerSumEt(); }

    float dr04HcalDepth1TowerSumEtBc() const
    { return object->dr04HcalDepth1TowerSumEtBc(); }

    float dr04HcalDepth2TowerSumEt() const
    { return object->dr04HcalDepth2TowerSumEt(); }

    float dr04HcalDepth2TowerSumEtBc() const
    { return object->dr04HcalDepth2TowerSumEtBc(); }

    float dr04HcalTowerSumEt() const { return object->dr04HcalTowerSumEt(); }

    float dr04HcalTowerSumEtBc() const
    { return object->dr04HcalTowerSumEtBc(); }

    const reco::GsfElectron::IsolationVariables dr04IsolationVariables() const
    { return object->dr04IsolationVariables(); }

    float dr04TkSumPt() const { return object->dr04TkSumPt(); }

    float e1x5() const { return object->e1x5(); }

    float e2x5Max() const { return object->e2x5Max(); }

    float e5x5() const { return object->e5x5(); }

    bool ecalDriven() const { return object->ecalDriven(); }

    const math::XYZTLorentzVector ecalDrivenMomentum() const
    { return object->ecalDrivenMomentum(); }

    bool ecalDrivenSeed() const { return object->ecalDrivenSeed(); }

    float ecalEnergy() const { return object->ecalEnergy(); }

    float ecalEnergyError() const { return object->ecalEnergyError(); }

    float ecalIso() const { return object->ecalIso(); }

    pat::IsoDeposit* ecalIsoDeposit() const
    { return (pat::IsoDeposit*)object->ecalIsoDeposit(); }

    float eEleClusterOverPout() const { return object->eEleClusterOverPout(); }

    std::vector<std::pair<std::string,pat::LookupTableRecord> >
    efficiencies() const
    { return object->efficiencies(); }

    const pat::LookupTableRecord efficiency(std::string name) const
    { return object->efficiency(name); }

    const std::vector<std::string> efficiencyNames() const
    { return object->efficiencyNames(); }

    const std::vector<pat::LookupTableRecord> efficiencyValues() const
    { return object->efficiencyValues(); }

    reco::CaloClusterPtr electronCluster() const
    { return object->electronCluster(); }

    float electronID(char* name) const { return object->electronID(name); }

    float electronID(std::string name) const
    { return object->electronID(name); }

    const std::vector<std::pair<std::string,float> > electronIDs() const
    { return object->electronIDs(); }

    double energy() const { return object->energy(); }

    float eSeedClusterOverP() const { return object->eSeedClusterOverP(); }

    float eSeedClusterOverPout() const
    { return object->eSeedClusterOverPout(); }

    float eSuperClusterOverP() const { return object->eSuperClusterOverP(); }

    double et() const { return object->et(); }

    double eta() const { return object->eta(); }

    float fbrem() const { return object->fbrem(); }

    const reco::GsfElectron::FiducialFlags fiducialFlags() const
    { return object->fiducialFlags(); }

    reco::GenParticle* genLepton() const
    { return (reco::GenParticle*)object->genLepton(); }

    std::vector<reco::GenParticleRef> genParticleRefs() const
    { return object->genParticleRefs(); }

    size_t genParticlesSize() const { return object->genParticlesSize(); }

    const pat::CandKinResolution getKinResolution(std::string label="") const
    { return object->getKinResolution(label); }

    reco::GsfTrackRef gsfTrack() const { return object->gsfTrack(); }

    float hadronicOverEm() const { return object->hadronicOverEm(); }

    float hadronicOverEm1() const { return object->hadronicOverEm1(); }

    float hadronicOverEm2() const { return object->hadronicOverEm2(); }

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

    float hcalDepth1OverEcal() const { return object->hcalDepth1OverEcal(); }

    float hcalDepth1OverEcalBc() const
    { return object->hcalDepth1OverEcalBc(); }

    float hcalDepth2OverEcal() const { return object->hcalDepth2OverEcal(); }

    float hcalDepth2OverEcalBc() const
    { return object->hcalDepth2OverEcalBc(); }

    float hcalIso() const { return object->hcalIso(); }

    pat::IsoDeposit* hcalIsoDeposit() const
    { return (pat::IsoDeposit*)object->hcalIsoDeposit(); }

    float hcalOverEcal() const { return object->hcalOverEcal(); }

    float hcalOverEcalBc() const { return object->hcalOverEcalBc(); }

    const std::vector<CaloTowerDetId> hcalTowersBehindClusters() const
    { return object->hcalTowersBehindClusters(); }

    double ip3d() const { return object->ip3d(); }

    bool isCaloMuon() const { return object->isCaloMuon(); }

    bool isConvertedPhoton() const { return object->isConvertedPhoton(); }

    bool isEB() const { return object->isEB(); }

    bool isEBEEGap() const { return object->isEBEEGap(); }

    bool isEBEtaGap() const { return object->isEBEtaGap(); }

    bool isEBGap() const { return object->isEBGap(); }

    bool isEBPhiGap() const { return object->isEBPhiGap(); }

    bool isEcalEnergyCorrected() const
    { return object->isEcalEnergyCorrected(); }

    bool isEE() const { return object->isEE(); }

    bool isEEDeeGap() const { return object->isEEDeeGap(); }

    bool isEEGap() const { return object->isEEGap(); }

    bool isEERingGap() const { return object->isEERingGap(); }

    bool isElectron() const { return object->isElectron(); }

    bool isElectronIDAvailable(char* name) const
    { return object->isElectronIDAvailable(name); }

    bool isElectronIDAvailable(std::string name) const
    { return object->isElectronIDAvailable(name); }

    bool isEnergyScaleCorrected() const
    { return object->isEnergyScaleCorrected(); }

    bool isGap() const { return object->isGap(); }

    bool isGlobalMuon() const { return object->isGlobalMuon(); }

    bool isGsfCtfChargeConsistent() const
    { return object->isGsfCtfChargeConsistent(); }

    bool isGsfCtfScPixChargeConsistent() const
    { return object->isGsfCtfScPixChargeConsistent(); }

    bool isGsfScPixChargeConsistent() const
    { return object->isGsfScPixChargeConsistent(); }

    bool isJet() const { return object->isJet(); }

    bool isMuon() const { return object->isMuon(); }

    const reco::GsfElectron::IsolationVariables isolationVariables03() const
    { return object->isolationVariables03(); }

    const reco::GsfElectron::IsolationVariables isolationVariables04() const
    { return object->isolationVariables04(); }

    bool isPhoton() const { return object->isPhoton(); }

    bool isStandAloneMuon() const { return object->isStandAloneMuon(); }

    bool isTrackerMuon() const { return object->isTrackerMuon(); }

    bool longLived() const { return object->longLived(); }

    double mass() const { return object->mass(); }

    bool massConstraint() const { return object->massConstraint(); }

    double massSqr() const { return object->massSqr(); }

    math::XYZVector momentum() const { return object->momentum(); }

    double mt() const { return object->mt(); }

    double mtSqr() const { return object->mtSqr(); }

    float mva() const { return object->mva(); }

    const reco::GsfElectron::MvaInput mvaInput() const
    { return object->mvaInput(); }

    const reco::GsfElectron::MvaOutput mvaOutput() const
    { return object->mvaOutput(); }

    float neutralHadronIso() const { return object->neutralHadronIso(); }

    int numberOfBrems() const { return object->numberOfBrems(); }

    size_t numberOfDaughters() const { return object->numberOfDaughters(); }

    size_t numberOfMothers() const { return object->numberOfMothers(); }

    size_t numberOfSourceCandidatePtrs() const
    { return object->numberOfSourceCandidatePtrs(); }

    size_t numberOfTracks() const { return object->numberOfTracks(); }

    reco::Candidate* originalObject() const
    { return (reco::Candidate*)object->originalObject(); }

    const edm::Ptr<reco::Candidate> originalObjectRef() const
    { return object->originalObjectRef(); }

    const std::vector<std::string> overlapLabels() const
    { return object->overlapLabels(); }

    const edm::PtrVector<reco::Candidate> overlaps(std::string label) const
    { return object->overlaps(label); }

    double p() const { return object->p(); }

    const math::XYZTLorentzVector p4() const { return object->p4(); }

    float particleIso() const { return object->particleIso(); }

    bool passConversionVeto() const { return object->passConversionVeto(); }

    bool passingCutBasedPreselection() const
    { return object->passingCutBasedPreselection(); }

    bool passingMvaPreselection() const
    { return object->passingMvaPreselection(); }

    int pdgId() const { return object->pdgId(); }

    reco::PFCandidateRef pfCandidateRef() const
    { return object->pfCandidateRef(); }

    const reco::GsfElectron::PflowIsolationVariables
    pfIsolationVariables() const
    { return object->pfIsolationVariables(); }

    reco::SuperClusterRef pflowSuperCluster() const
    { return object->pflowSuperCluster(); }

    const reco::GsfElectron::ShowerShape pfShowerShape() const
    { return object->pfShowerShape(); }

    float pfSuperClusterFbrem() const { return object->pfSuperClusterFbrem(); }

    double phi() const { return object->phi(); }

    float photonIso() const { return object->photonIso(); }

    const math::PtEtaPhiMLorentzVector polarP4() const
    { return object->polarP4(); }

    double pt() const { return object->pt(); }

    float puChargedHadronIso() const { return object->puChargedHadronIso(); }

    double px() const { return object->px(); }

    double py() const { return object->py(); }

    double pz() const { return object->pz(); }

    double r9() const { return object->r9(); }

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

    float scE1x5() const { return object->scE1x5(); }

    float scE2x5Max() const { return object->scE2x5Max(); }

    float scE5x5() const { return object->scE5x5(); }

    int scPixCharge() const { return object->scPixCharge(); }

    float scSigmaEtaEta() const { return object->scSigmaEtaEta(); }

    float scSigmaIEtaIEta() const { return object->scSigmaIEtaIEta(); }

    float shFracInnerHits() const { return object->shFracInnerHits(); }

    const reco::GsfElectron::ShowerShape showerShape() const
    { return object->showerShape(); }

    float sigmaEtaEta() const { return object->sigmaEtaEta(); }

    float sigmaIetaIeta() const { return object->sigmaIetaIeta(); }

    double sigmaIetaIphi() const { return object->sigmaIetaIphi(); }

    double sigmaIphiIphi() const { return object->sigmaIphiIphi(); }

    reco::TrackRef standAloneMuon() const { return object->standAloneMuon(); }

    int status() const { return object->status(); }

    reco::SuperClusterRef superCluster() const
    { return object->superCluster(); }

    float superClusterFbrem() const { return object->superClusterFbrem(); }

    math::XYZPoint superClusterPosition() const
    { return object->superClusterPosition(); }

    double theta() const { return object->theta(); }

    int threeCharge() const { return object->threeCharge(); }

    reco::TrackRef track() const { return object->track(); }

    const reco::GsfElectron::TrackClusterMatching trackClusterMatching() const
    { return object->trackClusterMatching(); }

    bool trackerDrivenSeed() const { return object->trackerDrivenSeed(); }

    const reco::GsfElectron::TrackExtrapolations trackExtrapolations() const
    { return object->trackExtrapolations(); }

    float trackFbrem() const { return object->trackFbrem(); }

    float trackIso() const { return object->trackIso(); }

    pat::IsoDeposit* trackIsoDeposit() const
    { return (pat::IsoDeposit*)object->trackIsoDeposit(); }

    math::XYZVectorF trackMomentumAtCalo() const
    { return object->trackMomentumAtCalo(); }

    math::XYZVectorF trackMomentumAtEleClus() const
    { return object->trackMomentumAtEleClus(); }

    math::XYZVectorF trackMomentumAtVtx() const
    { return object->trackMomentumAtVtx(); }

    math::XYZVectorF trackMomentumAtVtxWithConstraint() const
    { return object->trackMomentumAtVtxWithConstraint(); }

    float trackMomentumError() const { return object->trackMomentumError(); }

    math::XYZVectorF trackMomentumOut() const
    { return object->trackMomentumOut(); }

    math::XYZPointF trackPositionAtCalo() const
    { return object->trackPositionAtCalo(); }

    math::XYZPointF trackPositionAtVtx() const
    { return object->trackPositionAtVtx(); }

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
