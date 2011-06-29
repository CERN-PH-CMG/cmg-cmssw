#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace photon{
  
  //
  CandidateCollection filter(edm::Handle<edm::View<reco::Candidate> > &hPhoton,
			     EcalClusterLazyTools &lazyTool,
			     edm::Handle<EcalRecHitCollection> ebrechits,
			     const edm::ParameterSet &iConfig,
			     double minEt)
  {
    CandidateCollection selPhotons;

    try{

      minEt = max(minEt,iConfig.getParameter<double>("minEt"));
      double maxEta = iConfig.getParameter<double>("maxEta");
      double maxSihihEB=iConfig.getParameter<double>("maxSihihEB");
      double maxSihihEE=iConfig.getParameter<double>("maxSihihEE");
      double maxHoE=iConfig.getParameter<double>("maxHoE");
      double minSipipEb=iConfig.getParameter<double>("minSipipEB");

      //iterate over the photons
      for(size_t iPhoton=0; iPhoton< hPhoton.product()->size(); ++iPhoton)
        {
	  reco::CandidatePtr photonPtr = hPhoton->ptrAt(iPhoton);
	  const pat::Photon *photon = dynamic_cast<const pat::Photon *>( photonPtr.get() );
	  reco::SuperClusterRef scphoton = photon->superCluster();
	  if(scphoton.isNull()) continue;
	  const reco::CaloClusterPtr  seed_clu = scphoton->seed();
  
	  //kinematics
	  float et = photon->et();
	  float eta = scphoton->eta();
	  bool fallsInCrackRegion( fabs(eta)>1.4442 && fabs(eta)<1.566 );
	  bool isGood( et>minEt && fabs(eta)<maxEta && !fallsInCrackRegion);

	  //pixel seed veto
	  bool hasPixelSeed = photon->hasPixelSeed();
	  bool isPrompt( !hasPixelSeed ); 

	  //id
	  //float r9 = photon->r9();
	  float sihih = photon->sigmaIetaIeta();
	  std::vector<float> cov = lazyTool.localCovariances(*seed_clu);
	  float sipip = cov[2];
	  float hoe = photon->hadronicOverEm();
	  bool hasId(hoe<maxHoE);
	  DetId id=scphoton->seed()->hitsAndFractions()[0].first;
	  bool hasOutofTime(false);
	  if(photon->isEB())
	    {
	      EcalRecHitCollection::const_iterator seedcry_rh = ebrechits->find( id );
	      if( seedcry_rh != ebrechits->end() ) hasOutofTime = seedcry_rh->checkFlag(EcalRecHit::kOutOfTime);
	      hasId &= (!hasOutofTime);
	      hasId &= (sipip>minSipipEb);
	      hasId &= (sihih<maxSihihEB);
	    }
	  else
	    hasId &= (sihih<maxSihihEE);
	    

	  //isolation (apply rho correction? relIso )
	  float trkIso = photon->trkSumPtSolidConeDR04();
	  float ecalIso = photon->ecalRecHitSumEtConeDR04();
	  float hcalIso  = photon->hcalTowerSumEtConeDR04();
	  bool isIso( trkIso < 2.0 + 0.001*et );
	  isIso &= ( ecalIso < 4.2 + 0.003*et );
	  isIso &= ( hcalIso < 2.2 + 0.001*et );

	  if(!isGood || !isPrompt || !hasId || !isIso) continue;
	  selPhotons.push_back( photonPtr );
	}
    }catch(std::exception &e){
      cout << "[photon::filter] failed with " << e.what() << endl;
    }

    return selPhotons;
  }
}



