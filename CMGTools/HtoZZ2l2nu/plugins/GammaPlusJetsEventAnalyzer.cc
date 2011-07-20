#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Common/interface/MergeableCounter.h"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TString.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/TSelectionMonitor.h"

//
class GammaPlusJetsEventAnalyzer : public edm::EDAnalyzer 
{
public:
  GammaPlusJetsEventAnalyzer(const edm::ParameterSet &iConfig);
  virtual void analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup) ;
  void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
  
private:

  std::map<std::string, edm::ParameterSet> objConfig_;
  ZZ2l2nuSummaryHandler summaryHandler_;
  TSelectionMonitor controlHistos_;
};

using namespace std;

//
GammaPlusJetsEventAnalyzer::GammaPlusJetsEventAnalyzer(const edm::ParameterSet &iConfig)
  : controlHistos_( iConfig.getParameter<std::string>("dtag") )
{
  /*  try{
    
    edm::Service<TFileService> fs;
    summaryHandler_.initTree(  fs->make<TTree>("data","Event Summary") );
    TFileDirectory baseDir=fs->mkdir(iConfig.getParameter<std::string>("dtag"));    

    //configuration for objects
    std::string objects[]={"Vertices","Photons","Jets","MET","Trigger"};
    for(size_t iobj=0; iobj<sizeof(objects)/sizeof(std::string); iobj++)
      objConfig_[ objects[iobj]] = iConfig.getParameter<edm::ParameterSet>(objects[iobj]);
    
    //book histograms

    controlHistos_.addHistogram("cutflow",";Selection step; Events",5,0,5);
    TH1 *h=controlHistos_.getHisto("cutflow");
    h->GetXaxis()->SetBinLabel(1,"Reco");
    h->GetXaxis()->SetBinLabel(2,"Trigger");
    h->GetXaxis()->SetBinLabel(3,"#geq 1 vertex");
    h->GetXaxis()->SetBinLabel(4,"#geq 1 photon");

    //trigger used
    controlHistos_.addHistogram("triggerthr",";p_{T} [GeV/c]; Events",100,0,200);

    //photons
    TString regs[]={"eb","ee"};
    for(size_t ireg=0; ireg<2; ireg++)
      {
	controlHistos_.addHistogram(regs[ireg]+"pt",";p_{T} [GeV/c]; Photons",50,0,200);
	controlHistos_.addHistogram(regs[ireg]+"eta",";#eta; Photons",100,-3,3);
	controlHistos_.addHistogram(regs[ireg]+"r9",";R_{9}; Photons",100,0,1);
	controlHistos_.addHistogram(regs[ireg]+"sihih",";#sigma_{i#eta,i#eta}; Photons",100,0,0.1);
	controlHistos_.addHistogram(regs[ireg]+"hoe",";h/e; Photons",100,0,0.2);
	controlHistos_.addHistogram(regs[ireg]+"trkiso",";Tracker isolation; Photons",100,0,15);
	controlHistos_.addHistogram(regs[ireg]+"ecaliso",";ECAL isolation; Photons",100,0,15);
	controlHistos_.addHistogram(regs[ireg]+"hcaliso",";HCAL isolation; Photons",100,0,15);
      }
    controlHistos_.initMonitorForStep("photon");
    
    //jets
    controlHistos_.addHistogram("jetfassoc",";#beta_{vtx}(Jet); Jets",50,0,1.0);
    controlHistos_.addHistogram("jetpt",";p_{T} [GeV/c]; Jets",50,0,200);
    controlHistos_.addHistogram("jeteta",";#eta; Jets",50,-2.5,2.5);
    controlHistos_.addHistogram("jetmult",";Jet multiplicity; Events",4,0,4);
    h=controlHistos_.getHisto("jetmult");
    for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
      {
	TString ilabel(""); ilabel+=(ibin-1);
	if(ibin==h->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	controlHistos_.getHisto("jetmult")->GetXaxis()->SetBinLabel(ibin,ilabel);
      }
    
    //MET
    controlHistos_.addHistogram("met", ";E^{miss}_{T} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.addHistogram("puffomet", ";charged-E^{miss}_{T} [GeV/c]; Events", 100,  0.,500.);
    controlHistos_.initMonitorForStep("isophoton");
  }
  catch(std::exception &e){
    cout << e.what() << endl;
  }  
  */
}


