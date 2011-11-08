import os
from fnmatch import fnmatch

from ROOT import kPink

from CMGTools.RootTools.DataMC.AnalysisDataMCPlot import AnalysisDataMC
from CMGTools.H2TauTau.macros.Weight import Weight

class H2TauTauDataMC( AnalysisDataMC ):

    def __init__(self, histName, directory, filePattern, weights):
        self.filePattern = filePattern
        super(H2TauTauDataMC, self).__init__(histName, directory, weights)

    def _GetFileNames(self, directory):
        fileNames = []
        for root,dirs,files in os.walk(directory):
            if root is directory:
                continue
            # print root, dirs, files
            matchingFiles = [file for file in files if fnmatch(file, self.filePattern)]
            # print matchingFiles
            if len(matchingFiles)!=1:
                raise ValueError('files matching %s in %s: %s. Need to match only 1 file.'
                                 % (self.filePattern,
                                    root,
                                    matchingFiles))
            else:
                fileNames.append( '/'.join([root, matchingFiles[0]])) 
        return fileNames

    def _ComponentName(self, name):
        # print name 
        return name.split('/')[-2]

    
def scaleWJets( plot ):
    wjet = copy.deepcopy(plot.Hist('PromptReco-v4'))
    wjet.Add(plot.Hist('DYJets'), -1)
    wjet.Add(plot.Hist('TTJets'), -1)

    plot.AddHistogram( 'Data - DY - TT', wjet.weighted, 1010)
    plot.Hist('Data - DY - TT').stack = False

    pink = kPink+7
    sPinkHollow = Style( lineColor=pink, markerColor=pink, markerStyle=4)
    plot.Hist('Data - DY - TT').SetStyle( sPinkHollow )

    mtmin, mtmax = 50, 200
    scale_WJets = plot.Hist('Data - DY - TT').Integral(True, mtmin, mtmax) / plot.Hist('WJets').Integral(True, mtmin, mtmax)
    plot.Hist('WJets').Scale(scale_WJets)

    plot.Hist('Data - DY - TT').on = False
    

if __name__ == '__main__':

    from CMGTools.RootTools.RootInit import *
    from CMGTools.H2TauTau.macros.AnalysisConfig import AnalysisConfig
    import copy

    cfgFile = 'Colin/analysis.cfg'
    anacfg = AnalysisConfig( cfgFile )
    intLumi = float(anacfg.components['PromptReco-v4'].lumi)
    weights = dict( [ (key,comp.GetWeight()) for key, comp in anacfg.components.iteritems()] )
    for weight in weights.values():
        weight.intLumi = intLumi

    
    mtSS = H2TauTauDataMC('tauMu/tauMu_h_mT', 'Ana_Nov8', 'SS.root', weights)
    mtSS.Hist('PromptReco-v4').stack = False

    scaleWJets( mtSS )

    # saving this guy for later
    mtSSWithQCD = copy.deepcopy(mtSS)

    qcd = copy.deepcopy(mtSSWithQCD.Hist('PromptReco-v4'))
    qcd.Add(mtSSWithQCD.Hist('DYJets'), -1)
    qcd.Add(mtSSWithQCD.Hist('TTJets'), -1)
    qcd.Add(mtSSWithQCD.Hist('WJets'), -1)
    
    mtSSWithQCD.AddHistogram( 'QCD', qcd.weighted, 1020)
    mtSSWithQCD.Hist('QCD').stack = True

    mtSSWithQCD.Hist('QCD').SetStyle( sGreen )

    # mtSSWithQCD.DrawStack('HIST')

    mtOS = H2TauTauDataMC('tauMu/tauMu_h_mT', 'Ana_Nov8', 'OS.root', weights)
    mtOS.Hist('PromptReco-v4').stack = False

    scaleWJets( mtOS )

    qcdOS = copy.deepcopy( mtSSWithQCD.Hist('QCD') )
    qcdOS.Scale( 1.06 )
    mtOS.AddHistogram('QCD', qcdOS.weighted, 1030)
    mtOS.Hist('QCD').layer=2.5
    mtOS.DrawStack('HIST')
    
