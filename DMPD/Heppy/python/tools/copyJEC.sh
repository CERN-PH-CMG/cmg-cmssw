#!/bin/bash

echo "Do grid login first... voms-proxy-init -voms cms"

# Target directory has to have a JEC/ dir, just as local

#srmls srm://t2-srm-02.lnl.infn.it:8443/pnfs/lnl.infn.it/data/cms/store/user/zucchett/JEC/

for f in JEC/*
do
    echo Copying $f...
    srmcp file:///$CMSSW_BASE/src/DMPD/Heppy/python/tools/$f srm://t2-srm-02.lnl.infn.it:8443/pnfs/lnl.infn.it/data/cms/store/user/zucchett/$f
done
