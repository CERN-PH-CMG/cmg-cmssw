#!/usr/bin/perl
# -w

$NMAX=shift;

@samples=(
	  "TauPlusX2011B"
#	  ,"Embedded2011B"
#	  ,"TauPlusX2011B_SS"
#	  ,"Embedded2011B_SS"
#	  ,"TauPlusXv4"
#	  ,"TauPlusXv4_SS"
#	  ,"EmbeddedV4"
#	  ,"EmbeddedV4_SS"
#	  ,"TauPlusXMay"
#	  ,"TauPlusXAug"
#	  ,"TauPlusXOct3"
#	  ,"EmbeddedMay"
#	  ,"EmbeddedAug"
#	  ,"EmbeddedOct"
#	  ,"TauPlusXMay_SS"
#	  ,"TauPlusXAug_SS"
#	  ,"TauPlusXOct3_SS"
#	  ,"EmbeddedMay_SS"
#	  ,"EmbeddedAug_SS"
#	  ,"EmbeddedOct_SS"
#	  ,"ZToTauTau"
#	  ,"ZToMuMu"
#	  ,"ZToLJet"
#	  ,"WJetsToLNu"
#	  ,"W2JetsToLNu"
#	  ,"W3JetsToLNu"
#	  ,"TTJets"
#	  ,"ZToTauTau_SS"
#	  ,"ZToMuMu_SS"
#	  ,"ZToLJet_SS"
#	  ,"WJetsToLNu_SS"
#	  ,"W2JetsToLNu_SS"
#	  ,"W3JetsToLNu_SS"
#	  ,"TTJets_SS"
#	  ,"WW"
#	  ,"WZ"
#	  ,"ZZ"
#	  ,"WW_SS"
#	  ,"WZ_SS"
#	  ,"ZZ_SS"
#	  ,"HiggsGG110"
#	  ,"HiggsGG115"
#	  ,"HiggsGG120"
#	  ,"HiggsGG125"
#	  ,"HiggsGG130"
#	  ,"HiggsGG135"
#	  ,"HiggsGG140"
#	  ,"HiggsGG145"
#	  ,"HiggsVBF110"
#	  ,"HiggsVBF115"
#	  ,"HiggsVBF120"
#	  ,"HiggsVBF125"
#	  ,"HiggsVBF130"
#	  ,"HiggsVBF135"
#	  ,"HiggsVBF140"
#	  ,"HiggsVBF145"
#	  ,"HiggsVH110"
#	  ,"HiggsVH115"
#	  ,"HiggsVH120"
#	  ,"HiggsVH125"
#	  ,"HiggsVH130"
#	  ,"HiggsVH135"
#	  ,"HiggsVH140"
#	  ,"HiggsVH145"
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
