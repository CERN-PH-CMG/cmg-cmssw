#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

# This module will write the data from a passed software object
# to the database file on disk
import os 
os.system("source /afs/cern.ch/cms/slc5_amd64_gcc434/external/oracle/11.2.0.1.0p2/etc/profile.d/init.sh")
os.system("source /afs/cern.ch/cms/slc5_amd64_gcc434/external/python/2.6.4-cms16/etc/profile.d/init.sh")
os.system("source /afs/cern.ch/cms/slc5_amd64_gcc434/external/py2-cx-oracle/5.1/etc/profile.d/init.sh") 

import sys, re
#import CMGTools.Production.cx_Oracle as cx_Oracle
import cx_Oracle
class CmgdbApi(object):
    
    def __init__(self):
        # Creates the connection object that will be used by the class to interface with the database
        try:
            self.insertConn = cx_Oracle.connect("cms_cmgdb_w/Bamboo66@(DESCRIPTION = (ADDRESS = (PROTOCOL = TCP)(HOST = int9r1-v.cern.ch)(PORT = 10121)) (ADDRESS = (PROTOCOL = TCP)(HOST = int9r2-v.cern.ch)(PORT = 10121)) (LOAD_BALANCE = yes) (CONNECT_DATA = (SERVER = DEDICATED) (SERVICE_NAME = int9r_lb.cern.ch) (FAILOVER_MODE = (TYPE = SELECT)(METHOD = BASIC)(RETRIES = 200)(DELAY = 15))))")
            self.selectConn = cx_Oracle.connect("cms_cmgdb_r/Chocolate100@(DESCRIPTION = (ADDRESS = (PROTOCOL = TCP)(HOST = int9r1-v.cern.ch)(PORT = 10121)) (ADDRESS = (PROTOCOL = TCP)(HOST = int9r2-v.cern.ch)(PORT = 10121)) (LOAD_BALANCE = yes) (CONNECT_DATA = (SERVER = DEDICATED) (SERVICE_NAME = int9r_lb.cern.ch) (FAILOVER_MODE = (TYPE = SELECT)(METHOD = BASIC)(RETRIES = 200)(DELAY = 15))))")
            self.selectCur = self.selectConn.cursor()
            self.insertCur = self.insertConn.cursor()
        except Exception as dbError:
            print "Unable to connect to CMGDB"
            print dbError.args[0]
            return None

    # Clear missing file entries
    def clearDatasetMissingFiles(self, dsName, datasetID):
        try:
            self.insertCur.execute('DELETE FROM cms_cmgdb.missing_files WHERE dataset_id = %d' % int(datasetID))
            self.insertConn.commit()
        except cx_Oracle.IntegrityError:
            print "Unable to delete missing file record"
            
    # Log missing files
    def addMissingFile(self, dsName, datasetID, missingFileName):
        try:
            self.insertCur.execute("INSERT INTO cms_cmgdb.missing_files(dataset_id, missing_file) values(%d, '%s')" % (datasetID, missingFileName))
            self.insertConn.commit()
        except cx_Oracle.IntegrityError:
            # If exception is thrown display error message and ignore
            print 'File: '+missingFileName+ " in dataset " + dsName + " is already logged as missing on the system"
            
    # Clear bad file entries
    def clearDatasetBadFiles(self, dsName, datasetID):
        try:
            self.insertCur.execute('DELETE FROM cms_cmgdb.bad_files WHERE dataset_id = %d' % int(datasetID))
            self.insertConn.commit()
        except cx_Oracle.IntegrityError:
            print "Unable to delete file entries"
            
    # Log bad files
    def addBadFile(self, dsName, datasetID, badFileName):
        try:
            self.insertCur.execute("INSERT INTO cms_cmgdb.bad_files(dataset_id, bad_file) values(%d, '%s')" % (datasetID, str(badFileName)))
            self.insertConn.commit()
        except cx_Oracle.IntegrityError:
            # If exception is thrown display error message and ignore
            print 'File: '+badFileName+ " in dataset " + dsName + " is already logged as bad on the system"
    # Clear bad jobs entries
    def clearDatasetBadJobs(self, dsName, datasetID):
        try:
            self.insertCur.execute('DELETE FROM cms_cmgdb.bad_jobs WHERE dataset_id = %d' % int(datasetID))
            self.insertConn.commit()
        except cx_Oracle.IntegrityError:
            print "Unable to delete bad job record"
            
    # Log missing files
    def addBadJob(self, dsName, datasetID, badJobNum):
        try:
            self.insertCur.execute('INSERT INTO cms_cmgdb.bad_jobs(dataset_id, bad_job) values(%d, %d)' % (datasetID, int(badJobNum)))
            self.insertConn.commit()
        except cx_Oracle.IntegrityError:
            # If exception is thrown display error message and ignore
            print 'Job: '+badJobName+ " in dataset " + dsName + " is already logged as bad on the system"
    # Clear missing file entries
    def clearDatasetDuplicateFiles(self, dsName, datasetID):
        try:
            self.insertCur.execute('DELETE FROM cms_cmgdb.duplicate_files WHERE dataset_id = %d' % int(datasetID))
            self.insertConn.commit()

        except cx_Oracle.IntegrityError:
            print "Unable to delete duplicate file record"
            
    # Log missing files
    def addDuplicateFile(self, dsName, datasetID, duplicateFileName):
        try:
            self.insertCur.execute('INSERT INTO cms_cmgdb.duplicate_files(dataset_id, duplicate_file) values(%d, %s)' % (datasetID, duplicateFileName))
            self.insertConn.commit()
        except cx_Oracle.IntegrityError:
            # If exception is thrown display error message and ignore
            print 'File: '+duplicateFileName+ " in dataset " + dsName + " is already logged as bad on the system"
            
    # Log a tag against a package
    def addTag(self, package, tag):
        try:
            # Insert tag and package into database
            print tag," " ,package
            tagID=self.getTagID(package, tag)
            if tagID!= None:
                return tagID
            tagID = self.selectCur.execute("select cms_cmgdb.tag_id_seq.NEXTVAL from dual").fetchone()[0]
            self.insertCur.execute("INSERT INTO cms_cmgdb.tags(tag_id, package_name, tag) values(:tagID , :package, :tag)",{"tagID":tagID,"package":package,"tag":tag})
            self.insertConn.commit()
            return tagID
        except cx_Oracle.IntegrityError:
            # If exception is thrown display error message and ignore
            print tag + " " + package + " was not added."
            return None


    # Adds a set to the database
    def addDataset(self, dsName,eos,lfn, fileOwner, parentID, username):
        
        try:
            datasetID = self.getDatasetIDWithName(dsName)
            if datasetID is not None:
                print "Dataset %s is already present on the system\n" % dsName
                return datasetID
            # Insert information into database
            datasetID = self.selectCur.execute("select cms_cmgdb.dataset_id_seq.NEXTVAL from dual").fetchone()[0]
            if parentID is None:self.insertCur.execute("INSERT INTO cms_cmgdb.dataset_details(dataset_id,parent_dataset_id,cmgdb_name,path_name,lfn,date_recorded,last_commented,file_owner,published_by) values(%d,NULL,'%s','%s','%s',CURRENT_TIMESTAMP,CURRENT_TIMESTAMP,'%s','%s')" % (datasetID,dsName,eos,lfn,fileOwner, username))
            else: self.insertCur.execute("INSERT INTO cms_cmgdb.dataset_details(dataset_id,parent_dataset_id,cmgdb_name,path_name,lfn,date_recorded,last_commented,file_owner,published_by) values(%d,%d,'%s','%s','%s',CURRENT_TIMESTAMP,CURRENT_TIMESTAMP,'%s','%s')" % (datasetID,parentID,dsName,eos,lfn,fileOwner, username))
            self.insertConn.commit()
            return datasetID
        except cx_Oracle.IntegrityError:
            # If set is already in the database then print error message and ignore
            print "Dataset %s is already present on the system\n" % dsName
        
    # Adds a set to the database
    def closeDataset(self, dsName):
        
        try:
            # Insert information into database
            datasetID = self.insertCur.execute("update cms_cmgdb.dataset_details set dataset_open='N' where cmgdb_name = %s" % dsName)
            self.insertConn.commit()
        except cx_Oracle.IntegrityError:
            # If set is already in the database then print error message and ignore
            print "Dataset %s failed to be closed\n" % dsName
        
    

    
    # Add a Dataset fraction to a logged dataset
    def addPrimaryDatasetFraction(self,datasetID, fraction):
        try:
            self.insertCur.execute("UPDATE cms_cmgdb.dataset_details set dataset_fraction='%f' WHERE dataset_id='%d'" % (fraction, datasetID))
            
            self.insertConn.commit()
        except cx_Oracle.IntegrityError:
            # If set doesn't exist print error message and ignore
            print "Dataset doesn't exist"
        except TypeError:
            pass
    
    # Add a Dataset size to a logged dataset
    def addDatasetSize(self,datasetID, size):
        try:
            self.insertCur.execute("UPDATE cms_cmgdb.dataset_details set dataset_size_in_tb='%f' WHERE dataset_id='%d'" % (size, datasetID))
            
            self.insertConn.commit()
        except cx_Oracle.IntegrityError:
            # If set doesn't exist print error message and ignore
            print "Dataset doesn't exist"
        except TypeError:
            pass
    
    # Add a Dataset file entries to a logged dataset
    def addFileEntries(self,datasetID, entries):
        try:
            self.insertCur.execute("UPDATE cms_cmgdb.dataset_details set dataset_entries='%d' WHERE dataset_id='%d'" % (int(entries), datasetID))
            
            self.insertConn.commit()
        except cx_Oracle.IntegrityError:
            # If set doesn't exist print error message and ignore
            print "Dataset doesn't exist"
        except TypeError:
            pass
    
    # Get dataset ID with name
    def getDatasetIDWithName(self, datasetName):
        try:
            return self.selectCur.execute("SELECT dataset_id from cms_cmgdb.dataset_details where cmgdb_name='%s'" % datasetName).fetchone()[0]
        except:
            return None
    def getParentsWithName(self, datasetName):
        try:
            strippedDSName = re.sub("--[\w_]+---[\w_]+","---%",datasetName)
            datasets = self.selectCur.execute("SELECT cmgdb_name, dataset_id from cms_cmgdb.dataset_details where cmgdb_name LIKE '%s'" % strippedDSName)
            checkedDatasets = []
            for row in datasets:
                check = re.sub("--[\w_]+---[\w_]+","---[\w_]+",datasetName)
                if re.match(check, row[0]):
                    checkedDatasets.append(row)
            return checkedDatasets
        except:
            print "No Parent Dataset found"
            return []

        
    # Get dataset ID with DBS
    def getDatasetIDWithTaskID(self,taskID,test):
        
        taskType = "task_id"
        # Insert information into database
        if test == True: taskType = "test_task_id"
        try:
            return self.selectCur.execute("SELECT dataset_id from cms_cmgdb.dataset_details where %s=%d" % (taskType, int(taskID))).fetchone()[0]
        except: 
            print "Datset is new in CMGDB"
            return None
            
    
    # Method adds a Savannah Task ID to a dataset using the task_id attribute.
    def addTaskID(self, datasetID, taskID, test):
        try:
            taskType = "task_id"
            writtenType = "Task ID"
            # Insert information into database
            if test == True: 
                taskType = "test_task_id"
                writtenType = "Test task ID"
            self.insertCur.execute("UPDATE cms_cmgdb.dataset_details set last_commented = CURRENT_TIMESTAMP WHERE dataset_id=%d" % int(datasetID))
            self.insertCur.execute("UPDATE cms_cmgdb.dataset_details set task_id=%d WHERE dataset_id=%d" % (int(taskID),int(datasetID)))
            
            self.insertConn.commit()
            print "%s: %s added to Dataset ID: %d on CMGDB" % (writtenType,taskID,datasetID)
        except cx_Oracle.IntegrityError:
            # If set doesn't exist print error message and ignore
            print "Dataset doesn't exist on CMGDB archive\n"
        except TypeError:
            pass
            
            
    # Method adds a Tag Set ID to a dataset using the tagset_id attribute.
    def addTagSetID(self, tagsetID, datasetID):
        try:
            # Insert information into database (THIS IS THE LINE THAT WILL NEED TO CHANGE)
            self.insertCur.execute("UPDATE cms_cmgdb.dataset_details set tagset_id='%d' WHERE dataset_id=%d" % (tagsetID, datasetID))
            self.insertConn.commit()
        except cx_Oracle.IntegrityError:
            # If set doesn't exist print error message and ignore
            print "Dataset doesn't exist on CMGDB archive\n"
        

    # Returns the tag_ID value which can be used to map datasets to packages
    def getTagID(self, package, tag):
        # Get the packages row using the select statement (tuple will only have 1 item, but will be returned as a tuple, not an int)
        row =  self.selectCur.execute("SELECT tag_id FROM cms_cmgdb.tags WHERE package_name = :package AND tag=:tag",{"package":package,"tag":tag})
        # Return tuples first (and only) value
        try:
            return row.fetchone()[0]
        except Exception as ex:
            return None

    # Returns the tag_ID value which can be used to map datasets to packages
    def _getPackageName(self, tagID):
        # Get the packages row using the select statement (tuple will only have 1 item, but will be returned as a tuple, not an int)
        row =  self.selectCur.execute("SELECT package_name FROM cms_cmgdb.tags WHERE tag_id = :tagID",{"tagID":tagID})
        # Return tuples first (and only) value
        return row.fetchone()[0]

    
    def getTaskID(self, datasetID, test):
        try:
            taskType = "task_id"
            # Insert information into database
            if test == True: taskType = "test_task_id"
            return self.selectCur.execute("SELECT %s from cms_cmgdb.dataset_details where dataset_id='%d'" % (taskType, datasetID)).fetchone()[0]
        except:return None
    
    def getTags(self, datasetID):
        try:
            setID = self.selectCur.execute("Select tagset_id from cms_cmgdb.dataset_details where dataset_id='%d'"% datasetID).fetchone()[0]
            tagDicts = []
            tags = self.selectCur.execute("select * from cms_cmgdb.tags where tagset_id = '%d'" % setID)
            for tag in tags:
                tagDicts.append({"Package":details[1],"Tag":details[2]})
            return tagDicts
        except: return None

    
    ### Adds tags to tag set and set to dataset
    def addTagSet(self, release, hash):
        tagSetID = self.getTagSetID(hash)
        if tagSetID is not None:
            return tagSetID
        tagSetID = self.selectCur.execute("select cms_cmgdb.set_id_seq.NEXTVAL from dual").fetchone()[0]
        self.insertCur.execute("insert into cms_cmgdb.tag_sets values(:tagSetID, :release, :hash)", {"tagSetID":tagSetID,"release":release,"hash":hash})
        try:            
            self.insertConn.commit()
            return tagSetID
        except:
            print "Database error encountered, exiting"
            print "Tagset not added to dataset"
            return None
    
    def addTagToSet(self, tagID, tagSetID):
        try:
            self.insertCur.execute("insert into cms_cmgdb.tags_in_sets values(:setID,:tag)",{"setID":tagSetID,"tag":tagID})
        except cx_Oracle.IntegrityError:
            print "Tag already in set"
        self.insertConn.commit()
        return tagSetID

    ### get tag set ID with hash
    def getTagSetID(self, hash):
        try:
            set = self.selectCur.execute("select tagset_id from cms_cmgdb.tag_sets where tag_hash=%d" % hash).fetchone()
            return set[0]
        except:
            return None
            
    # Close connection with database and destroy object
    def close(self):
        self.insertConn.close()


    
