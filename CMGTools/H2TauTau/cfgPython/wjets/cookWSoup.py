import copy
from CMGTools.RootTools.PyRoot import *
from CMGTools.RootTools.statistics.TreeNumpy import *

files = []

class Component(object):
    '''
    Just a tree (keeps its file to prevent deletion by garbage collector.
    '''
    
    def __init__(self, name, channel='TauMu'):
        self.name = name.rstrip('/')
        self.tree = None
        self.attachTree( channel )

    def attachTree(self, channel):
        fileName = '{name}/H2TauTauTreeProducer{channel}/H2TauTauTreeProducer{channel}_tree.root'.format(channel=channel, name=self.name)
        treeName = 'H2TauTauTreeProducer{channel}'.format(channel=channel)
        self.file = TFile(fileName)
        self.tree = self.file.Get(treeName)


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    
class H2TauTauSoup(TreeNumpy):
    '''Manages the creation of a soup tree, using an inclusive WJet sample,
    and some exclusive WJet samples.

    usage example: 
    python ../Colin/cookWSoup.py WJets W1Jets W2Jets W3Jets W4Jets -f ../Colin/fractions_tauEle.txt -n ../Colin/events_tauEle.txt -c TauEle

    All variables in the input trees are copied to the soup tree.
    A new event weight "WJetWeight" is added.
    This weight is multiplied to the global event weight "weight".
    '''

    def __init__(self, name, title, fractions, nevents):
        '''
        fractions gives the fraction of events with a given number of jets
        in the inclusive sample:
        [
        0.752276599407,    # frac with 0 jet
        0.171668857336,    # with 1 jet
        0.0536961443722,   # etc...
        0.0159474294633,
        0.00641100015491,
        ]

        nevents is a list of floats like this:
        [
        81035922.744426474,  # nevents in inclusive sample
        0,                   # nevents in exclusive W1Jets sample
        25326376.405680001,  # nevents in exclusive W2Jets sample
        7514498.0491649993,  # nevents in exclusive W3Jets sample
        13022311.366807999,  # nevents in exclusive W4Jets sample
        ]
        '''
        super(H2TauTauSoup, self).__init__(name,title)
        self.var('WJetWeight')
        self.fractions = fractions
        self.nexc = nevents
        self.ninc = self.nexc[0]
        self.nexc[0] = 0.
        self.Ni = [frac*self.ninc for frac in self.fractions]
        self.WJetWeights = []
        for nJets in range (5):
            self.WJetWeights.append (self.Ni[nJets] / ( self.Ni[nJets] + self.nexc[nJets]))
            print nJets, self.WJetWeights[nJets]


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    
    def importEntries(self, comp, nEntries=-1):
        '''
        Add a component to the soup
        '''
        print 'importing', comp.name
        tree = comp.tree
        for index, ie in enumerate(tree):
            if index%1000==0: print 'entry:', index
            # calculating the W+jets weight
            nup = getattr(ie, 'NUP')
            nJets = int(nup-5)
            self.fill('WJetWeight', self.WJetWeights[nJets])
            for varName in self.vars:
                if not hasattr(ie, varName):
                    continue
                val = getattr(ie, varName)
                if varName == 'weight':
                    val *= self.WJetWeights[nJets]
                self.fill(varName, val)
            self.tree.Fill()
            if nEntries>0 and index+1>=nEntries:
                return 


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    
if __name__ == '__main__':

    import imp
    import pprint

    from optparse import OptionParser
    parser = OptionParser(usage = '')
    parser.add_option('-f', '--fractions',
                      dest = 'fractions',
                      default=None,
                      help='file containing the fraction of events with a given number of jets in the inclusive sample')
    parser.add_option('-n', '--nevents',
                      dest = 'nevents',
                      default=None,
                      help='file containing the number of events for the inclusive sample and the exclusive samples')
    parser.add_option('-c', '--channel',
                      dest = 'channel',
                      default='TauMu',
                      help='channel: TauMu by default')

    (options, args) = parser.parse_args()

    def numbers( fileName ):
        file = open(fileName)
        nums = [float(num) for num in file]
        print fileName
        pprint.pprint(nums)
        return nums

    fractions = numbers( options.fractions )
    nevents = numbers( options.nevents )
    
    components = []

    channel = options.channel
    incComp = Component( args[0], channel )  
    components.append( incComp )
    print 'Inclusive WJets sample: ', incComp.name

    excNames = args[1:]
    for arg in excNames:
        print 'Exclusive WJets sample: ', arg
        components.append( Component(arg, channel) )
    
    soupFile = TFile('soup.root','recreate')
    treeName = 'H2TauTauTreeProducer{channel}'.format(channel=channel)
    soup = H2TauTauSoup(treeName, treeName, fractions, nevents)
    soup.copyStructure( incComp.tree )
    for c in components:
        soup.importEntries( c )

    soupFile.Write()
    soupFile.Close()
