from CMGTools.Common.Tools.applyJSON_cff import *
from CMGTools.H2TauTau.tools.jsonPick import *


def setupJSON( process ):

    print 'setting up JSON:'

    fileName = process.source.fileNames[0]
    print fileName
    #    json = None
    #    if fileName.find('Run201')>-1:
    #COLIN : I think the if is not necessary anymore
    json = jsonPick( fileName )
    applyJSON(process, json )
    return json
