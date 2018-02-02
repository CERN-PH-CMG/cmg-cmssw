#!/bin/bash

if [[ "$1" == "--help" || "$1" == "-h"  || "$1" == "-?" ]]; then
    echo "usage: $0 [ -f flavour | -t max_runtime_in_minutes ] script.sh "
    exit 1;
fi

flavour=""
if [[ "$1" == "-f" && "$2" != "" ]]; then
    flavour=$2;
    shift; shift
fi
maxruntime="-t" # time in minutes
if [[ "$1" == "-t" && "$2" != "" ]]; then
    if [[ "${flavour}" != "" ]] ; then 
        echo "Can't set both flavour and maxruntime"; 
        exit 1; 
    fi;
    maxruntime=$(( $2 * 60 ));
    shift; shift
fi

updir=$(dirname $(pwd))

scriptName=${1:-./batchScript.sh}

if [ ! -f ${updir}/src.tar.gz ]; then
    tmp_in_dir=`mktemp -d 2>/dev/null || mktemp -d -t 'tmp'`
    pushd $CMSSW_BASE
    tar --exclude=.git -czf ${tmp_in_dir}/src.tar.gz src lib bin python
    mv ${tmp_in_dir}/src.tar.gz ${updir}
    rmdir ${tmp_in_dir}
    popd
fi
transfer_input_files=${updir}/src.tar.gz

if [ -d ${HOME}/.cmgdataset ]; then
    if [ ! -f ${updir}/cmgdataset.tar.gz ]; then
        pushd $HOME
        tar -czf ${updir}/cmgdataset.tar.gz .cmgdataset
        popd
    fi
    transfer_input_files=${transfer_input_files},${updir}/cmgdataset.tar.gz
fi

tar -czf chunk.tar.gz *
transfer_input_files=${transfer_input_files},chunk.tar.gz


cat > ./job_desc.cfg <<EOF
Universe = vanilla
Executable = ${scriptName}
use_x509userproxy = \$ENV(X509_USER_PROXY)
Log        = condor_job_\$(ProcId).log
Output     = condor_job_\$(ProcId).out
Error      = condor_job_\$(ProcId).error
should_transfer_files   = YES 
when_to_transfer_output = ON_EXIT
transfer_input_files = ${transfer_input_files}
request_memory = 2000
EOF

[[ "${flavour}" != "" ]] && echo "+JobFlavour = \"${flavour}\"" >> ./job_desc.cfg
[[ "${maxruntime}" != "" ]] && echo "+MaxRuntime = ${maxruntime}" >> ./job_desc.cfg

echo "queue 1" >> ./job_desc.cfg

# Submit job
/usr/bin/condor_submit job_desc.cfg
