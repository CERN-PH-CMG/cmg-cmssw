######################################################################
## File: TestHemi.py
######################################################################

import CMGTools.RootTools.TestTools as TestTools
import CMGTools.RootTools.cmgTuple as cmgTuple

import unittest

class TestHemi(TestTools.CFGTest):

    def __init__(self, methodName):
        TestTools.CFGTest.__init__(self, methodName)
        self.cfgsRunOnce.append("CMGTools/Common/test/testHemi_cfg.py")

    def testTreeEntries(self):
        """Test that the Events tree is not empty."""

        output = self.__class__.cfgsRunOnceCache["CMGTools/Common/test/testHemi_cfg.py"]
        self.assert_(TestTools.getEntries(output[1]) > 0,
                     "The CMGTuple must contain at least some entries")

    def testHemiBranchExists(self):
        """Tests that the hemisphere branch exists."""

        output = self.__class__.cfgsRunOnceCache["CMGTools/Common/test/testHemi_cfg.py"]
        events = TestTools.getObject(output[1], "Events")

        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.aliases.has_key("cmgHemi"),
                        "We are expecting a branch called cmgHemi")

    def testDiHemiBranchExists(self):
        """Tests that the di-hemisphere branch exists."""

        output = self.__class__.cfgsRunOnceCache["CMGTools/Common/test/testHemi_cfg.py"]
        events = TestTools.getObject(output[1], "Events")

        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.aliases.has_key("cmgDiHemi"),
                        "We are expecting a branch called cmgDiHemi")

######################################################################

if __name__ == "__main__":

    unittest.main()

######################################################################
