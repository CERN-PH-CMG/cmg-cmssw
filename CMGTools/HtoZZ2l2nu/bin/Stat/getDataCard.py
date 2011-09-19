#!/usr/bin/env python

import ROOT
import os,sys
import getopt
from math import sqrt,pow,fabs
myscriptpath=os.path.expandvars('${CMSSW_BASE}/bin/${SCRAM_ARCH}')
sys.path.append( myscriptpath )
from rounding import toLatexRounded

def usage() :
    print ' '
    print 'getSystematics.py [options]'
    print '  -i : input file'
    print ' '
    
#parse the options
try:
    # retrive command line options
    shortopts  = "i:h?"
    opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
    # print help information and exit:
    print "ERROR: unknown options in argument %s" % sys.argv[1:]
    usage()
    sys.exit(1)

inputFile='plotter.root'
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(1)
    elif o in('-i'): inputFile = a

countHisto='finaleventflow'
masspts=['200','300','400','500','600']
evCats=['mumu','ee']
evSubCats=['eq0jets','eq1jets','geq2jets']
systs={"jesup":["jesup"],
       "jesdown":["jesdown"],
       "factscale":["hfactup","hfactdown"],
       "renscale":["hrenup","hrendown"],
       "pileup":["flatpu"]}

sigXsecUnc=[1.1067, 1.1083, 1.1131, 1.1192, 1.1265]

bckgs={
    "zz":"ZZ",
    "ww":"WW",
    "wz":"WZ",
    "st":"Single top",
    "ttbar":"t#bar{t}",
    "wjets":"W+jets",
    "dy":"Z-#gamma^{*}+jets#rightarrow ll"
    }

bckgXsecUnc={
    "zz":1.0254,
    "ww":1.0349,
    "wz":1.0385,
    "st":1.0334,
    "ttbar":1.0606,
    "wjets":1.04976,
    "dy":1.04345
    }


f = ROOT.TFile.Open(inputFile)
for imp in xrange(0,len(masspts)):

    fdatacardName='ggdatacard_'+masspts[imp]+'.txt'

    fdatacard=open(fdatacardName,'w')

    fdatacard.write( '# Counting experiment with multiple channels for mass point: ' + str(masspts[imp]) + '\n')
    fdatacard.write( 'imax ' + str(len(evCats)*len(evSubCats))  + '\n')
    fdatacard.write( 'jmax *'  + '\n')
    fdatacard.write( 'kmax *'  + '\n')
    fdatacard.write( '----------------------------------'  + '\n')
    
    fdatacard.write( 'Channels and observed events'  + '\n')
    binDefString   = 'bin '
    binCountString = 'observation '
    for ec in evCats:
        for esc in evSubCats :
            h=f.Get("data/" + ec + esc + "_" +countHisto)
            binDefString += ec+esc + ' '
            binCountString += str( h.GetBinContent(imp+1) ) + ' '
    fdatacard.write( binDefString  + '\n')
    fdatacard.write( binCountString  + '\n')
    fdatacard.write( '-----------------------------------'  + '\n')
    fdatacard.write( '# now we list the expected events for signal and all backgrounds in the different bins'  + '\n') 
    fdatacard.write( '# processes are signaled as follows: 0 = signal, >0 = backgrounds'  + '\n')
    fdatacard.write( '# rates are normalized to an hypothetical cross section in units of pb'  + '\n')
    fdatacard.write( '# absolute limits will be given in units of pb'  + '\n')
    fdatacard.write( '# follow independent sources of uncertainties and their effect (syst.error) for each process and category'  + '\n')

    sigDir='H('+masspts[imp]+')#rightarrow VV'
 
    binRow          = 'bin     '
    processRow      = 'process '
    processCountRow = 'process '
    rateRow         = 'rate    '
    lumiRow         = 'lumi    lnN '
    mcStatsRow      = 'mcstats lnN '
    thxsecRow       = 'thxsec  lnN '
    systsRows       = {}
    for isyst in systs.items(): systsRows[isyst[0]]= isyst[0] + ' lnN '
    
    for ec in evCats:
        for esc in evSubCats :

            binRow     += ec+esc + ' '


            #signal  yields and systematics
            h=f.Get(sigDir+"/" + ec + esc + "_" +countHisto)
            centralSignal      = h.GetBinContent(imp+1)
            centralSignalError=1.0
            if(centralSignal>0 ): centralSignalError += h.GetBinError(imp+1)/centralSignal
            
            processRow      += 'higgs '
            processCountRow += '{0:d} '.format(0)
            rateRow         += '{0:.4f} '.format(centralSignal)
            lumiRow         += '{0:.4f} '.format(1.045)
            mcStatsRow      += '{0:.4f} '.format(centralSignalError)
            thxsecRow       += '{0:.4f} '.format(sigXsecUnc[imp])
            
            for isyst in systs.items():
                systvars=[]
                for ivar in isyst[1] :
                     h=f.Get(sigDir+"/" + ec + esc + "_"+ ivar  +countHisto)
                     signalYieldVar=0;
                     if(centralSignal>0)   : signalYieldVar      += h.GetBinContent(imp+1)/centralSignal
                     if(signalYieldVar==0) : signalYieldVar=1.0
                     systvars.append(fabs(signalYieldVar))
                systsRows[ isyst[0] ] += '{0:.4f} '.format( max(systvars) )

            #background yields and systematics
            catBckgSummary={}
            ibckgCtr=0
            for ibckg in bckgs.items() :
                ibckgCtr=ibckgCtr+1
                h=f.Get(ibckg[1]+"/" + ec + esc + "_" +countHisto)
                centralBckg=h.GetBinContent(imp+1)
                centralBckgError=1.0
                if(centralBckg>0) : centralBckgError+=h.GetBinError(imp+1)/centralBckg

                processRow      += format(ibckg[0]) + ' '
                processCountRow += '{0:d} '.format(ibckgCtr)
                rateRow         += '{0:.4f} '.format(centralBckg)
                lumiRow         += '{0:.4f} '.format(1.045)
                mcStatsRow      += '{0:.4f} '.format(centralBckgError)
                thxsecRow       += '{0:.4f} '.format(bckgXsecUnc[ibckg[0]])
                
                for isyst in systs.items():
                    systvars=[]
                    for ivar in isyst[1] :
                        h=f.Get(ibckg[1]+"/" + ec + esc + "_"+ ivar  +countHisto)
                        bckgYieldVar=0;
                        if(centralBckg>0) :   bckgYieldVar   += h.GetBinContent(imp+1)/centralBckg
                        if(bckgYieldVar==0) : bckgYieldVar  =1.0
                        systvars.append(fabs(bckgYieldVar))
                    systsRows[ isyst[0] ] += '{0:.4f} '.format( max(systvars) )
        
    fdatacard.write( binRow  + '\n')
    fdatacard.write( processRow  + '\n')
    fdatacard.write( processCountRow  + '\n')
    fdatacard.write( rateRow  + '\n')
    fdatacard.write( '----------------------------'  + '\n')
    fdatacard.write( lumiRow  + '\n')
    fdatacard.write( mcStatsRow  + '\n')
    fdatacard.write( thxsecRow  + '\n')
    for isyst in systsRows.items() : fdatacard.write( isyst[1]  + '\n')

    fdatacard.close()

    print ' ****** Created: ' + fdatacardName

f.Close()


    
