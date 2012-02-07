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
    def __init__(self, username, password, dbsApi):
        self._dbsAPI = dbsApi
        self._cmgdbAPI=CmgdbApi()
        self._username = username
        self._password = password
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
        
    def publish(self, procds, comment, opts):
    	# Initialise values for later use
    	test = False
    	dbsID = None
    	parentDbsID = None
    	cmgdbID = None
    	
    	# If item is test
    	if 'category_id' in opts:
    		if opts['category_id'] == '101': test = True

    	# Run checks before wasting proccessing
    	details = procds['PathList'][0].lstrip("/").split("/")
    	
    	# Get file information
        fileOps = FileOps(getCastor(procds['PathList'][0]), getDbsUser(procds['PathList'][0]))
    	
    	
    	
    	# Check if user has dataset files, and DO NOT allow publish if they do not
    	if len(fileOps.getRootFiles()) == 0:
        	raise NameError("No dataset files found on EOS, exiting")
        	return None

    	
    	# Also publish on dbs HERE
    	self.dbsPublish(procds)
    	
    	self.cmgdbPublish(procds)
    	
    	self.savannahPublish(procds, opts, comment)
    	
            
    def dbsPublish(self, procds):
    	
    	# Get file information
        fileOps = FileOps(getCastor(procds['PathList'][0]), getDbsUser(procds['PathList'][0]))
        
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
        
    def savannahPublish(self, procds, opts, comment):
    	# Get file information
        fileOps = FileOps(getCastor(procds['PathList'][0]), getDbsUser(procds['PathList'][0]))
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
    	#if fileOps.getLFNGroups() is not None:
    		#self.savannah.appendExtra(fileOps.getLFNGroups())
    		
    	
            
    	newTask = self.savannah.publish() 
        if newTask is None:
    		print "Unable to publish Dataset to Savannah, an error occured"
    		return None
    	elif newTask is taskID:
        	print "Comment added to Savannah"
        else:
    		print "Dataset published to Savannah"
    	
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
    
    
    def cmgdbPublish(self, procds, dbsID, taskID, test):
    	if self._cmgdbAPI is None:
    		return None
    	# Get file information
        fileOps = FileOps(getCastor(procds['PathList'][0]), getDbsUser(procds['PathList'][0]))
    	tags = fileOps.getTags()
    	release = fileOps.getRelease()
    	
    	if taskID is None and dbsID is None:
    		raise NameError("No dataset found, nothing added to CMGDB")
    		return None
    		
    	# See if cmgdb already has record of ds with sav
    	cmgdbID = self._cmgdbAPI.getDatasetIDWithTaskID(taskID, test)
    	
    	# if no entry found, test (or non-test) set should be checked too
    	
    	if cmgdbID is None:
    		categoryID = '101'
    		if test:
    			categoryID = '103'	
    		altTaskID = findDSOnSav.getTaskID(procds['PathList'][0], categoryID, self._username, self._password, False)
    		cmgdbID = self._cmgdbAPI.getDatasetIDWithTaskID(altTaskID, (not test))
    		
    			
    	
    	# If not see if dbs record exists
    	if cmgdbID is None:
    		cmgdbID = self._cmgdbAPI.getDatasetIDWithDbsID(dbsID)
    	
    	# Add entry if nothing found
    	if cmgdbID is None:
    		cmgdbID = self._cmgdbAPI.addDataset(procds['PathList'][0], getDbsUser(procds['PathList'][0]))
    	
    	self._cmgdbAPI.addDbsID(cmgdbID, dbsID)
    	self._cmgdbAPI.addTaskID(cmgdbID, taskID, test)
    	
    	# Add tags to CMGDB
    	if tags is None or len(tags) is 0: return None
    	tagIDs = []
    	setID = None
    	if tags:
    		for package in tags:
    			self._cmgdbAPI.addTag(package,tags[package])
    			tagIDs.append(self._cmgdbAPI.getTagID(package, tags[package]))
    		setID = self._cmgdbAPI.addSet(tagIDs,release)
    	if setID is not None: self._cmgdbAPI.addTagSetID(setID, cmgdbID)
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
