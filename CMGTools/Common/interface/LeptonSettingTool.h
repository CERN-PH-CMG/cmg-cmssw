#ifndef CMGTOOLS_COMMON_LEPTONFACTORY_H_
#define CMGTOOLS_COMMON_LEPTONFACTORY_H_

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/Lepton.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "CMGTools/Common/interface/SettingTool.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositVetos.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
 
#include "DataFormats/RecoCandidate/interface/IsoDepositDirection.h"

#include "CMGTools/Common/interface/SpecificIsolation.h"

#include <iostream>
#include <memory>

namespace cmg{

template <class LeptonType>
class LeptonSettingTool : public SettingTool<LeptonType,cmg::Lepton<LeptonType> >{
  public:
    LeptonSettingTool(const edm::ParameterSet& ps):
      chargedHadron_(pat::PfChargedHadronIso),
      chargedAll_(pat::PfChargedAllIso),		  
      puChargedHadron_(pat::PfPUChargedHadronIso),
      neutralHadron_(pat::PfNeutralHadronIso),
      photon_(pat::PfGammaIso),
      useIsoDeposits_(ps.getParameter<bool>("useIsoDeposits")),
      vertexTag_(ps.getParameter<edm::InputTag>("vertexCollection")),
      vertexType_(convert_vertex_types(ps.getParameter<int>("vertexType"))){
      chargedHadronIsoPar_.initialize( ps.getParameter<edm::ParameterSet>("chargedHadronIsoPar"));
      chargedAllIsoPar_.initialize( ps.getParameter<edm::ParameterSet>("chargedAllIsoPar"));
      puChargedHadronIsoPar_.initialize( ps.getParameter<edm::ParameterSet>("puChargedHadronIsoPar"));
      neutralHadronIsoPar_.initialize( ps.getParameter<edm::ParameterSet>("neutralHadronIsoPar"));
      photonsIsoPar_.initialize( ps.getParameter<edm::ParameterSet>("photonsIsoPar"));
      
      if(!ps.getParameter<bool>("useParticleFlowIso")){
	//switch to "traditional" isolation
	chargedHadron_ = pat::TrackIso;
	neutralHadron_ = pat::HcalIso;
	photon_ = pat::EcalIso;
      }
      
    }
        
   ///Set the isolation parameters etc
   virtual void set(const LeptonType& lepton, cmg::Lepton<LeptonType>* const obj, const edm::Event&, const edm::EventSetup&) const;
   
   //Set the dxy and dz
   template <class TrackType>
     void set(const TrackType& track, cmg::Lepton<LeptonType>* const obj, const edm::Event&, const edm::EventSetup&) const;
   
   enum vertex_types {Primary=0,BeamSpot,Other=-1};
   
   typedef reco::isodeposit::AbsVetos AbsVetos;
   
  private:
    
   vertex_types convert_vertex_types(const int i) const{
     switch (i){
     case Primary: return Primary;
     case BeamSpot: return BeamSpot;
     default:
       edm::LogWarning("cmg::LeptonSettingTool") << "The vertex type specified was not found." << std::endl;
       return Other; 
     }
   }
        
   //utilities for handling the vertex
   reco::TrackBase::Point getVertex(const edm::Event&, const edm::EventSetup&) const;
    
   /// parameters for charged hadron isolation value
   SpecificIsolation chargedHadronIsoPar_;
   
   /// parameters for charged particle isolation value
   SpecificIsolation chargedAllIsoPar_;
      
   /// parameters for pile-up charged hadron isolation value 
   SpecificIsolation puChargedHadronIsoPar_;
   
   /// parameters for neutral hadron isolation value
   SpecificIsolation neutralHadronIsoPar_;
   
   /// parameters for photon isolation value
   SpecificIsolation photonsIsoPar_;
   
   pat::IsolationKeys chargedHadron_;
   pat::IsolationKeys chargedAll_;
   pat::IsolationKeys puChargedHadron_;
   pat::IsolationKeys neutralHadron_;
   pat::IsolationKeys photon_;
   const bool useIsoDeposits_;
   
   //tags for the dxy
   edm::InputTag vertexTag_;
   vertex_types vertexType_;
    	
};

}

