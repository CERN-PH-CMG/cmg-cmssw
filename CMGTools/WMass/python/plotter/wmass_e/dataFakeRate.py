#!/usr/bin/env python
#from mcAnalysis import *
from CMGTools.WMass.plotter.mcEfficiencies import *
import itertools

def _h1NormWithError(h,normSyst):
    y = h.Integral()
    stat2 = sum([h.GetBinError(b)**2 for b in xrange(1,h.GetNbinsX()+1)])
    return (y, sqrt(stat2 + (y*normSyst)**2))

def addPolySystBands(tlist,amplitude,order):
    ref = tlist.At(0)
    funcs = {
        1: lambda u : 2*(u-0.5),
        2: lambda u : 8*((u-0.5)**2) - 1,
    }
    xmin, xmax = ref.GetXaxis().GetXmin(), ref.GetXaxis().GetXmax()
    hi = ref.Clone(); hi.SetDirectory(None)
    lo = ref.Clone(); lo.SetDirectory(None)
    for b in xrange(1,ref.GetNbinsX()+1):
        xval = ref.GetXaxis().GetBinCenter(b)
        r = 1 + amplitude * funcs[order]((xval-xmin)/(xmax-xmin))
        hi.SetBinContent(b, ref.GetBinContent(b) * r)
        lo.SetBinContent(b, ref.GetBinContent(b) / r)
    tlist.Add(hi)
    tlist.Add(lo)
def addStretchBands(tlist,amplitude,fineSlices=100):
    ref = tlist.At(0)
    ax = ref.GetXaxis() 
    xmin, xmax = ax.GetXmin(), ax.GetXmax()
    hi = ref.Clone(); hi.SetDirectory(None)
    lo = ref.Clone(); lo.SetDirectory(None)
    hi.Reset(); lo.Reset()
    for b in xrange(1,ref.GetNbinsX()+1):
        x0 = ax.GetBinLowEdge(b)
        dx = ax.GetBinWidth(b)/fineSlices
        w  = ref.GetBinContent(b)/fineSlices
        for fineSlice in xrange(0,fineSlices):
            x = x0 + dx*fineSlice
            hi.Fill(x*(1+amplitude), w)
            lo.Fill(x*(1-amplitude), w)
    tlist.Add(hi)
    tlist.Add(lo)
