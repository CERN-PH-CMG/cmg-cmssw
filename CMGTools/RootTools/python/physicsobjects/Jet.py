from CMGTools.RootTools.physicsobjects.PhysicsObject import *

class Jet( PhysicsObject):
    def looseJetId(self):
        '''This jet id is slightly different from the one present in the cmg::PFJet on May 28.
        no cut on the mu fraction.
        '''
        return self.getSelection('cuts_looseJetId_h0Fraction') and \
               self.getSelection('cuts_looseJetId_gammaFraction') and \
               self.getSelection('cuts_looseJetId_nConstituents') and \
               self.getSelection('cuts_looseJetId_hFraction') and \
               self.getSelection('cuts_looseJetId_hChargedMultiplicity') and \
               self.getSelection('cuts_looseJetId_eFraction')

class GenJet( PhysicsObject):
    pass

