from CMGTools.RootTools.fwlite.Config import CFG
from CMGTools.HToZZTo4Leptons.tools.EfficiencyCorrector import EfficiencyCorrector


eff2011 = CFG(
    name='eff',
    muonFile = 'data/eff_mu.root',
    muonHisto = 'eff2011',
    eleFile = 'data/eff_ele.root',    
    eleHisto = 'heff'
    )


eff2012 = CFG(
    name='eff',
    muonFile = 'data/eff_mu.root',
    muonHisto = 'TH2D_ALL_2012',
    eleFile = 'data/eff_ele.root',    
    eleHisto = 'heff'
    )


