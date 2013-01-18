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
if($cat==6){
    @Cat=(0,1,2,3,5);
    
    ###For VBF Projection Study
    #@Cat=(0,1,5);
    ###For 1-Jet Projection Study
    #@Cat=(0,1,3);

    ###For VBF+1Jet Projection Study
    #@Cat=(0,1,3,5);
}

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
    $layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_D.py";
}
if($channel>200){
    $ECMS="14";
    $layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout_ProjectionStudy.py";
}

###scale to the crossections
if($Input ne "0"){
    `cp ${Input} ./htt_${ch}.inputs-sm-${ECMS}TeV.root`;
    `scale2SM.py -i htt_${ch}.inputs-sm-${ECMS}TeV.root --samples="ggH, qqH, VH" -e ${ECMS} -v 110-145:5`;
}

$topdir=`pwd`;
chomp($topdir);


#######limit for each category
print "creating datacard txt files:\n";
if($cat==6){
    `rm -rf SMAll`;
    `mkdir SMAll`;
	
    foreach  $i (@Mass){
	#print "${i} ";
	`mkdir SMAll/${i}`;	  
	
	foreach $d (@Cat){
	    chdir("./SMAll/${i}");
	    $cardcommand="create-datacard.py -i ${topdir}/htt_${ch}.inputs-sm-${ECMS}TeV.root  -o ${d}_${i}.txt -c ${UncsDir}/cgs-sm-${ECMS}TeV-0${d}.conf -u ${UncsDir}/unc-sm-${ECMS}TeV-0${d}.vals -d ${UncsDir}/unc-sm-${ECMS}TeV-0${d}.conf ${i}";

	    print "${cardcommand}\n";
            `${cardcommand}`;
	    chdir("../../.");
	    
	}
	print "\n";
    }
    #print "Computing limits:\n";
    #print "${limitcommand} ./SMAll/*\n";
    #`${limitcommand} ./SMAll/*`;
    foreach  $i (@Mass){
	print "${limitcommand} ./SMAll/${i}\n";
	`${limitcommand} ./SMAll/${i} >> ./SMAll/${i}/limitcommand.log 2>&1`;
    }

}else {
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


######plot the limits 
if($plot==1){
    print "Ploting the limits:\n";
    `rm -f limits_sm.root`;
    if($cat==6){
	$plotcommand="plot asymptotic ${layout} SMAll";
	print "${plotcommand}\n";
	`rm -f SMAll_sm.*`;
	`${plotcommand}`;
    }else {
	foreach $d (@Cat){
	    `rm -f SM$d_sm.*`;
	    $plotcommand="plot asymptotic ${layout} SM$d";
	    print "${plotcommand}\n";
	    `${plotcommand}`;
	}
    }
}

