#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

import os, sys, re
from CMGTools.Production.publishController import PublishController
from CMGTools.Production.nameOps import *
from CMGTools.Production.castorBaseDir import getUserAndArea
from CMGTools.Production.datasetInformation import DatasetInformation


def publish(sampleName,fileown,comment,test,user,password, force, savannah,primary, run_range = None):
	"""Publish the given dataset to CMGDB and Savannah
		
	'sampleName' takes the name of the dataset, in either format
	'fileown' takes the NICE username of the space on EOS in which the dataset resides
	'comment' takes a users comment for publishing to Savannah or None
	'test' takes True/False on whether the posting is a test or not
	'user' takes the NICE username of the person making the post
	'password' takes the NICE password of the person making the post
	'force' takes True/False on whether the dataset should be published if no log file exists
	'savannah' takes True/False on whether Savannah publish is desired
	"""
	
	def checkName(sampleName, fileown):
		# Validate name, and escape if name is invalidate
		# Convert name to EOS format (castor)
		if re.search("---",sampleName):
			fileown = getFileOwner(sampleName)
			sampleName = getSampleName(sampleName)
			if sampleName is None:
				print "\nError, dataset name is not valid, please use valid name\n"
				return None
		
		# Check the length of the dataset name	
		if len(sampleName.lstrip(os.sep).rstrip(os.sep).split(os.sep)) < 3:
			print "Error, "+sampleName+" is not valid, please use valid name"
			return None
		elif len(sampleName.lstrip(os.sep).rstrip(os.sep).split(os.sep)) < 4:
			print "Dataset "+ sampleName + "is a CMS base dataset and cannot be published, please use DAS."
			return None
		return sampleName, fileown
	
	datasetDetails = None
	try:
		if not primary:
			sampleName, fileown = checkName(sampleName, fileown)
		if sampleName is None: return None
		print "\n\t-------Publishing New Dataset-------"
		print sampleName+"\n"
		
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
		
		# If login fails return None
		if loginClear is False:
			print "User authentication failed, exiting\n\n"
			return None
		
		
		# Get DS Information
		datasetDetails = DatasetInformation(sampleName, fileown ,comment ,force,test,primary, user, password)
		
		# Build all reports on the dataset
		if datasetDetails is None: return None
		datasetDetails.buildAllReports()
		if datasetDetails.dataset_details is None: return None
		# Print dataset names
		print "\n------DataSet Information------"
		print datasetDetails.createDirectoryDetailString()
		for group_name in datasetDetails.dataset_details['FileGroups']:
			print datasetDetails.createFileGroupDetailString(group_name)
		
		## Savannah operations
		print "\n------Savanah------\n"
		
		if savannah or datasetDetails.dataset_details['TaskID'] == None:
			(datasetDetails.dataset_details['TaskID'], datasetDetails.dataset_details['ParentTaskID']) = publishController.savannahPublish(datasetDetails.dataset_details)
		else:
		    print "NO SAVANNAH PUBLISH REQUIRED"
		
		if datasetDetails.dataset_details['TaskID'] is not None: status = 'Success'
		
		# Sent data (with updated task ID) to CMGDB
		if publishController.cmgdbOnline():
			print "\n-------CMGDB-------\n"
			cmgdbid = publishController.cmgdbPublish(datasetDetails.dataset_details)
			
		return datasetDetails.dataset_details
	except KeyboardInterrupt:
		raise
	except ValueError as err:
		print err.args, '.\nDataset not published'
		return None
	except NameError as err:
		print err.args[0]
		return datasetDetails



