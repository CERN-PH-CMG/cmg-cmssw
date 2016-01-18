#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
from glob import glob
import os.path, re, types, itertools

MODULES = []

from CMGTools.TTHAnalysis.tools.eventVars_2lss import EventVars2LSS 
MODULES.append( ('ttH2lss', lambda : EventVars2LSS()) )
from CMGTools.TTHAnalysis.tools.susyVars_2lssInc import SusyVars2LSSInc 
MODULES.append( ('susy2lss', lambda : SusyVars2LSSInc()) )
from CMGTools.TTHAnalysis.tools.leptonJetReCleaner import LeptonJetReCleaner,_susy2lss_lepId_CB,_susy2lss_lepId_CBloose,_susy2lss_multiIso,_tthlep_lepId,_susy2lss_idEmu_cuts,_susy2lss_idIsoEmu_cuts,_susy2lss_lepId_loosestFO,_susy2lss_lepId_tighterFO,_susy2lss_lepId_IPcuts,_susy2lss_lepConePt1015,_susy2lss_lepId_inSituLoosestFO,_susy2lss_lepId_inSituTighterFO,_susy2lss_multiIso_relaxedForInSituApp
from CMGTools.TTHAnalysis.tools.leptonChoiceRA5 import LeptonChoiceRA5
##--- TTH instances
#MODULES.append( ('leptonJetReCleanerTTH', lambda : LeptonJetReCleaner("I03Sip8", 
#                lambda lep : lep.relIso03 < 0.5 and lep.sip3d < 8 and _tthlep_lepId(lep), 
#                lambda lep : lep.mvaTTH > 0.6 and lep.mediumMuonId,
#                cleanJet = lambda lep,jet,dr : (lep.pt > 10 and dr < 0.4)) ))
#MODULES.append( ('leptonJetReCleanerTTH', lambda : LeptonJetReCleaner("MiniSip8", 
#                lambda lep : lep.miniRelIso < 0.4 and lep.sip3d < 8 and _tthlep_lepId(lep), 
#                lambda lep : lep.mvaTTH > 0.6 and lep.mediumMuonId,
#                cleanJet = lambda lep,jet,dr : (lep.pt > 10 and dr < 0.4)) ))

isFastSim = False

utility_files_dir= "/afs/cern.ch/work/p/peruzzi/ra5trees/cms_utility_files"
btagSF = utility_files_dir+"/CSVv2_25ns.csv"
btagEFF = utility_files_dir+"/btageff__ttbar_powheg_pythia8_25ns.root"
btagSF_FastSim = utility_files_dir+"/CSV_13TEV_Combined_20_11_2015_FullSim_FastSim.csv"

#--- Susy multilep instances
MODULES.append( ('leptonJetReCleanerSusyQCD', lambda : LeptonJetReCleaner("Mini", 
                   lambda lep : lep.miniRelIso < 0.4 and _susy2lss_lepId_CBloose(lep), 
                   lambda lep : lep.pt>10 and _susy2lss_lepId_loosestFO(lep) and _susy2lss_lepId_IPcuts(lep), # cuts applied on top of loose
                   lambda lep,ht : lep.pt>10 and _susy2lss_lepConePt1015(lep) and _susy2lss_lepId_IPcuts(lep) and (_susy2lss_lepId_loosestFO(lep) if ht>300 else _susy2lss_lepId_tighterFO(lep)), # cuts applied on top of loose
                   lambda lep,ht : lep.pt>10 and _susy2lss_lepConePt1015(lep) and _susy2lss_multiIso(lep) and _susy2lss_lepId_CB(lep) and (ht>300 or _susy2lss_idIsoEmu_cuts(lep)), # cuts applied on top of loose
                   cleanJet = lambda lep,jet,dr : dr<0.4,
                   selectJet = lambda jet: abs(jet.eta)<2.4,
                   isFastSim = isFastSim,
                   CSVbtagFileName = btagSF, EFFbtagFileName = btagEFF, CSVbtagFileNameFastSim = btagSF_FastSim ) ))

