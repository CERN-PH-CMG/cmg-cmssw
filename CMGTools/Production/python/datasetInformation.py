
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
    base, _ = os.path.splitext(fname)    
    #two possible patterns - cmsbatch and crab respectively
    pattern = "(^.*?)(_\d+_\d+_\w+$)|(^.*?)(_\d+$)"  
    regex = re.compile(pattern)
    result = regex.match( base ).groups()         
    
    if ( result[0] == None ):                      
        if ( result[2] == None ):             
            raise NameError( "Invalid filename - \
            Crab or cmsbatch pattern not detected" )
         #if it's a cmsbatch file
        else:  
            return False           
    #if it'scrab file
    else:   
        return True          

def removeIndex(name):
    _, fname = os.path.split(name)
    base, _ = os.path.splitext(fname)      
    #two possible patterns - cmsbatch and crab respectively
    pattern = "(^.*?)(_\d+_\d+_\w+$)|(^.*?)(_\d+$)"  
    regex = re.compile(pattern)
    result = regex.match( base ).groups()

    if ( result[0] == None ):                      
        if ( result[2] == None ):             
            raise NameError( "Invalid filename - \
            Crab or cmsbatch pattern not detected" )
        #if it's a cmsbatch file
        else:   
            return result[2]           
    #if it's a crab file
    else:   
        return result[0]          

def getIndex(name):
    _, fname = os.path.split(name)
    base, _ = os.path.splitext(fname)   
    #two possible patterns - cmsbatch and crab respectively
    pattern = "(^.*_)(\d+)(_\d+_\w+$)|(^.*_)(\d+$)" 
    regex = re.compile(pattern)
    result = regex.match( base ).groups()           

    if ( result[1] == None ):
        if ( result[4] == None ):
            raise NameError( "Invalid filename - \
            Crab of cmsbatch pattern not detected" )
        else:
            return int( result[4] )
    else:
        return int( result[1] )

def createFileName(groupName, number, isCrab):
    if isCrab:
        return groupName + "_" + str(number)+"_*_***"
    else:
        return groupName + "_" + str(number)

def getFileName(name):
    return name.split("/")[-1]

