#!/usr/bin/env python

##
## small script to submit the calibration (using massfitter2.py) step to the bacth system
## it takes an output directory, and optionally the number of toy experiments
## the samplesize and the channel
##

import os
import optparse


#masses = [ '1615','1635','1665','1695','1725','1755','1785','1815','1845' ]
#masses = [ '1635','1665','1695','1725','1755','1785','1815','1845']
masses = ['dyup','dydown']

if __name__ == '__main__':

    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option('-o', '--outputdir' ,    dest='outputdir'      , help='Name of the local output directory for your jobs. This directory will be created automatically.'    , default=None)
    parser.add_option('-n', '--ntoys'     ,    dest='ntoys'          , help='Number of toy experiments.'             , default=500  ,type = int)
    parser.add_option('-s', '--samplesize',    dest='samplesize'     , help='Number of events per toy experiment.'   , default=0    ,type = int)
    parser.add_option('-c', '--channel'   ,    dest='channel'        , help='If only one channel is processed.'      , default=None)

    (opt, args) = parser.parse_args()


    if opt.outputdir is None:
        parser.error('No outputdir defined!')
    if opt.channel is None:
        parser.error('Pease give a channel!')


        
    outdir = opt.outputdir
    chan = opt.channel
    ntoys = opt.ntoys
    samplesize = opt.samplesize

    pwd = os.getcwd()

    if not os.path.isdir(outdir):
        os.mkdir(outdir)
    
    for mass in masses:
        sub = open(outdir+'/submit_'+chan+'_'+mass+'.sh','w')
        sub.write('#!/bin/bash\n')
        sub.write('cd '+pwd+'\n')
        sub.write('eval `scramv1 ru -sh`\n')
        sub.write('./scripts/massfitter2.py toys -i FitSecVtxDistributionsWS.root -c '+str(chan)+' -m '+str(mass)+' -n '+str(ntoys)+' -s '+str(samplesize)+'\n')
        sub.write('mv *_'+str(chan)+'_'+str(mass)+'.* '+outdir)
        sub.close()
        os.system('chmod +x '+outdir+'/submit_'+chan+'_'+mass+'.sh')

    os.chdir(outdir)
    for mass in masses:
        os.system('bsub -q 1nd submit_'+chan+'_'+mass+'.sh')