def addBbB(tlist,ycutoff,relcutoff,verbose=False):
    ret = []
    ref = tlist.At(0)
    ytot = ref.Integral()
    if (ytot == 0): return ret
    for b in xrange(1,ref.GetNbinsX()+1):
        y, e = ref.GetBinContent(b), ref.GetBinError(b)
        if y/ytot < ycutoff: continue
        if e/y    < relcutoff: continue
        if e < 0.2*sqrt(y+1): continue
        if verbose: print "\tbin %3d: yield %9.1f +- %9.1f (rel %.5f), rel err. %.4f, poisson %.1f" % (b, y, e, y/ytot, e/y if y else 1, sqrt(y+1))
        hi = ref.Clone(); hi.SetDirectory(None)
        lo = ref.Clone(); lo.SetDirectory(None)
        hi.SetBinContent(b, y+e)
        lo.SetBinContent(b, y*y/(y+e))
        tlist.Add(hi)
        tlist.Add(lo)
        ret.append("bin%d" % b)
    return ret

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] mc.txt cuts.txt ids.txt xvars.txt")
    addMCEfficiencyOptions(parser)
    for dup in ["--showRatio","--rebin","--sP","--xP","--legendWidth"]: parser.remove_option(dup)
    addPlotMakerOptions(parser,  addAlsoMCAnalysis=False)
    parser.add_option("-i", "--in", dest="infile", default=None, help="Input file name");
    parser.add_option("--algo", dest="algo", default="fitND", help="Algorithm to use");
    parser.add_option("--bare", dest="bare", action="store_true", default=False, help="Just make the first histograms and stop");
    parser.add_option("--subSyst", dest="subSyst", type="float", default=0, help="Extra systematic on EWK subtraction");
    parser.add_option("--shapeSystSignal", dest="shapeSystSig", type="string", default="l", help="Shape systematic for signal: l = linear, q = quadratic, s = stretch");
    parser.add_option("--shapeSystBackground", dest="shapeSystBkg", type="string", default="l", help="Shape systematic for background: l = linear, q = quadratic, s = stretch");
    parser.add_option("--fcut", dest="fcut", default=None, nargs=2, type='float', help="Cut in the discriminating variable");
    parser.add_option("--fqcd-ranges", dest="fqcdRanges", default=(0.0, 20.0, 50.0, 120.0), nargs=4, type='float', help="Boundaries for the fqcd method");
    parser.add_option("--same-nd-templates", dest="sameNDTemplates", action="store_true", default=False, help="Just make the first histograms and stop");
    (options, args) = parser.parse_args()
    mca  = MCAnalysis(args[0],options)
    procs = mca.listProcesses()
    cut = CutsFile(args[1],options).allCuts()
    ids   = PlotFile(args[2],options).plots()
    xvars = PlotFile(args[3],options).plots()
    fitvarname = args[4]
    
    print "Processes = ",procs

    outname  = options.out if options.out else (args[2].replace(".txt","")+".root")
    if os.path.dirname(outname) != "":
        dirname = os.path.dirname(outname)
        options.printDir = dirname
        if not os.path.exists(dirname):
            os.system("mkdir -p "+dirname)
            if os.path.exists("/afs/cern.ch"): os.system("cp /afs/cern.ch/user/g/gpetrucc/php/index.php "+dirname)
    outfile  = ROOT.TFile(outname,"RECREATE")
    plotter = PlotMaker(outfile, options)

    vars2d = []
    for xspec in xvars:
        if xspec.name == fitvarname: continue
        for fspec in xvars:
            if fspec.name != fitvarname: continue
            if xspec.bins[0] == "[":
                if fspec.bins[0] == "[":        
                    fbind = fspec.bins
                else:
                    (nbins,fmin,fmax) = map(float, fspec.bins.split(','))
                    fbins = "[" + ",".join(map(str, [fmin+i*(fmax-fmin)/nbins for i in xrange(0,int(nbins+1))]))  + "]"
                bins2d = xspec.bins + "*" + fbins
            elif fspec.bins[0] == "[":
                (nbins,xmin,xmax) = map(float, xspec.bins.split(','))
                xbins = "[" + ",".join(map(str, [xmin+i*(xmax-xmin)/nbins for i in xrange(0,int(nbins+1))])) + "]"
                bins2d = xbins + "*" + fspec.bins
            else:
                bins2d = xspec.bins + "," + fspec.bins
            pspec = PlotSpec("%s_%s"  % (fspec.name, xspec.name), "%s:%s" % (fspec.expr, xspec.expr), bins2d, xspec.opts) 
            pspec.xvar = xspec
            pspec.fvar = fspec
            vars2d.append(pspec) 
    if options.infile:
        print "Reading from file: "+options.infile
        infile = ROOT.TFile.Open(options.infile)
        hists = []
        for y in ids:
            for x2 in vars2d:
                report = {}
                for p in procs + [ "signal", "background", "total", "data", "data_sub" ]:
                    hname = "%s_vs_%s_%s" % (y.name, x2.name, p)
                    if infile.Get(hname):
                        report[p] = infile.Get(hname)
                hists.append( (y,x2.xvar,x2.fvar,x2,report) )
    else:
        backup = options.globalRebin; options.globalRebin = 1
        hists = [ (y,x2.xvar,x2.fvar,x2,makeEff(mca,cut,y,x2,returnSeparatePassFail=True,notDoProfile=True,mainOptions=options)) for y in ids for x2 in vars2d ]
        options.globalRebin = backup
        if options.bare:
            for (yspec,xspec,fspec,x2d,report) in hists:
                for k,v in report.iteritems(): 
                    outfile.WriteTObject(v)
            outfile.ls()
            outfile.Close()
            print "Output saved to %s. exiting." % outname
            exit()
    for (yspec,xspec,fspec,x2d,report) in hists:
        for k,v in report.iteritems(): outfile.WriteTObject(v)
        myname = outname.replace(".root","_%s_%s.root" % (yspec.name,xspec.name))
        bindirname = myname.replace(".root","")+".dir";
        if not os.path.exists(bindirname):
            os.system("mkdir -p "+bindirname)
            if os.path.exists("/afs/cern.ch"): os.system("cp /afs/cern.ch/user/g/gpetrucc/php/index.php "+bindirname)
        is2D =  (":" in xspec.expr.replace("::","--"))
        if is2D: raise RuntimeError, "Not implemented"
        projection = makeHistFromBinsAndSpec("projection",xspec.expr,xspec.bins,xspec); projection.SetDirectory(None)
        fitvarhist = makeHistFromBinsAndSpec("fitvarhist",fspec.expr,fspec.bins,fspec); fitvarhist.SetDirectory(None)
        fr_fit     = ROOT.TGraphAsymmErrors()
        if not is2D:
            xzbins  = xspec.bins + ( "*[-0.5,0.5,1.5]" if xspec.bins[0] == "[" else ",2,-0.5,1.5" )
            xzexpr  = yspec.expr + ":" + xspec.expr
            xzreport0 = dict( [ (p,makeHistFromBinsAndSpec("%s_vs_%s_%s_2d_prefit" % (yspec.name,xspec.name,p), xzexpr, xzbins, xspec)) for p in report.iterkeys() ] )
            xzreport  = dict( [ (p,makeHistFromBinsAndSpec("%s_vs_%s_%s_2d"        % (yspec.name,xspec.name,p), xzexpr, xzbins, xspec)) for p in report.iterkeys() ] )
            fzbins  = fspec.bins + ( "*[-0.5,0.5,1.5]" if fspec.bins[0] == "[" else ",2,-0.5,1.5" )
            fzexpr  = yspec.expr + ":" + fspec.expr
            fzreport  = dict( [ (p,makeHistFromBinsAndSpec("%s_vs_%s_%s_2d"        % (yspec.name,fspec.name,p), fzexpr, fzbins, fspec)) for p in report.iterkeys() ] )
            xereport  = dict( [ (p,ROOT.TGraphAsymmErrors(projection.GetNbinsX()))   for p in report.iterkeys() ] )
            if options.algo == "globalFit":
                # make plot above xcut
                (bzname,iz) = ("_pass",2)
                greport = {}; gnorms = {}
                for (p,h) in report.iteritems():
                    hproj = fitvarhist.Clone("%s_for_%s%s_%s" % (fspec.name,yspec.name,bzname,p))
                    hproj.SetDirectory(None)
                    for ix in xrange(1,projection.GetNbinsX()+1):
                        if options.xcut:
                            xval = projection.GetXaxis().GetBinCenter(ix)
                            if not ( options.xcut[0] <= xval and xval <= options.xcut[1] ): continue
                        for iy in xrange(1,h.GetNbinsY()+1):
                            hproj.SetBinContent(iy, hproj.GetBinContent(iy) + h.GetBinContent(ix,iy,iz))
                            hproj.SetBinError(iy,   hypot(hproj.GetBinError(iy), h.GetBinError(ix,iy,iz)))
                    mca.stylePlot(p, hproj, fspec, mayBeMissing=True)
                    if options.globalRebin > 1: hproj.Rebin(options.globalRebin)
                    cropNegativeBins(hproj)
                    greport[p] = hproj
                    gnorms[p]  = { 'pre': hproj.Integral() }
                    outfile.WriteTObject(hproj, hproj.GetName()+"_prefit")
                plotter.printOnePlot(mca, fspec, greport, printDir=bindirname, outputName= "%s_for_%s%s_prefit" % (fspec.name,yspec.name,bzname) ) 
                for p in mca.listBackgrounds()+mca.listSignals():
                    mca.setProcessOption(p, 'FreeFloat',      True)
                doNormFit(fspec,greport,mca,saveScales=True)
                for (p,h) in greport.iteritems():
                    outfile.WriteTObject(h, h.GetName()+"_postfit")
                    gnorms[p]['post'] = h.Integral() 
                if options.subSyst > 0:
                    for p in mca.listBackgrounds():
                        mca.setProcessOption(p, 'NormSystematic', hypot(options.subSyst, mca.getProcessOption(p,'NormSystematic',0.)))
                plotter.printOnePlot(mca, fspec, greport, printDir=bindirname, outputName = "%s_for_%s%s" % (fspec.name,yspec.name,bzname))
                for (p,h) in report.iteritems():
                    if gnorms[p]['pre'] != gnorms[p]['post'] and gnorms[p]['post'] > 0:
                        h.Scale(gnorms[p]['post']/gnorms[p]['pre'])
            for ix in xrange(1,projection.GetNbinsX()+1):
                bxname = "_bin_%g_%g" % (projection.GetXaxis().GetBinLowEdge(ix),projection.GetXaxis().GetBinUpEdge(ix))
                xval = projection.GetXaxis().GetBinCenter(ix)
                if options.xcut:
                    if not ( options.xcut[0] <= xval and xval <= options.xcut[1] ): continue
                freport_num_den = {"pass":{},"fail":{}}
                for (bzname,iz) in [("_pass",2),("_fail",1)]:
                    if options.algo == "fitND":
                        for p in mca.listBackgrounds():
                            mca.setProcessOption(p, 'FreeFloat',      (iz == 2))
                            mca.setProcessOption(p, 'NormSystematic', hypot(1, mca.getProcessOption(p,'NormSystematic',0.)))
                        for p in mca.listSignals():
                            mca.setProcessOption(p, 'FreeFloat',      True)
                            mca.setProcessOption(p, 'NormSystematic', 0.  )
                    freport = {}
                    for (p,h) in report.iteritems():
                        if p in [ "signal", "background", "total", "data_sub" ] : continue
                        hproj = fitvarhist.Clone("%s_for_%s%s_%s%s_%s" % (fspec.name,xspec.name,bxname,yspec.name,bzname,p))
                        hproj.SetDirectory(None)
                        # sanity check binning
                        if h.GetNbinsX() != projection.GetNbinsX(): raise RuntimeError, "Inconsistent input binning with x variable binning"
                        if h.GetXaxis().GetBinLowEdge(ix) != projection.GetXaxis().GetBinLowEdge(ix): raise RuntimeError, "Inconsistent input binning with x variable binning"
                        if h.GetXaxis().GetBinUpEdge(ix) != projection.GetXaxis().GetBinUpEdge(ix): raise RuntimeError, "Inconsistent input binning with x variable binning"
                        myfz = fzreport[p]
                        for iy in xrange(1,h.GetNbinsY()+1):
                            if options.fcut:
                                fval = h.GetYaxis().GetBinCenter(iy)
                                if fval < options.fcut[0] or fval > options.fcut[1]: continue
                            hproj.SetBinContent(iy, h.GetBinContent(ix,iy,iz))
                            hproj.SetBinError(iy, h.GetBinError(ix,iy,iz))
                            myfz.SetBinContent(iy, iz, h.GetBinContent(ix,iy,iz))
                            myfz.SetBinError(iy, iz, h.GetBinError(ix,iy,iz))
                        mca.stylePlot(p, hproj, fspec, mayBeMissing=True)
                        cropNegativeBins(hproj)
                        if options.globalRebin > 1: hproj.Rebin(options.globalRebin)
                        freport[p] = hproj
                        freport_num_den[bzname[1:]][p] = hproj
                        outfile.WriteTObject(hproj, hproj.GetName()+"_prefit")
                        xzreport0[p].SetBinContent(ix,iz, hproj.Integral())
                        xzreport0[p].SetBinError(  ix,iz, _h1NormWithError(hproj, 0.0)[1])
                    if options.algo == "fitND":
                        plotter.printOnePlot(mca, fspec, freport, printDir=bindirname, 
                                             outputName = "%s_for_%s%s_%s%s_prefit" % (fspec.name,xspec.name,bxname,yspec.name,bzname)) 
                        doNormFit(fspec,freport,mca,saveScales=True)
                        for (p,h) in freport.iteritems():
                            outfile.WriteTObject(h, h.GetName()+"_postfit")
                            xzreport[p].SetBinContent(ix,iz, h.Integral())
                            xzreport[p].SetBinError(  ix,iz, _h1NormWithError(h, mca.getProcessOption(p,'NormSystematic',0.))[1])
                        plotter.printOnePlot(mca, fspec, freport, printDir=bindirname,
                                             outputName = "%s_for_%s%s_%s%s" % (fspec.name,xspec.name,bxname,yspec.name,bzname)) 
                if options.algo in ("fQCD","ifQCD"):
                    # == save settings ===
                    ybackup = options.yrange; xcbackup = options.xcut; rbackup = options.showRatio
                    options.yrange = (0,1.3); options.xcut = None; options.showRatio = False
                    # ==== Control plot: FR vs FVar
                    fzrebinrep = dict([(k,h.RebinX(4, h.GetName()+"_rebin")) for (k,h) in fzreport.iteritems()])
                    ereport = dict([(title, effFromH2D(hist,options)) for (title, hist) in fzrebinrep.iteritems()])
                    procsToStack = mca.listSignals()+mca.listBackgrounds()+["data"]
                    effs = styleEffsByProc(ereport,procsToStack,mca)
                    if not effs: continue # cases of completely empty bins
                    feffname = myname.replace(".root",".dir/%s_vs_%s_%s%s.root" % (yspec.name,fspec.name,xspec.name,bxname))
                    stackEffs(feffname,fspec,effs,options)
                    # ==== Now split in high, low 
                    r_s = [ options.fqcdRanges[0], options.fqcdRanges[1] ]; r_l = [ options.fqcdRanges[2], options.fqcdRanges[3] ]
                    for (k,h) in fzreport.iteritems():
                        for iz in 1,2:
                            s_s, s_l = 0., 0.; s2_s, s2_l = 0., 0.
                            for ib in xrange(1,h.GetNbinsX()+1):
                                fval = h.GetXaxis().GetBinCenter(ib)
                                if r_s[0] <= fval and fval <= r_s[1]: 
                                    s_s  += h.GetBinContent(ib,iz)
                                    s2_s += h.GetBinError(ib,iz)**2
                                elif r_l[0] <= fval and fval <= r_l[1]: 
                                    s_l  += h.GetBinContent(ib,iz)
                                    s2_l += h.GetBinError(ib,iz)**2
                            h.SetBinContent(1, iz, s_s); h.SetBinError(1, iz, sqrt(s2_s))
                            h.SetBinContent(2, iz, s_l); h.SetBinError(2, iz, sqrt(s2_l))
                    fzreport["signal"]     = mergePlots("signal",    [fzreport[p] for p in mca.listSignals() if p in fzreport]) # edm
                    fzreport["background"] = mergePlots("background", [fzreport[p] for p in mca.listBackgrounds() if p in fzreport])
                    ereport = dict([(title, effFromH2D(hist,options)) for (title, hist) in fzreport.iteritems()])
                    for (k,h) in ereport.iteritems():
                        if not h: continue
                        while h.GetN() > 2: h.RemovePoint(2)
                        h.SetPoint( 0, 0.5*(r_s[0]+r_s[1]), h.GetY()[0])
                        h.SetPointEXlow( 0, 0.5*(r_s[1]-r_s[0]))
                        h.SetPointEXhigh(0, 0.5*(r_s[1]-r_s[0]))
                        h.SetPoint( 1, 0.5*(r_l[0]+r_l[1]), h.GetY()[1])
                        h.SetPointEXlow( 1, 0.5*(r_l[1]-r_l[0]))
                        h.SetPointEXhigh(1, 0.5*(r_l[1]-r_l[0]))
                    effs = styleEffsByProc(ereport,procsToStack,mca)
                    feffname = myname.replace(".root",".dir/%s_vs_%s_%s%s_rebin.root" % (yspec.name,fspec.name,xspec.name,bxname))
                    stackEffs(feffname,fspec,effs,options)
                    f_s = (ereport["data"].GetY()[0], ereport["data"].GetErrorYlow(0), ereport["data"].GetErrorYhigh(0))
                    f_l = (ereport["data"].GetY()[1], ereport["data"].GetErrorYlow(1), ereport["data"].GetErrorYhigh(1))
                    fb_s = (ereport["background"].GetY()[0], ereport["background"].GetErrorYlow(0), ereport["background"].GetErrorYhigh(0))
                    fb_l = (ereport["background"].GetY()[1], ereport["background"].GetErrorYlow(1), ereport["background"].GetErrorYhigh(1))
                    b_s_a = (fzreport["background"].GetBinContent(1,1) + fzreport["background"].GetBinContent(1,2))
                    b_l_a = (fzreport["background"].GetBinContent(2,1) + fzreport["background"].GetBinContent(2,2))
                    n_s_a = (fzreport["data"].GetBinContent(1,1) + fzreport["data"].GetBinContent(1,2))
                    n_l_a = (fzreport["data"].GetBinContent(2,1) + fzreport["data"].GetBinContent(2,2))
                    db_s_a = hypot(fzreport["background"].GetBinError(1,1), fzreport["background"].GetBinError(1,2))
                    db_l_a = hypot(fzreport["background"].GetBinError(2,1), fzreport["background"].GetBinError(2,2))
                    dn_s_a = hypot(fzreport["data"].GetBinError(1,1), fzreport["data"].GetBinError(1,2))
                    dn_l_a = hypot(fzreport["data"].GetBinError(2,1), fzreport["data"].GetBinError(2,2))
                    r_slp = (b_s_a/n_s_a)/(b_l_a/n_l_a)
                    print "b_s_a = ",b_s_a,"  b_l_a = ",b_l_a
                    print "x_s = ",(b_s_a/n_s_a),"  x_l = ",(b_l_a/n_l_a)
                    r_slp_stat = r_slp * sqrt(sum(x**2 for x in [db_s_a/b_s_a, db_l_a/b_l_a, dn_s_a/n_s_a, dn_l_a/n_l_a]))
                    r_slp_syst = r_slp * 2 * abs(fb_s[0]-fb_l[0])/(fb_s[0]+fb_l[0])
                    if options.algo == "fQCD":
                        if options.subSyst > 0: 
                            r_slp_syst = hypot(r_slp_syst, r_slp * options.subSyst)
                        dr     = hypot(r_slp_stat, r_slp_syst)
                        f_qcd  = max(0,f_s[0] - r_slp*f_l[0])/(1-r_slp)
                        df_s = max(f_s[1],f_s[2])/(1-r_slp)
                        df_l = (r_slp*max(f_l[1],f_l[2]))/(1-r_slp)
                        df_r = 0.5*abs((f_s[0] - (r_slp-dr)*f_l[0])/(1-(r_slp-dr)) - (f_s[0] - (r_slp+dr)*f_l[0])/(1-(r_slp+dr)))
                        df   = sqrt(sum(x**2 for x in [df_s,df_l,df_r]))
                        print "%s  f_s %.3f  f_l %.3f  r_slp %.3f +- %.3f +- %.3f  f_qcd %.3f +- %.3f" % (bxname, f_s[0], f_l[0], r_slp, r_slp_stat, r_slp_syst, f_qcd, df)
                    elif options.algo == "ifQCD":
                        fs_s = (ereport["signal"].GetY()[0], ereport["signal"].GetErrorYlow(0), ereport["signal"].GetErrorYhigh(0))
                        fs_l = (ereport["signal"].GetY()[1], ereport["signal"].GetErrorYlow(1), ereport["signal"].GetErrorYhigh(1))
                        gamma, dgamma = fs_l[0]/fs_s[0], fs_l[0]/fs_s[0]*hypot(max(fs_s[1:])/fs_s[0], max(fs_l[1:])/fs_l[0])
                        mu   , dmu    = fb_s[0]/fb_l[0], fb_s[0]/fb_l[0]*hypot(max(fb_s[1:])/fb_s[0], max(fb_l[1:])/fb_l[0])
                        s_s_a = (fzreport["signal"].GetBinContent(1,1) + fzreport["signal"].GetBinContent(1,2))
                        ds_s_a = hypot(fzreport["signal"].GetBinError(1,1), fzreport["signal"].GetBinError(1,2))
                        bs_ns  = b_s_a / (b_s_a + s_s_a)
                        dbs_ns = (b_s_a * s_s_a)/((b_s_a+s_s_a)**2)*hypot(db_s_a/b_s_a, ds_s_a/s_s_a)
                        def ifqcd(fs,fl,r,m,g,bsns):
                            fs,fl,r,m,g,bsns = map( lambda x : max(x,0), [fs,fl,r,m,g,bsns] );
                            return max(0,fs - r*m*fl)/(1 - r*m*g + bsns*(g*m-1)) 
                        def mhypot(*args):
                            return sqrt(sum(x**2 for x in args))
                        def ifqcd_werr(fsv,flv,r,dr,m,dm,g,dg,bsns,dbsns):
                            fs, fl = fsv[0], flv[0]
                            f = ifqcd(fs,fl,r,m,g,bsns)
                            df_f = hypot(ifqcd(max(fsv[1],fsv[2]),0,r,m,g,bsns), ifqcd(0,max(flv[1],flv[2]),r,m,g,bsns))
                            df_r = 0.5*abs(ifqcd(fs,fl,r+dr,m,g,bsns)-ifqcd(fs,fl,r-dr,m,g,bsns))
                            if m == 1: 
                                return (f, df_f, df_r, mhypot(df_r,df_r))
                            df_m = 0.5*abs(ifqcd(fs,fl,r,m+dm,g,bsns)-ifqcd(fs,fl,r,m-dm,g,bsns))
                            if g == 1:
                                return (f, df_f, df_r, df_m, mhypot(df_r,df_r,df_m))
                            df_g = 0.5*abs(ifqcd(fs,fl,r,m,g+dg,bsns)-ifqcd(fs,fl,r,m,g-dg,bsns))
                            if bsns == 0:
                                return (f, df_f, df_r, df_m, df_g, mhypot(df_r,df_r,df_m,df_g))
                            df_bsns = 0.5*abs(ifqcd(fs,fl,r,m,g,bsns+dbsns)-ifqcd(fs,fl,r,m,g,bsns-dbsns))
                            return (f, df_f, df_r, df_m, df_g, df_bsns, mhypot(df_r,df_r,df_m,df_g,df_bsns))
                        print "First iteration for %s, f_s %.3f  f_l %.3f :" % (bxname, f_s[0], f_l[0])
                        print "    r_slp               %.3f +- %.3f (stat) as in fQCD " % (r_slp, r_slp_stat)
                        print "    mu    = fb_s/fb_l = %.3f +- %.3f (stat) from MC" % (mu, dmu)
                        print "    gamma = fs_l/fs_s = %.3f +- %.3f (stat) from MC" % (gamma, dgamma)
                        print "    b_s/n_s           = %.3f +- %.3f (stat) from MC" % (bs_ns, dbs_ns)
                        print "    f_qcd(r, mu=1, gamma=1, bsns=*) = %.4f +- %.4f (f) +- %.4f (r)                                            [ +- %.4f (tot) ]" % ( 
                                ifqcd_werr(f_s,f_l, r_slp,r_slp_stat,  1,0,   1,0,    0,0  ) )
                        print "    f_qcd(r,  mu , gamma=1, bsns=0) = %.4f +- %.4f (f) +- %.4f (r) +- %.4f (g)                              [ +- %.4f (tot) ]" % ( 
                                ifqcd_werr(f_s,f_l, r_slp,r_slp_stat, mu,dmu,  1,0,   0,0  ) )
                        print "    f_qcd(r,  mu ,  gamma , bsns=0) = %.4f +- %.4f (f) +- %.4f (r) +- %.4f (g) +- %.4f (m)                [ +- %.4f (tot) ]" % ( 
                                ifqcd_werr(f_s,f_l, r_slp,r_slp_stat, mu,dmu,  gamma,dgamma,  0,0  ) ) 
                        print "    f_qcd(r,  mu ,  gamma ,  bsns ) = %.4f +- %.4f (f) +- %.4f (r) +- %.4f (g) +- %.4f (m) +- %.4f (b)  [ +- %.4f (tot) ]" % ( 
                                ifqcd_werr(f_s,f_l, r_slp,r_slp_stat, mu,dmu,  gamma,dgamma,  bs_ns,dbs_ns) )
                        print "Now with conservative systematics:"
                        print "    mu    = fb_s/fb_l = %.3f +- %.3f (full) from MC with additional %3.0f%% uncertainty" % (mu, hypot(dmu,options.subSyst*(mu-1)), 100*options.subSyst)
                        print "    gamma = fs_l/fs_s = %.3f +- %.3f (full) from MC with additional %3.0f%% uncertainty" % (gamma, hypot(dgamma,options.subSyst*(gamma-1)), 100*options.subSyst)
                        print "    b_s/n_s           = %.3f +- %.3f (full) from MC with additional %3.0f%% uncertainty" % (bs_ns, hypot(dbs_ns,options.subSyst*bs_ns), 100*options.subSyst)
                        print "    f_qcd(r,  mu ,  gamma ,  bsns ) = %.4f +- %.4f (f) +- %.4f (r) +- %.4f (g) +- %.4f (m) +- %.4f (b)  [ +- %.4f (tot) ]" % ( 
                                ifqcd_werr(f_s,f_l, r_slp, r_slp_stat, mu,hypot(dmu,options.subSyst*(mu-1)),  gamma,hypot(dgamma,options.subSyst*(gamma-1)),  bs_ns,hypot(dbs_ns,options.subSyst*bs_ns)) )
                        final = ifqcd_werr(f_s,f_l, r_slp, r_slp_stat, mu,hypot(dmu,options.subSyst*(mu-1)),  gamma,hypot(dgamma,options.subSyst*(gamma-1)),  bs_ns,hypot(dbs_ns,options.subSyst*bs_ns))
                        f_qcd, df = final[0], final[-1] 
                    ilast = fr_fit.GetN()
                    fr_fit.Set(ilast+1)
                    fr_fit.SetPoint(ilast, xval, f_qcd)
                    fr_fit.SetPointError(ilast, 
                                          -projection.GetXaxis().GetBinLowEdge(ix) + xval,
                                          +projection.GetXaxis().GetBinUpEdge(ix)  - xval, 
                                          df, df)
                    # == restore settings ===
                    options.yrange = ybackup; options.showRatio = rbackup; options.xcut = xcbackup
                elif options.algo == "fitSimND":
                    w = ROOT.RooWorkspace("w")
                    ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
                    w.factory("num[pass=2,fail=1]")
                    hist = freport_num_den["pass"]["data"]
                    w.factory("f[%g,%g]" % (hist.GetXaxis().GetXmin(), hist.GetXaxis().GetXmax()))
                    fedges =  [ hist.GetXaxis().GetBinLowEdge(i) for i in xrange(1,hist.GetNbinsX()+1) ]
                    fedges += [ hist.GetXaxis().GetXmax() ]
                    w.var("f").setBinning(ROOT.RooBinning(hist.GetNbinsX(),array('d',fedges)))
                    Ndata = sum(freport_num_den[i]["data"].Integral() for i in ("pass", "fail"))
                    Newk  = sum(freport_num_den[i][p].Integral() for i in ("pass", "fail") for p in mca.listBackgrounds() if p in freport_num_den[i])
                    Nqcd  = sum(freport_num_den[i][p].Integral() for i in ("pass", "fail") for p in mca.listSignals()     if p in freport_num_den[i])
                    if Newk+Nqcd == 0: continue
                    fewk  = sum(freport_num_den["pass"][p].Integral() for p in mca.listBackgrounds() if p in freport_num_den["pass"])/(Newk if Newk else 1)
                    fqcd  = sum(freport_num_den["pass"][p].Integral() for p in mca.listSignals()     if p in freport_num_den["pass"])/(Nqcd if Nqcd else 1)
                    Nqcd, Newk = Nqcd*Ndata/(Nqcd+Newk), Newk*Ndata/(Nqcd+Newk)
                    w.factory("expr::Nsig_pass(\"@0* @1   \",N_sig[%g,0,%g], fsig[%g,0,1])" % (Nqcd,Ndata,fqcd))
                    w.factory("expr::Nbkg_pass(\"@0* @1   \",N_bkg[%g,0,%g], fbkg[%g,0,1])" % (Newk,Ndata,fewk))
                    w.factory("expr::Nsig_fail(\"@0*(1-@1)\",N_sig, fsig)")
                    w.factory("expr::Nbkg_fail(\"@0*(1-@1)\",N_bkg, fbkg)")
                    combiner = ROOT.CombDataSetFactory(ROOT.RooArgSet(w.var("f")), w.cat("num"))
                    nuis = {}
                    for k,o in ('sig',options.shapeSystSig),('bkg',options.shapeSystBkg):
                        nuis[k] = [ (f[:1],float(f[2:])) for f in o.split(",") ]
                    nuislists = {}; constraints = []; allnuis = ROOT.RooArgSet()
                    for (k,ns) in nuis.iteritems():
                        nuislists[k] = dict([ (s,ROOT.RooArgList()) for s in ('pass','fail') ])
                        for n,val in ns:
                            if n == "b": continue
                            w.factory("Gaussian::nuis_{0}_shapePdf(nuis_{0}_shape[0,-3,3], 0, 1)".format(k+n))
                            constraints.append(w.pdf("nuis_{0}_shapePdf".format(k+n)))
                            for s in "pass","fail": nuislists[k][s].add(w.var("nuis_{0}_shape".format(k+n)))
