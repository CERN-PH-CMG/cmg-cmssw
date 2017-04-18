#!/usr/bin/env python
from math import *
import re
import os, os.path
from array import array

## safe batch mode
import sys
args = sys.argv[:]
sys.argv = ['-b']
import ROOT
sys.argv = args
ROOT.gROOT.SetBatch(True)

from copy import *

from CMGTools.WMass.plotter.cutsFile import *
from CMGTools.TTHAnalysis.plotter.mcCorrections import *
from CMGTools.TTHAnalysis.plotter.fakeRate import *

if "/functions_cc.so" not in ROOT.gSystem.GetLibraries(): 
    ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/WMass/python/plotter/functions.cc+" % os.environ['CMSSW_BASE']);

def scalarToVector(x):
    x0 = x
    x = re.sub(r"(LepGood|Lep|GammaGood|JetFwd|Jet|JetClean|GenTop|SV)(\d)_(\w+)", lambda m : "%s_%s[%d]" % (m.group(1),m.group(3),int(m.group(2))-1), x)
    x = re.sub(r"\bmet\b", "met_pt", x)
    return x

class PlotSpec:
    def __init__(self,name,expr,bins,opts):
        self.name = name
        self.expr = expr
        self.bins = bins
        self.opts = opts
        self.logs = {}
    def hasOption(self,name):
        return (name in self.opts)
    def getOption(self,name,default=None):
        return self.opts[name] if (name in self.opts) else default
    def setOption(self,name,value):
        self.opts[name] = value
    def setLog(self,name,value):
        self.logs[name] = value
    def allLogs(self):
        return self.logs.iteritems()

def stylePlot(plot,spec,getOption):
        ## Sample specific-options, from self
        if getOption('FillColor',None) != None:
            plot.SetFillColor(getOption('FillColor',0))
            plot.SetFillStyle(getOption('FillStyle',1001))
        else:
            plot.SetFillStyle(0)
            plot.SetLineWidth(getOption('LineWidth',1))
        plot.SetLineColor(getOption('LineColor',1))
        plot.SetLineStyle(getOption('LineStyle',1))
        plot.SetMarkerColor(getOption('MarkerColor',1))
        plot.SetMarkerStyle(getOption('MarkerStyle',20))
        plot.SetMarkerSize(getOption('MarkerSize',1.1))
        ## Plot specific-options, from spec
        if "TH3" not in plot.ClassName():
            plot.GetYaxis().SetTitle(spec.getOption('YTitle',"Events"))
            plot.GetXaxis().SetTitle(spec.getOption('XTitle',spec.name))
            plot.GetXaxis().SetNdivisions(spec.getOption('XNDiv',510))
            plot.GetXaxis().SetMoreLogLabels(True)

