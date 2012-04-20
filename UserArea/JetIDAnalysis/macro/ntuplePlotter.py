#!/bin/env python

from optparse import OptionParser, make_option
import fnmatch, glob, os, sys

from CMG.JetIDAnalysis.ntupletools import *

## ------------------------------------------------------------------------------------------------------------------------------------------------------
def main(options,args):
    
    from ROOT import TFile, TChain, gROOT, TF1, TH1, TGraph
    from CMGTools.RootTools.statistics.Histograms import Histograms
    from CMGTools.RootTools.utils.PileupJetHistograms import PileupJetHistograms, mkBinLabels, findBin
    from CMGTools.RootTools.analyzers.SimpleJetAnalyzer import ResolutionJetHistograms, FractionJetHistograms, JetHistograms

    ## customize the histograms filled by PileupJetHistograms
    if options.customPrototypes:
        PileupJetHistograms.prototypes={
            "d0"         : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","cm",100,0,2),
            "dZ"         : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","cm",100,0,10),
            
            "beta"         : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","",200,0,1.),
            "betaStar"     : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","",200,0,1.),
            "nParticles" : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,50),
 
            "dRMean"     :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.5),
            "dR2Mean"     :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.25),
            
            "etaW"   :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.5),
            "phiW"   :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.5),
            "majW"   :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.5),
            "minW"   :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.5),
            "jetW"   :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.5),
            
            "jetPt"      : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","GeV/c",300,0,150),
            "jetEta"     : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",101,-5.05,5.05,),
            
            "nvtx"     : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,.5,50.5),
            
            }
        if options.tmva:
            for mva in options.mvas.split(","):
                PileupJetHistograms.prototypes[mva] = ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,-1.,1.)
        elif options.customPrototypes:
            PileupJetHistograms.prototypes["mva"] = ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,-1.,1.)

    
    ## cuts and binning
    genDrCut = 0.3
    genPtCut = 0.
    genDrAntiCut = 0.2
    
    vtxBins   = (1,10,20)
    ## ptBins    = (0,10,20,30)
    ptBins    = (0,10,20,25,30,40,50)
    #### vtxBins   = (0,10,15,20,30)
    #### ptBins    = (20,30,50)
    #### if options.tmva:
    ####     vtxBins   = (1,10,20)
    ####     ## ptBins    = (0,10,20,30)
    ####     ptBins    = (0,10,20,25,30,40,50)
    vtxBinLabels = mkBinLabels(vtxBins)

    etaBins   = (0,2.5,2.75,3.0)
    puEtaLables = ["_TK","_HEin","_HEout","_HF"]

    ### etaBins   = (0,2.5,3.0)
    ### puEtaLables = ["_central","_endNOtk","_fwd"]
    
    reweight_f = TF1("f","pol2(0)+expo(3)")
    reweight_f.SetParameters(0.1955298,-0.003830591,1.944794e-05,4.649755,-0.1722024)
    reweight = ("jetPt", reweight_f)

    ## book histograms
    fout = TFile.Open(options.outfile,"recreate")
    fout.cd()

    if options.tmva:
        sig = PileupJetHistograms("signal",vtxBins,ptBins,etaBins,etalabels=puEtaLables,reweight=reweight)
        bkg = PileupJetHistograms("background",vtxBins,ptBins,etaBins,etalabels=puEtaLables,reweight=reweight)
    else:
        matchedCleanJetHistos = JetHistograms ('MatchedCleanJets')
        unmatchedCleanJetHistos = JetHistograms ('UnmatchedCleanJets')
        
        gluCleanHistosId = PileupJetHistograms("GluonMatchedCleanHistosId",vtxBins,ptBins,etaBins,etalabels=puEtaLables,reweight=reweight)
        quarkCleanHistosId = PileupJetHistograms("QuarkMatchedCleanHistosId",vtxBins,ptBins,etaBins,etalabels=puEtaLables,reweight=reweight)
        reweiMatchedCleanHistosId = PileupJetHistograms("ReweiMatchedCleanHistosId",vtxBins,ptBins,etaBins,etalabels=puEtaLables,reweight=reweight)
        unmatchedCleanHistosId = PileupJetHistograms("UnmatchedCleanHistosId",vtxBins,ptBins,etaBins,etalabels=puEtaLables)

    ## create TChain and MakeClass
    if "," in options.indir:
        files = []
        for d in options.indir.split(","):
            files += getListOfFiles( d, options.filePattern)
    else:
        files = getListOfFiles( options.indir, options.filePattern)
    print files
    chain = mkChain(files,options.treename)
    if options.tmva:
        tr    = mkReader(chain,True,"tmva_reader")
    else:
        tr    = mkReader(chain,options.redoReader)
    
    ## Loop over ntuples
    nentries = chain.GetEntries()
    print "Reading %d entries" % nentries
    for i in xrange(nentries):
        if i % 1000 == 0:
            print>> sys.stderr, "Entry %d (%1.0f%%)\r" % (i, float(i) / nentries * 100.),
        tr.GetEntry(i)

        ## TMVA training/test tree
        if options.tmva:
            if tr.classID == 0:
                ## tr.weight = tr.sigwei
                tr.weight = 1.
                sig.fillRootTuple(tr)
            else:
                tr.weight = 1.
                ## tr.weight = tr.bkgwei
                bkg.fillRootTuple(tr)
                
        ## Original ntuples
        else:
            if not tr.jetLooseID:
                continue

            if options.forceFlavour != 0:
                tr.jetFlavour = options.forceFlavour
            
            ## tr.weight = reweight[1](getattr(tr,reweight[0]))
            tr.weight = 1.
            
            if tr.isMatched and tr.jetGenPt > genPtCut and tr.jetGenDr < genDrCut:
                reweiMatchedCleanHistosId.fillRootTuple(tr)
                matchedCleanJetHistos.fillRootTuple(tr)
                if abs(tr.jetFlavour) == 21:
                    gluCleanHistosId.fillRootTuple(tr)
                elif abs(tr.jetFlavour) <=3 and abs(tr.jetFlavour) !=0:
                    quarkCleanHistosId.fillRootTuple(tr)

            elif not tr.isMatched or ( tr.jetGenDr > genDrAntiCut and tr.jetGenPt < genPtCut ):
                unmatchedCleanJetHistos.fillRootTuple(tr)
                unmatchedCleanHistosId.fillRootTuple(tr)

    ## Write out histograms
    for name,obj in locals().iteritems():
        if isinstance(obj,Histograms):
            obj.Write(fout)
        elif isinstance(obj,TH1) or isinstance(obj,TGraph):
            fout.cd()
            obj.Write()
            
        ### for h in matchedCleanJetHistos, unmatchedCleanJetHistos, gluCleanHistosId, quarkCleanHistosId, reweiMatchedCleanHistosId, unmatchedCleanHistosId:
        ###     h.Write(fout)
        
    fout.Close()
    
