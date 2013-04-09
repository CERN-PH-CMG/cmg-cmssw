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
        self.pfJetIdPassed = ( abs(self.eta()) <= 2.4                                             and 
                              (self.component(1).fraction()                                > 0    and
                               self.component(2).fraction()                                < 0.99 and 
                               self.component(4).fraction()                                < 0.99 and 
                               self.component(5).fraction() + self.component(6).fraction() < 0.99 and 
                               self.component(1).number()                                  > 0    and
                               self.nConstituents()                                        > 1   )or
                              abs(self.eta()) > 2.4                                               and
                              (self.component(4).fraction()                                < 0.99 and 
                               self.component(5).fraction() + self.component(6).fraction() < 0.99 and 
                               self.nConstituents()                                        > 1    ) 
                            ) 
        #return self.getSelection('cuts_looseJetId') 
        return self.pfJetIdPassed 

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