def makeHistFromBinsAndSpec(name,expr,bins,plotspec):
        profile1D      = plotspec.getOption('Profile1D',False) if plotspec != None else False
        profile2D      = plotspec.getOption('Profile2D',False) if plotspec != None else False
        nvars = expr.replace("::","--").count(":")+1
        if nvars == 1 or (nvars == 2 and profile1D):
            if bins[0] == "[":
                edges = [ float(f) for f in bins[1:-1].split(",") ]
                if profile1D: 
                    histo = ROOT.TProfile(name,name,len(edges)-1,array('f',edges))
                else:
                    histo = ROOT.TH1D(name,name,len(edges)-1,array('f',edges))
            else:
                (nb,xmin,xmax) = bins.split(",")
                if profile1D:
                    histo = ROOT.TProfile(name,name,int(nb),float(xmin),float(xmax))
                else:
                    histo = ROOT.TH1D(name,name,int(nb),float(xmin),float(xmax))
        elif nvars == 2 or (nvars == 3 and profile2D):
            if bins[0] == "[":
                xbins, ybins = bins.split("*")
                xedges = [ float(f) for f in xbins[1:-1].split(",") ]
                yedges = [ float(f) for f in ybins[1:-1].split(",") ]
                if profile2D:
                    histo = ROOT.TProfile2D(name,name,len(xedges)-1,array('d',xedges),len(yedges)-1,array('d',yedges))
                else:
                    histo = ROOT.TH2D(name,name,len(xedges)-1,array('f',xedges),len(yedges)-1,array('f',yedges))
            else:
                (nbx,xmin,xmax,nby,ymin,ymax) = bins.split(",")
                if profile2D:
                    histo = ROOT.TProfile2D(name,name,int(nbx),float(xmin),float(xmax),int(nby),float(ymin),float(ymax))
                else:
                    histo = ROOT.TH2D(name,name,int(nbx),float(xmin),float(xmax),int(nby),float(ymin),float(ymax))
        elif nvars == 3:
            ez,ey,ex = [ e.replace("--","::") for e in expr.replace("::","--").split(":") ]
            if bins[0] == "[":
                xbins, ybins, zbins = bins.split("*")
                xedges = [ float(f) for f in xbins[1:-1].split(",") ]
                yedges = [ float(f) for f in ybins[1:-1].split(",") ]
                zedges = [ float(f) for f in zbins[1:-1].split(",") ]
                histo = ROOT.TH3D(name,name,len(xedges)-1,array('f',xedges),len(yedges)-1,array('f',yedges),len(zedges)-1,array('f',zedges))
            else:
                (nbx,xmin,xmax,nby,ymin,ymax,nbz,zmin,zmax) = bins.split(",")
                histo = ROOT.TH3D(name,name,int(nbx),float(xmin),float(xmax),int(nby),float(ymin),float(ymax),int(nbz),float(zmin),float(zmax))
            histo.GetXaxis().SetTitle(ex)
            histo.GetYaxis().SetTitle(ey)
            histo.GetZaxis().SetTitle(ez)
        else:
            raise RuntimeError, "Can't make a plot with %d dimensions" % nvars
        histo.Sumw2()
        return histo

def cropNegativeBins(histo):
            if "TH1" in histo.ClassName():
                for b in xrange(0,histo.GetNbinsX()+2):
                    if histo.GetBinContent(b) < 0: histo.SetBinContent(b, 0.0)
            elif "TH2" in histo.ClassName():
                for bx in xrange(0,histo.GetNbinsX()+2):
                    for by in xrange(0,histo.GetNbinsY()+2):
                        if histo.GetBinContent(bx,by) < 0: histo.SetBinContent(bx,by, 0.0)
            elif "TH3" in histo.ClassName():
                for bx in xrange(0,histo.GetNbinsX()+2):
                    for by in xrange(0,histo.GetNbinsY()+2):
                        for bz in xrange(0,histo.GetNbinsZ()+2):
                            if histo.GetBinContent(bx,by,bz) < 0: histo.SetBinContent(bx,by,bz, 0.0)


class TreeToYield:
    def __init__(self,root,options,scaleFactor=1.0,name=None,cname=None,settings={},treename=None):
        self._name  = name  if name != None else root
        self._cname = cname if cname != None else self._name
        self._fname = root
        self._isInit = False
        self._options = options
        self._treename = treename if treename else options.tree
        self._weight  = (options.weight and 'data' not in self._name and '2012' not in self._name and '2011' not in self._name )
        self._isdata = 'data' in self._name
        self._weightString  = options.weightString if not self._isdata else "1"
        self._scaleFactor = scaleFactor
        self._fullYield = 0 # yield of the full sample, as if it passed the full skim and all cuts
        self._fullNevt = 0 # number of events of the full sample, as if it passed the full skim and all cuts
        self._settings = settings
        loadMCCorrections(options)            ## make sure this is loaded
        self._mcCorrs = globalMCCorrections() ##  get defaults
        if 'SkipDefaultMCCorrections' in settings: ## unless requested to 
            self._mcCorrs = []                     ##  skip them
        if self._isdata: 
