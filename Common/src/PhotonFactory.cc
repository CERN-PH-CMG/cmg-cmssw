#include "CMGTools/Common/interface/PhotonFactory.h"

cmg::PhotonFactory::event_ptr
cmg::PhotonFactory::create(const edm::Event& iEvent,
                           const edm::EventSetup& iSetup)
{
  edm::Handle<reco::PFCandidateCollection> cands;
  edm::Handle<reco::PFCandidateCollection> pfCands;
  edm::Handle<pat::MuonCollection> muonCands;

  cmg::PhotonFactory::event_ptr result(new cmg::PhotonFactory::collection);
  iEvent.getByLabel(photonLabel_, cands);
  iEvent.getByLabel(pfLabel_,pfCands);
  iEvent.getByLabel(muonLabel_, muonCands);
  iEvent.getByLabel(isoDepWithChargedLabel_, isoDepWithCharged);
  iEvent.getByLabel(isoDepWithPhotonsLabel_, isoDepWithPhotons);
  iEvent.getByLabel(isoDepWithNeutralLabel_, isoDepWithNeutral);
  iEvent.getByLabel(isoDepWithPULabel_, isoDepWithPU);

  for(size_t i = 0, end = cands->size(); i != end; ++i) {

    pat::PhotonPtr photonPtr(cands, i);
    cmg::Photon photon(photonPtr);
    set(photonPtr, &photon);

    result->push_back(photon);
  }

  // Now add the ECAL energy swallowed by the muons
  unsigned index = 0;
  for(pat::MuonCollection::const_iterator m = muonCands->begin();
      m != muonCands->end(); ++m, ++index){
  			
    pat::MuonPtr muonPtr(muonCands, index);

    edm::Ptr<reco::Candidate> muonref = muonPtr->originalObjectRef();
    
    // Find the corresponding PF muon
    bool isPF = false;
    unsigned int indexPF = 0;
    for (unsigned i = 0, end = pfCands->size(); i!=end; ++i) { 
      pat::PhotonPtr pfCand(pfCands,i);
      if ( abs(pfCand->pdgId()) != 13 ) continue;
      const reco::MuonRef mref = pfCand->muonRef();
      if (mref.isNonnull() ) {
	if (muonref.key() == mref.key()) {
	  isPF = true; 
	  indexPF = i;
	  break;
	}
      }
    }
	
    // Found !
    if( !isPF  ) continue;
    pat::PhotonPtr pfmuon(pfCands, indexPF);
    cmg::Photon photon(pfmuon);

    // Keep only those with a swallowed ECAL energy > 2 GeV
    if ( pfmuon->ecalEnergy() < 2.0 ) continue;
    // Request the pt to be in excess of 2 GeV/c
    double sintet = pfmuon->pt()/pfmuon->energy();
    double phpt = pfmuon->ecalEnergy() * sintet;
    if ( phpt < 2.0 ) continue;

    double ratio = pfmuon->ecalEnergy()/pfmuon->energy();
    photon.setP4(photon.p4() * ratio);
    photon.chargedHadronIso_ = muonPtr->userIsolation(pat::PfChargedHadronIso);
    photon.chargedAllIso_ = muonPtr->userIsolation(pat::PfChargedAllIso);
    photon.puChargedHadronIso_ = muonPtr->userIsolation(pat::PfPUChargedHadronIso);
    photon.neutralHadronIso_ = muonPtr->userIsolation(pat::PfNeutralHadronIso);
    photon.photonIso_ = muonPtr->userIsolation(pat::PfGammaIso);
    photon.charge_ = 0.;
    photon.isFromMuon_ = true;

    result->push_back(photon);

  }

  return result;

}

//--------------------------------------------------------------------

void
cmg::PhotonFactory::set(const pat::PhotonPtr& input, cmg::Photon* const output) const
{

  output->charge_ = input->charge();

  //use all the vetos etc to recalcuate the isolation
    
  const double Eta = input->eta();
  const double Phi = input->phi();
  //std::cout << "pt, eta, phi = " << input->pt() << " " << input->eta() << " " << input->phi() << std::endl; 

  // computing isolation value in cone for each
  // type of particle. 
  
  // retrieve the AbsVetos from the SpecificIsolation
  AbsVetos chargedHadronVetos = chargedHadronIsoPar_.getAbsVetoes();
  AbsVetos neutralHadronVetos = neutralHadronIsoPar_.getAbsVetoes();
  AbsVetos photonsVetos = photonsIsoPar_.getAbsVetoes();
  AbsVetos puVetos = puIsoPar_.getAbsVetoes();

  // center the vetoes around the lepton
  for(unsigned int i = 0; i<chargedHadronVetos.size(); i++){
    chargedHadronVetos[i]->centerOn(Eta,Phi);
  }
  for(unsigned int i = 0; i<neutralHadronVetos.size(); i++){
    neutralHadronVetos[i]->centerOn(Eta,Phi);
  }
  for(unsigned int i = 0; i<photonsVetos.size(); i++){
    photonsVetos[i]->centerOn(Eta,Phi);
  }
  for(unsigned int i = 0; i<puVetos.size(); i++){
    puVetos[i]->centerOn(Eta,Phi);
  }

  output->chargedHadronIso_ = (*isoDepWithCharged)[input].depositAndCountWithin( chargedHadronIsoPar_.coneSize(), chargedHadronVetos, false ).first;
  output->neutralHadronIso_ = (*isoDepWithNeutral)[input].depositAndCountWithin( neutralHadronIsoPar_.coneSize(), neutralHadronVetos, false ).first;
  output->photonIso_        = (*isoDepWithPhotons)[input].depositAndCountWithin( photonsIsoPar_.coneSize(), photonsVetos, false ).first;
  output->puChargedHadronIso_ = (*isoDepWithPU)[input].depositAndCountWithin( puIsoPar_.coneSize(), puVetos, false ).first;

}

//--------------------------------------------------------------------
