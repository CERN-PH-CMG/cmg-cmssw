from math import sqrt

def findWW(jetPairs):

    ww = []

    w1 = jetPairs[0]
    w2 = jetPairs[5]
    wwmin = 999.
    for i,pair1 in enumerate(jetPairs):
        pair2 = jetPairs[5-i]
        wwchi = sqrt((pair1.M()-80.3)*(pair1.M()-80.3) + (pair2.M()-80.3)*(pair2.M()-80.3))

        if wwchi < wwmin :
            wwmin = wwchi
            w1 = pair1
            w2 = pair2

    ww = [w1,w2]
    return ww, wwmin

def findZZ(jetPairs):

    zz = []

    z1 = jetPairs[0]
    z2 = jetPairs[5]
    zzmin = 999.
    for i,pair1 in enumerate(jetPairs):
        pair2 = jetPairs[5-i]
        zzchi = sqrt((pair1.M()-91.2)*(pair1.M()-91.2) + (pair2.M()-91.2)*(pair2.M()-91.2))

        if zzchi < zzmin :
            zzmin = zzchi
            z1 = pair1
            z2 = pair2

    zz = [z1,z2]
    return zz, zzmin

