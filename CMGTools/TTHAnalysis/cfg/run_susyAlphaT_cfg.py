import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import *

##------------------------------------------
## Redefine analyzer parameters
##------------------------------------------

# Muons
#------------------------------
ttHLepAna.loose_muon_pt               = 10.,
ttHLepAna.loose_muon_eta              = 2.5,
ttHLepAna.loose_muon_dxy              = 0.5
ttHLepAna.loose_muon_dz               = 1.0
#ttHLepAna.loose_muon_relIso           = 0.15

# Electrons
#------------------------------
ttHLepAna.loose_electron_id           = "POG_Cuts_ID_2012_Veto"
ttHLepAna.loose_electron_pt           = 10
ttHLepAna.loose_electron_eta          = 2.5
ttHLepAna.loose_electron_dxy          = 0.5
ttHLepAna.loose_electron_dz           = 0.
# ttHLepAna.loose_electron_relIso       = 0.15
# ttHLepAna.loose_electron_lostHits     = 999 # no cut
# ttHLepAna.inclusive_electron_lostHits = 999 # no cut
# ttHLepAna.ele_isoCorr                 = "deltaBeta"
# ttHLepAna.ele_tightId                 = "Cuts_2012"

# Photons
#------------------------------
ttHPhoAna.ptMin                        = 25,
ttHPhoAna.epaMax                       = 2.5,

# Taus 
#------------------------------
ttHTauAna.etaMax         = 2.3
ttHTauAna.dxyMax         = 99999.
ttHTauAna.dzMax          = 99999.
ttHTauAna.vetoLeptons    = False
ttHTauAna.vetoLeptonsPOG = True


# Jets (for event variables do apply the jetID and not PUID yet)
#------------------------------
ttHJetAna.relaxJetId      = False
ttHJetAna.doPuId          = False
ttHJetAna.jetEta          = 5.
ttHJetAna.jetEtaCentral   = 3.
ttHJetAna.jetPt           = 50.
ttHJetAna.recalibrateJets = False
ttHJetAna.jetLepDR        = 0.4
ttHJetMCAna.smearJets     = False


# Energy sums
#------------------------------
# NOTE: Currently energy sums are calculated with 40 GeV jets (ttHCoreEventAnalyzer.py)
#       However, the input collection is cleanjets which have a 50 GeV cut so this is a labeling problem

ttHJetMETSkim.htCut       = ('htJet50j', 0)
ttHJetMETSkim.mhtCut      = ('htJet40j', 0)
ttHJetMETSkim.nBJet       = ('CSVM', 0, "jet.pt() > 50")     # require at least 0 jets passing CSVM and pt > 50

##------------------------------------------
##  ISOLATED TRACK
##------------------------------------------

# those are the cuts for the nonEMu
ttHIsoTrackAna = cfg.Analyzer(
            'ttHIsoTrackAnalyzer',
#            candidates='cmgCandidates',
#            candidatesTypes='std::vector<cmg::Candidate>',
            candidates      ='packedPFCandidates',
            candidatesTypes ='std::vector<pat::PackedCandidate>',
            ptMin           = 5, ### for pion 
            ptMinEMU        = 5, ### for EMU
            dzMax           = 0.1,
            #####
            isoDR           = 0.3,
            ptPartMin       = 0,
            dzPartMax       = 0.1,
            maxAbsIso       = 8,
            #####
            MaxIsoSum       = 0.1, ### unused
            MaxIsoSumEMU    = 0.2, ### unused
            doSecondVeto    = False
            )


##------------------------------------------
##  ALPHAT VARIABLES
##------------------------------------------

# Tree Producer
ttHAlphaTAna = cfg.Analyzer(
            'ttHAlphaTVarAnalyzer'
            )

##------------------------------------------
##  PRODUCER
##------------------------------------------

from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import triggers_RA1_Bulk, triggers_RA1_Prompt, triggers_RA1_Parked, triggers_RA1_Single_Mu, triggers_RA1_Photon, triggers_RA1_Muon

# Tree Producer
treeProducer = cfg.Analyzer(
        'treeProducerSusyAlphaT',
        vectorTree = True,
        PDFWeights = PDFWeights,
        triggerBits = {
            'Bulk'     : triggers_RA1_Bulk,
            'Prompt'   : triggers_RA1_Prompt,
            'Parked'   : triggers_RA1_Parked,
            'SingleMu' : triggers_RA1_Single_Mu,
            'Photon'   : triggers_RA1_Photon,
            'Muon'     : triggers_RA1_Muon,
            }
        )

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.TTHAnalysis.samples.samples_13TeV_CSA14 import *

