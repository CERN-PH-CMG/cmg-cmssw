#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "TObjArray.h"
#include "TObjString.h"

using namespace std;
using namespace reco;

///                            ///   
/// VERTEX SELECTION UTILITIES ///
///                            ///   

//
vector<reco::VertexRef> getGoodVertices( edm::Handle<reco::VertexCollection> &hVtx, const edm::ParameterSet &iConfig)
{
  vector<reco::VertexRef> selVertices;
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
  catch(exception &e){
    cout << "[vertex::filter] failed with : " << e.what() << endl;
  }
  
  return selVertices;
}

//
float getVertexMomentumFlux(const reco::Vertex *vtx, float minWeight)
{
  float momFlux(0);
  if(vtx==0) return momFlux;
  for(reco::Vertex::trackRef_iterator iTrack= vtx->tracks_begin(); iTrack != vtx->tracks_end(); iTrack++)  
    {
      if(vtx->trackWeight(*iTrack)<minWeight) continue;
      momFlux += (*iTrack)->pt();
    }
  return momFlux;
}


///                            ///   
/// LEPTON SELECTION UTILITIES ///
///                            ///   

//
vector<reco::CandidatePtr> getGoodMuons(edm::Handle<edm::View<reco::Candidate> > &hMu, 
					const reco::VertexRef &primVertex,
					const double& rho, 
					const edm::ParameterSet &iConfig)
{
    vector<reco::CandidatePtr> selMuons;
    
    try{
      //config parameters
      double minPt = iConfig.getParameter<double>("minPt");
      double maxEta = iConfig.getParameter<double>("maxEta");
      bool requireGlobal = iConfig.getParameter<bool>("requireGlobal"); 
      bool requireTracker = iConfig.getParameter<bool>("requireTracker"); 
      int minValidMuonHits = iConfig.getParameter<int>("minValidMuonHits");
      int minMatchingMuonStations = iConfig.getParameter<int>("minMatchingMuonStations");
      int minValidTrackerHits = iConfig.getParameter<int>("minValidTrackerHits");
      int minPixelHits = iConfig.getParameter<int>("minPixelHits");
      double maxTrackChi2 = iConfig.getParameter<double>("maxTrackChi2");
      double maxRelPtUncertainty = iConfig.getParameter<double>("maxRelPtUncertainty");
      double maxD0 = iConfig.getParameter<double>("maxD0");
      double maxDZ = iConfig.getParameter<double>("maxDz");
      string id = iConfig.getParameter<string>("id");
      double maxRelIso = iConfig.getParameter<double>("maxRelIso");
      bool applySoftMuonIsolationVeto = iConfig.getParameter<bool>("applySoftMuonIsolationVeto");
      bool usePFIso = iConfig.getParameter<bool>("usePFIso");
      bool doDeltaBetaCorrection = iConfig.getParameter<bool>("doDeltaBetaCorrection");

      //iterate over the muons
      for(size_t iMuon=0; iMuon< hMu.product()->size(); ++iMuon)      
	{
	  reco::CandidatePtr muonPtr = hMu->ptrAt(iMuon);
	  const pat::Muon *muon = dynamic_cast<const pat::Muon *>( muonPtr.get() );
	  
	  //kinematics
	  double mPt = muon->pt();
	  double mEta = muon->eta();
	  if( mPt<minPt || fabs(mEta)>maxEta) continue; 

	  //muon type
	  bool isTracker = muon->isTrackerMuon();
	  if(requireTracker && !isTracker)  continue;
	  bool isGlobal = muon->isGlobalMuon();
	  if(requireGlobal && !isGlobal) continue;
	  
	  //primary vertex compatibility (requires tracker)
	  if(requireTracker)
	    {
	      double d0=fabs(muon->innerTrack()->dxy(primVertex->position())); //muon->dB(pat::Muon::PV2D);
	      float dZ = fabs(muon->innerTrack()->dz(primVertex->position()));
	      if(fabs(d0)>maxD0 || fabs(dZ)>maxDZ) continue;

	      double sigmaPt=muon->innerTrack()->ptError();
	      if(fabs(sigmaPt/mPt)>maxRelPtUncertainty) continue;
	    }
	  
	  //global track selection	  
	  if(requireGlobal)
	    {
	      double chi2 = muon->globalTrack()->normalizedChi2();	  
	      int nValidPixelHits = muon->globalTrack()->hitPattern().numberOfValidPixelHits();
	      int nValidTrackerHits = muon->globalTrack()->hitPattern().numberOfValidTrackerHits();
	      int nValidMuonHits = muon->globalTrack()->hitPattern().numberOfValidMuonHits();
	      int nMatches = muon->numberOfMatches();
	      if(chi2>maxTrackChi2
		 || nValidPixelHits<minPixelHits
		 || nValidTrackerHits<minValidTrackerHits 
		 || nValidMuonHits<minValidMuonHits
		 || nMatches<minMatchingMuonStations) continue;
	    }
      


	  //id 
	  if(!id.empty())
	    {
	      bool hasId = (muon->muonID(id)>0) ;
	      if(!hasId) continue;
	    }
	  
	  //isolation
	  std::vector<double> isoVals = getLeptonIso( muonPtr, mPt, rho );
	  double relIso = isoVals[REL_ISO];
	  if(rho>0)     relIso = isoVals[RELRHOCORR_ISO];
	  if(usePFIso)  relIso = isoVals[ doDeltaBetaCorrection ? PFRELBETCORR_ISO : PFREL_ISO];
	  if(applySoftMuonIsolationVeto && mPt>20 && relIso<0.1) continue;
	  else if(relIso>maxRelIso) continue;

	  //muon is selected
	  selMuons.push_back(muonPtr);
	}
      
    }catch(exception &e){
      cout << "[muon::filter] failed with : " << e.what() << endl;
    }    
    
    return selMuons;
}


