from PhysicsTools.Heppy.physicsobjects.Lepton import Lepton
from RecoEgamma.ElectronIdentification.FWLite import electron_mvas, working_points
from PhysicsTools.Heppy.physicsutils.electronID_Egamma_dict import electronID_Egamma_dict
from PhysicsTools.HeppyCore.utils.deltar import deltaR
import ROOT
import sys
from math import exp

def raw_to_normalized(raw):
    return 2.0/(1.0+exp(-2.0*raw))-1

class Electron( Lepton ):

    def __init__(self, *args, **kwargs):
        '''Initializing tightIdResult to None. The user is responsible
        for setting this attribute externally if he wants to use the tightId
        function.'''
        super(Electron, self).__init__(*args, **kwargs)
        self._physObjInit()

    def _physObjInit(self):
        self.tightIdResult = None
        self.associatedVertex = None
        self.rho              = None
        self.rhoHLT           = None
        self._mvaNonTrigV0  = {True:None, False:None}
        self._mvaTrigV0     = {True:None, False:None}
        self._mvaTrigNoIPV0 = {True:None, False:None}
        self._mvaRun2 = {}

    def electronID_category(self, id):
        return self.electronID(id)[0]

    def electronID_score_raw(self, id):
        return self.electronID(id)[1]

    def electronID_score_norm(self, id):
        score_raw = self.electronID_score_raw(id)
        if not score_raw == None :
            return raw_to_normalized(score_raw)
        else:
            return None

    def electronID_passed(self, id):
        return self.electronID(id)[2]

    def electronID(self, id):
        # Initializing ID outputs
        category = None
        score_raw = None
        passed = True # value by default!

        # If id is not given, return True by default.
        # Use also to stop looking for the id if it is found.
        ID_done = id is None or id == ""

        # First, try to get directly from miniAOD
        # Only gives if passed or not
        miniAODids = [miniAODid[0] for miniAODid in self.electronIDs()]
        if id in miniAODids and not ID_done: 
            ID_done = True
            passed = self.electronIDs()[miniAODids.index(id)][1]

        # Else, compute it with FWLite and Egamma code
        if id in electronID_Egamma_dict.keys() and not ID_done :
            name, wp = electronID_Egamma_dict[id]
            if name in electron_mvas.keys():
                ID_done = True
                score_raw, category = electron_mvas[name](self.physObj, self.conversions, self.beamspot, [self.rho])
                passed = working_points[name].passed(self.physObj, score_raw, category, wp)

        if ID_done :
            return category, score_raw, passed
        else :
            raise RuntimeError(
                "Electron id " + id \
                + " not yet implemented in Electron.py, availables are:" \
                + "\n\n from miniAOD:\n {}".format(miniAODids) \
                + "\n\n from FWLite:\n {}".format(electronID_Egamma_dict.keys())
                )

    def dEtaInSeed(self):
        if self.physObj.superCluster().isNonnull() and self.physObj.superCluster().seed().isNonnull(): return self.physObj.deltaEtaSuperClusterTrackAtVtx() - self.physObj.superCluster().eta() + self.physObj.superCluster().seed().eta()
        else: return sys.float_info.max

    def normalizedGsfChi2(self):
        if self.physObj.gsfTrack().isNonnull(): return self.physObj.gsfTrack().normalizedChi2()
        else: return sys.float_info.max

    def hltPFIso(self,isoType):
        SCEta = abs(self.superCluster().eta())
        hltEA = 0.0
        if isoType == 'ECALPFIsoEA': hltEA = 0.165 if SCEta < 1.4790 else 0.132
        elif isoType == 'HCALPFIsoEA': hltEA = 0.060 if SCEta < 1.4790 else 0.131
        if 'ECALPFIso' in isoType: isoValue = self.ecalPFClusterIso()
        elif 'HCALPFIso' in isoType: isoValue = self.hcalPFClusterIso()
        else: isoValue = -999
        return max(0, isoValue - self.rhoHLT*hltEA)

    def chargedHadronIso(self,R=0.4):
        if   R == 0.3: return self.physObj.pfIsolationVariables().sumChargedHadronPt
        elif R == 0.4: return self.physObj.chargedHadronIso()
        raise RuntimeError("Electron chargedHadronIso missing for R=%s" % R)

    def neutralHadronIso(self,R=0.4):
        if   R == 0.3: return self.physObj.pfIsolationVariables().sumNeutralHadronEt
        elif R == 0.4: return self.physObj.neutralHadronIso()
        raise RuntimeError("Electron neutralHadronIso missing for R=%s" % R)

    def photonIso(self,R=0.4):
        if   R == 0.3: return self.physObj.pfIsolationVariables().sumPhotonEt
        elif R == 0.4: return self.physObj.photonIso()
        raise RuntimeError("Electron photonIso missing for R=%s" % R)

    def chargedAllIso(self,R=0.4):
        if   R == 0.3: return self.physObj.pfIsolationVariables().sumChargedParticlePt
        raise RuntimeError("Electron chargedAllIso missing for R=%s" % R)

    def puChargedHadronIso(self,R=0.4):
        if   R == 0.3: return self.physObj.pfIsolationVariables().sumPUPt
        elif R == 0.4: return self.physObj.puChargedHadronIso()
        raise RuntimeError("Electron chargedHadronIso missing for R=%s" % R)


    def absIsoWithFSR(self, R=0.4, puCorr="rhoArea", dBetaFactor=0.5):
        '''
        Calculate Isolation, subtract FSR, apply specific PU corrections" 
        '''
        photonIso = self.photonIso(R)
        if hasattr(self,'fsrPhotons'):
            for gamma in self.fsrPhotons:
                dr = deltaR(gamma.eta(), gamma.phi(), self.physObj.eta(), self.physObj.phi())
                if (self.isEB() or dr > 0.08) and dr < R:
                    photonIso = max(photonIso-gamma.pt(),0.0)                
        if puCorr == "deltaBeta":
            offset = dBetaFactor * self.puChargedHadronIso(R)
        elif puCorr == "rhoArea":
            offset = self.rho*getattr(self,"EffectiveArea"+(str(R).replace(".","")))
        elif puCorr in ["none","None",None]:
            offset = 0
        else:
             raise RuntimeError("Unsupported PU correction scheme %s" % puCorr)
        return self.chargedHadronIso(R)+max(0.,photonIso+self.neutralHadronIso(R)-offset)            


    def dxy(self, vertex=None):
        '''Returns dxy.
        Computed using vertex (or self.associatedVertex if vertex not specified),
        and the gsf track.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return self.gsfTrack().dxy( vertex.position() )

    def edxy(self):
        '''returns the uncertainty on dxy (from gsf track)'''
        return self.gsfTrack().dxyError()

    def p4(self):
	 return ROOT.reco.Candidate.p4(self.physObj)

#    def p4(self):
#        return self.physObj.p4(self.physObj.candidateP4Kind()) # if kind == None else kind)

    def dz(self, vertex=None):
        '''Returns dz.
        Computed using vertex (or self.associatedVertex if vertex not specified),
        and the gsf track.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return self.gsfTrack().dz( vertex.position() )

    def edz(self):
        '''returns the uncertainty on dxz (from gsf track)'''
        return self.gsfTrack().dzError()


    def lostInner(self) :
        if hasattr(self.gsfTrack(),"trackerExpectedHitsInner") :
		return self.gsfTrack().trackerExpectedHitsInner().numberOfLostHits()
	else :
		return self.gsfTrack().hitPattern().numberOfLostHits(ROOT.reco.HitPattern.MISSING_INNER_HITS)

    def validCandidateP4Kind(self):
        raw = self.physObj.candidateP4Kind()
        return raw in (0,1,2) 

    def ptErr(self):
        return self.p4Error(self.candidateP4Kind())*self.pt()/self.p() if self.validCandidateP4Kind() else None
 
