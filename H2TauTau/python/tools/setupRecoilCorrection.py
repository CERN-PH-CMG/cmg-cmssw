from CMGTools.H2TauTau.generator.metRecoilCorrection.recoilCorrectedMET_cfi import rootfile_dir
import copy

def lookup( fileName, stringToFind ):
    '''predicate for identifying samples. could be more solid'''
    if fileName.find( stringToFind )>-1:
        return True
    else:
        return False

def fileAndLeg(fileName, is53X):
    correctFileName = None
    leptonLeg = None
    if lookup( fileName, 'DYJets' ) :
        print '\tENABLED : Z->l tau mode (tau is true)'
        if is53X :
            correctFileName = '/'.join([rootfile_dir,'recoilfit_zmm53X_20pv_njet.root'])
        else :
            correctFileName = '/'.join([rootfile_dir,'recoilfit_zjets_ltau_njet.root'])
        leptonLeg = 0
    if lookup( fileName, 'GluGluToHToTauTau' ) or \
           lookup( fileName, 'VBF_HToTauTau' ):
        print '\tENABLED : Higgs mode (tau is true)'
        if is53X : 
            correctFileName = '/'.join([rootfile_dir,'recoilfit_higgs53X_20pv_njet.root'])
        else :
            correctFileName = '/'.join([rootfile_dir,'recoilfit_zjets_ltau_njet.root'])
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
            correctFileName = '/'.join([rootfile_dir,'recoilfit_wjets_njet.root'])
        leptonLeg = 2            
    else:
        pass
    if correctFileName:
        print '\tCorrecting to:', correctFileName
        print '\tLeg number   :',leptonLeg
    return correctFileName, leptonLeg


def setupRecoilCorrection( process, runOnMC, enable=True, is53X=True):

    print 'setting up recoil corrections:'

    if not hasattr( process, 'recoilCorMETTauMu') and \
       not hasattr( process, 'recoilCorMETTauEle') :
        print 'recoil correction module not in the path -> DISABLED'
        return False
        
    fileName = process.source.fileNames[0]
    print fileName

    if is53X:
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
    # the 2 parameters below depend on the type of events being processed:
    correctFileName = None
    leptonLeg = None
    if enable:
        correctFileName, leptonLeg = fileAndLeg(fileName, is53X)
        if correctFileName is None:
            enable = False
    if enable:
        if hasattr( process, 'recoilCorMETTauMu'):
            process.recoilCorMETTauMu.enable = True
            process.recoilCorMETTauMu.fileCorrectTo = correctFileName
            process.recoilCorMETTauMu.leptonLeg = leptonLeg
        if hasattr( process, 'recoilCorMETTauEle'):
            process.recoilCorMETTauEle.enable = True
            process.recoilCorMETTauEle.fileCorrectTo = correctFileName
            process.recoilCorMETTauEle.leptonLeg = leptonLeg 
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
    for line in sys.stdin:
        print 
        line = line.rstrip()
        print line
        print fileAndLeg(line, True)
