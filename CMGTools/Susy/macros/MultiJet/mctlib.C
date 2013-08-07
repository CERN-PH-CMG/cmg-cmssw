#define mctlib_cxx
#include "mctlib.h"

mctlib::mctlib()
{}

mctlib::~mctlib()
{}

double mctlib::mctcorr(const double v1[4],const double v2[4]
		       ,const double vds[4],const double ptm[2]
		       ,const double ecm,const double mxlo)
{
// v1, v2 are the 4-vectors of the two aggregated decay products
// vds is the 4-vector of the downstream objects (excluding v1 and v2)
// ptm is the pTmiss 2-vector {pxmiss,pymiss}
// ecm is the centre-of-mass energy (D=14TeV)
// mxlo is a lower bound on the mass of each invisible decay product (D=0)

  double ptus[2] = {-v1[1]-v2[1]-vds[1]-ptm[0],-v1[2]-v2[2]-vds[2]-ptm[1]};
  m_pb = sqrt(pow(ptus[0],2)+pow(ptus[1],2));

  if (m_pb==0) {

    return mct(v1,v2);    

  } else {

// Transform to new basis in transverse plane
// ISR goes in +ve x direction

    double vb1[4] = {v1[0],(v1[1]*ptus[0]+v1[2]*ptus[1])/m_pb
	      ,(v1[1]*ptus[1]-v1[2]*ptus[0])/m_pb,v1[3]};
    double vb2[4] = {v2[0],(v2[1]*ptus[0]+v2[2]*ptus[1])/m_pb
	      ,(v2[1]*ptus[1]-v2[2]*ptus[0])/m_pb,v2[3]};
    double vey1 = sqrt(fmax(pow(vb1[0],2)-pow(vb1[1],2)-pow(vb1[3],2),0.0));
    double vey2 = sqrt(fmax(pow(vb2[0],2)-pow(vb2[1],2)-pow(vb2[3],2),0.0));
    double ax = vb1[1]*vey2+vb2[1]*vey1;

// Boost v1 and v2 with Ecm
// v1 and v2 were boosted in -ve x direction,
// so to correct we boost v1 and v2 in +ve x direction 

    double beta = m_pb/ecm;
    double gamma = 1.0/sqrt(1.0-beta*beta);
    double vb1p[4] = {gamma*(vb1[0]+vb1[1]*beta),gamma*(vb1[1]+vb1[0]*beta)
		      ,vb1[2],vb1[3]};
    double vb2p[4] = {gamma*(vb2[0]+vb2[1]*beta),gamma*(vb2[1]+vb2[0]*beta)
		      ,vb2[2],vb2[3]};
    double vey1p = sqrt(fmax(pow(vb1p[0],2)-pow(vb1p[1],2)
			     -pow(vb1p[3],2),0.0));
    double vey2p = sqrt(fmax(pow(vb2p[0],2)-pow(vb2p[1],2)
			     -pow(vb2p[3],2),0.0));
    double axecm = vb1p[1]*vey2p+vb2p[1]*vey1p;
    m_mctecm = mct(vb1p,vb2p);

// Boost v1 and v2 with e0
// v1 and v2 were boosted in -ve x direction,
// so to correct we boost v1 and v2 in +ve x direction  

    beta = m_pb/(v1[0]+v2[0]+vds[0]
	       +sqrt(pow(ptm[0],2)+pow(ptm[1],2)+4.0*pow(mxlo,2)));
    gamma = 1.0/sqrt(1.0-beta*beta);
    double vb1pp[4] = {gamma*(vb1[0]+vb1[1]*beta),gamma*(vb1[1]+vb1[0]*beta)
		      ,vb1[2],vb1[3]};
    double vb2pp[4] = {gamma*(vb2[0]+vb2[1]*beta),gamma*(vb2[1]+vb2[0]*beta)
		      ,vb2[2],vb2[3]};
    double vey1pp = sqrt(fmax(pow(vb1pp[0],2)-pow(vb1pp[1],2)
			      -pow(vb1pp[3],2),0.0));
    double vey2pp = sqrt(fmax(pow(vb2pp[0],2)-pow(vb2pp[1],2)
			      -pow(vb2pp[3],2),0.0));
    double axehat = vb1pp[1]*vey2pp+vb2pp[1]*vey1pp;
    m_mctehat = mct(vb1pp,vb2pp);

    if ( (ax>=0) || (axecm>=0) ) {
      return m_mctecm;
    } else {
      if (axehat<0) {
	return m_mctehat;
      } else {    
	return sqrt(fmax(pow(vey1+vey2,2)-pow(vb1[2]-vb2[2],2),0.0));
      }
    }
  }
}

