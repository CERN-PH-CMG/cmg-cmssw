#!/usr/bin/env python
#from mcPlots import *
from CMGTools.TTHAnalysis.plotter.mcPlots import *


class Param:
    def __init__(self,name,value):
        self.name  = name
        self.value = value
    def getPoints(self):
        return [self.value]

class DiscreteParam(Param):
    def __init__(self,name,value,values):
        Param.__init__(self,name,value)
        self.values = values
    def getPoints(self):
        return self.values

class ParamSampler:
    def __init__(self,params):
        self._params = params
    def _addAllPoints(self, param, points):
        ## trivial case
        if len(points) == 0:
            return [ [ (param.name,p) ] for p in param.getPoints() ]
        ## generic case, recursive
        ret = []
        for p in param.getPoints():
            for rest in points:
                pfull = [ (param.name,p) ] + rest
                ret.append(pfull)
        return ret
    def getAllPoints(self):
        allpoints = []
        for p in self._params:
            allpoints = self._addAllPoints(p, allpoints)
        return [ dict(x) for x in allpoints ]
    def optimize(self,mca,cut,fom,algo):
        if algo == "scan": return self.scan(mca,cut,fom,algo)
    def scan(self,mca,cut,fom,algo):
        best, bval = None, None
        allpoints = self.getAllPoints()
        print "I have %d points to test... please sit back and relax." % len(allpoints)
        for i,point in enumerate(allpoints):
            print "trying point ",i,": ",point
            mycut = CutsFile([('all',cut.format(**point))])   ## need the '**' to unpack a map
            yields = mca.getYields(mycut,nodata=True,noEntryLine=True)
            fomval = fom(yields)
            print "     ---> ",fomval
            if best == None or bval < fomval:
                best = point
                bval = fomval
        return (best,bval)

class ParamFile:
    def __init__(self,params,options):
        self._params = []
        for line in open(params,'r'):
            if re.match("\s*#.*", line): continue
            line = re.sub("#.*","",line)
            extra = {}
            if ";" in line:
                (line,more) = line.split(";")[:2]
                for setting in [f.strip() for f in more.split(',')]:
                    if "=" in setting: 
                        (key,val) = [f.strip() for f in setting.split("=")]
                        extra[key] = eval(val)
                    else: extra[setting] = True
            field = [f.strip() for f in line.split(':')]
            if len(field) < 2: continue
            if len(field) == 2:
                self._params.append( Param(field[0], float(field[1])) )
            else:
                self._params.append( DiscreteParam(field[0], float(field[1]), [float(x) for x in field[2].split(",")]) )
    def allParams(self):
        return self._params[:]

class SimpleFOM:
    def __init__(self,mca,syst=0.0,includeS=False):
        self._signals     = mca.listSignals()
        self._backgrounds = [ (b,syst) for b in mca.listBackgrounds() ]
        self._includeS = includeS
    def __call__(self,report):
        tots, totb, totbsyst = 0.,0.,0.
        for s in  self._signals:
            if s not in report: continue
            tots += report[s][-1][1][0]
        for b,syst in self._backgrounds:
            if b not in report: continue
            totb     += report[b][-1][1][0]
            totbsyst += (syst*report[b][-1][1][0])**2
        if self._includeS: totb += tots
        return tots/sqrt(totb + totbsyst**2)


if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] mc.txt cuts.txt algo params.txt")
    addMCAnalysisOptions(parser)
    (options, args) = parser.parse_args()
    mca  = MCAnalysis(args[0],options)
    cuts = CutsFile(args[1],options)
    fom = SimpleFOM(mca)
    algo = args[2]
    params = ParamFile(args[3],options)
    sampler = ParamSampler(params.allParams())
    (point,val) = sampler.optimize(mca,cuts.allCuts(),fom,algo)
    print "The best set of parameters is "
    for k,v in point.iteritems(): print "   %-20s: %8.3f" % (k,v)
    print "The corresponding figure of merit is ",val
    cuts.setParams(point)
    report = mca.getYields(cuts,nodata=True)
    mca.prettyPrint(report)
