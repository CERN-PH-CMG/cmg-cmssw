from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average

from CMGTools.H2TauTau.proto.TriggerEfficiency import TriggerEfficiency
from CMGTools.H2TauTau.proto.analyzers.RecEffCorrection import recEffMapEle, recEffMapMu


class LeptonWeighter( Analyzer ):
    '''Gets lepton efficiency weight and puts it in the event'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(LeptonWeighter,self).__init__(cfg_ana, cfg_comp, looperName)

        self.leptonName = self.cfg_ana.lepton
        # self.lepton = None
        self.weight = None
        # self.weightFactor = 1.
        self.trigEff = None
        if (self.cfg_comp.isMC or self.cfg_comp.isEmbed) and \
               not ( hasattr(self.cfg_ana,'disable') and self.cfg_ana.disable is True ):
                self.trigEff = TriggerEfficiency()
                self.trigEff.lepEff = getattr( self.trigEff,
                                               self.cfg_ana.effWeight )
                self.trigEff.lepEffMC = None
                if hasattr( self.cfg_ana, 'effWeightMC'):
                    self.trigEff.lepEffMC = getattr( self.trigEff,
                                                     self.cfg_ana.effWeightMC )

            
    def beginLoop(self):
        print self, self.__class__
        super(LeptonWeighter,self).beginLoop()
        self.averages.add('weight', Average('weight') )
        self.averages.add('triggerWeight', Average('triggerWeight') )
        self.averages.add('eff_data', Average('eff_data') )
        self.averages.add('eff_MC', Average('eff_MC') )
        self.averages.add('recEffWeight', Average('recEffWeight') )
        self.averages.add('idWeight', Average('idWeight') )
        self.averages.add('isoWeight', Average('isoWeight') )


    def process(self, iEvent, event):
        self.readCollections( iEvent )
        lep = getattr( event, self.leptonName )
        lep.weight = 1
        lep.triggerWeight = 1
        lep.triggerEffData = 1
        lep.triggerEffMC = 1 
        lep.recEffWeight = 1
        lep.idWeight = 1
        lep.isoWeight = 1 
        if (self.cfg_comp.isMC or self.cfg_comp.isEmbed) and \
           not ( hasattr(self.cfg_ana,'disable') and self.cfg_ana.disable is True ):
            assert( self.trigEff is not None )
            lep.triggerEffData = self.trigEff.lepEff( lep.pt(),
                                                      lep.eta() )
            lep.triggerWeight = lep.triggerEffData

            if self.trigEff.lepEffMC is not None and \
                   len(self.cfg_comp.triggers)>0:
                lep.triggerEffMC = self.trigEff.lepEffMC( lep.pt(),
                                                          lep.eta() )
                if lep.triggerEffMC>0:
                    lep.triggerWeight /= lep.triggerEffMC
                else:
                    lep.triggerWeight = 1.                    

            if hasattr( self.cfg_ana, 'idWeight'):
                # import pdb; pdb.set_trace()
                lep.idWeight = self.cfg_ana.idWeight.weight( pt=lep.pt(),
                                                             eta=abs(lep.eta()) ).weight.value
            if hasattr( self.cfg_ana, 'isoWeight'):
                lep.isoWeight = self.cfg_ana.isoWeight.weight( pt=lep.pt(),
                                                               eta=abs(lep.eta()) ).weight.value
            
##             recEffMap = None
##             if self.cfg_ana.recEffVersion == '2011':
##                 if abs(lep.pdgId()) == 11:
##                     recEffMap = recEffMapEle
##                 elif abs(lep.pdgId()) == 13:
##                     recEffMap = recEffMapMu
##                 elif abs(lep.pdgId()) == 15:
##                     pass
##                 else:
##                     raise ValueError('bad lepton pdgid: {pdgid}'.format(pdgid = lep.pdgId()))
            
##             if recEffMap is not None:
##                 lep.recEffWeight = recEffMap.effCor( lep.pt(), lep.eta())[0]
            
            
        lep.recEffWeight = lep.idWeight * lep.isoWeight
        lep.weight = lep.triggerWeight * lep.recEffWeight

        # import pdb; pdb.set_trace()
        event.eventWeight *= lep.weight
        self.averages['weight'].add( lep.weight )
        self.averages['triggerWeight'].add( lep.triggerWeight )
        self.averages['eff_data'].add( lep.triggerEffData )
        self.averages['eff_MC'].add( lep.triggerEffMC )
        self.averages['recEffWeight'].add( lep.recEffWeight )
        self.averages['idWeight'].add( lep.idWeight )
        self.averages['isoWeight'].add( lep.isoWeight )
                
