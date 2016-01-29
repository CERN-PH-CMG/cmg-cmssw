import ROOT, os
from array import array
from glob import glob
from math import sqrt, hypot, log


def hypsub(all,stat):
    return sqrt(max(all*all-stat*stat,0))

def uniformAxis_(expr):
    (bins,xmin,xmax) = map(float,expr.split(","))
    dx = (xmax-xmin)/int(bins)
    return [ (xmin + dx*i) for i in xrange(0,int(bins)+1) ]

def makeTGraph(effs):
    ret = ROOT.TGraphAsymmErrors(len(effs))
    for i,(x,xl,xh,eff,effl,effh) in enumerate(effs):
        ret.SetPoint(i, x, eff)
        ret.SetPointError(i, -xl,xh, -effl,effh)
    return ret

def makeRatios(data):
    numeff = data["data"]
    deneff = data["ref"]
    xn = numeff.GetX()
    yn = numeff.GetY()
    xd = deneff.GetX()
    yd = deneff.GetY()
    ratio = ROOT.TGraphAsymmErrors(numeff.GetN())
    rater = ROOT.TGraphAsymmErrors(numeff.GetN())
    unity = ROOT.TGraphAsymmErrors(numeff.GetN())
    for i in xrange(numeff.GetN()):
        found = False
        for i2 in xrange(deneff.GetN()):
            if abs(xn[i]-xd[i2]) < 1e-4: 
                found = True; break
        if yd[i2] <= 0 or not found:
            unity.SetPoint(i , xn[i ], -99)
            ratio.SetPoint(i , xn[i ], -99)
            rater.SetPoint(i , xn[i ], -99)
        else:
            ratio.SetPoint(i , xn[i ], yn[i ]/yd[i2])
            rater.SetPoint(i , xn[i ], yn[i ]/yd[i2])
            unity.SetPoint(i , xn[i ], 1.0)
            ratio.SetPointError(i , numeff.GetErrorXlow(i ), numeff.GetErrorXhigh(i ), numeff.GetErrorYlow(i )/yd[i2], numeff.GetErrorYhigh(i )/yd[i2])
            unity.SetPointError(i , deneff.GetErrorXlow(i2), deneff.GetErrorXhigh(i2), deneff.GetErrorYlow(i2)/yd[i2], deneff.GetErrorYhigh(i2)/yd[i2])
            rater.SetPointError(i , numeff.GetErrorXlow(i ), numeff.GetErrorXhigh(i ), 
                                    hypot(ratio.GetErrorYlow(i),unity.GetErrorYhigh(i)),
                                    hypot(ratio.GetErrorYhigh(i),unity.GetErrorYlow(i)))
    data["ratio"] = ratio
    data["unity"] = unity
    data["ratioErr"] = rater

def styleCommon(graph,options):
    graph.GetYaxis().SetRangeUser(options.yrange[0], options.yrange[1])
    graph.GetYaxis().SetDecimals(True)
    if options.xtitle: graph.GetXaxis().SetTitle(options.xtitle)
    if options.ytitle: graph.GetYaxis().SetTitle(options.ytitle)

def styleAsData(graph,options):
    styleCommon(graph,options)
    graph.SetMarkerStyle(20)
    graph.SetLineWidth(2)
    graph.SetLineColor(ROOT.kBlack)
    graph.SetMarkerColor(ROOT.kBlack)
    gsyst = getattr(graph, 'syst', None)
    if gsyst:
        styleCommon(gsyst,options)
        gsyst.SetMarkerStyle(0)
        gsyst.SetLineWidth(4)
        gsyst.SetLineColor(ROOT.kRed+0)
        gsyst.SetMarkerColor(ROOT.kRed+0)

def styleAsRef(graph,options):
    styleCommon(graph,options)
    graph.SetMarkerStyle(20)
    graph.SetFillColor(ROOT.kAzure+10)
    gsyst = getattr(graph, 'syst', None)
    if gsyst:
        styleCommon(gsyst,options)
        gsyst.SetMarkerStyle(20)
        gsyst.SetFillColor(ROOT.kViolet+6)

def stackEfficiencies(base,ref,options):
    styleAsData(base,options)
    styleAsRef(ref,options)
    ref.Draw("AE2");
    if hasattr(ref,"syst"):
        ref.syst.Draw("E2 SAME");
        ref.Draw("E2 SAME");
    if hasattr(base,"syst"):
        base.syst.Draw("PZ SAME");
    base.Draw("PZ SAME");

