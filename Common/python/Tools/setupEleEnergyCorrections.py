
def setupEleEnergyCorrections( process ):

    print 'setting up electron energy collections:'

    fileName = process.source.fileNames[0]
    print fileName

    def lookup( fileName, stringToFind ):
        '''predicate for identifying samples. could be more solid'''
        
        if fileName.find( stringToFind )>-1:
            return True
        else:
            return False
        
    # choose which kind of scale correction/MC smearing should be applied for electrons. Options are:
    if lookup (fileName,'Fall11'):    
        process.calibratedGsfElectrons.inputDataset = "Fall11"
    elif lookup(fileName, 'Summer11' ):
        process.calibratedGsfElectrons.inputDataset = "Summer11"
    elif lookup(fileName, 'Jan16ReReco' ):
        process.calibratedGsfElectrons.inputDataset = "Jan16ReReco"
    elif lookup(fileName, 'ReReco' ):
        process.calibratedGsfElectrons.inputDataset = "ReReco"
    elif lookup(fileName, 'Prompt' ):
        process.calibratedGsfElectrons.inputDataset = "Prompt"