# bug: does not work
#            self._mcCorrs = [c for c in self._mcCorrs if c.alsoData] ## most don't apply to data, some do 
            newcorrs=[]
            for corr in self._mcCorrs:
                newcorr = copy(corr)
                newlist = copy(newcorr._corrections)
                newlist = [icorr for icorr in newlist if icorr.alsoData]
                newcorr._corrections = newlist
                newcorrs.append(newcorr)
            self._mcCorrs=newcorrs
        if 'MCCorrections' in settings:
            self._mcCorrs = self._mcCorrs[:] # make copy
            for cfile in settings['MCCorrections'].split(','): 
                self._mcCorrs.append( MCCorrections(cfile) )
        if self._mcCorrs and self._scaleFactor and self._scaleFactor != 1.0:
            # apply MC corrections to the scale factor
            self._scaleFactor = self.adaptExpr(self._scaleFactor, cut=True)
        if 'FakeRate' in settings:
            self._FR = FakeRate(settings['FakeRate'],self._options.lumi)
            ## add additional weight correction.
            ## note that the weight receives the other mcCorrections, but not itself
            frweight = self.adaptExpr(self._FR.weight(), cut=True)
            ## modify cuts to get to control region. order is important
            self._mcCorrs = self._mcCorrs + self._FR.cutMods()  + self._FR.mods()
            self._weightString = self.adaptExpr(self._weightString, cut=True) + "* (" + frweight + ")"
            self._weight = True
        else:
            self._weightString = self.adaptExpr(self._weightString, cut=True)
        for macro in self._options.loadMacro:
            libname = macro.replace(".cc","_cc.so").replace(".cxx","_cxx.so")
            if libname not in ROOT.gSystem.GetLibraries():
                ROOT.gROOT.ProcessLine(".L %s+" % macro);
        self._appliedCut = None
        self._elist = None
        self._entries = None
        #print "Done creation  %s for task %s in pid %d " % (self._fname, self._name, os.getpid())
    def setScaleFactor(self,scaleFactor):
        if self._mcCorrs and scaleFactor and scaleFactor != 1.0:
            # apply MC corrections to the scale factor
            self._scaleFactor = self.adaptExpr(scaleFactor, cut=True)
        else:
            self._scaleFactor = scaleFactor
    def getScaleFactor(self):
        return self._scaleFactor
    def setFullYield(self,fullYield):
        self._fullYield = fullYield
    def setFullNevt(self,fullNevt):
        self._fullNevt = fullNevt
    def name(self):
        return self._name
    def cname(self):
        return self._cname
    def hasOption(self,name):
        return (name in self._settings)
    def getOption(self,name,default=None):
        if name in self._settings: return self._settings[name]
        return default
    def setOption(self,name,value):
        self._settings[name] = value
    def adaptDataMCExpr(self,expr):
        ret = expr
        if self._isdata:
            ret = re.sub(r'\$MC\{.*?\}', '', re.sub(r'\$DATA\{(.*?)\}', r'\1', expr));
        else:
            ret = re.sub(r'\$DATA\{.*?\}', '', re.sub(r'\$MC\{(.*?)\}', r'\1', expr));
        return ret
    def adaptExpr(self,expr,cut=False):
        ret = self.adaptDataMCExpr(expr)
        for mcc in self._mcCorrs:
            ret = mcc(ret,self._name,self._cname,cut)
        return ret
    def _init(self):
        if "root://" in self._fname:
            ROOT.gEnv.SetValue("TFile.AsyncReading", 1);
            ROOT.gEnv.SetValue("XNet.Debug", -1); # suppress output about opening connections
            #self._tfile = ROOT.TFile.Open(self._fname+"?readaheadsz=200000") # worse than 65k
            #self._tfile = ROOT.TFile.Open(self._fname+"?readaheadsz=32768") # worse than 65k
            self._tfile = ROOT.TFile.Open(self._fname+"?readaheadsz=65535") # good
            #self._tfile = ROOT.TFile.Open(self._fname+"?readaheadsz=0") #worse than 65k
        else:
            self._tfile = ROOT.TFile.Open(self._fname)
        if not self._tfile: raise RuntimeError, "Cannot open %s\n" % self._fname
        t = self._tfile.Get(self._treename)
        if not t: raise RuntimeError, "Cannot find tree %s in file %s\n" % (self._treename, self._fname)
        self._tree  = t
        #self._tree.SetCacheSize(10*1000*1000)
        if "root://" in self._fname: self._tree.SetCacheSize()
        self._friends = []
        friendOpts = self._options.friendTrees[:]
        friendOpts += [ ('sf/t', d+"/evVarFriend_{cname}.root") for d in self._options.friendTreesSimple]
        friendOpts += (self._options.friendTreesData if self._isdata else self._options.friendTreesMC)
        friendOpts += [ ('sf/t', d+"/evVarFriend_{cname}.root") for d in (self._options.friendTreesDataSimple if self._isdata else self._options.friendTreesMCSimple) ]
        if 'Friends' in self._settings: friendOpts += self._settings['Friends']
        if 'FriendsSimple' in self._settings: friendOpts += [ ('sf/t', d+"/evVarFriend_{cname}.root") for d in self._settings['FriendsSimple'] ]
        for tf_tree,tf_file in friendOpts:
