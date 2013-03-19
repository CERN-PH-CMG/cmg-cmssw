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

        #Book the Loose OS H for fake rate application
        self.var("HLooseOSExists")
        self.bookHiggs("HLooseOS")
        self.bookBoson("HLooseOS_Z1")
        self.bookBoson("HLooseOS_Z2")
        self.bookLepton("HLooseOS_Z1_leg1")
        self.bookLepton("HLooseOS_Z1_leg2")
        self.bookLepton("HLooseOS_Z2_leg1")
        self.bookLepton("HLooseOS_Z2_leg2")


        #Book the Lepton Tagged Higgs
        self.var("LT_H")
        self.bookHiggs("LT_H")
        self.bookBoson("LT_H_Z1")
        self.bookBoson("LT_H_Z2")
        self.bookLepton("LT_H_Z1_leg1")
        self.bookLepton("LT_H_Z1_leg2")
        self.bookLepton("LT_H_Z2_leg1")
        self.bookLepton("LT_H_Z2_leg2")
        self.bookLepton("LT_H_lepton")

        #Book the Lepton Tagged Higgs
        self.var("LT_HLoose")
        self.bookHiggs("LT_HLoose")
        self.bookBoson("LT_HLoose_Z1")
        self.bookBoson("LT_HLoose_Z2")
        self.bookLepton("LT_HLoose_Z1_leg1")
        self.bookLepton("LT_HLoose_Z1_leg2")
        self.bookLepton("LT_HLoose_Z2_leg1")
        self.bookLepton("LT_HLoose_Z2_leg2")
        self.bookLepton("LT_HLoose_lepton")





        #Book the fake rate
        self.var('HLoose_fakeRate')
        self.var('HLoose_fakeRateUp')
        self.var('HLoose_fakeRateDwn')

        self.var('HLooseOS_fakeRate')
        self.var('HLooseOS_fakeRateUp')
        self.var('HLooseOS_fakeRateDwn')


        self.var('LT_HLoose_fakeRate')
        self.var('LT_HLoose_fakeRateUp')
        self.var('LT_HLoose_fakeRateDwn')



        #Book the fake rate
        self.var('H_eff')
        self.var('H_effUp')
        self.var('H_effDwn')

        self.var('LT_H_eff')
        self.var('LT_H_effUp')
        self.var('LT_H_effDwn')
        

        self.var("vertices",int)
        self.var("vertexWeight")
        self.var('eventWeight')

        self.var('H_otherLeptons',int)
        self.var('H_otherTightLeptons',int)
        self.var('HLoose_otherLeptons',int)
        self.var('HLoose_otherTightLeptons',int)
        self.var('recoil')

        if self.cfg_comp.isMC:
            self.var("genHMass")
            self.var('genHDecay')
            self.var('genHProcess')
            


        
    def process(self, iEvent, event):
        self.reset()



        if hasattr(event,'recoil'):
            self.fill('recoil',event.recoil)

        #get the vertex analyzer event and fill the vertices
        self.fill('vertices',len(event.vertices))
        self.fill('vertexWeight',(event.vertexWeight))
        self.fill('eventWeight',(event.eventWeight))

        if hasattr(event,'otherLeptons'):
            self.fill('H_otherLeptons',len(event.otherLeptons))
        if hasattr(event,'otherTightLeptons'):
            self.fill('H_otherTightLeptons',len(event.otherTightLeptons))
        if hasattr(event,'otherLeptonsLoose'):
            self.fill('HLoose_otherLeptons',len(event.otherLeptonsLoose))
        if hasattr(event,'otherTightLeptonsLoose'):
            self.fill('HLoose_otherTightLeptons',len(event.otherTightLeptonsLoose))

        #Fill run,lumi,event
        self.fillEventInfo(iEvent,event)

        #Fill the Higgs and legs
        self.fill('HExists',0)

        if self.cfg_comp.isMC:
            if hasattr(event,'higgsGen'):
                self.fill("genHMass",event.higgsGen.mass())
            if hasattr(event,'genDecay'):
                self.fill('genHDecay',event.genDecay)
            if hasattr(event,'genProcess'):
                self.fill('genHProcess',event.genProcess)




        
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

        if hasattr( event, 'higgsCandLooseOS' ):
            self.fill('HLooseOSExists',1)
            self.fillHiggs("HLooseOS",event.higgsCandLooseOS)
            self.fillBoson("HLooseOS_Z1",event.higgsCandLooseOS.leg1)
            self.fillBoson("HLooseOS_Z2",event.higgsCandLooseOS.leg2)
            self.fillLepton("HLooseOS_Z1_leg1",event.higgsCandLooseOS.leg1.leg1,event)
            self.fillLepton("HLooseOS_Z1_leg2",event.higgsCandLooseOS.leg1.leg2,event)
            self.fillLepton("HLooseOS_Z2_leg1",event.higgsCandLooseOS.leg2.leg1,event)
            self.fillLepton("HLooseOS_Z2_leg2",event.higgsCandLooseOS.leg2.leg2,event)

            if hasattr(event.higgsCandLooseOS.leg2.leg1,'fR'):
                self.fill('HLooseOS_fakeRate',event.higgsCandLooseOS.leg2.leg1.fR*event.higgsCandLooseOS.leg2.leg2.fR)
                self.fill('HLooseOS_fakeRateUp',event.higgsCandLooseOS.leg2.leg1.fRUp*event.higgsCandLooseOS.leg2.leg2.fRUp)
                self.fill('HLooseOS_fakeRateDwn',event.higgsCandLooseOS.leg2.leg1.fRDwn*event.higgsCandLooseOS.leg2.leg2.fRDwn)






        if hasattr( event, 'higgsCandTagged' ):
            self.fillHiggs("LT_H",event.higgsCandTagged)
            self.fillBoson("LT_H_Z1",event.higgsCandTagged.leg1)
            self.fillBoson("LT_H_Z2",event.higgsCandTagged.leg2)
            self.fillLepton("LT_H_Z1_leg1",event.higgsCandTagged.leg1.leg1,event)
            self.fillLepton("LT_H_Z1_leg2",event.higgsCandTagged.leg1.leg2,event)
            self.fillLepton("LT_H_Z2_leg1",event.higgsCandTagged.leg2.leg1,event)
            self.fillLepton("LT_H_Z2_leg2",event.higgsCandTagged.leg2.leg2,event)

            if hasattr(event.higgsCandTagged,'leptonTag'):
                self.fillLepton("LT_H_lepton",event.higgsCandTagged.leptonTag,event)
                

            if hasattr(event.higgsCandTagged.leg2.leg1,'eff'):
                self.fill('LT_H_eff',event.higgsCandTagged.leg2.leg1.eff* \
                          event.higgsCandTagged.leg2.leg2.eff* \
                          event.higgsCandTagged.leg1.leg1.eff* \
                          event.higgsCandTagged.leg1.leg2.eff)
                self.fill('LT_H_effUp',event.higgsCandTagged.leg2.leg1.effUp* \
                          event.higgsCandTagged.leg2.leg2.effUp* \
                          event.higgsCandTagged.leg1.leg1.effUp* \
                          event.higgsCandTagged.leg1.leg2.effUp)

                self.fill('LT_H_effDwn',event.higgsCandTagged.leg2.leg1.effDwn* \
                          event.higgsCandTagged.leg2.leg2.effDwn* \
                          event.higgsCandTagged.leg1.leg1.effDwn* \
                          event.higgsCandTagged.leg1.leg2.effDwn)


        if hasattr( event, 'higgsCandTaggedLoose' ):
            self.fillHiggs("LT_HLoose",event.higgsCandTaggedLoose)
            self.fillBoson("LT_HLoose_Z1",event.higgsCandTaggedLoose.leg1)
            self.fillBoson("LT_HLoose_Z2",event.higgsCandTaggedLoose.leg2)
            self.fillLepton("LT_HLoose_Z1_leg1",event.higgsCandTaggedLoose.leg1.leg1,event)
            self.fillLepton("LT_HLoose_Z1_leg2",event.higgsCandTaggedLoose.leg1.leg2,event)
            self.fillLepton("LT_HLoose_Z2_leg1",event.higgsCandTaggedLoose.leg2.leg1,event)
            self.fillLepton("LT_HLoose_Z2_leg2",event.higgsCandTaggedLoose.leg2.leg2,event)

            if hasattr(event.higgsCandTaggedLoose,'leptonTag'):
                self.fillLepton("LT_HLoose_lepton",event.higgsCandTaggedLoose.leptonTag,event)


            if hasattr(event.higgsCandTaggedLoose.leg2.leg1,'fR'):
                self.fill('LT_HLoose_fakeRate',event.higgsCandTaggedLoose.leg2.leg1.fR*event.higgsCandTaggedLoose.leg2.leg2.fR*event.higgsCandTaggedLoose.leptonTag.fR)
                self.fill('LT_HLoose_fakeRateUp',event.higgsCandTaggedLoose.leg2.leg1.fRUp*event.higgsCandTaggedLoose.leg2.leg2.fRUp*event.higgsCandTaggedLoose.leptonTag.fRUp)
                self.fill('LT_HLoose_fakeRateDwn',event.higgsCandTaggedLoose.leg2.leg1.fRDwn*event.higgsCandTaggedLoose.leg2.leg2.fRDwn*event.higgsCandTaggedLoose.leptonTag.fRDwn)



        self.tree.Fill()
        

