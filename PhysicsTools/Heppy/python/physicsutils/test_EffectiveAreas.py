import unittest
import pprint 

from EffectiveAreas import * 

class FakeSuperCluster(object): 
    def __init__(self, eta):
        self._eta = eta
    def eta(self):
        return self._eta

class FakeLepton(object):
    def __init__(self, pdgid, eta, sceta=None):
        self._pdgid = pdgid
        self._eta = eta
        if sceta: 
            self._sc = FakeSuperCluster(sceta)
    def eta(self):
        return self._eta
    def pdgid(self):
        return self._pdgid
    def superCluster(self):
        return self._sc  

class TestEffectiveAreas(unittest.TestCase):

    def test_1_fall17(self):
        '''test access to fall 17 eas'''
        scd_value = 0.1626
        last_value = 0.1524
        ele = FakeLepton(11, 0.9, 1.1)
        ea_table = effective_area_table(ele, 'Fall17')
        ea = effective_area(ele, '03', ea_table)
        self.assertEqual(ea, scd_value)
        # test out of range: 
        ele = FakeLepton(11, 99, 99)
        ea = effective_area(ele, '03', ea_table)
        self.assertEqual(ea, last_value)
        # test negative eta
        ele = FakeLepton(11, -0.9, -1.1)
        ea = effective_area(ele, '03', ea_table)
        self.assertEqual(ea, scd_value)

    def test_2_Phys14_25ns_v1(self):
        '''test access to Phys14_25ns_v1 
        (ele.eta() instead of supercluster eta)
        '''
        ele = FakeLepton(11, 0.81, 0.79)
        ea_table = effective_area_table(ele, 'Phys14_25ns_v1')
        ea = effective_area(ele, '03', ea_table)
        self.assertEqual(ea, 0.0988)

    def test_table_integrity(self):
        for era, value in areas.iteritems(): 
            for leptype, table in value.iteritems(): 
                # check lepton type
                self.assertIn(leptype, ['electron','muon'])
                # check presence of eta function
                self.assertIn('eta', table) 
                self.assertTrue( hasattr(table['eta'], '__call__'))
                del table['eta']
                # now only the tables for each cone size are left
                for cone, eas in table.iteritems():
                    cone_size = int(cone)
                    for ea in eas: 
                        self.assertTrue(len(ea)==2)
                    
    def test_is_ea_table(self):
        self.assertTrue( is_ea_table(areas['Fall17']['electron']) ) 
        self.assertFalse( is_ea_table('03')) 
        


if __name__ == '__main__':
    unittest.main()
