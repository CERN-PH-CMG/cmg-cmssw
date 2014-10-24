##########################################################
##       CONFIGURATION FOR SUSY MULTILEPTON TREES       ##
## skim condition: >= 2 loose leptons, no pt cuts or id ##
##########################################################

import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import * 

# --- LEPTON SKIMMING ---
ttHLepSkim.minLeptons = 0
ttHLepSkim.maxLeptons = 999

ttHGenLevel = cfg.Analyzer(
    'ttHGenLevelOnlyStudy'
)


from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import triggers_mumu, triggers_ee, triggers_mue, triggers_1mu
# Tree Producer
treeProducer = cfg.Analyzer(
    'treeProducerSusyGenLevelOnly',
    vectorTree = True,
    saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
    PDFWeights = PDFWeights,
    # triggerBits = {} # no HLT
    )


#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import * 
Test  = kreator.makePrivateMCComponent('Test', '/store/cmst3/user/gpetrucc/maiani', ['m100_g050_3mu.GEN.root'] )
WZ3l_ascms = kreator.makePrivateMCComponent('WZ3l_ascms', '/store/cmst3/user/gpetrucc/maiani/tests', ['xs_wz_3l_ascms.GEN.root'])
WZ3mu_ascms = kreator.makePrivateMCComponent('WZ3mu_ascms', '/store/cmst3/user/gpetrucc/maiani/tests', ['xs_wz_3mu_ascms.GEN.root'])
WZ3mu_offshell = kreator.makePrivateMCComponent('WZ3mu_offshell', '/store/cmst3/user/gpetrucc/maiani/tests', ['xs_wz_3mu_offshell.GEN.root'])

GEN_S3m_lo = kreator.makePrivateMCComponent('GEN_S3m_lo', '/store/cmst3/user/gpetrucc/lmutau/madtests/', [ 'lmutau_lo_test.GEN.root' ])
GEN_S3m_lo_012j = kreator.makePrivateMCComponent('GEN_S3m_lo_012j', '/store/cmst3/user/gpetrucc/lmutau/madtests/', [ 'lmutau_lo_012j_test.GEN.root' ])
GEN_S3m_lo_01j = kreator.makePrivateMCComponent('GEN_S3m_lo_01j', '/store/cmst3/user/gpetrucc/lmutau/madtests/', [ 'lmutau_lo_01j_test.GEN.root' ])
GEN_S3m_nlo = kreator.makePrivateMCComponent('GEN_S3m_nlo', '/store/cmst3/user/gpetrucc/lmutau/madtests/', [ 'lmutau_nlo_test.GEN.root' ])
GEN_S3m_nlo_01j = kreator.makePrivateMCComponent('GEN_S3m_nlo_01j', '/store/cmst3/user/gpetrucc/lmutau/madtests/', [ 'lmutau_nlo_01j_test.GEN.root', 'lmutau_nlo_01j_test.2.GEN.root' ])
GEN_S3m_lo_direct = kreator.makePrivateMCComponent('GEN_S3m_lo_direct', '/store/cmst3/user/gpetrucc/lmutau/madtests/', [ 'lmutau_lo_direct_test.GEN.root' ])

GEN_T2tt_py8had = kreator.makePrivateMCComponent('GEN_T2tt_py8had', '/store/cmst3/user/gpetrucc/SUSY/TestProd/T2tt/', [ 'T2tt_onshell_pyt8had.root' ])
GEN_T2tt_py8dec_py8had = kreator.makePrivateMCComponent('GEN_T2tt_py8dec_py8had', '/store/cmst3/user/gpetrucc/SUSY/TestProd/T2tt/', [ 'T2tt_onshell_py8decay_pyt8had.root' ])
GEN_T2tt_mgdec_py8had  = kreator.makePrivateMCComponent('GEN_T2tt_mgdec_py8had',  '/store/cmst3/user/gpetrucc/SUSY/TestProd/T2tt/', [ 'T2tt_decayed_pyt8had.root' ])

#selectedComponents = [ GEN_S3m_lo, GEN_S3m_lo_012j, GEN_S3m_lo_01j, GEN_S3m_nlo, GEN_S3m_nlo_01j, GEN_S3m_lo_direct ]
selectedComponents = [ GEN_T2tt_py8had, GEN_T2tt_py8dec_py8had, GEN_T2tt_mgdec_py8had ]

#-------- SEQUENCE

sequence = cfg.Sequence([
    skimAnalyzer,
    ttHGenLevel, 
    ttHLepSkim,
    treeProducer,
    ])


#-------- HOW TO RUN
test = 0
if test==1:
    # test a single component, using a single thread.
    comp = GEN_S3m_nlo_01j
    comp.files = comp.files[:1]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:    
    # test all components (1 thread per component).
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:1]



config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
