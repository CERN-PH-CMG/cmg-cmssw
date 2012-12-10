#include "CMGTools/TTHAnalysis/interface/SignedImpactParameter.h"

#include "MagneticField/UniformEngine/src/UniformMagneticField.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

SignedImpactParameter::SignedImpactParameter(double bfield) :
    bfield_(new UniformMagneticField(bfield)) {}

SignedImpactParameter::SignedImpactParameter(const SignedImpactParameter &other) :
    bfield_(new UniformMagneticField(other.bfield_->inTesla(GlobalPoint(0,0,0)).z())) {}

SignedImpactParameter::~SignedImpactParameter() {
    delete bfield_;
}

Measurement1D 
SignedImpactParameter::signedIP3D(const reco::Track &tk, const reco::Vertex &vtx, const reco::Track::Vector jetdir) const {
    reco::TransientTrack ttk(tk,bfield_);
    return IPTools::signedImpactParameter3D(ttk, GlobalVector(jetdir.X(),jetdir.Y(),jetdir.Z()), vtx).second;
}
