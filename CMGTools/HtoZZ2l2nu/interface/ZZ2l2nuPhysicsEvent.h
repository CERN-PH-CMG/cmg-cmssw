#ifndef zz2l2nuphysicsevent_h
#define zz2l2nuphysicsevent_h

#include <vector>

#include "Math/LorentzVector.h"
#include "TVector2.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

class PhysicsObject : public LorentzVector
{
public :
  PhysicsObject(LorentzVector vec, Int_t id_):
    LorentzVector(vec), id(id_){ }
    Int_t id;
};


//
class PhysicsObject_Lepton : public LorentzVector
{
 public :
  PhysicsObject_Lepton(LorentzVector vec, Int_t id_,Int_t genid_=0, Float_t ptErr_=0, 
		       Float_t ecalIso_=0, Float_t hcalIso_=0, Float_t trkIso_=0, 
		       Float_t gIso_=0,     Float_t chIso_=0,    Float_t puchIso_=0, Float_t nhIso_=0, Int_t passIso_=0,
		       Float_t pid_=0):
    LorentzVector(vec), id(id_), genid(genid_), ptErr(ptErr_), 
    ecalIso(ecalIso_), hcalIso(hcalIso_), trkIso(trkIso_), gIso(gIso_), chIso(chIso_), puchIso(puchIso_), nhIso(nhIso_),  passIso(passIso_),
    pid(pid_) { }

    inline bool passesIso(int isoType=PFRELBETCORR_ISO)
    {
      bool pass(false);
      switch(isoType)
	{
	case REL_ISO:          pass=(passIso & 0x1); break;
	case RELRHOCORR_ISO:   pass=((passIso>>1) & 0x1); break;
	case PFREL_ISO:        pass=((passIso>>2) & 0x1); break;
	default:               pass=((passIso>>3) & 0x1); break;
	}
      return pass;
    }

    Int_t id,genid;
    Float_t ptErr, ecalIso, hcalIso, trkIso, gIso, chIso, puchIso, nhIso;
    Int_t passIso, pid;
};


class PhysicsObject_Jet : public LorentzVector
{
public :
  PhysicsObject_Jet(LorentzVector vec, Int_t genid_=0, Float_t btag1_=0, Float_t btag2_=0, Float_t neutHadFrac_=0, Float_t neutEmFrac_=0, Float_t chHadFrac_=0, Int_t pid_=0):
    LorentzVector(vec), genid(genid_), btag1(btag1_), btag2(btag2_), neutHadFrac(neutHadFrac_), neutEmFrac(neutEmFrac_), chHadFrac(chHadFrac_), pid(pid_) 
    {
      genPt=0;
      pumva=-9999.;
    }
    void setGenPt(float val) { genPt=val; }
    void setPUmva(float val) { pumva=val; }
    Int_t genid;
    Float_t btag1, btag2, neutHadFrac, neutEmFrac, chHadFrac, genPt,pumva;
    Int_t pid;
};
bool JetPtOrdering (PhysicsObject_Jet i,PhysicsObject_Jet j);


class PhysicsObject_Gamma : public LorentzVector
{ 
public : 
  PhysicsObject_Gamma(LorentzVector vec, Float_t ptErr_=0, Float_t iso1_=0, Float_t iso2_=0, Float_t iso3_=0, Float_t sihih_=0, Float_t r9_=0, Float_t hoe_=0):
    LorentzVector(vec), iso1(iso1_), iso2(iso2_), iso3(iso3_), sihih(sihih_), r9(r9_), hoe(hoe_) 
    { 
      setConversionInfo(false,LorentzVector(0,0,0,0));
      hasCtfTrkVeto=false;
      hasGsfTrkVeto=false;
      hasElectronVeto=false;
    }
    inline void setConversionInfo(bool isConv_, LorentzVector convP4_)
    {
      isConv=isConv_;
      convP4=convP4_;
    }
    inline void setTrackVeto(int trkVeto)
      {
	hasCtfTrkVeto   = (trkVeto & 0x1);
	hasGsfTrkVeto   = ((trkVeto>>1) & 0x1);
	hasElectronVeto = ((trkVeto>>2) & 0x1);
      }
    Bool_t hasCtfTrkVeto,hasGsfTrkVeto,hasElectronVeto;
    Bool_t isConv;
    LorentzVector convP4;
    Float_t iso1, iso2, iso3, sihih, r9, hoe;
};

typedef std::vector<PhysicsObject>        PhysicsObjectCollection;
typedef std::vector<PhysicsObject_Lepton> PhysicsObjectLeptonCollection;
typedef std::vector<PhysicsObject_Jet>    PhysicsObjectJetCollection;
typedef std::vector<PhysicsObject_Gamma>  PhysicsObjectGammaCollection;

//
struct PhysicsEvent_t
{
  float puWeight;
  int cat;
  int nvtx;
  LorentzVectorCollection met;
  LorentzVector vtx;
  PhysicsObjectGammaCollection gammas;
  PhysicsObjectJetCollection jets, ajets;
  PhysicsObjectLeptonCollection leptons;

  PhysicsObjectCollection genjets, genleptons, genmet, genhiggs, gengammas;
};

//
PhysicsEvent_t getPhysicsEventFrom(ZZ2l2nuSummary_t &ev);
std::pair<int,int> getWZdecayMode(ZZ2l2nuSummary_t &ev);
int getNgenLeptons(int mcChannelCode, int pdgId);
int getGenProcess(int mcChannelCode);
bool isDYToLL(int mcChannelCode);
bool isDYToTauTau(int mcChannelCode);
bool isZZ2l2nu(int mcChannelCode);
bool isW(int mcChannelCode);
bool isWW(int mcChannelCode);

#endif
