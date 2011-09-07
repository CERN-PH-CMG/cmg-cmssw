#ifndef metsummary_h
#define metsummary_h

#include "TTree.h"

#define MAXVERTICES 100
struct METSummary_t
{
  enum RegionTypes { GLOBAL, FWD, CENTRAL};

  //event info
  Int_t run, lumi,event;
  
  //vertex info
  Int_t nvtx;
  Float_t chi2[MAXVERTICES],       sumpt[MAXVERTICES];
  Int_t jn[MAXVERTICES];
  Float_t vtxmet_pt[MAXVERTICES],     vtxmet_phi[MAXVERTICES],     vtxsumEt[MAXVERTICES];
  Float_t vtxchmet_pt[MAXVERTICES],   vtxchmet_phi[MAXVERTICES],   vtxchsumEt[MAXVERTICES];
  Float_t vtxneutmet_pt[MAXVERTICES], vtxneutmet_phi[MAXVERTICES], vtxneutsumEt[MAXVERTICES];

  //global pf met
  Float_t met_pt[3],     met_phi[3],     sumEt[3];
  Float_t chmet_pt[3],   chmet_phi[3],   chsumEt[3];
  Float_t neutmet_pt[3], neutmet_phi[3], neutsumEt[3];

  //associated met
  Float_t resmet_pt[3],     resmet_phi[3],     ressumEt[3];
  Float_t chresmet_pt[3],   chresmet_phi[3],   chressumEt[3];
  Float_t neutresmet_pt[3], neutresmet_phi[3], neutressumEt[3];

  //not associated
  Float_t neutnonassocmet_pt[3], neutnonassocmet_phi[3], neutnonassocsumEt[3];

};

class MetSummaryHandler
{
 public:

