import ROOT
ROOT.gSystem.Load("libFWCoreFWLite")
ROOT.AutoLibraryLoader.enable()


class PyJet(object):
    def __init__(self,p4):
        self.LV = ROOT.math.XYZTLorentzVector(p4.px(),p4.py(),p4.pz(),p4.energy())
        self.rawF = 1.0
        self.matched=0
        
        self.chargedHadronEnergy = 0.0
        self.neutralHadronEnergy = 0.0
        self.photonEnergy = 0.0
        self.hfEMEnergy = 0.0
        self.hfHADEnergy = 0.0
        self.muonEnergy = 0.0
        self.electronEnergy = 0.0
        self.btag=0.0

    def chargedHadronEnergyFraction(self):
        return self.chargedHadronEnergy/(self.LV.energy()*self.rawF)

    def neutralHadronEnergyFraction(self):
        return self.neutralHadronEnergy/((self.LV.energy()*self.rawF))

    def photonEnergyFraction(self):
        return self.photonEnergy/((self.LV.energy()*self.rawF))

    def HFHadronEnergyFraction(self):
        return self.hfHADEnergy/((self.LV.energy()*self.rawF))

    def HFEMEnergyFraction(self):
        return self.hfEMEnergy/((self.LV.energy()*self.rawF))

    def muonEnergyFraction(self):
        return self.muonEnergy/((self.LV.energy()*self.rawF))

    def electronEnergyFraction(self):
        return self.electronEnergy/((self.LV.energy()*self.rawF))

    def leptonEnergyFraction(self):
        return (self.electronEnergy+self.muonEnergy)/((self.LV.energy()*self.rawF))


    def setConstituents(self,constituents):
        self.constituents = constituents

    def bTag(self):
        return self.btag
        

    def p4(self):
        return self.LV


    def setP4(self,p4):
        self.LV = p4

    def numberOfDaughters(self):
        return len(constituents)

    def daughter(self,i):
        if i<len(constituents):
            return constituents[i]
        else:
            return None

    def rawFactor(self):
        return self.rawF

    def setRawFactor(self,factor):
        self.rawF=factor


    def jetArea(self):
            return self.area

    def __getattr__(self, name):
        return getattr(self.LV,name)



class PyJetToolbox(object):
    
    def __init__(self,collection):
        self.collection = collection
        self.p4s = ROOT.std.vector("math::XYZTLorentzVector")()
        for p in collection:
            self.p4s.push_back(p.p4())


        self.doMassDrop = False
        self.doPrunning = False
        self.doSubjets = False
        self.doSoftDrop = False
        self.doNTau = False
    def setInterface(self,doArea,ktpower,rparam,active_area_repeats=1,ghost_area = 0.01,ghost_eta_max = 5.0,rho_eta_max = 4.4):        
        if doArea:
            self.interface = ROOT.cmg.FastJetInterface(self.p4s,ktpower,rparam,active_area_repeats,ghost_area,ghost_eta_max,rho_eta_max)
        else:    
            self.interface = ROOT.cmg.FastJetInterface(self.p4s,ktpower,rparam)


    def setMassDrop(self,activate,mu=0.667 ,y=0.08):
        self.doMassDrop = activate
        self.massdrop = {'mu':mu,'y':y}


    def setSubjets(self,activate,style = 'inc',setting = 2):
        self.doSubjets = activate
        self.subjets = {'style':style,'setting':setting}


    def setPruning(self,activate,zcut = 0.1,rcutfactor = 0.5):
        self.doPrunning = activate       
        self.prunning = {'zcut':zcut,'rcutfactor':rcutfactor}

    def setSoftDrop(self,activate,beta=0.0,zcut=0.1,R0=0.8):
        self.doSoftDrop = activate
        self.softdrop = {'beta':beta,'zcut':zcut,'R0':R0}


    def setNtau(self,activate,NMAX = 4,measureDef = 0 , axesDef = 6 , beta= 1.0 , R0 = 0.8 , Rcutoff = -999.0,akAxisR0 = -999.0, nPass = -999 ):
        self.doNTau = activate
        self.ntau = {'NMAX':NMAX,'measureDef':measureDef,'axesDef':axesDef, 'beta':beta,'R0':R0,'Rcutoff':Rcutoff,'akAxesR0':-999.0, 'nPass':-999}
        

    def convert(self,lorentzVectors,isFat = False,isJet=True):
        output = []

        for LV in lorentzVectors:
            output.append(PyJet(LV))
        for i,jet in enumerate(output):
            jet.area = self.interface.getArea(isJet,i)
            jet.constituents=[]
            constituents = self.interface.getConstituents(isJet,i)
            for c in constituents:
                constituent = self.collection[c]
                jet.constituents.append(constituent)
                if abs(constituent.pdgId()) ==211:
                    jet.chargedHadronEnergy=jet.chargedHadronEnergy+constituent.energy()
                elif constituent.pdgId() ==22:
                    jet.photonEnergy=jet.photonEnergy+constituent.energy()
                elif constituent.pdgId() ==130:
                    jet.neutralHadronEnergy=jet.neutralHadronEnergy+constituent.energy()
                elif constituent.pdgId() ==1:
                    jet.hfHADEnergy=jet.hfHADEnergy+constituent.energy()

                elif constituent.pdgId() ==2:
                    jet.hfEMEnergy=jet.hfEMEnergy+constituent.energy()
                elif abs(constituent.pdgId())==11:
                    jet.electronEnergy=jet.electronEnergy+constituent.energy()
                elif abs(constituent.pdgId())==13:
                    jet.muonEnergy=jet.muonEnergy+constituent.energy()                  
            if isFat:
                if self.doPrunning:
                    self.interface.prune(isJet,self.prunning['zcut'],self.prunning['rcutfactor'])
                    jet.prunedJet = self.convert(self.interface.get(isJet),False,isJet)
                if self.doSoftDrop:
                    self.interface.softDrop(True,self.softdrop['beta'],self.softdrop['zcut'],self.softdrop['R0'])
                    jet.softDropJet = self.convert(self.interface.get(False),False,True)[i]
                if self.doSubjets:
                    if self.subjets['style'] == 'inc':
                        self.interface.makeSubJets(i,self.subjets['setting'])
                        jet.subjets = self.convert(self.interface.get(True),False,False)
                    else:    
                        self.interface.makeSubJetsUpTo(i,self.subjets['setting'])
                        jet.subjets = self.convert(self.interface.get(True),False,False)
                    if self.doNTau:
                        jet.Ntau = self.interface.nSubJettiness(i,self.ntau['NMAX'],self.ntau['measureDef'],self.ntau['axesDef'],self.ntau['beta'],self.ntau['R0'],self.ntau['Rcutoff'],self.ntau['akAxesR0'],self.ntau['nPass'])
                if self.doMassDrop:
                    mu= ROOT.Double(self.massdrop['mu'])
                    y= ROOT.Double(self.massdrop['y'])
                    jet.massDropTag =  self.interface.massDropTag(i,mu,y)
                    jet.massDrop = (mu,y)
        return output
            
    def inclusiveJets(self,ptmin = 0.0,isFat=True):
        self.interface.makeInclusiveJets(ptmin)
        return self.convert(self.interface.get(False),isFat)

    def exclusiveJets(self,r =0.1,isFat = True):
        self.interface.makeExclusiveJets(r)
        return self.convert(self.interface.get(False),isFat)

    def exclusiveJetsUpTo(self,N=2,isFat = True ):
        self.interface.makeExclusiveJetsUpTo(N)
        return self.convert(self.interface.get(False),isFat)






