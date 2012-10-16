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


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    
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

        # number of events in the inclusive sample, corrected for prod efficiency
        self.ninc = 57142200.4185   # 57709905 w/o weights
        # approximate number of events in each exclusive component of the inclusive sample
        self.Ninc = [frac * self.ninc for frac in self.fractions]

        #s number of events in the excusive samples, corrected for prod efficiency
        self.Nexc = [
            0,
            21715355.1552,         # 23141598 w/o weights
            33438921.4062,         # 34044921 w/o weights
            15214991.5589,         # 15539503 w/o weights
            13129814.4921          # 13382803 w/o weights
            ]

        self.WJetWeights = []
        for nJets in range (5):
            self.WJetWeights.append (self.Ninc[nJets] / ( self.Ninc[nJets] + self.Nexc[nJets]))
            print nJets, self.WJetWeights[nJets]

        # weights from Christian
#        self.WJetWeights[0] = 1
#        self.WJetWeights[1] = 0.37
#        self.WJetWeights[2] = 0.11
#        self.WJetWeights[3] = 0.077
#        self.WJetWeights[4] = 0.038

        # weights that let the two distributions match 
#        self.WJetWeights[0] = 1
#        self.WJetWeights[1] = 0.37
#        self.WJetWeights[2] = 0.125
#        self.WJetWeights[3] = 0.077
#        self.WJetWeights[4] = 0.038

#        self.WJetWeights[0] = 1
#        self.WJetWeights[1] = 1
#        self.WJetWeights[2] = 1
#        self.WJetWeights[3] = 1
#        self.WJetWeights[4] = 1

#        self.Nexc = [
#            0,
#            21715355.1552,
#            0, # 33438921.4062,
#            0, # 715214991.5589,
#            0  # 13129814.4921
#            ]



# .... .... .... .... .... .... .... .... .... .... .... .... ....

    
    def importEntries(self, comp, nEntries=-1):
        print 'importing', comp.name
        tree = comp.tree
        for index, ie in enumerate(tree):
            if index%1000==0: print 'entry:', index
            # get the additional weight
            nJets = None
            for varName in self.vars:
                if not hasattr(ie, varName):
                    continue
                if varName == 'NUP':
                    val = getattr(ie, varName)
                    nJets = int(val-5)
            # fill all the variables
            for varName in self.vars:
                if not hasattr(ie, varName):
                    continue
                val = getattr(ie, varName)
                if varName == 'weight':
                    val = val * self.WJetWeights[nJets]
                self.fill(varName, val)
            self.tree.Fill()
            if nEntries>0 and index+1>=nEntries:
                return 


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    
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

    for c in components:
        soup.importEntries (c)
#        soup.importEntries (c, nEntries = 20)

    soupFile.Write()
    soupFile.Close()