  MetSummaryHandler(TTree *t, bool create) : t_(t)
    {
      if(t==0) return;
      if(create)
	{
	  t_->Branch("run",              &ev_.run,            "run/I");
	  t_->Branch("lumi",             &ev_.lumi,           "lumi/I");
	  t_->Branch("event",            &ev_.event,          "event/I");
	  
	  //vertex info
	  t_->Branch("nvtx",             &ev_.nvtx,           "nvtx/I");
	  t_->Branch("chi2",             ev_.chi2,           "chi2[nvtx]/F");
	  t_->Branch("sumpt",            ev_.sumpt,          "sumpt[nvtx]/F");
	  t_->Branch("jn",               ev_.jn,             "jn[nvtx]/I");
	  t_->Branch("vtxmet_pt",        ev_.vtxmet_pt,      "vtxmet_pt[nvtx]/F");
	  t_->Branch("vtxmet_phi",       ev_.vtxmet_phi,     "vtxmet_phi[nvtx]/F");
	  t_->Branch("vtxsumEt",         ev_.vtxsumEt,       "vtxsumEt[nvtx]/F");
	  t_->Branch("vtxchmet_pt",      ev_.vtxchmet_pt,    "vtxchmet_pt[nvtx]/F");
	  t_->Branch("vtxchmet_phi",     ev_.vtxchmet_phi,   "vtxchmet_phi[nvtx]/F");
	  t_->Branch("vtxchsumEt",       ev_.vtxchsumEt,     "vtxchsumEt[nvtx]/F");
	  t_->Branch("vtxneutmet_pt",    ev_.vtxneutmet_pt,  "vtxneutmet_pt[nvtx]/F");
	  t_->Branch("vtxneutmet_phi",   ev_.vtxneutmet_phi, "vtxneutmet_phi[nvtx]/F");
	  t_->Branch("vtxneutsumEt",     ev_.vtxneutsumEt,   "vtxneutsumEt[nvtx]/F");

	  //pf-MET
	  t_->Branch("met_pt",        ev_.met_pt,      "met_pt[3]/F");
	  t_->Branch("met_phi",       ev_.met_phi,     "met_phi[3]/F");
	  t_->Branch("sumEt",         ev_.sumEt,       "sumEt[3]/F");
	  t_->Branch("chmet_pt",      ev_.chmet_pt,    "chmet_pt[3]/F");
	  t_->Branch("chmet_phi",     ev_.chmet_phi,   "chmet_phi[3]/F");
	  t_->Branch("chsumEt",       ev_.chsumEt,     "chsumEt[3]/F");
	  t_->Branch("neutmet_pt",    ev_.neutmet_pt,  "neutmet_pt[3]/F");
	  t_->Branch("neutmet_phi",   ev_.neutmet_phi, "neutmet_phi[3]/F");
	  t_->Branch("neutsumEt",     ev_.neutsumEt,   "neutsumEt[3]/F");

	  //residual met
	  t_->Branch("resmet_pt",        ev_.resmet_pt,      "resmet_pt[3]/F");
	  t_->Branch("resmet_phi",       ev_.resmet_phi,     "resmet_phi[3]/F");
	  t_->Branch("ressumEt",         ev_.ressumEt,       "ressumEt[3]/F");
	  t_->Branch("chresmet_pt",      ev_.chresmet_pt,    "chresmet_pt[3]/F");
	  t_->Branch("chresmet_phi",     ev_.chresmet_phi,   "chresmet_phi[3]/F");
	  t_->Branch("chressumEt",       ev_.chressumEt,     "chressumEt[3]/F");
	  t_->Branch("neutresmet_pt",    ev_.neutresmet_pt,  "neutresmet_pt[3]/F");
	  t_->Branch("neutresmet_phi",   ev_.neutresmet_phi, "neutresmet_phi[3]/F");
	  t_->Branch("neutressumEt",     ev_.neutressumEt,   "neutressumEt[3]/F");

	  //non associated
	  t_->Branch("neutnonassocmet_pt",    ev_.neutnonassocmet_pt,  "neutnonassocmet_pt[3]/F");
	  t_->Branch("neutnonassocmet_phi",   ev_.neutnonassocmet_phi, "neutnonassocmet_phi[3]/F");
	  t_->Branch("neutnonassocsumEt",     ev_.neutnonassocsumEt,   "neutnonassocsumEt[3]/F");
	}
      else
	{
	  t_->GetBranch("run")->SetAddress(&ev_.run);
	  t_->GetBranch("lumi")->SetAddress(&ev_.lumi);
	  t_->GetBranch("event")->SetAddress(&ev_.event);
	  
	  //vertexinfo
	  t_->GetBranch("nvtx")->SetAddress(&ev_.nvtx);
	  t_->GetBranch("chi2")->SetAddress(ev_.chi2);
	  t_->GetBranch("sumpt")->SetAddress(ev_.sumpt);
	  t_->GetBranch("jn")->SetAddress(ev_.jn);
	  t_->GetBranch("vtxmet_pt")->SetAddress(ev_.vtxmet_pt);
	  t_->GetBranch("vtxmet_phi")->SetAddress(ev_.vtxmet_phi);
	  t_->GetBranch("vtxsumEt")->SetAddress(ev_.vtxsumEt);
	  t_->GetBranch("vtxchmet_pt")->SetAddress(ev_.vtxchmet_pt);
	  t_->GetBranch("vtxchmet_phi")->SetAddress(ev_.vtxchmet_phi);
	  t_->GetBranch("vtxchsumEt")->SetAddress(ev_.vtxchsumEt);
	  t_->GetBranch("vtxneutmet_pt")->SetAddress(ev_.vtxneutmet_pt);
	  t_->GetBranch("vtxneutmet_phi")->SetAddress(ev_.vtxneutmet_phi);
	  t_->GetBranch("vtxneutsumEt")->SetAddress(ev_.vtxneutsumEt);
	  
	  //pf met
	  t_->GetBranch("met_pt")->SetAddress(ev_.met_pt);
	  t_->GetBranch("met_phi")->SetAddress(ev_.met_phi);
	  t_->GetBranch("sumEt")->SetAddress(ev_.sumEt);
	  t_->GetBranch("chmet_pt")->SetAddress(ev_.chmet_pt);
	  t_->GetBranch("chmet_phi")->SetAddress(ev_.chmet_phi);
	  t_->GetBranch("chsumEt")->SetAddress(ev_.chsumEt);
	  t_->GetBranch("neutmet_pt")->SetAddress(ev_.neutmet_pt);
	  t_->GetBranch("neutmet_phi")->SetAddress(ev_.neutmet_phi);
	  t_->GetBranch("neutsumEt")->SetAddress(ev_.neutsumEt);

	  t_->GetBranch("resmet_pt")->SetAddress(ev_.resmet_pt);
	  t_->GetBranch("resmet_phi")->SetAddress(ev_.resmet_phi);
	  t_->GetBranch("ressumEt")->SetAddress(ev_.ressumEt);
	  t_->GetBranch("chresmet_pt")->SetAddress(ev_.chresmet_pt);
	  t_->GetBranch("chresmet_phi")->SetAddress(ev_.chresmet_phi);
	  t_->GetBranch("chressumEt")->SetAddress(ev_.chressumEt);
	  t_->GetBranch("neutresmet_pt")->SetAddress(ev_.neutresmet_pt);
	  t_->GetBranch("neutresmet_phi")->SetAddress(ev_.neutresmet_phi);
	  t_->GetBranch("neutressumEt")->SetAddress(ev_.neutressumEt);
	  
	  //nonassociated
	  t_->GetBranch("neutnonassocmet_pt")->SetAddress(ev_.neutnonassocmet_pt);
	  t_->GetBranch("neutnonassocmet_phi")->SetAddress(ev_.neutnonassocmet_phi);
	  t_->GetBranch("neutnonassocsumEt")->SetAddress(ev_.neutnonassocsumEt);
	}
      
    }

    void fill() { t_->Fill(); }

    METSummary_t &get(int i=-1) 
      { 
	if(i==-1) return ev_;
	else t_->GetEntry(i); 
	return ev_;
      }

    TTree *getTree() { return t_; }

    ~MetSummaryHandler(){};
    
 private:
    TTree *t_;
    METSummary_t ev_;
};



#endif
