#!/bin/bash

#configuration
ME=`whoami`
MYLETTER=${ME:0:1}
MYCMSSWDIR=${CMSSW_BASE}/src

#check
if [ -z "$CMSSW_BASE" ]
then
    echo "$ME please setup CMSSW environment"
    exit -1
fi

#parse the command line
for i in $*
do
  case $i in
      -cfg=*)
      CFG=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
      ;;
      -src=*)
      INPUTDIR=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
      ;;
      -f=*)
      FFILE=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
      ;;
      -step=*)
      STEP=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
      ;;
      -castor=*)
      OUTDIR=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
      ;;
      *)
      echo "submitLocalRun.sh -cfg=cfg.py -src=input_dir [-f=first_file] [-step=n_files] [-castor=castor_output]"
      exit -1
      ;;
  esac
done

#check configuration
if [ -z "$CFG" ]
then
    echo "Please provide configuration file with -cfg=cfg.py"
    exit -1
fi
if [ -z "$INPUTDIR" ]
then
    echo "Please provide sample name with -src=input_dir"
    exit -1
fi

SRC=${INPUTDIR##/*/}
LOCALOUT="/tmp/${SRC}.root"
if [ -n "$FFILE" ]
then
    LOCALOUT="/tmp/${SRC}_${FFILE}.root"
fi
if [ -n "$STEP" ]
then

    LOCALOUT="/tmp/${SRC}_${FFILE}_${STEP}.root"
fi

#call cmsRun
cd ${MYCMSSWDIR}/CMGTools/HtoZZ2l2nu/test
cmsRun ${CFG} ${INPUTDIR} ${LOCALOUT} ${FFILE} ${STEP}
cd -

#move to OUTDIR directory if required
if [ -z "$OUTDIR" ]
then
    echo "Result is available in ${LOCALOUT}"
    exit -1
fi
case "$OUTDIR" in 
    *castor*)
    #copy to castor
    rfmkdir $OUTDIR/${SRC}
    rfcp $LOCALOUT $OUTDIR/${SRC}
    rm $LOCALOUT
    echo "Result is available in castor @ ${OUTDIR}/${SRC}"
    ;;
    *)
    #copy locally
    mkdir $OUTDIR/${SRC}
    mv $LOCALOUT $OUTDIR/${SRC}
    echo "Result is available in @ ${OUTDIR}/${SRC}"
    ;;
esac