def plotRatio(effs,ratio,options):
    for e in effs:
        e.GetXaxis().SetLabelOffset(999) ## send them away
        e.GetXaxis().SetTitleOffset(999) ## in outer space
        e.GetYaxis().SetLabelSize(0.05)
    ratiosyst = getattr(ratio, 'syst', None)
    allratios = [ratio]
    styleAsData(ratio,options)
    if ratiosyst: 
        allratios += [ratiosyst, ratio.systOnly]
        ymax = max(ratiosyst.GetY()[i]+ratiosyst.GetErrorYhigh(i) for i in xrange(ratiosyst.GetN()))
        ymin = min(ratiosyst.GetY()[i]-ratiosyst.GetErrorYlow(i)  for i in xrange(ratiosyst.GetN()))
    else:
        ymax = max(ratio.GetY()[i]+ratio.GetErrorYhigh(i) for i in xrange(ratio.GetN()))
        ymin = min(ratio.GetY()[i]-ratio.GetErrorYlow(i)  for i in xrange(ratio.GetN()))
    for r in allratios:
        r.GetYaxis().SetRangeUser(1+1.3*(min(0.99,ymin)-1),1+1.3*(max(1.01,ymax)-1));
        r.GetXaxis().SetTitleSize(0.14)
        r.GetYaxis().SetTitleSize(0.14)
        r.GetXaxis().SetLabelSize(0.11)
        r.GetYaxis().SetLabelSize(0.11)
        r.GetYaxis().SetNdivisions(505)
        r.GetYaxis().SetTitle("ratio")
        r.GetYaxis().SetTitleOffset(0.52);
    ratio.Draw("APZ")
    if ratiosyst:
        styleAsRef(ratiosyst,options)
        ratiosyst.SetFillColor(ROOT.kViolet+6)
        ratiosyst.Draw("E2 SAME")
        styleAsRef(ratio.systOnly,options)
        ratio.systOnly.SetFillColor(ROOT.kOrange-3)
        ratio.systOnly.Draw("E2 SAME")
    line = ROOT.TLine(ratio.GetXaxis().GetXmin(),1,ratio.GetXaxis().GetXmax(),1)
    line.SetLineWidth(2);
    line.SetLineColor(ROOT.kGray+2);
    line.SetLineStyle(2);
    line.DrawLine(ratio.GetXaxis().GetXmin(),1,ratio.GetXaxis().GetXmax(),1)
    ratio.Draw("PZ SAME")

def plotEffs(name,effs,ratio,options):
    c1 = ROOT.TCanvas("c1", "c1", 600, (750 if options.doRatio else 600))
    c1.Draw()
    p1, p2 = c1, None # high and low panes
    # set borders, if necessary create subpads
    if len(effs) > 1 and options.doRatio:
        c1.SetWindowSize(600 + (600 - c1.GetWw()), (750 + (750 - c1.GetWh())));
        p1 = ROOT.TPad("pad1","pad1",0,0.31,1,1);
        p1.SetBottomMargin(0);
        p1.Draw();
        p2 = ROOT.TPad("pad2","pad2",0,0,1,0.31);
        p2.SetTopMargin(0);
        p2.SetBottomMargin(0.3);
        p2.SetFillStyle(0);
        p2.Draw();
        p1.cd();
    else:
        c1.SetWindowSize(600 + (600 - c1.GetWw()), 600 + (600 - c1.GetWh()));
    if len(effs) == 2:
        stackEfficiencies(effs[0],effs[1],options)
    else:
        styleAsData(effs[0],options)
        effs[0].Draw("AP")
    text = []
    if len(effs) > 1 and options.doRatio:
        p2.cd()
        plotRatio(effs,ratio,options)
    for ext in "pdf","png":
        c1.Print("%s/%s.%s" % (options.printDir, name, ext))
    if len(effs) == 2:
        dump = open("%s/%s.%s" % (options.printDir, name, "txt"), "w")
        dump.write(" xmin   xmax     data[%] uncertainty  ref[%] uncertainty   scalef    (+/- stat)       (+/- syst)     scalef    (+/- tot)   \n")
        data, fref, fratio = effs[0], TGraphByX(effs[1]), TGraphByX(ratio.syst)
        for i in xrange(data.GetN()):
            x = data.GetX()[i]
            xl = x-data.GetErrorXlow(i)
            xh = x+data.GetErrorXhigh(i)
            y = data.GetY()[i], data.GetErrorYlow(i), data.GetErrorYhigh(i)
            iref = fref[x]; iratio = fratio[x]
            if iref != None:
                       y0 = effs[1].GetY()[iref], effs[1].GetErrorYlow(iref), effs[1].GetErrorYhigh(iref)
            else:      y0 = 0, 0, 0
            if iratio != None:
                       r0 = ratio.GetY()[iratio], ratio.GetErrorYlow(iratio), ratio.GetErrorYhigh(iratio)
                       r1 = ratio.syst.GetY()[iratio], ratio.syst.GetErrorYlow(iratio), ratio.syst.GetErrorYhigh(iratio)
                       r2 = ratio.systOnly.GetY()[iratio], ratio.systOnly.GetErrorYlow(iratio), ratio.systOnly.GetErrorYhigh(iratio)
            else:      r0 = 1, 1, 1; r1 = 1, 1, 1; r2 = 1, 1, 1
            def peff(e,em,ep): return "%5.1f -%3.1f +%3.1f" % (e*100., max(em,0)*100., max(ep,0)*100.)
            def psf1(r,rm,rp): return "%5.3f -%5.3f +%5.3f" % (r, max(rm,0), max(rp,0))
            def psf2(rstat,rsyst): return "%5.3f -%5.3f +%5.3f -%5.3f +%5.3f" % (rstat[0], max(rstat[1],0), max(rstat[2],0), max(rsyst[1],0), max(rsyst[2],0))
            dump.write("% 5.1f  % 5.1f    %s  %s     %s    %s\n" % (xl,xh, peff(*y), peff(*y0), psf2(r0,r2), psf1(*r1)))
        dump.close()
