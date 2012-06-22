from CMGTools.Common.generator.metRecoilCorrection.recoilCorrectedMET_cfi import rootfile_dir

#TODO remove code duplication

def setupRecoilCorrection( process, runOnMC, enable=True, is52X=False):

    print 'setting up recoil corrections:'

    fileName = process.source.fileNames[0]
    print fileName

    def lookup( fileName, stringToFind ):
        '''predicate for identifying samples. could be more solid'''
        if fileName.find( stringToFind )>-1:
            return True
        else:
            return False

    if is52X:
        print 'picking up 52X recoil fits'
        if hasattr( process, 'recoilCorMETTauMu'):
            process.recoilCorMETTauMu.fileZmmData = rootfile_dir + 'recoilfit_datamm52X_njet.root'
            process.recoilCorMETTauMu.fileZmmMC = rootfile_dir + 'recoilfit_zmm52X_njet.root'
        if hasattr( process, 'recoilCorMETTauEle'):
            process.recoilCorMETTauEle.fileZmmData = rootfile_dir + 'recoilfit_datamm52X_njet.root'
            process.recoilCorMETTauEle.fileZmmMC = rootfile_dir + 'recoilfit_zmm52X_njet.root'
    
    #recoil correction does not handle WH_ZH_TTH_HToTauTau, because there are two bosons
    if enable: 
        if lookup( fileName, 'DYJets' ) or \
               lookup( fileName, 'GluGluToHToTauTau' ) or \
               lookup( fileName, 'VBF_HToTauTau' ):
            print '\tENABLED : Z->l tau mode (tau is true)'
            if hasattr( process, 'recoilCorMETTauMu'):
                process.recoilCorMETTauMu.enable = True
                process.recoilCorMETTauMu.fileCorrectTo = '/'.join([rootfile_dir,
                                                                    'recoilfit_zjets_ltau_njet.root'])
                process.recoilCorMETTauMu.leptonLeg = 0
            if hasattr( process, 'recoilCorMETTauEle'):
                process.recoilCorMETTauEle.enable = True
                process.recoilCorMETTauEle.fileCorrectTo = '/'.join([rootfile_dir,
                                                                    'recoilfit_zjets_ltau_njet.root'])
                process.recoilCorMETTauEle.leptonLeg = 0
            if hasattr( process, 'recoilCorMETMuEle'):
                process.recoilCorMETMuEle.enable = True
                process.recoilCorMETMuEle.fileCorrectTo = '/'.join([rootfile_dir,
                                                                    'recoilfit_zjets_ltau_njet.root'])
                process.recoilCorMETMuEle.leptonLeg = 0
        elif lookup( fileName, 'WJetsToLNu' ):
            print '\tENABLED : W+jet mode (tau is fake)'
            if hasattr( process, 'recoilCorMETTauMu'):
                process.recoilCorMETTauMu.enable = True
                process.recoilCorMETTauMu.fileCorrectTo = '/'.join([rootfile_dir,
                                                                    'recoilfit_wjets_njet.root'])
                process.recoilCorMETTauMu.leptonLeg = 2
            if hasattr( process, 'recoilCorMETTauEle'):
                process.recoilCorMETTauEle.enable = True
                process.recoilCorMETTauEle.fileCorrectTo = '/'.join([rootfile_dir,
                                                                    'recoilfit_wjets_njet.root'])
                process.recoilCorMETTauEle.leptonLeg = 2
            if hasattr( process, 'recoilCorMETMuEle'):
                process.recoilCorMETMuEle.enable = True
                process.recoilCorMETMuEle.fileCorrectTo = '/'.join([rootfile_dir,
                                                                    'recoilfit_wjets_njet.root'])
            process.recoilCorMETMuEle.leptonLeg = 2
        else:
            enable = False
    if enable is False:
        print '\tDISABLED'
        if runOnMC:
            process.metRecoilCorrectionInputSequence.remove( process.genWorZ ) 
        if hasattr( process, 'recoilCorMETTauMu'):
            process.recoilCorMETTauMu.enable = False            
        if hasattr( process, 'recoilCorMETTauEle'):
            process.recoilCorMETTauEle.enable = False
        if hasattr( process, 'recoilCorMETMuEle'):
            process.recoilCorMETMuEle.enable = False
            

