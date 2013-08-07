#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TString.h"
#include "TSystem.h"
#include "FWCore/Version/interface/GetReleaseVersion.h"

using namespace std;
using namespace reco;

//
ObjectIdSummary::ObjectIdSummary()
{
  p4=LorentzVector(0,0,0,0); id=0;    charge=0;
  genP4=p4;                  genid=0; genflav=0;
  idBits=0;
  for(size_t i=0; i<15; i++) isoVals[i]=0;
  for(size_t i=0; i<5; i++)  mva[i]=0; 
  ensf=0;                       ensferr=0;
  trkd0=0;                      trkdZ=0;             trkip3d=0;
  trkip3dsig=0;                 trkpt=0;             trketa=0; 
  trkphi=0;                     trkchi2=0;           trkValidPixelHits=0;
  trkValidTrackerHits=0;        trkLostInnerHits=0;  trkValidMuonHits=0;
  trkMatches=0;                 innerTrackChi2=0;    trkLayersWithMeasurement=0;
  trkMatchedStations=0;         pixelLayersWithMeasurement=0;
  dPhiTrack=0;                   dEtaTrack=0;         ooemoop=0;
  fbrem=0;                       eopin=0;             hoe=0;
  hoebc=0;                       sihih=0;             sipip=0;
  sce=0;                         sceta=0;             scphi=0;
  e2x5max=0;                     e1x5=0;              e5x5=0;
  h2te=0;                        h2tebc=0;            r9=0;
  dEtaCalo=0;                    dPhiCalo=0;          kfchi2=0;
  kfhits=0;                      kfhitsall=0;         sihip=0;
  nbrems=0;                      etawidth=0;          phiwidth=0;
  e1x5e5x5=0;                    preShowerOverRaw=0;  eopout=0;
  emvatrigv0=0;                  emvanontrigv0=0;
  aeff=0;
  neutHadFrac=0;                 neutEmFrac=0;        chHadFrac=0;
  tche=0;                        csv=0;               jp=0;       tchp=0;
  beta=0;                        betaStar=0;          dRMean=0;
  ptD=0;                         ptRMS=0;
  lxy=0;                         slxy=0;              svmass=0;  svpt=0; svdr=0;
  ssvhe=0;                       ssvhp=0;
  customTaggers.clear();
}

