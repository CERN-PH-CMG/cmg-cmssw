#!/usr/bin/env python
# Colin
# additional layer, on top of cmsBatch.py

import os


def addToDatasets(sampleAndTier):
    notThere = os.system( 'grep %s ~/DataSets.txt' % sampleAndTier )
    if notThere:
        os.system('echo %s >> ~/DataSets.txt' % sampleAndTier ) 

