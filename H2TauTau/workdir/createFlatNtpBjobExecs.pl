#!/usr/bin/perl
# -w
#
###create the job execution file
$outputdir=shift;
$sample=shift;
$treename="tauMuFlatNtp";    
$outfile="executeBjob_${sample}.sh";
system("rm -f ${outputdir}/${outfile}");

open OUTFILE, "> ${outputdir}/${outfile}" or die "@_";

print OUTFILE "export TOP=\$PWD\n";
print OUTFILE "cd /afs/cern.ch/user/b/benitezj/CMSSW/src\n";
print OUTFILE "eval `scramv1 runtime -sh`\n";
print OUTFILE "cd \$TOP\n";
print OUTFILE "ulimit -v 2000000\n";    
print OUTFILE "export SAMPLENAME=\"${sample}\"\n";
print OUTFILE "cmsRun ${outputdir}/tauMuFlatNtp_cfg.py >> ${treename}.log 2>&1\n";
print OUTFILE "mv -f ${treename}.log ${outputdir}/${treename}_${sample}.log\n";
print OUTFILE "mv -f ${treename}.root ${outputdir}/${treename}_${sample}.root\n";

close OUTFILE;


