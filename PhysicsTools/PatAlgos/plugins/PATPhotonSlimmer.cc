/**
  \class    pat::PATPhotonSlimmer PATPhotonSlimmer.h "PhysicsTools/PatAlgos/interface/PATPhotonSlimmer.h"
  \brief    slimmer of PAT Taus 
*/


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "DataFormats/PatCandidates/interface/Photon.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "FWCore/Utilities/interface/isFinite.h"

namespace pat {

  class PATPhotonSlimmer : public edm::EDProducer {
    public:
      explicit PATPhotonSlimmer(const edm::ParameterSet & iConfig);
      virtual ~PATPhotonSlimmer() { }

      virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

    private:
      edm::EDGetTokenT<edm::View<pat::Photon> > src_;

      StringCutObjectSelector<pat::Photon> dropSuperClusters_, dropBasicClusters_, dropPreshowerClusters_, dropSeedCluster_, dropRecHits_;

      edm::EDGetTokenT<edm::ValueMap<std::vector<reco::PFCandidateRef>>> reco2pf_;
      edm::EDGetTokenT<edm::Association<pat::PackedCandidateCollection>> pf2pc_;
      edm::EDGetTokenT<pat::PackedCandidateCollection>  pc_;
      bool linkToPackedPF_;
      StringCutObjectSelector<pat::Photon> saveNonZSClusterShapes_;
      edm::EDGetTokenT<EcalRecHitCollection> reducedBarrelRecHitCollectionToken_, reducedEndcapRecHitCollectionToken_;
  };

} // namespace

pat::PATPhotonSlimmer::PATPhotonSlimmer(const edm::ParameterSet & iConfig) :
    src_(consumes<edm::View<pat::Photon> >(iConfig.getParameter<edm::InputTag>("src"))),
    dropSuperClusters_(iConfig.getParameter<std::string>("dropSuperCluster")),
    dropBasicClusters_(iConfig.getParameter<std::string>("dropBasicClusters")),
    dropPreshowerClusters_(iConfig.getParameter<std::string>("dropPreshowerClusters")),
    dropSeedCluster_(iConfig.getParameter<std::string>("dropSeedCluster")),
    dropRecHits_(iConfig.getParameter<std::string>("dropRecHits")),
    linkToPackedPF_(iConfig.getParameter<bool>("linkToPackedPFCandidates")),
    saveNonZSClusterShapes_(iConfig.getParameter<std::string>("saveNonZSClusterShapes")),
    reducedBarrelRecHitCollectionToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("reducedBarrelRecHitCollection"))),
    reducedEndcapRecHitCollectionToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("reducedEndcapRecHitCollection")))
{
    produces<std::vector<pat::Photon> >();
    if (linkToPackedPF_) {
        reco2pf_ = consumes<edm::ValueMap<std::vector<reco::PFCandidateRef>>>(iConfig.getParameter<edm::InputTag>("recoToPFMap"));
        pf2pc_   = consumes<edm::Association<pat::PackedCandidateCollection>>(iConfig.getParameter<edm::InputTag>("packedPFCandidates"));
        pc_   = consumes<pat::PackedCandidateCollection>(iConfig.getParameter<edm::InputTag>("packedPFCandidates"));
    }
    mayConsume<EcalRecHitCollection>(edm::InputTag("reducedEcalRecHitsEB"));
    mayConsume<EcalRecHitCollection>(edm::InputTag("reducedEcalRecHitsEE"));
}

