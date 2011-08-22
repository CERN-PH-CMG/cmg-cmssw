#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace dilepton{

  //
  std::pair<CandidateWithVertex,CandidateWithVertex> filter(CandidateWithVertexCollection &selLeptons,
							    const edm::ParameterSet &iConfig,
                                                            const edm::EventSetup &iSetup)
  {
    bool candidateFound(false);
    std::pair<CandidateWithVertex,CandidateWithVertex> selDilepton;
    
    try{
    
      //config parameters
      double minDileptonMass = iConfig.getParameter<double>("minDileptonMass");
      double maxDileptonMass = iConfig.getParameter<double>("maxDileptonMass");
      double maxDz = iConfig.getParameter<double>("maxDz");

      using namespace lepton;

      for(size_t ilep=0; ilep<selLeptons.size(); ilep++)
	{
	  reco::CandidatePtr lep1Ptr = selLeptons[ilep].first;
	  reco::VertexRef lep1Vtx = selLeptons[ilep].second;
	  if(lep1Vtx.isNull()) continue;
	  //	  int id1 = lepton::getLeptonId(lep1Ptr);
	  
	  //iterate over the second lepton
	  for(size_t jlep=ilep+1; jlep<selLeptons.size(); jlep++)
	    {
	      reco::CandidatePtr lep2Ptr = selLeptons[jlep].first;
	      reco::VertexRef lep2Vtx = selLeptons[jlep].second;
	      if(lep2Vtx.isNull()) continue;
	      //int id2 = lepton::getLeptonId(lep2Ptr);
	      
	      double dz( lep1Vtx->position().z()-lep2Vtx->position().z());
	      if(fabs(dz)>fabs(maxDz) ) continue;
	      
	      //compute the mass
	      LorentzVector dilepton=lep1Ptr->p4()+lep2Ptr->p4();
	      double mass = dilepton.mass();
	      if(mass<minDileptonMass || mass >maxDileptonMass) continue;
	      
	      //build the dilepton candidate
	      std::vector<reco::CandidatePtr> dilCand;
	      dilCand.push_back(lep1Ptr);
	      dilCand.push_back(lep2Ptr);
	      
	      //take if leading in sum pT
	      if(!candidateFound)
		{
		  selDilepton.first = selLeptons[ilep];
		  selDilepton.second = selLeptons[jlep];
		}
	      else
		{
		  double sumpt=selDilepton.first.first->pt()+selDilepton.second.first->pt();
		  double candsumpt=selLeptons[ilep].first->pt()+selLeptons[jlep].first->pt();
		  if(sumpt>candsumpt) continue;
		  selDilepton.first = selLeptons[ilep];
		  selDilepton.second = selLeptons[jlep];
		}
	    }
	}
    }
    catch(std::exception &e){
      cout << "[dilepton::filter] failed with : " << e.what() << endl;
    }
    
    //result
    return selDilepton;
  }


  //
  int classify(std::pair<CandidateWithVertex,CandidateWithVertex> &selDilepton)
  {
    if(selDilepton.first.first.isNull() || selDilepton.second.first.isNull()) return UNKNOWN;

    int id1=fabs(lepton::getLeptonId(selDilepton.first.first));
    int id2=fabs(lepton::getLeptonId(selDilepton.second.first));
    if( id1==lepton::MUON && id2==lepton::MUON) return MUMU;
    if( id1==lepton::ELECTRON && id2==lepton::ELECTRON) return EE;
    if( ( id1==lepton::MUON && id2==lepton::ELECTRON) || ( id1==lepton::ELECTRON && id2==lepton::MUON)  ) return EMU;
    return UNKNOWN;
  }
}
