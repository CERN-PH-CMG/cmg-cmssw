from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.statistics.average import Average

from CMGTools.H2TauTau.proto.TriggerEfficiency import TriggerEfficiency


class JetWeighter( Analyzer ):
    '''Gets jet1 efficiency weight and puts it in the event'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(JetWeighter,self).__init__(cfg_ana, cfg_comp, looperName)

        self.jet1 = None
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
            
            
    def beginLoop(self, setup):
        print self, self.__class__
        super(JetWeighter,self).beginLoop(setup)
        self.averages.add('weight', Average('weight') )
        self.averages.add('triggerWeight', Average('triggerWeight') )
        self.averages.add('eff_data', Average('eff_data') )
        self.averages.add('eff_MC', Average('eff_MC') )
        self.averages.add('recEffWeight', Average('recEffWeight') )


    def process(self, event):
        self.readCollections( event.input )
	if len(event.cleanJets)<1: return
	#import pdb ; pdb.set_trace()
        self.jet1 = event.cleanJets[0]
        self.jet1.weight = 1
        self.jet1.triggerWeight = 1
        self.jet1.triggerEffData = 1
        self.jet1.triggerEffMC = 1 
        if self.cfg_comp.isMC or self.cfg_comp.isEmbed:
            assert( self.trigEff is not None )
            #import pdb ; pdb.set_trace()
            self.jet1.triggerEffData = self.trigEff.lepEff( self.jet1.pt(), self.jet1.eta() )
            self.jet1.triggerWeight = self.jet1.triggerEffData

            #import pdb ; pdb.set_trace()
            if self.trigEff.lepEffMC is not None and len(self.cfg_comp.triggers)>0:
                self.jet1.triggerEffMC = self.trigEff.lepEffMC( self.jet1.pt(), self.jet1.eta() )
                if self.jet1.triggerEffMC>0:
                    self.jet1.triggerWeight /= self.jet1.triggerEffMC
                else:
                    self.jet1.triggerWeight = 1.                    

        #import pdb ; pdb.set_trace()

        self.jet1.weight = self.jet1.triggerWeight
        
##        varName = '_'.join([self.jet1Name, 'eff'])
##         setattr( event, varName, self.jet1.triggerEffData )

##         varName = '_'.join([self.jet1Name, 'effMC'])
##         setattr( event, varName, self.triggerEffMC )

##         varName = '_'.join([self.jet1Name, 'triggerWeight'])
##         setattr( event, varName, self.jet1.triggerWeight )

##         varName = '_'.join([self.jet1Name, 'weight'])
##         setattr( event, varName, self.jet1.weight )


######## commented for now, need to change the variables names
#         event.eventWeight *= self.jet1.weight
# 	if not hasattr(event,"triggerWeight") : event.triggerWeight  = 1.0
# 	event.triggerWeight  *= self.jet1.triggerWeight
# 	if not hasattr(event,"triggerEffMC")  : event.triggerEffMC   = 1.0
#         event.triggerEffMC   *= self.jet1.triggerEffMC
# 	if not hasattr(event,"triggerEffData"): event.triggerEffData = 1.0
#         event.triggerEffData *= self.jet1.triggerEffData


        event.eventWeight *= self.jet1.weight
	if not hasattr(event,"triggerWeight_diTauJet") : event.triggerWeight_diTauJet  = 1.0
	event.triggerWeight_diTauJet  *= self.jet1.triggerWeight
	if not hasattr(event,"triggerEffMC_diTauJet")  : event.triggerEffMC_diTauJet   = 1.0
        event.triggerEffMC_diTauJet   *= self.jet1.triggerEffMC
	if not hasattr(event,"triggerEffData_diTauJet"): event.triggerEffData_diTauJet = 1.0
        event.triggerEffData_diTauJet *= self.jet1.triggerEffData



        self.averages['weight'].add( self.jet1.weight )
        self.averages['triggerWeight'].add( self.jet1.triggerWeight )
        self.averages['eff_data'].add( self.jet1.triggerEffData )
        self.averages['eff_MC'].add( self.jet1.triggerEffMC )
                
