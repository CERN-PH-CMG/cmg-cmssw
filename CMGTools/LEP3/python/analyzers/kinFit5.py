import math
from numpy import array, linalg, dot, add
from CMGTools.LEP3.kinfitters import FourJetEpMKinFitter


def kinFit5(jets,ebeam, mass):

    fourJetfitter = FourJetEpMKinFitter('fitter','Four Jet Kin fitter',ebeam*2, mass)

    finalFourJetsp4_1 = fourJetfitter.fit(jets[0].p4(), jets[1].p4(), jets[2].p4(), jets[3].p4(), 0, 1)
    chi2fit_1 = fourJetfitter.getChi2()
    status_1 = fourJetfitter.getStatus()
    if (chi2fit_1<0 or status_1 !=0):
        print 'status and chi2 fit1 with problems :', status_1, chi2fit_1
        chi2fit_1 = 1E10

    finalFourJetsp4_2 = fourJetfitter.fit(jets[0].p4(), jets[1].p4(), jets[2].p4(), jets[3].p4(), 0, 2)
    chi2fit_2 = fourJetfitter.getChi2()
    status_2 = fourJetfitter.getStatus()
    if (chi2fit_2<0 or status_2 !=0):
        print 'status and chi2 fit2 with problems :', status_2, chi2fit_2
        chi2fit_2 = 1E10

    finalFourJetsp4_3 = fourJetfitter.fit(jets[0].p4(), jets[1].p4(), jets[2].p4(), jets[3].p4(), 0, 3)
    chi2fit_3 = fourJetfitter.getChi2()
    status_3 = fourJetfitter.getStatus()
    if (chi2fit_3<0 or status_3 !=0):
        print 'status and chi2 fit3 with problems :', status_3, chi2fit_3
        chi2fit_3 = 1E10

    finalFourJetsp4_4 = fourJetfitter.fit(jets[0].p4(), jets[1].p4(), jets[2].p4(), jets[3].p4(), 1, 2)
    chi2fit_4 = fourJetfitter.getChi2()
    status_4 = fourJetfitter.getStatus()
    if (chi2fit_4<0 or status_4 !=0):
        print 'status and chi2 fit4 with problems :', status_4, chi2fit_4
        chi2fit_4 = 1E10
   

    finalFourJetsp4_5 = fourJetfitter.fit(jets[0].p4(), jets[1].p4(), jets[2].p4(), jets[3].p4(), 1, 3)
    chi2fit_5 = fourJetfitter.getChi2()
    status_5 = fourJetfitter.getStatus()
    if (chi2fit_5<0 or status_5 !=0):
        print 'status and chi2 fit5 with problems :', status_5, chi2fit_5
        chi2fit_5 = 1E10

    finalFourJetsp4_6 = fourJetfitter.fit(jets[0].p4(), jets[1].p4(), jets[2].p4(), jets[3].p4(), 2, 3)
    chi2fit_6 = fourJetfitter.getChi2()
    status_6 = fourJetfitter.getStatus()
    if (chi2fit_6<0 or status_6 !=0):
        print 'status and chi2 fit6 with problems :', status_6, chi2fit_6
        chi2fit_6 = 1E10

    #print 'chi2s:', chi2fit_1, chi2fit_2, chi2fit_3, chi2fit_4, chi2fit_5, chi2fit_6

    chi2s = [(chi2fit_1,finalFourJetsp4_1), (chi2fit_2,finalFourJetsp4_2), (chi2fit_3,finalFourJetsp4_3), (chi2fit_4, finalFourJetsp4_4), (chi2fit_5,finalFourJetsp4_5) , (chi2fit_6, finalFourJetsp4_6)]
    chi2s.sort()

    #print 'chi2s first:', chi2s[0][0]

#     eta1 = jets[0].eta()
#     eta2 = jets[1].eta()
#     eta3 = jets[2].eta()
#     eta4 = jets[3].eta()
        
    
    for i,jet in enumerate(jets):

        finalFourJetsp4 = chi2s[0][1]
        p4 = jet.p4()
        p4.SetPxPyPzE(finalFourJetsp4[i].Px(),finalFourJetsp4[i].Py(),finalFourJetsp4[i].Pz(),finalFourJetsp4[i].Energy())
        # do not reassign if fit failed for all combinations
        if chi2s[0][0]<1E9:
            jet.setP4(p4)

   #  if (abs(jets[0].eta())>10 or abs(jets[1].eta())>10 or abs(jets[2].eta())>10 or abs(jets[3].eta())>10):
#         print 'etas before:', eta1, eta2, eta3, eta4    
#         print 'etas after:', jets[0].eta(), jets[1].eta(), jets[2].eta(), jets[3].eta()    
#         print chi2s[0][0]

    return chi2s[0][0]
