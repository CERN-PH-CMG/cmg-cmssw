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
        self.assertTrue(cmg.aliases.has_key('cmgDiMuonSel'),'We are expecting a branch called cmgDiMuonSel')
        
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
        self.assertTrue(cmg.Draw("cmgDiMuonSel.pt()","cmgDiMuonSel.getSelection(\"cuts_zmumu\")","goff") > 0,\
                         'The Z id should be applied')
        
    def testHasZID(self):
        """Verify that the Z id is applied"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMuon_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertEqual(cmg.Draw("cmgDiMuonSel.pt()","!cmgDiMuonSel.hasSelection(\"cuts_zmumu\")","goff"),0,\
                         'The Z id should be applied')
        
    def testHasSelection(self):
        """Verify that hasSelection returns false if there is nothing there"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMuon_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertEqual(cmg.Draw("cmgDiMuonSel.pt()","cmgDiMuonSel.hasSelection(\"foo_bar_dummy\")","goff"),0,\
                         'foo_bar_dummy must not be found')
        
    def testLegOrdering(self):
        """Verify that the leg ordering is applied"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMuon_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertEqual(cmg.Draw("cmgDiMuonSel.mass()","cmgDiMuonSel.leg1().pt() < cmgDiMuonSel.leg2().pt()","goff"),0,\
                         'The legs should be pt ordered')
        
    def testRazorMR(self):
        """Verify that MR is set"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMuon_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.Draw("cmgDiMuonSel.mR()",
                                 "(abs(cmgDiMuonSel.mR() + 9999) > 1e-3) ","goff") > 0,\
                         'MR should be filled for relevant events')

    def testRazorMRT(self):
        """Verify that MRT is set"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testMuon_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.Draw("cmgDiMuonSel.mRT()",
                                 "(abs(cmgDiMuonSel.mRT() + 9999) > 1e-3) ","goff") > 0,\
                         'MRT should be filled for relevant events')



if __name__ == '__main__':
    
    unittest.main()
