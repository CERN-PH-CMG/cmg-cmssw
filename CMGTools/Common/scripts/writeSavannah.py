#!/usr/bin/env python

from datetime import datetime
from savannahConnect import savannahConnect
import sys,os, re, subprocess, datetime, castortools, castorBaseDir, glob, DBLogger
from optparse import OptionParser
from DBSAPI.dbsProcessedDataset import DbsProcessedDataset
from DBSAPI.dbsPrimaryDataset import DbsPrimaryDataset

if __name__ == '__main__':
    
    parser = OptionParser()

    parser.usage = """
%prog [options] <sampleName>
"""

    # If user is not specified default is current user
    # This option will be used to find dataset on castor, and assign dataset on savannah
    parser.add_option("-u", "--user", 
                      dest="user",
                      help="User who is the owner of the castor base directory. Note that this user must have his/her ~/public/DataSets.txt up to date",
                      default=os.environ['USER'] )
    # If this is specified, a savannah record will be made with (either) the --user option as a username, or,
    # if specified, the --savuser option.
    parser.add_option("-s", "--savannah",
                      action = "store",
                      dest="savannah",
                      help="Add entry to Savannah with current user and this password",
                      default=None )
    # If specified is used to log in to savannah (only required if user that created the dataset,
    # is different to user publishing it)
    parser.add_option("--su", "--savuser",
                      action = "store",
                      dest="savuser",
                      help="If Savannah user is different to user on Castor, enter Savannah username here",
                      default=None )
    
    
    

    
    (options,args) = parser.parse_args()

    # Check if Savannah user and Castor user are different
    if options.savuser == None:
        options.savuser = options.user
    
    # Allow no more than one argument
    if len(args)!=1:
        parser.print_help()
        sys.exit(1)

    # Store full dataset name
    dirOrFile = args[0]

    # Castor base directory for specified user
    castorBaseDir = castortools.lfnToCastor(castorBaseDir.castorBaseDir(options.user))

    # Locations of Logger(.tgz) and containing folder on Castor
    targetDir = castorBaseDir + "/" + args[0].lstrip("/").rstrip("/")
    targetTgz = targetDir + "/Logger.tgz"

    # Get array containing 1) Primary dataset name; 2) Dataset Name; 3+) Tiers
    details = dirOrFile.lstrip("/").split("/")

    # If element is not Primary dataset of dataset name, add to the Tiers array
    count = 0
    tiers = []
    for i in details:
        if count>1:
            tiers.append(i)
        count +=1
    
    # Create Primary dataset object (will reference complete object on DBS)
    primary = DbsPrimaryDataset (Name = details[0])
    path= dirOrFile.lstrip("/").split("/")
    if len(path)>2:
        path[-1]=None
        parent = ""
        for i in path:
            if i:
                parent += "/" + i
    
    # Create Processed dataset object to be published on Savannah and recorded on DBS
    dataset = DbsProcessedDataset(PrimaryDataset = primary,
                                  Name = details[1],
                                  PhysicsGroup = "CMG",
                                  Status = "INVALID",
                                  TierList = tiers,
                                  AlgoList = [],
                                  RunList = [],
                                  PathList = [dirOrFile,],
                                  ParentList = [parent,],
                                  CreatedBy = options.user,
                                  DateCreated = datetime.datetime.now().strftime("%s"),
                                  )
                                  
    
    
    try:
        # Create DBLogger object to interact with Castor, DBS, and CMGDB
        log = DBLogger.DBLogger(dirOrFile, targetTgz, None)


        # Stage in logger file from Castor
        log.stageIn()
        # If the root files are contiguous, set Status to VALID
        
        tags = log.getTags()
       

        # Check if files are contiguous and set valid variable accordingly
        files = log.checkContiguity(targetDir)
        if files[-1] == "VALID":
                dataset['Status'] = "VALID"
        del files[-1]
        # Delete log file from local system
        log.stageOut()
        
        if options.savannah != None:
            #Open session in savannah using savannah pass & user
            sav = savannahConnect(options.savuser, options.savannah)

            #Submit dataset to savannah and assign to files owner on Castor
            savannahURL =sav.submitItem(dataset,files,tags, options.user)

            # Log the item's savannah url CMGDB
            log.addSavannah(dirOrFile, savannahURL)
        else: print "Please enter Savannah password in command line using option -s"
    except ValueError as err:
        print err, '. Exit!'
        sys.exit(1)

