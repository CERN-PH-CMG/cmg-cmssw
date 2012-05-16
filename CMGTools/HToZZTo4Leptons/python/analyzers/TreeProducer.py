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
        else:
            print 'Unknown type '


    def fill(self, varName, value ):
        self.vars[varName][0]=value

        

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
        self.fill('{pName}_IsoDB'.format(pName=pName), particle.absIso(0.5) )
        if hasattr(event,'rho'):
            self.fill('{pName}_IsoEA'.format(pName=pName), particle.absEffAreaIso(event.rho,self.cfg_ana.effectiveAreas) )
        if abs(particle.pdgId())==11:
            self.fill('{pName}_eleMVA'.format(pName=pName), particle.mvaNonTrigV0())
            self.fill('{pName}_eleMVAID'.format(pName=pName), particle.mvaIDZZ())
            self.fill('{pName}_eleConvHits'.format(pName=pName), particle.numberOfHits())
            self.fill('{pName}_ID'.format(pName=pName), particle.mvaIDZZ())
        if abs(particle.pdgId())==13:
            self.fill('{pName}_muPF'.format(pName=pName), particle.sourcePtr().userFloat("isPFMuon"))
            self.fill('{pName}_muPixelHits'.format(pName=pName), particle.numberOfValidPixelHits())
            self.fill('{pName}_muMatches'.format(pName=pName), particle.numberOfMatches())
            self.fill('{pName}_ID'.format(pName=pName), particle.sourcePtr().userFloat("isPFMuon"))

            
            

    def fillEventInfo(self, iEvent,event ):
        self.fill('RUN',iEvent.eventAuxiliary().id().run())
        self.fill('LUMI',iEvent.eventAuxiliary().id().luminosityBlock())
        self.fill('EVENT',iEvent.eventAuxiliary().id().event())
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

    def bookHiggs(self, pName ):
        self.var('{pName}_Pt'.format(pName=pName))
        self.var('{pName}_Eta'.format(pName=pName))
        self.var('{pName}_Phi'.format(pName=pName))
        self.var('{pName}_Charge'.format(pName=pName))
        self.var('{pName}_Mass'.format(pName=pName))
        self.var('{pName}_MinPairMass'.format(pName=pName))
        self.var('{pName}_MinOSPairMass'.format(pName=pName))


    def fillBoson(self, pName,particle ):
        self.fillBasic(pName,particle)

    def fillHiggs(self, pName,particle ):
        self.fillBasic(pName,particle)
        self.fill('{pName}_MinPairMass'.format(pName=pName), particle.minPairMass() )
        self.fill('{pName}_MinOSPairMass'.format(pName=pName), particle.minOSPairMass() )
        


    def bookEventInfo(self):    
        self.var('RUN',int)
        self.var('LUMI',int)
        self.var('EVENT',int)
        self.var('met')
        self.var('rho')
        self.var('step')
        self.var('skim')

            

    

    def reset(self):
        for name,value in self.vars.iteritems():
            value[0]=-99

    def write(self):
        super(TreeProducer, self).write()
        self.file.cd(self.name)
        self.tree.Write()
        self.file.Close()
