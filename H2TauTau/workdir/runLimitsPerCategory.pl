#!/usr/bin/perl
# -w

$channel=shift;
$Input=shift;
$UncsDir=shift;
$cat=shift;
$mass=shift;


#$limitcommand="limit.py --asymptotic --expectedOnly --noprefit   --userOpt '--minosAlgo stepping'";
#$limitcommand="limit.py --asymptotic --expectedOnly  --userOpt '--minosAlgo stepping'";
$limitcommand="limit.py --asymptotic --userOpt '--minosAlgo stepping'";

###plot layout
$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout.py";
#$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout_30.py";

@Cat=($cat);
if($cat==-1) {
    @Cat=(2,3,5);
}
if($cat==6) {
    @Cat=(0,1,2,3,5);
}

@Mass=($mass);
if($mass==0){
    @Mass=(110,115,120,125,130,135,140,145);
}

$ch="mt";
if($channel==2||$channel==102){
    $ch="et";
}

$ECMS="7";
if($channel>100){
    $ECMS="8";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012.py";
    #$layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_30.py";
    $layout="/afs/cern.ch/user/b/benitezj/public/datacards/sm_htt_layout2012_15.py";
}

###scale to the crossections
`cp ${Input} ./htt_${ch}.inputs-sm-${ECMS}TeV.root`;
`scale2SM.py -i htt_${ch}.inputs-sm-${ECMS}TeV.root --samples="ggH, qqH, VH" -e ${ECMS} -v 110-145:5`;

#######limit for each category
print "creating datacard txt files:\n";
if($cat==6){
    `rm -rf SMAll`;
    `mkdir SMAll`;
	
    foreach  $i (@Mass){
	print "${i} ";
	`mkdir SMAll/${i}`;	  
	
	foreach $d (@Cat){
	    chdir("./SMAll/${i}");
            `create-datacard.py -i ../../htt_${ch}.inputs-sm-${ECMS}TeV.root  -o ${d}_${i}.txt -c ${UncsDir}/cgs-sm-${ECMS}TeV-0${d}.conf -u ${UncsDir}/unc-sm-${ECMS}TeV-0${d}.vals -d ${UncsDir}/unc-sm-${ECMS}TeV-0${d}.conf ${i}`;
	    chdir("../../.");
	    
	}
	print "\n";
    }
    print "Computing limits:\n";
    print "${limitcommand} ./SMAll/*\n";
    `${limitcommand} ./SMAll/*`;

}else {
    foreach $d (@Cat){
	print "SM${d}: ";
        `rm -rf SM${d}`;
	`mkdir SM${d}`;
	
	foreach  $i (@Mass){
            print "${i} ";
	    `rm -rf ./SM${d}/${i}`;
	    `mkdir SM${d}/${i}`;	  
	    
	    chdir("./SM${d}/${i}");
            `create-datacard.py -i ../../htt_${ch}.inputs-sm-${ECMS}TeV.root  -o ${d}_${i}.txt -c ${UncsDir}/cgs-sm-${ECMS}TeV-0${d}.conf -u ${UncsDir}/unc-sm-${ECMS}TeV-0${d}.vals -d ${UncsDir}/unc-sm-${ECMS}TeV-0${d}.conf ${i}`;
	    chdir("../../.");
	    
	}
	print "\n";
    }
    print "Computing limits:\n";
    foreach $d (@Cat){
	print "${limitcommand} ./SM${d}/*\n";
	`${limitcommand} ./SM${d}/*`;
    }
}


######plot the limits 
print "Ploting the limits:\n";
`rm -f limits_sm.root`;
if($cat==6){
    print "plot asymptotic ${layout} SMAll\n";
    `rm -f SMAll_sm.*`;
    `plot asymptotic ${layout} SMAll`;
}else {
    foreach $d (@Cat){
	print "plot asymptotic ${layout} SM$d\n";
	`rm -f SM$d_sm.*`;
	`plot asymptotic ${layout} SM$d`;
    }
}

