#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"

PhysicsEvent_t getPhysicsEventFrom(ZZ2l2nuSummary_t &ev)
{
  
  PhysicsEvent_t phys;
  phys.puWeight=ev.puWeight;
  phys.cat = ev.cat;
  phys.nvtx = ev.nvtx;
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
  phys.met[7] = LorentzVector( ev.met8_pt*cos(ev.met8_phi), ev.met8_pt*sin(ev.met8_phi), 0, ev.met8_pt);
  phys.met[8]  = LorentzVector( ev.met9_pt *cos(ev.met9_phi ), ev.met9_pt *sin(ev.met9_phi ), 0, ev.met9_pt );
  phys.met[9]  = LorentzVector( ev.met10_pt*cos(ev.met10_phi), ev.met10_pt*sin(ev.met10_phi), 0, ev.met10_pt);
  phys.met[10] = LorentzVector( ev.met11_pt*cos(ev.met11_phi), ev.met11_pt*sin(ev.met11_phi), 0, ev.met11_pt);
  phys.met[11] = LorentzVector( ev.met12_pt*cos(ev.met12_phi), ev.met12_pt*sin(ev.met12_phi), 0, ev.met12_pt);
  phys.met[12] = LorentzVector( ev.met13_pt*cos(ev.met13_phi), ev.met13_pt*sin(ev.met13_phi), 0, ev.met13_pt);
  phys.met[13] = LorentzVector( ev.met14_pt*cos(ev.met14_phi), ev.met14_pt*sin(ev.met14_phi), 0, ev.met14_pt);
  phys.met[14] = LorentzVector( ev.met15_pt*cos(ev.met15_phi), ev.met15_pt*sin(ev.met15_phi), 0, ev.met15_pt);


  /////////////////////////////////////////   TEMPORARY MOVING THIS CODE HERE, TO SWAP BACK THE SUM ET QUANTITIES IN THE RIGHT ORDER (NEED TO REDO THE NTUPLE OTHERWISE)
  //   float sumEtcentral     = ev.chsumEtcentral;
  //   float chSumEtcentral   = ev.neutsumEt;
  //   float chSumEt          = ev.sumEtcentral;
  //   float neutsumEt        = ev.chsumEt;
  
  //   ev.sumEtcentral        = sumEtcentral;
  //   ev.chsumEtcentral      = chSumEtcentral;
  //   ev.chsumEt             = chSumEt;
  //   ev.neutsumEt           = neutsumEt;
  ////////////////////////////////////////////

  //gamams
  for(Int_t ipart=0; ipart<ev.gn; ipart++)
    {
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


