import ROOT

from math import sqrt,pow,sin,cos,tan
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,isNewerThan
from CMGTools.HToZZTo4Leptons.tools.fullPath import getFullPath
import copy
ROOT.gSystem.Load("libCMGToolsHToZZTo4Leptons")

class MassErrors(object):
    def __init__(self,isData = True,doComponents = True,scaleErrors = True ):
        self.is44X = cmsswIs44X()
        self.doComponents = doComponents
        if scaleErrors:
            self.rootfile = ROOT.TFile(getFullPath('data/ebe_scalefactors.root'))
            if self.is44X:
                if isData:
                    self.muonHisto = self.rootfile.Get('mu_reco42x')
                    self.eleHisto = self.rootfile.Get('el_reco42x')
                else:
                    self.muonHisto = self.rootfile.Get('mu_mc42x')
                    self.eleHisto = self.rootfile.Get('el_mc42x')
            else:
                if isData:
                    self.muonHisto = self.rootfile.Get('mu_reco53x')
                    self.eleHisto = self.rootfile.Get('el_reco53x')
                else:
                    self.muonHisto = self.rootfile.Get('mu_mc53x')
                    self.eleHisto = self.rootfile.Get('el_mc53x')
                


    def calculateElectronMatrix(self,electron,matrix,offset):
        p = electron.p()

        if electron.ecalDriven():
            dp = electron.sourcePtr().p4Error(1)
        else:    
            if self.is44X:
               ecalEnergy = electron.sourcePtr().ecalEnergy() 
            else:   
               ecalEnergy = electron.sourcePtr().correctedEcalEnergy() 

            err2 = 0.0
            if electron.sourcePtr().isEB():
                err2 += (5.24e-02*5.24e-02)/ecalEnergy  
                err2 += (2.01e-01*2.01e-01)/(ecalEnergy*ecalEnergy)
                err2 += 1.00e-02*1.00e-02
            elif electron.sourcePtr().isEE():
                err2 += (1.46e-01*1.46e-01)/ecalEnergy  
                err2 += (9.21e-01*9.21e-01)/(ecalEnergy*ecalEnergy)
                err2 += 1.94e-03*1.94e-03

            dp = ecalEnergy * sqrt(err2)


        mtrx = ROOT.TMatrixDSym(3)

        
        mtrx[0][0]=(dp*electron.px()/p)*(dp*electron.px()/p)
        mtrx[0][1]=(dp*electron.px()/p)*(dp*electron.py()/p)
        mtrx[0][2]=(dp*electron.px()/p)*(dp*electron.pz()/p)
        mtrx[1][0]=mtrx(0,1)
        mtrx[1][1]=(dp*electron.py()/p)*(dp*electron.py()/p)
        mtrx[1][2]=(dp*electron.py()/p)*(dp*electron.pz()/p)
        mtrx[2][0]=mtrx(0,2)
        mtrx[2][1]=mtrx(1,2)
        mtrx[2][2]=(dp*electron.pz()/p)*(dp*electron.pz()/p)

        for i in range(0,3):
            for j in range(0,3):
                matrix[offset+i][offset+j] = mtrx[i][j]


    def calculateMuonMatrix(self,muon,matrix,offset):
        for i in range(0,3):
            for j in range(0,3):
                matrix[offset+i][offset+j] = muon.covarianceMatrix()(i+3,j+3)



    def calculatePhotonMatrix(self,photon,matrix,offset):
        dp = self.getEnergyResolution(photon.energy(), photon.eta())
        p=photon.energy()


        mtrx = ROOT.TMatrixDSym(3)


        mtrx[0][0]=(dp*photon.px()/p)*(dp*photon.px()/p)
        mtrx[0][1]=(dp*photon.px()/p)*(dp*photon.py()/p)
        mtrx[0][2]=(dp*photon.px()/p)*(dp*photon.pz()/p)
        mtrx[1][0]=mtrx(0,1)
        mtrx[1][1]=(dp*photon.py()/p)*(dp*photon.py()/p)
        mtrx[1][2]=(dp*photon.py()/p)*(dp*photon.pz()/p)
        mtrx[2][0]=mtrx(0,2)
        mtrx[2][1]=mtrx(1,2)
        mtrx[2][2]=(dp*photon.pz()/p)*(dp*photon.pz()/p)


        for i in range(0,3):
            for j in range(0,3):
                matrix[offset+i][offset+j] = mtrx[i][j]


    def calculateLeptonMatrix(self,lepton,matrix,offset):
        if abs(lepton.pdgId())==11:
            return self.calculateElectronMatrix(lepton,matrix,offset)
        else:
            return self.calculateMuonMatrix(lepton,matrix,offset)


    def getEnergyResolution(self,energy,eta):

      if abs(eta)<1.48:
          C=0.35/100
          S=5.51/100
          N=98./1000
      else:
          C=0
          S=12.8/100
          N=440./1000.   

      return sqrt(C*C*energy*energy + S*S*energy + N*N);

    def errorScale(self,lepton):
        if abs(lepton.pdgId())==11:
            histo=self.eleHisto
        else:
            histo=self.muonHisto
        binx = histo.GetXaxis().FindBin(lepton.pt())
        if binx>histo.GetNbinsX():
            binx = histo.GetNbinsX()
        biny = histo.GetYaxis().FindBin(abs(lepton.eta()))
        if biny>histo.GetNbinsY():
            biny = histo.GetNbinsX()
        scalefactor=histo.GetBinContent(binx,biny)
        return scalefactor
        
    def calculate(self,fourLepton):

        N = len(fourLepton.daughterLeptons()+fourLepton.daughterPhotons())    
        NDIM = N*3
        bigCov = ROOT.TMatrixDSym(NDIM)
        jacobian = ROOT.TMatrixD(1,NDIM)
        offset=0