#MODULES.append( ('leptonJetReCleanerSusyInSitu', lambda : LeptonJetReCleaner("MiniInSitu", 
#                lambda lep : lep.miniRelIso < 0.4 and _susy2lss_lepId_CBloose(lep), 
#                lambda lep : lep.pt>10 and _susy2lss_lepId_loosestFO(lep) and _susy2lss_lepId_IPcuts(lep), # cuts applied on top of loose
#                lambda lep,ht : lep.pt>10 and _susy2lss_lepConePt1015(lep) and _susy2lss_lepId_IPcuts(lep) and (_susy2lss_lepId_inSituLoosestFO(lep) if ht>300 else _susy2lss_lepId_inSituTighterFO(lep)) and _susy2lss_multiIso_relaxedForInSituApp(lep), # cuts applied on top of loose
#                lambda lep,ht : lep.pt>10 and _susy2lss_lepConePt1015(lep) and _susy2lss_multiIso(lep) and _susy2lss_lepId_CB(lep) and (ht>300 or _susy2lss_idIsoEmu_cuts(lep)), # cuts applied on top of loose
#                cleanJet = lambda lep,jet,dr : dr<0.4,
#                selectJet = lambda jet: abs(jet.eta)<2.4,
#                isMC = True, # SET TO THE RIGHT THING
#                CSVbtagFileName = btagSF, EFFbtagFileName = btagEFF ) ))

#FRname=utility_files_dir+"/FakeRatesUCSXMethod_301115_withEWKsyst_v6.root"
FRname="hardcodedUCSx"
FS_lepSF=[utility_files_dir+"/sf_mu_mediumID_multi.root",utility_files_dir+"/sf_el_tight_IDEmu_ISOEMu_ra5.root"]

MODULES.append( ('leptonChoiceRA5', lambda : LeptonChoiceRA5("Loop","Mini",whichApplication="Fakes",lepChoiceMethod="TT_loopTF_2FF",FRFileName=FRname,isFastSim=isFastSim,lepSFFileNameFastSim=FS_lepSF))) 
#MODULES.append( ('leptonChoiceRA5_FO', lambda : LeptonChoiceRA5("SortFO","Mini",whichApplication="Fakes",lepChoiceMethod="sort_FO",FRFileName=FRname,isFastSim=isFastSim,lepSFFileNameFastSim=FS_lepSF))) 
#MODULES.append( ('leptonChoiceRA5_InSitu', lambda : LeptonChoiceRA5("InSitu","MiniInSitu",whichApplication="Fakes",lepChoiceMethod="TT_loopTF_2FF",FRFileName="InSituHardCoded",isFastSim=isFastSim,lepSFFileNameFastSim=FS_lepSF))) 
MODULES.append( ('leptonChoiceRA5_Flips', lambda : LeptonChoiceRA5("Flips","Mini",whichApplication="Flips",FRFileName="hardcodedUCSx",isFastSim=isFastSim,lepSFFileNameFastSim=FS_lepSF)))
#MODULES.append( ('leptonChoiceRA5_WZ', lambda : LeptonChoiceRA5("WZ","Mini",whichApplication="WZ",isFastSim=isFastSim,lepSFFileNameFastSim=FS_lepSF)))


#MODULES.append( ('leptonJetReCleanerSusyFO', lambda : LeptonJetReCleaner("FOsel", 
#                lambda lep : lep.miniRelIso < 0.4 and _susy2lss_lepId_CBloose(lep), 
#                lambda lep : (_susy2lss_lepId_loosestFO(lep) and _susy2lss_lepId_IPcuts(lep)), # cuts applied on top of previous selection
#                lambda lep,ht : _susy2lss_lepId_IPcuts(lep) and (_susy2lss_lepId_loosestFO(lep) if ht>300 else _susy2lss_lepId_tighterFO(lep)),
#                cleanJet = lambda lep,jet,dr : (lep.conept > (10 if abs(lep.pdgId)==13 else 15) and dr < 0.4),
#                selectJet = lambda jet: abs(jet.eta)<2.4,
#                isMC = True ) )) # SET TO THE RIGHT THING
from CMGTools.TTHAnalysis.tools.leptonFakeRateQCDVars import LeptonFakeRateQCDVars
#--- TTH instances
MODULES.append( ('leptonFakeRateQCDVarsTTH', lambda : LeptonFakeRateQCDVars(
                lambda lep : lep.sip3d < 8, # and lep.relIso03 < 0.5 and _tthlep_lepId(lep),
                lambda jet, lep, dr : jet.pt > (20 if abs(jet.eta)<2.4 else 30) and dr > 0.7) ) )
