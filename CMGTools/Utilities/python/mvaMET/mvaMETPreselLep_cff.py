from CMGTools.Common.eventCleaning.goodPVFilter_cfi import goodPVFilter 
from CMGTools.Utilities.mvaMET.mvaMETPreselLep_cfi import *

from CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi import cmgBaseMETFromPFMET as cmgMvaMETPreselLep
cmgMvaMETPreselLep.cfg.inputCollection = 'mvaMETPreselLep'

mvaMETPreselLepSequence = cms.Sequence(
      goodPVFilter+
      cmgMuonMVAMETPresel +
      cmgElectronMVAMETPresel +
      cmgTauMVAMETPresel + 
      mvaMETPreselLep +
      cmgMvaMETPreselLep
      )
