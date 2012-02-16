import os 
from CMGTools.Common.Tools.applyJSON_cff import *
from CMGTools.H2TauTau.tools.jsonPick import *


def setupJSON( process ):

    print 'setting up JSON:'

    fileName = process.source.fileNames[0]
    print fileName
    # in case filename is a local filename, removing CMGLOCALBASEDIR
    fileName = fileName.replace( os.environ['CMGLOCALBASEDIR'],'' ) 
    json = jsonPick( fileName )
    applyJSON(process, json )
    return json
