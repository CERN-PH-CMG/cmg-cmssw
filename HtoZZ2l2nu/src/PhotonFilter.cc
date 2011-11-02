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
			     edm::Handle<std::vector<reco::Track> > & hTracks,
			     edm::Handle<edm::View<reco::Candidate> > &hEle,
			     edm::Handle<std::vector<reco::Conversion> > &hConversions,
			     std::vector<reco::VertexRef> &selVertices,
			     double rho,
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
      std::string id = iConfig.getParameter<string>("id");
     
      //iterate over the photons
      for(size_t iPhoton=0; iPhoton< hPhoton.product()->size(); ++iPhoton)
        {
	  reco::CandidatePtr photonPtr = hPhoton->ptrAt(iPhoton);
	  const pat::Photon *photon = dynamic_cast<const pat::Photon *>( photonPtr.get() );
	
	  //kinematics
	  float et = photon->et();
	  float eta = photon->eta(); 
	  bool isPrompt(true);
	  
	  //these require photon core
	  try{
	    
	    eta = photon->superCluster()->eta();
	    bool hasPixelSeed = photon->hasPixelSeed();
	    isPrompt = !hasPixelSeed;
	    int tType(0);
	    float minDr(99999.);
	    if(hTracks.isValid())
	      {
		for(std::vector<reco::Track>::const_iterator tit = hTracks->begin(); tit != hTracks->end(); tit++)
		  {
		    float dr =deltaR(tit->eta(),tit->phi(),photon->eta(),photon->phi());
		    if(dr>minDr) continue;
		    minDr=dr;
		    tType=1;
		  }
	      }
	    if(hEle.isValid())
	      {
		for(edm::View<reco::Candidate>::const_iterator eit = hEle->begin(); eit != hEle->end(); eit++)
		  {
		    float dr =deltaR(eit->eta(),eit->phi(),photon->eta(),photon->phi());
		    if(dr>minDr) continue;
		    minDr=dr;
		    tType=11;
		  }
	      }
	    if(minDr<0.05) 
	      { 
		//cout << "Vetoing photon - px seed=" << hasPixelSeed << " but closest trk(" << tType << ")=" << minDr << endl;
		isPrompt=false;
	      }

	    //finally math to conversion
	    const reco::Vertex *primVertex=0;
	    if(selVertices.size()) primVertex=selVertices[0].get();
	    const reco::Conversion *phoConvMatch = photon::matchToConversion( photon, primVertex, hConversions );
	    if(phoConvMatch) isPrompt=false;
	    
	  }catch(std::exception &e){
	    cout << "Photon core is missing (assuming photon eta and no pixel seed)" << endl;
	  }
	  

	  bool fallsInCrackRegion( fabs(eta)>1.4442 && fabs(eta)<1.566 );
	  bool isGood( et>minEt && fabs(eta)<maxEta && !fallsInCrackRegion);

	  //id
	  float r9 = photon->r9();
	  float sihih = photon->sigmaIetaIeta();
	  float sipip(999999.);
	  float hoe = photon->hadronicOverEm();
	  bool hasId(hoe<maxHoE);
	  bool hasOutofTime(false);
	  bool hasCiCId( true );
	  if(!id.empty()) hasCiCId = photon->photonID(id);

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
		cout << "Calo clustered not valid for EB photon (assuming defaults)" << endl;
	      }
	      
	      hasId &= (!hasOutofTime);
	      hasId &= (sipip>minSipipEb);
	      hasId &= (sihih<maxSihihEB);
	    }
	  else
	    hasId &= (sihih<maxSihihEE);
	  
	  //isolation (apply rho correction? relIso )
	  float Aeff_trk  (photon->isEB() ? 0.317 : 0.269),
	    Aeff_ecal     (photon->isEB() ? 0.162 : 0.071),
	    Aeff_hcal     (photon->isEB() ? 0.042 : 0.095);
	  float trkIso  = max(photon->trkSumPtSolidConeDR04()  - rho*Aeff_trk,  0.);
	  float ecalIso = max(photon->ecalRecHitSumEtConeDR04()- rho*Aeff_ecal, 0.);
	  float hcalIso = max(photon->hcalTowerSumEtConeDR04() - rho*Aeff_hcal, 0.);
	  bool isIso( (trkIso-1.5)/et  < 0.001);
	  isIso &=  ( (ecalIso-2.0)/et < 0.006);
	  isIso &=  ( (hcalIso-2.0)/et < 0.0025);
	  if(!isGood || !isPrompt || !hasId || !hasCiCId || !isIso) continue;
	  selPhotons.push_back( photonPtr );
	}
    }catch(std::exception &e){
      cout << "[photon::filter] failed with " << e.what() << endl;
    }

    return selPhotons;
  }

  //
  const reco::Conversion *matchToConversion(const pat::Photon *pho, const reco::Vertex *pv, edm::Handle<std::vector<reco::Conversion> > &hConversions)
  {
    const reco::Conversion *matchConv=0;
    if(pho==0 || !hConversions.isValid()) return matchConv;
    if(hConversions->size()==0) return matchConv;

    //photon kinematics
    double phoEta = pho->superCluster()->eta();
    double phoPhi = pho->superCluster()->phi();
    
    //find closest conversion in phase space
    float dEtaMin(9999.), dPhiMin(9999.);   
    for(std::vector<reco::Conversion>::const_iterator it = hConversions->begin(); it != hConversions->end(); it++)
      {
	LorentzVector pairMom( it->refittedPair4Momentum().Px(),
			       it->refittedPair4Momentum().Py(),
			       it->refittedPair4Momentum().Pz(),
			       it->refittedPair4Momentum().E() );
	if (pairMom.pt() < 1 ) continue;    
	
	const reco::Vertex &convVtx = it->conversionVertex();	
	if(!convVtx.isValid()) continue;
	
	double convEta = pairMom.eta();
	double dEta = convEta-phoEta;

	double convPhi = pairMom.phi();
	double dPhi = deltaPhi(convPhi,phoPhi);
	
	if ( fabs(dEta) > dEtaMin || fabs(dPhi) > dPhiMin ) continue; 
	dEtaMin=  fabs(dEta);
	dPhiMin=  fabs(dPhi);
	matchConv = &(*it);
      }
    
    //reset if too far away 
    if ( dEtaMin > 0.1 || dPhiMin > 0.1 ) matchConv=0;

    return matchConv;
  }

}



