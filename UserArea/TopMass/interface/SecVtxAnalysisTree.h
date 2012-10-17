#ifndef SecVtxAnalysisTree_h
#define SecVtxAnalysisTree_h

#include "TTree.h"


#define MAXPARTICLES 20

struct SecVtxAnalysisEvent_t
{
  //event identifier
  Int_t run, event, lumi;
  Int_t nvtx;       //vertex multiplicity
  Int_t ngenITpu;   //generated PU
  Float_t l_pt, l_eta, l_phi, l_en, l_relIso, l_spt;  //lepton variables
  Int_t jn;  //number of selected jets
  Float_t j_pt[MAXPARTICLES],    j_eta[MAXPARTICLES],    j_phi[MAXPARTICLES], j_en[MAXPARTICLES];    //jet kinematics
  Float_t j_lxy[MAXPARTICLES], j_slxy[MAXPARTICLES], j_csv[MAXPARTICLES];
  Int_t   j_genId[MAXPARTICLES], j_flavId[MAXPARTICLES];   //generator level matched parton and flavor
  Float_t met_pt, met_phi;  //MET kinematics
};


void initSecVtxAnalysisTree(TTree *t, SecVtxAnalysisEvent_t &ev, bool initToWrite=true)
{
  if(t==0) return;

  //set the addresses to be used to fill the tree
  if(initToWrite)
    {
      t->Branch("run",        &ev.run,        "run/I");
      t->Branch("lumi",       &ev.lumi,       "lumi/I");
      t->Branch("event",      &ev.event,      "event/I");
      t->Branch("nvtx",       &ev.nvtx,       "nvtx/I");
      t->Branch("ngenITpu",   &ev.ngenITpu,   "ngenITpu/I");
      t->Branch("l_pt",       &ev.l_pt,       "l_pt/F");
      t->Branch("l_eta",      &ev.l_eta,      "l_eta/F");
      t->Branch("l_phi",      &ev.l_phi,      "l_phi/F");
      t->Branch("l_en",       &ev.l_en,       "l_en/F");
      t->Branch("l_relIso",   &ev.l_relIso,   "l_relIso/F");
      t->Branch("jn",         &ev.jn,         "jn/I");
      t->Branch("j_pt",      ev.j_pt,       "j_pt[jn]/F");
      t->Branch("j_eta",     ev.j_eta,      "j_eta[jn]/F");
      t->Branch("j_phi",     ev.j_phi,      "j_phi[jn]/F");
      t->Branch("j_en",      ev.j_en,       "j_en[jn]/F");
      t->Branch("j_lxy",     ev.j_lxy,      "j_lxy[jn]/F");
      t->Branch("j_slxy",      ev.j_slxy,       "j_slxy[jn]/F");
      t->Branch("j_csv",      ev.j_csv,       "j_csv[jn]/F");
      t->Branch("j_genId",   ev.j_genId,    "j_genId[jn]/I");
      t->Branch("j_flavId",   ev.j_flavId,    "j_flavId[jn]/I");
      t->Branch("met_pt",     &ev.met_pt,     "met_pt/F");
      t->Branch("met_phi",    &ev.met_phi,    "met_phi/F");
    }

  //set the addresses to be used to read the tree
  else
    {
      t->GetBranch("run")->SetAddress(&ev.run);
      t->GetBranch("lumi")->SetAddress(&ev.lumi);
      t->GetBranch("event")->SetAddress(&ev.event);
      t->GetBranch("nvtx")->SetAddress(&ev.nvtx);
      t->GetBranch("ngenITpu")->SetAddress(&ev.ngenITpu);
      t->GetBranch("l_pt")->SetAddress(&ev.l_pt);
      t->GetBranch("l_eta")->SetAddress(&ev.l_eta);
      t->GetBranch("l_phi")->SetAddress(&ev.l_phi);
      t->GetBranch("l_en")->SetAddress(&ev.l_en);
      t->GetBranch("l_relIso")->SetAddress(&ev.l_relIso);
      t->GetBranch("jn")->SetAddress(&ev.jn);
      t->GetBranch("j_pt")->SetAddress(ev.j_pt);
      t->GetBranch("j_eta")->SetAddress(ev.j_eta);
      t->GetBranch("j_phi")->SetAddress(ev.j_phi);
      t->GetBranch("j_en")->SetAddress(ev.j_en);
      t->GetBranch("j_lxy")->SetAddress(ev.j_lxy);
      t->GetBranch("j_slxy")->SetAddress(ev.j_slxy);
      t->GetBranch("j_csv")->SetAddress(ev.j_csv);
      t->GetBranch("j_genId")->SetAddress(ev.j_genId);
      t->GetBranch("j_flavId")->SetAddress(ev.j_flavId);
      t->GetBranch("met_pt")->SetAddress(&ev.met_pt);
      t->GetBranch("met_phi")->SetAddress(&ev.met_phi);
    }
    
}


#endif
