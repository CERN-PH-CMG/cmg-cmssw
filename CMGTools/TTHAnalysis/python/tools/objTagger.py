from CMGTools.TTHAnalysis.treeReAnalyzer import *

class ObjTagger:
    def __init__(self,label,coll,sel,sizelimit=10):
        self.label = "" if (label in ["",None]) else (label)
        self.coll = coll
        self.sel = sel
        self.sizelimit = sizelimit
    def listBranches(self):
        biglist = [ ("n"+self.coll,"I"), ("n"+self.coll+"_"+self.label, "I"), (self.coll+"_is"+self.label,"I",100,"n"+self.coll) ]
        return biglist
    def __call__(self,event):
        try :
            assert (getattr(event,"n"+self.coll) <= self.sizelimit)
        except AssertionError:
            print 'ERROR in ObjTagger: branch size limit is '+str(self.sizelimit)+' while n'+self.coll+'=='+str(getattr(event,"n"+self.coll))
            raise
        objs = [l for l in Collection(event,self.coll,"n"+self.coll)]
        ret = {"n"+self.coll : getattr(event,"n"+self.coll) }
        ret["n"+self.coll+"_"+self.label]=0
        ret[self.coll+"_is"+self.label]=[0] * getattr(event,"n"+self.coll)
        for i,ob in enumerate(objs):
            ispassing = True
            for selector in self.sel:
                if not selector(ob):
                    ispassing = False
                    break
            if ispassing:
                ret["n"+self.coll+"_"+self.label] += 1
                ret[self.coll+"_is"+self.label][i] = 1
        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    tree.vectorTree = True
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf1 = ObjTagger("LepPt25","LepGood",[lambda ob : ob.pt >= 25])
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf1(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

        
