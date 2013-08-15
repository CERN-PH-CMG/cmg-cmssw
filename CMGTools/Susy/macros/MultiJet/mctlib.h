#ifndef mctlib_h
#define mctlib_h

class mctlib {

 public:

  mctlib();
  virtual ~mctlib();

  virtual double mctcorr(const double v1[4],const double v2[4]
			 ,const double vds[4],const double ptm[2]
			 ,const double ecm=14000.0,const double mxlo=0.0);
  virtual double mct(const double v1[4],const double v2[4]);
  virtual double mt2(const double v1[4],const double v2[4]
		     ,const double vds[4],const double ptm[2]
		     ,const double ecm=14000.0,const double mxlo=0.0);
  virtual double mctminmt2(const double mctsqr,const double m1sqr,
			   const double m2sqr,const double chisqr=0.0);
  virtual double mt2neg(const double v1[4],const double v2[4]
			,const double ptm[2],const double mxlo=0.0);
  virtual double mcy(const double v1[4],const double v2[4]
		     ,const double vds[4],const double ptm[2]);
  virtual double mcx(const double v1[4],const double v2[4]
		     ,const double vds[4],const double ptm[2]);

 private:

  double m_mctecm, m_mctehat, m_pb;

};

#endif


  
