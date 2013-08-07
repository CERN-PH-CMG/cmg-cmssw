from CMGTools.HToZZTo4Leptons.analyzers.TreeProducer import TreeProducer
import numpy
import ROOT

class FakeRateTreeProducer( TreeProducer ):
    '''Tree producer for the H->ZZ->4 fake measurement
    FIXME: A SINGLE TREE FOR ALL ANALYZERS
    Some of the functions in this class should be made available to everybody.'''

    def declareVariables(self):

        #Book event Info
        self.bookEventInfo()
        
        #Book the Z for fake rate measurement
        self.bookBoson("ZFR")
        self.bookLepton("ZFR_leg1")
        self.bookLepton("ZFR_leg2")
        self.bookLepton("ZFR_probe")
        self.var("ZFR_nProbes",int)
        self.var("ZFR_probe_FSR",int)

        self.var("vertices",int)
        self.var("vertexWeight")
        self.var('eventWeight')

        
    def process(self, iEvent, event):
        self.reset()


        #get the vertex analyzer event and fill the vertices
        self.fill('vertices',len(event.vertices))
        self.fill('vertexWeight',(event.vertexWeight))
        self.fill('eventWeight',(event.eventWeight))

        #Fill run,lumi,event
        self.fillEventInfo(iEvent,event)


        #Fill the fake rate measurement info
        if hasattr( event, 'bestZForFakeRate' ):
            self.fillBoson("ZFR",event.bestZForFakeRate)
            self.fillLepton("ZFR_leg1",event.bestZForFakeRate.leg1,event)
            self.fillLepton("ZFR_leg2",event.bestZForFakeRate.leg2,event)
            if hasattr( event,'leptonsForFakeRate') and len(event.leptonsForFakeRate)==1:
                self.fillLepton("ZFR_probe",event.leptonsForFakeRate[0],event)
                self.fill("ZFR_probe_FSR",hasattr(event.leptonsForFakeRate[0],'fsrPhoton'))

        self.tree.Fill()
        

