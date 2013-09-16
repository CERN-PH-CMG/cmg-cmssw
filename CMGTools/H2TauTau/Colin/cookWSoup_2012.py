# python cookWSoup_2012.py /afs/cern.ch/work/m/manzoni/public/diTau3May/MVA/mva_nom/WJets /afs/cern.ch/work/m/manzoni/public/diTau3May/MVA/mva_nom/W1Jets /afs/cern.ch/work/m/manzoni/public/diTau3May/MVA/mva_nom/W2Jets /afs/cern.ch/work/m/manzoni/public/diTau3May/MVA/mva_nom/W3Jets /afs/cern.ch/work/m/manzoni/public/diTau3May/MVA/mva_nom/W4Jets
# python cookWSoup_2012.py /afs/cern.ch/work/m/manzoni/public/diTau3May/MVA2/mva2_nom/WJets /afs/cern.ch/work/m/manzoni/public/diTau3May/MVA2/mva2_nom/W1Jets /afs/cern.ch/work/m/manzoni/public/diTau3May/MVA2/mva2_nom/W2Jets /afs/cern.ch/work/m/manzoni/public/diTau3May/MVA2/mva2_nom/W3Jets /afs/cern.ch/work/m/manzoni/public/diTau3May/MVA2/mva2_nom/W4Jets
# python cookWSoup_2012.py /afs/cern.ch/work/m/manzoni/public/diTau3May/DB3H/db3h_nom/WJets /afs/cern.ch/work/m/manzoni/public/diTau3May/DB3H/db3h_nom/W1Jets /afs/cern.ch/work/m/manzoni/public/diTau3May/DB3H/db3h_nom/W2Jets /afs/cern.ch/work/m/manzoni/public/diTau3May/DB3H/db3h_nom/W3Jets /afs/cern.ch/work/m/manzoni/public/diTau3May/DB3H/db3h_nom/W4Jets
##### good
# python cookWSoup_2012.py /afs/cern.ch/work/m/manzoni/public/diTau_parked_15may_2/nom/WJets /afs/cern.ch/work/m/manzoni/public/diTau_parked_15may_2/nom/W1Jets /afs/cern.ch/work/m/manzoni/public/diTau_parked_15may_2/nom/W2Jets /afs/cern.ch/work/m/manzoni/public/diTau_parked_15may_2/nom/W3Jets /afs/cern.ch/work/m/manzoni/public/diTau_parked_15may_2/nom/W4Jets
#
# python cookWSoup_2012.py /afs/cern.ch/work/m/manzoni/public/diTau_parked_2Jun/nom/WJets /afs/cern.ch/work/m/manzoni/public/diTau_parked_2Jun/nom/W1Jets /afs/cern.ch/work/m/manzoni/public/diTau_parked_2Jun/nom/W2Jets /afs/cern.ch/work/m/manzoni/public/diTau_parked_2Jun/nom/W3Jets /afs/cern.ch/work/m/manzoni/public/diTau_parked_2Jun/nom/W4Jets
# python cookWSoup_2012.py /afs/cern.ch/work/m/manzoni/public/diTau22Jun/nom/WJets /afs/cern.ch/work/m/manzoni/public/diTau22Jun/nom/W1Jets /afs/cern.ch/work/m/manzoni/public/diTau22Jun/nom/W2Jets /afs/cern.ch/work/m/manzoni/public/diTau22Jun/nom/W3Jets /afs/cern.ch/work/m/manzoni/public/diTau22Jun/nom/W4Jets
# python cookWSoup_2012.py /afs/cern.ch/work/m/manzoni/public/diTau_22Jul/nom/WJets /afs/cern.ch/work/m/manzoni/public/diTau_22Jul/nom/W1Jets /afs/cern.ch/work/m/manzoni/public/diTau_22Jul/nom/W2Jets /afs/cern.ch/work/m/manzoni/public/diTau_22Jul/nom/W3Jets /afs/cern.ch/work/m/manzoni/public/diTau_22Jul/nom/W4Jets
# python cookWSoup_2012.py /afs/cern.ch/work/m/manzoni/public/diTau_26Jul/nom/WJets /afs/cern.ch/work/m/manzoni/public/diTau_26Jul/nom/W1Jets /afs/cern.ch/work/m/manzoni/public/diTau_26Jul/nom/W2Jets /afs/cern.ch/work/m/manzoni/public/diTau_26Jul/nom/W3Jets /afs/cern.ch/work/m/manzoni/public/diTau_26Jul/nom/W4Jets

