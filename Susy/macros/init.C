{
  loadFWLite();
  
  // TFile f("gen.root");
  TFile f("PF2PATonGEN.root");
  
  Events.SetAlias("pfCands", "recoPFCandidates_particleFlow__PAT");
  Events.SetAlias("pfGens", "recoGenParticles_particleFlow_GEN_PAT");
  Events.SetAlias("metTrue", "recoGenMETs_genMetTrue__PAT");
  Events.SetAlias("metTrueEt", "recoGenMETs_genMetTrueEt__PAT");
}
