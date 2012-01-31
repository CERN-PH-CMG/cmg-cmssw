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

import sys
#import CMGTools.Production.cx_Oracle as cx_Oracle
import cx_Oracle
class CmgdbApi(object):
    
    def __init__(self):
        # Creates the connection object that will be used by the class to interface with the database
        try:
            self.conn = cx_Oracle.connect("cmgbookkeeping_user/Cmguser1@(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=oradev11.cern.ch)(PORT=10121))(ENABLE=BROKEN)(CONNECT_DATA= (SID=DEVDB11)))")
            self.cur = self.conn.cursor()
        except Exception as dbError:
            print "Unable to connect to CMGDB"
            print dbError.args[0]
            return None

    # Log a tag against a package
    def addTag(self, package, tag):
        try:
            # Insert tag and package into database
            self.cur.execute("INSERT INTO tags(tag_id, package_name, tag) values(tag_id_seq.NEXTVAL , :package, :tag)",{"package":package,"tag":tag})
            self.conn.commit()
        except cx_Oracle.IntegrityError:
            # If exception is thrown display error message and ignore
            print tag + " " + package + " is already present on the system"


    # Adds a set to the database
    def addDataset(self, dsName, user):
        
        try:
            # Insert information into database
            datasetID = self.cur.execute("select dataset_id_seq.NEXTVAL from dual").fetchone()[0]
            self.cur.execute("INSERT INTO set_details(dataset_id,date_recorded,nice_username) values(%d,sysdate,'%s')" % (datasetID,user))
            self.conn.commit()
            return datasetID
        except cx_Oracle.IntegrityError:
            # If set is already in the database then print error message and ignore
            print "Dataset %s is already present on the system\n" % dsName
    
    # Add a DBS ID to a logged dataset
    def addDbsID(self,datasetID, dbsID):
        try:
            self.cur.execute("UPDATE set_details set dbs_id='%d' WHERE dataset_id='%d'" % (dbsID, datasetID))
            
            self.conn.commit()
        except cx_Oracle.IntegrityError:
            # If set doesn't exist print error message and ignore
            print "DBS entry doesn't exist on CMGDB archive\n"
        except TypeError:
            pass
            
    # Get dataset ID with DBS
    def getDatasetIDWithDbsID(self,dbsID):
        try:
            return self.cur.execute("SELECT dataset_id from set_details where dbs_id='%d'" % dbsID).fetchone()[0]
        except:return None
        
    # Get dataset ID with DBS
    def getDatasetIDWithTaskID(self,taskID,test):
        
        taskType = "task_id"
        # Insert information into database
        if test == True: taskType = "test_task_id"
        try:
            return self.cur.execute("SELECT dataset_id from set_details where %s='%d'" % (taskType, int(taskID))).fetchone()[0]
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
            self.cur.execute("UPDATE set_details set %s='%d' WHERE dataset_id='%d'" % (taskType, int(taskID), datasetID))
            
            self.conn.commit()
            print "%s: %s added to Dataset ID: %d on CMGDB" % (writtenType,taskID,datasetID)
        except cx_Oracle.IntegrityError:
            # If set doesn't exist print error message and ignore
            print "Dataset doesn't exist on CMGDB archive\n"
            
            
    # Method adds a Tag Set ID to a dataset using the set_id attribute.
    def addTagSetID(self, setID, datasetID):
        try:
            # Insert information into database (THIS IS THE LINE THAT WILL NEED TO CHANGE)
            self.cur.execute("UPDATE set_details set set_id='%d' WHERE dataset_id='%d'" % (setID, datasetID))
            self.conn.commit()
        except cx_Oracle.IntegrityError:
            # If set doesn't exist print error message and ignore
            print "Dataset doesn't exist on CMGDB archive\n"
        

    # Returns the tag_ID value which can be used to map datasets to packages
    def getTagID(self, package, tag):
        # Get the packages row using the select statement (tuple will only have 1 item, but will be returned as a tuple, not an int)
        row =  self.cur.execute("SELECT tag_id FROM tags WHERE package_name = :package AND tag=:tag",{"package":package,"tag":tag})
        # Return tuples first (and only) value
        return row.fetchone()[0]

    # Returns the tag_ID value which can be used to map datasets to packages
    def _getPackageName(self, tagID):
        # Get the packages row using the select statement (tuple will only have 1 item, but will be returned as a tuple, not an int)
        row =  self.cur.execute("SELECT package_name FROM tags WHERE tag_id = :tagID",{"tagID":tagID})
        # Return tuples first (and only) value
        return row.fetchone()[0]

    
    def getTaskID(self, dbsID, test):
        try:
            taskType = "task_id"
            # Insert information into database
            if test == True: taskType = "test_task_id"
            return self.cur.execute("SELECT %s from set_details where dbs_id='%d'" % (taskType, dbsID)).fetchone()[0]
        except:return None
    
    def getTags(self, dbsID):
        try:
            setID = self.cur.execute("Select set_id from set_details where dbs_id='%d'"% dbsID).fetchone()[0]
            tagDicts = []
            tags = self.cur.execute("select * from tags where set_id = '%d'" % setID)
            for tag in tags:
                tagDicts.append({"Package":details[1],"Tag":details[2]})
            return tagDicts
        except: return None

    def checkSet(self, tag_ids):
        try:

            for row in self.cur.execute("Select set_id from tags_in_sets where tag_id=%d" %tag_ids[0]):

                for tag in tag_ids:
                    missed = False
                    if not self.cur.execute("select * from tags_in_sets where set_id=:setID and tag_id=:tagID",{"setID":row[0],"tagID":tag}).fetchone():
                        missed = True
                        break
                    if missed == False: return row[0]
            return None
        except: return None
    
    ### Adds tags to tag set and set to dataset
    def addSet(self, tag_ids, release):
        print release
        setID = self.checkSet(tag_ids)
        if setID is not None: return setID
        setID = self.cur.execute("select set_id_seq.NEXTVAL from dual").fetchone()[0]
        self.cur.execute("insert into tag_sets values(:setID, :release)", {"setID":setID,"release":release})
        try:
            
            print setID
            
            self.conn.commit()
        except:
            print "Database error encountered, exiting"
            print "Tagset not added to dataset"
            return None
            
        for tag in tag_ids:
            try:
                self.cur.execute("insert into tags_in_sets values(:setID,:tag)",{"setID":setID,"tag":tag})
            except cx_Oracle.IntegrityError:
                print "Tag already in set"
        self.conn.commit()
        return setID

    # Close connection with database and destroy object
    def close(self):
        self.conn.close()


    
