
#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## November 2nd, 2011
## Refactoring: Anastasios Antoniadis
## November 9th, 2012

import sys
import os
import re
import subprocess
import glob
import tempfile
import tarfile
import CMGTools.Production.eostools as eostools
import CMGTools.Production.castorBaseDir as castorBaseDir
import datetime
from CMGTools.Production.edmIntegrityCheck import PublishToFileSystem
from CMGTools.Production.edmIntegrityCheck import IntegrityCheck
from CMGTools.Production.cmgdbToolsApi import CmgdbToolsApi
from CMGTools.Production.findDSOnSav import *
from CMGTools.Production.nameOps import *
from CMGTools.Production.dataset import *
from datetime import *
from CMGTools.Production.eostools import *
from CMGTools.Production.fileNameUtils import getFileGroup
from time import strptime


def isCrabFile(name):
    _, fname = os.path.split(name)
    base, _ = os.path.splitext(fname)              #splits fname at the '.'
    pattern = "(^.*)(_\d+$)|(^.*)(_\d+_\d+_\w+$)"  #two possible patterns - cmsbatch and crab respectively
    regex = re.compile(pattern)
    result = regex.match( base ).groups()          #the returned tuple will be either ( None, None, string, string )
                                                   #or ( string, string, None, None ) if the filename is valid

    if ( result[0] == None ):                      #if it's not cmsbatch file
        if ( result[2] == None ):              #if neither cmsbatch nor crab raise Exception
            raise NameError( "Invalid filename - Crab of cmsbatch pattern not detected" )
        else:   #else it's a crab file
            return True           #return unindexed name
    else:   #if cmsbatch
        return False          #return unindexed name

def removeIndex(name):
    _, fname = os.path.split(name)
    base, _ = os.path.splitext(fname)              #splits fname at the '.'
    pattern = "(^.*)(_\d+$)|(^.*)(_\d+_\d+_\w+$)"  #two possible patterns - cmsbatch and crab respectively
    regex = re.compile(pattern)
    result = regex.match( base ).groups()          #the returned tuple will be either ( None, None, string, string )
                                                   #or ( string, string, None, None ) if the filename is valid

    if ( result[0] == None ):                      #if it's not cmsbatch file
        if ( result[2] == None ):              #check if it is a crab file#if neither cmsbatch nor crab raise Exception
            raise NameError( "Invalid filename - Crab of cmsbatch pattern not detected" )
        else:   #else it's a crab file
            return result[2]           #return unindexed name
    else:   #if cmsbatch
        return result[0]          #return unindexed name

def getIndex(name):
    _, fname = os.path.split(name)
    base, _ = os.path.splitext(fname)               #splits fname at the '.'
    pattern = "(^.*_)(\d+$)|(^.*_)(\d+)(_\d+_\w+$)" #two possible patterns - cmsbatch and crab respectively
    regex = re.compile(pattern)
    result = regex.match( base ).groups()           #the returned tuple will be either ( None, None, string, string )
                                                    #or ( string, string, None, None ) if the filename is valid
    if ( result[1] == None ):
        if ( result[3] == None ):
            raise NameError( "Invalid filename - Crab of cmsbatch pattern not detected" )
        else:
            return int( result[3] )
    else:
        return int( result[1] )

def createFileName(groupName, number, isCrab):
    if isCrab:
        return groupName + "_" + str(number)+"_0_***"
    else:
        return groupName + "_" + str(number)

def getFileName(name):
    return name.split("/")[-1]

