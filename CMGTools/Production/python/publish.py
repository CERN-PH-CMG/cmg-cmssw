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


def publish(dsName,fileown,comment,test,dbsApi,user,password, force, checkGroups, savannah):
    
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
    try:
        # Store full dataset name
    
        opts = dict()
        print "\n---Publish Dataset---"
        print "Retrieving dataset information...\n"
        

        def timeout_handler(signum, frame):
            raise OSError("Restarting connection to EOS, due to timeout\n")
        completed = False
        fileOps = None
        while True:
            signal.signal(signal.SIGALRM, timeout_handler) 
            signal.alarm(60) # triger alarm in 3 seconds
        
            try:
                fileOps = None
                fileOps = FileOps(dsName, fileown ,force, checkGroups)
                break
            except KeyboardInterrupt:
                raise
            except OSError as io:
                print io.args[0]
                signal.alarm(0)
        signal.alarm(0)
                
                
        formatter = CastorToDbsFormatter(dsName,fileOps.getUser())
        procds =formatter.getDataset()
        dayMonthYear = date.fromtimestamp(int(procds['DateCreated'])).strftime('%d-%B-%Y').split("-")
        opts['planned_starting_date_dayfd']=dayMonthYear[0].lstrip("0")
        opts['planned_starting_date_monthfd']=dayMonthYear[1]    
        opts['planned_starting_date_yearfd']=dayMonthYear[2]
        if test:
    	    opts['category_id']='101'
        else:opts['category_id']='103'
        opts['assigned_to']=fileown
        opts['summary'] = procds['PathList'][0]
    
    
    
    	dbsID = None
    	parentDbsID = None
    	taskID = None
    	parentTaskID = None
    	loginClear = False
    	status = 'Failed'
    	publishController = PublishController(user, password, force)
    	try:
    	    loginClear = publishController.loginValid()
    	except KeyboardInterrupt:
    	    raise
    	except:
    	    try:
    	        loginClear = publishController.loginValid()
    	    except KeyboardInterrupt:
    	        raise
    	if loginClear is False:
    		print "User authentication failed, exiting\n\n"
    		return None
    	
    	print "\n------DataSet------\n"
    	print "CMGDB name:",getDbsWithUser(dsName, fileOps.getUser())
        print "EOS name:  ",dsName
    	
    	print "\n------Savanah------\n"
    	
    	try:
    	    if taskID is None: taskID = getTaskID(procds['PathList'][0], opts['category_id'], user, password, False)
    	except:
    	    taskID = getTaskID(procds['PathList'][0], opts['category_id'], user, password, False)
    	
    	if savannah or taskID == None:
    	    
    	    (taskID, parentTaskID) = publishController.savannahPublish(procds, opts, comment, fileOps)
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



