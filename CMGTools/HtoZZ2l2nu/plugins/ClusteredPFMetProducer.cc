// system include files
#include <memory>
#include <cmath>
#include <algorithm>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "RecoMET/METAlgorithms/interface/PFSpecificAlgo.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "RecoParticleFlow/PostProcessing/src/FastJetAlgo.cc"

#include "Math/LorentzVector.h"

#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraphAsymmErrors.h"

#include "CMGTools/HtoZZ2l2nu/interface/TSelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/ClusteredPFMETSummary.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

class ClusteredPFMetProducer : public edm::EDProducer 
{
public:
  explicit ClusteredPFMetProducer(const edm::ParameterSet&);
  ~ClusteredPFMetProducer();
  
private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  int computeVertexAssociationFor(const reco::PFCandidateRef &candptr);
  std::vector<bool> getPFCandidatesMaskForVertex(int iVtx);

  edm::Handle<reco::VertexCollection> vtxH_;  
  edm::Handle<reco::PFCandidateCollection> pfCandsH_;
  edm::InputTag collectionTag_, vertexTag_;    
  double minNeutralPt_, maxNeutralEta_;
  double minJetPt_, maxJetEta_;
  std::vector<int> vertexAssociationMasks_;

  FastJetAlgoPFCandPFJet jetProducer_;

  double jetCone_;

  bool minBiasMode_;
  bool produceSummary_;

  TTree *t_;
  TSelectionMonitor controlHistos_;
  MetSummaryHandler *evHandler_;
};

using namespace std;

