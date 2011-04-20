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

#include <cmath>
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
        mva_(UnSet(float)),
        sigmaIetaIeta_(UnSet(Double_t)),
        deltaPhiSuperClusterTrackAtVtx_(UnSet(Double_t)),
        deltaEtaSuperClusterTrackAtVtx_(UnSet(Double_t)),
        hadronicOverEm_(UnSet(Double_t)),
        numberOfHits_(UnSet(Double_t)),
        convDist_(UnSet(Double_t)),
        convDcot_(UnSet(Double_t)),
        isEcalDriven_(UnSet(cmg::TriBool)){
		}
	virtual ~Electron(){
	}
	
    float mva() const{
        return mva_;
    }
    Double_t sigmaIetaIeta() const{
        return sigmaIetaIeta_;
    }
    Double_t deltaPhiSuperClusterTrackAtVtx() const{
        return deltaPhiSuperClusterTrackAtVtx_;
    }
    Double_t deltaEtaSuperClusterTrackAtVtx() const{
        return deltaEtaSuperClusterTrackAtVtx_;
    }
    Double_t hadronicOverEm() const{
        return hadronicOverEm_;
    }
    Double_t numberOfHits() const{
        return numberOfHits_;
    }
    Double_t convDist() const{
        return convDist_;   
    }
    Double_t convDcot() const{
        return convDcot_;
    }
    cmg::TriBool isConv() const{
        cmg::TriBool result = UnSet(cmg::TriBool);
        if(!cmg::isUnset(convDist()) && !cmg::isUnset(convDcot())){
            result = cmg::toTriBool(fabs(convDist()) < 0.02 && fabs(convDcot()) < 0.02);
        }
        return result;
    }
    cmg::TriBool ecalDriven() const{
        return isEcalDriven_;
    }
    // from LeafCandidate
    virtual bool isElectron() const {
      return true;
    }
    
private:
    
    float mva_;
    // variables needed for WP selections
    Double_t sigmaIetaIeta_;
    Double_t deltaPhiSuperClusterTrackAtVtx_;
    Double_t deltaEtaSuperClusterTrackAtVtx_;
    Double_t hadronicOverEm_;
    Double_t numberOfHits_;
    Double_t convDist_;
    Double_t convDcot_;
    cmg::TriBool isEcalDriven_;
          
    friend class cmg::ElectronFactory;    

};

}

#endif /*ANALYSISDATAFORMATS_CMGTOOLS_ELECTRON_H_*/
