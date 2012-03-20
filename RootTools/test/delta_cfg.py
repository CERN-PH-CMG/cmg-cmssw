import copy
import glob
import CMGTools.RootTools.fwlite.Config as cfg



def idMuon(muon):
    return muon.getSelection('cuts_vbtfmuon') 

ana = cfg.Analyzer(
    'DeltaAnalyzer',
    col1_instance = 'cmgMuonSel',
    col1_type = 'std::vector< cmg::Muon >',
    sel2 = idMuon,
    col2_instance = 'cmgMuonSelStdLep',
    col2_type = 'std::vector< cmg::Muon >',
    deltaR = 999999,
    gen_instance = 'genLeptonsStatus1',
    gen_type = 'std::vector<reco::GenParticle>',
    gen_pdgId = 13
    )



tree = cfg.Analyzer(
    'DeltaTreeAnalyzer',
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.cmg_testMVAs import *

#########################################################################################


selectedComponents  = [DYJets] 

splitFactor = 14
DYJets.files = DYJets.files[:560]
DYJets.splitFactor = splitFactor
QCDMuH20Pt15.splitFactor = splitFactor
QCDMuH15to20Pt5.splitFactor = splitFactor
Hig105.splitFactor = splitFactor

test = False
if test:
    sam = DYJets
    sam.files = sam.files[:1]
    selectedComponents = [sam]
    sam.splitFactor = 1


sequence = cfg.Sequence( [ana, tree] )

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