## ------------------------------------------------------------------------------------------------------------------------------------------------------    
if __name__ == "__main__":
    parser = OptionParser(option_list=[
            make_option("-i", "--indir",
                        action="store", type="string", dest="indir",
                        default="/store/cmst3/user/malberti/JETID_NTUPLES",
                        help="input directory", metavar="DIR"
                        ),
            make_option("-f", "--files",
                        action="store", type="string", dest="filePattern",
                        default="*.root",
                        help="pattern of files to be read", metavar="PATTERN"
                        ),
            make_option("-o", "--outfile",
                        action="store", type="string", dest="outfile",
                        default="test.root",
                        help="outputfile", metavar="FILE"
                        ),
            make_option("-r", "--forceRedoReader",
                        action="store_true", dest="redoReader",
                        default=False,
                        help="re-generate tree reader using TChain::MakeClass"
                        ),
            make_option("-t", "--treeName",
                        action="store", type="string", dest="treename",
                        default="pfjetanalyzer/tree",
                        help="TTree name", metavar="TREENAME"
                        ),
            make_option("-c", "--customPrototypes",
                        action="store_true", dest="customPrototypes",
                        default=True,
                        help="",
                        ),
            make_option("-d", "--defaultPrototypes",
                        action="store_false", dest="customPrototypes",
                        help="",
                        ),
            make_option("-F", "--forceFlavour",
                        action="store", type="int", dest="forceFlavour", 
                        default=0,
                        help="",
                        ),
            make_option("-T", "--tmva",
                        action="store_true", dest="tmva",
                        default=False,
                        help="",
                        ),
            make_option("-m", "--mvas",
                        action="store", type="string", dest="mvas",
                        default="",
                        help="list of MVAs to study",
                        ),
                 ])

    (options, args) = parser.parse_args()
    
    main(options, args)

            
