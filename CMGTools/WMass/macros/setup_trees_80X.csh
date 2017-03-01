#!/bin/tcsh
# usage: ./setup_trees_80X.csh <rel53X> <rel80X>

setenv rel53X $1
setenv rel80X $2

cd $rel53X 
eval `scramv1 runtime -csh`
setenv pythonPath53X $CMSSW_BASE/python/
echo "Now in: " $rel53X

cd $rel80X 
eval `scramv1 runtime -csh`
setenv PYTHONPATH $pythonPath53X\:$PYTHONPATH
echo "Now in: " $rel80X

cd $rel53X
echo "back to " $rel53X
