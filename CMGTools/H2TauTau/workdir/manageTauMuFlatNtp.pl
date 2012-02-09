#!/usr/bin/perl
# -w

$outputdir=shift;
$option=shift;

@samples=(
	  "TauPlusXMay"
	  ,"TauPlusXv4"
	  ,"TauPlusXAug"
	  ,"TauPlusXOct3"
	  ,"TauPlusX2011B"
	  ,"EmbeddedV4"
	  ,"EmbeddedMay"
	  ,"EmbeddedAug"
	  ,"EmbeddedOct"
	  ,"Embedded2011B"
	  ,"ZToTauTau"
	  ,"ZToMuMu"
	  ,"ZToLJet"
	  ,"WJetsToLNu"
	  ,"W2JetsToLNu"
	  ,"W3JetsToLNu"
	  ,"TTJets"
	  ,"WW"
	  ,"WZ"
	  ,"ZZ"
	  ,"HiggsGG110"
	  ,"HiggsGG115"
	  ,"HiggsGG120"
	  ,"HiggsGG125"
	  ,"HiggsGG130"
	  ,"HiggsGG135"
	  ,"HiggsGG140"
	  ,"HiggsGG145"
	  ,"HiggsVBF110"
	  ,"HiggsVBF115"
	  ,"HiggsVBF120"
	  ,"HiggsVBF125"
	  ,"HiggsVBF130"
	  ,"HiggsVBF135"
	  ,"HiggsVBF140"
	  ,"HiggsVBF145"
	  ,"HiggsVH110"
	  ,"HiggsVH115"
	  ,"HiggsVH120"
	  ,"HiggsVH125"
	  ,"HiggsVH130"
	  ,"HiggsVH135"
	  ,"HiggsVH140"
	  ,"HiggsVH145"
	  );
$nsamples=@samples;


if($option==0){
#create the batch executables
    system("rm -f ${outputdir}/*~");
    
    foreach $s (@samples){
	$command="./createFlatNtpBjobExecs.pl ${outputdir} ${s}";	
	print "$command\n";
	system("$command");
    }
    print "Done creating bjob execs\n";
}
    

if($option==1){
#submit all jobs
    $index=0;
    foreach $s (@samples){
	system("rm -f ${outputdir}/tauMuFlatNtp_${s}.root");
	system("rm -f ${outputdir}/tauMuFlatNtp_${s}.log");
	system("rm -f ${outputdir}/lsf_${s}.log");
	$command="bsub -C 0 -R \"pool>10000\" -q 8nh -J ${s} -o ${outputdir}/lsf_${s}.log < ${outputdir}/executeBjob_${s}.sh";	
	print "$command\n";
	system("$command");
	$index++;
    }
    print "Done submitting $index samples\n\n\n";
}


if($option==2){
##just check if all samples finished successfully
    $ngood=0;
    foreach $s (@samples){
	$saved=`cat  ${outputdir}/tauMuFlatNtp_${s}.log | grep "counter = "`;
	if($saved ne ""){
	    $ngood++;
	}else {
	    print " bad job: tauMuFlatNtp_${s}.log \n";
	}
    }
    print "$ngood/$nsamples successful jobs\n";
}

