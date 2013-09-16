import os
from CMGTools.Common.Tools.cmsswRelease import *

weights_gbrmet = None
weights_gbrmetphi = None
weights_gbrmetu1cov = None
weights_gbrmetu2cov = None

weightdir = '/'.join([os.environ['CMSSW_BASE'],
                      'src/CMGTools/Utilities/data/mvaMET'])


#COLIN: add gbrmet_53 files (check package)
if isNewerThan('CMSSW_5_3_3'):
    weights_gbrmet = '/'.join([weightdir, 'gbrmet_53_Dec2012.root'])
    weights_gbrmetphi = '/'.join([weightdir, 'gbrmetphi_53_Dec2012.root'])
    weights_gbrmetu1cov = '/'.join([weightdir, 'gbru1cov_53_Dec2012.root'])
    weights_gbrmetu2cov = '/'.join([weightdir, 'gbru2cov_53_Dec2012.root'])
elif isNewerThan('CMSSW_5_2_0'):
    weights_gbrmet = '/'.join([weightdir, 'gbrmet_52.root'])
    weights_gbrmetphi = '/'.join([weightdir, 'gbrmetphi_52.root'])
    weights_gbrmetu1cov = '/'.join([weightdir, 'gbrmetu1cov_52.root'])
    weights_gbrmetu2cov = '/'.join([weightdir, 'gbrmetu2cov_52.root'])
elif cmsswIs44X():
    weights_gbrmet = '/'.join([weightdir, 'gbrmet_42.root'])
    weights_gbrmetphi = '/'.join([weightdir, 'gbrmetphi_42.root'])
    weights_gbrmetu1cov = '/'.join([weightdir, 'gbru1cov_42.root'])
    weights_gbrmetu2cov = '/'.join([weightdir, 'gbru2cov_42.root'])
    
