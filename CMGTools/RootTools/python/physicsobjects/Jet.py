from CMGTools.RootTools.physicsobjects.PhysicsObject import *

class Jet( PhysicsObject):
    def looseJetId(self):
        '''This jet id is slightly different from the one present in the cmg::PFJet on May 28.
        no cut on the mu fraction.
        '''
        return self.getSelection('cuts_looseJetId') 

class GenJet( PhysicsObject):
    pass

