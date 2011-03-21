#include "CMGTools/HtoZZ2l2nu/plugins/ObjectFilters.h"

using namespace std;

namespace vertex{
  
  //
  std::vector<reco::CandidatePtr> filter( edm::Handle<edm::View<reco::Candidate> > &hVtx, 
					  const edm::ParameterSet &iConfig)
  {
    std::vector<reco::CandidatePtr> selVertices;
    
    try{
      
      //config parameters
      double maxZ = iConfig.getParameter<double>("maxZ");
      double maxRho = iConfig.getParameter<double>("maxRho");
      int minNDOF = iConfig.getParameter<int>("minNDOF");
     
      //select the vertices
      for(size_t iVtx=0; iVtx< hVtx.product()->size(); ++iVtx)      
	{
	  reco::CandidatePtr vtxPtr = hVtx->ptrAt(iVtx);
	  const reco::Vertex *vtx = dynamic_cast<const reco::Vertex *>( vtxPtr.get() );
	  
	  bool isReal = !(vtx->isFake());
	  if(!isReal) continue;
	  
	  double z = vtx->z();
	  double rho = vtx->position().Rho();
	  int ndof = vtx->ndof();
	  if(fabs(z)>maxZ || rho>maxRho || ndof<minNDOF) continue;
	  
	  selVertices.push_back(vtxPtr);
	}
    }
    catch(std::exception &e){
      cout << "[vertex::filter] failed with : " << e.what() << endl;
    }

    return selVertices;
  }

}