def loadFile(name,options):
    tfile = ROOT.TFile.Open(options.inDir+"/"+name+".root")
    if not tfile: print "Can't find file "+name
    data = tfile.Get(name)
    ref  = tfile.Get(name+"_ref")
    truth  = tfile.Get(name+"_ref_truth")
    ret = { 'tfile':tfile, 'data':data, 'ref':ref, 'truth':truth }
    makeRatios(ret)
    return ret

class FetchByX:
    def __init__(self,data,withErrors=False):
        self._data = data[:]
        #self._data.sort(key  = lambda (k,v):k)
        self._withErrors = withErrors
    def __getitem__(self,x):
        for (dx,dy) in self._data:
            if abs(x-dx) < 1e-4: return dy
        return None
class TGraphByX:
    def __init__(self,graph):
        self._graph = graph
        self._x = None
        self._i = None
    def __getitem__(self,x):
        if self._x == None or abs(x-self._x) > 1e-4: 
            self._fetch(x)
        return self._i
    def _fetch(self,x):
        xd = self._graph.GetX()
        for i2 in xrange(self._graph.GetN()):
            if abs(x-xd[i2]) < 1e-4: 
                self._i = i2
                self._x = x
                return i2
        print "could not find ",x," in ",[xd[i] for i in xrange(self._graph.GetN())]
        return None
        
def diffBbB(numeff,deneff,relative=False):
    xn = numeff.GetX()
    yn = numeff.GetY()
    xd = deneff.GetX()
    yd = deneff.GetY()
    ret = {}
    for i in xrange(numeff.GetN()):
        found = False
        for i2 in xrange(deneff.GetN()):
            if abs(xn[i]-xd[i2]) < 1e-4: 
                found = True; break
        if found:
            diff = yn[i]-yd[i2]
            if relative: diff /= yd[i2]
            ret[xn[i]] = diff
        else:
            print "Warning, point at x ",xn[i]," missing in ",deneff.GetName()
            ret[xn[i]] = 0.
    return FetchByX(ret.items())

def applyDiff1DAsUncertainty(graph,diff,relative=False,oneSided=False):
    x = graph.GetX()
    for i in xrange(graph.GetN()):
        d = diff[x[i]]
        if d is None: continue
        if relative: d *= graph.GetY()[i]
        if oneSided:
            if d > 0: graph.SetPointEYhigh(i, hypot(graph.GetErrorYhigh(i),d))
            else:     graph.SetPointEYlow(i, hypot(graph.GetErrorYlow(i),-d))
        else:
            graph.SetPointEYhigh(i, hypot(graph.GetErrorYhigh(i),abs(d)))
            graph.SetPointEYlow(i, hypot(graph.GetErrorYlow(i),abs(d)))
    return graph

def applyDiff2DAsUncertainty(graph,diff,relative=False):
    x = graph.GetX()
    for i in xrange(graph.GetN()):
        d = diff[x[i]]
        if d is None: continue
        dl,dh = d
        if relative: 
            dh *= graph.GetY()[i]; dl *= graph.GetY()[i]
        graph.SetPointEYhigh(i, hypot(graph.GetErrorYhigh(i),abs(dh)))
        graph.SetPointEYlow(i, hypot(graph.GetErrorYlow(i),abs(dl)))
    return graph

def diffAsGraphBand(graph,diff,relative=False):
    x = graph.GetX()
    y = graph.GetY()
    ret = ROOT.TGraphAsymmErrors(graph.GetN())
    for i in xrange(graph.GetN()):
        d = diff[x[i]]
        if d is None: dl,dh = 0.,0.
        elif type(d) == float:  dl,dh = -abs(d),abs(d)
        else: dl,dh = d
        if relative: 
            dh *= graph.GetY()[i]; dl *= graph.GetY()[i]
        graph.SetPoint(i, x[i], y[i])
        graph.SetPointError(i, graph.GetErrorXlow(i), graph.GetErrorXhigh(i), -dl, dh)
    return graph


