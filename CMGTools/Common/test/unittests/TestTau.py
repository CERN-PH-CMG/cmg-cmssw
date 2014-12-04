import CMGTools.RootTools.TestTools as TestTools
import CMGTools.RootTools.cmgTuple as cmgTuple
import ROOT as rt

import unittest

class TestTau(TestTools.CFGTest):
    
    def __init__(self,methodName):
        TestTools.CFGTest.__init__(self,methodName)
        self.cfgsRunOnce.append('CMGTools/Common/test/testTau_cfg.py')
    
    def testTreeEntries(self):
        """Test that the Events tree has some content"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTau_cfg.py']
        self.assert_(TestTools.getEntries(output[1]) > 0,'The CMGTuple must contain some entries')
        
        
    def testTauPt(self):
        """Very the default pt selection"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTau_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertEqual(cmg.Draw("cmgTauSel.pt()","cmgTauSel.pt() < 15.0","goff"),0,\
                         'All taus should have a 15 GeV default cut')

    def testTauID(self):
        """Verify the tauID is applied"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTau_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertNotEqual(cmg.Draw("cmgTauSel.tauID(\"decayModeFinding\")","cmgTauSel.tauID(\"decayModeFinding\")==1","goff"),0,\
                         'HPS taudID should be filled')


if __name__ == '__main__':
    
    unittest.main()