class DatasetInformation(object):
    """Class gives access to attributes of the\
    dataset that are stored on disk"""

    def __init__(self, sampleName, fileOwner, comment, force, test, 
                 primary, username, password, development=False):
        """Initialises attributes of object, and validates existence of dataset
        'sampleName' takes the name of the dataset \
        e.g. /QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/
        Fall11-PU_S6_START44_V9B-v1/AODSIM/V3
        'fileOwner' takes the files owner on EOS e.g. cmgtools
        'force' takes True/False on whether the dataset should
        be considered valid if there is no logger file found
        'test' takes True/False on whether the sample is a test or not
        'username' takes the username of the person submitting 
        the sample
        'password' takes that users password
        'development' takes True/False depending on whether 
        wants to publish on the official or the devdb11 database"""

        self.dataset = None                                 
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
        """Builds a dictionary containing the different
        file groups within a directory"""
        self.dataset.buildListOfFiles()
        self.dataset_details['FileGroups'] = dict()    
        for fileName in self.dataset.files:           

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
                isCrab = isCrabFile( fileName )                          
                instanceGroup['IsCrab']=isCrab                          
                self.dataset_details['FileGroups'][name]=instanceGroup  
        for fg in self.dataset_details['FileGroups']:                   
            self.dataset_details['FileGroups'][fg]['Files'].sort(key=lambda x: int(getIndex(x)))    

    def createFileGroupDetailString(self, group_name):
        """Build a string containing the important 
        information pertaining to a file group
        'group_name' takes the groups name
        as a string e.g. 'cmgtuple'"""

        if self.dataset_details is None:
            return None
        
        if 'FileGroups' not in self.dataset_details:
            return None

        if group_name not in self.dataset_details['FileGroups']:
            return None
            
        r = "\t----"+group_name+"----\n"
        sizeInTB=self.dataset_details['FileGroups'][group_name]['SizeInTB']
        if sizeInTB is not None:
            r+="\tDataset size: "+ str(sizeInTB)+" TB\n"
        
        fileEntries=self.dataset_details['FileGroups'][group_name]['FileEntries']
        if fileEntries is not None:
            r+="\tDataset file entries: "+str(fileEntries)+"\n"

        pdf=self.dataset_details['FileGroups'][group_name]['PrimaryDatasetFraction']
        if pdf is not None:
            r+="\tFraction of primary dataset used: "+str(pdf)+"\n"

        nbf=self.dataset_details['FileGroups'][group_name]['NumberBadFiles']
        if nbf is not None:
            r+="\tNumber of bad files: "+str(nbf)+"\n"
            bf=self.dataset_details['FileGroups'][group_name]['BadFiles']
            if bf:
                for i in bf:
                    r+="\t-- "+i+"\n"
        
        nmf=self.dataset_details['FileGroups'][group_name]['NumberMissingFiles']
        if nmf is not None:
            r+="\tNumber of missing files: "+str(nmf)+"\n"
            mf=self.dataset_details['FileGroups'][group_name]['MissingFiles']
            if mf:
                for i in mf:
                    r+="\t-- "+i+"\n"

        return r

    def createDirectoryDetailString(self):
        """Build a string containing the important
        information pertaining to the directory"""
        if self.dataset_details is None:
            return None

        r="Dataset Name: "+self.dataset_details['SampleName']+"\n"
        r+="CMGDB Name: "+self.dataset_details['CMGDBName']+"\n"
        r+="File Owner: "+self.dataset_details['FileOwner']+"\n"

        dsTB = self.dataset_details['DirectorySizeInTB']
        if dsTB is not None:
            r+="Directory size: "+str(dsTB)+" TB\n"
        
        fe=self.dataset_details['FileEntries']
        if fe is not None:
            r+="Directory file entries: "+str(fe)+"\n"
        
        pde=self.dataset_details['PrimaryDatasetEntries']
        if pde is not None:
            r+="Primary dataset file entries: "+str(pde)+"\n"
        
        return r

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
        """Checks the EOS directory for a Logger.tgz file,
        if not found, escapes
        'sampleName' takes the name of the sample as a string
        'fileOwner' takes the file owner on EOS as a string
        """
        lfn = self.dataset_details['LFN']
        if len(eostools.matchingFiles(lfn,"Logger.tgz"))==1 or self._force:
            self.createLoggerTemporaryFile()
            return True
        else:
            raise NameError("ERROR: No Logger.tgz file found for\
            this sample. If you would like to preceed anyway, \
            please copy Logger.tgz from your local \
            production directory to your production \
            directory on eos.\n")

    def createLoggerTemporaryFile(self):
        """Build a temporary logger file object and tarfile 
        object to be used when retrieving tags and jobs"""
        
        try:
            self._logger_file = tempfile.NamedTemporaryFile()
            lfn = self.dataset_details['LFN']
            os.system("cmsStage -f "+lfn+"/Logger.tgz "+self._logger_file.name)
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
        """Checks the dataset object for presence of 
        root files, if not found, escapes"""
        
        self.dataset.buildListOfFiles()
        if len(self.dataset.files) > 0:
            self.dataset_details['RootFiles'] = self.dataset.files
            return True
        else:
            raise NameError("ERROR: Dataset root files\
            not found on EOS, dataset is invalid\n")

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
        cmgdbname = self.dataset_details['CMGDBName']
        pcmgdbname = self.dataset_details['ParentCMGDBName']
        did=cmgdbAPI.getDatasetIDWithName(cmgdbname)
        pid=cmgdbAPI.getDatasetIDWithName(pcmgdbname)
        self.dataset_details['CMGDBID']=did
        self.dataset_details['ParentCMGDBID']=pid

    def buildBasicDetailsReport(self):
        """Gets the basic dataset information"""
        self.dataset_details['LFN'] = self.dataset.lfnDir
        self.dataset_details['EOSPath'] = self.dataset.castorDir
        name = self.dataset.name
        user = self.dataset.user
        self.dataset_details['CMGDBName']=getCMGDBWithUser(name, 
                                                           user)

        if not self._primary:
            samplename=self.dataset_details['SampleName']
            cmgdbname=self.dataset_details['CMGDBName']
            parent=getParentWithSampleName(samplename)
            pdn=self.dataset.name.lstrip("/").split("/")[0]
            pcmgdbname=getUnknownParentWithCMGDB(cmgdbname)
            tiers=samplename.lstrip("/").split("/")[2]
            self.dataset_details['ParentSampleName']=parent
            self.dataset_details['PrimaryDatasetName']=pdn
            self.dataset_details['ParentCMGDBName']=pcmgdbname
            self.dataset_details['Tiers']=tiers
            # Can make addition here for physics group

        self.dataset_details['PhysicsGroup']="CMG"
        dateCreated=datetime.now().strftime("%s")
        self.dataset_details['DateCreated']=dateCreated
        self.dataset_details['Status']="VALID"

    def buildSavannahReport(self):
        """Access Savannah and get all 
        information relating to the dataset"""
        
        dtCreated = int(self.dataset_details['DateCreated'])
        dt = date.fromtimestamp(dtCreated)
        dayMonthYear = dt.strftime('%d-%B-%Y').split("-")

        d=dayMonthYear[0].lstrip("0")
        m=dayMonthYear[1]
        y=dayMonthYear[2]

        self.dataset_details['SavannahOptions']['planned_starting_date_dayfd']=d
        self.dataset_details['SavannahOptions']['planned_starting_date_monthfd']=m
        self.dataset_details['SavannahOptions']['planned_starting_date_yearfd']=y

        # Create test category MAY BE REMOVED IF TEST NOT REQUIRED
        if self.dataset_details['Test']:
            category_id = '101'
        else:
            category_id = '103'

        # More savannah opts
        fowner=self.dataset_details['FileOwner']
        cmgdbname=self.dataset_details['CMGDBName']
        self.dataset_details['SavannahOptions']['assigned_to']=fowner
        self.dataset_details['SavannahOptions']['summary']=cmgdbname
        self.dataset_details['TaskID']=getTaskID(cmgdbname, 
                                                 category_id, 
                                                 self._username, 
                                                 self._password, 
                                                 False)

        # If task already exists you NO NOT WANT TO CHANGE THE CATEGORY 
        # so only add this field if it is a new DS
        if self.dataset_details['TaskID'] is None: 
            self.dataset_details['SavannahOptions']['category_id']=category_id

        # If dataset is primary
        if self._primary:
            print "NOTE: Dataset is a primary dataset"
            return None

        # Check if parent exists
        pcmgdbname = self.dataset_details['ParentCMGDBName']
        self.dataset_details['ParentTaskID']=getTaskID(pcmgdbname, 
                                                       category_id, 
                                                       self._username, 
                                                       self._password, 
                                                       True)
        ptid = self.dataset_details['ParentTaskID']     
        if ptid is not None and len(ptid) > 0:
            if len(ptid)>1:
                cmgdbname = self.dataset_details['CMGDBName']
                raise NameError("Multiple possible parents found for dataset: "\
                                +cmgdbname+". Please find the duplicate and remove it" )

            self.dataset_details['ParentSavannahString']="[https://savannah.cern.ch/task/?"+str(ptid[0])+" "+getNameWithID(ptid[0])+"]"
            self.dataset_details['ParentTaskID'] = ptid[0]
            self.dataset_details['ParentCMGDBName'] = getNameWithID(ptid[0])

        # If Parent is a CMS dataset (i.e. not a CMG dataset)
        elif not re.search("--", pcmgdbname):

            self.dataset_details['ParentSavannahString']="[https://cmsweb.cern.ch/das/request?view=list&limit=10&instance=cms_dbs_prod_global&input=dataset%3D%2F"\
                +pcmgdbname.lstrip("/").split("/")[0]+"%2F"+pcmgdbname.lstrip("/").split("/")[1]\
                +"%2F"+pcmgdbname.lstrip("/").split("/")[2]+" "+pcmgdbname+"]"
            self.dataset_details['ParentTaskID'] = None

        else:
            raise NameError("No Parent was found for Dataset: "+\
                            cmgdbname+\
                            " not publishing.",
                            self.dataset_details['TaskID'])

    def buildShowtagsReport(self):
        """Stage the logger_showtags.txt file in Logger.tgz 
        and get a list of tags and the CMSSW version"""
        
        if self._logger_tar_object is None:
            raise NameError("ERROR: self._logger_tar_object not set")
        showtagsFile = None
        lines = None
        try:
            logger=self._logger_tar_object
            showtagsFile=logger.extractfile("Logger/logger_showtags.txt")
            lines = showtagsFile.read().split("\n")
        except:
            print "ERROR: No showtags file found in logger"
            return None
        #Sets tags and release
        #Get the release from the first line of showtags
        self.dataset_details['Release']=lines[0].split(":")[1].lstrip().rstrip()
        tagPattern = re.compile('^\s*(\S+)\s+(\S+)\s*$')
        tags = []

        for line in lines:
            m = tagPattern.match(line)
            if m != None:
                package = m.group(2)
                tag = m.group(1)
                if tag is not "NoCVS" and tag is not "NoTag":
                    tags.append({"package":package,"tag":tag})
        self.dataset_details['Tags'] = tags

    def buildJobsReport(self):                                                                            
        """Stage the logger_jobs.txt file in Logger.tgz 
        and read the total number of jobs"""
        
        if self._logger_tar_object is None:                                                            
            raise NameError("ERROR: self._logger_tar_object not set")
        nJobs = None
        try:
            # Open the file in the logger and get the value
            # import pdb; pdb.set_trace()
            logger=self._logger_tar_object
            nJobsFile=logger.extractfile("Logger/logger_jobs.txt")             
            nJobs = int(nJobsFile.read().split(": ")[1].split("\n")[0])                            
        except:
            raise NameError("ERROR: No jobs file found in logger")                               

        # Set the class variable
        if nJobs == None:
            self.dataset_details['TotalJobs'] = -1
        else:
            self.dataset_details['TotalJobs'] = nJobs

        for file_group in self.dataset_details['FileGroups']:  
            totalJobs = self.dataset_details['TotalJobs']
            self.dataset_details['FileGroups'][file_group]['TotalJobs']=totalJobs


    def buildDatasetSizeReport(self):
        """Get the dataset size for both the directory, 
        and the individual file groups"""
        
        if self.dataset_details['FileGroups'] is None:
            raise NameError( "ERROR: No file groups found" )
        self.dataset.extractFileSizes()
        self.dataset_details['DirectorySizeInTB'] = 0.0
        for group_name in self.dataset_details['FileGroups']:
            size_in_bytes = 0
            for file_name in self.dataset_details['FileGroups'][group_name]['Files']:
                size_in_bytes += int(self.dataset.filesAndSizes[file_name])

            size_in_tb = float(size_in_bytes)/1000000000000
            self.dataset_details['FileGroups'][group_name]["SizeInTB"]=size_in_tb
            self.dataset_details['DirectorySizeInTB']+=size_in_tb

    def buildMissingFileReport(self):
        """Create a list of missing files and number of 
        total missing files for each file group"""
        
        if self.dataset_details['FileGroups'] is None:
            raise NameError( "ERROR: No file groups found" )
        totalMissing = 0                                                

        for fg in self.dataset_details['FileGroups']:          
            numbers = []                                                     
            missing_files = []                                              

            if self.dataset_details['FileGroups'][fg]['TotalJobs'] != -1:
                file_list=self.dataset_details['FileGroups'][fg]['Files']
                isCrab=self.dataset_details['FileGroups'][fg]['IsCrab']

                os.chdir(os.environ['CMSSW_BASE'] + '/src/CMGTools/Production')
                p = subprocess.Popen(['cvs','status','-v','scripts/cmsBatch.py'], 
                                     stdout=subprocess.PIPE, 
                                     stderr=subprocess.STDOUT)

                #pattern matches       cbern_workflow_16Nov11           (revision: 1.6)
                #and                   abis_cmgtools                    (branch: 1.6.2)
                pattern = "\t(.*?)\s*?\t\((revision:|branch:) (.*?)\)"
                regex = re.compile(pattern)
                try:
                    #create a list with lines of format abis_cmgtools   (branch: 1.6.2) after splitting the output
                    revision_info=[item for item in p.communicate()[0].split("Existing Tags:")[1].split('\n') if item != '' ]
                except:
                    raise IOError("ERROR: Unexpected output\
                    from 'cvs status scripts/cmsBatch'")

                tag_to_revision = dict()
                for item in revision_info:
                    try:
                        result = regex.match(item).groups()
                    except AttributeError:
                        raise IOError( "ERROR: Unexpected output from \
                        'cvs status scripts/cmsBatch' execution" )
                    tag_to_revision[result[0]] = result[2]  

                #get the revision corresponding to the dataset's tag
                for item in self.dataset_details['Tags']:
                    if item['package'] == "CMGTools/Production":
                        if item['tag'] == 'HEAD':
                            revision = '1.17'
                        else:
                            revision = tag_to_revision[item['tag']]
                        break
                if revision is None:
                    raise IOError( "ERROR: Unexpected output from\
                    'cvs status scripts/cmsBatch' execution - \
                    couldn't match tag with an existing revision" )
                else:
                    revision = revision.split('.')

                if (int(revision[0]) <= 1 and int( revision[1] ) <= 16):

                    if (isCrab):
                        start_index=1  
                    else:
                        start_index=0  

                    for file_name in file_list:  
                        numbers.append( getIndex(file_name) ) 
                        if start_index == 0:
                            #e.g 22 jobs: range(0,22) = 0...21
                            end_index=self.dataset_details['FileGroups'][fg]['TotalJobs'] 
                        else:
                            #e.g 22 jobs: range(1,23) = 1...22
                            end_index=self.dataset_details['FileGroups'][fg]['TotalJobs']+1 

                    for number in range( start_index, end_index ):
                        if number not in numbers:                            
                            missing_files.append(createFileName(fg, number, isCrab))
                #Changed since 12-12-2012: both cmsBatch and crabfiles have indexing from 1 to n
                else:  

                    for file_name in file_list:
                        numbers.append( getIndex( file_name ) )  
                    for number in range(1, self.dataset_details['FileGroups'][fg]['TotalJobs']+1):
                        if number not in numbers: 
                            missing_files.append(createFileName(fg,number,isCrab))

                if len( missing_files ) > 0:                                    
                    self.dataset_details['FileGroups'][fg]['MissingFiles']=missing_files             
                    self.dataset_details['FileGroups'][fg]['NumberMissingFiles']=len(missing_files)   
                else:
                    self.dataset_details['FileGroups'][fg]['NumberMissingFiles']=0                       
                totalMissing += len(missing_files)                               

            else:
                self.dataset_details['FileGroups'][fg]['MissingFiles']=[]
                totalMissing = -1

            self.dataset_details["TotalFilesMissing"] = totalMissing


    def buildBadFileReport(self):
        """Create a list of bad files and no. total bad files for each file group"""
        if self.dataset_details['FileGroups'] is None:
            raise NameError( "ERROR: No file groups found" )

        for fg in self.dataset_details['FileGroups']:
            bf = []

            for file_name in self.dataset.bad_files:
                name = removeIndex( file_name )
                if name==fg and self.dataset.bad_files[file_name]=='MarkedBad':
                    bf.append(getFileName(file_name))

            if len(bf) > 0:
                bfn = len(bf)
                self.dataset_details['FileGroups'][fg]['BadFiles']=bf
                self.dataset_details['FileGroups'][fg]['NumberBadFiles']=bfn
            else:
                self.dataset_details['FileGroups'][fg]['NumberBadFiles']=0

    def buildGoodFileReport(self):
        """Creates a list of good files and number of
        total good files for each file group"""
        
        if self.dataset_details['FileGroups'] is None:
            raise NameError( "ERROR: No file groups found" )

        for fg in self.dataset_details['FileGroups']:
            gf = []
            for file_name in self.dataset.good_files:
                if removeIndex( file_name ) == fg:
                    gf.append( getFileName(file_name))

            if len(gf)>0:
                gfn = len(gf)
                self.dataset_details['FileGroups'][fg]['GoodFiles']=gf
                self.dataset_details['FileGroups'][fg]['NumberGoodFiles']=gfn
            else:
                self.dataset_details['FileGroups'][fg]['NumberGoodFiles']=0

    def buildFileEntriesReport(self):
        """Use EDM data to calculate no. of entries,
        also calculate fraction of primary dataset used"""
        
        if self._report is None:
            raise NameError( "ERROR: No file groups found" )
        files = self._report['Files']

        if len(files) == 0:
            print "ERROR: Integrity check was not completed properly,\
            resubmission is advised."
            return 0

        for fg in self.dataset_details['FileGroups']:
            entries = 0
            for file_name in files:
                gr = getFileGroup(file_name)
                if gr is not None and gr == fg:
                    entries += files[file_name][1]
            self.dataset_details['FileGroups'][fg]['FileEntries']=entries
            pde = self.dataset_details['PrimaryDatasetEntries']
            if pde is not None and entries != 0 and pde > 0:
                fraction=float(entries)/float(pde)
                self.dataset_details['FileGroups'][fg]['PrimaryDatasetFraction']=fraction

    def buildEDMReport(self):
        """Retrieve the revelevant information 
        from the EDM report and add it to the dictionary"""
        
        if self._report is None:
            return None
        # Check if integrity check report is there. If yes, get it.
        if 'BadJobs' in self._report:
            self.dataset_details['BadJobs']=self._report['BadJobs']
        if 'FilesBad' in self._report:
            self.dataset_details['TotalFilesBad']=self._report['FilesBad']
        if 'FilesGood' in self._report:
            self.dataset_details['TotalFilesGood']=self._report['FilesGood']
        if 'FilesEntries' in self._report:
            self.dataset_details['FileEntries']=self._report['FilesEntries']
        if 'PrimaryDatasetFraction' in self._report:
            self.dataset_details['PrimaryDatasetFraction']=self._report['PrimaryDatasetFraction']
        if 'PrimaryDatasetEntries' in self._report:
            self.dataset_details['PrimaryDatasetEntries']=self._report['PrimaryDatasetEntries']
        if 'ValidDuplicates' in self._report:
            self.dataset_details['ValidDuplicates']=self._report['ValidDuplicates']


if __name__ == '__main__':

    import sys, os, getpass

    dataset = sys.argv[1]
    owner = sys.argv[2]
    user = os.environ['USER']
    pw = getpass.getpass()

    #sampleName,fileOwner,comment, test, primary, username, password
    d = DatasetInformation(dataset, owner, 'This is a test', 
                           True, False, user, pw)
    d.buildMissingFileReport()
    d.buildFileEntriesReport()

    #print d.dataset_details
    for fg in d.dataset_details['FileGroups']:
        print '='*72
        print fg, d.dataset_details['FileGroups'][fg]['FileEntries']
