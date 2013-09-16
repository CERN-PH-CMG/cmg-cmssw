#include "CMGTools/Common/plugins/PFVertexProducer.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"


PFVertexProducer::PFVertexProducer(const edm::ParameterSet & iConfig) {

  inputTagPFCandidates_ 
    = iConfig.getParameter<edm::InputTag>("PFCandidate");

  inputTagVertices_ 
    = iConfig.getParameter<edm::InputTag>("Vertices");

  checkClosestZVertex_ = iConfig.getParameter<bool>("checkClosestZVertex");

  produces<edm::ValueMap<reco::VertexRef> >();

}

PFVertexProducer::~PFVertexProducer() {;}

void 
PFVertexProducer::beginRun(edm::Run& run,const edm::EventSetup & es) {;}

void 
PFVertexProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  
  std::auto_ptr<edm::ValueMap<reco::VertexRef> > 
    vOutput(new edm::ValueMap<reco::VertexRef>());
  edm::ValueMap<reco::VertexRef>::Filler vertexFiller(*vOutput);
  
  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  iEvent.getByLabel( inputTagPFCandidates_, pfCandidates );
    
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel( inputTagVertices_, vertices);
    
  reco::VertexRefVector vertexRefs;

  for( unsigned i=0; i<pfCandidates->size(); ++i) {

    const reco::PFCandidate& cand = (*pfCandidates)[i];
    
    int ivertex;

    switch( cand.particleId() ) {
    case reco::PFCandidate::h:
    case reco::PFCandidate::e:
    case reco::PFCandidate::mu:
      ivertex = chargedHadronVertex( *vertices, cand );
      break;
    default:
      ivertex = 0;
      break;
    } 
    
    reco::VertexRef vertexRef(vertices,ivertex);
    vertexRefs.push_back(vertexRef);
    
  }      

  vertexFiller.insert(pfCandidates,vertexRefs.begin(),vertexRefs.end());
  vertexFiller.fill();
  iEvent.put(vOutput);
}

int 
PFVertexProducer::chargedHadronVertex( const reco::VertexCollection& vertices, 
				       const reco::PFCandidate& pfcand ) const {

  
  reco::TrackBaseRef trackBaseRef( pfcand.trackRef() );
  
  size_t  iVertex = 0;
  unsigned index=0;
  unsigned nFoundVertex = 0;
  typedef reco::VertexCollection::const_iterator IV;
  typedef reco::Vertex::trackRef_iterator IT;
  float bestweight=0;
  for(IV iv=vertices.begin(); iv!=vertices.end(); ++iv, ++index) {

    const reco::Vertex& vtx = *iv;
    
    // loop on tracks in vertices
    for(IT iTrack=vtx.tracks_begin(); 
	iTrack!=vtx.tracks_end(); ++iTrack) {
	 
      const reco::TrackBaseRef& baseRef = *iTrack;

      // one of the tracks in the vertex is the same as 
      // the track considered in the function
      if(baseRef == trackBaseRef ) {
	float w = vtx.trackWeight(baseRef);
	//select the vertex for which the track has the highest weight
	if (w > bestweight){
	  bestweight=w;
	  iVertex=index;
	  nFoundVertex++;
	}	 	
      }
    }
  }

  if (nFoundVertex>0){
    if (nFoundVertex!=1)
      edm::LogWarning("TrackOnTwoVertex")<<"a track is shared by at least two verteces. Used to be an assert";
    return iVertex;
  }
  // no vertex found with this track. 

  // optional: as a secondary solution, associate the closest vertex in z
  if ( checkClosestZVertex_ ) {

    double dzmin = 10000;
    double ztrack = pfcand.vertex().z();
    bool foundVertex = false;
    index = 0;
    for(IV iv=vertices.begin(); iv!=vertices.end(); ++iv, ++index) {

      double dz = fabs(ztrack - iv->z());
      if(dz<dzmin) {
	dzmin = dz; 
	iVertex = index;
	foundVertex = true;
      }
    }

    if( foundVertex ) 
      return iVertex;  

  }


  return -1 ;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PFVertexProducer);
