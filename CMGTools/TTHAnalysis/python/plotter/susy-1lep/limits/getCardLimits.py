#!/usr/bin/env python

import shutil
import subprocess
import os
import glob
from multiprocessing import Pool

#def _getLimits(sample, cardDir, binName):
def _getLimits(inargs):

    (sample, cardDir, binName) = inargs

#    print inargs
    if len(inargs) < 1:
        return (binName,[0,0])

    limfName = "Limits_"+sample+"_"+binName
    signfName = "Significance_"+sample+"_"+binName
    cardName = cardDir+"/"+sample+"/CnC2015X_"+binName+".card.txt"

    sigline = 'Fail'
    limitline = 'Fail'

    print "# Starting Bin:", binName

    os.system("combine -M Asymptotic "+ cardName + " -n " + limfName+ " &> "+ limfName + ".txt")
    os.system("combine -M ProfileLikelihood --significance "+ cardName + " -t -1 --expectSignal=1 -n "+ signfName +" &> "+ signfName + ".txt")

    lf=open(limfName+".txt")
    llines=lf.readlines()

    for line in llines:
        if "50.0%" in line:
            limitline = line

    sf=open(signfName+".txt")
    slines=sf.readlines()

    for sline in slines:
        if "Signif" in sline:
            sigline = sline

    sigma = -1
    limit = -1

    print "# Results for bin:", binName
    if limitline == 'Fail':
        print 'Limit not found!'
    elif sigline == 'Fail':
        print 'Significance not found!'
    else:
        print limitline, sigline
        sigma = float(sigline.split(':')[1])
        limit = float(limitline.split('<')[1])

#    return [binName,sigma, limit]
    return (binName,[sigma, limit])

# check CMSSW version
cmssw_vers = os.environ['CMSSW_VERSION']

if 'CMSSW_7_1' not in cmssw_vers:
    print "You've got the wrong CMSSW env:"
    print "Need CMSSW_7_1_X with limit tool!"
    exit(0)

cardDirectory="susy_cards_1l_4fb_allBins"
cardDirectory = os.path.abspath(cardDirectory)

limitDir = cardDirectory+"/limits_test/"
if not os.path.isdir(limitDir):
    print 'Creating limit dir', limitDir
    os.mkdir(limitDir)

print 'Entering out dir', limitDir
os.chdir(limitDir)
print

Samples = []
#Samples.append("T1tttt_HL_1500_100")
Samples.append("T1tttt_HM_1200_800")

VariantSnippet = ['3B_ST4_9Infj_HT1']
#VariantSnippet = ["standardnJ", "finenJ"]

limitdict = {}
sigdict = {}

nJobs = 12

for s_i, s in enumerate(Samples):

    print 80*'#'
    print "Limits for", s

    # get card file list
    inDir = cardDirectory+'/'+s
    cardFnames = glob.glob(inDir+'/CnC2015X_*.txt')
    cardNames = [os.path.basename(name) for name in cardFnames]
    cardNames = [(name.replace('CnC2015X_','')).replace('.card.txt','') for name in cardNames]

    print 'Card list'#, cardNames

#    for v_i, v in enumerate(VariantSnippet):
#        _getLimits((s,cardDirectory,v))

#    for v_i, v in enumerate(cardNames):
#        _getLimits(s,cardDirectory,v)

    argTuple = [(s,cardDirectory,name) for name in cardNames]
#    argTuple = [(s,cardDirectory,name) for name in VariantSnippet]

#    print argTuple
#    print (s,cardDirectory,cardNames)

    pool = Pool(nJobs)
#    limDict  = dict(pool.map(_, jobs)) if options.jobs > 0 else dict([_runIt(j) for j in jobs])
    limDict = dict(pool.map(_getLimits, argTuple))
#   limDict  = dict(pool.map(_getLimits, cardNames))

    print 80*'#'
    print 'Results:'
    print 'Sample [sigma,r-value]'

#    for key in limDict:
#        if limDict[key][0] != -1:
#            print key, limDict[key]

    # make dict[sigma] = samplename
    sigDict = {}
    for key in limDict:
        if limDict[key][0] != -1:
            sigDict[limDict[key][0]] = key

    sigList = sigDict.keys()
    sigList.sort(reverse = True)

#    for sigma in sigDict:
#        print sigma, sigDict[sigma], limDict[sigDict[sigma]]
    for sigma in sigList:
        print sigDict[sigma],'\t', limDict[sigDict[sigma]][0],'\t', limDict[sigDict[sigma]][1]
