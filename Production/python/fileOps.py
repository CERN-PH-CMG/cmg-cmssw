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

    
    def __init__(self, setName,user, force):

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
        self._force = force
        if re.search('group',user):
            castor = eostools.lfnToEOS(castorBaseDir.castorBaseDir(user.rstrip("_group"), "group"))+self._setName
        else:
            castor = eostools.lfnToEOS(castorBaseDir.castorBaseDir(user))+self._setName

        # Check if local first (obviously)
        if os.path.isdir(setName) and user == os.environ['USER']:
            print "File is on local machine: " + local
            self._LFN = setName.lstrip("/")
            self._localTags()
        # Check if on castor next
        elif eostools.fileExists(castor+"/Logger.tgz"):
            print "File is directory on EOS"
            self._castor =  castor
            self._LFN = eostools.eosToLFN(castor)
            self._castorTags()
            self._checkContiguity()
        # If logger is not present but directory exists
        elif eostools.isDirectory(castor) and self._force:
            print "Directory is valid on EOS, but no logger file is present."
             
            self._castor = castor
            self._LFN = eostools.eosToLFN(castor)
            self._checkContiguity()
            if self.getRootFiles() is None: raise NameError("No valid directory found for dataset did you mean to use the group space or perhaps a different user?: "+setName, setName)
            
        elif eostools.isDirectory(castor):
            raise NameError("Directory is valid on EOS, but no logger file is present.\nNo force requested, publish cancelled")
        # If neither then raise an exception
        else:
            if not re.search("group",user):raise NameError("No valid directory found for dataset did you mean to use the group space or perhaps a different user?: "+setName, setName)
            else: raise NameError('No valid directory found for dataset: '+setName, setName)    
            return None
    
    def getIntegrity(self):
        return self._integrity
        
    def isValid(self):
        return self._valid

    # Return tags
    def getTags(self):
        return self._tags
        
        

  	# Return all root files in directory
    def getRootFiles(self):
        if self._castor is not None:				# If is file on Castor
        	return eostools.matchingFiles(self._LFN, ".*root")
        else:										# If local
            return glob.glob(self.getLFN()+'/'+'/.*.root')  
  		
  	def getCastorRootFiles(self):
  	    if self._castor is not None:				# If is file on Castor
  	        return eostools.matchingFiles(self._castor, ".*root")
  	    # If local
  	    else:
  	        return glob.glob(self.getLFN()+'/'+'/.*.root')
  	    
    # Stage in the Logger.tgz file in a tmp file, load the showtags file and split it on newlines
    def _castorTags(self):
        f = tempfile.NamedTemporaryFile()
        os.system("cmsStage -f "+self.getLFN() + "/Logger.tgz "+f.name)
        tar =tarfile.open(fileobj=f)
        file=tar.extractfile("Logger/logger_showtags.txt")
        lines = file.read().split("\n")
        tar.close()
        f.close()
        self._findTags(lines)
        #print self._tags
        
    def _localTags(self):
        print self.getLFN()
        try:
    	    lines = open(self.getLFN()+"/Logger/logger_showtags.txt")
    	    self._findTags(lines)
    	except:
    	    print "No Logger file present"
        
    # Return castor directory name
    def getCastor(self):
        return self._castor
        
    # Return LFN on castor
    def getLFN(self):
        return self._LFN
    
    def getLFNGroups(self):
        return self._groups
        
    def getCastorGroups(self):
        return self._castorGroups
    
    def getMC(self):
        return self._MC
            
    # Get CMSSW release
    def getRelease(self):
        return self._release
    
    # Get tags
    def _findTags(self, showtags):
        try:
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
	
	# Method to check validity of root files, returns python dict
    def _checkContiguity(self):
        #GET ALL ROOT NAMES
        fileNames = self.getRootFiles()
        fileGroups = []
        integrityCheck = {}
        
        # Take first item in list and get tiers
        tiers = self._setName.lstrip("/").split("/")[2]
        self._MC = False
        if re.search("SIM", tiers): self._MC = True
        
        if len(fileNames) is not 0:
            report = None
            integrity = PublishToFileSystem("IntegrityCheck")
            try:
                report = integrity.get(self.getLFN())
            except Error as ex:
                print ex
            # Build in some kind of wildcard
            if report is None and (self._user == 'cmgtools' or self._user == os.environ['USER']):
                try:
                    checkopts = CheckValues(defaults = {'verbose': 0, 'idx': 0, 'format': 'json', 'user': self._user, 'printout': True, 'host': 'https://cmsweb.cern.ch', 'limit': 10, 'resursive': False, 'wildcard': '*', 'device': 'cmst3', 'query': False, 'name': None})
                    check = IntegrityCheck(self._setName, checkopts)
                    check.test()
                    check.report()
                    report = check.structured()
                    pub = PublishToFileSystem(check)
                    pub.publish(report)
                except Exception as error:
                    print "Integrity check failed."
                    print error.args[0]
            if report is None:
                "Integrity Check Failed - No results will be published"
            else:
                
                if report['Status'] == "VALID":
                    self._valid = True
                if 'BadJobs' in report:
                    integrityCheck['BadJobs'] = report['BadJobs']
                if 'FilesEntries' in report:
                    integrityCheck['FilesEntries'] = report['FilesEntries']
                if 'PrimaryDatasetFraction' in report:
                    integrityCheck['PrimaryDatasetFraction'] = report['PrimaryDatasetFraction']
                if 'PrimaryDatasetEntries' in report:
                    integrityCheck['PrimaryDatasetEntries'] = report['PrimaryDatasetEntries']
                if 'ValidDuplicates' in report:
                    integrityCheck['ValidDuplicates'] = report['ValidDuplicates']
                self._integrity = integrityCheck
        
        # Loop while there are still filenames that do not belong to a file group
        while len(fileNames)>0:
            # Set filename for this pass as the current first element of the filename array
            filename = fileNames[0]

            # Create a new array to hold the names of the group
            fileGroup = []

            # Check for same group
            for listItem in fileNames:
            	if isSameGroup(listItem, filename):
                    fileGroup.append(listItem)
                    
            # Remove the filenames that have been grouped, from the original filenames array,
            # so they do not get processed twice
            for item in fileGroup:
                fileNames.remove(item)

            # Add the new fileGroup to the array of fileGroups
            fileGroups.append(fileGroup)
            
        # Add information to class attributes
        for group in fileGroups:
        	checkedGroup = self._checkGroup(group)
        	if checkedGroup['valid']==False:
        		self._valid=False
        	self._groups.append(checkedGroup)
        self._integrity = integrityCheck
        self._castorGroups = self._groups
        if self._groups is not None:
            for group in self._castorGroups:
                group['name'] = self.getLFN()+ "/" + group['name'].split("/")[-1]
                for file in group['files']:
                    file = self.getLFN() + file.split("/")[-1]
                for file in group['duplicateFiles']:
                    file = self.getLFN() + file.split("/")[-1]
                
                if self._integrity is not None:
                    for file in group['duplicateFiles']:
                        for valid in self._integrity['ValidDuplicates']:
                            if file == valid:
                                
                                del(group['duplicateFiles'][group['duplicateFiles'].index(file)])
                                break
                    group['invalidDuplicates'] = group['duplicateFiles']
                    del(group['duplicateFiles'])
                
                for file in group['missingFiles']:
                    file = self.getLFN() + file.split("/")[-1]
            
        
    
    
    # Check if group is valid and return python dictionary with details
    def _checkGroup(self, group):
        
            

        groupInfo = self._makeGroupInfo(group)
        if groupInfo['qFiles']>1:
            count = groupInfo['bottom']
            if groupInfo['bottom'] > 1: count = 1
            # Check that all numbers are there and index every element
            if checkRootType(group[0]):
                for i in range(count, groupInfo['top']+1):
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
                for i in range(count, groupInfo['top']+1):
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
    num = re.compile("^\d+$")
    suffix = name.rstrip(".root").split("_")
    if num.match(suffix[-2]) and num.match(suffix[-3]) or num.match(suffix[-1]) is None:
        return True
    else: return False

# Turns a grid name into a name that is easy to compare. (helper method)
def reformatGridName(name):
    return name.split("_"+name.split("_")[-3]+ "_"+name.split("_")[-2]+"_"+name.split("_")[-1])[0]

# Returns true if the item is not from the grid
def checkIfUnique(name):
    num = re.compile("^\d+$")
    	
    if num.match(name.rstrip(".root").split("_")[-1]) or num.match(name.rstrip(".root").split("_")[-2]) or num.match(name.rstrip(".root").split("_")[-3]):
        return False
    else: return True

# Match files
def isSameGroup(fileA, fileB):
    if checkRootType(fileA) == checkRootType(fileB):	# Check files are same type e.g. grid job
        if checkRootType(fileA): 	# Test if files from grid
            if reformatGridName(fileA)==reformatGridName(fileB): 	# Check for grid files
            	return True
        else:
            if fileA.rstrip("_[1234567890]*\.root")==fileB.rstrip("_[1234567890]*\.root"): 	# Check for non grid files
            	return True
    return False
    	
##############################
