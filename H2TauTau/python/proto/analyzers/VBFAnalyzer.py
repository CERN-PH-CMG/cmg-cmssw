import random
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection
from CMGTools.RootTools.physicsobjects.VBF import VBF
from CMGTools.RootTools.statistics.Counter import Counter, Counters
# from CMGTools.H2TauTau.proto.VBFMVA import VBFMVA
from CMGTools.H2TauTau.proto.VBFMVA import VBFMVA2012 as VBFMVA
from CMGTools.H2TauTau.proto.BtagSF import BtagSF
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.utils.DeltaR import deltaR2
from CMGTools.Common.Tools.cmsswRelease import isNewerThan

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
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG_HCP_42X.weights.xml',
    jetCol = 'cmgPFJetSel',
    jetPt = 30,
    jetEta = 5.0,
    btagSFseed = 123456,
    **vbfKwargs
    )


    '''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(VBFAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)
        self.vbfMva = VBFMVA (cfg_ana.vbfMvaWeights)
        self.btagSF = BtagSF (cfg_ana.btagSFseed)
        # import pdb; pdb.set_trace()
        self.is2012 = isNewerThan('CMSSW_5_2_0')

    def declareHandles(self):
        super(VBFAnalyzer, self).declareHandles()

        self.handles['jets'] = AutoHandle( self.cfg_ana.jetCol,
                                           'std::vector<cmg::PFJet>' )
        if self.cfg_comp.isMC and ("BB" in self.cfg_comp.name):
            self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )

    def beginLoop(self):
        super(VBFAnalyzer,self).beginLoop()
        self.counters.addCounter('VBF')
        count = self.counters.counter('VBF')
        count.register('all events')
        count.register('at least 2 good jets')
        count.register('at least 2 clean jets')
        count.register('M_jj > {cut:3.1f}'.format(cut=self.cfg_ana.Mjj))
        count.register('delta Eta > {cut:3.1f}'.format(cut=self.cfg_ana.deltaEta) )
        count.register('no central jets')
        
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )
        cmgJets = self.handles['jets'].product()

        allJets = []
        event.jets = []
        event.bJets = []
        event.cleanJets = []
        event.cleanBJets = []

        leg1 = event.diLepton.leg1()
        leg2 = event.diLepton.leg2()
     
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
        leg1.jet = pairs[leg1]
        leg2.jet = pairs[leg2]
        if leg1.jet is None:
            leg1.jet = leg1
        if leg2.jet is None:
            leg2.jet = leg2

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

        event.vbf = VBF( event.cleanJets, event.diLepton, self.vbfMva.vbfMvaCalc)
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
        
        
    def testJet( self, jet ):
        # 2 is loose pile-up jet id
        return jet.pt() > self.cfg_ana.jetPt and \
               abs( jet.eta() ) < self.cfg_ana.jetEta and \
               jet.getSelection('cuts_looseJetId') and \
               jet.puJetId()
               # jet.passPuJetId('full', 2)

    def testBJet(self, jet):
        # medium csv working point
        # https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP#B_tagging_Operating_Points_for_3
        jet.btagMVA = jet.btag("combinedSecondaryVertexBJetTags")
        jet.btagFlag = self.btagSF.BtagSFcalc.isbtagged(jet.pt(), 
                          jet.eta(),
                          jet.btag("combinedSecondaryVertexBJetTags"),
                          abs(jet.partonFlavour()),
                          not self.cfg_comp.isMC,
                          0,0,
                          self.is2012 )
        return jet.pt()>20 and \
               abs( jet.eta() ) < 2.4 and \
               jet.btagFlag and \
               jet.getSelection('cuts_looseJetId') and \
               jet.puJetId()
               # jet.passPuJetId('full', 2)
               # jet.btag("combinedSecondaryVertexBJetTags")>0.679 and \
