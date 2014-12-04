from CMGTools.RootTools.RootTools import loadLibs
from ROOT import gSystem

# loading the shared library containing the MVAMetPyWrapper class in ROOT
gSystem.Load("libCMGToolsUtilities")
gSystem.Load("libDataFormatsMETReco")

# importing the python binding to the C++ class from ROOT 
from ROOT import MVAMetPyWrapper
from ROOT import reco, MetUtilities

PFMET = reco.PFMET

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
               iJets_p4,
               iJets_mva,
               iJets_neutFrac,
               iPrintDebug,
               visObjectP4s
               ):
        map( self.addVisObject, visObjectP4s)
        map( self.add_iJets, iJets_p4, iJets_mva, iJets_neutFrac)
        return self.GetMet(
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
               iPrintDebug            
               )
    

if __name__ == '__main__':
    mvaMet = MVAMet()

    # jetInfo = MetUtilities.JetInfo()
    pfmet = PFMET()
