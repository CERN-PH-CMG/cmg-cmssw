

def leptonsFromDiTaus( diTaus ):
    '''Returns a dictionary of leptons used as leg2 in a collection of diTaus.

    The dictionary is indexed by sourcePtr, so 
    two leg2s with the same sourcePtr are considered as being the same,
    and enter the dictionary only once.'''
    leptons = {}
    for diTau in diTaus:
        # print diTau, diTau.leg2().pt()
        lepton = diTau.leg2()
        ptr = lepton.sourcePtr()
        leptons[ (ptr.key(), ptr.id().id()) ] = lepton
    return leptons


def testMuon( muon ):
    '''basically recoding the muon selection of muCuts_cff, except for the eta range.'''
    if muon.pt()>15 and \
       abs( muon.eta() ) < 2.5 and \
       muon.isGlobal() and \
       muon.isTracker() and \
       muon.numberOfValidTrackerHits() > 10 and \
       muon.numberOfValidPixelHits() > 0 and \
       muon.numberOfValidMuonHits() > 0 and \
       muon.numberOfMatches() > 1 and \
       muon.normalizedChi2() < 10 and \
       abs(muon.dxy()) < 0.045 and \
       abs(muon.dz()) < 0.2 and \
       muon.relIso(0.5)<0.1:
        return True
    else:
        return False
    

def leptonsByCharge( leptons ):
    '''splits a list of leptons by charge, returns plus list and minus list.'''
    plus = []
    minus = []
    for lepton in leptons:
        if lepton.charge()>0:
            plus.append(lepton)
        elif lepton.charge()<0:
            minus.append(lepton)
        else:
            raise ValueError("a lepton cannot have charge 0")
    return (plus, minus)


def noOppositeChargePair( leptons ):
    '''Returns False if at least one pair of opposite charge leptons in leptons'''
    (plus, minus) = leptonsByCharge( leptons )
    if len(plus)>0 and len(minus)>0:
        return False
    else:
        return True
    

def leptonAcceptFromDiTaus( diTaus ):
    '''Returns False if the diTaus altogether contain at least a pair of leptons
    of opposite charge on their leg2.'''
    # print 'lepton accept'
    # print map( str, diTaus)
    leptons = leptonsFromDiTaus( diTaus )
    return noOppositeChargePair( leptons.values() )


def leptonAcceptFromLeptons( allLeptons ):
    # print 'from leptonsq'
    leptons = filter( testMuon, allLeptons )
    return noOppositeChargePair( leptons )
    

