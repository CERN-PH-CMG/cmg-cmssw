from CMGTools.Common.generator.metRecoilCorrection.recoilCorrectedMET_cfi import rootfile_dir

def setupRecoilCorrection( process ):

    print 'setting up recoil corrections:'

    fileName = process.source.fileNames[0]
    print fileName

    def lookup( fileName, stringToFind ):
        '''predicate for identifying samples. could be more solid'''
        if fileName.find( stringToFind )>-1:
            return True
        else:
            return False
    
    if lookup( fileName, '/DYJets' ):
        print '\tENABLED : Z->l tau mode (tau is true)'
        process.recoilCorMETTauMu.enable = True
        process.recoilCorMETTauMu.fileCorrectTo = '/'.join([rootfile_dir,
                                                            'recoilfit_zjets_ltau_njet.root'])
        process.recoilCorMETTauMu.leptonLeg = 0
    elif lookup( fileName, '/WJetsToLNu' ):
        print '\tENABLED : W+jet mode (tau is fake)'
        process.recoilCorMETTauMu.enable = True
        process.recoilCorMETTauMu.fileCorrectTo = '/'.join([rootfile_dir,
                                                           'recoilfit_wjets_njet.root'])
        process.recoilCorMETTauMu.leptonLeg = 2
    else:
        print '\tDISABLED'
        process.metRecoilCorInputSequence.remove( process.genWorZ ) 
        process.recoilCorMETTauMu.enable = False
        

