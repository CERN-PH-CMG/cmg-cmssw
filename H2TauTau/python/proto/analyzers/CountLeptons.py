from CMGTools.RootTools.utils.DeltaR import deltaR

## def leptonsFromDiTaus( diTaus ):
##     '''Returns a dictionary of leptons used as leg2 in a collection of diTaus.

##     The dictionary is indexed by sourcePtr, so 
##     two leg2s with the same sourcePtr are considered as being the same,
##     and enter the dictionary only once.'''
##     leptons = {}
##     for diTau in diTaus:
##         # print diTau, diTau.leg2().pt()
##         lepton = diTau.leg2()
##         ptr = lepton.sourcePtr()
##         leptons[ (ptr.key(), ptr.id().id()) ] = lepton
##     return leptons


def testMuonTight( muon, ptCut=15 ):
    '''basically recoding the muon selection of muCuts_cff.'''
    if muon.pt()>ptCut and \
       abs( muon.eta() ) < 2.1 and \
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


def testMuonLoose( muon, ptCut=15 ):
    '''Loose muon selection, for the lepton veto'''
       #COLIN: not sure the vertex constraints should be kept 
    if muon.pt()>ptCut and \
       abs( muon.eta() ) < 2.5 and \
       muon.isGlobal() and \
       abs(muon.dxy()) < 0.045 and \
       abs(muon.dz()) < 0.2 and \
       muon.relIso(0.5)<0.3:
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
    

## def leptonAcceptFromDiTaus( diTaus ):
##     '''Returns False if the diTaus altogether contain at least a pair of leptons
##     of opposite charge on their leg2.'''
##     # print 'lepton accept'
##     # print map( str, diTaus)
##     leptons = leptonsFromDiTaus( diTaus )
##     # return noOppositeChargePair( leptons.values() )
##     return len(leptons)<2


def muonAccept( allLeptons ):
    # print 'from leptonsq'
    tightLeptons = set(filter( testMuonTight, allLeptons ))
    looseLeptons = set(filter( testMuonLoose, allLeptons ))
    if not tightLeptons.issubset( looseLeptons):
        #COLIN: hehe not sure about that 
        raise ValueError('tight leptons must be a subset of loose leptons')
    # return noOppositeChargePair( leptons )
    #COLIN: the following works if the previous exception didn't fire
    # otherwise, could do an xor
    nLeptons = len(looseLeptons)
    return nLeptons<2
    

def testEleLoose( ele, ptCut=15 ):
    '''Loose electron selection, for the lepton veto'''
    if ele.pt()<ptCut : return False
    eleID = int(ele.sourcePtr().electronID('simpleEleId95relIso'))
    # print 'TEST',eleID,eleID&1,ele.sourcePtr().passConversionVeto()
    if (eleID&1 == 1 ) : return True   
    return False
    

def testEleLooseLorenzo( ele, ptCut=15 ):
    '''Loose electron selection, for the lepton veto, according to Lorenzo prescription'''
    if ele.pt() < ptCut : return False
#    eop = ele.sourcePtr().eSuperClusterOverP()
    hoe = ele.sourcePtr().hcalOverEcal()
    deta = ele.sourcePtr().deltaEtaSuperClusterTrackAtVtx()
    dphi = ele.sourcePtr().deltaPhiSuperClusterTrackAtVtx()
#    nhits = ele.sourcePtr().gsfTrack().recHitsSize() #PG all the hits
    nvhits = ele.sourcePtr().gsfTrack().found() #PG valid hits
    sihih = ele.sourcePtr().sigmaIetaIeta() 
#    print 'TEST',hoe,deta,dphi,nvhits,sihih,ele.dxy(),ele.dz(),ele.sourcePtr().isEB()
    if ele.dxy() >= 0.045 : return False
    if ele.dz()  >= 0.2   : return False
    if nhits     >  999   : return False
    if deta      >= 0.10  : return False
    if hoe       >= 999   : return False
    if ele.sourcePtr().isEB() :
        if deta  >= 0.007     : return False
        if hoe   >= 0.15      : return False
        if sihih >= 0.010     : return False
        if dphi  >= 0.80      : return False 
    return True


def testEleLoosePhil( ele, ptCut = 15, isoCut = 0.3 ):
    '''Loose electron selection, for the lepton veto, according to Phil sync prescription'''
    if ele.pt()        < ptCut  : return False
    if ele.relIso(0.5) > isoCut : return False
    if ele.dxy()       >= 0.045 : return False
    if ele.dz()        >= 0.2   : return False
#    eop = ele.sourcePtr().eSuperClusterOverP()
    hoe = ele.sourcePtr().hcalOverEcal()
    deta = ele.sourcePtr().deltaEtaSuperClusterTrackAtVtx()
    dphi = ele.sourcePtr().deltaPhiSuperClusterTrackAtVtx()
#    nvhits = ele.sourcePtr().gsfTrack().found() #PG valid hits
    sihih = ele.sourcePtr().sigmaIetaIeta() 
    if ele.sourcePtr().isEB() :
        if sihih >= 0.010     : return False
        if dphi  >= 0.80      : return False 
        if deta  >= 0.007     : return False
        if hoe   >= 0.15      : return False
    elif ele.sourcePtr().isEE() :
        if sihih >= 0.030     : return False
        if dphi  >= 0.70      : return False 
        if deta  >= 0.010     : return False
    else : return False
    return True


def electronAccept( allLeptons ):
    ''' returns True if the additional lepton veto is successful'''
    looseLeptons = filter( testEleLoosePhil, allLeptons )
    nLeptons = len(looseLeptons)
    if nLeptons < 2 : return True
    if nLeptons > 2 : return False
    if looseLeptons[0].charge() == looseLeptons[1].charge() : return True
    if deltaR (looseLeptons[0].eta(), looseLeptons[0].phi(), looseLeptons[1].eta(), looseLeptons[1].phi()) < 0.15 : return True 
    else : return False
    

