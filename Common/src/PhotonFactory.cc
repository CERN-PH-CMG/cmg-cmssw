#include "CMGTools/Common/interface/PhotonFactory.h"

cmg::PhotonFactory::event_ptr
cmg::PhotonFactory::create(const edm::Event& iEvent,
                           const edm::EventSetup&)
{
  edm::Handle<reco::PFCandidateCollection> cands;

  cmg::PhotonFactory::event_ptr result(new cmg::PhotonFactory::collection);
  iEvent.getByLabel(photonLabel_, cands);
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
  AbsVetos allVetos = allIsoPar_.getAbsVetoes();

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
  for(unsigned int i = 0; i<allVetos.size(); i++){
    allVetos[i]->centerOn(Eta,Phi);
  }

  output->chargedHadronIso_ = (*isoDepWithCharged)[input].depositAndCountWithin( chargedHadronIsoPar_.coneSize(), chargedHadronVetos, false ).first;
  output->neutralHadronIso_ = (*isoDepWithNeutral)[input].depositAndCountWithin( neutralHadronIsoPar_.coneSize(), neutralHadronVetos, false ).first;
  output->photonIso_        = (*isoDepWithPhotons)[input].depositAndCountWithin( photonsIsoPar_.coneSize(), photonsVetos, false ).first;
  output->puChargedHadronIso_ = (*isoDepWithPU)[input].depositAndCountWithin( puIsoPar_.coneSize(), puVetos, false ).first;

  output->chargedHadronVeto_ = (*isoDepWithCharged)[input].depositAndCountWithin( chargedHadronIsoPar_.coneSize(), allVetos, false ).first;
  output->neutralHadronVeto_ = (*isoDepWithNeutral)[input].depositAndCountWithin( neutralHadronIsoPar_.coneSize(), allVetos, false ).first;
  output->photonVeto_        = (*isoDepWithPhotons)[input].depositAndCountWithin( photonsIsoPar_.coneSize(), allVetos, false ).first;

  output->chargedHadronVeto_ -= output->chargedHadronIso_;
  output->neutralHadronVeto_ -= output->neutralHadronIso_;
  output->photonVeto_ -= output->photonIso_;

  //std::cout << "pt, eta, phi = " << output->pt() << " " << output->eta() << " " << output->phi() << std::endl;
  //std::cout << "Isolations   = " << output->chargedHadronIso() << " " << output->neutralHadronIso() << " " << output->photonIso() << std::endl;
  //std::cout << "Vetoes       = " << output->chargedHadronVeto() << " " << output->neutralHadronVeto() << " " << output->photonVeto() << std::endl;
}

//--------------------------------------------------------------------
