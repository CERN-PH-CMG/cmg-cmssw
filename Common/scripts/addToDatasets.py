#!/usr/bin/env python
# Colin
# additional layer, on top of cmsBatch.py

import os


def addToDatasets(sampleAndTier, user=os.getlogin()):
    notThere = os.system( 'grep %s ~%s/public/DataSets.txt' % (sampleAndTier,user) )
    if notThere:
        os.system('echo %s >> ~%s/public/DataSets.txt' % (sampleAndTier,user) ) 

