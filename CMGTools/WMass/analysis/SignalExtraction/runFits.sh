#! /bin/bash

# LUMI=0.020549
# ECM=8

source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.28.00h/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh

root -l -b -q rootlogon.C fitWm.C+\(\"${1}\"\)
# root -l -q fitWm.C+\(\"Wmunu\",${LUMI},${ECM},0\)
#root -l -q fitZmm.C+\(\"Zmumu\",${LUMI},${ECM},0\)

#root -l -q fitWe.C+\(\"Wenu\",${LUMI},${ECM},0\)
#root -l -q fitZee.C+\(\"Zee\",${LUMI},${ECM},0\)

rm *.so *.d
