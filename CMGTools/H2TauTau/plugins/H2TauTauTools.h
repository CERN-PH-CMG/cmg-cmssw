#include "CMGTools/H2TauTau/interface/DiTauWithSVFitProducer.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

#include "CMGTools/H2TauTau/interface/DiObjectUpdateFactory.h"
#include "CMGTools/H2TauTau/interface/DiTauObjectFactory.h"
#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"


namespace cmg {

typedef DiTauObjectFactory< cmg::DiTau::type1, cmg::DiTau::type2 > DiTauFactory;
typedef DiTauObjectFactory< cmg::TauEle::type1, cmg::TauEle::type2 > TauEleFactory;
//typedef DiObjectFactory< cmg::TauMu::type1, cmg::TauMu::type2 > TauMuFactory;
typedef DiTauObjectFactory< cmg::TauMu::type1, cmg::TauMu::type2 > TauMuFactory;
typedef DiTauObjectFactory< cmg::MuEle::type1, cmg::MuEle::type2 > MuEleFactory;

typedef DiObjectUpdateFactory< cmg::TauMu > TauMuUpdateFactory;
typedef DiObjectUpdateFactory< cmg::TauEle > TauEleUpdateFactory;
typedef DiObjectUpdateFactory< cmg::MuEleDiTau > MuEleUpdateFactory;
typedef DiObjectUpdateFactory< cmg::DiTau > DiTauUpdateFactory;
  
typedef PhysicsObjectProducer<cmg::DiTauFactory> DiTauPOProducer;
typedef PhysicsObjectProducer<cmg::TauEleFactory> TauElePOProducer;
typedef PhysicsObjectProducer<cmg::TauMuFactory> TauMuPOProducer;
typedef PhysicsObjectProducer<cmg::MuEleFactory> MuElePOProducer;

typedef PhysicsObjectProducer<cmg::TauMuUpdateFactory> TauMuUpdatePOProducer;
typedef PhysicsObjectProducer<cmg::TauEleUpdateFactory> TauEleUpdatePOProducer;
typedef PhysicsObjectProducer<cmg::MuEleUpdateFactory> MuEleUpdatePOProducer;
typedef PhysicsObjectProducer<cmg::DiTauUpdateFactory> DiTauUpdatePOProducer;

}

typedef DiTauWithSVFitProducer< cmg::TauMu > TauMuWithSVFitProducer;
typedef DiTauWithSVFitProducer< cmg::TauEle > TauEleWithSVFitProducer;
typedef DiTauWithSVFitProducer< cmg::MuEle > MuEleWithSVFitProducer;
typedef DiTauWithSVFitProducer< cmg::DiTau > TauTauWithSVFitProducer;

