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
            self.conn = cx_Oracle.connect("cmgbookkeepingtest/Cmguser1@(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=oradev11.cern.ch)(PORT=10121))(ENABLE=BROKEN)(CONNECT_DATA= (SID=DEVDB11)))")
            self.cur = self.conn.cursor()
        except Exception as dbError:
            print "Unable to connect to CMGDB"
            print dbError.args[0]
            return None

    # Clear missing file entries
    def clearDatasetMissingFiles(self, dsName, datasetID):
        try:
            self.cur.execute('DELETE FROM missing_files WHERE dataset_id = %d' % int(datasetID))
            self.conn.commit()
        except cx_Oracle.IntegrityError:
            print "Unable to delete missing file record"
            
    # Log missing files
    def addMissingFile(self, dsName, datasetID, missingFileName):
        try:
            self.cur.execute("INSERT INTO missing_files(dataset_id, missing_file) values(%d, '%s')" % (datasetID, missingFileName))
            self.conn.commit()
        except cx_Oracle.IntegrityError:
            # If exception is thrown display error message and ignore
            print 'File: '+missingFileName+ " in dataset " + dsName + " is already logged as missing on the system"
            
    # Clear bad file entries
    def clearDatasetBadFiles(self, dsName, datasetID):
        try:
            self.cur.execute('DELETE FROM bad_files WHERE dataset_id = %d' % int(datasetID))
            self.conn.commit()
        except cx_Oracle.IntegrityError:
            print "Unable to delete file entries"
            
    # Log bad files
    def addBadFile(self, dsName, datasetID, badFileName):
        try:
            self.cur.execute("INSERT INTO bad_files(dataset_id, bad_file) values(%d, '%s')" % (datasetID, str(badFileName)))
            self.conn.commit()
        except cx_Oracle.IntegrityError:
            # If exception is thrown display error message and ignore
            print 'File: '+badFileName+ " in dataset " + dsName + " is already logged as bad on the system"
    # Clear bad jobs entries
    def clearDatasetBadJobs(self, dsName, datasetID):
        try:
            self.cur.execute('DELETE FROM bad_jobs WHERE dataset_id = %d' % int(datasetID))
            self.conn.commit()
        except cx_Oracle.IntegrityError:
            print "Unable to delete bad job record"
            
    # Log missing files
    def addBadJob(self, dsName, datasetID, badJobNum):
        try:
            self.cur.execute('INSERT INTO bad_jobs(dataset_id, bad_job) values(%d, %d)' % (datasetID, int(badJobNum)))
            self.conn.commit()
        except cx_Oracle.IntegrityError:
            # If exception is thrown display error message and ignore
            print 'Job: '+badJobName+ " in dataset " + dsName + " is already logged as bad on the system"
    # Clear missing file entries
    def clearDatasetDuplicateFiles(self, dsName, datasetID):
        try:
            self.cur.execute('DELETE FROM duplicate_files WHERE dataset_id = %d' % int(datasetID))
            self.conn.commit()

        except cx_Oracle.IntegrityError:
            print "Unable to delete duplicate file record"
            
    # Log missing files
    def addDuplicateFile(self, dsName, datasetID, duplicateFileName):
        try:
            self.cur.execute('INSERT INTO duplicate_files(dataset_id, duplicate_file) values(%d, %s)' % (datasetID, duplicateFileName))
            self.conn.commit()
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
            tagID = self.cur.execute("select tag_id_seq.NEXTVAL from dual").fetchone()[0]
            self.cur.execute("INSERT INTO tags(tag_id, package_name, tag) values(:tagID , :package, :tag)",{"tagID":tagID,"package":package,"tag":tag})
            self.conn.commit()
            return tagID
        except cx_Oracle.IntegrityError:
            # If exception is thrown display error message and ignore
            print tag + " " + package + " was not added."
            return None


    # Adds a set to the database
    def addDataset(self, dsName,eos,lfn, user, parentID):
        
        try:
            datasetID = self.getDatasetIDWithName(dsName)
            if datasetID is not None:
                print "Dataset %s is already present on the system\n" % dsName
                return datasetID
            # Insert information into database
            datasetID = self.cur.execute("select dataset_id_seq.NEXTVAL from dual").fetchone()[0]
            if parentID is None:self.cur.execute("INSERT INTO dataset_details(dataset_id,parent_dataset_id,cmgdb_name,path_name,lfn,date_recorded,last_commented,nice_username) values(%d,NULL,'%s','%s','%s',CURRENT_TIMESTAMP,CURRENT_TIMESTAMP,'%s')" % (datasetID,dsName,eos,lfn,user))
            else: self.cur.execute("INSERT INTO dataset_details(dataset_id,parent_dataset_id,cmgdb_name,path_name,lfn,date_recorded,last_commented,nice_username) values(%d,%d,'%s','%s','%s',CURRENT_TIMESTAMP,CURRENT_TIMESTAMP,'%s')" % (datasetID,parentID,dsName,eos,lfn,user))
            self.conn.commit()
            return datasetID
        except cx_Oracle.IntegrityError:
            # If set is already in the database then print error message and ignore
            print "Dataset %s is already present on the system\n" % dsName
        
    

    
    # Add a Dataset fraction to a logged dataset
    def addPrimaryDatasetFraction(self,datasetID, fraction):
        try:
            self.cur.execute("UPDATE dataset_details set dataset_fraction='%f' WHERE dataset_id='%d'" % (fraction, datasetID))
            
            self.conn.commit()
        except cx_Oracle.IntegrityError:
            # If set doesn't exist print error message and ignore
            print "Dataset doesn't exist"
        except TypeError:
            pass
    # Add a Dataset fraction to a logged dataset
    def addFileEntries(self,datasetID, entries):
        try:
            self.cur.execute("UPDATE dataset_details set dataset_entries='%d' WHERE dataset_id='%d'" % (int(entries), datasetID))
            
            self.conn.commit()
        except cx_Oracle.IntegrityError:
            # If set doesn't exist print error message and ignore
            print "Dataset doesn't exist"
        except TypeError:
            pass
    # Get dataset ID with name
    def getDatasetIDWithName(self, datasetName):
        try:
            return self.cur.execute("SELECT dataset_id from dataset_details where cmgdb_name='%s'" % datasetName).fetchone()[0]
        except:
            return None
    def getParentsWithName(self, datasetName):
        try:
            strippedDSName = re.sub("--[\w_]+---[\w_]+","---%",datasetName)
            datasets = self.cur.execute("SELECT cmgdb_name, dataset_id from dataset_details where cmgdb_name LIKE '%s'" % strippedDSName)
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
            return self.cur.execute("SELECT dataset_id from dataset_details where %s=%d" % (taskType, int(taskID))).fetchone()[0]
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
            self.cur.execute("UPDATE dataset_details set last_commented = CURRENT_TIMESTAMP WHERE dataset_id=%d" % int(datasetID))
            self.cur.execute("UPDATE dataset_details set task_id=%d WHERE dataset_id=%d" % (int(taskID),int(datasetID)))
            
            self.conn.commit()
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
            self.cur.execute("UPDATE dataset_details set tagset_id='%d' WHERE dataset_id=%d" % (tagsetID, datasetID))
            self.conn.commit()
        except cx_Oracle.IntegrityError:
            # If set doesn't exist print error message and ignore
            print "Dataset doesn't exist on CMGDB archive\n"
        

    # Returns the tag_ID value which can be used to map datasets to packages
    def getTagID(self, package, tag):
        # Get the packages row using the select statement (tuple will only have 1 item, but will be returned as a tuple, not an int)
        row =  self.cur.execute("SELECT tag_id FROM tags WHERE package_name = :package AND tag=:tag",{"package":package,"tag":tag})
        # Return tuples first (and only) value
        try:
            return row.fetchone()[0]
        except Exception as ex:
            return None

    # Returns the tag_ID value which can be used to map datasets to packages
    def _getPackageName(self, tagID):
        # Get the packages row using the select statement (tuple will only have 1 item, but will be returned as a tuple, not an int)
        row =  self.cur.execute("SELECT package_name FROM tags WHERE tag_id = :tagID",{"tagID":tagID})
        # Return tuples first (and only) value
        return row.fetchone()[0]

    
    def getTaskID(self, datasetID, test):
        try:
            taskType = "task_id"
            # Insert information into database
            if test == True: taskType = "test_task_id"
            return self.cur.execute("SELECT %s from dataset_details where dataset_id='%d'" % (taskType, datasetID)).fetchone()[0]
        except:return None
    
    def getTags(self, datasetID):
        try:
            setID = self.cur.execute("Select tagset_id from dataset_details where dataset_id='%d'"% datasetID).fetchone()[0]
            tagDicts = []
            tags = self.cur.execute("select * from tags where tagset_id = '%d'" % setID)
            for tag in tags:
                tagDicts.append({"Package":details[1],"Tag":details[2]})
            return tagDicts
        except: return None

    
    ### Adds tags to tag set and set to dataset
    def addTagSet(self, release, hash):
        tagSetID = self.getTagSetID(hash)
        if tagSetID is not None:
            return tagSetID
        tagSetID = self.cur.execute("select set_id_seq.NEXTVAL from dual").fetchone()[0]
        self.cur.execute("insert into tag_sets values(:tagSetID, :release, :hash)", {"tagSetID":tagSetID,"release":release,"hash":hash})
        try:            
            self.conn.commit()
            return tagSetID
        except:
            print "Database error encountered, exiting"
            print "Tagset not added to dataset"
            return None
    
    def addTagToSet(self, tagID, tagSetID):
        try:
            self.cur.execute("insert into tags_in_sets values(:setID,:tag)",{"setID":tagSetID,"tag":tagID})
        except cx_Oracle.IntegrityError:
            print "Tag already in set"
        self.conn.commit()
        return tagSetID

    ### get tag set ID with hash
    def getTagSetID(self, hash):
        try:
            set = self.cur.execute("select tagset_id from tag_sets where tag_hash=%d" % hash).fetchone()
            return set[0]
        except:
            return None
            
    # Close connection with database and destroy object
    def close(self):
        self.conn.close()


    
