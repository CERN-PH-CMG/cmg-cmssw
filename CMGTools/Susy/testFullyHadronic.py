
import FWCore.ParameterSet.Config as cms

from ROOT import *


def loadLibs():
    #load the libaries needed
    from ROOT import gROOT,gSystem
    gSystem.Load("libFWCoreFWLite")
    gROOT.ProcessLine('AutoLibraryLoader::enable();')
    gSystem.Load("libFWCoreFWLite")
    gSystem.Load("libCintex")
    gROOT.ProcessLine('ROOT::Cintex::Cintex::Enable();')
        
    #now the RootTools stuff
    gSystem.Load("libCMGToolsRootTools")

loadLibs()

# from ROOT import TFile,TTree, TCanvas, TBrowser

b = TBrowser()

file = TFile('susy_tree_CMG.root')
tree = file.Get('Events')

can1 = TCanvas('can1','Jets',1000,1000)
can1.Divide(2,2)
can1.cd(1)
tree.Draw('cmgBaseJets_cmgPFBaseJetSel__ANA.obj.pt()')
can1.cd(2)
tree.Draw('cmgBaseJets_cmgPFBaseJetSel__ANA.obj.pt()-cmgPFJets_cmgPFJetSel__ANA.obj.pt()')
can1.cd(3)
tree.Draw('cmgBaseJets_cmgCaloBaseJetSel__ANA.obj.pt()')
can1.cd(4)

can2 = TCanvas('can2','MET',1000,1000)
can2.Divide(2,2)
can2.cd(1)
tree.Draw('cmgBaseMETs_RA2cmgMHTPFJets50__ANA.obj.sumEt()')
can2.cd(2)
tree.Draw('cmgBaseMETs_cmgMHTPFJets30__ANA.obj.et()')
can2.cd(3)
tree.Draw('cmgBaseMETs_cmgPFMET__ANA.obj.et()')
can2.cd(4)
tree.Draw('cmgBaseMETs_cmgPFMET__ANA.obj.et():cmgBaseMETs_cmgCaloMET__ANA.obj.et()','','col')