#--- Susy multilep instances
MODULES.append( ('leptonFakeRateQCDVarsSusy', lambda : LeptonFakeRateQCDVars(
                lambda lep : lep.miniRelIso < 0.4 and _susy2lss_lepId_CBloose(lep) and _susy2lss_lepId_loosestFO(lep) and _susy2lss_lepId_IPcuts(lep),
                lambda jet, lep, dr : jet.pt > 40 and abs(jet.eta)<2.4 and dr > 1.0 and jet.id ) ) )

#from CMGTools.TTHAnalysis.tools.leptonJetReCleaner import _susy2lss_multiIso_withMiniIsoRelaxed_ConePtJetPtRatio,_susy2lss_multiIso_withMiniIsoRelaxed_CutForFO4

from CMGTools.TTHAnalysis.tools.objTagger import ObjTagger
MODULES.append ( ('leptonFakeRateFO2', lambda: ObjTagger('FO2','LepGood',[
                lambda lep: lep.miniRelIso<0.4,
                lambda lep: _susy2lss_lepId_CBloose(lep),
                lambda lep: _susy2lss_lepId_loosestFO(lep),
                lambda lep: _susy2lss_lepId_IPcuts(lep),
                ] ) ) )
MODULES.append ( ('leptonFakeRateFO2iso', lambda: ObjTagger('FO2iso','LepGood',[
                lambda lep: lep.miniRelIso<0.4,
                lambda lep: _susy2lss_lepId_CBloose(lep),
                lambda lep: _susy2lss_lepId_tighterFO(lep),
                lambda lep: _susy2lss_lepId_IPcuts(lep),
                ] ) ) )
MODULES.append ( ('leptonFakeRateFO2InSitu', lambda: ObjTagger('FO2InSitu','LepGood',[
                lambda lep: lep.miniRelIso<0.4,
                lambda lep: _susy2lss_lepId_CBloose(lep),
                lambda lep: _susy2lss_lepId_loosestFO(lep),
                ] ) ) )
MODULES.append ( ('leptonFakeRateFO2isoInSitu', lambda: ObjTagger('FO2isoInSitu','LepGood',[
                lambda lep: lep.miniRelIso<0.4,
                lambda lep: _susy2lss_lepId_CBloose(lep),
                lambda lep: _susy2lss_lepId_tighterFO(lep),
                ] ) ) )

# obsolete: it is now calculated in ntupleTypes
#MODULES.append ( ('leptonIdEmuCuts', ObjTagger('idEmu','LepGood',[lambda lep: _susy2lss_idEmu_cuts(lep)]) ) )
#MODULES.append ( ('leptonIdIsoEmuCuts', ObjTagger('idIsoEmu','LepGood',[lambda lep: _susy2lss_idIsoEmu_cuts(lep)]) ) )

from CMGTools.TTHAnalysis.tools.vertexWeightFriend import VertexWeightFriend

pufile="/afs/cern.ch/user/p/peruzzi/work/cmgtools/CMSSW_7_4_14/src/CMGTools/TTHAnalysis/python/plotter/susy-multilepton/for-pu-rew/pu_plots_258750/zjets-4-nvtx_plots.root"
MODULES.append ( ('puWeightsVtx', lambda : VertexWeightFriend(pufile,pufile,"nvtx_signal","nvtx_data",verbose=True) ) )

putruefiledata_central = utility_files_dir+"/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_pileup_69000_50.root"
putruefiledata_up = utility_files_dir+"/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_pileup_69000_50_p5pc.root"
putruefiledata_down = utility_files_dir+"/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_pileup_69000_50_m5pc.root"
putruefilemc = "/afs/cern.ch/user/p/peruzzi/work/cmgtools/CMSSW_7_4_14/src/CMGTools/TTHAnalysis/python/plotter/susy-multilepton/for-pu-rew/pu_plots_true_miniAODv2/zjets-4-nvtx_plots_true.root"
MODULES.append ( ('puWeightsTrue_central', lambda : VertexWeightFriend(putruefilemc,putruefiledata_central,"nTrueInt_signal","pileup",verbose=True,vtx_coll_to_reweight="nTrueInt",name="vtxWeight") ) )
MODULES.append ( ('puWeightsTrue_up', lambda : VertexWeightFriend(putruefilemc,putruefiledata_up,"nTrueInt_signal","pileup",verbose=True,vtx_coll_to_reweight="nTrueInt",postfix="up",name="vtxWeightUp") ) )
MODULES.append ( ('puWeightsTrue_down', lambda : VertexWeightFriend(putruefilemc,putruefiledata_down,"nTrueInt_signal","pileup",verbose=True,vtx_coll_to_reweight="nTrueInt",postfix="down",name="vtxWeightDown") ) )



