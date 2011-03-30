#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace jet{

  //
  std::vector<reco::CandidatePtr> filter(edm::Handle<edm::View<reco::Candidate> > &hJet,  std::vector<reco::CandidatePtr> &selLeptons, const edm::ParameterSet &iConfig)
  {

    std::vector<reco::CandidatePtr> selJets;

    using namespace edm;
    try{

      //config parameters
      double minPt = iConfig.getParameter<double>("minPt");
      double maxEta = iConfig.getParameter<double>("maxEta");
      double minDeltaRtoLepton = iConfig.getParameter<double>("minDeltaRtoLepton");
      ///edm::ParameterSet jetId = iConfig.getParameter<edm::ParameterSet>("jetId");
      PFJetIDSelectionFunctor jetIdSelector( iConfig.getParameter<edm::ParameterSet>("jetId") );
      pat::strbitset hasId = jetIdSelector.getBitTemplate();

      //iterate over the jets
      for(size_t iJet=0; iJet< hJet.product()->size(); ++iJet)
	{
	  reco::CandidatePtr jetPtr = hJet->ptrAt(iJet);
	  const pat::Jet *jet = dynamic_cast<const pat::Jet *>( jetPtr.get() );

	  //basic kinematics
	  double pt = jet->pt();
	  double eta = jet->eta();
	  if(pt<minPt || fabs(eta)>maxEta) continue;

	  //check overlaps with selected leptons
	  double minDR(1000);
	  for(std::vector<reco::CandidatePtr>::iterator lIt = selLeptons.begin(); lIt != selLeptons.end(); lIt++)
	    {
	      double dR = deltaR( *jet, **lIt );
	      if(dR > minDR) continue; 
	      minDR = dR;
	    }
	  if(minDR < minDeltaRtoLepton) continue;

	  //jet id
	  hasId.set(false);
	  if( !jetIdSelector( *jet, hasId ) ) continue;

	  //jet is selected
	  selJets.push_back(jetPtr);

	}
    }catch(std::exception &e){
      cout << "[jet::filter] failed with " << e.what() << endl;
    }
    
    return selJets;
  }


  //
  double fAssoc(const pat::Jet *jet, const reco::Vertex *vtx)
  {
    double fassoc(-1);
    if(jet==0 || vtx==0) return fassoc;
    
    //iterate over the tracks associated to a jet
    double sumpttracks(0),assocsumpttracks(0);
    const reco::TrackRefVector &jtracks = jet->associatedTracks();
    for(reco::TrackRefVector::const_iterator jtIt = jtracks.begin();
	jtIt != jtracks.end();
	jtIt++)
      {
	if( jtIt->isNull() ) continue;
	const reco::Track *jtrack = jtIt->get();
	sumpttracks += jtrack->pt();

	//find track match
	for(reco::Vertex::trackRef_iterator vtIt= vtx->tracks_begin(); vtIt != vtx->tracks_end(); vtIt++)
	  {
	    if( vtIt->isNull() ) continue;
	    const reco::Track *vtrack = vtIt->get();
	    if(vtrack!=jtrack) continue;
	    assocsumpttracks += jtrack->pt();
	    break;
	  }
      }
    
    if(sumpttracks>0) fassoc = assocsumpttracks/sumpttracks;
    return fassoc;
  }

}
