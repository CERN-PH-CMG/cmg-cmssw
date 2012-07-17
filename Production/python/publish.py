#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

import os, sys, re
from CMGTools.Production.publishController import PublishController
from CMGTools.Production.nameOps import *
from CMGTools.Production.castorBaseDir import getUserAndArea
from CMGTools.Production.datasetInformation import DatasetInformation


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
	
	datasetDetails = None
	user, area = getUserAndArea(user)
	if area == 'group':
		checkGroups = True
	
		
	try:
		print "\n---Publish Dataset---"
		print "Retrieving dataset information...\n"
		
		# Initialise PublishController
		publishController = PublishController(user, password)
		
		# Attempt Login, if unexplained fail occurs, retry
		loginClear = False
		try:
		    loginClear = publishController.loginValid()
		except KeyboardInterrupt:
		    raise
		except:
		    try:
		        loginClear = publishController.loginValid()
		    except KeyboardInterrupt:
		        raise
		
		# Get DS Information
		datasetDetails = DatasetInformation(dsName, fileown ,comment ,force,test, user, password, run_range)
		
		if datasetDetails is None: return None
		datasetDetails.buildAllReports()
		if datasetDetails.dataset_details is None: return None
		
		# Print dataset names
		print "\n------DataSet------\n"
		print "CMGDB name:",datasetDetails.dataset_details['CMGDBName']
		print "Sample name:  ",datasetDetails.dataset_details['SampleName']
		print "Total Jobs: ",datasetDetails.dataset_details['TotalJobs']
		print "Directory size: ",datasetDetails.dataset_details['DirectorySizeInTB']," TB"
		print ""
		for group_name in datasetDetails.dataset_details['FileGroups']:
			print datasetDetails.createFileGroupDetailString(group_name)
		# If login fails return None
		if loginClear is False:
			print "User authentication failed, exiting\n\n"
			return None
		
		
		
		## Savannah operations
		print "\n------Savanah------\n"
		
		if savannah or datasetDetails.dataset_details['TaskID'] == None:
			(datasetDetails.dataset_details['TaskID'], datasetDetails.dataset_details['ParentTaskID']) = publishController.savannahPublish(datasetDetails.dataset_details)
		else:
		    print "NO SAVANNAH PUBLISH REQUIRED"
		
		if datasetDetails.dataset_details['TaskID'] is not None: status = 'Success'
		if publishController.cmgdbOnline():
			print "\n-------CMGDB-------\n"
			cmgdbid = publishController.cmgdbPublish(datasetDetails.dataset_details)
			
		return datasetDetails
	except KeyboardInterrupt:
		raise
	except ValueError as err:
		print err.args, '.\nDataset not published'
		return None
	except NameError as err:
		print err.args[0]
		return datasetDetails



