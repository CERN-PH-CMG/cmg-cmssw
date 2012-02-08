from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average
from CMGTools.RootTools.statistics.Histograms import Histograms
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle,Jet, GenJet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection

from ROOT import TH1F, TFile

class JetHistograms( Histograms ):
    def __init__(self, name):
        self.h_pt = TH1F(name + '_h_pt', '', 100, 0, 200)
        self.h_dpt = TH1F(name + '_h_dpt', '', 100, 0, 2)
        self.h_eta = TH1F(name + '_h_eta', '', 240, -6, 6)
        super( JetHistograms, self).__init__(name)

    def fillJet(self, jet):
        # import pdb
        # pdb.set_trace()
        self.h_pt.Fill( jet.pt() )
        self.h_eta.Fill( jet.eta() )
        if hasattr(jet, 'gen') and jet.gen is not None:
            self.h_dpt.Fill( jet.pt() / jet.gen.pt() )
            
    def fillEvent(self, jets):
        for jet in jets:
            self.fillJet( jet )


class SimpleJetAnalyzer( Analyzer ):
    '''A simple jet analyzer for Pietro.'''

    def declareHandles(self):
        super(SimpleJetAnalyzer, self).declareHandles()
        self.handles['jets'] =  AutoHandle(
            'cmgPFJetSel',
            'std::vector<cmg::PFJet>'
            )
        self.mchandles['leptons1'] =  AutoHandle(
            'genLeptonsStatus2',
            'std::vector<reco::GenParticle>'
            )
        self.mchandles['genJets'] =  AutoHandle(
            'genJetSel',
            'std::vector< cmg::PhysicsObjectWithPtr< edm::Ptr< reco::GenJet > > >'
            )

    def beginLoop(self):
        super(SimpleJetAnalyzer,self).beginLoop()
        self.file = TFile( '/'.join( [self.looperName, 'test.root']),
                           'recreate')
        self.jetHistos = JetHistograms('Jets')
        self.cleanJetHistos = JetHistograms('CleanJets')

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        jets = self.handles['jets'].product()

        event.jets = [ Jet(jet) for jet in jets if jet.pt()>self.cfg_ana.ptCut ]
        event.genParticles = map( GenParticle, self.mchandles['leptons1'].product() )
        event.genJets = map( GenJet, self.mchandles['genJets'].product() )
        
        event.cleanJets = cleanObjectCollection( event.jets, event.genParticles, 0.2 )

        event.matchedCleanJets = matchObjectCollection( event.cleanJets, event.genJets )
        self.jetHistos.fillEvent( event.jets )

        for jet in event.cleanJets:
            jet.gen = event.matchedCleanJets[ jet ]
        self.cleanJetHistos.fillEvent( event.cleanJets )

        
        
    def write(self):
        self.jetHistos.Write( self.file )
        self.cleanJetHistos.Write( self.file )