//
vector<CandidatePtr> getGoodElectrons(edm::Handle<edm::View<reco::Candidate> > &hEle, 
				      edm::Handle<edm::View<reco::Candidate> > &hMu, 
				      const reco::VertexRef &primVertex,
				      const double& rho, 
				      const edm::ParameterSet &iConfig)
{
  vector<CandidatePtr> selElectrons;

  try{
    //config parameters                                                                                                                                                 
    double minPt = iConfig.getParameter<double>("minPt");
    double maxEta = iConfig.getParameter<double>("maxEta");
    bool vetoTransitionElectrons     = iConfig.getParameter<bool>("vetoTransitionElectrons");
    std::vector<double> maxSihih     = iConfig.getParameter<std::vector<double> >("maxSihih");
    std::vector<double> maxDetaTrack = iConfig.getParameter<std::vector<double> >("maxDetaTrack");
    std::vector<double> maxDphiTrack = iConfig.getParameter<std::vector<double> >("maxDphiTrack");
    std::vector<double> maxHoE       = iConfig.getParameter<std::vector<double> >("maxHoE");
    std::vector<double> maxD0        = iConfig.getParameter<std::vector<double> >("maxD0");
    std::vector<double> maxDZ        = iConfig.getParameter<std::vector<double> >("maxDz");
    std::vector<int> maxTrackLostHits = iConfig.getParameter<std::vector<int> >("maxTrackLostHits");
    string applyConversionVetoFrom   = iConfig.getParameter<string>("applyConversionVetoFrom");
    double maxRelIso    = iConfig.getParameter<double>("maxRelIso");
    double minDeltaRtoMuons = iConfig.getParameter<double>("minDeltaRtoMuons");
    bool usePFIso = iConfig.getParameter<bool>("usePFIso");
    bool doDeltaBetaCorrection = iConfig.getParameter<bool>("doDeltaBetaCorrection");
    
    //iterate over the electrons
    for(size_t iElec=0; iElec< hEle.product()->size(); ++iElec)
      {
	reco::CandidatePtr elePtr = hEle->ptrAt(iElec);
	const pat::Electron *ele = dynamic_cast<const pat::Electron *>( elePtr.get() );
	
	//kinematics
	double ePt = ele->pt();
	reco::SuperClusterRef sc = ele->superCluster();
	//double eSuperClusterEt  = sc->energy()/cosh(sc->eta());
	double eEta = ele->eta();
	double scEta= ele->superCluster()->eta();

	if(ePt<minPt || fabs(eEta)>maxEta) continue; 
	if(vetoTransitionElectrons && fabs(scEta)>1.4442 && fabs(scEta)<1.566) continue;

	//electron id (apply on the fly simple cut based)
	bool isEE(ele->isEE());
	double hoe       = ele->hadronicOverEm();
	double dPhiTrack = ele->deltaPhiSuperClusterTrackAtVtx();
	double dEtaTrack = ele->deltaEtaSuperClusterTrackAtVtx();
	double sihih     = ele->sigmaIetaIeta();
	bool hasId( hoe<maxHoE[isEE] && sihih<maxSihih[isEE] && dPhiTrack<maxDphiTrack[isEE] && dEtaTrack<maxDetaTrack[isEE]);
	if(!hasId) continue;

	//vertex compatibility
	const reco::GsfTrackRef & eTrack = ele->gsfTrack();
	double d0=fabs(eTrack->dxy(primVertex->position())); 
	float dZ = fabs(eTrack->dz(primVertex->position()));
	bool isVertexCompatible(d0<maxD0[isEE] && dZ<maxDZ[isEE]);
	if(!isVertexCompatible) continue;
	
	//conversion veto
	int nTrackLostHits=eTrack->trackerExpectedHitsInner().numberOfLostHits();
	bool hasConversionTag(nTrackLostHits>maxTrackLostHits[isEE]);
	if( !applyConversionVetoFrom.empty() )
	  {
	    int eid = (int) ele->electronID(applyConversionVetoFrom);
	    hasConversionTag = !((eid>>2) & 0x1);	 
	  }
	if(hasConversionTag) continue;
	
	//isolation
	std::vector<double> isoVals = getLeptonIso( elePtr, ePt, rho);
	double relIso = isoVals[REL_ISO];
	if(rho>0)     relIso = isoVals[RELRHOCORR_ISO];
	if(usePFIso)  relIso = isoVals[ doDeltaBetaCorrection ? PFRELBETCORR_ISO : PFREL_ISO];
	if(relIso>maxRelIso) continue;
	
	//cross clean with overlapping muons
	bool isOverLappingWithMuon(false);
	for(size_t iMuon=0; iMuon<hMu.product()->size(); ++iMuon)
	  {
	    reco::CandidatePtr muPtr = hMu->ptrAt(iMuon);
	    const pat::Muon *muon = dynamic_cast<const pat::Muon *>( muPtr.get() );
	    
	    if( !muon->isGlobalMuon() && !muon->isTrackerMuon() ) continue;
	    if( muon->innerTrack().isNull() ) continue;
	    if( muon->innerTrack()->numberOfValidHits() <=10 ) continue;
	    
	    double dR = deltaR(*muon->innerTrack(),*eTrack);
	    if(dR>minDeltaRtoMuons) continue;
	    isOverLappingWithMuon=true;
	    break;
	  }
	if(isOverLappingWithMuon) continue;
	
	//the electron is selected (add vertex)
	selElectrons.push_back( elePtr );
      }
  }catch(exception &e){
    cout << "[electron::filter] failed with " << e.what() << endl;
  }
  
  return selElectrons;
}


