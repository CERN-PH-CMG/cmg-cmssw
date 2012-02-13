#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

import os, getpass, sys, re
from CMGTools.Production.castorToDbsFormatter import CastorToDbsFormatter
from CMGTools.Production.publishController import PublishController
from DBSAPI.dbsProcessedDataset import DbsProcessedDataset
from DBSAPI.dbsPrimaryDataset import DbsPrimaryDataset
from datetime import *
from DBSAPI.dbsOptions import DbsOptionParser
from DBSAPI.dbsApi import DbsApi
from DBSAPI.dbsApiException import *
from DBSAPI.dbsException import *
from CMGTools.Production.nameOps import *
from CMGTools.Production.findDSOnSav import getTaskID



def publish(dsName,fileown,comment,test,dbsApi,user,password, force):
    
    # if len(dsName.lstrip(os.sep).rstrip(os.sep).split(os.sep)) == 3 and dsName.split(os.sep)[0].endswith("%"):
#     	# Get list of samples
#     	user = dsName.split(os.sep)[0].rstrip('%')
#     	dataSets = '/afs/cern.ch/user/{first}/{user}/public/DataSets.txt'.format(
#     	first = user[0], # first letter of the username
#     	user = user
#     	)
#     	
#     	ifile=open(dataSets,'r')
#     	dsPattern = dsNames.lstrip(dsNames.split(os.sep)[0])
#     	pattern = re.compile( dsPattern )
    	
    if re.search("---",dsName):
        fileown = getDbsUser(dsName)
        dsName = getCastor(dsName)
    
    if len(dsName.lstrip(os.sep).rstrip(os.sep).split(os.sep)) < 3:
    	print "Error, "+dsName+" is not valid, please use valid name"
    	return None
    elif len(dsName.lstrip(os.sep).rstrip(os.sep).split(os.sep)) < 4:
    	print "Dataset "+ dsName + "is a CMS base dataset and cannot be published, please use DAS."
    	return None
    
    
    # Store full dataset name
    
    opts = dict()
    formatter = CastorToDbsFormatter(dsName,fileown)
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
    
    
    try:
    	dbsID = None
    	parentDbsID = None
    	taskID = None
    	parentTaskID = None
    	
    	publishController = PublishController(user, password, dbsApi, force)
    	if not publishController.loginValid():
    		print "User authentication failed, exiting\n\n"
    		return None
    	print "\n------DataSet------\n"
    	print "DBS name: ",getDbsWithUser(dsName, fileown)
        print "EOS name: ",dsName
    	if procds['ParentList'][0].endswith("---*") and dbsApi is not None:
    		procds['ParentList'][0] = publishController.chooseParent(procds['ParentList'][0], opts['category_id'])
    	if dbsApi is not None:
    		print "\n--------DBS--------\n"
    		publishController.dbsPublish(procds)
    	print "\n------Savanah------\n"
    	(taskID, parentTaskID) = publishController.savannahPublish(procds, opts, comment)
    	
    	if publishController.cmgdbOnline():
    		print "\n-------CMGDB-------\n"
    		try:
    			if taskID is not None:
    				publishController.cmgdbPublish(procds, dbsID, taskID, test)
    			if parentTaskID is not None:
    				publishController.cmgdbPublish(procds, parentDbsID, parentTaskID, test)
    		except ImportError:
    			print "cx_Oracle not properly installed"
    			return None

    except ValueError as err:
        print err, '.\nDataset not published'
        return None
    except NameError as err:
        print err.args[0]
        savannah = None
        if len(err.args)>1:
            print "Logging dataset: "+err.args[1] 
            f = open("/afs/cern.ch/user/"+user[0]+"/"+user+"/public/obsoleteDatasets.txt","a")
            
            if len(err.args)>2:
                f.write(err.args[1]+" https://savannah.cern.ch/task/?"+str(err.args[2])+"\n")
                savannah = "https://savannah.cern.ch/task/?"+str(err.args[2])
            else:
                f.write(err.args[1])
            f.close()
            
        return {'Status':'Failed','Savannah':savannah, 'Dataset':dsName, 'File Owner':fileown}



