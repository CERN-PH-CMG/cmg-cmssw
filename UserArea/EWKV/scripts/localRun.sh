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
TAG="Events"
EXE=cmsRun
for i in $*
do
  case $i in
      -exe=*)
      EXE=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
      ;;
      -cfg=*)
      CFG=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
      ;;
      -src=*)
      INPUTDIR=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
      ;;
      -tag=*)
      TAG=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
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
      -jsonr=*)
      JSON=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
      ;;
      *)
      echo "submitLocalRun.sh -cfg=cfg.py -src=input_dir [-f=first_file] [-step=n_files] [-castor=castor_output] [-json=json_file]"
      exit -1
      ;;
  esac
done

if [ -z "$CFG" ]
    then
    echo "Please provide configuration file with -cfg=cfg.py"
    exit -1
fi


#check specific configuration for cmsRun
if [ "$EXE" == "cmsRun"];
then
    if [ -z "$INPUTDIR" ]
	then
	echo "Please provide sample name with -src=input_dir"
	exit -1
    fi
    LOCALOUT="/tmp/${TAG}.root"
    if [ -n "$FFILE" ]
	then
	LOCALOUT="/tmp/${TAG}_${FFILE}.root"
    fi
    if [ -n "$STEP" ]
	then
	LOCALOUT="/tmp/${TAG}_${FFILE}_${STEP}.root"
    fi
fi

df -H
$EXE ${CFG} ${INPUTDIR} ${LOCALOUT} ${FFILE} ${STEP}

#check the exit code of cmsRun and return if error
if [ $? -ne 0 ]
then
    exit 1
fi 

ls -lth ${LOCALOUT}
df -H

#move to OUTDIR directory if required
if [ -z "$OUTDIR" ]
then
    echo "Result is available in ${LOCALOUT}"
    exit -1
fi
case "$OUTDIR" in 
    *castor*)
    #copy to castor
    rfmkdir -p $OUTDIR
    rfcp $LOCALOUT $OUTDIR
    rm $LOCALOUT
    echo "Result is available in castor @ ${OUTDIR}"
    ;;
    /store*)
    #copy to EOS
    cmsMkdir $OUTDIR
    cmsStage -f $LOCALOUT $OUTDIR
    rm $LOCALOUT
    echo "Result is available in castor @ ${OUTDIR}"
    ;;
    *)
    #copy locally
    mkdir -p $OUTDIR
    mv $LOCALOUT $OUTDIR
    echo "Result is available in @ ${OUTDIR}"
    ;;
esac