template <class LeptonType>
void cmg::LeptonSettingTool<LeptonType>::set(const LeptonType& lepton, cmg::Lepton<LeptonType>* const obj, 
    const edm::Event& iEvent, const edm::EventSetup&) const{

  obj->charge_ = lepton->charge();

  if(useIsoDeposits_){
    //use all the vetos etc to recalcuate the isolation
    
    const double Eta = lepton->p4().eta();
    const double Phi = lepton->p4().phi();

    // computing isolation value in cone for each
    // type of particle. 

    // retrieve the AbsVetos from the SpecificIsolation
    AbsVetos chargedHadronVetos = chargedHadronIsoPar_.getAbsVetoes();
    AbsVetos chargedAllVetos = chargedAllIsoPar_.getAbsVetoes();
    AbsVetos puChargedHadronVetos = puChargedHadronIsoPar_.getAbsVetoes();
    AbsVetos neutralHadronVetos = neutralHadronIsoPar_.getAbsVetoes();
    AbsVetos photonsVetos = photonsIsoPar_.getAbsVetoes();

    // center the vetoes around the lepton
    for(unsigned int i = 0; i<chargedHadronVetos.size(); i++){
        chargedHadronVetos[i]->centerOn(Eta,Phi);
    }
    for(unsigned int i = 0; i<chargedAllVetos.size(); i++){
        chargedAllVetos[i]->centerOn(Eta,Phi);
    }
    for(unsigned int i = 0; i<puChargedHadronVetos.size(); i++){
        puChargedHadronVetos[i]->centerOn(Eta,Phi);
    }
    for(unsigned int i = 0; i<neutralHadronVetos.size(); i++){
        neutralHadronVetos[i]->centerOn(Eta,Phi);
    }
    for(unsigned int i = 0; i<photonsVetos.size(); i++){
        photonsVetos[i]->centerOn(Eta,Phi);
    }

    obj->chargedHadronIso_ = (lepton->isoDeposit(chargedHadron_)->depositAndCountWithin( chargedHadronIsoPar_.coneSize(), chargedHadronVetos, false ).first);
    obj->chargedAllIso_ = (lepton->isoDeposit(chargedAll_)->depositAndCountWithin( chargedAllIsoPar_.coneSize(), chargedAllVetos, false ).first);
    obj->puChargedHadronIso_ = (lepton->isoDeposit(puChargedHadron_)->depositAndCountWithin( puChargedHadronIsoPar_.coneSize(), puChargedHadronVetos, false ).first);
    obj->neutralHadronIso_ = (lepton->isoDeposit(neutralHadron_)->depositAndCountWithin( neutralHadronIsoPar_.coneSize(), neutralHadronVetos, false ).first);
    obj->photonIso_ = (lepton->isoDeposit(photon_)->depositAndCountWithin( photonsIsoPar_.coneSize(), photonsVetos ,false ).first);
  
  }else{
    //ignore everything and just taked the cached value from pat
    obj->chargedHadronIso_ = lepton->userIsolation(chargedHadron_);
    obj->chargedAllIso_ = lepton->userIsolation(chargedAll_);
    obj->puChargedHadronIso_ = lepton->userIsolation(puChargedHadron_);
    obj->neutralHadronIso_ = lepton->userIsolation(neutralHadron_);
    obj->photonIso_ = lepton->userIsolation(photon_);
  }
}

template <class LeptonType> template <class TrackType>
void cmg::LeptonSettingTool<LeptonType>::set(const TrackType& track, cmg::Lepton<LeptonType>* const obj, 
    const edm::Event& iEvent, const edm::EventSetup& iSetup) const{
        if(track.isNonnull() && track.isAvailable()){
            reco::TrackBase::Point vertex = getVertex(iEvent,iSetup);
            obj->dxy_ = track->dxy(vertex);
            obj->dz_ = track->dz(vertex);
        }
}

template <class LeptonType>
reco::TrackBase::Point cmg::LeptonSettingTool<LeptonType>::getVertex(const edm::Event& iEvent, const edm::EventSetup&) const{
    
  reco::TrackBase::Point result;  
  
  switch (vertexType_){
   case BeamSpot:
    {
        edm::Handle<reco::BeamSpot> vertexCands;
        iEvent.getByLabel(vertexTag_,vertexCands);
        result = vertexCands->position();
        break;
    }
   case Primary:
    {
        edm::Handle<reco::VertexCollection> vertexCands;
        iEvent.getByLabel(vertexTag_,vertexCands);
        result = vertexCands->at(0).position();//just take the leading from the PAT ordering
        break;
    }
   default:
        break; 
  }
  return result;    
    
}


#endif /*CMGTOOLS_COMMON_LEPTONFACTORY_H_*/
