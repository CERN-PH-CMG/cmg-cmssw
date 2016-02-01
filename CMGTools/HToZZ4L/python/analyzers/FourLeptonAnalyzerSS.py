from math import *
from CMGTools.HToZZ4L.analyzers.FourLeptonAnalyzer import *

        
class FourLeptonAnalyzerSS( FourLeptonAnalyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(FourLeptonAnalyzerSS,self).__init__(cfg_ana,cfg_comp,looperName)
        self.tag = cfg_ana.tag

    def declareHandles(self):
        super(FourLeptonAnalyzerSS, self).declareHandles()

    def beginLoop(self, setup):
        super(FourLeptonAnalyzerSS,self).beginLoop(setup)
        self.counters.addCounter('FourLepton')
        count = self.counters.counter('FourLepton')
        count.register('all events')


    #For the good lepton preselection redefine the thingy so that leptons are loose    
    def leptonID(self,lepton):
        return self.leptonID_loose(lepton)

    def zSorting(self,Z1,Z2):
        return True


    #Redefine the QUADS so Z2 is SF/SS!!!
    def findOSSFQuads(self, leptons):
        '''Make combinatorics and make permulations of four leptons
           Cut the permutations by asking Z1 nearest to Z and also 
           that plus is the first
           Include FSR if in cfg file
        '''
        out = []
        for l1, l2,l3,l4 in itertools.permutations(leptons, 4):
            if (l1.pdgId()+l2.pdgId())!=0: 
                continue;
            if (l3.pdgId()!=l4.pdgId()):
                continue;
            if (l3.pt()<l4.pt()): # don't make the same pair twice
                continue;
            if (l1.pdgId()<l2.pdgId())!=0: 
                continue;
            quadObject =DiObjectPair(l1, l2,l3,l4)
            if not self.zSorting(quadObject.leg1,quadObject.leg2):
                continue;
            out.append(quadObject)

        return out




    def fourLeptonIsolation(self,fourLepton):
        ##Fancy! Here apply only tight ID on Z1 and no ID in Z2
        for l in [fourLepton.leg1.leg1,fourLepton.leg1.leg2]:
            if not self.leptonID_tight(l):
                return False
            if abs(l.pdgId())==11:
                if not (self.electronIsolation(l)):
                    return False
            if abs(l.pdgId())==13:
                if not self.muonIsolation(l):
                    return False
        return True   

    def stupidCut(self,fourLepton):
        #if not 4mu/4e  pass 
        if abs(fourLepton.leg1.leg1.pdgId())!=abs(fourLepton.leg2.leg1.pdgId()):
            return True
        leptons  = [ fourLepton.leg1.leg1, fourLepton.leg1.leg2, fourLepton.leg2.leg1, fourLepton.leg2.leg2 ]
        #def prt(quad):
        #    return ",".join("%d" % (leptons.index(l)+1) for l in quad.daughterLeptons())
        #print "\nNominal, mZ1 %.3f, mZ2 %.3f: %s" % (fourLepton.leg1.M(),fourLepton.leg2.M(),prt(fourLepton))
        quads = []
        for quad in self.findOSSFQuads(leptons): 
            # skip self
            if fourLepton.leg1.leg1 == quad.leg1.leg1 and fourLepton.leg1.leg2 == quad.leg1.leg2 and fourLepton.leg2.leg1 == quad.leg2.leg1:
                continue

            # skip those that have a worse Z1 mass than the nominal
            if abs(fourLepton.leg1.M()-91.1876) < abs(quad.leg1.M()-91.1876):
                continue

            #print "Found better alternate, mZ1 %.3f, mZ2 %.3f: %s" % (quad.leg1.M(),quad.leg2.M(),prt(quad))
            if quad.leg2.M() < 12:
                #print "Fails mZ2 cut"
                return False

        return True
     
