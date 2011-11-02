#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace lepton{

  //returns the pT error associated to a reconstructed lepton
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
	ptError = (lepton->pt()/lepton->p()) * ele->trackMomentumError();
      }
    return ptError;
  }
 
  //returns the lepton id (with associated charge)
  int getLeptonId(reco::CandidatePtr &lepton)
  {
    int id=lepton->charge();
    if( dynamic_cast<const pat::Muon *>( lepton.get() ) ) id *= lepton::MUON;
    else if( dynamic_cast<const pat::Electron *>( lepton.get() ) ) id *= lepton::ELECTRON;
    return id;
  }

  //returns the generator level particle associated to the lepton
  const reco::GenParticle *getLeptonGenMatch(reco::CandidatePtr &lepton)
  {
    if( dynamic_cast<const pat::Muon *>( lepton.get() ) ) 
      {
	const pat::Muon *mu=dynamic_cast<const pat::Muon *>( lepton.get() );
	return mu->genLepton();
      }
    else if( dynamic_cast<const pat::Electron *>( lepton.get() ) )
      {
	const pat::Electron *ele=dynamic_cast<const pat::Electron *>( lepton.get() );
	return ele->genLepton();
      }
    return 0;
  }

  //returns the standard lepton isolation
  std::vector<double> getLeptonIso(reco::CandidatePtr &lepton,float minRelNorm, float puOffsetCorrection)
  {
    std::vector<double> leptonIso(8,99999.);
    if(lepton.get()==0) return leptonIso;

    int lid=fabs(getLeptonId(lepton));
    if( lid==lepton::MUON )
      {
	const pat::Muon *mu=dynamic_cast<const pat::Muon *>( lepton.get() );
	leptonIso[N_ISO]=mu->neutralHadronIso();
	leptonIso[C_ISO]=mu->chargedHadronIso();
	leptonIso[G_ISO]=mu->photonIso ();
 	leptonIso[ECAL_ISO]=mu->ecalIso();
	leptonIso[HCAL_ISO]=mu->hcalIso();
	leptonIso[TRACKER_ISO]=mu->trackIso();
      }
    else if( lid==lepton::ELECTRON )
      {
	//ecal barrel pedestal is subtracted
	const pat::Electron *ele=dynamic_cast<const pat::Electron *>( lepton.get() );
	leptonIso[N_ISO]=ele->neutralHadronIso();
	leptonIso[C_ISO]=ele->chargedHadronIso();
	leptonIso[G_ISO]=ele->photonIso ();
	leptonIso[ECAL_ISO]=ele->ecalIso();
//	if(ele->isEB()) leptonIso[ECAL_ISO] = max(leptonIso[ECAL_ISO]-1.0,0.);
	leptonIso[HCAL_ISO]=ele->hcalIso();
	leptonIso[TRACKER_ISO]=ele->trackIso();

      }
  
    leptonIso[REL_ISO]=(max(leptonIso[ECAL_ISO]+leptonIso[HCAL_ISO]-puOffsetCorrection*0.28,0.)+leptonIso[TRACKER_ISO])/max(float(lepton->pt()),float(minRelNorm));
    leptonIso[PFREL_ISO]=(leptonIso[N_ISO]+leptonIso[G_ISO]+leptonIso[C_ISO])/max(float(lepton->pt()),float(minRelNorm));
    
    return leptonIso;
  }


}