#                        allnuis.add(nuislists[k]["pass"], False)
                        for idx in xrange(nuislists[k]["pass"].getSize()):
                            allnuis.add(nuislists[k]["pass"].at(idx),False)
                    for zstate in "pass", "fail":
                        rep = freport_num_den[zstate];  
                        # make nominal templates 
                        if options.sameNDTemplates:
                            print [ (p,r[p].Integral()) for p in mca.listSignals()     for r in freport_num_den.values() if p in r]
                            print [ (p,r[p].Integral()) for p in mca.listBackgrounds() for r in freport_num_den.values() if p in r]
                            rep["signal"]     = mergePlots("signal_"+zstate,     [r[p] for p in mca.listSignals()     for r in freport_num_den.values() if p in r])
                            rep["background"] = mergePlots("background_"+zstate, [r[p] for p in mca.listBackgrounds() for r in freport_num_den.values() if p in r])
                        else: 
                            rep["signal"]     = mergePlots("signal_"+zstate,     [rep[p] for p in mca.listSignals()     if p in rep])
                            rep["background"] = mergePlots("background_"+zstate, [rep[p] for p in mca.listBackgrounds() if p in rep])
                            nsig, nsigErr = _h1NormWithError(rep["signal"],0)
                            if zstate == "pass" and (nsig < 10*nsigErr):
                                print "Very poor statistics in the signal passing template (%g +/- %g), will use the failing one (normalized to the passing)" % (nsig, nsigErr)
                                rep["signal"] = mergePlots("signal_"+zstate,     [r[p] for p in mca.listSignals()     for r in freport_num_den.values() if p in r])
                                rep["signal"].Scale(nsig/rep["signal"].Integral()) 
                        #rep["signal"].Smooth()
                        #rep["background"].Smooth()
                        # make dataset 
                        for b in xrange(1,rep["data"].GetNbinsX()+1):
                            if rep["data"].GetBinContent(b) > 0:
                                if rep["signal"].GetBinContent(b) <= 0 and rep["background"].GetBinContent(b) <= 0:
                                    print "WARNING, bin %d filled in data (%d/%d) and not in MC" % ( b, rep["data"].GetBinContent(b), Ndata )
                                    rep["data"].SetBinContent(b, 0) 
                                    rep["data"].SetBinError(b, 0) 
                        rdh = ROOT.RooDataHist("data_"+zstate,"data",ROOT.RooArgList(w.var("f")), rep["data"])
                        combiner.addSetAny(zstate,rdh)
                        # make systematic histograms
                        sighists = ROOT.TList(); sighists.Add(rep["signal"])
                        bkghists = ROOT.TList(); bkghists.Add(rep["background"])
                        for what,tlist in [('sig',sighists),('bkg',bkghists)]:
                            for n,val in nuis[what]:
                                if n == "l": addPolySystBands(tlist, val, 1)
                                if n == "q": addPolySystBands(tlist, val, 2)
                                if n == "s": addStretchBands(tlist, val)
                                if n == "b": 
                                    print "Adding bin-by-bin uncertainties on %s %s, threshold %g" % (what,zstate,val)
                                    bins = addBbB(tlist,1e-3,val)
                                    for b in bins:
                                        key = "%s_%s_%s" % (what,zstate,b)
                                        w.factory("Gaussian::nuis_{0}_shapePdf(nuis_{0}_shape[0,-3,3], 0, 1)".format(key))
                                        nuislists[what][zstate].add(w.var("nuis_{0}_shape".format(key)))
                                        constraints.append(w.pdf("nuis_{0}_shapePdf".format(key)))
                                        allnuis.add(w.var("nuis_{0}_shape".format(key)))
                        # make summary plots of templates
                        lsig = mca.listSignals()[0]; lbkg = mca.listBackgrounds()[0]
                        for what,label,tlist in [('sig',lsig,sighists),('bkg',lbkg,bkghists)]:
                            postfixes = ["_"+x+d for (x,v) in nuis[what] for d in "Up", "Dn" if x != "b"]
                            shiftrep = dict([(label+p, tlist.At(i)) for (i,p) in enumerate([""]+postfixes)])
                            for p,h in shiftrep.iteritems(): mca.stylePlot(p, h, fspec)
                            plotter.printOnePlot(mca, fspec, shiftrep, extraProcesses = [label+x for x in postfixes], plotmode="norm", printDir=bindirname, 
                                             outputName = "%s_for_%s%s_%s_%s_%sSyst" % (fspec.name,xspec.name,bxname,yspec.name,zstate,what)) 
                        sigpdf = ROOT.FastVerticalInterpHistPdf2("signal_"+zstate,     "", w.var("f"), sighists, nuislists['sig'][zstate], 1., 1)
                        bkgpdf = ROOT.FastVerticalInterpHistPdf2("background_"+zstate, "", w.var("f"), bkghists, nuislists['bkg'][zstate], 1., 1)
                        getattr(w,'import')(sigpdf, ROOT.RooFit.RecycleConflictNodes(), ROOT.RooFit.Silence())
                        getattr(w,'import')(bkgpdf, ROOT.RooFit.RecycleConflictNodes(), ROOT.RooFit.Silence())
                        w.factory('SUM::all_{0}(Nsig_{0} * signal_{0}, Nbkg_{0} * background_{0})'.format(zstate))
                    data = combiner.doneUnbinned("data","data")
                    w.factory('SIMUL::all(num, pass=all_pass, fail=all_fail)') 
                    sim = ROOT.RooSimultaneousOpt(w.pdf("all"), "")
                    for c in constraints: sim.addExtraConstraint(c)
                    cmdArgs = ROOT.RooLinkedList()
                    cmdArgs.Add(ROOT.RooFit.Constrain(allnuis))
                    nll = sim.createNLL(data, cmdArgs)
                    minim = ROOT.RooMinimizer(nll)
                    minim.setPrintLevel(-1); minim.setStrategy(0);
                    minim.minimize("Minuit2","migrad")
                    minim.setPrintLevel(-1); minim.setStrategy(1);
