#ifndef zz2l2nuphysicsevent_h
#define zz2l2nuphysicsevent_h

#include <vector>

#include "Math/LorentzVector.h"
#include "TVector2.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;


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
  LorentzVector met[7];
  LorentzVector vtx;
  LorentzVector gamma;
  PhysicsObjectJetCollection jets;
  PhysicsObjectLeptonCollection leptons;

  PhysicsObjectCollection genjets, genleptons, genmet, genhiggs, gengammas;
};

//
PhysicsEvent_t getPhysicsEventFrom(ZZ2l2nuSummary_t &ev)
{
  
  PhysicsEvent_t phys;


  phys.leptons.push_back(PhysicsObject_Lepton(LorentzVector(ev.l1_px,ev.l1_py,ev.l1_pz,ev.l1_en), ev.l1_id, ev.l1_genid, ev.l1_ptErr, ev.l1_iso1, ev.l1_iso2, ev.l1_iso3) );
  phys.leptons.push_back(PhysicsObject_Lepton(LorentzVector(ev.l2_px,ev.l2_py,ev.l2_pz,ev.l2_en), ev.l2_id, ev.l2_genid, ev.l2_ptErr, ev.l2_iso1, ev.l2_iso2, ev.l2_iso3) );
  for(Int_t i=0;i<ev.ln;i++){
  phys.leptons.push_back(PhysicsObject_Lepton(LorentzVector(ev.ln_px[i],ev.ln_py[i],ev.ln_pz[i],ev.ln_en[i]), ev.ln_id[i], ev.ln_genid[i], ev.ln_ptErr[i], ev.ln_iso1[i], ev.ln_iso2[i], ev.ln_iso3[i]) );
  }

  for(Int_t i=0;i<ev.jn;i++){
  phys.jets.push_back(PhysicsObject_Jet(LorentzVector(ev.jn_px[i],ev.jn_py[i],ev.jn_pz[i],ev.jn_en[i]), ev.jn_genid[i], ev.jn_btag1[i], ev.jn_btag2[i], ev.jn_btag3[i], ev.jn_btag4[i], ev.jn_vtxAssoc) );
  }

  phys.vtx = LorentzVector(ev.vtx_px,ev.vtx_py,ev.vtx_pz,ev.vtx_en);

  phys.met[0] = LorentzVector( ev.met1_pt*cos(ev.met1_phi), ev.met1_pt*sin(ev.met1_phi), 0, ev.met1_pt);
  phys.met[1] = LorentzVector( ev.met2_pt*cos(ev.met2_phi), ev.met2_pt*sin(ev.met2_phi), 0, ev.met2_pt);
  phys.met[2] = LorentzVector( ev.met3_pt*cos(ev.met3_phi), ev.met3_pt*sin(ev.met3_phi), 0, ev.met3_pt);
  phys.met[3] = LorentzVector( ev.met4_pt*cos(ev.met4_phi), ev.met4_pt*sin(ev.met4_phi), 0, ev.met4_pt);
  phys.met[4] = LorentzVector( ev.met5_pt*cos(ev.met5_phi), ev.met5_pt*sin(ev.met5_phi), 0, ev.met5_pt);
  phys.met[5] = LorentzVector( ev.met6_pt*cos(ev.met6_phi), ev.met6_pt*sin(ev.met6_phi), 0, ev.met6_pt);
  phys.met[6] = LorentzVector( ev.met7_pt*cos(ev.met7_phi), ev.met7_pt*sin(ev.met7_phi), 0, ev.met7_pt);

  phys.gamma = PhysicsObject_Gamma(LorentzVector(ev.g_px,ev.g_py,ev.g_pz,ev.g_en), ev.g_iso1, ev.g_iso2, ev.g_iso3, ev.g_sihih, ev.g_sipip, ev.g_r9, ev.g_hoe, ev.g_eop);
 
  //similar for the mc truth
  LorentzVector hp4(ev.h_px, ev.h_py, ev.h_pz, ev.h_en);
  phys.genhiggs.push_back( PhysicsObject(hp4,25) );

  LorentzVector genmet(0,0,0,0);
  for(Int_t ipart=0; ipart<ev.nmcparticles; ipart++)
    {
      LorentzVector p4(ev.mc_px[ipart],ev.mc_py[ipart],ev.mc_pz[ipart],ev.mc_en[ipart]);
      switch( ev.mc_id[ipart] )
	{
	case 12: case -12: case 14: case -14: case 16: case -16:
	  genmet += p4;
	  break;
	case 1:
	  phys.genjets.push_back( PhysicsObject(p4,ev.mc_id[ipart]) );
	  break;
	case 22:
	  phys.gengammas.push_back( PhysicsObject(p4,ev.mc_id[ipart]) );
	  break;
	case 11: case -11: case 13: case -13: case 15: case -15:
	  phys.genleptons.push_back( PhysicsObject(p4,ev.mc_id[ipart]) );
	  break;
	}
    }
  phys.genmet.push_back( PhysicsObject(genmet,0) );

  return phys;
}



#endif
