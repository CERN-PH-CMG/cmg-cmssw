#!/use/bin/env python
import ROOT as rt
from optparse import OptionParser
import os
from CalcBDT import CalcBDT

from CMGTools.RootTools.RootIterator import RootIterator
from CMGTools.RootTools.RootFile import RootFile

if __name__ == '__main__':

    parser = OptionParser()
    (options,args) = parser.parse_args()

    for f in args:

        if f.lower().endswith('.root') and os.path.exists(f):
            inputFile = rt.TFile.Open(f)
            store = RootFile('foo.root')
            for key in RootIterator(inputFile.GetListOfKeys()):
                if key.GetName().startswith('RMRTree'):
                    tree = key.ReadObj()
                    calc = CalcBDT(tree)
                    calc.tree.SetName(key.GetName())
                    for i in xrange(tree.GetEntries()):
                        tree.GetEntry(i)
                        if calc.select(): calc.Fill()
                    store.add(calc.tree)
            store.write()
                        
                        
                    
            
            

            
