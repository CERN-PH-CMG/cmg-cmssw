#!/usr/bin/env python

import sys
import optparse
import os
import subprocess

##import UserCode.TopMass.topinfo

##sys.path.append('/afs/cern.ch/user/j/jueugste/cmssw/top/CMGTools/CMSSW_5_3_3_patch3/src/CMGTools/UserCode/TopMass/python')
sys.path.append('../python')
sys.path.append('python')
import topinfo


def get_cmsLsList(directory, tag):
    print 'looking into: '+directory+'...'
    #print 'searching files containig: '+tag

    cmd = 'cmsLs' 
    data = subprocess.Popen([cmd,directory], stdout=subprocess.PIPE)
    a = [f.split()[-1].split('/')[-1] for f in data.stdout if len(f.split())>0]

    ## strip the list of files
    if tag != '':
        b = [s for s in a if tag in s]
        return b
    ## if no tag given, run over all files
    else:
        return a

if __name__ == '__main__':
    print topinfo.samples
      
    
    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    
    #    parser.add_option('-f'               ,     dest='force'          , help='Tag used for the shape file name'                          , default='')
    parser.add_option('-n', '--nfiles' ,       dest='nfiles'         , help='Number of files per job. Default is 10.'                                  , default=10)
    parser.add_option('-o', '--outputdir' ,    dest='outputdir'      , help='Name of the local output directory for your jobs. This directory will be created automatically.'    , default=None)
    parser.add_option('-i', '--inputdir' ,    dest='inputdir'      , help='Name of the directory containing the input trees.'    , default=None)
    parser.add_option('-c', '--config' ,      dest='config'        , help='Name of the configuration to cmsRun.'    , default=None)
    parser.add_option('-t', '--tag' ,         dest='tag'           , help='Tag. This is the name of the sample to run over. i.e. a string contained in the sample name.'    , default='')
    parser.add_option('-r', '--remotecopy' ,   dest='remotecopy'     , help='Remote output directory for your jobs. Something of the form \"/store/cmst3/user/USERNAME/bla/bla/bla/\"'    , default=None)
    
    (opt, args) = parser.parse_args()

    if opt.outputdir is None:
        parser.error('No output directory defined!')
    if opt.remotecopy is None:
        parser.error('No remote output directory defined!')
    if opt.config is None:
        parser.error('No config file specified!')
        

    nfiles = opt.nfiles
    outputdir = opt.outputdir
    remotecopy = opt.remotecopy
    inputdir = opt.inputdir
    tag = opt.tag
    config = opt.config
    
    if config == 'skimLeptonJets.py':
        for sample in topinfo.samples:
            print sample
            samplename = sample.split('_Tune')[0].replace('/','')
            spec = 'sampleName='+samplename+' sampleLocation='+sample+'\''
        
            cmd = 'cmsBatch.py '+str(nfiles)+' skimLeptonJets.py -f -c \''+spec+' -o \''+outputdir+'/'+samplename+'\' -r '+remotecopy+' -b \'bsub -q 2nd < ./batchScript.sh\' '
            print cmd
            os.system(cmd)

    if config == 'finalSelLeptonJets.py':

        ## make one job per smaple in order to aovid problems with the numbering
        ## FIXME: can maybe be fixed at some point...
        nfiles = 10000
        

        ## if no tag given, loop over all samples
        if tag == '':
            samples = set([f.split('_treeCMG')[0] for f in get_cmsLsList(inputdir, tag)])
            print 'Samples found:'
            for sample in samples:
                print sample
            print '-' * 100
            
        else:
            samples = [tag]

        ## loop over the samples list
        for sample in samples:
            print '-' * 100
                     
            ## get the list from SE
            list = get_cmsLsList(inputdir, sample)
            #print list
        
            ## split the list in chunks according to nfiles
            splitlist = [list[i:i+int(nfiles)] for i in range(0,len(list),int(nfiles))]
            #print splitlist
        
            for st in splitlist:
                separator = ' inputFiles='+inputdir+'/'
                infiles = 'inputFiles='+inputdir+'/'+separator.join(st)
                #print infiles
                spec = 'sampleName='+sample+' maxEvents=-1 '+infiles
                #print spec
                cmd = 'cmsBatch.py '+str(1000)+' finalSelLeptonJets.py -f -c \''+spec+'\' -o '+outputdir+'/'+sample+' -r '+remotecopy+' -b \'bsub -q 2nd < ./batchScript.sh\' '
                print cmd
                os.system(cmd)


                
    else:
        print 'Unknown config file...'


        ## Have the following problems:
        ##
        ## - have to pass -n 100000
        ## - 
