#!/usr/bin/perl
# -w

$channel=shift;
$Input=shift;
$UncsDir=shift;
$cat=shift;
$mass=shift;
$plot=shift;


#$limitcommand="limit.py --asymptotic --expectedOnly --noprefit  --userOpt '--minosAlgo stepping'";
#$limitcommand="limit.py --asymptotic --expectedOnly --userOpt '--minosAlgo stepping'";
$limitcommand="limit.py --asymptotic --userOpt '--minosAlgo stepping'";
#$limitcommand="limit.py --significance";

###############
@Cat=($cat);
if($cat==12){@Cat=(0,2);}#Boost_low
if($cat==13){@Cat=(1,3);}#Boost_high
if($cat==23){@Cat=(0,1,2,3);}#Boost
if($cat==15){@Cat=(0,1,5);}#VBF
if($cat==100){@Cat=(0,1,2,3,5);}##All
if( 1000<=$cat && $cat<1010){ @Cat=($cat-1000); } ##fits for ZTT and  but no signal

#############
@Mass=($mass);
if($mass==0){
    @Mass=(110,115,120,125,130,135,140,145);
}

############
$ch="mt";
if($channel==2||$channel==102||$channel==202){
    $ch="et";
}
if($channel==3||$channel==103||$channel==203){
    $ch="em";
}

############plot layout
$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout.py";
#$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout_30.py";

$ECMS="7";
if($channel>100){
    $ECMS="8";
    $layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_30.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_15.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout_ProjectionStudy.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_ABC.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_D.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_ABCD.py";
}
if($channel>200){
    $ECMS="14";
    $layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout_ProjectionStudy.py";
}

###scale to the crossections
if($Input ne "0"){
    `cp ${Input} ./htt_${ch}.inputs-sm-${ECMS}TeV.root`;
    if($cat<1000){
	`scale2SM.py -i htt_${ch}.inputs-sm-${ECMS}TeV.root --samples="ggH, qqH, VH" -e ${ECMS} -v 110-145:5`;
    }
}

$topdir=`pwd`;
chomp($topdir);


#######limit for individual categories
print "creating datacard txt files:\n";
if(0<=$cat&&$cat<10){
    foreach $d (@Cat){
	print "SM${d}: ";
        `rm -rf SM${d}`;
	`mkdir SM${d}`;
	
	foreach  $i (@Mass){
            #print "${i} ";
	    `rm -rf ./SM${d}/${i}`;
	    `mkdir SM${d}/${i}`;	  
	    
	    chdir("./SM${d}/${i}");
	    $cardcommand="create-datacard.py -i ${topdir}/htt_${ch}.inputs-sm-${ECMS}TeV.root  -o ${d}_${i}.txt -c ${UncsDir}/cgs-sm-${ECMS}TeV-0${d}.conf -u ${UncsDir}/unc-sm-${ECMS}TeV-0${d}.vals -d ${UncsDir}/unc-sm-${ECMS}TeV-0${d}.conf ${i}";
	    print "${cardcommand}\n";
            `${cardcommand}`;
	    chdir("../../.");
	    
	}
	print "\n";
    }
    print "Computing limits:\n";
    foreach $d (@Cat){
	foreach  $i (@Mass){
	    print "${limitcommand} ./SM${d}/${i}\n";
	    `${limitcommand} ./SM${d}/${i}  >> ./SM${d}/${i}/limitcommand.log 2>&1`;
	}
    }
}

#########Combination of categories
if(10<$cat && $cat<1000){
    `rm -rf SM${cat}`;
    `mkdir SM${cat}`;
	
    foreach  $i (@Mass){
	#print "${i} ";
	`mkdir SM${cat}/${i}`;	  
	
	foreach $d (@Cat){
	    chdir("./SM${cat}/${i}");
	    $cardcommand="create-datacard.py -i ${topdir}/htt_${ch}.inputs-sm-${ECMS}TeV.root  -o ${d}_${i}.txt -c ${UncsDir}/cgs-sm-${ECMS}TeV-0${d}.conf -u ${UncsDir}/unc-sm-${ECMS}TeV-0${d}.vals -d ${UncsDir}/unc-sm-${ECMS}TeV-0${d}.conf ${i}";

	    print "${cardcommand}\n";
            `${cardcommand}`;
	    chdir("../../.");
	    
	}
	print "\n";
    }

    foreach  $i (@Mass){
	print "${limitcommand} ./SM${cat}/${i}\n";
	`${limitcommand} ./SM${cat}/${i} >> ./SM${cat}/${i}/limitcommand.log 2>&1`;
    }

}


