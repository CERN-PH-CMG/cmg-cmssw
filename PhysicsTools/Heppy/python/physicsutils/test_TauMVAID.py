import unittest 
import pprint 

import logging 
logging.basicConfig(level=logging.INFO)


class TestTauMVAID(unittest.TestCase): 

    def test_mvaid_2017(self): 
        from TauMVAID import tau_mvaid_2017 as mvaid
        mvaid.threshold(50, 'Eff50')
        mvaid.threshold(20, 'Eff50')
        mvaid.threshold(1900, 'Eff50')
        mvaid.threshold(1900, 'Eff90')
        


if __name__ == '__main__':
    unittest.main()
