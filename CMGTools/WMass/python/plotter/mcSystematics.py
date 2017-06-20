#!/usr/bin/env python
#from mcAnalysis import *
from CMGTools.MonoXAnalysis.plotter.mcPlots import *
import itertools

class SystematicSpec:
    def __init__(self,name,expr):
        self.name = name
        self.syst = expr

class SystematicFile:
    def __init__(self,txtfileOrSysts,options=None):
        self._systs = []
        file = open(txtfileOrSysts, "r")
        if not file: raise RuntimeError, "Cannot open "+txtfileOrSysts+"\n"
        for line in file:
            try:
                if len(line.strip()) == 0 or line.strip()[0] == '#': continue
                while line.strip()[-1] == "\\":
                    line = line.strip()[:-1] + file.next()
                (name,syst) = [x.strip().replace(";",":") for x in line.replace("\:",";").split(":")]
                self._systs.append(SystematicSpec(name,syst))
            except ValueError, e:
                print "Error parsing cut line [%s]" % line.strip()
                raise 
    def systs(self):
        return self._systs[:]


class TFactorMaker:
    def __init__(self,tdir):
        self._options = options
        self._dir = tdir
        ROOT.gROOT.ProcessLine(".x tdrstyle.cc")
        ROOT.gROOT.ProcessLine(".L smearer.cc+")
        ROOT.gStyle.SetOptStat(0)
        ROOT.gStyle.SetOptTitle(0)
    def run(self,mca,cuts,plots,systematics):
        sets = [ (None, 'all cuts', cuts.allCuts()) ]
        if not self._options.final:
            allcuts = cuts.sequentialCuts()
            if self._options.nMinusOne: allcuts = cuts.nMinusOneCuts()+[None] # add a dummy entry since we use allcuts[:-1] below
            for i,(cn,cv) in enumerate(allcuts[:-1]): # skip the last one which is equal to all cuts
                cnsafe = "cut_%02d_%s" % (i, re.sub("[^a-zA-Z0-9_.]","",cn.replace(" ","_")))
                sets.append((cnsafe,cn,cv))
        for subname, title, cut in sets:
            print "cut set: ",title
            dir = self._dir
            if subname:
                if self._dir.Get(subname):
                    dir = self._dir.Get(subname)
                else:
                    dir = self._dir.mkdir(subname,title)
            dir.cd()
            
            sspecs = systematics.systs()
            for sspec in sspecs:
                print "    systematic weight: ",sspec.name

                pspecs = plots.plots()
                for pspec in pspecs:
                    print "       plot: ",pspec.name
                    pmap = mca.getPlots(pspec,cut,makeSummary=True,replaceweight=sspec.syst)
                    for k,v in pmap.iteritems():
                        if v.InheritsFrom("TH1"): 
                            h = v.Clone("%s_%s" % (v.GetName(),sspec.name))
                            h.SetDirectory(dir) 
                        dir.WriteTObject(h)

                
def addTFactorMakerOptions(parser):
    addPlotMakerOptions(parser)


if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] mc.txt cuts.txt plots.txt systematics.txt")
    addTFactorMakerOptions(parser)
    parser.add_option("-o", "--out", dest="out", default=None, help="Output file name. by default equal to plots -'.txt' +'.root'");
    (options, args) = parser.parse_args()
    mca  = MCAnalysis(args[0],options)
    cuts = CutsFile(args[1],options)
    plots = PlotFile(args[2],options)
    systematics = SystematicFile(args[3],options)
    outname  = options.out if options.out else (args[2].replace(".txt","")+"_systematics.root")
    if (not options.out) and options.printDir:
        outname = options.printDir + "/"+os.path.basename(args[2].replace(".txt","")+"_systematics.root")
    if os.path.dirname(outname) and not os.path.exists(os.path.dirname(outname)):
        os.system("mkdir -p "+os.path.dirname(outname))
        if os.path.exists("/afs/cern.ch"): os.system("cp /afs/cern.ch/user/g/gpetrucc/php/index.php "+os.path.dirname(outname))
    print "Will save plots to ",outname
    fcmd = open(re.sub("\.root$","",outname)+"_command.txt","w")
    fcmd.write("%s\n\n" % " ".join(sys.argv))
    fcmd.write("%s\n%s\n" % (args,options))
    fcmd.close()
    fcut = open(re.sub("\.root$","",outname)+"_cuts.txt","w")
    fcut.write("%s\n" % cuts); fcut.close()
    os.system("cp %s %s " % (args[0], re.sub("\.root$","",outname)+"_mca.txt"))
    os.system("cp %s %s " % (args[3], re.sub("\.root$","",outname)+"_systematics.txt"))
    #fcut = open(re.sub("\.root$","",outname)+"_cuts.txt")
    #fcut.write(cuts); fcut.write("\n"); fcut.close()
    outfile  = ROOT.TFile(outname,"RECREATE")
    tfactor = TFactorMaker(outfile)
    tfactor.run(mca,cuts,plots,systematics)
    outfile.Close()


