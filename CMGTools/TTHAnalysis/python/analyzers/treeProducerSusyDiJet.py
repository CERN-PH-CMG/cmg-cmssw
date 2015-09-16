from CMGTools.TTHAnalysis.analyzers.treeProducerSusyCore import *
from CMGTools.TTHAnalysis.analyzers.ntupleTypes import *

susyDiJet_globalVariables = susyCore_globalVariables + [
    ##--------------------------------------------------
    ## Generator information
    ##--------------------------------------------------
##    NTupleVariable("genQScale", lambda ev : ev.genQScale, help="Generator level binning quantity, QScale"),
    NTupleVariable("LHEweight_original", lambda ev: ev.LHE_originalWeight if  hasattr(ev,'LHE_originalWeight') else  0, mcOnly=True, help="original LHE weight"),
    
    ##--------------------------------------------------

    NTupleVariable("ht", lambda ev : ev.htJetXj10l5t, help="H_{T} computed from jets (with |eta|<2.5, pt > 40 GeV) and leptons (electrons and muons with |eta|<2.5, pt > 10 GeV)"),
    NTupleVariable("mht_pt", lambda ev : ev.mhtJetXj10l5t, help="H_{T}^{miss} computed from jets (with |eta|<2.5, pt > 40 GeV) and leptons (electrons ans muons with |eta|<2.5, pt > 10 GeV)"),
    NTupleVariable("mht_phi", lambda ev : ev.mhtPhiJetXj10l5t, help="H_{T}^{miss} #phi computed from jets (with |eta|<2.5, pt > 40 GeV) and leptons (electrons ans muons with |eta|<2.5, pt > 10 GeV)"),
    NTupleVariable("diffMetMht", lambda ev : ev.diffMetMht_Xj, help="abs( vec(mht) - vec(met) ) - with jets and leptons"),
    NTupleVariable("deltaPhiMin", lambda ev : ev.deltaPhiMin_Xj, help="minimal deltaPhi between the MET and the four leading jets with pt>40 and eta<2.4 and leptons (electrons ans muons with |eta|<2.5, pt > 10 GeV)"),
    NTupleVariable("jet1_pt", lambda ev : ev.cleanJets[0].pt() if len(ev.cleanJets)>0 else -99, help="pt of leading central jet"),
    NTupleVariable("jet2_pt", lambda ev : ev.cleanJets[1].pt() if len(ev.cleanJets)>1 else -99, help="pt of second central jet"),
    
    ### for now store the hadronic only
    NTupleVariable("ht_had", lambda ev : ev.htJetXj, help="H_{T} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
    NTupleVariable("mht_had_pt", lambda ev : ev.mhtJetXj, help="H_{T}^{miss} computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
    NTupleVariable("mht_had_phi", lambda ev : ev.mhtPhiJetXj, help="H_{T}^{miss} #phi computed from only jets (with |eta|<2.5, pt > 40 GeV)"),
    NTupleVariable("diffMetMht_had", lambda ev : ev.diffMetMht_Xj_had, help="abs( vec(mht) - vec(met) ) - only jets"),
    NTupleVariable("deltaPhiMin_had", lambda ev : ev.deltaPhiMin_Xj_had, help="minimal deltaPhi between the MET and the four leading jets with pt>40 and eta<2.4"),

    ##--------------------------------------------------
    # Met definitions
    ##--------------------------------------------------

    NTupleVariable("met_rawPt", lambda ev : ev.met.uncorPt(), help="raw met p_{T}"),
    NTupleVariable("met_rawPhi", lambda ev : ev.met.uncorPhi(), help="raw met phi"),
    NTupleVariable("met_rawSumEt", lambda ev : ev.met.uncorSumEt(), help="raw met sumEt"),

    NTupleVariable("met_caloPt", lambda ev : ev.met.caloMETPt(), help="calo met p_{T}"),
    NTupleVariable("met_caloPhi", lambda ev : ev.met.caloMETPhi(), help="calo met phi"),
    NTupleVariable("met_caloSumEt", lambda ev : ev.met.caloMETSumEt(), help="calo met sumEt"),

    NTupleVariable("met_trkPt", lambda ev : ev.tkMet.pt() if  hasattr(ev,'tkMet') else  0, help="tkmet p_{T}"),
    NTupleVariable("met_trkPhi", lambda ev : ev.tkMet.phi() if  hasattr(ev,'tkMet') else  0, help="tkmet phi"),
    NTupleVariable("met_trkSumEt", lambda ev : ev.tkMet.sumEt if  hasattr(ev,'tkMet') else  0 , help="TK sumEt charged dz<0.1 pt"),

    ##--------------------------------------------------
    # Physics object multplicities
    ##--------------------------------------------------
    
    NTupleVariable("nJet20", lambda ev: sum([j.pt() > 20 for j in ev.cleanJets]), int, help="Number of jets with pt > 20, |eta|<2.4"),
    NTupleVariable("nJet20a", lambda ev: sum([j.pt() > 20 for j in ev.cleanJetsAll]), int, help="Number of jets with pt > 20, |eta|<4.7"),
    NTupleVariable("nBJetLoose20", lambda ev: sum([j.btagWP("CSVv2IVFL") for j in ev.cleanJets if j.pt() > 20]), int, help="Number of jets with pt > 20 passing CSV loose"),
    NTupleVariable("nBJetMedium20", lambda ev: sum([j.btagWP("CSVv2IVFM") for j in ev.bjetsMedium if j.pt() > 20]), int, help="Number of jets with pt > 20 passing CSV medium"),
    NTupleVariable("nBJetTight20", lambda ev: sum([j.btagWP("CSVv2IVFT") for j in ev.bjetsMedium if j.pt() > 20]), int, help="Number of jets with pt > 20 passing CSV tight"),

    NTupleVariable("nBJet40", lambda ev: sum([j.btagWP("CSVv2IVFM") for j in ev.cleanJets if j.pt() > 40]), int, help="Number of jets with pt > 40 passing CSV medium"),
    NTupleVariable("nBJet30", lambda ev: sum([j.btagWP("CSVv2IVFM") for j in ev.cleanJets if j.pt() > 30]), int, help="Number of jets with pt > 25 passing CSV medium"),
    NTupleVariable("nBJet25", lambda ev: sum([j.btagWP("CSVv2IVFM") for j in ev.cleanJets if j.pt() > 25]), int, help="Number of jets with pt > 25 passing CSV medium"),
    NTupleVariable("nBJet20", lambda ev: sum([j.btagWP("CSVv2IVFM") for j in ev.cleanJets if j.pt() > 20]), int, help="Number of jets with pt > 20 passing CSV medium"),
    NTupleVariable("nMuons10", lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 13 for l in ev.selectedLeptons]), int, help="Number of muons with pt > 10"),
    NTupleVariable("nElectrons10", lambda ev: sum([l.pt() > 10 and abs(l.pdgId()) == 11 for l in ev.selectedLeptons]), int, help="Number of electrons with pt > 10"),
    NTupleVariable("nTaus20", lambda ev: sum([l.pt() > 20 for l in ev.selectedTaus]), int, help="Number of taus with pt > 20"),
    NTupleVariable("nGammas20", lambda ev: sum([l.pt() > 20 for l in ev.selectedPhotons]), int, help="Number of photons with pt > 20"),


    
    NTupleVariable("nPFLep5LowMT", lambda ev: ev.nPFLep5LowMT, int, help="number of PF leptons (e,mu) with pt > 5, reliso < 0.2, MT < 100 "),
    NTupleVariable("nPFHad10LowMT", lambda ev: ev.nPFHad10LowMT, int, help="number of PF hadrons with pt > 10, reliso < 0.1, MT < 100 "),
    NTupleVariable("nLepLowMT", lambda ev: ev.nLepLowMT, int, help="number of leptons (POGID and isoTrack ) with MT < 100 "),
    ##--------------------------------------------------
]


