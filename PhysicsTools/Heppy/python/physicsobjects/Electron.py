from PhysicsTools.Heppy.physicsobjects.Lepton import Lepton
from RecoEgamma.ElectronIdentification.FWLite import electron_mvas, working_points
from PhysicsTools.Heppy.physicsutils.electronID_Egamma_dict import wps_dict, methods_dict
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
        self._mvaid_category = {}
        self._mvaid_score = {}
        self._mvaid_normscore = {}
        self._mvaid_passed = {}

    def mva_category(self, name):
        '''takes an mva name and returns the category of the electron,
        only available using FWLite and Egamma code.'''
        if name not in self._mvaid_category :
            category = self._mva_score_and_category(name)[1]
        return self._mvaid_category[name]

    def mvaId(self, name):
        '''For a transparent treatment of electrons, muons and taus.'''
        return self.mva_score(name)

    def mva_score(self, name, norm=False):
        '''returns the score of the given mva,
        only available using FWLite and Egamma code.'''
        if norm :
            if name not in self._mvaid_normscore :
                score_raw = self._mva_score_and_category(name)[0]
            return self._mvaid_normscore[name]
        else:
            if name not in self._mvaid_score :
                score_raw = self._mva_score_and_category(name)[0]
            return self._mvaid_score[name]

    def mva_passes(self, name, wp):
        '''returns True if the electron passes the given working point of the mva'''
        id = name + '-' + wp
        if id not in self._mvaid_passed :
            miniAODids = [miniAODid[0] for miniAODid in self.electronIDs()]
            if id in miniAODids :
                passed = self.electronIDs()[miniAODids.index(id)][1]
            elif id in wps_dict.keys() :
                if name not in self._mvaid_score or name not in self._mvaid_category :
                    score_raw, category = self._mva_score_and_category(name)
                FWLitename, FWLitewp = wps_dict[id]
                passed = working_points[FWLitename].passed(
                    self.physObj,
                    self._mvaid_score[name],
                    self._mvaid_category[name],
                    FWLitewp
                    )
            else:
                raise RuntimeError(
                    "Electron id " + id \
                        + " not yet implemented in Electron.py, availables are:" \
                        + "\n\n from miniAOD:\n {}".format(miniAODids) \
                        + "\n\n from FWLite:\n {}".format(wps_dict.keys())
                    )
            self._mvaid_passed[id] = 1. if passed else 0.
        return self._mvaid_passed[id]

    def countWP(self,name):
        '''Returns the number of Working Points 
        that are passed for given ID name.
        
        For example if an electron only passes Loose, and 80,
        Working Points, this will return 2.
        Uses self.electronID() to evaluate the WPs.
        Will test all WPs in ['wp90', 'wp80', 'wpLoose'].
        If any WP is not available it is skipped.
        '''
        WPs = ['wp90', 'wp80', 'Loose']
        n_WP = 0
        for WP in WPs:
            try:
                if self.electronID(name, WP):
                    n_WP += 1
            except RuntimeError:
                continue #WP not found so it is skipped
        return n_WP

    def electronID(self, name, wp):
        '''For a transparent treatment of electrons, muons and taus,
        in the case of electrons this is only equivalent to mva_passes.'''
        return self.mva_passes(name, wp)

    def _mva_score_and_category(self, name):
        '''Returns the mva score and the electron category for the given mva'''
        if name in methods_dict.keys() :
            FWLitename = methods_dict[name]
            score_raw, category = electron_mvas[FWLitename](
                self.physObj,
                self.conversions,
                self.beamspot,
                [self.rho]
                )
            self._mvaid_category[name] = category
            self._mvaid_score[name] = score_raw
            self._mvaid_normscore[name] = raw_to_normalized(score_raw)
            return self._mvaid_score[name], self._mvaid_category[name]
        else:
            raise RuntimeError(
                "Electron mva " + name \
                + " not available in FWLite,\n" \
                + "availables are:\n {}".format(methods_dict.keys())
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
 