//
double getLeptonPtError(reco::CandidatePtr &lepton)
{
  double ptError(0);
  int id = fabs(getLeptonId(lepton));
  if( id==MUON )
    {
      const pat::Muon *mu=dynamic_cast<const pat::Muon *>(lepton.get());
      ptError = mu->innerTrack()->ptError();
    }
  else if( id==ELECTRON )
    {
      const pat::Electron *ele=dynamic_cast<const pat::Electron *>( lepton.get() ) ;
      ptError = (lepton->pt()/lepton->p()) * ele->trackMomentumError();
    }
  return ptError;
}
 
//
int getLeptonId(reco::CandidatePtr &lepton)
{
  int id=lepton->charge();
  if( dynamic_cast<const pat::Muon *>( lepton.get() ) ) id *= MUON;
  else if( dynamic_cast<const pat::Electron *>( lepton.get() ) ) id *= ELECTRON;
  return id;
}


//  
const reco::GenParticle *getLeptonGenMatch(reco::CandidatePtr &lepton)
{
  int id=fabs(getLeptonId(lepton));
  if(id==MUON)
    {  
      const pat::Muon *l=dynamic_cast<const pat::Muon *>( lepton.get() );
      return l->genLepton();
    }
  else if(id==ELECTRON)
    {
      const pat::Electron *l=dynamic_cast<const pat::Electron *>( lepton.get() );
      return l->genLepton();
    }

  return 0;
}

