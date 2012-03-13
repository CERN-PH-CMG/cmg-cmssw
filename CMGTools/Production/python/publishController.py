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
    def __init__(self, username, password, dbsApi, force):
        self._dbsAPI = dbsApi
        self._cmgdbAPI=CmgdbApi()
        self._username = username
        self._password = password
        self._force = force
    def cmgdbOnline(self):
    	if self._cmgdbAPI is not None: return True
    	else: return False
    def loginValid(self):
    	return findDSOnSav.validLogin(self._username, self._password)
        
    def chooseParent(parentName, category):
    	if self._dbsAPI is not None:
    		parent = parentName.lstrip("/").split("/")
    		parentDatasets = self._dbsAPI.listProcessedDatasets(parent[0],parent[2],parent[1])
    		if len(parentDatasets) == 0:
    			print "No parent found, please add before preceding"
    			return None
    		elif len(parentDatasets) == 1:
    			print "One dataset found: " + parentDatasets[0]['PathList'][0]
    			answer = raw_input("Use this Dataset?(y/n)")
    			while answer is not "y" or answer is not "n":
    				if answer is "y":
    					return parentDatasets[0]['PathList'][0]
    				elif answer is "n":
    					print "Please add desired dataset"
    					return None
    				else:
    					answer = raw_input("Use this Dataset?(y/n)")
    		else:
    			print "Several potential parents found, please choose:"
    			sets = []
    			for set in parentDatasets:
    				sets.append(set['PathList'][0])
    			sets.append("None of above")
    			index = self._getOption(sets)
    			try:
    				return parentDatasets[int(index)]
    			except:
    				print "Please add desired dataset"
    				return None
    	else:
    		print "Searching savannah for parents"
    		
    		parentDatasets = findDSOnSav.getTaskID(getCastor(procds['PathList'][0]), category, self._username, self._password, True)
    		if parentDatasets is None or len(parentDatasets) is 0:
    			print "No parent found, please add before proceeding"
    			return None
    		elif len(parentDatasets) is 1:
    			print "One dataset found: " + getNameWithID(parentDatasets[0])
    			answer = raw_input("Use this Dataset?(y/n)")
    			while answer is not "y" or answer is not "n":
    				if answer is "y":
    					return getNameWithID(parentDatasets[0])
    				elif answer is "n":
    					print "Please add desired dataset"
    					return None
    				else:
    					answer = raw_input("Use this Dataset?(y/n)")
    		else:
    			print "Several potential parents found, please choose:"
    			sets = []
    			for set in parentDatasets:
    				sets.append(getNameWithID(set))
    			sets.append("None of above")
    			index = self._getOption(sets)
    			del(sets[-1])
    			try:
    				return sets[int(index)]
    			except:
    				print "Please add desired dataset"
    				return None	
            
    def dbsPublish(self, procds):
    	
    	# Get file information
        fileOps = FileOps(getCastor(procds['PathList'][0]), getDbsUser(procds['PathList'][0]),self._force)
        
        
    	# Check if user has dataset files, and DO NOT allow publish if they do not
    	if len(fileOps.getRootFiles()) == 0:
        	raise NameError("No dataset files found on EOS, exiting")
        	return None
    	# Check dbsAPI is valid
    	if self._dbsAPI is None:
    		print "DBS not connected"
    		return None
    	details = procds["PathList"][0].lstrip("/").split("/")
    	# Check that primary dataset exists
        if not self._dbsAPI.listPrimaryDatasets(details[0]):
        	self._dbsAPI.insertPrimaryDataset(DbsPrimaryDataset(Name=details[0],Type="test"))
        	
        if self._dbsAPI.listPrimaryDatasets(details[0]):
        	parent = None
        	if len(procds["ParentList"]) > 0:
        		parent = procds["ParentList"][0].lstrip("/").split("/")
        	if parent is not None:
        		parentDatasets = self._dbsAPI.listProcessedDatasets(parent[0],parent[2],parent[1])
        		# If only one possible parent exists, use it as parent
        		if len(parentDatasets) == 1:
        			procds["ParentList"][0] = parentDatasets[0]['PathList'][0]
        		if len(parentDatasets) == 0:
        			#if the parent is 
        			if not re.search("---",parent[1]):
        				self.dbsPublish(CastorToDbsFormatter(procds["ParentList"][0]).getDataset(), None)
        			else:
        				print "Parent dataset missing, please add parent before proceeding"
        				return None
        		else:
        			"Please use specific parent name, not REGEX"
        			return None
        			
                	
        	# Check if dataset already exists
        	dbsOutput = self._dbsAPI.listProcessedDatasets(details[0],details[2],details[1])
        	  	
        	# Check if self._procds doesn't exist
        	if len(dbsOutput) == 0:
        		print "Adding Dataset to DBS"
        		
        		# Add Dataset
        		self._dbsAPI.insertProcessedDataset(procds)
        		block = DbsFileBlock (Name=procds['PathList'][0]+"#1")
        		# Add the corresponding file block
        		self._dbsAPI.insertBlock(procds['PathList'][0], block)
    		else:
    			print "Dataset already on DBS"
    			
    		return self._getDbsIDs(procds['PathList'][0])
    		
        else:
        	print "Primary dataset does not exist"
        	return None
        
    def savannahPublish(self, procds, opts, comment, fileOps):
    	# Check if user has dataset files, and DO NOT allow publish if they do not
    	if fileOps.getRootFiles() is None or len(fileOps.getRootFiles()) == 0:
    		taskID = findDSOnSav.getTaskID(procds['PathList'][0], opts['category_id'], self._username, self._password, False)
    		ds = procds['PathList'][0]
    		if taskID is None:
    			taskID = findDSOnSav.getTaskID(getCastor(procds['PathList'][0]), opts['category_id'], self._username, self._password, False)
    			ds = getCastor(procds['PathList'][0])
    		#For old datasets
    		if taskID is None:
    			taskID = findDSOnSav.getTaskID(getCastor(procds['PathList'][0]), '100', self._username, self._password, False)
    			ds = getCastor(procds['PathList'][0])
        	raise NameError("No dataset found on EOS, exiting",ds, taskID)
        	return None
        
        
    	test = False
    	# If item is test
    	if 'category_id' in opts:
    		if opts['category_id'] == '101': test = True

    	
    	taskID = findDSOnSav.getTaskID(procds['PathList'][0], opts['category_id'], self._username, self._password, False)
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
    	
		
    def _getDbsIDs(self, path):
    	if dbsAPI is not None:	
    		dbsID = None
    		parentID = None
    		# Get ID from DBS
    		dbsID = int(self._dbsAPI.executeQuery(query="find procds.id where dataset="+path, type='exe').split("results>")[1].split("procds.id>")[1].rstrip("</"))
        	# Get stored Parent name
        	parentName = self._dbsAPI.executeQuery(query="find procds.parent where dataset="+path, type='exe').split("results>")[1].split("procds.parent>")[1].rstrip("</")
        	
        	# Get parents ID
        	print parentName
        	# Create name
        	if len(parentName) > 0:
        		parts = path.lstrip("/").split("/")
        		parts[1] = parentName
        		parentName = "/" + "/".join(parts)
        		parentID = int(self._dbsAPI.executeQuery(query="find procds.id where dataset="+parentName, type='exe').split("results>")[1].split("procds.id>")[1].rstrip("</"))
        		
        	return dbsID, parentID
        else: return None, None
    
    
    def cmgdbPublish(self, procds, dbsID, taskID, test, fileOps):
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
    				
    		cmgdbID = self._cmgdbAPI.addDataset(procds['PathList'][0],getCastor(procds['PathList'][0]),fileOps.getLFN(), getDbsUser(procds['PathList'][0]),parentID)
    	
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
