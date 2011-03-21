#include "CMGTools/HtoZZ2l2nu/plugins/ObjectFilters.h"

using namespace std;

namespace dilepton{
  
  //
  std::vector<reco::CandidatePtr> filter(std::vector<reco::CandidatePtr> &selLeptons, 
					 std::vector<reco::CandidatePtr> &selVertices, 
					 const edm::ParameterSet &iConfig,
					 const edm::EventSetup &iSetup)
  {
    std::vector<reco::CandidatePtr> selDilepton;
    
    try{
    
      //config parameters
      double minDileptonMass = iConfig.getParameter<double>("minDileptonMass");
      double maxDileptonMass = iConfig.getParameter<double>("maxDileptonMass");
      
      for(size_t ilep=0; ilep<selLeptons.size(); ilep++)
	{
	  reco::CandidatePtr lep1Ptr = selLeptons[ilep];
	  reco::CandidatePtr v1=
	    dynamic_cast<const pat::Electron *>( lep1Ptr.get() ) ?
	    vertex::getClosestVertexTo<pat::Electron>( dynamic_cast<const pat::Electron *>(lep1Ptr.get()), selVertices, iSetup,true) :
	    vertex::getClosestVertexTo<pat::Muon>( dynamic_cast<const pat::Muon *>(lep1Ptr.get()), selVertices, iSetup,true) ;
	  if(v1.get()==0) continue;

	  //iterate over the second lepton
	  for(size_t jlep=ilep+1; jlep<selLeptons.size(); jlep++)
	    {
	      reco::CandidatePtr lep2Ptr = selLeptons[jlep];
	      reco::CandidatePtr v2=
		dynamic_cast<const pat::Electron *>( lep2Ptr.get() ) ?
		vertex::getClosestVertexTo<pat::Electron>( dynamic_cast<const pat::Electron *>(lep2Ptr.get()), selVertices, iSetup,true) :
		vertex::getClosestVertexTo<pat::Muon>( dynamic_cast<const pat::Muon *>(lep2Ptr.get()), selVertices, iSetup,true) ;
	      if(v2.get()==0) continue;	      

	      //same vertex must be assigned
	      if(v1.get()!=v2.get()) continue;

	      //compute the mass
	      double en = lep1Ptr->energy() + lep2Ptr->energy();
	      double px = lep1Ptr->px() + lep2Ptr->px();
	      double py = lep1Ptr->py() + lep2Ptr->py();
	      double pz = lep1Ptr->pz() + lep2Ptr->pz();
	      double mass = sqrt(en*en - px*px - py*py - pz*pz);
	      if(mass<minDileptonMass || mass >maxDileptonMass) continue;
	      
	      //build the dilepton candidate
	      std::vector<reco::CandidatePtr> dilCand;
	      dilCand.push_back(v1);
	      dilCand.push_back(lep1Ptr);
	      dilCand.push_back(lep2Ptr);

	      //take if leading in sum pT
	      if(selDilepton.size()==0) selDilepton=dilCand;
	      else
		{
		  double sumpt=selDilepton[1]->pt()+selDilepton[2]->pt();
		  double candsumpt=dilCand[1]->pt()+dilCand[2]->pt();
		  if(sumpt>candsumpt) continue;
		  selDilepton=dilCand;
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
