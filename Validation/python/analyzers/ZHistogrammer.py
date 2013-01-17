from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from ROOT import TH1F, TFile


class ZHistogrammer( Analyzer ):

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(ZHistogrammer, self).__init__(cfg_ana, cfg_comp, looperName)
        
        self.file = TFile('/'.join([self.dirName,
                                    'histograms.root']), 'recreate' )
        self.mass = TH1F('mass', 'mass', 100,0,200)

    def process(self, iEvent, event):
        # import pdb; pdb.set_trace()
        self.mass.Fill( event.diLepton.mass() )
        return True

    def write(self):
        super(ZHistogrammer, self).write()
        self.file.Write()
        self.file.Close()
