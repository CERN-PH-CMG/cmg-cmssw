from CMGTools.RootTools.fwlite.Config import CFG
from CMGTools.HToZZTo4Leptons.tools.EfficiencyCorrector import EfficiencyCorrector


eff2011 = CFG(
    name='eff',
    muonFile = 'data/eff_mu11.root',
    muonHisto = 'eff2011',
#    muonHisto = 'TH2D_ALL_2012',

    eleFile = 'data/eff_ele.root',    
    eleHisto = 'heff'
    )


eff2012 = CFG(
    name='eff',
    muonFile = 'data/eff_mu12.root',
    muonHisto = 'TH2D_ALL_2012',
    eleFile = 'data/eff_ele12.root',    
    eleHisto = 'h_electron_scale_factor_RECO_ID_ISO_SIP'
    )


