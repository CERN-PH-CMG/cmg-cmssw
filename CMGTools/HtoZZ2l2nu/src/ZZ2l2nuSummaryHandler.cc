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

  //event info
  t_->Branch("run",        &evSummary_.run,       "run/I");
  t_->Branch("lumi",       &evSummary_.lumi,      "lumi/I");
  t_->Branch("event",      &evSummary_.event,     "event/I");
  t_->Branch("cat",        &evSummary_.cat,       "cat/I");

  //vertices and average energy density
  t_->Branch("nvtx",       &evSummary_.nvtx,      "nvtx/I");
  t_->Branch("rho",        &evSummary_.rho,        "rho/F");

  //generator level info
  t_->Branch("ngenITpu",   &evSummary_.ngenITpu,  "ngenITpu/I");
  t_->Branch("ngenOOTpu",  &evSummary_.ngenOOTpu,  "ngenOOTpu/I");
  t_->Branch("weight",     &evSummary_.weight,    "weight/F");
  t_->Branch("hptWeights", evSummary_.hptWeights, "hptWeights[5]/F");
  t_->Branch("pthat",      &evSummary_.pthat,      "pthat/F");
  t_->Branch("genWeight",  &evSummary_.genWeight,  "genWeight/F");
  t_->Branch("qscale",     &evSummary_.qscale,     "qscale/F");
  t_->Branch("x1",         &evSummary_.x1,         "x1/F");
  t_->Branch("x2",         &evSummary_.x2,         "x2/F");
  t_->Branch("id1",        &evSummary_.id1,        "id1/I");
  t_->Branch("id2",        &evSummary_.id2,        "id2/I");

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

  //mc truth
  t_->Branch("nmcparticles", &evSummary_.nmcparticles, "nmcparticles/I");
  t_->Branch("mcpx", evSummary_.mcpx, "mcpx[nmcparticles]/F");
  t_->Branch("mcpy", evSummary_.mcpy, "mcpy[nmcparticles]/F");
  t_->Branch("mcpz", evSummary_.mcpz, "mcpz[nmcparticles]/F");
  t_->Branch("mcen", evSummary_.mcen, "mcen[nmcparticles]/F");
  t_->Branch("mcid", evSummary_.mcid, "mcid[nmcparticles]/I");


  return true;
}

//
bool ZZ2l2nuSummaryHandler::attachToTree(TTree *t)
{
  if(t==0) return false;
  t_ = t;

  //event info
  t_->GetBranch("run")->SetAddress(&evSummary_.run);
  t_->GetBranch("lumi")->SetAddress(&evSummary_.lumi);
  t_->GetBranch("event")->SetAddress(&evSummary_.event);
  t_->GetBranch("cat")->SetAddress(&evSummary_.cat);

  //vertices and average energy density
  t_->GetBranch("nvtx")->SetAddress(&evSummary_.nvtx);
  t_->GetBranch("rho")->SetAddress( &evSummary_.rho );

  //generator level info
  t_->GetBranch("weight")->SetAddress(&evSummary_.weight);
  t_->GetBranch("hptWeights")->SetAddress(evSummary_.hptWeights);
  t_->GetBranch("ngenITpu")->SetAddress(&evSummary_.ngenITpu);
  t_->GetBranch("ngenOOTpu")->SetAddress(&evSummary_.ngenOOTpu);
  t_->GetBranch("pthat")->SetAddress(&evSummary_.pthat);
  t_->GetBranch("genWeight")->SetAddress(&evSummary_.genWeight);
  t_->GetBranch("qscale")->SetAddress(&evSummary_.qscale);
  t_->GetBranch("x1")->SetAddress(&evSummary_.x1);
  t_->GetBranch("x2")->SetAddress(&evSummary_.x2);
  t_->GetBranch("id1")->SetAddress(&evSummary_.id1);
  t_->GetBranch("id2")->SetAddress(&evSummary_.id2);
  
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

  //mc truth event
  t_->GetBranch("nmcparticles")->SetAddress( &evSummary_.nmcparticles );
  t_->GetBranch("mcpx")->SetAddress( evSummary_.mcpx );
  t_->GetBranch("mcpy")->SetAddress( evSummary_.mcpy );
  t_->GetBranch("mcpz")->SetAddress( evSummary_.mcpz );
  t_->GetBranch("mcen")->SetAddress( evSummary_.mcen );
  t_->GetBranch("mcid")->SetAddress( evSummary_.mcid );

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
