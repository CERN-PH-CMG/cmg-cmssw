#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## July 1st 2012

from datetime import datetime
from CMGTools.Production.nameOps import *
from CMGTools.Production.fileOps import FileOps
from CMGTools.Production.findDSOnSav import *
import re, os
from datetime import *



def buildAttributeDictionary(dsName, fileown ,force, checkGroups, run_range, comment, test, username, password):
	
	
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
	
	# Create FileOps object
	fileOps = FileOps(dsName, fileown ,force, checkGroups, run_range)
	
	parent = getUnknownParentWithDbs(getDbs(dsName))
	path = getDbsWithUser(dsName, fileown)
	parts = path.lstrip("/").split("/")
	tierList=parts[2].split("-")
	datasetDetails = {"PrimaryDataset":parts[0],
                      "EOSCheckSuccess":False,
                      "LFN":fileOps.getLFN(),
                      "LFNGroups":fileOps.getLFNGroups(),
                      "Tags":fileOps.getTags(),
                      "Release":fileOps.getRelease() ,
                      "EOSPath":fileOps.getCastor(),
                      "PhysicsGroup":"CMG",
                      "TierList":tierList,
                      "CMGDBName":path,
                      "DateCreated":datetime.now().strftime("%s"),
                      "ParentCMGDBName":parent,
                      "FileOwner":fileown,
                      "taskID":None,
                      "parentTaskID":None,
                      "Status":"INVALID",
                      "EOSName":dsName,
                      "ParentEOSName":getParentWithCastor(dsName),
                      "Comment":comment,
                      "CMGDBID":None,
                      "ParentCMGDBID":None,
                      "Test":test,
                      "ParentSavannahString":"",
                      "ValidDuplicates":fileOps._validDuplicates,
                      "BadJobs":fileOps._badJobs,
                      "BadFiles":fileOps._allFilesBad,
                      "MissingFiles":fileOps._allMissingFiles,
                      "TotalJobs":fileOps._totalJobs,
                      "TotalFilesMissing":fileOps._totalFilesMissing,
                      "TotalFilesGood":fileOps._totalFilesGood,
                      "TotalFilesBad":fileOps._totalFilesBad,
                      "PrimaryDatasetFraction":fileOps._primaryDatasetFraction,
                      "PrimaryDatasetEntries":fileOps._primaryDatasetEntries,
                      "FileEntries":fileOps._fileEntries,
                      "DatasetSizeInTB":fileOps.getDatasetSize(),
                      "SavannahOptions":dict()
                      }
	dayMonthYear = date.fromtimestamp(int(datasetDetails['DateCreated'])).strftime('%d-%B-%Y').split("-")
	datasetDetails['SavannahOptions']['planned_starting_date_dayfd']=dayMonthYear[0].lstrip("0")
	datasetDetails['SavannahOptions']['planned_starting_date_monthfd']=dayMonthYear[1]    
	datasetDetails['SavannahOptions']['planned_starting_date_yearfd']=dayMonthYear[2]
	
	# Create test category MAY BE REMOVED IF TEST NOT REQUIRED
	if test:
		category_id='101'
	else:category_id='103'
	
	# More savannah opts
	datasetDetails['SavannahOptions']['assigned_to']=fileown
	datasetDetails['SavannahOptions']['summary']=datasetDetails['CMGDBName']
	
	# Check if task already exists for dataset
	datasetDetails['taskID'] = getTaskID(datasetDetails['CMGDBName'], category_id, username, password, False)
	
	# If task already exists you NO NOT WANT TO CHANGE THE CATEGORY so only add this field if it is a new DS
	if datasetDetails['taskID'] is None: datasetDetails['SavannahOptions']['category_id']=category_id
	
	# Check if parent exists
	datasetDetails['parentTaskID'] = getTaskID(datasetDetails['ParentCMGDBName'], category_id, username, password, True)
	
	if datasetDetails['parentTaskID'] is not None and len(datasetDetails['parentTaskID']) > 0:
		if len(datasetDetails['parentTaskID'])>1: raise NameError("Multiple possible parents found for dataset: "+datasetDetails['CMGDBName']+". Please find the duplicate and remove it" )
		datasetDetails['ParentSavannahString']= "[https://savannah.cern.ch/task/?"+str(datasetDetails['parentTaskID'][0])+" "+getNameWithID(datasetDetails['parentTaskID'][0])+"]"
		datasetDetails['parentTaskID'] = datasetDetails['parentTaskID'][0]
	# If Parent is a CMS dataset (i.e. not a CMG dataset)
	elif not re.search("--", datasetDetails['ParentCMGDBName']):
		datasetDetails['ParentSavannahString']= "[https://cmsweb.cern.ch/das/request?view=list&limit=10&instance=cms_dbs_prod_global&input=dataset%3D%2F" +datasetDetails['ParentCMGDBName'].lstrip("/").split("/")[0]+ "%2F" +datasetDetails['ParentCMGDBName'].lstrip("/").split("/")[1]+"%2F" + datasetDetails['ParentCMGDBName'].lstrip("/").split("/")[2]+ " "+ datasetDetails['ParentCMGDBName']+"]"
		datasetDetails['parentTaskID'] = None
	# If parent doesn't exist throw exception
	else: raise NameError("No Parent was found for Dataset: "+datasetDetails['CMGDBName']+" not publishing.",datasetDetails['taskID'])
	
	
	if fileOps is not None: datasetDetails["EOSCheckSuccess"]=True
	if fileOps.isValid():
		datasetDetails['Status'] = "VALID"
	else:
		datasetDetails['Status'] = "INVALID"
	return datasetDetails