// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include "CMG/MetAnalysis/interface/MetUtilities.h"

using namespace reco;

class PUMetProducer : public edm::EDProducer {
    public:
        explicit PUMetProducer(const edm::ParameterSet&);
        ~PUMetProducer();

    private:
        virtual void beginJob() ;
        virtual void produce(edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

	edm::InputTag PFcandCollTag;
	edm::InputTag uncorPFJetCollectionTag;
	edm::InputTag srcVertices_;

	double iDZCut_;
	double jetPtThreshold_;
	bool isData_;

	MetUtilities *utils;

	FactorizedJetCorrector* iJetCorrector;
};
