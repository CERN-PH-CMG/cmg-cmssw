#include "CMGTools/WMass/plugins/MyMetFlavorProducerWeights.h"

MyMetFlavorProducerWeights::MyMetFlavorProducerWeights(const edm::ParameterSet& iConfig){
  produces<reco::PFMETCollection>();
  vtxLabel_   = iConfig.getParameter<edm::InputTag>("vtxTag");
  weightMin_  = iConfig.getUntrackedParameter<double>("myweight");
  fDZMin      = iConfig.getUntrackedParameter<double>("dzcut");
  fChargedCandPtMin = iConfig.getUntrackedParameter<double>("chargedCandPtMin");
  isMC_       = iConfig.getParameter<bool>("isMC");
  fMetFlavor  = iConfig.getParameter<int>("MetFlavor");  
}
MyMetFlavorProducerWeights::~MyMetFlavorProducerWeights(){ }

// ------------ method called once each job just before starting event loop  ------------
void MyMetFlavorProducerWeights::beginJob(){ }

void MyMetFlavorProducerWeights::endJob(){ }

double MyMetFlavorProducerWeights::DZ(const math::XYZTLorentzVector & p4, const math::XYZPointD vtx, const math::XYZPointD vertexPos){
  return  (vtx.z()-vertexPos.z()) - ((vtx.x()-vertexPos.x())*p4.x()+(vtx.y()-vertexPos.y())*p4.y())/ p4.pt() *  p4.z()/ p4.pt();
}

