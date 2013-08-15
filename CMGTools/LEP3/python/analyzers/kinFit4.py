import math
from numpy import array, linalg, dot, add
from CMGTools.LEP3.kinfitters import FourJetEpKinFitter


def kinFit4(jets,ebeam):

    fourJetfitter = FourJetEpKinFitter('fitter','Four Jet Kin fitter',ebeam*2)
    finalFourJetsp4 = fourJetfitter.fit(jets[0].p4(), jets[1].p4(), jets[2].p4(), jets[3].p4())

    chi2fit = fourJetfitter.getChi2()
    
    for i,jet in enumerate(jets):

        p4 = jet.p4()
        p4.SetPxPyPzE(finalFourJetsp4[i].Px(),finalFourJetsp4[i].Py(),finalFourJetsp4[i].Pz(),finalFourJetsp4[i].Energy())
        jet.setP4(p4)
        

    return chi2fit
