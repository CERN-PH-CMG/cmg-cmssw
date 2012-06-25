import copy

from CMGTools.H2TauTau.proto.plotter.categories_common import *
from CMGTools.H2TauTau.proto.plotter.jetcategories import *
from CMGTools.H2TauTau.proto.plotter.cut import *

inc_sig_tau_Te = Cut('l1_looseMvaIso>0.5 && (l1_EOverp>0.2 || l1_decayMode!=0) && l1_byLooseIsoMVA > 0.5 && l1_againstElectronMVA > 0.5 && l1_againstElectronMedium > 0.5 && l1_againstMuonLoose > 0.5 && l1_dxy<0.045 && l1_dz<0.2')
#PG is the first one required for the eleTau channel as well?
inc_sig_ele_Te = Cut('l2_relIso05<0.1 && l2_tightId>0.5 && l2_dxy<0.045 && l2_dz<0.2 && l2_pt>20')
inc_sig_Te = inc_sig_ele_Te & inc_sig_tau_Te


def cutstr_signal_Te():
    return inc_sig_Te

def cutstr_rlxeleiso_Te(cutstr, eleIsoCut):
    '''WARNING: assumes ele iso cut is 0.1'''
    return cutstr.replace( 'l2_relIso05<0.1',
                           'l2_relIso05<{cut}'.format(cut=muIsoCut) )

def cutstr_rlxtauiso_Te(cutstr, tauIsoCut):
    return cutstr.replace('l1_looseMvaIso>0.5',
                          'l1_rawMvaIso>{cut}'.format(cut=tauIsoCut) )

def cutstr_rlxtaueleiso(cutstr, tauIsoCut, eleIsoCut):
    '''WARNING: assumes mu iso cut is 0.1'''
    cutstr = cutstr_rlxeleiso_Te(cutstr, eleIsoCut)
    return cutstr_rlxtauiso_Te(cutstr, tauIsoCut)


cat_Inc_Te_RlxEleIso = str(inc_sig_Te).replace('l2_relIso05<0.1','l2_relIso05<1.0')
cat_Inc_Te_RlxTauIso = str(inc_sig_Te).replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>0.')
cat_Inc_Te_RlxEleTauIso = str(inc_sig_Te).replace('l2_relIso05<0.1','l2_relIso05<0.5').replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>-0.5')
cat_Inc_Te_AntiEleTauIso = str(inc_sig_Te).replace('l2_relIso05<0.1','l2_relIso05>0.1').replace('l1_looseMvaIso>0.5', 'l1_looseMvaIso<0.5')
cat_Inc_Te_AntiEleTauIso_B = str(inc_sig_Te).replace('l2_relIso05<0.1','l2_relIso05>0.2').replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>-0.75')

## cat_Inc_AntiMuTauIsoJosh = str(inc_sig_Te).replace('l2_relIso05<0.1','l2_relIso05>0.2 && l2_relIso05<0.5').replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>0.7')
cat_Inc_Te_AntiEleIso = str(inc_sig_Te).replace('l2_relIso05<0.1','l2_relIso05>0.1')
cat_Inc_Te_AntiTauIso = str(inc_sig_Te).replace('l1_looseMvaIso>0.5', 'l1_looseMvaIso<0.5')
cat_Inc_Te_RlxEleTauIso_b = str(inc_sig_Te).replace('l2_relIso05<0.1','l2_relIso05<0.5').replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>0.5')
cat_Inc_Te = str(inc_sig_Te)

categories_Te = {
#    'Xcat_Inc_AntiEleTauIsoJoshX':cat_Inc_AntiEleTauIsoJosh,
    'Xcat_Inc_RlxEleIsoX':cat_Inc_Te_RlxEleIso,
    'Xcat_Inc_TeX':cat_Inc_Te,
    'Xcat_VBFX':cat_VBF,
    'Xcat_VHX':cat_VH,
    'Xcat_J1X':cat_J1,
    'Xcat_J1BX':cat_J1B,
    'Xcat_J0X':cat_J0
    }
