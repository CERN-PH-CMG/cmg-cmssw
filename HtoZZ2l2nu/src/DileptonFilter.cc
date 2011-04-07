#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace dilepton{

  double getPtErrorFor(reco::CandidatePtr &lepton)
  {
    double ptError(0);

    if( dynamic_cast<const pat::Muon *>( lepton.get() ) )
      {
	const pat::Muon *mu=dynamic_cast<const pat::Muon *>(lepton.get());
	ptError = mu->innerTrack()->ptError();
      }
    else if( dynamic_cast<const pat::Electron *>( lepton.get() ) )
      {
	const pat::Electron *ele=dynamic_cast<const pat::Electron *>( lepton.get() ) ;
	ptError = (lepton->pt()/lepton->p()) * ele->electronMomentumError();
      }
    return ptError;
  }
  
  //
  std::pair<reco::VertexRef, std::vector<reco::CandidatePtr> > filter(std::vector<reco::CandidatePtr> &selLeptons, 
								      std::vector<reco::VertexRef> &selVertices, 
								      const edm::ParameterSet &iConfig,
								      const edm::EventSetup &iSetup)
  {
    reco::VertexRef selVtx;
    std::vector<reco::CandidatePtr> selDilepton;
    
    try{
    
      //config parameters
      double minDileptonMass = iConfig.getParameter<double>("minDileptonMass");
      double maxDileptonMass = iConfig.getParameter<double>("maxDileptonMass");
      
      for(size_t ilep=0; ilep<selLeptons.size(); ilep++)
	{
	  reco::CandidatePtr lep1Ptr = selLeptons[ilep];
	  reco::VertexRef v1=
	    dynamic_cast<const pat::Electron *>( lep1Ptr.get() ) ?
	    vertex::getClosestVertexTo<reco::GsfTrack>( dynamic_cast<const pat::Electron *>(lep1Ptr.get())->gsfTrack().get(), selVertices, iSetup,true) :
	    vertex::getClosestVertexTo<reco::Track>( dynamic_cast<const pat::Muon *>(lep1Ptr.get())->innerTrack().get() , selVertices, iSetup,true) ;
	  if(v1.get()==0) continue;

	  //iterate over the second lepton
	  for(size_t jlep=ilep+1; jlep<selLeptons.size(); jlep++)
	    {
	      reco::CandidatePtr lep2Ptr = selLeptons[jlep];
	      reco::VertexRef v2=
		dynamic_cast<const pat::Electron *>( lep2Ptr.get() ) ?
		vertex::getClosestVertexTo<reco::GsfTrack>( dynamic_cast<const pat::Electron *>(lep2Ptr.get())->gsfTrack().get(), selVertices, iSetup,true) :
		vertex::getClosestVertexTo<reco::Track>( dynamic_cast<const pat::Muon *>(lep2Ptr.get())->innerTrack().get(), selVertices, iSetup,true) ;
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
	      dilCand.push_back(lep1Ptr);
	      dilCand.push_back(lep2Ptr);

	      //take if leading in sum pT
	      if(selDilepton.size()==0) 
		{
		  selVtx = v1;
		  selDilepton=dilCand;
		}
	      else
		{
		  double sumpt=selDilepton[0]->pt()+selDilepton[1]->pt();
		  double candsumpt=dilCand[0]->pt()+dilCand[1]->pt();
		  if(sumpt>candsumpt) continue;
		  selVtx=v1;
		  selDilepton=dilCand;
		}
	      
	    }
	}
    }
    catch(std::exception &e){
      cout << "[dilepton::filter] failed with : " << e.what() << endl;
    }

    //result
    return std::pair<reco::VertexRef, std::vector<reco::CandidatePtr> >(selVtx,selDilepton);
  }


  //
  int classify(std::vector<reco::CandidatePtr> &selDilepton)
  {
    if(selDilepton.size()==0) return UNKNOWN;
    if(dynamic_cast<const pat::Muon *>(selDilepton[0].get()) && dynamic_cast<const pat::Muon *>(selDilepton[1].get()) ) return MUMU;
    if(dynamic_cast<const pat::Electron *>(selDilepton[0].get()) && dynamic_cast<const pat::Electron *>(selDilepton[1].get()) ) return EE;
    if(dynamic_cast<const pat::Muon *>(selDilepton[0].get()) && dynamic_cast<const pat::Electron *>(selDilepton[1].get()) ) return EMU;
    if(dynamic_cast<const pat::Electron *>(selDilepton[0].get()) && dynamic_cast<const pat::Muon *>(selDilepton[1].get()) ) return EMU;
    return UNKNOWN;
  }
}
