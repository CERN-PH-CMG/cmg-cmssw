#! /bin/bash
# run example: ./submitlhe.sh -q 8nh -n 10 -e "/store/cmst3/user/emanuele/monox/lhe/" tarballs

if [[ "$1" == "" || "$1" == "-h" || "$1" == "--help" ]]; then
    echo "Usage: $0 [ -q <queue> ] [ -n nEvents ] [ -e eospath ] [ dir-where-gridpacks ] "
    echo "Will generate LHE files with nEvents (default 50k) from gridpacks stored in the input directory "
    echo "if -q option is not given, do not submnit the jobs, just untar the gridpacks."
    exit 1;
fi
   
Q=8nh
if [[ "$1" == "-q" ]]; then
    Q=$2; shift; shift;
    echo "# Will also submit the jobs to queue $Q"
fi;

NEVENTS=50000
if [[ "$1" == "-n" ]]; then
    NEVENTS=$2; shift; shift;
    echo "# Will generate $NEVENTS events"
fi;

E=0
if [[ "$1" == "-e" ]]; then
    E=$2; shift; shift;
    echo "# Will copy the output LHE file on EOS path $E"
fi;

SEED=$RANDOM

dir=$1
workdir=$PWD
echo "input directory = $dir"
for F in $(ls ${dir}/*.tgz) ; do
    echo "preparing tarball $F";
    point=${F%.tgz} # remove extension
    point=${point#*/} # remove path
    outdir=$dir"/"$point
    mkdir -p $outdir
    tar xzf $F -C $outdir
    if [[ "$Q" != "0" ]]; then
        SCRIPTBASE=$workdir/$outdir/batchjob.sh
        cat << EOF > $SCRIPTBASE
export SCRAM_ARCH=$SCRAM_ARCH
cd $workdir/$outdir
eval \`scramv1 runtime -sh\`
./runcmsgrid.sh $NEVENTS $SEED 1
EOF
        if [[ "$E" != "0" ]]; then
            eosfile=$E/$point.lhe
            echo "cmsStageIn -f cmsgrid_final.lhe $eosfile" >> $SCRIPTBASE
        fi;
        chmod u+x $SCRIPTBASE
        LOGFILE=$outdir/batchjob.log
        #echo "scriptfile = $SCRIPTBASE"
        bsub -q $Q -o $LOGFILE -J $point $SCRIPTBASE
    fi;
done
