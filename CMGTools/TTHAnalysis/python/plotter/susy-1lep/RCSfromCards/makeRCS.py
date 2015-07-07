#!/usr/bin/env python
#Script to read data cards and turn them either into a table that can be copied to Excel/OpenOffice
#1;2cor print out in latex format.

import shutil
import subprocess
import os
import sys
import glob
from multiprocessing import Pool
from ROOT import *
import math

def _getYieldsFromInput(inargs):

    (sample, cardDir, binName, ratDict) = inargs

    if len(inargs) < 1:
        return (binName,[0,0])

    cardName = cardDir+"/common/CnC2015X_"+binName+".input.root"

    #print "# Starting Bin:", binName

    cardf = TFile(cardName,"READ")

    hTT_DiLep = cardf.Get("x_TT_DiLep")
    nTT_DiLep = hTT_DiLep.Integral()
    nTTErr_DiLep = hTT_DiLep.GetBinError(1)
                             
    hTT_SemiLep = cardf.Get("x_TT_SemiLep")
    nTT_SemiLep = hTT_SemiLep.Integral()
    nTTErr_SemiLep = hTT_SemiLep.GetBinError(1)

    hTT_FullHad = cardf.Get("x_TT_FullHad")
    nTT_FullHad = hTT_FullHad.Integral()
    nTTErr_FullHad = hTT_FullHad.GetBinError(1)

    nTT_SemiLep=nTT_SemiLep+nTT_FullHad
    nTTErr_SemiLep = math.sqrt(nTTErr_SemiLep*nTTErr_SemiLep + nTTErr_FullHad*nTTErr_FullHad)

    nTT = nTT_SemiLep + nTT_DiLep
    nTTErr = math.sqrt(nTTErr_SemiLep*nTTErr_SemiLep + nTTErr_DiLep*nTTErr_DiLep)

    hSingleT = cardf.Get("x_SingleT")
    nSingleT = hSingleT.Integral()
    nSingleTErr = hSingleT.GetBinError(1)

    hTTV = cardf.Get("x_TTV")
    nTTV = hTTV.Integral()
    nTTVErr = hTTV.GetBinError(1)

    hWJets = cardf.Get("x_WJets")
    nWJets = hWJets.Integral()
    nWJetsErr = hWJets.GetBinError(1)

    hQCD = cardf.Get("x_QCD")
    nQCD = hQCD.Integral()
    nQCDErr = hQCD.GetBinError(1)

    hDY = cardf.Get("x_DY")
    nDY = hDY.Integral()
    nDYErr = hDY.GetBinError(1)

    cardf.Close()

    binName=binName.replace('CR','')
    binName=binName.replace('SR','')

    return (binName,[nTT, nTTErr, nTT_SemiLep, nTTErr_SemiLep, nTT_DiLep, nTTErr_DiLep, nTT_FullHad, nTTErr_FullHad,nSingleT, nSingleTErr, nTTV, nTTVErr, nWJets,  nWJetsErr, nQCD, nQCDErr, nDY, nDYErr])

def makeRCS(yieldDictSR, yieldDictCR):
    RCSval={}
    print "bin | Nsignal +/- Err| NControl +/- Err |  RCS +/- Err| "
    
    ykeys = sorted(yieldDictSR.keys())
    for bin in ykeys:
        (nTTSR, nTTSRErr, nTTSR_SemiLep, nTTSRErr_SemiLep, nTTSR_DiLep, nTTSRErr_DiLep, nTTSR_FullHad, nTTSRErr_FullHad, nSingleTSR, nSingleTSRErr, nTTVSR, nTTVSRErr, nWJetsSR,  nWJetsSRErr, nQCDSR, nQCDSRErr, nDYSR, nDYSRErr) = yieldDictSR[bin]
        
        (nTTCR, nTTCRErr, nTTCR_SemiLep, nTTCRErr_SemiLep, nTTCR_DiLep, nTTCRErr_DiLep, nTTCR_FullHad, nTTCRErr_FullHad, nSingleTCR, nSingleTCRErr, nTTVCR, nTTVCRErr, nWJetsCR,  nWJetsCRErr, nQCDCR, nQCDCRErr, nDYCR, nDYCRErr) = yieldDictCR[bin]
        