from CMGTools.TTHAnalysis.tools.objFloatCalc import ObjFloatCalc
from CMGTools.TTHAnalysis.tools.friendLepAwareVars import *
MODULES.append ( ('recalcLepAwareVars',ObjFloatCalc("recalcLepAwareVars","LepGood",{
                "jetPtRatio_LepAwareJECv2_p0": (lambda lep : friendPtRatiov2(lep)),
                "jetPtRelv2_p0": (lambda lep : friendPtRelv2(lep)),
                "jetPtRatio_LepAwareJECv2_p1": (lambda lep : friendPtRatiov2(lep,1.01)),
                "jetPtRelv2_p1": (lambda lep : friendPtRelv2(lep,1.01)),
                "jetPtRatio_LepAwareJECv2_p2": (lambda lep : friendPtRatiov2(lep,1.02)),
                "jetPtRelv2_p2": (lambda lep : friendPtRelv2(lep,1.02)),
                "jetPtRatio_LepAwareJECv2_p3": (lambda lep : friendPtRatiov2(lep,1.03)),
                "jetPtRelv2_p3": (lambda lep : friendPtRelv2(lep,1.03)),
                "jetPtRatio_LepAwareJECv2_p4": (lambda lep : friendPtRatiov2(lep,1.04)),
                "jetPtRelv2_p4": (lambda lep : friendPtRelv2(lep,1.04)),
                "jetPtRatio_LepAwareJECv2_p5": (lambda lep : friendPtRatiov2(lep,1.05)),
                "jetPtRelv2_p5": (lambda lep : friendPtRelv2(lep,1.05)),
                })) )

#from CMGTools.TTHAnalysis.tools.MultiIsoMVAFriend import MultiIsoMVAFriend
#MODULES.append ( ('newMultiIsoMVAtraining_2015',
#                  MultiIsoMVAFriend("2015",
#                                    (os.environ["CMSSW_BASE"]+"/src/CMGTools/TTHAnalysis/python/plotter/ttH-multilepton/train-MVA-Iso/weights/2015_%s_BDTG.weights.xml",
#                                     os.environ["CMSSW_BASE"]+"/src/CMGTools/TTHAnalysis/python/plotter/ttH-multilepton/train-MVA-Iso/weights/2015_%s_BDTG.weights.xml"),
#                                    label="MultiIso2015")) )
#MODULES.append ( ('newMultiIsoMVAtraining_2015v2',
#                  MultiIsoMVAFriend("2015v2",
#                                    (os.environ["CMSSW_BASE"]+"/src/CMGTools/TTHAnalysis/python/plotter/ttH-multilepton/train-MVA-Iso/weights/2015v2_%s_BDTG.weights.xml",
#                                     os.environ["CMSSW_BASE"]+"/src/CMGTools/TTHAnalysis/python/plotter/ttH-multilepton/train-MVA-Iso/weights/2015v2_%s_BDTG.weights.xml"),
#                                    label="MultiIso2015v2")) )

