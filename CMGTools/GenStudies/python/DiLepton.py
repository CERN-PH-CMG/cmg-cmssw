#import copy, sys
#from ROOT import TLorentzVector
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
import copy

def DecayTracer(particle, e, mu, visible, rank):
    
    '''Check the tau decay'''

    rank += 1

    for i in range( particle.numberOfDaughters() ):
        dau = GenParticle(particle.daughter(i))
#        print "   "*rank, dau.pdgId(), dau.status(), dau.p4().pt()
        if(abs(dau.pdgId()) == 11 and abs(particle.pdgId())==15):
#            print 'electron !', particle.pdgId()
            e.append(dau)
        elif(abs(dau.pdgId()) == 13 and abs(particle.pdgId())==15):
#            print 'muon !', particle.pdgId()
            mu.append(dau)

        if(dau.status()==1):
            if(abs(dau.pdgId()) in [12,14,16]):
                pass
            else:
                visible.append(dau)

        DecayTracer(dau, e, mu, visible, rank)


def DecayAnalyzer(leg):
    e = []
    mu = []
    vis = []
    rank = 0
    DecayTracer(leg, e, mu, vis, rank)

#    vis_mom = leg.__class__(leg).p4()
    vis_mom = copy.deepcopy(leg.p4())
    vis_mom -= vis_mom

    for ivisible in vis:
        vis_mom += ivisible.p4()

    return len(e), len(mu), vis_mom.pt()



#def DecayTracer(particle, daughters, rank):
#    
#    '''Check the tau decay'''
#    rank+=1
#
##    print 'daughters', particle.numberOfDaughters(), particle.daughter(0).pdgId(),particle.daughter(1).pdgId(),particle.daughter(2).pdgId()
#
#    for i in range( particle.numberOfDaughters() ):
#        dau = GenParticle(particle.daughter(i))
#        print "   "*rank, dau.pdgId(),
#        if(abs(dau.pdgId()) in [11,13]):
##            print '\t Here is a status=1 lepton !'
#            daughters.append(dau)
#            
#            daughters = TauDecayTracer(dau, daughters, rank)
#            return daughters



#def METfromNeutrino(particle, daughters):
#
#    '''Fills daughters with all the daughters of particle.
#    Recursive function.'''
#
#    for i in range( particle.numberOfDaughters() ):
#        dau = GenParticle(particle.daughter(i))
##        print dau.status(), dau.pdgId()
#        if(dau.status()==1 and (abs(dau.pdgId()) in [12,14,16])):
#            daughters.append( dau )
#
##            print 'Here we have neutrino', dau.p4().pt()
#
#        daughters = METfromNeutrino(dau, daughters)
#    return daughters


class MET(object):
    def __init__(self, p4):
        self._p4 = p4

    def p4(self):
        return self._p4

    
class DiLepton(object):

    def __init__(self, leg1, leg2, met,
                 leg1_e=0,
                 leg1_mu=0,
                 leg1_vis=0,
                 leg2_e=0,
                 leg2_mu=0,
                 leg2_vis=0):
        
        self._leg1 = leg1
        self._leg2 = leg2
        self._p4 = leg1.p4() + leg2.p4()
        self._met = met
        self._decay = -1
        self._leg1_vis = leg1_vis
        self._leg2_vis = leg2_vis
        self._leg1_e = leg1_e
        self._leg2_e = leg2_e
        self._leg1_mu = leg1_mu
        self._leg2_mu = leg2_mu
        
    def p4(self):
        return self._p4

    def met(self):
        return self._met
    
    def leg1(self):
        return self._leg1
    
    def leg2(self):
        return self._leg2

    def leg1_vis(self):
        return self._leg1_vis

    def leg2_vis(self):
        return self._leg2_vis

    def decayId(self):

        if(self._leg1_e == 0 and self._leg2_e == 0 and
           self._leg1_mu == 0 and self._leg2_mu == 0):
            return 0 # all hadronic
        elif(self._leg1_e == 1 and self._leg2_e == 1):
            return 1 # ee
        elif(self._leg1_mu == 1 and self._leg2_mu == 1):
            return 2 # mm
        elif(self._leg1_e == 1 and self._leg2_mu == 1):
            return 3 # em
        elif(self._leg1_mu == 1 and self._leg2_e == 1):
            return 4 # me
        elif(self._leg1_e == 1 and (self._leg2_mu + self._leg2_e) == 0):
            return 5 #etau
        elif(self._leg1_mu == 1 and (self._leg2_mu + self._leg2_e) == 0):
            return 6 #mutau
        elif(self._leg2_e == 1 and (self._leg1_mu + self._leg1_e) == 0):
            return 7 #etau
        elif(self._leg2_mu == 1 and (self._leg1_mu + self._leg1_e) == 0):
            return 8 #mutau
        else:
            print 'Unexpected decay !', self._leg1_e, self._leg1_mu, self._leg2_e, self._leg2_mu
            return -1
        





        
