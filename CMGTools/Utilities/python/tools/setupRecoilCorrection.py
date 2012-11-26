# from CMGTools.H2TauTau.generator.metRecoilCorrection.recoilCorrectedMET_cfi import rootfile_dir
from CMGTools.Utilities.generator.metRecoilCorrection.recoilCorrectedMET_cfi import rootfile_dir
import copy

def lookup( fileName, stringToFind ):
    '''predicate for identifying samples. could be more solid'''
    if fileName.find( stringToFind )>-1:
        return True
    else:
        return False

def fileAndLeg(fileName, is53X, mode=None):
    correctFileName = None
    leptonLeg = None
    if mode:
        # forcing a mode
        fileName = mode
    if lookup( fileName, 'DYJets' ) :
        print '\tENABLED : Z->l tau mode (tau is true)'
        if is53X :
            correctFileName = '/'.join([rootfile_dir,'recoilfit_zmm53X_20pv_njet.root'])
        else :
            correctFileName = '/'.join([rootfile_dir,'recoilfit_zmm42X_20pv_njet.root'])
        leptonLeg = 0
    if lookup( fileName, 'GluGluToHToTauTau' ) or \
           lookup( fileName, 'VBF_HToTauTau' ):
        print '\tENABLED : Higgs mode (tau is true)'
        if is53X : 
            correctFileName = '/'.join([rootfile_dir,'recoilfit_higgs53X_20pv_njet.root'])
        else :
            correctFileName = '/'.join([rootfile_dir,'recoilfit_higgs42X_20pv_njet.root'])
        leptonLeg = 0
    elif lookup( fileName, 'WJetsToLNu' ) or \
             lookup( fileName, 'W1Jet' ) or \
             lookup( fileName, 'W2Jets' ) or \
             lookup( fileName, 'W3Jets' ) or \
             lookup( fileName, 'W4Jets' ):
        print '\tENABLED : W+jet mode (tau is fake)'
        if is53X : 
            correctFileName = '/'.join([rootfile_dir,'recoilfit_wjets53X_20pv_njet.root'])
        else :
            correctFileName = '/'.join([rootfile_dir,'recoilfit_wjets42X_20pv_njet.root'])
        leptonLeg = 2            
    else:
        pass
    if correctFileName:
        print '\tCorrecting to:', correctFileName
        print '\tLeg number   :',leptonLeg
    return correctFileName, leptonLeg



def basicParameters(is53X):
    fileZmmData = None
    fileZmmMC = None
    correctionType = None
    if is53X:
        print 'picking up 53X recoil fits'
        fileZmmData = rootfile_dir + 'recoilfit_datamm53X_20pv_njet.root'
        fileZmmMC = rootfile_dir + 'recoilfit_zmm53X_20pv_njet.root'
        correctionType = 2
    else:
        print 'picking up 44X recoil fits'
        fileZmmData = rootfile_dir + 'recoilfit_datamm42X_20pv_njet.root'
        fileZmmMC = rootfile_dir + 'recoilfit_zmm42X_20pv_njet.root'        
        correctionType = 1
    print '\tZmm data:',fileZmmData
    print '\tZmm MC  :',fileZmmMC
    print '\ttype    :',correctionType
    return fileZmmData, fileZmmMC, correctionType


def setupRecoilCorrection( process, runOnMC, enable=True, is53X=True, mode=None):

    print 'setting up recoil corrections:'
    if not runOnMC:
        enable=False

    if not hasattr( process, 'recoilCorMETTauMu') and \
       not hasattr( process, 'recoilCorMETTauEle') :
        print 'recoil correction module not in the path -> DISABLED'
        return False
        
    fileName = process.source.fileNames[0]
    print fileName

    if mode:
        print 'FORCING TO', mode

    fileZmmData, fileZmmMC, correctionType = basicParameters(is53X)

    correctFileName = None
    leptonLeg = None
    if enable:
        correctFileName, leptonLeg = fileAndLeg(fileName, is53X, mode)
        if correctFileName is None:
            enable = False
    if enable:
        if hasattr( process, 'recoilCorMETTauMu'):
            if mode:
                process.recoilCorMETTauMu.force = True
            process.recoilCorMETTauMu.enable = True
            process.recoilCorMETTauMu.fileCorrectTo = correctFileName
            process.recoilCorMETTauMu.leptonLeg = leptonLeg
            process.recoilCorMETTauMu.fileZmmData = fileZmmData
            process.recoilCorMETTauMu.fileZmmMC = fileZmmMC
            process.recoilCorMETTauMu.correctionType = correctionType
        if hasattr( process, 'recoilCorMETTauEle'):
            if mode:
                process.recoilCorMETTauEle.force = True
            process.recoilCorMETTauEle.enable = True
            process.recoilCorMETTauEle.fileCorrectTo = correctFileName
            process.recoilCorMETTauEle.leptonLeg = leptonLeg 
            process.recoilCorMETTauEle.fileZmmData = fileZmmData
            process.recoilCorMETTauEle.fileZmmMC = fileZmmMC
            process.recoilCorMETTauEle.correctionType = correctionType
    else:
        print '\tDISABLED'
        if runOnMC:
            process.metRecoilCorrectionInputSequence.remove( process.genWorZ ) 
        if hasattr( process, 'recoilCorMETTauMu'):
            process.recoilCorMETTauMu.enable = False
        if hasattr( process, 'recoilCorMETTauEle'):
            process.recoilCorMETTauEle.enable = False
            

if __name__ == '__main__':

    import sys
    from CMGTools.Common.Tools.cmsswRelease import isNewerThan

    data, mc, type = basicParameters(isNewerThan('CMSSW_5_2_X'))
    
    for line in sys.stdin:
        print 
        line = line.rstrip()
        print line
        print fileAndLeg(line, True)