susyDiJet_globalObjects = susyCore_globalObjects.copy()
susyDiJet_globalObjects.update({
#            # put more here
})

susyDiJet_collections = susyCore_collections.copy()
susyDiJet_collections.update({
        # put more here
        "selectedLeptons" : NTupleCollection("lep", leptonType, 50, help="Leptons after the preselection", filter=lambda l : l.pt()>10 ),
        "selectedTaus"    : NTupleCollection("tau", tauTypeSusy, 50, help="Taus after the preselection"),
        "cleanJetsAll"       : NTupleCollection("jet",     jetTypeSusyExtra, 100, help="all jets (w/ x-cleaning, w/ ID applied w/o PUID applied pt>20 |eta|<5.2) , sorted by pt", filter=lambda l : l.pt()>20  ),
        "selectedPhotons"    : NTupleCollection("gamma", photonTypeSusy, 50, help="photons with pt>20 and loose cut based ID"),
        "selectedIsoTrack"    : NTupleCollection("isoTrack", isoTrackType, 50, help="isoTrack, sorted by pt"),
        "genParticles" : NTupleCollection("genPart", genParticleWithMotherId, 300, help="all pruned genparticles"),
        "LHE_weights"    : NTupleCollection("LHEweight",  weightsInfoType, 1000, help="LHE weight info"),
})
        
            
