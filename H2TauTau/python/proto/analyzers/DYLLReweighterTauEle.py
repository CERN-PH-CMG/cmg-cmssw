from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.utils.DeltaR import matchObjectCollection

class DYLLReweighterTauEle( Analyzer ):
    '''Apply the reweighting calculated by Jose on the basis of the data/mc agreement
       in the inclusive sample, see here:
       https://indico.cern.ch/getFile.py/access?contribId=38&resId=0&materialId=slides&confId=212612
    '''

    def process(self, iEvent, event):
        event.isFake = 0
        if not self.cfg_comp.isMC:
            return True

        # do nothing in all cases, but the DY -> ll
        if event.isFake != 1:
            return True

        if self.cfg_ana.verbose:
            print 'DYLLReweighterTauEle VERBOSE'
            
        localweight = 1
        if event.diLepton.leg1().decayMode() == 0 :   # 1prong 0pi
            if abs (event.diLepton.leg1().eta()) < 1.5 : localweight = 0.82
            else                                       : localweight = 0.76 
        elif event.diLepton.leg1().decayMode() == 1 : # 1prong 1pi
            if abs (event.diLepton.leg1().eta()) < 1.5 : localweight = 1.65
            else                                       : localweight = 0.24 
        
        event.eventWeight = event.eventWeight * localweight
        return True
