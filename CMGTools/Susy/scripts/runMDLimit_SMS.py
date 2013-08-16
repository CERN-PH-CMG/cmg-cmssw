#! /usr/bin/env python
import os
import sys
import ROOT as rt
from optparse import OptionParser
import time

pwd = os.environ['PWD']
#boxes = ['Had','Mu','Ele','MuMu','EleEle','MuEle']
if __name__ == '__main__':
    
    parser = OptionParser()
    parser.add_option('-q','--queue',dest="queue",type="string",default="1nh", help="Name of queue to use")
    parser.add_option('-t','--toys',dest="toys",type="int",default="40", help="Number of toys per job")
    parser.add_option('--tree_name',dest="tree_name",type="string",default='EVENTS', help="The name of the TTree to look at")    
    parser.add_option('-i','--index',dest="iJob",type="int",default="5", help="Integer index (to label outputs)")
    parser.add_option('-n','--number',dest="numJobs",type="int",default="100", help="number of jobs")
    parser.add_option('-b','--bjet',dest="doBjet", default=False, action='store_true', help="Run the RazorB analysis")
    parser.add_option('-m','--multijet',dest="doMultijet", default=False, action='store_true', help="Run the Razor Multijet analysis")    
    parser.add_option("--input",dest="input",default="razor_output.root", help="input file containing the bkg fits") 
    parser.add_option('--xsec',dest="xsec",type="float",default="-99", help="Signal cross section (in pb) for SMSs limit setting")
    parser.add_option('-c','--config',dest="config",type="string",default="config_winter2012/SingleBoxFit_Prompt_fR1fR2fR3fR4_2012_TightRsq.cfg", help="Name of the config file to use")
    parser.add_option('-f','--file',dest='inputFile',type='string',default='RAZORFITS/razor_output_all_Winter2012.root', help='Input file with the fit results for the bkg model')
    
    # the config files are committed in the CVS repository
    # INCLUSIVE: config_winter2012/SingleBoxFit_Prompt_fR1fR2fR3fR4_2012_TightRsq.cfg
    # BJET:      config_winter2012/RazorB_fR1fR2fR3fR4_winter2012_TightRsq.cfg
    # TAU:       
    # MULTIJET:  
    # DIPHOTON:  

    # the input file has to be provided. The default are
    # INCLUSIVE: /afs/cern.ch/user/m/mpierini/public/RAZORFITS/razor_output_all_Winter2012.root
    # BJET:      /afs/cern.ch/user/m/mpierini/public/RAZORFITS/razor_output_BJET_all_Winter2012.root
    # TAU:       
    # MULTIJET:  
    # DIPHOTON:  
    # make a local copy, not to get AFS paralized
    
    (options,args) = parser.parse_args()
    
    print 'Input files are %s' % ', '.join(args)

    toys = options.toys
    xsec = options.xsec
    input = options.input
    script = "Chris2BinnedDataset_ALLBOXES_BYPROCESS.py"
    treeName = options.tree_name

    
    for signalpath in args:

        mStop = int(treeName.split('_')[-2])
        mLSP = int(treeName.split('_')[-1])
        signal = 'SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z-Summer11-PU_START42_V11_FastSim-v1-wreece_080212-SMS-ByPoint'
        print "Creating jobs for %s\n" % ('SMS-T2tt-%i-%i' % (mStop,mLSP))

        for i in range(options.iJob,options.iJob+options.numJobs):
            outputFileName = "%s_%s_xsec_%f" %(signal, options.tree_name, xsec)
            outputname = "%s_%i.src" %(outputFileName,i)
            outputfile = open(outputname,'w')
            outputfile.write('#!/bin/bash\n')
            outputfile.write('cd '+pwd+'\n')
            outputfile.write("eval `scramv1 run -sh`\n")
            mydir = "/tmp/wreece/%s_%s_xsec_%f_%i" %(signal,options.tree_name,xsec,i)
            outputfile.write("mkdir %s\n" %mydir)
            outputfile.write("cd %s\n" %mydir)
            outputfile.write("cmsStage /store/cmst3/user/wreece/Razor2011/MultiJetAnalysis/everything.tgz .\n")            
            outputfile.write("tar zxvf everything.tgz\n")
            outputfile.write("cd RazorCombinedFit\n")
            outputfile.write("source setup.sh\n")
            outputfile.write("make\n")

            # run options
            runOptions = ""
            makePDFOptions = ""
            if options.doBjet:
                runOptions = runOptions+" --bjet"
                makePDFOptions = makePDFOptions +" --b 1"
            elif options.doMultijet:
                runOptions = runOptions+" --multijet"

            # convert original signal file to box-by-box datasets
            pid = os.getpid()
            now = rt.TDatime()
            today = now.GetDate()
            clock = now.GetTime()
            seed = today+clock+pid+137*i
            # run SMS
            outputfile.write("python scripts/%s %s --tree_name %s -c %s --sms -t %i %s %s >& /dev/null\n" %(script, runOptions, options.tree_name, options.config, toys, signalpath,makePDFOptions))
            # perform limit toys(signal + bkgd) setting fits
            outputfile.write("python scripts/runAnalysis.py -a SingleBoxFit --xsec %f -s %i -c %s -o %s/LimitBkgSigToys_%s_%s_%i.root -i %s %s/RazorCombinedFit/%s_MR*.root -b --limit -t %i %s >& /dev/null \n" %(xsec,seed,options.config,mydir,outputFileName,options.tree_name,i,input,mydir,signal,toys,runOptions))
            # perform limit toys(bkgd only) setting fits
            outputfile.write("python scripts/runAnalysis.py -a SingleBoxFit --xsec %f -s %i -c %s -o %s/LimitBkgToys_%s_%s_%i.root -i %s %s/RazorCombinedFit/%s_MR*.root -b --limit -e -t %i %s >& /dev/null \n" %(xsec,seed,options.config,mydir,outputFileName,options.tree_name,i,input,mydir,signal,toys,runOptions))
            # copy output files
            outputfile.write("cmsStage -f %s/LimitBkgSigToys_%s_%s_%i.root /store/cmst3/user/wreece/Razor2011/MultiJetAnalysis/scratch0/T2tt\n" %(mydir,outputFileName,options.tree_name,i))
            outputfile.write("cmsStage -f %s/LimitBkgToys_%s_%s_%i.root /store/cmst3/user/wreece/Razor2011/MultiJetAnalysis/scratch0/T2tt\n" %(mydir,outputFileName,options.tree_name,i))
            # remove output files                                                          
            outputfile.write("cd /tmp; rm -rf %s\n" %(mydir))
            outputfile.close
            # submit to batch
            name = '%s_%f' % (options.tree_name,xsec)
            os.system("echo bsub -q "+options.queue+" -J "+name+" source  "+pwd+"/"+outputname)
            os.system("bsub -q "+options.queue+" -J "+name+" -o /dev/null -e /dev/null source source "+pwd+"/"+outputname)
            #os.system("bsub -q "+options.queue+" -o job_"+str(i)+".log source "+pwd+"/"+outputname)
            #time.sleep(0.1)
            #os.system("gzip "+pwd+"/"+outputname)
            continue
