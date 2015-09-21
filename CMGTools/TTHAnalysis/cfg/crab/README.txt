1) Source the CRAB3 environment and run cmsenv (in this order)
> source /cvmfs/cms.cern.ch/crab3/crab.sh
> cmsenv

2) Ensure that you have a valid proxy:
> voms-proxy-init -voms cms --valid=50:00

3) Execute heppy_crab.py with the correct options: see the integrated help
> ./heppy_crab.py --help