//
ObjectIdSummary::ObjectIdSummary(ObjectIdSummary const&other)
{
  p4=other.p4;           id=other.id;       charge=other.charge;
  genP4=other.genP4;     genid=other.genid; genflav=other.genflav;
  idBits=other.idBits;
  for(size_t i=0; i<15; i++) isoVals[i]=other.isoVals[i];
  for(size_t i=0; i<5; i++)  mva[i]=other.mva[i]; 
  ensf=other.ensf;ensferr=other.ensferr;
  trkd0=other.trkd0; trkdZ=other.trkdZ;                    trkip3d=other.trkip3d;                                        trkip3dsig=other.trkip3dsig;
  trkpt=other.trkpt; trketa=other.trketa;                  trkphi=other.trkphi;                                          trkchi2=other.trkchi2;
  trkValidPixelHits=other.trkValidPixelHits;               trkValidTrackerHits=other.trkValidTrackerHits;                trkLostInnerHits=other.trkLostInnerHits;
  trkValidMuonHits=other.trkValidMuonHits;                 trkMatches=other.trkMatches;                                  innerTrackChi2=other.innerTrackChi2;
  trkLayersWithMeasurement=other.trkLayersWithMeasurement; pixelLayersWithMeasurement=other.pixelLayersWithMeasurement;  trkMatchedStations=other.trkMatchedStations;
  dPhiTrack=other.dPhiTrack;                               dEtaTrack=other.dEtaTrack;                                    ooemoop=other.ooemoop;
  fbrem=other.fbrem;                                       eopin=other.eopin;                                            hoe=other.hoe;
  hoebc=other.hoebc;                                       sihih=other.sihih;                                            sipip=other.sipip;
  sce=other.sce;                                           sceta=other.sceta;                                            scphi=other.scphi;
  e2x5max=other.e2x5max;                                   e1x5=other.e1x5;                                              e5x5=other.e5x5;
  h2te=other.h2te;                                         h2tebc=other.h2tebc;                                          r9=other.r9;
  dEtaCalo=other.dEtaCalo;                                 dPhiCalo=other.dPhiCalo;                                      kfchi2=other.kfchi2;
  kfhits=other.kfhits;                                     kfhitsall=other.kfhitsall;                                    sihip=other.sihip;
  nbrems=other.nbrems;                                     etawidth=other.etawidth;                                      phiwidth=other.phiwidth;
  e1x5e5x5=other.e1x5e5x5;                                 preShowerOverRaw=other.preShowerOverRaw;                      eopout=other.eopout;
  emvatrigv0=other.emvatrigv0;                             emvanontrigv0=other.emvanontrigv0;
  aeff=other.aeff;
  neutHadFrac=other.neutHadFrac;                           neutEmFrac=other.neutEmFrac;                                  chHadFrac=other.chHadFrac;
  tche=other.tche;                                         csv=other.csv;                                                jp=other.jp;
  tchp=other.tchp;
  beta=other.beta;                                         betaStar=other.betaStar;                                      dRMean=other.dRMean;
  ptD=other.ptD;                                           ptRMS=other.ptRMS;
  customTaggers=other.customTaggers;
  lxy=other.lxy;                         slxy=other.slxy;              svmass=other.svmass;
  svpt=other.svpt;                       svdr=other.svdr;
  ssvhe=other.ssvhe;                     ssvhp=other.ssvhp;
}


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
	if(!isReal)  continue;
	
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
					const edm::ParameterSet &iConfig,
					const edm::EventSetup & iSetup,
					std::vector<ObjectIdSummary> &selMuonIds)
{
    vector<reco::CandidatePtr> selMuons;
    selMuonIds.clear();

    bool sourceIsPF            = iConfig.getParameter<bool>("sourceIsPF");
    double minPt               = iConfig.getParameter<double>("minPt");
    double maxEta              = iConfig.getParameter<double>("maxEta");
    string id                  = iConfig.getParameter<string>("id");
    double maxRelIso           = iConfig.getParameter<double>("maxRelIso");
    bool usePFIso              = iConfig.getParameter<bool>("usePFIso");
    bool reComputePFIso        = iConfig.getParameter<bool>("reComputePFIso");
    bool doDeltaBetaCorrection = iConfig.getParameter<bool>("doDeltaBetaCorrection");
    edm::ParameterSet vbtf2011 = iConfig.getParameter<edm::ParameterSet>("vbtf2011"); 
    
    try{
      //iterate over the muons
      for(size_t iMuon=0; iMuon< hMu.product()->size(); ++iMuon)      
	{
	  reco::CandidatePtr muonPtr = hMu->ptrAt(iMuon);
	  const pat::Muon *muon = dynamic_cast<const pat::Muon *>( muonPtr.get() );
	  
	  ObjectIdSummary lepId;
	  lepId.p4 = LorentzVector(muon->px(),muon->py(),muon->pz(),muon->energy());
	  lepId.id=13;
	  const reco::Candidate *genLep = muon->genLepton();
	  lepId.genid   = genLep ? genLep->pdgId() : -9999;
	  lepId.genflav = lepId.genid;
	  if(genLep) lepId.genP4 = LorentzVector(genLep->px(),genLep->py(),genLep->pz(),genLep->energy());
	  lepId.charge=muon->charge();
	  std::vector<double> isoVals = getLeptonIso( muonPtr, muon->pt(), rho );
	  if(reComputePFIso)
	    {
	      const reco::MuonPFIsolation &muIso=muon->pfIsolationR04();
	      isoVals[C_ISO]   = muIso.sumChargedHadronPt;
	      isoVals[CPU_ISO] = muIso.sumPUPt;
	      isoVals[G_ISO]   = muIso.sumPhotonEt;
	      isoVals[N_ISO]   = muIso.sumNeutralHadronEt;
	      isoVals[PFREL_ISO]        = (isoVals[N_ISO]+isoVals[G_ISO]+isoVals[C_ISO])/max(float(muon->pt()),float(20.0));
	      isoVals[PFRELBETCORR_ISO] = (max(isoVals[N_ISO]+isoVals[G_ISO]-0.5*isoVals[CPU_ISO],0.)+isoVals[C_ISO])/max(float(muon->pt()),float(20.0));
	    }

	  for(size_t iiso=0; iiso<isoVals.size(); iiso++) lepId.isoVals[iiso] = isoVals[iiso]; 
	  bool isTracker = muon->isTrackerMuon();
	  if(isTracker)
	    {
	      lepId.ensf                  = 1.0;
	      lepId.ensferr               = fabs(muon->innerTrack()->ptError()/muon->innerTrack()->pt());
	      lepId.trkd0                      = fabs(muon->innerTrack()->dxy(primVertex->position())); //muon->dB(pat::Muon::PV2D);
	      lepId.trkdZ                      = fabs(muon->innerTrack()->dz(primVertex->position()));
	      std::pair<bool,Measurement1D> ip3dRes=getImpactParameter<reco::TrackRef>(muon->innerTrack(), primVertex, iSetup, true);
	      lepId.trkip3d                    = ip3dRes.second.value();
	      lepId.trkip3dsig                 = ip3dRes.second.significance();
	      lepId.trkchi2                    = muon->innerTrack()->normalizedChi2();	  
	      lepId.innerTrackChi2             = lepId.trkchi2;
	      lepId.pixelLayersWithMeasurement = muon->innerTrack()->hitPattern().pixelLayersWithMeasurement();
	      lepId.trkValidPixelHits          = muon->innerTrack()->hitPattern().numberOfValidPixelHits();
	      lepId.trkValidTrackerHits        = muon->innerTrack()->hitPattern().numberOfValidTrackerHits();
	      lepId.trkLayersWithMeasurement   = muon->track()->hitPattern().trackerLayersWithMeasurement();
	      lepId.trkLostInnerHits           = muon->innerTrack()->trackerExpectedHitsInner().numberOfLostHits();
	    }
	  lepId.trkValidMuonHits           = 0;
	  lepId.trkMatches                 = muon->numberOfMatches();
	  lepId.trkMatchedStations         = muon->numberOfMatchedStations();
	  
	  bool isGlobal = muon->isGlobalMuon();
	  if(isGlobal)
	    {
	      lepId.trkchi2 = muon->globalTrack()->normalizedChi2();	  
	      lepId.trkValidPixelHits = muon->globalTrack()->hitPattern().numberOfValidPixelHits();
	      lepId.trkValidTrackerHits = muon->globalTrack()->hitPattern().numberOfValidTrackerHits();
	      lepId.trkValidMuonHits = muon->globalTrack()->hitPattern().numberOfValidMuonHits();
	    }

	  //2012 categories : https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId
	  bool isLoose( (sourceIsPF || muon->isPFMuon()) && (isTracker || isGlobal));
	  bool isSoft(muon->muonID("TMOneStationTight") 
		      && lepId.trkLayersWithMeasurement>5
		      && lepId.pixelLayersWithMeasurement>1
		      && lepId.innerTrackChi2 < 1.8
		      && fabs(lepId.trkd0)<3.
		      && fabs(lepId.trkdZ)<30.);
	  bool isTight( (sourceIsPF || muon->isPFMuon())  
		       && isGlobal
		       && lepId.trkchi2<10.
		       && lepId.trkValidMuonHits>0
		       && lepId.trkMatchedStations>1
		       && fabs(lepId.trkd0)<0.2
		       && fabs(lepId.trkdZ)<0.5
		       && lepId.trkValidPixelHits>0
		       && lepId.trkLayersWithMeasurement>5);
	  bool isHighPt(isGlobal
			&& lepId.trkMatchedStations>1
			&& lepId.trkValidMuonHits>0
			&& fabs(lepId.trkd0)<0.2
			&& fabs(lepId.trkdZ)<0.5
			&& lepId.trkValidPixelHits>0
			&& lepId.trkLayersWithMeasurement>8);
	  
	  bool isSoftVBTF2011(false);
	  bool isVBTF2011(true);
	  if(fabs(lepId.ensferr)  > vbtf2011.getParameter<double>("maxRelPtUncertainty") )             isVBTF2011=false;
	  if(fabs(lepId.trkd0)    > vbtf2011.getParameter<double>("maxD0") 
	     || fabs(lepId.trkdZ) > vbtf2011.getParameter<double>("maxDz") )                           isVBTF2011=false;
	  if(lepId.trkchi2        > vbtf2011.getParameter<double>("maxTrackChi2")
	     || lepId.trkValidPixelHits < vbtf2011.getParameter<int>("minPixelHits") 
	     || lepId.trkValidTrackerHits < vbtf2011.getParameter<int>("minValidTrackerHits") )        isVBTF2011=false; 
	  if(lepId.trkValidMuonHits < vbtf2011.getParameter<int>("minValidMuonHits")
	     || lepId.trkMatches < vbtf2011.getParameter<int>("minMatchingMuonStations") )              isVBTF2011=false;
	  if(!vbtf2011.getParameter<bool>("applySoftMuonIsolationVeto"))
	    {
	      if(!isGlobal || !isTracker) isVBTF2011=false;
	      if(lepId.isoVals[RELRHOCORR_ISO] > maxRelIso)       isVBTF2011=false;
	    }
	  else
	    {
	      if(!isTracker)      isVBTF2011=false;
	      if(muon->pt()>20 && lepId.isoVals[RELRHOCORR_ISO]<0.1) isVBTF2011=false;
	      isSoftVBTF2011 = isVBTF2011;
	    }
	  
	  //complete id information
	  lepId.idBits=( (int(muon->muonID("GlobalMuonPromptTight")) & 0x1) << MID_GLOBALMUONPROMPTTIGHT )
	    | ( (int(muon->muonID("TMLastStationLoose")) & 0x1) << MID_TMLASTSTATIONLOOSE)
	    | ( (int(muon->muonID("TMLastStationTight")) & 0x1) << MID_TMLASTSTATIONTIGHT)
	    | ( (int(muon->muonID("TMLastStationAngTight")) & 0x1) << MID_TMLASTSTATIONANGTIGHT)
	    | ( (int(muon->muonID("TMOneStationTight")) & 0x1) << MID_TMONESTATIONTIGHT)
	    | (  muon->isTrackerMuon() << MID_TRACKER )
	    | (  muon->isGlobalMuon() << MID_GLOBAL )
	    | (  (sourceIsPF || muon->isPFMuon()) << MID_PF )
	    | isLoose << MID_LOOSE
	    | isSoft << MID_SOFT
	    | isTight << MID_TIGHT
	    | isHighPt << MID_HIGHPT
	    | isVBTF2011 << MID_VBTF2011
	    | isSoftVBTF2011 << MID_SOFT2011;

	  //select the muon
	  if( lepId.p4.pt()<minPt || fabs(lepId.p4.eta())>maxEta) continue; 
	  if(!id.empty())
	    {
	      if(id=="loose" && !isLoose) continue;
	      if(id=="soft" && !isSoft) continue;
	      if(id=="tight" && !isTight) continue;
	      if(id=="highpt" && !isHighPt) continue;
	    }
	  double relIso = lepId.isoVals[REL_ISO];
	  if(rho>0)     relIso = lepId.isoVals[RELRHOCORR_ISO];
	  if(usePFIso)  relIso = lepId.isoVals[ doDeltaBetaCorrection ? PFRELBETCORR_ISO : PFREL_ISO];
	  if(relIso>maxRelIso) continue;
	  
	  //muon is selected
	  selMuons.push_back(muonPtr);
	  selMuonIds.push_back(lepId);
	}  
    }catch(exception &e){
      cout << "[muon::filter] failed with : " << e.what() << endl;
    }    
 
    return selMuons;
}