#            print 'Adding friend',tf_tree,tf_file
            tf = self._tree.AddFriend(tf_tree, tf_file.format(name=self._name, cname=self._cname, P=getattr(self._options,'path',''))),
            self._friends.append(tf)
        self._isInit = True
        
    def getTree(self):
        if not self._isInit: self._init()
        return self._tree

    def getEntries(self,useEList=True,closeFileAfterwards=True):
        if useEList and self._elist: 
            return self._elist.GetN()
        if self._entries is None:
            if closeFileAfterwards and (not self._isInit):
                if "root://" in self._fname: ROOT.gEnv.SetValue("XNet.Debug", -1); # suppress output about opening connections
                tfile = ROOT.TFile.Open(self._fname)
                if not tfile: raise RuntimeError, "Cannot open %s\n" % self._fname
                t = tfile.Get(self._treename)
                if not t: raise RuntimeError, "Cannot find tree %s in file %s\n" % (self._treename, self._fname)
                self._entries = t.GetEntries()
            else:
                self._entries = self.getTree().GetEntries()
        return self._entries
    def hasEntries(self,useEList=True):
        if useEList and self._elist:
            return True
        return (self._entries != None)
    def setEntries(self,entries):
        self._entries = entries
    def getYields(self,cuts,noEntryLine=False,fsplit=None):
        if not self._isInit: self._init()
        report = []; cut = ""
        cutseq = [ ['entry point','1'] ]
        if noEntryLine: cutseq = []
        sequential = False
        if self._options.nMinusOne: 
            cutseq = cuts.nMinusOneCuts()
            cutseq += [ ['all',cuts.allCuts()] ]
            sequential = False
        elif self._options.final:
            cutseq = [ ['all', cuts.allCuts()] ]
        else:
            cutseq += cuts.cuts();
            sequential = True
        for cn,cv in cutseq:
            if sequential:
                if cut: cut += " && "
                cut += "(%s)" % cv
            else:
                cut = cv
            report.append((cn,self._getYield(self._tree,cut,fsplit=fsplit)))
        return report
    def prettyPrint(self,report):
        # maximum length of the cut descriptions
        clen = max([len(cut) for cut,yields in report]) + 3
        cfmt = "%%-%ds" % clen;

        fmtlen = 12
        nfmtL = "    %8d"
        nfmtS = "    %8.3f" if self._weight else nfmtL

        if self._options.errors:
            nfmtS+=u"%8.3f"
            nfmtL+=u"%8.3f"
            fmtlen+=8
        if self._options.fractions:
            nfmtS+="%7.1f%%"
            nfmtL+="%7.1f%%"
            fmtlen+=8

        print "cut".center(clen),"yield".center(fmtlen)
        print "-"*((fmtlen+1)+clen)
        for i,(cut,(nev,err)) in enumerate(report):
            print cfmt % cut,
            den = report[i-1][1][0] if i>0 else 0
            fraction = nev/float(den) if den > 0 else 1
            if self._options.nMinusOne: 
                fraction = report[-1][1][0]/nev if nev > 0 else 1
            toPrint = (nev,)
            if self._options.errors:    toPrint+=(err,)
            if self._options.fractions: toPrint+=(fraction*100,)
            if self._weight and nev < 1000: print nfmtS % toPrint,
            else                          : print nfmtL % toPrint,
            print ""
    def _getYield(self,tree,cut,fsplit=None):
        if self._weight:
            if self._isdata: cut = "(%s)     *(%s)*(%s)" % (self._weightString,                    self._scaleFactor, self.adaptExpr(cut,cut=True))
            else:            cut = "(%s)*(%s)*(%s)*(%s)" % (self._weightString,self._options.lumi, self._scaleFactor, self.adaptExpr(cut,cut=True))
            if self._options.doS2V:
                cut  = scalarToVector(cut)
