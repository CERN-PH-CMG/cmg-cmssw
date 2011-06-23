#ifndef zz2l2nuphysicsevent_h
#define zz2l2nuphysicsevent_h

#include <vector>

#include "Math/LorentzVector.h"
#include "TVector2.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

//
class PhysicsObject : public LorentzVector
{
public :
  PhysicsObject(LorentzVector vec, Int_t id,Int_t mcid=0):
    LorentzVector(vec), id_(id), mcid_(mcid) { }
    Int_t id_,mcid_;
    std::vector<Float_t> info;
 private:
};

//
typedef std::vector<PhysicsObject> PhysicsObjectCollection;

//
struct PhysicsEvent_t
{
  PhysicsObjectCollection jets, leptons, met, vtx;
  PhysicsObjectCollection genjets, genleptons, genmet, genhiggs;
};

//
PhysicsEvent_t getPhysicsEventFrom(ZZ2l2nuSummary_t &ev)
{
  
  PhysicsEvent_t phys;
  //get the physics object
  for(Int_t ipart=0; ipart<ev.nparticles; ipart++)
    {
      PhysicsObject obj( LorentzVector(ev.px[ipart],ev.py[ipart],ev.pz[ipart],ev.en[ipart]) , ev.id[ipart], ev.genid[ipart] );
      obj.info.push_back(ev.info1[ipart]);
      obj.info.push_back(ev.info2[ipart]);
      obj.info.push_back(ev.info3[ipart]);
      obj.info.push_back(ev.info4[ipart]);
      obj.info.push_back(ev.info5[ipart]);
      switch( ev.id[ipart] )
	{
	case 0:
	  phys.met.push_back( obj );
	  break;
	case 1:
	  phys.jets.push_back( obj );
	  break;
	case 11: case -11: case 13: case -13:
	  phys.leptons.push_back( obj );
	  break;
	case 500:
	  phys.vtx.push_back( obj );
	  break;
	}
    }

  //similar for the mc truth
  LorentzVector genmet(0,0,0,0);
  for(Int_t ipart=0; ipart<ev.nmcparticles; ipart++)
    {
      LorentzVector p4(ev.mcpx[ipart],ev.mcpy[ipart],ev.mcpz[ipart],ev.mcen[ipart]);
      switch( ev.mcid[ipart] )
	{
	case 12: case -12: case 14: case -14: case 16: case -16:
	  genmet += p4;
	  break;
	case 1:
	  phys.genjets.push_back( PhysicsObject(p4,ev.mcid[ipart],ev.mcid[ipart]) );
	  break;
	case 11: case -11: case 13: case -13: case 15: case -15:
	  phys.genleptons.push_back( PhysicsObject(p4,ev.mcid[ipart],ev.mcid[ipart]) );
	  break;
	case 25:
	  phys.genhiggs.push_back( PhysicsObject(p4,ev.mcid[ipart],ev.mcid[ipart]) );
	  break;
	}
    }
  phys.genmet.push_back( PhysicsObject(genmet,0,0) );

  return phys;
}



#endif
