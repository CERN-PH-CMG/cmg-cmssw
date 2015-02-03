#include "TauAnalysis/SVFitStandAlone2011/interface/svFitAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include <Math/VectorUtil.h>
#include <limits>

namespace SVfit_namespace {

  // Adapted for our vector types from TVector3 class
  reco::Candidate::Vector rotateUz(const math::RThetaPhiVector& toRotate,
				   const reco::Candidate::Vector& newUzVector)
  {
    //std::cout << "<rotateUz>:" << std::endl;

    // NB: newUzVector must be a unit vector !
    Double_t u1 = newUzVector.X();
    Double_t u2 = newUzVector.Y();
    Double_t u3 = newUzVector.Z();
    Double_t up = u1*u1 + u2*u2;

    Double_t fX = toRotate.X();
    Double_t fY = toRotate.Y();
    Double_t fZ = toRotate.Z();

    if ( up ) {
      up = TMath::Sqrt(up);
      Double_t px = fX,  py = fY,  pz = fZ;
      fX = (u1*u3*px - u2*py + u1*up*pz)/up;
      fY = (u2*u3*px + u1*py + u2*up*pz)/up;
      fZ = (u3*u3*px -    px + u3*up*pz)/up;
    } else if ( u3 < 0. ) {
      fX = -fX;
      fZ = -fZ;
    } else {}; // phi = 0, theta = pi

    //std::cout << " fX = " << fX << ", fY = " << fY << ", fZ = " << fZ << std::endl;

    return reco::Candidate::Vector(fX, fY, fZ);
  }

  reco::Candidate::LorentzVector boostToCOM(
      const reco::Candidate::LorentzVector& comSystem,
      const reco::Candidate::LorentzVector& p4ToBoost) {
    reco::Candidate::Vector boost = comSystem.BoostToCM();
    return ROOT::Math::VectorUtil::boost(p4ToBoost, boost);
  }

  double gjAngleFromX(double x, double visMass, double pVis_rf, double enVis_lab) {
    double enVis_rf = energyFromMomentum(pVis_rf, visMass);
    double beta = TMath::Sqrt(1. - square(tauLeptonMass*x/enVis_lab));
    double cosGjAngle = (tauLeptonMass*x - enVis_rf)/(pVis_rf*beta);
    double gjAngle = TMath::ACos(cosGjAngle);
    return gjAngle;
  }

  double pVisRestFrame(double tauVisMass, double tauNuNuMass)
  {
    //std::cout << "<pVisRestFrame>:" << std::endl;
    //std::cout << " tauVisMass = " << tauVisMass << std::endl;
    //std::cout << " tauNuNuMass = " << tauNuNuMass << std::endl;

    double pVis = TMath::Sqrt((tauLeptonMass2 - square(tauVisMass + tauNuNuMass))
			      *(tauLeptonMass2 - square(tauVisMass - tauNuNuMass)))/(2*tauLeptonMass);
    //std::cout << "--> pVis = " << pVis << std::endl;

    return pVis;
  }

  double gjAngleToLabFrame(double pVisRestFrame, double gjAngle, double pVisLabFrame)
  {
    //std::cout << "<gjAngleToLabFrame>:" << std::endl;

    // Get the compenent of the rest frame momentum perpindicular to the tau
    // boost direction. This quantity is Lorentz invariant.
    double pVisRestFramePerp = pVisRestFrame*TMath::Sin(gjAngle);
    //std::cout << " pVisRestFramePerp = " << pVisRestFramePerp << std::endl;

    // Determine the corresponding opening angle in the LAB frame
    double gjAngleLabFrame = TMath::ASin(pVisRestFramePerp/pVisLabFrame);
    //std::cout << "--> gjAngleLabFrame = " << gjAngleLabFrame << std::endl;

    return gjAngleLabFrame;
  }

