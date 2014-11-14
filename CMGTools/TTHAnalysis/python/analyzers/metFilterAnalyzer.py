from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Counter import Counter
from CMGTools.RootTools.utils.TriggerList import TriggerList
from CMGTools.RootTools.utils.TriggerMatching import selTriggerObjects
from CMGTools.RootTools.physicsobjects.PhysicsObjects import TriggerObject
from ROOT import TriggerBitChecker


class metFilterAnalyzer ( Analyzer ):
    '''Access to trigger information, and trigger selection'''

    def declareHandles(self):
        super(metFilterAnalyzer, self).declareHandles()

        self.handles['metFilterResults'] =  AutoHandle( ('TriggerResults','','PAT'), 'edm::TriggerResults' )
 
        self.handles['cmgTriggerObjectListSel'] =  AutoHandle(
            'cmgTriggerObjectListSel',
            'std::vector<cmg::TriggerObject>'
            )
 
    def beginLoop(self):
        super(metFilterAnalyzer,self).beginLoop()
      
       
    def process(self, iEvent, event):
        self.readCollections( iEvent )
       
        event.METflags = self.handles['metFilterResults'].product()
       
        event.checkerEcalDead = TriggerBitChecker('EcalDeadCellTriggerPrimitiveFilterPath')
        event.checkEcalDead = event.checkerEcalDead.check(iEvent.object(),event.METflags)
        event.checkerhcalLaser = TriggerBitChecker('hcalLaserEventFilterPath')
        event.checkhcalLaser = event.checkerhcalLaser.check(iEvent.object(),event.METflags)
        event.checkertrackingFailure= TriggerBitChecker('trackingFailureFilterPath')
        event.checktrackingFailure= event.checkertrackingFailure.check(iEvent.object(),event.METflags)
        event.checkerprimaryVertexFilterPath= TriggerBitChecker('primaryVertexFilterPath')
        event.checkprimaryVertex= event.checkerprimaryVertexFilterPath.check(iEvent.object(),event.METflags)
        event.checkernoscrapingFilterPath = TriggerBitChecker('noscrapingFilterPath')
        event.checknoscraping= event.checkernoscrapingFilterPath.check(iEvent.object(),event.METflags)
        event.checkertrackIsolationMakerFilterPath = TriggerBitChecker('trackIsolationMakerFilterPath')
        event.checktrackIsolationMaker=event.checkertrackIsolationMakerFilterPath.check(iEvent.object(),event.METflags)
        event.checkermetNoiseCleaningPath = TriggerBitChecker('metNoiseCleaningPath')
        event.checkmetNoiseCleaning=event.checkermetNoiseCleaningPath.check(iEvent.object(),event.METflags)
        event.checkereeBadScFilterPath = TriggerBitChecker('eeBadScFilterPath')
        event.checkeeBadSc=event.checkereeBadScFilterPath.check(iEvent.object(),event.METflags)
        event.checkerecalLaserFilterPath = TriggerBitChecker('ecalLaserFilterPath')
        event.checkecalLaser=event.checkerecalLaserFilterPath.check(iEvent.object(),event.METflags)
        event.checkertotalKinematicsFilterPath = TriggerBitChecker('totalKinematicsFilterPath')
        event.checktotalKinematics= event.checkertotalKinematicsFilterPath.check(iEvent.object(),event.METflags)
        event.checkerCSCTightHaloFilterPath = TriggerBitChecker('CSCTightHaloFilterPath')
        event.checkCSCTightHalo= event.checkerCSCTightHaloFilterPath.check(iEvent.object(),event.METflags)
        event.checkerHBHENoiseFilterPath = TriggerBitChecker('HBHENoiseFilterPath')
        event.checkHBHENoise= event.checkerHBHENoiseFilterPath.check(iEvent.object(),event.METflags)
      
        return True

    def write(self):
        print 'writing FilterAnalyzer'
        super(metFilterAnalyzer, self).write()

    def __str__(self):
        tmp = super(metFilterAnalyzer,self).__str__()
        return '\n'