//
ClusteredPFMetProducer::ClusteredPFMetProducer(const edm::ParameterSet& iConfig):
  collectionTag_(iConfig.getParameter<edm::InputTag>("collectionTag")),
  vertexTag_(iConfig.getParameter<edm::InputTag>("vertexTag")),
  minNeutralPt_(iConfig.getParameter<double>("minNeutralPt")),
  maxNeutralEta_(iConfig.getParameter<double>("maxNeutralEta")),
  minJetPt_(iConfig.getParameter<double>("minJetPt")),
  maxJetEta_(iConfig.getParameter<double>("maxJetEta")),
  jetProducer_(iConfig.getParameter<edm::ParameterSet>("fastjet")),
  jetCone_(iConfig.getParameter<edm::ParameterSet>("fastjet").getParameter<double>("distance_par")),
  minBiasMode_(iConfig.getParameter<bool>("minBiasMode")),
  produceSummary_(iConfig.getParameter<bool>("produceSummary")),
  controlHistos_("control")
{
  produces<reco::PFMET>("assocPfMet");
  produces<reco::PFMET>("assocPfMetWithFwd");
  produces<reco::PFMET>("assocOtherVtxPfMet");
  produces<reco::PFMET>("trkPfMet");
  produces<reco::PFMET>("globalPfMet");
  produces<reco::PFMET>("centralPfMet");
  produces<reco::PFMET>("cleanPfMet");
  produces<std::vector<double> >("globalPfMetSums"); 
  
  edm::Service<TFileService> fs;
  if(produceSummary_)
    evHandler_ = new MetSummaryHandler( fs->make<TTree>("data","MET Summary"), true);
  
  //vertex histograms
  controlHistos_.addHistogram("vtx_multiplicity",";Vertices;Events",25,0,25);
  TString vtxTypes[]={"vtxprim","vtxother"};
  TString vtxLabels[]={"PV", "other Vtx"};
  for(size_t ivtx=0; ivtx<2; ivtx++)
    {
      TString label=vtxTypes[ivtx];
      controlHistos_.addHistogram(label+"_ntracks",vtxLabels[ivtx]+";Tracks;Vertices",200,0,200);
      controlHistos_.addHistogram(label+"_chi2",vtxLabels[ivtx]+";#chi^{2}/ndof;Vertices",100,0,2);
      controlHistos_.addHistogram(label+"_pt",vtxLabels[ivtx]+";p_{T} [GeV/c];Vertices",100,0,100);

      controlHistos_.addHistogram(label+"_chargedMet",vtxLabels[ivtx]+" ;ch-E_{T}^{miss} [GeV];Vertices",100,0,100);
      controlHistos_.addHistogram(label+"_neutralMet",vtxLabels[ivtx]+";neut-E_{T}^{miss} [GeV];Vertices",100,0,100);
      controlHistos_.addHistogram(label+"_met",vtxLabels[ivtx]+";E_{T}^{miss} [GeV];Vertices",100,0,100);
      controlHistos_.addHistogram(label+"_chargedSumEt",vtxLabels[ivtx]+";#splitline{#Sigma}{charged} p_{T} [GeV/c];Vertices",100,0,200);
      controlHistos_.addHistogram(label+"_neutralSumEt",vtxLabels[ivtx]+";#splitline{#Sigma}{neutral} p_{T} [GeV/c];Vertices",100,0,200);
      controlHistos_.addHistogram(label+"_SumEt",vtxLabels[ivtx]+";#splitline{#Sigma}{charged,neutral} p_{T} [GeV/c];Vertices",100,0,200);
    }
  
  //particle flow histograms
  controlHistos_.addHistogram("pf_multiplicity","All;PF candidates;Events",100,0,500);
  controlHistos_.addHistogram("pf_neutrals","neutrals;PF candidates;Events",100,0,500);
  controlHistos_.addHistogram("pffwd_neutrals","neutrals |#eta|>2.4;PF candidates;Events",100,0,500);
  controlHistos_.addHistogram("pf2v4_neutrals","neutrals |#eta|<2.4;PF candidates;Events",100,0,500);
  controlHistos_.addHistogram("pf2v4assoc_neutrals","assoc-neutrals;PF candidates;Events",100,0,500);
  controlHistos_.addHistogram("pf2v4notassoc_neutrals","non assoc-neutrals |#eta|<2.4;PF candidates;Events",100,0,500);
  controlHistos_.addHistogram("pf_charged","charged;PF candidates;Events",100,0,500);      
  controlHistos_.addHistogram("pfassoc_charged","assoc-charged;PF candidates;Events",100,0,500);      
  controlHistos_.addHistogram("pfnotassoc_charged","non assoc-charged;PF candidates;Events",100,0,500);      
  controlHistos_.addHistogram("pf_ptneutrals","neutrals;p_{T} [GeV/c];Events",100,0,100);      
  controlHistos_.addHistogram("pffwd_ptneutrals","neutrals |#eta|>2.4;p_{T} [GeV/c];Events",100,0,100);      
  controlHistos_.addHistogram("pf2v4_ptneutrals","neutrals |#eta|<2.4;p_{T} [GeV/c];Events",100,0,100);      
  controlHistos_.addHistogram("pf2v4assoc_ptneutrals","assoc-neutrals;p_{T} [GeV/c];Events",100,0,100);      
  controlHistos_.addHistogram("pf2v4notassoc_ptneutrals","non assoc-neutrals;p_{T} [GeV/c];Events",100,0,100);      
  controlHistos_.addHistogram("pf_ptcharged","charged;p_{T} [GeV/c];Events",100,0,100);      
  controlHistos_.addHistogram("pfassoc_ptcharged","assoc-charged;p_{T} [GeV/c];Events",100,0,100);      
  controlHistos_.addHistogram("pfnotassoc_ptcharged","non assoc-charged;p_{T} [GeV/c];Events",100,0,100);      
  controlHistos_.addHistogram("pf_dzcharged","charged;#Delta Z [cm];PF candidates;",100,-1,1);
  controlHistos_.addHistogram("pf_d0charged","charged;d_{0} [cm];PF candidates;",100,0,1);
  controlHistos_.addHistogram("pf_weightcharged","charged;Weight;PF candidates;",100,0,1);
  controlHistos_.addHistogram("pf_etavsphicharged","charged;#eta;#phi;",100,-5,5,100,-3.4,3.4);
  controlHistos_.addHistogram("pf_etavsptcharged","charged;#eta;p_{T} [GeV/c];",100,-5,5,100,0,50);
  controlHistos_.addHistogram("pf_etavsphineutrals","neutral;#eta;#phi;",100,-5,5,100,-3.4,3.4);
  controlHistos_.addHistogram("pf_etavsptneutrals","neutral;#eta;p_{T} [GeV/c];",100,-5,5,100,0,50);
  controlHistos_.addHistogram("pf_chi2charged","charged;#chi^{2}/ndof;PF candidates;",100,0,10);
  controlHistos_.addHistogram("pf_nhitscharged","charged;N_{hits};PF candidates;",40,0,40);
  controlHistos_.addHistogram("pf_npixelhitscharged","charged;N_{pixel hits};PF candidates;",10,0,10);
  controlHistos_.addHistogram("pf_dzvsptcharged","charged;#Delta Z [cm]; p_{T};PF candidates;",100,-1,1,100,0,25);
  controlHistos_.addHistogram("pf_dzvsetacharged","charged;#Delta Z [cm]; #eta;PF candidates;",100,-1,1,100,-2.5,2.5);
  controlHistos_.addHistogram("pf_dzvsd0charged","charged;#Delta Z [cm]; d_{0} [cm];PF candidates;",100,-1,1,100,0,1);
  controlHistos_.addHistogram("pf_dzvsweightcharged","charged;#Delta Z [cm]; Weight;PF candidates;",100,-1,1,100,0,1);
  controlHistos_.addHistogram("pf_dzvschi2charged","charged;#Delta Z [cm];#chi^{2}/ndof ;PF candidates;",100,-1,1,100,0,10);
  controlHistos_.addHistogram("pf_dzvsnhitscharged","charged;#Delta Z [cm];N_{hits} ;PF candidates;",100,-1,1,40,0,40);
  controlHistos_.addHistogram("pf_dzvsnpixelhitscharged","charged;#Delta Z [cm]; N_{pixel hits};PF candidates;",100,-1,1,10,0,10);
      
  //jets
  for(size_t ivtx=0; ivtx<2; ivtx++)
    {
      controlHistos_.addHistogram(vtxTypes[ivtx]+"_jetmultiplicity",vtxLabels[ivtx]+";Jet multiplicity;Events",25,0,25);
      controlHistos_.addHistogram(vtxTypes[ivtx]+"_jetpt",vtxLabels[ivtx]+";p_{T} [GeV/c];Jets",200,0,200);
      controlHistos_.addHistogram(vtxTypes[ivtx]+"_jeteta",vtxLabels[ivtx]+";#eta;Jets",100,-2.5,2.5);
      controlHistos_.addHistogram(vtxTypes[ivtx]+"_jetchmultiplicity",vtxLabels[ivtx]+";Charged multiplicity;Jets",25,0,25);
      controlHistos_.addHistogram(vtxTypes[ivtx]+"_jetneutmultiplicity",vtxLabels[ivtx]+";Neutral multiplicity;Jets",25,0,25);
      controlHistos_.addHistogram(vtxTypes[ivtx]+"_jetetaetamoment",vtxLabels[ivtx]+";#sigma(#eta,#eta);Jets",100,0,0.1);
      controlHistos_.addHistogram(vtxTypes[ivtx]+"_jetetaphimoment",vtxLabels[ivtx]+";#sigma(#eta,#phi);Jets",100,0,0.1);
      controlHistos_.addHistogram(vtxTypes[ivtx]+"_jetphiphimoment",vtxLabels[ivtx]+";#sigma(#phi,#phi);Jets",100,0,0.1);
      controlHistos_.addHistogram(vtxTypes[ivtx]+"_jetradius",vtxLabels[ivtx]+";Jet radius=#sqrt{#sigma(#phi,#phi)^{2}+#Sigma(#eta,#eta)^{2}};Jets",100,0,0.1);
    }
      
  //MET
  controlHistos_.addHistogram("evt_chargedmet",            "charged;ch-E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt_neutralmet",            "neutral;neut-E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evtfwd_neutralmet",         "neutral |#eta|>2.4;neut-E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt2v4_neutralmet",         "neutral |#eta|<2.4;neut-E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt2v4assoc_neutralmet",    "assoc-neutral;neut-E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt2v4notassoc_neutralmet", "non assoc-neutral;neut-E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt_met",                   "all;E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt2v4_met",                "all |#eta|<2.4;E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evtfwd_met",                "all |#eta|>2.4;E_{T}^{miss} [GeV];Events",100,0,200);

  //Sum Et
  controlHistos_.addHistogram("evt_chargedSumEt",          "charged;#splitline{#Sigma}{charged} p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evt_neutralSumEt",          "neutral;#splitline{#Sigma}{neutral} p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evtfwd_neutralSumEt",       "neutral |#eta|>2.4;#splitline{#Sigma}{neutral} p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evt2v4_neutralSumEt",       "neutral |#eta|<2.4;#splitline{#Sigma}{neutral} p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evt2v4assoc_neutralSumEt",  "assoc-neutral;#splitline{#Sigma}{neutral} p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evt2v4notassoc_neutrlSumEt","non assoc-neutral;#splitline{#Sigma}{neutral} p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evt_fracchargedSumEt",      "charged;#splitline{#Sigma}{charged} p_{T} frac;Events",100,0,1);
  controlHistos_.addHistogram("evt_fracneutralSumEt",            "neutral;#splitline{#Sigma}{neutral} p_{T} frac;Events",100,0,1);
  controlHistos_.addHistogram("evtfwd_fracneutralSumEt",         "neutral |#eta|>2.4;#splitline{#Sigma}{neutral} p_{T} frac;Events",100,0,1);
  controlHistos_.addHistogram("evt2v4_fracneutralSumEt",         "neutral |#eta|<2.4;#splitline{#Sigma}{neutral} p_{T} frac;Events",100,0,1);
  controlHistos_.addHistogram("evt2v4assoc_fracneutralSumEt",    "assoc-neutral;#splitline{#Sigma}{neutral} p_{T} frac;Events",100,0,1);
  controlHistos_.addHistogram("evt2v4notassoc_fracneutralSumEt", "non assoc-neutral;#splitline{#Sigma}{neutral} p_{T} frac;Events",100,0,1);
  controlHistos_.addHistogram("evt_SumEt",      "all;#Sigma p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evtfwd_SumEt",   "all |#eta|>2.4;#Sigma p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evt2v4_SumEt",   "all |#eta|>2.4;#Sigma p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evt_metVsSumEt", "all;#Sigma p_{T} [GeV/c];E_{T}^{miss} [GeV];Events",100,0,200,100,0,1000);

  controlHistos_.addHistogram("evt_metVsNvtx", "all;Vertices;E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt2v4_metVsNvtx", "all |#eta|<2.4;Vertices;E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evtfwd_metVsNvtx", "all |#eta|>2.4;Vertices;E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt_chargedmetVsNvtx", "charged |#eta|>2.4;Vertices;ch-E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt2v4assoc_neutralmetVsNvtx", "assoc-neutral;Vertices;neut-E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt2v4notassoc_neutralmetVsNvtx", "non assoc-neutral;Vertices;neut-E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt_chargedmetRes", "charged;Vertices;ch-E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt_neutralmetRes", "neutral;Vertices;neut-E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt_metRes", "all;Vertices;E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt2v4_metRes", "all |#eta|<2.4;Vertices;E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt_metMin", "all;Vertices;min-E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt_chargedMetMin", "charged;Vertices;min-E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt2v4_metMin", "all;Vertices;min-E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt2v4_chargedMetMin", "charged;Vertices;min-E_{T}^{miss} [GeV];Events",25,0,25,100,0,1000);
  controlHistos_.addHistogram("evt_metMinAllvs2v4", "all;min-E_{T}^{miss} [GeV];min-E_{T}^{miss} |#eta|<2.4[GeV]Events",100,0,1000,100,0,1000);
  controlHistos_.addHistogram("evt_chargedmetMinAllvs2v4", "charged;min-E_{T}^{miss} [GeV];min-E_{T}^{miss} |#eta|<2.4[GeV]Events",100,0,1000,100,0,1000);
  controlHistos_.addHistogram("evt_metMin2v4vsFwd", "all;min-E_{T}^{miss} |#eta|<2.4 [GeV];min-E_{T}^{miss} |#eta|>2.4[GeV]Events",100,0,1000,100,0,1000);
  controlHistos_.addHistogram("evt_chargedmetMin2v4vsFwd", "charged;min-E_{T}^{miss} |#eta|<2.4 [GeV];min-E_{T}^{miss} |#eta|>2.4[GeV]Events",100,0,1000,100,0,1000);

  //residual MET
  controlHistos_.addHistogram("evt_chargedmetRes",    "charged;ch-E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt_neutralmetRes",    "neutral;neut-E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt_metRes",           "all;E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evtfwd_metRes",        "all |#eta|>2.4;E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt2v4_neutralmetRes", "neutral |#eta|<2.4;neut-E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt2v4_metRes",        "all |#eta|<2.4;E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt_metMin",           "all;min-E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt_chargedMetMin",           "all;min-E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt2v4_metMin",        "all |#eta|<2.4;min-E_{T}^{miss} [GeV];Events",100,0,200);
  controlHistos_.addHistogram("evt2v4_chargedMetMin",        "all |#eta|<2.4;min-E_{T}^{miss} [GeV];Events",100,0,200);

  //residual Sum ET
  controlHistos_.addHistogram("evt_chargedSumEtRes",  "charged;#splitline{#Sigma}{charged} p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evt_neutralSumEtRes",  "neutral;#splitline{#Sigma}{neutral} p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evt_SumEtRes",         "all;#splitline{#Sigma} p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evt2v4_neutralSumEtRes","neutral |#eta|<2.4;#splitline{#Sigma}{neutral} p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evtfwd_SumEtRes",         "neutral |#eta|>2.4;#splitline{#Sigma} p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evt2v4_SumEtRes",         "neutral |#eta|<2.4;#splitline{#Sigma} p_{T} [GeV/c];Events",100,0,1000);
  controlHistos_.addHistogram("evt_metVsSumEtRes",       "all;#Sigma p_{T} [GeV/c];E_{T}^{miss} [GeV];Events",100,0,200,100,0,1000);
  
  //replicate for different categories
  TString cats[]={"eq0jets","geq1jets"};
  TString subcats[]={"","lowfwdactivity"};
  for(int i=0; i<2; i++)
    for(int j=0; j<2; j++)
      controlHistos_.initMonitorForStep(cats[i]+subcats[j]);
}

