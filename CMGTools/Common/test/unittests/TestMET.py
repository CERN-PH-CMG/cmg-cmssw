import CMGTools.RootTools.TestTools as TestTools
import CMGTools.RootTools.cmgTuple as cmgTuple
import ROOT as rt

import unittest

class TestMET(TestTools.CFGTest):
    
    def __init__(self,methodName):
        TestTools.CFGTest.__init__(self,methodName)
        self.cfgsRunOnce.append('CMGTools/Common/test/testMET_cfg.py')
    
    def testTreeEntries(self):
        """Test that the Events tree has some content"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMET_cfg.py']
        self.assert_(TestTools.getEntries(output[1]) > 0,'The CMGTuple must contain some entries')
        
#    def testPFMETBranchExists(self):
#        """Tests that the cmgMETPFCandidates branch exists"""
        
#        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMET_cfg.py']
#        events = TestTools.getObject(output[1], 'Events')
        
#        cmg = cmgTuple.cmgTuple(events)
#        self.assertTrue(cmg.aliases.has_key('cmgMETPFCandidates'),'We are expecting a branch called cmgMETPFCandidates')
        
    def testPATMETBranchExists(self):
        """Tests that the patMETsPFlow branch exists"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMET_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.aliases.has_key('patMETsAK5'),'We are expecting a branch called patMETsAK5')
        
    def testCMGPFMETBranchExists(self):
        """Tests that the cmgPFMET branch exists"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMET_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.aliases.has_key('cmgPFMET'),'We are expecting a branch called cmgPFMET')
        
    def testMETET(self):
        """Verify that the cmg::MET has the same et as the pat::MET"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMET_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertEqual(cmg.Draw("abs(patMETsAK5.et() - cmgPFMET.et())",
                                  "abs(patMETsAK5.et() - cmgPFMET.et()) > 1e-3","goff"),0,\
                                  'The cmg::MET and the pat::MET should be the same')
        

if __name__ == '__main__':
    
    unittest.main()
