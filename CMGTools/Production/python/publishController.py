#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

from CMGTools.Production.cmgdbToolsApi import CmgdbToolsApi
from CMGTools.Production.savannahFormatter import SavannahFormatter
from datetime import *
import CMGTools.Production.findDSOnSav as findDSOnSav
import sys, re


class PublishController(object):
	"""This class controls the interactions between a user and the two publishing platforms, Savannah and CMGDB"""
	def __init__(self, username, password):
		"""Initialise CMGDB and set username and password
			
		'username' takes the NICE username of the current user, NOT the files owner on EOS
		'password' takes the NICE password of the current user
		'force' takes a boolean value which determines whether a lack of log file can be ignored
		"""
		self._cmgdbAPI=CmgdbToolsApi()
		self._cmgdbAPI.connect()
		self._username = username
		self._password = password
	
	def cmgdbOnline(self):
		"""Returns True if CMGDB is online and working"""
		if self._cmgdbAPI is not None: return True
		else: return False
	
	def loginValid(self):
		"""Returns true if the login values that the user provided were a valid CMG group NICE login"""
		return findDSOnSav.validLogin(self._username, self._password)
	
	def savannahPublish(self, datasetDetails):
		"""Publish dataset details to savannah and returns task ID
			
		'datasetDetails' takes a dict object which contains all of the datasets details. This object is strictly definied.
			
		The function sends the dataset information to a SavannahFormatter object, in the order of appearance in Savannah
		"""
		
		# Initialise SavannahFormatter object and add the main attributes
		self.savannah = SavannahFormatter(self._username, self._password, datasetDetails['TaskID'], datasetDetails['SavannahOptions'])
		
		#Append all the elements of the integrity check to the savannah output.
		if datasetDetails['CMGDBName']is not None:self.savannah.appendField("*CMGDB Name:* "+datasetDetails['CMGDBName'])
		if datasetDetails['SampleName']is not None:self.savannah.appendField("*Sample Name:* "+datasetDetails['SampleName'])
		if datasetDetails['LFN']is not None:self.savannah.appendField("*LFN:* "+datasetDetails['LFN'])
		if datasetDetails['EOSPath']is not None:self.savannah.appendField("*EOS Path:* "+datasetDetails['EOSPath'])
		if datasetDetails['Comment']is not None:self.savannah.appendField("*User Comment:* "+datasetDetails['Comment'])
		if datasetDetails['ParentSavannahString']is not None:self.savannah.appendField("*Parent:* "+datasetDetails['ParentSavannahString'])
		elif datasetDetails['ParentSampleName'] is not None:self.savannah.appendField("*Parent:* "+datasetDetails['ParentSampleName'])
		if datasetDetails['DateCreated']is not None:self.savannah.appendField("*Date Created:* "+date.fromtimestamp(int(datasetDetails['DateCreated'])).strftime('%d-%m-%Y'))
		if datasetDetails['PhysicsGroup'] is not None:self.savannah.appendField("*Physics Group:* "+datasetDetails['PhysicsGroup'])
		if datasetDetails['PrimaryDataset'] is not None:
			self.savannah.appendField("*Primary Dataset:* "+datasetDetails['PrimaryDataset'])
		if datasetDetails['TierList'] is not None:
			tiers = ""
			for i in datasetDetails['TierList']:
				tiers += "\t"+i+"\n"
				self.savannah.appendField("*Tier List:* "+ tiers)
		if datasetDetails["Status"] is not None:self.savannah.appendField("*Status:* "+ datasetDetails["Status"])
		if datasetDetails['FileOwner'] is not None:self.savannah.appendField("*Created By:* "+ datasetDetails["FileOwner"])
		if datasetDetails['Tags'] is not None:    
			detailString = ""
			for row in datasetDetails['Tags']:
				tag = row['tag']
				package = row['package']
				detailString +="_"+tag+"_ - "+package +"\n"
			if detailString is not "":
				self.savannah.appendField("\n*Tags:*\n"+detailString)
		
		if datasetDetails['TotalJobs'] is not None:self.savannah.appendField("*Total Jobs:* "+str(datasetDetails['TotalJobs']))
		if datasetDetails['TotalFilesMissing'] is not None:self.savannah.appendField("*Total Missing Files:* "+str(datasetDetails['TotalFilesMissing']))
		if datasetDetails['TotalFilesGood'] is not None:self.savannah.appendField("*Total Good Files:* "+str(datasetDetails['TotalFilesGood']))
		if datasetDetails['TotalFilesBad'] is not None:self.savannah.appendField("*Total Bad Files:* "+str(datasetDetails['TotalFilesBad']))
		if datasetDetails['PrimaryDatasetEntries'] is not None:self.savannah.appendField("*Primary Dataset Entries:* "+str(datasetDetails['PrimaryDatasetEntries']))
		if datasetDetails['FileEntries'] is not None:self.savannah.appendField("*Total entries in directory:* "+str(datasetDetails['FileEntries']))
		if datasetDetails['DirectorySizeInTB'] is not None:self.savannah.appendField("*Directory Size:* "+str(datasetDetails['DirectorySizeInTB'])+" TB")
		if datasetDetails['BadJobs'] is not None:
			jobs = "*Bad Jobs:* "
			for job in datasetDetails['BadJobs']:jobs += str(job)+", "
			self.savannah.appendField(jobs)
		if datasetDetails['FileGroups'] is not None:
			for group_name in datasetDetails['FileGroups']:
				self.savannah.appendField("*-----File Group:* "+group_name+"-----")
				if datasetDetails['FileGroups'][group_name]["SizeInTB"] is not None:
					self.savannah.appendField("*Dataset Size:* "+str(datasetDetails['FileGroups'][group_name]["SizeInTB"]))
				if datasetDetails['FileGroups'][group_name]["PrimaryDatasetFraction"] is not None:
					self.savannah.appendField("*Primary Dataset Fraction used:* "+str(datasetDetails['FileGroups'][group_name]["PrimaryDatasetFraction"]))
				if datasetDetails['FileGroups'][group_name]["FileEntries"] is not None:
					self.savannah.appendField("*File Entries:* "+str(datasetDetails['FileGroups'][group_name]["FileEntries"]))
				if datasetDetails['FileGroups'][group_name]['NumberMissingFiles'] is not None:self.savannah.appendField("*Total Good Files:* "+str(datasetDetails['TotalFilesGood']))
				if datasetDetails['FileGroups'][group_name]['NumberBadFiles'] is not None:self.savannah.appendField("*Total Bad Files:* "+str(datasetDetails['TotalFilesBad']))
				if datasetDetails['FileGroups'][group_name]["MissingFiles"] is not None:
					missingFiles ="*Missing Files:*\n"
					for missingFile in datasetDetails['FileGroups'][group_name]["MissingFiles"]: missingFiles+="* "+ missingFile+"\n"
					self.savannah.appendField(missingFiles)
				if datasetDetails['FileGroups'][group_name]["BadFiles"] is not None:
					badFiles ="*Bad Files:*\n"
					for badFile in datasetDetails['FileGroups'][group_name]["BadFiles"]: badFiles+="* "+ badFile+"\n"
					self.savannah.appendField(badFiles)
				
			
			# Publish to Savannah
		newTask = self.savannah.publish()
		if newTask is None:
			print "Unable to publish Dataset to Savannah, an error occured"
			return None, datasetDetails['ParentTaskID']
		elif newTask is datasetDetails['TaskID']:
			print "Comment added to Savannah"
			print "URL: https://savannah.cern.ch/task/?"+newTask
		else:
			print "Dataset published to Savannah"
			print "URL: https://savannah.cern.ch/task/?"+newTask
			datasetDetails['TaskID']=newTask
		return datasetDetails['TaskID'], datasetDetails['ParentTaskID']
	
	def cmgdbPublish(self, datasetDetails):
		"""Publish dataset information to CMGDB, and return unique CMGDB dataset ID
		
		'datasetDetails' takes a dict object which contains all of the datasets details. This is a strictly defined stucture.
		"""
		if self._cmgdbAPI is None:
			return None
		
		
		# See if cmgdb already has record of ds with sav
		datasetDetails['CMGDBID'] = self._cmgdbAPI.getDatasetIDWithName(datasetDetails['CMGDBName'])
		
		# If not add dataset
		if datasetDetails['CMGDBID'] is None:
			datasetDetails['CMGDBID'] = self._cmgdbAPI.addDataset(datasetDetails['CMGDBName'],datasetDetails['SampleName'],datasetDetails["LFN"], datasetDetails['FileOwner'],datasetDetails['ParentCMGDBID'], self._username)
		# Clear 4 tables relating to bad files & jobs, and missing & duplicate files
		self._cmgdbAPI.clearDatasetBadFiles(datasetDetails['CMGDBName'],datasetDetails['CMGDBID'])
		self._cmgdbAPI.clearDatasetMissingFiles(datasetDetails['CMGDBName'],datasetDetails['CMGDBID'])
		self._cmgdbAPI.clearDatasetBadJobs(datasetDetails['CMGDBName'],datasetDetails['CMGDBID'])
		
		
		if datasetDetails["TotalJobs"] is not None:self._cmgdbAPI.addTotalJobs(datasetDetails['CMGDBID'], datasetDetails["TotalJobs"])
		if datasetDetails["TotalFilesMissing"] is not None:self._cmgdbAPI.addMissingFileNum(datasetDetails['CMGDBID'], datasetDetails["TotalFilesMissing"])
		if datasetDetails["TotalFilesGood"] is not None:self._cmgdbAPI.addGoodFileNum(datasetDetails['CMGDBID'], datasetDetails["TotalFilesGood"])
		if datasetDetails["TotalFilesBad"] is not None:self._cmgdbAPI.addBadFileNum(datasetDetails['CMGDBID'], datasetDetails["TotalFilesBad"])
		for badJob in datasetDetails["BadJobs"]:self._cmgdbAPI.addBadJob(datasetDetails['CMGDBID'], badJob)
		for group_name in datasetDetails['FileGroups']:
			group_id = self._cmgdbAPI.addFileGroup(group_name,datasetDetails['CMGDBID'])
			print "Group: %s added with ID: %d" % (group_name, group_id)
			if datasetDetails['FileGroups'][group_name]["BadFiles"] is not None:
				for badFile in datasetDetails['FileGroups'][group_name]["BadFiles"]:self._cmgdbAPI.addBadFile(datasetDetails['CMGDBName'],datasetDetails['CMGDBID'], badFile.split('/')[-1], group_id)
			if datasetDetails['FileGroups'][group_name]["MissingFiles"] is not None:
				for missingFile in datasetDetails['FileGroups'][group_name]["MissingFiles"]:self._cmgdbAPI.addMissingFile(datasetDetails['CMGDBName'],datasetDetails['CMGDBID'], missingFile.split('/')[-1],group_id)
			if datasetDetails['FileGroups'][group_name]["NumberMissingFiles"] is not None:self._cmgdbAPI.addGroupMissingFileNum(group_id, datasetDetails['FileGroups'][group_name]["NumberMissingFiles"])
			if datasetDetails['FileGroups'][group_name]["NumberBadFiles"] is not None:self._cmgdbAPI.addGroupBadFileNum(group_id, datasetDetails['FileGroups'][group_name]["NumberBadFiles"])
			if datasetDetails['FileGroups'][group_name]["NumberGoodFiles"] is not None:self._cmgdbAPI.addGroupGoodFileNum(group_id, datasetDetails['FileGroups'][group_name]["NumberGoodFiles"])
			if datasetDetails['FileGroups'][group_name]["PrimaryDatasetFraction"] is not None:self._cmgdbAPI.addGroupPrimaryDatasetFraction(group_id, datasetDetails['FileGroups'][group_name]["PrimaryDatasetFraction"])
			if datasetDetails['FileGroups'][group_name]["SizeInTB"] is not None:self._cmgdbAPI.addDatasetSize(group_id, datasetDetails['FileGroups'][group_name]["SizeInTB"])
			if datasetDetails['FileGroups'][group_name]["FileEntries"] is not None:self._cmgdbAPI.addGroupFileEntries(group_id, datasetDetails['FileGroups'][group_name]["FileEntries"])
		if datasetDetails["PrimaryDatasetFraction"] is not None:self._cmgdbAPI.addPrimaryDatasetFraction(datasetDetails['CMGDBID'], datasetDetails["PrimaryDatasetFraction"])
		if datasetDetails["PrimaryDatasetEntries"] is not None:self._cmgdbAPI.addPrimaryDatasetEntries(datasetDetails['CMGDBID'], datasetDetails["PrimaryDatasetEntries"])
		if datasetDetails["FileEntries"] is not None:self._cmgdbAPI.addFileEntries(datasetDetails['CMGDBID'], datasetDetails["FileEntries"])
		if datasetDetails["DirectorySizeInTB"] is not None:self._cmgdbAPI.addDirectorySize(datasetDetails['CMGDBID'], datasetDetails["DirectorySizeInTB"])
		
		# Add task id
		self._cmgdbAPI.addTaskID(datasetDetails['CMGDBID'], datasetDetails['TaskID'], datasetDetails['Test'])
		
		# Add tags to CMGDB
		if datasetDetails['Tags'] is None or len(datasetDetails['Tags']) is 0:
			print "No tags could be added to CMGDB as none were found"
			return datasetDetails['CMGDBID'] 
		tagIDs = []
		
		# Create hash code for the tag set
		taghash = []
		for i in datasetDetails['Tags']:
			a=hash((i['package'],i['tag']))
			taghash.append(a)
		taghash.sort()
		endhash = hash(tuple(taghash))
		
		# check if tag set is already on CMGDB
		tagSetID = self._cmgdbAPI.getTagSetID(endhash)
		
		# If it isn't found, add the tags, and the tag set
		if tagSetID is None:
			if datasetDetails['Tags']:
				tagIDs
				for row in datasetDetails['Tags']:
					tagID = self._cmgdbAPI.addTag(row["package"],row["tag"])
					if tagID is not None: tagIDs.append(tagID)
			
				tagSetID = self._cmgdbAPI.addTagSet(datasetDetails['Release'],endhash)
				for tagID in tagIDs:
					self._cmgdbAPI.addTagToSet(tagID,tagSetID)
			    	
		if tagSetID is not None: self._cmgdbAPI.addTagSetID(tagSetID, datasetDetails['CMGDBID'])
		return datasetDetails['CMGDBID']
	
