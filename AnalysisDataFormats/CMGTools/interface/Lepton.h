#ifndef ANALYSISDATAFORMATS_CMGTOOLS_LEPTON_H_
#define ANALYSISDATAFORMATS_CMGTOOLS_LEPTON_H_

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

#include <vector>

namespace cmg
{

//forward def needed
template <class LeptonType> class Lepton;
template <class LeptonType> class LeptonSettingTool;

template <class LeptonType>
class Lepton : public cmg::PhysicsObjectWithPtr< LeptonType >{
public:
	
	Lepton(){
	}
	Lepton(const LeptonType& o):
		cmg::PhysicsObjectWithPtr<LeptonType>::PhysicsObjectWithPtr(o),
        charge_(UnSet(int)),
        chargedIso_(UnSet(double)),
        neutralIso_(UnSet(double)),
        photonIso_(UnSet(double)),
        dxy_(UnSet(float)),
        dz_(UnSet(float)){
		}
	virtual ~Lepton(){
	}

    int charge() const{
        return charge_;
    }

    //isolations    
    double chargedIso() const{
        return chargedIso_;
    }
    double neutralIso() const{
        return neutralIso_;
    }
    double photonIso() const{
        return photonIso_;
    }
    double absIso() const{
        return chargedIso() + neutralIso() + photonIso();   
    }
    double relIso() const{
        return absIso()/this->pt();
    }
    
    //impart parameters
    float dxy() const{
        return dxy_;
    }
    float dz() const{
        return dz_;
    }
    
	friend class cmg::LeptonSettingTool<LeptonType>;
	
private:

    int charge_;
    double chargedIso_;
    double neutralIso_;
    double photonIso_;
    
    float dxy_;
    float dz_;


};

}

#endif /*ANALYSISDATAFORMATS_CMGTOOLS_LEPTON_H_*/