double mctlib::mct(const double v1[4],const double v2[4]) 
{
  double et1 = sqrt(fmax(v1[0]*v1[0]-v1[3]*v1[3],0.0));
  double et2 = sqrt(fmax(v2[0]*v2[0]-v2[3]*v2[3],0.0));
  return sqrt(fmax(pow(et1+et2,2)-pow(v1[1]-v2[1],2)
		      -pow(v1[2]-v2[2],2),0.0));
}

double mctlib::mt2(const double v1[4],const double v2[4]
		       ,const double vds[4],const double ptm[2]
		       ,const double ecm,const double mxlo)
{
  // Boost corrected analytical mt2
  double m1sqr = pow(v1[0],2)-pow(v1[1],2)-pow(v1[2],2)-pow(v1[3],2);
  double m2sqr = pow(v2[0],2)-pow(v2[1],2)-pow(v2[2],2)-pow(v2[3],2);
  double chisqr = pow(mxlo,2);
  double m1 = sqrt(fmax(m1sqr,0.0));
  double m2 = sqrt(fmax(m2sqr,0.0));
  double chi = mxlo;

  double qtest[3] = {0.0,ptm[0]-(chi/m1)*v1[1],ptm[1]-(chi/m1)*v1[2]};
  qtest[0] = sqrt(chisqr+pow(qtest[1],2)+pow(qtest[2],2));
  double ptest[3] = {0.0,ptm[0]-(chi/m2)*v2[1],ptm[1]-(chi/m2)*v2[2]};
  ptest[0] = sqrt(chisqr+pow(ptest[1],2)+pow(ptest[2],2));

  double et1 = sqrt(fmax(v1[0]*v1[0]-v1[3]*v1[3],0.0));
  double et2 = sqrt(fmax(v2[0]*v2[0]-v2[3]*v2[3],0.0));

  double bq[3] = {et2+qtest[0],v2[1]+qtest[1],v2[2]+qtest[2]};
  double ap[3] = {et1+ptest[0],v1[1]+ptest[1],v1[2]+ptest[2]};

  // Use unbalanced solutions
  // This is an approximation as we haven't boost-corrected bq and ap

  if (pow(m1+chi,2)>=pow(bq[0],2)-pow(bq[1],2)-pow(bq[2],2)) {
    return m1+chi;
  } else if (pow(m2+chi,2)>=pow(ap[0],2)-pow(ap[1],2)-pow(ap[2],2)) {
    return m2+chi;
  }

  // Else use balanced solution
  // Note that call to mctcorr also sets m_mctehat, m_mctecm and m_pb

  double mctminsqr = pow(mctcorr(v1,v2,vds,ptm,ecm,mxlo),2);
  double mdmin = mctminmt2(mctminsqr,m1sqr,m2sqr,chisqr);
  if (m_pb == 0) return mdmin;

  double mctecmsqr = pow(m_mctecm,2);
  double mctehatsqr = pow(m_mctehat,2);
  double mctmaxsqr = fmax(mctecmsqr,mctehatsqr);

  double mctdsqr[4] = {(chi*(3.0*m1sqr+m2sqr)+2.0*m1*(m1sqr+m2sqr))/(chi+m1),
		       (chi*(3.0*m1sqr+m2sqr)-2.0*m1*(m1sqr+m2sqr))/(chi-m1),
		       (chi*(3.0*m2sqr+m1sqr)+2.0*m2*(m2sqr+m1sqr))/(chi+m2),
		       (chi*(3.0*m2sqr+m1sqr)-2.0*m2*(m2sqr+m1sqr))/(chi-m2)};
  mdmin = fmin(mdmin,mctminmt2(mctmaxsqr,m1sqr,m2sqr,chisqr));

  for (int i=0;i<4;i++)
    if ( (mctdsqr[i]>mctminsqr) && (mctdsqr[i]<mctmaxsqr) )
      mdmin = fmin(mdmin,mctminmt2(mctdsqr[i],m1sqr,m2sqr,chisqr));
  
  return fmax(fmax(mdmin,m1+chi),m2+chi);
}

double mctlib::mctminmt2(const double mctsqr,const double m1sqr,
			  const double m2sqr, const double chisqr)
{
  double at = 0.5*(mctsqr - m1sqr - m2sqr);
  return sqrt(fmax(chisqr+at+sqrt(fmax((1.0+(4.0*chisqr)/(2.0*at-m1sqr-m2sqr))
				       *(pow(at,2)-m1sqr*m2sqr),0.0)),0.0));
}

