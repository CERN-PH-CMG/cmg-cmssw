from CMGTools.TTHAnalysis.treeReAnalyzer import *

class Sort3L:
    def __init__(self):
 	self.branches = [ "iJetByCSV_0" ]
    def listBranches(self):
        return self.branches[:]	
    def __call__(self,event):
        # make python lists as Collection does not support indexing in slices
        jets = [j for j in Collection(event,"Jet","nJet25",8)]
        ret = dict([(name,0.0) for name in self.branches])
 	jbtag = [ j.btagCSV for j in jets]
	i = 0
	maks = 0
	maks_index = 0
	for i in range(len(jets)):
	    if jbtag[i] > maks: 
	    	maks = jbtag[i]
		maks_index = i
	ret["iJetByCSV_0"] = maks_index
	return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("ttHLepTreeProducerBase")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = EventVars2LSS()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

        