//
vector<double> getLeptonIso(reco::CandidatePtr &lepton,float minRelNorm, float rho)
{
  vector<double> leptonIso(11,99999.);
  if(lepton.get()==0) return leptonIso;

  int id=fabs(getLeptonId(lepton));
  if(id==MUON)
    {
      const pat::Muon *l=dynamic_cast<const pat::Muon *>( lepton.get() );
      leptonIso[N_ISO]=l->neutralHadronIso();
      leptonIso[C_ISO]=l->chargedHadronIso();
      leptonIso[G_ISO]=l->photonIso ();
      leptonIso[CPU_ISO]=l->puChargedHadronIso();
      leptonIso[ECAL_ISO]=l->ecalIso();
      leptonIso[HCAL_ISO]=l->hcalIso();
      leptonIso[TRACKER_ISO]=l->trackIso();
    }
  else if( id==ELECTRON )
    {
      const pat::Electron *l=dynamic_cast<const pat::Electron *>( lepton.get() );
      leptonIso[N_ISO]=l->neutralHadronIso();
      leptonIso[C_ISO]=l->chargedHadronIso();
      leptonIso[CPU_ISO]=l->puChargedHadronIso();
      leptonIso[G_ISO]=l->photonIso ();
      leptonIso[ECAL_ISO]=l->ecalIso();
      if(l->isEB()) leptonIso[ECAL_ISO] = max(leptonIso[ECAL_ISO]-1.0,0.);  
      leptonIso[HCAL_ISO]=l->hcalIso();
      leptonIso[TRACKER_ISO]=l->trackIso();
    }

  leptonIso[REL_ISO]        = (leptonIso[ECAL_ISO]+leptonIso[HCAL_ISO]+leptonIso[TRACKER_ISO])/max(float(lepton->pt()),float(minRelNorm));
  leptonIso[RELRHOCORR_ISO] = (max(leptonIso[ECAL_ISO]+leptonIso[HCAL_ISO]-0.3*0.3*3.1415*rho,0.)+leptonIso[TRACKER_ISO])/max(float(lepton->pt()),float(minRelNorm));
  
  leptonIso[PFREL_ISO]        = (leptonIso[N_ISO]+leptonIso[G_ISO]+leptonIso[C_ISO])/max(float(lepton->pt()),float(minRelNorm));
  leptonIso[PFRELBETCORR_ISO] = (max(leptonIso[N_ISO]+leptonIso[G_ISO]-0.5*leptonIso[CPU_ISO],0.)+leptonIso[C_ISO])/max(float(lepton->pt()),float(minRelNorm));
  
  return leptonIso;
}


