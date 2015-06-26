from CMGTools.TTHAnalysis.treeReAnalyzer import *
from ROOT import TLorentzVector, TVector2
import ROOT
import itertools

def deltaR(eta1,eta2,phi1,phi2):
    deta = eta1-eta2
    dphi = ROOT.TVector2.Phi_mpi_pi(phi1-phi2);
    return sqrt( deta*deta+dphi*dphi )

RMinCut = 0.85 * 80.385/173.5
RMaxCut = 1.25 * 80.385/173.5

#(mW = 80.385 GeV & mtop = 173.5 GeV)


def collectFinalTopPermutations(jetidxlist, jets, requireBTagforCombo = False, coneSizeFinal = 1.5, doIteration = True):
#    print "jetidxlist", jetidxlist
    allcomblists = []
    for i, ijetidx in enumerate(jetidxlist):
#        print ijetidx, ": "
        closeByJets = [i] #also put the seed jet into the combinations...
        for jjetidx in jetidxlist[i+1:]: #loop over all following (i+1->end) jets
#            print jjetidx,
            deltaR = jets[ijetidx].p4().DeltaR(jets[jjetidx].p4())
            if deltaR<(2*coneSizeFinal):
                closeByJets.append(jjetidx)
#        print closeByJets
        combList = []
        if(len(closeByJets)>2):
            combList = list(itertools.combinations(closeByJets, 3)) # get all combinations with three jets
#        print combList
        allcomblists = allcomblists+combList
    cleanedallcomblists = set(allcomblists)
    
    nTriplets = len(allcomblists)
    nCleanedTriplets = len(cleanedallcomblists)
    
    topPermutations = []
    for comb in cleanedallcomblists:
        topPermutations.append(TopPermutation(comb[0],comb[1],comb[2],jets, coneSizeFinal))
    successfulTopPermutations = []
    if requireBTagforCombo:
        successfulTopPermutations = [value for value in topPermutations if value.passesTopWBTagCuts]
    else:
        successfulTopPermutations = [value for value in topPermutations if value.passesTopWCuts]

    sortedsuccessfulTopPermutations = sorted(successfulTopPermutations, key=lambda TopPermutation: TopPermutation.distanceToMTop)

    if doIteration:
        returnIteratedPerms = []
        if(len(sortedsuccessfulTopPermutations)>0):
            bestPerm = sortedsuccessfulTopPermutations[0]
            returnIteratedPerms.append(bestPerm)                                           #add best permutation
            cleanedJetIdxList = [jetidx for jetidx in jetidxlist if jetidx not in bestPerm.ids]
            #        print "about to iterate", cleanedJetIdxList
            returnIteratedPerms += (collectFinalTopPermutations(cleanedJetIdxList, jets))      #add more permutations iteratively, working on cleaned jetidxlist
        return returnIteratedPerms
    else:
        return sortedsuccessfulTopPermutations

