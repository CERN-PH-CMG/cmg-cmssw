######################################################################
## File: TestPhoton.py
######################################################################

import CMGTools.RootTools.TestTools as TestTools
import CMGTools.RootTools.cmgTuple as cmgTuple

import unittest

class TestPhoton(TestTools.CFGTest):

    cfg_file_name = "CMGTools/Common/test/testPhoton_cfg.py"

    def __init__(self, methodName):
        TestTools.CFGTest.__init__(self, methodName)
        self.cfgsRunOnce.append(TestPhoton.cfg_file_name)

    def testTreeEntries(self):
        """Test that the Events tree is not empty."""

        output = self.__class__.cfgsRunOnceCache[TestPhoton.cfg_file_name]
        self.assert_(TestTools.getEntries(output[1]) > 0,
                     "The CMGTuple must contain at least some entries")

    def testPhotonBranchExists(self):
        """Tests that the photon branch exists."""

        output = self.__class__.cfgsRunOnceCache[TestPhoton.cfg_file_name]
        events = TestTools.getObject(output[1], "Events")

        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.aliases.has_key("cmgPhotonSel"),
                        "We are expecting a branch called cmgPhotonSel")

#     def testDiPhotonBranchExists(self):
#         """Tests that the di-photon branch exists."""

#         output = self.__class__.cfgsRunOnceCache[TestPhoton.cfg_file_name]
#         events = TestTools.getObject(output[1], "Events")

#         cmg = cmgTuple.cmgTuple(events)
#         self.assertTrue(cmg.aliases.has_key("cmgDiPhoton"),
#                         "We are expecting a branch called cmgDiPhoton")

######################################################################

if __name__ == "__main__":

    unittest.main()

######################################################################
