#ifndef COMPOUNDTYPES_H_
#define COMPOUNDTYPES_H_

#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/DiObject.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"

namespace cmg{
    
 typedef cmg::DiObject<cmg::Electron,cmg::Electron> DiElectron;   
 typedef cmg::DiObject<cmg::BaseJet,cmg::BaseJet> DiJet;   
 typedef cmg::DiObject<cmg::Muon,cmg::Muon> DiMuon;
 typedef cmg::DiObject<cmg::PFJet,cmg::PFJet> DiPFJet;
 typedef cmg::DiObject<cmg::Electron,cmg::BaseMET> WENu;
 typedef cmg::DiObject<cmg::Muon,cmg::BaseMET> WMuNu;   
    
}


#endif /*COMPOUNDTYPES_H_*/