#                    nll.setZeroPoint()
                    minim.minimize("Minuit2","migrad")
                    minim.hesse();
                    result = minim.save()
                    # post-fit plots
                    for zstate in "pass","fail":
                        pfrep = { 'data':freport_num_den[zstate]["data"] }
                        for what,wlong,label in [('sig','signal',lsig),('bkg','background',lbkg)]:
                            pdf  = w.pdf(wlong+"_"+zstate)
                            # nominal
                            w.allVars().assignValueOnly(result.floatParsFinal())
                            hist = pdf.createHistogram(wlong+"_"+zstate,w.var("f")); hist.SetDirectory(None)
                            hist.Scale(w.function("N%s_%s"%(what,zstate)).getVal()/hist.Integral())
                            # toys
                            ntoys = 500
                            sumw2s = [ 0. for b in xrange(1,hist.GetNbinsX()+1) ]
                            for itoy in xrange(ntoys):
                                w.allVars().assignValueOnly(result.randomizePars())
                                histT = pdf.createHistogram(wlong+"_"+zstate+"_toy",w.var("f")); histT.SetDirectory(None)
                                histT.Scale(w.function("N%s_%s"%(what,zstate)).getVal()/histT.Integral())
                                for b in xrange(1,hist.GetNbinsX()+1):
                                    sumw2s[b-1] += (histT.GetBinContent(b)-hist.GetBinContent(b))**2
                                del histT
                            for b in xrange(1,hist.GetNbinsX()+1):
                                hist.SetBinError(b, sqrt(sumw2s[b-1]/ntoys))
                            mca.stylePlot(label, hist, fspec)
                            pfrep[label] = hist
                        plotter.printOnePlot(mca, fspec, pfrep, printDir=bindirname, 
                                             outputName = "%s_for_%s%s_%s_%s_postfit" % (fspec.name,xspec.name,bxname,yspec.name,zstate)) 
                    # pre-fit plots
                    for zstate in "pass","fail":
                        pfrep = { 'data':freport_num_den[zstate]["data"] }
                        for what,wlong,label in [('sig','signal',lsig),('bkg','background',lbkg)]:
                            pdf  = w.pdf(wlong+"_"+zstate)
                            # nominal
                            w.allVars().assignValueOnly(result.floatParsInit())
                            hist = pdf.createHistogram(wlong+"_"+zstate,w.var("f")); hist.SetDirectory(None)
                            hist.Scale(w.function("N%s_%s"%(what,zstate)).getVal()/hist.Integral())
                            # toys
                            nuisancesSet = ROOT.RooArgSet(nuislists[what][zstate])
                            nuispdfs  = ROOT.RooArgList()
                            for c in constraints: nuispdfs.add(c)
                            nuispdf = ROOT.RooProdPdf("nuispdf","",nuispdfs)
                            ntoys = 500
                            nuisvals = nuispdf.generate(nuisancesSet, ntoys)
                            sumw2s = [ 0. for b in xrange(1,hist.GetNbinsX()+1) ]
                            for itoy in xrange(ntoys):
                                w.allVars().assignValueOnly(nuisvals.get(itoy))
                                histT = pdf.createHistogram(wlong+"_"+zstate+"_toy",w.var("f")); histT.SetDirectory(None)
                                histT.Scale(w.function("N%s_%s"%(what,zstate)).getVal()/histT.Integral())
                                for b in xrange(1,hist.GetNbinsX()+1):
                                    sumw2s[b-1] += (histT.GetBinContent(b)-hist.GetBinContent(b))**2
                                del histT
                            for b in xrange(1,hist.GetNbinsX()+1):
                                hist.SetBinError(b, sqrt(sumw2s[b-1]/ntoys))
                            mca.stylePlot(label, hist, fspec)
                            pfrep[label] = hist
                        plotter.printOnePlot(mca, fspec, pfrep, printDir=bindirname, 
                                             outputName = "%s_for_%s%s_%s_%s_prefit" % (fspec.name,xspec.name,bxname,yspec.name,zstate)) 
                    # minos for the efficiency
                    w.allVars().assignValueOnly(result.floatParsFinal())
                    nll = sim.createNLL(data, cmdArgs)
