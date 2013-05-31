#!/usr/bin/perl
# -w

$channel=shift;
$Input=shift;
$addbbb=shift;
$UncsDir=shift;
$cat=shift;
$mass=shift; # use mass=-1 to skip to the plotting
$plot=shift;

print "$channel\n";
print "$Input\n";
print "$addbbb\n";
print "$UncsDir\n";
print "$cat\n";
print "$mass\n";
print "$plot\n";



#$limitcommand="limit.py --asymptotic --expectedOnly --noprefit ";#--userOpt '--minosAlgo stepping'
$limitcommand="limit.py --asymptotic --expectedOnly";#--userOpt '--minosAlgo stepping'
#limit.py --significance --expectedOnly --toys=-1 SM100/125 >> SM100/125/significance..log
#limit.py --max-likelihood --stable --rMin -5 --rMax 5 SM100/125 >>  SM100/125/maxLikelihood.log

###############
@Cat=($cat);
#note codes <10 are reserved for individual cat fits
if($cat==20){@Cat=(0,2);}#Boost_low
if($cat==13){@Cat=(0,1,3);}#Boost_high
if($cat==15){@Cat=(0,1,5);}#VBF
if($cat==23){@Cat=(0,1,2,3);}#1-jet
if($cat==135){@Cat=(0,1,3,5);}#Boost+VBF
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
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_30.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_15.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout_ProjectionStudy.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_ABC.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_D.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_ABCD.py";
    $layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layoutSummer13.py";
}
if($channel>200){
    $ECMS="14";
    $layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout_ProjectionStudy.py";
}




$newinputname="htt_${ch}.inputs-sm-${ECMS}TeV.root";
$topdir=`pwd`;
chomp($topdir);
$newinput="$topdir/std/${ch}/$newinputname";
$newuncsdir="$topdir/std";

###scale to the crossections
if($Input ne "0"){
    print "Creating std directory and scaling the signal\n";
    `rm -rf ./std`;
    `mkdir std`;
    `cp -r $UncsDir/${ch} ./std/`;
    `rm -f ./std/${ch}/$newinputname`;
    `cp $Input ./std/${ch}/$newinputname`;

    if($cat<1000){
	#`scale2SM.py -i $newinput --samples="ggH, qqH, VH" -e ${ECMS} -v 110-145:5`;
	`scale2SM.py -i $newinput --samples="ggH, qqH, VH" -v 110-145:5`;
    }
}  

##new paths for the inputs


####add bin-by-bin uncs
if($addbbb==1){ 
    `add_bbb_errors.py '${ch}:8TeV:01,03,05:ZLL,ZL,QCD>W' -i std -o stdbbb --threshold 0.10`;
    $newinput="$topdir/stdbbb/${ch}/$newinputname";
    $newuncsdir="$topdir/stdbbb";
    `rm -rf ./std`;
}
if($addbbb==2){##already done once 
    $newinput="$topdir/stdbbb/${ch}/$newinputname";
    $newuncsdir="$topdir/stdbbb";
}



#######limit for individual categories

