#!/usr/bin/perl
# -w

$outputdir=shift; #need absolute path /afs/...
$option=shift;


sub submit {
   $outputdir= $_[0];
   $s= $_[1];
   $idx = $_[2];

   system("rm -f ${outputdir}/tauMuFlatNtp_${s}_${idx}.root");
   system("rm -f ${outputdir}/tauMuFlatNtp_${s}_${idx}.log");
   system("rm -f ${outputdir}/lsf_${s}_${idx}.log");
   $command="bsub -C 0 -R \"pool>10000\" -q 8nh -J ${s}_${idx} -o ${outputdir}/lsf_${s}_${idx}.log < ${outputdir}/executeBjob_${s}_${idx}.sh";	
   print "$command\n";
   system("$command");

}


@samples=(
	  "TauPlusXMay",6
	  ,"TauPlusXv4",7
	  ,"TauPlusXAug",2
	  ,"TauPlusXOct3",3
	  ,"TauPlusX2011B",12
	  ,"EmbeddedV4",1
	  ,"EmbeddedMay",1
	  ,"EmbeddedAug",1
	  ,"EmbeddedOct",1
	  ,"Embedded2011B",3
	  ,"ZToTauTau",12
	  ,"ZToMuMu",12
	  ,"ZToLJet",12
	  ,"WJetsToLNu",102
	  ,"W2JetsToLNu",17
	  ,"W3JetsToLNu",6
	  ,"TTJets",4
	  ,"WW",3
	  ,"WZ",3
	  ,"ZZ",3
	  ,"HiggsGG110",1
	  ,"HiggsGG115",1
	  ,"HiggsGG120",1
	  ,"HiggsGG125",1
	  ,"HiggsGG130",1
	  ,"HiggsGG135",1
	  ,"HiggsGG140",1
	  ,"HiggsGG145",1
	  ,"HiggsVBF110",1
	  ,"HiggsVBF115",1
	  ,"HiggsVBF120",1
	  ,"HiggsVBF125",1
	  ,"HiggsVBF130",1
	  ,"HiggsVBF135",1
	  ,"HiggsVBF140",1
	  ,"HiggsVBF145",1
	  ,"HiggsVH110",1
	  ,"HiggsVH115",1
	  ,"HiggsVH120",1
	  ,"HiggsVH125",1
	  ,"HiggsVH130",1
	  ,"HiggsVH135",1
	  ,"HiggsVH140",1
	  ,"HiggsVH145",1
	  );
$nsamples=@samples/2;


#number of jobs should cover even the largest sample (WJets which has <1200 input files, merge factor in cfg is 30)
#$njobspersample = 40;

if($option==0){#create the batch executables
    system("rm -f ${outputdir}/*~");
    #system("rm -f ${outputdir}/*.log");
    #system("rm -f ${outputdir}/*.root");
    #system("rm -f ${outputdir}/*.py");
    #system("rm -f ${outputdir}/*.pyc");
    #system("rm -f ${outputdir}/*.sh");
    
    #cfg to be used (e.g. ./tauMuFlatNtp_cfg.py)
    $cfg=shift;
    `cp ${cfg} ${outputdir}/.`;

    $i=0;
    while($i < $nsamples){	
	$s=$samples[$i*2];
	$njobs=$samples[$i*2+1];
	foreach $idx (0 .. $njobs-1){
	    $command="./createFlatNtpBjobExecs.pl ${outputdir} ${s} ${idx} ${cfg}";	
	    #print "$command\n";
	    system("$command");
	}
	print "Created bjob execs for ${s}\n";
	$i++;
    }
}



    

if($option==1){
#submit all jobs
    $i=0;
    while($i < $nsamples){	
	$s=$samples[$i*2];
	$njobs=$samples[$i*2+1];
	foreach $idx (0 .. $njobs-1){
	    #keep loop over samples inside to submit jobs which actually have events first.	   
	    submit(${outputdir},$s,$idx);
	}	
	$i++;
    }
}


if($option==2){
##just check if all samples finished successfully

    $submitfailed=shift;

    $i=0;
    while($i < $nsamples){
	
	$s=$samples[$i*2];
	$njobs=$samples[$i*2+1];

	print "$s : ";
	$ngood=0;
       	foreach $idx (0 .. $njobs-1){
	    $good=0;
	    $counter=`cat  ${outputdir}/tauMuFlatNtp_${s}_${idx}.log | grep "counterall = "`;
	    if($counter ne ""){
		chomp($counter);
		@evtsproc=split(" ",$counter);
		if($evtsproc[2] > 0){ # job had input files and processed some events
		    $good=1;
		    $ngood++;
		    print " ${idx}";
		}
	    }
	    if(!$good){
		print " (bad ${idx})";
		if($submitfailed){ submit(${outputdir},$s,$idx); }
	    }
	}
	print " ==> $ngood successful jobs\n";
	$i++;
    }
}


