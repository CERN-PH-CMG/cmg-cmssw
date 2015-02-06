import CMGTools.RootTools.TestTools as TestTools
import CMGTools.RootTools.cmgTuple as cmgTuple
import ROOT as rt

import unittest

class TestJET(TestTools.CFGTest):
    
    def __init__(self,methodName):
        TestTools.CFGTest.__init__(self,methodName)
        self.cfgsRunOnce.append('CMGTools/Common/test/testJet_cfg.py')
    
    def testTreeEntries(self):
        """Test that the Events tree has some content"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testJet_cfg.py']
        self.assert_(TestTools.getEntries(output[1]) > 0,'The CMGTuple must contain some entries')
        
    def testCMGPFJetBranchExists(self):
        """Tests that the cmgPFJet branch exists"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testJet_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.aliases.has_key('cmgPFJetSel'),'We are expecting a branch called cmgPFJetSel')
        
    def testJetID(self):
        """Verify that the jet id is applied"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testJet_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.Draw("cmgPFJetSel.pt()","cmgPFJetSel.getSelection(\"cuts_looseJetId\")","goff") > 0,\
                         'The jet id should be applied')
        
    def testBTag(self):
        """Verify that the Btag is applied"""
        
        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testJet_cfg.py']
        events = TestTools.getObject(output[1], 'Events')
        
        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.Draw("cmgPFJetSel.btag()","cmgPFJetSel.getSelection(\"cuts_btag_loose\")","goff") > 0,\
                         'The btag should be applied')
 

if __name__ == '__main__':
    
    unittest.main()
