import FWCore.ParameterSet.Config as cms
import os,sys
import getopt
import commands

"""
lists the files available in castor
"""
def fillFromCastor(dir,ffile=0,step=-1,generatePfn=True):

    localdataset=cms.untracked.vstring()
 
    #check if it is a directory (check if it is castor, eos or local)
    prefix='singlefile'
    lsout=[dir]
    if(dir.find('castor')>=0) :
        prefix='rfio'
        lscommand ='rfdir ' + dir + ' | awk \'{print $9}\''
        lsout = commands.getstatusoutput(lscommand)[1].split()
    elif(dir.find('/store/cmst3')==0):
        prefix='eoscms'
        lscommand = 'cmsLs ' + dir + ' | grep root | awk \'{print $5}\''
        lsout = commands.getstatusoutput(lscommand)[1].split()
    elif(dir.find('.root')<0):
        prefix='file'
        lscommand='ls ' + dir
        lsout = commands.getstatusoutput(lscommand)[1].split()

    #check for the files needed (first file, firstfile+step)
    ifile=0
    for line in lsout :
        if(type(line) is not str ) : continue
        if(len(line)==0) : continue
        if(line.find('root')<0) : continue

        if(ifile>=ffile):
            if( (step<0) or  (step>0 and ifile<ffile+step) ):
                
                sline=''
                if(prefix=='eoscms') :
                    if(generatePfn) : sline=commands.getstatusoutput('cmsPfn ' + line )[1]
                    else            : sline=line
                elif(prefix=='singlefile') :
                    sline='file://' + line
                else :
                    sline=str(prefix+'://' + dir + '/' + line.split()[0])
                    if(len(sline)==0): continue

                localdataset.extend( [ sline ] )
        ifile=ifile+1

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

                             
