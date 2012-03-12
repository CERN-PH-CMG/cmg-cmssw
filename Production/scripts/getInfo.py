#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## November 2nd 2011

import os, getpass, sys, re, optparse
from datetime import *
from CMGTools.Production.fileOps import FileOps
from CMGTools.Production.nameOps import *
os.system("source /afs/cern.ch/cms/slc5_amd64_gcc434/external/oracle/11.2.0.1.0p2/etc/profile.d/init.sh")
os.system("source /afs/cern.ch/cms/slc5_amd64_gcc434/external/python/2.6.4-cms16/etc/profile.d/init.sh")
os.system("source /afs/cern.ch/cms/slc5_amd64_gcc434/external/py2-cx-oracle/5.1/etc/profile.d/init.sh") 
import CMGTools.Production.cx_Oracle as cx_Oracle

if __name__ == '__main__':

    parser = optparse.OptionParser()

    parser.usage = """
Table Structure:
    dataset_details
        -dataset_id
        -parent_dataset_id
        -cmgdb_name
        -path_name
        -lfn
        -nice_username
        -dataset_fraction
        -dataset_entries
        -date_recorded
        -last_commented
        -task_id
        -tagset_id
    tag_sets
        -tagset_id
        -release
    tags
        -tag_id
        -package_name
        -tag
    tags_in_sets
        -tag_id
        -tagset_id
    bad_files
        -dataset_id
        -bad_file
    missing_files
        -dataset_id
        -missing_file
    bad_jobs
        -dataset_id
        -bad_job
    duplicate_files
        -dataset_id
        -duplicate_file

The database server being used is Oracle 11g, so a good idea would be to become farmiliar with the Oracle 11g query semantics.

Here are some example queries explained:
    If you want to get a list of tags that were used by cmgtools on the 7th March 2012, 
    - You will have to first make sure that you use the distinct() method to prevent repetitions
    - Once you have specified the information you want, you will then need to perform a join with the tags_in_sets table on tag_id
    - From the product of this join you then want to join with the dataset_details table, this time on tagset_id
    - You then specify the WHERE clause as where nice_username = 'cmgtools and the date...
    - To give the results of the entire day, you must do two things,
    -- First, use the trunc() method to truncate the timestamp to the desired length
    -- Then, use the to_timestamp() method in order to change the date you want to a timestamp

        "SELECT distinct(tags.tag_id), tags.tag, tags.package_name 
        FROM tags 
        INNER JOIN tags_in_sets ON tags_in_sets.tag_id = tags.tag_id 
        JOIN dataset_details ON dataset_details.tagset_id = tags_in_sets.tagset_id 
        WHERE dataset_details.nice_username = 'cmgtools' 
        AND trunc(dataset_details.date_recorded) = to_timestamp('07-03-2012','DD-MM-YYYY') 
        ORDER BY tags.tag_id"
    
    If you want to get a list of dataset names from the same time period, again from cmgtools,
    - You would first select the details you want to see, e.g dataset_fraction
    - Then you would use the same WHERE clause as in the previous example
        
        "SELECT dataset_id, path_name, date_recorded, dataset_fraction 
        FROM dataset_details 
        WHERE trunc(date_recorded) = to_timestamp('07-03-2012','DD-MM-YYYY') 
        AND nice_username = 'cmgtools' 
        ORDER BY dataset_id"
        
    If you want a list of datasets that used a certain tag
    - First specify the fields you want to select, in this case dataset id and name
    - Perform an INNER JOIN to on tagset_id to get
    - Then perform another join with the tags table, and specify the WHERE clause
    - Finally specify what to order the list by
    
        "SELECT dataset_details.dataset_id, dataset_details.path_name 
        FROM dataset_details 
        INNER JOIN tags_in_sets on tags_in_sets.tagset_id = dataset_details.tagset_id 
        JOIN tags on tags.tag_id = tags_in_sets.tag_id 
        WHERE tags.tag = 'B4_2_X_V00-03-00' 
        AND tags.package_name = 'CommonTools/ParticleFlow'
        ORDER BY dataset_details.dataset_id"
    
    If you want to find the missing files on a particular dataset
    - First specify what you want to select
    - Then join on dataset id
    - Then specify which dataset
        
        "SELECT missing_files.missing_file from missing_files
        INNER JOIN dataset_details on dataset_details.dataset_id = missing_files.dataset_id
        WHERE cmgdb_name = '/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START44_V9B-v1--V3---cmgtools_group/AODSIM'"

Usage: 
-----
%prog -s <query>
-----

Alias commands will be added soon for the more common tasks.
Please experiment and email Peter Meckiffe with your suggestions for alias
"""
    
    parser.add_option("-s", "--sql",
                      action = "store_true",
                      dest="sql",
                      help="Enter a raw sql query for cmgdb"
                      )

    (options, args) = parser.parse_args()
    # Allow no more than one argument
    noOptions = False
    if options.sql is None:
        parser.print_help()
        sys.exit(1)
    if len(args)!=1:
        parser.print_help()
        sys.exit(1)
    if options.sql:
        query = args[0]
        select = re.compile('select', re.IGNORECASE)
        if not select.search(query):
            print "getDataset.py is for search uses only (SELECT queries). To publish, please use the publish.py script"
            sys.exit(1)
        conn = cx_Oracle.connect("cmgbookkeepingtest/Cmguser1@(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=oradev11.cern.ch)(PORT=10121))(ENABLE=BROKEN)(CONNECT_DATA= (SID=DEVDB11)))")
        cur = conn.cursor()
        cur.execute(query)
        colnames = ""
        for column in cur.description:
            colnames += column[0] + "\t"
        print colnames
        for row in cur:
            string = ""
            for column in row:
                string += str(column) + " ||\t"
            string = string.rstrip(" ||\t")
            print string
    
 