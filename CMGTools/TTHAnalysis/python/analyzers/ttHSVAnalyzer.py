from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.TTHAnalysis.signedSip import SignedImpactParameterComputer

from CMGTools.RootTools.utils.DeltaR import deltaR

class ttHSVAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHSVAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(ttHSVAnalyzer, self).declareHandles()
        self.handles['ivf'] = AutoHandle( ('slimmedSecondaryVertices',''),'std::vector<reco::VertexCompositePtrCandidate>')
        self.mchandles['packedGen'] = AutoHandle( 'packedGenParticles', 'std::vector<pat::PackedGenParticle>' )

    def beginLoop(self):
        super(ttHSVAnalyzer,self).beginLoop()

       
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        #get all vertices from IVF
        allivf = [ v for v in self.handles['ivf'].product() ]
       
        # attach distances to PV
        pv = event.goodVertices[0]
        for sv in allivf:
             sv.dxy = SignedImpactParameterComputer.vertexDxy(sv, pv)
             sv.d3d = SignedImpactParameterComputer.vertexD3d(sv, pv)
             sv.cosTheta = SignedImpactParameterComputer.vertexDdotP(sv, pv)

        event.ivf = allivf

        if self.cfg_comp.isMC:
            packedGen = [ (p.eta(),p.phi(),p) for p in self.mchandles['packedGen'].product() if p.charge() != 0 and abs(p.eta()) < 2.7 ]
            packedGen.sort(key = lambda (e,p,x) : e)
            for s in event.ivf:
                #print "SV with %d tracks, mass %5.2f, pt %5.2f, eta %+4.2f, phi %+4.2f: " % (s.numberOfDaughters(), s.mass(), s.pt(), s.eta(), s.phi())
                mctracks, matchable, matched = 0, 0, 0
                ancestors = {}
                for id in xrange(s.numberOfDaughters()):
                    dau = s.daughter(id)
                    #print "  daughter track with pt %6.3f, eta %+5.3f, phi %+5.3f, dxy %+6.4f, dz %+6.4f" % (dau.pt(), dau.eta(), dau.phi(), dau.dxy(), dau.dz())
                    dau.match = ( None, 0.05, 2 )
                    myeta, myphi = dau.eta(), dau.phi()
                    # now, we don't loop over all the packed gen candidates, but rather do fast bisection to find the rightmost with eta > dau.eta() - 0.05
                    etacut = myeta - dau.match[1]
                    ileft, iright = 0, len(packedGen)
                    while iright - ileft > 1:
                        imid = (iright + ileft)/2
                        if packedGen[imid][0] > etacut:
                            iright = imid
                        else:
                            ileft = imid
                    # now scan from imid to the end (but stop much earlier)
                    etacut = myeta +  dau.match[1]
                    for i in xrange(ileft,len(packedGen)):
                        (eta,phi,pg) = packedGen[i]
                        if eta > etacut: break
                        dr  = deltaR(myeta,myphi,eta,phi)
                        if dr > dau.match[1]: continue
                        if pg.charge() != dau.charge(): continue
                        dpt = abs(dau.pt() - pg.pt())/(dau.pt()+pg.pt())
                        if pg.pt() > 10: dpt /= 2; # scale down 
                        if dpt < 0.1:
                            dau.match = ( pg, dr, dpt )
                    if dau.match[0]: 
                        mctracks += 1
                        # print "     \--> gen cand pdgId %+6d with pt %6.3f, eta %+5.3f, phi %+5.3f: dr = %.3f, dptrel = %.4f" % (dau.match[0].pdgId(), dau.match[0].pt(), dau.match[0].eta(), dau.match[0].phi(), dau.match[1], dau.match[2])
                        # ancestry
                        mom = dau.match[0].mother(0)
                        depth = 1; found = False
                        while mom:
                            id = abs(mom.pdgId())
                            flav = max((id/1000) % 10, (id/100) % 10)
                            key = (mom.pt(), mom.eta(), mom.phi())
                            # print "     \---> mother pdgId %+6d, flav %d (pt %6.3f, eta %+5.3f, phi %+5.3f) at %s" % (mom.pdgId(), mom.pt(), mom.eta(), mom.phi(), flav, hash(key))
                            if mom.status() != 2 or (abs(mom.pdgId()) < 100 and (abs(mom.pdgId()) != 15)):
                                break
                            if flav in [4,5]:
                                found = True
                                if key in ancestors:
                                    ancestors[key][1] += 1;
                                    ancestors[key][2] = min(ancestors[key][2], depth)
                                else:
                                    ancestors[key] = [mom, 1, depth, flav]
                            mom = mom.mother(0) if mom.numberOfMothers() > 0 else None
                            depth += 1
                        if found: matchable += 1
                s.mcMatchNTracks   = mctracks
                s.mcMatchNTracksHF = matchable
                s.mcMatchFraction  = -1.0
                s.mcFlavFirst      = 0
                s.mcFlavHeaviest   = 0
                if matchable:
                    maxhits  = max([h for (p,h,d,f) in ancestors.itervalues() ])
                    mindepth = min([d for (p,h,d,f) in ancestors.itervalues() if h == maxhits])
                    if matchable > 1:
                        s.mcMatchFraction = maxhits / float(matchable)
                    for (mom,hits,depth,flav) in ancestors.itervalues():
                        if hits != maxhits: continue
                        if depth == mindepth:
                            s.mcFlavFirst = flav
                        s.mcFlavHeaviest = max(flav, s.mcFlavHeaviest)
                        #print " \==> ancestor  pdgId %+6d with %d/%d hits at depth %d at %s" % (mom.pdgId(), hits, matchable, depth, hash(mom))
                        #if hits == maxhits and depth == mindepth: print "           ^^^^^--- this is our best match"
        return True
