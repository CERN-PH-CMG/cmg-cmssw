import ROOT as rt
import pickle, os, sys

from CalcBDT import *

if __name__ == '__main__':

    from optparse import OptionParser
    parser = OptionParser()

    parser.add_option("-o", "--output", dest="output",
                  help="The output file name", default='out.txt')
    parser.add_option("-p", "--pickle", dest="pickle",
                  help="The input pickle file name", default=None)
    parser.add_option("-t", "--tree", dest="tree",
                  help="The input root file name", default=None)
    parser.add_option("-x", "--xs", dest="xs",
                  help="The cross-section in pb", default=1.0)

    (options,args) = parser.parse_args()

    if options.tree is None or not os.path.exists(options.tree):
        print 'Need a root file'
        sys.exit(-1)

    norms = None
    if options.pickle is not None:
        norms = pickle.load(file(options.pickle))
    tree_file = rt.TFile.Open(options.tree)

    tree = tree_file.Get('RMRTree')
    dumper = BranchDumper(tree,norms,options.xs)

    tr = dumper.make_tree()

    output = file(options.output,'w')
    print >> output, '\t'.join(dumper.headers())

    for i in xrange(max(100,tree.GetEntries())):
        tree.GetEntry(i)
        if not dumper.select(): continue
        dumper.set_tree(tr)

        print >> output, '\t'.join([str(v) for v in dumper.values()])

    output.close()
    tree_file.Close()

    out_root = options.output.replace('.txt','.root')
    output = rt.TFile.Open(out_root,'recreate')
    tr.Write()
    output.Close()
