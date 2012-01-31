#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

import os, getpass, sys, re
from CMGTools.Production.castorToDbsFormatter import CastorToDbsFormatter
from CMGTools.Production.publishController import PublishController
from DBSAPI.dbsProcessedDataset import DbsProcessedDataset
from DBSAPI.dbsPrimaryDataset import DbsPrimaryDataset
from datetime import *
from DBSAPI.dbsOptions import DbsOptionParser
from CMGTools.Production.findDSOnSav import validLogin
from DBSAPI.dbsApi import DbsApi
from DBSAPI.dbsApiException import *
from DBSAPI.dbsException import *
from CMGTools.Production.publish import publish



if __name__ == '__main__':
    optManager  = DbsOptionParser()
    parser = optManager.parser

    parser.usage = """
%prog [options] <sampleName>

Use this script to publish dataset details to DBS, CmgDB, and savannah.
Example:
publish.py -F cbern /VBF_HToTauTau_M-120_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0_Test_v2
"""
    
    # If user is not specified default is current user
    # This option will be used to find dataset on castor, and assign dataset on savannah
    parser.add_option("-F", "--fileown", 
                      dest="fileown",
                      help="User who is the files owner on Castor." ,
                      default=os.environ['USER'] )
    # If specified is used to log in to savannah (only required if user that created the dataset,
    # is different to user publishing it)
    parser.add_option("-u", "--user",
                      action = "store",
                      dest="user",
                      help="Specify the username to access both the DBS and savannah servers. Default is $USER.",
                      default=os.environ['USER'] )
    # If the purpose is to test the software use this parameter, it will not be recognised by the
    # non-testing algorithm
    parser.add_option("-T", "--test",
                      action = "store_true",
                      dest="test",
                      help="Flag task as a test",
                      default=False )
    # If user wants to add their own comments
    parser.add_option("-C", "--comment",
                      action = "store",
                      dest="commented",
                      help="Take comment as arg",
                      default = None)
    # If user wants to add their own comments
    parser.add_option("-D", "--dbs",
                      action = "store_true",
                      dest="dbs",
                      help="Use DBS",
                      default = False)
    # If user wants to add multiple datasets from file
    parser.add_option("-M", "--multi",
                      action = "store_true",
                      dest="multi",
                      help="""Argument is now LFN to location of .txt file
                      Entries in the file should be on independant lines in the form: dataset fileowner 'comment'""",
                      default = False)
    
    
    (options, args) = optManager.getOpt()
    options.url="http://cmsphys05.cern.ch:8081/cms_dbs_prod_local_01/servlet/DBSServlet"
    
    
    
    if options.dbs:
    	dbsApi = DbsApi(options.__dict__)
    else: dbsApi = None
    # Allow no more than one argument
    if len(args)!=1:
        parser.print_help()
        sys.exit(1)
    
    
    
    password = getpass.getpass("Enter NICE Password: ")
    if not validLogin(options.user, password):
    	print "Authentication Failed, exiting\n\n"
    	sys.exit(1)
    
    # For multiple file input
    if options.multi:
        file = open(args[0], 'r')
        lines = file.readlines()
        for line in lines:
            try:
                fileown = None
                dataset = line.split(" ")[0]
                if not re.search("---", dataset):
                    fileown = line.split("'")[0].split(" ")[1]
                comment = None
                if len(line.split("'"))>1:
                    comment = line.rstrip("'").split("'")[1]
                publish(dataset,fileown,comment,options.test,dbsApi,options.user,password)
            except Exception as err:
                print err, "\nDataset not published"
    # For singular file input
    else:
        dataset = args[0]
        comment = options.commented
        publish(dataset,options.fileown,comment,options.test,dbsApi,options.user,password)
       