#                    nll.setZeroPoint()
                    var = w.var("fsig"); var.setConstant(True)
                    minim = ROOT.RooMinimizer(nll)
                    minim.setPrintLevel(-1); minim.setStrategy(0);
                    minim.minimize("Minuit2","migrad");
                    nll0 = nll.getVal(); f0 = var.getVal()
                    bounds = []; search = []
                    if f0 > 0: search.append((f0,max(0,f0-4*var.getError())))
                    if f0 < 0: search.append((f0,min(1,f0+4*var.getError())))
                    for x1,x2 in search:
                        for iTry in xrange(10):
                            var.setVal(x2)
                            minim.minimize("Minuit2","migrad");
                            y2 = 2*(nll.getVal()-nll0)
                            if y2 > 1: break
                            if x2 > x1: x2 = min((x2+1)/2, x2+(x2-x1))
                            else:       x2 = max((x2+0)/2, x2-(x1-x2))
                        while abs(x1-x2) > 0.0005:
                            xc = 0.5*(x1+x2)
                            var.setVal(xc)
                            minim.minimize("Minuit2","migrad");
                            y = 2*(nll.getVal()-nll0)
                            if y < 1: x1 = xc
                            else:     x2 = xc
                        bounds.append(x2)
                    df = max(abs(b-f0) for b in bounds)
                    ilast = fr_fit.GetN()
                    fr_fit.Set(ilast+1)
                    fr_fit.SetPoint(ilast, xval, f0)
                    fr_fit.SetPointError(ilast, 
                                          -projection.GetXaxis().GetBinLowEdge(ix) + xval,
                                          +projection.GetXaxis().GetBinUpEdge(ix)  - xval, 
                                          df, df)
                    #print "MC fake rate: %.4f " % fqcd
                    #print "Data fake rate: %.4f +- %.4f " % (f0, df)
        #print "\n"*5,"===== ALL BINS DONE ===== "
        for rep in xzreport, xzreport0: 
            for p,h in rep.iteritems(): 
                if p in [ "signal", "background", "total", "data_sub", "data" ] : continue
                rep["total"].Add(h) # edm
                if  mca.isSignal(p): rep["signal"    ].Add(h)
                else:                rep["background"].Add(h)
            makeDataSub(rep,mca)
        #print "\n"
        #for p,h in xzreport.iteritems(): print "%s: %s %s" % (p,h.Integral(),xzreport0[p].Integral())
        #print "\n"
        for p,h in xzreport0.iteritems(): xzreport[p+"_prefit"] = h
        for p,h in xzreport.iteritems(): outfile.WriteTObject(h)
        if is2D: ereport = dict([(title, effFromH3D(hist,options)) for (title, hist) in xzreport.iteritems()])
        else:    ereport = dict([(title, effFromH2D(hist,options, uncertainties="PF")) for (title, hist) in xzreport.iteritems()])
        if options.algo in ("fQCD","ifQCD"):
            ereport["data_fqcd"] = fr_fit
        elif options.algo == "fitSimND":
            ereport["data_fit"] = fr_fit
        for p,g in ereport.iteritems(): 
            print "%-30s: %s" % (p,g) 
            if g: outfile.WriteTObject(g)
        procsToStack = options.compare.split(",") if options.compare else ereport.keys()
        effs = styleEffsByProc(ereport,procsToStack,mca)
        if len(effs) == 0: continue
        stackEffs(myname,xspec,effs,options)
        for p in procsToStack: outfile.WriteTObject(ereport[p], "%s_vs_%s_%s_%s" % (yspec.name,xspec.name,fspec.name,p))
    outfile.Close()


