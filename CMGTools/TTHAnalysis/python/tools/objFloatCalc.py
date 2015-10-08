from CMGTools.TTHAnalysis.treeReAnalyzer import *

class ObjFloatCalc:
    def __init__(self,label,coll,newvars,sizelimit=10):
        self.label = "" if (label in ["",None]) else (label)
        self.coll = coll
        self.newvars = newvars
        self.sizelimit = sizelimit
    def listBranches(self):
        biglist = [ ("n"+self.coll,"I") ]
        biglist.extend([(self.coll+"_"+newvar,"F",100,"n"+self.coll) for newvar in self.newvars.keys()])
        return biglist
    def __call__(self,event):
        try :
            assert (getattr(event,"n"+self.coll) <= self.sizelimit)
        except AssertionError:
            print 'ERROR in ObjFloatCalc: branch size limit is '+str(self.sizelimit)+' while n'+self.coll+'=='+str(getattr(event,"n"+self.coll))
            raise
        objs = [l for l in Collection(event,self.coll,"n"+self.coll)]
        ret = {"n"+self.coll : getattr(event,"n"+self.coll) }
        for newvar in self.newvars.keys():
            ret[self.coll+"_"+newvar]=[-999] * getattr(event,"n"+self.coll)
            for i,ob in enumerate(objs):
                ret[self.coll+"_"+newvar][i] = self.newvars[newvar](ob)
        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    tree.vectorTree = True
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf1 = ObjFloatCalc("LepPtPlusCalibration","LepGood",{"PtPlus1": (lambda lep : lep.pt*1.01), "PtPlus2": (lambda lep : lep.pt*1.02)})
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf1(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

        