#        allBkgSR = nTTSR + nSingleTSR + nTTVSR + nWJetsSR + nQCDSR + nDYSR
#        allBkgSRErr = math.sqrt(nTTSRErr*nTTSRErr + nSingleTSRErr*nSingleTSRErr +nTTVSRErr*nTTVSRErr+ nWJetsSRErr*nWJetsSRErr +nQCDSRErr*nQCDSRErr+nDYSRErr*nDYSRErr)
        allBkgSR = nTTSR + nSingleTSR + nTTVSR + nWJetsSR + nDYSR
        allBkgSRErr = math.sqrt(nTTSRErr*nTTSRErr + nSingleTSRErr*nSingleTSRErr +nTTVSRErr*nTTVSRErr+ nWJetsSRErr*nWJetsSRErr+nDYSRErr*nDYSRErr)
        
#        allBkgCR = nTTCR + nSingleTCR + nTTVCR + nWJetsCR + nQCDCR + nDYCR
#        allBkgCRErr = math.sqrt(nTTCRErr*nTTCRErr + nSingleTCRErr*nSingleTCRErr +nTTVCRErr*nTTVCRErr+ nWJetsCRErr*nWJetsCRErr +nQCDCRErr*nQCDCRErr+nDYCRErr*nDYCRErr) 
        allBkgCR = nTTCR + nSingleTCR + nTTVCR + nWJetsCR + nDYCR
        allBkgCRErr = math.sqrt(nTTCRErr*nTTCRErr + nSingleTCRErr*nSingleTCRErr +nTTVCRErr*nTTVCRErr+ nWJetsCRErr*nWJetsCRErr +nDYCRErr*nDYCRErr) 
        
        RCS = allBkgSR/allBkgCR
        RCS_Err = RCS * math.sqrt(allBkgSRErr/allBkgSR*allBkgSRErr/allBkgSR + allBkgCRErr/allBkgCR*allBkgCRErr/allBkgCR)
        print "%s:|%2.2f +/- %2.2f | %2.2f +/- %2.2f |%2.4f +/- %2.4f" % ( bin, allBkgSR, allBkgSRErr,allBkgCR, allBkgCRErr, RCS, RCS_Err)

        RCSval[bin] = [RCS, RCS_Err] 

    return RCSval

nameDict = {}
nameDict['ST1'] = " LT > 250 &&  LT < 350)"                                                                           
nameDict['ST2'] = " LT > 350  && LT < 450)"
nameDict['ST3'] = " LT > 450 && LT < 600)"                                                                              
nameDict['ST4'] = " LT > 600)"
nameDict['1B'] = "if ((nB == 1) && "
nameDict['2B'] = "if ((nB == 2) && "
nameDict['3B'] = "if ((nB >= 3) && "

def convLine(line):          
    line = line.replace('_45j_HT012','')
    line = line.replace('_','')
    for key in nameDict.keys(): 
        if key in line: 
            line = ' '+line   
            line = line.replace(key,nameDict[key]) 
            cline = line

    return cline 

def printRCSfunction(RCS_SB):
    ykeys = sorted(RCS_SB.keys())
    nkeys = nameDict.keys()
    for bin in ykeys:
        print convLine(bin), ' weight = ' + str(RCS_SB[bin][0])+ ';'
    


def makeKfactor(RCS_SB, RCS_SR):
    #doesn't quite work
    #would need to add up bjet multiplicity doesn't quite work yet
    STbins = ['ST1','ST2','ST3','ST4']
    names_SB = RCS_SB.keys()
    names_SR = RCS_SR.keys()
    for ST in STbins:
        for name_SB in names_SB:
            for name_SR in names_SR:
                if ST in name_SB and ST in name_SR:
                    print name_SB, name_SR, ST, RCS_SR[name_SR][0]/RCS_SB[name_SB][0]
                    

# MAIN
if __name__ == "__main__":

    nJobs = 12

    # read f-ratios
    ratDict = {}