//
vector<CandidatePtr> getDileptonCandidate(vector<CandidatePtr> &selLeptons,  const edm::ParameterSet &iConfig, const edm::EventSetup &iSetup)
{
  vector<CandidatePtr> selDilepton;
  
  try{
    
    //config parameters
    double minDileptonMass = iConfig.getParameter<double>("minDileptonMass");
    double maxDileptonMass = iConfig.getParameter<double>("maxDileptonMass");

    for(size_t ilep=0; ilep<selLeptons.size(); ilep++)
      {
	reco::CandidatePtr lep1Ptr = selLeptons[ilep];
	  
	//iterate over the second lepton
	for(size_t jlep=ilep+1; jlep<selLeptons.size(); jlep++)
	  {
	    reco::CandidatePtr lep2Ptr = selLeptons[jlep];
	      
	    //compute the mass
	    LorentzVector dilepton=lep1Ptr->p4()+lep2Ptr->p4();
	    double mass = dilepton.mass();
	    if(mass<minDileptonMass || mass >maxDileptonMass) continue;
	      
	    //build the dilepton candidate
	    double candsumpt=lep1Ptr->pt()+lep2Ptr->pt();
	    
	    //if a candidate is already available take this if leading in sum pT
	    if(selDilepton.size()==2) 
	      {
		double sumpt=selDilepton[0]->pt()+selDilepton[1]->pt();
		if(sumpt>candsumpt) continue;
		selDilepton.clear();
	      }
	    
	    //save ordered by pt
	    selDilepton.push_back(lep1Ptr->pt() > lep2Ptr->pt() ? lep1Ptr : lep2Ptr);
	    selDilepton.push_back(lep1Ptr->pt() > lep2Ptr->pt() ? lep2Ptr : lep1Ptr);
	  }
      }
  }
  catch(exception &e){
    cout << "[dilepton] failed with : " << e.what() << endl;
  }
  
  //result
  return selDilepton;
}


//
int getDileptonId(std::vector<CandidatePtr> &selDilepton)
{
  if(selDilepton.size()!=2) return UNKNOWN;
  
  int id1=fabs(getLeptonId(selDilepton[0]));
  int id2=fabs(getLeptonId(selDilepton[1]));
  if( id1==MUON     && id2==MUON)     return MUMU;
  if( id1==ELECTRON && id2==ELECTRON) return EE;
  if( ( id1==MUON && id2==ELECTRON) || ( id1==ELECTRON && id2==MUON)  ) return EMU;
  return UNKNOWN;
}



///                            ///   
/// JET SELECTION UTILITIES    ///
///                            ///   

//
vector<CandidatePtr> getGoodJets(edm::Handle<edm::View<reco::Candidate> > &hJet, vector<CandidatePtr> &selPhysicsObjects, const edm::ParameterSet &iConfig)
{
  
  vector<CandidatePtr> selJets;
  
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
	for(vector<CandidatePtr>::iterator lIt = selPhysicsObjects.begin(); lIt != selPhysicsObjects.end(); lIt++)
	  {
	    double dR = deltaR( *jet, *(lIt->get()) );
	    if(dR > minDR) continue; 
	    minDR = dR;
	  }
	if(minDR < minDeltaRtoLepton) continue;
	
	//jet id
	hasId.set(false);
	if( !jetIdSelector( *jet, hasId ) ) continue;
	
	//jet is selected
	selJets.push_back(jetPtr);
      }
  }catch(exception &e){
    cout << "[jet::filter] failed with " << e.what() << endl;
  }
  
  return selJets;
}


//
double computeVtxAssocFracForJet(const pat::Jet *jet, const reco::Vertex *vtx)
{
  double fassoc(-1);
  if(jet==0 || vtx==0) return fassoc;
  try{
    
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
    
  }catch(exception &e){
    //unable to associate (no tracks embed?)
  }
  
  return fassoc;
}



//                               //
//  PHOTON UTILITIES             //
//                               //

bool getPhotonTrackVeto(const reco::Photon *pho, edm::Handle<std::vector<reco::Track> > &ctfTracks)
{
  if(pho==0) return false;

  //veto against any ctf track
  try{
    if(ctfTracks.isValid())
      {
	for(std::vector<reco::Track>::const_iterator tIt = ctfTracks->begin(); tIt != ctfTracks->end(); tIt++)
	  {
	    double dR=deltaR(pho->eta(),pho->phi(),tIt->eta(),tIt->phi());
	    if(dR>0.1) continue;
	    return true;
	    break;
	  }
      }
  }catch(std::exception &e){ }

  return false;
}

