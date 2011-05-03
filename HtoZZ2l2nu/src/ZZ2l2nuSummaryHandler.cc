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
  t_->Branch("njets",      &evSummary_.njets,   "njets/I");
  t_->Branch("nbtags",     &evSummary_.nbtags,   "nbtags/I");
  t_->Branch("weight",     &evSummary_.weight,   "weight/F");

  t_->Branch("pfmet_pt",     &evSummary_.pfmet_pt,    "pfmet_pt/F");
  t_->Branch("pfmet_phi",    &evSummary_.pfmet_phi,   "pfmet_phi/F");
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

  //leptons
  t_->Branch("l1px", &evSummary_.l1px, "l1px/F");
  t_->Branch("l1py", &evSummary_.l1py, "l1py/F");
  t_->Branch("l1pz", &evSummary_.l1pz, "l1pz/F");
  t_->Branch("l1e", &evSummary_.l1e, "l1e/F");
  t_->Branch("l1id", &evSummary_.l1id, "l1id/I");

  t_->Branch("l2px", &evSummary_.l2px, "l2px/F");
  t_->Branch("l2py", &evSummary_.l2py, "l2py/F");
  t_->Branch("l2pz", &evSummary_.l2pz, "l2pz/F");
  t_->Branch("l2e", &evSummary_.l2e, "l2e/F");
  t_->Branch("l2id", &evSummary_.l2id, "l2id/I");

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
  t_->GetBranch("njets")->SetAddress(&evSummary_.njets);
  t_->GetBranch("nbtags")->SetAddress(&evSummary_.nbtags);
  t_->GetBranch("weight")->SetAddress(&evSummary_.weight);

  //met
  t_->GetBranch("pfmet_pt")->SetAddress( &evSummary_.pfmet_pt );
  t_->GetBranch("pfmet_phi")->SetAddress( &evSummary_.pfmet_phi );
  t_->GetBranch("pfmet_sig")->SetAddress( &evSummary_.pfmet_sig );
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

  //leptons
  t_->GetBranch("l1px")->SetAddress( &evSummary_.l1px );
  t_->GetBranch("l1py")->SetAddress( &evSummary_.l1py );
  t_->GetBranch("l1pz")->SetAddress( &evSummary_.l1pz );
  t_->GetBranch("l1e")->SetAddress( &evSummary_.l1e );
  t_->GetBranch("l1id")->SetAddress( &evSummary_.l1id );
  t_->GetBranch("l2px")->SetAddress( &evSummary_.l2px );
  t_->GetBranch("l2py")->SetAddress( &evSummary_.l2py );
  t_->GetBranch("l2pz")->SetAddress( &evSummary_.l2pz );
  t_->GetBranch("l2e")->SetAddress( &evSummary_.l2e );
  t_->GetBranch("l2id")->SetAddress( &evSummary_.l2id );



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
