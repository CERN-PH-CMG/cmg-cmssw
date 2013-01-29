from CMGTools.RootTools.fwlite.Analyzer import Analyzer
import numpy
import ROOT

class TreeProducer( Analyzer ):
    '''Tree producer generic'''
    
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(TreeProducer,self).__init__(cfg_ana, cfg_comp, looperName)
        fileName = '/'.join([self.dirName,
                             self.name+'_tree.root'])
        self.file = ROOT.TFile( fileName, 'recreate' )
        self.file.mkdir(self.name)
        self.file.cd(self.name)
        self.tree = ROOT.TTree("tree",self.name)
        self.vars={}
        self.declareVariables()

    def var(self, varName,type=float ):
        self.vars[varName]=numpy.zeros(1,type)
        if type is float  : 
            self.tree.Branch(varName,self.vars[varName],varName+'/D')
        elif type is int    : 
            self.tree.Branch(varName,self.vars[varName],varName+'/I')



    def fill(self, varName, value ):
        self.vars[varName][0]=value

    def bookGenFSR(self, pName ):
        self.var('{pName}_Pt'.format(pName=pName))
        self.var('{pName}_Eta'.format(pName=pName))
        self.var('{pName}_Phi'.format(pName=pName))
        self.var('{pName}_E'.format(pName=pName))


    def bookRecoFSR(self, pName ):
        self.bookGenFSR(pName)
        self.var('{pName}_Match'.format(pName=pName))
        self.var('{pName}_MatchE'.format(pName=pName))
        self.var('{pName}_Type'.format(pName=pName))
        self.var('{pName}_IsoCharged'.format(pName=pName))
        self.var('{pName}_IsoGamma'.format(pName=pName))
        self.var('{pName}_IsoNeutral'.format(pName=pName))
        self.var('{pName}_IsoPU'.format(pName=pName))
        self.var('{pName}_IsoDB'.format(pName=pName))

    def fillGenFSR(self, pName,particle ):
        self.fill('{pName}_Pt'.format(pName=pName), particle.pt() )
        self.fill('{pName}_Eta'.format(pName=pName), particle.eta() )
        self.fill('{pName}_Phi'.format(pName=pName), particle.phi() )
        self.fill('{pName}_E'.format(pName=pName), particle.energy() )

    def fillRecoFSR(self, pName,particle ):
        self.fillGenFSR(pName,particle)
        if hasattr(particle,'match') and hasattr(particle,'matchedE'):
            self.fill('{pName}_Match'.format(pName=pName), particle.match )
            self.fill('{pName}_MatchE'.format(pName=pName), particle.matchedE )
        else:    
            self.fill('{pName}_Match'.format(pName=pName), -99 )

            
        self.fill('{pName}_IsoCharged'.format(pName=pName), particle.chargedHadronIso() )
        self.fill('{pName}_IsoGamma'.format(pName=pName), particle.photonIso() )
        self.fill('{pName}_IsoNeutral'.format(pName=pName), particle.neutralHadronIso() )
        self.fill('{pName}_IsoPU'.format(pName=pName), particle.puChargedHadronIso() )
        self.fill('{pName}_IsoDB'.format(pName=pName), particle.absIso(0.5) )
        self.fill('{pName}_Type'.format(pName=pName), particle.isFromMuon() )
            

    def bookLepton(self, pName ):
        self.var('{pName}_Pt'.format(pName=pName))
        self.var('{pName}_Eta'.format(pName=pName))
        self.var('{pName}_Phi'.format(pName=pName))
        self.var('{pName}_Charge'.format(pName=pName))
        self.var('{pName}_PdgId'.format(pName=pName))
        self.var('{pName}_SIP3D'.format(pName=pName))
        self.var('{pName}_SIP'.format(pName=pName))
        self.var('{pName}_DXY'.format(pName=pName))
        self.var('{pName}_DZ'.format(pName=pName))
        self.var('{pName}_IsoDB'.format(pName=pName))
        self.var('{pName}_IsoEA'.format(pName=pName))
        self.var('{pName}_Mass'.format(pName=pName))
        self.var('{pName}_muPF'.format(pName=pName))
        self.var('{pName}_muPixelHits'.format(pName=pName))
        self.var('{pName}_muMatches'.format(pName=pName))
        self.var('{pName}_eleMVA'.format(pName=pName))
        self.var('{pName}_eleMVAID'.format(pName=pName))
        self.var('{pName}_eleConvHits'.format(pName=pName))
        self.var('{pName}_ID'.format(pName=pName))




    def fillBasic(self, pName,particle ):
        self.fill('{pName}_Pt'.format(pName=pName), particle.pt() )
        self.fill('{pName}_Eta'.format(pName=pName), particle.eta() )
        self.fill('{pName}_Phi'.format(pName=pName), particle.phi() )
        self.fill('{pName}_Charge'.format(pName=pName), particle.charge() )
        self.fill('{pName}_Mass'.format(pName=pName), particle.mass())

    def fillLepton(self, pName,particle,event ):
        self.fillBasic(pName,particle)
        self.fill('{pName}_PdgId'.format(pName=pName), particle.pdgId() )
        self.fill('{pName}_SIP3D'.format(pName=pName), particle.sip3D() )
        self.fill('{pName}_SIP'.format(pName=pName), particle.sourcePtr().dB(2) )
        self.fill('{pName}_DXY'.format(pName=pName), particle.dxy() )
        self.fill('{pName}_DZ'.format(pName=pName), particle.dz() )
        self.fill('{pName}_IsoDB'.format(pName=pName), particle.absIso(0.5) )

        if abs(particle.pdgId())==11 :
            self.fill('{pName}_eleMVA'.format(pName=pName), particle.mvaNonTrigV0())
            self.fill('{pName}_eleMVAID'.format(pName=pName), particle.mvaIDZZ())
            self.fill('{pName}_eleConvHits'.format(pName=pName), particle.numberOfHits())
            self.fill('{pName}_ID'.format(pName=pName), particle.mvaIDZZ())
            self.fill('{pName}_IsoEA'.format(pName=pName), particle.absEffAreaIso(event.rhoEle,self.cfg_ana.effectiveAreas) )

        if abs(particle.pdgId())==13:
            self.fill('{pName}_muPF'.format(pName=pName), particle.sourcePtr().userFloat("isPFMuon"))
            self.fill('{pName}_muPixelHits'.format(pName=pName), particle.numberOfValidPixelHits())
            self.fill('{pName}_muMatches'.format(pName=pName), particle.numberOfMatches())
            self.fill('{pName}_ID'.format(pName=pName), particle.sourcePtr().userFloat("isPFMuon"))
            self.fill('{pName}_IsoEA'.format(pName=pName), particle.absEffAreaIso(event.rhoMu,self.cfg_ana.effectiveAreas) )

            
            

    def fillEventInfo(self, iEvent,event ):
        self.fill('RUN',iEvent.eventAuxiliary().id().run())
        self.fill('LUMI',iEvent.eventAuxiliary().id().luminosityBlock())
        self.fill('EVENT',iEvent.eventAuxiliary().id().event())
        if hasattr(event,'met'):
            self.fill('met',event.met.pt())
        if hasattr(event,'step'):
            self.fill('step',event.step)
            
        
    def bookBoson(self, pName ):
        self.var('{pName}_Pt'.format(pName=pName))
        self.var('{pName}_Eta'.format(pName=pName))
        self.var('{pName}_Phi'.format(pName=pName))
        self.var('{pName}_Charge'.format(pName=pName))
        self.var('{pName}_Mass'.format(pName=pName))
        self.var('{pName}_FSR_Exists'.format(pName=pName))
        self.var('{pName}_FSR_UncorrMass'.format(pName=pName))
        self.var('{pName}_FSR_Theta1'.format(pName=pName))
        self.var('{pName}_FSR_Theta2'.format(pName=pName))
        self.var('{pName}_FSR_DR1'.format(pName=pName))
        self.var('{pName}_FSR_DR2'.format(pName=pName))
        self.var('{pName}_FSR_ThetaStar'.format(pName=pName))
        self.var('{pName}_FSR_DRStar'.format(pName=pName))
        self.bookRecoFSR('{pName}_FSR'.format(pName=pName))

    def bookHiggs(self, pName ):
        self.var('{pName}_Pt'.format(pName=pName))
        self.var('{pName}_PtOM'.format(pName=pName))
        self.var('{pName}_Eta'.format(pName=pName))
        self.var('{pName}_Phi'.format(pName=pName))
        self.var('{pName}_Charge'.format(pName=pName))
        self.var('{pName}_Mass'.format(pName=pName))
        self.var('{pName}_MinPairMass'.format(pName=pName))
        self.var('{pName}_MinOSPairMass'.format(pName=pName))
        self.var('{pName}_FSRExists'.format(pName=pName))
        self.var('{pName}_FSRMatch'.format(pName=pName))
        self.var('{pName}_FSRUncorrMass'.format(pName=pName))
        self.var('{pName}_KD'.format(pName=pName))
        self.var('{pName}_GG0KD'.format(pName=pName))
        self.var('{pName}_GG0HKD'.format(pName=pName))
        self.var('{pName}_QQ1MinusKD'.format(pName=pName))
        self.var('{pName}_QQ1PlusKD'.format(pName=pName))
        self.var('{pName}_GG2PlusKD'.format(pName=pName))
        self.var('{pName}_QQ2PlusKD'.format(pName=pName))
        self.var('{pName}_Weight'.format(pName=pName))
        self.var('{pName}_MassErr'.format(pName=pName))
        self.var('{pName}_MassErrRaw'.format(pName=pName))
        for postfix in ['','Up','Dwn']:
            self.var('{pName}_DEta'.format(pName=pName)+postfix)
            self.var('{pName}_DPhi'.format(pName=pName)+postfix)
            self.var('{pName}_MJJ'.format(pName=pName)+postfix)
            self.var('{pName}_Fisher'.format(pName=pName)+postfix)
            self.var('{pName}_MJJJ'.format(pName=pName)+postfix)
            self.var('{pName}_MBB'.format(pName=pName)+postfix)
            self.var('{pName}_HT'.format(pName=pName)+postfix)
            self.var('{pName}_Ptjj'.format(pName=pName)+postfix)
            self.var('{pName}_NJets'.format(pName=pName)+postfix,int)
            self.var('{pName}_NBJets'.format(pName=pName)+postfix,int)
            self.var('{pName}_NBJetsTight'.format(pName=pName)+postfix,int)
            self.var('{pName}_LeadingJetPt'.format(pName=pName)+postfix)
            self.var('{pName}_LeadingJetEta'.format(pName=pName)+postfix)
            self.var('{pName}_SubleadingJetPt'.format(pName=pName)+postfix)
            self.var('{pName}_SubleadingJetEta'.format(pName=pName)+postfix)


    def fillBoson(self, pName,particle ):
        self.fillBasic(pName,particle)
        self.fill('{pName}_FSR_UncorrMass'.format(pName=pName), particle.fsrUncorrected().M() )

        if hasattr(particle,'fsrPhoton'):
            self.fill('{pName}_FSR_Exists'.format(pName=pName), 1.0 )
            self.fill('{pName}_FSR_Theta1'.format(pName=pName), particle.fsrTheta1() )
            self.fill('{pName}_FSR_Theta2'.format(pName=pName), particle.fsrTheta2() )
            self.fill('{pName}_FSR_DR1'.format(pName=pName), particle.fsrDR1() )
            self.fill('{pName}_FSR_DR2'.format(pName=pName), particle.fsrDR2() )
            self.fill('{pName}_FSR_ThetaStar'.format(pName=pName), particle.fsrThetaStar() )
            self.fill('{pName}_FSR_DRStar'.format(pName=pName), particle.fsrDRStar() )
            self.fillRecoFSR('{pName}_FSR'.format(pName=pName),particle.fsrPhoton )
        else:
            self.fill('{pName}_FSR_Exists'.format(pName=pName), 0.0 )
            
    def fillHiggs(self, pName,particle ):
        self.fillBasic(pName,particle)
        self.fill('{pName}_PtOM'.format(pName=pName), particle.Pt()/particle.M() )
        self.fill('{pName}_MinPairMass'.format(pName=pName), particle.minPairMass() )
        self.fill('{pName}_MinOSPairMass'.format(pName=pName), particle.minOSPairMass() )
        self.fill('{pName}_FSRUncorrMass'.format(pName=pName), particle.fsrUncorrected().M() )
        self.fill('{pName}_FSRExists'.format(pName=pName), hasattr(particle.leg1,'fsrPhoton') or hasattr(particle.leg2,'fsrPhoton'))
        self.fill('{pName}_FSRUncorrMass'.format(pName=pName), particle.fsrUncorrected().M() )
        if hasattr(particle,'KD'):
            self.fill('{pName}_KD'.format(pName=pName), particle.KD )
        if hasattr(particle,'gg0KD'):
            self.fill('{pName}_GG0KD'.format(pName=pName), particle.gg0KD )
        if hasattr(particle,'gg0HKD'):
            self.fill('{pName}_GG0HKD'.format(pName=pName), particle.gg0HKD )
        if hasattr(particle,'qq1MinusKD'):
            self.fill('{pName}_QQ1MinusKD'.format(pName=pName), particle.qq1MinusKD )
        if hasattr(particle,'qq1PlusKD'):
            self.fill('{pName}_QQ1PlusKD'.format(pName=pName), particle.qq1PlusKD )
        if hasattr(particle,'qq2PlusKD'):
            self.fill('{pName}_QQ2PlusKD'.format(pName=pName), particle.qq2PlusKD )
        if hasattr(particle,'gg2PlusKD'):
            self.fill('{pName}_GG2PlusKD'.format(pName=pName), particle.gg2PlusKD )

        if hasattr(particle,'weight'):
            self.fill('{pName}_Weight'.format(pName=pName), particle.weight )
        if hasattr(particle,'massErr'):
            self.fill('{pName}_MassErr'.format(pName=pName), particle.massErr )
        if hasattr(particle,'massErrRaw'):
            self.fill('{pName}_MassErrRaw'.format(pName=pName), particle.massErrRaw )

        if hasattr(particle,'jets'):
            for postfix in ['','Up','Dwn']: 
                self.fill('{pName}_DEta'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['dEta'] )
                self.fill('{pName}_DPhi'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['dPhi'] )
                self.fill('{pName}_MJJ'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['Mjj'] )
                self.fill('{pName}_Fisher'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['Fisher'] )
                self.fill('{pName}_MJJJ'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['Mjjj'] )
                self.fill('{pName}_MBB'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['Mbb'] )
                self.fill('{pName}_HT'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['HT'] )
                self.fill('{pName}_NJets'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['nJets'] )
                self.fill('{pName}_NBJets'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['nBJets'] )
                self.fill('{pName}_NBJetsTight'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['nBJetsTight'] )
                self.fill('{pName}_Ptjj'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['Ptjj'] )
                self.fill('{pName}_LeadingJetPt'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['leadingPt'] )
                self.fill('{pName}_LeadingJetEta'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['leadingEta'] )
                self.fill('{pName}_SubleadingJetPt'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['subleadingPt'] )
                self.fill('{pName}_SubleadingJetEta'.format(pName=pName)+postfix, getattr(particle,'jets'+postfix)['subleadingEta'] )
            


    def bookEventInfo(self):    
        self.var('RUN',int)
        self.var('LUMI',int)
        self.var('EVENT',int)
        self.var('met')
        self.var('step')

            

    

    def reset(self):
        for name,value in self.vars.iteritems():
            value[0]=-99

    def write(self):
        super(TreeProducer, self).write()
        self.file.cd(self.name)
        self.tree.Write()
        self.file.Close()
