from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.physicsobjects.genutils import isNotFromHadronicShower

def interestingPdgId(id,includeLeptons=False):        
    id = abs(id)
    return id in [6,7,8,17,18] or (includeLeptons and 11 <= id and id < 16) or (22 <= id and id < 40) or id > 1000000
def realDaughters(gp,excludeRadiation=True):
    ret = []
    for i in xrange(gp.numberOfDaughters()):
        dau = gp.daughter(i)
        if dau.pdgId() == gp.pdgId():
            if excludeRadiation:
                return realDaughters(dau)
            else:
                ret += realDaughters(dau)
        else:
            ret.append(dau)
    return ret
def realMothers(gp):
    ret = []
    for i in xrange(gp.numberOfMothers()):
        mom = gp.mother(i)
        if mom.pdgId() == gp.pdgId():
            ret += realMothers(mom)
        else:
            ret.append(mom)
    return ret

class susyGenStatusThree( Analyzer ):
    """Save status==3 genParticles    """
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(susyGenStatusThree,self).__init__(cfg_ana,cfg_comp,looperName)
        self.pythia6only = getattr(cfg_comp, 'pythia6only', False)
      
    #---------------------------------------------
    # DECLARATION OF HANDLES OF GEN LEVEL OBJECTS 
    #---------------------------------------------
        
    def declareHandles(self):
        super(susyGenStatusThree, self).declareHandles()

        #mc information
        self.mchandles['genParticles'] = AutoHandle( 'prunedGenParticles',
                                                     'std::vector<reco::GenParticle>' )
    def beginLoop(self):
        super(susyGenStatusThree,self).beginLoop()

    def makeMCInfo(self, event):
        verbose = getattr(self.cfg_ana, 'verbose', False)
        if self.pythia6only: # legacy mode
            genParticles = map( GenParticle, self.mchandles['genParticles'].product() )
            event.genStatusThree  = [p for p in genParticles if (p.status() == 3)]
            for p in event.genStatusThree: 
                p.rawIndex = -1 # not implemented
                p.motherIndex = -1 # not implemented
        else:
            rawGenParticles = self.mchandles['genParticles'].product() 
            good = []; keymap = {};
            for rawIndex,p in enumerate(rawGenParticles):
                id     = abs(p.pdgId())
                status = p.status()
                # particles must be status > 2, except for prompt leptons and photons
                if status <= 2:
                    if ((id not in [1000022,]) and
                        (id not in [11,12,13,14,15,16,22] or not isNotFromHadronicShower(p))):
                            continue
                # a particle must not be decaying into itself
                #print "  test %6d  : %+8d  %3d :  %8.2f   %+5.2f   %+5.2f : %d %d : %+8d {%6d}: %s" % ( rawIndex,
                #        p.pdgId(), p.status(), p.pt(), p.eta(), p.phi(), p.numberOfMothers(), p.numberOfDaughters(), 
                #        p.motherRef(0).pdgId() if p.numberOfMothers() > 0 else -999, p.motherRef(0).key()   if p.numberOfMothers() > 0 else -999, 
                #        "  ".join("%d[%d]" % (p.daughter(i).pdgId(), p.daughter(i).status()) for i in xrange(p.numberOfDaughters())))
                if id <= 5 or (11 <= id and id <= 16) or id == 21:
                    # for light objects, we want them pre-radiation
                    if any((p.mother(j).pdgId() == p.pdgId()) for j in xrange(p.numberOfMothers())):
                        #print "    fail auto-decay"
                        continue
                else:
                    # everything else, we want it after radiation, i.e. just before decay
                    if any((p.daughter(j).pdgId() == p.pdgId() and p.daughter(j).status > 2) for j in xrange(p.numberOfDaughters())):
                        #print "    fail auto-decay"
                        continue
                # FIXME find a better criterion to discard there
                if status == 71: 
                    #drop QCD radiation with unclear parentage
                    continue 
                # is it an interesting particle?
                ok = False
                if interestingPdgId(id):
                    #print "    pass pdgId"
                    ok = True
                ### no: we don't select by decay, so that we keep the particle summary free of incoming partons and such
                # if not ok and any(interestingPdgId(d.pdgId()) for d in realDaughters(p)):
                #    #print "    pass dau"
                #     ok = True
                if not ok:
                  for mom in realMothers(p):
                    if interestingPdgId(mom.pdgId()) or (getattr(mom,'rawIndex',-1) in keymap):
                        #print "    interesting mom"
                        # exclude extra x from p -> p + x
                        if not any(mom.daughter(j2).pdgId() == mom.pdgId() for j2 in xrange(mom.numberOfDaughters())):
                            #print "         pass no-self-decay"
                            ok = True
                    if abs(mom.pdgId()) == 15:
                        # if we're a tau daughter we're status 2
                        # if we passed all the previous steps, then we're a prompt lepton
                        # so we'd like to be included
                        ok = True
                    if not ok and p.pt() > 10 and id in [1,2,3,4,5,21,22] and any(interestingPdgId(d.pdgId()) for d in realDaughters(mom)):
                        # interesting for being a parton brother of an interesting particle (to get the extra jets in ME+PS) 
                        ok = True 
                if ok:
                    gp = GenParticle(p)
                    gp.rawIndex = rawIndex # remember its index, so that we can set the mother index later
                    keymap[rawIndex] = len(good)
                    good.append(gp)
            for igp,gp in enumerate(good):
                gp.motherIndex = -1
                ancestor = None if gp.numberOfMothers() == 0 else gp.motherRef(0)
                while ancestor != None and ancestor.isNonnull():
                    if ancestor.key() in keymap:
                        gp.motherIndex = keymap[ancestor.key()]
                        if ancestor.pdgId() != good[gp.motherIndex].pdgId():
                            print "Error keying %d: motherIndex %d, ancestor.pdgId %d, good[gp.motherIndex].pdgId() %d " % (igp, gp.motherIndex, ancestor.pdgId(),  good[gp.motherIndex].pdgId())
                        break
                    ancestor = None if ancestor.numberOfMothers() == 0 else ancestor.motherRef(0)
            event.genStatusThree = good
        # add the ID of the mother to be able to recreate last decay chains
        for ip,p in enumerate(event.genStatusThree):
            moms = realMothers(p)
            if len(moms)==0:
                p.motherId = 0
            elif len(moms)==1:
                p.motherId = moms[0].pdgId()
            else:
                #print "    unclear what mothers to give to this particle, among ","  ".join("%d[%d]" % (m.pdgId(),m.status()) for m in moms)
                p.motherId = -9999
            if verbose:
                print "%3d  {%6d}: %+8d  %3d :  %8.2f   %+5.2f   %+5.2f : %d %2d : %+8d {%3d}: %s" % ( ip,p.rawIndex,
                        p.pdgId(), p.status(), p.pt(), p.eta(), p.phi(), len(moms), p.numberOfDaughters(), 
                        p.motherId, p.motherIndex,
                        "  ".join("%d[%d]" % (p.daughter(i).pdgId(), p.daughter(i).status()) for i in xrange(p.numberOfDaughters())))
        if verbose:
            print "\n\n"

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        ## creating a "sub-event" for this analyzer
        #myEvent = Event(event.iEv)
        #setattr(event, self.name, myEvent)
        #event = myEvent
        
        # if not MC, nothing to do
        if not self.cfg_comp.isMC: 
            return True
        # do MC level analysis
        self.makeMCInfo(event)
        return True
