import random
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.physicsobjects.PhysicsObjects import Jet, GenJet
from PhysicsTools.HeppyCore.utils.deltar import cleanObjectCollection, matchObjectCollection
from PhysicsTools.Heppy.physicsobjects.VBF import VBF
from PhysicsTools.Heppy.physicsutils.BTagSF import BTagSF
from PhysicsTools.Heppy.physicsobjects import GenParticle
from PhysicsTools.HeppyCore.utils.deltar import deltaR2
from PhysicsTools.Heppy.utils.cmsswRelease import isNewerThan

class VBFAnalyzer( Analyzer ):
    '''Analyze jets, and in particular VBF.
    This analyzer filters the jets that do not correspond to the di-lepton
    legs, and stores in the event:
    - jets: all jets passing the pt and eta cuts
    - cleanJets: the collection of clean jets
    - vbf: the VBF object with all necessary variables, if it can be defined
    - bJets: the bjets passing testBJet (see this method)

    Example configuration:

    vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    jetCol = 'cmgPFJetSel',
    jetPt = 30,
    jetEta = 5.0,
    btagSFseed = 123456,
    **vbfKwargs
    )


    '''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(VBFAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)
        self.btagSF = BTagSF (cfg_ana.btagSFseed)
        # import pdb; pdb.set_trace()
        self.is2012 = isNewerThan('CMSSW_5_2_0')

    def declareHandles(self):
        super(VBFAnalyzer, self).declareHandles()

        self.handles['jets'] = AutoHandle( self.cfg_ana.jetCol,
                                           'std::vector<cmg::PFJet>' )
        
        if self.cfg_comp.isMC:
            self.mchandles['genJets'] = AutoHandle('genJetSel',
                                                   'std::vector<cmg::PhysicsObjectWithPtr< edm::Ptr<reco::GenJet> > >')

        if self.cfg_comp.isMC and ("BB" in self.cfg_comp.name):
            self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )

    def beginLoop(self, setup):
        super(VBFAnalyzer,self).beginLoop(setup)
        self.counters.addCounter('VBF')
        count = self.counters.counter('VBF')
        count.register('all events')
        count.register('at least 2 good jets')
        count.register('at least 2 clean jets')
        count.register('M_jj > {cut:3.1f}'.format(cut=self.cfg_ana.Mjj))
        count.register('delta Eta > {cut:3.1f}'.format(cut=self.cfg_ana.deltaEta) )
        count.register('no central jets')
        
    def process(self, event):
        
        self.readCollections( event.input )
        cmgJets = self.handles['jets'].product()

        allJets = []
        event.jets = []
        event.bJets = []
        event.cleanJets = []
        event.cleanBJets = []

        leg1 = event.diLepton.leg1()
        leg2 = event.diLepton.leg2()

        genJets = None
        if self.cfg_comp.isMC:
            genJets = map( GenJet, self.mchandles['genJets'].product() ) 
     
        for cmgJet in cmgJets:
            jet = Jet( cmgJet )
            allJets.append( jet )
            if self.cfg_comp.isMC and hasattr( self.cfg_comp, 'jetScale'):
                scale = random.gauss( self.cfg_comp.jetScale,
                                      self.cfg_comp.jetSmear )
                jet.scaleEnergy( scale )

            if self.testJet( jet ):
                event.jets.append(jet)
            if self.testBJet(jet):
                event.bJets.append(jet)
            if genJets:
                # Use DeltaR = 0.25 matching like JetMET
                pairs = matchObjectCollection( [jet], genJets, 0.25*0.25)
                if pairs[jet] is None:
                    pass
                else:
                    jet.matchedGenJet = pairs[jet] 

            #Add JER correction for MC jets. Requires gen-jet matching
            if self.cfg_comp.isMC and hasattr(self.cfg_ana, 'jerCorr') and self.cfg_ana.jerCorr:
                self.jerCorrection(jet)

        self.counters.counter('VBF').inc('all events')

        event.cleanJets, dummy = cleanObjectCollection( event.jets,
                                                        masks = [leg1,
                                                                 leg2 ],
                                                        deltaRMin = 0.5 )
        

        event.cleanBJets, dummy = cleanObjectCollection( event.bJets,
                                                         masks = [leg1,
                                                                  leg2 ],
                                                         deltaRMin = 0.5 )        

        pairs = matchObjectCollection( [ leg1,
                                         leg2 ], allJets, 0.5*0.5)

        # associating a jet to each leg
        leg1.jet = pairs[leg1]
        leg2.jet = pairs[leg2]
        if leg1.jet is None: #COLIN: I don't understand the code below...
            leg1.jet = leg1
        if leg2.jet is None:
            leg2.jet = leg2        

        # associating a leg to each clean jet
        invpairs = matchObjectCollection( event.cleanJets, [ leg1,leg2 ], 99999. )
        for jet in event.cleanJets:
            leg = invpairs[jet]
            jet.leg = leg

        for jet in event.cleanJets:
            jet.matchGenParton=999.0

        if self.cfg_comp.isMC and "BB" in self.cfg_comp.name:
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
            for gen in genParticles:
                if abs(gen.pdgId())==5 and gen.mother() and abs(gen.mother().pdgId())==21:
                    for jet in event.cleanJets:
                        dR=deltaR2(jet.eta(), jet.phi(), gen.eta(), gen.phi() )
                        if dR<jet.matchGenParton:
                            jet.matchGenParton=dR

        event.jets30 = [jet for jet in event.jets if jet.pt()>30]
        event.cleanJets30 = [jet for jet in event.cleanJets if jet.pt()>30]
        
        if len( event.jets30 )>=2:
            self.counters.counter('VBF').inc('at least 2 good jets')
               
        if len( event.cleanJets30 )>=2:
            self.counters.counter('VBF').inc('at least 2 clean jets')

        if len(event.cleanJets)<2:
            return True

        event.vbf = VBF( event.cleanJets, event.diLepton,
                         None, self.cfg_ana.cjvPtCut )
        if event.vbf.mjj > self.cfg_ana.Mjj:
            self.counters.counter('VBF').inc('M_jj > {cut:3.1f}'.format(cut=self.cfg_ana.Mjj) )
        else:
            return True 
        if abs(event.vbf.deta) > self.cfg_ana.deltaEta:
            self.counters.counter('VBF').inc('delta Eta > {cut:3.1f}'.format(cut=self.cfg_ana.deltaEta) )
        else:
            return True 
        if len(event.vbf.centralJets)==0:
            self.counters.counter('VBF').inc('no central jets')
        else:
            return True
        
        return True
    
    def jerCorrection(self, jet):
        ''' Adds JER correction according to first method at
        https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution

        Requires some attention when genJet matching fails.
        '''
        if not hasattr(jet, 'matchedGenJet'):
            return

        corrections = [0.052, 0.057, 0.096, 0.134, 0.288]
        maxEtas = [0.5, 1.1, 1.7, 2.3, 5.0]
        eta = abs(jet.eta())

        for i, maxEta in enumerate(maxEtas):
            if eta < maxEta:
                pt = jet.pt()
                deltaPt = (pt - jet.matchedGenJet.pt()) * corrections[i]
                totalScale = (pt + deltaPt) / pt
                if totalScale < 0.:
                    totalScale = 0.
                jet.scaleEnergy(totalScale)
                break

    def testJetID(self, jet):
        # Use new PU jet ID working point
        jet.puJetIdPassed = jet.puJetId(wp53x=True)

        jet.pfJetIdPassed = jet.looseJetId()        
        if self.cfg_ana.relaxJetId:
            return True
        else:
            return jet.puJetIdPassed and jet.pfJetIdPassed
        
        
    def testJet( self, jet ):
        # 2 is loose pile-up jet id
        return jet.pt() > self.cfg_ana.jetPt and \
               abs( jet.eta() ) < self.cfg_ana.jetEta and \
               self.testJetID(jet)
               # jet.passPuJetId('full', 2)


    def testBJet(self, jet):
        # medium csv working point
        # https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP#B_tagging_Operating_Points_for_3
        jet.btagMVA = jet.btag("combinedSecondaryVertexBJetTags")
        jet.btagFlag = self.btagSF.BTagSFcalc.isbtagged(jet.pt(), 
                          jet.eta(),
                          jet.btag("combinedSecondaryVertexBJetTags"),
                          abs(jet.partonFlavour()),
                          not self.cfg_comp.isMC,
                          0,0,
                          self.is2012 )
        return jet.pt()>20 and \
               abs( jet.eta() ) < 2.4 and \
               jet.btagFlag and \
               self.testJetID(jet)
               # jet.passPuJetId('full', 2)
               # jet.btag("combinedSecondaryVertexBJetTags")>0.679 and \