//
int ClusteredPFMetProducer::computeVertexAssociationFor(const reco::PFCandidateRef &candptr)
{
  int assocVtxRef(-1);

  reco::TrackBaseRef trackBaseRef( candptr->trackRef() );

  //no association for neutrals
  if(trackBaseRef.isNull()) return assocVtxRef;

  //check the vertices which use the track in the fit
  int nVerticesAss(0);
  float bestweight(0),bestDz(9999.);
  const size_t nVtx(vtxH_->size());
  for(size_t jVtx=0; jVtx<nVtx; jVtx++) 
    {
      const reco::VertexRef vtxref(vtxH_,jVtx);
      float vtxWeight(0);
      try{
	vtxWeight= vtxref->trackWeight(trackBaseRef);
      }catch(std::exception &e){
	//if not available then track was not used in vertex fit
      }
      float vtxDz( fabs( trackBaseRef->dz( vtxref->position()) ) );
      
      if(vtxWeight > bestweight || ( vtxWeight == bestweight && vtxDz < bestDz))
	{
	  bestweight=vtxWeight;
	  bestDz=vtxDz;
	  assocVtxRef=jVtx;
	  nVerticesAss++;
	}
    }
 
//   if (nVerticesAss>1) edm::LogWarning("AmbiguousAssociation") << " charged hadron track is shared by at least two vertices, assigning by highest weight: " 
// 							      << bestweight << " (bestDZ=" << bestDz << ")";
  return assocVtxRef;
}

//
std::vector<bool> ClusteredPFMetProducer::getPFCandidatesMaskForVertex(int iVtx)
{
  size_t nPFCands(pfCandsH_->size());
  std::vector<bool> pfCandMasks(nPFCands,true);
  for(size_t iPFcand=0; iPFcand<nPFCands; iPFcand++)
    pfCandMasks[iPFcand]=(vertexAssociationMasks_[iPFcand] == iVtx);
  
  return pfCandMasks;
}