def envelopeOfDiffs(graph,diffs):
    x = graph.GetX()
    ret = []
    for i in xrange(graph.GetN()):
        dvals = [d[x[i]] for d in diffs]
        ret.append((x[i], (min(dvals),max(dvals))))
    return FetchByX(ret)

def closureSystematic(data):
    return diffBbB(data['ref'],data['truth'],relative=True)    

def fitShapeSystematic(data,alts,key):
    diffs = [ diffBbB(data[key],alt[key]) for alt in alts ]
    return envelopeOfDiffs(data[key],diffs)

def graphQSub(gsyst,gstat):
    gonly = gsyst.Clone()
    for i in xrange(gonly.GetN()):
        gonly.SetPointEYhigh(i, hypsub(gsyst.GetErrorYhigh(i),gstat.GetErrorYhigh(i)))
        gonly.SetPointEYlow(i, hypsub(gsyst.GetErrorYlow(i),gstat.GetErrorYlow(i)))
    return gonly 

def addTnPHarvestOptions(parser):
    parser.add_option("-t", "--tree",    dest="tree", default='tree', help="Pattern for tree name");
    parser.add_option("-s", "--signalModel",   dest="sigModel", default='voigt', help="Signal model");
    parser.add_option("-b", "--backgroundModel",   dest="bkgModel", default='expo', help="Background model");
    parser.add_option("--salt", "--altSignalModel",   dest="altSigModel", default=[], action="append", help="Signal model");
    parser.add_option("--balt", "--altBackgroundModel",   dest="altBkgModel", default=[], action="append", help="Background model");
    parser.add_option("--exclude", "--exclude", dest="exclude", type="string", default=[], action="append", nargs=2, help="make a single fit");
    parser.add_option("-N", "--name",    dest="name", type="string", help="name", default="eff")
    parser.add_option("--xtitle",   dest="xtitle", type="string", default=None, help="X title")
    parser.add_option("--ytitle",   dest="ytitle", type="string", default="Efficiency", help="Y title")
    parser.add_option("--mtitle",   dest="mtitle", type="string", default="Mass (GeV)", help="M title")
    parser.add_option("--pdir", "--print-dir", dest="printDir", type="string", default="plots", help="print out plots in this directory");
    parser.add_option("--idir", "--in-dir", dest="inDir", type="string", default="plots", help="print out plots in this directory");
    parser.add_option("--yrange", dest="yrange", type="float", nargs=2, default=(0,1.025));
    parser.add_option("--rrange", dest="rrange", type="float", nargs=2, default=(0.95,1.05));
    parser.add_option("--doRatio", dest="doRatio", action="store_true", default=False, help="Add a ratio plot at the bottom")


if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] tree reftree")
    addTnPHarvestOptions(parser)
    (options, args) = parser.parse_args()
    ROOT.gROOT.SetBatch(True)
    ROOT.gROOT.ProcessLine(".x ~/cpp/tdrstyle.cc")
    ROOT.gStyle.SetOptStat(0)
    if not os.path.exists(options.printDir):
        os.system("mkdir -p %s" % options.printDir)
        os.system("cp /afs/cern.ch/user/g/gpetrucc/php/index.php  %s/" % options.printDir)
    if not os.path.exists(options.inDir):
        raise RuntimeError, "Input directory missin"
    pieces = options.name.split("_",1)
    pattern = "%s_%%s_%%s_%s" % (pieces[0], pieces[1])
    nominal = pattern % (options.sigModel, options.bkgModel)
    allsigs = [ options.sigModel ] + options.altSigModel
    allbkgs = [ options.bkgModel ] + options.altBkgModel
    altmods = [ (s,b) for s in allsigs for b in allbkgs if (s,b) != (options.sigModel, options.bkgModel) and (s,b) not in options.exclude ]
    altNames = [ pattern % (s,b) for (s,b) in altmods ]
    
    main = loadFile(nominal, options)
    alts = [ loadFile(alt, options) for alt in altNames ]
    closure = closureSystematic(main)
    fitShape = dict([ (x,fitShapeSystematic(main, alts, x)) for x in ("data","ref","ratio") ])
    
    for k in "data","ref","ratio","ratioErr":
        syst = main[k].Clone()
        applyDiff1DAsUncertainty(syst,closure,relative=True,oneSided=False)
        applyDiff2DAsUncertainty(syst,fitShape[x])
        main[k].syst = syst
        main[k].systOnly = graphQSub(syst, main[k])
    plotEffs(options.name, [ main['data'], main['ref'] ], main['ratioErr'], options)

