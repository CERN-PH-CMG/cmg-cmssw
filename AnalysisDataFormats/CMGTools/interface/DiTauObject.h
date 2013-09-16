#ifndef DITAUOBJECT_H
#define DITAUOBJECT_H

#include "AnalysisDataFormats/CMGTools/interface/DiObject.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"

namespace cmg {

template <typename T, typename U> class DiTauObject;
template <typename T, typename U> class DiTauObjectFactory;

template<typename T, typename U>
class DiTauObject : public DiObject<T, U>{
  public:

    /// default constructor
    DiTauObject():
      DiObject<T, U>() {
    }

    /// constructs from leg1 and leg2  
    DiTauObject(const T& leg1, const U& leg2):
        DiObject<T, U>(leg1, leg2),
      ptSVFit_(UnSet(Double_t)),
      ptErrSVFit_(UnSet(Double_t)),
      massErrSVFit_(UnSet(Double_t))
	{
    }
    
    /// copy constructor
    DiTauObject(const DiTauObject<T, U>& other):
      DiObject<T, U>(other),
      metSig_(other.metSig_),
      ptSVFit_(other.ptSVFit_),
      ptErrSVFit_(other.ptErrSVFit_),
      massErrSVFit_(other.massErrSVFit_)
    {
    }

    virtual ~DiTauObject(){}

    /// MET significance from MVA MET that's associated to this di-object
    const cmg::METSignificance& metSig() const { return metSig_;}
    void setMetSig(const cmg::METSignificance& metSig) { metSig_ = metSig;}

    /// pT from the SV fit
    double ptSVFit() const { return ptSVFit_;}
    void setPtSVFit(double pt) { ptSVFit_ = pt;}

    /// pT error from the SV fit
    double ptErrSVFit() const { return ptErrSVFit_;}
    void setPtErrSVFit(double ptErr) { ptErrSVFit_ = ptErr;}

    /// mass error from the SV fit
    double massErrSVFit() const { return massErrSVFit_;}
    void setMassErrSVFit(double massErr) { massErrSVFit_ = massErr;}

  private:

    cmg::METSignificance metSig_;
    
    /// SVFit mass
    double ptSVFit_;
    double ptErrSVFit_;
    double massErrSVFit_;

    friend class cmg::DiTauObjectFactory<T, U>;

};

} // namespace cmg

#endif /*DITAUOBJECT_H*/
