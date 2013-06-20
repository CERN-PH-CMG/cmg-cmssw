import copy, sys
from ROOT import TLorentzVector
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle

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
        
    def __init__(self, leg1, leg2, met):
        self._leg1 = leg1
        self._leg2 = leg2
        self._p4 = leg1.p4() + leg2.p4()
        self._met = met
        
    def p4(self):
        return self._p4

    def met(self):
        return self._met
    
    def leg1(self):
        return self._leg1
    
    def leg2(self):
        return self._leg2


class Higgs(DiLepton):
    
    def __init__(self, genHiggs, neutrinos):

        higgsDaughters = []

        # find the two daugthers of genHiggs                
        for i in range(genHiggs.numberOfDaughters()):
            if(genHiggs.daughter(i).status()==3):
                higgsDaughters.append(GenParticle(genHiggs.daughter(i)))

        if(len(higgsDaughters)!=2):
            print 'Not 2 daughters !'

        # sort the legs with pT
        leg1, leg2 = self.buildLeg(higgsDaughters)
        met = self.buildMet(genHiggs, neutrinos)
        
        super(Higgs, self).__init__(leg1, leg2, met)
        
    def buildLeg(self, node):

        _leg1_ = node[0]
        _leg2_ = node[1]
        
        pT1 = node[0].p4().pt()
        pT2 = node[1].p4().pt()
        
        if(pT1 < pT2):
            _leg1_ = node[1]
            _leg2_ = node[0]
            
        return _leg1_, _leg2_
        
    
    def buildMet(self, genHiggs, neutrinos):

        # find all status 1 neutrinos that are in the decay tree of node
        # add up all the momenta of these neutrinos
        # return a MET object (see MET class above)

        # I know this is very dirty ! Need to be modified !
        met = genHiggs.__class__(genHiggs).p4()
        met -= met

        for ineutrino in neutrinos:
            met += ineutrino.p4()

        return MET(_met_)
