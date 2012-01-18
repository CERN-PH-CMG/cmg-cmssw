#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 26th 2011

from DBSAPI.dbsProcessedDataset import DbsProcessedDataset
from DBSAPI.dbsPrimaryDataset import DbsPrimaryDataset
from datetime import datetime
from CMGTools.Production.nameOps import *

class CastorToDbsFormatter(object):
    def __init__(self,DSName,user):


        parent = getUnknownParentWithDbs(getDbs(DSName))
        path = getDbsWithUser(DSName, user)
        parts = path.lstrip("/").split("/")
        primary = DbsPrimaryDataset (Name = parts[0])


        tierList=parts[2].split("-")
        self.procds = DbsProcessedDataset(PrimaryDataset = primary,
                                          Name = parts[1],
                                          PhysicsGroup = "CMG",
                                          Status = "VALID",
                                          TierList = tierList,
                                          PathList = [path,],
                                          DateCreated = datetime.now().strftime("%s")
                                          )
        if parent is not None:
            self.procds['ParentList'] = [parent,]
        if user is not None:
            self.procds['CreatedBy'] = user

    def addElement(self, name, element):
        if self.procds[name]:
            self.procds[name]=element
            return True
        else: return False

    def getDataset(self):
        return self.procds
        