#        print 'DIMENSIONS',NDIM, 'DAUGHTERS',N
#        print 'leptons+photons',fourLepton.daughterLeptons()+fourLepton.daughterPhotons()
#        print 'Empty Covariance Matrix'
#        bigCov.Print()
        
        for lepton in fourLepton.daughterLeptons():
            self.calculateLeptonMatrix(lepton,bigCov,offset)
            jacobian[0][offset] = (fourLepton.energy()*(lepton.px()/lepton.energy()) - fourLepton.px())/fourLepton.mass()
            jacobian[0][offset+1] = (fourLepton.energy()*(lepton.py()/lepton.energy()) - fourLepton.py())/fourLepton.mass()
            jacobian[0][offset+2] = (fourLepton.energy()*(lepton.pz()/lepton.energy()) - fourLepton.pz())/fourLepton.mass()
            offset=offset+3                           

        for lepton in fourLepton.daughterPhotons():
            self.calculatePhotonMatrix(lepton,bigCov,offset)
            jacobian[0][offset] = (fourLepton.energy()*(lepton.px()/lepton.energy()) - fourLepton.px())/fourLepton.mass()
            jacobian[0][offset+1] = (fourLepton.energy()*(lepton.py()/lepton.energy()) - fourLepton.py())/fourLepton.mass()
            jacobian[0][offset+2] = (fourLepton.energy()*(lepton.pz()/lepton.energy()) - fourLepton.pz())/fourLepton.mass()
            offset=offset+3                           



###CALCULATE RAW           
        bigCov2 = copy.copy(bigCov)
        jacobian2 = copy.copy(jacobian)
        
        massCovRAW = bigCov2.Similarity(jacobian2)
        dm2RAW = massCovRAW(0,0)
        if dm2RAW<=0:
            dm2RAW=0
            
        fourLepton.massErrRaw = sqrt(dm2RAW)
        if not self.doComponents:
            fourLepton.massErr = sqrt(dm2RAW)


        errs = []
        offset=0
        for i in range(0,N):
            bigCovOne = ROOT.TMatrixDSym(NDIM)
            for ir in range(0,3):
                for ic in range(0,3):
                    bigCovOne[offset+ir][offset+ic]=bigCov(offset+ir,offset+ic)
            dmOneCov = bigCovOne.Similarity(jacobian)
            dmOne2 =dmOneCov(0,0) 
            if dmOne2>0.0:
                errs.append(sqrt(dmOne2))
            else:    
                errs.append(0.0)
            offset=offset+3     
#            print 'CovarianceMatrix Per Lepton'
#            bigCovOne.Print()

        #Now that the components have been calculated
        #let's scale them.Recall the way we do it here
        #is to have the 4 leptons first. Then the FSR photons
#        print 'Errors before scale',errs
        for Ni,lepton in enumerate(fourLepton.daughterLeptons()):
            errs[Ni]=errs[Ni]*self.errorScale(lepton)

#        print 'Errors after scale',errs
                
            #Now recalculate the error
        dm2=0.0
        for component in errs: 
            dm2=dm2+component*component

#        print 'error=',dm2    
        if dm2>0:
            fourLepton.massErr = sqrt(dm2)
        else:
            fourLepton.massErr = 0




        
              