//
void ClusteredPFMetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  //vertices
  iEvent.getByLabel(vertexTag_ ,vtxH_);
  const size_t nVtx(vtxH_->size());
  if( nVtx == 0 ) throw cms::Exception("CorruptData") << "No vertex was found in the event\n";
  
  //particle flow candidates
  iEvent.getByLabel(collectionTag_, pfCandsH_); 
  const size_t nPFCands( pfCandsH_->size() );
  if( nPFCands == 0 ) throw cms::Exception("CorruptData") << "No particle flow candidates collection was found in the event\n";

  //loop over the pf candidates and:
  // - compute sums 
  // - associate pf candidates to vertices
  vertexAssociationMasks_.clear();
  vertexAssociationMasks_.resize(nPFCands,-1);
  
  for (size_t iPFCand=0; iPFCand<nPFCands; iPFCand++)
    {
      reco::PFCandidateRef candRef(pfCandsH_,iPFCand);
      int assocVertexRef = computeVertexAssociationFor(candRef);
      if(assocVertexRef<0) continue;
      vertexAssociationMasks_[iPFCand] = assocVertexRef;
    }
 


  //now loop over the vertices and:
  // - build jets from associated candidates
  // - compute charged sums, associated neutral sums
  std::map<int, std::vector<reco::PFJet> > vtxJets;
  LorentzVectorCollection vtxChSum(nVtx,LorentzVector(0,0,0,0)), vtxNeutralSum(nVtx,LorentzVector(0,0,0,0)),  vtxPFMet(nVtx,LorentzVector(0,0,0,0));
  std::vector<double>     vtxChSumEt(nVtx,0),                    vtxNeutralSumEt(nVtx,0),                     vtxSumEt(nVtx,0);
  for(size_t iVtx=0; iVtx<nVtx; iVtx++)
    {
      const std::vector<bool> candMasks = getPFCandidatesMaskForVertex(iVtx);
  
      //Jet building
      vtxJets[iVtx] = jetProducer_.produce(pfCandsH_,candMasks,false,2.,99999);
      const std::vector<reco::PFJet> &jetColl=vtxJets[iVtx];
      std::vector<reco::PFJet>::const_iterator jIt(jetColl.begin()), jItEnd(jetColl.end());
      
      //loop over PF candidates and compute charged + neutral sums
      for (size_t iPFCand=0; iPFCand<nPFCands; iPFCand++)
	{
	  reco::PFCandidateRef candRef(pfCandsH_,iPFCand);
	  if(candRef->charge()==0)
	    {
	      if(candRef->pt()<minNeutralPt_ || fabs(candRef->eta()) > maxNeutralEta_) continue;
	      if(vertexAssociationMasks_[iPFCand]!=-1) continue;

	      //associate neutrals using the jets
	      jIt=jetColl.begin();
	      for(;jIt!=jItEnd; jIt++)
		{
		  if(jIt->pt()<minJetPt_ || fabs(jIt->eta()) > maxJetEta_) continue;
		  double dr = deltaR(jIt->eta(),jIt->phi(), candRef->eta(), candRef->phi());
		  if(dr>jetCone_) continue;

		  //association is done for the first jet only
		  vertexAssociationMasks_[iPFCand]  = iVtx;
		  vtxNeutralSum[iVtx]              += candRef->p4();
		  vtxNeutralSumEt[iVtx]            += candRef->pt();
		  break;
		}
	    }
	  else if(vertexAssociationMasks_[iPFCand]==int(iVtx))
	    {
	      vtxChSum[iVtx]   += candRef->p4();
	      vtxChSumEt[iVtx] += candRef->pt();
	    }
	}
      
      vtxPFMet[iVtx] = vtxChSum[iVtx]   + vtxNeutralSum[iVtx];
      vtxSumEt[iVtx] = vtxChSumEt[iVtx] + vtxNeutralSumEt[iVtx];
    }  

  //compute the pf-MET components
  // - check how many PF candidates we were able to associate
  int           nch(0),               nneut(0),                  nneutfwd(0),                  nneut2v4(0),                  nneut2v4ass(0),                  nneut2v4notass(0);
  int           nchnotass(0),         nchass(0);
  LorentzVector globalChSum(0,0,0,0), globalNeutralSum(0,0,0,0), globalNeutralFwdSum(0,0,0,0), globalNeutral2v4Sum(0,0,0,0), globalNeutral2v4assSum(0,0,0,0), globalNeutral2v4notassSum(0,0,0,0);
  double        globalChSumEt(0),     globalNeutralSumEt(0),     globalNeutralFwdSumEt(0),     globalNeutral2v4SumEt(0),     globalNeutral2v4assSumEt(0),     globalNeutral2v4notassSumEt(0);
  LorentzVector globalNonAssocNeutralFwdSum(0,0,0,0);
  double        globalNonAssocNeutralFwdSumEt(0);
  for (size_t iPFCand=0; iPFCand<nPFCands; iPFCand++)
    {
      reco::PFCandidateRef candptr(pfCandsH_,iPFCand);
      bool isAssociatedToVertex(vertexAssociationMasks_[iPFCand]>-1);      
      if(candptr->charge()== 0)
	{
	  nneut ++;
	  globalNeutralSum    += candptr->p4();
	  globalNeutralSumEt  += candptr->pt();
	  
	  if( fabs(candptr->eta())<2.4 )
	    {
      	      nneut2v4++;
	      globalNeutral2v4Sum += candptr->p4();
	      globalNeutral2v4SumEt += candptr->pt();
	      
	      if(isAssociatedToVertex)
		{
		  nneut2v4ass++;
		  globalNeutral2v4assSum += candptr->p4();
		  globalNeutral2v4assSumEt += candptr->pt();
		}
	      else
		{
		  nneut2v4notass++;
		  globalNeutral2v4notassSum += candptr->p4();
		  globalNeutral2v4notassSumEt += candptr->pt();
		}
	    }
	  else
	    {
	      nneutfwd++;
	      globalNeutralFwdSum += candptr->p4();
              globalNeutralFwdSumEt += candptr->pt();

	      if(vertexAssociationMasks_[iPFCand]==-1)
		{
		  globalNonAssocNeutralFwdSum += candptr->p4();
		  globalNonAssocNeutralFwdSumEt += candptr->pt();
		}

	    }
	}
      else
	{
	  nch++;
	  globalChSum         += candptr->p4();
	  globalChSumEt       += candptr->pt();
	  if(isAssociatedToVertex) nchass++;
	  else                     nchnotass++; 
	}
    }
  LorentzVector globalPFMet(globalChSum+globalNeutralSum);
  double        globalSumEt(globalNeutralSumEt+globalChSumEt);
  LorentzVector globalFwdPFMet(globalNeutralFwdSum);
  double        globalFwdSumEt(globalNeutralFwdSumEt);
  LorentzVector global2v4PFMet(globalChSum+globalNeutral2v4Sum);
  double        global2v4SumEt(globalNeutral2v4SumEt+globalChSumEt);
  LorentzVector global2v4assPFMet(globalChSum+globalNeutral2v4assSum);
  double        global2v4assSumEt(globalNeutral2v4assSumEt+globalChSumEt);
  
  //compute the residual MET
  // - subtracting all associated to other vertices than the PV
  // - except if mibiasStudy is active where all that is associated to all vertices is removed
  size_t iVtxStart( minBiasMode_ ? 0 : 1 );
  LorentzVector resChSum(globalChSum),     resNeutralSum(globalNeutralSum),     res2v4NeutralSum(globalNeutral2v4Sum);
  double        resChSumEt(globalChSumEt), resNeutralSumEt(globalNeutralSumEt), res2v4NeutralSumEt(globalNeutral2v4SumEt);
  for(size_t iVtx=iVtxStart; iVtx<nVtx; iVtx++)
    {
      resChSum        -= vtxChSum[iVtx];
      resChSumEt      -= vtxChSumEt[iVtx];

      resNeutralSum   -= vtxNeutralSum[iVtx];
      resNeutralSumEt -= vtxNeutralSumEt[iVtx];

      res2v4NeutralSum   -= vtxNeutralSum[iVtx];
      res2v4NeutralSumEt -= vtxNeutralSumEt[iVtx];
    }
  LorentzVector resPFMet(resChSum+resNeutralSum);
  double        resSumEt(resChSumEt+resNeutralSumEt);
  LorentzVector res2v4PFMet(resChSum+res2v4NeutralSum);
  double        res2v4SumEt(resChSumEt+res2v4NeutralSumEt);
  LorentzVector resFwdPFMet(globalNeutralFwdSum);
  double        resFwdSumEt(globalNeutralFwdSumEt);


  
  //
  // control the results according to the category of events
  //
  //if(minBiasMode_ && nVtx>1) return;

  //save event summary
  if(produceSummary_)
    {
      METSummary_t &ev=evHandler_->get();
      ev.run = iEvent.id().run();
      ev.lumi = iEvent.luminosityBlock();
      ev.event = iEvent.id().event();
      
      ev.nvtx=nVtx;
      for(size_t ivtx=0; ivtx<nVtx; ivtx++)
	{
	  const reco::VertexRef vtxref(vtxH_,ivtx);
	  
	  int njets(0);
	  for(std::vector<reco::PFJet>::const_iterator jIt=vtxJets[ivtx].begin();  jIt!=vtxJets[ivtx].end(); jIt++) njets += (jIt->pt()>minJetPt_ && fabs(jIt->eta())<maxJetEta_); 
	  ev.jn[ivtx]=njets;
	  ev.chi2[ivtx]=vtxref->normalizedChi2();
	  ev.sumpt[ivtx]=vtxref->p4().pt();
	  ev.vtxmet_pt[ivtx] = vtxPFMet[ivtx].pt();               ev.vtxmet_phi[ivtx] = vtxPFMet[ivtx].phi();                ev.vtxsumEt[ivtx] = vtxSumEt[ivtx];
	  ev.vtxchmet_pt[ivtx] = vtxChSum[ivtx].pt();             ev.vtxchmet_phi[ivtx] = vtxChSum[ivtx].phi();              ev.vtxchsumEt[ivtx] = vtxChSumEt[ivtx];
	  ev.vtxneutmet_pt[ivtx] = vtxNeutralSum[ivtx].pt();      ev.vtxneutmet_phi[ivtx] = vtxNeutralSum[ivtx].phi();       ev.vtxneutsumEt[ivtx] = vtxNeutralSumEt[ivtx];
	}
      
      //pf MET
      ev.met_pt[METSummary_t::GLOBAL]   = globalPFMet.pt();       
      ev.met_phi[METSummary_t::GLOBAL]  = globalPFMet.phi();     
      ev.sumEt[METSummary_t::GLOBAL]    = globalSumEt;
      ev.met_pt[METSummary_t::FWD]      = globalFwdPFMet.pt();      
      ev.met_phi[METSummary_t::FWD]     = globalFwdPFMet.phi();     
      ev.sumEt[METSummary_t::FWD]       = globalFwdSumEt;
      ev.met_pt[METSummary_t::CENTRAL]  = global2v4PFMet.pt();  
      ev.met_phi[METSummary_t::CENTRAL] = global2v4PFMet.phi();  
      ev.sumEt[METSummary_t::CENTRAL]   = global2v4SumEt;
      
      ev.chmet_pt[METSummary_t::GLOBAL]   = globalChSum.pt();                    
      ev.chmet_phi[METSummary_t::GLOBAL]  = globalChSum.phi();                   
      ev.chsumEt[METSummary_t::GLOBAL]    = globalChSumEt;
      ev.chmet_pt[METSummary_t::FWD]      = 0;                                       
      ev.chmet_phi[METSummary_t::FWD]     = 0;                                        
      ev.chsumEt[METSummary_t::FWD]       = 0;
      ev.chmet_pt[METSummary_t::CENTRAL]  = ev.chmet_pt[METSummary_t::GLOBAL];  
      ev.chmet_phi[METSummary_t::CENTRAL] = ev.chmet_phi[METSummary_t::GLOBAL]; 
      ev.chsumEt[METSummary_t::CENTRAL]   = ev.chsumEt[METSummary_t::GLOBAL];
      
      ev.neutmet_pt[METSummary_t::GLOBAL]   = globalNeutralSum.pt();        
      ev.neutmet_phi[METSummary_t::GLOBAL]  = globalNeutralSum.phi();       
      ev.neutsumEt[METSummary_t::GLOBAL]    = globalNeutralSumEt;
      ev.neutmet_pt[METSummary_t::FWD]      = globalNeutralFwdSum.pt();        
      ev.neutmet_phi[METSummary_t::FWD]     = globalNeutralFwdSum.phi();       
      ev.neutsumEt[METSummary_t::FWD]       = globalNeutralFwdSumEt;
      ev.neutmet_pt[METSummary_t::CENTRAL]  = globalNeutral2v4Sum.pt();    
      ev.neutmet_phi[METSummary_t::CENTRAL] = globalNeutral2v4Sum.phi();    
      ev.neutsumEt[METSummary_t::CENTRAL]   = globalNeutral2v4SumEt;
      
      //residual MET
      ev.resmet_pt[METSummary_t::GLOBAL]     = resPFMet.pt();                  
      ev.resmet_phi[METSummary_t::GLOBAL]    = resPFMet.phi();               
      ev.ressumEt[METSummary_t::GLOBAL]      = resSumEt;
      ev.resmet_pt[METSummary_t::FWD]        = resFwdPFMet.pt();                 
      ev.resmet_phi[METSummary_t::FWD]       = resFwdPFMet.phi();              
      ev.ressumEt[METSummary_t::FWD]         = resFwdSumEt;
      ev.resmet_pt[METSummary_t::CENTRAL]    = res2v4PFMet.pt();    
      ev.resmet_phi[METSummary_t::CENTRAL]   = res2v4PFMet.phi();         
      ev.ressumEt[METSummary_t::CENTRAL]     = res2v4SumEt;
      
      ev.chresmet_pt[METSummary_t::GLOBAL]   = resChSum.pt();                  
      ev.chresmet_phi[METSummary_t::GLOBAL]  = resChSum.phi();   
      ev.chressumEt[METSummary_t::GLOBAL]    = resChSumEt;
      ev.chresmet_pt[METSummary_t::FWD]      = 0;
      ev.chresmet_phi[METSummary_t::FWD]     = 0;   
      ev.chressumEt[METSummary_t::FWD]       = 0;
      ev.chresmet_pt[METSummary_t::CENTRAL]  = ev.chresmet_pt[METSummary_t::GLOBAL];
      ev.chresmet_phi[METSummary_t::CENTRAL] = ev.chresmet_phi[METSummary_t::GLOBAL];   
      ev.chressumEt[METSummary_t::CENTRAL]   = ev.chressumEt[METSummary_t::GLOBAL];
      
      LorentzVector resFwdNeutralSum(resNeutralSum-res2v4NeutralSum);
      ev.neutresmet_pt[METSummary_t::GLOBAL]   = resNeutralSum.pt(); 
      ev.neutresmet_phi[METSummary_t::GLOBAL]  = resNeutralSum.phi();   
      ev.neutressumEt[METSummary_t::GLOBAL]    = resNeutralSumEt;
      ev.neutresmet_pt[METSummary_t::FWD]      = resFwdNeutralSum.pt(); 
      ev.neutresmet_phi[METSummary_t::FWD]     = resFwdNeutralSum.phi();          
      ev.neutressumEt[METSummary_t::FWD]       = resNeutralSumEt-res2v4NeutralSumEt;
      ev.neutresmet_pt[METSummary_t::CENTRAL]  = res2v4NeutralSum.pt();           
      ev.neutresmet_phi[METSummary_t::CENTRAL] = res2v4NeutralSum.phi();   
      ev.neutressumEt[METSummary_t::CENTRAL]   = res2v4NeutralSumEt;
      
      //not associated
      LorentzVector globalNeutralNonAssoc(globalNeutralFwdSum+globalNeutral2v4notassSum);
      ev.neutnonassocmet_pt[METSummary_t::GLOBAL]   = globalNeutralNonAssoc.pt();         
      ev.neutnonassocmet_phi[METSummary_t::GLOBAL]  = globalNeutralNonAssoc.phi(); 
      ev.neutnonassocsumEt[METSummary_t::GLOBAL]    = globalNeutralFwdSumEt+globalNeutral2v4notassSumEt;
      ev.neutnonassocmet_pt[METSummary_t::FWD]      = globalNeutralFwdSum.pt();               
      ev.neutnonassocmet_phi[METSummary_t::FWD]     = globalNeutralFwdSum.phi();    
      ev.neutnonassocsumEt[METSummary_t::FWD]       = globalNeutralFwdSumEt;
      ev.neutnonassocmet_pt[METSummary_t::CENTRAL]  = globalNeutral2v4notassSum.pt();    
      ev.neutnonassocmet_phi[METSummary_t::CENTRAL] = globalNeutral2v4notassSum.phi();   
      ev.neutnonassocsumEt[METSummary_t::CENTRAL]   = globalNeutral2v4notassSumEt;
      
      evHandler_->fill();
    }


  //count the jets above the selected threshold
  std::vector<TString> jetCategories;
  int njets(0);
  for(std::vector<reco::PFJet>::const_iterator jIt=vtxJets[0].begin();  jIt!=vtxJets[0].end(); jIt++)  njets += (jIt->pt()>minJetPt_ && fabs(jIt->eta())<maxJetEta_); 
  jetCategories.push_back("all");
  jetCategories.push_back( njets==0 ? "eq0jets" : "geq1jets" );

  //check transverse activity in the forward region of CMS
  std::vector<TString> fwdActivityCategories;
  fwdActivityCategories.push_back("");
  if(globalFwdPFMet.pt()<20) fwdActivityCategories.push_back("lowfwdactivity");

  for(std::vector<TString>::iterator jcIt = jetCategories.begin(); jcIt != jetCategories.end(); jcIt++)
    for(std::vector<TString>::iterator facIt = fwdActivityCategories.begin(); facIt != fwdActivityCategories.end(); facIt++) 
      {
	TString ctf=*jcIt+*facIt;

	//vertex histograms
	controlHistos_.fillHisto("vtx_multiplicity",ctf,nVtx);
	for(size_t iVtx=0; iVtx<nVtx; iVtx++)
	  {
	    string vtxType(iVtx==0 ? "vtxprim": "vtxother");
	    
	    const reco::VertexRef vtxref(vtxH_,iVtx);
	    controlHistos_.fillHisto(vtxType + "_ntracks",ctf,vtxref->nTracks());
	    controlHistos_.fillHisto(vtxType + "_chi2",ctf,vtxref->normalizedChi2());
	    controlHistos_.fillHisto(vtxType + "_pt",ctf,vtxref->p4().pt());

	    controlHistos_.fillHisto(vtxType+"_jetmultiplicity",ctf,vtxJets[iVtx].size());
	    std::vector<reco::PFJet>::const_iterator jIt(vtxJets[iVtx].begin()), jItEnd(vtxJets[iVtx].end());	    
	    for(;jIt!=jItEnd; jIt++)
	      {
		controlHistos_.fillHisto(vtxType+"_jetpt",ctf,jIt->pt());
		controlHistos_.fillHisto(vtxType+"_jeteta",ctf,jIt->eta());
		controlHistos_.fillHisto(vtxType+"_jetchmultiplicity",ctf,jIt->chargedMultiplicity());
		controlHistos_.fillHisto(vtxType+"_jetneutmultiplicity",ctf,jIt->neutralMultiplicity());
		controlHistos_.fillHisto(vtxType+"_jetetaetamoment",ctf,jIt->etaetaMoment());
		controlHistos_.fillHisto(vtxType+"_jetetaphimoment",ctf,jIt->phiphiMoment());
		controlHistos_.fillHisto(vtxType+"_jetphiphimoment",ctf,jIt->etaphiMoment());
		controlHistos_.fillHisto(vtxType+"_jetradius",ctf, sqrt(pow(jIt->etaetaMoment(),2)+pow(jIt->phiphiMoment(),2)) );
	      }
	    
	    controlHistos_.fillHisto(vtxType+"_chargedMet",ctf,vtxChSum[iVtx].pt());
	    controlHistos_.fillHisto(vtxType+"_neutralMet",ctf,vtxNeutralSum[iVtx].pt());
	    controlHistos_.fillHisto(vtxType+"_met",ctf,vtxPFMet[iVtx].pt());
	    controlHistos_.fillHisto(vtxType+"_chargedSumEt",ctf,vtxChSumEt[iVtx]);
	    controlHistos_.fillHisto(vtxType+"_neutralSumEt",ctf,vtxNeutralSumEt[iVtx]);
	    controlHistos_.fillHisto(vtxType+"_SumEt",ctf,vtxSumEt[iVtx]);
	  }

	//particle flow candidates
	controlHistos_.fillHisto("pf_multiplicity",ctf,nPFCands);
	controlHistos_.fillHisto("pf_charged",ctf,nch);
	controlHistos_.fillHisto("pfassoc_charged",ctf,nchass);
	controlHistos_.fillHisto("pfnotassoc_charged",ctf,nchnotass);
	controlHistos_.fillHisto("pf_neutrals",ctf,nneut);
	controlHistos_.fillHisto("pffwd_neutrals",ctf,nneutfwd);
	controlHistos_.fillHisto("pf2v4_neutrals",ctf,nneut2v4);
	controlHistos_.fillHisto("pf2v4assoc_neutrals",ctf,nneut2v4ass);
	controlHistos_.fillHisto("pf2v4notassoc_neutrals",ctf,nneut2v4notass);
	for (size_t iPFCand=0; iPFCand<nPFCands; iPFCand++)
	  {
	    reco::PFCandidateRef candRef(pfCandsH_,iPFCand);
	    bool isVtxAssociated(vertexAssociationMasks_[iPFCand] > -1);

	    if(candRef->charge() != 0)
	      {
		reco::TrackBaseRef trackBaseRef( candRef->trackRef() );
		double normChi2(9999.), nhits(0),npixelHits(0);
		if(!trackBaseRef.isNull())
		  {
		    normChi2 = trackBaseRef->normalizedChi2();
		    nhits = trackBaseRef->numberOfValidHits();
		    npixelHits =trackBaseRef->hitPattern().numberOfValidPixelHits();
		  }
		controlHistos_.fillHisto("pf_ptcharged",ctf,candRef->pt());	
		controlHistos_.fillHisto("pf_chi2charged",ctf,normChi2);
		controlHistos_.fillHisto("pf_nhitscharged",ctf,nhits);
		controlHistos_.fillHisto("pf_npixelhitscharged",ctf,npixelHits);
		controlHistos_.fill2DHisto("pf_etavsphicharged",ctf,candRef->eta(),candRef->phi());
		controlHistos_.fill2DHisto("pf_etavsptcharged",ctf,candRef->eta(),candRef->pt());
		if(isVtxAssociated)
		  {
		    const reco::VertexRef vtxref(vtxH_, vertexAssociationMasks_[iPFCand]);
		    double d0 = trackBaseRef->dxy( vtxref->position() );
		    double dz = trackBaseRef->dz( vtxref->position() );
		    float w = vtxref->trackWeight(trackBaseRef);
		    
		    controlHistos_.fillHisto("pfassoc_ptcharged",ctf,candRef->pt());			
		    controlHistos_.fillHisto("pf_dzcharged",ctf,dz);
		    controlHistos_.fillHisto("pf_d0charged",ctf,d0);
		    controlHistos_.fillHisto("pf_weightcharged",ctf,w);
		    controlHistos_.fill2DHisto("pf_dzvsd0charged",ctf,dz,d0);
		    controlHistos_.fill2DHisto("pf_dzvsetacharged",ctf,dz,candRef->eta());
		    controlHistos_.fill2DHisto("pf_dzvsptcharged",ctf,dz,candRef->pt());
		    controlHistos_.fill2DHisto("pf_dzvsweightcharged",ctf,dz,w);
		    controlHistos_.fill2DHisto("pf_dzvschi2charged",ctf,dz,normChi2);
		    controlHistos_.fill2DHisto("pf_dzvsnhitscharged",ctf,dz,nhits);
		    controlHistos_.fill2DHisto("pf_dzvsnpixelhitscharged",ctf,dz,npixelHits);
		  }
		else
		  {
		    controlHistos_.fillHisto("pfnotassoc_ptcharged",ctf,candRef->pt());
		  }
	      }
	    else
	      {
		controlHistos_.fillHisto("pf_ptneutrals",ctf,candRef->pt());
		controlHistos_.fill2DHisto("pf_etavsphineutrals",ctf,candRef->eta(),candRef->phi());
		controlHistos_.fill2DHisto("pf_etavsptneutrals",ctf,candRef->eta(),candRef->pt());
		if(fabs(candRef->eta())<2.4)
		  {
		    controlHistos_.fillHisto("pf2v4_ptneutrals",ctf,candRef->pt());
		    if(isVtxAssociated) controlHistos_.fillHisto("pf2v4assoc_ptneutrals",ctf,candRef->pt());	
		    else  controlHistos_.fillHisto("pf2v4notassoc_ptneutrals",ctf,candRef->pt());
		  }
	      }
	  }
      
	//full event 
	controlHistos_.fillHisto("evt_chargedmet",ctf,globalChSum.pt());
	controlHistos_.fillHisto("evt_neutralmet",ctf,globalNeutralSum.pt());
	controlHistos_.fillHisto("evtfwd_neutralmet",ctf,globalNeutralFwdSum.pt());
	controlHistos_.fillHisto("evt2v4_neutralmet",ctf,globalNeutral2v4Sum.pt());
	controlHistos_.fillHisto("evt2v4assoc_neutralmet",ctf,globalNeutral2v4assSum.pt());
	controlHistos_.fillHisto("evt2v4notassoc_neutralmet",ctf,globalNeutral2v4notassSum.pt());
	controlHistos_.fillHisto("evt_met",ctf,globalPFMet.pt());
	controlHistos_.fillHisto("evtfwd_met",ctf,globalFwdPFMet.pt());
	controlHistos_.fillHisto("evt2v4_met",ctf,global2v4PFMet.pt());
	
	//SUM ET
	controlHistos_.fillHisto("evt_chargedSumEt",ctf,globalChSumEt);
	controlHistos_.fillHisto("evt_neutralSumEt",ctf,globalNeutralSumEt);
	controlHistos_.fillHisto("evtfwd_neutralSumEt",ctf,globalNeutralFwdSumEt);
	controlHistos_.fillHisto("evt2v4_neutralSumEt",ctf,globalNeutral2v4SumEt);
	controlHistos_.fillHisto("evt2v4assoc_neutralSumEt",ctf,globalNeutral2v4assSumEt);
	controlHistos_.fillHisto("evt2v4notassoc_neutrlSumEt",ctf,globalNeutral2v4notassSumEt);
	
	controlHistos_.fillHisto("evt_fracchargedSumEt",ctf,globalChSumEt/globalSumEt);
	controlHistos_.fillHisto("evt_fracneutralSumEt",ctf,globalNeutralSumEt/globalSumEt);
	controlHistos_.fillHisto("evtfwd_fracneutralSumEt",ctf,globalNeutralFwdSumEt/globalSumEt);
	controlHistos_.fillHisto("evt2v4_fracneutralSumEt",ctf,globalNeutral2v4SumEt/globalSumEt);
	controlHistos_.fillHisto("evt2v4assoc_fracneutralSumEt",ctf,globalNeutral2v4assSumEt/globalSumEt);
	controlHistos_.fillHisto("evt2v4notassoc_fracneutralSumEt",ctf,globalNeutral2v4notassSumEt/globalSumEt);
	
	controlHistos_.fillHisto("evt_SumEt",ctf,globalSumEt);
	controlHistos_.fillHisto("evtfwd_SumEt",ctf,globalFwdSumEt);
	controlHistos_.fillHisto("evt2v4_SumEt",ctf,global2v4SumEt);
	controlHistos_.fill2DHisto("evt_metVsSumEt",ctf,globalSumEt,globalPFMet.pt());
     
	controlHistos_.fillHisto("evt_chargedmetRes",ctf,resChSum.pt());
	controlHistos_.fillHisto("evt_neutralmetRes",ctf,resNeutralSum.pt());
	controlHistos_.fillHisto("evt_metRes",ctf,resPFMet.pt());
	controlHistos_.fillHisto("evt2v4_metRes",ctf,res2v4PFMet.pt());
	controlHistos_.fillHisto("evtfwd_metRes",ctf,globalFwdPFMet.pt());
	controlHistos_.fillHisto("evt_metMin",ctf,min(resPFMet.pt(),globalPFMet.pt()));
	controlHistos_.fillHisto("evt_chargedMetMin",ctf,min(resChSum.pt(),globalPFMet.pt()));
	controlHistos_.fillHisto("evt2v4_neutralmetRes",ctf,resNeutralSum.pt());
	controlHistos_.fillHisto("evt2v4_metMin",ctf,min(res2v4PFMet.pt(),global2v4PFMet.pt()));
	controlHistos_.fillHisto("evt2v4_chargedMetMin",ctf,min(resChSum.pt(),global2v4PFMet.pt()));
	controlHistos_.fillHisto("evt_chargedSumEtRes",ctf,resChSumEt);
	controlHistos_.fillHisto("evt_neutralSumEtRes",ctf,resNeutralSumEt);
	controlHistos_.fillHisto("evt_SumEtRes",ctf,resSumEt);
	controlHistos_.fillHisto("evtfwd_SumEtRes",ctf,globalFwdSumEt);
	controlHistos_.fillHisto("evt2v4_neutralSumEtRes",ctf,resNeutralSumEt);
	controlHistos_.fillHisto("evt2v4_SumEtRes",ctf,res2v4SumEt);
	controlHistos_.fill2DHisto("evt_metVsSumEtRes",ctf,globalSumEt,resPFMet.pt());

	controlHistos_.fill2DHisto("evt_metVsNvtx",ctf,nVtx,globalPFMet.pt());
	controlHistos_.fill2DHisto("evt2v4_metVsNvtx",ctf,nVtx,global2v4PFMet.pt());
	controlHistos_.fill2DHisto("evtfwd_metVsNvtx", ctf,nVtx,globalFwdPFMet.pt());
	controlHistos_.fill2DHisto("evt_chargedmetVsNvtx",ctf,nVtx,globalChSum.pt());
	controlHistos_.fill2DHisto("evt2v4assoc_neutralmetVsNvtx",ctf,nVtx,globalNeutral2v4assSum.pt());
	controlHistos_.fill2DHisto("evt2v4notassoc_neutralmetVsNvtx",ctf,nVtx,globalNeutral2v4notassSum.pt());
	controlHistos_.fill2DHisto("evt_chargedmetRes", ctf,nVtx,resChSum.pt());
	controlHistos_.fill2DHisto("evt_neutralmetRes", ctf,nVtx,resNeutralSum.pt());
	controlHistos_.fill2DHisto("evt_metRes", ctf,nVtx,resPFMet.pt());
	controlHistos_.fill2DHisto("evt2v4_metRes", ctf,nVtx,res2v4PFMet.pt());
	controlHistos_.fill2DHisto("evt_metMin", ctf,nVtx,min(resPFMet.pt(),globalPFMet.pt()));
	controlHistos_.fill2DHisto("evt_chargedMetMin",ctf,nVtx,min(resChSum.pt(),globalPFMet.pt()));
	controlHistos_.fill2DHisto("evt2v4_metMin", ctf,nVtx,min(res2v4PFMet.pt(),global2v4PFMet.pt()));
	controlHistos_.fill2DHisto("evt2v4_chargedMetMin",ctf,nVtx,min(resChSum.pt(),global2v4PFMet.pt()));
	controlHistos_.fill2DHisto("evt_metMinAllvs2v4", ctf,nVtx,min(res2v4PFMet.pt(),global2v4PFMet.pt()),min(res2v4PFMet.pt(),global2v4PFMet.pt()));
	controlHistos_.fill2DHisto("evt_chargedmetMinAllvs2v4", ctf,nVtx,min(resChSum.pt(),globalPFMet.pt()),min(resChSum.pt(),globalPFMet.pt()));
	controlHistos_.fill2DHisto("evt_metMin2v4vsFwd", ctf,nVtx,min(res2v4PFMet.pt(),global2v4PFMet.pt()),min(res2v4PFMet.pt(),globalFwdPFMet.pt()));
	controlHistos_.fill2DHisto("evt_chargedmetMin2v4vsFwd", ctf,nVtx,min(resChSum.pt(),globalPFMet.pt()),min(resChSum.pt(),globalFwdPFMet.pt()));
      }
  
  // and finally put it in the event
  std::auto_ptr<reco::PFMET> assocPfMetPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> assocPfMetWithFwdPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> assocOtherVtxPfMetPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> globalPfMetPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> centralPfMetPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> cleanPfMetPtr(new reco::PFMET);
  std::auto_ptr<reco::PFMET> trkPfMetPtr(new reco::PFMET);

  CommonMETData pfOutput;
  PFSpecificAlgo pf;
  edm::Handle<edm::View<reco::Candidate> > pfRecoCandsH;
  iEvent.getByLabel(collectionTag_, pfRecoCandsH); 
  pfOutput.mex = -globalPFMet.px();
  pfOutput.mey = -globalPFMet.py();
  pfOutput.mez = -globalPFMet.pz();
  pfOutput.met = globalPFMet.pt();
  pfOutput.sumet = globalSumEt;
  pfOutput.phi = atan2(-globalPFMet.py(),-globalPFMet.px());
  *globalPfMetPtr = pf.addInfo(pfRecoCandsH,pfOutput);

  pfOutput.mex = -global2v4PFMet.px();
  pfOutput.mey = -global2v4PFMet.py();
  pfOutput.mez = -global2v4PFMet.pz();
  pfOutput.met = global2v4PFMet.pt();
  pfOutput.sumet = global2v4SumEt;
  pfOutput.phi = atan2(-global2v4PFMet.py(),-global2v4PFMet.px());
  *centralPfMetPtr = pf.addInfo(pfRecoCandsH,pfOutput);

  pfOutput.mex = -vtxPFMet[0].px();
  pfOutput.mey = -vtxPFMet[0].py();
  pfOutput.mez = -vtxPFMet[0].pz();
  pfOutput.met = vtxPFMet[0].pt();
  pfOutput.sumet = vtxSumEt[0];
  pfOutput.phi = atan2(-vtxPFMet[0].py(),-vtxPFMet[0].px());
  *assocPfMetPtr = pf.addInfo(pfRecoCandsH,pfOutput);

  LorentzVector assocPfMetWithFwd(vtxPFMet[0]+globalNonAssocNeutralFwdSum);
  double assocPfMetWithFwdSumEt(vtxSumEt[0]+globalNonAssocNeutralFwdSumEt);
  pfOutput.mex = -assocPfMetWithFwd.px();
  pfOutput.mey = -assocPfMetWithFwd.py();
  pfOutput.mez = -assocPfMetWithFwd.pz();
  pfOutput.met = assocPfMetWithFwd.pt();
  pfOutput.sumet = assocPfMetWithFwdSumEt;
  pfOutput.phi = atan2(-assocPfMetWithFwd.py(),-assocPfMetWithFwd.px());
  *assocPfMetWithFwdPtr = pf.addInfo(pfRecoCandsH,pfOutput);

  pfOutput.mex=0;
  pfOutput.mey=0;
  pfOutput.mez=0;
  pfOutput.sumet=0;
  for(size_t ivtx=1; ivtx<nVtx; ivtx++)
    {
      pfOutput.mex -= vtxPFMet[ivtx].px();
      pfOutput.mey -= vtxPFMet[ivtx].py();
      pfOutput.mez -= vtxPFMet[ivtx].pz();
      pfOutput.sumet += vtxSumEt[ivtx];
    }
  pfOutput.met = sqrt(pow(pfOutput.mex,2)+pow(pfOutput.mey,2));
  pfOutput.phi = atan2(pfOutput.mey,pfOutput.mex);
  *assocOtherVtxPfMetPtr = pf.addInfo(pfRecoCandsH,pfOutput);
  
  pfOutput.mex = -vtxChSum[0].px();
  pfOutput.mey = -vtxChSum[0].py();
  pfOutput.mez = -vtxChSum[0].pz();
  pfOutput.met = vtxChSum[0].pt();
  pfOutput.sumet = vtxChSumEt[0];
  pfOutput.phi = atan2(-vtxChSum[0].py(),-vtxChSum[0].px());
  *trkPfMetPtr = pf.addInfo(pfRecoCandsH,pfOutput);

  pfOutput.mex = -resPFMet.px();
  pfOutput.mey = -resPFMet.py();
  pfOutput.mez = -resPFMet.pz();
  pfOutput.met = resPFMet.pt();
  pfOutput.sumet = resSumEt;
  pfOutput.phi = atan2(-resPFMet.py(),-resPFMet.px());
  *cleanPfMetPtr = pf.addInfo(pfRecoCandsH,pfOutput);

  std::auto_ptr<std::vector<double> > globalPfMetSumsPtr(new std::vector<double> );
  globalPfMetSumsPtr->resize(12,0);
  (*globalPfMetSumsPtr)[0] = globalSumEt;
  (*globalPfMetSumsPtr)[1] = globalChSumEt;
  (*globalPfMetSumsPtr)[2] = globalNeutralSumEt;
  (*globalPfMetSumsPtr)[3] = global2v4SumEt;
  (*globalPfMetSumsPtr)[4] = globalChSumEt;
  (*globalPfMetSumsPtr)[5] = globalNeutral2v4SumEt;
  (*globalPfMetSumsPtr)[6] = vtxSumEt[0];
  (*globalPfMetSumsPtr)[7] = vtxChSumEt[0];
  (*globalPfMetSumsPtr)[8] = vtxNeutralSumEt[0];
  for(size_t ivtx=1; ivtx<nVtx; ivtx++)
    {
      (*globalPfMetSumsPtr)[9] += vtxSumEt[ivtx]; 
      (*globalPfMetSumsPtr)[10] += vtxChSumEt[ivtx]; 
      (*globalPfMetSumsPtr)[11] += vtxNeutralSumEt[ivtx]; 
    }

  iEvent.put(assocPfMetPtr,"assocPfMet");
  iEvent.put(assocPfMetWithFwdPtr,"assocPfMetWithFwd");
  iEvent.put(assocOtherVtxPfMetPtr,"assocOtherVtxPfMet");
  iEvent.put(globalPfMetPtr,"globalPfMet");
  iEvent.put(centralPfMetPtr,"centralPfMet");
  iEvent.put(trkPfMetPtr,"trkPfMet");
  iEvent.put(cleanPfMetPtr,"cleanPfMet");
  iEvent.put(globalPfMetSumsPtr,"globalPfMetSums");
}