if(0<=$cat&&$cat<10 && $mass>=0){
    print "creating datacard txt files:\n";
    foreach $d (@Cat){
	print "SM${d}: ";
        `rm -rf SM${d}`;
	`mkdir SM${d}`;
	
	foreach  $i (@Mass){
            #print "${i} ";
	    `rm -rf ./SM${d}/${i}`;
	    `mkdir SM${d}/${i}`;	  
	    
	    chdir("./SM${d}/${i}");
	    $cardcommand="create-datacard.py -i $newinput  -o SM_${d}_${i}.txt -c $newuncsdir/${ch}/cgs-sm-${ECMS}TeV-0${d}.conf -u $newuncsdir/${ch}/unc-sm-${ECMS}TeV-0${d}.vals -d $newuncsdir/${ch}/unc-sm-${ECMS}TeV-0${d}.conf ${i}";
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
if(10<=$cat && $cat<1000 && $mass>=0){
    print "creating datacard txt files:\n";
    `rm -rf SM${cat}`;
    `mkdir SM${cat}`;
	
    foreach  $i (@Mass){
	#print "${i} ";
	`mkdir SM${cat}/${i}`;	  
	
	foreach $d (@Cat){
	    chdir("./SM${cat}/${i}");
	    $cardcommand="create-datacard.py -i $newinput  -o SM_${d}_${i}.txt -c $newuncsdir/${ch}/cgs-sm-${ECMS}TeV-0${d}.conf -u $newuncsdir/${ch}/unc-sm-${ECMS}TeV-0${d}.vals -d $newuncsdir/${ch}/unc-sm-${ECMS}TeV-0${d}.conf ${i}";

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
if(1000<=$cat && $cat<1010 && $mass>=0){
    print "creating datacard txt files:\n";
    foreach $d (@Cat){
	print "SM${d}: \n";
        `rm -rf SM${d}`;
	`mkdir SM${d}`;
	`mkdir SM${d}/125`;
	chdir("./SM${d}/125");
	$cardcommand="create-datacard.py -i $newinput   -o SM_${d}_125.txt -c $newuncsdir/${ch}/cgs-ZTT-0${d}.conf -u $newuncsdir/${ch}/unc-ZTT-0${d}.vals -d $newuncsdir/${ch}/unc-ZTT-0${d}.conf 125";
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
	$plotcommand="plot --asymptotic ${layout} SM${CAT}";
	print "${plotcommand}\n";
	`rm -f SM${CAT}_sm.*`;
	`${plotcommand}`;
    }else {
	foreach $d (@Cat){
	    $CAT=abs($d);
	    `rm -f SM$CAT_sm.*`;
	    $plotcommand="plot --asymptotic ${layout} SM${CAT}";
	    print "${plotcommand}\n";
	    `${plotcommand}`;
	}
    }
}

#################make post-fit plots
if($plot==2){
    ##this currently does not work for the ZTT yield fits because templates want the ggH signal not ZTT
    print "Make post-fit plots:\n";
    `rm -rf postfit_SM${cat}`;
    `mkdir postfit_SM${cat}`;
    `mkdir postfit_SM${cat}/root`;
    `mkdir postfit_SM${cat}/datacards`;
    `mkdir postfit_SM${cat}/fitresults`;
    `mkdir postfit_SM${cat}/templates`;
    chdir("postfit_SM${cat}");
    
    `cp ../SM${cat}/125/out/mlfit.txt fitresults/mlfit_sm.txt`;
    `cp ${newinput} root/htt_${ch}.input_${ECMS}TeV.root`;

    $CMSSW_BASE=$ENV{'CMSSW_BASE'};
    #print "$CMSSW_BASE\n";

    foreach $d (@Cat){
	`cp ../SM${cat}/125/SM_${d}_125.txt datacards/htt_${ch}_${d}_${ECMS}TeV.txt`;
	`cp ${CMSSW_BASE}/src/HiggsAnalysis/HiggsToTauTau/test/templates/*.C templates/.`;
	`python ${CMSSW_BASE}/src/HiggsAnalysis/HiggsToTauTau/test/produce_macros.py -a sm -c '${ch}' --sm-categories-${ch} "${d}" -u 1 -p "${ECMS}TeV"`;
	`sed -i 's/bool log=true/bool log=false/g' *.C`;
	#`sed -i 's/BLIND_DATA = true/BLIND_DATA = false/g' *.C`;
	#`sed -i 's/BLIND_DATA = false/BLIND_DATA = true/g' *.C`;
	#htt_et_5_8TeV.C
	if($d==2 || $d==3 || $d==5){
	    `sed -i 's/BLIND_DATA = false/BLIND_DATA = true/g' htt_${ch}_${d}_${ECMS}TeV.C`;
	}
	`python ${CMSSW_BASE}/src/HiggsAnalysis/HiggsToTauTau/test/run_macros.py -a sm -c '${ch}' --sm-categories-${ch} "${d}"  -p "${ECMS}TeV"`;
    }
    
    chdir("../.");
}


print "runLimitsPerCategory done.\n";
