// $Id: GenericSubtractor.cc 2997 2013-01-28 20:52:04Z soyez $
//
// Copyright (c) 2012-, Matteo Cacciari, Jihun Kim, Gavin P. Salam and Gregory Soyez
//
//----------------------------------------------------------------------
// This file is part of FastJet contrib.
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

#include "fastjet/ClusterSequenceAreaBase.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"

#include "GenericSubtractor.hh"
#include "SimpleGhostRescaler.hh"
#include "ShapeWithPartition.hh"

using namespace std;

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

namespace contrib{

//------------------------------------------------------------------------
// implementation of Genericsubtractor
//------------------------------------------------------------------------


//----------------------------------------------------------------------
// the action on a given jet for a given shape
double GenericSubtractor::operator()(const FunctionOfPseudoJet<double> &shape,
                                      const PseudoJet &jet) const{
  GenericSubtractorInfo dummy_info;
  return (*this)(shape, jet, dummy_info);
}

//----------------------------------------------------------------------
// the action on a given jet for a given shape
double GenericSubtractor::operator()(const FunctionOfPseudoJet<double> &shape,
                                      const PseudoJet &jet, GenericSubtractorInfo &info) const{
  // make sure we have a BGE
  if (!_bge_rho)
    throw Error("GenericSubtractor::operator(): generic subtraction needs a JetMedianBackgroundEstimator");

  // if the shape is of the "ShapeWithPartition" type, first compute
  // the partition and work with that
  const ShapeWithPartition * shape_with_partition_ptr = dynamic_cast<const ShapeWithPartition*>(&shape);    
  PseudoJet working_jet = (shape_with_partition_ptr != 0)
    ? shape_with_partition_ptr->partition(jet) : jet;

  // for a shape with components, we will recursively use a separation
  // function that recursively calls the subtraction on the different
  // components and then assembles them.
  const ShapeWithComponents * shape_ptr = dynamic_cast<const ShapeWithComponents*>(&shape);
  if (shape_ptr != 0) {
    return _component_subtraction(shape_ptr, working_jet, info);
  }
  

  //----------------------------------------------------------------------
  // compute the average ghost scale (as a reference)
  vector<PseudoJet> ghosts = SelectorIsPureGhost()(working_jet.constituents());
  if (ghosts.size() == 0){ 
    // Note: no need to worry about any potential partitioning at this stage
    //       we just do it for efficiency (in case it has been computed already 
    info._unsubtracted = (shape_with_partition_ptr != 0)
      ? shape_with_partition_ptr->result_from_partition(working_jet) : shape(jet);
    info._first_order_subtracted  = info._unsubtracted;
    info._second_order_subtracted = info._unsubtracted;
    info._third_order_subtracted  = info._unsubtracted;
    info._first_derivative = info._second_derivative = info._third_derivative = 0.0;
    info._ghost_scale_used = 0;
    return info._unsubtracted;
  }


  double ghost_scale=0.0;
  for (vector<PseudoJet>::iterator git=ghosts.begin(); git!=ghosts.end();git++) {
    ghost_scale += git->perp();
  }
  ghost_scale /= ghosts.size();

  //----------------------------------------------------------------------
  // compute once and for all the "unsubtracted" shape
  double f0 = _shape_with_rescaled_ghosts(shape, working_jet, ghost_scale, ghost_scale, 0);
  info._unsubtracted = f0;

  //----------------------------------------------------------------------
  // estimate the background (both pt and dt=mt-pt) densities
  double ghost_area = ghosts[0].area();

  double rho = _bge_rho->rho(jet);
  double rho_mt;
  // rho_mt may be evaluated from a dedicated background estimator or
  // from a modification of the jet density class of the "rho" background estimator;
  // if neither of those options is chosen, we set it to zero.
  if (_bge_rhom){
    rho_mt = _bge_rhom->rho(jet);
  } else if (_common_bge){
    BackgroundJetPtMDensity _m_density;
    JetMedianBackgroundEstimator *jmbge = dynamic_cast<JetMedianBackgroundEstimator*>(_bge_rho);
    const FunctionOfPseudoJet<double> * orig_density = jmbge->jet_density_class();
    jmbge->set_jet_density_class(&_m_density);
    rho_mt = jmbge->rho(jet);
    jmbge->set_jet_density_class(orig_density);
  } else {
    rho_mt = 0.0;
  }
  double rho_sum = rho + rho_mt;

  double rho_pt_fraction = (rho_sum == 0) ? 0.0 : rho/rho_sum;

  //----------------------------------------------------------------------
  // compute the average ghost scale (as a reference)
  _compute_derivatives(shape, working_jet, ghost_scale, ghost_area, f0, rho_pt_fraction, info);

  info._first_order_subtracted  = f0 - rho_sum * info._first_derivative;
  info._second_order_subtracted = info._first_order_subtracted + 0.5 * pow(rho_sum,2) * info._second_derivative;
  info._third_order_subtracted  = info._second_order_subtracted - pow(rho_sum,3)/6.0 * info._third_derivative;

  return info._second_order_subtracted;
}


//----------------------------------------------------------------------
void GenericSubtractor::use_common_bge_for_rho_and_rhom(bool value){ 
  if (value){
    // make sure we only have one bge
    if (_bge_rhom)
      throw Error("GenericSubtractor::use_common_bge_for_rho_and_rhom is not allowed in the presence of an existing background estimator for rhom.");

    JetMedianBackgroundEstimator *jmbge = dynamic_cast<JetMedianBackgroundEstimator*>(_bge_rho);
    if (!jmbge)
      throw Error("GenericSubtractor::use_common_bge_for_rho_and_rhom is currently only allowed for background estimators of JetMedianBackgroundEstimator type.");
  }

  _common_bge=value;
}


//----------------------------------------------------------------------
// a description of what this class does
string GenericSubtractor::description() const{
  if (_bge_rhom) {
    return string("generic subtractor using (")+_bge_rho->description()+
      string(") and (")+_bge_rhom->description()+string(") to estimate the background");
  }

  return string("generic subtractor using (")+_bge_rho->description()+string(") to estimate the background");
}


// tools to help compute the derivatives
//----------------------------------------------------------------------

// do the computation of the various derivatives
// 
// rho_pt_fraction is rho/(rho + rho_m) is used to know along
// which trajectory in the (rho, rho_m) plane one must estimate
// the derivative.
void GenericSubtractor::_compute_derivatives(
          const FunctionOfPseudoJet<double> &shape,
          const PseudoJet &jet,
          const double original_ghost_scale,
          const double ghost_area,
          const double f0, 
	  const double rho_pt_fraction,
	  GenericSubtractorInfo &info) const{
  // here's how we proceed:
  //
  //  1. compute the 1st and 2nd derivatives in (rho+rho_m) at various step sizes
  //  2. search for the optimal step size
  //  3. recompute the 1st, 2nd and 3rd derivatives and store them in info

  //----------------------------------------------------------------------
  // compute the optimal step sizes
  double cached_functions[4];
  // the maximum step is chosen such that the sum of the ghosts will have a
  // pt equal to the jet's pt.
  double step_max = jet.pt() / (jet.area()/ghost_area);

  // go and compute the optimal step-size in pt
  double h = _optimize_step(shape, jet, original_ghost_scale, ghost_area,
			    rho_pt_fraction, f0, cached_functions, step_max);
  double f1 = cached_functions[0];
  double f2 = cached_functions[1];
  double f3 = cached_functions[2];
  double f4 = cached_functions[3];
  info._ghost_scale_used = h;

  //----------------------------------------------------------------------
  // compute all the derivatives these points
  double d1 = (f1-f0)*8;
  double d2 = (f2-f0)*4;
  double d3 = (f3-f0)*2;
  double d4 = (f4-f0);
  info._first_derivative = (64.0/21.0*d1 - 8.0/3.0*d2 + 2.0/3.0*d3 - 1.0/21.0*d4)/h * ghost_area;

  double s1 = 8*(d2/h-d1/h);
  double s2 = 4*(d3/h-d2/h);
  double s3 = 2*(d4/h-d3/h);
  info._second_derivative = (8.0/3.0*s1-2.0*s2+1/3.0*s3)/(h/2) * ghost_area * ghost_area;

  double t1 = (s2-s1)/h;
  double t2 = (s3-s2)/h;
  info._third_derivative = (4*t1-t2)/(h/8) * ghost_area * ghost_area * ghost_area;
}


//----------------------------------------------------------------------
// make a sweep in stepsize to determine which step is the most precise
//
// Note that this returns the values of the function at the points
// needed to compute the derivatives
double GenericSubtractor::_optimize_step(
          const FunctionOfPseudoJet<double> &shape,
          const PseudoJet &jet,
          const double original_ghost_scale,
	  const double ghost_area,
          const double x_fraction,
          const double f0, 
	  double cached_functions[4],
          const double max_step=1.0) const{

  // do the scale sweep for the 1st derivative in x
  //
  // we need to compute the derivative using steps 
  //   h_i = 2^{-i} h0             i=0, ..., nh
  //
  // Even with a very good numerical precision, a scale of
  // h=1e-3..1e-4 gave very good results, so we shall use h0=pt,
  // nh=28 (to have a margin of security and go down to 1e-6 for pt=1000)
  //
  // For each of these stepsizes, we need to compute the derivative at
  //   x, x+h_i/8, x+h_i/4, x+h_i/2, x+h_i
  //
  // We start by computing the derivatives at each scale
  //
  // Note that one may perhaps want to use a forward rule i.e. not use
  // the shape at x to estimate the derivatives.
  const int nh=28;
  const double h0 = max_step;

  double ref_pt = _jet_pt_fraction * jet.perp();
  double d1[nh+1], d2[nh+1], stab[nh+1], fcts[nh+4];
  double f1, f2, f3, f4;

  double h = h0*pow(2.0,-nh);

  fcts[0] = f1 = _shape_with_rescaled_ghosts(shape, jet, original_ghost_scale,
					     x_fraction*h/8, (1-x_fraction)*h/8);
  fcts[1] = f2 = _shape_with_rescaled_ghosts(shape, jet, original_ghost_scale,
					     x_fraction*h/4, (1-x_fraction)*h/4);
  fcts[2] = f3 = _shape_with_rescaled_ghosts(shape, jet, original_ghost_scale,
					     x_fraction*h/2, (1-x_fraction)*h/2);

  for (int i=0;i<=nh;i++){
    fcts[i+3] = f4 = _shape_with_rescaled_ghosts(shape,jet,original_ghost_scale, 
						 x_fraction*h, (1-x_fraction)*h);

    // apply a forward 5-point rule (including f0)
    double D1 = (f1-f0)/(h/8);
    double D2 = (f2-f0)/(h/4);
    double D3 = (f3-f0)/(h/2);
    double D4 = (f4-f0)/h;
    d1[nh-i] = (64.0/21.0*D1 - 8.0/ 3.0*D2
		+ 2.0/ 3.0*D3 - 1.0/21.0*D4) * ghost_area;
    double S1 = (D2-D1)/(h/8);
    double S2 = (D3-D2)/(h/4);
    double S3 = (D4-D3)/(h/2);
    d2[nh-i] = (2*(8.0/3.0*S1-2.0*S2+1/3.0*S3)) * ghost_area * ghost_area;
    
    stab[nh-i] = ref_pt * (abs(d1[nh-i]) + ref_pt*abs(d2[nh-i]));

    // some of the points can be reused for the next scale
    h = h0*pow(2.0,-nh+i+1);
    f1 = f2;
    f2 = f3;
    f3 = f4;
  }

  int n_plateau = 4;
  double mindiff = numeric_limits<double>::max();
  int n_mindiff = 0;
  for (int iscale = n_plateau/2; iscale <= ((int) nh)-n_plateau/2+1; iscale++){
    // this was mostly Gavin and Matteo's original code. I've replaced
    // it by a test discarding when the diff is 0. That gives better
    // results for the 2nd derivative as all its building blocks can
    // be 0 (in which case we'd better increase the step size).
    //
    // // ignore cases where one of the derivatives is zero (could this
    // // get us into trouble if the derivative is genuinely zero?)
    // if (stab[iscale-1] == 0 || stab[iscale] == 0 || stab[iscale+1] == 0) break;

    double diff=0;
    for (int i = -n_plateau/2+1; i <= n_plateau/2-1; i++)
      diff += abs(stab[iscale+i] - stab[iscale+i-1]);

    if ((diff>0) && (diff < mindiff)){
      mindiff = diff;
      n_mindiff = iscale;
    }
  }

  for (unsigned int i=0;i<4;i++)
    cached_functions[i] = fcts[nh-n_mindiff+i];

  return h0*pow(2.0,-n_mindiff);
}

//----------------------------------------------------------------------
// the function that does the rescaling
double GenericSubtractor::_shape_with_rescaled_ghosts(
          const FunctionOfPseudoJet<double> &shape,
          const PseudoJet &jet,
          double original_ghost_scale,
          double new_ghost_scale, 
          double new_dmass_scale) const{
  const ShapeWithPartition * shape_with_partition_ptr = dynamic_cast<const ShapeWithPartition*>(&shape);
  if (shape_with_partition_ptr != 0)
    return shape_with_partition_ptr->result_from_partition(SimpleGhostRescaler(new_ghost_scale, 
									       new_dmass_scale, 
									       original_ghost_scale)(jet));

  return shape(SimpleGhostRescaler(new_ghost_scale, 
                                   new_dmass_scale, 
                                   original_ghost_scale)(jet));
}



//----------------------------------------------------------------------
// perform subtractor on individual components of a shape; currently
// not necessarily the most efficient of all possible implementations (e.g.
// scaling is repeated for each of the individual components, etc.)
double GenericSubtractor::_component_subtraction(
                                const ShapeWithComponents * shape_ptr, 
				const PseudoJet & jet,
				GenericSubtractorInfo &info) const {
  unsigned n = shape_ptr->n_components();

  // prepare vectors to contain (un)subtracted results of individual components
  vector<double> subtracted1_components(n);
  vector<double> subtracted2_components(n);
  vector<double> subtracted3_components(n);
  vector<double> unsubtracted_components(n);

  // then perform subtraction of each of the components
  GenericSubtractorInfo component_info;
  for (unsigned i = 0; i < n; i++) {
    // make a subsiduary shape that returns just the i^{th} component
    // (an auto_ptr makes sure memory usage is safe)
    auto_ptr<const FunctionOfPseudoJet<double> > shape(shape_ptr->component_shape(i));
    subtracted2_components[i] = (*this)(*shape, jet, component_info);
    subtracted1_components[i] = component_info.first_order_subtracted();
    subtracted3_components[i] = component_info.third_order_subtracted();
    unsubtracted_components[i] = component_info.unsubtracted();
  }

  // obtain (un)subtracted results by combining components.
  info._unsubtracted            = shape_ptr->result_from_components(unsubtracted_components);
  info._first_order_subtracted  = shape_ptr->result_from_components(subtracted1_components);
  info._second_order_subtracted = shape_ptr->result_from_components(subtracted2_components);
  info._third_order_subtracted  = shape_ptr->result_from_components(subtracted3_components);

  // final result (without any care for safety estimates)
  double result_subtracted = info._second_order_subtracted;

  // there is no straightforward way of evaluating individual derivatives for 
  // the combination of the components, so these are set to zero.
  info._first_derivative = info._second_derivative = info._third_derivative = 0.0;

  return result_subtracted;
}

} // namespace contrib

FASTJET_END_NAMESPACE
