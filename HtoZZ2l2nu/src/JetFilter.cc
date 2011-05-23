#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace jet{

  //
  CandidateWithVertexCollection filter(edm::Handle<edm::View<reco::Candidate> > &hJet, 
				       CandidateWithVertexCollection &selLeptons, 
				       std::vector<reco::VertexRef> &goodVertices,
				       const edm::ParameterSet &iConfig)
  {

    CandidateWithVertexCollection selJets;

    using namespace edm;
    try{

      //config parameters
      double minPt = iConfig.getParameter<double>("minPt");
      double maxEta = iConfig.getParameter<double>("maxEta");
      double minDeltaRtoLepton = iConfig.getParameter<double>("minDeltaRtoLepton");
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
	  for(CandidateWithVertexCollection::iterator lIt = selLeptons.begin(); lIt != selLeptons.end(); lIt++)
	    {
	      double dR = deltaR( *jet, *(lIt->first.get()) );
	      if(dR > minDR) continue; 
	      minDR = dR;
	    }
	  if(minDR < minDeltaRtoLepton) continue;

	  //jet id
	  hasId.set(false);
	  if( !jetIdSelector( *jet, hasId ) ) continue;
	  
	  //associate a vertex using the beta variable
	  double betaMax(-1);
	  reco::VertexRef bestVtx;
	  for(std::vector<reco::VertexRef>::iterator vit=goodVertices.begin();
	      vit != goodVertices.end();
	      vit++)
	    {
	      double beta=fAssoc(jet,vit->get());
	      if(beta<betaMax) continue;
	      betaMax=beta;
	      bestVtx=*vit;
	    }
	  
	  //jet is selected
	  selJets.push_back(CandidateWithVertex(jetPtr,bestVtx));
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

  //
  void classifyJetsForDileptonEvent(CandidateWithVertexCollection &selJets, 
				    std::pair<CandidateWithVertex,CandidateWithVertex> &dilepton,
				    CandidateWithVertexCollection &assocJets, 
				    CandidateWithVertexCollection &puJets,
				    double maxDz)
  {
    for(CandidateWithVertexCollection::iterator jIt=selJets.begin();
	jIt != selJets.end();
	jIt++)
      {
	//	assocJets.push_back(*jIt);continue;

	if( jIt->second.isNonnull() )
	  {
	    double mindz=1e+6;
	    if( dilepton.first.second.isNonnull() )
	      {
		if(jIt->second.get()==dilepton.first.second.get()) 
		  { assocJets.push_back(*jIt); continue; }
		double idz=fabs(jIt->second->position().z()-dilepton.first.second->position().z());
		if(idz<mindz) mindz=idz;
	      }
	    if( dilepton.second.second.isNonnull() )
	      {
		if(jIt->second.get()==dilepton.second.second.get()) 
		  { assocJets.push_back(*jIt); continue; }
		double idz=fabs(jIt->second->position().z()-dilepton.second.second->position().z());
		if(idz<mindz) mindz=idz;
	      }
	    
	    //is associated if near to one of the vertices otherwise is pileup
	    //if(mindz<0.5*maxDz) assocJets.push_back(*jIt);
	    //else 
	    puJets.push_back(*jIt);
	  }
	else 
	  {
	    //jet has no tracks associated to a selected vertex = take it as associated (may be a photon)
	    //puJets.push_back(*jIt);
	    assocJets.push_back(*jIt);
	  }
      }
  }
  
}

