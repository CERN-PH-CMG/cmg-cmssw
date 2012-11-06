from CMGTools.Common.Tools.cmsswRelease import isNewerThan

# working points from Matthew, Oct 6
VBF_mva_WP = 0.676
if isNewerThan('CMSSW_5_2_0'):
    VBF_mva_WP = 0.978
  

#cat_VBF = 'nJets>=2 && VBF_nCentral==0 && VBF_mva>{VBF_mva_WP}'.format(VBF_mva_WP=VBF_mva_WP)
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
    }
