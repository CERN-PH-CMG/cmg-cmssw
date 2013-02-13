from HiggsAnalysis.CombinedLimit.PhysicsModel import *
from HiggsAnalysis.CombinedLimit.SMHiggsBuilder import SMHiggsBuilder
import ROOT, os

### This base class implements signal yields by production and decay mode
### Specific models can be obtained redefining getHiggsSignalYieldScale
class XsecCorrelation(PhysicsModel):
    "Float xsec ratio and one of the signal strengths together"
    def __init__(self):
        print 'XsecCorrelation initating'
        #SMLikeHiggsModel.__init__(self) # not using 'super(x,self).__init__' since I don't understand it
        PhysicsModel.__init__(self) # not using 'super(x,self).__init__' since I don't understand it
        self.floatMass = False        

    def setPhysicsOptions(self,physOptions):
        for po in physOptions:
            print 'XsecCorrelation with: %s',po 
            if po.startswith("higgsMassRange="):
                self.floatMass = True
                self.mHRange = po.replace("higgsMassRange=","").split(",")
                print 'The Higgs mass range:', self.mHRange
                if len(self.mHRange) != 2:
                    raise RuntimeError, "Higgs mass range definition requires two extrema."
                elif float(self.mHRange[0]) >= float(self.mHRange[1]):
                    raise RuntimeError, "Extrema for Higgs mass range defined with inverterd order. Second must be larger the first."

    def doParametersOfInterest(self):
        """Create POI out of signal strength and MH"""

        print 'XsecCorrelation creating POI'
        # --- Signal Strength and BSM contribution as POI --- 
        self.modelBuilder.doVar("mu_8TeV[1,0,3]")
        self.modelBuilder.doVar("mu_7TeV[1,0,3]")
        #self.modelBuilder.doVar("epsBSM[0,-1,1]")
        #self.modelBuilder.doVar('expr:mu_7TeV("@0*(@1+1)",mu_8TeV,epsBSM)')

        if self.floatMass:
            if self.modelBuilder.out.var("MH"):
                self.modelBuilder.out.var("MH").setRange(float(self.mHRange[0]),float(self.mHRange[1]))
                self.modelBuilder.out.var("MH").setConstant(False)
            else:
                self.modelBuilder.doVar("MH[%s,%s]" % (self.mHRange[0],self.mHRange[1])) 
            self.modelBuilder.doSet("POI",'mu_8TeV,mu_7TeV,MH')
        else:
            if self.modelBuilder.out.var("MH"):
                self.modelBuilder.out.var("MH").setVal(self.options.mass)
                self.modelBuilder.out.var("MH").setConstant(True)
            else:
                self.modelBuilder.doVar("MH[%g]" % self.options.mass) 
            self.modelBuilder.doSet("POI",'mu_8TeV,mu_7TeV')
        return 0

    def getYieldScale(self,bin,process):
        if not self.DC.isSignal[process]: return 1
        for sqrts in ['7TeV', '8TeV', '7', '8']:
            if sqrts in process:
                if(sqrts.find('TeV')<0): sqrts =sqrts+'TeV'
                return 'mu_%(sqrts)s'%locals()

xsecCorrelation=XsecCorrelation()
