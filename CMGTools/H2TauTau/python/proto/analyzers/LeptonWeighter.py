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
        self.lepton = None
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


    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.lepton = getattr( event, self.leptonName )
        self.lepton.weight = 1
        self.lepton.triggerWeight = 1
        self.lepton.triggerEffData = 1
        self.lepton.triggerEffMC = 1 
        self.lepton.recEffWeight = 1
        if (self.cfg_comp.isMC or self.cfg_comp.isEmbed) and \
           not ( hasattr(self.cfg_ana,'disable') and self.cfg_ana.disable is True ):
            assert( self.trigEff is not None )
            self.lepton.triggerEffData = self.trigEff.lepEff( self.lepton.pt(),
                                                              self.lepton.eta() )
            self.lepton.triggerWeight = self.lepton.triggerEffData

            if self.trigEff.lepEffMC is not None and \
                   len(self.cfg_comp.triggers)>0:
                self.lepton.triggerEffMC = self.trigEff.lepEffMC( self.lepton.pt(),
                                                                  self.lepton.eta() )
                if self.lepton.triggerEffMC>0:
                    self.lepton.triggerWeight /= self.lepton.triggerEffMC
                else:
                    self.lepton.triggerWeight = 1.                    

            recEffMap = None
            if self.cfg_ana.recEffVersion == '2011':
                if abs(self.lepton.pdgId()) == 11:
                    recEffMap = recEffMapEle
                elif abs(self.lepton.pdgId()) == 13:
                    recEffMap = recEffMapMu
                elif abs(self.lepton.pdgId()) == 15:
                    pass
                else:
                    raise ValueError('bad lepton pdgid: {pdgid}'.format(pdgid = self.lepton.pdgId()))
            
            if recEffMap is not None:
                self.lepton.recEffWeight = recEffMap.effCor( self.lepton.pt(), self.lepton.eta())[0]
            

        self.lepton.weight = self.lepton.triggerWeight * self.lepton.recEffWeight
        
        event.eventWeight *= self.lepton.weight
        self.averages['weight'].add( self.lepton.weight )
        self.averages['triggerWeight'].add( self.lepton.triggerWeight )
        self.averages['eff_data'].add( self.lepton.triggerEffData )
        self.averages['eff_MC'].add( self.lepton.triggerEffMC )
        self.averages['recEffWeight'].add( self.lepton.recEffWeight )
                
