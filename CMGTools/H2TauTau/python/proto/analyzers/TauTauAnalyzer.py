import math
from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import TauTau
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Tau, GenParticle
# from CMGTools.H2TauTau.proto.analyzers.CountLeptons import electronAccept
from CMGTools.RootTools.utils.DeltaR import deltaR2
from ROOT import TFile

class TauTauAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass = TauTau
    LeptonClass = Tau

    def declareHandles(self):
        super(TauTauAnalyzer, self).declareHandles()
        # print 'TauTauAnalyzer.declareHandles'
        self.handles['diLeptons'] = AutoHandle(
            'cmgDiTauCorSVFitFullSel',
            'std::vector<cmg::DiObject<cmg::Tau,cmg::Tau>>'
            )
        #self.handles['leptons'] = AutoHandle(
        #    'cmgTauSel',
        #    'std::vector<cmg::Tau>'
        #    )
        if self.cfg_comp.isMC and ("DY" in self.cfg_comp.name or "W" in self.cfg_comp.name or "Higgsgg" in self.cfg_comp.name):
            self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )
	if "Higgsgg" in self.cfg_comp.name:
	    masspoint=self.cfg_comp.name[7:10]
            self.higgsPtWeightFile=TFile("$CMSSW_BASE/src/CMGTools/H2TauTau/data/weight_ptH_"+masspoint+".root")
            self.higgsPtWeightHistogram=self.higgsPtWeightFile.Get("powheg_weight/weight_hqt_fehipro_fit_"+masspoint)

    def process(self, iEvent, event):
        result = super(TauTauAnalyzer, self).process(iEvent, event)
        
        if result is False:
            return result

        # Simone, that's what you probably want to check.
        # Also have a look at CMGTools.RootTools.physicsobjects.DiObject
        event.genMatched = None
        if self.cfg_comp.isMC and "DY" in self.cfg_comp.name:
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
            leg1DeltaR, leg2DeltaR = event.diLepton.match( event.genParticles ) 
            if leg1DeltaR>-1 and leg1DeltaR < 0.1 and \
               leg2DeltaR>-1 and leg2DeltaR < 0.1:
                event.genMatched = True
            else:
                event.genMatched = False
            event.isPhoton=False
            event.isElectron=False
	    for gen in genParticles:
                if abs(gen.pdgId())==15 and gen.mother().pdgId()==23 and (gen.mother().mass()<80 or gen.mother().mass()>100):
                    event.isPhoton=True
                if abs(gen.pdgId())==11 and gen.mother().pdgId()==23:
                    event.isElectron=True
        if self.cfg_comp.isMC and "W" in self.cfg_comp.name:
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
            genTaus = []
            event.genMatched = False
            for gen in genParticles:
                if abs(gen.pdgId())==15 and gen.mother().pdgId()==24: # W -> tau nu_tau
                    genTaus.append( gen )
            if len(genTaus)>=1:
                dR2leg1Min, event.diLepton.leg1Gen = ( float('inf'), None)
                dR2leg2Min, event.diLepton.leg2Gen = ( float('inf'), None) 
                for genTau in genTaus:
                    dR2leg1 = deltaR2(event.diLepton.leg1().eta(), event.diLepton.leg1().phi(),
                                      genTau.eta(), genTau.phi() )
                    dR2leg2 = deltaR2(event.diLepton.leg2().eta(), event.diLepton.leg2().phi(),
                                      genTau.eta(), genTau.phi() )
                    if dR2leg1 <  dR2leg1Min:
                        dR2leg1Min, event.diLepton.leg1Gen = (dR2leg1, genTau)
                    if dR2leg2 <  dR2leg2Min:
                        dR2leg2Min, event.diLepton.leg2Gen = (dR2leg2, genTau)
                leg1DeltaR = math.sqrt( dR2leg1Min )
                leg2DeltaR = math.sqrt( dR2leg2Min )
                if (leg1DeltaR>-1 and leg1DeltaR < 0.1) or \
                   (leg2DeltaR>-1 and leg2DeltaR < 0.1):
                     event.genMatched = True
		
        if self.cfg_comp.isMC and "Higgsgg" in self.cfg_comp.name:
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
	    higgsPt=-1
	    for gen in genParticles:
                if abs(gen.pdgId())==25:
		    higgsPt = gen.pt()
		    break
	    event.higgsPtWeight = self.higgsPtWeightHistogram.GetBinContent(self.higgsPtWeightHistogram.FindBin(higgsPt))
            event.eventWeight *= event.higgsPtWeight
		
        return True
        

    def testLeg1(self, leg):
        return self.testTau(leg) and \
               super( TauTauAnalyzer, self).testLeg1( leg )


    def testLeg2(self, leg):
        return self.testTau(leg) and \
               super( TauTauAnalyzer, self).testLeg2( leg )


##     def leptonAccept(self, leptons):
##         return electronAccept( leptons )
