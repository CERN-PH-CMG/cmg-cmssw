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
        if self.cfg_comp.isMC or self.cfg_comp.isEmbed:
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
        self.eff = 1
        self.effMC = 1 
        self.weight = 1
        self.triggerWeight = 1
        self.recEffWeight = 1
        if self.cfg_comp.isMC or self.cfg_comp.isEmbed:
            
            self.lepton = getattr( event, self.leptonName )

            if self.trigEff is not None:
                self.eff = self.trigEff.lepEff( self.lepton.pt(),
                                                self.lepton.eta() )
                self.triggerWeight = self.eff
                if self.trigEff.lepEffMC is not None:
                    self.effMC = self.trigEff.lepEffMC( self.lepton.pt(),
                                                        self.lepton.eta() )
                    self.triggerWeight /= self.effMC

            recEffMap = None
            if abs(self.lepton.pdgId()) == 11:
                recEffMap = recEffMapEle
            elif abs(self.lepton.pdgId()) == 13:
                recEffMap = recEffMapMu
            elif abs(self.lepton.pdgId()) == 15:
                pass
            else:
                raise ValueError('bad lepton pdgid: {pdgid}'.format(pdgid = self.lepton.pdgId()))
            if recEffMap is not None:
                self.recEffWeight = recEffMap.effCor( self.lepton.pt(), self.lepton.eta())[0]
            

        self.weight = self.triggerWeight * self.recEffWeight
        
        varName = '_'.join([self.leptonName, 'eff'])
        setattr( event, varName, self.eff )

        varName = '_'.join([self.leptonName, 'effMC'])
        setattr( event, varName, self.effMC )

        varName = '_'.join([self.leptonName, 'triggerWeight'])
        setattr( event, varName, self.triggerWeight )

        varName = '_'.join([self.leptonName, 'recEffWeight'])
        setattr( event, varName, self.recEffWeight )

        varName = '_'.join([self.leptonName, 'weight'])
        setattr( event, varName, self.weight )

        event.eventWeight *= self.weight
        self.averages['weight'].add( self.weight )
        self.averages['triggerWeight'].add( self.triggerWeight )
        self.averages['eff_data'].add( self.eff )
        self.averages['eff_MC'].add( self.effMC )
        self.averages['recEffWeight'].add( self.recEffWeight )
                
