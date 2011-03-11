import os,sys
import ROOT
from DataFormats.FWLite import Events, Handle
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from CMGTools.HtoZZ2l2nu.localPatTuples_cff import fillFromCastor

#total run information
def getTotalRunInfo(fnames,ffile,lfile) :
    totalRunInfo=[0,0,0,0]
    for ifile in xrange(ffile,lfile) :
        file = ROOT.TFile.Open(fnames[ifile])
        runInfo = ROOT.getRunInfo(file)
        for i in xrange(0,4):
            totalRunInfo[i] += runInfo[i]
        print 'File #' + str(ifile) + ' has ' + str(runInfo[0]) + ' events'
        file.Close()
    return totalRunInfo

# info
if(len(sys.argv)<2):
    print 'python getRunInfoForLocalSample.py directory <first_file=0 last_file=total files in directory>'
    exit

url=sys.argv[1]
if(os.path.isdir(url)):
    #configure
    fnames = fillFromCastor(sys.argv[1])
    ffile=0
    if(len(sys.argv)>2) : ffile=int(sys.argv[2])
    lfile=len(fnames)
    if(len(sys.argv)>3) : lfile=int(sys.argv[3])
   
    #final results
    totalRunInfo = getTotalRunInfo(fnames,ffile,lfile)
    print ' ***** TOTAL ****** '
    for i in xrange(0,4):
        print str(i) + ' ' + str(totalRunInfo[i])
