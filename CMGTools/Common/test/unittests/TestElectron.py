import CMGTools.RootTools.TestTools as TestTools
import CMGTools.RootTools.cmgTuple as cmgTuple
import ROOT as rt

import unittest

class TestMuon(TestTools.CFGTest):
    
    def __init__(self,methodName):
        TestTools.CFGTest.__init__(self,methodName)
        self.cfgsRunOnce.append('CMGTools/Common/test/testElectron_cfg.py')
    
    def testTreeEntries(self):
        """Test that the Events tree has some content"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testElectron_cfg.py']
        self.assert_(TestTools.getEntries(output[1]) > 0,'The CMGTuple must contain some entries')
        
    def testDiElectronBranchExists(self):
        """Tests that the DiElectron branch exists"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testElectron_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.aliases.has_key('cmgDiElectronSel'),'We are expecting a branch called cmgDiElectronSel')
        
    def testElectronIsolation(self):
        """Verify that the isolation variables are set"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testElectron_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertEqual(cmg.Draw("abs(cmgElectronSel.absIso() + 3)","abs(cmgElectronSel.absIso() + 3) < 1e-6","goff"),0,\
                         'There must be no isolations unset by pat')
        #9999 is the value of Unset(double)
        self.assertEqual(cmg.Draw("abs(cmgElectronSel.absIso() + 9999*3)","abs(cmgElectronSel.absIso() + 9999*3) < 1e-6","goff"),0,\
                         'There must be no isolations unset by cmgTools')
        

if __name__ == '__main__':
    
    unittest.main()
