// $Id: GenericSubtractor.hh 3001 2013-01-29 10:41:40Z soyez $
//
// Copyright (c) 2012-, Matteo Cacciari, Jihun Kim, Gavin P. Salam and Gregory Soyez
//
//----------------------------------------------------------------------
// This file is part of the GenericSubtractor package of FastJet Contrib.
//
// It is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at
// your option) any later version.
//
// It is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this code. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------

#ifndef __FASTJET_CONTRIB_GENERIC_SUBTRACTOR_HH__
#define __FASTJET_CONTRIB_GENERIC_SUBTRACTOR_HH__

#include "fastjet/PseudoJet.hh"
#include "fastjet/FunctionOfPseudoJet.hh"
#include "fastjet/tools/BackgroundEstimatorBase.hh"

#include "ShapeWithComponents.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

namespace contrib{

// forward declaration of assitional class(es) defined below.
class GenericSubtractorInfo;


//------------------------------------------------------------------------
/// \class GenericSubtractor
///
/// A class to perform subtraction of background (eg pileup or
/// underlying event) from a jet shape.
///
/// This class is a tool that allows one to subtract jet shapes
/// (i.e. a FunctionOfPseudoJet<double>). It implements the method
/// described in arXiv:1211.2811, by Gregory Soyez, Gavin P. Salam,
/// Jihun Kim, Souvik Dutta and Matteo Cacciari.
///
/// The basic usage of this class goes as follows:
///
///   GenericSubtractor gensub(& _some_background_estimator);
///   FunctionOfPseudoJet<double> shape;
///   double subtracted_shape_value = gensub(shape, jet);
///
/// Extra information can be retrieved using
///
///   GenericSubtractorInfo info;
///   double subtracted_shape_value = gensub(shape, jet, info);
///
class GenericSubtractor{
public:
  /// default constructor
  /// leaves the object unusable
  GenericSubtractor() : 
    _bge_rho(0), _bge_rhom(0), _jet_pt_fraction(0.01), _common_bge(false){}

  /// Constructor that takes a pointer to a background estimator for
  /// rho and optionally a pointer to a background estimator for
  /// rho_m.  If the latter is not supplies, rho_m is assumed to
  /// always be zero (this behaviour can be changed by calling
  /// use_common_bge_for_rho_and_rhom).
  GenericSubtractor(BackgroundEstimatorBase *bge_rho,
		    BackgroundEstimatorBase *bge_rhom=0)
    : _bge_rho(bge_rho), _bge_rhom(bge_rhom), _jet_pt_fraction(0.01), _common_bge(false){}

  /// destructor
  ~GenericSubtractor(){}

  /// a description of what this class does
  std::string description() const;

  /// returns the estimate of the supplied shape, for the given jet,
  /// after background subtraction.
  double operator()(const FunctionOfPseudoJet<double> &shape,
		    const PseudoJet &) const;

  /// returns the estimate of the supplied shape, for the given jet,
  /// after background subtraction. It also sets the "info" variable
  /// with information about the details of the subtraction (e.g. the
  /// shape's derivatives wrt to the amount of pileup).
  double operator()(const FunctionOfPseudoJet<double> & shape,
		    const PseudoJet & jet, GenericSubtractorInfo & info) const;

  /// when only one background estimator ('bge_rho') is specified,
  /// calling this routine with argument "true", causes rho_m to be be
  /// calculated from the same background estimator as rho, instead of
  /// being set to zero. 
  ///
  /// Currently his only works if the estimator is a
  /// JetMedianBackgroundEstimator (or derived from it), and makes use
  /// of that class's set_jet_density_class(...) facility.
  /// 
  void use_common_bge_for_rho_and_rhom(bool value=true);

