from PhysicsTools.Heppy.analyzers.core.AutoFillTreeProducer  import * 

dmCore_globalVariables = [
            NTupleVariable("rho",  lambda ev: ev.rho, float, help="kt6PFJets rho"),
            NTupleVariable("nVert",  lambda ev: len(ev.goodVertices), int, help="Number of good vertices"),
            NTupleVariable("nJet25", lambda ev: len(ev.cleanJets), int, help="Number of jets with pt > 25"),
            NTupleVariable("nBJetLoose25", lambda ev: len(ev.bjetsLoose), int, help="Number of jets with pt > 25 passing CSV loose"),
            NTupleVariable("nBJetMedium25", lambda ev: len(ev.bjetsMedium), int, help="Number of jets with pt > 25 passing CSV medium"),
            NTupleVariable("nBJetTight25", lambda ev: sum([j.btagWP("CSVv2IVFT") for j in ev.bjetsMedium]), int, help="Number of jets with pt > 25 passing CSV tight"),
            NTupleVariable("nBJetCMVALoose25", lambda ev: sum([j.btagWP("CMVAL") for j in ev.cleanJets]), int, help="Number of jets with pt > 25 passing CMVA Loose"),
            NTupleVariable("nBJetCMVAMedium25", lambda ev: sum([j.btagWP("CMVAM") for j in ev.cleanJets]), int, help="Number of jets with pt > 25 passing CMVA Medium"),
            NTupleVariable("nBJetCMVATight25", lambda ev: sum([j.btagWP("CMVAT") for j in ev.cleanJets]), int, help="Number of jets with pt > 25 passing CMVA Tight"),
            NTupleVariable("nJet40", lambda ev: sum([j.pt() > 40 for j in ev.cleanJets]), int, help="Number of jets with pt > 40, |eta|<2.4"),
            NTupleVariable("nJet40a", lambda ev: sum([j.pt() > 40 for j in ev.cleanJetsAll]), int, help="Number of jets with pt > 40, |eta|<4.7"),
            NTupleVariable("nBJetLoose40", lambda ev: sum([j.btagWP("CSVv2IVFL") for j in ev.cleanJets if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV loose"),
            NTupleVariable("nBJetMedium40", lambda ev: sum([j.btagWP("CSVv2IVFM") for j in ev.bjetsMedium if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV medium"),
            NTupleVariable("nBJetTight40", lambda ev: sum([j.btagWP("CSVv2IVFT") for j in ev.bjetsMedium if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV tight"),
            NTupleVariable("nBJetCMVALoose40", lambda ev: sum([j.btagWP("CMVAL") for j in ev.cleanJets if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CMVA Loose"),
            NTupleVariable("nBJetCMVAMedium40", lambda ev: sum([j.btagWP("CMVAM") for j in ev.cleanJets if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CMVA Medium"),
            NTupleVariable("nBJetCMVATight40", lambda ev: sum([j.btagWP("CMVAT") for j in ev.cleanJets if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CMVA Tight"),
            ##--------------------------------------------------
            NTupleVariable("nLepGood20", lambda ev: sum([l.pt() > 20 for l in ev.selectedLeptons]), int, help="Number of leptons with pt > 20"),
            NTupleVariable("nLepGood15", lambda ev: sum([l.pt() > 15 for l in ev.selectedLeptons]), int, help="Number of leptons with pt > 15"),
            NTupleVariable("nLepGood10", lambda ev: sum([l.pt() > 10 for l in ev.selectedLeptons]), int, help="Number of leptons with pt > 10"),
            ##--------------------------------------------------
            #NTupleVariable("GenHeaviestQCDFlavour", lambda ev : ev.heaviestQCDFlavour, int, mcOnly=True, help="pdgId of heaviest parton in the event (after shower)"),
            #NTupleVariable("LepEff_1lep", lambda ev : ev.LepEff_1lep, mcOnly=True, help="Lepton preselection SF (1 lep)"),
            #NTupleVariable("LepEff_2lep", lambda ev : ev.LepEff_2lep, mcOnly=True, help="Lepton preselection SF (2 lep)"),
            ##------------------------------------------------
]

dmCore_globalObjects = {
            "met" : NTupleObject("met", metType, help="PF E_{T}^{miss}, after type 1 corrections"),
            "metNoMu" : NTupleObject("metNoMu", metType, help="PF E_{T}^{miss}, with muon pt added back"),
            #"metNoPU" : NTupleObject("metNoPU", fourVectorType, help="PF noPU E_{T}^{miss}"),
}

dmCore_collections = {
            "genleps"         : NTupleCollection("genLep",     genParticleWithLinksType, 10, help="Generated leptons (e/mu) from W/Z decays"),                                                                                                
            "gentauleps"      : NTupleCollection("genLepFromTau", genParticleWithLinksType, 10, help="Generated leptons (e/mu) from decays of taus from W/Z/h decays"),                                                                       
            "gentaus"         : NTupleCollection("genTau",     genParticleWithLinksType, 10, help="Generated leptons (tau) from W/Z decays"),                            
            "generatorSummary" : NTupleCollection("GenPart", genParticleWithLinksType, 100 , help="Hard scattering particles, with ancestry and links"),
}
