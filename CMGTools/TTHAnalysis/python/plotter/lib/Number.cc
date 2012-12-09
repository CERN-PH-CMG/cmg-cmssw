
/*
 *  See header file for a description of this class.
 *
 *  $Date: 2012/05/14 16:04:23 $
 *  $Revision: 1.1 $
 *  \author G. Cerminara - INFN Torino
 */

#include "Number.h"

#include <TMath.h>
#include <math.h>
using namespace std;


Number::Number() :  theNumber(0.),
		    theError(0.) {}


Number::Number(double n, double err) : theNumber(n),
				      theError(err) {}


Number::~Number(){}


Number Number::operator+(const Number& number1) const {
  Number ret(number1.number()+theNumber,
	     sqrt(number1.error()*number1.error()+theError*theError));
  return ret;
}

Number Number::operator-(const Number& number1) const {
  Number ret(theNumber-number1.number(),
	     sqrt(number1.error()*number1.error()+theError*theError));
  return ret;
}

Number Number::operator*(const Number& number1) const {
  Number ret(theNumber*number1.number(),
	     sqrt(TMath::Power(number1.number()*theError,2)+
		  TMath::Power(theNumber*number1.error(),2)));
  return ret;
}

Number Number::operator/(const Number& number1) const {
    Number ret(theNumber/number1.number(),
	       sqrt(TMath::Power(theError/number1.number(),2)+
		    TMath::Power(theNumber*number1.error()/(number1.number()*number1.number()),2)));
    return ret;
}


Number Number::operator+=(const Number& number1) {
  theNumber+= number1.number();
  theError = sqrt(theError*theError+number1.error()*number1.error());
  return *this;
}

double Number::number() const {
  return theNumber;
}

double Number::error() const {
  return theError;
}

double Number::relError() const {
  return theError/theNumber;
}

void Number::setNumber(double n) {
  theNumber = n;
}

void Number::setError(double e) {
  theError = e;
}




// ---------------------------------------------------------
// external operators

Number operator+(const Number& number, double n) {
  Number ret(number.theNumber+n, number.theError);
  return ret;
}

Number operator+(double n, const Number& number) {
  Number ret(n+number.theNumber,number.theError);
  return ret;
}


Number operator*(const Number& number, double n) {
  Number ret(number.theNumber*n,number.theError*n);
  return ret;
}


Number operator*(double n, const Number& number) {
  Number ret(n*number.theNumber,number.theError*n);
  return ret;
}



Number operator/(const Number& number, double n) {
  Number ret(number.theNumber/n,number.theError/n);
  return ret;
}


Number operator/(double n, const Number& number) {
  Number ret(n/number.theNumber,
	     n*number.theError/(number.theNumber*number.theNumber));
  return ret;
}





ostream& operator<<(ostream &os, const Number& nb){
  os << nb.number() << " +/- " << nb.error();
  return os;
}
