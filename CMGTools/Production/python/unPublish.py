#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## April 4th 2012

#from CMGTools.Production.castorToDbsFormatter import CastorToDbsFormatter
from CMGTools.Production.savannahBrowser import SavannahBrowser
from CMGTools.Production.cmgdbToolsApi import CmgdbToolsApi
import os, getpass, sys, re
from CMGTools.Production.nameOps import *
from CMGTools.Production.findDSOnSav import *
from CMGTools.Production.dataset import *

def unPublish( dsName, fileown, user, password, development=False ):
    
    if re.search("---",dsName):
        fileown = getDbsUser(dsName)
        dsName2 = getCastor(dsName)
        if dsName2 is None:
            print "\nError, "+dsName+" is not valid, please use valid name\n"
            return None
        else:
            dsName = dsName2
    	
    if len(dsName.lstrip(os.sep).rstrip(os.sep).split(os.sep)) < 3:
    	print "Error, "+dsName+" is not valid, please use valid name"
    	return None
    elif len(dsName.lstrip(os.sep).rstrip(os.sep).split(os.sep)) < 4:
    	print "Dataset "+ dsName + "is a CMS base dataset and cannot be published, please use DAS."
    	return None
    
    taskID = None
    loginClear = False
    if not validLogin(user, password):
        print "Authentication Failed, exiting\n\n"
        sys.exit(1)

    dataset = None
    try:
        dataset = Dataset( dsName, fileown ) #create the dataset
    except: 
        print "Dataset does not exist on eos, proceeding to unpublish"

    cmgdbName = getCMGDBWithUser( dsName, fileown )
    print "\n------DataSet------\n"
    print "CMGDB NAME  : " + cmgdbName
    if dataset:
	dataset.printInfo()
    	if dataset.files:               #check if the dataset has root files
        	print "\nERROR: Dataset has root files present, you can only unpublish empty datasets!" 
#if it does cancel the unpublishing
        	exit( -1 ) 
    	
    print "\n------Savanah------\n"
    	
    try:
        if taskID is None: taskID = getTaskID( cmgdbName, opts['category_id'], user, password, False)
    except:
        taskID = getTaskID( cmgdbName, '103', user, password, False)
    if taskID is not None:
        browser = SavannahBrowser( user, password, taskID )
        browser.closeTask()
        print "Task closed on Savannah"
        print "URL: https://savannah.cern.ch/task/?"+taskID
    else:
        print "No task was found, no closing neccesary"
    	 
    print "\n-------CMGDB-------\n"
    ID = None    
    cmgdbAPI=CmgdbToolsApi(development)
    cmgdbAPI.connect()
    ID = cmgdbAPI.getDatasetIDWithName( cmgdbName )
    if ID is not None:
        try:
            cmgdbAPI.closeDataset( cmgdbName )
            print "Dataset closed on CMGDB"
        except:
            print "Dataset failed to close"
    else:
        print "Dataset not found on CMGDB"
    	        			
    