class DatasetInformation(object):	
	"""Class gives access to attributes of the dataset that are stored on disk"""
		
	def __init__(self, sampleName, fileOwner, comment, force, test, primary, username, password, development=False):
		"""Initialises attributes of object, and validates existence of dataset
		'sampleName' takes the name of the dataset e.g. /QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START44_V9B-v1/AODSIM/V3
		'fileOwner' takes the files owner on EOS e.g. cmgtools
		'force' takes True/False on whether the dataset should be considered valid if there is no logger file found
		'test' takes True/False on whether the sample is a test or not
		'username' takes the username of the person submitting the sample
		'password' takes that users password
		'development' takes True/False depending on whether wants to publish on the official or the devdb11 database"""
		
		self.dataset = None                                 #initializes the members
		self._report = None
		self._reportBuilt = False
		self._force = force
		self._primary = primary
		self._username = username
		self._password = password
		self._logger_file = None
		self._logger_tar_object = None
        	self.development = development
		
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
		self.dataset_details['FileGroups'] = dict()    #So for the dataset_details dictionary at key 'FileGroups' we initialize a dictionary
		# import pdb; pdb.set_trace()
		for fileName in self.dataset.files:            #for each filename in dataset_files
			
			# Distinguish group
			name = removeIndex(fileName)                                                        #removes index from the filename 
			if name in self.dataset_details['FileGroups']:                                      #if the filegroup name is in the FileGroups dictionary
				self.dataset_details['FileGroups'][name]['Files'].append(fileName)          #add the file to the FileGroup's specific instance group
			else:                                                                               #else      
				instanceGroup = {"SizeInTB":None,                                           #create an instanceGroup dictionary for that FileGroup
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
				instanceGroup['Files'].append(fileName)                  #add the file to the instanceGroup['Files']
				isCrab = isCrabFile( fileName )                          #check if the filename is generated by crab
				instanceGroup['IsCrab']=isCrab                           #then store the result in the instanceGroup['IsCrab']
				self.dataset_details['FileGroups'][name]=instanceGroup   #make the FileGroups dictionary with this specific group name point to the instanceGroup
		# Sort the file groups
		for file_group in self.dataset_details['FileGroups']:                    #for each filegroup key in the dictionary referenced by dataset_details['FileGroups']
			self.dataset_details['FileGroups'][file_group]['Files'].sort(key=lambda x: int(getIndex(x)))     #sort its files
	
	def createFileGroupDetailString(self, group_name):
		"""Build a string containing the important information pertaining to a file group
		'group_name' takes the groups name as a string e.g. 'cmgtuple'"""
		# import pdb; pdb.set_trace()
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
		except IOError as ex:
			print ex.args[0]
			exit(-1)
		except:
			print "ERROR:", sys.exc_info()[0]
			raise
	
	def checkForLogger(self, sampleName, fileOwner):
		"""Checks the EOS directory for a Logger.tgz file, if not found, escapes
		'sampleName' takes the name of the sample as a string
		'fileOwner' takes the file owner on EOS as a string
		"""
		if len( eostools.matchingFiles( self.dataset_details['LFN'], "Logger.tgz" ) )  == 1 or self._force:
			self.createLoggerTemporaryFile()
			return True
		else: 
			raise NameError("ERROR: No Logger.tgz file found for this sample. If you would like to preceed anyway, please copy Logger.tgz from your local production directory to your production directory on eos.\n")
	
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
			self.dataset_details['RootFiles'] = self.dataset.files
			return True
		else: 
			raise NameError("ERROR: Dataset root files not found on EOS, dataset is invalid\n")
	
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
		cmgdbAPI=CmgdbToolsApi(self.development)
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
			category_id = '101'
		else:category_id = '103'
		
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
			if len(self.dataset_details['ParentTaskID'])>1: 
				raise NameError("Multiple possible parents found for dataset: "+self.dataset_details['CMGDBName']+". Please find the duplicate and remove it" )

			self.dataset_details['ParentSavannahString']= "[https://savannah.cern.ch/task/?"+str(self.dataset_details['ParentTaskID'][0])+" "+getNameWithID(self.dataset_details['ParentTaskID'][0])+"]"
			self.dataset_details['ParentTaskID'] = self.dataset_details['ParentTaskID'][0]
			self.dataset_details['ParentCMGDBName'] = getNameWithID(self.dataset_details['ParentTaskID'])
		# If Parent is a CMS dataset (i.e. not a CMG dataset)
		elif not re.search("--", self.dataset_details['ParentCMGDBName']):
			self.dataset_details['ParentSavannahString']= "[https://cmsweb.cern.ch/das/request?view=list&limit=10&instance=cms_dbs_prod_global&input=dataset%3D%2F" +self.dataset_details['ParentCMGDBName'].lstrip("/").split("/")[0]+ "%2F" +self.dataset_details['ParentCMGDBName'].lstrip("/").split("/")[1]+"%2F" + self.dataset_details['ParentCMGDBName'].lstrip("/").split("/")[2]+ " "+ self.dataset_details['ParentCMGDBName']+"]"
			self.dataset_details['ParentTaskID'] = None
		
		# If parent doesn't exist throw exception
		else: 
			raise NameError("No Parent was found for Dataset: "+self.dataset_details['CMGDBName']+" not publishing.",self.dataset_details['TaskID'])
	
	def buildShowtagsReport(self):
		"""Stage the logger_showtags.txt file in Logger.tgz and get a list of tags and the CMSSW version"""
		if self._logger_tar_object is None:
			raise NameError( "ERROR: self._logger_tar_object not set" )
		showtagsFile = None
		lines = None
		try:
			showtagsFile = self._logger_tar_object.extractfile("Logger/logger_showtags.txt")
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
			if m != None:
				# get package name
				package = m.group(2)
				# get tag name
				tag = m.group(1)
				if tag is not "NoCVS" and tag is not "NoTag":
					tags.append({"package":package,"tag":tag})
		self.dataset_details['Tags'] = tags
	
	def buildJobsReport(self):                                                                             #buildJobsReport method of DatasetInformation class
		"""Stage the logger_jobs.txt file in Logger.tgz and read the total number of jobs"""
		if self._logger_tar_object is None:                                                            #if no _logger_tar_object is set return None (i think this is a problem too)
			raise NameError( "ERROR: self._logger_tar_object not set" )
		nJobs = None
		try:
			# Open the file in the logger and get the value
			# import pdb; pdb.set_trace()
			nJobsFile = self._logger_tar_object.extractfile("Logger/logger_jobs.txt")              #etract Logger/logger_jobs.txt if it exists    
			nJobs = int(nJobsFile.read().split(": ")[1].split("\n")[0])                            #read job number from file
		except:
			raise NameError( "ERROR: No jobs file found in logger" )                               #raise NameError exception if no jobs file found in logger
		
		# Set the class variable
		if nJobs == None:
			self.dataset_details['TotalJobs'] = -1
		else:
			self.dataset_details['TotalJobs'] = nJobs
		
		for file_group in self.dataset_details['FileGroups']:  #update the 'TotalJobs' value for all filegroups
			self.dataset_details['FileGroups'][file_group]['TotalJobs'] = self.dataset_details['TotalJobs']
		

	def buildDatasetSizeReport(self):
		"""Get the dataset size for both the directory, and the individual file groups"""
		if self.dataset_details['FileGroups'] is None: 
			raise NameError( "ERROR: No file groups found" )
		self.dataset.extractFileSizes()
		self.dataset_details['DirectorySizeInTB'] = 0.0
		for group_name in self.dataset_details['FileGroups']:
			size_in_bytes = 0
			for file_name in self.dataset_details['FileGroups'][group_name]['Files']:
				size_in_bytes += int(self.dataset.filesAndSizes[file_name])
			self.dataset_details['FileGroups'][group_name]["SizeInTB"] = float(size_in_bytes)/1000/1000/1000/1000
			self.dataset_details['DirectorySizeInTB'] += float(size_in_bytes)/1000/1000/1000/1000
	
	def buildMissingFileReport(self):
		"""Create a list of missing files and no. total missing files for each file group"""
 		if self.dataset_details['FileGroups'] is None:
			raise NameError( "ERROR: No file groups found" )
		totalMissing = 0                                                #start counting the missing files
		
		for group_name in self.dataset_details['FileGroups']:           #for each group name in FileGroups
			numbers = []                                                     #create empty list numbers
			missing_files = []                                               #missing_files list is empty at first

			if self.dataset_details['FileGroups'][group_name]['TotalJobs'] != -1:
				#create a list with the files of the FG
				file_list = self.dataset_details['FileGroups'][group_name]['Files']   
				isCrab = self.dataset_details['FileGroups'][group_name]['IsCrab']
     				
				os.chdir( os.environ['CMSSW_BASE'] + '/src/CMGTools/Production' )
				#create a subprocess to call cvs status for cmsBatch.py 
				p = subprocess.Popen(['cvs', 'status', '-v', 'scripts/cmsBatch.py'], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
				#pattern matches       cbern_workflow_16Nov11   	(revision: 1.6)
				#and                   abis_cmgtools            	(branch: 1.6.2)
				pattern = "\t(.*?)\s*?\t\((revision:|branch:) (.*?)\)" 
			       	regex = re.compile(pattern) 
				try:
					#create a list with lines of format abis_cmgtools            	(branch: 1.6.2) after splitting the output
					revision_info = [item for item in p.communicate()[0].split("Existing Tags:")[1].split('\n') if item != '' ]
				except:
					raise IOError( "ERROR: Unexpected output from 'cvs status scripts/cmsBatch' execution" )
				
				tag_to_revision = dict()
				for item in revision_info: 
					try:
						#match the list items to pattern
						result = regex.match(item).groups()
					except AttributeError:
						raise IOError( "ERROR: Unexpected output from 'cvs status scripts/cmsBatch' execution" )
					tag_to_revision[result[0]] = result[2]  #add tag and its correspoding branch/revision to the dictionary
				#get the revision corresponding to the dataset's tag
				for item in self.dataset_details['Tags']:
					if item['package'] == "CMGTools/Production":
						revision = tag_to_revision[item['tag']]
						break
				if revision is None:
					raise IOError( "ERROR: Unexpected output from 'cvs status scripts/cmsBatch' execution - couldn't match tag with an existing revision" )
				else:
					revision = revision.split('.')
			
				if ( int( revision[0] ) <= 1 and int( revision[1] ) <= 16 ): #Old version 
				
					if ( isCrab ):
						start_index = 1  #crab indexing 1 to n
					else:
						start_index = 0  #cmsBatch indexing 0 to n-1

						for file_name in file_list:  #for each file_name
							numbers.append( getIndex( file_name ) )  #add its indexing number to the numbers list			
						if start_index == 0:
							end_index = self.dataset_details['FileGroups'][group_name]['TotalJobs'] #e.g 22 jobs: range(0,22) = 0...21
						else:
							end_index = self.dataset_details['FileGroups'][group_name]['TotalJobs'] + 1 #e.g 22 jobs: range(1,23) = 1...22

						for number in range( start_index, end_index ):
							if number not in numbers:                            #if this number is missing from numbers list
								missing_files.append( createFileName( group_name,number, isCrab ) )
				else:   #Changed since 12-12-2012: both cmsBatch and crabfiles have indexing from 1 to n

					for file_name in file_list:
						numbers.append( getIndex( file_name ) )  #add each file's indexing number to the numbers list
					for number in range( 1, self.dataset_details['FileGroups'][group_name]['TotalJobs'] + 1  ):
						if number not in numbers: #if this number is missing from numbers list
							missing_files.append( createFileName( group_name,number, isCrab ) )
				
				if len( missing_files ) > 0:                                     #if the length of the list is > 0
					self.dataset_details['FileGroups'][group_name]['MissingFiles'] = missing_files             #add missing files
					self.dataset_details['FileGroups'][group_name]['NumberMissingFiles'] = len( missing_files )    #add NoMissFiles
				else: 
					self.dataset_details['FileGroups'][group_name]['NumberMissingFiles'] = 0                       #else 0
				totalMissing += len(missing_files)                               #update total missing files for this file group
				
			else:
				self.dataset_details['FileGroups'][group_name]['MissingFiles'] = []
				totalMissing = -1
			                           
			self.dataset_details["TotalFilesMissing"] = totalMissing
		
	
	def buildBadFileReport(self):
		"""Create a list of bad files and no. total bad files for each file group"""
		if self.dataset_details['FileGroups'] is None: 
			raise NameError( "ERROR: No file groups found" )

		for group_name in self.dataset_details['FileGroups']:
			bad_files = []
			for file_name in self.dataset.bad_files:
				name = removeIndex( file_name )
				if name == group_name and self.dataset.bad_files[file_name] == 'MarkedBad':
					bad_files.append( getFileName( file_name ) )
			if len(bad_files) > 0: 
				self.dataset_details['FileGroups'][group_name]['BadFiles'] = bad_files
				self.dataset_details['FileGroups'][group_name]['NumberBadFiles'] = len( self.dataset_details['FileGroups'][group_name]['BadFiles'] )
			else: 
				self.dataset_details['FileGroups'][group_name]['NumberBadFiles'] = 0

	def buildGoodFileReport(self):
		"""Creates a list of good files and no. total good files for each file group"""
		if self.dataset_details['FileGroups'] is None: 
			raise NameError( "ERROR: No file groups found" )

		for group_name in self.dataset_details['FileGroups']:
			good_files = []
			for file_name in self.dataset.good_files:
				if removeIndex( file_name ) == group_name:
					good_files.append( getFileName( file_name ) )
			if len(good_files)>0:
				self.dataset_details['FileGroups'][group_name]['GoodFiles'] = good_files
				self.dataset_details['FileGroups'][group_name]['NumberGoodFiles'] = len( self.dataset_details['FileGroups'][group_name]['GoodFiles'] )
			else: 
				self.dataset_details['FileGroups'][group_name]['NumberGoodFiles'] = 0
	
	def buildFileEntriesReport(self):
		"""Use EDM data to calculate no. of entries, also calculate fraction of primary dataset used"""
		# import pdb; pdb.set_trace()
		if self._report is None:
			raise NameError( "ERROR: No file groups found" )
		files = self._report['Files']
		if len(files) == 0: 
			print "ERROR: Integrity check was not completed properly, resubmission is advised."
			return 0
		for group_name in self.dataset_details['FileGroups']:
			entries = 0
			for file_name in files:
				gr = getFileGroup(file_name)
				if gr is not None and gr == group_name:
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
		

if __name__ == '__main__':

    import sys, os, getpass

    dataset = sys.argv[1]
    owner = sys.argv[2]
    user = os.environ['USER']
    pw = getpass.getpass()

    #sampleName,fileOwner,comment, test, primary, username, password
    d = DatasetInformation(dataset, owner, 'This is a test', True, False, user, pw)
    d.buildMissingFileReport()
    d.buildFileEntriesReport()

    #print d.dataset_details
    for group_name in d.dataset_details['FileGroups']:
        print '='*72
        print group_name, d.dataset_details['FileGroups'][group_name]['FileEntries']
