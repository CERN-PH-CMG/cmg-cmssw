#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## November 2nd 2011

import sys, os, re, subprocess, glob, tempfile, tarfile
import CMGTools.Production.eostools as eostools
import CMGTools.Production.castorBaseDir as castorBaseDir
from CMGTools.Production.edmIntegrityCheck import PublishToFileSystem
from CMGTools.Production.edmIntegrityCheck import IntegrityCheck
from CMGTools.Production.cmgdbToolsApi import CmgdbToolsApi
from CMGTools.Production.findDSOnSav import *
from CMGTools.Production.nameOps import *
from CMGTools.Production.dataset import *
from datetime import *
from CMGTools.Production.eostools import *

class DatasetInformation(object):
	
	"""Class gives access to attributes of the dataset that are stored on disk"""
	
	### Important definitions:
	### CMGDB Name is in the form which contains the file owner
	##### e.g. /DiPhotonBox_Pt-250_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1--V3---cmgtools/AODSIM
	
	### Sample Name is in the format usually used when accessing datasets on EOS
	##### e.g. /DiPhotonBox_Pt-250_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V3
	
	### Group name (or file group name) is the name of a set of files within a directory, represented by the common root
	##### e.g. 'patTuple' for the files:
	##### patTuple_2159.root and patTuple_1996.root
	
	### User or Username relates to the current user making a request/publishing NOT NECESARILY the files owner on EOS
	
	### File owner is the name of the area that stores the sample on EOS 
	##### e.g. 'cmgtools' is the file owner of a sample stored in user/cmgtools/CMG/
	##### whereas 'cmgtools_group' is the file owner of a sample stored in group/cmgtools/CMG/
	##### and 'cbern' is the file owner of a sample stored in user/cbern/CMG/
	
	def __init__(self, sampleName,fileOwner,comment, force, test, primary, username, password):
		"""Initialises attributes of object, and validates existence of dataset
		'sampleName' takes the name of the dataset e.g. /QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START44_V9B-v1/AODSIM/V3
		'fileOwner' takes the files owner on EOS e.g. cmgtools
		'force' takes True/False on whether the dataset should be considered valid if there is no logger file found
		'test' takes True/False on whether the sample is a test or not
		'username' takes the username of the person submitting the sample
		'password' takes that users password
			
		"""
		self.dataset = None
		self._report = None
		self._reportBuilt = False
		self._force = force
		self._primary = primary
		self._username = username
		self._password = password
		self._logger_file = None
		self._logger_tar_object = None
		
		self.dataset_details = {"SampleName":sampleName,
								"ParentSampleName":None,
								"CMGDBName":None,
								"ParentCMGDBName":None,
								"Comment":comment,
								"FileOwner":fileOwner,
								"PrimaryDataset":None,
								"LFN":None,
								"EOSPath":None,
								"FileGroups":None,
								"Tags":None,
								"Release":None,
								"PhysicsGroup":None,
								"TierList":None,
								"DateCreated":None,
								"TaskID":None,
								"ParentTaskID":None,
								"Status":None,
								"CMGDBID":None,
								"ParentCMGDBID":None,
								"Test":test,
								"ParentSavannahString":None,
								"BadJobs":None,
								"FileEntries":None,
								"TotalJobs":None,
								"TotalFilesMissing":None,
								"TotalFilesGood":None,
								"TotalFilesBad":None,
								"PrimaryDatasetFraction":None,
								"PrimaryDatasetEntries":None,
								"TotalFileEntries":None,
								"DirectorySizeInTB":None,
								"SavannahOptions":dict()
								}
		# Check if directory is valid
		self.checkDatasetDirectoryExists(sampleName, fileOwner)
		# Build the basic details report
		self.buildBasicDetailsReport()
		# Build bad file list (in order to fetch edm report)
		self.dataset.buildListOfBadFiles()
		self._report = self.dataset.report
		# Check if a logger file exists
		self.checkForLogger(sampleName, fileOwner)
		# Check if root files exist
		self.checkForRootFiles()
		# Check if an EDM report exists
		self.buildEDMReport()
		# Divide the root files into separate groups
		self.createFileGroups()
	
	def createFileGroups(self):
		"""Builds a dictionary containing the different file groups within a directory"""
		# First get the root files and divide them into file groups
		self.dataset.buildListOfFiles()
		
		def isCrabFile(name):
			_, fname = os.path.split(name)
			base, _ = os.path.splitext(fname)
			return re.match(".*_\d+_\d+_\w+$", base) is not None, base
		
		def removeIndex(name):
			isCrab, base = isCrabFile(name)
			tokens = base.split('_')
			if isCrab and len(tokens) > 2:
				del(tokens[-3])
				del(tokens[-2])
				del(tokens[-1])
				return "_".join(tokens)
			else:
				del(tokens[-1])
				return "_".join(tokens)
			return None
		
		def getIndex(name):
			isCrab, base = isCrabFile(name)
			tokens = base.split('_')
			if isCrab and len(tokens) > 2:
				return int(tokens[-3])
			else:
				return int(tokens[-1])
			return None
		
		self.dataset_details['FileGroups'] = dict()
		for fileName in self.dataset.files:
			
			# Distinguish group
			name = removeIndex(fileName)
			if name in self.dataset_details['FileGroups']:
				self.dataset_details['FileGroups'][name]['Files'].append(fileName)
			else:
				instanceGroup = {"SizeInTB":None,
								"IsCrab":None,
								"Files":None,
								"FileEntries":None,
								"PrimaryDatasetFraction":None, 
								"BadFiles":None,
								"NumberBadFiles":None,
								"MissingFiles":None,
								"NumberMissingFiles":None,
								"GoodFiles":None,
								"NumberGoodFiles":None,
								"TotalJobs":None}
				instanceGroup['Files']=[]
				instanceGroup['Files'].append(fileName)
				isCrab, base = isCrabFile(fileName)
				instanceGroup['IsCrab']=isCrab
				self.dataset_details['FileGroups'][name]=instanceGroup
		# Sort the file groups
		for file_group in self.dataset_details['FileGroups']:
			if self.dataset_details['FileGroups'][file_group]['IsCrab']:
				self.dataset_details['FileGroups'][file_group]['Files'].sort(key=lambda x: int(getIndex(x)))
			else:
				self.dataset_details['FileGroups'][file_group]['Files'].sort(key=lambda x: int(getIndex(x)))
			self.dataset_details['FileGroups'][file_group]['TotalJobs'] = getIndex(self.dataset_details['FileGroups'][file_group]['Files'][-1])+1
	
	def createFileGroupDetailString(self, group_name):
		"""Build a string containing the important information pertaining to a file group
			
		'group_name' takes the groups name as a string e.g. 'cmgtuple'"""
		if self.dataset_details is None or 'FileGroups' not in self.dataset_details or group_name not in self.dataset_details['FileGroups']:
			return None
		string = "\t----"+group_name+"----\n"
		if self.dataset_details['FileGroups'][group_name]['SizeInTB'] is not None:
			string +="\tDataset size: "+str(self.dataset_details['FileGroups'][group_name]['SizeInTB'])+" TB\n"
		if self.dataset_details['FileGroups'][group_name]['FileEntries'] is not None:
			string +="\tDataset file entries: "+str(self.dataset_details['FileGroups'][group_name]['FileEntries'])+"\n"
		if self.dataset_details['FileGroups'][group_name]['PrimaryDatasetFraction'] is not None:
			string +="\tFraction of primary dataset used: "+str(self.dataset_details['FileGroups'][group_name]['PrimaryDatasetFraction'])+"\n"
		if self.dataset_details['FileGroups'][group_name]['NumberBadFiles'] is not None:
			string +="\tNumber of bad files: "+str(self.dataset_details['FileGroups'][group_name]['NumberBadFiles'])+"\n"
			if self.dataset_details['FileGroups'][group_name]['BadFiles']:
				for i in self.dataset_details['FileGroups'][group_name]['BadFiles']:
					string +="\t-- "+i+"\n"
		if self.dataset_details['FileGroups'][group_name]['NumberMissingFiles'] is not None:
			string +="\tNumber of missing files: "+str(self.dataset_details['FileGroups'][group_name]['NumberMissingFiles'])+"\n"
			if self.dataset_details['FileGroups'][group_name]['MissingFiles']:
				for i in self.dataset_details['FileGroups'][group_name]['MissingFiles']:
					string +="\t-- "+i+"\n"
		return string
	
	def createDirectoryDetailString(self):
		"""Build a string containing the important information pertaining to the directory"""
		if self.dataset_details is None :
			return None
		string = "Dataset Name: "+self.dataset_details['SampleName']+"\n"
		string += "CMGDB Name: "+self.dataset_details['CMGDBName']+"\n"
		string += "File Owner: "+self.dataset_details['FileOwner']+"\n"
		if self.dataset_details['DirectorySizeInTB'] is not None:
			string +="Directory size: "+str(self.dataset_details['DirectorySizeInTB'])+" TB\n"
		if self.dataset_details['FileEntries'] is not None:
			string +="Directory file entries: "+str(self.dataset_details['FileEntries'])+"\n"
		if self.dataset_details['PrimaryDatasetEntries'] is not None:
			string +="Primary dataset file entries: "+str(self.dataset_details['PrimaryDatasetEntries'])+"\n"
		return string
	
	def checkDatasetDirectoryExists(self, sampleName, fileOwner):
		"""Checks that the directory of the sample exists on EOS
			
		'sampleName' takes the name of the sample as a string
		'fileOwner' takes the file owner on EOS as a string """
		try:
			self.dataset = Dataset(sampleName, fileOwner)
		except:
			print sampleName
			print fileOwner
			raise NameError("ERROR: Dataset not found on EOS, please enter correct dataset name and fileowner\n")
	
	def checkForLogger(self, sampleName, fileOwner):
		"""Checks the EOS directory for a Logger.tgz file, if not found, escapes
			
		'sampleName' takes the name of the sample as a string
		'fileOwner' takes the file owner on EOS as a string
		"""
		if len(eostools.matchingFiles(self.dataset_details['LFN'], "Logger.tgz"))  == 1 or self._force:
			self.createLoggerTemporaryFile()
			return True
		else: raise NameError("ERROR: No Logger file found, and force not requested.\n")
	
	def createLoggerTemporaryFile(self):
		"""Build a temporary logger file object and tarfile object to be used when retrieving tags and jobs"""
		try:
			self._logger_file = tempfile.NamedTemporaryFile()
			os.system("cmsStage -f "+self.dataset_details['LFN'] + "/Logger.tgz "+self._logger_file.name)
			self._logger_tar_object = tarfile.open(fileobj=self._logger_file)
			if len(self._logger_tar_object.getmembers())==0: 
				print "\nERROR: Failed to stage logger file"
				self._logger_tar_object = None
				self._logger_file = None
		except:
			print "\nERROR: Failed to stage logger file"
			self._logger_tar_object = None
			self._logger_file = None
	
	def checkForRootFiles(self):
		"""Checks the dataset object for presence of root files, if not found, escapes"""
		self.dataset.buildListOfFiles()
		if len(self.dataset.files)  > 0:
			self.dataset_details['RootFiles']=self.dataset.files
			return True
		else: NameError("ERROR: Dataset root files not found on EOS, dataset is invalid\n")
	
	def buildAllReports(self):
		"""Builds all of the optional reports in the class"""
		# Get available info from Savannah
		self.buildSavannahReport()
		
		# Get available info from CMGDB
		self.buildCMGDBReport()
		
		# Collect data from Logger file
		self.buildJobsReport()
		self.buildShowtagsReport()
		
		# Analyse files in dataset
		self.buildMissingFileReport()
		self.buildDatasetSizeReport()
		
		# Try EDM
		self.buildBadFileReport()
		self.buildGoodFileReport()
		self.buildFileEntriesReport()
	
	def buildCMGDBReport(self):
		"""Finds CMGDB ID of parent sample and sample"""
		cmgdbAPI=CmgdbToolsApi()
		cmgdbAPI.connect()
		self.dataset_details['CMGDBID'] = cmgdbAPI.getDatasetIDWithName(self.dataset_details['CMGDBName'])
		self.dataset_details['ParentCMGDBID'] = cmgdbAPI.getDatasetIDWithName(self.dataset_details['ParentCMGDBName'])
	
	def buildBasicDetailsReport(self):
		"""Gets the basic dataset information"""
		self.dataset_details['LFN'] = self.dataset.lfnDir
		self.dataset_details['EOSPath']= self.dataset.castorDir
		self.dataset_details['CMGDBName'] = getCMGDBWithUser(self.dataset.name, self.dataset.user)
		if not self._primary:
			self.dataset_details['ParentSampleName']=getParentWithSampleName(self.dataset_details['SampleName'])
			self.dataset_details['PrimaryDatasetName'] = self.dataset.name.lstrip("/").split("/")[0]
			self.dataset_details['ParentCMGDBName'] = getUnknownParentWithCMGDB(self.dataset_details['CMGDBName'])
			self.dataset_details['Tiers'] = self.dataset_details['SampleName'].lstrip("/").split("/")[2]
			# Can make addition here for physics group
		self.dataset_details['PhysicsGroup'] = "CMG"
		self.dataset_details['DateCreated'] = datetime.now().strftime("%s")
		self.dataset_details['Status'] = "VALID"
	
	def buildSavannahReport(self):
		"""Access Savannah and get all information relating to the dataset"""
		dayMonthYear = date.fromtimestamp(int(self.dataset_details['DateCreated'])).strftime('%d-%B-%Y').split("-")
		self.dataset_details['SavannahOptions']['planned_starting_date_dayfd']=dayMonthYear[0].lstrip("0")
		self.dataset_details['SavannahOptions']['planned_starting_date_monthfd']=dayMonthYear[1]    
		self.dataset_details['SavannahOptions']['planned_starting_date_yearfd']=dayMonthYear[2]
		
		# Create test category MAY BE REMOVED IF TEST NOT REQUIRED
		if self.dataset_details['Test']:
			category_id='101'
		else:category_id='103'
		
		# More savannah opts
		self.dataset_details['SavannahOptions']['assigned_to']=self.dataset_details['FileOwner']
		self.dataset_details['SavannahOptions']['summary']=self.dataset_details['CMGDBName']
		self.dataset_details['TaskID'] = getTaskID(self.dataset_details['CMGDBName'], category_id, self._username, self._password, False)
		
		# If task already exists you NO NOT WANT TO CHANGE THE CATEGORY so only add this field if it is a new DS
		if self.dataset_details['TaskID'] is None: self.dataset_details['SavannahOptions']['category_id']=category_id
		
		# If dataset is primary
		if self._primary:
			print "NOTE: Dataset is a primary dataset"
			return None
		
		# Check if parent exists
		self.dataset_details['ParentTaskID'] = getTaskID(self.dataset_details['ParentCMGDBName'], category_id, self._username, self._password, True)
		
		if self.dataset_details['ParentTaskID'] is not None and len(self.dataset_details['ParentTaskID']) > 0:
			if len(self.dataset_details['ParentTaskID'])>1: raise NameError("Multiple possible parents found for dataset: "+self.dataset_details['CMGDBName']+". Please find the duplicate and remove it" )
			self.dataset_details['ParentSavannahString']= "[https://savannah.cern.ch/task/?"+str(self.dataset_details['ParentTaskID'][0])+" "+getNameWithID(self.dataset_details['ParentTaskID'][0])+"]"
			self.dataset_details['ParentTaskID'] = self.dataset_details['ParentTaskID'][0]
			self.dataset_details['ParentCMGDBName'] = getNameWithID(self.dataset_details['ParentTaskID'])
		# If Parent is a CMS dataset (i.e. not a CMG dataset)
		elif not re.search("--", self.dataset_details['ParentCMGDBName']):
			self.dataset_details['ParentSavannahString']= "[https://cmsweb.cern.ch/das/request?view=list&limit=10&instance=cms_dbs_prod_global&input=dataset%3D%2F" +self.dataset_details['ParentCMGDBName'].lstrip("/").split("/")[0]+ "%2F" +self.dataset_details['ParentCMGDBName'].lstrip("/").split("/")[1]+"%2F" + self.dataset_details['ParentCMGDBName'].lstrip("/").split("/")[2]+ " "+ self.dataset_details['ParentCMGDBName']+"]"
			self.dataset_details['ParentTaskID'] = None
		
		# If parent doesn't exist throw exception
		else: raise NameError("No Parent was found for Dataset: "+self.dataset_details['CMGDBName']+" not publishing.",self.dataset_details['TaskID'])
	
	def buildShowtagsReport(self):
		"""Stage the logger_showtags.txt file in Logger.tgz and get a list of tags and the CMSSW version"""
		
		if self._logger_tar_object is None:
			return None
		showtagsFile=None
		lines = None
		try:
			showtagsFile=self._logger_tar_object.extractfile("Logger/logger_showtags.txt")
			lines = showtagsFile.read().split("\n")
		except:
			print "ERROR: No showtags file found in logger"
			return None
		#Sets tags and release 
		# Get the release from the first line of showtags
		self.dataset_details['Release'] = lines[0].split(":")[1].lstrip().rstrip()
		# Creates regexp to test incoming lines from 'showtags'
		tagPattern = re.compile('^\s*(\S+)\s+(\S+)\s*$')
		tags = []
		# For every line in showtags...
		for line in lines:
				# Check if it is a tag
				m = tagPattern.match(line)
				# if line is a tag...
				if m!=None:
					# get package name
					package = m.group(2)
					# get tag name
					tag = m.group(1)
					if tag is not "NoCVS" and tag is not "NoTag":
						tags.append({"package":package,"tag":tag})
		self.dataset_details['Tags'] = tags
	
	def buildJobsReport(self):
		"""Stage the logger_jobs.txt file in Logger.tgz and read the total number of jobs"""
		
		if self._logger_tar_object is None:
			return None
		nJobs = None
		try:
			# Open the file in the logger and get the value
			nJobsFile=tar.extractfile("Logger/logger_jobs.txt")
			nJobs = int(nJobsFile.read().split(": ")[1].split("\n")[0])
		except:
			print "ERROR: No jobs file found in logger (non-fatal error)"
			return None
		
		# Set the class variable
		if nJobs is not None:self.dataset_details['TotalJobs']=nJobs
		
		# COLIN TODO Change here if -1 is prefered for if no jobs report found
		if self.dataset_details['TotalJobs'] is not None:
			for file_group in self.dataset_details['FileGroups']:
				self.dataset_details['FileGroups'][file_group]['TotalJobs'] = self.dataset_details['TotalJobs']
		else:
			for file_group in self.dataset_details['FileGroups']:
				self.dataset_details['TotalJobs']=self.dataset_details['FileGroups'][file_group]['TotalJobs']
	
	def buildDatasetSizeReport(self):
		"""Get the dataset size for both the directory, and the individual file groups"""
		if self.dataset_details['FileGroups'] is None: return None
		self.dataset.extractFileSizes()
		self.dataset_details['DirectorySizeInTB'] = 0.0
		for group_name in self.dataset_details['FileGroups']:
			size_in_bytes=0
			for file_name in self.dataset_details['FileGroups'][group_name]['Files']:
				size_in_bytes+=int(self.dataset.filesAndSizes[file_name])
			self.dataset_details['FileGroups'][group_name]["SizeInTB"] = float(size_in_bytes)/1000/1000/1000/1000
			self.dataset_details['DirectorySizeInTB'] += float(size_in_bytes)/1000/1000/1000/1000
	
	def buildMissingFileReport(self):
		"""Create a list of missing files and no. total missing files for each file group"""
		def isCrabFile(name):
			_, fname = os.path.split(name)
			base, _ = os.path.splitext(fname)
			return re.match(".*_\d+_\d+_\w+$", base) is not None, base
		
		def getIndex(name):
			isCrab, base = isCrabFile(name)
			tokens = base.split('_')
			if isCrab and len(tokens) > 2:
				return int(tokens[-3])
			else:
				return int(tokens[-1])
			return None
		
		def createFileName(groupName, number, isCrab):
			if isCrab:
				return groupName + "_" + str(number)+"_0_***"
			else:
				return groupName + "_" + str(number)
		
		if self.dataset_details['FileGroups'] is None: return None
		totalMissing = 0
		for group_name in self.dataset_details['FileGroups']:
			numbers = []
			file_list = self.dataset_details['FileGroups'][group_name]['Files']
			missing_files = []
			for file_name in file_list:
				#create a list of found file numbers
				numbers.append(getIndex(file_name))
			for number in range(0, self.dataset_details['FileGroups'][group_name]['TotalJobs']):
				if number not in numbers:
					missing_files.append(createFileName(group_name,number,self.dataset_details['FileGroups'][group_name]['IsCrab']))
			if len(missing_files)>0:
				self.dataset_details['FileGroups'][group_name]['MissingFiles'] = missing_files
				self.dataset_details['FileGroups'][group_name]['NumberMissingFiles']=len(missing_files)
			else: self.dataset_details['FileGroups'][group_name]['NumberMissingFiles']=0
			totalMissing += len(missing_files)
	
	def buildBadFileReport(self):
		"""Create a list of bad files and no. total bad files for each file group"""
		def isCrabFile(name):
			_, fname = os.path.split(name)
			base, _ = os.path.splitext(fname)
			return re.match(".*_\d+_\d+_\w+$", base) is not None, base
		
		def removeIndex(name):
			isCrab, base = isCrabFile(name)
			tokens = base.split('_')
			if isCrab and len(tokens) > 2:
				del(tokens[-3])
				del(tokens[-2])
				del(tokens[-1])
				return "_".join(tokens)
			else:
				del(tokens[-1])
				return "_".join(tokens)
			return None
		
		def getIndex(name):
			isCrab, base = isCrabFile(name)
			tokens = base.split('_')
			if isCrab and len(tokens) > 2:
				return int(tokens[-3])
			else:
				return int(tokens[-1])
			return None
		
		def getFileName(name):
			return name.split("/")[-1]
		
		if self.dataset_details['FileGroups'] is None: return None
		for group_name in self.dataset_details['FileGroups']:
			bad_files = []
			for file_name in self.dataset.bad_files:
				name = removeIndex(file_name)
				if name ==group_name and self.dataset.bad_files[file_name]=='MarkedBad':
					bad_files.append(getFileName(file_name))
			if len(bad_files)>0: 
				self.dataset_details['FileGroups'][group_name]['BadFiles'] = bad_files
				self.dataset_details['FileGroups'][group_name]['NumberBadFiles']=len(self.dataset_details['FileGroups'][group_name]['BadFiles'])
			else: self.dataset_details['FileGroups'][group_name]['NumberBadFiles']=0
	
	def buildGoodFileReport(self):
		"""Creates a list of good files and no. total good files for each file group"""
		def isCrabFile(name):
			_, fname = os.path.split(name)
			base, _ = os.path.splitext(fname)
			return re.match(".*_\d+_\d+_\w+$", base) is not None, base
		
		def removeIndex(name):
			isCrab, base = isCrabFile(name)
			tokens = base.split('_')
			if isCrab and len(tokens) > 2:
				del(tokens[-3])
				del(tokens[-2])
				del(tokens[-1])
				return "_".join(tokens)
			else:
				del(tokens[-1])
				return "_".join(tokens)
			return None
		
		def getIndex(name):
			isCrab, base = isCrabFile(name)
			tokens = base.split('_')
			if isCrab and len(tokens) > 2:
				return int(tokens[-3])
			else:
				return int(tokens[-1])
			return None
		
		def getFileName(name):
			return name.split("/")[-1]
		
		if self.dataset_details['FileGroups'] is None: return None
		for group_name in self.dataset_details['FileGroups']:
			good_files = []
			for file_name in self.dataset.good_files:
				if removeIndex(file_name)==group_name:
					good_files.append(getFileName(file_name))
			if len(good_files)>0:
				self.dataset_details['FileGroups'][group_name]['GoodFiles'] = good_files
				self.dataset_details['FileGroups'][group_name]['NumberGoodFiles']=len(self.dataset_details['FileGroups'][group_name]['GoodFiles'])
			else: self.dataset_details['FileGroups'][group_name]['NumberGoodFiles']=0
	
	def buildFileEntriesReport(self):
		"""Use EDM data to calculate no. of entries, also calculate fraction of primary dataset used"""
		if self._report is None:return None
		files = self._report['Files']
		if len(files) == 0: 
			print "ERROR: Integrity check was not completed properly, resubmission is advised."
			return 0
		for group_name in self.dataset_details['FileGroups']:
			entries = 0
			for file_name in files:
				if file_name.split("/")[-1].split("_")[0]==group_name:
					entries += files[file_name][1]
			self.dataset_details['FileGroups'][group_name]['FileEntries']=entries
			if self.dataset_details['PrimaryDatasetEntries'] is not None and entries != 0 and self.dataset_details['PrimaryDatasetEntries'] > 0:
				self.dataset_details['FileGroups'][group_name]['PrimaryDatasetFraction']=float(entries)/float(self.dataset_details['PrimaryDatasetEntries'])
	
	def buildEDMReport(self):
		"""Retrieve the revelevant information from the EDM report and add it to the dictionary"""
		if self._report is None:
			return None
		# Check if integrity check report is there. If yes, get it.
		if 'BadJobs' in self._report:
			self.dataset_details['BadJobs'] = self._report['BadJobs']
		if 'FilesBad' in self._report:
			self.dataset_details['TotalFilesBad']=self._report['FilesBad']
		if 'FilesGood' in self._report:
			self.dataset_details['TotalFilesGood']=self._report['FilesGood']
		if 'FilesEntries' in self._report:
			self.dataset_details['FileEntries'] = self._report['FilesEntries']
		if 'PrimaryDatasetFraction' in self._report:
			self.dataset_details['PrimaryDatasetFraction'] = self._report['PrimaryDatasetFraction']
		if 'PrimaryDatasetEntries' in self._report:
			self.dataset_details['PrimaryDatasetEntries'] = self._report['PrimaryDatasetEntries']
		if 'ValidDuplicates' in self._report:
			self.dataset_details['ValidDuplicates'] = self._report['ValidDuplicates']
		
	