#######fit for ZTT yield
if(1000<=$cat && $cat<1010){
    foreach $d (@Cat){
	print "SM${d}: \n";
        `rm -rf SM${d}`;
	`mkdir SM${d}`;
	`mkdir SM${d}/125`;
	chdir("./SM${d}/125");
	$cardcommand="create-datacard.py -i ${topdir}/htt_${ch}.inputs-sm-${ECMS}TeV.root  -o ${d}_125.txt -c ${UncsDir}/cgs-ZTT-0${d}.conf -u ${UncsDir}/unc-ZTT-0${d}.vals -d ${UncsDir}/unc-ZTT-0${d}.conf 125";
	print "${cardcommand}\n";
	`${cardcommand}`;
	chdir("../../.");

	print "\n";
    }
    print "Running fit:\n";
    foreach $d (@Cat){
	$cmd="limit.py --max-likelihood --stable --rMin 0 --rMax 2 SM${d}/125 >> SM${d}/125/maxLikelihood.log 2>&1";
	print "${cmd}\n";
	`${cmd}`;
    }

}



###########################plot the limits ##############################
if($plot==1){
    print "Ploting the limits:\n";
    `rm -f limits_sm.root`;
    if(abs($cat)>10){
	$CAT=abs($cat);
	$plotcommand="plot asymptotic ${layout} SM${CAT}";
	print "${plotcommand}\n";
	`rm -f SM${CAT}_sm.*`;
	`${plotcommand}`;
    }else {
	foreach $d (@Cat){
	    $CAT=abs($d);
	    `rm -f SM$CAT_sm.*`;
	    $plotcommand="plot asymptotic ${layout} SM${CAT}";
	    print "${plotcommand}\n";
	    `${plotcommand}`;
	}
    }
}

#################make post-fit plots
if($plot==2){
    ##this currently does not work for the ZTT yield fits because templates want the ggH signal not ZTT
    print "Make post-fit plots:\n";
    foreach $d (@Cat){
	$CAT=abs($d);
	`rm -rf test_SM${CAT}`;
	`mkdir test_SM${CAT}`;
	`mkdir test_SM${CAT}/root`;
	`mkdir test_SM${CAT}/datacards`;
	`mkdir test_SM${CAT}/fitresults`;
	`mkdir test_SM${CAT}/templates`;
	chdir("test_SM${CAT}");
	`cp ../SM${CAT}/125/out/mlfit.txt fitresults/mlfit_sm.txt`;
	`cp ../SM${CAT}/125/0_125.txt datacards/htt_${ch}_0_${ECMS}TeV.txt`;
	`cp ../htt_${ch}.inputs-sm-${ECMS}TeV.root root/htt_${ch}.input_${ECMS}TeV.root`;
	`cp /afs/cern.ch/user/b/benitezj/scratch1/V5_8_0/CMGTools/CMSSW_5_3_3_patch3/src/HiggsAnalysis/HiggsToTauTau/test/templates/*.C templates/.`;
	`python /afs/cern.ch/user/b/benitezj/scratch1/V5_8_0/CMGTools/CMSSW_5_3_3_patch3/src/HiggsAnalysis/HiggsToTauTau/test/produce_macros.py -a sm -c '${ch}' --sm-categories-${ch} "${CAT}" -u 1 -p "${ECMS}TeV"`;
	`sed -i 's/bool log=true/bool log=false/g' *.C ; sed -i 's/BLIND_DATA = true/BLIND_DATA = false/g' *.C`;
	`python /afs/cern.ch/user/b/benitezj/scratch1/V5_8_0/CMGTools/CMSSW_5_3_3_patch3/src/HiggsAnalysis/HiggsToTauTau/test/run_macros.py -a sm -c '${ch}' --sm-categories-${ch} "${CAT}"  -p "${ECMS}TeV"`;
	chdir("../.");
    }

}


