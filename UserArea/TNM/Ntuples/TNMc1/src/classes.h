//
//--------------------------------------------------------------------
#include "Ntuples/TNMc1/interface/patJetHelper.h"
#include "Ntuples/TNMc1/interface/patMuonHelper.h"
#include "Ntuples/TNMc1/interface/patElectronHelper.h"
#include "Ntuples/TNMc1/interface/edmEventHelperExtra.h"
#include "Ntuples/TNMc1/interface/LHEEventProductHelper.h"




namespace
{
  HelperFor<pat::Jet> t_patJetHelper;
  HelperFor<pat::Muon> t_patMuonHelper;
  HelperFor<pat::Electron> t_patElectronHelper;
  HelperFor<edm::Event> t_edmEventHelperExtra;
  HelperFor<LHEEventProduct> t_LHEEventProductHelper;
}
