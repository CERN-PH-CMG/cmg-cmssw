import copy

class Cut(object):

    def __init__(self, cutstr):
        self.cutstr = cutstr

    def __and__(self, other):
        newone = copy.deepcopy( self )
        newone.cutstr = '({cut1}) && ({cut2})'.format(cut1=str(self), cut2=str(other))
        return newone
        
    def __or__(self, other):
        newone = copy.deepcopy( self )
        newone.cutstr = '({cut1}) || ({cut2})'.format(cut1=str(self), cut2=str(other))
        return newone

    def __str__(self):
        return self.cutstr
        
# inc_sig_mu = Cut('l2_relIso05<0.1 && l2_tightId>0.5 && l2_dxy<0.045 && l2_dz<0.2')
inc_sig_tau = Cut('l1_looseMvaIso>0.5 && (l1_EOverp>0.2 || l1_decayMode!=0) && l1_againstMuonTight>0.5 && l1_againstElectronLoose>0.5 && l1_dxy<0.045 && l1_dz<0.2')
inc_sig_mu = Cut('l2_relIso05<0.1 && l2_tightId>0.5 && l2_dxy<0.045 && l2_dz<0.2 && l2_pt>20')
# inc_sig_tau = Cut('l1_looseMvaIso>0.5 && (l1_EOverp>0.2 || l1_decayMode!=0) && l1_againstMuonTight>0.5 && l1_againstElectronLoose>0.5 && l1_dxy<0.045 && l1_dz<0.2 && l1_pt>25')
inc_sig = inc_sig_mu & inc_sig_tau


def cutstr_signal():
    return inc_sig

def cutstr_rlxmuiso(cutstr, muIsoCut):
    '''WARNING: assumes mu iso cut is 0.1'''
    return cutstr.replace( 'l2_relIso05<0.1',
                           'l2_relIso05<{cut}'.format(cut=muIsoCut) )

def cutstr_rlxtauiso(cutstr, tauIsoCut):
    '''WARNING: assumes mu iso cut is 0.1'''
    return cutstr.replace('l1_looseMvaIso>0.5',
                          'l1_rawMvaIso>{cut}'.format(cut=tauIsoCut) )

def cutstr_rlxtaumuiso(cutstr, tauIsoCut, muIsoCut):
    '''WARNING: assumes mu iso cut is 0.1'''
    cutstr = cutstr_rlxmuiso(cutstr, muIsoCut)
    return cutstr_rlxtauiso(cutstr, tauIsoCut)


cat_Inc_RlxMuIso = str(inc_sig).replace('l2_relIso05<0.1','l2_relIso05<1.0')
cat_Inc_RlxMuTauIso = str(inc_sig).replace('l2_relIso05<0.1','l2_relIso05<0.5').replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>-0.5')
cat_Inc_AntiMuTauIso = str(inc_sig).replace('l2_relIso05<0.1','l2_relIso05>0.1').replace('l1_looseMvaIso>0.5', 'l1_looseMvaIso<0.5')
cat_Inc_AntiMuIso = str(inc_sig).replace('l2_relIso05<0.1','l2_relIso05>0.1')
cat_Inc_AntiTauIso = str(inc_sig).replace('l1_looseMvaIso>0.5', 'l1_looseMvaIso<0.5')
cat_Inc_RlxMuTauIso_b = str(inc_sig).replace('l2_relIso05<0.1','l2_relIso05<0.5').replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>0.5')
cat_Inc = str(inc_sig)
cat_VBF = 'nJets>=2 && VBF_nCentral==0 && VBF_mva>0.5'
cat_VBF_Rel = 'nJets>=2 && VBF_nCentral==0 && VBF_mva>-0.7'
cat_VH  = 'nJets>=2 && VBF_mjj>60 && VBF_mjj<120 && VBF_dijetpt>150 && VBF_mva<0.8 && nBJets==0'
cat_J1  = 'nJets>=1 && !({VBF}) && !({VH}) && nBJets==0'.format(VBF=cat_VBF,
                                                                VH=cat_VH)
cat_J1B = 'nBJets>=1 && nJets<2'
cat_J0  = '!({VBF}) && !({VH}) && !({J1}) && !({J1B})'.format(VBF=cat_VBF,
                                                              VH=cat_VH,
                                                              J1=cat_J1,
                                                              J1B=cat_J1B)

categories = {
    'cat_Inc_RlxMuIso':cat_Inc_RlxMuIso,
    'cat_Inc':cat_Inc,
    'cat_VBF':cat_VBF,
    'cat_VH':cat_VH,
    'cat_J1':cat_J1,
    'cat_J1B':cat_J1B,
    'cat_J0':cat_J0
    }

def replaceCategories(cutstr):
    for catname, cat in categories.iteritems():
        cutstr = cutstr.replace( catname, cat )
    return cutstr

if __name__ == '__main__':

    import copy

    sig_mu = Cut('l2_relIso05<0.1 && l2_tightId>0.5 && l2_dxy<0.045 && l2_dz<0.2')
    sig_tau = Cut('l1_looseMvaIso>0.5 && (l1_EOverp>0.2 || l1_decayMode!=0) && l1_againstMuonTight>0.5 && l1_againstElectronLoose>0.5 && l1_dxy<0.045 && l1_dz<0.2')
    #  import pdb; pdb.set_trace()
    print sig_mu
    sig = sig_mu & sig_tau
    print sig
