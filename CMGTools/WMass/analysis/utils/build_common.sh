#!\bin\bash

# static const double Zmass_value = _ZMASS_VALUE;
sed "s:_ZMASS_VALUE:${1}:g" -i ${14}

#static const int PDF_sets = _PDF_SETS;
sed "s:_PDF_SETS:${2}:g" -i ${14}
#static const int PDF_members = _PDF_MEMBERS;
sed "s:_PDF_MEMBERS:${3}:g" -i ${14}

# static const double etaMaxMuons = _ETA_MAX_MUONS;
sed "s:_ETA_MAX_MUONS:${4}:g" -i ${14}
# static const double etaMaxMuons2 = _ETA_MAX_SUBMUONS;
sed "s:_ETA_MAX_SUBMUONS:${5}:g" -i ${14}

# static const int WlikeCharge = _WLIKE_CHARGE;
sed "s:_WLIKE_CHARGE:${6}:g" -i ${14}

# static const int MuonKalmanVariation = _MUON_KALMAN_VARIATION; // 1 to 45
sed "s:_MUON_KALMAN_VARIATION:${7}:g" -i ${14}
# static const bool MuonScaleVariation = _MUON_SCALE_VARIATION;
sed "s:_MUON_SCALE_VARIATION:${8}:g" -i ${14}
# static const int MuonVariationSigmas = _MUON_VARIATION_SIGMAS;
sed "s:_MUON_VARIATION_SIGMAS:${9}:g" -i ${14}

# static const int RecoilStatVariation = _RECOIL_STAT_VARIATION; // 1 to 144
sed "s:_RECOIL_STAT_VARIATION:${10}:g" -i ${14}
# static const int RecoilVariationSigmas = _RECOIL_VARIATION_SIGMAS;
sed "s:_RECOIL_VARIATION_SIGMAS:${11}:g" -i ${14}

# static const int PtSFNSteps = _PTSF_NSTEPS;
PtSFNSteps=$(grep -o "," <<< "${12}" | wc -l)
PtSFNSteps=$((PtSFNSteps+1))
sed "s:_PTSF_NSTEPS:${PtSFNSteps}:g" -i ${14}
# static const int PtSFtype[PtSFNSteps] = { _PTSF_TYPE };
sed "s:_PTSF_TYPE:${12}:g" -i ${14}

# static const int LHE_mass_central_index = _LHE_MASS_CENTRAL_INDEX;
sed "s:_LHE_MASS_CENTRAL_INDEX:${13}:g" -i ${14}


# echo ZMASS_VALUE: ${1}
# echo PDF_SETS: ${2}
# echo PDF_MEMBERS: ${3}
# echo ETA_MAX_MUONS: ${4}
# echo ETA_MAX_SUBMUONS: ${5}
# echo WLIKE_CHARGE: ${6}
# echo MUON_KALMAN_VARIATION: ${7}
# echo MUON_SCALE_VARIATION: ${8}
# echo MUON_VARIATION_SIGMAS: ${9}
# echo RECOIL_STAT_VARIATION: ${10}
# echo RECOIL_VARIATION_SIGMAS: ${11}
# echo PTSF_NSTEPS: ${PtSFNSteps}
# echo PTSF_TYPE: ${12}
# echo LHE_MASS_CENTRAL_INDEX: ${13}
# echo filename: ${14}
