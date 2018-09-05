from PhysicsTools.Heppy.physicsobjects.PhysicsObject import *
import ROOT

class Lepton( PhysicsObject):
    def ip3D(self):
        '''3D impact parameter value.'''
        return abs(self.dB(self.PV3D))
 
    def sip3D(self):
        '''3D impact parameter significance.'''
        db, edb = self.dB(self.PV3D), self.edB(self.PV3D)
        return abs(db/edb) if edb > 0 else 999.

    def absIso(self, cone_size, iso_type, dbeta_factor=None, area=None, 
               all_charged = False):
        iso_ch = None
        if all_charged:
            iso_ch = self.chargedAllIso(cone_size)
        else:
            iso_ch = self.chargedHadronIso(cone_size)
        iso_photon = self.photonIso(cone_size)
        iso_nh = self.neutralHadronIso(cone_size)
        corr_neutral = None
        if iso_type == 'EA':
            if dbeta_factor:  
                raise ValueError('using EA iso, do not specify dbeta_factor')
            if area is None: 
                raise ValueError('using EA iso, provide area, e.g. "03"')
            elif area != '03':
                raise ValueError('the only EA iso to be supported is "03"')
            corr_neutral = self.rho * getattr(self, '_'.join(["effarea", area]))
        elif iso_type == 'dbeta': 
            if area: 
                raise ValueError('using delta beta iso, do not specify area')
            if dbeta_factor is None: 
                raise ValueError('using delta beta iso, provide dbeta_factor')
            iso_photon = self.photonIso(cone_size)
            iso_ch = self.chargedHadronIso(cone_size)
            iso_nh = self.neutralHadronIso(cone_size)
            iso_puch =  self.puChargedHadronIso(cone_size)
            corr_neutral = dbeta_factor * iso_puch
        elif iso_type == 'raw':
            corr_neutral = 0.
        else: 
            raise ValueError('unknown isolation type: '+iso_type)
        return iso_ch + max(0., iso_photon + iso_nh - corr_neutral)     

    def relIso(self, cone_size, iso_type, dbeta_factor=None, area=None, 
               all_charged = False):
        return self.absIso(cone_size, iso_type, dbeta_factor, area, all_charged) / self.pt()

    def absIsoFromEA(self, area='04'):
        '''Calculate absolute isolation using the effective area approach, 
        for backward compatibility, should be removed. 
        '''
        return self.absIso(0.4, 'EA', area=area)
    
    def relIsoFromEA(self, area='04'):
        '''Calculate relative isolation using the effective area approach, 
        for backward compatibility, should be removed. 
        '''
        return self.relIso(0.4, 'EA', area=area)
    
    def absIsoR(self, R=0.4, dbeta_factor=0, all_charged=False):
        '''Calculate absolute isolation in given cone 
        with optional delta-beta subtraction.
        for backward compatibility, should be removed. 
        '''
        return self.absIso(R, 'dbeta', dbeta_factor=dbeta_factor,
                           all_charged = all_charged) 
    
    def relIsoR(self, R=0.4, dBetaFactor=0, all_charged=False):
        '''Calculate relative isolation in given cone 
        with optional delta-beta subtraction.
        for backward compatibility, should be removed. 
        '''
        return self.relIso(R, 'dbeta', dbeta_factor=dbeta_factor,
                           all_charged = all_charged) 

    def lostInner(self):
        if hasattr(self.innerTrack(),"trackerExpectedHitsInner") :
		return self.innerTrack().trackerExpectedHitsInner().numberOfLostHits()
	else :	
		return self.innerTrack().hitPattern().numberOfLostHits(ROOT.reco.HitPattern.MISSING_INNER_HITS)	

    def p4WithFSR(self):
        ret = self.p4()
        for p in getattr(self, 'ownFsrPhotons', getattr(self, 'fsrPhotons', [])):
            ret += p.p4()
        return ret

    def __str__(self):
        ptc = super(Lepton, self).__str__()
        return '{ptc}, iso={iso:5.2f}'.format(ptc=ptc, iso=self.relIso())
