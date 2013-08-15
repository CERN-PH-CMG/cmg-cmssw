#ifndef HIGGSCSANDWIDTH_H
#define HIGGSCSANDWIDTH_H

#define PI 3.14159

#define  ID_ggToH  1
#define  ID_VBF    2
#define  ID_WH     3
#define  ID_ZH     4
#define  ID_ttH    5
#define  ID_Total  0 

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <string>

#include "TROOT.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"


/**********************************************************/
/*            Class for Higgs Width and CS                */
/*                                                        */
/*  All numbers for CS and width are taken from official  */
/*  numbers on Higgs CS Twiki (Spring 2011)               */
/*                                                        */
/*  Cross Sections are given in pb                        */
/*  Widths are given in GeV                               */
/*                                                        */
/*  These numbers are taken into memory and a simple      */
/*  linear interpolation is done.                         */
/*                                                        */
/*  For any invalid process or mH out of range, -1 will   */
/*  be returned.                                          */
/*                                                        */
/*    Written by:                                         */
/*         Matt Snowball                                  */
/*         University of Florida                          */
/*         snowball@phys.ufl.edu                          */
/*                                                        */
/*       Last Update: April 5, 2012                       */
/*                                                        */
/**********************************************************/



class HiggsCSandWidth
{

 public:

  HiggsCSandWidth();
  ~HiggsCSandWidth();

  double HiggsCS(int ID, double mH, double sqrts);
  double HiggsCSErrPlus(int ID, double mH, double sqrts);
  double HiggsCSErrMinus(int ID, double mH, double sqrts);
  double HiggsCSscaleErrPlus(int ID, double mH, double sqrts);
  double HiggsCSscaleErrMinus(int ID, double mH, double sqrts);
  double HiggsCSpdfErrPlus(int ID, double mH, double sqrts);
  double HiggsCSpdfErrMinus(int ID, double mH, double sqrts);
  double HiggsWidth(int ID,double mH);
  double HiggsBR(int ID,double mH);

 private:

  double scratchMass;
  double mass_BR[217];
  double mass_XS[197];
  double BR[26][217];
  double CS[6][197];
  double CSerrPlus[6][197];
  double CSerrMinus[6][197];
  double CSscaleErrPlus[6][197];
  double CSscaleErrMinus[6][197];
  double CSpdfErrPlus[6][197];
  double CSpdfErrMinus[6][197];


  double mass_XS_8tev[6][223];
  double CS_8tev[6][223];
  double CSerrPlus_8tev[6][223];
  double CSerrMinus_8tev[6][223];
  double CSscaleErrPlus_8tev[6][223];
  double CSscaleErrMinus_8tev[6][223];
  double CSpdfErrPlus_8tev[6][223];
  double CSpdfErrMinus_8tev[6][223];

  int N_BR;
  int N_CS;
  int N_CSggToH_8tev;  
  int N_CSvbf_8tev;
  int N_CSWH_8tev;
  int N_CSZH_8tev;
  int N_CSttH_8tev;
  std::string FileLoc;


};

#endif
