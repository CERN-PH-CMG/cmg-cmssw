#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"

//
ZZ2l2nuSummaryHandler::ZZ2l2nuSummaryHandler()
{

}
 
//
bool ZZ2l2nuSummaryHandler::initTree(TTree *t)
{
  if(t==0) return false;
  t_ = t;

  t_->Branch("run",        &evSummary_.run,    "run/I");
  t_->Branch("lumi",       &evSummary_.lumi,   "lumi/I");
  t_->Branch("event",      &evSummary_.event,  "event/I");
  t_->Branch("cat",        &evSummary_.cat,   "cat/I");
  t_->Branch("nvtx",       &evSummary_.nvtx,   "nvtx/I");
  t_->Branch("ngenpu",     &evSummary_.ngenpu,   "ngenpu/I");
  t_->Branch("njets",      &evSummary_.njets,   "njets/I");
  t_->Branch("nbtags",     &evSummary_.nbtags,   "nbtags/I");
  t_->Branch("weight",     &evSummary_.weight,   "weight/F");
  t_->Branch("cormet_pt",     &evSummary_.cormet_pt,   "cormet_pt/F");
  t_->Branch("cormet_phi",    &evSummary_.cormet_phi,  "cormet_phi/F");
  t_->Branch("redmet_pt",     &evSummary_.redmet_pt,   "redmet_pt/F");
  t_->Branch("redmet_phi",    &evSummary_.redmet_phi,  "redmet_phi/F");
  t_->Branch("redmet_dilL",   &evSummary_.redmet_dilL,   "redmet_dilL/F");
  t_->Branch("redmet_dilT",   &evSummary_.redmet_dilT,   "redmet_dilT/F");
  t_->Branch("redmet_deltaL",   &evSummary_.redmet_deltaL,   "redmet_deltaL/F");
  t_->Branch("redmet_deltaT",   &evSummary_.redmet_deltaT,   "redmet_deltaT/F");
  t_->Branch("redmet_rjetL",   &evSummary_.redmet_rjetL,   "redmet_rjetL/F");
  t_->Branch("redmet_rjetT",   &evSummary_.redmet_rjetT,   "redmet_rjetT/F");
  t_->Branch("redmet_metL",   &evSummary_.redmet_metL,   "redmet_metL/F");
  t_->Branch("redmet_metT",   &evSummary_.redmet_metT,   "redmet_metT/F");
  t_->Branch("redmet_rL",   &evSummary_.redmet_rL,   "redmet_rL/F");
  t_->Branch("redmet_rT",   &evSummary_.redmet_rT,   "redmet_rT/F");
  t_->Branch("projmet_pt",     &evSummary_.projmet_pt,   "projmet_pt/F");
  t_->Branch("tkmet_pt",     &evSummary_.tkmet_pt,   "tkmet_pt/F");
  t_->Branch("tkmet_phi",    &evSummary_.tkmet_phi,  "tkmet_phi/F");
  t_->Branch("genmet_pt",     &evSummary_.genmet_pt,   "genmet_pt/F");
  t_->Branch("genmet_phi",    &evSummary_.genmet_phi,  "genmet_phi/F");

  t_->Branch("rho", &evSummary_.rho, "rho/F");

  //selected particles
  t_->Branch("nparticles", &evSummary_.nparticles, "nparticles/I");
  t_->Branch("px", evSummary_.px, "px[nparticles]/F");
  t_->Branch("py", evSummary_.py, "py[nparticles]/F");
  t_->Branch("pz", evSummary_.pz, "pz[nparticles]/F");
  t_->Branch("en", evSummary_.en, "en[nparticles]/F");
  t_->Branch("id", evSummary_.id, "id[nparticles]/I");
  t_->Branch("genid", evSummary_.genid, "genid[nparticles]/I");
  t_->Branch("info1", evSummary_.info1, "info1[nparticles]/F");
  t_->Branch("info2", evSummary_.info2, "info2[nparticles]/F");
  t_->Branch("info3", evSummary_.info3, "info3[nparticles]/F");
  t_->Branch("info4", evSummary_.info4, "info4[nparticles]/F");
  t_->Branch("info5", evSummary_.info5, "info5[nparticles]/F");

  return true;
}

