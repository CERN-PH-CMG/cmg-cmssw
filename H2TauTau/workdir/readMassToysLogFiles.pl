#!/usr/bin/perl
# -w

$logdir=shift;
$mass=shift; 
$first=shift; 
$last=shift; 
$nlltype=shift;

foreach $n ($first .. $last){
    

    print "$n";
    foreach $m (0 .. 7){
	$nllsb=0.;
	$nllb=0.;
	
	$cat=`cat ${logdir}/injected-cmb+_vToys_${n}_${m}_${mass}.log | grep "nll S+B ->" `;

	chomp($cat);
	#print "${cat} \n";
	
	@nllarray=split(" ",$cat);
	if($nllarray[3]<0 && $nllarray[3]> -10000){ 
	    $nllsb=$nllarray[3];
	    $nllb=$nllarray[7];
	}
	
	if($nlltype==1){print " $nllsb";}
	if($nlltype==2){print " $nllb";}
	
    }
    print "\n";
}

exit;




