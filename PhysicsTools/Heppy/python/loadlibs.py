from ROOT import gROOT,gSystem

def decorate_tlv(tlv_class):
    tlv_class.__add__ = lambda self, other : self.__class__(self.X()+other.X(),self.Y()+other.Y(),self.Z()+other.Z(),self.T()+other.T())
    tlv_class.__sub__ = lambda self, other : self.__class__(self.X()-other.X(),self.Y()-other.Y(),self.Z()-other.Z(),self.T()-other.T())

def load_libs():
    print 'loading FWLite.'
    #load the libaries needed
    gSystem.Load("libFWCoreFWLite")
    gROOT.ProcessLine('AutoLibraryLoader::enable();')
    gSystem.Load("libFWCoreFWLite")
        
    #now the RootTools stuff
    gSystem.Load("libPhysicsToolsHeppy")

    import ROOT
    decorate_tlv( ROOT.reco.Candidate.LorentzVector )

load_libs()
