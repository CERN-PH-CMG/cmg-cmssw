import ROOT
import ctypes
import os.path
from numpy import exp
import sys
import RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_tools

# Python wrappers around the Electron MVAs.
# Usage example in RecoEgamma/ElectronIdentification/test

def loadEffectiveAreasFile(filename):
    EAs = None
    for path in [ "$CMSSW_BASE/src", "$CMSSW_RELEASE_BASE/src", "$CMSSW_RELEASE_BASE/external/$SCRAM_ARCH/data" ]:
        fpath = os.path.expandvars(path)+"/"+filename
        if os.path.exists(fpath):
            EAs = []
            for line in open(fpath, 'r'):
                line = line.strip()
                if line.startswith("#"): continue
                try:
                    (etamin, etamax, ea) = map(float, line.split())
                    if len(EAs) > 0 and etamin != EAs[-1][0]: print "WARNING: in EA file %s: gap between eta ranges." % filename
                    EAs.append((etamax,ea))
                except:
                    print "WARNING: skipping malformed line %s in EA file %s" % (filename, line)
            if len(EAs) == 0: raise RuntimeError("ERROR: found no valid EA lines in file %s" % filename)
            break
    if not EAs: RuntimeError("ERROR: can't locate EA file "+filename)
    return EAs

class ElectronCutBasedID(object):
    """ Electron cut based ID wrapper class. Allows testing cut based ID working points
    with python.
    """
    def __init__(self, name, tag, working_points):
        self.name = name 
        self.tag = tag
        self.working_points = working_points
        self._isoInputs = {}
    def effectiveArea(self, ele, eaFile):
        if eaFile not in self._isoInputs:
            self._isoInputs[eaFile] = loadEffectiveAreasFile(eaFile)
        table = self._isoInputs[eaFile]
        eta = abs(ele.superCluster().eta())
        for etamax, ea in table:
            if eta < etamax:
                return ea
        return table[-1][1]
    def passed(self, ele, rho, wp):
        '''return true if the electron passes the cut based ID working point.
        see https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Offline_selection_criteria_for_V
        
        ele: a reco::GsfElectron
        rho: energy density in the event
        wp: working point to test

        example: 
        
            event.getByLabel(('slimmedElectrons'),                 ele_handle)
            event.getByLabel(('fixedGridRhoFastjetAll'),           rho_handle)
            
            electrons = ele_handle.product()
            rho       = rho_handle.product()

            is_veto = passed(electron[0], rho,'cutBasedElectronID-Fall17-94X-V2-veto')
        '''
        if ele.isEB():
            WP = self.working_points[wp][0]
        else:
            WP = self.working_points[wp][1]

        if isinstance(WP, RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_tools.EleWorkingPoint_V4):
            WPtype = 4
        elif isinstance(WP, RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_tools.EleWorkingPoint_V5):
            WPtype = 5
        else:
            raise RuntimeError("WP type of %s for %s-%s is not supported." % (WP, self.tag, wp))
        isoInputs = self.working_points[wp][2]

        full5x5_sigmaIetaIeta = ele.full5x5_sigmaIetaIeta()

        dEtaInSeed = sys.float_info.max
        if ele.superCluster().isNonnull() and ele.superCluster().seed().isNonnull():
            dEtaInSeed = ele.deltaEtaSuperClusterTrackAtVtx() - ele.superCluster().eta() + ele.superCluster().seed().eta()

        dPhiIn = ele.deltaPhiSuperClusterTrackAtVtx()

        h_over_e = ele.hadronicOverEm()
        h_over_e_cut = WP.hOverECut_C0 + WP.hOverECut_CE / ele.superCluster().energy() + WP.hOverECut_Cr * rho / ele.superCluster().energy()

        pfIso = ele.pfIsolationVariables()
        chad = pfIso.sumChargedHadronPt
        nhad = pfIso.sumNeutralHadronEt
        pho  = pfIso.sumPhotonEt
        eA  = self.effectiveArea(ele, isoInputs.isoEffAreas)
        iso  = chad + max([0.0, nhad + pho - rho*eA])
        relIsoWithEA = iso/ele.pt()
        if WPtype == 5:
            relIsoWithEA_cut = WP.relCombIsolationWithEACut_C0+WP.relCombIsolationWithEACut_Cpt/ele.pt()
        elif WPtype == 4:
            if ele.pt() < 20.: # this 20 GeV value is not in the parameter set
                relIsoWithEA_cut = WP.relCombIsolationWithEAHighPtCut
            else:
                relIsoWithEA_cut = WP.relCombIsolationWithEALowPtCut

        ecal_energy_inverse = 1.0/ele.ecalEnergy()
        eSCoverP = ele.eSuperClusterOverP()
        absEInverseMinusPInverse = abs(1.0 - eSCoverP)*ecal_energy_inverse
            
        missingHits = ele.gsfTrack().hitPattern().numberOfLostHits(ROOT.reco.HitPattern.MISSING_INNER_HITS)

        if full5x5_sigmaIetaIeta < WP.full5x5_sigmaIEtaIEtaCut and \
                abs(dEtaInSeed) < WP.dEtaInSeedCut and \
                abs(dPhiIn) < WP.dPhiInCut and \
                h_over_e < h_over_e_cut and \
                relIsoWithEA < relIsoWithEA_cut and \
                absEInverseMinusPInverse < WP.absEInverseMinusPInverseCut and \
                missingHits <= WP.missingHitsCut and \
                ele.passConversionVeto() :
                return True
        return False

