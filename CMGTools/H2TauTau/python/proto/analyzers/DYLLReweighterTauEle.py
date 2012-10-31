from CMGTools.RootTools.fwlite.Analyzer import Analyzer

class DYLLReweighterTauEle( Analyzer ):
    '''Apply the reweighting calculated by Jose on the basis of the data/mc agreement
       in the inclusive sample, see here:
       https://indico.cern.ch/getFile.py/access?contribId=38&resId=0&materialId=slides&confId=212612
    '''

    def process(self, iEvent, event):
        if not self.cfg_comp.isMC:
            return True

        # do nothing in all cases, but the DY -> ll
        if event.isFake != 1 or self.cfg_comp.name != 'DYJets':
            return True

        if self.cfg_ana.verbose:
            print 'DYLLReweighterTauEle processing',self.cfg_comp.name,
            
        localweight = 1
        if event.diLepton.leg1().decayMode() == 0 :   # 1prong 0pi
            if abs (event.diLepton.leg1().eta()) < 1.5 : localweight = self.cfg_ana.W1p0PB #0.82
            else                                       : localweight = self.cfg_ana.W1p0PE #0.76 
        elif event.diLepton.leg1().decayMode() == 1 : # 1prong 1pi
            if abs (event.diLepton.leg1().eta()) < 1.5 : localweight = self.cfg_ana.W1p1PB #1.65
            else                                       : localweight = self.cfg_ana.W1p1PE #0.24 

        if self.cfg_ana.verbose:
            print 'DYLLReweighterTauEle',event.diLepton.leg1().decayMode(),event.diLepton.leg1().eta(),localweight
        
        event.eventWeight = event.eventWeight * localweight
        return True

# FIXME read from cfg file the scaling factors
