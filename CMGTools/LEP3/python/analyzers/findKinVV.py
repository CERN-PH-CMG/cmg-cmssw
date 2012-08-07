from math import sqrt
from CMGTools.LEP3.kinfitters import DiJetMKinFitter

def findKinWW(jetPairs):

    ww = []

    w1 = jetPairs[0]
    w2 = jetPairs[5]
    wwmin = 999.

    diWJetfitter = DiJetMKinFitter('fitter','Two Jet Kin fitter',80.4)
    #print 'chi2 at the beginning: ', diWJetfitter.getChi2()

    for i,pair1 in enumerate(jetPairs):
        pair2 = jetPairs[5-i]

        finalW1JetsM = diWJetfitter.fit(pair1.leg1.p4(), pair1.leg2.p4())
        chi2fitW1M = diWJetfitter.getChi2()
        
        finalW2JetsM = diWJetfitter.fit(pair2.leg1.p4(), pair2.leg2.p4())
        chi2fitW2M = diWJetfitter.getChi2()

        #print 'chi2s of the W couples: ', chi2fitW1M, chi2fitW2M
        wwchi = sqrt((chi2fitW1M)*(chi2fitW1M)+(chi2fitW2M)*(chi2fitW2M))

        if wwchi < wwmin :
            wwmin = wwchi
            w1 = pair1
            w2 = pair2

    ww = [w1,w2]
    return ww, wwmin

def findKinZZ(jetPairs):

    zz = []

    z1 = jetPairs[0]
    z2 = jetPairs[5]
    zzmin = 999.

    diZJetfitter = DiJetMKinFitter('fitter','Two Jet Kin fitter',91.2)
    #print 'chi2 at the beginning: ', diZJetfitter.getChi2()
    
    for i,pair1 in enumerate(jetPairs):
        pair2 = jetPairs[5-i]

        finalZ1JetsM = diZJetfitter.fit(pair1.leg1.p4(), pair1.leg2.p4())
        chi2fitZ1M = diZJetfitter.getChi2()
        
        finalZ2JetsM = diZJetfitter.fit(pair2.leg1.p4(), pair2.leg2.p4())
        chi2fitZ2M = diZJetfitter.getChi2()

        #print 'chi2s of the Z couples: ', chi2fitZ1M, chi2fitZ2M
        zzchi = sqrt((chi2fitZ1M)*(chi2fitZ1M)+(chi2fitZ2M)*(chi2fitZ2M))

        if zzchi < zzmin :
            zzmin = zzchi
            z1 = pair1
            z2 = pair2

    zz = [z1,z2]
    return zz, zzmin
