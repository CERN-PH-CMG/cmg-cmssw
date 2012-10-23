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
        

        self.var("vertices",int)
        self.var("vertexWeight")
        self.var('eventWeight')

        self.var('otherLeptons',int)
        self.var('otherTightLeptons',int)
        self.var('recoil')

        if self.cfg_comp.isMC:
            self.var("genHMass")
            self.var('genHDecay')
            


        
    def process(self, iEvent, event):
        self.reset()



        if hasattr(event,'recoil'):
            self.fill('recoil',event.recoil)

        #get the vertex analyzer event and fill the vertices
        self.fill('vertices',len(event.vertices))
        self.fill('vertexWeight',(event.vertexWeight))
        self.fill('eventWeight',(event.eventWeight))

        if hasattr(event,'otherLeptons'):
            self.fill('otherLeptons',len(event.otherLeptons))
        if hasattr(event,'otherTightLeptons'):
            self.fill('otherTightLeptons',len(event.otherTightLeptons))

        #Fill run,lumi,event
        self.fillEventInfo(iEvent,event)

        #Fill the Higgs and legs
        self.fill('HExists',0)

        if self.cfg_comp.isMC:
            if hasattr(event,'higgsGen'):
                self.fill("genHMass",event.higgsGen.mass())
            if hasattr(event,'genDecay'):
                self.fill('genHDecay',event.genDecay)




        
        if hasattr( event, 'higgsCand' ):
            self.fill('HExists',1)
            
            self.fillHiggs("H",event.higgsCand)
            self.fillBoson("H_Z1",event.higgsCand.leg1)
            self.fillBoson("H_Z2",event.higgsCand.leg2)
            self.fillLepton("H_Z1_leg1",event.higgsCand.leg1.leg1,event)
            self.fillLepton("H_Z1_leg2",event.higgsCand.leg1.leg2,event)
            self.fillLepton("H_Z2_leg1",event.higgsCand.leg2.leg1,event)
            self.fillLepton("H_Z2_leg2",event.higgsCand.leg2.leg2,event)

            if hasattr(event.higgsCand.leg2.leg1,'eff'):
                self.fill('H_eff',event.higgsCand.leg2.leg1.eff* \
                          event.higgsCand.leg2.leg2.eff* \
                          event.higgsCand.leg1.leg1.eff* \
                          event.higgsCand.leg1.leg2.eff)
                self.fill('H_effUp',event.higgsCand.leg2.leg1.effUp* \
                          event.higgsCand.leg2.leg2.effUp* \
                          event.higgsCand.leg1.leg1.effUp* \
                          event.higgsCand.leg1.leg2.effUp)

                self.fill('H_effDwn',event.higgsCand.leg2.leg1.effDwn* \
                          event.higgsCand.leg2.leg2.effDwn* \
                          event.higgsCand.leg1.leg1.effDwn* \
                          event.higgsCand.leg1.leg2.effDwn)
                



        if hasattr( event, 'higgsCandLoose' ):
            self.fill('HLooseExists',1)
            self.fillHiggs("HLoose",event.higgsCandLoose)
            self.fillBoson("HLoose_Z1",event.higgsCandLoose.leg1)
            self.fillBoson("HLoose_Z2",event.higgsCandLoose.leg2)
            self.fillLepton("HLoose_Z1_leg1",event.higgsCandLoose.leg1.leg1,event)
            self.fillLepton("HLoose_Z1_leg2",event.higgsCandLoose.leg1.leg2,event)
            self.fillLepton("HLoose_Z2_leg1",event.higgsCandLoose.leg2.leg1,event)
            self.fillLepton("HLoose_Z2_leg2",event.higgsCandLoose.leg2.leg2,event)

            if hasattr(event.higgsCandLoose.leg2.leg1,'fR'):
                self.fill('HLoose_fakeRate',event.higgsCandLoose.leg2.leg1.fR*event.higgsCandLoose.leg2.leg2.fR)
                self.fill('HLoose_fakeRateUp',event.higgsCandLoose.leg2.leg1.fRUp*event.higgsCandLoose.leg2.leg2.fRUp)
                self.fill('HLoose_fakeRateDwn',event.higgsCandLoose.leg2.leg1.fRDwn*event.higgsCandLoose.leg2.leg2.fRDwn)



        self.tree.Fill()
        

