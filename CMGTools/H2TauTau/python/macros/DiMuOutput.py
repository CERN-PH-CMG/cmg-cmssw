from CMGTools.H2TauTau.macros.Output import Output 
from CMGTools.H2TauTau.macros.H2TauTauHistogramList import H2TauTauHistogramList

class DiMuOutput( Output ):

    def __init__(self, name):
        super(DiMuOutput, self).__init__(name)
        self.histoLists['hists'] = H2TauTauHistogramList( '/'.join( [self.name, 'hists'] ) )  

    def Fill(self, event):
        self.histoLists['hists'].Fill( event, event.eventWeight )