class TopPermutation:
    def __init__(self, idxJ1, idxJ2, idxJ3, jets, coneSize):
        self.ids = (idxJ1,idxJ2,idxJ3)
        self.ConeCompatible, self.passesWCuts, self.passesTopWCuts, self.passesWBTagCuts, self.passesTopWBTagCuts = False, False, False, False, False
        self.checkConeCompatibility(jets, coneSize)
        if self.ConeCompatible:
            self.findBestBTags(jets)
            self.calculateMasses(jets)
    def __repr__(self):
        return repr((self.ids[0], self.ids[1], self.ids[2]))
    def checkConeCompatibility(self, jets, coneSize):
        etaSum = 0
        for id in self.ids: etaSum += jets[id].eta
        etaAve = etaSum/3
        
        phi1 = jets[self.ids[0]].phi
        phi2 = jets[self.ids[1]].phi
        phi3 = jets[self.ids[2]].phi
        
        phiAve = ROOT.TVector2.Phi_mpi_pi(phi1 + ROOT.TVector2.Phi_mpi_pi(phi2-phi1)/3 + ROOT.TVector2.Phi_mpi_pi(phi3-phi1)/3)
        
        self.ConeCompatible = True
        for id in self.ids:
            if deltaR(etaAve, jets[id].eta, phiAve, jets[id].phi)>coneSize: self.ConeCompatible = False



    def calculateMasses(self, jets):
        jet1 = jets[self.ids[0]].p4()
        jet2 = jets[self.ids[1]].p4()
        jet3 = jets[self.ids[2]].p4()
        self.m12   = (jet1+jet2).M()
        self.m23   = (jet2+jet3).M()
        self.m13   = (jet1+jet3).M()
        self.m123  = (jet1+jet2+jet3).M()
        self.pt123  = (jet1+jet2+jet3).Pt()


        passesm23 = True if RMinCut < self.m23/self.m123 < RMaxCut else False
        passesm12 = True if RMinCut < self.m12/self.m123 < RMaxCut and self.m23/self.m123>0.35 else False
        passesm13 = True if RMinCut < self.m13/self.m123 < RMaxCut and self.m23/self.m123>0.35 else False

        #do unambiguous decision about kinematic b candidate in top (if there are two possibilities, take the higher CMVA-value)
        if passesm23 and not passesm12 and not passesm13:
            self.kinBCandidateidx = self.ids[0]
        elif passesm12 and not passesm23 and not passesm13:
            self.kinBCandidateidx = self.ids[2]
        elif passesm13 and not passesm23 and not passesm12:
            self.kinBCandidateidx = self.ids[1]
        elif passesm23 and passesm12 and not passesm13:
            self.kinBCandidateidx = self.ids[0] if jets[self.ids[0]].btagCMVA > jets[self.ids[2]].btagCMVA else self.ids[2]
        elif passesm23 and passesm13 and not passesm12:
            self.kinBCandidateidx = self.ids[0] if jets[self.ids[0]].btagCMVA > jets[self.ids[1]].btagCMVA else self.ids[1]
        elif passesm12 and passesm13 and not passesm23:
            self.kinBCandidateidx = self.ids[2] if jets[self.ids[2]].btagCMVA > jets[self.ids[1]].btagCMVA else self.ids[1]
        else:
            self.kinBCandidateidx = -999

        self.passesWCuts = True if passesm23 or passesm12 or passesm13 else False
        self.passesTopWCuts = True if self.passesWCuts and 80. < self.m123 < 270. else False
        
        sum = ROOT.TLorentzVector(0,0,0,0)
        for idx in self.ids:
            sum+= jets[idx].p4()
#        print "compare m123: ", self.m123, sum.M()
        self.m123 = sum.M()
                
        idsWithoutBestBTag = self.ids[:self.sortIdsByBTag[0]] + self.ids[(self.sortIdsByBTag[0] + 1):] #copy of self.ids, leaving out element with best btag
        sum.SetPxPyPzE(0,0,0,0)
        for idx in idsWithoutBestBTag:
            sum+= jets[idx].p4()
        self.mNonB = sum.M()

        rejectQCD = 999
        if self.sortIdsByBTag[0] != 0:
            sum  = jets[self.ids[self.sortIdsByBTag[0]]].p4()
            sum += jets[idsWithoutBestBTag[1]].p4() # pair b-jet with lower pt jet (of the two W jet cands) to suppress bkg
            rejectQCD = sum.M()/self.m123
        
        self.passesWBTagCuts = True if rejectQCD > 0.35 and RMinCut < self.mNonB/self.m123 < RMaxCut and jets[self.ids[self.sortIdsByBTag[0]]].btagCMVA>0.732 else False
        self.passesTopWBTagCuts = True if rejectQCD > 0.35 and RMinCut < self.mNonB/self.m123 < RMaxCut and 80. < self.m123 < 270. and jets[self.ids[self.sortIdsByBTag[0]]].btagCMVA>0.732 else False

        self.distanceToMTop = abs(self.m123-173.5)

    def findBestBTags(self, jets):
        iBTagDict = {i: jets[idx].btagCMVA for i, idx in enumerate(self.ids)}
        self.sortIdsByBTag = sorted(iBTagDict, key=iBTagDict.get, reverse=True)


