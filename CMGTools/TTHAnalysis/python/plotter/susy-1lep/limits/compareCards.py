#!/usr/bin/env python
from copy import deepcopy
from tabulate import tabulate
from collections import OrderedDict

import re
from sys import argv
import os.path
from pprint import pprint
from optparse import OptionParser
parser = OptionParser(
    usage="%prog [options] [label=datacard.txt | datacard.txt]",
    epilog="The label=datacard.txt syntax allows to specify the label that channels from datacard.txt will have in the comparison. Example: ./compareCards.py Bkg=susy_cards_1l_4fb_BKGEstImpr_NoAddMET_DiLepFlag/T1tttt_HL_1500_100/CnC2015X_ForYieldComparisons_DPhi00.card.txt Sig=susy_cards_1l_4fb_1lepImpr_DiLepFlag/T1tttt_HL_1500_100/CnC2015X_ForYieldComparisons_DPhi05.card.txt. Needs sorced HiggsAnalysis environment (because of DatacardParser)"
    )
parser.add_option("-s", "--stat",   dest="stat",          default=False, action="store_true", help="Drop all systematics")
parser.add_option("-S", "--force-shape", dest="shape",    default=False, action="store_true", help="Treat all channels as shape analysis. Useful for mixed combinations") 
parser.add_option("-a", "--asimov", dest="asimov",  default=False, action="store_true", help="Replace observation with asimov dataset. Works only for counting experiments")
parser.add_option("-P", "--prefix", type="string", dest="fprefix", default="",  help="Prefix this to all file names")
parser.add_option("--xc", "--exclude-channel", type="string", dest="channelVetos", default=[], action="append", help="Exclude channels that match this regexp; can specify multiple ones")
parser.add_option("--ic", "--include-channel", type="string", dest="channelIncludes", default=[], action="append", help="Only include channels that match this regexp; can specify multiple ones")
parser.add_option("--X-no-jmax",  dest="noJMax", default=False, action="store_true", help="FOR DEBUG ONLY: Turn off the consistency check between jmax and number of processes.")
parser.add_option("--xn-file", "--exclude-nuisances-from-file", type="string", dest="nuisVetoFile", help="Exclude all the nuisances in this file")
parser.add_option("--en-file", "--edit-nuisances-from-file", type="string", dest="editNuisFile", help="edit the nuisances in this file")

(options, args) = parser.parse_args()
options.bin = True # fake that is a binary output, so that we parse shape lines
options.nuisancesToExclude = []
options.verbose = 0
options.allowNoSignal = True
options.allowNoBackground = True


from HiggsAnalysis.CombinedLimit.DatacardParser import *

obsline = []; obskeyline = [] ;
keyline = []; expline = []; systlines = {}
signals = []; backgrounds = []; shapeLines = []
cmax = 5 # column width

DC_Sig = Datacard()
DC_Bkg = Datacard()



if not args:
    raise RuntimeError, "No input datacards specified."
for ich,fname in enumerate(args):
    label = "ch%d" % (ich+1)
    if "=" in fname: (label,fname) = fname.split("=")
    fname = options.fprefix+fname
    dirname = os.path.dirname(fname)
    file = open(fname, "r")
    DC = parseCard(file, options)
    if label == "Bkg": DC_Bkg = DC #parseCard(file, options) #deepcopy(DC)
    elif label == "Sig": DC_Sig = DC #parseCard(file, options)
        

bins = []


#for b in DC_Bkg.bins:
CollectYieldDicts = []
CollectYieldCompDicts = []

for b in DC_Bkg.bins:
    if "_1p_" in b:
        YieldDict = OrderedDict( [ ("Bin",b), ("CR",0), ("CR-Sig.Cont.",0), ("CR-DiLep.Pur.",0), #("1B", 0), ("1BRel", 0), 
                                   ("2B", 0), ("2BRel", 0), ("2BBkgFrac", 0), ("3p", 0), ("3pRel", 0), ("3pBkgFrac", 0)])
        YieldDictComp = OrderedDict( [ ("Bin",b)])
        for (p,e) in DC_Bkg.exp[b].items(): # so that we get only self.DC.processes contributing to this bin
            if "DiLep" in p:
                YieldDict["CR"] += e
                

        for (p,e) in DC_Bkg.exp[b].items(): # so that we get only self.DC.processes contributing to this bin
            if DC_Bkg.isSignal[p]:
                YieldDict["CR-Sig.Cont."]= e/YieldDict["CR"] if YieldDict["CR"]!=0 else -1

        if DC_Bkg.obs[b]!=0: YieldDict["CR-DiLep.Pur."]= YieldDict["CR"]/DC_Bkg.obs[b]
        else: YieldDict["CR-DiLep.Pur."]= -1
        for repl in [#"_1B_",
            "_2B_","_3p_"]:
#            replacedBin = b.replace("_1p_",repl).replace("DPhi00","DPhi10")
            replacedBin = b.replace("_1p_",repl).replace("DPhi00","DPhi05")
#            replacedBin = b.replace("_1p_",repl).replace("DPhi00","Stop")
#            replacedBin = b.replace("_1p_",repl).replace("DPhi00","Top")
            if replacedBin in DC_Sig.list_of_bins():
                for (p,e) in DC_Sig.exp[replacedBin].items(): # so that we get only self.DC.processes contributing to this bin
                    if "DiLep" in p:
                        YieldDict[repl.replace("_","")] += e
                        YieldDict[repl.replace("_","")+"Rel"] = YieldDict["CR"]/e
                        YieldDict[repl.replace("_","")+"BkgFrac"] = e/DC_Sig.obs[replacedBin]
                        YieldDictComp["SR_"+p+"_"+repl.replace("_","")] = e
                        
                        

            else:
                YieldDict[repl.replace("_","")] = -1
                YieldDict[repl.replace("_","")+"Rel"] = -1


        for repl in [#"_1B_",
            "_2B_","_3p_"]:
            replacedBin = b.replace("_1p_",repl).replace("DPhi00","DPhi05")
            if replacedBin in DC_Sig.list_of_bins():
                for (p,e) in DC_Sig.exp[replacedBin].items(): # so that we get only self.DC.processes contributing to this bin
                    if "DiLep" in p:
#                        print p
                        YieldDictComp["SR_"+p+"_"+repl.replace("_","")] = YieldDictComp["SR_"+p+"_"+repl.replace("_","")]/YieldDict[repl.replace("_","")]


        CollectYieldDicts.append(YieldDict)
        CollectYieldCompDicts.append(YieldDictComp)
print 
print tabulate(CollectYieldDicts, headers="keys", floatfmt=".4f")
print tabulate(CollectYieldDicts, headers="keys", floatfmt=".4f", tablefmt="latex")
print tabulate(CollectYieldCompDicts, headers="keys", floatfmt=".4f", tablefmt="latex")

