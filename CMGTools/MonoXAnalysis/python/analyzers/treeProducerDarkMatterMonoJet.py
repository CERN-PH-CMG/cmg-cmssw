from CMGTools.MonoXAnalysis.analyzers.treeProducerDarkMatterCore import *
from CMGTools.TTHAnalysis.analyzers.ntupleTypes import *

dmMonoJet_globalVariables = dmCore_globalVariables + [
    ##--------------------------------------------------
    ## Generator information
    ##--------------------------------------------------
    ##    NTupleVariable("genQScale", lambda ev : ev.genQScale, help="Generator level binning quantity, QScale"),
    
    ##--------------------------------------------------
    ## energy sums
    ##--------------------------------------------------
    ##-------- custom jets ------------------------------------------
    NTupleVariable("htJet25", lambda ev : ev.htJet25, help="H_{T} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
    NTupleVariable("mhtJet25", lambda ev : ev.mhtJet25, help="H_{T}^{miss} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
    NTupleVariable("htJet40j", lambda ev : ev.htJet40j, help="H_{T} computed from only jets (with |eta|<2.4, pt > 40 GeV)"),
    NTupleVariable("htJet40ja", lambda ev : ev.htJet40ja, help="H_{T} computed from only jets (with |eta|<4.7, pt > 40 GeV)"),
    NTupleVariable("htJet40", lambda ev : ev.htJet40, help="H_{T} computed from leptons and jets (with |eta|<2.4, pt > 40 GeV)"),
    NTupleVariable("htJet40a", lambda ev : ev.htJet40a, help="H_{T} computed from leptons and jets (with |eta|<4.7, pt > 40 GeV)"),
    NTupleVariable("mhtJet40", lambda ev : ev.mhtJet40, help="H_{T}^{miss} computed from leptons and jets (with |eta|<2.4, pt > 40 GeV)"),
    NTupleVariable("mhtJet40a", lambda ev : ev.mhtJet40a, help="H_{T}^{miss} computed from leptons and jets (with |eta|<4.7, pt > 40 GeV)"),
    ##--------------------------------------------------            
    NTupleVariable("mZ1", lambda ev : ev.bestZ1[0], help="Best m(ll) SF/OS"),
    NTupleVariable("mZ1SFSS", lambda ev : ev.bestZ1sfss[0], help="Best m(ll) SF/SS"),
    NTupleVariable("minMllSFOS", lambda ev: ev.minMllSFOS, help="min m(ll), SF/OS"),
    NTupleVariable("maxMllSFOS", lambda ev: ev.maxMllSFOS, help="max m(ll), SF/OS"),
    NTupleVariable("minMllAFOS", lambda ev: ev.minMllAFOS, help="min m(ll), AF/OS"),
    NTupleVariable("maxMllAFOS", lambda ev: ev.maxMllAFOS, help="max m(ll), AF/OS"),
    NTupleVariable("minMllAFSS", lambda ev: ev.minMllAFSS, help="min m(ll), AF/SS"),
    NTupleVariable("maxMllAFSS", lambda ev: ev.maxMllAFSS, help="max m(ll), AF/SS"),
    NTupleVariable("minMllAFAS", lambda ev: ev.minMllAFAS, help="min m(ll), AF/AS"),
    NTupleVariable("maxMllAFAS", lambda ev: ev.maxMllAFAS, help="max m(ll), AF/AS"),
    NTupleVariable("m2l", lambda ev: ev.m2l, help="m(ll)"),
    
    ##--------------------------------------------------
    # Physics object multplicities
    ##--------------------------------------------------
    NTupleVariable("nBJet40", lambda ev: sum([j.btagWP("CSVv2IVFM") for j in ev.cleanJets if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV medium"),
    NTupleVariable("nMuons10", lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 13 for l in ev.selectedLeptons]), int, help="Number of muons with pt > 10"),
    NTupleVariable("nElectrons10", lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 11 for l in ev.selectedLeptons]), int, help="Number of electrons with pt > 10"),
    NTupleVariable("nTaus20", lambda ev: sum([l.pt() > 20 for l in ev.selectedTaus]), int, help="Number of taus with pt > 20"),
    NTupleVariable("nGammas20", lambda ev: sum([l.pt() > 20 for l in ev.selectedPhotons]), int, help="Number of photons with pt > 20"),


]


dmMonoJet_globalObjects = dmCore_globalObjects.copy()
dmMonoJet_globalObjects.update({
            # put more here
})

dmMonoJet_collections = dmCore_collections.copy()
dmMonoJet_collections.update({
            # put more here
            "gentopquarks"    : NTupleCollection("GenTop",     genParticleType, 2, help="Generated top quarks from hard scattering (needed separately for top pt reweighting)"),
            ##--------------------------------------------------                                                                                                                                   
            "selectedTaus"    : NTupleCollection("TauGood",  tauTypeSusy, 3, help="Taus after the preselection"),
            "selectedLeptons" : NTupleCollection("LepGood",  leptonTypeSusyExtra, 8, help="Leptons after the preselection"),
            "otherLeptons"    : NTupleCollection("LepOther", leptonTypeSusyExtra, 8, help="Leptons after the preselection"),
            ##------------------------------------------------
            "cleanJets"       : NTupleCollection("Jet",     jetTypeSusy, 15, help="Cental jets after full selection and cleaning, sorted by pt"),
            "cleanJetsFwd"    : NTupleCollection("JetFwd",  jetTypeSusy,  6, help="Forward jets after full selection and cleaning, sorted by pt"),            
            "fatJets"         : NTupleCollection("FatJet",  fatJetType,  15, help="AK8 jets, sorted by pt"),
            ##------------------------------------------------
            "discardedJets"    : NTupleCollection("DiscJet", jetTypeSusy, 15, help="Jets discarted in the jet-lepton cleaning"),
            "discardedLeptons" : NTupleCollection("DiscLep", leptonTypeSusyExtra, 8, help="Leptons discarded in the jet-lepton cleaning"),
            ##------------------------------------------------
            #"genBHadrons"  : NTupleCollection("GenBHad", heavyFlavourHadronType, 20, mcOnly=True, help="Gen-level B hadrons"),
            #"genDHadrons"  : NTupleCollection("GenDHad", heavyFlavourHadronType, 20, mcOnly=True, help="Gen-level D hadrons"),
})
        
            
