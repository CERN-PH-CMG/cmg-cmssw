#ifndef zz2l2nuphysicsevent_h
#define zz2l2nuphysicsevent_h

#include <vector>

#include "Math/LorentzVector.h"
#include "TVector2.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"

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
  PhysicsObject_Lepton(LorentzVector vec, Int_t id_,Int_t genid_=0, Float_t ptErr_=0, Float_t iso1_=0, Float_t iso2_=0, Float_t iso3_=0):
    LorentzVector(vec), id(id_), genid(genid_), ptErr(ptErr_), iso1(iso1_), iso2(iso2_), iso3(iso3_) { }
    Int_t id,genid;
    Float_t ptErr, iso1, iso2, iso3;
};

class PhysicsObject_Jet : public LorentzVector
{
public :
  PhysicsObject_Jet(LorentzVector vec, Int_t genid_=0, Float_t btag1_=0, Float_t btag2_=0, Float_t btag3_=0, Float_t btag4_=0, Bool_t vtxAssoc_=false):
    LorentzVector(vec), genid(genid_), btag1(btag1_), btag2(btag2_), btag3(btag3_), btag4(btag4_), vtxAssoc(vtxAssoc_) { }
    Int_t genid;
    Float_t btag1, btag2, btag3, btag4; 
    Bool_t vtxAssoc;
};

class PhysicsObject_Gamma : public LorentzVector
{ 
public : 
  PhysicsObject_Gamma(LorentzVector vec, Float_t ptErr_=0, Float_t iso1_=0, Float_t iso2_=0, Float_t iso3_=0, Float_t sihih_=0, Float_t sipip_=0, Float_t r9_=0, Float_t hoe_=0, Float_t eop_=0):
    LorentzVector(vec), iso1(iso1_), iso2(iso2_), iso3(iso3_), sihih(sihih_), sipip(sipip_), r9(r9_), hoe(hoe_), eop(eop_) { }
    Float_t iso1, iso2, iso3, sihih, sipip, r9, hoe, eop;
};

typedef std::vector<PhysicsObject>        PhysicsObjectCollection;
typedef std::vector<PhysicsObject_Lepton> PhysicsObjectLeptonCollection;
typedef std::vector<PhysicsObject_Jet>    PhysicsObjectJetCollection;
typedef std::vector<PhysicsObject_Gamma>  PhysicsObjectGammaCollection;

//
struct PhysicsEvent_t
{
  LorentzVector met[12];
  LorentzVector vtx;
  LorentzVector gamma;
  PhysicsObjectJetCollection jets;
  PhysicsObjectLeptonCollection leptons;

  PhysicsObjectCollection genjets, genleptons, genmet, genhiggs, gengammas;
};

//
PhysicsEvent_t getPhysicsEventFrom(ZZ2l2nuSummary_t &ev);

#endif
