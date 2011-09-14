#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 12th 2011

from datetime import datetime
from savannahConnect import savannahConnect
import sys,os, re, subprocess, datetime, castortools, castorBaseDir, glob, DBLogger, getpass
from optparse import OptionParser
from DBSAPI.dbsProcessedDataset import DbsProcessedDataset
from DBSAPI.dbsPrimaryDataset import DbsPrimaryDataset

 
if __name__ == '__main__':
    
    parser = OptionParser()

    parser.usage = """
%prog [options] <sampleName>
-----
sampleName should be given in the form /[PrimaryDS]/[ProcDS]/*[Parent]/[Tiers] 
-----
Use this script to publish dataset details to savannah.
-----
All selected datasets, must either have a logger directory with showtags.txt file on the local machine,
or a Logger.tgz directory on Castor
-----
If no -u option is provided, it is assumed that the current user is the files owner on Castor.
If no -s option is provided, it is assumed that the current user is the user on Savannah 
"""

    # If user is not specified default is current user
    # This option will be used to find dataset on castor, and assign dataset on savannah
    parser.add_option("-u", "--user", 
                      dest="user",
                      help="User who is the files owner on Castor." ,
                      default=os.environ['USER'] )

    # If specified is used to log in to savannah (only required if user that created the dataset,
    # is different to user publishing it)
    parser.add_option("-s", "--savuser",
                      action = "store",
                      dest="savuser",
                      help="If Savannah user is different to current user, enter Savannah username here",
                      default=os.environ['USER'] )
    # If the purpose is to test the software use this parameter, it will not be recognised by the
    # non-testing algorithm
    parser.add_option("-T", "--test",
                      action = "store_true",
                      dest="test",
                      help="Flag task as a test",
                      default=False )
    
    

    
    (options,args) = parser.parse_args()
    if len(args)!=1:
        parser.print_help()
        sys.exit(1)

    savpass = getpass.getpass("Enter Savannah Password: ")


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
    parent = None
    if len(path)>3:
        
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
                                  ParentList = [],
                                  CreatedBy = options.user,
                                  DateCreated = datetime.datetime.now().strftime("%s"),
                                  )

    if parent != None: dataset['ParentList'] = [parent,]

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


        #Open session in savannah using savannah pass & user
        sav = savannahConnect(options.savuser, savpass)

        #Submit dataset to savannah and assign to files owner on Castor
        savannahURL =sav.submitItem(dataset,files,tags,targetDir ,options.user, options.test)

    except ValueError as err:
        print err, '. Exit!'
        sys.exit(1)

