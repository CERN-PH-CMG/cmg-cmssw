import copy
from CMGTools.RootTools.PyRoot import *
from CMGTools.RootTools.statistics.TreeNumpy import *

files = []

class Component(object):
    def __init__(self, name):
        self.name = name.rstrip('/')
        self.tree = None
        self.attachTree()

    def attachTree(self):
        fileName = '{name}/H2TauTauTreeProducerTauMu/H2TauTauTreeProducerTauMu_tree.root'.format(name=self.name)
        treeName = 'H2TauTauTreeProducerTauMu'
        self.file = TFile(fileName)
        self.tree = self.file.Get(treeName)



class H2TauTauSoup(TreeNumpy):

    def __init__(self, name, title):
        super(H2TauTauSoup, self).__init__(name,title)
        self.var('WJetWeight')
        # fraction of events with a given parton jet multiplicity, from Andrew
##         self.fractions = [
##             0.743925,  # 0 jet
##             0.175999,  # 1 jet
##             0.0562617, # ... 
##             0.0168926, # 3 jet 
##             0.00692218,
##             ]
##         self.fractions = [
##             0.751815736294,
##             0.172145709395,
##             0.0537378564477,
##             0.0159421432763,
##             0.00635857135057,
##             ]
        self.fractions = [ # 7 million events
            0.752276599407,
            0.171668857336,
            0.0536961443722,
            0.0159474294633,
            0.00641100015491
            ]

        self.ninc = 79622424.776491657
        self.n = [
            0,
            0,
            0,
            7514498.0491649993,
            0,
            ]
        self.Ni = [frac*self.ninc for frac in self.fractions]
        # self.nTot = sum(self.n)
    
    def importEntries(self, comp, inclusive, nEntries=-1):
        print 'importing', comp.name
        tree = comp.tree
        if inclusive:
            self.ninc = tree.GetEntries()
        for index, ie in enumerate(tree):
            if index%1000==0: print 'entry:', index
            for varName in self.vars:
                if not hasattr(ie, varName):
                    continue
                val = getattr(ie, varName)
                self.fill(varName, val)
                if varName == 'NUP':
                    nJets = int(val-5)
                    # print nJets, self.Ni[nJets], self.n[nJets]
                    WJetWeight = self.Ni[nJets] / ( self.Ni[nJets] + self.n[nJets] )
                    self.fill('WJetWeight', WJetWeight)
                    # print 'nJets', nJets, WJetWeight
            self.tree.Fill()
            if nEntries>0 and index+1>=nEntries:
                return 

if __name__ == '__main__':

    # import sys
    import imp

    args = sys.argv[1:]

##     anaDir = args[0]
##     cfgFileName = args[1]
##     file = open( cfgFileName, 'r' )
##     cfg = imp.load_source( 'cfg', cfgFileName, file)

    components = []
    
    incComp = Component( args[0] )  
    components.append( incComp )
    print 'Inclusive WJets sample: ', incComp.name

    excNames = args[1:]
    for arg in excNames:
        print 'Exclusive WJets sample: ', arg
        components.append( Component(arg) )
    
    soupFile = TFile('soup.root','recreate')
    soup = H2TauTauSoup('soup', 'WJetSoup')
    soup.copyStructure( incComp.tree )
    inclusive = True
    for c in components:
        soup.importEntries( c, inclusive=inclusive)
        inclusive = False
