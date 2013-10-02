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
    elif lookup(fileName, 'Summer12' ):
        return "Summer12"
    elif lookup(fileName, 'Jan16ReReco' ) or lookup(fileName, '16Jan2012' ):
        return "Jan16ReReco"
    elif lookup(fileName, 'ReReco' ):
        return "ReReco"
    elif (lookup(fileName, 'Run2012') and lookup(fileName, 'PromptReco')):
        return "Prompt2012"        
    elif lookup(fileName, 'Prompt' ) or lookup(fileName, 'Nov2011' ):
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
    process.gsfElectrons.inputDataset = cms.string(eleCorrectionType)
    process.gsfElectrons.isMC = cms.bool(runOnMC)
