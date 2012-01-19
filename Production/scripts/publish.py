#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

import os, getpass, sys
from CMGTools.Production.castorToDbsFormatter import CastorToDbsFormatter
from CMGTools.Production.publishController import PublishController
from DBSAPI.dbsProcessedDataset import DbsProcessedDataset
from DBSAPI.dbsPrimaryDataset import DbsPrimaryDataset
from datetime import *
from DBSAPI.dbsOptions import DbsOptionParser
from DBSAPI.dbsApi import DbsApi
from DBSAPI.dbsApiException import *
from DBSAPI.dbsException import *


if __name__ == '__main__':
    optManager  = DbsOptionParser()
    parser = optManager.parser

    parser.usage = """
%prog [options] <sampleName>
-----
sampleName should be given in the form /[PrimaryDS]/[ProcDS]/[Tiers]/*[Parent]
-----
Use this script to publish dataset details to DBS, CmgDB, and optionally savannah.
-----
If no -u option is provided, it is assumed that the current user is the files owner on Castor.
If no -s option is provided, it is assumed that the current user is the user on Savannah 
"""
    
    # If user is not specified default is current user
    # This option will be used to find dataset on castor, and assign dataset on savannah
    parser.add_option("-F", "--fileown", 
                      dest="fileown",
                      help="User who is the files owner on Castor." ,
                      default=os.environ['USER'] )
    # If specified is used to log in to savannah (only required if user that created the dataset,
    # is different to user publishing it)
    parser.add_option("-u", "--user",
                      action = "store",
                      dest="user",
                      help="If DBS and Savannah user is different to current user, enter Savannah username here",
                      default=os.environ['USER'] )
    # If the purpose is to test the software use this parameter, it will not be recognised by the
    # non-testing algorithm
    parser.add_option("-T", "--test",
                      action = "store_true",
                      dest="test",
                      help="Flag task as a test",
                      default=False )
    # If user wants to add their own comments
    parser.add_option("-C", "--comment",
                      action = "store",
                      dest="commented",
                      help="Take comment as arg",
                      default = None)
    # If user wants to add their own comments
    parser.add_option("-D", "--dbs",
                      action = "store_true",
                      dest="dbs",
                      help="Use DBS",
                      default = False)
    # If user wants to add their own comments
    parser.add_option("-M", "--multi",
                      action = "store",
                      dest="multi",
                      help="Takes argument as user%/sample/name and publishes all samples with that name",
                      default = False)
    
    
    
    (options, args) = optManager.getOpt()
    options.url="http://cmsphys05.cern.ch:8081/cms_dbs_prod_local_01/servlet/DBSServlet"
    
    dsName = args[0]
    
    if options.dbs:
    	dbsApi = DbsApi(options.__dict__)
    else: dbsApi = None
    # Allow no more than one argument
    if len(args)!=1:
        parser.print_help()
        sys.exit(1)
        
    
    if len(dsName.lstrip(os.sep).rstrip(os.sep).split(os.sep)) < 3:
    	print "Error, please use valid name"
    	sys.exit(1)
    elif len(dsName.lstrip(os.sep).rstrip(os.sep).split(os.sep)) < 4:
    	print "Can not publish base dataset. No file owner."
    	sys.exit(1)
    password = getpass.getpass("Enter NICE Password: ")
    comment = options.commented

    # Store full dataset name
    
    opts = dict()
    formatter = CastorToDbsFormatter(dsName,options.fileown)
    procds =formatter.getDataset()
    dayMonthYear = date.fromtimestamp(int(procds['DateCreated'])).strftime('%d-%B-%Y').split("-")
    opts['planned_starting_date_dayfd']=dayMonthYear[0].lstrip("0")
    opts['planned_starting_date_monthfd']=dayMonthYear[1]    
    opts['planned_starting_date_yearfd']=dayMonthYear[2]
    if options.test:
    	opts['category_id']='101'
    else:opts['category_id']='100'
    opts['assigned_to']=options.fileown
    opts['summary'] = procds['PathList'][0]
    
    
    try:
    	dbsID = None
    	parentDbsID = None
    	taskID = None
    	parentTaskID = None
    	
    	publishController = PublishController(options.user, password, dbsApi)
    	if not publishController.loginValid():
    		print "User authentication failed, exiting"
    		sys.exit(1)
    	if procds['ParentList'][0].endswith("---*"):
    		procds['ParentList'][0] = publishController.chooseParent(procds['ParentList'][0], options.dbs, opts['category_id'])
    	if options.dbs:
    		print "\n--------DBS--------\n"
    		publishController.dbsPublish(procds)
    	print "\n------Savanah------\n"
    	(taskID, parentTaskID) = publishController.savannahPublish(procds, opts, comment)
    	
    	if publishController.cmgdbOnline():
    		print "\n-------CMGDB-------\n"
    		try:
    			if taskID is not None:
    				publishController.cmgdbPublish(procds, dbsID, taskID, options.test)
    			if parentTaskID is not None:
    				publishController.cmgdbPublish(procds, parentDbsID, parentTaskID, options.test)
    		except ImportError:
    			print "cx_Oracle not properly installed"

    except ValueError as err:
        print err, '. Exit!'
        sys.exit(1)



