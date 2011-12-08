#!/usr/bin/perl
# -w

$NMAX=shift;

@samples=(
#	  "TTJets"
	  "ZToTauTau"
#	  ,"ZToMuMu"
#	  ,"ZToLJet"
#	  ,"WJetsToLNu"
#	  ,"TauPlusXv4"
#	  ,"TauPlusXMay"
#	  ,"TauPlusXAug"
#	  ,"TauPlusXOct3"
#	  ,"TauPlusX2011B"
#	  ,"TTJets_SS"
#	  ,"ZToTauTau_SS"
#	  ,"ZToMuMu_SS"
#	  ,"ZToLJet_SS"
#	  ,"WJetsToLNu_SS"
#	  ,"TauPlusXMay_SS"
#	  ,"TauPlusXv4_SS"
#	  ,"TauPlusXAug_SS"
#	  ,"TauPlusXOct3_SS"
#	  ,"TauPlusX2011B_SS"
#	  ,"EmbeddedMay"
#	  ,"EmbeddedV4"
#	  ,"EmbeddedAug"
#	  ,"EmbeddedOct"
#	  ,"Embedded2011B"
#	  ,"EmbeddedMay_SS"
#	  ,"EmbeddedV4_SS"
#	  ,"EmbeddedAug_SS"
#	  ,"EmbeddedOct_SS"
#	  ,"Embedded2011B_SS"
#	  ,"WW"
#	  ,"WZ"
#	  ,"ZZ"
#	  ,"WW_SS"
#	  ,"WZ_SS"
#	  ,"ZZ_SS"
#	  ,"HiggsGG115"
#	  ,"HiggsVBF115"
#	  ,"HiggsGG105"
#	  ,"HiggsGG110"
#	  ,"HiggsGG120"
#	  ,"HiggsGG125"
#	  ,"HiggsGG130"
#	  ,"HiggsGG135"
#	  ,"HiggsVBF105"
#	  ,"HiggsVBF110"
#	  ,"HiggsVBF120"
#	  ,"HiggsVBF125"
#	  ,"HiggsVBF130"
#	  ,"HiggsVBF135"
	  );
$nsamples=@samples;

##just check if all samples finished successfully
if($NMAX==0){
    $ngood=0;
    for($i=0;$i<$nsamples;$i++){
	$saved=`cat processHistos_${samples[$i]}.log | grep "saved histograms"`;
	if($saved ne ""){
	    $ngood++;
	}else {
	    print " bad job: processHistos_${samples[$i]}.log \n";
	}
    }
    print "$ngood/$nsamples successful jobs\n";
    exit;
}

##submit all jobs
$index=0;
$trial=0;
while($trial<10000 && $index<$nsamples){
    $nroots=`ps -u benitezj | grep root.exe | wc -l`;
    #print "$trial $nroots\n";
    if($nroots<$NMAX){
	#submit another
	$samp="\\\"$samples[$index]\\\"";
	$command="rm -f processHistos_${samples[$index]}.log; time root -b -q -l \"batchSample.C\(${samp})\" >> processHistos_${samples[$index]}.log 2>&1 &";
	print "$command\n";
	system("$command");
	$index++;
    }
    $trial++;
    sleep(5);
}
print "Done submitting $index samples\n\n\n";
exit;