  /// set the fraction of the jet pt that will be used in the
  /// stability condition when computing the derivatives
  void set_jet_pt_fraction_for_stability(double jet_pt_fraction){
    _jet_pt_fraction=jet_pt_fraction;
  }



protected:
  // tools to help compute the derivatives
  //----------------------------------------------------------------------
  /// do the computation of the various derivatives
  /// 
  /// rhoA_pt_fraction is rho_p/(rho_p + rho_m) used to know along
  /// which trajectory in the (rho_p, rho_m) plane one must estimate
  /// the derivative.
  void _compute_derivatives(const FunctionOfPseudoJet<double> &shape,
                            const PseudoJet &jet,
                            const double original_ghost_scale,
                            const double ghost_area,
                            const double f0, 
			    const double rho_pt_fraction,
			    GenericSubtractorInfo &info) const;

  /// make a sweep in stepsize to determine which step is the most precise
  ///
  /// Note that this returns the values of the function at the points
  /// needed to compute the derivatives
  double _optimize_step(const FunctionOfPseudoJet<double> &shape,
                        const PseudoJet &jet,
                        const double original_ghost_scale,
                        const double ghost_area,
                        const double x_fraction,
                        const double f0, 
			double cached_functions[4],
                        const double max_step) const;

  /// the function that does the rescaling
  double _shape_with_rescaled_ghosts(const FunctionOfPseudoJet<double> &shape,
				     const PseudoJet &jet,
				     const double original_ghost_scale,
				     const double new_ghost_scale, 
				     const double new_dmass_scale) const;

  /// if the shape is a ShapeWithComponents, apply the subtraction on
  /// each of the components
  double _component_subtraction(const ShapeWithComponents * shape_ptr, 
				const PseudoJet & jet,
				GenericSubtractorInfo &info) const;

  BackgroundEstimatorBase *_bge_rho, *_bge_rhom;
  double _jet_pt_fraction;
  bool _common_bge;
};

//------------------------------------------------------------------------
/// \class GenericSubtractorInfo
///
/// Helper class that allows to get extra information about the shape
/// subtraction
class GenericSubtractorInfo{
public:
  /// returns the unsubtracted shape
  double unsubtracted() const{ return _unsubtracted;}

  /// returns the result of applying the subtraction including only the
  /// first-order correction
  double first_order_subtracted() const{ return _first_order_subtracted;}
  
  /// returns the result of applying the subtraction including the
  /// first and second-order corrections (this is the default returned
  /// by the subtractions).
  double second_order_subtracted() const{ return _second_order_subtracted;}
  
  /// returns an estimate of the 3rd order correction. Note that the
  /// third derivative is quite likely to be poorly evaluated in some
  /// cases. For this reason, it is not used by default.
  double third_order_subtracted() const{ return _third_order_subtracted;}

  /// returns the first derivative (wrt to rho+rhom). Derivatives
  /// are taken assuming a constant value for the ratio of rhom/rho, as
  /// determined from the background estimates for the jet.
  ///
  /// Note: derivatives are not evaluated for shapes with components,
  /// and therefore set to zero.
  double first_derivative() const{ return _first_derivative;}

  /// returns the second derivative (wrt to rho+rhom). It is not evaluated
  /// for shapes with components, and therefore set to zero.
  double second_derivative() const{ return _second_derivative;}

  /// returns an estimate of the 3rd derivative (wrt to rho+rhom);
  /// note that the third derivative is quite likely to be poorly
  /// evaluated in some cases. It is not used by default. It is not
  /// evaluated for shapes with components, and therefore set to zero.
  double third_derivative() const{ return _third_derivative;}

  /// returns the ghost scale actually used for the computation
  double ghost_scale_used() const{ return _ghost_scale_used;}


protected:
  double _unsubtracted;
  double _first_order_subtracted;
  double _second_order_subtracted;
  double _third_order_subtracted;
  double _first_derivative, _second_derivative, _third_derivative;
  double _ghost_scale_used;

  friend class GenericSubtractor;
};


}

FASTJET_END_NAMESPACE

#endif  // __FASTJET_CONTRIB_GENERIC_SUBTRACTION_HH__
