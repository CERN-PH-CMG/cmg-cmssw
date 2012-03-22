#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

from CMGTools.Production.cmgdbApi import CmgdbApi
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
    def __init__(self, username, password, force):
        self._cmgdbAPI=CmgdbApi()
        self._username = username
        self._password = password
        self._force = force
    def cmgdbOnline(self):
    	if self._cmgdbAPI is not None: return True
    	else: return False
    def loginValid(self):
    	return findDSOnSav.validLogin(self._username, self._password)
        
     
    def savannahPublish(self, procds, opts, comment, fileOps):
    	test = False
    	# If item is test
    	if 'category_id' in opts:
    		if opts['category_id'] == '101': test = True
		try:
			taskID = findDSOnSav.getTaskID(procds['PathList'][0], opts['category_id'], self._username, self._password, False)
		except KeyboardInterrupt:
			raise
    		
    	parentTaskID = findDSOnSav.getTaskID(procds['ParentList'][0], opts['category_id'], self._username, self._password, True)
    	
    	if parentTaskID is not None and len(parentTaskID) > 0:
    		if len(parentTaskID)>1: raise NameError("Multiple possible parents found for dataset: "+procds['PathList'][0]+". _getOption() method could be implemented here" )
    		procds['ParentList'][0]= "[https://savannah.cern.ch/task/?"+str(parentTaskID[0])+" "+findDSOnSav.getNameWithID(parentTaskID[0])+"]"
    		parentTaskID = parentTaskID[0]
    	elif not re.search("--", procds['ParentList'][0]):
    		procds['ParentList'][0]= "[https://cmsweb.cern.ch/das/request?view=list&limit=10&instance=cms_dbs_prod_global&input=dataset%3D%2F" +procds['ParentList'][0].lstrip("/").split("/")[0]+ "%2F" +procds['ParentList'][0].lstrip("/").split("/")[1]+"%2F" + procds['ParentList'][0].lstrip("/").split("/")[2]+ " "+ procds['ParentList'][0]+"]"
    		parentTaskID = None
    	else: raise NameError("No Parent was found for Dataset: "+procds['PathList'][0]+" not publishing.",['PathList'][0],taskID)
    		
    	passOpts = opts
    	if taskID is not None:
    		del(passOpts['category_id'])
    	
    	if fileOps.isValid():
			procds['status'] = "VALID"
    	else: procds['status'] = "INVALID"
    	
    	self.savannah = SavannahFormatter(self._username, self._password, taskID, passOpts )
    	self.savannah.addMain(procds,fileOps.getTags(),self._deleteExtras(fileOps.getLFNGroups()),fileOps.getCastor(), fileOps.getLFN(),fileOps.getRelease(), comment)
    	
    	if fileOps.getIntegrity() is not None:
    		report = fileOps.getIntegrity()
    		if 'PrimaryDatasetEntries' in report:
    			self.savannah.appendExtra("*Primary Dataset Entries:* "+str(report['PrimaryDatasetEntries']))
    		if 'FilesEntries' in report:
    			self.savannah.appendExtra("*File Entries:* "+str(report['FilesEntries']))
    		if 'PrimaryDatasetFraction' in report:
    			self.savannah.appendExtra("*Primary Dataset Fraction used:* "+str(report['PrimaryDatasetFraction']))
    		if 'ValidDuplicates' in report:
    			validDuplicates = ["\n"]
    			for i in report['ValidDuplicates']:
    				validDuplicates.append("* " +i+": \n** "+str(report['ValidDuplicates'][i])+" events")
    			self.savannah.appendExtra({"Valid Duplicates":validDuplicates})
    		if 'BadJobs' in report:
    			self.savannah.appendExtra({"Bad Jobs":report['BadJobs']})
    		if 'NumFilesBad' in report:
    			self.savannah.appendExtra({"Number of Bad Files":report['NumFilesBad']})
    		if 'FilesBad' in report:
    			filesBad = []
    			for i in report['FilesBad']:
    				filesBad.append("* "+i)
    			if len(filesBad)>0:
    				filesBad[0] = "\n"+filesBad[0]
    			self.savannah.appendExtra({"Bad Files":filesBad})
    	#if fileOps.getLFNGroups() is not None:
    		#self.savannah.appendExtra(fileOps.getLFNGroups())
    		
    	
            
    	newTask = self.savannah.publish() 
        if newTask is None:
    		print "Unable to publish Dataset to Savannah, an error occured"
    		return None
    	elif newTask is taskID:
        	print "Comment added to Savannah"
        	print "URL: https://savannah.cern.ch/task/?"+newTask
        else:
    		print "Dataset published to Savannah"
    		print "URL: https://savannah.cern.ch/task/?"+newTask
    	
    	return newTask, parentTaskID
    	

    def cmgdbPublish(self, procds, taskID, test, fileOps):
    	if self._cmgdbAPI is None:
    		return None
    	# Get file information
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
    	
    	if fileOps is not None:
    		self._cmgdbAPI.clearDatasetBadFiles(procds['PathList'][0],cmgdbID)
    		self._cmgdbAPI.clearDatasetDuplicateFiles(procds['PathList'][0],cmgdbID)
    		self._cmgdbAPI.clearDatasetMissingFiles(procds['PathList'][0],cmgdbID)
    		self._cmgdbAPI.clearDatasetBadJobs(procds['PathList'][0],cmgdbID)
    		
    		
    		for i in fileOps.getLFNGroups():
    			if 'missingFiles' in i:
    				for i in i['missingFiles']:
    					self._cmgdbAPI.addMissingFile(procds['PathList'][0],cmgdbID,i.split('/')[-1])
    		
    		integrity = fileOps.getIntegrity()
    		
    		if integrity is not None:
    			if 'BadJobs' in integrity:
    				for i in integrity['BadJobs']:
    					self._cmgdbAPI.addBadJob(procds['PathList'][0],cmgdbID,i)
    			if 'FilesBad' in integrity:
    				for i in integrity['FilesBad']:
    					i= i.split('/')[-1]
    					self._cmgdbAPI.addBadFile(procds['PathList'][0],cmgdbID,i)
    			if 'PrimaryDatasetFraction' in integrity:
    				self._cmgdbAPI.addPrimaryDatasetFraction(cmgdbID, integrity['PrimaryDatasetFraction'])
    			if 'FilesEntries' in integrity:
    				self._cmgdbAPI.addFileEntries(cmgdbID,integrity['FilesEntries'])
    	
    	self._cmgdbAPI.addTaskID(cmgdbID, taskID, test)
    	
    	
    	
    	# Add tags to CMGDB
    	if tags is None or len(tags) is 0: return None
    	tagIDs = []
    	
    	tagSetID = self._cmgdbAPI.getTagSetID(endhash)
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
    	print "Please enter the number of the set you would like to use:"
    	for i in opts:
    		print str(opts.index(i)) + ": " + str(i)
    	num = raw_input("-")
    	while int(num) < len(opts) - 1:
    		num = raw_input("Please enter a valid number:")
    	return opts[int(num)]
    	
    def _deleteExtras(self,groups):
    	for i in groups:
    			if 'top' in i:del i['top']
    			if 'bottom' in i:del i['bottom']
    			if 'fileNums' in i:del i['fileNums']
    			if 'duplicateFiles' in i:
    				if i['duplicateFiles']== []: del i['duplicateFiles']
    			if 'missingFiles' in i:
    				if i['missingFiles'] == []: del i['missingFiles']
    	return groups
          
    def __del__(self):
        self._cmgdbAPI.close()