#from DataFormats.FWLite import Events, Handle

#pfH = Handle('std::vector<pat::PackedCandidate')
#jetsH = Handle('std::vector<pat::Jet')
#
#events=Events(
#'root://eoscms//eos/cms/store/cmst3/user/bachtis/CMG/vv.root'
#)
#
#
#
#from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi
#
#
#for ev in events:
#    ev.getByLabel('packedPFCandidates',pfH)
#    ev.getByLabel('slimmedJets',jetsH)
#    pf = pfH.product()
#    pfCHS = filter(lambda x: x.fromPV(0) , pf)
#    jetsDefault = jetsH.product()
#    if len(jetsDefault)==0:
#        continue
#    toolbox  = PyJetToolbox(pfCHS)
#
#
#    toolbox.setInterface(True,-1.0,0.4)
#    toolbox.setMassDrop(False)
#    toolbox.setSubjets(False,'inc',2)
#    toolbox.setPruning(False)
#    toolbox.setNtau(False)
#    toolbox.setSoftDrop(False)
#    jets=toolbox.inclusiveJets(30.0)


#    print 'OLD VS NEW'
#    print '----------'
#    for new in jets:
#        for old in jetsDefault:
#            if deltaR(new.eta(),new.phi(),old.eta(),old.phi())<0.2:
#                print 'New',new.pt(),new.eta(),new.phi(),new.mass(),new.jetArea(),'Old',old.pt(),old.eta(),old.phi(),old.mass(),old.jetArea(),'Uncorrected',old.correctedP4('Uncorrected').pt(),'L1',old.correctedP4('L1FastJet').pt(),'L2',old.correctedP4('L2Relative').pt(),'L3',old.correctedP4('L3Absolute').pt()
#                for s in  old.availableJECLevels():
#                    print s

#    print 'NEW VS OLD'
#    print '----------'
#    for old in jetsDefault:
#        for new in jets:
#            if deltaR(new.eta(),new.phi(),old.eta(),old.phi())<0.2:
#                print 'New',new.pt(),new.eta(),new.phi(),new.mass(),new.jetArea(),'Old',old.pt(),old.eta(),old.phi(),old.mass(),old.jetArea(),'Uncorrected',old.correctedP4('Uncorrected').pt(),'L1',old.correctedP4('L1FastJet').pt(),'L2',old.correctedP4('L2Relative').pt(),'L3',old.correctedP4('L3Absolute').pt()

       



