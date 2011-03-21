#include "CMGTools/HtoZZ2l2nu/plugins/ObjectFilters.h"

using namespace std;

namespace vertex{
  
  //
  std::vector<reco::VertexRef> filter( edm::Handle<reco::VertexCollection> &hVtx, 
				       const edm::ParameterSet &iConfig)
  {
    std::vector<reco::VertexRef> selVertices;
    
    try{
      
      //config parameters
      double maxZ = iConfig.getParameter<double>("maxZ");
      double maxRho = iConfig.getParameter<double>("maxRho");
      int minNDOF = iConfig.getParameter<int>("minNDOF");
      
      //select the vertices
      for(size_t iVtx=0; iVtx< hVtx.product()->size(); ++iVtx)      
	{
	  
	  reco::VertexRef vtxRef(hVtx,iVtx);
	  
	  bool isReal = !(vtxRef->isFake());
	  if(!isReal) continue;
	  
	  double z = vtxRef->z();
	  double rho = vtxRef->position().Rho();
	  int ndof = vtxRef->ndof();
	  if(fabs(z)>maxZ || rho>maxRho || ndof<minNDOF) continue;
	  
	  selVertices.push_back(vtxRef);
	}
    }
    catch(std::exception &e){
      cout << "[vertex::filter] failed with : " << e.what() << endl;
    }

    return selVertices;
  }

}
