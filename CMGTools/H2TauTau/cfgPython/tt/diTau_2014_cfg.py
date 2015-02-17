import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.HeppyCore.framework.config import printComps

# Tau-tau analyzers
from CMGTools.H2TauTau.proto.analyzers.TauTauAnalyzer             import TauTauAnalyzer
from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducerTauTau import H2TauTauTreeProducerTauTau
from CMGTools.H2TauTau.proto.analyzers.TauDecayModeWeighter       import TauDecayModeWeighter

# common configuration and sequence
from CMGTools.H2TauTau.htt_ntuple_base_cff import *

# local switches
syncntuple = False

eventSelector.toSelect = []

diLeptonAna.class_object = TauTauAnalyzer              
diLeptonAna.name         = 'TauTauAnalyzer'            
diLeptonAna.pt1          = 45                          
diLeptonAna.eta1         = 2.1                         
diLeptonAna.iso1         = 1.                          
diLeptonAna.looseiso1    = 10.                         
diLeptonAna.pt2          = 45                          
diLeptonAna.eta2         = 2.1                         
diLeptonAna.iso2         = 1.                          
diLeptonAna.looseiso2    = 10.                         
diLeptonAna.isolation    = 'byIsolationMVA3newDMwLTraw'
diLeptonAna.m_min        = 10                          
diLeptonAna.m_max        = 99999                       
diLeptonAna.dR_min       = 0.5                         
diLeptonAna.triggerMap   = pathsAndFilters             
diLeptonAna.jetPt        = 30.                         
diLeptonAna.jetEta       = 4.7                         
diLeptonAna.relaxJetId   = False                       
diLeptonAna.verbose      = False           

dyJetsFakeAna.channel = 'tt'

tauDecayModeWeighter = cfg.Analyzer(
  TauDecayModeWeighter   ,
  'TauDecayModeWeighter' ,
  legs = ['leg1', 'leg2'],
  )

treeProducer.class_object = H2TauTauTreeProducerTauTau
treeProducer.name         = 'H2TauTauTreeProducerTauTau'

###################################################
### CONNECT SAMPLES TO THEIR ALIASES AND FILES  ###
###################################################
from CMGTools.H2TauTau.proto.samples.phys14.diTau_Ric_Jan27 import *

###################################################
###     ASSIGN JET SMEAR, SCALE and PU to MC    ###
###################################################
mc_jet_scale = 1.
mc_jet_smear = 0.
for mc in MC_list:
  mc.jetScale   = mc_jet_scale
  mc.jetSmear   = mc_jet_smear
  mc.puFileData = puFileData
  mc.puFileMC   = puFileMC

###################################################
###             SET COMPONENTS BY HAND          ###
###################################################
selectedComponents = allsamples
# selectedComponents  = [ ZZJetsTo4L ]
# for c in selectedComponents : c.splitFactor *= 5

###################################################
###                  SEQUENCE                   ###
###################################################
sequence = commonSequence
sequence.append( tauDecayModeWeighter ) # insert at the end
# RIC: off until fixed
# if not syncntuple:
#   sequence.remove( treeProducerXCheck )

###################################################
###             CHERRY PICK EVENTS              ###
###################################################
# sequence.insert(0, eventSelector)

###################################################
###    SET THE TRIGGERS TO BE USED WITH DATA    ###
###################################################
# for data in data_parked_2012:
#   data.triggers  = data_parked_triggers_2012  ## ORDER MATTERS!
#   data.triggers += data_triggers_2012         ## ORDER MATTERS!
  
###################################################
###     SET THE TRIGGERS TO BE USED WITH MC     ###
###################################################
# for mc in MC_list:
#   mc.triggers = ['HLT_DoubleMediumIsoPFTau35_Trk5_eta2p1_v6']

###################################################
###   SET THE TRIGGERS TO BE USED WITH RH EMB   ###
###################################################
# for emb in embed_list:
#   emb.triggers = emb_rechit_triggers

###################################################
###            SET BATCH OR LOCAL               ###
###################################################
test = 1 # test = 0 run on batch, test = 1 run locally
if test == 1 :
  cache              = True
  comp               = HiggsGGH125
  comp.triggers      = [] # empty for now
  selectedComponents = [comp]
  comp.splitFactor   = 1
  comp.files         = comp.files[:1]

###################################################
###                SOME PRINTOUT                ###
###################################################
print '_'*70
print 'Processing...' 
print [s.name for s in selectedComponents]

# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components   = selectedComponents,
                     sequence     = sequence          ,
                     services     = []                ,  
                     events_class = Events
                     )
