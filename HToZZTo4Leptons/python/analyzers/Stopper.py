from CMGTools.RootTools.fwlite.Analyzer import Analyzer

class Stopper( Analyzer ):

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        #         if event.iEv == 10:
        #            raise UserWarning('stopper')

        if not hasattr( event.GenGenFourLeptonAnalyzer, 'allSelLeptons'):
            return True
        if not hasattr( event.MuEleFourLeptonAnalyzer, 'allSelLeptons'):
            return True
        
        if event.GenGenFourLeptonAnalyzer.allSelLeptons < event.MuEleFourLeptonAnalyzer.allSelLeptons:
            raise UserWarning('diff in number of sel leptons')
