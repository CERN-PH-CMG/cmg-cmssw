#ifndef objectfilters_h
#define objectfilters_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "RecoEgamma/EgammaTools/interface/EGEnergyCorrector.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"


#include "EGamma/EGammaAnalysisTools/interface/PFIsolationEstimator.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "CMGTools/External/interface/PileupJetIdAlgo.h"

#include "TVector3.h"
#include "TH1D.h"

#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

enum PhysicsChannels  { SINGLETOP_CH, TTBAR_CH, W_CH, WW_CH, Z_CH, ZZ_CH, WZ_CH, SIGNAL_CH };
enum PhysicsObjects   { MET=0,JET=1,TOP=6,ELECTRON=11, MUON=13, TAU=15, GLUON=21, PHOTON=22, Z=23, W=24};
enum DileptonChannels { UNKNOWN=0,MUMU=1,EE=2,EMU=3,ETAU=4,MUTAU=5, GAMMA=22};
enum IsolType         { ECAL_ISO=0, HCAL_ISO, TRACKER_ISO, REL_ISO, RELRHOCORR_ISO, N_ISO, C_ISO, CPU_ISO, G_ISO, PFREL_ISO, PFRELBETCORR_ISO};
enum ElectronIdArbitrations { EID_VBTF2011=0, EID_VETO, EID_LOOSE, EID_MEDIUM, EID_TIGHT, EID_CONVERSIONVETO, EID_EOPCUT, EID_TRIGGER2011, EID_TIGHTTRIGGER, EID_ECALDRIVEN, EID_TRACKERDRIVEN };
enum MuonIdArbitrations     { MID_GLOBALMUONPROMPTTIGHT, MID_TMLASTSTATIONLOOSE, MID_TMLASTSTATIONTIGHT, MID_TMLASTSTATIONANGTIGHT, MID_TMONESTATIONTIGHT, MID_TRACKER, MID_GLOBAL, MID_PF, MID_LOOSE, MID_SOFT, MID_TIGHT, MID_HIGHPT, MID_VBTF2011, MID_SOFT2011 };
enum JetIdArbitrations      { JETID_LOOSE=0,           JETID_TIGHT=1, 
			      JETID_MIN_TIGHT=4,       JETID_MIN_MEDIUM=5,       JETID_MIN_LOOSE=6,
			      JETID_OPT_TIGHT=8,       JETID_OPT_MEDIUM=9,       JETID_OPT_LOOSE=10,
			      JETID_CUTBASED_TIGHT=12, JETID_CUTBASED_MEDIUM=13, JETID_CUTBASED_LOOSE=14};

class ObjectIdSummary
{
 public:
  ObjectIdSummary();
  ObjectIdSummary(ObjectIdSummary const &);
  ~ObjectIdSummary() {}

  //generic
  LorentzVector p4,genP4;
  int id,genid,genflav;
  double charge;
  int idBits;
  double isoVals[15], mva[5];
  double ensf,ensferr;
  double trkd0,trkdZ,trkip3d,trkip3dsig,trkpt,trketa,trkphi,trkchi2,trkValidPixelHits,trkValidTrackerHits,trkLostInnerHits;
  //muon specific
  double trkValidMuonHits,trkMatches,trkMatchedStations,innerTrackChi2,trkLayersWithMeasurement,pixelLayersWithMeasurement;
  //electron specific
  double dPhiTrack,dEtaTrack,ooemoop,fbrem,eopin,emvatrigv0,emvanontrigv0;
  double dEtaCalo, dPhiCalo, kfchi2, kfhits, kfhitsall, sihip, nbrems, etawidth, phiwidth, e1x5e5x5, preShowerOverRaw, eopout;
  //common to photon and electron
  double hoe,hoebc,sihih,sipip,sce,sceta,scphi,e2x5max,e1x5,e5x5,h2te,h2tebc,r9;
  //common to electron, photon, muon and jet
  double aeff;
  //jet specific
  double neutHadFrac,neutEmFrac,chHadFrac,tche, tchp, csv, jp,beta,betaStar,dRMean,ptD,ptRMS,lxy,slxy,svmass,svpt,svdr,ssvhe,ssvhp;
  std::vector<double> customTaggers;
};


///                            ///   
/// VERTEX SELECTION UTILITIES ///                                                                                                                                                                                                                                                                 
///                            /// 
std::vector<reco::VertexRef> getGoodVertices(edm::Handle<reco::VertexCollection> &hVtx, const edm::ParameterSet &iConfig);
float getVertexMomentumFlux(const reco::Vertex *vtx, float minWeight=0.5);

template<class T>
std::pair<bool,Measurement1D> getImpactParameter(const T &trkRef, reco::VertexRef vtx, const edm::EventSetup &iSetup, bool is3d=true)
{
  edm::ESHandle<TransientTrackBuilder> trackBuilder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);
  reco::TransientTrack tt = trackBuilder->build(trkRef.get());
  if(is3d) return IPTools::absoluteImpactParameter3D(tt, *(vtx.get()));
  else     return IPTools::absoluteTransverseImpactParameter(tt, *(vtx.get()));
}
  
