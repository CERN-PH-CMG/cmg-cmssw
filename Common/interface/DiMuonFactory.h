#ifndef DIMUONFACTORY_H_
#define DIMUONFACTORY_H_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/DiMuon.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/MuonFactory.h"

#include <iostream>
#include <memory>

namespace cmg{

class DiMuonFactory : public Factory<cmg::DiMuon>{
	public:
		DiMuonFactory(const edm::ParameterSet& ps):
			leg1ps_(ps.getParameter<edm::ParameterSet>("leg1")),
			leg2ps_(ps.getParameter<edm::ParameterSet>("leg2")){
		}
		virtual event_ptr create(const edm::Event&, const edm::EventSetup&) const;
		
	private:
		const edm::ParameterSet leg1ps_;
		const edm::ParameterSet leg2ps_;
};

}

#endif /*DIMUONFACTORY_H_*/
