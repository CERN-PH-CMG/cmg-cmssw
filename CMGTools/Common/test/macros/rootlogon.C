
void rootlogon() {
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gSystem->Load("libPhysics");
  gSystem->Load("libEG");
  gStyle->SetHistMinimumZero(kTRUE);
}

void loadFWLite() {
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();
  gSystem->Load("libCintex.so");
  ROOT::Cintex::Cintex::Enable();
}

void initAOD(const char* process, TTree* tree) {

  string verticesAod = "recoVertexs_offlinePrimaryVerticesWithBS__"; 
  verticesAod += process;

  string pfCandidatesAod = "recoPFCandidates_particleFlow__"; 
  pfCandidatesAod += process;

  string ic5GenJetsAod = "recoGenJets_iterativeCone5GenJets__";  
  ic5GenJetsAod += process;

  string pfJetsAod = "recoPFJets_iterativeCone5PFJets__";  
  pfJetsAod += process;

  tree->SetAlias("verticesAod", verticesAod.c_str()); 
  tree->SetAlias("pfCandidatesAod",  pfCandidatesAod.c_str());
  tree->SetAlias("ic5GenJetsAod",  ic5GenJetsAod.c_str());
  tree->SetAlias("pfJetsAod",  pfJetsAod.c_str());

}

void initPF2PAT(const char* process, TTree* tree) {


  string met = "recoMETs_pfMET__"; met += process;
  string pu = "recoPileUpPFCandidates_pfPileUp__";  pu+= process;
  string jetsin = "recoPFJets_pfJets__"; jetsin += process;
  string jetsout = "recoPFJets_pfNoTau__"; jetsout += process;
  string taus = "recoPFTaus_allLayer0Taus__"; taus += process;
  string muons = "recoPFCandidates_pfIsolatedMuons__"; muons += process;
  string electrons = "recoPFCandidates_pfIsolatedElectrons__"; electrons += process;
  string pfcandout = "recoPFCandidates_pfNoJet__"; pfcandout += process;  
  string noPileUp = "recoPFCandidates_pfNoPileUp__"; noPileUp += process;  


  string genMetTrue = "recoGenMETs_genMetTrue__";
  genMetTrue += process;
  string decaysFromZs = "recoGenParticles_decaysFromZs__";
  decaysFromZs += process;

  tree->SetAlias("met", met.c_str() );
  tree->SetAlias("pileUp", pu.c_str() );
  tree->SetAlias("jetsAll", jetsin.c_str() );
  tree->SetAlias("jets", jetsout.c_str() );
  tree->SetAlias("taus", taus.c_str());
  tree->SetAlias("muons", muons.c_str());
  tree->SetAlias("electrons", electrons.c_str());
  tree->SetAlias("pfCandOut", pfcandout.c_str());
  tree->SetAlias("noPileUp", noPileUp.c_str());

  tree->SetAlias("genmet",  genMetTrue.c_str());
  tree->SetAlias("decaysFromZs",  decaysFromZs.c_str());
}


void initPAT(const char* process, TTree* tree) {
  string taus = "patTaus_selectedPatTausPFlow__"; taus += process;
  string jets = "patJets_selectedPatJetsPFlow__"; jets += process;
  string met = "patMETs_patMETsPFlow__";  met+= process;
  string mus = "patMuons_selectedPatMuonsPFlow__"; mus += process;
  string eles = "patElectrons_selectedPatElectronsPFlow__"; eles += process;
  
  string patCaloJets = "patJets_selectedPatJets__"; patCaloJets += process;
  string patCaloMet = "patMETs_patMETs__"; patCaloMet += process;

  tree->SetAlias("patTaus", taus.c_str() );
  tree->SetAlias("patJets", jets.c_str() );
  tree->SetAlias("patCaloJets", patCaloJets.c_str() );
  tree->SetAlias("patMET", met.c_str() );
  tree->SetAlias("patCaloMET", patCaloMet.c_str() );
  tree->SetAlias("patMuons", mus.c_str() );
  tree->SetAlias("patElectrons", eles.c_str() );
}

void initCMG(const char* process, TTree* tree) {
  string pfJets = "cmgPFJets_cmgPFJet__"; pfJets += process;
  string pfBaseJets = "cmgBaseJets_cmgPFBaseJet__"; pfBaseJets += process;
  string mhtPFJets = "cmgBaseMETs_cmgMHTPFJets__"; mhtPFJets += process;
  string metPF = "cmgBaseMETs_cmgMETPFCandidates__"; metPF += process;

  string caloBaseJets = "cmgBaseJets_cmgCaloBaseJet__"; caloBaseJets += process;
  string mhtCaloJets = "cmgBaseMETs_cmgMHTCaloJets__"; mhtCaloJets += process;
  
  tree->SetAlias("cmgPFJets", pfJets.c_str() );
  tree->SetAlias("cmgPFBaseJets", pfBaseJets.c_str() );
  tree->SetAlias("cmgPFMHT", mhtPFJets.c_str() );
  tree->SetAlias("cmgPFMET", metPF.c_str() );
  
  tree->SetAlias("cmgCaloBaseJets", caloBaseJets.c_str() );
  tree->SetAlias("cmgCaloMHT", mhtCaloJets.c_str() );
}

