#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace photon{
  
  //
  CandidateCollection filter(edm::Handle<edm::View<reco::Candidate> > &hPhoton,
			     double rho,
			     const edm::ParameterSet &iConfig)
  {
    CandidateCollection selPhotons;

    try{
      //config parameters                                                                                                                                                 
      double minEt = iConfig.getParameter<double>("minEt");
      double maxEta = iConfig.getParameter<double>("maxEta");
      //maxHoE=0.05
      double maxHoE=iConfig.getParameter<double>("maxHoE");
      //aeff=0.17;
      double aeff=iConfig.getParameter<double>("aeff");

      //iterate over the electrons
      for(size_t iPhoton=0; iPhoton< hPhoton.product()->size(); ++iPhoton)
        {
	  reco::CandidatePtr photonPtr = hPhoton->ptrAt(iPhoton);
	  const pat::Photon *photon = dynamic_cast<const pat::Photon *>( photonPtr.get() );

	  //kinematics
	  float et = photon->et();
	  float eta = photon->eta();
	  
	  //pixel/converion veto
	  bool hasPixelSeed = photon->hasPixelSeed();
	  bool hasConversionTracks = photon->hasConversionTracks();
	  bool nconv= photon->conversions().size();
	  bool isPrompt( !hasPixelSeed && !hasConversionTracks && nconv==0);

	  //id
	  float r9 = photon->r9();
	  float sihih = photon->sigmaIetaIeta();
	  float hoe = photon->hadronicOverEm();
	  bool hasId(hoe<maxHoE);
	  hasId &= sihih>0.001;

	  //isolation (apply rho correction? relIso )
	  float trkIso = photon->trkSumPtSolidConeDR04();
	  float ecalIso = photon->ecalRecHitSumEtConeDR04();
	  float hcalIso  = photon->hcalTowerSumEtConeDR04();
	  bool isIso( trkIso < 2.0 + 0.001*et );
	  isIso &= ( ecalIso < 4.2 + 0.003*et );
	  isIso &= ( hcalIso < 2.2 + 0.001*et );
	  
	  if(!isPrompt || !hasId || !isIso) continue;
	  selPhotons.push_back( photonPtr );
	}
    }catch(std::exception &e){
      cout << "[photon::filter] failed with " << e.what() << endl;
    }

    return selPhotons;
  }
}



