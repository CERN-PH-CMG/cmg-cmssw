from PhysicsTools.Heppy.physicsobjects.Lepton import Lepton
from RecoEgamma.ElectronIdentification.FWLite import electron_mvas, working_points, electron_cut_based_IDs
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

    def electronID( self, id, wp=None):
        if id is None or id == "": return True
        if wp is not None:
            return self.id_passes(id, wp)
        if   id == "POG_MVA_ID_Spring15_NonTrig_VLoose":   return self.mvaIDRun2Custom("NonTrigSpring15MiniAOD","VLoose")
        elif id == "POG_MVA_ID_Spring15_NonTrig_VLooseIdEmu":   return self.mvaIDRun2Custom("NonTrigSpring15MiniAOD","VLooseIdEmu")
        elif id == "POG_MVA_ID_Spring15_NonTrig_VLooseIdIsoEmu":   return self.mvaIDRun2Custom("NonTrigSpring15MiniAOD","VLooseIdIsoEmu")
        elif id == "POG_MVA_ID_Spring15_NonTrig_Tight":    return self.mvaIDRun2Custom("NonTrigSpring15MiniAOD","Tight")
        elif id == "MVA_ID_NonTrig_Spring15_HZZ":     return self.id_passes("mvaEleID-Spring15-25ns-nonTrig-V1","wpLoose")
        elif id == "MVA_ID_NonTrig_Spring16_HZZ":     return self.id_passes("mvaEleID-Spring16-HZZ-V1","wpLoose") 
        elif id == "MVA_ID_NonTrig_Spring16_VLoose":   return self.mvaIDRun2Custom("Spring16","VLoose")
        elif id == "MVA_ID_NonTrig_Spring16_VLooseIdEmu":   return self.mvaIDRun2Custom("Spring16","VLooseIdEmu")
        elif id == "MVA_ID_NonTrig_Spring16_Tight":    return self.mvaIDRun2Custom("Spring16","Tight")
        elif id == "MVA_ID_nonIso_Fall17_Loose":       return self.id_passes("mvaEleID-Fall17-noIso-V1","wpLoose")
        elif id == "MVA_ID_nonIso_Fall17_wp90":        return self.id_passes("mvaEleID-Fall17-noIso-V1","wp90")
        elif id == "MVA_ID_nonIso_Fall17_wp80":        return self.id_passes("mvaEleID-Fall17-noIso-V1","wp80")
        elif id == "MVA_ID_nonIso_Fall17_SUSYVLooseFO": return self.mvaIDRun2Custom("Fall17noIso","SUSYVLooseFO")
        elif id == "MVA_ID_nonIso_Fall17_SUSYVLoose":   return self.mvaIDRun2Custom("Fall17noIso","SUSYVLoose")
        elif id == "MVA_ID_nonIso_Fall17_SUSYTight":    return self.mvaIDRun2Custom("Fall17noIso","SUSYTight")
        elif id == "MVA_ID_Iso_Fall17_Loose":       return self.id_passes("mvaEleID-Fall17-iso-V1","wpLoose")
        elif id == "MVA_ID_Iso_Fall17_wp90":        return self.id_passes("mvaEleID-Fall17-iso-V1","wp90")
        elif id == "MVA_ID_Iso_Fall17_wp80":        return self.id_passes("mvaEleID-Fall17-iso-V1","wp80")
        elif id.startswith("POG_Cuts_ID_"):
                return self.cutBasedIdCustom(id.replace("POG_Cuts_ID_","POG_"))
        for ID in self.electronIDs():
            if ID.first == id:
                return ID.second
        raise RuntimeError("Electron id '%s' not yet implemented in Electron.py" % id)

    def cutBasedIdCustom(self, wp, showerShapes="auto"):
        if "_zs" in wp:
            showerShapes = "zs"
            wp = wp.replace("_zs","")
        elif showerShapes == "auto":
            showerShapes = "full5x5"

        hOverE_CE, hOverE_Cr = 0, 0
        if "POG_FALL17_94X_v1" in wp:
            hOverE_CE = 1.12 if self.physObj.isEB() else 0.5
            hOverE_Cr = 0.0368 if self.physObj.isEB() else 0.201

        vars = {
            'dEtaIn' : abs(self.dEtaInSeed()) if (("POG_SPRING16_25ns_v1" in wp) or ("POG_FALL17_94X_v1" in wp)) else abs(self.physObj.deltaEtaSuperClusterTrackAtVtx()),
            'dPhiIn' : abs(self.physObj.deltaPhiSuperClusterTrackAtVtx()),
            'sigmaIEtaIEta' : self.physObj.full5x5_sigmaIetaIeta() if showerShapes == "full5x5" else self.physObj.sigmaIetaIeta(),
            'H/E' : self.physObj.hadronicOverEm() - (hOverE_CE  + hOverE_Cr * self.rho)/(self.physObj.superCluster().energy()),
            #'1/E-1/p' : abs(1.0/self.physObj.ecalEnergy() - self.physObj.eSuperClusterOverP()/self.physObj.ecalEnergy()),
            '1/E-1/p' : abs(1.0/self.physObj.ecalEnergy() - self.physObj.eSuperClusterOverP()/self.physObj.ecalEnergy()) if self.physObj.ecalEnergy()>0. else 9e9,
            'conversionVeto' : self.physObj.passConversionVeto(),
            'missingHits' : self.physObj.gsfTrack().hitPattern().numberOfLostHits(ROOT.reco.HitPattern.MISSING_INNER_HITS), # http://cmslxr.fnal.gov/source/DataFormats/TrackReco/interface/HitPattern.h?v=CMSSW_7_2_3#0153
            'dxy' : abs(self.dxy()),
            'dz' : abs(self.dz()),
            'chi2' : self.normalizedGsfChi2(),
            'ECALPFIsoEA' : self.hltPFIso('ECALPFIsoEA')/self.pt() if "HLT" in wp else None,
            'HCALPFIsoEA' : self.hltPFIso('HCALPFIsoEA')/self.pt() if "HLT" in wp else None,
            'trkIso' : self.dr03TkSumPt()/self.pt(),
        }
        WP = {
            ## ------- https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2?rev=24#Working_points_for_Spring15_MC_s
            'POG_SPRING15_50ns_v2_Veto'   :  [('dEtaIn', [0.01260, 0.01090]), ('dPhiIn', [0.1070, 0.2190]), ('sigmaIEtaIEta', [0.0120, 0.0339]), ('H/E', [0.1860, 0.0962]), ('1/E-1/p', [0.2390, 0.141])],
            'POG_SPRING15_50ns_v2_Loose'  :  [('dEtaIn', [0.00976, 0.00952]), ('dPhiIn', [0.0929, 0.1810]), ('sigmaIEtaIEta', [0.0105, 0.0318]), ('H/E', [0.0765, 0.0824]), ('1/E-1/p', [0.1840, 0.125])],
            'POG_SPRING15_50ns_v2_Medium' :  [('dEtaIn', [0.00940, 0.00773]), ('dPhiIn', [0.0296, 0.1480]), ('sigmaIEtaIEta', [0.0101, 0.0287]), ('H/E', [0.0372, 0.0546]), ('1/E-1/p', [0.1180, 0.104])],
            'POG_SPRING15_50ns_v2_Tight'  :  [('dEtaIn', [0.00864, 0.00762]), ('dPhiIn', [0.0291, 0.0439]), ('sigmaIEtaIEta', [0.0101, 0.0287]), ('H/E', [0.0372, 0.0544]), ('1/E-1/p', [0.0116, 0.010])],
            'POG_SPRING15_25ns_v1_Veto'   :  [('dEtaIn', [0.01520, 0.01130]), ('dPhiIn', [0.2160, 0.2370]), ('sigmaIEtaIEta', [0.0114, 0.0352]), ('H/E', [0.1810, 0.1160]), ('1/E-1/p', [0.2070, 0.17400])],
            'POG_SPRING15_25ns_v1_Loose'  :  [('dEtaIn', [0.01050, 0.00814]), ('dPhiIn', [0.1150, 0.1820]), ('sigmaIEtaIEta', [0.0103, 0.0301]), ('H/E', [0.1040, 0.0897]), ('1/E-1/p', [0.1020, 0.12600])],
            'POG_SPRING15_25ns_v1_Medium' :  [('dEtaIn', [0.01030, 0.00733]), ('dPhiIn', [0.0336, 0.1140]), ('sigmaIEtaIEta', [0.0101, 0.0283]), ('H/E', [0.0876, 0.0678]), ('1/E-1/p', [0.0174, 0.08980])],
            'POG_SPRING15_25ns_v1_Tight'  :  [('dEtaIn', [0.00926, 0.00724]), ('dPhiIn', [0.0336, 0.0918]), ('sigmaIEtaIEta', [0.0101, 0.0279]), ('H/E', [0.0597, 0.0615]), ('1/E-1/p', [0.0120, 0.00999])],
            ## ------- https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Working_points_for_2016_data_for
            'POG_SPRING16_25ns_v1_Veto'   :  [('dEtaIn', [0.00749, 0.00895]), ('dPhiIn', [0.2280, 0.2130]), ('sigmaIEtaIEta', [0.01150, 0.0370]), ('H/E', [0.3560, 0.2110]), ('1/E-1/p', [0.2990, 0.15])],
            'POG_SPRING16_25ns_v1_Loose'  :  [('dEtaIn', [0.00477, 0.00868]), ('dPhiIn', [0.2220, 0.2130]), ('sigmaIEtaIEta', [0.01100, 0.0314]), ('H/E', [0.2980, 0.1010]), ('1/E-1/p', [0.2410, 0.14])],
            'POG_SPRING16_25ns_v1_Medium' :  [('dEtaIn', [0.00311, 0.00609]), ('dPhiIn', [0.1030, 0.0450]), ('sigmaIEtaIEta', [0.00998, 0.0298]), ('H/E', [0.2530, 0.0878]), ('1/E-1/p', [0.1340, 0.13])],
            'POG_SPRING16_25ns_v1_Tight'  :  [('dEtaIn', [0.00308, 0.00605]), ('dPhiIn', [0.0816, 0.0394]), ('sigmaIEtaIEta', [0.00998, 0.0292]), ('H/E', [0.0414, 0.0641]), ('1/E-1/p', [0.0129, 0.0129])],
            'POG_SPRING16_25ns_v1_HLT'    :  [('dEtaIn', [0.004, 999]), ('dPhiIn', [0.020, 999]), ('sigmaIEtaIEta', [0.011, 0.031]), ('H/E', [0.060, 0.065]), ('1/E-1/p', [0.013, 0.013]), ('chi2', [sys.float_info.max, 3.0]), ('ECALPFIsoEA', [0.160, 0.120]), ('HCALPFIsoEA', [0.120, 0.120]), ('trkIso', [0.08, 0.08])],
            ## ------- https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Working_points_for_92X_and_later
            'POG_FALL17_94X_v1_Veto'      :  [('dEtaIn', [0.00523, 0.00984]), ('dPhiIn', [0.1590, 0.1570]), ('sigmaIEtaIEta', [0.0128, 0.0445]), ('H/E', [0.050, 0.0500]), ('1/E-1/p', [0.1930, 0.0962])],
            'POG_FALL17_94X_v1_Loose'     :  [('dEtaIn', [0.00387, 0.00720]), ('dPhiIn', [0.0716, 0.1470]), ('sigmaIEtaIEta', [0.0105, 0.0356]), ('H/E', [0.050, 0.0414]), ('1/E-1/p', [0.1290, 0.0875])],
            'POG_FALL17_94X_v1_Medium'    :  [('dEtaIn', [0.00365, 0.00625]), ('dPhiIn', [0.0588, 0.0355]), ('sigmaIEtaIEta', [0.0105, 0.0309]), ('H/E', [0.026, 0.0260]), ('1/E-1/p', [0.0327, 0.0335])],
            'POG_FALL17_94X_v1_Tight'     :  [('dEtaIn', [0.00353, 0.00567]), ('dPhiIn', [0.0499, 0.0165]), ('sigmaIEtaIEta', [0.0104, 0.0305]), ('H/E', [0.026, 0.0260]), ('1/E-1/p', [0.0278, 0.0158])],
        }
        WP_conversion_veto = {
            # missing Hits incremented by 1 because we return False if >=, note the '='
            ## ------- https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2?rev=24#Working_points_for_Spring15_MC_s
            'POG_SPRING15_50ns_v2_ConvVeto_Veto'   :  WP['POG_SPRING15_50ns_v2_Veto'  ]+[('conversionVeto', [True, True]), ('missingHits', [3, 4])],
            'POG_SPRING15_50ns_v2_ConvVeto_Loose'  :  WP['POG_SPRING15_50ns_v2_Loose' ]+[('conversionVeto', [True, True]), ('missingHits', [3, 2])],
            'POG_SPRING15_50ns_v2_ConvVeto_Medium' :  WP['POG_SPRING15_50ns_v2_Medium']+[('conversionVeto', [True, True]), ('missingHits', [3, 2])],
            'POG_SPRING15_50ns_v2_ConvVeto_Tight'  :  WP['POG_SPRING15_50ns_v2_Tight' ]+[('conversionVeto', [True, True]), ('missingHits', [3, 2])],
            'POG_SPRING15_25ns_v1_ConvVeto_Veto'   :  WP['POG_SPRING15_25ns_v1_Veto'  ]+[('conversionVeto', [True, True]), ('missingHits', [3, 4])],
            'POG_SPRING15_25ns_v1_ConvVeto_Loose'  :  WP['POG_SPRING15_25ns_v1_Loose' ]+[('conversionVeto', [True, True]), ('missingHits', [3, 2])],
            'POG_SPRING15_25ns_v1_ConvVeto_Medium' :  WP['POG_SPRING15_25ns_v1_Medium']+[('conversionVeto', [True, True]), ('missingHits', [3, 2])],
            'POG_SPRING15_25ns_v1_ConvVeto_Tight'  :  WP['POG_SPRING15_25ns_v1_Tight' ]+[('conversionVeto', [True, True]), ('missingHits', [3, 2])],
            ## ------- https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Working_points_for_2016_data_for
            'POG_SPRING16_25ns_v1_ConvVeto_Veto'   :  WP['POG_SPRING16_25ns_v1_Veto'  ]+[('conversionVeto', [True, True]), ('missingHits', [3, 4])],
            'POG_SPRING16_25ns_v1_ConvVeto_Loose'  :  WP['POG_SPRING16_25ns_v1_Loose' ]+[('conversionVeto', [True, True]), ('missingHits', [2, 2])],
            'POG_SPRING16_25ns_v1_ConvVeto_Medium' :  WP['POG_SPRING16_25ns_v1_Medium']+[('conversionVeto', [True, True]), ('missingHits', [2, 2])],
            'POG_SPRING16_25ns_v1_ConvVeto_Tight'  :  WP['POG_SPRING16_25ns_v1_Tight' ]+[('conversionVeto', [True, True]), ('missingHits', [2, 2])],
            ## ------- https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Working_points_for_92X_and_later
            'POG_FALL17_94X_v1_ConvVeto_Veto'   :  WP['POG_FALL17_94X_v1_Veto'  ]+[('conversionVeto', [True, True]), ('missingHits', [3, 4])],
            'POG_FALL17_94X_v1_ConvVeto_Loose'  :  WP['POG_FALL17_94X_v1_Loose' ]+[('conversionVeto', [True, True]), ('missingHits', [2, 2])],
            'POG_FALL17_94X_v1_ConvVeto_Medium' :  WP['POG_FALL17_94X_v1_Medium']+[('conversionVeto', [True, True]), ('missingHits', [2, 2])],
            'POG_FALL17_94X_v1_ConvVeto_Tight'  :  WP['POG_FALL17_94X_v1_Tight' ]+[('conversionVeto', [True, True]), ('missingHits', [2, 2])],
        }

        WP.update(WP_conversion_veto)

        WP_conversion_veto_DxyDz = {
            ## ------- https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2?rev=24#Working_points_for_Spring15_MC_s
            'POG_SPRING15_50ns_v2_ConvVetoDxyDz_Veto'   :  WP['POG_SPRING15_50ns_v2_ConvVeto_Veto'  ]+[('dxy',[0.0621, 0.2790]), ('dz',[0.613, 0.947])],
            'POG_SPRING15_50ns_v2_ConvVetoDxyDz_Loose'  :  WP['POG_SPRING15_50ns_v2_ConvVeto_Loose' ]+[('dxy',[0.0227, 0.2420]), ('dz',[0.379, 0.921])],
            'POG_SPRING15_50ns_v2_ConvVetoDxyDz_Medium' :  WP['POG_SPRING15_50ns_v2_ConvVeto_Medium']+[('dxy',[0.0151, 0.0535]), ('dz',[0.238, 0.572])],
            'POG_SPRING15_50ns_v2_ConvVetoDxyDz_Tight'  :  WP['POG_SPRING15_50ns_v2_ConvVeto_Tight' ]+[('dxy',[0.0144, 0.0377]), ('dz',[0.170, 0.571])],
            'POG_SPRING15_25ns_v1_ConvVetoDxyDz_Veto'   :  WP['POG_SPRING15_25ns_v1_ConvVeto_Veto'  ]+[('dxy',[0.0564, 0.2220]), ('dz',[0.472, 0.921])],
            'POG_SPRING15_25ns_v1_ConvVetoDxyDz_Loose'  :  WP['POG_SPRING15_25ns_v1_ConvVeto_Loose' ]+[('dxy',[0.0261, 0.1180]), ('dz',[0.410, 0.822])],
            'POG_SPRING15_25ns_v1_ConvVetoDxyDz_Medium' :  WP['POG_SPRING15_25ns_v1_ConvVeto_Medium']+[('dxy',[0.0118, 0.0739]), ('dz',[0.373, 0.602])],
            'POG_SPRING15_25ns_v1_ConvVetoDxyDz_Tight'  :  WP['POG_SPRING15_25ns_v1_ConvVeto_Tight' ]+[('dxy',[0.0111, 0.0351]), ('dz',[0.0466,0.417])],
        }
        ## ------- in Spring16, not optimised simultaneously to the rest of ID. Cuts independent on WP
        for wps in ['Veto','Loose','Medium','Tight']:
            WP_conversion_veto_DxyDz['POG_SPRING16_25ns_v1_ConvVetoDxyDz_%s' % wps] =  WP['POG_SPRING16_25ns_v1_ConvVeto_%s' % wps ]+[('dxy',[0.05, 0.10]), ('dz',[0.10,0.20])]
        ## ------- in   Fall17, not optimised simultaneously to the rest of ID. Cuts independent on WP
        for wps in ['Veto','Loose','Medium','Tight']:
            WP_conversion_veto_DxyDz['POG_FALL17_94X_v1_ConvVetoDxyDz_%s' % wps] =  WP['POG_FALL17_94X_v1_ConvVeto_%s' % wps ]+[('dxy',[0.05, 0.10]), ('dz',[0.10,0.20])]

        WP.update(WP_conversion_veto_DxyDz)


        if wp not in WP:
            raise RuntimeError("Working point '%s' not yet implemented in Electron.py" % wp)
        for (cut_name,(cut_eb,cut_ee)) in WP[wp]:
            if cut_name == 'conversionVeto':
                if (cut_eb if self.physObj.isEB() else cut_ee) and not vars[cut_name]:
                    return False
            elif vars[cut_name] >= (cut_eb if self.physObj.isEB() else cut_ee):
                return False
        return True

    def tightId( self ):
        return self.tightIdResult

    def hltId( self ):
        return self.hltSafeIdResult

    def mvaRun2(self, name):
        '''Returns the normalized score of an MVA ID (range: [-1,1])'''
        if name.startswith("mvaEleID-"):
            return self.mva_score(name, norm=True)
        elif name == "NonTrigSpring15MiniAOD":
            return self.physObj.userFloat("ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Values")
        else:
            ShortNames = { "NonTrigSpring15" : "mvaEleID-Spring15-25ns-nonTrig-V1",
                           "Fall17noIso" : "mvaEleID-Fall17-noIso-V1",
                           "Fall17Iso" : "mvaEleID-Fall17-iso-V1",
                           "Fall17V1noIso" : "mvaEleID-Fall17-noIso-V1",
                           "Fall17V1Iso" : "mvaEleID-Fall17-iso-V1",
                           "Fall17V2noIso" : "mvaEleID-Fall17-noIso-V2",
                           "Fall17V2Iso" : "mvaEleID-Fall17-iso-V2",
                           "Spring16GP" : "mvaEleID-Spring16-GeneralPurpose-V1",
                           "Spring16HZZ" : "mvaEleID-Spring16-HZZ-V1" }
            if name in ShortNames:
                return self.mva_score(ShortNames[name], norm=True)
            else:
                raise RuntimeError("Can't do %s at the moment" % name)

    def mvaIDRun2Custom(self, name, wp):
            '''Checks whether an electron passes a custom working point of an MVA ID not provided by VID'''
            eta = abs(self.superCluster().eta())
            if name in ("NonTrigSpring15","NonTrigSpring15MiniAOD"):
                if wp=="VLoose":
                    if self.pt() <= 10:
                        if   (eta < 0.8)  : return self.mvaRun2(name) > -0.11;
                        elif (eta < 1.479): return self.mvaRun2(name) > -0.55;
                        else              : return self.mvaRun2(name) > -0.60;
                    else:
                        if   (eta < 0.8)  : return self.mvaRun2(name) > -0.16;
                        elif (eta < 1.479): return self.mvaRun2(name) > -0.65;
                        else              : return self.mvaRun2(name) > -0.74;                        
                elif wp=="VLooseIdEmu":
                    if   (eta < 0.8)  : return self.mvaRun2(name) > -0.70;
                    elif (eta < 1.479): return self.mvaRun2(name) > -0.83;
                    else              : return self.mvaRun2(name) > -0.92;
                elif wp=="VLooseIdIsoEmu":
                    if   (eta < 0.8)  : return self.mvaRun2(name) > -0.155;
                    elif (eta < 1.479): return self.mvaRun2(name) > -0.56;
                    else              : return self.mvaRun2(name) > -0.76;
                elif wp=="Tight":
                    if   (eta < 0.8)  : return self.mvaRun2(name) > 0.87;
                    elif (eta < 1.479): return self.mvaRun2(name) > 0.60;
                    else              : return self.mvaRun2(name) > 0.17;
                elif wp == "HZZ":
                    return self.id_passes("mvaEleID-Spring15-25ns-nonTrig-V1","wpLoose")
                elif wp == "POG80":
                    return self.id_passes("mvaEleID-Spring15-25ns-nonTrig-V1","wp80")
                elif wp == "POG90":
                    return self.id_passes("mvaEleID-Spring15-25ns-nonTrig-V1","wp90")
                else: raise RuntimeError("Ele MVA ID Working point '%s' of '%s' not found" % (wp, name))
            elif name == "Spring16":
                smooth_cut = False
                if wp == "HZZ":
                    return self.id_passes("mvaEleID-Spring16-HZZ-V1","wpLoose")
                elif wp == "POG80":
                    return self.id_passes("mvaEleID-Spring16-GeneralPurpose-V1","wp80")
                elif wp == "POG90":
                    return self.id_passes("mvaEleID-Spring16-GeneralPurpose-V1","wp90")
                elif wp=="VLoose":
                    smooth_cut = True
                    _vlow = [0.46,-0.03,0.06]
                    _low = [-0.48,-0.67,-0.49]
                    _high = [-0.85,-0.91,-0.83]
                elif wp=="VLooseIdEmu":
                    smooth_cut = True
                    _vlow = [-0.30,-0.46,-0.63]
                    _low = [-0.86,-0.85,-0.81]
                    _high = [-0.96,-0.96,-0.95]
                elif wp=="Tight":
                    smooth_cut = True
                    _low = [0.77,0.56,0.48]
                    _vlow = _low
                    _high = [0.52,0.11,-0.01]
                if not smooth_cut: raise RuntimeError("Ele MVA ID Working point '%s' not found for '%s'" % (wp, name))
                val = self.mvaRun2(name+'GP') if self.pt()>10 else self.mvaRun2(name+'HZZ')
                if self.pt()<=10:
                    return (val > _vlow[(eta>=0.8)+(eta>=1.479)])
                else: # _low below 15 GeV, _high above 25 GeV, interpolation in between
                    a = _low[(eta>=0.8)+(eta>=1.479)]
                    b = _high[(eta>=0.8)+(eta>=1.479)]
                    c = (a-b)/10
                    cut = min(a,max(b,a-c*(self.pt()-15))) # warning: the _high WP must be looser than the _low one
                    return (val>cut)
            elif name == "Fall17noIso":
                if wp == 'Loose':
                    return self.id_passes("mvaEleID-Fall17-noIso-V1","wpLoose")
                elif wp == 'wp90':
                    return self.id_passes("mvaEleID-Fall17-noIso-V1","wp90")
                elif wp == 'wp80':
                    return self.id_passes("mvaEleID-Fall17-noIso-V1","wp80")
                elif wp == 'SUSYVLooseFO':
                    if self.pt()<5:
                        raise RuntimeError('MVA_ID_nonIso_Fall17_SUSYVLooseFO electron ID cannot be called for objects with pt below 5 GeV')
                    elif self.pt()<10:
                        if eta<0.8: _thiscut = -0.135
                        elif eta<1.479: _thiscut = -0.417
                        else: _thiscut = -0.470
                    elif self.pt()<25:
                        if eta<0.8: _thiscut = (-0.93 + (0.043/15.)*(self.pt()-10.))
                        elif eta<1.479: _thiscut = (-0.93 + (0.04/15.)*(self.pt()-10.))
                        else: _thiscut = (-0.942 + (0.032/15.)*(self.pt()-10.))
                    else:
                        if eta<0.8: _thiscut = -0.887
                        elif eta<1.479: _thiscut = -0.89
                        else: _thiscut = -0.91
                    return self.mvaRun2(name) > _thiscut
                elif wp == 'SUSYVLoose':
                    if self.pt()<5:
                        raise RuntimeError('MVA_ID_nonIso_Fall17_SUSYVLoose electron ID cannot be called for objects with pt below 5 GeV')
                    elif self.pt()<10:
                        if eta<0.8: _thiscut = 0.488
                        elif eta<1.479: _thiscut = -0.045
                        else: _thiscut = 0.176
                    elif self.pt()<25:
                        if eta<0.8: _thiscut = (-0.788 + (0.148/15.)*(self.pt()-10.))
                        elif eta<1.479: _thiscut = (-0.85 + (0.075/15.)*(self.pt()-10.))
                        else: _thiscut = (-0.81 + (0.077/15.)*(self.pt()-10.))
                    else:
                        if eta<0.8: _thiscut = -0.64
                        elif eta<1.479: _thiscut = -0.775
                        else: _thiscut = -0.733
                    return self.mvaRun2(name) > _thiscut
                elif wp == 'SUSYTight':
                    if self.pt()<10:
                        raise RuntimeError('MVA_ID_nonIso_Fall17_SUSYTight electron ID cannot be called for objects with pt below 10 GeV')
                    elif self.pt()<25:
                        if eta<0.8: _thiscut = 0.2+0.032*(self.pt()-10)
                        elif eta<1.479: _thiscut = 0.1+0.025*(self.pt()-10)
                        else: _thiscut = -0.1+0.028*(self.pt()-10)
                    else:
                        if eta<0.8: _thiscut = 0.68
                        elif eta<1.479: _thiscut = 0.475
                        else: _thiscut = 0.32
                    return self.mvaRun2(name) > _thiscut
                else: raise RuntimeError("Unsupported WP '%s' for '%s'" % (wp, name))
            elif name == "Fall17Iso":
                if wp == 'Loose':
                    return self.id_passes("mvaEleID-Fall17-iso-V1","wpLoose")
                elif wp == 'wp90':
                    return self.id_passes("mvaEleID-Fall17-iso-V1","wp90")
                elif wp == 'wp80':
                    return self.id_passes("mvaEleID-Fall17-iso-V1","wp80")
                else: raise RuntimeError("Unsupported WP '%s' for '%s'" % (wp, name))

            else: raise RuntimeError("Ele MVA ID '%s' not found" % name)

    def mva_category(self, name):
        '''takes an mva name and returns the category of the electron,
        only available using FWLite and Egamma code.'''
        if 'cutBased' in name :
            return None
        if name not in self._mvaid_category :
            self._fill_mva_score_and_category(name)
        return self._mvaid_category[name]

    def mvaId(self, name):
        '''For a transparent treatment of electrons, muons and taus.'''
        return self.mva_score(name)

    def mva_score(self, name, norm=False):
        '''returns the score of the given mva,
        only available using FWLite and Egamma code.'''
        if 'cutBased' in name :
            return None
        if norm :
            if name not in self._mvaid_normscore :
                self._fill_mva_score_and_category(name)
            return self._mvaid_normscore[name]
        else:
            if name not in self._mvaid_score :
                self._fill_mva_score_and_category(name)
            return self._mvaid_score[name]

    def id_passes(self, name, wp):
        '''returns True if the electron passes the given working point of the identification'''
        id = name + '-' + wp
        if id not in self._mvaid_passed :
            miniAODids = [miniAODid[0] for miniAODid in self.electronIDs()]
            if id in miniAODids :
                passed = self.electronIDs()[miniAODids.index(id)][1]
            elif id in wps_dict.keys() :
                FWLitename, FWLitewp = wps_dict[id]
                if 'cutBased' in name :
                    passed = electron_cut_based_IDs[FWLitename].passed(
                        self.physObj,
                        self.rho,
                        FWLitewp
                        )
                else :
                    if name not in self._mvaid_score or name not in self._mvaid_category:
                        self._fill_mva_score_and_category(name)
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

    def countWP(self,name,WPs=None,skipMissing=False):
        '''Returns the number of Working Points 
        that are passed for given ID name.
        
        For example if an electron only passes Loose, and 80,
        Working Points, this will return 2.
        Uses self.electronID() to evaluate the WPs.
        If WPs are not specified:
            for mva IDs it will test ['wp90', 'wp80', 'wpLoose']
            for cutbased IDs it will test [ "veto","loose","medium","tight" ]
            if any WP is not available it is skipped.
        '''
        if WPs is None:
            skipMissing = True
            if name.startswith("mvaEleID-"):
                WPs = ['wp90', 'wp80', 'wpLoose']
            elif name.startswith("cutBasedElectronID-"):
                WPs = ["veto","loose","medium","tight" ]
            else:
                raise RuntimeError("Can't figure out WPs for %s" % name)
        n_WP = 0
        for WP in WPs:
            try:
                if self.electronID(name, WP):
                    n_WP += 1
            except RuntimeError:
                if not skipMissing: raise
                continue #WP not found so it is skipped
        return n_WP

    def _fill_mva_score_and_category(self, name):
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

