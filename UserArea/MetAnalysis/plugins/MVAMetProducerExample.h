// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include "CMG/MetAnalysis/interface/MVAMet.h"

using namespace reco;

class MVAMetProducerExample : public edm::EDProducer {
    public:
        explicit MVAMetProducerExample(const edm::ParameterSet&);
        ~MVAMetProducerExample();

    private:
        virtual void beginJob() ;
        virtual void produce(edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

	edm::InputTag PFcandCollTag;
	edm::InputTag uncorPFJetCollectionTag;
	edm::InputTag srcVertices_;

	bool          isData_;
	MVAMet        *mvaMET_;
};
