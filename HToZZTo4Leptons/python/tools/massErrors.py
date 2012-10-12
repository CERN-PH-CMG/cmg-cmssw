import ROOT

from math import sqrt,pow,sin,cos,tan
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,isNewerThan
ROOT.gSystem.Load("libCMGToolsHToZZTo4Leptons")

class MassErrors(object):
    def __init__(self):
        self.is44X = cmsswIs44X() 


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

        
        mtrx[0][0]=pow(dp*electron.px()/p,2)
        mtrx[0][1]=pow(dp*electron.px()/p,2)*electron.py()/electron.px()
        mtrx[0][2]=pow(dp*electron.px()/p,2)*electron.pz()/electron.px()
        mtrx[1][0]=mtrx(0,1)
        mtrx[1][1]=pow(dp*electron.py()/p,2)
        mtrx[1][2]=pow(dp*electron.py()/p,2)*electron.pz()/electron.py()
        mtrx[2][0]=mtrx(0,2)
        mtrx[2][1]=mtrx(1,2)
        mtrx[2][2]=pow(dp*electron.pz()/p,2)

        for i in range(0,3):
            for j in range(0,3):
                matrix[offset+i][offset+j] = mtrx[i][j]


    def calculateMuonMatrix(self,muon,matrix,offset):
        for i in range(0,3):
            for j in range(0,3):
                matrix[offset+i][offset+j] = muon.covarianceMatrix()(i+3,j+3)



    def calculatePhotonMatrix(self,photon,matrix,offset):
        dp = self.getEnergyResolution(photon.energy(), photon.eta())
        p=photon.p()
        mtrx = ROOT.TMatrixDSym(3)

        
        mtrx[0][0]=pow(dp*photon.px()/p,2)
        mtrx[0][1]=pow(dp*photon.px()/p,2)*photon.py()/photon.px()
        mtrx[0][2]=pow(dp*photon.px()/p,2)*photon.pz()/photon.px()
        mtrx[1][0]=mtrx(0,1)
        mtrx[1][1]=pow(dp*photon.py()/p,2)
        mtrx[1][2]=pow(dp*photon.py()/p,2)*photon.pz()/photon.py()
        mtrx[2][0]=mtrx(0,2)
        mtrx[2][1]=mtrx(1,2)
        mtrx[2][2]=pow(dp*photon.pz()/p,2)

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

      return sqrt(C*C*energy*energy + S*S*energy*energy + N*N);


    def calculate(self,fourLepton):

        N = len(fourLepton.daughterLeptons()+fourLepton.daughterPhotons())    
        NDIM = N*3
        bigCov = ROOT.TMatrixDSym(NDIM)
        jacobian = ROOT.TMatrixD(1,NDIM)
        offset=0

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

   
        massCov = bigCov.Similarity(jacobian)
   
        dm2 = massCov(0,0)
        if dm2>0:
            return sqrt(dm2)
        else:
            return 0



        
              
