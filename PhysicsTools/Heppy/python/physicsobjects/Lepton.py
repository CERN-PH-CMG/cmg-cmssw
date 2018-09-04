from PhysicsTools.Heppy.physicsobjects.PhysicsObject import *
from PhysicsTools.Heppy.physicsutils.EffectiveAreas import is_ea_table

import ROOT

class Lepton( PhysicsObject):

    def ip3D(self):
        '''3D impact parameter value.'''
        return abs(self.dB(self.PV3D))
 
    def sip3D(self):
        '''3D impact parameter significance.'''
        db, edb = self.dB(self.PV3D), self.edB(self.PV3D)
        return abs(db/edb) if edb > 0 else 999.

    def absIso(self, cone_size, iso_type, dbeta_factor=None, 
               area_cone_size=None, area_table=None, all_charged = False):
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
            if area_cone_size is None: 
                raise ValueError('using EA iso, provide area_cone_size, e.g. "03"')
            else:
                area = effective_area(area_cone_size, area_table)
            corr_neutral = self.rho * area
        elif iso_type == 'dbeta': 
            if area_cone_size or area_table: 
                raise ValueError('using delta beta iso, do not specify area_cone_size or area table')
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


    def relIso(self, cone_size, iso_type, dbeta_factor=None, 
               area_cone_size=None, area_table=None,
               all_charged = False):
        return self.absIso(cone_size, iso_type, 
                           dbeta_factor, 
                           area_cone_size, area_table, 
                           all_charged) / self.pt()

    def effective_area(area_cone_size, area_table):
        if area_table and not is_ea_table(area_table):
            raise ValueError('area_table is not an area table')
        if area_table is None: 
            # area table not provided by the user as argument to the isolation method.
            # try to find it in the object or its class
            ea_attr_name = ''.join(['EffectiveArea',area_cone_size])
            area_table = getattr(self, ea_attr_name, None)
            if area_table is None: 
                area_table = getattr(self.__class__, ea_attr_name, None)
                if area_table is None: 
                    raise ValueError('{} not found. It should be added to the lepton object as an attribute or a class attribute.')
        return effective_area(self, area_cone_size, area_table)


    def absIsoFromEA(self, area_cone_size='04'):
        '''Calculate absolute isolation using the effective area approach, 
        for backward compatibility, should be removed. 
        '''
        return self.absIso(0.4, 'EA', area_cone_size=area_cone_size)
    
    def relIsoFromEA(self, area_cone_size='04'):
        '''Calculate relative isolation using the effective area approach, 
        for backward compatibility, should be removed. 
        '''
        return self.relIso(0.4, 'EA', area_cone_size=area_cone_size)
    
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
