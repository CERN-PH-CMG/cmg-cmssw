import FWCore.ParameterSet.Config as cms
import os,sys
import getopt
import commands

"""
lists the files available in castor
"""
def fillFromCastor(dir,ffile=0,step=-1):
 
    #a root file is already there
    if(dir.find(".root")>=0):
        localdataset=cms.untracked.vstring()
        localdataset.extend( [ str(dir) ] )
        return localdataset

    #it is a directory (check if it is castor or not)
    prefix='rfio'
    if(dir.find('castor')>=0) :
        os.system('rfdir ' + dir + ' | awk \'{print $9}\' > /tmp/castorDump')
    elif(dir.find('/store/cmst3')==0):
        prefix='eoscms'
        os.system('a=(`cmsLs ' + dir + ' | grep root | awk \'{print $5}\'`); for i in ${a[@]}; do cmsPfn ${i} >> /tmp/castorDump; done')
    else:
        prefix='file'
        os.system('ls ' + dir + ' > /tmp/castorDump')
    inFile = open('/tmp/castorDump', 'r')
    localdataset=cms.untracked.vstring()
    ifile=0
    for line in inFile.readlines():
        if(len(line)==0) : continue
        if(line.find('root')<0) : continue
        if(line.find('histograms')>0 or line.find('monitor')>0): continue
        sline=''
        if(prefix=='eoscms') :
            sline=line
        else :
            sline=str(prefix+'://' + dir + '/' + line.split()[0])
        if(len(sline)==0): continue
        if(ifile>=ffile):
            if( (step<0) or  (step>0 and ifile<ffile+step) ):
                localdataset.extend( [ sline ] )
        ifile=ifile+1

    os.system('rm /tmp/castorDump')
    return localdataset


"""
wrapper to read the configuration from comand line
args: castor_directory output_file first_file step
"""
def configureFromCommandLine() :
    castorDir='/castor/cern.ch/cms/store/cmst3/user/cbern/CMG/TT_TuneZ2_7TeV-pythia6-tauola/Summer11-PU_S3_START42_V11-v2/AODSIM'
    outputFile='output.root'
    ffile=0
    step=-1

    if(len(sys.argv)>2 ):
        if(sys.argv[2].find('/')>=0 or sys.argv[2].find('.root')>0) :
            castorDir=sys.argv[2]
            if(len(sys.argv)>3 ):
                if(sys.argv[3].find('.root')>0):  outputFile=sys.argv[3]
                if(len(sys.argv)>4 ):
                    if(sys.argv[4].isdigit()) : ffile=int(sys.argv[4])
                    if(len(sys.argv)>5 ):
                        if(sys.argv[5].isdigit()) : step=int(sys.argv[5])

    return castorDir, outputFile, fillFromCastor(castorDir,ffile,step)

                             
