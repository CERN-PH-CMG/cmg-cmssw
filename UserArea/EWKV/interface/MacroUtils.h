#ifndef macroutils_h
#define macroutils_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "TVector3.h"

namespace utils
{
  namespace cmssw
  {
    
    //retrieve last state before decay
    const reco::Candidate *getGeneratorFinalStateFor(const reco::Candidate *p);

    //check pdg id code for b-flavored hadrons
    bool isBhadron(int pdgId);

    //get impact parameter for a track
    template<class T>
    std::pair<bool,Measurement1D> getImpactParameter(const T &trkRef, reco::VertexRef &vtx, const edm::EventSetup &iSetup, bool is3d=true)
      {
	edm::ESHandle<TransientTrackBuilder> trackBuilder;
	iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);
	reco::TransientTrack tt = trackBuilder->build(trkRef.get());
	if(is3d) return IPTools::absoluteImpactParameter3D(tt, *vtx);
	else     return IPTools::absoluteTransverseImpactParameter(tt, *vtx);
      }

    //
    template<class T>
    std::pair<float,float> getArcCos(T &a,T &b)
      {
	TVector3 mom1(a.px(),a.py(),a.pz());
	TVector3 mom2(b.px(),b.py(),b.pz());
	double cosine = mom1.Dot(mom2)/(mom1.Mag()*mom2.Mag());
	double arcCosine = acos(cosine);
	return std::pair<float,float>(cosine,arcCosine);
      }

    Float_t getEffectiveArea(int id, float eta);
  }
}

#endif
