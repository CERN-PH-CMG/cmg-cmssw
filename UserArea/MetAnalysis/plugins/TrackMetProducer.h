// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "CMG/MetAnalysis/interface/MetUtilities.h"

class TrackMetProducer : public edm::EDProducer {
    public:
        explicit TrackMetProducer(const edm::ParameterSet&);
        ~TrackMetProducer();

    private:
        virtual void beginJob() ;
        virtual void produce(edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

	edm::InputTag collectionTag; 
	edm::InputTag vtxcollectionTag;

	MetUtilities *utils_;
        double        dZCut_;
	bool          isData_;
};