template<class T>
reco::VertexRef getClosestVertexTo(const T *trk, std::vector<reco::VertexRef> &selVertices)
{
  reco::VertexRef bestvtx;
  double bestDz(1.0e7);
  for(std::vector<reco::VertexRef>::iterator vIt = selVertices.begin(); vIt != selVertices.end(); vIt++)
    {
      double dz = fabs( trk->dz( vIt->get()->position() ) );
      if( dz>bestDz ) continue;
      bestvtx=*vIt;
      bestDz=dz;
    }
  return bestvtx;
}


///                            ///   
/// LEPTON SELECTION UTILITIES ///
///                            ///   
std::vector<reco::CandidatePtr> getGoodMuons(edm::Handle<edm::View<reco::Candidate> > &hMu,
					     const reco::VertexRef &primVertex, 
					     const double& rho, 
					     const edm::ParameterSet &iConfig,
					     const edm::EventSetup & iSetup,
					     std::vector<ObjectIdSummary> &selMuonIds);
std::vector<reco::CandidatePtr> getGoodElectrons(edm::Handle<edm::View<reco::Candidate> > &hEle, 
						 edm::Handle<edm::View<reco::Candidate> > &hMu, 
						 edm::Handle<reco::VertexCollection> &hVtx,
						 const reco::BeamSpot &beamspot,
						 edm::Handle<reco::ConversionCollection> &hConversions,
						 EGEnergyCorrector *ecorr,
						 EcalClusterLazyTools &lazyTool,
						 PFIsolationEstimator *eIsolator,
						 edm::Handle<reco::PFCandidateCollection> &hPFcands,
						 const double& rho, 
						 const edm::ParameterSet &iConfig,
						 const edm::EventSetup & iSetup,
						 std::vector<ObjectIdSummary> &selElectronIds);
std::vector<int> getDileptonCandidate(std::vector<reco::CandidatePtr> &selLeptons,  const edm::ParameterSet &iConfig,  const edm::EventSetup &iSetup);
int getLeptonId(reco::CandidatePtr &lepton);
int getDileptonId(reco::CandidatePtr &l1,reco::CandidatePtr &l2);
double getLeptonPtError(reco::CandidatePtr &lepton);
std::vector<double> getLeptonIso(reco::CandidatePtr &lepton, float minRelNorm=20, float rho=0);
const reco::GenParticle *getLeptonGenMatch(reco::CandidatePtr &lepton);


///                            ///   
/// PHOTON SELECTION UTILITIES ///
///                            ///   
// cf. https://twiki.cern.ch/twiki/bin/viewauth/CMS/Vgamma2011PhotonID
std::vector<reco::CandidatePtr> getGoodPhotons(edm::Handle<edm::View<reco::Candidate> > &hPhoton,
					       EGEnergyCorrector *ecorr,
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
					       std::vector<ObjectIdSummary> &selPhotonIds);

///                            ///   
/// JET SELECTION UTILITIES    ///
///                            ///   
std::vector<reco::CandidatePtr> getGoodJets(edm::Handle<edm::View<reco::Candidate> > &hJet, 
					    std::vector<reco::CandidatePtr> &selPhysicsObjects, 
					    edm::Handle<reco::VertexCollection> &hVtx,
					    std::vector<PileupJetIdAlgo *> &puJetIdAlgo,
					    std::vector<edm::Handle<reco::JetTagCollection> > &jetTagsH,
					    const edm::ParameterSet &iConfig,
					    std::vector<ObjectIdSummary> &selJetsId);
std::pair<double,double> computeBetaForJet(const pat::Jet *jet, edm::Handle<reco::VertexCollection> &hVtx);
int getJetTag(const pat::Jet *jet, edm::Handle<reco::JetTagCollection > jetTags);


///                           ///
/// GENERATOR LEVEL UTILITIES ///
///                           ///
const reco::Candidate *getGeneratorFinalStateFor(const reco::Candidate *p);
std::pair<int,std::vector<const reco::Candidate *> > assignPhysicsChannel(edm::Handle<edm::View<reco::Candidate> > &genParticles,const edm::ParameterSet &iConfig);
std::vector<reco::CandidatePtr> filterHFfromGSplit(edm::Handle<edm::View<reco::Candidate> > &genParticles);
const reco::Candidate *getHFmatchFromGSplit(reco::CandidatePtr &jet, std::vector<reco::CandidatePtr> &hfFromGsplit,int flavId);

//                    //
// TRIGGER UTILITILES //
//                    //
std::pair<std::string,double> getHighestPhotonTrigThreshold(edm::Handle<edm::TriggerResults> &triggerBitsH, 
							    const edm::TriggerNames &triggerNames, 
							    std::vector<std::string> &gammaTriggers,
							    unsigned int &gammaTriggerWord);
bool checkIfTriggerFired(edm::Handle<edm::TriggerResults> &allTriggerBis, const edm::TriggerNames &triggerNames, std::vector<std::string> &trigList); 


#endif
