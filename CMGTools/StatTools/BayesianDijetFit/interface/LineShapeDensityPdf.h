#ifndef __LINESHAPEDENSITY_PDF_H__
#define __LINESHAPEDENSITY_PDF_H__

//Resonance Shape of any mass value using Interpolation technique 
//Author: Sertac Ozturk
// sertac@fnal.gov,  sertac.ozturk@cern.ch

void LineShapeDensity_pdf(double mass, int sResonance);
void setArray (double mass);
double FastQstarBinnedProb(double mjj );

#endif
