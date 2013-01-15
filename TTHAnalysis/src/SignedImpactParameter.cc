#include "CMGTools/TTHAnalysis/interface/SignedImpactParameter.h"

#include "MagneticField/UniformEngine/src/UniformMagneticField.h"
#include "MagneticField/ParametrizedEngine/plugins/OAEParametrizedMagneticField.h"

#include "TrackingTools/IPTools/interface/IPTools.h"

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