void 
pat::PATPhotonSlimmer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {
    using namespace edm;
    using namespace std;

    Handle<View<pat::Photon> >      src;
    iEvent.getByToken(src_, src);

    Handle<edm::ValueMap<std::vector<reco::PFCandidateRef>>> reco2pf;
    Handle<edm::Association<pat::PackedCandidateCollection>> pf2pc;
    Handle<pat::PackedCandidateCollection> pc;
    if (linkToPackedPF_) {
        iEvent.getByToken(reco2pf_, reco2pf);
        iEvent.getByToken(pf2pc_, pf2pc);
        iEvent.getByToken(pc_, pc);
    }
    noZS::EcalClusterLazyTools lazyToolsNoZS(iEvent, iSetup, reducedBarrelRecHitCollectionToken_, reducedEndcapRecHitCollectionToken_);

    auto_ptr<vector<pat::Photon> >  out(new vector<pat::Photon>());
    out->reserve(src->size());

    for (View<pat::Photon>::const_iterator it = src->begin(), ed = src->end(); it != ed; ++it) {
        out->push_back(*it);
        pat::Photon & photon = out->back();

        if (dropSuperClusters_(photon)) { photon.superCluster_.clear(); photon.embeddedSuperCluster_ = false; }
	if (dropBasicClusters_(photon)) { photon.basicClusters_.clear(); }
	if (dropPreshowerClusters_(photon)) { photon.preshowerClusters_.clear(); }
	if (dropSeedCluster_(photon)) { photon.seedCluster_.clear(); photon.embeddedSeedCluster_ = false; }
        if (dropRecHits_(photon)) { photon.recHits_ = EcalRecHitCollection(); photon.embeddedRecHits_ = false; }

        if (linkToPackedPF_) {
            photon.setPackedPFCandidateCollection(edm::RefProd<pat::PackedCandidateCollection>(pc));
            //std::cout << " PAT  photon in  " << src.id() << " comes from " << photon.refToOrig_.id() << ", " << photon.refToOrig_.key() << std::endl;
            edm::RefVector<pat::PackedCandidateCollection> origs;
            for (const reco::PFCandidateRef & pf : (*reco2pf)[photon.refToOrig_]) {
                if (pf2pc->contains(pf.id())) {
                    origs.push_back((*pf2pc)[pf]);
                } //else std::cerr << " Photon linked to a PFCand in " << pf.id() << " while we expect them in " << pf2pc->ids().front().first << "\n";
            }
            //std::cout << "Photon with pt " << photon.pt() << " associated to " << origs.size() << " PF Candidates\n";
            photon.setAssociatedPackedPFCandidates(origs);
            //if there's just one PF Cand then it's me, otherwise I have no univoque parent so my ref will be null 
            if (origs.size() == 1) {
                photon.refToOrig_ = refToPtr(origs[0]);
            } else {
                photon.refToOrig_ = reco::CandidatePtr(pc.id());
            }
        }
        if (saveNonZSClusterShapes_(photon)) {
            std::vector<float> full5x5_locCov = lazyToolsNoZS.localCovariances(*( photon.superCluster()->seed()));
            std::vector<float> full5x5_cov =  lazyToolsNoZS.covariances(*( photon.superCluster()->seed()));

            float r9 = lazyToolsNoZS.e3x3( *( photon.superCluster()->seed())) / photon.superCluster()->rawEnergy() ;
            float sigmaIetaIeta = ( !edm::isNotFinite(full5x5_locCov[0]) ) ? sqrt(full5x5_locCov[0]) : 0;
            float sigmaIetaIphi = full5x5_locCov[1];
            float sigmaIphiIphi = ( !edm::isNotFinite(full5x5_locCov[2]) ) ? sqrt(full5x5_locCov[2]) : 0;
            float e15o55 = lazyToolsNoZS.e1x5( *( photon.superCluster()->seed()) ) / lazyToolsNoZS.e5x5( *( photon.superCluster()->seed()) );
            photon.addUserFloat("sigmaIetaIeta_NoZS", sigmaIetaIeta);
            photon.addUserFloat("sigmaIetaIphi_NoZS", sigmaIetaIphi);
            photon.addUserFloat("sigmaIphiIphi_NoZS", sigmaIphiIphi);
            photon.addUserFloat("r9_NoZS", r9);
            photon.addUserFloat("e1x5_over_e5x5_NoZS", e15o55);

            float full5x5_maxXtal =   lazyToolsNoZS.eMax( *( photon.superCluster()->seed()) );
            float full5x5_e1x5    =   lazyToolsNoZS.e1x5( *( photon.superCluster()->seed()) );
            float full5x5_e2x5    =   lazyToolsNoZS.e2x5Max(  *( photon.superCluster()->seed()) );
            float full5x5_e3x3    =   lazyToolsNoZS.e3x3(  *( photon.superCluster()->seed()) );
            float full5x5_e5x5    =   lazyToolsNoZS.e5x5( *( photon.superCluster()->seed()) );

            float full5x5_sigmaEtaEta = sqrt(full5x5_cov[0]);
            float full5x5_sigmaIetaIeta = ( !edm::isNotFinite(full5x5_locCov[0]) ) ? sqrt(full5x5_locCov[0]) : 0; 

            /// fill full5x5 shower shape block
            reco::Photon::ShowerShape  full5x5_showerShape;
            full5x5_showerShape.e1x5= full5x5_e1x5;
            full5x5_showerShape.e2x5= full5x5_e2x5;
            full5x5_showerShape.e3x3= full5x5_e3x3;
            full5x5_showerShape.e5x5= full5x5_e5x5;
            full5x5_showerShape.maxEnergyXtal =  full5x5_maxXtal;
            full5x5_showerShape.sigmaEtaEta =    full5x5_sigmaEtaEta;
            full5x5_showerShape.sigmaIetaIeta =  full5x5_sigmaIetaIeta;
            photon.full5x5_setShowerShapeVariables ( full5x5_showerShape ); 

        }
     }

    iEvent.put(out);
}

#include "FWCore/Framework/interface/MakerMacros.h"
using namespace pat;
DEFINE_FWK_MODULE(PATPhotonSlimmer);
