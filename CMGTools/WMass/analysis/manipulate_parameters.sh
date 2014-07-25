#!\bin\bash

# echo "PAR_ZMASS_CENTRAL:${1} PAR_WMASS_CENTRAL:${2} PAR_WMASS_STEP:${3} PAR_WMASS_NSTEPS:${4}"
# echo "PAR_ETA_NSTEP:${5} PAR_ETA_STEPS:${6} PAR_NPDF_SETS:${7} PAR_PDF_SETS:${8} PAR_PDF_MEMBERS:${9} PAR_TOYS_MOMCORR:${10}"

  # static const double ZMassCentral_MeV = PAR_ZMASS_CENTRAL;
  sed "s:PAR_ZMASS_CENTRAL:${1}:g" -i includes/common.h
  # static const int WMassCentral_MeV = PAR_WMASS_CENTRAL;
  sed "s:PAR_WMASS_CENTRAL:${2}:g" -i includes/common.h
  # static const int WMassStep_MeV = PAR_WMASS_STEP;
  sed "s:PAR_WMASS_STEP:${3}:g" -i includes/common.h
  # static const int WMassNSteps = PAR_WMASS_NSTEPS;
  sed "s:PAR_WMASS_NSTEPS:${4}:g" -i includes/common.h
  # static const int etaMuonNSteps = PAR_ETA_NSTEP;
  sed "s:PAR_ETA_NSTEP:${5}:g" -i includes/common.h
  # static const int Wmass_values_array = PAR_WMASS_VALUES_ARRAY;
  sed "s:PAR_WMASS_VALUES_ARRAY:${11}:g" -i includes/common.h
  # static const int Zmass_values_array = PAR_ZMASS_VALUES_ARRAY;
  sed "s:PAR_ZMASS_VALUES_ARRAY:${12}:g" -i includes/common.h
  # static const int LHE_mass_central_index = PAR_DELTAM_MEV_CENTRAL_INDEX;
  sed "s:PAR_DELTAM_MEV_CENTRAL_INDEX:${13}:g" -i includes/common.h
  # static const float etaMaxMuons[etaMuonNSteps] = {PAR_ETA_STEPS};
  sed "s:PAR_ETA_STEPS:${6}:g" -i includes/common.h
  # static const int NPDF_sets = PAR_NPDF_SETS;
  sed "s:PAR_NPDF_SETS:${7}:g" -i includes/common.h
  # TString PDF_sets[NPDF_sets] = {PAR_PDF_SETS};
  sed "s:PAR_PDF_SETS:${8}:g" -i includes/common.h
  # int PDF_members[NPDF_sets] = {PAR_PDF_MEMBERS};
  sed "s:PAR_PDF_MEMBERS:${9}:g" -i includes/common.h
  # int NtoysMomCorr = PAR_TOYS_MOMCORR;
  sed "s:PAR_TOYS_MOMCORR:${10}:g" -i includes/common.h