#    ratDict = readRatios()

    ## usage: python read.py cardDir textformat

    if len(sys.argv) > 1:
        cardDirectory = sys.argv[1]
    else:
        cardDirectory="yields/QCD_yields_3fb_test3"

    if len(sys.argv) > 2:
        pfmt = sys.argv[3]
    else:
        pfmt = "text"


    cardDirectory = os.path.abspath(cardDirectory)
    cardDirName = os.path.basename(cardDirectory)

    print 'Using cards from', cardDirName
    commondir = 'common'
    cardPattern = 'CnC2015X'

    limitdict = {}
    sigdict = {}


    #print 80*'#'
    #print "Yields for", QCDdir

    # get card file list
    inDir = cardDirectory+'/'+commondir
    cardFnames = glob.glob(inDir+'/'+ cardPattern + '_*.root')
    cardNames = [os.path.basename(name) for name in cardFnames]

    cardNames = [(name.replace(cardPattern+'_','')).replace('.input.root','') for name in cardNames]

    SB = '45j'
    cardNamesSR_SB = [name for name in cardNames if name.find('SR_' + SB) > 0]
    cardNamesCR_SB = [name for name in cardNames if name.find('CR_' + SB) > 0]

    nj68 = '68j'
    cardNamesSR_nj68 = [name for name in cardNames if name.find('SR_' + nj68) > 0]
    cardNamesCR_nj68 = [name for name in cardNames if name.find('CR_' + nj68) > 0]

    nj9Inf = '9Infj'
    cardNamesSR_nj9Inf = [name for name in cardNames if name.find('SR_' + nj9Inf) > 0]
    cardNamesCR_nj9Inf = [name for name in cardNames if name.find('CR_' + nj9Inf) > 0]

    #for kappa determination
    cardNamesSR_K = [name for name in cardNames if name.find('SRK_6') > 0 or name.find('SRK_9') > 0]
    cardNamesCR_K = [name for name in cardNames if name.find('CRK_6') > 0 or name.find('CRK_9') > 0]


    cardNamesSR_SB_K = [name for name in cardNames if name.find('SRK_4') > 0]
    cardNamesCR_SB_K = [name for name in cardNames if name.find('CRK_4') > 0]


    argTupleSR_SB = [(commondir, cardDirectory, name, ratDict) for name in cardNamesSR_SB]
    argTupleCR_SB = [(commondir, cardDirectory, name, ratDict) for name in cardNamesCR_SB]
    pool = Pool(nJobs)
    yieldDictSR_SB = dict(pool.map(_getYieldsFromInput, argTupleSR_SB))             
    yieldDictCR_SB = dict(pool.map(_getYieldsFromInput, argTupleCR_SB))                   

    argTupleSR_nj68 = [(commondir, cardDirectory, name, ratDict) for name in cardNamesSR_nj68]
    argTupleCR_nj68 = [(commondir, cardDirectory, name, ratDict) for name in cardNamesCR_nj68]
    pool = Pool(nJobs)
    yieldDictSR_nj68 = dict(pool.map(_getYieldsFromInput, argTupleSR_nj68))             
    yieldDictCR_nj68 = dict(pool.map(_getYieldsFromInput, argTupleCR_nj68))                   

    argTupleSR_nj9Inf = [(commondir, cardDirectory, name, ratDict) for name in cardNamesSR_nj9Inf]
    argTupleCR_nj9Inf = [(commondir, cardDirectory, name, ratDict) for name in cardNamesCR_nj9Inf]
    pool = Pool(nJobs)
    yieldDictSR_nj9Inf = dict(pool.map(_getYieldsFromInput, argTupleSR_nj9Inf))             
    yieldDictCR_nj9Inf = dict(pool.map(_getYieldsFromInput, argTupleCR_nj9Inf))                   

    argTupleSR_K = [(commondir, cardDirectory, name, ratDict) for name in cardNamesSR_K]
    argTupleCR_K = [(commondir, cardDirectory, name, ratDict) for name in cardNamesCR_K]
    pool = Pool(nJobs)
    yieldDictSR_K = dict(pool.map(_getYieldsFromInput, argTupleSR_K))             
    yieldDictCR_K = dict(pool.map(_getYieldsFromInput, argTupleCR_K))                   

    argTupleSR_SB_K = [(commondir, cardDirectory, name, ratDict) for name in cardNamesSR_SB_K]
    argTupleCR_SB_K = [(commondir, cardDirectory, name, ratDict) for name in cardNamesCR_SB_K]
    pool = Pool(nJobs)
    yieldDictSR_SB_K = dict(pool.map(_getYieldsFromInput, argTupleSR_SB_K))             
    yieldDictCR_SB_K = dict(pool.map(_getYieldsFromInput, argTupleCR_SB_K))                   

                            
    RCS_SB = makeRCS(yieldDictSR_SB,yieldDictCR_SB)
    RCS_nj68 = makeRCS(yieldDictSR_nj68,yieldDictCR_nj68)
    RCS_nj9Inf = makeRCS(yieldDictSR_nj9Inf,yieldDictCR_nj9Inf)

    RCS_K = makeRCS(yieldDictSR_K,yieldDictCR_K)
    RCS_SB_K = makeRCS(yieldDictSR_SB_K,yieldDictCR_SB_K)


    printRCSfunction(RCS_SB)
#    print RCS_SB.keys(), RCS_nj68, RCS_nj9Inf
    
    makeKfactor(RCS_SB_K, RCS_K)
