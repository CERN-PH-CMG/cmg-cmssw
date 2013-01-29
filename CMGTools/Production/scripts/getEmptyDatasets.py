#!/usr/bin/env python
## Author: Anastasios Antoniadis

import CMGTools.Production.cx_Oracle as cx_Oracle

from CMGTools.Production.cmgdbApi import CmgdbApi
from CMGTools.Production.castorBaseDir import castorBaseDir
import CMGTools.Production.eostools as castortools


if __name__ == '__main__':
    cmgdbApi = CmgdbApi()
    cmgdbApi.connect()
    
    columns, rows = cmgdbApi.sql( "select file_owner, path_name from dataset_details where dataset_is_open='Y'" )

    f = open('emptyDatasets.txt','w')
    
    for row in rows:
        fileown = row[0]
        dsName = row[1]
        
	if fileown == "--cmgtools":
	    fileown = "cmgtools"
	
        lfnDir = castorBaseDir(fileown) + dsName
        castorDir = castortools.lfnToCastor( lfnDir )

        if castortools.datasetNotEmpty( castorDir, ".*root" ):
            continue
        else:
            f.write( fileown + "%" + dsName  + '\n' )
            
    f.close()
    