  double tauMomentumLabFrame(double tauVisMass, double pVisRestFrame, double gjAngle, double pVisLabFrame)
  {
    //std::cout << "<tauMomentumLabFrame>:" << std::endl;

    // Determine the corresponding opening angle in the LAB frame
    double angleVisLabFrame = gjAngleToLabFrame(pVisRestFrame, gjAngle, pVisLabFrame);
    //std::cout << " angleVisLabFrame = " << angleVisLabFrame << std::endl;

    // Get the visible momentum perpindicular/parallel to the tau boost direction in the LAB
    double pVisLabFramePara = pVisLabFrame*TMath::Cos(angleVisLabFrame);
    //std::cout << " pVisLabFramePara = " << pVisLabFramePara << std::endl;

    // Now use the Lorentz equation for pVis along the tau direction to solve for
    // the gamma of the tau boost.
    double pVisRestFramePara = pVisRestFrame*TMath::Cos(gjAngle);
    //std::cout << " pVisRestFramePara = " << pVisRestFramePara << std::endl;
    double eVisRestFrame = TMath::Sqrt(square(tauVisMass) + square(pVisRestFrame));
    //std::cout << " eVisRestFrame = " << eVisRestFrame << std::endl;

    double gamma = (eVisRestFrame * TMath::Sqrt(square(eVisRestFrame) + square(pVisLabFramePara)
      - square(pVisRestFramePara)) - pVisRestFramePara*pVisLabFramePara)/(square(eVisRestFrame) - square(pVisRestFramePara));
    //std::cout << " gamma = " << gamma << std::endl;

    double pTauLabFrame = TMath::Sqrt(square(gamma) - 1)*tauLeptonMass;
    //std::cout << "--> pTauLabFrame = " << pTauLabFrame << std::endl;

    return pTauLabFrame;
  }

  reco::Candidate::Vector tauDirection(
      const reco::Candidate::Vector& pVisLabFrame, double angleVisLabFrame,
      double phiLab) {
    // The direction is defined using polar coordinates in a system where the visible energy
    // defines the Z axis.
    math::RThetaPhiVector tauDirectionVisibleSystem(1.0, angleVisLabFrame, phiLab);

    // Rotate into the LAB coordinate system
    return rotateUz(tauDirectionVisibleSystem, pVisLabFrame.Unit());
  }

  reco::Candidate::LorentzVector tauP4(
      const reco::Candidate::Vector& tauDirection, double tauMomentumLabFrame) {
    //std::cout << "<tauP4>:" << std::endl;

    // NB: tauDirection must be a unit vector !
    //reco::Candidate::Vector tauMomentum = tauDirection*tauMomentumLabFrame;

    reco::Candidate::LorentzVector tauP4LabFrame =
      reco::Candidate::LorentzVector(
          tauDirection.x()*tauMomentumLabFrame,
          tauDirection.y()*tauMomentumLabFrame,
          tauDirection.z()*tauMomentumLabFrame,
          TMath::Sqrt(tauMomentumLabFrame*tauMomentumLabFrame
            + tauLeptonMass*tauLeptonMass));

      //math::PtEtaPhiMLorentzVector(tauMomentum.rho(), tauMomentum.eta(), tauMomentum.phi(), tauLeptonMass));
    //std::cout << "--> tauMomentum: E = " << tauP4LabFrame.energy() << ","
    //          << " eta = " << tauP4LabFrame.eta() << ", phi = " << tauP4LabFrame.phi()*180./TMath::Pi() << ","
    //          << " mass = " << tauP4LabFrame.mass() << std::endl;

    return tauP4LabFrame;
  }

//
//-------------------------------------------------------------------------------
//

  double logGaussian(double residual, double sigma)
  {
    if ( sigma > 0. ) {
      return -0.5*TMath::Log(2*TMath::Pi()*square(sigma)) - 0.5*square(residual/sigma);
    } else {
      edm::LogError ("logGaussian")
	<< " Parameter sigma must not be zero !!";
      return std::numeric_limits<float>::min();
    }
  }

}
