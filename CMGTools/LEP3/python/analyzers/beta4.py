import math
from numpy import array, linalg, dot, add

def beta4(jets,ebeam):

    rows = []
    for i in range(4):
        rows.append([])
    for jet in jets:
        rows[0].append(jet.px()/jet.energy())
        rows[1].append(jet.py()/jet.energy())
        rows[2].append(jet.pz()/jet.energy())
        rows[3].append(jet.energy()/jet.energy())        

    constraint = [0.,0.,0.,2.*ebeam]

    d2 = array(rows)
    d = array(constraint)
    #print d2
    #print d
    energies = linalg.solve(d2,d)
    #print energies
    chi2 = 0.
    for i,jet in enumerate(jets):

        if energies[i] > 0. :
            uncert = 0.5*math.sqrt(jet.energy()) + 0.05*jet.energy()
            delta = (jet.energy()-energies[i])/uncert
            if delta > 0. : 
                chi2 += delta*delta
            else:
                chi2 += delta*delta/4.
        else:
            chi2 += 1000.

        p4 = jet.p4()
        p4.SetPxPyPzE(jet.px()*energies[i]/jet.energy(),
                      jet.py()*energies[i]/jet.energy(),
                      jet.pz()*energies[i]/jet.energy(),
                      energies[i])
        jet.setP4(p4)

    return chi2
