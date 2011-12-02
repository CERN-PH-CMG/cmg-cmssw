#!/usr/bin/perl
# -w

$NMAX=shift;

@samples=(
#	  "Higgs"
#	  ,"TTJets"
#	  ,"ZToTauTau"
#	  ,"ZToMuMu"
#	  ,"ZToLJet"
#	  ,"WJetsToLNu"
##	  ,"TauPlusXv4"
##	  ,"TauPlusXMay"
##	  ,"TauPlusXAug"
##	  ,"TauPlusXOct3"
#	  ,"TTJets_SS"
#	  ,"ZToTauTau_SS"
#	  ,"ZToMuMu_SS"
#	  ,"ZToLJet_SS"
#	  ,"WJetsToLNu_SS"
##	  ,"TauPlusXMay_SS"
##	  ,"TauPlusXv4_SS"
##	  ,"TauPlusXAug_SS"
##	  ,"TauPlusXOct3_SS"
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
	  );
$nsamples=@samples;

$index=0;
$trial=0;
while($trial<6000 && $index<$nsamples){
    $nroots=`ps -u benitezj | grep root.exe | wc -l`;
    #print "$trial $nroots\n";
    if($nroots<$NMAX){
	#submit another
	$samp="\\\"$samples[$index]\\\"";
	print "time root -b -q -l \"batchSample.C\(${samp})\" >> processHistos_${samples[$index]}.log 2>&1 &\n";
	system("time root -b -q -l \"batchSample.C\(${samp})\" >> processHistos_${samples[$index]}.log 2>&1 &");
	$index++;
    }
    $trial++;
    sleep(10);
}
print "Done submitting $index samples\n\n\n";