#    def decay(self):
#    def leg1_decay(self):

#        if(len(e1)==0 and len(mu1)==0):
#            return False, vis_mom.pt()
#        else:
#            return True, vis_mom.pt()

#        print 'pT_leg1 = ', vis_mom.pt()

#    def leg2_decay(self):

#        e2 = []
#        mu2 = []
#        vis2 = []
#        DecayTracer(self._leg2, e2, mu2, vis2, 0)
#
#        vis_mom = self._leg2.__class__(self._leg2).p4()
#        vis_mom -= vis_mom
#
#        for ivisible in vis2:
#            vis_mom += ivisible.p4()
#
#        return len(e2), len(mu2), vis_mom.pt()


#        print 'pT_leg2 = ', vis_mom.pt()


#        if(len(e2)==0 and len(mu2)==0):
#            return False, vis_mom.pt()
#        else:
#            return True, vis_mom.pt()


class Higgs(DiLepton):
    
    def __init__(self, genHiggs, neutrinos):

        higgsDaughters = []

        # find the two daugthers of genHiggs                
        for i in range(genHiggs.numberOfDaughters()):
            if(genHiggs.daughter(i).status()==3):
                higgsDaughters.append(GenParticle(genHiggs.daughter(i)))

        if(len(higgsDaughters)!=2):
            print 'Not 2 daughters !', len(higgsDaughters)
#            for i in range(genHiggs.numberOfDaughters()):
#                print genHiggs.daughter(i).pdgId(), genHiggs.daughter(i).status()
#                a = genHiggs.daughter(i)
#                for ii in range(a.numberOfDaughters()):
#                    print a.daughter(i).pdgId(), a.daughter(i).status()
                
                
        # sort the legs with pT
        leg1, leg2 = self.buildLeg(higgsDaughters)
        met = self.buildMet(genHiggs, neutrinos)


        leg1_e, leg1_mu, leg1_vis = DecayAnalyzer(leg1)
        leg2_e, leg2_mu, leg2_vis = DecayAnalyzer(leg2)

#        print 'leg1', leg1_e, leg1_mu, leg1_vis, leg1.p4().pt()
#        print 'leg2', leg2_e, leg2_mu, leg2_vis, leg2.p4().pt()


        super(Higgs, self).__init__(leg1, leg2, met, leg1_e, leg1_mu, leg1_vis, leg2_e, leg2_mu, leg2_vis)
        
    def buildLeg(self, node):

        _leg1_ = node[0]
        _leg2_ = node[1]
        
        pT1 = node[0].p4().pt()
        pT2 = node[1].p4().pt()

#        print 'pt1, pt2 = ', pT1, pT2
        
        if(pT1 < pT2):
            _leg1_ = node[1]
            _leg2_ = node[0]
            
        return _leg1_, _leg2_
        
    
    def buildMet(self, genHiggs, neutrinos):

        # find all status 1 neutrinos that are in the decay tree of node
        # add up all the momenta of these neutrinos
        # return a MET object (see MET class above)

        # I know this is very dirty ! Need to be modified !
#        met = genHiggs.__class__(genHiggs).p4()
        met = copy.deepcopy(genHiggs.p4())
        met -= met

        for ineutrino in neutrinos:
            met += ineutrino.p4()

        return MET(met)
