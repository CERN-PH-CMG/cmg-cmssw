from CMGTools.HToZZTo4Leptons.analyzers.TreeProducer import TreeProducer
import numpy
import ROOT

class FourLeptonTreeProducer( TreeProducer ):
    '''Tree producer for the H->ZZ->4 lepton analysis.

    FIXME: A SINGLE TREE FOR ALL ANALYZERS
    Some of the functions in this class should be made available to everybody.'''
    def declareVariables(self):

        #Book event Info
        self.bookEventInfo()

        #Book the Higgs
        self.var("HExists")
        self.bookHiggs("H")

        #Book the Z daughters
        self.bookBoson("H_Z1")
        self.bookBoson("H_Z2")

        #Book the Z legs
        self.bookLepton("H_Z1_leg1")
        self.bookLepton("H_Z1_leg2")
        self.bookLepton("H_Z2_leg1")
        self.bookLepton("H_Z2_leg2")

        #Book the Loose H for fake rate application
        self.var("HLooseExists")
        self.bookHiggs("HLoose")
        self.bookBoson("HLoose_Z1")
        self.bookBoson("HLoose_Z2")
        self.bookLepton("HLoose_Z1_leg1")
        self.bookLepton("HLoose_Z1_leg2")
        self.bookLepton("HLoose_Z2_leg1")
        self.bookLepton("HLoose_Z2_leg2")
        #Book the fake rate
        self.var('HLoose_fakeRate')
        self.var('HLoose_fakeRateUp')
        self.var('HLoose_fakeRateDwn')

        #Book the fake rate
        self.var('H_eff')
        self.var('H_effUp')
        self.var('H_effDwn')
        
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

        self.var('otherLeptons',int)
        self.var('otherTightLeptons',int)
        self.var('recoil')

        
    def process(self, iEvent, event):
        self.reset()

        #get the analyzer event
        subevent = getattr( event, self.cfg_ana.anaName )

        if hasattr(subevent,'recoil'):
            self.fill('recoil',subevent.recoil)

        #get the vertex analyzer event and fill the vertices
        self.fill('vertices',len(event.vertices))
        self.fill('vertexWeight',(event.vertexWeight))
        self.fill('eventWeight',(event.eventWeight))


        self.fill('otherLeptons',len(subevent.otherLeptons))
        self.fill('otherTightLeptons',len(subevent.otherTightLeptons))

        #Fill run,lumi,event
        self.fillEventInfo(iEvent,subevent)

        #Fill the Higgs and legs
        self.fill('HExists',0)
        
        if hasattr( subevent, 'higgsCand' ):
            self.fill('HExists',1)
            
            self.fillHiggs("H",subevent.higgsCand)
            self.fillBoson("H_Z1",subevent.higgsCand.leg1)
            self.fillBoson("H_Z2",subevent.higgsCand.leg2)
            self.fillLepton("H_Z1_leg1",subevent.higgsCand.leg1.leg1,subevent)
            self.fillLepton("H_Z1_leg2",subevent.higgsCand.leg1.leg2,subevent)
            self.fillLepton("H_Z2_leg1",subevent.higgsCand.leg2.leg1,subevent)
            self.fillLepton("H_Z2_leg2",subevent.higgsCand.leg2.leg2,subevent)

            if hasattr(subevent.higgsCand.leg2.leg1,'eff'):
                self.fill('H_eff',subevent.higgsCand.leg2.leg1.eff* \
                          subevent.higgsCand.leg2.leg2.eff* \
                          subevent.higgsCand.leg1.leg1.eff* \
                          subevent.higgsCand.leg1.leg2.eff)
                self.fill('H_effUp',subevent.higgsCand.leg2.leg1.effUp* \
                          subevent.higgsCand.leg2.leg2.effUp* \
                          subevent.higgsCand.leg1.leg1.effUp* \
                          subevent.higgsCand.leg1.leg2.effUp)

                self.fill('H_effDwn',subevent.higgsCand.leg2.leg1.effDwn* \
                          subevent.higgsCand.leg2.leg2.effDwn* \
                          subevent.higgsCand.leg1.leg1.effDwn* \
                          subevent.higgsCand.leg1.leg2.effDwn)
                



        if hasattr( subevent, 'higgsCandLoose' ):
            self.fill('HLooseExists',1)
            self.fillHiggs("HLoose",subevent.higgsCandLoose)
            self.fillBoson("HLoose_Z1",subevent.higgsCandLoose.leg1)
            self.fillBoson("HLoose_Z2",subevent.higgsCandLoose.leg2)
            self.fillLepton("HLoose_Z1_leg1",subevent.higgsCandLoose.leg1.leg1,subevent)
            self.fillLepton("HLoose_Z1_leg2",subevent.higgsCandLoose.leg1.leg2,subevent)
            self.fillLepton("HLoose_Z2_leg1",subevent.higgsCandLoose.leg2.leg1,subevent)
            self.fillLepton("HLoose_Z2_leg2",subevent.higgsCandLoose.leg2.leg2,subevent)

            if hasattr(subevent.higgsCandLoose.leg2.leg1,'fR'):
                self.fill('HLoose_fakeRate',subevent.higgsCandLoose.leg2.leg1.fR*subevent.higgsCandLoose.leg2.leg2.fR)
                self.fill('HLoose_fakeRateUp',subevent.higgsCandLoose.leg2.leg1.fRUp*subevent.higgsCandLoose.leg2.leg2.fRUp)
                self.fill('HLoose_fakeRateDwn',subevent.higgsCandLoose.leg2.leg1.fRDwn*subevent.higgsCandLoose.leg2.leg2.fRDwn)



        #Fill the fake rate measurement info
        if hasattr( subevent, 'bestZForFakeRate' ):
            self.fillBoson("ZFR",subevent.bestZForFakeRate)
            self.fillLepton("ZFR_leg1",subevent.bestZForFakeRate.leg1,subevent)
            self.fillLepton("ZFR_leg2",subevent.bestZForFakeRate.leg2,subevent)
            if hasattr( subevent,'leptonsForFakeRate'):
                self.fill('ZFR_nProbes',len(subevent.leptonsForFakeRate))
                if len(subevent.leptonsForFakeRate)>0:
                    self.fillLepton("ZFR_probe",subevent.leptonsForFakeRate[0],subevent)
                    self.fill("ZFR_probe_FSR",hasattr(subevent.leptonsForFakeRate[0],'fsrPhoton'))

        self.tree.Fill()
        

