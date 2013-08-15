from CMGTools.RootTools.fwlite.Config import CFG
from CMGTools.HToZZTo4Leptons.tools.FakeRateCalculator import FakeRateCalculator


fakeRate2011 = CFG(
    name='fR',
    muon = 'data/FR2_muon_11.root',
    electron = 'data/FR2_electron_11.root',    
    muonFSR = 'data/FR2_muon_11.root',
    electronFSR = 'data/FR2_electron_11.root'
    )


fakeRates2011 =[fakeRate2011] 



fakeRate2012_A = CFG(
    name='fR_A',
    muon = 'data/FR_A_muon_12.root',
    electron = 'data/FR_A_electron_12.root',    
    )

fakeRate2012_AA = CFG(
    name='fR_AA',
    muon = 'data/FR_AA_muon_12.root',
    electron = 'data/FR_AA_electron_12.root',    
    )


fakeRates2012 =[fakeRate2012_A,fakeRate2012_AA] 