#            print cut
            ROOT.gROOT.cd()
            if ROOT.gROOT.FindObject("dummy") != None: ROOT.gROOT.FindObject("dummy").Delete()
            histo = ROOT.TH1D("dummy","dummy",1,0.0,1.0); histo.Sumw2()
            (firstEntry, maxEntries) = self._rangeToProcess(fsplit)
            nev = tree.Draw("0.5>>dummy", cut, "goff", maxEntries, firstEntry)
            self.negativeCheck(histo)
            return [ histo.GetBinContent(1), histo.GetBinError(1), nev ]
        else: 
            cut = self.adaptExpr(cut,cut=True)
            if self._options.doS2V:
                cut  = scalarToVector(cut)
            (firstEntry, maxEntries) = self._rangeToProcess(fsplit)
            npass = tree.Draw("1",self.adaptExpr(cut,cut=True),"goff", maxEntries, firstEntry);
            return [ npass, sqrt(npass), npass ]
    def _stylePlot(self,plot,spec):
        return stylePlot(plot,spec,self.getOption)
    def getPlot(self,plotspec,cut,fsplit=None):
        ret = self.getPlotRaw(plotspec.name, plotspec.expr, plotspec.bins, cut, plotspec, fsplit=fsplit)
        # fold overflow
        if ret.ClassName() in [ "TH1F", "TH1D" ] :
            n = ret.GetNbinsX()
            if plotspec.getOption('IncludeOverflows',True) and ("TProfile" not in ret.ClassName()):
                ret.SetBinContent(1,ret.GetBinContent(0)+ret.GetBinContent(1))
                ret.SetBinContent(n,ret.GetBinContent(n+1)+ret.GetBinContent(n))
                ret.SetBinError(1,hypot(ret.GetBinError(0),ret.GetBinError(1)))
                ret.SetBinError(n,hypot(ret.GetBinError(n+1),ret.GetBinError(n)))
                ret.SetBinContent(0,0)
                ret.SetBinContent(n+1,0)
                ret.SetBinContent(0,0)
                ret.SetBinContent(n+1,0)
            rebin = plotspec.getOption('rebinFactor',0)
            if plotspec.bins[0] != "[" and rebin > 1 and n > 5:
                while n % rebin != 0: rebin -= 1
                if rebin != 1: ret.Rebin(rebin)
            if plotspec.getOption('Density',False):
                for b in xrange(1,n+1):
                    ret.SetBinContent( b, ret.GetBinContent(b) / ret.GetXaxis().GetBinWidth(b) )
                    ret.SetBinError(   b, ret.GetBinError(b) / ret.GetXaxis().GetBinWidth(b) )
        self._stylePlot(ret,plotspec)
        ret._cname = self._cname
        return ret
    def getPlotRaw(self,name,expr,bins,cut,plotspec,fsplit=None):
        unbinnedData2D = plotspec.getOption('UnbinnedData2D',False) if plotspec != None else False
        if not self._isInit: self._init()
        if self._appliedCut != None:
            if cut != self._appliedCut: 
                print "WARNING, for %s:%s, cut was set to '%s' but now plotting with cut '%s'." % (self._name, self._cname, self._appliedCut, cut)
                #self.clearCut()
            else:
                #print "INFO, for %s:%s, cut was already set to '%s', will use elist for plotting (%d entries)" % (self._name, self._cname, cut, self._elist.GetN())
                self._tree.SetEntryList(self._elist)
                #self._tree.SetEventList(self._elist)
        #print "for %s, %s, does my tree have an elist? %s " % ( self._name, self._cname, "yes" if self._tree.GetEntryList() else "no" )
        if self._weight:
            if self._isdata: cut = "(%s)     *(%s)*(%s)" % (self._weightString,                    self._scaleFactor, self.adaptExpr(cut,cut=True))
            else:            cut = "(%s)*(%s)*(%s)*(%s)" % (self._weightString,self._options.lumi, self._scaleFactor, self.adaptExpr(cut,cut=True))
        else:
            cut = self.adaptExpr(cut,cut=True)
        expr = self.adaptExpr(expr)
        if self._options.doS2V:
            cut  = scalarToVector(cut)
            expr = scalarToVector(expr)
