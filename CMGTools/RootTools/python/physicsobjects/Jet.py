from CMGTools.RootTools.physicsobjects.PhysicsObject import *

loose_WP = [
    (0, 2.5, -0.8),
    (2.5, 2.75, -0.74),
    (2.75, 3.0, -0.68),
    (3.0, 5.0, -0.77),
    ]

# Working point 2 May 2013 (Phil via H2tau list)
loose_53X_WP = [
    (0, 2.5, -0.63),
    (2.5, 2.75, -0.60),
    (2.75, 3.0, -0.55),
    (3.0, 5.0, -0.45),
    ]


class Jet(PhysicsObject):

    def looseJetId(self):
        '''PF Jet ID
        '''
        pfJetIdPassed = ( abs(self.eta()) <= 2.4                                             and 
                              (self.component(1).fraction()                                > 0    and
                               self.component(2).fraction()                                < 0.99 and 
                               self.component(4).fraction()                                < 0.99 and 
                               self.component(5).fraction() < 0.99 and 
                               self.component(1).number()                                  > 0    and
                               self.nConstituents()                                        > 1   )or
                              abs(self.eta()) > 2.4                                               and
                              (self.component(4).fraction()                                < 0.99 and 
                               self.component(5).fraction() < 0.99 and 
                               self.nConstituents()                                        > 1    ) 
                            ) 
        #return self.getSelection('cuts_looseJetId') 
        return pfJetIdPassed 

    def puJetId(self, wp53x=False):
        '''Full mva PU jet id (default: old loose working point used by Josh)'''

        puMva = self.puMva('full53x')
        wp = loose_53X_WP
        eta = abs(self.eta())
        
        if not wp53x:
            puMva = self.puMva('full')
            wp = loose_WP

        for etamin, etamax, cut in wp:
            if not(eta>=etamin and eta<etamax):
                continue
            return puMva>cut
        
    def puJetId53X(self):
        '''53X full mva PU jet id with new working point by Phil (2 May 2013)'''
        return self.puJetId(wp53x=True)


class GenJet( PhysicsObject):
    pass

