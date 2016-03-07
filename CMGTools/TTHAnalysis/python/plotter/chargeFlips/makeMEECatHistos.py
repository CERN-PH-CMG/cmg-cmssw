#!/usr/bin/env python
import os, sys
import ROOT
from copy import deepcopy
from CMGTools.TTHAnalysis.plotter.cutsFile import CutsFile
from CMGTools.TTHAnalysis.plotter.mcPlots import addPlotMakerOptions
from CMGTools.TTHAnalysis.plotter.mcAnalysis import MCAnalysis
from CMGTools.TTHAnalysis.plotter.tree2yield import PlotSpec
from optparse import OptionParser

usage="usage: %prog [options] mca.txt cuts.txt \nrun with --help to get list of options"
parser = OptionParser(usage=usage)
addPlotMakerOptions(parser)

(options, args) = parser.parse_args()

## Some default options (too lazy to give them on the command line)
options.tree = 'treeProducerSusyMultilepton'
# options.lumi = 0.2092
# options.lumi = 0.83231
options.lumi = 2.16

try:
    print "Processing %s" % args[1]
    mca    = MCAnalysis(args[0], options)
    evcuts = CutsFile(args[1], options)
except IndexError:
    print "Please provide at least two arguments"
    parser.print_help()
    sys.exit(1)


def makeLeptonCategories():
    from itertools import product

    ptbins  = list(reversed([(10,25), (25,50), (50,1e5)]))
    etabins = [(0,1.479), (1.479,2.5)]
    # ptbins  = list(reversed([(10,25), (25,50), (50,100), (100, 1e5)]))
    # etabins = [(0,0.8), (0.8,1.479), (1.479,2.5)]
    allbins = list(product(ptbins,etabins))

    ptcut  = ("LepGood_pt[{index}]>={ptlo}&&"
              "LepGood_pt[{index}]<{pthi}")
    etacut = ("abs(LepGood_eta[{index}])>={etalo}&&"
              "abs(LepGood_eta[{index}])<{etahi}")

    categories = []
    for n,((ptlo1,pthi1),(etalo1,etahi1)) in enumerate(allbins):
        for (ptlo2,pthi2),(etalo2,etahi2) in allbins[n:]:
            cut = '({}&&{}&&{}&&{})||({}&&{}&&{}&&{})'.format(
                             ptcut.format(index=0,ptlo=ptlo1,pthi=pthi1),
                             etacut.format(index=0,etalo=etalo1,etahi=etahi1),
                             ptcut.format(index=1,ptlo=ptlo2,pthi=pthi2),
                             etacut.format(index=1,etalo=etalo2,etahi=etahi2),
                             ptcut.format(index=1,ptlo=ptlo1,pthi=pthi1),
                             etacut.format(index=1,etalo=etalo1,etahi=etahi1),
                             ptcut.format(index=0,ptlo=ptlo2,pthi=pthi2),
                             etacut.format(index=0,etalo=etalo2,etahi=etahi2))
            name = 'l1Pt_{}_l1Eta_{}_l2Pt_{}_l2Eta_{}'.format(
                                            ptlo1,etalo1,ptlo2,etalo2)
            categories.append((name,cut))

    print "... processing {} lepton categories".format(len(categories))
    # for name,cut in categories:
    #     print name, cut
    return categories

def makePlotAndCutSpecs():
    lep_categories = makeLeptonCategories()
    cut_SS = "(LepGood_charge[0]*LepGood_charge[1]>0)"
    cut_OS = "(LepGood_charge[0]*LepGood_charge[1]<0)"

    allplots = []

    for charge_name, charge_cut in zip(['SS','OS'],[cut_SS, cut_OS]):
        for lep_name,lep_cut in lep_categories:
            cutspec = deepcopy(evcuts)
            cutspec.add("charge", charge_cut)
            cutspec.add("lepbin", lep_cut)

            plotspec = PlotSpec("mee_{}".format(lep_name),
                                "minMllAFAS","100,60,120",
                                {'XTitle':'m(ll)  [GeV]', 'Legend':'TL'})

            allplots.append(('{}_{}'.format(lep_name,charge_name),plotspec, cutspec))

    return allplots

def main():
    from pprint import pprint

    ofile = ROOT.TFile.Open("meecathistos.root","RECREATE")
    ofile.cd()

    allspecs = makePlotAndCutSpecs()
    for n,(name, pspec, cspec) in enumerate(allspecs):
        print '... processing %d/%d: %s' %(n,len(allspecs),name)

        pmap = mca.getPlots(pspec,cspec.allCuts(),makeSummary=False)
        for proc in ['DY', 'data']:
            ofile.cd()
            hname = "%s_%s"%(proc,name)
            ohisto = pmap[proc].Clone(hname)
            ohisto.SetDirectory(0)
            ohisto.Write(hname, ROOT.TObject.kOverwrite)

    ofile.Write()
    ofile.Close()

    return 0



if __name__ == '__main__':
    sys.exit(main())
