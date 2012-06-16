#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## April 4th 2012

from CMGTools.Production.castorToDbsFormatter import CastorToDbsFormatter
from CMGTools.Production.savannahBrowser import SavannahBrowser
from CMGTools.Production.cmgdbToolsApi import CmgdbToolsApi
import os, getpass, sys, re
from CMGTools.Production.nameOps import *
from CMGTools.Production.findDSOnSav import *


def unPublish(dsName,fileown,user,password):
    
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
    
    formatter = CastorToDbsFormatter(dsName,fileown)
    procds =formatter.getDataset()

    taskID = None
    loginClear = False
    if not validLogin(user, password):
        print "Authentication Failed, exiting\n\n"
        sys.exit(1)
    	
    print "\n------DataSet------\n"
    print "CMGDB name:",getDbsWithUser(dsName, fileown)
    print "EOS name:  ",dsName
    	
    print "\n------Savanah------\n"
    	
    try:
        if taskID is None: taskID = getTaskID(procds['PathList'][0], opts['category_id'], user, password, False)
    except:
        taskID = getTaskID(procds['PathList'][0], '103', user, password, False)
    if taskID is not None:
        browser = SavannahBrowser(user,password, taskID)
        browser.closeTask()
        print "Task closed on Savannah"
        print "URL: https://savannah.cern.ch/task/?"+taskID
    else:
        print "No task was found, no closing neccesary"
    	 
    print "\n-------CMGDB-------\n"
    ID = None    
    cmgdbAPI=CmgdbToolsApi()
    cmgdbAPI.connect()
    ID = cmgdbAPI.getDatasetIDWithName(procds['PathList'][0])
    if ID is not None:
        try:
            cmgdbAPI.closeDataset(procds['PathList'][0])
            print "Dataset closed on CMGDB"
        except:
            print "Dataset failed to close"
    else:
        print "Dataset not found on CMGDB"
    	        			
    


