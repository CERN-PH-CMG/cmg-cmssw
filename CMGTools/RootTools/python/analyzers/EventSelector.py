from CMGTools.RootTools.fwlite.Analyzer import Analyzer


class EventSelector( Analyzer ):
    '''Insert that at the beginning of your sequence to skip events you do not want. 
    '''

    def process(self, iEvent, event):
        run = iEvent.eventAuxiliary().id().run()
        lumi = iEvent.eventAuxiliary().id().luminosityBlock()
        eId = iEvent.eventAuxiliary().id().event()
        if eId in self.cfg_ana.toSelect:
            # raise ValueError('found')
            print 'Selecting', run, lumi, eId
            return True 
        else:
            return False
