from CMGTools.TTHAnalysis.treeReAnalyzer import *

class angular_vars:
    def __init__(self):
 	self.branches = [ "L_phi", 
			  "B_loose_phi", "B_medium_phi", "B_tight_phi", 
			  "DR_l_b_loose1", "DR_l_b_medium1", "DR_l_b_tight1", 
			  "DR_l_b_loose2", "DR_l_b_medium2", "DR_l_b_tight2",
			  "pTl_DR_l_b_loose1", "pTl_DR_l_b_medium1", "pTl_DR_l_b_tight1", 
			  "pTl_DR_l_b_loose2", "pTl_DR_l_b_medium2", "pTl_DR_l_b_tight2",  
			  "L_pt" ]
    def listBranches(self):
        return self.branches[:]	
    def __call__(self,event):
        # make python lists as Collection does not support indexing in slices
	leps = [l for l in Collection(event,"LepGood","nLepGood",4)]
        jets = [j for j in Collection(event,"Jet","nJet25",8)]
	bloose  = [j for j in jets if j.btagCSV > 0.605]
        bmedium = [j for j in jets if j.btagCSV > 0.890]
	btight = [j for j in jets if j.btagCSV > 0.970]
	(met, metphi)  = event.met_pt, event.met_phi
	njet = len(jets); nlep = len(leps); nbloose = len(bloose); nbmedium = len(bmedium); nbtight = len(btight);
	ret = dict([(name,0.0) for name in self.branches])
	
	#L_phi:
	
	l_phi = 0
        for i in range(len(leps)):
	    if leps[i].relIso03 < 0.1:
	       l_phi += leps[i].pt * abs(deltaPhi(metphi,leps[i].phi))
	ret["L_phi"] = l_phi
	
		
	#B_phi:
	
	b_loose_phi = 0
	for j in range(len(bloose)):
	    b_loose_phi += bloose[j].pt * abs(deltaPhi(metphi,bloose[j].phi))
	ret["B_loose_phi"] = b_loose_phi
	b_medium_phi = 0
	for k in range(len(bmedium)):
	    b_medium_phi += bmedium[k].pt * abs(deltaPhi(metphi,bmedium[k].phi))
	ret["B_medium_phi"] = b_medium_phi
	b_tight_phi = 0
	for l in range(len(btight)):
	    b_tight_phi += btight[l].pt * abs(deltaPhi(metphi,btight[l].phi))
	ret["B_tight_phi"] = b_tight_phi
	
	#DR_l_b_1:
		
	if nbloose >= 1:
	   if leps[0].relIso03 < 0.1:
              ret["DR_l_b_loose1"] = min([deltaR(m,leps[0]) for m in bloose]) if nlep >= 1 else 0;
	if nbmedium >= 1:
	   if leps[0].relIso03 < 0.1:
              ret["DR_l_b_medium1"] = min([deltaR(n,leps[0]) for n in bmedium]) if nlep >= 1 else 0;
	if nbtight >= 1:
	   if leps[0].relIso03 < 0.1:
              ret["DR_l_b_tight1"] = min([deltaR(p,leps[0]) for p in btight]) if nlep >= 1 else 0;
	      
	#DR_l_b_2:

	if nbloose >= 1:
	   if leps[1].relIso03 < 0.1:
              ret["DR_l_b_loose2"] = min([deltaR(q,leps[1]) for q in bloose]) if nlep >= 2 else 0;
	if nbmedium >= 1:
	   if leps[1].relIso03 < 0.1:
	      ret["DR_l_b_medium2"] = min([deltaR(r,leps[1]) for r in bmedium]) if nlep >= 2 else 0;
	if nbtight >= 1:
	   if leps[1].relIso03 < 0.1:
	      ret["DR_l_b_tight2"] = min([deltaR(s,leps[1]) for s in btight]) if nlep >= 2 else 0;

	#pTl_DR_l_b_1:

	if nbloose >= 1:
	   if leps[0].relIso03 < 0.1:
              ret["pTl_DR_l_b_loose1"] = leps[0].pt * min([deltaR(t,leps[0]) for t in bloose]) if nlep >= 1 else 0;
	if nbmedium >= 1:
	   if leps[0].relIso03 < 0.1:
              ret["pTl_DR_l_b_medium1"] = leps[0].pt * min([deltaR(u,leps[0]) for u in bmedium]) if nlep >= 1 else 0;
	if nbtight >= 1:
	   if leps[0].relIso03 < 0.1:
              ret["pTl_DR_l_b_tight1"] = leps[0].pt * min([deltaR(v,leps[0]) for v in btight]) if nlep >= 1 else 0;

	#pTl_DR_l_b_2:	      

	if nbloose >= 1:
	   if leps[1].relIso03 < 0.1:
              ret["pTl_DR_l_b_loose2"] = leps[1].pt * min([deltaR(w,leps[1]) for w in bloose]) if nlep >= 2 else 0;
	if nbmedium >= 1:
	   if leps[1].relIso03 < 0.1:
	      ret["pTl_DR_l_b_medium2"] = leps[1].pt * min([deltaR(x,leps[1]) for x in bmedium]) if nlep >= 2 else 0;
	if nbtight >= 1:
	   if leps[1].relIso03 < 0.1:
	      ret["pTl_DR_l_b_tight2"] = leps[1].pt * min([deltaR(y,leps[1]) for y in btight]) if nlep >= 2 else 0;

	#L_pt:

	l_pt = 0
	for z in range(len(leps)):
	    if leps[z].relIso03 < 0.1:
	       l_pt += leps[z].pt
	ret["L_pt"] = l_pt
	 	
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

        
