#include "TMath.h"
#include "TVector3.h"
#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > XYZTLorentzVectorD;
typedef XYZTLorentzVectorD LorentzVector;


double mR(LorentzVector const& ja, LorentzVector const& jb){
    //validated for 2011 variables
    const double A = ja.P();
    const double B = jb.P();
    const double az = ja.Pz();
    const double bz = jb.Pz();
  
    TVector3 jaT, jbT;
    jaT.SetXYZ(ja.px(),ja.py(),0.0);
    jbT.SetXYZ(jb.px(),jb.py(),0.0);
    const double ATBT = (jaT+jbT).Mag2();

    double temp = sqrt((A+B)*(A+B)-(az+bz)*(az+bz)-
                         (jbT.Dot(jbT)-jaT.Dot(jaT))*(jbT.Dot(jbT)-jaT.Dot(jaT))/(jaT+jbT).Mag2());

    const double mybeta = (jbT.Dot(jbT)-jaT.Dot(jaT))/
        TMath::Sqrt(ATBT*((A+B)*(A+B)-(az+bz)*(az+bz)));
    double mygamma = 1./sqrt(1.-mybeta*mybeta);

    //gamma times MRstar                                                                                                                                                                              
    temp *= mygamma;
    // protect against nan
    if(TMath::IsNaN(temp)) temp = -9999.;
    return temp;
}

double mRT(LorentzVector const& jaV, LorentzVector const& jbV, const LorentzVector& metV){
    ///validated for 2011 variables
    const double metMag = metV.Vect().R();
    
    double temp = metMag*(jaV.pt()+jbV.pt()) - metV.Vect().Dot(jaV.Vect()+jbV.Vect());
    temp /= 2.;

    temp = std::sqrt(temp);
    if(TMath::IsNaN(temp)) temp = -9999.;
    return temp;
}
