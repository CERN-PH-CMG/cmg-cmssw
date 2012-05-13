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
        self.bookBoson("H")

        #Book the Z daughters
        self.bookBoson("H_Z1")
        self.bookBoson("H_Z2")

        #Book the Z legs
        self.bookLepton("H_Z1_leg1")
        self.bookLepton("H_Z1_leg2")
        self.bookLepton("H_Z2_leg1")
        self.bookLepton("H_Z2_leg2")

        #Book the Loose H for fake rate application
        self.bookBoson("HLoose")
        self.bookBoson("HLoose_Z1")
        self.bookBoson("HLoose_Z2")
        self.bookLepton("HLoose_Z1_leg1")
        self.bookLepton("HLoose_Z1_leg2")
        self.bookLepton("HLoose_Z2_leg1")
        self.bookLepton("HLoose_Z2_leg2")

        #Book the Z for fake rate measurement
        self.bookBoson("ZFR")
        self.bookLepton("ZFR_leg1")
        self.bookLepton("ZFR_leg2")
        self.bookLepton("ZFR_probe")
        self.var("ZFR_nProbes",int)


    def process(self, iEvent, event):
        self.reset()

        #get the analyzer event
        subevent = getattr( event, self.cfg_ana.anaName )

        #Fill run,lumi,event
        self.fillEventInfo(iEvent,subevent)

        #Fill the Higgs and legs
        if hasattr( subevent, 'higgsCand' ):
            self.fillBoson("H",subevent.higgsCand)
            self.fillBoson("H_Z1",subevent.higgsCand.leg1)
            self.fillBoson("H_Z2",subevent.higgsCand.leg2)
            self.fillLepton("H_Z1_leg1",subevent.higgsCand.leg1.leg1,subevent)
            self.fillLepton("H_Z1_leg2",subevent.higgsCand.leg1.leg2,subevent)
            self.fillLepton("H_Z2_leg1",subevent.higgsCand.leg2.leg1,subevent)
            self.fillLepton("H_Z2_leg2",subevent.higgsCand.leg2.leg2,subevent)
            

        if hasattr( subevent, 'higgsCandLoose' ):
            self.fillBoson("H",subevent.higgsCandLoose)
            self.fillBoson("HLoose_Z1",subevent.higgsCandLoose.leg1)
            self.fillBoson("HLoose_Z2",subevent.higgsCandLoose.leg2)
            self.fillLepton("HLoose_Z1_leg1",subevent.higgsCandLoose.leg1.leg1,subevent)
            self.fillLepton("HLoose_Z1_leg2",subevent.higgsCandLoose.leg1.leg2,subevent)
            self.fillLepton("HLoose_Z2_leg1",subevent.higgsCandLoose.leg2.leg1,subevent)
            self.fillLepton("HLoose_Z2_leg2",subevent.higgsCandLoose.leg2.leg2,subevent)

        #Fill the fake rate measurement info
        if hasattr( subevent, 'bestZForFakeRate' ):
            self.fillBoson("ZFR",subevent.bestZForFakeRate)
            self.fillLepton("ZFR_leg1",subevent.bestZForFakeRate.leg1,subevent)
            self.fillLepton("ZFR_leg2",subevent.bestZForFakeRate.leg2,subevent)
        if hasattr( subevent,'leptonsForFakeRate'):
            self.fill('ZFR_nProbes',len(subevent.leptonsForFakeRate))
            if len(subevent.leptonsForFakeRate)>0:
                self.fillLepton("ZFR_probe",subevent.leptonsForFakeRate[0],subevent)
 
        self.tree.Fill()


