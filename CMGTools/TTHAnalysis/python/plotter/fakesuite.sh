#!/bin/bash

WHAT=$1; if [[ "$1" == "" ]]; then echo "frsuite.sh <what>"; exit 1; fi

if [[ "$HOSTNAME" == "cmsphys10" ]]; then
    T="/data/g/gpetrucc/SusyFakes/TREES_080414";
    J=8;
#elif [[ "$HOSTNAME" == "cmsphys05" ]]; then
#    T="/data/b/botta/TTHAnalysis/trees/TREES_250513_HADD";
#    J=6;
#elif [[ "$HOSTNAME" == "olsnba03" ]]; then
#    T="/data/gpetrucc/TREES_250513_HADD";
#    J=12;
else
    T="/afs/cern.ch/work/g/gpetrucc/SusyFakes/prod/TREES_190314";
    J=6;
fi
CORE="mcPlots.py -P $T -j $J -f -l 0.008106 --tree ttHLepFRAnalyzer "
CORE="${CORE} --showRatio --maxRatioRange 0 2.5  "
ROOT="plotFakes/080414/sync/$WHAT"

case $WHAT in
loose)
    RUN="${CORE} mca-fakeLepStudies.txt bins/fakeLepStudies.txt bins/fakeLepStudies_plots.txt  -X tight "
    echo "python ${RUN} --pdir $ROOT    "
    #echo "python ${RUN} --pdir $ROOT/no_met     -X met  --sP met  "
    echo "python ${RUN} --pdir $ROOT/no_met_mt  -X met  -X mtw    "
    #echo "python ${RUN} --pdir $ROOT/inv_met  -I met -X mtw --sP met,mtw  "
    echo "python ${RUN} --pdir $ROOT/inv_met30  -R met imet30 'met_t1 > 30'  -X mtw --sP met,mtw  "
    #echo "python ${RUN} --pdir $ROOT/no_met_mt/eta_0.0_0.5-pt_20_25  -X met  -X mtw  -A Mu17 pteta '0.0 < abs(Probe_eta) && abs(Probe_eta) < 0.5 && Probe_pt >= 20 && Probe_pt < 25' --sP met "
    #echo "python ${RUN} --pdir $ROOT/no_met_mt/eta_0.0_0.5-pt_45_in  -X met  -X mtw  -A Mu17 pteta '0.0 < abs(Probe_eta) && abs(Probe_eta) < 0.5 && Probe_pt >= 45'                  --sP met "
    #echo "python ${RUN} --pdir $ROOT/no_met_mt/eta_1.5_2.0-pt_35_45  -X met  -X mtw  -A Mu17 pteta '1.5 < abs(Probe_eta) && abs(Probe_eta) < 2.0 && Probe_pt >= 35 && Probe_pt < 45' --sP met "
    ;;
tight)
    RUN="${CORE} mca-fakeLepStudies.txt bins/fakeLepStudies.txt bins/fakeLepStudies_plots.txt   "
    echo "python ${RUN} --pdir $ROOT    "
    #echo "python ${RUN} --pdir $ROOT/no_met     -X met  --sP met  "
    echo "python ${RUN} --pdir $ROOT/no_met_mt  -X met  -X mtw   "
    #echo "python ${RUN} --pdir $ROOT/inv_met  -I met -X mtw --sP met,mtw  "
    echo "python ${RUN} --pdir $ROOT/inv_met30  -R met imet30 'met_t1 > 30'  -X mtw --sP met,mtw  "
    #echo "python ${RUN} --pdir $ROOT/no_met_mt/eta_0.0_0.5-pt_20_25  -X met  -X mtw  -A Mu17 pteta '0.0 < abs(Probe_eta) && abs(Probe_eta) < 0.5 && Probe_pt >= 20 && Probe_pt < 25' --sP met "
    #echo "python ${RUN} --pdir $ROOT/no_met_mt/eta_0.0_0.5-pt_45_in  -X met  -X mtw  -A Mu17 pteta '0.0 < abs(Probe_eta) && abs(Probe_eta) < 0.5 && Probe_pt >= 45'                  --sP met "
    #echo "python ${RUN} --pdir $ROOT/no_met_mt/eta_1.5_2.0-pt_35_45  -X met  -X mtw  -A Mu17 pteta '1.5 < abs(Probe_eta) && abs(Probe_eta) < 2.0 && Probe_pt >= 35 && Probe_pt < 45' --sP met "
    ;;
loose_vtxw)
    RUN="${CORE} mca-fakeLepStudies.txt bins/fakeLepStudies.txt bins/fakeLepStudies_plots.txt -W '(0.296+0.0971*nVert-0.00288*nVert*nVert)' -X tight "
    echo "python ${RUN} --pdir $ROOT    "
    echo "python ${RUN} --pdir $ROOT/no_met  -X met  --sP met  "
    echo "python ${RUN} --pdir $ROOT/no_met_mt  -X met  -X mtw    "
    echo "python ${RUN} --pdir $ROOT/inv_met  -I met -X mtw --sP met,mtw  "
    ;;
tight_vtxw)
    RUN="${CORE} mca-fakeLepStudies.txt bins/fakeLepStudies.txt bins/fakeLepStudies_plots.txt -W '(0.296+0.0971*nVert-0.00288*nVert*nVert)'  "
    echo "python ${RUN} --pdir $ROOT    "
    echo "python ${RUN} --pdir $ROOT/no_met  -X met  --sP met  "
    echo "python ${RUN} --pdir $ROOT/no_met_mt  -X met  -X mtw    "
    echo "python ${RUN} --pdir $ROOT/inv_met  -I met -X mtw --sP met,mtw  "
    ;;
esac;