//
vector<CandidatePtr> getGoodPhotons(edm::Handle<edm::View<reco::Candidate> > &hPhoton,
				    EcalClusterLazyTools &lazyTool,
				    edm::Handle<reco::GsfElectronCollection> &hEle,
				    edm::Handle<reco::ConversionCollection> &hConversions,
				    edm::Handle<reco::BeamSpot> &beamSpot,
				    double rho,
				    const edm::ParameterSet &iConfig)
{
  vector<CandidatePtr> selPhotons;
  if(!hPhoton.isValid()) return selPhotons;
  if(hPhoton->size()==0) return selPhotons;

  try{
    double minEt = iConfig.getParameter<double>("minEt");
    double maxEta = iConfig.getParameter<double>("maxEta");

    double minSipipEB=iConfig.getParameter<double>("minSipipEB"); 
    double minSihihEB=iConfig.getParameter<double>("minSihihEB");
    double maxSihihEB=iConfig.getParameter<double>("maxSihihEB");
    double maxSihihEE=iConfig.getParameter<double>("maxSihihEE");

    double maxHoE=iConfig.getParameter<double>("maxHoE");
    std::vector<double> trkIsoCoeffsEB = iConfig.getParameter< std::vector<double> >("trkIsoCoeffsEB");
    std::vector<double> trkIsoCoeffsEE = iConfig.getParameter< std::vector<double> >("trkIsoCoeffsEE");
    std::vector<double> ecalIsoCoeffsEB = iConfig.getParameter< std::vector<double> >("ecalIsoCoeffsEB");
    std::vector<double> ecalIsoCoeffsEE = iConfig.getParameter< std::vector<double> >("ecalIsoCoeffsEE");
    std::vector<double> hcalIsoCoeffsEB = iConfig.getParameter< std::vector<double> >("hcalIsoCoeffsEB");
    std::vector<double> hcalIsoCoeffsEE = iConfig.getParameter< std::vector<double> >("hcalIsoCoeffsEE");

    //iterate over the photons
    for(size_t iPhoton=0; iPhoton< hPhoton.product()->size(); ++iPhoton)
      {
	//get PF photon candidates
	const reco::Photon *pho = dynamic_cast<const reco::Photon *>( hPhoton->ptrAt(iPhoton).get() );
	if(pho==0) continue;
		
	//apply fiducial cuts on photon
	float eta=pho->eta();
	float et=pho->energy()/cosh(eta);
	bool fallsInCrackRegion( fabs(eta)>1.4442 && fabs(eta)<1.566 );
	bool isGood( et>minEt && fabs(eta)<maxEta);
	if(fallsInCrackRegion || !isGood) continue;
	
	//shower shape
	float hoe = pho->hadronicOverEm();
	float sihih = pho->sigmaIetaIeta();
	bool hasGoodShowerShape(hoe<maxHoE);
	if(pho->isEB())
	  {
	    hasGoodShowerShape &= (sihih<maxSihihEB);
	    hasGoodShowerShape &= (sihih>minSihihEB);
	  }
	else
	  {
	    hasGoodShowerShape &= (sihih<maxSihihEE);
	  }

	//these require the photon core
	bool hasPixelSeed(false);
	bool hasElectronVeto(false);
	try{
	  reco::SuperClusterRef scref = pho->superCluster();
	  const reco::CaloClusterPtr  seed_clu = scref->seed();
	  vector<float> cov = lazyTool.localCovariances(*seed_clu);
	  float sipip = sqrt(cov[2]);
	  if(pho->isEB()) hasGoodShowerShape &= (sipip>minSipipEB);

	  //pixel seed veto
	  hasPixelSeed=pho->hasPixelSeed();	

	  if(hEle.isValid() && hConversions.isValid())
	    hasElectronVeto = ConversionTools::hasMatchedPromptElectron(scref, hEle, hConversions, beamSpot->position());
	  
	}catch(std::exception &e){
	  //	  cout << pho->isEB() << " " << pho->isEE() << " " << pho->isStandardPhoton() << " " << pho->isPFlowPhoton() << endl;
	  // cout << e.what() << endl;
	}

	if(!hasGoodShowerShape || hasPixelSeed || hasElectronVeto) continue;

	//isolation
	float maxTrkIso(9999.), maxECALIso(99999.), maxHCALIso(99999.);
	if(pho->isEB())
	  {
	    maxTrkIso  = trkIsoCoeffsEB[0]  + trkIsoCoeffsEB[1]*et  + trkIsoCoeffsEB[2]*rho;
	    maxECALIso = ecalIsoCoeffsEB[0] + ecalIsoCoeffsEB[1]*et + ecalIsoCoeffsEB[2]*rho;
	    maxHCALIso = hcalIsoCoeffsEB[0] + hcalIsoCoeffsEB[1]*et + hcalIsoCoeffsEB[2]*rho;
	  }
	else
	  {
	    maxTrkIso  = trkIsoCoeffsEE[0]  + trkIsoCoeffsEE[1]*et  + trkIsoCoeffsEE[2]*rho;
	    maxECALIso = ecalIsoCoeffsEE[0] + ecalIsoCoeffsEE[1]*et + ecalIsoCoeffsEE[2]*rho;
	    maxHCALIso = hcalIsoCoeffsEE[0] + hcalIsoCoeffsEE[1]*et + hcalIsoCoeffsEE[2]*rho;
	  }
	bool isTrkIso(pho->trkSumPtHollowConeDR04()< maxTrkIso);
	bool isEcalIso(pho->ecalRecHitSumEtConeDR04()< maxECALIso);
	bool isHcalIso(pho->hcalTowerSumEtConeDR04()< maxHCALIso);
	bool isIso(isTrkIso && isEcalIso && isHcalIso);
	if(!isIso) continue;
	
	//save this photon
	selPhotons.push_back( hPhoton->ptrAt(iPhoton) );
      }
  }catch(exception &e){
    cout << "[photon] failed with " << e.what() << endl;
  }
  
  return selPhotons;
}


