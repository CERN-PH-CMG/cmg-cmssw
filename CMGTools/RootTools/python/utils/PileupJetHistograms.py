from CMGTools.RootTools.statistics.Histograms import Histograms
import CMGTools.External.External
from ROOT import TH1F, TH2F, TFile, THStack, TF1, TGraphErrors, PileupJetIdentifier

from bisect import bisect

# --------------------------------------------------------------------------------------
def mkBinLabels(bins, scale=1., fmt="%1.0f", addOFlow=True):
    labels = []
    last = bins[0]
    for bin in bins[1:]:
        labels.append( str(fmt+"_"+fmt) % (last*scale,bin*scale) )
        last=bin
    if addOFlow:
        labels.append( str(fmt+"_inf") % (last*scale) )
    return labels

# --------------------------------------------------------------------------------------
def findBin(bins,val):
    return bisect(bins,val)-1

# --------------------------------------------------------------------------------------
def formatTitle(title,args):
    unit = args["unit"]
    if unit == "":
        args["unitx"]=""
        args["unity"]="%(perbin)s"
    else:
        args["unitx"]="("+unit+")"
        args["unity"]="%(perbinunit)s "+unit
    args["jetbin"] = "%(vtxbin)s%(ptbin)s%(etabin)s" % args
    return title % args    
        
# --------------------------------------------------------------------------------------
def fillTitle(h):
    binw = h.GetBinWidth(1)
    if binw == 1:
        perbin = "/ bin"
        perbinunit = "/"
    else:
        perbin = "/ %1.2g" % binw
        perbinunit = perbin
    h.SetTitle( h.GetTitle() % { "perbin" : perbin, "perbinunit" : perbinunit } )
    h.GetXaxis().SetTitle( h.GetXaxis().GetTitle() % { "perbin" : perbin, "perbinunit" : perbinunit } )
    h.GetYaxis().SetTitle( h.GetYaxis().GetTitle() % { "perbin" : perbin, "perbinunit" : perbinunit } )
        

