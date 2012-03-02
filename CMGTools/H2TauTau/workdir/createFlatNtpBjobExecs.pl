#!/usr/bin/perl
# -w
#
###create the job execution file
$outputdir=shift;
$sample=shift;
$jobidx=shift;
$cfg=shift;

$workdir = $ENV{'PWD'};
$treename="tauMuFlatNtp";    
$outfile="executeBjob_${sample}_${jobidx}.sh";
system("rm -f ${outputdir}/${outfile}");

open OUTFILE, "> ${outputdir}/${outfile}" or die "@_";

print OUTFILE "export TOP=\$PWD\n";
#print OUTFILE "cd /afs/cern.ch/user/b/benitezj/CMSSW/src\n";
print OUTFILE "cd ${workdir}\n";
print OUTFILE "eval `scramv1 runtime -sh`\n";
print OUTFILE "cd \$TOP\n";
print OUTFILE "ulimit -v 2000000\n";    
print OUTFILE "export SAMPLENAME=\"${sample}\"\n";
print OUTFILE "export SAMPLEJOBIDX=${jobidx}\n";
print OUTFILE "cmsRun ${outputdir}/${cfg} >> ${treename}.log 2>&1\n";
print OUTFILE "mv -f ${treename}.log ${outputdir}/${treename}_${sample}_${jobidx}.log\n";
print OUTFILE "mv -f ${treename}.root ${outputdir}/${treename}_${sample}_${jobidx}.root\n";

close OUTFILE;


