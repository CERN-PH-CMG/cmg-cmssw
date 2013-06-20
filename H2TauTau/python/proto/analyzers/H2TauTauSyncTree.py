from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.H2TauTau.proto.analyzers.ntuple import *


class H2TauTauSyncTree( TreeAnalyzerNumpy ):
    '''Tree producer for the H->tau tau analysis.

    Some of the functions in this class should be made available to everybody.'''
    
    def declareVariables(self):

        tr = self.tree

        var( tr, 'run', int)
        var( tr, 'lumi', int)
        var( tr, 'evt', int)
        
        var( tr, 'npv', int)
        var( tr, 'npu', int)
        var( tr, 'rho')
        
        var( tr, 'mcweight')
        var( tr, 'puweight')
        var( tr, 'effweight')
        var( tr, 'weight')
        var( tr, 'embeddedWeight')
        
        var( tr, 'm_sv')
        var( tr, 'mvis')
        var( tr, 'm_sv_Up')
        var( tr, 'm_sv_Down')
        
        var( tr, 'pt_1')
        var( tr, 'phi_1')
        var( tr, 'eta_1')
        var( tr, 'm_1')
        var( tr, 'q_1')
        var( tr, 'iso_1')
        var( tr, 'mva_1')
        var( tr, 'd0_1')
        var( tr, 'dZ_1')
        var( tr, 'passid_1', int)
        var( tr, 'passiso_1', int)
        var( tr, 'mt_1')
        var( tr, 'trigweight_1')
        var( tr, 'idweight_1')
        var( tr, 'isoweight_1')
        
        
        var( tr, 'pt_2')
        var( tr, 'phi_2')
        var( tr, 'eta_2')
        var( tr, 'm_2')
        var( tr, 'q_2')
        var( tr, 'iso_2')
        var( tr, 'mva_2')
        var( tr, 'passid_2', int)
        var( tr, 'passiso_2', int)
        var( tr, 'mt_2')
        var( tr, 'trigweight_2')
        var( tr, 'idweight_2')
        var( tr, 'isoweight_2')
        
        var( tr, 'met')
        var( tr, 'metphi')
        var( tr, 'mvamet')
        var( tr, 'mvametphi')
        var( tr, 'pzetavis')
        var( tr, 'pzetamiss')

        var( tr, 'metcov00')
        var( tr, 'metcov01')
        var( tr, 'metcov10')
        var( tr, 'metcov11')
        
        var( tr, 'mvacov00')
        var( tr, 'mvacov01')
        var( tr, 'mvacov10')
        var( tr, 'mvacov11')
      
        var( tr, 'jpt_1')
        var( tr, 'jeta_1')
        var( tr, 'jphi_1')
        var( tr, 'jptraw_1')
        var( tr, 'jptunc_1')
        var( tr, 'jmva_1')
        var( tr, 'jpass_1', int)
        
        var( tr, 'jpt_2')
        var( tr, 'jeta_2')
        var( tr, 'jphi_2')
        var( tr, 'jptraw_2')
        var( tr, 'jptunc_2')
        var( tr, 'jmva_2')
        var( tr, 'jpass_2', int)
        
        var( tr, 'bpt')
        var( tr, 'beta')
        var( tr, 'bphi')
        
        var( tr, 'mjj')
        var( tr, 'jdeta')
        var( tr, 'njetingap', int)
        var( tr, 'mva')
        
        var( tr, 'jdphi')
        var( tr, 'dijetpt')
        var( tr, 'dijetphi')
        var( tr, 'hdijetphi')
        var( tr, 'visjeteta')
        var( tr, 'ptvis')
        
        var( tr, 'nbtag', int)
        var( tr, 'njets', int)
        var( tr, 'njetspt20', int)


    def declareHandles(self):
        super(H2TauTauSyncTree, self).declareHandles()
        self.handles['pfmetraw'] = AutoHandle(
            'cmgPFMETRaw',
            'std::vector<cmg::BaseMET>' 
            )        
        # self.handles['pfmetsig'] = AutoHandle(
        #     'pfMetSignificance',
        #     'cmg::METSignificance' 
        #     )        
        

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        
        # not event.thirdLeptonVeto or \
        # not event.leptonAccept:
        if not event.isSignal or \
           (hasattr(event, 'thirdLeptonVeto') and not event.thirdLeptonVeto ) or \
           (hasattr(event, 'leptonAccept') and not event.leptonAccept ):
            return False

        if hasattr(self.cfg_ana, 'isFake') and \
               event.isFake != self.cfg_ana.isFake:
            return False
        
        tr = self.tree
        tr.reset()
        
        fill( tr, 'run', event.run) 
        fill( tr, 'lumi',event.lumi)
        fill( tr, 'evt', event.eventId)
        
        fill( tr, 'npv', len(event.goodVertices)) 
        nPU = -1
        if hasattr(event, 'pileUpInfo'):
            nPU = event.pileUpInfo[1].nTrueInteractions()
            fill( tr, 'npu', nPU) 
        fill( tr, 'rho', event.rho)
        
        fill( tr, 'mcweight', event.eventWeight) 
        fill( tr, 'puweight', event.vertexWeight) #OK
        # warning: phil's leg definition is different from ours!
        # for him, leg1 = muon, leg2 = tau
        leg1 = event.diLepton.leg2()
        leg2 = event.diLepton.leg1()
        fill( tr, 'effweight', leg1.weight*leg2.weight) 
        fill( tr, 'weight', event.eventWeight ) 
        if hasattr(event, 'embedWeight'):
            fill( tr, 'embeddedWeight', event.embedWeight ) 
        
        fill( tr, 'mvis', event.diLepton.mass() ) 
        fill( tr, 'm_sv', event.diLepton.massSVFit() ) 
        fill( tr, 'm_sv_Up', -1) #? tau up 3%
        fill( tr, 'm_sv_Down', -1) #? tau down 3%
        
        fill( tr, 'pt_1', leg1.pt())
        fill( tr, 'phi_1', leg1.phi())
        fill( tr, 'eta_1', leg1.eta())
        fill( tr, 'm_1', leg1.mass())
        fill( tr, 'q_1', leg1.charge())
        fill( tr, 'iso_1', leg1.relIsoAllChargedDB05())
        fill( tr, 'mva_1', leg1.mvaId()) # should be filled for e-tau: mva id
        fill( tr, 'd0_1', leg1.dxy() )
        fill( tr, 'dZ_1', leg1.dz())
        fill( tr, 'passid_1', leg1.tightId() )
        fill( tr, 'passiso_1', leg1.relIsoAllChargedDB05()<0.1 )
        fill( tr, 'mt_1', event.diLepton.mTLeg2())
        fill( tr, 'trigweight_1', leg1.triggerWeight)
        fill( tr, 'idweight_1', leg1.idWeight)
        fill( tr, 'isoweight_1', leg1.isoWeight)
        
        fill( tr, 'pt_2', leg2.pt())
        fill( tr, 'phi_2', leg2.phi())
        fill( tr, 'eta_2', leg2.eta())
        fill( tr, 'm_2', leg2.mass())
        fill( tr, 'q_2', leg2.charge())
        fill( tr, 'iso_2', leg2.tauID("byRawIsoMVA"))
        fill( tr, 'mva_2', leg2.tauID("againstElectronMVA")) # we probably need the mva output here. I guess this is a working point
        fill( tr, 'passid_2',  1)
        fill( tr, 'passiso_2', 1)
        fill( tr, 'mt_2', event.diLepton.mTLeg1())
        fill( tr, 'trigweight_2', leg2.triggerWeight)
        fill( tr, 'idweight_2', leg2.idWeight)
        fill( tr, 'isoweight_2', leg2.isoWeight)
        
        met = self.handles['pfmetraw'].product()[0]
        fill( tr, 'met', met.pt()) # raw 
        fill( tr, 'metphi', met.phi()) # raw
        fill( tr, 'mvamet', event.diLepton.met().pt()) 
        fill( tr, 'mvametphi', event.diLepton.met().phi())
        fill( tr, 'pzetavis', event.diLepton.pZetaVis())
        fill( tr, 'pzetamiss', event.diLepton.pZetaMET())

        #metsig = self.handles['pfmetsig'].product().significance()
        metsig = event.diLepton.metSig().significance()
        # if hasattr(event.diLepton, 'mvaMetSig'):
        #     metsig = event.diLepton.mvaMetSig.significance()
        fill( tr, 'metcov00', metsig(0,0))
        fill( tr, 'metcov01', metsig(0,1))
        fill( tr, 'metcov10', metsig(1,0))
        fill( tr, 'metcov11', metsig(1,1))

        if hasattr( event.diLepton, 'mvaMetSig'):
            mvametsig = event.diLepton.mvaMetSig.significance()
            fill( tr, 'mvacov00', mvametsig(0,0))
            fill( tr, 'mvacov01', mvametsig(0,1))
            fill( tr, 'mvacov10', mvametsig(1,0))
            fill( tr, 'mvacov11', mvametsig(1,1))
        nJets = len(event.cleanJets30)
        nJetsPt20 = len(event.cleanJets)
        jets = event.cleanJets30
        if self.cfg_ana.pt20:
            jets = event.cleanJets
        if nJets>=1:
            j1 = jets[0]
            fill( tr, 'jpt_1', j1.pt())
            fill( tr, 'jeta_1', j1.eta())
            fill( tr, 'jphi_1', j1.phi())
            fill( tr, 'jptraw_1', j1.pt()*j1.rawFactor())
            fill( tr, 'jptunc_1', j1.uncOnFourVectorScale())
            fill( tr, 'jmva_1', j1.puMva("full"))
            # 2 is the loose working point
            fill( tr, 'jpass_1', j1.passPuJetId("full", 2))

        if nJets>=2:
            j2 = jets[1]
            fill( tr, 'jpt_2', j2.pt())
            fill( tr, 'jeta_2', j2.eta())
            fill( tr, 'jphi_2', j2.phi())
            fill( tr, 'jptraw_2', j2.pt()*j2.rawFactor())
            fill( tr, 'jptunc_2', j2.uncOnFourVectorScale())
            fill( tr, 'jmva_2', j2.puMva("full"))
            # 2 is the loose working point
            fill( tr, 'jpass_2', j2.passPuJetId("full", 2))

        if len(event.cleanBJets)>0:
            fill( tr, 'bpt', event.cleanBJets[0].pt())
            fill( tr, 'beta', event.cleanBJets[0].eta())
            fill( tr, 'bphi', event.cleanBJets[0].phi())

        if hasattr( event, 'vbf'):
            vbf = event.vbf
            # import pdb; pdb.set_trace()
            fill( tr, 'mjj', vbf.mjj)
            fill( tr, 'jdeta', abs(vbf.deta))
            fill( tr, 'njetingap', len(vbf.centralJets))
            fill( tr, 'mva', vbf.mva)
            
            # add the following variables to the vbf object in VBFAnalyzer
            fill( tr, 'jdphi', abs(vbf.dphi))
            fill( tr, 'dijetpt', vbf.dijetpt)
            fill( tr, 'dijetphi', vbf.dijetphi)
            fill( tr, 'hdijetphi', abs(vbf.dphidijethiggs)) # higgs p = p1 + p2 + met
            fill( tr, 'visjeteta', vbf.visjeteta)
            fill( tr, 'ptvis', vbf.ptvis)

        fill( tr, 'nbtag', len(event.cleanBJets))
        fill( tr, 'njets', nJets)
        fill( tr, 'njetspt20', nJetsPt20)
            
        self.tree.tree.Fill()
        return True
