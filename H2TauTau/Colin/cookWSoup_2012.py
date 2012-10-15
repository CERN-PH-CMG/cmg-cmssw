import copy
from CMGTools.RootTools.PyRoot import *
from CMGTools.RootTools.statistics.TreeNumpy import *

files = []

class Component(object):
    def __init__(self, name, numberForNaming = 99):
        self.name = name.rstrip('/')
        self.tree = None
        self.numberForNaming = numberForNaming
        self.attachTree()

    def attachTree(self):
        fileName = '{name}/H2TauTauTreeProducerTauEle/H2TauTauTreeProducerTauEle_tree.root'.format(name=self.name)
        treeName = 'H2TauTauTreeProducerTauEle'
        self.file = TFile(fileName)
        self.tree = self.file.Get(treeName)
        self.tree.SetName('H2TauTauTreeProducerTauEle_{0:d}'.format(self.numberForNaming))
        print self.tree.GetName()


class H2TauTauSoup(TreeNumpy):

    def __init__(self, name, title):
        super(H2TauTauSoup, self).__init__(name,title)
        self.var('WJetWeight')
        # fraction of events with a given parton jet multiplicity, 
        # from the LO cross sections in prep:
        #  0j : (30400.0 - 5400 - 1750 - 519 - 214) / 30400 = 0.740690789474
        #  1j : 5400.0 / 30400.0 = 0.177631578947
        #  2j : 1750.0 / 30400.0 = 0.0575657894737
        #  3j : 519.0 / 30400.0 = 0.0170723684211
        #  4j : 214.0 / 30400.0 = 0.00703947368421
        self.fractions = [ 
            0.740690789474,
            0.177631578947,
            0.0575657894737,
            0.0170723684211,
            0.00703947368421
            ]

        # number of events in the inclusive sample
        self.ninc = 57142200.4185
        #s number of events in the excusive samples, corrected for efficiency
        self.n = [
            0,
            21715355.1552,
            33438921.4062,
            715214991.5589,
            13129814.4921,
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

# call the function from the folder that contains the components
# arguments are: the inclusive component, the exclusive components in order

    components = []
    
    numberForNaming = 0
    incComp = Component( args[0], numberForNaming )
    numberForNaming = numberForNaming + 1  
    components.append( incComp )
    print 'Inclusive WJets sample: ', incComp.name

    excNames = args[1:]
    for arg in excNames:
        print 'Exclusive WJets sample: ', arg
        components.append( Component(arg, numberForNaming) )
        numberForNaming = numberForNaming + 1  
    
    soupFile = TFile('soup.root','recreate')
    soup = H2TauTauSoup('H2TauTauTreeProducerTauEle', 'H2TauTauTreeProducerTauEle')
    soup.copyStructure( incComp.tree )
    inclusive = True
    for c in components:
        soup.importEntries (c, inclusive=inclusive)
        inclusive = False