class ElectronMVAID:
    """ Electron MVA wrapper class.
    """

    def __init__(self, name, tag, categoryCuts, xmls, variablesFile, debug=False):
        self.name = name
        self.tag = tag
        self.categoryCuts = categoryCuts
        self.variablesFile = variablesFile
        self.xmls = ROOT.vector(ROOT.string)()
        for x in xmls: self.xmls.push_back(x)
        self._init = False
        self._debug = debug

    def __call__(self, ele, convs, beam_spot, rho, debug=False):
        '''returns a tuple mva_value, category 
        ele: a reco::GsfElectron
        convs: conversions
        beam_spot: beam spot
        rho: energy density in the event
        debug: enable debugging mode. 

        example: 
        
            event.getByLabel(('slimmedElectrons'),                 ele_handle)
            event.getByLabel(('fixedGridRhoFastjetAll'),           rho_handle)
            event.getByLabel(('reducedEgamma:reducedConversions'), conv_handle)
            event.getByLabel(('offlineBeamSpot'),                  bs_handle)
            
            electrons = ele_handle.product()
            convs     = conv_handle.product()
            beam_spot = bs_handle.product()
            rho       = rho_handle.product()

            mva, category = electron_mva_id(electron[0], convs, beam_spot, rho)
        '''
        if not self._init:
            print('Initializing ' + self.name + self.tag)
            ROOT.gSystem.Load("libRecoEgammaElectronIdentification")
            categoryCutStrings =  ROOT.vector(ROOT.string)()
            for x in self.categoryCuts : 
                categoryCutStrings.push_back(x)
            self.estimator = ROOT.ElectronMVAEstimatorRun2(
                self.tag, self.name, len(self.xmls), 
                self.variablesFile, categoryCutStrings, self.xmls, self._debug)
            self._init = True
        extra_vars = self.estimator.getExtraVars(ele, convs, beam_spot, rho[0])
        category = ctypes.c_int(0)
        mva = self.estimator.mvaValue(ele, extra_vars, category)
        return mva, category.value


class WorkingPoints(object):
    '''Working Points. Keeps track of the cuts associated to a given flavour of the MVA ID 
    for each working point and allows to test the working points'''

    def __init__(self, name, tag, working_points, logistic_transform=False):
        self.name = name 
        self.tag = tag
        self.working_points = self._reformat_cut_definitions(working_points)
        self.logistic_transform = logistic_transform

    def _reformat_cut_definitions(self, working_points):
        new_definitions = dict()
        for wpname, definitions in working_points.iteritems():
            new_definitions[wpname] = dict()
            for name, cut in definitions.cuts.iteritems():
                categ_id = int(name.lstrip('cutCategory'))
                cut = cut.replace('pt','x')
                formula = ROOT.TFormula('_'.join([self.name, wpname, name]), cut)
                new_definitions[wpname][categ_id] = formula
        return new_definitions

    def passed(self, ele, mva, category, wp):
        '''return true if ele passes wp'''
        threshold = self.working_points[wp][category].Eval(ele.pt())
        if self.logistic_transform:
            mva = 2.0/(1.0+exp(-2.0*mva))-1
        return mva > threshold


# Import information needed to construct the e/gamma MVAs

from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_tools \
        import EleMVA_6CategoriesCuts, mvaVariablesFile, EleMVA_3CategoriesCuts
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V1_cff \
        import mvaEleID_Fall17_noIso_V1_producer_config
mvaVariablesFileFall17V1 = mvaEleID_Fall17_noIso_V1_producer_config.variableDefinition.value()

from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V2_cff \
        import mvaWeightFiles as Fall17_iso_V2_weightFiles
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V2_cff \
        import mvaWeightFiles as Fall17_noIso_V2_weightFiles
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V1_cff \
        import mvaFall17WeightFiles_V1 as Fall17_iso_V1_weightFiles
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V1_cff \
        import mvaFall17WeightFiles_V1 as Fall17_noIso_V1_weightFiles
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring16_GeneralPurpose_V1_cff \
        import mvaSpring16WeightFiles_V1 as mvaSpring16GPWeightFiles_V1
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring16_HZZ_V1_cff \
        import mvaSpring16WeightFiles_V1 as mvaSpring16HZZWeightFiles_V1
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring15_25ns_nonTrig_V1_cff \
        import mvaSpring15NonTrigWeightFiles_V1 as mvaSpring15NonTrigWeightFiles_V1

