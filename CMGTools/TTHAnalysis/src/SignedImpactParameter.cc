#include "CMGTools/TTHAnalysis/interface/SignedImpactParameter.h"

#include "MagneticField/UniformEngine/src/UniformMagneticField.h"
#include "MagneticField/ParametrizedEngine/plugins/OAEParametrizedMagneticField.h"

#include "TrackingTools/IPTools/interface/IPTools.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include <vector>

MagneticField *SignedImpactParameter::paramField_ = 0;

SignedImpactParameter::SignedImpactParameter() {
}

SignedImpactParameter::~SignedImpactParameter() {
}

Measurement1D 
SignedImpactParameter::signedIP3D(const reco::Track &tk, const reco::Vertex &vtx, const reco::Track::Vector jetdir) const {
    if (paramField_ == 0) paramField_ = new OAEParametrizedMagneticField("3_8T");
    reco::TransientTrack ttk(tk,paramField_);
    return IPTools::signedImpactParameter3D(ttk, GlobalVector(jetdir.X(),jetdir.Y(),jetdir.Z()), vtx).second;
}

std::pair<double,double>
SignedImpactParameter::twoTrackChi2(const reco::Track &tk1, const reco::Track &tk2) const {
    if (paramField_ == 0) paramField_ = new OAEParametrizedMagneticField("3_8T");
    std::vector<reco::TransientTrack> ttks;
    ttks.push_back(reco::TransientTrack(tk1,paramField_));
    ttks.push_back(reco::TransientTrack(tk2,paramField_));
    KalmanVertexFitter vtxFitter;
    TransientVertex myVertex = vtxFitter.vertex(ttks);
    return std::make_pair(myVertex.totalChiSquared(),myVertex.degreesOfFreedom());  
}
