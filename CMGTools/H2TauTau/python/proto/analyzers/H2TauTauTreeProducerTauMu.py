from PhysicsTools.Heppy.analyzers.core.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.H2TauTau.proto.analyzers.ntuple import *
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle


class H2TauTauTreeProducerTauMu( TreeAnalyzerNumpy ):
    '''Tree producer for the H->tau tau analysis.

    Some of the functions in this class should be made available to everybody.'''
    
    def declareVariables(self, setup):

        tr = self.tree

        var( tr, 'run', int)
        var( tr, 'lumi', int)
        var( tr, 'evt', int)
        var( tr, 'NUP', int)
        var( tr, 'rho')

        bookDiLepton(tr)

        var( tr, 'pfmet')

        bookParticle(tr, 'diTau')
        bookTau(tr, 'l1')
        bookGenParticle(tr, 'l1_gen')
        var(tr, 'l1_gen_vis_pt')
        var(tr, 'l1_gen_vis_eta')
        var(tr, 'l1_gen_vis_phi')
        bookMuon(tr, 'l2')
        bookGenParticle(tr, 'l2_gen')
        bookParticle(tr, 'l1Jet')
        bookParticle(tr, 'l2Jet')

        var( tr, 'nJets')
        var( tr, 'nJets20')
        bookJet(tr, 'jet1')
        bookJet(tr, 'jet2')

        # b jets
        var( tr, 'nBJets')
        var(tr, 'nCSVLJets')
        bookJet(tr, 'bjet1')

        bookVBF( tr, 'VBF' )
        #bookJet(tr, 'cjet') # leading central veto jet from VBF

        var( tr, 'weight')
        var( tr, 'vertexWeight')
        var( tr, 'embedWeight')
        var( tr, 'hqtWeight')
        var( tr, 'hqtWeightUp')
        var( tr, 'hqtWeightDown')
        var( tr, 'NJetWeight')

        var( tr, 'tauFakeRateWeight')
        var( tr, 'tauFakeRateWeightUp')
        var( tr, 'tauFakeRateWeightDown')

        var( tr, 'nVert')

        var( tr, 'isFake')
        var( tr, 'isSignal')
        var( tr, 'leptonAccept')
        var( tr, 'thirdLeptonVeto')

        var(tr, 'genMass')
        var(tr, 'genMet')
        var(tr, 'genMex')
        var(tr, 'genMey')

        self.maxNGenJets = 4
        for i in range(0, self.maxNGenJets):
            bookGenParticle(tr, 'genQG_{i}'.format(i=i))


         
    def declareHandles(self):
        super(H2TauTauTreeProducerTauMu, self).declareHandles()
        self.handles['pfmetraw'] = AutoHandle(
                'slimmedMETs',
                'std::vector<pat::MET>' 
                )
        
    def process(self, event):
        self.readCollections( event.input )
                
        tr = self.tree
        tr.reset()

        fill( tr, 'run', event.run) 
        fill( tr, 'lumi',event.lumi)
        fill( tr, 'evt', event.eventId)

        # This is only relevant for the W/Z+N-jet samples
        if hasattr(event, 'NUP'):
            fill( tr, 'NUP', event.NUP)
        fill( tr, 'rho', event.rho)

        fillDiLepton( tr, event.diLepton )

        # import pdb; pdb.set_trace()
        pfmet = self.handles['pfmetraw'].product()[0]
        fill(tr, 'pfmet', pfmet.pt())


        fillParticle(tr, 'diTau', event.diLepton)
        fillTau(tr, 'l1', event.diLepton.leg1() )
        genTau = event.diLepton.leg1().genParticle()
        if genTau:
            fillGenParticle(tr, 'l1_gen', genTau)

            def getAllDaughters(p, l):
                for i in range(0, p.numberOfDaughters()):
                    d = p.daughter(i)
                    l.append(d)
                    getAllDaughters(d, l)

            p4vis = genTau.p4()
            daughters = []
            getAllDaughters(genTau, daughters)
            invisDaughters = [p for p in daughters if abs(p.pdgId()) in [12, 14, 16]]
            for d in invisDaughters:
                p4vis -= d.p4()

            fill(tr, 'l1_gen_vis_pt', p4vis.pt())
            fill(tr, 'l1_gen_vis_eta', p4vis.eta())
            fill(tr, 'l1_gen_vis_phi', p4vis.phi())


        fillMuon(tr, 'l2', event.diLepton.leg2() )
        if event.diLepton.leg2().genParticle():
            fillGenParticle(tr, 'l2_gen', event.diLepton.leg2().genParticle())
        fillParticle(tr, 'l1Jet', event.diLepton.leg1().jet )
        fillParticle(tr, 'l2Jet', event.diLepton.leg2().jet )


        fill(tr, 'nJets20', len(event.cleanJets) )
        nJets30 = len(event.cleanJets30)
        fill(tr, 'nJets', nJets30 )
        nJets = len(event.cleanJets)
        if nJets>=1:
             fillJet(tr, 'jet1', event.cleanJets[0] )
        if nJets>=2:
             fillJet(tr, 'jet2', event.cleanJets[1] )

        nBJets = len(event.cleanBJets)
        if nBJets>0:
             fillJet(tr, 'bjet1', event.cleanBJets[0] )                     
        fill(tr, 'nBJets', nBJets)

        # JAN - directly count CSVL jets as done in
        # other groups. May apply SFs as for CSVM jets
        # after rewriting BTagSF module in the future
        nCSVLJets = 0
        for jet in event.cleanJets:
            if jet.btag('combinedSecondaryVertexBJetTags') > 0.244:
                    nCSVLJets += 1
        fill(tr, 'nCSVLJets', nCSVLJets)

        if hasattr( event, 'vbf'):
             fillVBF( tr, 'VBF', event.vbf )
             #if len(event.vbf.centralJets) > 0:
             #        fillJet(tr, 'cjet', event.vbf.centralJets[0])

        fill(tr, 'weight', event.eventWeight)
        fill(tr, 'embedWeight', event.embedWeight)
        fill(tr, 'hqtWeight', event.higgsPtWeight)
        fill(tr, 'hqtWeightUp', event.higgsPtWeightUp)
        fill(tr, 'hqtWeightDown', event.higgsPtWeightDown)
        if hasattr( event, 'NJetWeight'):
            fill(tr, 'NJetWeight', event.NJetWeight)

        fill(tr, 'tauFakeRateWeightUp', event.tauFakeRateWeightUp)
        fill(tr, 'tauFakeRateWeightDown', event.tauFakeRateWeightDown)
        fill(tr, 'tauFakeRateWeight', event.tauFakeRateWeight)



        if hasattr( event, 'vertexWeight'): 
            fill(tr, 'vertexWeight', event.vertexWeight)
            fill(tr, 'nVert', len(event.vertices) ) 
            
        fill(tr, 'isFake', event.isFake)
        fill(tr, 'isSignal', event.isSignal)
        fill(tr, 'leptonAccept',        event.leptonAccept)
        fill(tr, 'thirdLeptonVeto', event.thirdLeptonVeto)


        if hasattr(event, 'genParticles'):
            # Get Higgs/Z mass
            for p in event.genParticles:
                if p.pdgId() in [23, 25, 35, 36, 37]:
                    fill(tr, 'genMass', p.mass())
                    break

            # Calculate gen MET
            neutrinos = [p for p in event.genParticles if abs(p.pdgId()) in (12, 14, 16)]
            if neutrinos:
                genMet = neutrinos[0].p4()
                for p in neutrinos[1:]:
                    genMet += p.p4()
                fill(tr, 'genMet', p.pt())
                fill(tr, 'genMex', p.px())
                fill(tr, 'genMey', p.py())
            else:
                fill(tr, 'genMet', 0.)
                fill(tr, 'genMex', 0.)
                fill(tr, 'genMey', 0.)

            # Fill hard quarks/gluons
            quarksGluons = [p for p in event.genParticles if abs(p.pdgId()) in (1, 2, 3, 4, 5, 21) and p.status() == 3 and (p.numberOfDaughters() == 0 or p.daughter(0).status() != 3)]
            quarksGluons.sort(key=lambda x: -x.pt())
            for i in range(0, min(self.maxNGenJets, len(quarksGluons))):
                fillGenParticle(tr, 'genQG_{i}'.format(i=i), quarksGluons[i])

        self.tree.tree.Fill()
