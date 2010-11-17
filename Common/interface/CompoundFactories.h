#ifndef COMPOUNDFACTORIES_H_
#define COMPOUNDFACTORIES_H_

#include "CMGTools/Common/interface/DiObjectFactory.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

namespace cmg{
    
    typedef DiObjectFactory< cmg::DiJet::type1, cmg::DiJet::type2 > DiJetFactory;
    typedef DiObjectFactory< cmg::DiMuon::type1, cmg::DiMuon::type2 > DiMuonFactory;
    
}

#endif /*COMPOUNDFACTORIES_H_*/
