#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"

using namespace std;

//
PhysicsEvent_t getPhysicsEventFrom(ZZ2l2nuSummary_t &ev)
{
  
  PhysicsEvent_t phys;
  phys.puWeight=ev.puWeight;
  phys.cat = ev.cat;
  phys.nvtx = ev.nvtx;
  phys.vtx = LorentzVector(ev.vtx_px,ev.vtx_py,ev.vtx_pz,ev.vtx_en);
  
  phys.leptons.push_back(PhysicsObject_Lepton(LorentzVector(ev.l1_px,ev.l1_py,ev.l1_pz,ev.l1_en), ev.l1_id, ev.l1_genid, ev.l1_ptErr, ev.l1_iso1, ev.l1_iso2, ev.l1_iso3, ev.l1_iso4, ev.l1_pid) );
  phys.leptons.push_back(PhysicsObject_Lepton(LorentzVector(ev.l2_px,ev.l2_py,ev.l2_pz,ev.l2_en), ev.l2_id, ev.l2_genid, ev.l2_ptErr, ev.l2_iso1, ev.l2_iso2, ev.l2_iso3, ev.l1_iso4, ev.l2_pid) );
  for(Int_t i=0;i<ev.ln;i++){
    phys.leptons.push_back(PhysicsObject_Lepton(LorentzVector(ev.ln_px[i],ev.ln_py[i],ev.ln_pz[i],ev.ln_en[i]), ev.ln_id[i], ev.ln_genid[i], ev.ln_ptErr[i], ev.ln_iso1[i], ev.ln_iso2[i], ev.ln_iso3[i], ev.ln_iso4[i], ev.ln_pid[i]) );
  }
  
  for(Int_t i=0;i<ev.jn;i++){
    phys.jets.push_back(PhysicsObject_Jet(LorentzVector(ev.jn_px[i],ev.jn_py[i],ev.jn_pz[i],ev.jn_en[i]), ev.jn_genid[i], ev.jn_btag1[i], ev.jn_btag2[i], ev.jn_neutHadFrac[i], ev.jn_neutEmFrac[i], ev.jn_chHadFrac[i], ev.jn_pid[i]));
  }

  for(Int_t i=0;i<ev.ajn;i++){
    phys.jets.push_back(PhysicsObject_Jet(LorentzVector(ev.ajn_px[i],ev.ajn_py[i],ev.ajn_pz[i],ev.ajn_en[i]), ev.ajn_genid[i], ev.ajn_btag1[i], ev.ajn_btag2[i], ev.ajn_neutHadFrac[i], ev.ajn_neutEmFrac[i], ev.ajn_chHadFrac[i], ev.ajn_pid[i]));
  }
  
  for(Int_t i=0; i<ev.nmet; i++){
    phys.met.push_back(  LorentzVector( ev.met_pt[i]*cos(ev.met_phi[i]), ev.met_pt[i]*sin(ev.met_phi[i]), 0, ev.met_pt[i]) );
  }

  for(Int_t ipart=0; ipart<ev.gn; ipart++){
    PhysicsObject_Gamma igamma(LorentzVector(ev.g_px[ipart],ev.g_py[ipart],ev.g_pz[ipart],ev.g_en[ipart]), 
			       0., 
			       ev.g_iso1[ipart], ev.g_iso2[ipart], ev.g_iso3[ipart], 
			       ev.g_sihih[ipart], ev.g_r9[ipart], ev.g_hoe[ipart]);
    igamma.setConversionInfo(ev.g_conv[ipart],LorentzVector(ev.g_conv_px[ipart],ev.g_conv_py[ipart],ev.g_conv_pz[ipart],ev.g_conv_en[ipart]));
    phys.gammas.push_back( igamma );
  }

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


