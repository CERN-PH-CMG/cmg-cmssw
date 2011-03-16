import CMGTools.RootTools.TestTools as TestTools
import CMGTools.RootTools.cmgTuple as cmgTuple
import ROOT as rt

import unittest

class TestMuon(TestTools.CFGTest):
    
    def __init__(self,methodName):
        TestTools.CFGTest.__init__(self,methodName)
        self.cfgsRunOnce.append('CMGTools/Common/test/testMuon_cfg.py')
    
    def testTreeEntries(self):
        """Test that the Events tree has some content"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMuon_cfg.py']
        self.assert_(TestTools.getEntries(output[1]) > 0,'The CMGTuple must contain some entries')
        
    def testDiMuonBranchExists(self):
        """Tests that the DiMuon branch exists"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMuon_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.aliases.has_key('cmgDiMuon'),'We are expecting a branch called cmgDiMuon')
        
    def testMuonIsolation(self):
        """Verify that the isolation variables are set"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMuon_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertEqual(cmg.Draw("abs(cmgMuonSel.absIso() + 3)","abs(cmgMuonSel.absIso() + 3) < 1e-6","goff"),0,\
                         'There must be no isolations unset by pat')
        #9999 is the value of Unset(double)
        self.assertEqual(cmg.Draw("abs(cmgMuonSel.absIso() + 9999*3)","abs(cmgMuonSel.absIso() + 9999*3) < 1e-6","goff"),0,\
                         'There must be no isolations unset by cmgTools')
        
    def testZID(self):
        """Verify that the Z id is applied"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMuon_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.Draw("cmgDiMuon.pt()","cmgDiMuon.getSelection(\"cuts_zmumu\")","goff") > 0,\
                         'The Z id should be applied')

if __name__ == '__main__':
    
    unittest.main()
