#!/usr/bin/env python
#from tree2yield import *
from CMGTools.TTHAnalysis.plotter.tree2yield import *
import pickle, re

## These must be defined as standalone functions, to allow runing them in parallel
def _runYields(args):
    key,tty,cuts,noEntryLine = args
    return (key, tty.getYields(cuts,noEntryLine=noEntryLine))

def _runPlot(args):
    key,tty,plotspec,cut = args
    #timer = ROOT.TStopwatch()
    #print "Starting plot %s for %s, %s" % (plotspec.name,key,tty._cname)
    ret = (key,tty.getPlot(plotspec,cut))
    #print "Done plot %s for %s, %s in %s s" % (plotspec.name,key,tty._cname,timer.RealTime())
    return ret

class MCAnalysis:
    def __init__(self,samples,options):
        self._options = options
        self._allData     = {}
        self._data        = []
        self._signals     = []
        self._backgrounds = [] 
        self._isSignal    = {}
        self._rank        = {} ## keep ranks as in the input text file
        for line in open(samples,'r'):
            if re.match("\s*#.*", line): continue
            line = re.sub(r"(?<!\\)#.*","",line)  ## regexp black magic: match a # only if not preceded by a \!
            line = line.replace(r"\#","#")        ## and now we just unescape the remaining #'s
            extra = {}
            if ";" in line:
                (line,more) = line.split(";")[:2]
                for setting in [f.replace(';',',').strip() for f in more.replace('\\,',';').split(',')]:
                    if "=" in setting: 
                        (key,val) = [f.strip() for f in setting.split("=")]
                        extra[key] = eval(val)
                    else: extra[setting] = True
            field = [f.strip() for f in line.split(':')]
            if len(field) <= 1: continue
            signal = False
            if field[0][-1] == "+": 
                signal = True
                field[0] = field[0][:-1]
            ## If we have a selection of process names, apply it
            skipMe = (len(options.processes) > 0)
            for p0 in options.processes:
                for p in p0.split(","):
                    if re.match(p+"$", field[0]): skipMe = False
            for p0 in options.processesToExclude:
                for p in p0.split(","):
                    if re.match(p+"$", field[0]): skipMe = True
            for p0 in options.filesToExclude:
                for p in p0.split(","):
                    if re.match(p+"$", field[1]): skipMe = True
            if skipMe: continue
            #endif
            ## If we have a user-defined list of processes as signal
            if len(options.processesAsSignal):
                signal = False
                for p0 in options.processesAsSignal:
                    for p in p0.split(","):
                        if re.match(p+"$", field[0]): signal = True
            ## endif
            rootfile = options.path+"/%s/ttHLepTreeProducerBase/ttHLepTreeProducerBase_tree.root" % field[1].strip()
            if options.remotePath:
                rootfile = options.remotePath+"/%s/ttHLepTreeProducerBase/ttHLepTreeProducerBase_tree.root" % field[1].strip()
            elif os.path.exists(rootfile+".url"): #(not os.path.exists(rootfile)) and :
                rootfile = open(rootfile+".url","r").readline().strip()
            pckfile = options.path+"/%s/skimAnalyzerCount/SkimReport.pck" % field[1].strip()
            tty = TreeToYield(rootfile, options, settings=extra, name=field[0], cname=field[1].strip())
            if signal: 
                self._signals.append(tty)
                self._isSignal[field[0]] = True
            elif field[0] == "data":
                self._data.append(tty)
            else:
                self._isSignal[field[0]] = False
                self._backgrounds.append(tty)
            if field[0] in self._allData: self._allData[field[0]].append(tty)
            else                        : self._allData[field[0]] =     [tty]
            if field[0] != "data":
                pckobj  = pickle.load(open(pckfile,'r'))
                nevt = dict(pckobj)['All Events']
                scale = "%s/%g" % (field[2], 0.001*nevt)
                if len(field) == 4: scale += "*("+field[3]+")"
                tty.setScaleFactor(scale)
            if field[0] not in self._rank: self._rank[field[0]] = len(self._rank)
        #if len(self._signals) == 0: raise RuntimeError, "No signals!"
        #if len(self._backgrounds) == 0: raise RuntimeError, "No backgrounds!"
    def listProcesses(self):
        ret = self._allData.keys()[:]
        ret.sort(key = lambda n : self._rank[n], reverse = True)
        return ret
    def isBackground(self,process):
        return process != 'data' and not self._isSignal[process]
    def isSignal(self,process):
        return self._isSignal[process]
    def listSignals(self):
        ret = [ p for p in self._allData.keys() if p != 'data' and self._isSignal[p] ]
        ret.sort(key = lambda n : self._rank[n], reverse = True)
        return ret
    def listBackgrounds(self):
        ret = [ p for p in self._allData.keys() if p != 'data' and not self._isSignal[p] ]
        ret.sort(key = lambda n : self._rank[n], reverse = True)
        return ret
    def hasProcess(self,process):
        return process in self._allData
    def scaleProcess(self,process,scaleFactor):
        for tty in self._allData[process]: tty.setScaleFactor(scaleFactor)
    def scaleUpProcess(self,process,scaleFactor):
        for tty in self._allData[process]: 
            tty.setScaleFactor( "((%s) * (%s))" % (tty.getScaleFactor(),scaleFactor) )
    def getProcessOption(self,process,name,default=None):
        return self._allData[process][0].getOption(name,default=default)
    def getScales(self,process):
        return [ tty.getScaleFactor() for tty in self._allData[process] ] 
    def getYields(self,cuts,process=None,nodata=False,makeSummary=False,noEntryLine=False):
        ## first figure out what we want to do
        tasks = []
        for key,ttys in self._allData.iteritems():
            if key == 'data' and nodata: continue
            if process != None and key != process: continue
            for tty in ttys:
                tasks.append((key,tty,cuts,noEntryLine))
        ## then do the work
        retlist = []
        if self._options.jobs == 0: 
            retlist = map(_runYields, tasks)
        else:
            #from sys import stderr
            #stderr.write("Will run %d tasks on %d multiple treads\n" % (len(tasks),self._options.jobs))
            from multiprocessing import Pool
            pool = Pool(self._options.jobs)
            retlist  = pool.map(_runYields, tasks)
        ## then gather results with the same process
        mergemap = {}
        for (k,v) in retlist: 
            if k not in mergemap: mergemap[k] = []
            mergemap[k].append(v)
        ## and finally merge them
        ret = dict([ (k,mergeReports(v)) for k,v in mergemap.iteritems() ])
        if makeSummary:
            allSig = []; allBg = []
            for (key,val) in ret.iteritems():
                if key != 'data':
                    if self._isSignal[key]: allSig.append(ret[key])
                    else: allBg.append(ret[key])
            if self._signals and not ret.has_key('signal') and len(allSig) > 0:
                ret['signal'] = mergeReports(allSig)
            if self._backgrounds and not ret.has_key('background') and len(allBg) > 0:
                ret['background'] = mergeReports(allBg)
        return ret
    def getPlotsRaw(self,name,expr,bins,cut,process=None,nodata=False,makeSummary=False):
        return self.getPlots(PlotSpec(name,expr,bins,{}),cut,process,nodata,makeSummary)
    def getPlots(self,plotspec,cut,process=None,nodata=False,makeSummary=False):
        ret = { }
        allSig = []; allBg = []
        tasks = []
        for key,ttys in self._allData.iteritems():
            if key == 'data' and nodata: continue
            if process != None and key != process: continue
            for tty in ttys:
                tasks.append((key,tty,plotspec,cut))
        retlist = []
        if self._options.jobs == 0: 
            retlist = map(_runPlot, tasks)
        else:
            #from sys import stderr
            #stderr.write("Will run %d tasks on %d multiple treads\n" % (len(tasks),self._options.jobs))
            from multiprocessing import Pool
            pool = Pool(self._options.jobs)
            retlist  = pool.map(_runPlot, tasks)
        ## then gather results with the same process
        mergemap = {}
        for (k,v) in retlist: 
            if k not in mergemap: mergemap[k] = []
            mergemap[k].append(v)
        ## and finally merge them
        ret = dict([ (k,mergePlots(plotspec.name+"_"+k,v)) for k,v in mergemap.iteritems() ])
        if makeSummary:
            allSig = [v for k,v in ret.iteritems() if k != 'data'and self._isSignal[k] == True  ]
            allBg  = [v for k,v in ret.iteritems() if k != 'data'and self._isSignal[k] == False ]
            if self._signals and not ret.has_key('signal') and len(allSig) > 0:
                ret['signal'] = mergePlots(plotspec.name+"_signal", allSig)
                ret['signal'].summary = True
            if self._backgrounds and not ret.has_key('background') and len(allBg) > 0:
                ret['background'] = mergePlots(plotspec.name+"_background",allBg)
                ret['background'].summary = True
        return ret
    def prettyPrint(self,reports,makeSummary=True):
        allSig = []; allBg = []
        for key in reports:
            if key != 'data':
                if self._isSignal[key]: allSig.append((key,reports[key]))
                else: allBg.append((key,reports[key]))
        allSig.sort(key = lambda (n,v): self._rank[n], reverse = True)
        allBg.sort( key = lambda (n,v): self._rank[n], reverse = True)
        table = allSig + allBg
        if makeSummary:
            if len(allSig)>1:
                table.append(('ALL SIG',mergeReports([v for n,v in allSig])))
            if len(allBg)>1:
                table.append(('ALL BKG',mergeReports([v for n,v in allBg])))
        if "data" in reports: table += [ ('DATA', reports['data']) ]

        # maximum length of the cut descriptions
        clen = max([len(cut) for cut,yields in table[0][1]]) + 3
        cfmt = "%%-%ds" % clen;

        fmtlen = 10
        nfmtL = "  %8d"
        nfmtS = "  %8.2f" if self._options.weight else nfmtL

        if self._options.errors:
            nfmtS+=u"%7.2f"
            nfmtL+=u"%7.2f"
            fmtlen+=8
        if self._options.fractions:
            nfmtS+="%7.1f%%"
            nfmtL+="%7.1f%%"
            fmtlen+=8

        print "CUT".center(clen),
        for h,r in table: print ("   "+h).center(fmtlen),
        print ""
        print "-"*((fmtlen+1)*len(table)+clen)
        for i,(cut,dummy) in enumerate(table[0][1]):
            print cfmt % cut,
            for name,report in table:
                (nev,err) = report[i][1]
                den = report[i-1][1][0] if i>0 else 0
                fraction = nev/float(den) if den > 0 else 1
                if self._options.nMinusOne: 
                    fraction = report[-1][1][0]/nev if nev > 0 else 1
                toPrint = (nev,)
                if self._options.errors:    toPrint+=(err,)
                if self._options.fractions: toPrint+=(fraction*100,)
                if self._options.weight and nev < 1000: print nfmtS % toPrint,
                else                                  : print nfmtL % toPrint,
            print ""
    def _getYields(self,ttylist,cuts):
        return mergeReports([tty.getYields(cuts) for tty in ttylist])
    def __str__(self):
        mystr = ""
        for a in self._allData:
            mystr += str(a) + '\n' 
        for a in self._data:
            mystr += str(a) + '\n' 
        for a in self._signals:
            mystr += str(a) + '\n' 
        for a in self._backgrounds:
            mystr += str(a) + '\n'
        return mystr[:-1]

