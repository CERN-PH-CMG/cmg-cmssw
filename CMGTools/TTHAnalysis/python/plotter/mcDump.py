#!/usr/bin/env python
#from tree2yield import *
from CMGTools.TTHAnalysis.plotter.tree2yield import *
from CMGTools.TTHAnalysis.plotter.projections import *
from CMGTools.TTHAnalysis.plotter.mcAnalysis import *
from CMGTools.TTHAnalysis.treeReAnalyzer import *
import string


class MCDumpEvent:
    def __init__(self):
        pass
    def beginComponent(self,tty):
        self._tty = tty
        self._exprMaps =  {}
    def update(self,event):
        self.event = event
    def __getitem__(self, attr):
        if attr not in self._exprMaps:
            expr = self._tty.adaptExpr(attr,cut=False)
            if self._tty._options.doS2V:
                expr = scalarToVector(expr)
            self._exprMaps[attr] = expr 
        return self.event.eval(self._exprMaps[attr])

class MCDumpModule(Module):
    def __init__(self,name,fmt,options=None,booker=None):
        Module.__init__(self,name,booker)
        self.fmt = fmt
        self.options = options
        self.mcde = MCDumpEvent()
        self.passing_entries = 0
        self.dumpFile = None
    def __del__(self):
        if self.dumpFile: self.dumpFile.close()
    def beginComponent(self,tty):
        self.mcde.beginComponent(tty)
    def openOutFile(self,dumpFileName):
        if self.dumpFile: raise RuntimeError,'Output file already open'
        self.dumpFile = open(dumpFileName,'w')
    def getPassingEntries(self):
        return self.passing_entries
    def analyze(self,ev):
        self.mcde.update(ev)
        out=string.Formatter().vformat(self.fmt.replace("\\t","\t"),[],self.mcde)
        if len(out)>0:
            self.passing_entries += 1
            if self.dumpFile: self.dumpFile.write(out+'\n')
        return True
    

def addMCDumpOptions(parser):
    addMCAnalysisOptions(parser)
    parser.add_option("-n", "--maxEvents",  dest="maxEvents", default=-1, type="int", help="Max events")
    parser.add_option("--dumpFile",  dest="dumpFile", default=None, type="string", help="Dump file name")

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] mca.txt cuts.txt 'format string' ")
    addMCDumpOptions(parser)
    (options, args) = parser.parse_args()
    mca = MCAnalysis(args[0],options)
    cut = CutsFile(args[1],options).allCuts()
    mcdm = MCDumpModule("dump",args[2],options)
    if options.dumpFile: mcdm.openOutFile(options.dumpFile)
    el = EventLoop([mcdm])
    mca.processEvents(EventLoop([mcdm]), cut=cut)
    print 'Passing entries:',mcdm.getPassingEntries()