#from CMGTools.TTHAnalysis.tools.finalMVA_2lss import FinalMVA_2LSS
#MODULES.append( ('2lss_mva', FinalMVA_2LSS()) )
#from CMGTools.TTHAnalysis.tools.finalMVA_3l import FinalMVA_3L
#MODULES.append( ('3l_mva', FinalMVA_3L()) )
#from CMGTools.TTHAnalysis.tools.bbvars import bbVars
#MODULES.append( ('bbvars', bbVars()) )
#from CMGTools.TTHAnalysis.tools.finalMVA_susy_2lss import FinalMVA_SUSY_2LSS
#MODULES.append( ('finalMVA_susy_2lss', FinalMVA_SUSY_2LSS()) )
#from CMGTools.TTHAnalysis.tools.ttbarEventReco_2lss import TTEventReco_MC
#MODULES.append( ('ttreco_mc', TTEventReco_MC()) )
#from CMGTools.TTHAnalysis.tools.ttbarEventReco_2lss import TTEventReco
#MODULES.append( ('ttreco', TTEventReco(sortersToUse={"BestGuess":"", "BestBySum4NoTJJb":"_bySum4"})) )
#MODULES.append( ('ttreco', TTEventReco(sortersToUse={"BestGuess":"","ByGuessLL2B":"_byLL"})) )
#from CMGTools.TTHAnalysis.tools.MuonMVAFriend import MuonMVAFriend
#MODULES.append( ('MuMVAId', MuonMVAFriend("BPH",     "/afs/cern.ch/work/g/gpetrucc/TREES_70X_240914/0_muMVAId_v1/train70XBPH_BDTG.weights.xml", label="BPH")) )
#MODULES.append( ('MuMVAId', MuonMVAFriend("BPHCalo", "/afs/cern.ch/work/g/gpetrucc/TREES_70X_240914/0_muMVAId_v1/train70XBPHCalo_BDTG.weights.xml", label="BPHCalo")) )
#MODULES.append( ('MuMVAId', MuonMVAFriend("Full",    "/afs/cern.ch/work/g/gpetrucc/TREES_70X_240914/0_muMVAId_v1/train70XFull_BDTG.weights.xml", label="Full")) )
#from CMGTools.TTHAnalysis.tools.LepMVAFriend import LepMVAFriend
#MODULES.append( ('LepMVAFriend', LepMVAFriend(("/afs/cern.ch/user/g/gpetrucc/w/TREES_72X_171214/0_lepMVA_v1/%s_BDTG.weights.xml",
#                                               "/afs/cern.ch/user/g/gpetrucc/w/TREES_72X_171214/0_lepMVA_v1/%s_BDTG.weights.xml"))) )
#MODULES.append( ('LepMVAFriend', LepMVAFriend(("/afs/cern.ch/work/g/gpetrucc/TREES_70X_240914/0_lepMVA_v1/SV_%s_BDTG.weights.xml",
#                                               "/afs/cern.ch/work/g/gpetrucc/TREES_70X_240914/0_lepMVA_v1/SV_%s_BDTG.weights.xml",),
#                                               training="muMVAId_SV", label="SV")) )


#from CMGTools.TTHAnalysis.tools.eventVars_MT2 import EventVarsMT2 
#MODULES.append( ('MT2', EventVarsMT2()) ) 

 
class VariableProducer(Module):
    def __init__(self,name,booker,modules):
        Module.__init__(self,name,booker)
        self._modules = [ (n,m() if type(m) == types.FunctionType else m) for (n,m) in modules ]
    def beginJob(self):
        self.t = PyTree(self.book("TTree","t","t"))
        self.branches = {}
        for name,mod in self._modules:
            print name
            print mod.listBranches()
            for B in mod.listBranches():
                # don't add the same branch twice
                if B in self.branches: 
                    print "Will not add branch %s twice" % (B,)
                    continue
                self.branches[B] = True
                if type(B) == tuple:
                    if len(B) == 2:
                        self.t.branch(B[0],B[1])
                    elif len(B) == 4:
                        self.t.branch(B[0],B[1],n=B[2],lenVar=B[3])
                else:
                    self.t.branch(B ,"F")
    def analyze(self,event):
        for name,mod in self._modules:
            keyvals = mod(event)
            for B,V in keyvals.iteritems():
                setattr(self.t, B, V)
                setattr(event,  B, V)
        self.t.fill()

import os, itertools

