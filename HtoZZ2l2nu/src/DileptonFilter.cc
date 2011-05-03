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
	ptError = (lepton->pt()/lepton->p()) * ele->trackMomentumError();//electronMomentumError();
      }
    return ptError;
  }

  //
  int getLeptonId(reco::CandidatePtr &lepton)
  {
    int id=lepton->charge();

    if( dynamic_cast<const pat::Muon *>( lepton.get() ) ) id *= 13;
    else if( dynamic_cast<const pat::Electron *>( lepton.get() ) ) id *= 11;
    return id;
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
      bool constrainByVertex = iConfig.getParameter<bool>("constrainByVertex");
      double maxDxy = iConfig.getParameter<double>("maxDxy");
      double maxDz = iConfig.getParameter<double>("maxDz");

      for(size_t ilep=0; ilep<selLeptons.size(); ilep++)
	{
	  reco::VertexRef curSelVtx;
	  reco::CandidatePtr lep1Ptr = selLeptons[ilep];
	  
	  //check vertex association
	  const reco::Vertex *pv1=0;
	  if(constrainByVertex)
	    {
	      reco::VertexRef v1=
		dynamic_cast<const pat::Electron *>( lep1Ptr.get() ) ?
		vertex::getClosestVertexTo<reco::GsfTrack>( dynamic_cast<const pat::Electron *>(lep1Ptr.get())->gsfTrack().get(), selVertices, iSetup,true) :
		vertex::getClosestVertexTo<reco::Track>( dynamic_cast<const pat::Muon *>(lep1Ptr.get())->innerTrack().get() , selVertices, iSetup,true) ;
	      if(v1.get()==0) continue;
	      pv1=v1.get();
	      curSelVtx=v1;
	    }
	  else if(selVertices.size()){
	    reco::VertexRef v1=selVertices[0];
	    double dxy(  dynamic_cast<const pat::Electron *>( lep1Ptr.get() ) ? 
			 dynamic_cast<const pat::Electron *>(lep1Ptr.get())->gsfTrack()->dxy( v1.get()->position() ) : 
			 dynamic_cast<const pat::Muon *>(lep1Ptr.get())->innerTrack()->dxy( v1.get()->position() ) );
	    double dz(  dynamic_cast<const pat::Electron *>( lep1Ptr.get() ) ? 
			dynamic_cast<const pat::Electron *>(lep1Ptr.get())->gsfTrack()->dz( v1.get()->position() ) : 
			dynamic_cast<const pat::Muon *>(lep1Ptr.get())->innerTrack()->dz( v1.get()->position() ) );
	    if(fabs(dxy)>fabs(maxDxy) || fabs(dz)>fabs(maxDz) ) continue;
	    pv1=v1.get();
	    curSelVtx=v1;
	  }

	  //iterate over the second lepton
	  for(size_t jlep=ilep+1; jlep<selLeptons.size(); jlep++)
	    {
	      reco::CandidatePtr lep2Ptr = selLeptons[jlep];

	      //check vertex association
	      const reco::Vertex *pv2=pv1;
	      if(constrainByVertex)
		{
		  reco::VertexRef v2=
		    dynamic_cast<const pat::Electron *>( lep2Ptr.get() ) ?
		    vertex::getClosestVertexTo<reco::GsfTrack>( dynamic_cast<const pat::Electron *>(lep2Ptr.get())->gsfTrack().get(), selVertices, iSetup,true) :
		    vertex::getClosestVertexTo<reco::Track>( dynamic_cast<const pat::Muon *>(lep2Ptr.get())->innerTrack().get(), selVertices, iSetup,true) ;
		  if(v2.get()==0) continue;	      
		  pv2=v2.get();
		  if(pv1==0 || pv2==0 || pv1!=pv2) continue;
		}
	      else if(pv2)
		{
		  double dxy(  dynamic_cast<const pat::Electron *>( lep2Ptr.get() ) ? 
			       dynamic_cast<const pat::Electron *>(lep2Ptr.get())->gsfTrack()->dxy( pv2->position() ) : 
			       dynamic_cast<const pat::Muon *>(lep2Ptr.get())->innerTrack()->dxy( pv2->position() ) );
		  double dz(  dynamic_cast<const pat::Electron *>( lep2Ptr.get() ) ? 
			      dynamic_cast<const pat::Electron *>(lep2Ptr.get())->gsfTrack()->dz( pv2->position() ) : 
			      dynamic_cast<const pat::Muon *>(lep2Ptr.get())->innerTrack()->dz( pv2->position() ) );
		  if(fabs(dxy)>fabs(maxDxy) || fabs(dz)>fabs(maxDz) ) continue;
		}

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
		  selVtx = curSelVtx;
		  selDilepton=dilCand;
		}
	      else
		{
		  double sumpt=selDilepton[0]->pt()+selDilepton[1]->pt();
		  double candsumpt=dilCand[0]->pt()+dilCand[1]->pt();
		  if(sumpt>candsumpt) continue;
		  selVtx=curSelVtx;
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
    int id1=fabs(getLeptonId(selDilepton[0]));
    int id2=fabs(getLeptonId(selDilepton[1]));
    if( id1==13 && id2==13) return MUMU;
    if( id1==11 && id2==11) return EE;
    if( ( id1==13 && id2==11) || ( id1==11 && id2==13)  ) return EMU;
    return UNKNOWN;
  }
}