# python cookWSoup_2012.py /afs/cern.ch/work/m/manzoni/public/diTau_29Jul/nom/WJets /afs/cern.ch/work/m/manzoni/public/diTau_29Jul/nom/W1Jets /afs/cern.ch/work/m/manzoni/public/diTau_29Jul/nom/W2Jets /afs/cern.ch/work/m/manzoni/public/diTau_29Jul/nom/W3Jets /afs/cern.ch/work/m/manzoni/public/diTau_29Jul/nom/W4Jets


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
        fileName = '{name}/H2TauTauSyncTreeTauTau/H2TauTauSyncTreeTauTau_tree.root'.format(name=self.name)
        treeName = 'H2TauTauSyncTreeTauTau'
        #fileName = '{name}/H2TauTauTreeProducerTauTau/H2TauTauTreeProducerTauTau_tree.root'.format(name=self.name)
        #treeName = 'H2TauTauTreeProducerTauTau'
        self.file = TFile(fileName)
        self.tree = self.file.Get(treeName)
        self.tree.SetName('H2TauTauSyncTreeTauTau_{0:d}'.format(self.numberForNaming))
        #self.tree.SetName('H2TauTauTreeProducerTauTau_{0:d}'.format(self.numberForNaming))


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    
class H2TauTauSoup(TreeNumpy):

    def __init__(self, name, title):
        super(H2TauTauSoup, self).__init__(name,title)
        self.var('WJetWeight')
        self.var('skim')
        self.var('inclusive')
        self.var('nobtag')
        self.var('btag')
        # fraction of events with a given parton jet multiplicity, 
        # from the LO cross sections in prep:

        #  0j : (30400.0 - 5400 - 1750 - 519 - 214) / 30400 = 0.740690789474
        #  1j : 5400.0 / 30400.0 = 0.177631578947
        #  2j : 1750.0 / 30400.0 = 0.0575657894737
        #  3j : 519.0 / 30400.0 = 0.0170723684211
        #  4j : 214.0 / 30400.0 = 0.00703947368421

        #  0j : (36257.2 - 6440.4 - 2087.2 - 619.0 - 255.2) / 36257.2 = 
        #  1j : 6440.4 / 36257.2 =
        #  2j : 2087.2 / 36257.2 =
        #  3j : 619.0 / 36257.2 = 
        #  4j : 255.2 / 36257.2 = 
        self.fractions = [ 
            (36257.2 - 6440.4 - 2087.2 - 619.0 - 255.2) / 36257.2,
            6440.4 / 36257.2,
            2087.2 / 36257.2,
            619.0 / 36257.2, 
            255.2 / 36257.2 
            ]

        # number of events in the inclusive sample, corrected for prod efficiency
        self.ninc = 57709905*1.0+18393090*1.0
        # approximate number of events in each exclusive component of the inclusive sample
        self.Ninc = [frac * self.ninc for frac in self.fractions]

        #s number of events in the excusive samples, corrected for prod efficiency
        self.Nexc = [
            0                                          ,
            23141598. * 1.0            + 29784800 *1.0 , #* 0.920294737113 , #w/o weights
            34044921. * 1.0            + 30693853 *1.0 , #* 0.966486506452 , #w/o weights
            15539503. * 0.997191011236 + 15241144 *1.0 , #* 0.846858477273 , #w/o weights
            13382803. * 0.996168582375                 , #* 0.867004523077   #w/o weights
            ]

        self.WJetWeights = []
        for nJets in range (5):
            self.WJetWeights.append (self.Ninc[nJets] / ( self.Ninc[nJets] + self.Nexc[nJets]))
            print nJets, self.WJetWeights[nJets]

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
                    if val > 0 :
                      nJets = int(val-5)
                    else :
                      nJets = int(comp.name.replace('W','').replace('Jets','').replace('_new',''))
            # fill all the variables
            for varName in self.vars:
                if not hasattr(ie, varName):
                    continue
                val = getattr(ie, varName)
                if varName == 'weight':
                    #print 'nJets:', nJets
                    val = val * self.WJetWeights[nJets]
                if varName == 'mcweight':
                    #print 'nJets:', nJets
                    #val = val * self.WJetWeights[nJets]
                    val = self.WJetWeights[nJets]/self.ninc
                self.fill(varName, val)
            
            self.fill( 'WJetWeight', self.WJetWeights[nJets])
            
            #antiel = getattr(ie, 'againstElectronNewLooseMVA3_2')
            #iso_1  = getattr(ie, 'byCombinedIsolationDeltaBetaCorrRaw3Hits_1')
            #iso_2  = getattr(ie, 'byCombinedIsolationDeltaBetaCorrRaw3Hits_2')
            #nbtag  = getattr(ie, 'nbtag')
            #njets  = getattr(ie, 'njets')
            #charge = getattr(ie, 'q_1')*getattr(ie, 'q_2')
            
            #if index%50 == 0 :
            # print antiel, iso_1, iso_2, nbtag, njets, charge
            #self.fill( 'skim'     , 0 )
            #self.fill( 'inclusive', 0 )
            #self.fill( 'nobtag'   , 0 )
            #self.fill( 'btag'     , 0 )
            #
            #self.fill( 'skim', 1 )
            #
            #if iso_1 < 1. and iso_2 < 1. and charge < 0 and antiel > 0.5 :
            #  self.fill( 'inclusive', 1 )
            #  if nbtag == 0 :
            #    self.fill( 'nobtag', 1 )
            #  if nbtag > 0 and njets < 2 :
            #    self.fill( 'btag', 1 )
                        
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


    incComp = Component( args[1], numberForNaming )
    numberForNaming = numberForNaming + 1  
    components.append( incComp )
    print 'Inclusive WJets sample: ', incComp.name


    #excNames = args[1:]
    excNames = args[2:]
    for arg in excNames:
        print 'Exclusive WJets sample: ', arg
        components.append( Component(arg, numberForNaming) )
        numberForNaming = numberForNaming + 1  
    
    soupFile = TFile('soupWdb3h_parked.root','recreate')
    #soup = H2TauTauSoup('H2TauTauTreeProducerTauTau', 'H2TauTauTreeProducerTauTau')
    soup = H2TauTauSoup('H2TauTauSyncTreeTauTau', 'H2TauTauSyncTreeTauTau')
    soup.copyStructure( incComp.tree )

    for c in components:
        soup.importEntries (c)
#        soup.importEntries (c, nEntries = 20)

    soupFile.Write()
    soupFile.Close()