double mctlib::mt2neg(const double v1[4],const double v2[4]
		       ,const double ptm[2],const double mxlo)
{
  // Non boost-corrected analytical mt2
  double m1sqr = pow(v1[0],2)-pow(v1[1],2)-pow(v1[2],2)-pow(v1[3],2);
  double m2sqr = pow(v2[0],2)-pow(v2[1],2)-pow(v2[2],2)-pow(v2[3],2);
  double chisqr = pow(mxlo,2);
  double m1 = sqrt(fmax(m1sqr,0.0));
  double m2 = sqrt(fmax(m2sqr,0.0));
  double chi = mxlo;

  double qtest[3] = {0.0,ptm[0]-(chi/m1)*v1[1],ptm[1]-(chi/m1)*v1[2]};
  qtest[0] = sqrt(chisqr+pow(qtest[1],2)+pow(qtest[2],2));
  double ptest[3] = {0.0,ptm[0]-(chi/m2)*v2[1],ptm[1]-(chi/m2)*v2[2]};
  ptest[0] = sqrt(chisqr+pow(ptest[1],2)+pow(ptest[2],2));

  double et1 = sqrt(fmax(v1[0]*v1[0]-v1[3]*v1[3],0.0));
  double et2 = sqrt(fmax(v2[0]*v2[0]-v2[3]*v2[3],0.0));

  double bq[3] = {et2+qtest[0],v2[1]+qtest[1],v2[2]+qtest[2]};
  double ap[3] = {et1+ptest[0],v1[1]+ptest[1],v1[2]+ptest[2]};

  // Use unbalanced solutions
  if (pow(m1+chi,2)>=pow(bq[0],2)-pow(bq[1],2)-pow(bq[2],2)) {
    return m1+chi;
  } else if (pow(m2+chi,2)>=pow(ap[0],2)-pow(ap[1],2)-pow(ap[2],2)) {
    return m2+chi;
  }

  // Else use balanced solution
  double mctminsqr = pow(mct(v1,v2),2);
  double mdmin = mctminmt2(mctminsqr,m1sqr,m2sqr,chisqr);
  
  return fmax(fmax(mdmin,m1+chi),m2+chi);
}


double mctlib::mcy(const double v1[4],const double v2[4]
		       ,const double vds[4],const double ptm[2])
{
// v1, v2 are the 4-vectors of the two aggregated decay products
// vds is the 4-vector of the downstream objects (excluding v1 and v2)
// ptm is the pTmiss 2-vector {pxmiss,pymiss}

  double ptus[2] = {-v1[1]-v2[1]-vds[1]-ptm[0],-v1[2]-v2[2]-vds[2]-ptm[1]};
  double pb = sqrt(pow(ptus[0],2)+pow(ptus[1],2));

  if (pb==0) {

    return mct(v1,v2);    

  } else {

// Transform to new basis in transverse plane
// ISR goes in +ve x direction

    double vb1[4] = {v1[0],(v1[1]*ptus[0]+v1[2]*ptus[1])/pb
	      ,(v1[1]*ptus[1]-v1[2]*ptus[0])/pb,v1[3]};
    double vb2[4] = {v2[0],(v2[1]*ptus[0]+v2[2]*ptus[1])/pb
	      ,(v2[1]*ptus[1]-v2[2]*ptus[0])/pb,v2[3]};
    double vey1 = sqrt(fmax(pow(vb1[0],2)-pow(vb1[1],2)-pow(vb1[3],2),0.0));
    double vey2 = sqrt(fmax(pow(vb2[0],2)-pow(vb2[1],2)-pow(vb2[3],2),0.0));
    return sqrt(fmax(pow(vey1+vey2,2)-pow(vb1[2]-vb2[2],2),0.0));
  }
}

double mctlib::mcx(const double v1[4],const double v2[4]
		       ,const double vds[4],const double ptm[2])
{
// v1, v2 are the 4-vectors of the two aggregated decay products
// vds is the 4-vector of the downstream objects (excluding v1 and v2)
// ptm is the pTmiss 2-vector {pxmiss,pymiss}

  double ptus[2] = {-v1[1]-v2[1]-vds[1]-ptm[0],-v1[2]-v2[2]-vds[2]-ptm[1]};
  double pb = sqrt(pow(ptus[0],2)+pow(ptus[1],2));

  if (pb==0) {

    return mct(v1,v2);    

  } else {

// Transform to new basis in transverse plane
// ISR goes in +ve x direction

    double vb1[4] = {v1[0],(v1[1]*ptus[0]+v1[2]*ptus[1])/pb
	      ,(v1[1]*ptus[1]-v1[2]*ptus[0])/pb,v1[3]};
    double vb2[4] = {v2[0],(v2[1]*ptus[0]+v2[2]*ptus[1])/pb
	      ,(v2[1]*ptus[1]-v2[2]*ptus[0])/pb,v2[3]};
    double vex1 = sqrt(fmax(pow(vb1[0],2)-pow(vb1[2],2)-pow(vb1[3],2),0.0));
    double vex2 = sqrt(fmax(pow(vb2[0],2)-pow(vb2[2],2)-pow(vb2[3],2),0.0));
    return sqrt(fmax(pow(vex1+vex2,2)-pow(vb1[1]-vb2[1],2),0.0));
  }
}
