#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## November 2nd 2011

import sys, os, re, subprocess, glob, tempfile, tarfile
import CMGTools.Production.eostools as eostools
import CMGTools.Production.castorBaseDir as castorBaseDir
from CMGTools.Production.edmIntegrityCheck import PublishToFileSystem
from CMGTools.Production.edmIntegrityCheck import IntegrityCheck
from optparse import Values as CheckValues
	        
class FileOps(object):
    
    """Class gives access to attributes of the dataset that are stored on disk"""
    
    def __init__(self, setName,user, force, checkGroups, run_range = None):
        """Initialises attributes of object, and validates existence of dataset
        'setName' takes the name of the dataset in the EOS format e.g. /QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START44_V9B-v1/AODSIM/V3
        'user' takes the files owner on EOS e.g. cmgtools
        'force' takes True/False on whether the dataset should be considered valid if there is no logger file found
        'checkGroups' takes True/False on whether the corresponding group area should be checked if the dataset is not found. e.g. cmgtools_group
        
        init function uses the checkDirectory method, to establish whether the directory of the dataset is valid.
        """
        
        self._setName = setName.rstrip("/")
        self._user = user
        self._castor=None
        self._LFN=None
        self._tags=[]
        self._release = None
        self._groups = []
        self._integrity = None
        self._valid = False
        self._castorGroups = None
        self._totalJobs = None
        self._totalFilesMissing = None
        self._totalFilesBad = None
        self._totalFilesGood = None
        self._primaryDatasetFraction = None
        self._fileEntries = None
        self._badJobs = None
        self._validDuplicates = None
        self._allMissingFiles = []
        self._allFilesBad = []
        self._force = force
        self._min_run = -1
        self._max_run = -1
        if run_range is not None:
            self._min_run = run_range[0]
            self._max_run = run_range[1]
        
        # If group space is specified, get EOS path using this, otherwise, use user space
        if re.search('group',user):
            castor = eostools.lfnToEOS(castorBaseDir.castorBaseDir(user.rstrip("_group"), "group"))+self._setName
        else:
            castor = eostools.lfnToEOS(castorBaseDir.castorBaseDir(user))+self._setName
        try:
            # Check the directory is valid
            self.checkDirectory(castor)
        # If An error occurs and checkGroups is True, check the group section
        except (NameError, ValueError) as ex:
            print ex.args[0] + "\n"
            if not re.search('group',user) and checkGroups:
                try:
                    print "Testing group directory\n"                    
                    castorgroup = eostools.lfnToEOS(castorBaseDir.castorBaseDir(user.rstrip("_group"), "group"))+self._setName
                    
                    self.checkDirectory(castorgroup)
                    self._user += "_group"
                except:
                    raise
            else:
                raise
                
    def checkDirectory(self, castor):
        """Checks if dataset files and logger file exist in a given directory
        
        'castor' takes the datasets path name in full EOS form e.g. /eos/cms/store/cmst3/group/cmgtools/DoubleMu/Run2011B-16Jan2012-v1/AOD/V2
        
        Sets class variables LFN castor (EOS), and also gets the tags root files and checks the contiguity of the dataset.
        """
        # Check if on castor next
        if eostools.isDirectory(castor) and len(eostools.matchingFiles(eostools.eosToLFN(castor), "Logger.tgz$"))==1:
            self._castor =  castor
            self._LFN = eostools.eosToLFN(castor)
            self._castorTags()
            self._readNJobs()
            self._checkContiguity()
            self.printStats()
            print "File is directory on EOS"
        # If logger is not present but directory exists
        elif eostools.isDirectory(castor) and self._force:
            self._castor = castor
            self._LFN = eostools.eosToLFN(castor)
            self._checkContiguity()
            self.printStats()
            roots = self.getRootFiles() 
            
            if roots is None or len(roots) == 0: 
                raise NameError("Directory: ",castor," is valid on EOS, but no logger file or dataset is present.")
            else:
                print "Directory: ",castor," is valid on EOS, no logger file is present but dataset files exist."
            
        elif eostools.isDirectory(castor):
            raise NameError("Directory is valid on EOS, but no logger file is present.\nNo force requested, publish cancelled")
        # If neither then raise an exception
        else:
            if not re.search("group",self._user):raise NameError("Directory not found check input details?")
            else: raise NameError('No valid directory found for dataset: '+self._setName)    
            return None
    
    def getUser(self):
        """Return the file's owner on EOS e.g. cmgtools"""
        return self._user
    
    def getIntegrity(self):
        """Return the dictionary object returned by Wills edm integrity check"""
        return self._integrity
        
    def isValid(self):
        """Return True/False on whether the dataset is valid"""
        return self._valid

    # Return tags
    def getTags(self):
        """Return the tags in CVS at the time the dataset was created
        
        return as a list of dict() objects in the form:
        [{"tag":"tag","package":"package"}]
        """
        return self._tags
        
        
  	# Return all root files in directory
    def getRootFiles(self):
        """Returns a list of the root files in the datasets directory"""
        if self._castor is not None:				# If is file on Castor
        	return eostools.matchingFiles(self._LFN, ".*root$")
        else:										# If local
            return glob.glob(self.getLFN()+'/'+'/.*.root$')  
  	    
    # Stage in the Logger.tgz file in a tmp file, load the showtags file and split it on newlines
    def _castorTags(self):
        """Sets the self._tags variable to a list of dict() objects containing the tags tags in CVS at the time the dataset was created
        
        Variable is set in the form:
        [{"tag":"tag","package":"package"}]
        """
        # Create temporary file to get logger info
        f = tempfile.NamedTemporaryFile()
        # Stage logger file to temp file and extract
        os.system("cmsStage -f "+self.getLFN() + "/Logger.tgz "+f.name)
        tar =tarfile.open(fileobj=f)
        showtagsFile=tar.extractfile("Logger/logger_showtags.txt")
        lines = showtagsFile.read().split("\n")
        
        tar.close()
        f.close()
        
        
        # Convert the tags in showtags to a Dict() object using findTags method
        self._findTags(lines)

    # Check for the total jobs file and record
    def _readNJobs(self):
        
        """Read the total number of jobs from file in the logger"""
        # Create temporary file to get logger info
        f = tempfile.NamedTemporaryFile()
        # Stage logger file to temp file and extract
        nJobs = None
        try:
            os.system("cmsStage -f "+self.getLFN() + "/Logger.tgz "+f.name)
            tar =tarfile.open(fileobj=f)
            # Open the file in the logger and get the value
            nJobsFile=tar.extractfile("Logger/logger_jobs.txt")
            nJobs = int(nJobsFile.read().split(": ")[1].split("\n")[0])
            tar.close()
        except:
            print "No jobs file found in logger"
        f.close()
        # Set the class variable
        self._totalJobs=nJobs

    # Return castor directory name
    def getCastor(self):
        """Return the EOS path that was stored when the checkDirectory command was executed
        
        Returns a string
        """
        return self._castor
        
    # Return LFN on castor
    def getLFN(self):
        """Return the LFN that was stored when the checkDirectory command was executed
        
        Returns a string
        """
        return self._LFN
    
    def getLFNGroups(self):
        """Returns the groups of root files in a dataset e.g. histograms, PAT, CMG tuples and data about those groups labelled by LFN"""
        return self._groups
        
    def getCastorGroups(self):
        """Returns the groups of root files in a dataset e.g. histograms, PAT, CMG tuples and data about those groups labelled by EOS name"""
        return self._castorGroups
    
    def getMC(self):
        """Returns True/False on whether the sample is a Monte Carlo sample"""
        return self._MC
            
    # Get CMSSW release
    def getRelease(self):
        """Returns a string of the name of the CMSSW release"""
        return self._release
    
    # Get tags
    def _findTags(self, showtags):
        """Append all tags to the _tags class variable and the _release variable
        
        'showtags' takes the output from the showtags file in the Logger folder on EOS as a string
        """
        #Sets tags and release 
        try:
            # Get the release from the first line of showtags
            self._release = showtags[0].split(":")[1].lstrip().rstrip()
            # Creates regexp to test incoming lines from 'showtags'
            tagPattern = re.compile('^\s*(\S+)\s+(\S+)\s*$')
            # For every line in showtags...
            for line in showtags:
                # Check if it is a tag
                m = tagPattern.match(line)
                # if line is a tag...
                if m!=None:
                    # get package name
                    package = m.group(2)
                    # get tag name
                    tag = m.group(1)
                    if tag is not "NoCVS" and tag is not "NoTag":
                        self._tags.append({"package":package,"tag":tag})
        except: print "No tags found"
    
    def getDatasetSize(self):
        """Returns the size of the dataset in TB as an int"""
        output = None
        # Get the command output as a string
        commandOut = eostools.runEOSCommand(self._castor,"find","--size")[0]
        # Separate the string into the required parts
        files = commandOut.split("\npath=")
        # If the root file is a PAT_CMG file
        if re.search("PAT_CMG", self._setName.split("/")[-1]):
            output = 0
            # Add the size of each file to the total size variable
            for file in files:
                if re.search('PAT_CMG',file):
                    if re.search(".root",file):
                        # convert output to TB
                        output += float(float(file.split('size=')[1].split("\n")[0])/1000/1000/1000/1000)
        # If the root file is a PFAOD file
        elif re.search("V", self._setName.split("/")[-1]):
            output = 0
        #Add the size of each file to the total size variable
        for file in files:
            if re.search('PFAOD',file):
                if re.search(".root",file):
                    # convert output to TB
                    output += float(float(file.split('size=')[1].split("\n")[0])/1000/1000/1000/1000)
                        
        return output

    # Method to check validity of root files, returns python dict
    def _checkContiguity(self):
        """Checks the contiguity of the files
        
        Run the integrity check script and format the output into a dict() object, set this as the _integrity class variable
        Run a contiguity check on the .root file names and find any duplicates and missing files,
        these will be recorded in the self._groups dict() 
        """
        ### This method could be much more simple
        
        #GET ALL ROOT NAMES
        fileNames = self.getRootFiles()
        fileGroups = []
        integrityCheck = {}
        
        # Take first item in list and get tiers
        tiers = self._setName.lstrip("/").split("/")[2]
        self._MC = False
        if re.search("SIM", tiers): self._MC = True
        
        # If dataset is present, perform integrity check
        if len(fileNames) is not 0:
            report = None
            # Check if integrity check report is there. If yes, get it.
            integrity = PublishToFileSystem("IntegrityCheck")
            try:
                report = integrity.get(self.getLFN())
            except Exception:
                raise
            # If report is none then do integrity check
            if report is None and (self._user == 'cmgtools' or self._user == os.environ['USER']):
                try:
                    checkopts = CheckValues(defaults = {'verbose': 0, 'idx': 0, 'format': 'json', 'user': self._user, 'printout': True, 'host': 'https://cmsweb.cern.ch', 'limit': 10, 'resursive': False, 'wildcard': '[!histo]*.root', 'device': 'cmst3', 'query': False, 'name': None, 'min_run':self._min_run, 'max_run':self._max_run, 'update':True})
                    check = IntegrityCheck(self._setName, checkopts)
                    check.test()
                    check.report()
                    report = check.structured()
                    pub = PublishToFileSystem(check)
                    pub.publish(report)
                # Zero division error occurs when dataset isn't found on dbs
                # @WILL I think this is due to a bug in your script, 
                # but this catches the error
                except ZeroDivisionError as error:
                    print "Integrity check failed."
                    print "This is usually because the dataset is not found on DAS"
                    
            # If report is still none, do nothing
            if report is None:
                "Integrity Check Failed - No results will be published"
            
            # Otherwise, format results into machine readable format
            else:
                if report['Status'] == "VALID":
                    self._valid = True
                if 'BadJobs' in report:
                    self._badJobs = report['BadJobs']
                if 'FilesBad' in report:
                    self._totalFilesBad = report['FilesBad']
                if 'FilesGood' in report:
                    self._totalFilesGood = report['FilesGood']
                if 'Files' in report:
                    self._allFilesBad = []
                    self._fileEntries = 0
                    for i in report['Files']:
                        if report['Files'][i][0] is False:
                            self._allFilesBad.append(i)
                        else:self._fileEntries += report['Files'][i][1]
                    if len(self._allFilesBad)>0:
                        if checkRootType(self._allFilesBad[0]):
                            self._allFilesBad.sort(key=lambda x: int(x.split("_")[-3]))
                        else:
                            self._allFilesBad.sort(key=lambda x: int(x.rstrip(".root").split("_")[-1]))
                if 'FilesEntries' in report:
                    self._filesEntries = report['FilesEntries']
                if 'PrimaryDatasetFraction' in report:
                    self._primaryDatasetFraction = report['PrimaryDatasetFraction']
                if 'PrimaryDatasetEntries' in report:
                    self._primaryDatasetEntries = report['PrimaryDatasetEntries']
                if 'ValidDuplicates' in report:
                    self._validDuplicates = report['ValidDuplicates']
                self._integrity = report
        

        
        # Loop while there are still filenames that do not belong to a file group
        while len(fileNames)>0:
            # Set filename for this pass as the current first element of the filename array
            filePart = fileNames[0].split("/")[-1].split("_")[0]
            # Create a new array to hold the names of the group
            fileGroup = []

            # Check for same group
            for listItem in fileNames:
            	if listItem.split("/")[-1].split("_")[0] == filePart:
                    fileGroup.append(listItem)
                    
            # Remove the filenames that have been grouped, from the original filenames array,
            # so they do not get processed twice
            for item in fileGroup:
                fileNames.remove(item)
            # Add the new fileGroup to the array of fileGroups
            fileGroups.append(fileGroup)
        
        # Check validity
        for group in fileGroups:
        	checkedGroup = self._checkGroup(group)
        	# Set valid variable
        	if checkedGroup['valid']==False:
        		self._valid=False
        	# Append file group to list
        	self._groups.append(checkedGroup)
        
        # 
        self._integrity = integrityCheck
        

        self._castorGroups = self._groups
        self._totalFilesMissing = 0
        if self._groups is not None:
            for group in self._castorGroups:
                group['name'] = self.getLFN()+ "/" + group['name'].split("/")[-1]
                for file in group['files']:
                    file = self.getLFN() + file.split("/")[-1]
                for file in group['duplicateFiles']:
                    file = self.getLFN() + file.split("/")[-1]
                
                if self._validDuplicates is not None:
                    for file in group['duplicateFiles']:
                        for valid in self._validDuplicates:
                            if file == valid:
                                
                                del(group['duplicateFiles'][group['duplicateFiles'].index(file)])
                                break
                    group['invalidDuplicates'] = group['duplicateFiles']
                    del(group['duplicateFiles'])
                
                for file in group['missingFiles']:
                    file = self.getLFN() + file.split("/")[-1]
                    self._allMissingFiles.append(file)
                    self._totalFilesMissing += 1
            
    
    # Give printout of dataset good/bad/missing/total files/jobs
    def printStats(self):
        """Prints the basic stats of the dataset"""
        print "\nDataset Overview:"
        if self._totalJobs is not None: print "Total jobs: %d" % self._totalJobs
        if self._totalFilesGood is not None: print "Total files good: %d" % self._totalFilesGood
        if self._totalFilesMissing is not None: print "Total files missing: %d" % self._totalFilesMissing
        if self._totalFilesBad is not None: print "Total files bad: %d" % self._totalFilesBad
        if self._primaryDatasetFraction is not None: print "Fraction of primary dataset used: %f" % self._primaryDatasetFraction
        if self._primaryDatasetEntries is not None: print "Total entries in primary dataset: %d" % self._primaryDatasetEntries
        if self._fileEntries is not None: print "Total file entries in dataset: %d" % self._fileEntries
        if self.getDatasetSize() is not None: print "Dataset Size = %f TB" % self.getDatasetSize()
        print ""
    
    # Check if group is valid and return python dictionary with details
    def _checkGroup(self, group):
        """Runs a contiguity check on a group of filenames found in the dataset's directory, all files must have similar names
        
        'group' takes a list object containing the file names in the 'group' of files
        """
        groupInfo = self._makeGroupInfo(group)
        top = groupInfo['top']+1
        if self._totalJobs is not None: top = self._totalJobs
        
        ### SIMPLIFY WITH REGEXP
        if groupInfo['qFiles']>1:
            count = groupInfo['bottom']
            top = groupInfo['top']+1
            if groupInfo['bottom'] > 1: count = 1
            # Check that all numbers are there and index every element
            if checkRootType(group[0]):
                for i in range(count, top):
                    if i not in groupInfo['fileNums']:
                        parts = groupInfo['name'].split("_")
                        parts[-3] = str(i)
                        parts[-2] = "0"
                        parts[-1] = "XXX.root"
                        missingFile = "_".join(parts)
                        groupInfo['missingFiles'].append(missingFile)
                        groupInfo['valid']=self._MC
                for i in groupInfo['fileNums']:
                    groupInfo['fileNums'].remove(i)
                    if i in groupInfo['fileNums']:
                        groupInfo['valid']=False
                        for duplicateFile in groupInfo['files']:
                            if str(i) == duplicateFile.split("_")[-3]:
                                groupInfo['duplicateFiles'].append(duplicateFile)
            else:
                for i in range(count, top):
                    if i not in groupInfo['fileNums']:
                        parts = groupInfo['name'].rstrip(".root").split("_")
                        parts[-1] = str(i)
                        missingFile = "_".join(parts) + ".root"
                        groupInfo['missingFiles'].append(missingFile)
                        groupInfo['valid']=self._MC
                for i in groupInfo['fileNums']:
                    groupInfo['fileNums'].remove(i)
                    if i in groupInfo['fileNums']:
                        groupInfo['valid']=False
                        for duplicateFile in groupInfo['files']:
                            if str(i) == duplicateFile.rstrip(".root").split("_")[-1]:
                                groupInfo['duplicateFiles'].append(duplicateFile)
        return groupInfo
        
        
    # Create python dictionary with groups properties
    def _makeGroupInfo(self, group):
        """Creates a dictionary holding information about the group of files
        
        'group' takes a list of file names
        returns dict() object with the following elements:
            'name' = a generic name that applies to all files in the group
            'valid' = True/False if the group is valid
            'files' = a list of the files in the group
            'missing files' = a list of the files missing from the group
            'duplicate files' = a list of the duplicated files in the group
            'qFiles' = how many files are in the group
            'numbers' = a list of the file numbers, e.g. numbers[-1] is the highest number in the group
            'top' = numbers[-1]
            'bottom' = numbers[0]
        """
    	# Check if manually named (not numbered)
    	if checkIfUnique(group[0]):
    		return {"name":group[0],"valid":True, "qFiles":1}
                
        else:
            numbers = []
            # If from grid:
            if checkRootType(group[0]):
                for element in group:
                    num = element.split("_")[-3]
                    numbers.append(int(num))
                    # Create name for grid type in format: name_[a-n]_identifier_XXX.root
                arr = group[0].split("_")
                arr[-1] = "XXX.root"
                group.sort(key=lambda x: int(x.split("_")[-3]))
                
                numbers.sort()
                arr[-3] = "["+str(numbers[0])+"-" + str(numbers[-1])+"]"
                groupName = "_".join(arr)
            # If not:
            else:
                for element in group:
                    num = element.rstrip(".root").split("_")[-1]
                    numbers.append(int(num))
                    # Create name for normal type in format name_[a-n].root
                
                group.sort(key=lambda x: int(x.rstrip(".root").split("_")[-1]))
                numbers.sort()
                groupName = group[0].rstrip(group[0].split("_")[-1]) +"["+str(numbers[0])+"-"+ str(numbers[-1])+"].root"
                    
        newGroup= {"name":groupName, "valid":True, "files":group, "missingFiles":[], "duplicateFiles":[], "qFiles":len(group), "fileNums":numbers, "top":numbers[-1], "bottom":numbers[0]}
        
        return newGroup
        
    
        
###### 4 HELPER METHODS #####      
def checkRootType(name):
    """Checks if the group matches the CRAB file type or not
    
    'name' takes a file name as a string
    """
    crab = re.compile(".*_\d+_\d+_\w+$")
    if crab.match(name.rstrip(".root")):
        return True
    else: return False

# Turns a grid name into a name that is easy to compare. (helper method)
def reformatGridName(name):
    """Normalises a file name so that it is easier to compare to others
    
    'name' takes a file name
    strips the numbers and letters and '.root' suffix from the end of the filename
    """
    a = re.sub("_\d+_\w+$", "",name.rstrip(".root"))
    return a

# Returns true if the item is not from the grid
def checkIfUnique(name):
    """Checks if the filename is unique by checking if it fails to match both crab and batch formats
    
    'name' takes the file name
    """
    num = re.compile("^\d+$")
    	
    if num.match(name.rstrip(".root").split("_")[-1]) or num.match(name.rstrip(".root").split("_")[-2]) or num.match(name.rstrip(".root").split("_")[-3]):
        return False
    else: return True


    	
##############################
