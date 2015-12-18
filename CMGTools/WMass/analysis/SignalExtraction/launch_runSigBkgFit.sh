source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.28.00h/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh
DATA=\"../JobOutputs/RecoCountingPlotting_iso01/output_DATA/WanalysisOnDATA.root\"
root -l -b -q rootlogon.C fitWm.C+\(\"${1}\"\)