//
bool ZZ2l2nuSummaryHandler::attachToTree(TTree *t)
{
  if(t==0) return false;
  t_ = t;

  t_->GetBranch("run")->SetAddress(&evSummary_.run);
  t_->GetBranch("lumi")->SetAddress(&evSummary_.lumi);
  t_->GetBranch("event")->SetAddress(&evSummary_.event);
  t_->GetBranch("cat")->SetAddress(&evSummary_.cat);
  t_->GetBranch("nvtx")->SetAddress(&evSummary_.nvtx);
  t_->GetBranch("ngenpu")->SetAddress(&evSummary_.ngenpu);
  t_->GetBranch("njets")->SetAddress(&evSummary_.njets);
  t_->GetBranch("nbtags")->SetAddress(&evSummary_.nbtags);
  t_->GetBranch("weight")->SetAddress(&evSummary_.weight);

  //met
  t_->GetBranch("cormet_pt")->SetAddress( &evSummary_.cormet_pt );
  t_->GetBranch("cormet_phi")->SetAddress( &evSummary_.cormet_phi );
  t_->GetBranch("redmet_pt")->SetAddress( &evSummary_.redmet_pt );
  t_->GetBranch("redmet_phi")->SetAddress( &evSummary_.redmet_phi );
  t_->GetBranch("redmet_dilL")->SetAddress( &evSummary_.redmet_dilL );
  t_->GetBranch("redmet_dilT")->SetAddress( &evSummary_.redmet_dilT );
  t_->GetBranch("redmet_deltaL")->SetAddress( &evSummary_.redmet_deltaL );
  t_->GetBranch("redmet_deltaT")->SetAddress( &evSummary_.redmet_deltaT );
  t_->GetBranch("redmet_rjetL")->SetAddress( &evSummary_.redmet_rjetL );
  t_->GetBranch("redmet_rjetT")->SetAddress( &evSummary_.redmet_rjetT );
  t_->GetBranch("redmet_metL")->SetAddress( &evSummary_.redmet_metL );
  t_->GetBranch("redmet_metT")->SetAddress( &evSummary_.redmet_metT );
  t_->GetBranch("redmet_rL")->SetAddress( &evSummary_.redmet_rL );
  t_->GetBranch("redmet_rT")->SetAddress( &evSummary_.redmet_rT );
  t_->GetBranch("projmet_pt")->SetAddress( &evSummary_.projmet_pt );
  t_->GetBranch("tkmet_pt")->SetAddress( &evSummary_.tkmet_pt );
  t_->GetBranch("tkmet_phi")->SetAddress( &evSummary_.tkmet_phi );
  t_->GetBranch("genmet_pt")->SetAddress( &evSummary_.genmet_pt );
  t_->GetBranch("genmet_phi")->SetAddress( &evSummary_.genmet_phi );
  t_->GetBranch("rho")->SetAddress( &evSummary_.rho );

  //selected particles
  t_->GetBranch("nparticles")->SetAddress( &evSummary_.nparticles );
  t_->GetBranch("px")->SetAddress( evSummary_.px );
  t_->GetBranch("py")->SetAddress( evSummary_.py );
  t_->GetBranch("pz")->SetAddress( evSummary_.pz );
  t_->GetBranch("en")->SetAddress( evSummary_.en );
  t_->GetBranch("id")->SetAddress( evSummary_.id );
  t_->GetBranch("genid")->SetAddress( evSummary_.genid );
  t_->GetBranch("info1")->SetAddress( evSummary_.info1 );
  t_->GetBranch("info2")->SetAddress( evSummary_.info2 );
  t_->GetBranch("info3")->SetAddress( evSummary_.info3 );
  t_->GetBranch("info4")->SetAddress( evSummary_.info4 );
  t_->GetBranch("info5")->SetAddress( evSummary_.info5 );

  return true;
}


//
void ZZ2l2nuSummaryHandler::fillTree()
{
  if(t_) t_->Fill();
}

//
ZZ2l2nuSummaryHandler::~ZZ2l2nuSummaryHandler()
{
}
