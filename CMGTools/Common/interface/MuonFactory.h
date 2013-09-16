#ifndef MUONFACTORY_H_
#define MUONFACTORY_H_

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"
#include "CMGTools/Common/interface/LeptonSettingTool.h"

#include <iostream>
#include <memory>
#include <string>

#include "FWCore/Framework/interface/ESHandle.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

namespace cmg{

class MuonFactory : public Factory<cmg::Muon>, public SettingTool<pat::MuonPtr,cmg::Muon>{
	public:
		MuonFactory(const edm::ParameterSet& ps):
			muonLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
            type_(convert(ps.getParameter<int>("trackType"))),
            muonIDType_(ps.getParameter<std::string>("muonIDType")),
            leptonFactory_(ps.getParameter<edm::ParameterSet>("leptonFactory")){
		}
		virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
        virtual void set(const pat::MuonPtr& input, cmg::Muon* const output, const edm::Event&, const edm::EventSetup&) const;
        
        enum track_type {Global=0,Inner,Track,Other=-1};
		
	private:
        
        track_type convert(const int i) const{
            switch (i){
                case Global: return Global;
                case Inner: return Inner;
                case Track: return Track;
                default:
                    edm::LogWarning("cmg::MuonFactory") << "The track type specified was not found." << std::endl;
                    return Other;    
            }
        }
        
        reco::TrackRef getTrack(const pat::MuonPtr& input) const;
        reco::TrackRef getTrackerTrack(const pat::MuonPtr& input) const;

		const edm::InputTag muonLabel_;
        const track_type type_;
        const std::string muonIDType_;
        
        LeptonSettingTool<pat::MuonPtr> leptonFactory_;
};

}


#endif /*MUONFACTORY_H_*/
