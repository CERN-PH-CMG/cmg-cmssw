#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace photon{
  
  //
  CandidateCollection filter(edm::Handle<edm::View<reco::Candidate> > &hPhoton,
			     EcalClusterLazyTools &lazyTool,
			     edm::Handle<EcalRecHitCollection> ebrechits,
			     double rho,
			     const edm::ParameterSet &iConfig)
  {
    CandidateCollection selPhotons;

    try{
      
      //20
      double minEt = iConfig.getParameter<double>("minEt");

      //maxEta=2.5
      double maxEta = iConfig.getParameter<double>("maxEta");
      //0.013
      double minSihihEB=iConfig.getParameter<double>("minSihihEB");

      //0.03
      double minSihihEE=iConfig.getParameter<double>("minSihihEE");

      //maxHoE=0.05
      double maxHoE=iConfig.getParameter<double>("maxHoE");
      
      //0
      double minSipipEb=iConfig.getParameter<double>("minSipipEB");

      //iterate over the photons
      for(size_t iPhoton=0; iPhoton< hPhoton.product()->size(); ++iPhoton)
        {
	  reco::CandidatePtr photonPtr = hPhoton->ptrAt(iPhoton);
	  const pat::Photon *photon = dynamic_cast<const pat::Photon *>( photonPtr.get() );
	  reco::SuperClusterRef scphoton = photon->superCluster();
	  if(!scphoton.isNull()) continue;
	  const reco::CaloClusterPtr  seed_clu = scphoton->seed();
  
	  //kinematics
	  float et = photon->et();
	  float eta = scphoton->eta();
	  bool fallsInCrackRegion( fabs(eta)>1.4442 && fabs(eta)<1.566 );
	  bool isGood( et>minEt && fabs(eta)<maxEta && !fallsInCrackRegion);

	  //pixel/converion veto
	  bool hasPixelSeed = photon->hasPixelSeed();
	  bool hasConversionTracks = photon->hasConversionTracks();
	  bool nconv= photon->conversions().size();
	  bool isPrompt( !hasPixelSeed && !hasConversionTracks && nconv==0);

	  //id
	  //float r9 = photon->r9();
	  float sihih = photon->sigmaIetaIeta();
	  std::vector<float> cov = lazyTool.localCovariances(*seed_clu);
	  float sipip = cov[2];
	  float hoe = photon->hadronicOverEm();
	  bool hasId(hoe<maxHoE);
	  DetId id=scphoton->seed()->hitsAndFractions()[0].first;
	  if(photon->isEB())
	    {
	      EcalRecHitCollection::const_iterator seedcry_rh = ebrechits->find( id );
	      if( seedcry_rh != ebrechits->end() ) hasId &= (  !seedcry_rh->checkFlag(EcalRecHit::kOutOfTime) );
	      hasId &= (sipip>minSipipEb);
	      hasId &= (sihih>minSihihEB);
	    }
	  else
	    hasId &= (sihih>minSihihEE);
	    

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



