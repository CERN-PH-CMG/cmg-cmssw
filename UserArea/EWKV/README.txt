#
# installation
#

export SCRAM_ARCH=slc5_amd64_gcc462
scramv1 project CMSSW CMSSW_5_3_9
cd CMSSW_5_3_9/src/
cvs co -p UserCode/CMG/UserArea/EWKV/TAGS.txt | sh