class resolvedTopTagVars1l:
    def __init__(self):
        self.branches = [("nTopTags","I"),("nTopBTags","I"),("nTopTagCandsNoIter","I"),
                         ("bestM123TopTag","F",10,"nTopTags"),("bestpt123TopTag","F",10,"nTopTags"),
#                         ("nTopTagsR2","I"),
#                         ("bestM123TopTagR2","F",10,"nTopTagsR2"),("bestpt123TopTagR2","F",10,"nTopTagsR2"),
                         ("bestM123BTagTopTag","F",10,"nTopBTags"),("bestpt123BTagTopTag","F",10,"nTopBTags"),
                         ("betterQGLTopTagidx","F",10,"nTopTags"), ("worseQGLTopTagidx","F",10,"nTopTags"),
                         ("bestBTagTopTagidx","F",10,"nTopTags"),("kinBCandidateidx","F",10,"nTopTags")
                         ]#, (jetptclone,"f",)]
    
    def listBranches(self):
        return self.branches[:]
    
    def __call__(self,event,keyvals):

        jets = [j for j in Collection(event,"Jet","nJet")]
        #WARNING: This will actually read in all jets, not just the ones passing the 1l selection criteria (currently pt>30 GeV); should revise this
        njet = len(jets)



        finalTopPermutations = []
        finalTopBTagPermutations = []
    
        jetlist = [i for i, jet in enumerate(jets)]

        finalTopPermutations = collectFinalTopPermutations(jetlist,jets)
#        finalTopR2Permutations = collectFinalTopPermutations(jetlist,jets, False, 2)
        finalTopBTagPermutations = collectFinalTopPermutations(jetlist,jets, True)
        noIterationCollectAllTopPermutations = collectFinalTopPermutations(jetlist,jets, False, 1.5, False)

#        print "strt"
#        print finalTopPermutations
#        print finalTopBTagPermutations
        
        ret = { 'nTopTags'   : len(finalTopPermutations) } #initialize the dictionary with a first entry
#        ret['nTopTagsR2'] = len(finalTopR2Permutations)
        ret['nTopBTags'] = len(finalTopBTagPermutations)
        ret['nTopTagCandsNoIter'] = len(noIterationCollectAllTopPermutations)


        
        ret['bestM123TopTag'] = [perm.m123 for perm in finalTopPermutations]
        ret['bestpt123TopTag'] = [perm.pt123 for perm in finalTopPermutations]
#        ret['bestM123TopTagR2'] = [perm.m123 for perm in finalTopR2Permutations]
    
#        ret['bestpt123TopTagR2'] = [perm.pt123 for perm in finalTopR2Permutations]
        ret['bestM123BTagTopTag'] = [perm.m123 for perm in finalTopBTagPermutations]
        ret['bestpt123BTagTopTag'] = [perm.pt123 for perm in finalTopBTagPermutations]
        
        ret['bestBTagTopTagidx'] = [perm.ids[perm.sortIdsByBTag[0]] for perm in finalTopPermutations]
        ret['kinBCandidateidx']  = [perm.kinBCandidateidx for perm in finalTopPermutations]

        betterQGLidx = []
        worseQGLidx = []
        for perm in finalTopPermutations:
            if jets[perm.ids[perm.sortIdsByBTag[1]]].qgl >= jets[perm.ids[perm.sortIdsByBTag[2]]].qgl:
                betterQGLidx.append(perm.ids[perm.sortIdsByBTag[1]])
                worseQGLidx.append(perm.ids[perm.sortIdsByBTag[2]])
            else:
                betterQGLidx.append(perm.ids[perm.sortIdsByBTag[2]])
                worseQGLidx.append(perm.ids[perm.sortIdsByBTag[1]])

        ret['betterQGLTopTagidx'] = betterQGLidx
        ret['worseQGLTopTagidx'] = worseQGLidx


        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree") 
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = resolvedTopTagVars1l()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

        