//                            //
// GENERATOR LEVEL UTILITIES  //
//                            //

//
const reco::Candidate *getGeneratorFinalStateFor(const reco::Candidate *p)
{
  if(p==0) return 0;
  
  const reco::Candidate *prevState=p;
  do{	
    const reco::Candidate *nextState=0;
    int nDaughters = prevState->numberOfDaughters();
    for(int iDaughter=0; iDaughter<nDaughters; iDaughter++)
      {
	const reco::Candidate *dau = prevState->daughter(iDaughter);
	if(dau==0) continue;
	if(dau->pdgId()!= p->pdgId()) continue;
	nextState=dau;	   
	break;
      }
    if(nextState==0) break;
    if(nextState==prevState) break;
    prevState=nextState;
  }while(1);
  
  return prevState;
}


//
std::pair<int,vector<const reco::Candidate *> > assignPhysicsChannel(edm::Handle<edm::View<reco::Candidate> > &genParticles,const edm::ParameterSet &iConfig)
{
  int filterId = iConfig.getParameter<int>("filterId");
  vector<const reco::Candidate *> genTree;

  //iterate over the collection and store leptonic decays from Z/g*, W or t->W
  bool isSignal(false);
  int nZgs(0), nWs(0), nTops(0);
  int nElecs(0), nMuons(0), nTaus(0), nNeutrinos(0);
  for(size_t i = 0; i < genParticles->size(); ++ i)
    {
      const reco::GenParticle & p = dynamic_cast<const reco::GenParticle &>( (*genParticles)[i] );
      if( p.status()!=3) continue;
      int id_p   = abs(p.pdgId());         

      if(id_p== filterId) { genTree.push_back(&p); isSignal=true; continue; }
      
      //select Z/g* or W
      bool isZg(id_p == Z || abs(id_p)==GAMMA); nZgs+=isZg;
      bool isW(id_p == W);                      nWs+=isW;
      bool isTop(id_p == TOP);                  nTops+=isTop;
      if(!isZg && !isW) continue;
      genTree.push_back( &p );
     
      //check leptonic decays of Z/g* or W
      for(size_t b = 0; b < p.numberOfDaughters(); ++ b)
	{
	  const reco::Candidate *p_d = p.daughter(b);
	  int id_d = abs (p_d->pdgId());
	  if(id_d==id_p) continue;
	  if(id_d == ELECTRON) { nElecs++; genTree.push_back(p_d); }
	  if(id_d == MUON) { nMuons++; genTree.push_back(p_d); }
	  if(id_d == TAU) { nTaus++;  genTree.push_back(p_d); } 
	  if(id_d==12 || id_d==14 || id_d==16) { nNeutrinos++; genTree.push_back(p_d); }
	}
    }

  //MC truth 
  int mcChannel((nElecs+nMuons+nTaus) & 0xf);
  mcChannel += ((nNeutrinos & 0xf) << 4);
  mcChannel += ((nElecs & 0xf) << 12);
  mcChannel += ((nMuons & 0xf) << 16);
  mcChannel += ((nTaus & 0xf) << 20);
  if(isSignal)      mcChannel |= (SIGNAL_CH << 8);
  else if(nTops==1) mcChannel |= (SINGLETOP_CH << 8);
  else if(nTops>1)  mcChannel |= (TTBAR_CH << 8);
  else if(nZgs==0)
    {
      if(nWs==1)    mcChannel |= (W_CH << 8);
      else if(nWs>1)     mcChannel |= (WW_CH << 8);
    }
  else if(nZgs>0)
    {
      if(nWs==0) 
	{
	  if(nZgs==1) mcChannel |= (Z_CH << 8);
	  else if(nZgs>1)  mcChannel |= (ZZ_CH << 8);
	}
      else if(nWs==1 && nZgs==1) mcChannel |= (WZ_CH << 8);
    }

  return std::pair<int,vector<const reco::Candidate *> >(mcChannel,genTree);
}


