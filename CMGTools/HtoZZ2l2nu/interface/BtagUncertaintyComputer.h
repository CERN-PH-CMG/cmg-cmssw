#ifndef btagunccomp_h
#define btagunccomp_h

namespace btag
{
  typedef std::pair<double,double> Weight_t;

  
  class UncertaintyComputer
  {
  public:
    
    UncertaintyComputer(double meb,double msfb,double msfb_err,double mel,double msfl, double msfl_err) 
      : eb(meb), sfb(msfb), sfb_err(msfb_err), el(mel), sfl(msfl), sfl_err(msfl_err)
      {}
    ~UncertaintyComputer() {}
    
    inline void compute(double nb, double nl)
    {
      //central values (based on the MC b-tag/mistag rates + data/MC scale factors)
      p0 = pow(1-sfb*eb,nb)*pow(1-sfl*el,nl);
     
      p1 = (nb>0 ? nb*sfb*eb*pow(1-sfb*eb,nb-1)*pow(1-sfl*el,nl) : 0);
      p1 += (nl>0 ? nl*sfl*el*pow(1-sfb*eb,nb)*pow(1-sfl*el,nl-1) : 0);
      
      p2 = 1-p0-p1;
      
      //errors (quadratic propagation of the uncertainty of the scale factor)
      p0_err = (nb>0 ? pow(nb*sfb_err*eb*pow(1-sfb*eb,nb-1)*pow(1-sfl*el,nl),2)  : 0. );
      p0_err += (nl>0 ? pow(nl*sfl_err*el*pow(1-sfb*eb,nb)*pow(1-sfl*el,nl-1),2) : 0. );
      p0_err = sqrt(p0_err);
      
      double a = (nb>1         ? nb*(nb-1)*pow(sfb_err*eb,2)*pow(1-sfb*eb,nb-2)*pow(1-sfl*el,nl) : 0.);
      a       -= (nb>0 && nl>0 ? nb*sfb_err*eb*nl*sfl_err*el*pow(1-sfb*eb,nb-1)*pow(1-sfl*el,nl-1) : 0.);
      double b = (nl>1         ? nl*(nl-1)*pow(sfl_err*el,2)*pow(1-sfb*eb,nb)*pow(1-sfl*el,nl-2) : 0.);
      b       -= (nb>0 && nl>0 ? nb*sfb_err*eb*nl*sfl_err*el*pow(1-sfb*eb,nb-1)*pow(1-sfl*el,nl-1) : 0.);
      p1_err = sqrt(pow(a,2)+pow(b,2));
      
      p2_err = sqrt( pow(p1_err,2) + pow(p0_err,2) );
    }
    
    inline std::vector<Weight_t> getWeights()
      {
	std::vector<Weight_t> results;
	results.push_back( std::make_pair(p0,p0_err) );
	results.push_back( std::make_pair(p1,p1_err) );
	results.push_back( std::make_pair(p2,p2_err) );
	return results;
      }
    
  private:
    double p0,p1,p2;
    double p0_err,p1_err,p2_err;
    double eb,sfb,sfb_err;
    double el,sfl,sfl_err;
  };

}

#endif