def addMCAnalysisOptions(parser,addTreeToYieldOnesToo=True):
    if addTreeToYieldOnesToo: addTreeToYieldOptions(parser)
    parser.add_option("-j", "--jobs",           dest="jobs", type="int", default=0, help="Use N threads");
    parser.add_option("-P", "--path",           dest="path",        type="string", default="./",      help="path to directory with input trees and pickle files (./)") 
    parser.add_option("--RP", "--remote-path",   dest="remotePath",  type="string", default=None,      help="path to remote directory with trees, but not other metadata (default: same as path)") 
    parser.add_option("-p", "--process", dest="processes", type="string", default=[], action="append", help="Processes to print (comma-separated list of regexp, can specify multiple ones)");
    parser.add_option("--xf", "--exclude-files", dest="filesToExclude", type="string", default=[], action="append", help="Files to exclude (comma-separated list of regexp, can specify multiple ones)");
    parser.add_option("--xp", "--exclude-process", dest="processesToExclude", type="string", default=[], action="append", help="Processes to exclude (comma-separated list of regexp, can specify multiple ones)");
    parser.add_option("--sp", "--signal-process", dest="processesAsSignal", type="string", default=[], action="append", help="Processes to set as signal (overriding the '+' in the text file)");

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] tree.root cuts.txt")
    addMCAnalysisOptions(parser)
    (options, args) = parser.parse_args()
    tty = TreeToYield(args[0],options) if ".root" in args[0] else MCAnalysis(args[0],options)
    cf  = CutsFile(args[1],options)
    for cutFile in args[2:]:
        temp = CutsFile(cutFile,options)
        for cut in temp.cuts():
            cf.add(cut[0],cut[1])
    report = tty.getYields(cf)#, process=options.process)
    tty.prettyPrint(report)
