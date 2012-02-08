#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
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
vector<reco::CandidatePtr> getGoodMuons(edm::Handle<edm::View<reco::Candidate> > &hMu, const reco::BeamSpot &theBeamSpot, const double& rho, const edm::ParameterSet &iConfig)
{
    vector<reco::CandidatePtr> selMuons;
    
    try{
      //config parameters
      double minPt = iConfig.getParameter<double>("minPt");
      double maxEta = iConfig.getParameter<double>("maxEta");
      string id = iConfig.getParameter<string>("id");
      double maxRelIso = iConfig.getParameter<double>("maxRelIso");
      double maxTrackChi2 = iConfig.getParameter<double>("maxTrackChi2");
      int minValidTrackerHits = iConfig.getParameter<int>("minValidTrackerHits");
      int minValidMuonHits = iConfig.getParameter<int>("minValidMuonHits");
      double maxDistToBeamSpot = iConfig.getParameter<double>("maxDistToBeamSpot");
      bool usePFIso = iConfig.getParameter<bool>("usePFIso");
      bool requireTracker = iConfig.getParameter<bool>("requireTracker"); 
      bool requireGlobal = iConfig.getParameter<bool>("requireGlobal"); 
      bool doDeltaBetaCorrection = iConfig.getParameter<bool>("doDeltaBetaCorrection");

      //iterate over the muons
      for(size_t iMuon=0; iMuon< hMu.product()->size(); ++iMuon)      
	{
	  reco::CandidatePtr muonPtr = hMu->ptrAt(iMuon);
	  const pat::Muon *muon = dynamic_cast<const pat::Muon *>( muonPtr.get() );

	  //muon type
	  bool isTracker = muon->isTrackerMuon();
	  if(requireTracker && !isTracker)  continue;
	  bool isGlobal = muon->isGlobalMuon();
	  if(requireGlobal && !isGlobal) continue;

	  //kinematics
	  double mPt = muon->pt();
	  double mEta = muon->eta();
	  if( mPt<minPt || fabs(mEta)>maxEta) continue; 

	  //beam spot compatibility
	  if(requireTracker)
	    {
	      double d0=muon->innerTrack()->dxy(theBeamSpot.position());
	      if(fabs(d0)>maxDistToBeamSpot) continue;
	    }

	  //global track selection	  
	  if(requireGlobal)
	    {
	      double chi2 = muon->globalTrack()->normalizedChi2();	  
	      // int nValidPixelHits = muon->globalTrack()->hitPattern().numberOfValidPixelHits();
	      int nValidTrackerHits = muon->globalTrack()->hitPattern().numberOfValidTrackerHits();
	      int nValidMuonHits = muon->globalTrack()->hitPattern().numberOfValidMuonHits();
	      // int nMatches = muon->numberOfMatches();
	      if(chi2>maxTrackChi2 || nValidTrackerHits < minValidTrackerHits || nValidMuonHits<minValidMuonHits) continue;
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
	  if(relIso>maxRelIso) continue;
	  
	  //muon is selected
	  selMuons.push_back(muonPtr);
	}
      
    }catch(exception &e){
      cout << "[muon::filter] failed with : " << e.what() << endl;
    }    
    
    return selMuons;
}


//
vector<CandidatePtr> getGoodElectrons(edm::Handle<edm::View<reco::Candidate> > &hEle, edm::Handle<edm::View<reco::Candidate> > &hMu, const reco::BeamSpot &theBeamSpot, const double& rho, const edm::ParameterSet &iConfig)
{
  vector<CandidatePtr> selElectrons;

  try{
    //config parameters                                                                                                                                                 
    double minPt = iConfig.getParameter<double>("minPt");
    double minSuperClusterEt = iConfig.getParameter<double>("minSuperClusterEt");
    double maxEta = iConfig.getParameter<double>("maxEta");
    bool applyConversionVeto = iConfig.getParameter<bool>("applyConversionVeto");
    bool vetoTransitionElectrons = iConfig.getParameter<bool>("vetoTransitionElectrons");
    int maxTrackLostHits = iConfig.getParameter<int>("maxTrackLostHits");
    string id = iConfig.getParameter<string>("id");
    double maxRelIso = iConfig.getParameter<double>("maxRelIso");
    double minDeltaRtoMuons = iConfig.getParameter<double>("minDeltaRtoMuons");
    double maxDistToBeamSpot = iConfig.getParameter<double>("maxDistToBeamSpot");
    bool usePFIso = iConfig.getParameter<bool>("usePFIso");
    bool doDeltaBetaCorrection = iConfig.getParameter<bool>("doDeltaBetaCorrection");
    
    //iterate over the electrons
    for(size_t iElec=0; iElec< hEle.product()->size(); ++iElec)
      {
	reco::CandidatePtr elePtr = hEle->ptrAt(iElec);
	const pat::Electron *ele = dynamic_cast<const pat::Electron *>( elePtr.get() );
	//  bool isEcalDriven(true);
	// 	  try{
	// 	    isEcalDriven = ele->ecalDrivenSeed();
	// 	  }catch(exception &e){
	// 	    //it may happen that GsfElectronCore is not stored (rely on the skim)
	// 	  }
	//if(!isEcalDriven) continue;

	int eid=5;  //assume eid+conversion rejection
	if( !id.empty() ) eid = (int) ele->electronID(id);

	//kinematics
	double ePt = ele->pt();
	reco::SuperClusterRef sc = ele->superCluster();
	double eSuperClusterEt  = sc->energy()/cosh(sc->eta());
	double eEta = ele->eta();
	double scEta= ele->superCluster()->eta();

	if(ePt<minPt || fabs(eEta)>maxEta || eSuperClusterEt<minSuperClusterEt) continue; 
	if(vetoTransitionElectrons && fabs(scEta)>1.4442 && fabs(scEta)<1.566) continue;
	
	//conversion veto (from track and info on electron id - 2nd bit)
	const reco::GsfTrackRef & eTrack = ele->gsfTrack();
	double d0=eTrack->dxy(theBeamSpot.position());
	int nTrackLostHits=eTrack->trackerExpectedHitsInner().numberOfLostHits();
	bool hasId =(eid & 0x1);
	bool hasConversionTag = !((eid>>2) & 0x1);	 

	//electron id + conversion veto
	if(fabs(d0)>maxDistToBeamSpot) continue;
	if( !hasId ) continue;
	if(applyConversionVeto && hasConversionTag) continue;
	if( nTrackLostHits>maxTrackLostHits) continue;
	
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

//
vector<CandidatePtr> getGoodPhotons(edm::Handle<edm::View<reco::Candidate> > &hPhoton,
				    EcalClusterLazyTools &lazyTool,
				    edm::Handle<EcalRecHitCollection> ebrechits,
				    edm::Handle<edm::View<reco::Candidate> > &hEle,
				    edm::Handle<vector<reco::Conversion> > &hConversions,
				    double rho,
				    const edm::ParameterSet &iConfig)
{
  vector<CandidatePtr> selPhotons;
  if(!hPhoton.isValid()) return selPhotons;
  if(hPhoton->size()==0) return selPhotons;

  try{
    double minEt = iConfig.getParameter<double>("minEt");
    double maxEta = iConfig.getParameter<double>("maxEta");

    double minSipipEb=iConfig.getParameter<double>("minSipipEB"); 
    double minSihihEB=iConfig.getParameter<double>("minSihihEB");
    double maxSihihEB=iConfig.getParameter<double>("maxSihihEB");
    double maxSihihEE=iConfig.getParameter<double>("maxSihihEE");
    double minR9= iConfig.getParameter<double>("minR9");
    double maxHoE=iConfig.getParameter<double>("maxHoE");
    std::vector<double> trkIsoCoeffsEB = iConfig.getParameter< std::vector<double> >("trkIsoCoeffsEB");
    std::vector<double> trkIsoCoeffsEE = iConfig.getParameter< std::vector<double> >("trkIsoCoeffsEE");
    std::vector<double> ecalIsoCoeffsEB = iConfig.getParameter< std::vector<double> >("ecalIsoCoeffsEB");
    std::vector<double> ecalIsoCoeffsEE = iConfig.getParameter< std::vector<double> >("ecalIsoCoeffsEE");
    std::vector<double> hcalIsoCoeffsEB = iConfig.getParameter< std::vector<double> >("hcalIsoCoeffsEB");
    std::vector<double> hcalIsoCoeffsEE = iConfig.getParameter< std::vector<double> >("hcalIsoCoeffsEE");
    bool requireNoElectron= iConfig.getParameter<bool>("requireNoElectron");

    //iterate over the photons
    for(size_t iPhoton=0; iPhoton< hPhoton.product()->size(); ++iPhoton)
      {
	reco::CandidatePtr photonPtr = hPhoton->ptrAt(iPhoton);
	const pat::Photon *photon = dynamic_cast<const pat::Photon *>( photonPtr.get() );
	
	//kinematics
	float et = photon->et();
	float eta = photon->eta(); 
	bool fallsInCrackRegion( fabs(eta)>1.4442 && fabs(eta)<1.566 );
	bool isGood( et>minEt && fabs(eta)<maxEta && !fallsInCrackRegion);

	//id
	float sihih = photon->sigmaIetaIeta();
	float sipip(999999.);
	float hoe = photon->hadronicOverEm();
	bool hasId(hoe<maxHoE);
	hasId &= (photon->r9()>minR9);
	if(photon->isEB())
	  {
	      hasId &= (sihih<maxSihihEB);
	      hasId &= (sihih>minSihihEB);
	  }
	else
	  {
	    hasId &= (sihih<maxSihihEE);
	  }
	
	//these require the photon core to be stored
	bool hasPixelSeed(false);
	try{
	  
	  reco::SuperClusterRef scphoton = photon->superCluster();
	  eta = scphoton->eta();
	  hasPixelSeed = photon->hasPixelSeed();
	 
	  if(photon->isEB())
	    {
	      const reco::CaloClusterPtr  seed_clu = scphoton->seed();
	      vector<float> cov = lazyTool.localCovariances(*seed_clu);
	      sipip = cov[2];

	      bool hasOutofTime(false);
	      DetId id=scphoton->seed()->hitsAndFractions()[0].first;
      	      EcalRecHitCollection::const_iterator seedcry_rh = ebrechits->find( id );
	      if( seedcry_rh != ebrechits->end() ) hasOutofTime = seedcry_rh->checkFlag(EcalRecHit::kOutOfTime);

	      hasId &= (!hasOutofTime);
	      hasId &= (sipip>minSipipEb);
	    }
	  
	}catch(exception &e){
	  //cout << "Photon core is missing (assuming photon eta and no pixel seed)" << endl;
	}
	

	//check if photon is prompt and uncoverted
	bool isPrompt( !hasPixelSeed );
	int tType(0);
	const reco::Conversion *phoConvMatch = matchPhotonToConversion( photon, hConversions );
	if(phoConvMatch) { isPrompt=false; tType=11*11; }
	float minDr(99999.);
	if(hEle.isValid() && requireNoElectron)
	  {
	    for(edm::View<reco::Candidate>::const_iterator eit = hEle->begin(); eit != hEle->end(); eit++)
	      {
		float dr =deltaR(eit->eta(),eit->phi(),photon->eta(),photon->phi());
		if(dr>minDr) continue;
		minDr=dr;
		tType=11;
	      }
	  }
	if(minDr<0.05) isPrompt=false; 

	//isolation
	float trk_a(trkIsoCoeffsEB[0]),   trk_b(trkIsoCoeffsEB[1]),   trk_c(trkIsoCoeffsEB[2]);
	float ecal_a(hcalIsoCoeffsEB[0]), ecal_b(ecalIsoCoeffsEB[1]), ecal_c(ecalIsoCoeffsEB[2]);
	float hcal_a(hcalIsoCoeffsEB[0]), hcal_b(hcalIsoCoeffsEB[1]), hcal_c(hcalIsoCoeffsEB[2]);
	if(!photon->isEB())
	  {
	    trk_a=trkIsoCoeffsEB[0];   trk_b=trkIsoCoeffsEB[1];   trk_c=trkIsoCoeffsEB[2];
	    ecal_a=hcalIsoCoeffsEB[0]; ecal_b=ecalIsoCoeffsEB[1]; ecal_c=ecalIsoCoeffsEB[2];
	    hcal_a=hcalIsoCoeffsEB[0]; hcal_b=hcalIsoCoeffsEB[1]; hcal_c=hcalIsoCoeffsEB[2];
	  }
	bool isTrkIso(photon->trkSumPtHollowConeDR04()< trk_a+trk_b*et+trk_c*rho);
	bool isEcalIso(photon->ecalRecHitSumEtConeDR04()< ecal_a+ecal_b*et+ecal_c*rho);
	bool isHcalIso(photon->hcalTowerSumEtConeDR04()< hcal_a+hcal_b*et+hcal_c*rho);
	bool isIso(isTrkIso && isEcalIso && isHcalIso);
	
	//final photon selection
	if(!isGood || !isPrompt || !hasId || !isIso) continue;
	selPhotons.push_back( photonPtr );
      }
  }catch(exception &e){
    cout << "[photon] failed with " << e.what() << endl;
  }

  return selPhotons;
}


//
const reco::Conversion *matchPhotonToConversion(const pat::Photon *pho, edm::Handle<vector<reco::Conversion> > &hConversions)
{
  const reco::Conversion *matchConv=0;
  if(pho==0 || !hConversions.isValid()) return matchConv;
  if(hConversions->size()==0) return matchConv;
  
  //photon kinematics
  double phoEta = pho->superCluster()->eta();
  double phoPhi = pho->superCluster()->phi();
  
  //find closest conversion in phase space
  float dEtaMin(9999.), dPhiMin(9999.);   
  for(vector<reco::Conversion>::const_iterator it = hConversions->begin(); it != hConversions->end(); it++)
    {
      LorentzVector pairMom( it->refittedPair4Momentum().Px(),
			     it->refittedPair4Momentum().Py(),
			     it->refittedPair4Momentum().Pz(),
			     it->refittedPair4Momentum().E() );
      if (pairMom.pt() < 1 ) continue;    
      
      const reco::Vertex &convVtx = it->conversionVertex();	
      if(!convVtx.isValid()) continue;
      
      double convEta = pairMom.eta();
      double dEta = convEta-phoEta;
      
      double convPhi = pairMom.phi();
      double dPhi = deltaPhi(convPhi,phoPhi);
      
      if ( fabs(dEta) > dEtaMin || fabs(dPhi) > dPhiMin ) continue; 
      dEtaMin=  fabs(dEta);
      dPhiMin=  fabs(dPhi);
      matchConv = &(*it);
    }
  
  //reset if too far away 
  if ( dEtaMin > 0.1 || dPhiMin > 0.1 ) matchConv=0;
  
  return matchConv;
}



//                            //
// GENERATOR LEVEL UTILITIES  //
//                            //

//
vector<const reco::Candidate *> getGeneratorEvent(edm::Handle<edm::View<reco::Candidate> > &hGen, const edm::ParameterSet &iConfig)
{
  vector<const reco::Candidate *> particles;
  
  try
    {
      int filterId = iConfig.getParameter<int>("filterId");
      //check the mothers and daugthers
      for(size_t igen=0; igen<hGen.product()->size(); igen++)
	{
	  const reco::Candidate *genPtr = hGen->ptrAt(igen).get();
	  if(genPtr->status()!=3) continue;
	  int pdgid=genPtr->pdgId();
	  if(fabs(pdgid)!= filterId) continue;
	  particles.push_back( genPtr );

	  //check mothers
	  for(size_t imother=0; imother<genPtr->numberOfMothers(); imother++)
	    {
	      const reco::Candidate *mother = genPtr->mother(imother);
	      if(mother->pdgId()==pdgid) continue;
	      particles.push_back(mother);
	    }
	  
	  //check daughters
	  for(size_t idau=0; idau<genPtr->numberOfDaughters(); idau++)
	    {
	      const reco::Candidate *daughter = genPtr->daughter(idau);
	      if(daughter->pdgId()==pdgid) continue;
	      particles.push_back(daughter);
	      
	      //check the final states
	      for(size_t igrandau =0; igrandau<daughter->numberOfDaughters(); igrandau++)
		{
		  const reco::Candidate *granddaughter = daughter->daughter(igrandau);
		  if( daughter->pdgId()==granddaughter->pdgId() ) continue;
		  particles.push_back(granddaughter);
		}
	    }

	}
    }catch(exception &e){
    cout << "[generator] failed with : " << e.what() << endl;
  }

  return particles;
}


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
int assignPhysicsChannel(edm::Handle<edm::View<reco::Candidate> > &genParticles)
{
  int dyChannel=NOTDIL;
  
  //iterate over the collection and store Z/gamma->ll decays
  for(size_t i = 0; i < genParticles->size(); ++ i)
    {
      const reco::GenParticle & p = dynamic_cast<const reco::GenParticle &>( (*genParticles)[i] );
      if( p.status()!=3) continue;
      
      int id_p   = p.pdgId();   
      
      //select Z/gamma
      if(abs(id_p) == 23 || abs(id_p)!=22)
	{
  	  int nElecs(0), nMuons(0), nTaus(0);
	  for(size_t b = 0; b < p.numberOfDaughters(); ++ b)
	    {
	      int id_d = abs (p.daughter(b)->pdgId());
	      if(id_d == 11) nElecs++; 
	      if(id_d == 13) nMuons++; 
	      if(id_d == 15) nTaus++;  
	    }
	  if(nElecs>1)      dyChannel=DY_EE;
	  else if(nMuons>1) dyChannel=DY_MUMU;
	  else if(nTaus>1)  dyChannel=DY_TAUTAU;
	}
    }
  
  return dyChannel;
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
      if( allTriggerBits->error(itrig) ) continue;
      if( !allTriggerBits->accept(itrig) ) continue;
      if( find(triggerPaths.begin(), triggerPaths.end(), trigName) == triggerPaths.end() ) continue;
      return true;
    }
  return false;
}
