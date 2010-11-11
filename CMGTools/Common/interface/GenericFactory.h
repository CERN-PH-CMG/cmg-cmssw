#ifndef CMGTOOLS_COMMON_GENERICFACTORY_H_
#define CMGTOOLS_COMMON_GENERICFACTORY_H_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/Generic.h"
#include "CMGTools/Common/interface/Factory.h"

#include <iostream>
#include <memory>

namespace cmg{

class GenericFactory : public cmg::Factory<cmg::Generic>{
	public:
		GenericFactory(const edm::ParameterSet& ps):
			inputLabel_(ps.getParameter<edm::InputTag>("inputCollection")){
		}
		virtual event_ptr create(const edm::Event&, const edm::EventSetup&) const;
		
	private:
		const edm::InputTag inputLabel_;
};

}


#endif /*CMGTOOLS_COMMON_GENERICFACTORY_H_*/