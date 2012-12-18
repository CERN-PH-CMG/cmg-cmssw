from CMGTools.RootTools.RootTools import loadLibs
from ROOT import gSystem

# loading the shared library containing the MVAMetPyWrapper class in ROOT
gSystem.Load("libCMGToolsUtilities")

# importing the python binding to the C++ class from ROOT 
from ROOT import MVAMetPyWrapper

class MVAMet(MVAMetPyWrapper):
    
    def getMet(self,
               iPFMet,
               iTKMet,
               iNoPUMet,
               iPUMet,
               iPUCMet,
               iLeadJet,
               i2ndJet,
               iNJetsGt30,
               iNJetsGt1,
               iNGoodVtx,
               iJets,
               iPrintDebug,
               visObjectP4s
               ):
        map( self.addVisObject, visObjectP4s)
        return GetMet(
               iPFMet,
               iTKMet,
               iNoPUMet,
               iPUMet,
               iPUCMet,
               iLeadJet,
               i2ndJet,
               iNJetsGt30,
               iNJetsGt1,
               iNGoodVtx,
               iJets,
               iPrintDebug            
               )
    

if __name__ == '__main__':
    mvaMet = MVAMet()
