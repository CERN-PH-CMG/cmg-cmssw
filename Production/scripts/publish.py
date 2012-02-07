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
from optparse import *



if __name__ == '__main__':
    optManager  = DbsOptionParser()
    parser = optManager.parser
    
    parser.usage = """
%prog [options] <sampleName>

Use this script to publish dataset details to DBS, CmgDB, and savannah.
Example:
publish.py -F cbern /VBF_HToTauTau_M-120_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0_Test_v2
"""
    
    group = OptionGroup(parser, "Publish Options", """These options affect Savannah and CMGDB""")
    genGroup = OptionGroup(parser, "Generic Options", """These options apply to both DBS and the Publish tools""")
    dbsGroup = OptionGroup(parser, "DBS Options", """These options are for use with DBS.
    If not using DBS, these options have no effect.""")
    for i in parser._get_all_options():
    	opt_string = i.get_opt_string()
    	parser.remove_option(opt_string)
    	if opt_string == "--help" or opt_string == "--quiet" or opt_string == "--verbose":
    		genGroup.add_option(i)
    	elif opt_string == "--username" or opt_string == "--password":
    		pass
    	else:
    		dbsGroup.add_option(i)
    	
    
    # If user is not specified default is current user
    # This option will be used to find dataset on castor, and assign dataset on savannah
    group.add_option("-F", "--fileown", 
                      dest="fileown",
                      help="User who is the files owner on Castor." ,
                      default=os.environ['USER'] )
    # If specified is used to log in to savannah (only required if user that created the dataset,
    # is different to user publishing it)
    genGroup.add_option("-u", "--username",
                      action = "store",
                      dest="username",
                      help="""Specify the username to access both the DBS and savannah servers. 
Default is $USER.""",
                      default=os.environ['USER'] )
    # If specified is used as password to DBS and savannah 
    # If ommited the secure password prompt will appear
    genGroup.add_option("-p", "--password",
                      action = "store",
                      dest="password",
                      help="""Specify the password to access both the DBS and savannah servers.
If not entered, secure password prompt will appear.""",
                      default=None )
    # If the purpose is to test the software use this parameter, it will not be recognised by the
    # non-testing algorithm
    group.add_option("-T", "--test",
                      action = "store_true",
                      dest="test",
                      help="Flag task as a test",
                      default=False )
    # If user wants to add their own comments
    group.add_option("-C", "--comment",
                      action = "store",
                      dest="commented",
                      help="Take comment as an argument",
                      default = None)
    # If user wants to add their own comments
    group.add_option("-D", "--dbs",
                      action = "store_true",
                      dest="dbs",
                      help="Use DBS",
                      default = False)
    # If user wants to add multiple datasets from file
    group.add_option("-M", "--multi",
                      action = "store_true",
                      dest="multi",
                      help="""Argument is now LFN to location of .txt file
							Entries in the file should be on independant lines in the form: DatasetName Fileowner 'comment'
							Comment is not compulsory, and if fileowner is not entered, $USER will be used as default.
							Comment MUST be enclosed in speech marks
							E.g.
							/MuHad/Run2011A-05Aug2011-v1/AOD/V2 cmgtools 'comment'
							Single or double speech marks are accepted""",
                      default = False)
    parser.add_option_group(genGroup)
    parser.add_option_group(dbsGroup)
    parser.add_option_group(group)
    
    (options, args) = optManager.getOpt()
    options.url="http://cmsphys05.cern.ch:8081/cms_dbs_prod_local_01/servlet/DBSServlet"
    
    # Allow no more than one argument
    if len(args)!=1:
        parser.print_help()
        sys.exit(1)
        
    if options.password == None:
    	password = getpass.getpass("Enter NICE Password: ")
    	options.password = password
    if not validLogin(options.username, password):
    	print "Authentication Failed, exiting\n\n"
    	sys.exit(1)
    
    
    if options.dbs:
    	dbsApi = DbsApi(options.__dict__)
    else: dbsApi = None
    
    
    # For multiple file input
    if options.multi:
        file = open(args[0], 'r')
        lines = file.readlines()
        for line in lines:
            try:
                fileown = None
                dataset = line.split(" ")[0]
                if not re.search("---", dataset):
                	if len(line.rstrip(" ").lstrip(" ").split(" ")) ==1:
                		dataset = line.rstrip("\n")
                		fileown = options.fileown
                	if len(line.split(" ")) >1 and re.search("'",line) is None and re.search('"',line) is None:
                		fileown = line.split(" ")[1].rstrip("\n")
                	elif re.search("'",line):
                		preComment = line.split("'")[0]
                		if len(preComment.rstrip(" ").split(" ")) == 2:
                			fileown = preComment.rstrip(" ").split(" ")[1]
                		else:
                			fileown = options.fileown
                	elif re.search('"',line):
                		preComment = line.split('"')[0]
                		if len(preComment.rstrip(" ").split(" ")) == 2:
                			fileown = preComment.rstrip(" ").split(" ")[1]
                		else:
                			fileown = options.fileown
                	else:
                		fileown = options.fileown
                	
                comment = None
                if len(line.split("'"))>1:
                	comment = line.rstrip("'").split("'")[1]
                elif len(line.split('"'))>1:
                	comment = line.rstrip('"').split('"')[1]
                publish(dataset,fileown,comment,options.test,dbsApi,options.username,password)
            except Exception as err:
                print err, "\nDataset not published"
    # For singular file input
    else:
        dataset = args[0]
        comment = options.commented
        publish(dataset,options.fileown,comment,options.test,dbsApi,options.username,password)
       