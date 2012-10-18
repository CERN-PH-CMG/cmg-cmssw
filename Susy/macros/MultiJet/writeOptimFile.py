import ROOT as rt
import pickle, os, sys

class BranchDumper(object):

    def __init__(self, tree, norm = None, xs = None):

        self.tree = tree
        self.norm = norm

        self.lumi = 5000.
        self.filter = 1.0

        self.weightVal = 1.0
        if norm is not None:
            point = (-1,-1)
            #point = (200,0)
            self.weightVal = (self.lumi*float(xs)*self.filter)/(1.0*int(self.norm[point]))
            print self.weightVal

        self.vars = None

    def select(self):
        return self.tree.BOX_NUM == 6 and self.tree.hadBoxFilter and self.tree.hadTriggerFilter and self.tree.NBJET > 0 and self.tree.MR >= 450 and self.tree.RSQ >= 0.03 and\
            self.tree.nMuonTight == 0 and self.tree.nElectronTight == 0 and self.tree.nTauTight == 0 and self.tree.nMuonLoose == 0 and self.tree.nElectronLoose == 0 #and self.tree.genInfo == 22000 
    
    def weight(self):
        return self.weightVal
        #return 1.0

    def thetaH1(self):
        if self.tree.bestHemi == 1:
            return self.tree.hemi1ThetaH
        return self.tree.hemi2ThetaH
    def thetaH2(self):
        if self.tree.bestHemi == 2:
            return self.tree.hemi1ThetaH
        return self.tree.hemi2ThetaH

    def topMass1(self):
        if self.tree.bestHemi == 1:
            return self.tree.hemi1TopMass
        return self.tree.hemi2TopMass
    def topMass2(self):
        if self.tree.bestHemi == 2:
            return self.tree.hemi1TopMass
        return self.tree.hemi2TopMass

    def wMass1(self):
        if self.tree.bestHemi == 1:
            return self.tree.hemi1WMass
        return self.tree.hemi2WMass
    def wMass2(self):
        if self.tree.bestHemi == 2:
            return self.tree.hemi1WMass
        return self.tree.hemi2WMass

    def jetNpt(self, n):
        return self.tree.jet_pt.at(n)

    def jet1pt(self):
        return self.jetNpt(0)
    def jet2pt(self):
        return self.jetNpt(1)
    def jet3pt(self):
        return self.jetNpt(2)
    def jet4pt(self):
        return self.jetNpt(3)
    def jet5pt(self):
        return self.jetNpt(4)
    def jet6pt(self):
        return self.jetNpt(5)

    def jet1mult(self):
        return self.tree.jet_mult.at(0)
    def jet2mult(self):
        return self.tree.jet_mult.at(1)
    def jet3mult(self):
        return self.tree.jet_mult.at(2)
    def jet4mult(self):
        return self.tree.jet_mult.at(3)
    def jet5mult(self):
        return self.tree.jet_mult.at(4)
    def jet6mult(self):
        return self.tree.jet_mult.at(5)

    def jet1girth(self):
        return self.tree.jet_girth.at(0)
    def jet2girth(self):
        return self.tree.jet_girth.at(1)
    def jet3girth(self):
        return self.tree.jet_girth.at(2)
    def jet4girth(self):
        return self.tree.jet_girth.at(3)
    def jet5girth(self):
        return self.tree.jet_girth.at(4)
    def jet6girth(self):
        return self.tree.jet_girth.at(5)


    def headers(self):
        return ['weight','thetaH1','thetaH2','topMass1','topMass2','wMass1','wMass2',\
                    'jet1pt','jet2pt','jet3pt','jet4pt','jet5pt','jet6pt',\
                    'jet1mult','jet2mult','jet3mult','jet4mult','jet5mult','jet6mult',\
                    'jet1girth','jet2girth','jet3girth','jet4girth','jet5girth','jet6girth']

    def headers_for_MVA(self):
        return ['thetaH1','thetaH2','topMass1','topMass2','wMass1','wMass2',\
                    'jet1mult','jet2mult','jet3mult','jet4mult','jet5mult','jet6mult',\
                    'jet1girth','jet2girth','jet3girth','jet4girth','jet5girth','jet6girth']

    def values(self):
        values = []
        for h in self.headers():
            values.append(getattr(self,h)())
        return values

    def make_tree(self):
        
        rt.gROOT.ProcessLine("""
struct BranchDumper{\
    Double_t weight;\
    Double_t thetaH1;\
    Double_t thetaH2;\
    Double_t topMass1;\
    Double_t topMass2;\
    Double_t wMass1;\
    Double_t wMass2;\
    Double_t jet1pt;\
    Double_t jet2pt;\
    Double_t jet3pt;\
    Double_t jet4pt;\
    Double_t jet5pt;\
    Double_t jet6pt;\
    Double_t jet1mult;\
    Double_t jet2mult;\
    Double_t jet3mult;\
    Double_t jet4mult;\
    Double_t jet5mult;\
    Double_t jet6mult;\
    Double_t jet1girth;\
    Double_t jet2girth;\
    Double_t jet3girth;\
    Double_t jet4girth;\
    Double_t jet5girth;\
    Double_t jet6girth;\
    Double_t MR;\
    Double_t RSQ;\
};""")
        from ROOT import BranchDumper
        tree = rt.TTree('RMRTree','Multijet events')
        tree.SetDirectory(0)
        
        def setAddress(obj, flag):
            for branch in dir(obj):
                if branch.startswith('__'): continue
                tree.Branch(branch,rt.AddressOf(obj,branch),'%s/%s' % (branch,flag) )
    
        self.vars = BranchDumper()
        setAddress(self.vars,'D')
        return tree
        
    def set_tree(self, tree):
        for h in self.headers():
            setattr(self.vars,h,getattr(self,h)())
        self.vars.MR = self.tree.MR
        self.vars.RSQ = self.tree.RSQ
        tree.Fill()

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

    if options.pickle is None or not os.path.exists(options.pickle):
        print 'Need a pickle file'
        sys.exit(-1)

    if options.tree is None or not os.path.exists(options.tree):
        print 'Need a root file'
        sys.exit(-1)

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
