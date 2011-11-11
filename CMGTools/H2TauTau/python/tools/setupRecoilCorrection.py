from CMGTools.Common.generator.metRecoilCorrection.recoilCorrectedMET_cfi import rootfile_dir

def setupRecoilCorrection( process, datasetName ):

    print 'setting up recoil corrections for dataset:'
    print datasetName
    
    if datasetName.startswith( '/DYJets' ):
        print '\tENABLED : Z->l tau mode (tau is true)'
        process.recoilCorMETTauMu.enable = True
        process.recoilCorMETTauMu.fileCorrectTo = '/'.join([rootfile_dir,
                                                           'recoilfit_wjets_njet.root'])
        process.recoilCorMETTauMu.leptonLeg = 2
    elif datasetName.startswith( '/WJetsToLNu' ):
        print '\tENABLED : W+jet mode (tau is fake)'
        process.recoilCorMETTauMu.enable = True
        process.recoilCorMETTauMu.fileCorrectTo = '/'.join([rootfile_dir,
                                                            'recoilfit_zjets_ltau_njet.root'])
        process.recoilCorMETTauMu.leptonLeg = 0
    else:
        print '\tDISABLED'
        process.metRecoilCorInputSequence.remove( process.genWorZ ) 
        process.recoilCorMETTauMu.enable = False
        