from optparse import OptionParser
parser = OptionParser(usage="%prog [options] <TREE_DIR> <OUT>")
parser.add_option("-m", "--modules", dest="modules",  type="string", default=[], action="append", help="Run these modules");
parser.add_option("-d", "--dataset", dest="datasets",  type="string", default=[], action="append", help="Process only this dataset (or dataset if specified multiple times)");
parser.add_option("-D", "--dm", "--dataset-match", dest="datasetMatches",  type="string", default=[], action="append", help="Process only this dataset (or dataset if specified multiple times): REGEXP");
parser.add_option("-c", "--chunk",   dest="chunks",    type="int",    default=[], action="append", help="Process only these chunks (works only if a single dataset is selected with -d)");
parser.add_option("-N", "--events",  dest="chunkSize", type="int",    default=500000, help="Default chunk size when splitting trees");
parser.add_option("-j", "--jobs",    dest="jobs",      type="int",    default=1, help="Use N threads");
parser.add_option("-p", "--pretend", dest="pretend",   action="store_true", default=False, help="Don't run anything");
parser.add_option("-T", "--tree-dir",   dest="treeDir",     type="string", default="sf", help="Directory of the friend tree in the file (default: 'sf')");
parser.add_option("-q", "--queue",   dest="queue",     type="string", default=None, help="Run jobs on lxbatch instead of locally");
parser.add_option("-t", "--tree",    dest="tree",      default='ttHLepTreeProducerTTH', help="Pattern for tree name");
parser.add_option("-V", "--vector",  dest="vectorTree", action="store_true", default=True, help="Input tree is a vector");
parser.add_option("-F", "--add-friend",    dest="friendTrees",  action="append", default=[], nargs=2, help="Add a friend tree (treename, filename). Can use {name}, {cname} patterns in the treename") 
parser.add_option("--FMC", "--add-friend-mc",    dest="friendTreesMC",  action="append", default=[], nargs=2, help="Add a friend tree (treename, filename) to MC only. Can use {name}, {cname} patterns in the treename") 
parser.add_option("--FD", "--add-friend-data",    dest="friendTreesData",  action="append", default=[], nargs=2, help="Add a friend tree (treename, filename) to data trees only. Can use {name}, {cname} patterns in the treename") 
parser.add_option("-L", "--list-modules",  dest="listModules", action="store_true", default=False, help="just list the configured modules");
parser.add_option("-n", "--new",  dest="newOnly", action="store_true", default=False, help="Make only missing trees");
(options, args) = parser.parse_args()

if options.listModules:
    print "List of modules"
    for (n,x) in MODULES:
        if type(x) == types.FunctionType: x = x()
        print "   '%s': %s" % (n,x)
    exit()

if "{P}" in args[1]: args[1] = args[1].replace("{P}",args[0])
if len(args) != 2 or not os.path.isdir(args[0]):
    print "Usage: program <TREE_DIR> <OUT>"
    exit()
if not os.path.isdir(args[1]): 
    os.system("mkdir -p "+args[1])
    if not os.path.isdir(args[1]): 
        print "Could not create output directory"
        exit()
if len(options.chunks) != 0 and len(options.datasets) != 1:
    print "must specify a single dataset with -d if using -c to select chunks"
    exit()

jobs = []
for D in glob(args[0]+"/*"):
    treename = options.tree
    fname    = "%s/%s/%s_tree.root" % (D,options.tree,options.tree)
    if (not os.path.exists(fname)) and (os.path.exists("%s/%s/tree.root" % (D,options.tree)) ):
        treename = "tree"
        fname    = "%s/%s/tree.root" % (D,options.tree)

    if (not os.path.exists(fname)) and (os.path.exists("%s/%s/tree.root.url" % (D,options.tree)) ):
        treename = "tree"
        fname    = "%s/%s/tree.root" % (D,options.tree)
        fname    = open(fname+".url","r").readline().strip()

    if os.path.exists(fname) or (os.path.exists("%s/%s/tree.root.url" % (D,options.tree))):
        short = os.path.basename(D)
        if options.datasets != []:
            if short not in options.datasets: continue
        if options.datasetMatches != []:
            found = False
            for dm in  options.datasetMatches:
                if re.match(dm,short): found = True
            if not found: continue
        data =  any(x in short for x in "DoubleMu DoubleEl DoubleEG MuEG MuonEG SingleMu SingleEl".split()) # FIXME
        f = ROOT.TFile.Open(fname)
        t = f.Get(treename)
        if not t:
            print "Corrupted ",fname
            continue
        entries = t.GetEntries()
        f.Close()
        if options.newOnly:
            fout = "%s/evVarFriend_%s.root" % (args[1],short)
            if os.path.exists(fout):
                f = ROOT.TFile.Open(fname);
                t = f.Get(treename)
                if t.GetEntries() != entries:
                    print "Component %s has to be remade, mismatching number of entries (%d vs %d)" % (short, entries, t.GetEntries()) 
                    f.Close()
                else:
                    print "Component %s exists already and has matching number of entries (%d)" % (short, entries) 
                    continue 
        chunk = options.chunkSize
        if entries < chunk:
            print "  ",os.path.basename(D),("  DATA" if data else "  MC")," single chunk"
            jobs.append((short,fname,"%s/evVarFriend_%s.root" % (args[1],short),data,xrange(entries),-1))
        else:
            nchunk = int(ceil(entries/float(chunk)))
            print "  ",os.path.basename(D),("  DATA" if data else "  MC")," %d chunks" % nchunk
            for i in xrange(nchunk):
                if options.chunks != []:
                    if i not in options.chunks: continue
                r = xrange(int(i*chunk),min(int((i+1)*chunk),entries))
                jobs.append((short,fname,"%s/evVarFriend_%s.chunk%d.root" % (args[1],short,i),data,r,i))
