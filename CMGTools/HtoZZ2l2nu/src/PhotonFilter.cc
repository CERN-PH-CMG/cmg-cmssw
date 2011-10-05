#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

#include "TObjArray.h"
#include "TString.h"
#include "TObjString.h"

using namespace std;

namespace photon{

  //
  std::pair<std::string,double> getPhotonTrigThreshold(edm::Handle<edm::TriggerResults> &triggerBitsH,
						       const edm::TriggerNames &triggerNames,
						       std::vector<std::string> &gammaTriggers)
  {
    double maxthr(0);
    std::string selTrigger("");
    int ntrigs = triggerBitsH->size();
    for(int itrig=0; itrig<ntrigs; itrig++)
      {
        //check if the trigger fired
        if( !triggerBitsH->wasrun(itrig) ) continue;
        if( triggerBitsH->error(itrig) ) continue;
        if( !triggerBitsH->accept(itrig) ) continue;
	
	//now check if trigger is to be kept                                                                                                                                                                                                                                              
	std::string trigName = triggerNames.triggerName(itrig);
	if( trigName.find("Photon") == std::string::npos ) continue;
	
        bool keepTrigger(false);
        for(std::vector<std::string>::iterator tIt = gammaTriggers.begin(); tIt != gammaTriggers.end(); tIt++)
          {
            if(trigName.find(*tIt) == std::string::npos) continue;
            keepTrigger=true;
            break;
          }
        if(!keepTrigger) continue;

        //get the trigger threshold
        TString fireTrigger(trigName);
        TObjArray *tkns=fireTrigger.Tokenize("_");
        if(tkns->GetEntriesFast()<2) continue;
        TString phoName=((TObjString *)tkns->At(1))->GetString();
        phoName.ReplaceAll("Photon","");
        Int_t thr=phoName.Atoi();
	
        if(thr<maxthr) continue;
	maxthr=thr;
	selTrigger=trigName;
      }
    return std::pair<std::string,double>(selTrigger,maxthr);
  }
  
  //
  CandidateCollection filter(edm::Handle<edm::View<reco::Candidate> > &hPhoton,
			     EcalClusterLazyTools &lazyTool,
			     edm::Handle<EcalRecHitCollection> ebrechits,
			     const edm::ParameterSet &iConfig,
			     double minEt)
  {
    CandidateCollection selPhotons;
    if(!hPhoton.isValid()) return selPhotons;
    if(hPhoton->size()==0) return selPhotons;

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
	  
	  //kinematics
	  float et = photon->et();
	  float eta = photon->eta(); //scphoton->eta();
	  bool fallsInCrackRegion( fabs(eta)>1.4442 && fabs(eta)<1.566 );
	  bool isGood( et>minEt && fabs(eta)<maxEta && !fallsInCrackRegion);
	  
	  //pixel seed veto
	  bool isPrompt(true);
	  try{
	    bool hasPixelSeed = photon->hasPixelSeed();
	    isPrompt = !hasPixelSeed;
	  }catch(std::exception &e){
	  }

	  //id
	  float r9 = photon->r9();
	  float sihih = photon->sigmaIetaIeta();
	  float sipip(999999.);
	  float hoe = photon->hadronicOverEm();
	  bool hasId(hoe<maxHoE);
	  bool hasOutofTime(false);
	  
	  if(photon->isEB())
	    {
	      //these require the photon core to be present
	      try{
		reco::SuperClusterRef scphoton = photon->superCluster();
		const reco::CaloClusterPtr  seed_clu = scphoton->seed();
		std::vector<float> cov = lazyTool.localCovariances(*seed_clu);
		sipip = cov[2];
		DetId id=scphoton->seed()->hitsAndFractions()[0].first;
		EcalRecHitCollection::const_iterator seedcry_rh = ebrechits->find( id );
		if( seedcry_rh != ebrechits->end() ) hasOutofTime = seedcry_rh->checkFlag(EcalRecHit::kOutOfTime);
	      }
	      catch(std::exception &e){
	      }
	      
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