#        print cut
#        print expr
        (firstEntry, maxEntries) = self._rangeToProcess(fsplit)
        if ROOT.gROOT.FindObject("dummy") != None: ROOT.gROOT.FindObject("dummy").Delete()
        histo = makeHistFromBinsAndSpec("dummy",expr,bins,plotspec)
        canKeys = (histo.ClassName() == "TH1D" and bins[0] != "[")
        if histo.ClassName != "TH2D" or self._name == "data": unbinnedData2D = False
        if unbinnedData2D:
            nent = self._tree.Draw("%s" % expr, cut, "", maxEntries, firstEntry)
            if nent == 0: return ROOT.TGraph(0)
            graph = ROOT.gROOT.FindObject("Graph").Clone(name) #ROOT.gPad.GetPrimitive("Graph").Clone(name)
            return graph
        drawOpt = "goff"
        if "TProfile" in histo.ClassName(): drawOpt += " PROF";
        self._tree.Draw("%s>>%s" % (expr,"dummy"), cut, drawOpt, maxEntries, firstEntry)
        if canKeys and histo.GetEntries() > 0 and histo.GetEntries() < self.getOption('KeysPdfMinN',2000) and not self._isdata and self.getOption("KeysPdf",False):
            #print "Histogram for %s/%s has %d entries, so will use KeysPdf " % (self._cname, self._name, histo.GetEntries())
            if "/TH1Keys_cc.so" not in ROOT.gSystem.GetLibraries(): 
                ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/TTHAnalysis/python/plotter/TH1Keys.cc+" % os.environ['CMSSW_BASE']);
            (nb,xmin,xmax) = bins.split(",")
            histo = ROOT.TH1KeysNew("dummyk","dummyk",int(nb),float(xmin),float(xmax),"a",1.0)
            self._tree.Draw("%s>>%s" % (expr,"dummyk"), cut, "goff", maxEntries, firstEntry)
            self.negativeCheck(histo)
            return histo.GetHisto().Clone(name)
        #elif not self._isdata and self.getOption("KeysPdf",False):
        #else:
        #    print "Histogram for %s/%s has %d entries, so won't use KeysPdf (%s, %s) " % (self._cname, self._name, histo.GetEntries(), canKeys, self.getOption("KeysPdf",False))
        self.negativeCheck(histo)
        return histo.Clone(name)
    def negativeCheck(self,histo):
        if not self._options.allowNegative: 
            cropNegativeBins(histo)
    def __str__(self):
        mystr = ""
        mystr += str(self._fname) + '\n'
        mystr += str(self._tfile) + '\n'
        mystr += str(self._weight) + '\n'
        mystr += str(self._scaleFactor)
        return mystr
    def processEvents(self,eventLoop,cut):
        if not self._isInit: self._init()
        cut = self.adaptExpr(cut,cut=True)
        if self._options.doS2V:
            cut  = scalarToVector(cut)
            self._tree.vectorTree = True 
        eventLoop.beginComponent(self)
        eventLoop.loop(self._tree, getattr(self._options, 'maxEvents', -1), cut=cut)
        eventLoop.endComponent(self)
    def applyCutAndElist(self,cut,elist):
        if self._appliedCut != None and self._appliedCut != cut: 
            print "WARNING: changing applied cut from %s to %s\n" % (self._appliedCut, cut)
        self._appliedCut = cut
        self._elist = elist
    def cutToElist(self,cut,fsplit=None):
        if not self._isInit: self._init()
        if self._weight:
            if self._isdata: cut = "(%s)     *(%s)*(%s)" % (self._weightString,                    self._scaleFactor, self.adaptExpr(cut,cut=True))
            else:            cut = "(%s)*(%s)*(%s)*(%s)" % (self._weightString,self._options.lumi, self._scaleFactor, self.adaptExpr(cut,cut=True))
        else: cut = self.adaptExpr(cut,cut=True)
        if self._options.doS2V: cut  = scalarToVector(cut)
        (firstEntry, maxEntries) = self._rangeToProcess(fsplit)
        self._tree.Draw('>>elist', cut, 'entrylist', maxEntries, firstEntry)
        elist = ROOT.gDirectory.Get('elist')
        if self._tree.GetEntries()==0 and elist==None: elist = ROOT.TEntryList("elist",cut) # empty list if tree is empty, elist would be a ROOT.nullptr TObject otherwise
        return elist
    def clearCut(self):
        #if not self._isInit: raise RuntimeError, "Error, clearing a cut on something that wasn't even initialized"
        self._appliedCut = None
        self._elist = None
        if self._isInit: self._tree.SetEntryList(None)
    def _rangeToProcess(self,fsplit):
        if fsplit != None and fsplit != (0,1):
            allEntries = min(self.getEntries(), self._options.maxEntries)
            chunkSize = int(ceil(allEntries/float(fsplit[1])))
            firstEntry = chunkSize * fsplit[0]
            maxEntries = chunkSize # the last chunk may go beyond the end of the tree, but ROOT stops anyway so we don't care
        else:
            firstEntry = 0
            maxEntries = self._options.maxEntries
        return (firstEntry, maxEntries)