# CSA13 PU20bx25 samples: DYJetsM50_PU20bx25, DYJetsM50pythia6_PU20bx25, DYJetsM50_HT200to400_PU20bx25, DYJetsM50_HT400to600_PU20bx25, DYJetsM50_HT600toInf_PU20bx25, DYJetsMuMuM50_PtZ180_PU20bx25, DYJetsMuMuM6pythia8_PU20bx25, DYJetsMuMuM15pythia8_PU20bx25, DYJetsMuMuM50pythia8_PU20bx25, DYJetsEEpythia8_PU20bx25, DYJetsMuMupythia8_PU20bx25, EWKWmin_PU20bx25, EWKWplus_PU20bx25, EWKZjj_PU20bx25, EleGun_PU20bx25, GGHTauTau_PU20bx25, GGHZZ4L_PU20bx25, GJet_PU20bx25, JPsiPt20_PU20bx25, JPsiPt7_PU20bx25, MinBias_PU20bx25, MuMinGunPt100_PU20bx25, MuMinGunPt10_PU20bx25, MuPlusGunPt100_PU20bx25, MuPlusGunPt10_PU20bx25, NeutrinoGun_PU20bx25, QCDEM_20to30_PU20bx25, QCDEM_30to80_PU20bx25, QCDEM_80to170_PU20bx25, QCDMu_20to30_PU20bx25, QCDMu_30to50_PU20bx25, QCDMu_50to80_PU20bx25, QCDMu_80to120_PU20bx25, QCDMu_pythia6_120to170_PU20bx25, QCDMu_pythia6_20to30_PU20bx25, QCDMu_pythia6_30to50_PU20bx25, QCDMu_pythia6_50to80_PU20bx25, QCDMu_pythia6_80to120_PU20bx25, T1tttt_PU20bx25, TTHBB_PU20bx25, TTHGG_PU20bx25, TTHTauTau_PU20bx25, TTHWW_PU20bx25, TTHZZ4L_PU20bx25, TTJets_PU20bx25, TTJets_PUS14, TTpythia8_PU20bx25, VBFHBB_PU20bx25, VBFHGG_PU20bx25, VBFHWWSemi_PU20bx25, VBFHWW_PU20bx25, VBFHZG_PU20bx25, VBFHZZ4L_PU20bx25, VHMuMu_PU20bx25, VHTauTau_PU20bx25, VHWWInc_PU20bx25, VHWWLep_PU20bx25, VHZZ4L_PU20bx25, WENupyhia8_PU20bx25, WJets_PU20bx25, WminTau_PU20bx25, WplusMu_PU20bx25, WplusTau_PU20bx25, ZHBBInv_PU20bx25, ZHBBLL_PU20bx25, ZHLLInv_PU20bx25



# Selected samples as defined on the AlphaT twiki
WJetsToLNu   = [ WJetsToLNu_HT100to200_PU_S14_POSTLS170, WJetsToLNu_HT200to400_PU_S14_POSTLS170, WJetsToLNu_HT400to600_PU_S14_POSTLS170, WJetsToLNu_HT600toInf_PU_S14_POSTLS170]

# Currently not defined in the samples file could be added from here: https://cmsweb.cern.ch/das/request?view=list&limit=100&instance=prod%2Fglobal&input=dataset%3D%2F*DYJetsToLL*13TeV*%2F*PU20bx25*%2F*AODSIM
#DYJetsToLL  = []
# Currently not defined in the samples file could be added from here: https://cmsweb.cern.ch/das/request?view=list&limit=100&instance=prod%2Fglobal&input=dataset%3D%2F*ZJetsToNuNu*13TeV*%2F*PU20bx25*%2F*AODSIM
#ZJetsToNuNu = []
# https://cmsweb.cern.ch/das/request?view=list&limit=100&instance=prod%2Fglobal&input=dataset%3D%2F*GJets*13TeV*%2F*PU20bx25*%2F*AODSIM
#GJets       = []

# NOT INCLUDING: /TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Spring14miniaod-PU20bx25_POSTLS170_V5-v2/MINIAODSIM
TTbar        = [ TTpythia8_PU20bx25 ]
# https://cmsweb.cern.ch/das/request?view=list&limit=100&instance=prod%2Fglobal&input=dataset%3D%2FTToBLNu*13TeV*%2FSpring*PU20bx25*%2F*AODSIM
#TToBLNu     = []
# https://cmsweb.cern.ch/das/request?view=list&limit=100&instance=prod%2Fglobal&input=dataset%3D%2FSMS-T1qqqq*13TeV*%2FSpring*PU20bx25*%2F*AODSIM
#T1qqqq       = []
# https://cmsweb.cern.ch/das/request?view=list&limit=100&instance=prod%2Fglobal&input=dataset%3D%2FSMS-T1bbbb*13TeV*%2FSpring*PU20bx25*%2F*AODSIM
#T1bbbb       = []
T1tttt       = [ T1tttt_PU20bx25 ]


#selectedComponents = [ SingleMu, DoubleElectron, TTHToWW_PUS14, DYJetsM50_PU20bx25, TTJets_PUS14 ]
selectedComponents = []
selectedComponents.extend( WJetsToLNu )
selectedComponents.extend( TTbar )




#-------- SEQUENCE

sequence = cfg.Sequence(susyCoreSequence + [
                        ttHIsoTrackAna,
                        ttHAlphaTAna,
                        treeProducer,
                        ])


#-------- HOW TO RUN
test = 4

# Test a single component, using a single thread.
#--------------------------------------------------
if test==1:
    comp               = TTJets_PU20bx25
    #comp.files = ['/afs/cern.ch/work/p/pandolf/CMSSW_7_0_6_patch1_2/src/CMGTools/TTHAnalysis/cfg/pickevents.root']
    comp.files         = comp.files[:2]
    
    selectedComponents = [comp]
    comp.splitFactor   = 1
#--------------------------------------------------

# Test all components (1 thread per component).
#--------------------------------------------------
elif test==2:
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files       = comp.files[:1]
#--------------------------------------------------

# Run on local files
#--------------------------------------------------
elif test==4:
    comp = TTJets_PU20bx25
#    comp.name = 'TTJets'
    #    comp.files = [ '/store/mc/Spring14miniaod/TT_Tune4C_13TeV-pythia8-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/063013AD-9907-E411-8135-0026189438BD.root' ]

    comp.files = [ '/afs/cern.ch/user/m/mbaber/WORK/public/CSA14Samples/TT_Tune4C_13TeV-pythia8_PU20bx25.root' ]

    selectedComponents = [comp]
    comp.splitFactor = 1
#--------------------------------------------------


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
        