//
vector<CandidatePtr> getGoodElectrons(edm::Handle<edm::View<reco::Candidate> > &hEle, 
				      edm::Handle<edm::View<reco::Candidate> > &hMu, 
				      edm::Handle<reco::VertexCollection> &hVtx,
				      const reco::BeamSpot &beamspot,
				      edm::Handle<reco::ConversionCollection> &hConversions,
				      EGEnergyCorrector *ecorr,
				      EcalClusterLazyTools &lazyTool,
				      PFIsolationEstimator *eIsolator,
				      edm::Handle<reco::PFCandidateCollection> &hPFCands,
				      const double& rho, 
				      const edm::ParameterSet &iConfig,
				      const edm::EventSetup & iSetup,
				      std::vector<ObjectIdSummary> &selElectronIds)
{
  vector<CandidatePtr> selElectrons;
  selElectronIds.clear();
  reco::VertexRef primVtx(hVtx, 0);
  
  try{
    //config parameters
    double minPt = iConfig.getParameter<double>("minPt");
    double maxEta = iConfig.getParameter<double>("maxEta");
    std::string id                    = iConfig.getParameter<std::string>("id");
    bool vetoTransitionElectrons      = iConfig.getParameter<bool>("vetoTransitionElectrons");
    edm::ParameterSet vbtfCfg         = iConfig.getParameter<edm::ParameterSet>("vbtf2011");
    std::vector<double> maxSihih      = vbtfCfg.getParameter<std::vector<double> >("maxSihih");
    std::vector<double> maxDetaTrack  = vbtfCfg.getParameter<std::vector<double> >("maxDetaTrack");
    std::vector<double> maxDphiTrack  = vbtfCfg.getParameter<std::vector<double> >("maxDphiTrack");
    std::vector<double> maxHoE        = vbtfCfg.getParameter<std::vector<double> >("maxHoE");
    std::vector<double> maxD0         = vbtfCfg.getParameter<std::vector<double> >("maxD0");
    std::vector<double> maxDZ         = vbtfCfg.getParameter<std::vector<double> >("maxDz");
    std::vector<int> maxTrackLostHits = vbtfCfg.getParameter<std::vector<int> >("maxTrackLostHits");
    string vbtfConversionVetoSource   = vbtfCfg.getParameter<string>("applyConversionVetoFrom");
    double maxRelIso                  = iConfig.getParameter<double>("maxRelIso");
    bool usePFIso                     = iConfig.getParameter<bool>("usePFIso");
    bool reComputePFIso                 = iConfig.getParameter<bool>("reComputePFIso");
    bool doDeltaBetaCorrection        = iConfig.getParameter<bool>("doDeltaBetaCorrection");
    double minDeltaRtoMuons           = iConfig.getParameter<double>("minDeltaRtoMuons");

    //iterate over the electrons
    for(size_t iElec=0; iElec< hEle.product()->size(); ++iElec)
      {
	reco::CandidatePtr elePtr = hEle->ptrAt(iElec);
	const pat::Electron *ele = dynamic_cast<const pat::Electron *>( elePtr.get() );
	if(ele->gsfTrack().isNull()) continue;
	if(ele->superCluster().isNull()) continue;

	std::pair<bool,Measurement1D> ip3dRes =getImpactParameter<reco::GsfTrackRef>(ele->gsfTrack(), primVtx, iSetup, true);

	bool validKF(false);
	reco::TrackRef myTrackRef = ele->closestCtfTrackRef();
	validKF = (myTrackRef.isAvailable());
	validKF = (myTrackRef.isNonnull());  

	//build a summary of this object
	ObjectIdSummary lepId;
	lepId.p4 = LorentzVector(ele->px(),ele->py(),ele->pz(),ele->energy());
	lepId.id=11;
	const reco::Candidate *genLep = ele->genLepton();
        lepId.genid   = genLep ? genLep->pdgId() : -9999;
        lepId.genflav = lepId.genid;
	if(genLep) lepId.genP4 = LorentzVector(genLep->px(),genLep->py(),genLep->pz(),genLep->energy());
	else        lepId.genP4=LorentzVector(0,0,0,0);
	lepId.charge=ele->charge();
	std::pair<double,double> enSF(1.0,0);
	try{

	  if(ecorr) 
	    {
	      if(!ecorr->IsInitialized())
		{
		  TString path(iConfig.getParameter<std::string>("scCorrector"));
		  gSystem->ExpandPathName(path);
		  if(gSystem->AccessPathName(path))
		    {
		      path=TString("${CMSSW_BASE}/")+gSystem->BaseName(path);
		      gSystem->ExpandPathName(path);
		      
		    }
		  if(!gSystem->AccessPathName(path)) ecorr->Initialize(iSetup,path.Data());
		  else /*{ cout << "Failed to find eSC corrector file" << endl;*/ throw std::exception(); 
		}

#if IS44x == 1
	      enSF=ecorr->CorrectedEnergyWithError(dynamic_cast<const reco::GsfElectron &>(*ele),lazyTool);
#else
	      enSF=ecorr->CorrectedEnergyWithError(*ele,*hVtx,lazyTool,iSetup);
#endif
	      enSF.first = enSF.first/ele->energy();  enSF.second = enSF.second/ele->energy();
	    }
	}
	catch(std::exception &e){
	}
	lepId.ensf              = enSF.first;
	lepId.ensferr           = enSF.second;
	std::vector<double> isoVals=getLeptonIso( elePtr, lepId.p4.pt(), rho);
	if(reComputePFIso && eIsolator && hPFCands.isValid() && hVtx.isValid())
	  {
	    eIsolator->fGetIsolation(dynamic_cast<const reco::GsfElectron *>(ele),&(*hPFCands),primVtx,hVtx);
	    isoVals[C_ISO]   = eIsolator->getIsolationCharged();
	    isoVals[CPU_ISO] = 0; //this one is not computed
	    isoVals[G_ISO]   = eIsolator->getIsolationPhoton();
	    isoVals[N_ISO]   = eIsolator->getIsolationNeutral();
	    isoVals[PFREL_ISO]        = (isoVals[N_ISO]+isoVals[G_ISO]+isoVals[C_ISO])/max(float(ele->pt()),float(20.0));
	    isoVals[PFRELBETCORR_ISO] = (max(isoVals[N_ISO]+isoVals[G_ISO]-0.5*isoVals[CPU_ISO],0.)+isoVals[C_ISO])/max(float(ele->pt()),float(20.0));
	  }
	for(size_t iiso=0; iiso<isoVals.size(); iiso++)	lepId.isoVals[iiso] = isoVals[iiso];
	
	lepId.hoe               = ele->hadronicOverEm();
	lepId.hoebc             = ele->hcalOverEcalBc();
	lepId.dPhiTrack         = ele->deltaPhiSuperClusterTrackAtVtx();
	lepId.dEtaTrack         = ele->deltaEtaSuperClusterTrackAtVtx();
	lepId.sihih             = ele->sigmaIetaIeta();
	lepId.sihip             = 0;
	lepId.sipip             = 0; 
	try { vector<float> cov = lazyTool.localCovariances(*ele->superCluster()->seed()); lepId.sipip=sqrt(cov[2]); lepId.sihip=cov[1]; } catch(std::exception &e) { }
	lepId.ooemoop           = (1.0/ele->ecalEnergy() - ele->eSuperClusterOverP()/ele->ecalEnergy());
	lepId.trkpt             = ele->gsfTrack()->pt();
	lepId.trketa            = ele->gsfTrack()->eta();
	lepId.trkphi            = ele->gsfTrack()->phi();
	lepId.trkd0             = fabs(ele->gsfTrack()->dxy(primVtx->position()));
	lepId.trkdZ             = fabs(ele->gsfTrack()->dz(primVtx->position()));
	lepId.trkchi2           = ele->gsfTrack()->normalizedChi2();
	lepId.trkValidPixelHits = ele->gsfTrack()->hitPattern().numberOfValidPixelHits();
	lepId.trkValidTrackerHits = ele->gsfTrack()->hitPattern().numberOfValidTrackerHits();
	lepId.trkLostInnerHits  = ele->gsfTrack()->trackerExpectedHitsInner().numberOfLostHits();
	lepId.sce               = ele->superCluster()->energy();
	lepId.sceta             = ele->superCluster()->eta();
	lepId.scphi             = ele->superCluster()->phi();
	lepId.e2x5max           = ele->e2x5Max();
	lepId.e1x5              = ele->e1x5();
	lepId.e5x5              = ele->e5x5();
	lepId.h2te              = ele->dr03HcalDepth2TowerSumEt();
	lepId.h2tebc            = ele->dr03HcalDepth2TowerSumEtBc();
	lepId.fbrem             = ele->fbrem();
	lepId.r9                = 0; try{ lepId.r9=lazyTool.e3x3(*ele->superCluster()->seed())/ele->superCluster()->rawEnergy(); } catch(std::exception &e) { }
	lepId.aeff              = 1.0; 
	lepId.eopin             = ele->eSuperClusterOverP(); 
	lepId.trkip3d           = ip3dRes.second.value();
	lepId.trkip3dsig        = ip3dRes.second.significance();
	lepId.eopout            =  ele->eEleClusterOverPout();
	lepId.preShowerOverRaw  =  ele->superCluster()->preshowerEnergy() / ele->superCluster()->rawEnergy();
	lepId.etawidth          =  ele->superCluster()->etaWidth();
	lepId.emvatrigv0        = ele->electronID("mvaTrigV0");
	lepId.emvanontrigv0     = ele->electronID("mvaNonTrigV0");
	lepId.phiwidth          =  ele->superCluster()->phiWidth();
	lepId.e1x5e5x5          =  (ele->e5x5()) !=0. ? 1.-(ele->e1x5()/ele->e5x5()) : -1. ; 
	lepId.dEtaCalo          = ele->deltaEtaSeedClusterTrackAtCalo();
	lepId.dPhiCalo          = ele->deltaPhiSeedClusterTrackAtCalo();
	lepId.nbrems            = fabs(ele->numberOfBrems());
	lepId.kfchi2            = (validKF) ? myTrackRef->normalizedChi2() : 0 ;
	lepId.kfhits            =  (validKF) ? myTrackRef->hitPattern().trackerLayersWithMeasurement() : -1. ;  
	lepId.kfhitsall         =  (validKF) ? myTrackRef->numberOfValidHits() : -1. ; 
  

	//2011 VBTF like id
	bool isEE(ele->isEE());	
	bool hasConversionTag(lepId.trkLostInnerHits>maxTrackLostHits[isEE]);
	int vbtf2011value = (int) ele->electronID(vbtfConversionVetoSource);
	hasConversionTag = !((vbtf2011value>>2) & 0x1);	 
	bool has2011Id = ( lepId.hoe<maxHoE[isEE] 
			   && lepId.sihih<maxSihih[isEE] 
			   && lepId.dPhiTrack<maxDphiTrack[isEE] 
			   && lepId.dEtaTrack<maxDetaTrack[isEE] 
			   && fabs(lepId.trkd0)<maxD0[isEE]
			   && fabs(lepId.trkdZ)<maxDZ[isEE]
			   && !hasConversionTag);
	
	//2012 CUT BASED IDs
	const reco::GsfElectron *gsfEle = dynamic_cast<const reco::GsfElectron *>(ele);
	bool passconversionveto(true);
	if(hConversions.isValid()) passconversionveto = !ConversionTools::hasMatchedConversion(*gsfEle,hConversions,beamspot.position());
	bool passEoP            = EgammaCutBasedEleId::PassEoverPCuts(lepId.sceta,lepId.eopin,lepId.fbrem);
	int  cutBasedIdsToTest[]= {EgammaCutBasedEleId::VETO, EgammaCutBasedEleId::LOOSE, EgammaCutBasedEleId::MEDIUM, EgammaCutBasedEleId::TIGHT};
	bool hasCutBasedIds[]   = {false,                     false,                      false,                       false};
	for(size_t iid=0; iid<4; iid++)
	  {
	    hasCutBasedIds[iid]=EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::WorkingPoint(cutBasedIdsToTest[iid]),
							    ele->isEB(),
							    ele->pt(),ele->eta(),
							    lepId.dEtaTrack, lepId.dPhiTrack, lepId.sihih, lepId.hoe,
							    lepId.ooemoop, lepId.trkd0, lepId.trkdZ, 
							    0., 0., 0.,
							    !passconversionveto, uint(lepId.trkLostInnerHits),
							    rho);
// 	    if(!hasCutBasedIds[iid] && lepId.genP4.pt()>0 && lepId.p4.pt()<30 && iid==2)
// 	      {
// 		int flag=EgammaCutBasedEleId::TestWP(EgammaCutBasedEleId::WorkingPoint(cutBasedIdsToTest[iid]),
// 						     ele->isEB(),
// 						     ele->pt(),ele->eta(),
// 						     lepId.dEtaTrack, lepId.dPhiTrack, lepId.sihih, lepId.hoe,
// 						     lepId.ooemoop, lepId.trkd0, lepId.trkdZ,
// 						     0., 0., 0.,
// 						     !passconversionveto, uint(lepId.trkLostInnerHits),
// 						     rho);
// 		cout << lepId.genP4.pt() << " " << lepId.p4.pt() << endl;
// 		EgammaCutBasedEleId::PrintDebug(flag);
// 	      }
	  }
	int triggerCutsToTest[] = {EgammaCutBasedEleId::TRIGGERTIGHT,EgammaCutBasedEleId::TRIGGERWP70};
	bool passTriggerCut[]   = {false,                            false};
	for(size_t icut=0; icut<2; icut++)
	  {
	    passTriggerCut[icut] = EgammaCutBasedEleId::PassTriggerCuts(EgammaCutBasedEleId::TriggerWorkingPoint(triggerCutsToTest[icut]),
									ele->isEB(),
									ele->pt(),
									lepId.dEtaTrack, lepId.dPhiTrack, lepId.sihih, lepId.hoe,
									lepId.isoVals[TRACKER_ISO],lepId.isoVals[ECAL_ISO],lepId.isoVals[HCAL_ISO]);
	  }
	
	//build a summary of IDs
	lepId.idBits = has2011Id << EID_VBTF2011 |
	  hasCutBasedIds[0] << EID_VETO | 
	  hasCutBasedIds[1] << EID_LOOSE | 
	  hasCutBasedIds[2] << EID_MEDIUM | 
	  hasCutBasedIds[3] << EID_TIGHT |
	  passconversionveto << EID_CONVERSIONVETO |
	  passEoP << EID_EOPCUT |
	  passTriggerCut[1] << EID_TRIGGER2011 |
	  passTriggerCut[0] << EID_TIGHTTRIGGER |
	  ele->ecalDrivenSeed() << EID_ECALDRIVEN |
	  ele->trackerDrivenSeed() << EID_TRACKERDRIVEN;

	//now do its selection
	if(lepId.p4.pt()<minPt || fabs(lepId.p4.eta())>maxEta) continue; 
	if(vetoTransitionElectrons && fabs(lepId.sceta)>1.4442 && fabs(lepId.sceta)<1.566) continue;
	if(!id.empty())
	  {
	    if(id=="veto" && !hasCutBasedIds[0]) continue;
	    if(id=="loose" && !hasCutBasedIds[1]) continue;
	    if(id=="medium" && !hasCutBasedIds[2]) continue;
	    if(id=="tight" && !hasCutBasedIds[3]) continue;
	  }
	double relIso        = lepId.isoVals[REL_ISO];
	if(rho>0)     relIso = lepId.isoVals[RELRHOCORR_ISO];
	if(usePFIso)  relIso = lepId.isoVals[ doDeltaBetaCorrection ? PFRELBETCORR_ISO : PFREL_ISO];
	if(relIso>maxRelIso) continue;
	
	//final cross clean with overlapping muons
	bool isOverLappingWithMuon(false);
	for(size_t iMuon=0; iMuon<hMu.product()->size(); ++iMuon)
	  {
	    reco::CandidatePtr muPtr = hMu->ptrAt(iMuon);
	    const pat::Muon *muon = dynamic_cast<const pat::Muon *>( muPtr.get() );
	    
	    if( !muon->isGlobalMuon() && !muon->isTrackerMuon() ) continue;
	    if( muon->innerTrack().isNull() ) continue;
	    if( muon->innerTrack()->numberOfValidHits() <=10 ) continue;
	    
	    double dR = deltaR(*muon->innerTrack(),*ele->gsfTrack());
	    if(dR>minDeltaRtoMuons) continue;
	    isOverLappingWithMuon=true;
	    break;
	  }
	if(isOverLappingWithMuon) continue;
	
	//the electron is selected (add id summary)
	selElectronIds.push_back(lepId);
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
std::vector<int> getDileptonCandidate(vector<CandidatePtr> &selLeptons,  const edm::ParameterSet &iConfig, const edm::EventSetup &iSetup)
{
  vector<int> selDilepton;
  
  try{
    
    //config parameters
    double minDileptonMass = iConfig.getParameter<double>("minDileptonMass");
    double maxDileptonMass = iConfig.getParameter<double>("maxDileptonMass");
    double leadSumPt(0);
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
	    if(selDilepton.size()>0)
	      {
		if(candsumpt<leadSumPt) continue;
		selDilepton.clear();
	      }
	    
	    //save ordered by pt
	    leadSumPt=candsumpt;
	    selDilepton.push_back(lep1Ptr->pt() > lep2Ptr->pt() ? ilep : jlep );
	    selDilepton.push_back(lep1Ptr->pt() > lep2Ptr->pt() ? jlep : ilep );
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
int getDileptonId(CandidatePtr &l1,CandidatePtr &l2)
{
  int id1=fabs(getLeptonId(l1));
  int id2=fabs(getLeptonId(l2));
  if( id1==MUON     && id2==MUON)     return MUMU;
  if( id1==ELECTRON && id2==ELECTRON) return EE;
  if( ( id1==MUON && id2==ELECTRON) || ( id1==ELECTRON && id2==MUON)  ) return EMU;
  return UNKNOWN;
}



///                            ///   
/// JET SELECTION UTILITIES    ///
///                            ///   

//
vector<CandidatePtr> getGoodJets(edm::Handle<edm::View<reco::Candidate> > &hJet, 
				 vector<CandidatePtr> &selPhysicsObjects, 
				 edm::Handle<reco::VertexCollection> &hVtx,
				 std::vector<PileupJetIdAlgo *> &puJetIdAlgo,
				 std::vector<edm::Handle<reco::JetTagCollection> > &jetTagsH,
				 const edm::ParameterSet &iConfig,
				 std::vector<ObjectIdSummary> &selJetsId)
{
  
  vector<CandidatePtr> selJets;
  selJetsId.clear();
  reco::VertexRef primVtx(hVtx, 0);

  using namespace edm;
  try{
    
    //config parameters
    double minPt = iConfig.getParameter<double>("minPt");
    double maxEta = iConfig.getParameter<double>("maxEta");
    double minDeltaRtoLepton = iConfig.getParameter<double>("minDeltaRtoLepton");
    PFJetIDSelectionFunctor looseJetIdSelector(PFJetIDSelectionFunctor::FIRSTDATA,PFJetIDSelectionFunctor::LOOSE);
    PFJetIDSelectionFunctor tightJetIdSelector(PFJetIDSelectionFunctor::FIRSTDATA,PFJetIDSelectionFunctor::TIGHT);
    pat::strbitset hasLooseId = looseJetIdSelector.getBitTemplate();
    pat::strbitset hasTightId = tightJetIdSelector.getBitTemplate();

    //iterate over the jets
    for(size_t iJet=0; iJet< hJet.product()->size(); ++iJet)
      {
	reco::CandidatePtr jetPtr = hJet->ptrAt(iJet);
	const pat::Jet *jet = dynamic_cast<const pat::Jet *>( jetPtr.get() );

	ObjectIdSummary jetId;
        jetId.p4 = LorentzVector(jet->px(),jet->py(), jet->pz(), jet->energy());
	jetId.id=1;
	const reco::Candidate *genParton = jet->genParton();
        jetId.genid   = genParton ? genParton->pdgId() : -9999;
        jetId.genflav = jet->partonFlavour();
	const reco::GenJet *gJet=jet->genJet();
	if(gJet) jetId.genP4 = LorentzVector(gJet->px(),gJet->py(),gJet->pz(),gJet->energy());
	jetId.charge = jet->charge();
	jetId.ensf = jet->jecFactor("Uncorrected");
	jetId.ensferr=0;
	jetId.tche=jet->bDiscriminator("trackCountingHighEffBJetTags");
	jetId.tchp=jet->bDiscriminator("trackCountingHighPurBJetTags");
	jetId.csv=jet->bDiscriminator("combinedSecondaryVertexBJetTags");
	jetId.ssvhe=jet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
	jetId.ssvhp=jet->bDiscriminator("simpleSecondaryVertexHighPurBJetTags");
	jetId.jp=jet->bDiscriminator("jetProbabilityBJetTags");
	for(size_t ijt=0; ijt<jetTagsH.size(); ijt++)
	  {
	    int idx=getJetTag(jet,jetTagsH[ijt]);
	    float disc(-1);
	    if(idx>=0) disc=(*(jetTagsH[ijt]))[idx].second;
	    jetId.customTaggers.push_back(disc);
	  }
	reco::SecondaryVertexTagInfo const *svTagInfos = jet->tagInfoSecondaryVertex("secondaryVertex");
	if( svTagInfos != 0 )
	  {
	    int nSecVtx=svTagInfos->nVertices();
	    if(nSecVtx>0)
	      {
		jetId.lxy=svTagInfos->flightDistance(0).value();
		jetId.slxy=svTagInfos->flightDistance(0).error();
		jetId.svmass=svTagInfos->secondaryVertex(0).p4().mass();
		jetId.svpt=svTagInfos->secondaryVertex(0).p4().pt();
		jetId.svdr=deltaR(svTagInfos->secondaryVertex(0).p4(),jet->p4());

	      }
	  }
	jetId.neutHadFrac = jet->neutralHadronEnergyFraction();
        jetId.neutEmFrac  = jet->neutralEmEnergyFraction();
        jetId.chHadFrac   = jet->chargedHadronEnergyFraction();
	jetId.r9          = jet->n90();
	std::vector<int> puIdFlags(puJetIdAlgo.size(),0);
	for(size_t iid=0; iid<puJetIdAlgo.size(); iid++)
	  {
	    PileupJetIdentifier puIdentifier = puJetIdAlgo[iid]->computeIdVariables(dynamic_cast<const reco::Jet*>(jet), 0, primVtx.get(), *hVtx.product(), true);
	    if(iid==0){
	      jetId.sihih       = puIdentifier.etaW();
	      jetId.sipip       = puIdentifier.phiW();
	      jetId.beta        = puIdentifier.beta();
	      jetId.betaStar    = puIdentifier.betaStar();
	      jetId.dRMean      = puIdentifier.dRMean();
	      jetId.ptD         = puIdentifier.ptD();
	      jetId.ptRMS       = puIdentifier.ptRMS();
	    }
	    puIdFlags[iid]    = int(puIdentifier.idFlag());
	    jetId.mva[iid]= puIdentifier.mva();
	  }
		
	//check overlaps with selected leptons
	double minDR(1000);
	for(vector<CandidatePtr>::iterator lIt = selPhysicsObjects.begin(); lIt != selPhysicsObjects.end(); lIt++)
	  {
	    double dR = deltaR( *jet, *(lIt->get()) );
	    if(dR > minDR) continue; 
	    minDR = dR;
	  }
	jetId.isoVals[TRACKER_ISO]=minDR;

	//jet id
	hasLooseId.set(false);
	hasTightId.set(false);
	bool passLooseId(looseJetIdSelector( *jet, hasLooseId ));
	bool passTightId(tightJetIdSelector( *jet, hasTightId ));
	jetId.idBits=passLooseId | (passTightId << 1 );
	for(size_t iid=0; iid<puJetIdAlgo.size(); iid++) jetId.idBits |= ((puIdFlags[iid] & 0xf) << (iid+1)*4);

	if(jet->pt()<minPt || fabs(jet->eta())>maxEta || minDR < minDeltaRtoLepton || !passLooseId) continue;
	
	//jet is selected
	selJets.push_back(jetPtr);
	selJetsId.push_back(jetId);
      }
  }catch(exception &e){
    cout << "[jet::filter] failed with " << e.what() << endl;
  }
  
  return selJets;
}

//
int getJetTag(const pat::Jet *jet, edm::Handle<reco::JetTagCollection > jetTags)
{
  int result = -1;
  if(jet==0) return result;

  //match by DeltaR
  for (size_t t = 0; t < jetTags->size(); t++) {
    edm::RefToBase<reco::Jet> jet_p = (*jetTags)[t].first;
    if (jet_p.isNull())  continue;
    if( deltaR( jet_p->eta(), jet_p->phi(), jet->eta(), jet->phi()) > 0.3 ) continue;
    result = (int ) t;
    break;
  }
  return result;
}


//
std::pair<double,double> computeBetaForJet(const pat::Jet *jet, edm::Handle<reco::VertexCollection> &hVtx)
{
  double beta(0),betaStar(0), sumTkPt(0);
  reco::VertexRef primVtx(hVtx, 0);
  std::vector<reco::PFCandidatePtr> const &constituents=jet->getPFConstituents(); 
  for(std::vector<reco::PFCandidatePtr>::const_iterator it=constituents.begin(); it!=constituents.end(); ++it) 
    {
      const reco::PFCandidatePtr & icand = *it;
      if( icand->particleId() != reco::PFCandidate::h ) continue;
      if( !icand->trackRef().isNonnull() || !icand->trackRef().isAvailable() ) continue;
      float tkpt = icand->trackRef()->pt(); 
      sumTkPt += tkpt;
      for(reco::VertexCollection::const_iterator  vi=hVtx->begin(); vi!=hVtx->end(); ++vi ) 
	{
	  const reco::Vertex & iv = *vi;
	  float dZ = fabs(icand->trackRef()->dz(primVtx->position()));
	  if( dZ < 0.2 ) {
	    if( (iv.position() - primVtx->position()).r() < 0.02 ) beta += tkpt;
	    else                                                   betaStar += tkpt;
	  }
	  
	}
    }
  if(sumTkPt>0) { beta/=sumTkPt; betaStar /=sumTkPt; }
  return std::pair<double,double>(beta,betaStar);
}



//                               //
//  PHOTON UTILITIES             //
//                               //
vector<CandidatePtr> getGoodPhotons(edm::Handle<edm::View<reco::Candidate> > &hPhoton,
				    EGEnergyCorrector *phocorr,
				    EcalClusterLazyTools &lazyTool,
				    edm::Handle<EcalRecHitCollection> ebrechits,
				    edm::Handle<reco::GsfElectronCollection> &hEle,
				    edm::Handle<reco::ConversionCollection> &hConversions,
				    edm::Handle<reco::TrackCollection> &hTracks,
				    edm::Handle<reco::VertexCollection> &hVtx,
				    edm::Handle<reco::BeamSpot> &beamSpot,
				    double rho,
				    const edm::ParameterSet &iConfig,
				    const edm::EventSetup & iSetup,
				    std::vector<ObjectIdSummary> &selPhotonIds)
{
  vector<CandidatePtr> selPhotons;
  selPhotonIds.clear();
  reco::VertexRef primVertex(hVtx, 0);

  try{
    //config parameters
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
	//get photon candidates
	const reco::Photon *pho = dynamic_cast<const reco::Photon *>( hPhoton->ptrAt(iPhoton).get() );
	//const pat::Photon *pho = dynamic_cast<const pat::Photon *>( hPhoton->ptrAt(iPhoton).get() );
	if(pho==0) continue;
	
	ObjectIdSummary phoId;
	phoId.p4 = LorentzVector(pho->px(),pho->py(),pho->pz(),pho->energy());
	phoId.id=22;
	const reco::Candidate *genPho = 0; //pho->genPhoton();
        phoId.genid   = genPho ? genPho->pdgId() : -9999;
        phoId.genflav = phoId.genid;
	if(genPho) phoId.genP4 = LorentzVector(genPho->px(),genPho->py(),genPho->pz(),genPho->energy());
	phoId.charge=0;
	std::pair<double,double> enSF(1.0,0);
	try{
	  if(phocorr) 
	    {
	      if(!phocorr->IsInitialized())
		{
		  TString path(iConfig.getParameter<std::string>("scCorrector"));
		  gSystem->ExpandPathName(path);
		  if(gSystem->AccessPathName(path))
		    {
		      path=TString("${CMSSW_BASE}/")+gSystem->BaseName(path);
		      gSystem->ExpandPathName(path);
		    }
		  if(!gSystem->AccessPathName(path)) phocorr->Initialize(iSetup,path.Data());
		  else /*{ cout << "Failed to find g SC corrector file" << endl;*/ throw std::exception(); 
		}
#if IS44x == 1
	      enSF=phocorr->CorrectedEnergyWithError(*pho);
#else
	      enSF=phocorr->CorrectedEnergyWithError(*pho,*hVtx,lazyTool,iSetup);
#endif
	      enSF.first = enSF.first/pho->energy();  enSF.second = enSF.second/pho->energy();
	    }
	}
	catch(std::exception &e){
	}

	phoId.ensf              = enSF.first;
	phoId.ensferr           = enSF.second;
	phoId.hoe               = pho->hadronicOverEm();
#if IS44x == 0
	phoId.hoebc           = pho->hadTowOverEm();
#endif
	phoId.sihih             = pho->sigmaIetaIeta();
	phoId.sipip             =0; try{ vector<float> cov       = lazyTool.localCovariances(*pho->superCluster()->seed()); phoId.sipip=!isnan(cov[2]) ? sqrt(cov[2]) : 0.; } catch(std::exception &e){ }
	phoId.sce               = pho->superCluster()->energy();
	phoId.sceta             = pho->superCluster()->eta();
	phoId.scphi             = pho->superCluster()->phi();
	phoId.e2x5max           = pho->e2x5();
	phoId.e1x5              = pho->e1x5();
	phoId.e5x5              = pho->e5x5();
	phoId.h2te              = pho->hadronicDepth2OverEm();
#if IS44x == 0
	phoId.h2tebc            = pho->hadTowDepth2OverEm();
#endif
	phoId.r9                = pho->r9();
	phoId.aeff              = 1.0;
	phoId.isoVals[TRACKER_ISO]=pho->trkSumPtHollowConeDR04();
	phoId.isoVals[ECAL_ISO]=pho->ecalRecHitSumEtConeDR04();
	phoId.isoVals[HCAL_ISO]=pho->hcalTowerSumEtConeDR04();
	phoId.isoVals[C_ISO]=pho->chargedHadronIso();
	phoId.isoVals[N_ISO]=pho->neutralHadronIso();
	phoId.isoVals[G_ISO]=pho->photonIso();

	bool hasBadSeed(false);
	if(pho->isEB() && ebrechits.isValid())
	  {
	    try{
	      DetId id=pho->superCluster()->seed()->hitsAndFractions()[0].first;
	      EcalRecHitCollection::const_iterator seedcry_rh = ebrechits->find( id );
	      if( seedcry_rh != ebrechits->end() ) 
		{
		  hasBadSeed |= seedcry_rh->checkFlag(EcalRecHit::kOutOfTime);
		  hasBadSeed |= seedcry_rh->checkFlag(EcalRecHit::kWeird);
		}
	    }catch(std::exception &e){
	    }
	  }

	bool hasPixelSeed=pho->hasPixelSeed();	
	bool hasElectronVeto(false); try{ hasElectronVeto = ConversionTools::hasMatchedPromptElectron(pho->superCluster(), hEle, hConversions, beamSpot->position()); } catch(std::exception &e) { }
	bool hasConvUnsafeElectronVeto(false);
	if(hEle.isValid())
	  {
	    double minDR(99999.);
	    for(reco::GsfElectronCollection::const_iterator eIt = hEle->begin(); eIt != hEle->end(); eIt++)
	      {
		double dR=deltaR(eIt->eta(),eIt->phi(),pho->eta(),pho->phi());
		minDR = min(minDR,dR);
	      }
	    if(minDR<0.15) hasConvUnsafeElectronVeto=true;
	  }

	reco::ConversionRef conv;    try{ conv = ConversionTools::matchedConversion(*(pho->superCluster()),hConversions,beamSpot->position()); } catch(std::exception &e) { }
	bool isConverted(!conv.isNull());
	bool isVtxConstrained(false);
	if(isConverted)
	  {
	    phoId.trkpt=conv->refittedPair4Momentum().pt();
	    phoId.trketa=conv->refittedPair4Momentum().eta();
	    phoId.trkphi=conv->refittedPair4Momentum().phi();
	    isVtxConstrained=(fabs(primVertex->position().z()-conv->conversionVertex().position().z()) < 0.1);
	  }
	bool hasTrkVeto(false);
	for(std::vector<reco::Track>::const_iterator tIt = hTracks->begin(); tIt != hTracks->end(); tIt++)
          {
            double dR=deltaR(pho->eta(),pho->phi(),tIt->eta(),tIt->phi());
            if(dR>0.1) continue;
            hasTrkVeto = true;
            break;
          }
	
	phoId.idBits = !hasPixelSeed |
	  (!hasElectronVeto << 1) |
	  (hasTrkVeto << 2 ) |
	  (isConverted << 3) |
	  (isVtxConstrained << 4) |
	  (!hasConvUnsafeElectronVeto << 5);
	
	//select the photon
	float et=pho->energy()/cosh(pho->eta());
	bool fallsInCrackRegion( fabs(pho->eta())>1.4442 && fabs(pho->eta())<1.566 );
	bool isGood( et>minEt && fabs(pho->eta())<maxEta);
	bool hasBaseId(phoId.hoe<maxHoE);
	if(pho->isEB())
	  {
	    hasBaseId &= (phoId.sihih<maxSihihEB);
	    hasBaseId &= (phoId.sihih>minSihihEB);
	    hasBaseId &= (phoId.sipip>minSipipEB);
	  }
	else
	  {
	    hasBaseId &= (phoId.sihih<maxSihihEE);
	  }
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
	if(fallsInCrackRegion || !isGood || !hasBaseId || hasBadSeed || hasPixelSeed /*|| hasElectronVeto*/ || !isIso) continue;

// 	cout << phoId.p4.pt() << " " << phoId.ensf << " px:" << hasPixelSeed
// 	     << " eveto:" << hasElectronVeto
// 	     << " trkveto:" << hasTrkVeto
// 	     << " conv:" << isConverted
// 	     << " vtx: " << isVtxConstrained 
// 	     << " eveto_danger:" << hasConvUnsafeElectronVeto << endl;

	//save this photon
	selPhotons.push_back( hPhoton->ptrAt(iPhoton) );
	selPhotonIds.push_back( phoId );
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
  int nPromptPhotons(0);
  for(size_t i = 0; i < genParticles->size(); ++ i)
    {
      const reco::GenParticle & p = dynamic_cast<const reco::GenParticle &>( (*genParticles)[i] );

      //PROMPT PHOTON COUNTING from Hgg
      if(p.status()==1 && abs(p.pdgId()==22) && p.pt()>20)
	{
	  bool isPromptOrRad(false);
	  for(size_t b = 0; b < p.numberOfMothers(); ++ b)
	    {
	      const reco::Candidate *p_m = p.mother(b);
	      if(abs(p_m->pdgId())>25) continue;
	      isPromptOrRad=true;
	    }
	  if(isPromptOrRad){
	    genTree.push_back(&p);
	    nPromptPhotons++;
	  }
	}

      if( p.status()!=3) continue;
      int id_p   = abs(p.pdgId());         
      if(id_p== filterId) { genTree.push_back(&p); isSignal=true; continue; }
      
      //select Z/g* or W
      bool isZg(id_p == Z || abs(id_p)==GAMMA); nZgs+=isZg;
      bool isW(id_p == W);                      nWs+=isW;
      bool isTop(id_p == TOP);                  nTops+=isTop;
      //this is a patch for Sherpa in which the bosons are simply absent
      if(!isZg && !isW) 
	{
	  if(id_p == ELECTRON)                 { nElecs++;     genTree.push_back(&p); }
	  if(id_p == MUON)                     { nMuons++;     genTree.push_back(&p); }
	  if(id_p == TAU)                      { nTaus++;      genTree.push_back(&p); } 
	  if(id_p==12 || id_p==14 || id_p==16) { nNeutrinos++; genTree.push_back(&p); }
	  continue;
	}
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
  mcChannel += (nPromptPhotons << 28);

  return std::pair<int,vector<const reco::Candidate *> >(mcChannel,genTree);
}

//
std::vector<reco::CandidatePtr> filterHFfromGSplit(edm::Handle<edm::View<reco::Candidate> > &genParticles)
{
  std::vector<reco::CandidatePtr> hfFromGsplit;
  for (size_t ip=0; ip<genParticles.product()->size()-1; ip++)
    {
      reco::CandidatePtr genP = genParticles->ptrAt(ip);
      int absid=abs(genP->pdgId());
      bool isHFquark( genP->status()==2 && (absid==5 || absid==4) );
      bool isBhadron( genP->status()==2 && ( (absid>= 5122 && absid<= 5554) || 
					     (absid>=20513 && absid<=20543) ||
					     (absid>=10511 && absid<=10543) || 
					     (absid>=  511 && absid<=  545) ) );
      if(!isHFquark && !isBhadron) continue;

      //check if it has not been stored previously
      if(!isBhadron)
	{
	  bool hasOverlap(false);
	  for(size_t jp=0; jp<hfFromGsplit.size(); jp++)
	    {
	      float dR=deltaR(genP->eta(),genP->phi(),hfFromGsplit[jp]->eta(),hfFromGsplit[jp]->phi());
	      if(dR>0.1) continue;
	      hasOverlap=true;
	      break;
	    }
	  if(hasOverlap) continue;
	}
      hfFromGsplit.push_back( genP );
    }

  return hfFromGsplit;
}

//
const reco::Candidate *getHFmatchFromGSplit(reco::CandidatePtr &jet, std::vector<reco::CandidatePtr> &hfFromGsplit,int flavId)
{
  if(hfFromGsplit.size()==0) return 0;

  const reco::Candidate *cand=0;
  double minDR(9999.);
  for(size_t ip=0; ip<hfFromGsplit.size(); ip++)
    {
      if( abs(hfFromGsplit[ip]->pdgId())!=flavId ) continue;
      double dR=deltaR(jet->eta(),jet->phi(),hfFromGsplit[ip]->eta(),hfFromGsplit[ip]->phi());
      if(dR>minDR) continue;
      minDR=dR;
      cand=hfFromGsplit[ip].get();
    }
  if(minDR>0.3) return 0;
  return cand;
}



//                    //    
// TRIGGER UTILITILES //
//                    //    

//
pair<string,double> getHighestPhotonTrigThreshold(edm::Handle<edm::TriggerResults> &triggerBitsH,
						  const edm::TriggerNames &triggerNames,
						  vector<string> &gammaTriggers,
						  unsigned int &gammaTriggerWord)
{
  int maxthr(0);
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
      int gTrigCount(0);
      for(vector<string>::iterator tIt = gammaTriggers.begin(); tIt != gammaTriggers.end(); tIt++,gTrigCount++)
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
      gammaTriggerWord |= (1<<gTrigCount);

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
	  if(trigName.find(triggerPaths[ip])!= std::string::npos) return true;
	}
    }
  return false;
}
