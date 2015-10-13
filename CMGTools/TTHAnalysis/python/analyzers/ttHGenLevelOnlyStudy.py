from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle

from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi
from PhysicsTools.Heppy.physicsutils.genutils import *

class LeptonFromGen:
    def __init__(self, physObj):
        self.physObj = physObj
    def __getattr__(self, attr):
        if hasattr(self.physObj, attr):
            return getattr(self.physObj, attr)
        raise RuntimeError, "Missing attribute '%s'" % attr

class JetFromGen:
    def __init__(self, physObj):
        self.physObj = physObj
        self.btag = False
    def btagWP(self,tag):
        return self.btag
    def __getattr__(self, attr):
        if hasattr(self.physObj, attr):
            return getattr(self.physObj, attr)
        raise RuntimeError, "Missing attribute '%s'" % attr

class ttHGenLevelOnlyStudy( Analyzer ):
    """
    Fakes a reco event starting from GEN-only files      
    """
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHGenLevelOnlyStudy,self).__init__(cfg_ana,cfg_comp,looperName)

    #---------------------------------------------
    # DECLARATION OF HANDLES OF GEN LEVEL OBJECTS 
    #---------------------------------------------
    def declareHandles(self):
        super(ttHGenLevelOnlyStudy, self).declareHandles()
        self.mchandles['genParticles'] = AutoHandle( 'genParticles', 'std::vector<reco::GenParticle>', fallbackLabel="prunedGenParticles" )
        self.mchandles['jets'] = AutoHandle( 'ak4GenJets', 'std::vector<reco::GenJet>', mayFail=True, lazy=False )
        self.mchandles['jetsNoNu'] = AutoHandle( 'slimmedGenJets', 'std::vector<reco::GenJet>', mayFail=True, lazy=False )
        self.mchandles['met'] = AutoHandle( 'genMetTrue', 'std::vector<reco::GenMET>', mayFail=True, lazy=False )
        self.mchandles['patmet'] = AutoHandle( 'slimmedMETs', 'std::vector<pat::MET>', mayFail=True, lazy=False )

    def beginLoop(self, setup):
        super(ttHGenLevelOnlyStudy,self).beginLoop(setup)

    def doLeptons(self,event):
            
        event.selectedLeptons = []
        event.genLeptons = []
        for l in event.genParticles: 
            if (abs(l.pdgId()) not in [11,13]) or l.status() != 1: continue
            if not isNotFromHadronicShower(l):
                continue
            event.genLeptons.append(LeptonFromGen(l))
            if abs(l.pdgId()) == 13:
                if l.pt() <= 5 or abs(l.eta()) > 2.4: continue
            if abs(l.pdgId()) == 11:
                if l.pt() <= 7 or abs(l.eta()) > 2.5: continue
            #print "selected lepton pdgId %d, status %d, pt %.1f, eta %.2f" % (l.pdgId(),l.status(),l.pt(),l.eta())
            event.selectedLeptons.append(LeptonFromGen(l))
        event.selectedLeptons.sort(key = lambda l : -l.pt())
        event.genLeptons.sort(key = lambda l : -l.pt())

        event.selectedNeutrinos = []
        for l in event.genParticles: 
            if abs(l.pdgId()) not in [12,14,16] or l.status() != 1: continue
            if not isNotFromHadronicShower(l):
                continue
            event.selectedNeutrinos.append(l)
        event.selectedNeutrinos.sort(key = lambda l : -l.pt())

    def doJets(self,event):
        event.cleanJetsAll = []
        event.cleanJetsFwd = []
        event.cleanJets = []
        if self.mchandles['jetsNoNu'].isValid():
            jets = [j for j in self.mchandles['jetsNoNu'].product()]
        else:
            jets = []
            for j in self.mchandles['jets'].product(): 
                #print "(pass 0) gen jet pt %7.1f eta %+5.2f phi %+5.2f invEnF %.3f " %  (j.pt(), j.eta(), j.phi(), j.invisibleEnergy()/j.energy())
                if j.invisibleEnergy() > 0:
                    p4noNu = j.p4()
                    for idau in xrange(j.numberOfDaughters()):
                        dau = j.daughter(idau)
                        if abs(dau.pdgId()) in [12,14,16]:
                            p4noNu -= dau.p4()
                    j.setP4(p4noNu)
                    #print "          w/o nu pt %7.1f eta %+5.2f phi %+5.2f " %  (j.pt(), j.eta(), j.phi())
                jets.append(j)
        jets.sort(key = lambda l : -l.pt())
        for j in jets: 
            good = True
            if j.pt() < 25: continue
            #print "(pass 1) gen jet pt %7.1f eta %+5.2f phi %+5.2f" %  (j.pt(), j.eta(), j.phi())
            for l in event.selectedLeptons:
                if l.pt() > 10 and deltaR(l.eta(),l.phi(),j.eta(),j.phi()) < 0.4:
                    #print "          ov lep pt %7.1f eta %+5.2f phi %+5.2f dr %.3f" %  (l.pt(), l.eta(), l.phi(), deltaR(l.eta(),l.phi(),j.eta(),j.phi()))
                    good = False; 
                    break
            if not good: continue
            jo = JetFromGen(j)
            event.cleanJetsAll.append(jo)
            if abs(j.eta()) < 2.4:
                event.cleanJets.append(jo)
            else:
                event.cleanJetsFwd.append(jo)
    def doBTag(self,event):
        bs = []
        for gp in event.genParticles:
            if gp.status() != 2: continue
            id = abs(gp.pdgId())
            if id == 5 or ((id % 1000) / 100) == 5 or ((id % 10000)/1000) == 5:
                bs.append(gp)
        for j in event.cleanJetsAll:
            for gp in bs:
                if deltaR(gp.eta(),gp.phi(),j.eta(),j.phi()) < 0.4:
                    gp.btag = True
    def doMET(self,event):
        if self.mchandles['met'].isValid():
            event.met = self.mchandles['met'].product().front()
        else:
            event.met = self.mchandles['patmet'].product().front().genMET()

    def makeZs(self, event, maxLeps):
        event.bestZ1 = [ 0., -1,-1 ]
        event.bestZ1sfss = [ 0., -1,-1 ]
        event.bestZ2 = [ 0., -1,-1, 0. ]
        nlep = len(event.selectedLeptons)
        for i,l1 in enumerate(event.selectedLeptons):
            for j in range(i+1,nlep):
                if j >= maxLeps: break
                l2 = event.selectedLeptons[j]    
                if l1.pdgId() == -l2.pdgId():
                    zmass = (l1.p4() + l2.p4()).M()
                    if event.bestZ1[0] == 0 or abs(zmass - 91.188) < abs(event.bestZ1[0] - 91.188):
                        event.bestZ1 = [ zmass, i, j ]
                if l1.pdgId() == l2.pdgId():
                    zmass = (l1.p4() + l2.p4()).M()
                    if event.bestZ1sfss[0] == 0 or abs(zmass - 91.188) < abs(event.bestZ1sfss[0] - 91.188):
                        event.bestZ1sfss = [ zmass, i, j ]
        if event.bestZ1[0] != 0 and nlep > 3:
            for i,l1 in enumerate(event.selectedLeptons):
                if i == event.bestZ1[1]: continue
                for j in range(i+1,nlep):
                    if j >= maxLeps: break
                    if j == event.bestZ1[2]: continue
                    l2 = event.selectedLeptons[j]    
                    if l1.pdgId() == -l2.pdgId():
                        if l1.pt() + l2.pt() > event.bestZ2[0]:
                            event.bestZ2 = [ l1.pt() + l2.pt(), i, j, (l1.p4() + l2.p4()).M() ]

    def makeMlls(self, event, maxLeps):
        mllsfos = self.mllValues(event,  lambda l1,l2 : l1.pdgId()  == -l2.pdgId(),  maxLeps)
        mllafos = self.mllValues(event,  lambda l1,l2 : l1.charge() == -l2.charge(),  maxLeps)
        mllafss = self.mllValues(event,  lambda l1,l2 : l1.charge() ==  l2.charge(),  maxLeps)
        mllafas = self.mllValues(event,  lambda l1,l2 : True, maxLeps)
        event.minMllSFOS = min(mllsfos)
        event.minMllAFOS = min(mllafos)
        event.minMllAFSS = min(mllafss)
        event.minMllAFAS = min(mllafas)
        event.maxMllSFOS = max(mllsfos)
        event.maxMllAFAS = max(mllafas)
        event.maxMllAFOS = max(mllafos)
        event.maxMllAFSS = max(mllafss)
        drllafos = self.drllValues(event,  lambda l1,l2 : l1.charge() == -l2.charge(),  maxLeps)
        drllafss = self.drllValues(event,  lambda l1,l2 : l1.charge() ==  l2.charge(),  maxLeps)
        event.minDrllAFSS = min(drllafss)
        event.minDrllAFOS = min(drllafos)
        event.maxDrllAFOS = max(drllafos)
        event.maxDrllAFSS = max(drllafss)
        ptllafos = self.ptllValues(event,  lambda l1,l2 : l1.charge() == -l2.charge(),  maxLeps)
        ptllafss = self.ptllValues(event,  lambda l1,l2 : l1.charge() ==  l2.charge(),  maxLeps)
        event.minPtllAFSS = min(ptllafss)
        event.minPtllAFOS = min(ptllafos)
        event.maxPtllAFOS = max(ptllafos)
        event.maxPtllAFSS = max(ptllafss)
        leps = event.selectedLeptons; nlep = len(leps)
        event.m2l = (leps[0].p4() + leps[1].p4()).M() if nlep >= 2 else 0
        event.pt2l = (leps[0].p4() + leps[1].p4()).Pt() if nlep >= 2 else 0
        event.q3l = sum([l.charge() for l in leps[:2]]) if nlep >= 3 else 0
        event.ht3l = sum([l.pt() for l in leps[:2]]) if nlep >= 3 else 0
        event.pt3l = (leps[0].p4() + leps[1].p4() + leps[2].p4()).Pt() if nlep >= 3 else 0
        event.m3l = (leps[0].p4() + leps[1].p4() + leps[2].p4()).M() if nlep >= 3 else 0
        event.q4l = sum([l.charge() for l in leps[:3]])  if nlep >= 4 else 0
        event.ht4l = sum([l.pt() for l in leps[:3]]) if nlep >= 4 else 0
        event.pt4l = (leps[0].p4() + leps[1].p4() + leps[2].p4() + leps[3].p4()).Pt() if nlep >= 4 else 0
        event.m4l = (leps[0].p4() + leps[1].p4() + leps[2].p4() + leps[3].p4()).M() if nlep >= 4 else 0

    def mllValues(self, event, pairSelection, maxLeps):
        return self.llValues(event, lambda l1,l2: (l1.p4() + l2.p4()).M(), pairSelection, maxLeps)

    def drllValues(self, event, pairSelection, maxLeps):
        return self.llValues(event, lambda l1,l2: deltaR(l1.eta(), l1.phi(), l2.eta(), l2.phi()), pairSelection, maxLeps)

    def ptllValues(self, event, pairSelection, maxLeps):
        return self.llValues(event, lambda l1,l2: (l1.p4() + l2.p4()).Pt(), pairSelection, maxLeps)

    def llValues(self, event, function, pairSelection, maxLeps):
        pairs = []
        nlep = len(event.selectedLeptons)
        for i,l1 in enumerate(event.selectedLeptons):
            for j in range(i+1,nlep):
                if j >= maxLeps: break
                l2 = event.selectedLeptons[j]    
                if pairSelection(l1,l2):
                    pairs.append( function(l1, l2) )
        if pairs == []: pairs.append(-1)
        return pairs


    def doHT(self, event):
        import ROOT

        objects25 = [ j for j in event.cleanJets if j.pt() > 25 ] + event.selectedLeptons
        objects30 = [ j for j in event.cleanJets if j.pt() > 30 ] + event.selectedLeptons
        objects40  = [ j for j in event.cleanJets if j.pt() > 40 ] + event.selectedLeptons
        objects40j = [ j for j in event.cleanJets if j.pt() > 40 ] 

        event.htJet25 = sum([x.pt() for x in objects25])
        event.mhtJet25vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects25])) , -1.*(sum([x.py() for x in objects25])), 0, 0 )     
        event.mhtPhiJet25 = event.mhtJet25vec.phi()
        event.mhtJet25 = event.mhtJet25vec.pt()

        event.htJet30 = sum([x.pt() for x in objects30])
        event.mhtJet30vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects30])) , -1.*(sum([x.py() for x in objects30])), 0, 0 )             
        event.mhtJet30 = event.mhtJet30vec.pt()
        event.mhtPhiJet30 = event.mhtJet30vec.phi()

        event.htJet40 = sum([x.pt() for x in objects40])
        event.mhtJet40vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects40])) , -1.*(sum([x.py() for x in objects40])), 0, 0 )             
        event.mhtJet40 = event.mhtJet40vec.pt()
        event.mhtPhiJet40 = event.mhtJet40vec.phi()

        event.htJet40j = sum([x.pt() for x in objects40j])
        event.mhtJet40jvec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objects40j])) , -1.*(sum([x.py() for x in objects40j])), 0, 0 )                     
        event.mhtJet40j = event.mhtJet40jvec.pt()
        event.mhtPhiJet40j = event.mhtJet40jvec.phi()

    def process(self, event):
        self.readCollections( event.input )

        # if not MC, nothing to do
        if not self.cfg_comp.isMC: 
            return True

        event.genParticles = [ gp for gp in self.mchandles['genParticles'].product() ]

        event.eventWeigth = 1.0
        event.run = event.input.eventAuxiliary().id().run()
        event.lumi = event.input.eventAuxiliary().id().luminosityBlock()
        event.eventId = event.input.eventAuxiliary().id().event()
        
        self.doLeptons(event)
        self.makeZs(event, 4)
        self.makeMlls(event, 4)
        self.doJets(event)
        self.doBTag(event)
        self.doMET(event)
        self.doHT(event)

        event.genTops = [ gp for gp in event.genParticles if abs(gp.pdgId()) == 6 and lastGenCopy(gp) ]

        return True