//
void GammaPlusJetsEventAnalyzer::analyze(const edm::Event &event, const edm::EventSetup &iSetup) 
{
  /*
  try{
    
    //event summary to be filled
    ZZ2l2nuSummary_t &ev = summaryHandler_.getEvent();

    //get the weight for the event
    float weight(1.0);
    edm::Handle<float> puWeightHandle;
    try{
      event.getByLabel(objConfig_["Generator"].getParameter<edm::InputTag>("puReweight"), puWeightHandle );
      if(puWeightHandle.isValid()) weight = *(puWeightHandle.product());
    }catch(std::exception &e){
    }
    ev.weight=weight;

    //pileup information
    int npuOOT(0),npuIT(0);
    edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
    try{
      event.getByType(puInfoH);
      if(puInfoH.isValid())
	{
	  for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++)
	    {
	      if(it->getBunchCrossing()==0) npuIT += it->getPU_NumInteractions();
	      else                          npuOOT += it->getPU_NumInteractions();
	    }
	}
    }catch(std::exception &e){
    }
    ev.ngenITpu=npuIT;
    ev.ngenOOTpu=npuOOT;


  //retrieve pdf info
  edm::Handle<GenEventInfoProduct> genEventInfoProd;
  try{
    event.getByType( genEventInfoProd );
    if(genEventInfoProd.isValid())
      {
	ev.genWeight = genEventInfoProd->weight();
	ev.qscale = genEventInfoProd->qScale();
	if(genEventInfoProd->pdf())
	  {
	    ev.x1  = genEventInfoProd->pdf()->x.first;
	    ev.x2  = genEventInfoProd->pdf()->x.second;
	    ev.id1 = genEventInfoProd->pdf()->id.first;
	    ev.id2 = genEventInfoProd->pdf()->id.second;
	  }
	if(genEventInfoProd->binningValues().size()>0) ev.pthat = genEventInfoProd->binningValues()[0];
	ev.hptWeights[ZZ2l2nuSummary_t::hKfactor]=1;
	ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renUp]=1;
	ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renDown]=1;
	ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factUp]=1;
	ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factDown]=1;
      }
  }catch(std::exception &e){
  }


    //get trigger results
    Int_t maxthr(0);
    std::string selTrigger("");
    edm::Handle<edm::TriggerResults> triggerBitsH;
    edm::InputTag trigSource = objConfig_["Trigger"].getParameter<edm::InputTag>("source");
    event.getByLabel( trigSource, triggerBitsH);
    int ntrigs = triggerBitsH->size();
    const edm::TriggerNames &triggerNames = event.triggerNames( *triggerBitsH );
    for(int itrig=0; itrig<ntrigs; itrig++)
      {
	//require only one photon triggering
	std::string trigName = triggerNames.triggerName(itrig);
	size_t found = trigName.find("Photon");
	if( found == std::string::npos ) continue;
	size_t foundDouble = trigName.find("Double");
	if( foundDouble != std::string::npos ) continue;
	found = trigName.find("Photon",found+1,6);
	if( found != std::string::npos ) continue;

	//check if the trigger fired
	if( !triggerBitsH->wasrun(itrig) ) continue;
        if( triggerBitsH->error(itrig) ) continue;
        if( !triggerBitsH->accept(itrig) ) continue;
	
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
    if(maxthr==0) return;
    controlHistos_.fillHisto("cutflow","photon",1.,weight);
    controlHistos_.fillHisto("triggerthr","photon",maxthr,weight);

    //select the vertex
    edm::Handle<reco::VertexCollection> hVtx;
    event.getByLabel(objConfig_["Vertices"].getParameter<edm::InputTag>("source"), hVtx);
    std::vector<reco::VertexRef> selVertices = vertex::filter(hVtx,objConfig_["Vertices"]);
    if(selVertices.size()==0) return;
    reco::VertexRef primVertex = selVertices[0]; 
    controlHistos_.fillHisto("cutflow","photon",2.,weight);

    //average energy density
    edm::Handle< double > rho;
    event.getByLabel( objConfig_["Jets"].getParameter<edm::InputTag>("rho"), rho );

    //photon
    CandidateWithVertexCollection gammaCandidate;
    edm::Handle<edm::View<reco::Candidate> > hPhoton;
    event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("source"), hPhoton );
    for(size_t ipho=0; ipho<hPhoton->size(); ipho++)
      {
	const pat::Photon *photon = dynamic_cast<const pat::Photon *>( &((*hPhoton)[ipho]) );
	TString reg(photon->isEB() ? "eb" : "ee");
	controlHistos_.fillHisto(reg+"pt","photon",photon->pt(),weight);
	controlHistos_.fillHisto(reg+"eta","photon",photon->superCluster()->eta(),weight);
	controlHistos_.fillHisto(reg+"r9","photon",photon->r9(),weight);
	controlHistos_.fillHisto(reg+"sihih","photon",photon->sigmaIetaIeta(),weight);
	controlHistos_.fillHisto(reg+"hoe","photon",photon->hadronicOverEm(),weight);
	controlHistos_.fillHisto(reg+"trkiso","photon",photon->trkSumPtSolidConeDR04(),weight);
	controlHistos_.fillHisto(reg+"ecaliso","photon",photon->ecalRecHitSumEtConeDR04(),weight);
	controlHistos_.fillHisto(reg+"hcaliso","photon",photon->hcalTowerSumEtConeDR04(),weight);
      }
    
    edm::Handle<EcalRecHitCollection> ebrechits;
    event.getByLabel( objConfig_["Photons"].getParameter<edm::InputTag>("ebrechits"), ebrechits );
    EcalClusterLazyTools lazyTool(event,iSetup,
				  objConfig_["Photons"].getParameter<edm::InputTag>("ebrechits"),
				  objConfig_["Photons"].getParameter<edm::InputTag>("eerechits"));
    CandidateCollection isolPhotons=photon::filter(hPhoton,lazyTool,ebrechits,objConfig_["Photons"],maxthr);
    for(size_t ipho=0; ipho<isolPhotons.size(); ipho++)
      {
	const pat::Photon *photon = dynamic_cast<const pat::Photon *>( isolPhotons[ipho].get() );
	TString reg(photon->isEB() ? "eb" : "ee");
	controlHistos_.fillHisto(reg+"pt","isophoton",photon->pt(),weight);
	controlHistos_.fillHisto(reg+"eta","isophoton",photon->superCluster()->eta(),weight);
	controlHistos_.fillHisto(reg+"r9","isophoton",photon->r9(),weight);
	controlHistos_.fillHisto(reg+"sihih","isophoton",photon->sigmaIetaIeta(),weight);
	controlHistos_.fillHisto(reg+"hoe","isophoton",photon->hadronicOverEm(),weight);
	controlHistos_.fillHisto(reg+"trkiso","isophoton",photon->trkSumPtSolidConeDR04(),weight);
	controlHistos_.fillHisto(reg+"ecaliso","isophoton",photon->ecalRecHitSumEtConeDR04(),weight);
	controlHistos_.fillHisto(reg+"hcaliso","isophoton",photon->hcalTowerSumEtConeDR04(),weight);
	if(gammaCandidate.size()==0) gammaCandidate.push_back( CandidateWithVertex(isolPhotons[ipho], primVertex) );
	else
	  {
	    if(gammaCandidate[0].first->pt() > isolPhotons[ipho]->pt()) continue;
	    gammaCandidate[0].first = isolPhotons[ipho];
	  }
      }
    if(gammaCandidate.size()==0) return;
    controlHistos_.fillHisto("cutflow","photon",3.,weight);

    //Jets
    edm::Handle<edm::View<reco::Candidate> > hJet;
    event.getByLabel(objConfig_["Jets"].getParameter<edm::InputTag>("source"), hJet);
    CandidateWithVertexCollection preSelJets = jet::filter(hJet, gammaCandidate, selVertices, objConfig_["Jets"]);
    CandidateWithVertexCollection selJets;
    for(size_t ijet=0; ijet<preSelJets.size(); ijet++)
      {
	selJets.push_back(preSelJets[ijet]);

	//monitor
	const pat::Jet *jet=dynamic_cast<const pat::Jet *>(preSelJets[ijet].first.get());
	controlHistos_.fillHisto("jetfassoc","isophoton",jet::fAssoc(jet,primVertex.get()),weight);
	controlHistos_.fillHisto("jetpt","isophoton",jet->pt(),weight);
        controlHistos_.fillHisto("jeteta","isophoton",jet->eta(),weight);
      }
   controlHistos_.fillHisto("jetmult","isophoton",selJets.size(),weight);

    //add the met                                                                                                                                                                                        
    edm::Handle<edm::View<reco::Candidate> > hMET;
    event.getByLabel(objConfig_["MET"].getParameter<edm::InputTag>("source"), hMET);
    reco::CandidatePtr met = hMET->ptrAt(0);
    edm::Handle< edm::ValueMap<reco::PFMET> > chargedMets;
    event.getByLabel(objConfig_["MET"].getParameter<edm::InputTag>("chsource"), chargedMets);
    LorentzVector chmet(0,0,0,0);
    for(size_t ivtx=0; ivtx<hVtx->size(); ivtx++)
      {
	reco::VertexRef vtxRef(hVtx,ivtx);
	if(vtxRef->position().z()!=primVertex->position().z()) continue;
	const reco::PFMET &chpfmet=(*chargedMets)[vtxRef];
	chmet=LorentzVector(chpfmet.px(),chpfmet.py(),0,chpfmet.pt());
	break;
      }
    controlHistos_.fillHisto("met","isophoton",met->pt(),weight);
    controlHistos_.fillHisto("puffomet","isophoton",chmet.pt(),weight);

    //save summary
    ev.run=event.id().run();
    ev.lumi=event.luminosityBlock();
    ev.event=event.id().event();
    ev.cat=22+1000*maxthr;
    ev.nvtx=selVertices.size();
    ev.rho=*rho;
    ev.nparticles=3+selJets.size();

    const pat::Photon *photon = dynamic_cast<const pat::Photon *>( gammaCandidate[0].first.get() );
    ev.px[0] = photon->px();   ev.py[0]=photon->py();     ev.pz[0]=photon->pz();      ev.en[0]=photon->energy();              ev.id[0]=22; 
    ev.info1[0] = photon->superCluster()->eta();          ev.info2[0] = photon->r9(); ev.info3[0]=photon->hadronicOverEm();   ev.info4[0]=photon->sigmaIetaIeta(); 
    ev.px[1] = met->px();      ev.py[1]=met->py();        ev.pz[1]=0;                 ev.en[1]=met->pt();                     ev.id[1]=0;
    ev.info1[1]=chmet.px();    ev.info2[1]=chmet.py();    ev.info3[1] = chmet.pz();
    ev.px[2]=primVertex->p4().px();  ev.py[2]=primVertex->p4().py();  ev.pz[2]=primVertex->p4().pz();  ev.en[2]=primVertex->p4().energy(); ev.id[2]=500;

    for(size_t ijet=0; ijet<selJets.size(); ijet++)
      {
	int ipart(3+ijet);
	const pat::Jet *jet = dynamic_cast<const pat::Jet *>( selJets[ijet].first.get() );
	ev.px[ipart] = jet->px();  ev.py[ipart]=jet->py();  ev.pz[ipart]=jet->pz(); ev.en[ipart]=jet->energy();  ev.id[ipart]=1;
        const reco::Candidate *genParton = jet->genParton();
        ev.genid[ipart] = genParton ? genParton->pdgId() : -9999;
        ev.info1[ipart]=jet->bDiscriminator("trackCountingHighEffBJetTags");
	ev.info2[ipart]=jet->bDiscriminator("trackCountingHighPurBJetTags");
        ev.info3[ipart]=jet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
        ev.info4[ipart]=jet->bDiscriminator("simpleSecondaryVertexHighPurBJetTags");
	ev.info5[ipart]=jet::fAssoc(jet,primVertex.get());
      }

    summaryHandler_.fillTree();

  }catch(std::exception &e){
    std::cout << "[CleanEventAnalysis][analyze] failed with " << e.what() << std::endl;
  }
  */
}

//
void GammaPlusJetsEventAnalyzer::endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup)
{
  /*
  //  cout << "[GammaPlusJetsEventAnalyzer::endLuminosityBlock]" << endl;
  edm::Handle<edm::MergeableCounter> ctrHandle;
  iLumi.getByLabel("startCounter", ctrHandle);
  controlHistos_.fillHisto("cutflow","photon",0.,ctrHandle->value);
  */
}


DEFINE_FWK_MODULE(GammaPlusJetsEventAnalyzer);


 
