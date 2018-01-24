#!/bin/bash

here=$(pwd)

scriptName=${1:-./batchScript.sh}

cat > ./job_desc.cfg <<EOF
Universe          = vanilla
Executable        = ${scriptName}
Log               = condor_job_\$(ProcId).log
Output            = condor_job_\$(ProcId).out
Error             = condor_job_\$(ProcId).error
getenv            = True
environment       = "LS_SUBCWD=${here}"
use_x509userproxy = true
request_memory    = 2000
+JobFlavour       = "espresso"
queue 1
EOF

# Submit job
/usr/bin/condor_submit job_desc.cfg
