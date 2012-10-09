from CMGTools.H2TauTau.generator.metRecoilCorrection.recoilCorrectedMET_cfi import rootfile_dir
import copy

#TODO remove code duplication

def setupRecoilCorrection( process, runOnMC, enable=True, is52X=True):

    print 'setting up recoil corrections:'

    if not hasattr( process, 'recoilCorMETTauMu') and \
       not hasattr( process, 'recoilCorMETTauEle') :
        print 'recoil correction module not in the path -> DISABLED'
        return False
        
    fileName = process.source.fileNames[0]
    print fileName

    def lookup( fileName, stringToFind ):
        '''predicate for identifying samples. could be more solid'''
        if fileName.find( stringToFind )>-1:
            return True
        else:
            return False

    if is52X:
        print 'picking up 53X recoil fits'
        if hasattr( process, 'recoilCorMETTauMu'):
            process.recoilCorMETTauMu.fileZmmData = rootfile_dir + 'recoilfit_datamm53X_20pv_njet.root'
            process.recoilCorMETTauMu.fileZmmMC = rootfile_dir + 'recoilfit_zmm53X_20pv_njet.root'
        if hasattr( process, 'recoilCorMETTauEle'):
            process.recoilCorMETTauEle.fileZmmData = rootfile_dir + 'recoilfit_datamm53X_20pv_njet.root'
            process.recoilCorMETTauEle.fileZmmMC = rootfile_dir + 'recoilfit_zmm53X_20pv_njet.root'
    else:
        pass
        #the default in the cfg file will have to point to the 2011 data things
    #recoil correction does not handle WH_ZH_TTH_HToTauTau, because there are two bosons
    if enable: 
        correctFileName = None
        if lookup( fileName, 'DYJets' ) :
            print '\tENABLED : Z->l tau mode (tau is true)'
            if is52X :
                correctFileName = '/'.join([rootfile_dir,'recoilfit_zmm53X_20pv_njet.root'])
            else :
                correctFileName = '/'.join([rootfile_dir,'recoilfit_zjets_ltau_njet.root'])
            if hasattr( process, 'recoilCorMETTauMu'):
                process.recoilCorMETTauMu.enable = True
                process.recoilCorMETTauMu.fileCorrectTo = correctFileName
                process.recoilCorMETTauMu.leptonLeg = 0
            if hasattr( process, 'recoilCorMETTauEle'):
                process.recoilCorMETTauEle.enable = True
                process.recoilCorMETTauEle.fileCorrectTo = correctFileName
                process.recoilCorMETTauEle.leptonLeg = 0
##             if hasattr( process, 'recoilCorMETMuEle'):
##                 process.recoilCorMETMuEle.enable = True
##                 process.recoilCorMETMuEle.fileCorrectTo = correctFileName
##                 process.recoilCorMETMuEle.leptonLeg = 0
        if lookup( fileName, 'GluGluToHToTauTau' ) or \
               lookup( fileName, 'VBF_HToTauTau' ):
            print '\tENABLED : Higgs mode (tau is true)'
            if is52X : 
                correctFileName = '/'.join([rootfile_dir,'recoilfit_higgs53X_20pv_njet.root'])
#                print 'DEBUG',correctFileName
            else :
                correctFileName = '/'.join([rootfile_dir,'recoilfit_zjets_ltau_njet.root'])
            if hasattr( process, 'recoilCorMETTauMu'):
                process.recoilCorMETTauMu.enable = True
                process.recoilCorMETTauMu.fileCorrectTo = correctFileName
                process.recoilCorMETTauMu.leptonLeg = 0
            if hasattr( process, 'recoilCorMETTauEle'):
                process.recoilCorMETTauEle.enable = True
                process.recoilCorMETTauEle.fileCorrectTo = correctFileName
                process.recoilCorMETTauEle.leptonLeg = 0
##             if hasattr( process, 'recoilCorMETMuEle'):
##                 process.recoilCorMETMuEle.enable = True
##                 process.recoilCorMETMuEle.fileCorrectTo = correctFileName
##                 process.recoilCorMETMuEle.leptonLeg = 0
        elif lookup( fileName, 'WJetsToLNu' ) or \
                 lookup( fileName, 'W1Jets' ) or \
                 lookup( fileName, 'W2Jets' ) or \
                 lookup( fileName, 'W3Jets' ) or \
                 lookup( fileName, 'W4Jets' ):
            print '\tENABLED : W+jet mode (tau is fake)'
            if is52X : 
                correctFileName = '/'.join([rootfile_dir,'recoilfit_wjets53X_20pv_njet.root'])
            else :
                correctFileName = '/'.join([rootfile_dir,'recoilfit_wjets_njet.root'])
            if hasattr( process, 'recoilCorMETTauMu'):
                process.recoilCorMETTauMu.enable = True
                process.recoilCorMETTauMu.fileCorrectTo = correctFileName
                process.recoilCorMETTauMu.leptonLeg = 2
            if hasattr( process, 'recoilCorMETTauEle'):
                process.recoilCorMETTauEle.enable = True
                process.recoilCorMETTauEle.fileCorrectTo = correctFileName
                process.recoilCorMETTauEle.leptonLeg = 2
##             if hasattr( process, 'recoilCorMETMuEle'):
##                 process.recoilCorMETMuEle.enable = True
##                 process.recoilCorMETMuEle.fileCorrectTo = correctFileName
##                 process.recoilCorMETMuEle.leptonLeg = 2
        else:
            enable = False
    if enable is False:
        print '\tDISABLED'
        if runOnMC:
#            process.metRecoilCorrectionInputSequence2012.remove( process.genWorZ ) 
            process.metRecoilCorrectionInputSequence.remove( process.genWorZ ) 
        if hasattr( process, 'recoilCorMETTauMu'):
            process.recoilCorMETTauMu.enable = False
        if hasattr( process, 'recoilCorMETTauEle'):
            process.recoilCorMETTauEle.enable = False
##         if hasattr( process, 'recoilCorMETMuEle'):
##             process.recoilCorMETMuEle.enable = False
            

