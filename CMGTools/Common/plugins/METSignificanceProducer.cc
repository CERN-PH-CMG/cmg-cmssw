
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <sstream>

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "RecoMET/METAlgorithms/interface/SignAlgoResolutions.h"
#include "RecoMET/METAlgorithms/interface/SigInputObj.h"
#include "RecoMET/METAlgorithms/interface/significanceAlgo.h"

#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"
//#include <TMatrixD.h>

class METSignificanceProducer : public edm::EDProducer {

public:
  
  explicit METSignificanceProducer(const edm::ParameterSet & iConfig);
  virtual ~METSignificanceProducer() {}
  
private:
  

  template <typename P>
  void addPFMEtSignObjects(std::vector<metsig::SigInputObj>& metSignObjects, edm::Handle< std::vector< P > > & particles) const{     
    //code copied and modified from TauAnalysis package:
    //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/TauAnalysis/CandidateTools/src/PFMEtSignInterface.cc v1.4
    for ( typename std::vector< P >::const_iterator particle = particles->begin(); particle != particles->end(); ++particle ) {
      double pt   = particle->pt();
      double eta  = particle->eta();
      double phi  = particle->phi();
      
      //resolutions depend on the type of particle
      if ( dynamic_cast<const pat::Electron*>(&(*particle)) != 0 ) {
	std::string particleType = "electron";
	// WARNING: SignAlgoResolutions::PFtype2 needs to be kept in sync with reco::PFCandidate::e !!
	double dpt  = pfMEtResolution_.eval(metsig::PFtype2, metsig::ET,  pt, phi, eta);
	double dphi = pfMEtResolution_.eval(metsig::PFtype2, metsig::PHI, pt, phi, eta);
	metSignObjects.push_back(metsig::SigInputObj(particleType, pt, phi, dpt, dphi));
      } else if ( dynamic_cast<const pat::Muon*>(&(*particle)) != 0 ) {
	std::string particleType = "muon";
	double dpt, dphi;
	const pat::Muon* patMuon = dynamic_cast<const pat::Muon*>(&(*particle));
	if ( patMuon->track().isNonnull() && patMuon->track().isAvailable() ) {
	  dpt  = patMuon->track()->ptError();
	  dphi = pt*patMuon->track()->phiError(); // CV: pt*dphi is indeed correct
	} else {
	  // WARNING: SignAlgoResolutions::PFtype3 needs to be kept in sync with reco::PFCandidate::mu !!
	  dpt  = pfMEtResolution_.eval(metsig::PFtype3, metsig::ET,  pt, phi, eta);
	  dphi = pfMEtResolution_.eval(metsig::PFtype3, metsig::PHI, pt, phi, eta);
	}
	metSignObjects.push_back(metsig::SigInputObj(particleType, pt, phi, dpt, dphi));
      } else if ( dynamic_cast<const reco::PFJet*>(&(*particle)) != 0 ) {
	const reco::PFJet* pfJet = dynamic_cast<const reco::PFJet*>(&(*particle));
	metSignObjects.push_back(pfMEtResolution_.evalPFJet(pfJet));
      } else if ( dynamic_cast<const reco::PFCandidate*>(&(*particle)) != 0 ) {
	const reco::PFCandidate* pfCandidate = dynamic_cast<const reco::PFCandidate*>(&(*particle));
	metSignObjects.push_back(pfMEtResolution_.evalPF(pfCandidate));
      } else throw cms::Exception("addPFMEtSignObjects")
	  << "Invalid type of particle:"
	  << " valid types = { pat::Electron, pat::Muon, pat::Tau, reco::PFJet, reco::PFCandidate } !!\n";
    }
  }




  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  metsig::SignAlgoResolutions pfMEtResolution_;
  const edm::InputTag inputPATMuons_;
  const edm::InputTag inputPATElectrons_;
  const edm::InputTag inputPFJets_;
  const edm::InputTag inputPFCandidates_;
  


  bool verbose_;
};


METSignificanceProducer::METSignificanceProducer(const edm::ParameterSet & iConfig) : 
  pfMEtResolution_(metsig::SignAlgoResolutions(iConfig.getParameter<edm::ParameterSet>("resolution"))),
  inputPATMuons_(iConfig.getParameter<edm::InputTag>("inputPATMuons")),
  inputPATElectrons_(iConfig.getParameter<edm::InputTag>("inputPATElectrons")),
  inputPFJets_(iConfig.getParameter<edm::InputTag>("inputPFJets")),
  inputPFCandidates_(iConfig.getParameter<edm::InputTag>("inputPFCandidates")),
  verbose_(iConfig.getUntrackedParameter<bool>("verbose", false ))
{
  produces< cmg::METSignificance >();
}


void METSignificanceProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  //Calculate the PFMET significance
  std::vector<metsig::SigInputObj> pfMEtSignObjects;  
    
  if(inputPATMuons_.label()!=""){
    edm::Handle< std::vector<pat::Muon> > patMuons;
    iEvent.getByLabel(inputPATMuons_, patMuons);
    addPFMEtSignObjects<pat::Muon>(pfMEtSignObjects,patMuons);
  }

  if(inputPATElectrons_.label()!=""){
    edm::Handle< std::vector<pat::Electron> > patElectrons;
    iEvent.getByLabel(inputPATElectrons_, patElectrons);
    addPFMEtSignObjects<pat::Electron>(pfMEtSignObjects,patElectrons);
  }
  
  if(inputPFJets_.label()!=""){
    edm::Handle<reco::PFJetCollection> pfJets;
    iEvent.getByLabel(inputPFJets_, pfJets);
    addPFMEtSignObjects<reco::PFJet>(pfMEtSignObjects,pfJets);
  }

  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  iEvent.getByLabel(inputPFCandidates_, pfCandidates);//PFCands after removing muons,electrons, and jets
  addPFMEtSignObjects<reco::PFCandidate>(pfMEtSignObjects,pfCandidates);
  
  metsig::significanceAlgo pfMEtSignAlgorithm;
  pfMEtSignAlgorithm.addObjects(pfMEtSignObjects);

  std::auto_ptr< cmg::METSignificance > pOut( new cmg::METSignificance(pfMEtSignAlgorithm.getSignifMatrix()) );

  iEvent.put( pOut ); 
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(METSignificanceProducer);
