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
  t_->Branch("weight",     &evSummary_.weight,   "weight/F");
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

  t_->GetBranch("run")->SetAddress(&evSummary_.run);
  t_->GetBranch("lumi")->SetAddress(&evSummary_.lumi);
  t_->GetBranch("event")->SetAddress(&evSummary_.event);
  t_->GetBranch("cat")->SetAddress(&evSummary_.cat);
  t_->GetBranch("nvtx")->SetAddress(&evSummary_.nvtx);
  t_->GetBranch("ngenpu")->SetAddress(&evSummary_.ngenpu);
  t_->GetBranch("njets")->SetAddress(&evSummary_.njets);
  t_->GetBranch("weight")->SetAddress(&evSummary_.weight);
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

  //mc truth
  if(t->GetBranch("nmcparticles"))
    {
      t_->GetBranch("nmcparticles")->SetAddress( &evSummary_.nmcparticles );
      t_->GetBranch("mcpx")->SetAddress( evSummary_.mcpx );
      t_->GetBranch("mcpy")->SetAddress( evSummary_.mcpy );
      t_->GetBranch("mcpz")->SetAddress( evSummary_.mcpz );
      t_->GetBranch("mcen")->SetAddress( evSummary_.mcen );
      t_->GetBranch("mcid")->SetAddress( evSummary_.mcid );
    }
  else   evSummary_.nmcparticles=0;

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
