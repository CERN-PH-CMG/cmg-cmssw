import copy

import PhysicsTools.HeppyCore.framework.config as cfg

HiggsSUSYGG160 = cfg.MCComponent(
    name='HiggsSUSYGG160',
    files=[],
    xSection=None, 
    nGenEvents=0,
    triggers=[],
    effCorrFactor=1)

mc_higgs_susy_gg = [
    HiggsSUSYGG160
    ]


mc_higgs_susy = copy.copy(mc_higgs_susy_gg)
# mc_higgs.extend(mc_higgs_ggh)
