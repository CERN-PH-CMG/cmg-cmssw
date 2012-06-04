#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

import os, getpass, sys, re, signal
from CMGTools.Production.castorToDbsFormatter import CastorToDbsFormatter
from CMGTools.Production.publishController import PublishController
from DBSAPI.dbsProcessedDataset import DbsProcessedDataset
from DBSAPI.dbsPrimaryDataset import DbsPrimaryDataset
from datetime import *
from CMGTools.Production.nameOps import *
from CMGTools.Production.findDSOnSav import getTaskID
from CMGTools.Production.fileOps import FileOps
from CMGTools.Production.castorBaseDir import getUserAndArea


def publish(dsName,fileown,comment,test,user,password, force, checkGroups, savannah, run_range = None):
    """Publish the given dataset to CMGDB and Savannah
    
    'dsName' takes the name of the dataset, in either format
    'fileown' takes the NICE username of the space on EOS in which the dataset resides
    'comment' takes a users comment for publishing to Savannah or None
    'test' takes True/False on whether the posting is a test or not
    'user' takes the NICE username of the person making the post
    'password' takes the NICE password of the person making the post
    'force' takes True/False on whether the dataset should be published if no log file exists
    'checkGroups' takes True/False on whether the associated group directory on EOS should be 
    checked if the Dataset does not exist in the use space
    'savannah' takes True/False on whether Savannah publish is desired
    """

    user, area = getUserAndArea(user)
    if area == 'group':
        checkGroups = True
    
    # Validate name, and escape if name is invalidate
    # Convert name to EOS format (castor)
    if re.search("---",dsName):
        fileown = getDbsUser(dsName)
        dsName2 = getCastor(dsName)
        if dsName2 is None:
            print "\nError, "+dsName+" is not valid, please use valid name\n"
            return None
        else:
            dsName = dsName2
    
    # Check the length of the dataset name	
    if len(dsName.lstrip(os.sep).rstrip(os.sep).split(os.sep)) < 3:
    	print "Error, "+dsName+" is not valid, please use valid name"
    	return None
    elif len(dsName.lstrip(os.sep).rstrip(os.sep).split(os.sep)) < 4:
    	print "Dataset "+ dsName + "is a CMS base dataset and cannot be published, please use DAS."
    	return None
    
    # THIS CAN BE SIMPLIFIED
    # Create a DBS object
    formatter = CastorToDbsFormatter(dsName,fileown)
    procds =formatter.getDataset()
    
    
    try:
        
        opts = dict()
        print "\n---Publish Dataset---"
        print "Retrieving dataset information...\n"
        
        # Create FileOps object
        fileOps = None
        try:
            fileOps = FileOps(dsName, fileown ,force, checkGroups, run_range)    
        except KeyboardInterrupt:
            raise
        
        # Create ANOTHER DBS object which is created using the updated user (could have been changed in FileOps)
        # CAN BE SIMPLIFIED
        formatter = CastorToDbsFormatter(dsName,fileOps.getUser())
        procds =formatter.getDataset()
        
        # Create opts for savannah
        # May be simplified
        dayMonthYear = date.fromtimestamp(int(procds['DateCreated'])).strftime('%d-%B-%Y').split("-")
        opts['planned_starting_date_dayfd']=dayMonthYear[0].lstrip("0")
        opts['planned_starting_date_monthfd']=dayMonthYear[1]    
        opts['planned_starting_date_yearfd']=dayMonthYear[2]
        
        # Create test category MAY BE REMOVED IF TEST NOT REQUIRED
        if test:
    	    opts['category_id']='101'
        else:opts['category_id']='103'
        
        # More savannah opts
        opts['assigned_to']=fileown
        opts['summary'] = procds['PathList'][0]
    
        # Initialise taskId's
    	taskID = None
    	parentTaskID = None
    	
    	# Initialise login and status to failed
    	loginClear = False
    	status = 'Failed'
    	
    	# Initialise PublishController
    	publishController = PublishController(user, password, force)
    	
    	# Attempt Login, if unexplained fail occurs, retry
    	try:
    	    loginClear = publishController.loginValid()
    	except KeyboardInterrupt:
    	    raise
    	except:
    	    try:
    	        loginClear = publishController.loginValid()
    	    except KeyboardInterrupt:
    	        raise
    	        
    	# If login fails return None
    	if loginClear is False:
    		print "User authentication failed, exiting\n\n"
    		return None
    	
    	# Print dataset names
    	print "\n------DataSet------\n"
    	print "CMGDB name:",getDbsWithUser(dsName, fileOps.getUser())
        print "EOS name:  ",dsName
    	
    	## Savannah operations
    	print "\n------Savanah------\n"
    	
    	# 
    	try:
    	    if taskID is None: taskID = getTaskID(procds['PathList'][0], opts['category_id'], user, password, False)
    	except:
    	    taskID = getTaskID(procds['PathList'][0], opts['category_id'], user, password, False)
    	
    	if savannah or taskID == None:
    	    (taskID, parentTaskID) = publishController.savannahPublish(procds, opts, comment, fileOps)
    	    print parentTaskID
    	else:
    	    print "NO SAVANNAH PUBLISH REQUIRED"
    	
   
    	if taskID is not None: status = 'Success'
    	if publishController.cmgdbOnline():
    		print "\n-------CMGDB-------\n"
    		cmgdbid = publishController.cmgdbPublish(procds, taskID, test, fileOps)
    			
    	return {'Status':status, 'Savannah':taskID,'CMGDB ID':cmgdbid,'DBS Dataset':procds['PathList'][0], 'EOS Dataset':dsName,'File Owner':fileown}
    except KeyboardInterrupt:
        raise
    except ValueError as err:
        print err.args, '.\nDataset not published'
        return None
    except NameError as err:
        print err.args[0]
        savannah = None
            
        return {'Status':'Failed','Savannah':savannah,'CMGDB ID':None, 'DBS Dataset':procds['PathList'][0], 'EOS Dataset':dsName, 'File Owner':fileown}



