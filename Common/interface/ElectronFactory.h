#ifndef ElectronFACTORY_H_
#define ElectronFACTORY_H_

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"
#include "CMGTools/Common/interface/LeptonSettingTool.h"
#include "CMGTools/Common/interface/CMGElectronMVAEstimator.h"

#include <iostream>
#include <memory>
#include <string>

namespace cmg{


class ElectronFactory : public Factory<cmg::Electron>, public SettingTool<pat::ElectronPtr,cmg::Electron>{

  public:
    ElectronFactory(const edm::ParameterSet& ps):
      electronLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
      primaryVertexLabel_(ps.getParameter<edm::InputTag>("primaryVertexCollection")),
	leptonFactory_(ps.getParameter<edm::ParameterSet>("leptonFactory")){
	};
      
      virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
      virtual void set(const pat::ElectronPtr& input, cmg::Electron* const output, int nVertices);

  private: 
      const edm::InputTag electronLabel_;
      const edm::InputTag primaryVertexLabel_;
      LeptonSettingTool<pat::ElectronPtr> leptonFactory_;
  };

}


#endif /*ElectronFACTORY_H_*/