def _copyPlotStyle(self,plotfrom,plotto):
        plotto.SetFillStyle(plotfrom.GetFillStyle())
        plotto.SetFillColor(plotfrom.GetFillColor())
        plotto.SetMarkerStyle(plotfrom.GetMarkerStyle())
        plotto.SetMarkerColor(plotfrom.GetMarkerColor())
        plotto.SetMarkerSize(plotfrom.GetMarkerSize())
        plotto.SetLineStyle(plotfrom.GetLineStyle())
        plotto.SetLineColor(plotfrom.GetLineColor())
        plotto.SetLineWidth(plotfrom.GetLineWidth())
        plotto.GetXaxis().SetTitle(plotfrom.GetXaxis().GetTitle())
        plotto.GetYaxis().SetTitle(plotfrom.GetYaxis().GetTitle())
        plotto.GetZaxis().SetTitle(plotfrom.GetZaxis().GetTitle())
        plotto.GetXaxis().SetNdivisions(plotfrom.GetXaxis().GetNdivisions())
        plotto.GetYaxis().SetNdivisions(plotfrom.GetYaxis().GetNdivisions())
        plotto.GetZaxis().SetNdivisions(plotfrom.GetZaxis().GetNdivisions())

def addTreeToYieldOptions(parser):
    parser.add_option("-l", "--lumi",           dest="lumi",   type="float", default="19.7", help="Luminosity (in 1/fb)");
    parser.add_option("-u", "--unweight",       dest="weight",       action="store_false", default=True, help="Don't use weights (in MC events)");
    parser.add_option("-W", "--weightString",   dest="weightString", type="string", default="1", help="Use weight (in MC events)");
    parser.add_option("-f", "--final",  dest="final", action="store_true", help="Just compute final yield after all cuts");
    parser.add_option("-e", "--errors",  dest="errors", action="store_true", help="Include uncertainties in the reports");
    parser.add_option("--tf", "--text-format",   dest="txtfmt", type="string", default="text", help="Output format: text, html");
    parser.add_option("-S", "--start-at-cut",   dest="startCut",   type="string", help="Run selection starting at the cut matched by this regexp, included.") 
    parser.add_option("-U", "--up-to-cut",      dest="upToCut",   type="string", help="Run selection only up to the cut matched by this regexp, included.") 
    parser.add_option("-X", "--exclude-cut", dest="cutsToExclude", action="append", default=[], help="Cuts to exclude (regexp matching cut name), can specify multiple times.") 
    parser.add_option("-E", "--enable-cut", dest="cutsToEnable", action="append", default=[], help="Cuts to enable if they were disabled in the cut file (regexp matching cut name), can specify multiple times.") 
    parser.add_option("-I", "--invert-cut",  dest="cutsToInvert",  action="append", default=[], help="Cuts to invert (regexp matching cut name), can specify multiple times.") 
    parser.add_option("-R", "--replace-cut", dest="cutsToReplace", action="append", default=[], nargs=3, help="Cuts to invert (regexp of old cut name, new name, new cut); can specify multiple times.") 
    parser.add_option("-A", "--add-cut",     dest="cutsToAdd",     action="append", default=[], nargs=3, help="Cuts to insert (regexp of cut name after which this cut should go, new name, new cut); can specify multiple times.") 
    parser.add_option("-N", "--n-minus-one", dest="nMinusOne", action="store_true", help="Compute n-minus-one yields and plots")
    parser.add_option("-t", "--tree",          dest="tree", default='treeProducerWMassEle', help="Pattern for tree name");
    parser.add_option("-G", "--no-fractions",  dest="fractions",action="store_false", default=True, help="Don't print the fractions");
    parser.add_option("-F", "--add-friend",    dest="friendTrees",  action="append", default=[], nargs=2, help="Add a friend tree (treename, filename). Can use {name}, {cname} patterns in the treename") 
    parser.add_option("--Fs", "--add-friend-simple",    dest="friendTreesSimple",  action="append", default=[], nargs=1, help="Add friends in a directory. The rootfile must be called evVarFriend_{cname}.root and tree must be called 't' in a subdir 'sf' inside the rootfile.") 
    parser.add_option("--FMC", "--add-friend-mc",    dest="friendTreesMC",  action="append", default=[], nargs=2, help="Add a friend tree (treename, filename) to MC only. Can use {name}, {cname} patterns in the treename") 
    parser.add_option("--FD", "--add-friend-data",    dest="friendTreesData",  action="append", default=[], nargs=2, help="Add a friend tree (treename, filename) to data trees only. Can use {name}, {cname} patterns in the treename") 
    parser.add_option("--FMCs", "--add-friend-mc-simple",    dest="friendTreesMCSimple",  action="append", default=[], nargs=1, help="Add friends in a directory to MC only. The rootfile must be called evVarFriend_{cname}.root and tree must be called 't' in a subdir 'sf' inside the rootfile.") 
    parser.add_option("--FDs", "--add-friend-data-simple",    dest="friendTreesDataSimple",  action="append", default=[], nargs=1, help="Add friends in a directory to data only. The rootfile must be called evVarFriend_{cname}.root and tree must be called 't' in a subdir 'sf' inside the rootfile.") 
    parser.add_option("--mcc", "--mc-corrections",    dest="mcCorrs",  action="append", default=[], nargs=1, help="Load the following file of mc to data corrections") 
    parser.add_option("--s2v", "--scalar2vector",     dest="doS2V",    action="store_true", default=False, help="Do scalar to vector conversion") 
    parser.add_option("--neg", "--allow-negative-results",     dest="allowNegative",    action="store_true", default=False, help="If the total yield is negative, keep it so rather than truncating it to zero") 
    parser.add_option("--max-entries",     dest="maxEntries", default=1000000000, type="int", help="Max entries to process in each tree") 
    parser.add_option("-L", "--load-macro",  dest="loadMacro",   type="string", action="append", default=[], help="Load the following macro, with .L <file>+");

def mergeReports(reports):
    import copy
    one = copy.deepcopy(reports[0])
    for i,(c,x) in enumerate(one):
        one[i][1][1] = pow(one[i][1][1], 2)
    for two in reports[1:]:
        for i,(c,x) in enumerate(two):
            one[i][1][0] += x[0]
            one[i][1][1] += pow(x[1],2)
            one[i][1][2] += x[2]
    for i,(c,x) in enumerate(one):
        one[i][1][1] = sqrt(one[i][1][1])
    return one

def mergePlots(name,plots):
    one = plots[0].Clone(name)
    if "TGraph" in one.ClassName():
        others = ROOT.TList()
        for two in plots[1:]: 
            others.Add(two)
        one.Merge(others)
    else:         
        for two in plots[1:]: 
            one.Add(two)
    return one