print "\n"
print "I have %d task(s) to process" % len(jobs)

if options.queue:
    import os, sys
    basecmd = "bsub -q {queue} {dir}/lxbatch_runner.sh {dir} {cmssw} python {self} -N {chunkSize} -T '{tdir}' -t {tree} {data} {output}".format(
                queue = options.queue, dir = os.getcwd(), cmssw = os.environ['CMSSW_BASE'], 
                self=sys.argv[0], chunkSize=options.chunkSize, tdir=options.treeDir, tree=options.tree, data=args[0], output=args[1]
            )
    if options.vectorTree: basecmd += " --vector "
    friendPost =  "".join(["  -F  %s %s " % (fn,ft) for fn,ft in options.friendTrees])
    friendPost += "".join([" --FM %s %s " % (fn,ft) for fn,ft in options.friendTreesMC])
    friendPost += "".join([" --FD %s %s " % (fn,ft) for fn,ft in options.friendTreesData])
    friendPost += "".join(["  -m  '%s'  " % m for m in options.modules])
    for (name,fin,fout,data,range,chunk) in jobs:
        if chunk != -1:
            print "{base} -d {data} -c {chunk} {post}".format(base=basecmd, data=name, chunk=chunk, post=friendPost)
        else:
            print "{base} -d {data} {post}".format(base=basecmd, data=name, chunk=chunk, post=friendPost)
        
    exit()

maintimer = ROOT.TStopwatch()
def _runIt(myargs):
    (name,fin,fout,data,range,chunk) = myargs
    timer = ROOT.TStopwatch()
    if "root://" in fin:        
        ROOT.gEnv.SetValue("TFile.AsyncReading", 1);
        ROOT.gEnv.SetValue("XNet.Debug", -1); # suppress output about opening connections
        ROOT.gEnv.SetValue("XrdClientDebug.kUSERDEBUG", -1); # suppress output about opening connections
        fb   = ROOT.TXNetFile(fin+"?readaheadsz=65535")
    else:
        fb = ROOT.TFile(fin)

    print "getting tree.."
    tb = fb.Get(options.tree)

    if not tb: tb = fb.Get("tree") # new trees
    if options.vectorTree:
        tb.vectorTree = True
    else:
        tb.vectorTree = False

    friends = options.friendTrees[:]
    friends += (options.friendTreesData if data else options.friendTreesMC)
    friends_ = [] # to make sure pyroot does not delete them
    for tf_tree,tf_file in friends:
        tf = tb.AddFriend(tf_tree, tf_file.format(name=name, cname=name)),
        friends_.append(tf) # to make sure pyroot does not delete them
    nev = tb.GetEntries()
    if options.pretend:
        print "==== pretending to run %s (%d entries, %s) ====" % (name, nev, fout)
        return (name,(nev,0))
    print "==== %s starting (%d entries) ====" % (name, nev)
    booker = Booker(fout)
    modulesToRun = MODULES
    if options.modules != []:
        toRun = {}
        for m,v in MODULES:
            for pat in options.modules:
                if re.match(pat,m):
                    toRun[m] = True 
        modulesToRun = [ (m,v) for (m,v) in MODULES if m in toRun ]
    el = EventLoop([ VariableProducer(options.treeDir,booker,modulesToRun), ])
    el.loop([tb], eventRange=range)
    booker.done()
    fb.Close()
    time = timer.RealTime()
    print "=== %s done (%d entries, %.0f s, %.0f e/s) ====" % ( name, nev, time,(nev/time) )
    return (name,(nev,time))

if options.jobs > 0:
    from multiprocessing import Pool
    pool = Pool(options.jobs)
    ret  = dict(pool.map(_runIt, jobs)) if options.jobs > 0 else dict([_runIt(j) for j in jobs])
else:
    ret = dict(map(_runIt, jobs))
fulltime = maintimer.RealTime()
totev   = sum([ev   for (ev,time) in ret.itervalues()])
tottime = sum([time for (ev,time) in ret.itervalues()])
print "Done %d tasks in %.1f min (%d entries, %.1f min)" % (len(jobs),fulltime/60.,totev,tottime/60.)
