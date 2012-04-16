import ROOT as rt

def getBTagScale(pt):
    """Return the data/MC scale factor from muon+jet events with the TCHEM tagger"""
    #see https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFb-mujet_payload.txt
    if pt > 670: pt = 670
    if pt < 30: pt = 30
    return 0.932251*((1.+(0.00335634*pt))/(1.+(0.00305994*pt)))

if __name__ == '__main__':

    rt.gROOT.ProcessLine("""
struct Extras{\
    Int_t PROC;\
    Double_t LEP_W;\
    Double_t W;\
    Double_t W_EFF;\
    Int_t BOX_NUM;\
};""")
    from ROOT import Extras, std
    extras = Extras()
    extras.PROC = 0
    extras.LEP_W = 1.0
    extras.W = 1.0

    fileName = 'SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z-Summer11-PU_START42_V11_FastSim-v1-wreece_080212-SMS-NoSkim.root'
    inputFile = rt.TFile.Open(fileName)
    oldTree = inputFile.Get('RMRTree')

    outputFile = rt.TFile.Open('foo.root','recreate')
    def cloneTree(point):
        newTree = oldTree.CloneTree(0)
        newTree.SetName('%s_%s' % (oldTree.GetName(),decoratePoint(point)) )
        newTree.Branch('PROC',rt.AddressOf(extras,'PROC'),'PROC/I')
        newTree.Branch('LEP_W',rt.AddressOf(extras,'LEP_W'),'LEP_W/D')
        newTree.Branch('W_EFF',rt.AddressOf(extras,'W_EFF'),'W_EFF/D')
        newTree.Branch('W',rt.AddressOf(extras,'W'),'W/D')
        newTree.Branch('BOX_NUM',rt.AddressOf(extras,'BOX_NUM'),'BOX_NUM/I')
        return newTree                
        
    import pickle
    norms = pickle.load(file(fileName.replace('.root','.pkl')))
    
    #make one tree per point
    trees = {}
    for point in bins.iterkeys():
        trees[point] = cloneTree(point)

    #fill the trees by point
    #for i in xrange(10000):
    for i in xrange(oldTree.GetEntries()):
        oldTree.GetEntry(i)
        
        point = (oldTree.mStop,oldTree.mLSP)
        total = 1.*norms[point]
        extras.W_EFF = 1.0/total

        #choose between Had and BJet boxes
        if oldTree.nBJet > 0:
            #BJet box
            extras.BOX_NUM = 6
            #take a data/MC scale factor
            extras.LEP_W = getBTagScale(oldTree.maxTCHE_PT)
        elif oldTree.nBJetLoose == 0:
            #Had Box
            extras.BOX_NUM = 5
            #ignore the mis-tag here
            extras.LEP_W = 1.0
        else:
            #this is neither our control channel or the signal box
            continue

        #the number of generator level taus
        nGenTau = int(str(oldTree.genInfo)[-1])
        if nGenTau > 0:
            #the data/MC scale factor for the tau ID
            extras.LEP_W *= (1/0.979) #i.e. tau events are more likely to pass the selection in data than in MC as the veto is worse
            
        trees[point].Fill()

    #save all of the trees in one file
    for tree in trees.itervalues():
        tree.Write()
    outputFile.Close()
    print bins
