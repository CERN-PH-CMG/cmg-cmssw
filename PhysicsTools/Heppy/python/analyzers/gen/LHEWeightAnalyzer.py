from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
import PhysicsTools.HeppyCore.framework.config as cfg
from DataFormats.FWLite import Handle,Lumis
from ROOT.gen import WeightsInfo

class LHEWeightAnalyzer( Analyzer ):
    """Read the WeightsInfo objects of the LHE branch and store them
       in event.LHE_weights list.

       If the WeightsInfo.id is a string, replace it with an integer.

       So far the only allowed string formats are "mg_reweight_X",
       which gets stored as str(10000+int(X)),
       and "rwgt_X",
       which gets stored as str(20000+int(X)),

       If w.id is an unknown string or anything but a string or int,
       a RuntimeError is raised.
    """
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(LHEWeightAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.savePSweights = getattr(self.cfg_ana,"savePSweights",False)
        self.LHEWeightsNames=[]

    def declareHandles(self):
        super(LHEWeightAnalyzer, self).declareHandles()
        self.mchandles['LHEweights'] = AutoHandle('externalLHEProducer',
                                                  'LHEEventProduct',
                                                  mayFail=True,
                                                  fallbackLabel='source',
                                                  lazy=False )
        if self.cfg_ana.useLumiInfo:
            self.genLumiHandle = Handle("GenLumiInfoHeader")
            if self.savePSweights: raise RuntimeError, 'this combination of options in LHEWeightAnalyzer is not supported'

        if self.cfg_ana.useLumiInfo or self.savePSweights:
            self.mchandles['GenInfos'] = AutoHandle('generator',
                                                    'GenEventInfoProduct',
                                                    mayFail=True,
                                                    fallbackLabel='source',
                                                    lazy=False )

    def beginLoop(self, setup):
        super(LHEWeightAnalyzer,self).beginLoop(setup)
        
        if self.cfg_ana.useLumiInfo:
            lumis = Lumis(self.cfg_comp.files)
            for lumi in lumis:
                if lumi.getByLabel('generator',self.genLumiHandle):
                    weightNames = self.genLumiHandle.product().weightNames()
                    for wn in weightNames:  #direct cast is not working properly, copy of elements is needed
                        self.LHEWeightsNames.append(wn)
                    break

    def process(self, event):
        self.readCollections( event.input )
        
        # if not MC, nothing to do
        if not self.cfg_comp.isMC:
            return True

        # Add LHE weight info
        event.LHE_weights = []
        event.LHE_originalWeight = 1.0
        
        if self.mchandles['LHEweights'].isValid() and not self.cfg_ana.useLumiInfo:
            event.LHE_originalWeight = self.mchandles['LHEweights'].product().originalXWGTUP()

            for w in self.mchandles['LHEweights'].product().weights():
                # Check if id is string or int and convert to int if it's a string
                try:
                    int(w.id)
                    event.LHE_weights.append(w)
                except ValueError:
                    if not type(w.id) == str:
                        raise RuntimeError('Non int or string type for LHE weight id')

                    newweight = WeightsInfo()
                    newweight.wgt = w.wgt
                    if w.id.startswith('mg_reweight'):
                        newid = str(10000 + int(w.id.rsplit('_',1)[1]))
                        newweight.id = newid
                    elif w.id.startswith('rwgt'):
                        newid = str(20000 + int(w.id.rsplit('_',1)[1]))
                        newweight.id = newid

                    else: raise RuntimeError('Unknown string id in LHE weights')
                    event.LHE_weights.append(newweight)

            if self.savePSweights and self.mchandles['GenInfos'].isValid():
                for cnt,w in enumerate(self.mchandles['GenInfos'].product().weights()):
                    newweight = WeightsInfo()
                    newweight.id = str(30000+cnt)
                    newweight.wgt = w
                    event.LHE_weights.append(newweight)

        if self.cfg_ana.useLumiInfo and self.mchandles['GenInfos'].isValid() :          
            for cnt,w in enumerate(self.mchandles['GenInfos'].product().weights()[1:10]):
                weight= WeightsInfo()
                weight.wgt=w
                idstr=self.LHEWeightsNames[cnt+1].split(',')[1]
                weight.id=str(10000 + int(idstr[6:]) )
                event.LHE_weights.append(weight)
                
        return True

setattr(LHEWeightAnalyzer,"defaultConfig",
    cfg.Analyzer(LHEWeightAnalyzer,
                 useLumiInfo = False,
    )
)
