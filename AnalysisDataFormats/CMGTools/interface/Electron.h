#ifndef ANALYSISDATAFORMATS_CMGTOOLS_ELECTRON_H_
#define ANALYSISDATAFORMATS_CMGTOOLS_ELECTRON_H_

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/Lepton.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"

#include <vector>

namespace cmg
{

//forward def needed
class Electron;
class ElectronFactory;

class Electron : public cmg::Lepton<pat::ElectronPtr>{
public:
	
	Electron(){
	}
	Electron(const value& e):
        cmg::Lepton<value>::Lepton(e),
        mva_(UnSet(float)){
		}
	virtual ~Electron(){
	}
	
    float mva() const{
        return mva_;
    }
    
	friend class cmg::ElectronFactory;
	
private:
    
    float mva_;

};

}

#endif /*ANALYSISDATAFORMATS_CMGTOOLS_ELECTRON_H_*/