//                    //    
// TRIGGER UTILITILES //
//                    //    

//
pair<string,double> getHighestPhotonTrigThreshold(edm::Handle<edm::TriggerResults> &triggerBitsH,
							    const edm::TriggerNames &triggerNames,
							    vector<string> &gammaTriggers)
{
  double maxthr(0);
  string selTrigger("");
  int ntrigs = triggerBitsH->size();
  for(int itrig=0; itrig<ntrigs; itrig++)
    {
      //check if the trigger fired
      if( !triggerBitsH->wasrun(itrig) ) continue;
      if( triggerBitsH->error(itrig) ) continue;
      if( !triggerBitsH->accept(itrig) ) continue;
      
      //now check if trigger is to be kept                                                                                                                                                                                                                                              
      string trigName = triggerNames.triggerName(itrig);
      if( trigName.find("Photon") == string::npos ) continue;
      
      bool keepTrigger(false);
      for(vector<string>::iterator tIt = gammaTriggers.begin(); tIt != gammaTriggers.end(); tIt++)
	{
            if(trigName.find(*tIt) == string::npos) continue;
            keepTrigger=true;
            break;
	}
      if(!keepTrigger) continue;
      
      //get the trigger threshold
      TString fireTrigger(trigName);
      TObjArray *tkns=fireTrigger.Tokenize("_");
      if(tkns->GetEntriesFast()<2) continue;
      TString phoName=((TObjString *)tkns->At(1))->GetString();
      phoName.ReplaceAll("Photon","");
      Int_t thr=phoName.Atoi();
      
      if(thr<maxthr) continue;
      maxthr=thr;
      selTrigger=trigName;
    }
  return pair<string,double>(selTrigger,maxthr);
}
  

//
bool checkIfTriggerFired(edm::Handle<edm::TriggerResults> &allTriggerBits, const edm::TriggerNames &triggerNames, std::vector<std::string> &triggerPaths)
{
  for (size_t itrig = 0; itrig != allTriggerBits->size(); ++itrig)
    {
      std::string trigName = triggerNames.triggerName(itrig);
      if( !allTriggerBits->wasrun(itrig) ) continue;
      // if(trigName.find("Photon") != std::string::npos) cout << trigName << endl;
      if( allTriggerBits->error(itrig) ) continue;
      if( !allTriggerBits->accept(itrig) ) continue;
      for(size_t ip=0; ip<triggerPaths.size(); ip++)
	{
	  if(trigName.find(triggerPaths[ip])!= std::string::npos)
	    {
	      //cout << trigName << endl;
	      return true;
	    }
	}
    }
  return false;
}