from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring16_GeneralPurpose_V1_cff \
        import workingPoints as mvaSpring16GP_V1_workingPoints
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring16_HZZ_V1_cff \
        import workingPoints as mvaSpring16HZZ_V1_workingPoints
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V2_cff \
        import workingPoints as Fall17_iso_V2_workingPoints
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V2_cff \
        import workingPoints as Fall17_noIso_V2_workingPoints
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V1_cff \
        import workingPoints as Fall17_iso_V1_workingPoints
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V1_cff \
        import workingPoints as Fall17_noIso_V1_workingPoints
from RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring15_25ns_nonTrig_V1_cff \
        import workingPoints as Spring15_25ns_nonTrig_V1_workingPoints

from RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff \
        import workingPoints as cutBasedElectronID_Fall17_94X_V2_wps
from RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V1_cff \
        import workingPoints as cutBasedElectronID_Fall17_94X_V1_wps


# Dictionary with the relecant e/gmma MVAs

electron_mvas = {
    "Fall17IsoV2"   : ElectronMVAID("ElectronMVAEstimatorRun2","Fall17IsoV2",
                                    EleMVA_6CategoriesCuts, Fall17_iso_V2_weightFiles, mvaVariablesFile),
    "Fall17NoIsoV2" : ElectronMVAID("ElectronMVAEstimatorRun2","Fall17NoIsoV2",
                                    EleMVA_6CategoriesCuts, Fall17_noIso_V2_weightFiles, mvaVariablesFile),
    "Fall17IsoV1"   : ElectronMVAID("ElectronMVAEstimatorRun2","Fall17IsoV1",
                                    EleMVA_6CategoriesCuts, Fall17_iso_V1_weightFiles, mvaVariablesFileFall17V1),
    "Fall17NoIsoV1" : ElectronMVAID("ElectronMVAEstimatorRun2","Fall17NoIsoV1",
                                    EleMVA_6CategoriesCuts, Fall17_noIso_V1_weightFiles, mvaVariablesFileFall17V1),
    "Spring16HZZV1" : ElectronMVAID("ElectronMVAEstimatorRun2","Spring16HZZV1",
                                    EleMVA_6CategoriesCuts, mvaSpring16HZZWeightFiles_V1, mvaVariablesFile),
    "Spring16GPV1"    : ElectronMVAID("ElectronMVAEstimatorRun2","Spring16GeneralPurposeV1",
                                    EleMVA_3CategoriesCuts, mvaSpring16GPWeightFiles_V1, mvaVariablesFile),
    "Spring15NonTrig25nsV1"    : ElectronMVAID("ElectronMVAEstimatorRun2","Spring15NonTrig25nsV1",
                                    EleMVA_6CategoriesCuts, mvaSpring15NonTrigWeightFiles_V1, mvaVariablesFile),
    }

working_points = {
    "Fall17IsoV2"   : WorkingPoints("ElectronMVAEstimatorRun2","Fall17IsoV2",
                                    Fall17_iso_V2_workingPoints),
    "Fall17NoIsoV2" : WorkingPoints("ElectronMVAEstimatorRun2","Fall17NoIsoV2",
                                    Fall17_noIso_V2_workingPoints),
    "Fall17IsoV1"   : WorkingPoints("ElectronMVAEstimatorRun2","Fall17IsoV1",
                                    Fall17_iso_V1_workingPoints, logistic_transform=True),
    "Fall17NoIsoV1" : WorkingPoints("ElectronMVAEstimatorRun2","Fall17NoIsoV1",
                                    Fall17_noIso_V1_workingPoints, logistic_transform=True),
    "Spring16HZZV1" : WorkingPoints("ElectronMVAEstimatorRun2","Spring16HZZV1",
                                    mvaSpring16HZZ_V1_workingPoints, logistic_transform=True),
    "Spring16GPV1"    : WorkingPoints("ElectronMVAEstimatorRun2","Spring16GeneralPurposeV1",
                                    mvaSpring16GP_V1_workingPoints, logistic_transform=True),
    "Spring15NonTrig25nsV1" : WorkingPoints("ElectronMVAEstimatorRun2","Spring15NonTrig25nsV1",
                                    Spring15_25ns_nonTrig_V1_workingPoints, logistic_transform=True), 
    }

electron_cut_based_IDs = {
    "Fall1794XV2"   : ElectronCutBasedID("ElectronMVAEstimatorRun2","Fall1794XV2",
                                         cutBasedElectronID_Fall17_94X_V2_wps),
    "Fall1794XV1"   : ElectronCutBasedID("ElectronMVAEstimatorRun2","Fall1794XV1",
                                         cutBasedElectronID_Fall17_94X_V1_wps),
}
