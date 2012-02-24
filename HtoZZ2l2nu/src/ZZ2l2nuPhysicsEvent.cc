#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"

using namespace std;

bool JetPtOrdering (PhysicsObject_Jet i,PhysicsObject_Jet j) { return (i.pt()>j.pt()); }

//
PhysicsEvent_t getPhysicsEventFrom(ZZ2l2nuSummary_t &ev)
{
  
  PhysicsEvent_t phys;
  phys.puWeight=ev.puWeight;
  phys.cat = ev.cat;
  phys.nvtx = ev.nvtx;
  phys.vtx = LorentzVector(ev.vtx_px,ev.vtx_py,ev.vtx_pz,ev.vtx_en);
  
  phys.leptons.push_back(PhysicsObject_Lepton(LorentzVector(ev.l1_px,ev.l1_py,ev.l1_pz,ev.l1_en), ev.l1_id, ev.l1_genid, ev.l1_ptErr, ev.l1_ecalIso, ev.l1_hcalIso, ev.l1_trkIso, ev.l1_gIso, ev.l1_chIso, ev.l1_puchIso, ev.l1_nhIso, ev.l1_passIso, ev.l1_pid) );
  phys.leptons.push_back(PhysicsObject_Lepton(LorentzVector(ev.l2_px,ev.l2_py,ev.l2_pz,ev.l2_en), ev.l2_id, ev.l2_genid, ev.l2_ptErr, ev.l2_ecalIso, ev.l2_hcalIso, ev.l2_trkIso, ev.l2_gIso, ev.l2_chIso, ev.l2_puchIso, ev.l1_nhIso, ev.l2_passIso, ev.l2_pid) );
  for(Int_t i=0;i<ev.ln;i++){
    phys.leptons.push_back(PhysicsObject_Lepton(LorentzVector(ev.ln_px[i],ev.ln_py[i],ev.ln_pz[i],ev.ln_en[i]), ev.ln_id[i], ev.ln_genid[i], ev.ln_ptErr[i], ev.ln_ecalIso[i], ev.ln_hcalIso[i], ev.ln_trkIso[i], ev.ln_gIso[i], ev.ln_chIso[i], ev.ln_puchIso[i], ev.ln_nhIso[i], ev.ln_passIso[i], ev.ln_pid[i]) );
  }
  
  for(Int_t i=0;i<ev.jn;i++){
    phys.jets.push_back(PhysicsObject_Jet(LorentzVector(ev.jn_px[i],ev.jn_py[i],ev.jn_pz[i],ev.jn_en[i]), ev.jn_genid[i], ev.jn_btag1[i], ev.jn_btag2[i], ev.jn_neutHadFrac[i], ev.jn_neutEmFrac[i], ev.jn_chHadFrac[i], ev.jn_pid[i]));
  }

  for(Int_t i=0;i<ev.ajn;i++){
    phys.ajets.push_back(PhysicsObject_Jet(LorentzVector(ev.ajn_px[i],ev.ajn_py[i],ev.ajn_pz[i],ev.ajn_en[i]), ev.ajn_genid[i], ev.ajn_btag1[i], ev.ajn_btag2[i], ev.ajn_neutHadFrac[i], ev.ajn_neutEmFrac[i], ev.ajn_chHadFrac[i], ev.ajn_pid[i]));
  }
//  //order the jet vector
//  std::sort(phys.ajets.begin(), phys.ajets.end(), JetPtOrdering);
  
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


