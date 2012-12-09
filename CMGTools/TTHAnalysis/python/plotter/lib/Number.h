#ifndef Number_H
#define Number_H

/** \class Number
 *  No description available.
 *
 *  $Date: 2012/05/14 16:04:23 $
 *  $Revision: 1.1 $
 *  \author G. Cerminara - INFN Torino
 */

#include <iostream>

class Number {
public:
  /// Constructor
  Number();

  Number(double number, double error);

  /// Destructor
  virtual ~Number();

  // Operations
  Number operator+(const Number& number1) const;
  Number operator-(const Number& number1) const;

  Number operator*(const Number& number1) const;
  Number operator/(const Number& number1) const;


  Number operator+=(const Number& number);
  friend Number operator+(const Number& number, double n);
  friend Number operator+(double n, const Number& number);


  friend Number operator*(const Number& number, double n);//FIXME implement this
  friend Number operator*(double n, const Number& number);//FIXME implement this
  friend Number operator/(const Number& number, double n);//FIXME implement this
  friend Number operator/(double n, const Number& number);//FIXME implement this



  double number() const;
  double error() const;
  double relError() const;

  void setNumber(double n);
  void setError(double e);

protected:

private:

  double theNumber;
  double theError;

};

std::ostream& operator<<(std::ostream &os, const Number& nb);



#endif

