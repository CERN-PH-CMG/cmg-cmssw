import CMGTools.RootTools.TestTools as TestTools

import os
import sys
import unittest

class CMGTestLoader(unittest.TestLoader):

    def suiteClass(self,foo):
    
        testDir = os.path.expandvars(os.path.join('$CMSSW_BASE','src','CMGTools','Common','test','unittests'))
        if testDir not in sys.path:
            sys.path.append(testDir)
    
        import TestElectron
        import TestMET
        import TestMuon
    
        loader = unittest.TestLoader()
        suites = []
        suites.append(loader.loadTestsFromModule(TestElectron))
        suites.append(loader.loadTestsFromModule(TestMET))
        suites.append(loader.loadTestsFromModule(TestMuon))
    
        tests = unittest.TestSuite()
        tests.addTests(suites)

        return tests

if __name__ == '__main__':
    
    unittest.main(testLoader=CMGTestLoader())
