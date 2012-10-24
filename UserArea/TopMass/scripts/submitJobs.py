#!/usr/bin/env python

import sys
import optparse
import os

##import UserCode.TopMass.topinfo

##sys.path.append('/afs/cern.ch/user/j/jueugste/cmssw/top/CMGTools/CMSSW_5_3_3_patch3/src/CMGTools/UserCode/TopMass/python')
sys.path.append('../python')
import topinfo


if __name__ == '__main__':
    print topinfo.samples
      
    
    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    
    #    parser.add_option('-f'               ,     dest='force'          , help='Tag used for the shape file name'                          , default='')
    parser.add_option('-n', '--nfiles' ,       dest='nfiles'         , help='Number of files per job. Default is 100.'                                  , default=100)
    parser.add_option('-o', '--outputdir' ,    dest='outputdir'      , help='Name of the local output directory for your jobs. This directory will be created automatically.'    , default=None)
    parser.add_option('-r', '--remotecopy' ,   dest='remotecopy'     , help='Remote output directory for your jobs. Something of the form \"/store/cmst3/user/USERNAME/bla/bla/bla/\"'    , default=None)
    
    (opt, args) = parser.parse_args()

    if opt.outputdir is None:
        parser.error('No output directory defined!')
    if opt.remotecopy is None:
        parser.error('No remote output directory defined!')

    nfiles = opt.nfiles
    outputdir = opt.outputdir
    remotecopy = opt.remotecopy

    for sample in topinfo.samples:
        samplename = sample.split('_Tune')[0].replace('/','')
        spec = 'sampleName='+samplename+' sampleLocation='+sample+'\''
        
        cmd = 'cmsBatch.py '+str(nfiles)+' skimLeptonJets.py -c \''+spec+' -o \''+outputdir+'/'+samplename+'\' -r '+remotecopy+' -b \'bsub -q 2nd < ./batchScript.sh\' '
        print cmd
        os.system(cmd)
