######################################################################
## File: TestMultiJet.py
######################################################################

import CMGTools.RootTools.TestTools as TestTools
import CMGTools.RootTools.cmgTuple as cmgTuple

import unittest

class TestMultiJet(TestTools.CFGTest):

    cfg_file_name = "CMGTools/Common/test/testMultiJet_cfg.py"

    def __init__(self, methodName):
        TestTools.CFGTest.__init__(self, methodName)
        self.cfgsRunOnce.append(TestMultiJet.cfg_file_name)

    def testTreeEntries(self):
        """Test that the Events tree is not empty."""

        output = self.__class__.cfgsRunOnceCache[TestMultiJet.cfg_file_name]
        self.assert_(TestTools.getEntries(output[1]) > 0,
                     "The CMGTuple must contain at least some entries")

    def testMultiJetBranchExists(self):
        """Tests that the multi-object branch exists."""

        output = self.__class__.cfgsRunOnceCache[TestMultiJet.cfg_file_name]
        events = TestTools.getObject(output[1], "Events")

        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.aliases.has_key("cmgMultiJet"),
                        "We are expecting a branch called cmgMultiJet")

######################################################################

if __name__ == "__main__":

    unittest.main()

######################################################################
