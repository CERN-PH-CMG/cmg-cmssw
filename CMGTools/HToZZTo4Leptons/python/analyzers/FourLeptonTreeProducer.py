from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer

class FourLeptonTreeProducer( TreeAnalyzer ):
    '''Tree producer for the H->ZZ->4 lepton analysis.

    FIXME: A SINGLE TREE FOR ALL ANALYZERS
    Some of the functions in this class should be made available to everybody.'''
    def var(self, varName ):
        self.tree.addVar('float', varName)

    def fill(self, varName, value ):
        setattr( self.tree.s, varName, value )


    def bookLepton(self, pName ):
        self.var('{pName}_Pt'.format(pName=pName))
        self.var('{pName}_Eta'.format(pName=pName))
        self.var('{pName}_Phi'.format(pName=pName))
        self.var('{pName}_Charge'.format(pName=pName))
        self.var('{pName}_PdgId'.format(pName=pName))
        self.var('{pName}_SIP3D'.format(pName=pName))
        self.var('{pName}_IsoDB'.format(pName=pName))
        self.var('{pName}_IsoEA'.format(pName=pName))
        self.var('{pName}_Mass'.format(pName=pName))


    def fillBasic(self, pName,particle ):
        self.fill('{pName}_Pt'.format(pName=pName), particle.pt() )
        self.fill('{pName}_Eta'.format(pName=pName), particle.eta() )
        self.fill('{pName}_Phi'.format(pName=pName), particle.phi() )
        self.fill('{pName}_Charge'.format(pName=pName), particle.charge() )
        self.fill('{pName}_PdgId'.format(pName=pName), particle.pdgId() )
        self.fill('{pName}_Mass'.format(pName=pName), particle.mass())

    def fillLepton(self, pName,particle,event ):
        self.fillBasic(pName,particle)
        self.fill('{pName}_SIP3D'.format(pName=pName), particle.sip3D() )
        self.fill('{pName}_IsoDB'.format(pName=pName), particle.absIso(0.5) )
        if hasattr(event,'rho'):
            self.fill('{pName}_IsoEA'.format(pName=pName), particle.absEffAreaIso(event.rho,self.cfg_ana.effectiveAreas) )

    def fillEventInfo(self, iEvent,event ):
        self.fill('EVENT',iEvent.eventAuxiliary().id().event())
        self.fill('RUN',iEvent.eventAuxiliary().id().run())
        self.fill('LUMI',iEvent.eventAuxiliary().id().luminosityBlock())
        if hasattr(event,'met'):
            self.fill('met',event.met.pt())
        if hasattr(event,'rho'):
            self.fill('rho',event.rho)
        if hasattr(event,'step'):
            self.fill('step',event.step)
        if hasattr(event,'skim'):
            self.fill('skim',event.skim)
        
    def bookBoson(self, pName ):
        self.var('{pName}_Pt'.format(pName=pName))
        self.var('{pName}_Eta'.format(pName=pName))
        self.var('{pName}_Phi'.format(pName=pName))
        self.var('{pName}_Charge'.format(pName=pName))
        self.var('{pName}_Mass'.format(pName=pName))

    def fillBoson(self, pName,particle ):
        self.fillBasic(pName,particle)



    def bookEventInfo(self):    
        self.tree.addVar('int', "RUN")
        self.tree.addVar('int', "LUMI")
        self.tree.addVar('int', "EVENT")
        self.var('met')
        self.var('rho')
        self.var('step')
        self.var('skim')
            

    
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
        self.tree.addVar('int', "ZFR_nProbes")


        
        self.tree.book()


    def process(self, iEvent, event):

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
 
        self.tree.fill()
