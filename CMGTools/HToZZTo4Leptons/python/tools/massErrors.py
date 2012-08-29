from ROOT import TMatrixDSym
import ROOT

from math import sqrt,pow,sin,cos,tan

class MassErrors(object):

    def energyError(self,energy,p):
        return sqrt(pow(p[0]/sqrt(energy),2)+pow(p[1]/energy,2)+pow(p[2],2))

    def calculateElectronError(self,electron):
        if not electron.ecalDriven():
            ecalEnergy = electron.sourcePtr().correctedEcalEnergy()
#            ecalEnergy = electron.ecalEnergy()
            if electron.sourcePtr().isEB():
                p=[5.24e-2,2.01e-1,1e-2]
                return ecalEnergy*self.energyError(ecalEnergy,p)
            elif electron.sourcePtr().isEE():
                p=[1.46e-1,9.21e-1,1.94e-3]
                return ecalEnergy*self.energyError(ecalEnergy,p)
                
        else:
            return electron.sourcePtr().p4Error(1)


    def calculateElectronMatrix(self,electron):
        mtrx = ROOT.TMatrixDSym(3)
        dp = self.calculateElectronError(electron)
        p = electron.p()

        
        mtrx[0][0]=pow(dp*electron.px()/p,2)
        mtrx[0][1]=pow(dp*electron.px()/p,2)*electron.py()/electron.px() 
        mtrx[0][2]=pow(dp*electron.px()/p,2)*electron.pz()/electron.px() 
        mtrx[1][0]=mtrx(0,1)
        mtrx[1][1]=pow(dp*electron.py()/p,2) 
        mtrx[1][2]=pow(dp*electron.py()/p,2)*electron.pz()/electron.py() 
        mtrx[2][0]=mtrx(0,2)
        mtrx[2][1]=mtrx(1,2)
        mtrx[2][2]=pow(dp*electron.pz()/p,2) 

        return mtrx


    def calculateMuonMatrix(self,muon):
        mtrx = TMatrixDSym(3)
        for i in range(0,3):
            for j in range(0,3):
                mtrx[i][j] = muon.covarianceMatrix()(i+3,j+3)

        return mtrx


    def calculateLeptonMatrix(self,lepton):
        if abs(lepton.pdgId())==11:
            return self.calculateElectronMatrix(lepton)
        else:
            return self.calculateMuonMatrix(lepton)

    def calculate(self,fourLepton):
        Lepton1 = ROOT.TVector3(fourLepton.leg1.leg1.px(),fourLepton.leg1.leg1.py(),fourLepton.leg1.leg1.pz())
        Lepton2 = ROOT.TVector3(fourLepton.leg1.leg2.px(),fourLepton.leg1.leg2.py(),fourLepton.leg1.leg2.pz())
        Lepton3 = ROOT.TVector3(fourLepton.leg2.leg1.px(),fourLepton.leg2.leg1.py(),fourLepton.leg2.leg1.pz())
        Lepton4 = ROOT.TVector3(fourLepton.leg2.leg2.px(),fourLepton.leg2.leg2.py(),fourLepton.leg2.leg2.pz())
        

        p1 = Lepton1.Mag()
        p2 = Lepton2.Mag()
        p3 = Lepton3.Mag()
        p4 = Lepton4.Mag()

        Lepton1Error = self.calculateLeptonMatrix(fourLepton.leg1.leg1) 
        Lepton2Error = self.calculateLeptonMatrix(fourLepton.leg1.leg2) 
        Lepton3Error = self.calculateLeptonMatrix(fourLepton.leg2.leg1) 
        Lepton4Error = self.calculateLeptonMatrix(fourLepton.leg2.leg2) 



        FourLeptonError = ROOT.TMatrixDSym(12)
        FourLeptonError.SetSub(0,0,Lepton1Error)	
        FourLeptonError.SetSub(3,3,Lepton2Error)	
        FourLeptonError.SetSub(6,6,Lepton3Error)	
        FourLeptonError.SetSub(9,9,Lepton4Error)	


        Jacobian = ROOT.TMatrixD(1,12)

        invMassSquared  = 2.*p1*p2*(1-cos(Lepton1.Angle(Lepton2))) + 2.*p1*p3*(1-cos(Lepton1.Angle(Lepton3))) + 2.*p1*p4*(1-cos(Lepton1.Angle(Lepton4)))
        invMassSquared = invMassSquared + 2.*p2*p3*(1-cos(Lepton2.Angle(Lepton3))) + 2.*p2*p4*(1-cos(Lepton2.Angle(Lepton4))) 
        invMassSquared = invMassSquared + 2.*p3*p4*(1-cos(Lepton3.Angle(Lepton4))) 

  
        dp1dp1x = Lepton1.x()/p1
        dp2dp2x = Lepton2.x()/p2
        dp3dp3x = Lepton3.x()/p3
        dp4dp4x = Lepton4.x()/p4
        dp1dp1y = Lepton1.y()/p1
        dp2dp2y = Lepton2.y()/p2
        dp3dp3y = Lepton3.y()/p3
        dp4dp4y = Lepton4.y()/p4
        dp1dp1z = Lepton1.z()/p1
        dp2dp2z = Lepton2.z()/p2
        dp3dp3z = Lepton3.z()/p3
        dp4dp4z = Lepton4.z()/p4

  

        dcos12dp1x = (Lepton2.x()*p1*p2 - Lepton1.x()*p2*p2*cos(Lepton1.Angle(Lepton2)))/(p1*p1*p2*p2)  							
        dcos13dp1x = (Lepton3.x()*p1*p3 - Lepton1.x()*p3*p3*cos(Lepton1.Angle(Lepton3)))/(p1*p1*p3*p3) 							
        dcos14dp1x = (Lepton4.x()*p1*p4 - Lepton1.x()*p4*p4*cos(Lepton1.Angle(Lepton4)))/(p1*p1*p4*p4) 							
        dcos23dp2x = (Lepton3.x()*p2*p3 - Lepton2.x()*p3*p3*cos(Lepton2.Angle(Lepton3)))/(p2*p2*p3*p3) 							
        dcos24dp2x = (Lepton4.x()*p2*p4 - Lepton2.x()*p4*p4*cos(Lepton2.Angle(Lepton4)))/(p2*p2*p4*p4) 							
        dcos34dp3x = (Lepton4.x()*p3*p4 - Lepton3.x()*p4*p4*cos(Lepton3.Angle(Lepton4)))/(p3*p3*p4*p4) 							
        dcos12dp1y = (Lepton2.y()*p1*p2 - Lepton1.y()*p2*p2*cos(Lepton1.Angle(Lepton2)))/(p1*p1*p2*p2) 							
        dcos13dp1y = (Lepton3.y()*p1*p3 - Lepton1.y()*p3*p3*cos(Lepton1.Angle(Lepton3)))/(p1*p1*p3*p3) 							
        dcos14dp1y = (Lepton4.y()*p1*p4 - Lepton1.y()*p4*p4*cos(Lepton1.Angle(Lepton4)))/(p1*p1*p4*p4) 							
        dcos23dp2y = (Lepton3.y()*p2*p3 - Lepton2.y()*p3*p3*cos(Lepton2.Angle(Lepton3)))/(p2*p2*p3*p3) 							
        dcos24dp2y = (Lepton4.y()*p2*p4 - Lepton2.y()*p4*p4*cos(Lepton2.Angle(Lepton4)))/(p2*p2*p4*p4) 							
        dcos34dp3y = (Lepton4.y()*p3*p4 - Lepton3.y()*p4*p4*cos(Lepton3.Angle(Lepton4)))/(p3*p3*p4*p4) 							
        dcos12dp1z = (Lepton2.z()*p1*p2 - Lepton1.z()*p2*p2*cos(Lepton1.Angle(Lepton2)))/(p1*p1*p2*p2) 							
        dcos13dp1z = (Lepton3.z()*p1*p3 - Lepton1.z()*p3*p3*cos(Lepton1.Angle(Lepton3)))/(p1*p1*p3*p3) 							
        dcos14dp1z = (Lepton4.z()*p1*p4 - Lepton1.z()*p4*p4*cos(Lepton1.Angle(Lepton4)))/(p1*p1*p4*p4) 							
        dcos23dp2z = (Lepton3.z()*p2*p3 - Lepton2.z()*p3*p3*cos(Lepton2.Angle(Lepton3)))/(p2*p2*p3*p3) 							
        dcos24dp2z = (Lepton4.z()*p2*p4 - Lepton2.z()*p4*p4*cos(Lepton2.Angle(Lepton4)))/(p2*p2*p4*p4) 							
        dcos34dp3z = (Lepton4.z()*p3*p4 - Lepton3.z()*p4*p4*cos(Lepton3.Angle(Lepton4)))/(p3*p3*p4*p4) 
        dcos12dp2x = (Lepton1.x()*p1*p2 - Lepton2.x()*p1*p1*cos(Lepton1.Angle(Lepton2)))/(p1*p1*p2*p2)  							
        dcos13dp3x = (Lepton1.x()*p1*p3 - Lepton3.x()*p1*p1*cos(Lepton1.Angle(Lepton3)))/(p1*p1*p3*p3) 							
        dcos14dp4x = (Lepton1.x()*p1*p4 - Lepton4.x()*p1*p1*cos(Lepton1.Angle(Lepton4)))/(p1*p1*p4*p4) 							
        dcos23dp3x = (Lepton2.x()*p2*p3 - Lepton3.x()*p2*p2*cos(Lepton2.Angle(Lepton3)))/(p2*p2*p3*p3) 							
        dcos24dp4x = (Lepton2.x()*p2*p4 - Lepton4.x()*p2*p2*cos(Lepton2.Angle(Lepton4)))/(p2*p2*p4*p4) 							
        dcos34dp4x = (Lepton3.x()*p3*p4 - Lepton4.x()*p3*p3*cos(Lepton3.Angle(Lepton4)))/(p3*p3*p4*p4) 							
        dcos12dp2y = (Lepton1.y()*p1*p2 - Lepton2.y()*p1*p1*cos(Lepton1.Angle(Lepton2)))/(p1*p1*p2*p2) 							
        dcos13dp3y = (Lepton1.y()*p1*p3 - Lepton3.y()*p1*p1*cos(Lepton1.Angle(Lepton3)))/(p1*p1*p3*p3) 							
        dcos14dp4y = (Lepton1.y()*p1*p4 - Lepton4.y()*p1*p1*cos(Lepton1.Angle(Lepton4)))/(p1*p1*p4*p4) 							
        dcos23dp3y = (Lepton2.y()*p2*p3 - Lepton3.y()*p2*p2*cos(Lepton2.Angle(Lepton3)))/(p2*p2*p3*p3) 							
        dcos24dp4y = (Lepton2.y()*p2*p4 - Lepton4.y()*p2*p2*cos(Lepton2.Angle(Lepton4)))/(p2*p2*p4*p4) 							
        dcos34dp4y = (Lepton3.y()*p3*p4 - Lepton4.y()*p3*p3*cos(Lepton3.Angle(Lepton4)))/(p3*p3*p4*p4) 							
        dcos12dp2z = (Lepton1.z()*p1*p2 - Lepton2.z()*p1*p1*cos(Lepton1.Angle(Lepton2)))/(p1*p1*p2*p2) 							
        dcos13dp3z = (Lepton1.z()*p1*p3 - Lepton3.z()*p1*p1*cos(Lepton1.Angle(Lepton3)))/(p1*p1*p3*p3) 							
        dcos14dp4z = (Lepton1.z()*p1*p4 - Lepton4.z()*p1*p1*cos(Lepton1.Angle(Lepton4)))/(p1*p1*p4*p4) 							
        dcos23dp3z = (Lepton2.z()*p2*p3 - Lepton3.z()*p2*p2*cos(Lepton2.Angle(Lepton3)))/(p2*p2*p3*p3) 							
        dcos24dp4z = (Lepton2.z()*p2*p4 - Lepton4.z()*p2*p2*cos(Lepton2.Angle(Lepton4)))/(p2*p2*p4*p4) 							
        dcos34dp4z = (Lepton3.z()*p3*p4 - Lepton4.z()*p3*p3*cos(Lepton3.Angle(Lepton4)))/(p3*p3*p4*p4) 	
        

        dgdp1x = 2.*dp1dp1x*p2*(1-cos(Lepton1.Angle(Lepton2))) + 2.*dp1dp1x*p3*(1-cos(Lepton1.Angle(Lepton3))) + 2.*dp1dp1x*p4*(1-cos(Lepton1.Angle(Lepton4)))
        dgdp1x = dgdp1x - 2.*p1*p2*dcos12dp1x - 2.*p1*p3*dcos13dp1x - 2.*p1*p4*dcos14dp1x 						
        dgdp1y = 2.*dp1dp1y*p2*(1-cos(Lepton1.Angle(Lepton2))) + 2.*dp1dp1y*p3*(1-cos(Lepton1.Angle(Lepton3))) + 2.*dp1dp1y*p4*(1-cos(Lepton1.Angle(Lepton4)))
        dgdp1y = dgdp1y - 2.*p1*p2*dcos12dp1y - 2.*p1*p3*dcos13dp1y - 2.*p1*p4*dcos14dp1y 							
        dgdp1z = 2.*dp1dp1z*p2*(1-cos(Lepton1.Angle(Lepton2))) + 2.*dp1dp1z*p3*(1-cos(Lepton1.Angle(Lepton3))) + 2.*dp1dp1z*p4*(1-cos(Lepton1.Angle(Lepton4)))
        dgdp1z = dgdp1z - 2.*p1*p2*dcos12dp1z - 2.*p1*p3*dcos13dp1z - 2.*p1*p4*dcos14dp1z 							
  
        dgdp2x = 2.*dp2dp2x*p1*(1-cos(Lepton1.Angle(Lepton2))) + 2.*dp2dp2x*p3*(1-cos(Lepton2.Angle(Lepton3))) + 2.*dp2dp2x*p4*(1-cos(Lepton2.Angle(Lepton4)))
        dgdp2x = dgdp2x -2.*p1*p2*dcos12dp2x - 2.*p2*p3*dcos23dp2x - 2.*p2*p4*dcos24dp2x 		
        dgdp2y = 2.*dp2dp2y*p1*(1-cos(Lepton1.Angle(Lepton2))) + 2.*dp2dp2y*p3*(1-cos(Lepton2.Angle(Lepton3))) + 2.*dp2dp2y*p4*(1-cos(Lepton2.Angle(Lepton4)))
        dgdp2y = dgdp2y - 2.*p1*p2*dcos12dp2y - 2.*p2*p3*dcos23dp2y - 2.*p2*p4*dcos24dp2y 		
        dgdp2z = 2.*dp2dp2z*p1*(1-cos(Lepton1.Angle(Lepton2))) + 2.*dp2dp2z*p3*(1-cos(Lepton2.Angle(Lepton3))) + 2.*dp2dp2z*p4*(1-cos(Lepton2.Angle(Lepton4)))
        dgdp2z = dgdp2z - 2.*p1*p2*dcos12dp2z - 2.*p2*p3*dcos23dp2z - 2.*p2*p4*dcos24dp2z 		
  
        dgdp3x = 2.*dp3dp3x*p1*(1-cos(Lepton1.Angle(Lepton3))) + 2.*dp3dp3x*p2*(1-cos(Lepton2.Angle(Lepton3))) + 2.*dp3dp3x*p4*(1-cos(Lepton3.Angle(Lepton4)))
        dgdp3x = dgdp3x - 2.*p1*p3*dcos13dp3x - 2.*p2*p3*dcos23dp3x - 2.*p3*p4*dcos34dp3x 		
        dgdp3y = 2.*dp3dp3y*p1*(1-cos(Lepton1.Angle(Lepton3))) + 2.*dp3dp3y*p2*(1-cos(Lepton2.Angle(Lepton3))) + 2.*dp3dp3y*p4*(1-cos(Lepton3.Angle(Lepton4)))
        dgdp3y = dgdp3y - 2.*p1*p3*dcos13dp3y - 2.*p2*p3*dcos23dp3y - 2.*p3*p4*dcos34dp3y 		
        dgdp3z = 2.*dp3dp3z*p1*(1-cos(Lepton1.Angle(Lepton3))) + 2.*dp3dp3z*p2*(1-cos(Lepton2.Angle(Lepton3))) + 2.*dp3dp3z*p4*(1-cos(Lepton3.Angle(Lepton4)))
        dgdp3z = dgdp3z - 2.*p1*p3*dcos13dp3z - 2.*p2*p3*dcos23dp3z - 2.*p3*p4*dcos34dp3z 		
  
        dgdp4x = 2.*dp4dp4x*p1*(1-cos(Lepton1.Angle(Lepton4))) + 2.*dp4dp4x*p2*(1-cos(Lepton2.Angle(Lepton4))) + 2.*dp4dp4x*p3*(1-cos(Lepton3.Angle(Lepton4)))
        dgdp4x = dgdp4x - 2.*p1*p4*dcos14dp4x - 2.*p2*p4*dcos24dp4x - 2.*p3*p4*dcos34dp4x 		
        dgdp4y = 2.*dp4dp4y*p1*(1-cos(Lepton1.Angle(Lepton4))) + 2.*dp4dp4y*p2*(1-cos(Lepton2.Angle(Lepton4))) + 2.*dp4dp4y*p3*(1-cos(Lepton3.Angle(Lepton4)))
        dgdp4y = dgdp4y - 2.*p1*p4*dcos14dp4y - 2.*p2*p4*dcos24dp4y - 2.*p3*p4*dcos34dp4y 		
        dgdp4z = 2.*dp4dp4z*p1*(1-cos(Lepton1.Angle(Lepton4))) + 2.*dp4dp4z*p2*(1-cos(Lepton2.Angle(Lepton4))) + 2.*dp4dp4z*p3*(1-cos(Lepton3.Angle(Lepton4)))
        dgdp4z = dgdp4z - 2.*p1*p4*dcos14dp4z - 2.*p2*p4*dcos24dp4z - 2.*p3*p4*dcos34dp4z 		
        
        Jacobian[0][0] = dgdp1x 	
        Jacobian[0][1] = dgdp1y 
        Jacobian[0][2] = dgdp1z 
        Jacobian[0][3] = dgdp2x 
        Jacobian[0][4] = dgdp2y 
        Jacobian[0][5] = dgdp2z 
        Jacobian[0][6] = dgdp3x 
        Jacobian[0][7] = dgdp3y 
        Jacobian[0][8] = dgdp3z 
        Jacobian[0][9] = dgdp4x 
        Jacobian[0][10]= dgdp4y 
        Jacobian[0][11]= dgdp4z 	


  
        C=ROOT.TMatrixD(FourLeptonError,ROOT.TMatrixD.kMultTranspose,Jacobian) 
        dg2=Jacobian*C
        dm2 = 0. 
        if invMassSquared!=0. :
            dm2=dg2(0,0)/(4.*invMassSquared)	
              
        return sqrt(dm2)
        
              
