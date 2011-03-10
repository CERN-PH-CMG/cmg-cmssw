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
            charged_(pat::PfChargedHadronIso),
            neutral_(pat::PfNeutralHadronIso),
            photon_(pat::PfGammaIso),
            useIsoDeposits_(ps.getParameter<bool>("useIsoDeposits")),
            vertexTag_(ps.getParameter<edm::InputTag>("vertexCollection")),
            vertexType_(convert_vertex_types(ps.getParameter<int>("vertexType"))){
            chargedIsoPar_.initialize( ps.getParameter<edm::ParameterSet>("chargedIsoPar"));
            neutralIsoPar_.initialize( ps.getParameter<edm::ParameterSet>("neutralIsoPar"));
            photonsIsoPar_.initialize( ps.getParameter<edm::ParameterSet>("photonsIsoPar"));
            
            if(!ps.getParameter<bool>("useParticleFlowIso")){
                //switch to "traditional" isolation
                charged_ = pat::TrackIso;
                neutral_ = pat::HcalIso;
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
        const reco::VertexCollection::const_iterator getLeading(const reco::VertexCollection& recoVertex) const;
        reco::TrackBase::Point getVertex(const edm::Event&, const edm::EventSetup&) const;
        
    
        /// parameters for charged hadron isolation value
        SpecificIsolation chargedIsoPar_;

        /// parameters for neutral hadron isolation value
        SpecificIsolation neutralIsoPar_;

        /// parameters for photon isolation value
        SpecificIsolation photonsIsoPar_;
        
        pat::IsolationKeys charged_;
        pat::IsolationKeys neutral_;
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
    AbsVetos chargedVetos = chargedIsoPar_.getAbsVetoes();
    AbsVetos neutralVetos = neutralIsoPar_.getAbsVetoes();
    AbsVetos photonsVetos = photonsIsoPar_.getAbsVetoes();

    // center the deposits around the lepton
    for(unsigned int i = 0; i<chargedVetos.size(); i++){
        chargedVetos[i]->centerOn(Eta,Phi);
    }
    for(unsigned int i = 0; i<neutralVetos.size(); i++){
        neutralVetos[i]->centerOn(Eta,Phi);
    }
    for(unsigned int i = 0; i<photonsVetos.size(); i++){
        photonsVetos[i]->centerOn(Eta,Phi);
    }

    obj->chargedIso_ = (lepton->isoDeposit(charged_)->depositAndCountWithin( chargedIsoPar_.coneSize(), chargedVetos, false ).first);
    obj->neutralIso_ = (lepton->isoDeposit(neutral_)->depositAndCountWithin( neutralIsoPar_.coneSize(), neutralVetos, false ).first);
    obj->photonIso_ = (lepton->isoDeposit(photon_)->depositAndCountWithin( photonsIsoPar_.coneSize(), photonsVetos ,false ).first);
  
  }else{
    //ignore everything and just taked the cached value from pat
    obj->chargedIso_ = lepton->userIsolation(charged_);
    obj->neutralIso_ = lepton->userIsolation(neutral_);
    obj->photonIso_ = lepton->userIsolation(photon_);
  }
}

template <class LeptonType> template <class TrackType>
void cmg::LeptonSettingTool<LeptonType>::set(const TrackType& track, cmg::Lepton<LeptonType>* const obj, 
    const edm::Event& iEvent, const edm::EventSetup& iSetup) const{
        if(!track.isNull()){
            reco::TrackBase::Point vertex = getVertex(iEvent,iSetup);
            obj->dxy_ = track->dxy(vertex);
            obj->dz_ = track->dz(vertex);
        }
}


/// Selected a vertex based on the highest sum pt
template <class LeptonType>
const reco::VertexCollection::const_iterator cmg::LeptonSettingTool<LeptonType>::getLeading(const reco::VertexCollection& recoVertex) const{
    
    // Vertex
    reco::VertexCollection::const_iterator vi = recoVertex.begin();
    //select the one with the higest sumPt on the tracks
    if(recoVertex.size() > 1){
        double maxSumPt = -1000;
        for(reco::VertexCollection::const_iterator i = recoVertex.begin(); i != recoVertex.end(); ++i){
            const std::vector<reco::Track>& refitted = i->refittedTracks();
            double sumPt = 0.0;
            for(std::vector<reco::Track>::const_iterator j = refitted.begin(); j != refitted.end(); ++j){
                sumPt += j->pt();
            }
            if(sumPt > maxSumPt){
                maxSumPt = sumPt;
                vi = i;
            }
        }
    }
    return vi;
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
        reco::VertexCollection::const_iterator vi = this->getLeading(*vertexCands);
        result = vi->position();
        break;
    }
   default:
        break; 
  }
  return result;    
    
}


#endif /*CMGTOOLS_COMMON_LEPTONFACTORY_H_*/
