from CMGTools.RootTools.fwlite.Analyzer import Analyzer


class Skipper( Analyzer ):
    '''Insert that at the beginning of your sequence to skip events you do not want. 
    '''

    def process(self, iEvent, event):
        run = iEvent.eventAuxiliary().id().run()
        lumi = iEvent.eventAuxiliary().id().luminosityBlock()
        eId = iEvent.eventAuxiliary().id().event()
        if eId == 755879:
            # raise ValueError('found')
            return True 
        else:
            return False
