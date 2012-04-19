// -*- C++ -*-
//
// Package:    PileupJetIdProducer
// Class:      PileupJetIdProducer
// 
/**\class PileupJetIdProducer PileupJetIdProducer.cc CMGTools/PileupJetIdProducer/src/PileupJetIdProducer.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Pasquale Musella,40 2-A12,+41227671706,
//         Created:  Wed Apr 18 15:48:47 CEST 2012
// $Id: PileupJetIdProducer.cc,v 1.1 2012/04/18 15:17:02 musella Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CMGTools/External/interface/PileupJetIdentifier.h"
#include "CMGTools/External/interface/PileupJetIdAlgo.h"

// ------------------------------------------------------------------------------------------
class PileupJetIdProducer : public edm::EDProducer {
public:
	explicit PileupJetIdProducer(const edm::ParameterSet&);
	~PileupJetIdProducer();

	static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
	virtual void beginJob() ;
	virtual void produce(edm::Event&, const edm::EventSetup&);
	virtual void endJob() ;
      
	virtual void beginRun(edm::Run&, edm::EventSetup const&);
	virtual void endRun(edm::Run&, edm::EventSetup const&);
	virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
	virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

	edm::InputTag jets_, vertexes_;
	bool runMvas_;
	std::vector<std::pair<std::string, PileupJetIdAlgo *> > algos_;
};

// ------------------------------------------------------------------------------------------
PileupJetIdProducer::PileupJetIdProducer(const edm::ParameterSet& iConfig)
{
	runMvas_ = iConfig.getParameter<bool>("runMvas");
	jets_ = iConfig.getParameter<edm::InputTag>("jets");
	vertexes_ = iConfig.getParameter<edm::InputTag>("vertexes");
	std::vector<edm::ParameterSet> algos = iConfig.getParameter<std::vector<edm::ParameterSet> >("algos");
	
	produces<edm::ValueMap<StoredPileupJetIdentifier> > ("");
	for(std::vector<edm::ParameterSet>::iterator it=algos.begin(); it!=algos.end(); ++it) {
		std::string label = it->getParameter<std::string>("label");
		algos_.push_back( std::make_pair(label,new PileupJetIdAlgo(*it)) );
		if( runMvas_ ) {
			produces<edm::ValueMap<float> > (label+"Discriminant");
			produces<edm::ValueMap<int> > (label+"Id");
		}
	}
}



// ------------------------------------------------------------------------------------------
PileupJetIdProducer::~PileupJetIdProducer()
{
}


// ------------------------------------------------------------------------------------------
void
PileupJetIdProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace std;
	Handle<View<pat::Jet> > jetHandle;
	iEvent.getByLabel(jets_,jetHandle);
	const View<pat::Jet> & jets = *jetHandle;
	
	Handle<reco::VertexCollection> vertexHandle;
	iEvent.getByLabel(vertexes_, vertexHandle);
	const reco::VertexCollection & vertexes = *(vertexHandle.product());
	const reco::Vertex & vtx  = *(vertexes.begin());;

	vector<StoredPileupJetIdentifier> ids; 
	map<string, vector<float> > mvas;
	map<string, vector<int> > idflags;

	for ( unsigned int i=0; i<jets.size(); ++i ) {
		vector<pair<string,PileupJetIdAlgo *> >::iterator algoi = algos_.begin();
		PileupJetIdAlgo * ialgo = algoi->second;
		const pat::Jet & patjet = jets.at(i);
 
		PileupJetIdentifier puIdentifier = ialgo->computeIdVariables(&patjet, 0., &vtx, vertexes, runMvas_);
		if( runMvas_ ) {
			ids.push_back( puIdentifier );
			mvas[algoi->first].push_back( puIdentifier.mva() );
			idflags[algoi->first].push_back( puIdentifier.idFlag() );
			for( ++algoi; algoi!=algos_.end(); ++algoi) {
				ialgo = algoi->second;
				ialgo->set(puIdentifier);
				PileupJetIdentifier id = ialgo->computeMva();
				mvas[algoi->first].push_back( id.mva() );
				idflags[algoi->first].push_back( id.idFlag() );
			}
		}
	}
	
	for(vector<pair<string,PileupJetIdAlgo *> >::iterator ialgo = algos_.begin(); ialgo!=algos_.end(); ++ialgo) {
		vector<float> & mva = mvas[ialgo->first];
		auto_ptr<ValueMap<float> > mvaout(new ValueMap<float>());
		ValueMap<float>::Filler mvafiller(*mvaout);
		mvafiller.insert(jetHandle,mva.begin(),mva.end());
		iEvent.put(mvaout,ialgo->first+"Discriminant");

		vector<int> & idflag = idflags[ialgo->first];
		auto_ptr<ValueMap<int> > idflagout(new ValueMap<int>());
		ValueMap<int>::Filler idflagfiller(*idflagout);
		idflagfiller.insert(jetHandle,idflag.begin(),idflag.end());
		iEvent.put(idflagout,ialgo->first+"Id");

	}
	
	auto_ptr<ValueMap<StoredPileupJetIdentifier> > idsout(new ValueMap<StoredPileupJetIdentifier>());
	ValueMap<StoredPileupJetIdentifier>::Filler idsfiller(*idsout);
	idsfiller.insert(jetHandle,ids.begin(),ids.end());
	iEvent.put(idsout);
}

// ------------------------------------------------------------------------------------------
void 
PileupJetIdProducer::beginJob()
{
}

// ------------------------------------------------------------------------------------------
void 
PileupJetIdProducer::endJob() {
}

// ------------------------------------------------------------------------------------------
void 
PileupJetIdProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------------------------------------------------------------------------------------
void 
PileupJetIdProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------------------------------------------------------------------------------------
void 
PileupJetIdProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------------------------------------------------------------------------------------
void 
PileupJetIdProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------------------------------------------------------------------------------------
void
PileupJetIdProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PileupJetIdProducer);