# --------------------------------------------------------------------------------------
class PileupJetHistograms(Histograms) :
    """
    
    """
    ## protoypes for histogram booking 
    prototypes={
        
	"mva"        : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,-1.,1.),
        
	"jetPt"      : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","GeV/c",300,0,150),
	"jetEta"     : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",101,-5.05,5.05,),
	"jetPhi"     : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",33,-3.21,3.21),
        "jetM"       : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","GeV/c^{c}",100,0,50),

    	"nCharged"   : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,50),
	"nNeutrals"  : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,50),
	"nParticles" : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,50),
        
	"chgEMfrac"  : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",100,0,1.),
	"neuEMfrac"  : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",100,0,1.),
	"chgHadrfrac": ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",100,0,1.),
	"neuHadrfrac": ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",100,0,1.),
	
	"d0"         : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","cm",100,0,2),
	"dZ"         : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","cm",100,0,10),

	"leadPt"     : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","GeV/c",70,0,35),
	"leadEta"    : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",101,-5.05,5.05,),
	"leadPhi"    : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",33,-3.21,3.21),
	"secondPt"   : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s""GeV/c",70,0,35),
	"secondEta"  : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",101,-5.05,5.05,),      
	"secondPhi"  : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",33,-3.21,3.21),
	"leadNeutPt" : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","GeV/c",70,0,35),
	"leadNeutEta": ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",101,-5.05,5.05,),      
	"leadNeutPhi": ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",33,-3.21,3.21),
	"leadEmPt"   : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","GeV/c",70,0,35),
	"leadEmEta"  : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",101,-5.05,5.05,),
	"leadEmPhi"  : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",33,-3.21,3.21),
	"leadChPt"   : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s","GeV/c",70,0,35),
	"leadChEta"  : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",101,-5.05,5.05,),
	"leadChPhi"  : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",33,-3.21,3.21),
	"leadFrac"   : ("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",100,0,1.),

	"dRLeadCent" :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.5),
	"dRLead2nd"  :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.5),
	"dRMean"     :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.5),
	"dRMeanNeut" :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.5),
	"dRMeanEm"   :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.5),
	"dRMeanCh"   :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",50,0,0.5),
	
	"ptD"        :("%(name)s %(hname)s%(jetbin)s;%(hname)s %(unitx)s;Jets %(unity)s",100,0,1.),
        }

    # --------------------------------------------------------------------------------------
    def __init__(self,name,vtxbins=None,ptbins=None,etabins=None,vtxlabels=None,ptlabels=None,etalabels=None,reweight=None,
                 jetIdMva=None):
        """
        """
        from ROOT import PileupJetIdentifier
        ## deal with nvtx/pt/eta binning and labels
        self.name = name
        self.vtxbins = vtxbins
        self.ptbins = ptbins
        self.etabins = etabins
        self.reweight = reweight
        
        if self.vtxbins and not vtxlabels:
            self.vtxlabels = [ "_vtx%s" % l for l in  mkBinLabels(self.vtxbins) ]
        elif vtxlabels:
            self.vtxlabels = vtxlabels
        else:
            self.vtxlabels = [""]

        if self.ptbins and not ptlabels:
            self.ptlabels = [ "_pt%s" % l for l in mkBinLabels(self.ptbins) ]
        elif ptlabels:
            self.ptlabels = ptlabels
        else:
            self.ptlabels = [""]
            
        if self.etabins and not etalabels:
            self.etalabels = [ "_eta%s" % l for l in  mkBinLabels(self.etabins) ]
        elif etalabels:
            self.etalabels = etalabels
        else:
            self.etalabels = [""]

        ## book histograms and keep track of what needs to be filled
        self.fillers = []
        if jetIdMva:
            print jetIdMva
            self.identifier = PileupJetIdentifier(*jetIdMva)
            self.jetIdMva = jetIdMva
            self.runMva = True
        else:
            self.identifier = PileupJetIdentifier()
            self.jetIdMva = ()
            self.runMva = True
        for name,proto in self.prototypes.iteritems():
            self.fillers.append( (self.bookHistos(name,proto), name) )

        ## call the Histograms constructor to get everything registered 
        super (PileupJetHistograms,self).__init__ (self.name)
        
    # --------------------------------------------------------------------------------------
    def bookHistos(self,hname,proto):
        title = proto[0]
        if type(proto[1]) == list:
            aux = proto[1]
            args = proto[2:]
        elif type(proto[1]) == str:
            aux = {"unit":proto[1]}
            args = proto[2:]
        else:
            aux = { "unit" : "" }
            args = proto[1:]
        t = tuple( tuple(
            tuple( TH1F("%s%s%s%s_%s" % (self.name,eta,vtx,pt,hname),
                        formatTitle(title, dict({"name":self.name, "hname":hname, "vtxbin": vtx, "ptbin":pt, "etabin":eta}.items()+aux.items())),*args)
                    for eta in self.etalabels ) for pt in self.ptlabels ) for vtx in self.vtxlabels) 
        ## self.__setattr__("list_%s" % hname) = t
        self.addHistos(t)
        return t

    # --------------------------------------------------------------------------------------
    def addHisto(self,h):
        fillTitle(h)
        self.__setattr__(h.GetName().replace("%s_"%self.name,""),h)

    # --------------------------------------------------------------------------------------
    def addHistos(self,histos):
        if type(histos[0]) == tuple or type(histos[0]) == list:
            for hi in histos:
                for h in hi: self.addHistos(h)
        else:
            for h in histos: self.addHisto(h)

    # --------------------------------------------------------------------------------------
    def fillJet(self,jet,nvtx=0):
        ptbin = 0
        etabin = 0
        if self.ptbins:  ptbin  = findBin(self.ptbins,jet.pt())
        if self.etabins: etabin = findBin(self.etabins,abs(jet.eta()))
        if self.vtxbins: vtxbin = findBin(self.vtxbins,nvtx)

        if ptbin < 0 or etabin < 0 or vtxbin < 0:
            return

        w = 1.
        if self.reweight:
            xbin = findBin(self.reweight[1],getattr(jet,self.reweight[0])())
            w = self.reweight[2][xbin]
        
        try:
            puid = jet.puIdentifier
        except:
            puid = self.identifier
            try:
                puid.computeIdVariables(jet.sourcePtr().get(),0,self.runMva)
            except:
                puid.computeIdVariables(jet,0,self.runMva)
            jet.puIdentifier = PileupJetIdentifier(puid)
            
        for t,m in self.fillers:
            t[vtxbin][ptbin][etabin].Fill( getattr(puid,m)(), w )
            
    # --------------------------------------------------------------------------------------
    def fillEvent(self,event,vertices):
        nvtx = len (vertices)
        for jet in event:
            self.fillJet(jet,nvtx)
