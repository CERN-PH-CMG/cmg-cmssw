#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

from CMGTools.Production.cmgdbToolsApi import CmgdbToolsApi
from CMGTools.Production.savannahFormatter import SavannahFormatter
from CMGTools.Production.fileOps import FileOps
from DBSAPI.dbsApiException import *
from DBSAPI.dbsApi import DbsApi
from DBSAPI.dbsException import *
from DBSAPI.dbsFileBlock import DbsFileBlock
from DBSAPI.dbsPrimaryDataset import DbsPrimaryDataset
import CMGTools.Production.findDSOnSav as findDSOnSav
from CMGTools.Production.castorToDbsFormatter import CastorToDbsFormatter
from CMGTools.Production.nameOps import *
import sys, re


class PublishController(object):
    """This class controls the interactions between a user and the two publishing platforms, Savannah and CMGDB"""
    def __init__(self, username, password, force):
        """Initialise CMGDB and set username and password
    	
        'username' takes the NICE username of the current user, NOT the files owner on EOS
        'password' takes the NICE password of the current user
        'force' takes a boolean value which determines whether a lack of log file can be ignored
        """
        self._cmgdbAPI=CmgdbToolsApi()
        self._cmgdbAPI.connect()
        self._username = username
        self._password = password
        self._force = force
    def cmgdbOnline(self):
    	"""Returns True if CMGDB is online and working"""
    	if self._cmgdbAPI is not None: return True
    	else: return False
    def loginValid(self):
    	"""Returns true if the login values that the user provided were a valid CMG group NICE login"""
    	return findDSOnSav.validLogin(self._username, self._password)
        
     
    def savannahPublish(self, procds, opts, comment, fileOps):
    	"""Publish dataset details to savannah and returns task ID
    	
    	'procds' takes a DbsProcessedDataset object which contains many of the datasets details
    	### THIS COULD BE SIMPLIFIED. DBS OBJECTS NO LONGER NEEDED
    	'opts' takes a Dict() object containing essential information about the post NOT THE DATA.
    		it includes date of post, file owner (for assignment)
    	'fileOps' takes a FileOps object, which generates metadata about the dataset
    	Returns Savannah task ID of the dataset
    	
    	First, checks if task already exits, then, sees if parent exists.
    	Next, the function gathers information from the FileOps object,
    	Finally, the function will send the dataset information to a SavannahFormatter object, 
    	which will then format the info and publish it to Savannah.
    	"""
    	test = False
    	# If item is test
    	if 'category_id' in opts:
    		if opts['category_id'] == '101': test = True
		
		# Check if task already exists for dataset
		try:
			taskID = findDSOnSav.getTaskID(procds['PathList'][0], opts['category_id'], self._username, self._password, False)
		except KeyboardInterrupt:
			raise
    	
    	# Check if parent exists
    	parentTaskID = findDSOnSav.getTaskID(procds['ParentList'][0], opts['category_id'], self._username, self._password, True)
    	
    	if parentTaskID is not None and len(parentTaskID) > 0:
    		if len(parentTaskID)>1: raise NameError("Multiple possible parents found for dataset: "+procds['PathList'][0]+". _getOption() method could be implemented here" )
    		procds['ParentList'][0]= "[https://savannah.cern.ch/task/?"+str(parentTaskID[0])+" "+findDSOnSav.getNameWithID(parentTaskID[0])+"]"
    		parentTaskID = parentTaskID[0]
    	# If Parent is a CMS dataset (i.e. not a CMG dataset)
    	elif not re.search("--", procds['ParentList'][0]):
    		procds['ParentList'][0]= "[https://cmsweb.cern.ch/das/request?view=list&limit=10&instance=cms_dbs_prod_global&input=dataset%3D%2F" +procds['ParentList'][0].lstrip("/").split("/")[0]+ "%2F" +procds['ParentList'][0].lstrip("/").split("/")[1]+"%2F" + procds['ParentList'][0].lstrip("/").split("/")[2]+ " "+ procds['ParentList'][0]+"]"
    		parentTaskID = None
    	# If parent doesn't exist throw exception
    	else: raise NameError("No Parent was found for Dataset: "+procds['PathList'][0]+" not publishing.",['PathList'][0],taskID)
    	
    	# If task already exists DO NOT re-enter category id, this would be BAD
    	passOpts = opts
    	if taskID is not None:
    		del(passOpts['category_id'])
    	
    	# Check validity of dataset
    	if fileOps.isValid():
			procds['status'] = "VALID"
    	else: procds['status'] = "INVALID"
    	
    	# Initialise SavannahFormatter object and add the main attributes
    	self.savannah = SavannahFormatter(self._username, self._password, taskID, passOpts )
    	self.savannah.addMain(procds,fileOps.getTags(),self._deleteExtras(fileOps.getLFNGroups()),fileOps.getCastor(), fileOps.getLFN(),fileOps.getRelease(), comment)
    	
    	# Append all the elements of the integrity check to the savannah output.
    	if fileOps._totalJobs is not None:self.savannah.appendExtra("*Total Jobs:* "+str(fileOps._totalJobs))
    	if fileOps._totalFilesMissing is not None:self.savannah.appendExtra("*Total Missing Files:* "+str(fileOps._totalFilesMissing))
    	if fileOps._totalFilesGood is not None:self.savannah.appendExtra("*Total Good Files:* "+str(fileOps._totalFilesGood))
    	if fileOps._totalFilesBad is not None:self.savannah.appendExtra("*Total Bad Files:* "+str(fileOps._totalFilesBad))
    	if fileOps._primaryDatasetFraction is not None:self.savannah.appendExtra("*Primary Dataset Fraction used:* "+str(fileOps._primaryDatasetFraction))
    	if fileOps._fileEntries is not None:self.savannah.appendExtra("*File Entries:* "+str(fileOps._fileEntries))
    	if fileOps.getDatasetSize() is not None:self.savannah.appendExtra("*Dataset Size:* "+str(fileOps.getDatasetSize())+" TB")
    	if fileOps._validDuplicates is not None:
    		validDuplicates = ["\n"]
    		for dupedFile in fileOps._validDuplicates:
    			validDuplicates.append("* " +dupedFile+": \n** "+str(fileOps._validDuplicates[dupedFile])+" events")
    		self.savannah.appendExtra({"Valid Duplicates":validDuplicates})
    	if fileOps._primaryDatasetEntries is not None:self.savannah.appendExtra("*Primary Dataset Entries:* "+str(fileOps._primaryDatasetEntries))
    	if fileOps._badJobs is not None:self.savannah.appendExtra({"Bad Jobs":fileOps._badJobs})
    	if len(fileOps._filesBad) > 0:
    		filesBad = []
    		for badFile in fileOps._filesBad:filesBad.append("* "+badFile)
    		filesBad[0] = "\n"+filesBad[0]
    		self.savannah.appendExtra({"Bad Files":fileOps.filesBad})
    		
        # Publish to Savannah
    	newTask = self.savannah.publish()
        if newTask is None:
    		print "Unable to publish Dataset to Savannah, an error occured"
    		return None, parentTaskID
    	elif newTask is taskID:
        	print "Comment added to Savannah"
        	print "URL: https://savannah.cern.ch/task/?"+newTask
        else:
    		print "Dataset published to Savannah"
    		print "URL: https://savannah.cern.ch/task/?"+newTask
    	return newTask, parentTaskID
    	

    def cmgdbPublish(self, procds, taskID, test, fileOps):
    	"""Publish dataset information to CMGDB, and return unique CMGDB dataset ID
    	
    	'procds' takes a DbsProcessedDataset object which contains many of the datasets details
    	### THIS COULD BE SIMPLIFIED. DBS OBJECTS NO LONGER NEEDED
    	'taskID' takes the Savannah task ID of the dataset
    	'test' takes a boolean relating to whether the dataset is a test dataset
    	### Colin, could this be removed now?
    	'fileOps' takes a FileOps object for retrieving data about the dataset
    	"""
    	if self._cmgdbAPI is None:
    		return None
    	# Create hash code for the tag set
    	tags = fileOps.getTags()
    	release = fileOps.getRelease()
    	taghash = []
    	for i in tags:
    		a=hash((i['package'],i['tag']))
    		taghash.append(a)
    	taghash.sort()
    	endhash = hash(tuple(taghash))
    	    		
    	# See if cmgdb already has record of ds with sav
    	cmgdbID = self._cmgdbAPI.getDatasetIDWithName(procds['PathList'][0])
    			
    	# If not add dataset
    	if cmgdbID is None:
    		parentID = None
    		if len(procds['PathList'][0].split("---")[0].split("--"))>2:
    			parents = self._cmgdbAPI.getParentsWithName(procds['PathList'][0])
    			if len(parents) == 0:
    				raise NameError("Dataset Should have Parent and does not, please publish parent first")
    			elif len(parents) == 1:
    				parentID = parents[0][1]
    			else:
    				parentID = self.getOption(parents)[1]
    		
    		cmgdbID = self._cmgdbAPI.addDataset(procds['PathList'][0],getCastor(procds['PathList'][0]),fileOps.getLFN(), getDbsUser(procds['PathList'][0]),parentID, self._username)
    	# Clear 4 tables relating to bad files & jobs, and missing & duplicate files
    	if fileOps is not None:
    		self._cmgdbAPI.clearDatasetBadFiles(procds['PathList'][0],cmgdbID)
    		self._cmgdbAPI.clearDatasetDuplicateFiles(procds['PathList'][0],cmgdbID)
    		self._cmgdbAPI.clearDatasetMissingFiles(procds['PathList'][0],cmgdbID)
    		self._cmgdbAPI.clearDatasetBadJobs(procds['PathList'][0],cmgdbID)
    		
    		
    		if fileOps._totalJobs is not None:self._cmgdbAPI.addTotalJobs(fileOps._totalJobs)
    		if fileOps._totalFilesMissing is not None:self._cmgdbAPI.addMissingFileNum(cmgdbID, fileOps._totalFilesMissing)
    		if fileOps._totalFilesGood is not None:self._cmgdbAPI.addGoodFileNum(cmgdbID, fileOps._totalFilesGood)
    		if fileOps._totalFilesBad is not None:self._cmgdbAPI.addBadFileNum(cmgdbID, fileOps._totalFilesBad)
    		for badFile in fileOps._allBadFiles:self._cmgdbAPI.addBadFile(procds['PathList'][0],cmgdbID, badFile.split('/')[-1])
    		for missingFile in fileOps._allMissingFiles:self._cmgdbAPI.addMissingFile(procds['PathList'][0],cmgdbID, missingFile.split('/')[-1])
    		for badJob in fileOps._badJobs:self._cmgdbAPI.addBadJob(cmgdbID, badJob)
    		if fileOps._primaryDatasetFraction is not None:self._cmgdbAPI.addPrimaryDatasetFraction(cmgdbID, fileOps._primaryDatasetFraction)
    		if fileOps._fileEntries is not None:self._cmgdbAPI.addFileEntries(cmgdbID, fileOps._fileEntries)
    		if fileOps.getDatasetSize() is not None:self._cmgdbAPI.addDatasetSize(cmgdbID, fileOps.getDatasetSize())
    		
    	# Add task id
    	self._cmgdbAPI.addTaskID(cmgdbID, taskID, test)
    	
    	# Add tags to CMGDB
    	if tags is None or len(tags) is 0: return None
    	tagIDs = []
    	
    	# check if tag set is already on CMGDB
    	tagSetID = self._cmgdbAPI.getTagSetID(endhash)
    	
    	# If it isn't found, add the tags, and the tag set
    	if tagSetID is None:
    		if tags:
    			tagIDs
    			for row in tags:
    				tagID = self._cmgdbAPI.addTag(row["package"],row["tag"])
    				if tagID is not None: tagIDs.append(tagID)
    			
    			tagSetID = self._cmgdbAPI.addTagSet(release,endhash)
    			for tagID in tagIDs:
    				self._cmgdbAPI.addTagToSet(tagID,tagSetID)
    			    	
    	if tagSetID is not None: self._cmgdbAPI.addTagSetID(tagSetID, cmgdbID)
    	return cmgdbID
            
         
    def _getOption(self, opts):
    	"""Present the user with a choice of options
    	
    	'opts' takes a Dict object
    	Returns chosen value
    	"""
    	print "Please enter the number of the set you would like to use:"
    	for i in opts:
    		print str(opts.index(i)) + ": " + str(i)
    	num = raw_input("-")
    	while int(num) < len(opts) - 1:
    		num = raw_input("Please enter a valid number:")
    	return opts[int(num)]
    	
    def _deleteExtras(self,groups):
    	"""Delete irrelevant data from group dictionaries
    	
    	'groups' takes the file groups object of the dataset from FileOps"""
    	
    	
    	for i in groups:
    			if 'top' in i:del i['top']
    			if 'bottom' in i:del i['bottom']
    			if 'fileNums' in i:del i['fileNums']
    			if 'duplicateFiles' in i:
    				if i['duplicateFiles']== []: del i['duplicateFiles']
    			if 'missingFiles' in i:
    				if i['missingFiles'] == []: del i['missingFiles']
    	return groups
          
