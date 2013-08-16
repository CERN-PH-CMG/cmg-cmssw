// system include files
#include <memory>
#include <cmath>
#include <algorithm>

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/METReco/interface/CommonMETData.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"

#include "RecoMET/METAlgorithms/interface/PFSpecificAlgo.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include <DataFormats/MuonReco/interface/Muon.h>

#include "FWCore/Utilities/interface/Exception.h"

#include "CMG/MetAnalysis/plugins/PUCorrMetProducer.h"
#include "CMG/MetAnalysis/interface/MetUtilities.h"


using namespace edm;
using namespace std;
using namespace reco;

PUCorrMetProducer::PUCorrMetProducer(const edm::ParameterSet& iConfig) {
  produces<reco::PFMETCollection>();
  utils_          = new MetUtilities(iConfig.getParameter<edm::ParameterSet>("puJetIDAlgo"));      
  dZCut_          = iConfig.getParameter<double>("dZCut");
  jetPtThreshold_ = iConfig.getParameter<double>("jetPtThreshold");
}
// MetUtilities utils(iConfig);
//  TString path(getenv("CMSSW_BASE"));
//  path += "/src/CMG/MetAnalysis/data/";  
//  std::vector<JetCorrectorParameters> correctionParameters;
//  correctionParameters.push_back(JetCorrectorParameters((path+"GR_R_42_V23_AK5PF_L1FastJet.txt").Data()));
//  correctionParameters.push_back(JetCorrectorParameters((path+"GR_R_42_V23_AK5PF_L2Relative.txt").Data()));
//  correctionParameters.push_back(JetCorrectorParameters((path+"GR_R_42_V23_AK5PF_L3Absolute.txt").Data()));
//  if(fData) correctionParameters.push_back(JetCorrectorParameters((path+"GR_R_42_V23_AK5PF_L2L3Residual.txt").Data()));
//  fJetCorrector = new FactorizedJetCorrector(correctionParameters);     
//}

PUCorrMetProducer::~PUCorrMetProducer() { 
  delete utils_;
}

void PUCorrMetProducer::beginJob() { }

void PUCorrMetProducer::endJob() { } 

void PUCorrMetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

   // PF candidates
  edm::Handle< edm::View<reco::Candidate> > PFcandCollHandle;
  try { iEvent.getByLabel("particleFlow", PFcandCollHandle); }
  catch ( cms::Exception& ex ) { edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection: particleFlow"; }
  const edm::View<reco::Candidate> *PFcandColl = PFcandCollHandle.product();

  // uncorrected PF jets
  edm::Handle< edm::View<reco::Candidate> > uncorPFJetCollectionHandle;
  try { iEvent.getByLabel("ak5PFJets",uncorPFJetCollectionHandle); }
  catch ( cms::Exception& ex ) { edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection: ak5PFJets"; }
  const edm::View<reco::Candidate> *uncorPFJetColl = uncorPFJetCollectionHandle.product();

  // fully corrected PF jets
  edm::Handle< edm::View<reco::Candidate> > corrPFJetCollectionHandle;
  if(isData_) { 
    try { iEvent.getByLabel("ak5PFJetsL1FastL2L3Residual",corrPFJetCollectionHandle); } 
    catch ( cms::Exception& ex ) { edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection for: ak5PFJetsL1FastL2L3Residual"; }
  } else { 
    try { iEvent.getByLabel("ak5PFJetsL1FastL2L3",corrPFJetCollectionHandle); } 
    catch ( cms::Exception& ex ) { edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection for: ak5PFJetsL1FastL2L3"; }
  }
  const edm::View<reco::Candidate> *corrPFJetColl = corrPFJetCollectionHandle.product();
  
  // vertices                                                                                                             
  edm::Handle<reco::VertexCollection> primaryVertex;
  try { iEvent.getByLabel("offlinePrimaryVertices", primaryVertex); }
  catch(cms::Exception& ex ) {edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection: offlinePrimaryVertices"; }
  
  //Rho
  edm::Handle<double> hRho;
  try{ iEvent.getByLabel("kt6PFJetsForRhoComputationVoronoi",hRho); }
  catch(cms::Exception& ex ) {edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection: rho"; }

  //Fisrt the PV
  VertexCollection::const_iterator vMax = primaryVertex->begin();
  reco::Vertex pv;
  if (primaryVertex->size()>0) pv = *vMax;

  //Now the Met basics
  Candidate::LorentzVector totalP4(0,0,0,0);
  double sumet = 0.0;
  
  //Track MET
  for(int index = 0; index < (int)PFcandColl->size(); index++) {
    const PFCandidate* pflowCand = dynamic_cast< const PFCandidate * >( &(PFcandColl->at(index)));
    if(primaryVertex->size()==0) continue;
    double pDZ  = utils_->pfCandDz(pflowCand,&pv);
    if(pDZ > dZCut_) continue;
    totalP4 -= pflowCand->p4();
    sumet   += pflowCand->pt();
  }
  //Neutral PF Candidates
  for(int index = 0; index < (int)PFcandColl->size(); index++) {
    const PFCandidateRef pflowCandRef = PFcandColl->refAt(index).castTo<PFCandidateRef>();
    if(pflowCandRef->trackRef().isNonnull() || pflowCandRef->gsfTrackRef().isNonnull() || (pflowCandRef->muonRef().isNonnull() && pflowCandRef->muonRef()->innerTrack().isNonnull())) continue;
    totalP4 -= pflowCandRef->p4();
    sumet   += pflowCandRef->pt();
  }
  //Remove Neutrals from the PU Jets
  for(int index = 0; index < (int)uncorPFJetColl->size(); index++) {
    const Candidate *uncorrCand   = &(uncorPFJetColl->at(index));
    const PFJet     *pUncorrPFJet = dynamic_cast< const PFJet * > ( &(*uncorrCand) );    
    for(int index2 = 0; index2 < (int)corrPFJetColl->size(); index2++) {   // corrected jets collection
      const Candidate *corrCand   = &(corrPFJetColl->at(index2));
      const PFJet     *pCorrPFJet = dynamic_cast< const PFJet * > ( &(*corrCand) );    
      if(  pUncorrPFJet->jetArea() == pCorrPFJet->jetArea() ) {      // to match corrected and uncorrected jets
	if(  fabs(pUncorrPFJet->eta() - pCorrPFJet->eta())<0.01 ) {  // to match corrected and uncorrected jets
	  if( pCorrPFJet->pt()< jetPtThreshold_ ) continue;  
	  //double lJec = pCorrPFJet->pt()/pUncorrPFJet->pt();
	  if(utils_->passJetId(pCorrPFJet,pUncorrPFJet,pv,*primaryVertex)) continue;
	  utils_->addNeut(pCorrPFJet,&totalP4,&sumet,-1);             
	  break;
	}
      }
    }
  }
  
  CommonMETData output;
  output.mex = totalP4.px();
  output.mey = totalP4.py();
  output.mez = totalP4.pz();
  output.met = totalP4.pt();
  output.sumet = sumet;
  output.phi = atan2(totalP4.py(),totalP4.px());
  
  PFSpecificAlgo pf;
  std::auto_ptr<reco::PFMETCollection> pfmetcoll;
  pfmetcoll.reset (new reco::PFMETCollection);
  pfmetcoll->push_back( pf.addInfo(PFcandCollHandle, output) );

  // and finally put it in the event
  iEvent.put( pfmetcoll );
}
