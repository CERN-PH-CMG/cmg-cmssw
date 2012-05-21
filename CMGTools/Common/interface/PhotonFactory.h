#ifndef PHOTONFACTORY_H_
#define PHOTONFACTORY_H_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/Photon.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"
#include "CMGTools/Common/interface/Factory.h"

#include "DataFormats/RecoCandidate/interface/IsoDepositVetos.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
//#include "DataFormats/PatCandidates/interface/Isolation.h"

#include "CMGTools/Common/interface/SpecificIsolation.h"


namespace cmg {

  typedef edm::Handle< edm::ValueMap<reco::IsoDeposit> > IsoDepositMap;
  typedef edm::Handle< edm::ValueMap<double> > IsolationValueMap;
  typedef reco::isodeposit::AbsVetos AbsVetos;
   

  class PhotonFactory : public Factory<cmg::Photon> {
  public:
    PhotonFactory(const edm::ParameterSet& ps):
      photonLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
      pfLabel_(ps.getParameter<edm::InputTag>("pfCollection")),
      muonLabel_(ps.getParameter<edm::InputTag>("muonCollection")),
      isoDepWithChargedLabel_(ps.getParameter<edm::InputTag>("isoDepWithCharged")),
      isoDepWithPhotonsLabel_(ps.getParameter<edm::InputTag>("isoDepWithPhotons")),
      isoDepWithNeutralLabel_(ps.getParameter<edm::InputTag>("isoDepWithPU")),
      isoDepWithPULabel_(ps.getParameter<edm::InputTag>("isoDepWithNeutral"))
	{
	  chargedHadronIsoPar_.initialize( ps.getParameter<edm::ParameterSet>("chargedHadronIsoPar"));
	  neutralHadronIsoPar_.initialize( ps.getParameter<edm::ParameterSet>("neutralHadronIsoPar"));
	  photonsIsoPar_.initialize( ps.getParameter<edm::ParameterSet>("photonsIsoPar"));
	  puIsoPar_.initialize( ps.getParameter<edm::ParameterSet>("puIsoPar"));
	}

    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
    virtual void set(const pat::PhotonPtr& input, cmg::Photon* const output) const;

  private:
    const edm::InputTag photonLabel_;
    const edm::InputTag pfLabel_;
    const edm::InputTag muonLabel_;
    const edm::InputTag isoDepWithChargedLabel_;
    const edm::InputTag isoDepWithPhotonsLabel_;
    const edm::InputTag isoDepWithNeutralLabel_;
    const edm::InputTag isoDepWithPULabel_;

    IsoDepositMap isoDepWithCharged, isoDepWithPhotons, isoDepWithNeutral, isoDepWithPU;

    /// parameters for charged hadron isolation value
    SpecificIsolation chargedHadronIsoPar_;
    
    /// parameters for neutral hadron isolation value
    SpecificIsolation neutralHadronIsoPar_;
    
    /// parameters for photon isolation value
    SpecificIsolation photonsIsoPar_;
   
    /// parameters for PU isolation value
    SpecificIsolation puIsoPar_;
   
  };

} // namespace cmg

#endif /*PHOTONFACTORY_H_*/
