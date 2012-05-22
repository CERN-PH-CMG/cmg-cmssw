import FWCore.ParameterSet.Config as cms

def getEleEnergyCorrectionType(fileName):
    """Return the string to be used to configure calibratedGsfElectrons"""    

    def lookup( fileName, stringToFind ):
        '''predicate for identifying samples. could be more solid'''
        
        if fileName.find( stringToFind )>-1:
            return True
        else:
            return False


        # choose which kind of scale correction/MC smearing should be applied for electrons. Options are:
    if lookup (fileName,'Fall11' ):
        return "Fall11"
    elif lookup(fileName, 'Summer11' ):
        return "Summer11"
    elif lookup(fileName, 'Run2012A' ) or lookup(fileName, 'START52' ):
        return "None"                    #No correction for 2012 data and MC!
    elif lookup(fileName, 'Jan16ReReco' ):
        return "Jan16ReReco"
    elif lookup(fileName, 'ReReco' ):
        return "ReReco"
    elif lookup(fileName, 'Prompt' ):
        return "Prompt"
    else :
        return "Unknown"


def setupGsfElectronCalibration( process, runOnMC, type=None):
    eleCorrectionType = type
    if eleCorrectionType is None:
        eleCorrectionType = getEleEnergyCorrectionType(process.source.fileNames[0])
    if (eleCorrectionType=="Unknown"):
        err = 'ERROR: Could not determine electron scale correction to be applied from dataset name {file}'.format(process.source.fileNames[0])
        raise ValueError(err)
    print "Setting process.calibratedGsfElectrons.inputDataset = ", eleCorrectionType
    process.gsfElectrons.inputDataset = eleCorrectionType
    process.gsfElectrons.isMC = cms.bool(runOnMC)
