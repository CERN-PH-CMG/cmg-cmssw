// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <DataFormats/PatCandidates/interface/Electron.h>


#include<vector>



class PATElectronMomentumEmbedder : public edm::EDProducer {
    public:
        explicit PATElectronMomentumEmbedder(const edm::ParameterSet&);
        ~PATElectronMomentumEmbedder();

    private:
        virtual void beginJob() ;
        virtual void produce(edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

        // ----------member data ---------------------------
        edm::InputTag electrons_;
        std::vector<std::pair<std::string, edm::InputTag> > modifiedElectrons_;
        int defaultMomentum_;
};


PATElectronMomentumEmbedder::PATElectronMomentumEmbedder(const edm::ParameterSet& iConfig) :
    electrons_(iConfig.getParameter<edm::InputTag>("electrons"))
{
  produces<pat::ElectronCollection>();
  // get list of all P4 options
  edm::ParameterSet pset = iConfig.getParameter<edm::ParameterSet>("allElectrons"); 
  std::vector<std::string> keys = pset.getParameterNamesForType<edm::InputTag>();
  modifiedElectrons_.reserve(keys.size());
  for (const std::string &k : keys) {
      modifiedElectrons_.emplace_back(std::make_pair(k, pset.getParameter<edm::InputTag>(k)));
  }
  // find which one to use
  std::string defaultMomentum = iConfig.getParameter<std::string>("defaultMomentum");
  std::vector<std::string>::const_iterator match = std::find(keys.begin(), keys.end(), defaultMomentum);
  if (match != keys.end()) {
      defaultMomentum_ = match - keys.begin();
      //if (modifiedElectrons_[defaultMomentum_].second == electrons_) {
      //  defaultMomentum_ = -1; // avoid re-setting it to itself
      //}
  } else {
      throw cms::Exception("Configuration") << "Error, default choice '" << defaultMomentum_ << "' not in the allElectrons pset.\n";
  }
}


PATElectronMomentumEmbedder::~PATElectronMomentumEmbedder() {

}


void PATElectronMomentumEmbedder::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    using namespace edm;
    using namespace std;

    edm::Handle<pat::ElectronCollection> electrons;
    iEvent.getByLabel(electrons_,electrons);

    std::vector<edm::Handle<pat::ElectronCollection>> handles(modifiedElectrons_.size());
    for (unsigned int i = 0, n = modifiedElectrons_.size(); i < n; ++i) {
        iEvent.getByLabel(modifiedElectrons_[i].second, handles[i]);
    }

    std::auto_ptr<pat::ElectronCollection> pOut(new pat::ElectronCollection);
    // ----- here is the real loop over the electrons ----
    for (pat::Electron clone : *electrons){    
        reco::CandidatePtr source = clone.originalObjectRef();
        for (int i = 0, n = modifiedElectrons_.size(); i < n; ++i) {
            for (const pat::Electron &mod : *handles[i]) {
                if (clone.originalObjectRef() == source) {
                    double ecalEnergy      = mod.correctedEcalEnergy();
                    double ecalEnergyError = mod.correctedEcalEnergyError();
                    double p      = mod.p();
                    double pError = mod.p4Error(mod.candidateP4Kind());
                    int    pKind  = mod.candidateP4Kind();

                    clone.addUserFloat("ecalEnergy_"+modifiedElectrons_[i].first, ecalEnergy);
                    clone.addUserFloat("ecalEnergyError_"+modifiedElectrons_[i].first, ecalEnergyError);
                    clone.addUserFloat("p_"+modifiedElectrons_[i].first, p);
                    clone.addUserFloat("pError_"+modifiedElectrons_[i].first, pError);
                    clone.addUserInt("pKind_"+modifiedElectrons_[i].first, pKind);

                    if (i == defaultMomentum_) {
                        clone.correctEcalEnergy( ecalEnergy, ecalEnergyError );
                        clone.setP4( mod.candidateP4Kind(), mod.p4(), pError, true );
                    }
                    break;
                } // found the electron
            } // loop on electrons
        } // loop on correction methods   
	pOut->push_back(clone);
    }
    iEvent.put(pOut);
}

// ------------ method called once each job just before starting event loop  ------------
void PATElectronMomentumEmbedder::beginJob() { }

// ------------ method called once each job just after ending the event loop  ------------
void PATElectronMomentumEmbedder::endJob() { }


DEFINE_FWK_MODULE(PATElectronMomentumEmbedder);