//
ClusteredPFMetProducer::~ClusteredPFMetProducer() { }

//
void ClusteredPFMetProducer::beginJob() { }

//
void ClusteredPFMetProducer::endJob() 
{ 
//   //compute the average and estimate the resolution for each category considered
//   std::string metProfiles[]  = {"evt_metVsSumEt",
// 				"evt_metVsSumEtRes",
// 				"evt_metVsNvtx",
// 				"evt2v4_metVsNvtx",
// 				"evtfwd_metVsNvtx", 
// 				"evt_chargedmetVsNvtx",
// 				"evt2v4assoc_neutralmetVsNvtx",
// 				"evt2v4notassoc_neutralmetVsNvtx",
// 				"evt_chargedmetRes", 
// 				"evt_neutralmetRes", 
// 				"evt_metRes", 
// 				"evt2v4_metRes", 
// 				"evt_metMin", 
// 				"evt_chargedMetMin",
// 				"evt2v4_metMin", 
// 				"evt2v4_chargedMetMin",
// 				"evt_metMinAllvs2v4", 
// 				"evt_chargedmetMinAllvs2v4", 
// 				"evt_metMin2v4vsFwd", 
// 				"evt_chargedmetMin2v4vsFwd"};

//   SelectionMonitor::StepMonitor_t mons=controlHistos_.getAllMonitors();
//   for(SelectionMonitor::StepMonitor_t::iterator cIt = mons.begin(); cIt != mons.end(); cIt++)
//     {
//       for(size_t i=0; i< sizeof(metProfiles)/sizeof(std::string); i++)    
// 	{
// 	  TH2D *profileH = (TH2D *) controlHistos_.getHisto(metProfiles[i],cIt->first);
// 	  cout << metProfiles[i] << " " << cIt->first << endl;
// 	  if(profileH==0) continue;
	  
// 	  TGraphAsymmErrors *avgGr = new TGraphAsymmErrors;
// 	  avgGr->SetName(TString(metProfiles[i]+"_avg"));

// 	  TGraphAsymmErrors *rmsGr = new TGraphAsymmErrors;
// 	  rmsGr->SetName(TString(metProfiles[i]+"_rms"));
	  
// 	  for(int ibin=1; ibin<=profileH->GetXaxis()->GetNbins(); ibin++) 
// 	    {
// 	      TH1*proj = profileH->ProjectionY("projy",ibin,ibin);
// 	      float x = profileH->GetXaxis()->GetBinCenter(ibin);
// 	      avgGr->SetPoint(ibin-1,x,proj->GetMean());
// 	      avgGr->SetPointError(ibin-1,0,0,proj->GetMeanError(),proj->GetMeanError());
// 	      rmsGr->SetPoint(ibin-1,x,proj->GetRMS());
// 	      rmsGr->SetPointError(ibin-1,0,0,proj->GetRMSError(),proj->GetRMSError());
// 	    }
	  
// 	  //put the graphics in the corresponding category
// 	  TFileDirectory *dir = controlHistos_.getDirectoryForStep(cIt->first);
// 	  dir->make<TGraphAsymmErrors>(*avgGr);
// 	  dir->make<TGraphAsymmErrors>(*rmsGr);
// 	} 
//     }
}


DEFINE_FWK_MODULE(ClusteredPFMetProducer);

//  LocalWords:  numberOfValidPixelHits
