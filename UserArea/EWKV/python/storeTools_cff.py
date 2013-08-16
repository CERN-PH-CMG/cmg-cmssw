import FWCore.ParameterSet.Config as cms
import os,sys
import getopt
import commands

"""
lists the files available in castor
"""
def fillFromStore(dir,ffile=0,step=-1,generatePfn=True):

    localdataset=cms.untracked.vstring()
    if(len(dir)==0) : return localdataset
 
    #check if it is a directory (check if it is castor, eos or local)
    prefix='singlefile'
    lsout=[dir]
    if(dir.find('path=')>=0) :
        print 'Using dbs to query %s'%(dir)
        prefix='eoscms'
        lsout=commands.getstatusoutput('dbs lsf --' + dir)[1].split()

    elif(dir.find('castor')>=0) :
        prefix='rfio'
        lscommand ='rfdir ' + dir + ' | awk \'{print $9}\''
        lsout = commands.getstatusoutput(lscommand)[1].split()

    elif(dir.find('/store/')==0):
        prefix='eoscms'
        lscommand = 'cmsLs ' + dir + ' | grep root | awk \'{print $5}\''
        lsouttmp = commands.getstatusoutput(lscommand)[1].split()

        #this is needed as cmsLs lists twice files staged from castor
        #(only needed during transition to EOS, can remove now)
        lsout=[]
        nduplicate=0
        for l in lsouttmp :
            if len(l)==0 : continue
            if l in lsout :
                nduplicate += 1
                continue
            #filter out CMG trees and histograms
            basename = os.path.basename(l)
            if(basename.find('tree_')==0) : continue
            if(basename.find('histogram')==0): continue
            lsout.append(l)
        print 'Discarded ' + str(nduplicate)  + ' files duplicated in cmsLs output'
       
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
check store for duplicates
"""
def checkStoreForDuplicates(outdir):
    rfdir_cmd = "rfdir " + outdir
    isEOS=False
    if(outdir.find('/store/cmst3')==0) :
        isEOS=True
        splitOnString=','
        rfdir_cmd='cmsLs ' + outdir + ' | grep root | awk \'{print $5}\''	
    nOutFile = 0
    outCastorDir_out = commands.getstatusoutput(rfdir_cmd)
    jobNumbers = []
    duplicatedJobs = []
    origFiles=[]
    duplicatedFiles=[]
    if outCastorDir_out[0] == 0:
        castorLines = outCastorDir_out[1].split("\n")
        if len(castorLines) != 0:
            for castorFileLine in castorLines:
                fileName=castorFileLine
                if "root" in castorFileLine:

		    if(checkInputFile(fileName)==True):

	                    if(not isEOS) : fileName = castorFileLine.split()[8]

        	            jobNumber=-1
                	    try:
                        	fileBaseName=os.path.basename(fileName)
	                        jobNumber=int(fileBaseName.split("_")[1])
        	            except:
                	        continue

	                    if jobNumber in jobNumbers:
        	                if not jobNumber in duplicatedJobs:  duplicatedJobs.append(jobNumber)
                	        duplicatedFiles.append(fileName)
	                    else :
        	                jobNumbers.append(jobNumber)
                	        origFiles.append(fileName)
                        	nOutFile += 1
		    else:
			    print("   #corrupted file found : " + fileName)
			    duplicatedFiles.append(fileName)
    return duplicatedFiles


"""
clean up for duplicats in the storage area
"""
def removeDuplicates(dirtag):
    duplicatedFiles=checkStoreForDuplicates(dirtag)
    print 'Removing ' + str(len(duplicatedFiles)) + ' duplicated files in ' + dirtag
    isEOS=False
    if(dirtag.find('/store/cmst3')==0) : isEOS=True
    for f in duplicatedFiles :
        print f
        if(isEOS) : commands.getstatusoutput('cmsRm ' + f)
        else : commands.getstatusoutput('rfrm ' +outdir + '/' + f)

            
"""
wrapper to read the configuration from comand line
args: castor_directory output_file first_file step
"""
def configureSourceFromCommandLine() :
    storeDir=''
    outputFile='Events.root'
    ffile=0
    step=-1
    try:
        if(len(sys.argv)>2 ):
            if(sys.argv[2].find('/')>=0 or sys.argv[2].find('.root')>0) :
                storeDir=sys.argv[2]
                if(len(sys.argv)>3 ):
                    if(sys.argv[3].find('.root')>0):
                        outputFile=sys.argv[3]
                    if(len(sys.argv)>4 ):
                        if(sys.argv[4].isdigit()) : ffile=int(sys.argv[4])
                        if(len(sys.argv)>5 ):
                            if(sys.argv[5].isdigit()) : step=int(sys.argv[5])
    except:
        print '[storeTools_cff] Could not configure from command line, will return default values'

    return outputFile, fillFromStore(storeDir,ffile,step)
