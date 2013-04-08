from CMGTools.RootTools.physicsobjects.PhysicsObject import *

loose_WP = [
    (0, 2.5, -0.8),
    (2.5, 2.75, -0.74),
    (2.75, 3.0, -0.68),
    (3.0, 5.0, -0.77),
    ]


class Jet( PhysicsObject):

    def looseJetId(self):
        '''PF Jet ID
        '''
        return self.getSelection('cuts_looseJetId') 

    def puJetId(self):
        '''Full mva PU jet id with old loose working point used by Josh'''
        puMva = self.puMva('full53x')
        eta = abs(self.eta())
        for etamin, etamax, cut in loose_WP:
            if not(eta>=etamin and eta<etamax):
                continue
            return puMva>cut
        


class GenJet( PhysicsObject):
    pass

