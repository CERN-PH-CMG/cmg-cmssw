from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.statistics.counter import Counter, Counters
from math import floor, exp, sqrt
import re
import os
import ROOT
        
class topPtAnalyzer( Analyzer ):
    """Get information for susy parameter scans    """
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(topPtAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.par_a = cfg_ana.par_a
        self.par_b = cfg_ana.par_b

    #---------------------------------------------
    # DECLARATION OF HANDLES OF GEN LEVEL OBJECTS 
    #---------------------------------------------
        

    def declareHandles(self):
        super(topPtAnalyzer, self).declareHandles()

        #mc information
        self.mchandles['genParticles'] = AutoHandle( 'prunedGenParticles',
                                                     'std::vector<reco::GenParticle>' )        
    def beginLoop(self, setup):
        super(topPtAnalyzer,self).beginLoop(setup)
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')
        count.register('sumW (ISR pT weight only)')
        count.register('sumW up (ISR pT weight only)')
        count.register('sumW down (ISR pT weight only)')
        count.register('sumW (top pT weight only)')
        count.register('sumW up (top pT weight only)')
        count.register('sumW down (top pT weight only)')
        count.register('sumW (antitop pT weight only)')
        count.register('sumW up (antitop pT weight only)')
        count.register('sumW down (antitop pT weight only)')



    def process(self, event):
        # if not MC, nothing to do
        if not self.cfg_comp.isMC: 
            return True

        self.readCollections( event.input )

        self.counters.counter('events').inc('all events')

        setattr(event,"isrTopWeight",1.)
        setattr(event,"isrTopWeight_Up",1.)
        setattr(event,"isrTopWeight_Down",1.)
        setattr(event,"isrAntitopWeight",1.)
        setattr(event,"isrAntitopWeight_Up",1.)
        setattr(event,"isrAntitopWeight_Down",1.)
        setattr(event,"isrBackgroundWeight",1.)
        setattr(event,"isrBackgroundWeight_Up",1.)
        setattr(event,"isrBackgroundWeight_Down",1.)
        setattr(event,"top_genPt",None)
        setattr(event,"antitop_genPt",None)

        p4_top, p4_antitop = [], []
        for p in event.genParticles:
            id = p.pdgId()
            if abs(id) != 6: continue
            if id == +6 and p.isLastCopy(): p4_top.append(p.p4())
            elif id == -6 and p.isLastCopy(): p4_antitop.append(p.p4())
        
        if len(p4_top) == 1 and len(p4_antitop) == 1:
            tlv_top = p4_top[0]
            top_pt = tlv_top.pt()
            setattr(event,"top_genPt",top_pt)
            tlv_antitop = p4_antitop[0]
            antitop_pt = tlv_antitop.pt()
            setattr(event,"antitop_genPt",antitop_pt)
            
            # Compute ISR weight: SF(x)=exp(a+bx)
            SF_top, SF_antitop = 1., 1.
            if top_pt<=400.: SF_top = exp(self.par_a+self.par_b*top_pt)
            else: SF_top = exp(self.par_a+self.par_b*400.)
            if antitop_pt<=400.: SF_antitop = exp(self.par_a+self.par_b*antitop_pt)
            else: SF_antitop = exp(self.par_a+self.par_b*400.)

            diff_top = SF_top-1.            
            SF_top_Down = 1.
            SF_top_Up = SF_top + diff_top
            setattr(event,"isrTopWeight",SF_top)
            setattr(event,"isrTopWeight_Up",SF_top_Up)
            setattr(event,"isrTopWeight_Down",SF_top_Down)
            diff_antitop = SF_antitop-1.            
            SF_antitop_Down = 1.
            SF_antitop_Up = SF_antitop + diff_antitop
            setattr(event,"isrAntitopWeight",SF_antitop)
            setattr(event,"isrAntitopWeight_Up",SF_antitop_Up)
            setattr(event,"isrAntitopWeight_Down",SF_antitop_Down)


            # Compute the total weight
            tot_weight = sqrt(SF_top*SF_antitop)
            tot_weight_down = 1.
            diff = tot_weight-1.
            tot_weight_up = tot_weight + diff
            setattr(event,"isrBackgroundWeight",tot_weight)
            setattr(event,"isrBackgroundWeight_Up",tot_weight_up)
            setattr(event,"isrBackgroundWeight_Down",tot_weight_down)

        self.counters.counter('events').inc('sumW (ISR pT weight only)',event.isrBackgroundWeight)
        self.counters.counter('events').inc('sumW up (ISR pT weight only)',event.isrBackgroundWeight_Up)
        self.counters.counter('events').inc('sumW down (ISR pT weight only)',event.isrBackgroundWeight_Down)
        self.counters.counter('events').inc('sumW (top pT weight only)',event.isrTopWeight)
        self.counters.counter('events').inc('sumW up (top pT weight only)',event.isrTopWeight_Up)
        self.counters.counter('events').inc('sumW down (top pT weight only)',event.isrTopWeight_Down)
        self.counters.counter('events').inc('sumW (antitop pT weight only)',event.isrAntitopWeight)
        self.counters.counter('events').inc('sumW up (antitop pT weight only)',event.isrAntitopWeight_Up)
        self.counters.counter('events').inc('sumW down (antitop pT weight only)',event.isrAntitopWeight_Down)
                
            
            

        return True