// ------------ method called to for each event  ------------
void MyMetFlavorProducerWeights::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(vtxLabel_,vertices);

  const math::XYZPointD &  myPosition = vertices->front().position();

  //-----------------------------------------------------------------------
  // GENERATOR LEVEL - retrieve the genTrackMET
  //-----------------------------------------------------------------------
  math::XYZTLorentzVector genTkMET(0,0,0,0), genPFMET(0,0,0,0);
  double genTkMET_sumEt=0, genPFMET_sumEt=0;

  if( isMC_ && (fMetFlavor == 0 || fMetFlavor ==1) ){
    Handle<reco::GenParticleCollection> GenParticleCandidate;
    iEvent.getByLabel( "genParticles", GenParticleCandidate );

    reco::GenParticleCollection genParticle = *GenParticleCandidate;

    for (reco::GenParticleCollection::const_iterator genp = genParticle.begin(); genp != genParticle.end(); ++ genp ){

      //    cout << "genp->pdgId() " << genp->pdgId() << " genp->status() " << genp->status() << endl;

      if(genp->status()!=1) continue;
      if(abs(genp->pdgId()) == 12 || abs(genp->pdgId()) == 14 || abs(genp->pdgId()) == 16) continue; // remove neutrinos
      if(abs(genp->pdgId()) == 1000022) continue; // remove LSPs
      if(abs(genp->pdgId()) == 1000012 || abs(genp->pdgId())==1000014 || abs(genp->pdgId())==1000016 ) continue;
      if(abs(genp->pdgId()) == 2000012 || abs(genp->pdgId())==2000014 || abs(genp->pdgId())==2000016 ) continue;
      if(abs(genp->pdgId()) == 1000039 || abs(genp->pdgId())==5100039 ) continue;
      if(abs(genp->pdgId()) == 4000012 || abs(genp->pdgId())==4000014 || abs(genp->pdgId())==4000016 ) continue;
      if(abs(genp->pdgId()) == 9900012 || abs(genp->pdgId())==9900014 || abs(genp->pdgId())==9900016 ) continue;
      if(abs(genp->pdgId()) == 39) continue;
      if(abs(genp->pdgId()) == 18) continue; // this is needed for the special samples from KEN

      /// https://cmssdt.cern.ch/SDT/lxr/source/RecoMET/Configuration/python/GenMETParticles_cff.py#056
      //      if(fabs(genp->eta())>3) continue;
      //            if(fabs(genp->eta())>3.0) continue;

      math::XYZTLorentzVector p3(genp->px(),genp->py(),genp->pz(),genp->energy());
      genPFMET-=p3;
      genPFMET_sumEt+=p3.pt();

      if(genp->charge()==0) continue;
      if(genp->status()!=1) continue;
      if(fabs(genp->eta())>2.5) continue;

      genTkMET-=p3;
      genTkMET_sumEt+=p3.pt();

    }
  }

  //-----------------------------------------------------------------------
  // RECONSTRUCTION LEVEL
  //-----------------------------------------------------------------------

  double pfMET_sumEt=0, pfTKMET_sumEt=0;
  math::XYZTLorentzVector pfMET, pfTKMET;
  double pfTKMET_dz_01_sumEt=0,pfTKMET_dz_005_sumEt=0, pfTKMET_dz_001_sumEt=0, pfTKMET_dz_01_weight_sumEt=0, pfTKMET_weight_sumEt=0, pfTKMET_strangetk_sumEt=0;
  math::XYZTLorentzVector pfTKMET_dz_01,pfTKMET_dz_005, pfTKMET_dz_001, pfTKMET_dz_01_weight, pfTKMET_weight, pfTKMET_strangetk;

  edm::Handle < reco::PFCandidateCollection > pfCandidates;
  iEvent.getByLabel ("particleFlow", pfCandidates);

  for(reco::PFCandidateCollection::const_iterator  itCand =  pfCandidates->begin(); itCand != pfCandidates->end(); itCand++){
    
    TrackRef PFChargedHadrCand_track=itCand->trackRef();
    if (!PFChargedHadrCand_track)continue;

    const math::XYZPointD &  myVertex = itCand->vertex();
    double dz=fabs(DZ(itCand->p4(),myVertex,myPosition));
    float trackWeight=vertices->front().trackWeight(itCand->trackRef());

    math::XYZTLorentzVector p3(itCand->px(),itCand->py(),itCand->pz(),itCand->energy());
    pfMET-=p3;
    pfMET_sumEt+=p3.pt();
    if(  itCand->charge() && itCand->pt() > fChargedCandPtMin ){
      pfTKMET-=p3; pfTKMET_sumEt+=p3.pt();
      if(dz<fDZMin){                           pfTKMET_dz_01-=p3; pfTKMET_dz_01_sumEt+=p3.pt(); }
      if(dz<0.05){                             pfTKMET_dz_005-=p3; pfTKMET_dz_005_sumEt+=p3.pt(); }
      if(dz<0.01){                             pfTKMET_dz_001-=p3; pfTKMET_dz_001_sumEt+=p3.pt(); }
      if(trackWeight>weightMin_ && dz<fDZMin){ pfTKMET_dz_01_weight-=p3;pfTKMET_dz_01_weight_sumEt+=p3.pt(); }
      if(trackWeight>weightMin_){              pfTKMET_weight-=p3; pfTKMET_weight_sumEt+=p3.pt(); }
      if((trackWeight>weightMin_||dz<fDZMin)){ pfTKMET_strangetk-=p3; pfTKMET_strangetk_sumEt+=p3.pt(); }
    }
  }

  //double u = fabs(sqrt( u_perp*u_perp + u_parall*u_parall ));
  //double unorm = fabs(sqrt( u_perp*u_perp + u_parall*u_parall ))/ptVis.Pt();
  //double uPhi=TMath::ATan2(u_perp,-u_parall);
  
  PFMET lDummy;
  // DEFAULT IS pfTKMET_strangetk (metflavor 2)
  double sumEt = pfTKMET_strangetk_sumEt;
  math::XYZTLorentzVector vMET = pfTKMET_strangetk;
  
  if(isMC_ && fMetFlavor == 0 ){
    sumEt = genPFMET_sumEt;
    vMET  = genPFMET;
  }else if(isMC_ && fMetFlavor == 1 ){
    sumEt = genTkMET_sumEt;
    vMET  = genTkMET;
  }else if(fMetFlavor == 3 ){
    sumEt = pfTKMET_weight_sumEt;
    vMET  = pfTKMET_weight;
  }else if(fMetFlavor == 4 ){
    sumEt = pfTKMET_dz_01_sumEt;
    vMET  = pfTKMET_dz_01;
  }
  
  PFMET lMet(lDummy.getSpecific(),sumEt,vMET,myPosition); 

  std::auto_ptr<reco::PFMETCollection> lPFMetColl;
  lPFMetColl.reset     ( new reco::PFMETCollection);
  lPFMetColl->push_back( lMet);
  iEvent.put( lPFMetColl );
  
}

#include "FWCore/Framework/interface/MakerMacros.h"

//define this as a plug-in
DEFINE_FWK_MODULE(MyMetFlavorProducerWeights);