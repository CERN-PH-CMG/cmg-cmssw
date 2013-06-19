from CMGTools.Common.Tools.cmsswRelease import isNewerThan

cat_VBF = 'nJets>=2 && VBF_nCentral==0 && VBF_mjj > 500 && abs(VBF_deta) > 3.5'
cat_VBF_Rel_30 = 'nJets>=2 && VBF_nCentral==0 && VBF_mjj>200 && abs(VBF_deta) > 2.'
cat_VBF_Rel_20 = 'nJets20>=2 && VBF_nCentral==0 && VBF_mjj>200 && abs(VBF_deta) > 2.'

cat_J2 = 'nJets>=2'
# cat_VH  = 'nJets>=2 && VBF_mjj>60 && VBF_mjj<120 && VBF_dijetpt>150 && VBF_mva<0.5 && nBJets==0'
cat_J1  = 'nJets>=1 && !({VBF}) && nBJets==0'.format(VBF=cat_VBF)
#cat_J1  = 'nJets>=1 && nBJets==0' #Jose
cat_J1B = 'nBJets>=1 && nJets<2'
cat_J0 = 'nJets==0 && nBJets==0' # Jose
#cat_J0  = '!({VBF}) && !({J1}) && !({J1B}) && nJets==0'.format(VBF=cat_VBF,
#                                                               J1=cat_J1,
#                                                               J1B=cat_J1B)

cat_J0_high = cat_J0 + ' && l1_pt>45.'
cat_J0_medium = cat_J0 + ' && l1_pt>30. && l1_pt<=45.'
cat_J0_low = cat_J0 + ' && l1_pt>20. && l1_pt<=30.'

cat_J1_high_mediumhiggs = cat_J1 + ' && l1_pt>45. && pthiggs>100.'
cat_J1_high_lowhiggs = cat_J1 + ' && l1_pt>45. && pthiggs<100.'
cat_J1_medium = cat_J1 + ' && l1_pt>30. && l1_pt<=45.'

cat_VBF_tight = 'nJets>=2 && nBJets==0 && VBF_nCentral==0 && VBF_mjj>700 && abs(VBF_deta)>4. && pthiggs>100.'
cat_VBF_loose = 'nJets>=2 && nBJets==0 && VBF_nCentral==0 && VBF_mjj>500 && abs(VBF_deta)>3.5 && !({VBF_tight})'.format(VBF_tight=cat_VBF_tight)

def replaceCategories(cutstr, categories):
    for catname, cat in categories.iteritems():
        cutstr = cutstr.replace( catname, cat )
    return cutstr

categories_common = {
    'Xcat_VBFX':cat_VBF,
    'Xcat_VBF_Rel_30X':cat_VBF_Rel_30,
    'Xcat_VBF_Rel_20X':cat_VBF_Rel_20,
    'Xcat_J2X':cat_J2,
    'Xcat_J1X':cat_J1,
    'Xcat_J1BX':cat_J1B,
    'Xcat_J0X':cat_J0,
    # The following are the Summer 13 categories
    'Xcat_J0_highX':cat_J0_high,
    'Xcat_J0_mediumX':cat_J0_medium,
    'Xcat_J0_lowX':cat_J0_low,
    'Xcat_J1_high_mediumhiggsX':cat_J1_high_mediumhiggs,
    'Xcat_J1_high_lowhiggsX':cat_J1_high_lowhiggs,
    'Xcat_J1_mediumX':cat_J1_medium,
    'Xcat_VBF_tightX':cat_VBF_tight,
    'Xcat_VBF_looseX':cat_VBF_loose,
    }
