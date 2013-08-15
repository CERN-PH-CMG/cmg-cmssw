from CMGTools.RootTools.fwlite.Config import CFG

FSRConfig = CFG(
        name='FSR',
        minPhotonPt = 2.0,
        minPhotonPtTight = 4.0,
        maxPhotonEta=2.4,
        maxPhotonDBIso=1,
        electronCleaningDR=0.01,
        vetoElectronDR=0.15,
        vetoElectronDEta=0.05,
        vetoElectronDPhi=2,
        maxLeptonPhotonDR=0.5,
        maxLeptonPhotonAngle=9999.,
        maxLeptonPhotonDRTight=0.07,
        maxLeptonPhotonAngleTight=1000.,
        maxThetaStar = 1000.,
        leptonIsoCone=0.4,
        maxZMass=100.
        )

