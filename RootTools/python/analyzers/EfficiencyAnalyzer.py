from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average
from CMGTools.RootTools.statistics.Counter import Counter
from CMGTools.RootTools.statistics.Histograms import Histograms
from CMGTools.RootTools.physicsobjects.PhysicsObjects import PhysicsObject, GenParticle
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection

from ROOT import TH1F, TFile




class PhaseSpace( object ):
    def __init__(self, name, etaRanges, ptRanges=None):
        self.name = name
        self.etaRanges = etaRanges
        self.ptRanges = ptRanges

    def etaPass(self, eta):
        for range in self.etaRanges:
            if eta>range[0] and eta<range[1]:
                return True
        return False

ECAL_barrel = PhaseSpace('EB',
                         [(-1.4, 1.4)])
ECAL = PhaseSpace('ECAL',
                  [(-2.9, 2.9)])
ECAL_endcaps = PhaseSpace('EE',
                          [(-2.9, -1.6),
                           (1.6, 2.9) ])

muon_barrel = PhaseSpace('MB',
                         [(-0.9, 0.9)])
muon_transition = PhaseSpace('MT',
                             [(-1.2, -0.9),
                              (0.9, 1.2) ])
muon_endcaps = PhaseSpace('ME',
                          [ (-2.5, -1.2),
                            (1.2, 2.5) ])
muon = PhaseSpace('MUON',
                  [(-2.5, 2.5)])

electronPhaseSpaces = [ECAL_barrel, ECAL_endcaps, ECAL]
muonPhaseSpaces = [muon_barrel, muon_transition, muon_endcaps, muon]


class EfficiencyHistograms( Histograms ):
    '''
    Define variable binning in pT
    Define detector boundaries in a generic way
    Efficiency plotter
    '''
    def __init__(self, name, space):
        self.h_pt = TH1F(name + '_h_pt', '', 50, 0, 200)
        self.h_eta = TH1F(name + '_h_eta', '', 50, -6, 6)
        self.h_phi = TH1F(name + '_h_phi', '', 50, -6.3, 6.3)
        self.space = space 
        super( EfficiencyHistograms, self).__init__(name)
 
    def ptPass(self, pt):
        ptMin = 10.
        return pt>ptMin  

    def fillParticle(self, particle):
        eta = particle.eta()
        pt = particle.pt()

        #COLIN not good! should test only the gen particle.
        
        # temp: 
        if not self.space.etaPass(eta) or \
           not self.ptPass(pt):
            return
        
        if self.space.etaPass(eta):
            self.h_pt.Fill(pt)
        if self.ptPass(pt):
                self.h_eta.Fill( eta )
        if self.space.etaPass(eta) and \
           self.ptPass(pt):
            self.h_phi.Fill( particle.phi() )
            
    def fillParticles(self, particles):
        for particle in particles:
            self.fillParticle( particle )



class EfficiencyAnalyzer( Analyzer ):
    '''A simple jet analyzer for Pietro.'''

    
    def declareHandles(self):
        super(EfficiencyAnalyzer, self).declareHandles()

        instance = self.cfg_ana.instance
        type = self.cfg_ana.type
        self.handles['rec'] =  AutoHandle(
            instance,
            type 
            )

        geninstance = 'genLeptonsStatus1'
        gentype = 'std::vector<reco::GenParticle>'
        self.mchandles['gen'] =  AutoHandle(
            geninstance,
            gentype 
            )
  
    def beginLoop(self):
        super(EfficiencyAnalyzer,self).beginLoop()
        self.file = TFile( '/'.join( [self.looperName, 'EfficiencyAnalyzer.root']),
                           'recreate')

        print self.cfg_ana
        self.phaseSpaces = None
        if self.cfg_ana.genPdgId==13:
            self.phaseSpaces = muonPhaseSpaces
        elif self.cfg_ana.genPdgId==11:
            self.phaseSpaces = electronPhaseSpaces

        for space in self.phaseSpaces:
            space.denomHistos = EfficiencyHistograms('_'.join([space.name,
                                                               'Denom']),
                                                     space )
            space.numHistos = EfficiencyHistograms('_'.join([space.name,
                                                             'Num']),
                                                   space)
            space.counter = Counter( space.name )
            
        # self.denomHistos = EfficiencyHistograms('Denom')
        # self.numHistos = EfficiencyHistograms('Num')
        # self.counters.addCounter('effcount') 


    def process(self, iEvent, event):
        self.readCollections( iEvent )

        rec = self.handles['rec'].product()
        gen = self.mchandles['gen'].product()

        recselstr = None
        if hasattr( self.cfg_ana, 'recsel' ):
            recselstr = self.cfg_ana.recsel 

        event.rec = rec
        if recselstr is not None:
            event.recsel = [ PhysicsObject(obj) for obj in rec if obj.getSelection( recselstr ) ]
        else:
            event.recsel = event.rec

        genpdgid = self.cfg_ana.genPdgId
        event.gensel = [ GenParticle(obj) for obj in gen if abs(obj.pdgId())==genpdgid]

        pairs = matchObjectCollection( event.gensel, event.recsel, 0.1)
        event.genmatched = [ obj for obj in pairs.values() if obj is not None]

##         for gen in event.gensel:
##             self.counters.counter('effcount').inc('all muons')
##             if pairs[gen] is not None:
##                 self.counters.counter('effcount').inc('passing cuts')
 
        for space in self.phaseSpaces:
            space.denomHistos.fillParticles( event.gensel )
            space.numHistos.fillParticles( event.genmatched )
            # space.counter.inc('passed')
        
        
    def write(self):
        for space in self.phaseSpaces:
            space.denomHistos.Write( self.file )
            space.numHistos.Write( self.file )
