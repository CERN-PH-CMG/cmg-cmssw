#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

import os, sys, re
from CMGTools.Production.publishController import PublishController
from CMGTools.Production.nameOps import *
from CMGTools.Production.castorBaseDir import getUserAndArea
from CMGTools.Production.datasetAttributeAddressBook import *


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
		datasetDetails = buildAttributeDictionary(dsName, fileown ,force, checkGroups, run_range, comment, test, user, password)
		if datasetDetails is None: return None
		
		# If login fails return None
		if loginClear is False:
			print "User authentication failed, exiting\n\n"
			return None
		
		# Print dataset names
		print "\n------DataSet------\n"
		print "CMGDB name:",datasetDetails['CMGDBName']
		print "EOS name:  ",datasetDetails['EOSName']
		
		## Savannah operations
		print "\n------Savanah------\n"
		
		if savannah or datasetDetails['taskID'] == None:
			(datasetDetails['taskID'], datasetDetails['parentTaskID']) = publishController.savannahPublish(datasetDetails)
		else:
		    print "NO SAVANNAH PUBLISH REQUIRED"
		
		if datasetDetails['taskID'] is not None: status = 'Success'
		if publishController.cmgdbOnline():
			print "\n-------CMGDB-------\n"
			cmgdbid = publishController.cmgdbPublish(datasetDetails)
			
		return datasetDetails
	except KeyboardInterrupt:
		raise
	except ValueError as err:
		print err.args, '.\nDataset not published'
		return None
	except NameError as err:
		print err.args[0]
		return datasetDetails



