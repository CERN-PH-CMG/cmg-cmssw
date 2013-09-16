import CMGTools.RootTools.TestTools as TestTools

import os
import sys
import unittest

class CMGTestLoader(unittest.TestLoader):

    def suiteClass(self,foo):

        testDir = os.path.expandvars(os.path.join('$CMSSW_BASE','src','CMGTools','Common','test','unittests'))
        if testDir not in sys.path:
            sys.path.append(testDir)

        # Just list whatever tests you want to run here.
        tests = [
#             "TestBuild",
#            "TestPhoton",
            "TestElectron",
            "TestMET",
            "TestMuon",
            "TestJet",
            "TestWENu",
            "TestWMuNu",
            "TestHemi",
            "TestMultiJet",
            "TestTrigger"
            ]

        sep_line = "-" * 50
        print sep_line
        print "Going to run %d tests: %s" % (len(tests), ", ".join(tests))
        print sep_line

        modules = map(__import__, tests)

        suites = []
        for test in tests:
            suites.append(unittest.defaultTestLoader.loadTestsFromName(test))

        tests = unittest.TestSuite()
        tests.addTests(suites)

        return tests

if __name__ == '__main__':

    unittest.main(testLoader=CMGTestLoader())
