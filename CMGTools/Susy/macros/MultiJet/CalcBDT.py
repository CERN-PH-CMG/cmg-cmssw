import ROOT as rt
import array

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
        self.jets = range(6)

    def tag_jets(self):
        #order by btag output - high to low
        jets = sorted([(self.tree.jet_csv.at(i),i) for i in xrange(len(self.tree.jet_csv))],reverse=True)
        self.jets = [i for b, i in jets]

    def select(self): #TTbar MC
        return self.tree.metFilter and self.tree.hadBoxFilter and self.tree.hadTriggerFilter and self.tree.nCSVM > 0 and self.tree.MR >= 450 and self.tree.RSQ >= 0.03 and\
            self.tree.nMuonTight == 0 and self.tree.nElectronTight == 0 and not self.tree.isolatedTrack10Filter and self.tree.nMuonLoose == 0 and self.tree.nElectronLoose == 0
#    def select(self): # QCD CR from data
#        return self.tree.metFilter and self.tree.hadBoxFilter and self.tree.hadTriggerFilter and self.tree.nCSVL == 0 and self.tree.MR >= 450 and self.tree.RSQ >= 0.03 and\
#            self.tree.nMuonTight == 0 and self.tree.nElectronTight == 0 and not self.tree.isolatedTrack10Filter and self.tree.nMuonLoose == 0 and self.tree.nElectronLoose == 0
    
    def weight(self):
        return self.weightVal
        #return 1.0
    def weightPU(self):
        return self.tree.pileUpWeight

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
        return self.tree.jet_pt.at(self.jets[n])

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
        return self.tree.jet_mult.at(self.jets[0])
    def jet2mult(self):
        return self.tree.jet_mult.at(self.jets[1])
    def jet3mult(self):
        return self.tree.jet_mult.at(self.jets[2])
    def jet4mult(self):
        return self.tree.jet_mult.at(self.jets[3])
    def jet5mult(self):
        return self.tree.jet_mult.at(self.jets[4])
    def jet6mult(self):
        return self.tree.jet_mult.at(self.jets[5])

    def jet1girth(self):
        return self.tree.jet_girth_ch.at(self.jets[0])
    def jet2girth(self):
        return self.tree.jet_girth_ch.at(self.jets[1])
    def jet3girth(self):
        return self.tree.jet_girth_ch.at(self.jets[2])
    def jet4girth(self):
        return self.tree.jet_girth_ch.at(self.jets[3])
    def jet5girth(self):
        return self.tree.jet_girth_ch.at(self.jets[4])
    def jet6girth(self):
        return self.tree.jet_girth_ch.at(self.jets[5])

    def nVertex(self):
        return self.tree.nVertex


    def headers(self):
        return ['nVertex','weightPU','weight','thetaH1','thetaH2','topMass1','topMass2','wMass1','wMass2',\
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

    def make_tree(self, clone = False):
        
        rt.gROOT.ProcessLine("""
struct BranchDumper{\
    Double_t weight;\
    Double_t weightPU;\
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
    Double_t nVertex;\
    Double_t BDT;\
};""")
        from ROOT import BranchDumper
        if not clone:
            tree = rt.TTree('RMRTree','Multijet events')
        else:
            tree = self.tree.CloneTree(0)
        tree.SetDirectory(0)
        
        def setAddress(obj, flag):
            for branch in dir(obj):
                if branch.startswith('__'): continue
                tree.Branch(branch,rt.AddressOf(obj,branch),'%s/%s' % (branch,flag) )
    
        self.vars = BranchDumper()
        setAddress(self.vars,'D')
        return tree
        
    def set_tree(self, tree, fill = True):
        #self.tag_jets()
        for h in self.headers():
            setattr(self.vars,h,getattr(self,h)())
        self.vars.MR = self.tree.MR
        self.vars.RSQ = self.tree.RSQ
        if fill:
            tree.Fill()

class CalcBDT(object):

    def __init__(self, oldTree):
        
        self.sel = BranchDumper(oldTree)
        self.tree = self.sel.make_tree(False)

        self.reader = rt.TMVA.Reader()
        self.bdt_vars = {}
        for h in self.sel.headers_for_MVA():
            self.bdt_vars['%s_var'%h] = array.array('f',[0])
            self.reader.AddVariable(h,self.bdt_vars['%s_var'%h])
        self.mr_var = array.array('f',[0])
        self.rsq_var = array.array('f',[0])
        self.nvertex_var = array.array('f',[0])
        
        self.reader.AddSpectator('MR',self.mr_var)
        self.reader.AddSpectator('RSQ',self.rsq_var)
        self.reader.AddSpectator('nVertex',self.nvertex_var)
        self.reader.BookMVA('BDT','/afs/cern.ch/user/w/wreece/work/CMGTools/V5_6_0/CMGTools/CMSSW_5_3_3_patch3/src/CMGTools/Susy/prod/MultiJet/TMVAClassification_BDT.weights.xml')

    def select(self):
        return self.sel.select()

    def bdt(self):
        
        for h in self.sel.headers_for_MVA():
            self.bdt_vars['%s_var'%h][0] = getattr(self.sel,h)()
        bdt = self.reader.EvaluateMVA('BDT')
        return bdt

    def Fill(self):
        self.sel.set_tree(self.tree, False)
        self.sel.vars.BDT = self.bdt()
        self.tree.Fill()
        
