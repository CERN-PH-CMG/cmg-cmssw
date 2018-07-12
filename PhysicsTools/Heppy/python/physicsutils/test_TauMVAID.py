import unittest 
import pprint 

import logging 
# logging.basicConfig(level=logging.INFO)

from TauMVAID import * 

class TestTauMVAID(unittest.TestCase): 

    def test_mva(self): 
        basedir = '$CMSSW_BASE/src/PhysicsTools/Heppy/data/'
        working_points = dict(
            Eff50 = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff50.root',
            Eff90 = 'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2WPEff90.root'
            )
        for wp in working_points:
            working_points[wp] = '/'.join([basedir, working_points[wp]])
        mvaid = TauMVAID(
            basedir+'GBRForest_tauIdMVAIsoDBoldDMwLT2017v2.root', 
            'RecoTauTag_tauIdMVAIsoDBoldDMwLT2017v2', 'oldDMwLT',
            working_points
            )
        mvaid.threshold(50, 'Eff50')
        mvaid.threshold(20, 'Eff50')
        mvaid.threshold(1900, 'Eff50')
        mvaid.threshold(1900, 'Eff90')
        


if __name__ == '__main__':
    unittest.main()
