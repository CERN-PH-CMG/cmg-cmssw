from CMGTools.TTHAnalysis.treeReAnalyzer import *

class VertexWeightFriend:
    def __init__(self,myfile,targetfile,myhist="pileup",targethist="pileup",name="vtxWeight",verbose=False):
        self.name = name
        self.verbose = verbose
        self.myvals = self.load(myfile,myhist)
        self.targetvals = self.load(targetfile,targethist)
        def w2(t,m):
            if t == 0: return (0 if m else 1)
            return (t/m if m else 1)
        self.weights = [ w2(t,m) for (m,t) in zip(self.myvals,self.targetvals) ]
        if self.verbose:
            print "Raw weights for vertex multiplicity up to %d; max %.3f, min %.3f, avg %.3f" % (
                len(self.weights), max(self.weights), min(self.weights), sum(self.weights)/len(self.weights) )
            print self.weights
        self.fixLargeWeights()
        if self.verbose:
            print "Initialized weights for vertex multiplicity up to %d; max %.3f, min %.3f, avg %.3f" % (
                len(self.weights), max(self.weights), min(self.weights), sum(self.weights)/len(self.weights) )
            print self.weights
    def fixLargeWeights(self,maxshift=0.0025,hardmax=3):
        def checkIntegral(weights):
            myint  = sum(a*b for (a,b) in zip(weights,     self.myvals)) 
            refint = sum(a*b for (a,b) in zip(self.weights,self.myvals)) 
            return (myint-refint)/refint
        maxw = min(max(self.weights),5)
        while maxw > hardmax:
            cropped = [ min(maxw,w) for w in self.weights ]
            check = checkIntegral(cropped)
            if self.verbose:
                print "For maximum weight %.3f: integral match: %.5f" % (maxw, check)
            if abs(check) > maxshift:
                break
            maxw *= 0.9
        maxw /= 0.9
        cropped = [ min(maxw,w) for w in self.weights ]
        normshift = checkIntegral(cropped)
        recalibrated = [ c*(1-normshift) for c in cropped ]
        if self.verbose:
            print "Cropped weights up to maximum %d. Normalization shift %.5f, corrected overall to %g" % (maxw,normshift,checkIntegral(recalibrated))
        self.weights = recalibrated
    def load(self,filename,hname,norm=True):
        tf = ROOT.TFile.Open(filename)
        hist = tf.Get(hname)
        vals = [ hist.GetBinContent(i) for i in xrange(1,hist.GetNbinsX()+1) ]
        tf.Close()
        if norm: 
            scale = 1.0/sum(vals)
            vals = [ v*scale for v in vals ]
        return vals
    def listBranches(self):
        return [ (self.name,'F') ]
    def __call__(self,event):
        nvtx = int(event.nVert)
        weight = self.weights[nvtx] if nvtx <= len(self.weights) else 1
        return { self.name: weight }

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    tree.vectorTree = True
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.mc  = VertexWeightFriend(myfile=argv[2],targetfile=argv[2],myhist="nvtx_background",targethist="nvtx_data",verbose=True)
        def analyze(self,ev):
            ret = self.mc(ev)
            print ev.nVert, ret.values()[0]
    test = Tester("tester")              
    el = EventLoop([ test ])
    el.loop([tree], maxEvents = 100000 if len(argv) < 4 else int(argv[3]))
