#!/bin/bash

if [[ "$1" == "--help" || "$1" == "-h"  || "$1" == "-?" ]] then
    echo "usage: $0 [ -f flavour | -t max_runtime_in_minutes ] script.sh "
    exit 1;
fi

flavour=""
if [[ "$1" == "-f" && "$2" != "" ]] then
    flavour=$2;
    shift; shift
fi
maxruntime="-t" # time in minutes
if [[ "$1" == "-t" && "$2" != "" ]] then
    if [[ "${flavour}" != "" ]] ; then 
        echo "Can't set both flavour and maxruntime"; 
        exit 1; 
    fi;
    maxruntime=$$(( $2 * 60 ));
    shift; shift
fi

here=$(pwd)

scriptName=${1:-./batchScript.sh}

cat > ./job_desc.cfg <<EOF
Universe = vanilla
Executable = ${scriptName}
use_x509userproxy = true
Log        = condor_job_\$(ProcId).log
Output     = condor_job_\$(ProcId).out
Error      = condor_job_\$(ProcId).error
getenv      = True
environment = "LS_SUBCWD=${here}"
request_memory = 2000
EOF

[[ "${flavour}" != "" ]] && echo "+JobFlavour = \"${flavour}\"" >> ./job_desc.cfg
[[ "${maxruntime}" != "" ]] && echo "+MaxRuntime = \"${maxruntime}\"" >> ./job_desc.cfg

echo "queue 1" >> ./job_desc.cfg

# Submit job
/usr/bin/condor_submit job_desc.cfg
