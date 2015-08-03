from CMGTools.H2TauTau.proto.plotter.categories_common import categories_common
from CMGTools.H2TauTau.proto.plotter.cut import Cut

pt2 = 18
pt1 = 20


# NEW one - to be implemented as soon as trees are there
inc_sig_tau = Cut(
    '!veto_dilepton && !veto_thirdlepton && !veto_otherlepton && l2_byCombinedIsolationDeltaBetaCorrRaw3Hits<1.5 && l2_againstMuon3>1.5 && l2_againstElectronMVA5>0.5 && l2_pt>{pt2}'.format(pt2=pt2))

inc_sig_mu = Cut('l1_reliso05<0.1 && l1_muonid_medium>0.5 && l1_pt>{pt1}'.format(pt1=pt1))

inc_sig = inc_sig_mu & inc_sig_tau

cat_Inc_RlxMuIso = str(inc_sig).replace('l1_reliso05<0.1', 'l1_reliso05<1.0')
cat_Inc_RlxTauIso = str(inc_sig).replace('l2_threeHitIso<1.5', 'l2_threeHitIso<10.0')
cat_Inc_RlxMuTauIso = str(inc_sig).replace('l1_reliso05<0.1', 'l1_reliso05<0.5').replace('l2_threeHitIso<1.5', 'l2_threeHitIso<10.0')
cat_Inc_AntiMuTauIso = str(inc_sig).replace('l1_reliso05<0.1', 'l1_reliso05>0.1').replace('l2_looseMvaIso>0.5', 'l2_looseMvaIso<0.5')

cat_Inc_AntiMuIso = str(inc_sig).replace('l1_reliso05<0.1', 'l1_reliso05>0.1')
cat_Inc_AntiTauIso = str(inc_sig).replace('l2_threeHitIso<1.5', 'l2_threeHitIso>1.5 && l2_threeHitIso<5.0')

cat_Inc = str(inc_sig)

categories = {
    'Xcat_Inc_RlxMuIsoX': cat_Inc_RlxMuIso,
    'Xcat_Inc_RlxTauIsoX': cat_Inc_RlxTauIso,
    'Xcat_IncX': cat_Inc,
    'Xcat_Inc_AntiTauIsoX': cat_Inc_AntiTauIso,
}

categories.update(categories_common)
