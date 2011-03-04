import sys
import ROOT
from DataFormats.FWLite import Events, Handle
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libpsilvah2zz.so')
from psilva.h2zz.localPatTuples_cff import getLocalSourceFor
fnames, trigMenu, isData = getLocalSourceFor( sys.argv[1] )

ffile=0
if(len(sys.argv)>2) : ffile=int(sys.argv[2])
lfile=len(fnames)
if(len(sys.argv)>3) : lfile=int(sys.argv[3])

totalRunInfo=[0,0,0,0]
for ifile in xrange(ffile,lfile) :
    file = ROOT.TFile.Open(fnames[ifile])
    runInfo = ROOT.getRunInfo(file)
    for i in xrange(0,4):
        totalRunInfo[i] += runInfo[i]
    print 'File #' + str(ifile) + ' has ' + str(runInfo[0]) + ' events'
    file.Close()

#final results
print ' ***** TOTAL ****** '
for i in xrange(0,4):
    print str(i) + ' ' + str(totalRunInfo[i])

    
