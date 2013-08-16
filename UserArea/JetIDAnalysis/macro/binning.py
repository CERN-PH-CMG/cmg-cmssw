from CMGTools.RootTools.utils.PileupJetHistograms import mkBinLabels

vtxlabels = [ "_vtx%s" % l for l in  mkBinLabels((10,20),addOFlow=True) ]
ptbins    = (10,20,25,30,40,50)
ptlabels  = [ "_pt%s" % l for l in mkBinLabels(ptbins,addOFlow=True) ]
etalables = ["TK","HEin","HEout","HF"]
