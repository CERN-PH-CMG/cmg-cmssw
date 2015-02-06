import CMGTools.RootTools.TestTools as TestTools
import CMGTools.RootTools.cmgTuple as cmgTuple
import ROOT as rt

import unittest

class TestTrigger(TestTools.CFGTest):

    def __init__(self,methodName):
        TestTools.CFGTest.__init__(self,methodName)
        self.cfgsRunOnce.append('CMGTools/Common/test/testTriggerObjects_cfg.py')

        #The source file is ZMM so we can use a muon trigger to test
        self.triggerName = 'HLT_Mu15_v2'
        self.triggerNameRe = 'HLT_Mu15_v[0-9]+'

    def testTreeEntries(self):
        """Test that the Events tree has some content"""

        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTriggerObjects_cfg.py']
        self.assert_(TestTools.getEntries(output[1]) > 0,'The CMGTuple must contain some entries')

    def testcmgTriggerObjectFullExists(self):
        """Tests that the cmgTriggerObjectFull branch exists"""

        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTriggerObjects_cfg.py']
        events = TestTools.getObject(output[1], 'Events')

        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.aliases.has_key('cmgTriggerObjectFull'),'We are expecting a branch called cmgTriggerObjectFull')

    def testcmgTriggerObjectSelExists(self):
        """Tests that the cmgTriggerObjectSel branch exists"""

        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTriggerObjects_cfg.py']
        events = TestTools.getObject(output[1], 'Events')

        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.aliases.has_key('cmgTriggerObjectSel'),'We are expecting a branch called cmgTriggerObjectSel')

    def testcmgTriggerObjectSelLength(self):
        """Verify that cmgTriggerObjectSel has length 1"""

        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTriggerObjects_cfg.py']
        events = TestTools.getObject(output[1], 'Events')

        cmg = cmgTuple.cmgTuple(events)
        self.assertEquals(cmg.Draw("cmgTriggerObjectSel.pt()",
                                 "cmgTriggerObjectSelVec.size() != 1","goff"), 0,\
                         'cmgTriggerObjectSel should have exactly one entry')

    def testcmgTriggerObjectSelTriggerExists(self):
        """Verify that cmgTriggerObjectSel has the selected trigger"""

        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTriggerObjects_cfg.py']
        events = TestTools.getObject(output[1], 'Events')

        cmg = cmgTuple.cmgTuple(events)
        entries = cmg.tree.GetEntries()

        self.assertEquals(cmg.Draw("cmgTriggerObjectSel.pt()",
                                 'cmgTriggerObjectSel.hasSelection("%s")' % self.triggerName,"goff"), entries,\
                         'Each event has the test trigger present')

    def testcmgTriggerObjectSelTriggerFires(self):
        """Verify that cmgTriggerObjectSel has the selected trigger fires"""

        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTriggerObjects_cfg.py']
        events = TestTools.getObject(output[1], 'Events')

        cmg = cmgTuple.cmgTuple(events)

        self.assertTrue(cmg.Draw("cmgTriggerObjectSel.pt()",
                                 'cmgTriggerObjectSel.getSelection("%s")' % self.triggerName,"goff") > 0,\
                         'There must be at least one event where the trigger fired')

    def testcmgTriggerObjectSelTriggerFirseRegExpBoth(self):
        """Verify that cmgTriggerObjectSel has the selected trigger fires"""

        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTriggerObjects_cfg.py']
        events = TestTools.getObject(output[1], 'Events')

        cmg = cmgTuple.cmgTuple(events)

        self.assertEqual(cmg.Draw("cmgTriggerObjectSel.pt()",
                                 'cmgTriggerObjectSel.getSelection("%s") && !cmgTriggerObjectSel.getSelectionRegExp("%s")' % (self.triggerName,self.triggerNameRe),"goff"),0,\
                         'There must be at least one event where the trigger fired')
        self.assertEqual(cmg.Draw("cmgTriggerObjectSel.pt()",
                                 'cmgTriggerObjectSel.getSelection("%s")' % self.triggerName,"goff"),
                         cmg.Draw("cmgTriggerObjectSel.pt()",
                                 'cmgTriggerObjectSel.getSelectionRegExp("%s")' % self.triggerNameRe,"goff"),
                         'The selections must match')

    def testcmgTriggerObjectSelTriggerFirseRegExp(self):
        """Verify that cmgTriggerObjectSel has the selected trigger fires"""

        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTriggerObjects_cfg.py']
        events = TestTools.getObject(output[1], 'Events')

        cmg = cmgTuple.cmgTuple(events)

        self.assertTrue(cmg.Draw("cmgTriggerObjectSel.pt()",
                                 'cmgTriggerObjectSel.getSelectionRegExp("%s")' % self.triggerNameRe,"goff") > 0,\
                         'There must be at least one event where the trigger fired')


    def testcmgTriggerObjectFullLength(self):
        """Verify that cmgTriggerObjectFull has some entries"""

        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTriggerObjects_cfg.py']
        events = TestTools.getObject(output[1], 'Events')

        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.Draw("cmgTriggerObjectFull.pt()",
                                 'cmgTriggerObjectFullVec.size() > 0',"goff") > 0,\
                        'cmgTriggerObjectFull should have some entries')

    def testcmgTriggerObjectFullSelection(self):
        """Verify that cmgTriggerObjectFull has some entries"""

        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTriggerObjects_cfg.py']
        events = TestTools.getObject(output[1], 'Events')

        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.Draw("cmgTriggerObjectFull.pt()",
                                 'cmgTriggerObjectFull.getSelection("%s")' % self.triggerName,"goff") > 0,\
                        'cmgTriggerObjectFull should have some entries')

    def testcmgTriggerObjectFullPt(self):
        """Verify that cmgTriggerObjectFull has some entries"""

        output = self.__class__.cfgsRunOnceCache['CMGTools/Common/test/testTriggerObjects_cfg.py']
        events = TestTools.getObject(output[1], 'Events')

        cmg = cmgTuple.cmgTuple(events)
        self.assertTrue(cmg.Draw("cmgTriggerObjectFull.pt()",
                                 'cmgTriggerObjectFull.getSelectionRegExp("%s") && cmgTriggerObjectFull.pt() > 0' % self.triggerNameRe,"goff") > 0,\
                         'cmgTriggerObjectFull has some pt')


if __name__ == '__main__':

    unittest.main()
