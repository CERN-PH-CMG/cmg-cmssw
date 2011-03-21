#include "CMGTools/HtoZZ2l2nu/plugins/ObjectFilters.h"

using namespace std;

namespace dilepton{
  
  //
  std::vector<reco::CandidatePtr> filter(std::vector<reco::CandidatePtr> &selMuons, 
					 std::vector<reco::CandidatePtr> &selElectrons, 
					 std::vector<reco::CandidatePtr> &selVertices, 
					 const edm::ParameterSet &iConfig)
  {
    std::vector<reco::CandidatePtr> selDilepton;
    
    try{
    
      //config parameters
      double minDileptonMass = iConfig.getParameter<double>("minDileptonMass");
      double maxDileptonMass = iConfig.getParameter<double>("maxDileptonMass");
      
      //di-muon selection
      std::vector<reco::CandidatePtr> mumuCand;
      if(selMuons.size()>1)
	{
	  for(size_t imu=0; imu<selMuons.size(); imu++)
	    {
	      for(size_t jmu=imu+1; jmu<selMuons.size(); jmu++)
		{
		  
		}
	    }
	}

      //di-electron selection
      std::vector<reco::CandidatePtr> eeCand;
      if(selElectrons.size()>1)
	{
	  for(size_t iele=0; iele<selElectrons.size(); iele++)
	    {
	      for(size_t jele=iele+1; jele<selElectrons.size(); jele++)
		{
		}
	    }
	}

      //emu selection
      std::vector<reco::CandidatePtr> emuCand;
      if(selElectrons.size()>0 && selMuons.size()>0)
	{
	  for(size_t iele=0; iele<selElectrons.size(); iele++)
	    {
	      for(size_t jmu=0; jmu<selMuons.size(); jmu++)
		{
		}
	    }
	}
      

    }
    catch(std::exception &e){
      cout << "[dilepton::filter] failed with : " << e.what() << endl;
    }

    return selDilepton;
  }
}
