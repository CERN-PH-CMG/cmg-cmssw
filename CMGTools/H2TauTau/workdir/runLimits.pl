#!/usr/bin/perl
# -w

$muTauInput=shift;
$muTauUncsDir=shift;
$eTauInput=shift;
$eTauUncsDir=shift;
#1=muTau, 2=eTau, 12=muTau+eTau
$channel=shift;


####inputs
#$muTauInput="/afs/cern.ch/user/b/bianchi/public/Roger/datacards2011v3/muTauSM.root";
#$eTauInput="/afs/cern.ch/user/b/bianchi/public/Roger/datacards2011v3/eTauSM.root";
#$muTauInput="/afs/cern.ch/user/b/benitezj/public/datacards/2011/Aug4/muTauSM_svfitmass.root";
#$eTauInput="/afs/cern.ch/user/b/benitezj/public/datacards/2011/Aug4/eTauSM_svfitmass.root";

#####uncertainties
#$muTauUncsDir="/afs/cern.ch/user/b/benitezj/scratch0/V5_5_0_44X/CMGTools/CMSSW_4_4_4/src/HiggsAnalysis/HiggsToTauTau/setup/mt";
#$eTauUncsDir="/afs/cern.ch/user/b/benitezj/scratch0/V5_5_0_44X/CMGTools/CMSSW_4_4_4/src/HiggsAnalysis/HiggsToTauTau/setup/et";




###plot layout
$layoutDir="/afs/cern.ch/user/b/benitezj/public/datacards";

$Ncat=5;
@cat=(0,1,2,3,5);
$Nmass=8;
@mass=(110,115,120,125,130,135,140,145);

if($channel==1){

    #####setup root file
    print "Setup root file\n";
    `rm -rf muTau`;
    `mkdir muTau`;
    chdir("muTau");
    `cp ${muTauInput} ./muTauSM.root`;
    `scale2SM.py -i muTauSM.root  -e 7 -v 110-145:5`;

    ##########create datacards
    print "make the data cards:\n";
    foreach $d (@cat){
	foreach $i (@mass){
	    #print "SM${d}_${i}\n";
	    `create-datacard.py -i muTauSM.root -o muTau_${d}_${i}.txt -c ${muTauUncsDir}/cgs-sm-7TeV-0${d}.conf -u ${muTauUncsDir}/unc-sm-7TeV-0${d}.vals -d ${muTauUncsDir}/unc-sm-7TeV-0${d}.conf ${i}`;
	}
    }
    
    #######compute limit for each category
    print "make directories per category:\n";
    foreach $d (@cat){
	#print "SM${d}\n";
	`rm -rf SM${d}`;
	`mkdir SM${d}`;
	foreach  $i (@mass){
	    #print "SM${d}/${i}\n";
	    `mkdir SM${d}/${i}`;
	    `rm -f ./SM${d}/${i}/muTau_${d}_${i}.txt`;
	    `cp muTau_${d}_${i}.txt ./SM${d}/${i}/.`;
	    `rm -f ./SM${d}/${i}/muTauSM.root`;
	    `cp muTauSM.root ./SM${d}/${i}/.`;
	}
    }
    print "computing limits per category:\n";
    foreach $d (@cat){
	#print "SM${d}\n";
	`limit.py --asymptotic --expectedOnly  --userOpt '--minosAlgo stepping ' SM${d}/*`;
    }
    ######plot the limits 
    `rm -f limits_sm.root`;
    foreach $d (@cat){
	#print "plot $d\n";
	`rm -f SM$d_sm.*`;
	`plot asymptotic $layoutDir/sm_htt_layout.py SM$d`;
    }
   
    #######Compute combined limit
    print "make directory for combined:\n";
    `rm -rf SMAll`;
    `mkdir SMAll`;
    foreach $i (@mass){
	#print "SMAll/${i}\n";
	`mkdir SMAll/${i}`;
	`rm -f ./SMAll/${i}/muTauSM.root`;
	`cp muTauSM.root ./SMAll/${i}/.`;
	for $d (@cat){
	    #print "${d}\n";
	    `rm -f ./SMAll/${i}/muTau_${d}_${i}.txt`;
	    `cp muTau_${d}_${i}.txt ./SMAll/${i}/.`;
	}
    }
    print "compute combined limit:\n";
    `limit.py --asymptotic --expectedOnly  --userOpt '--minosAlgo stepping ' SMAll/*`;
    `plot asymptotic $layoutDir/sm_htt_layout.py SMAll`;

    chdir("..");
}


if($channel==2){

    #####setup root file
    print "Setup root file\n";
    `rm -rf eTau`;
    `mkdir eTau`;
    chdir("eTau");
    `cp ${eTauInput} ./eTauSM.root`;
    `scale2SM.py -i eTauSM.root  -e 7 -v 110-145:5`;

    ##########create datacards
    print "make the data cards:\n";
    foreach $d (@cat){
	foreach $i (@mass){
	    #print "SM${d}_${i}\n";
	    `create-datacard.py -i eTauSM.root -o eTau_${d}_${i}.txt -c ${eTauUncsDir}/cgs-sm-7TeV-0${d}.conf -u ${eTauUncsDir}/unc-sm-7TeV-0${d}.vals -d ${eTauUncsDir}/unc-sm-7TeV-0${d}.conf ${i}`;
	}
    }
    
    #######compute limit for each category
    print "make directories per category:\n";
    foreach $d (@cat){
	#print "SM${d}\n";
	`rm -rf SM${d}`;
	`mkdir SM${d}`;
	foreach  $i (@mass){
	    #print "SM${d}/${i}\n";
	    `mkdir SM${d}/${i}`;
	    `rm -f ./SM${d}/${i}/eTau_${d}_${i}.txt`;
	    `cp eTau_${d}_${i}.txt ./SM${d}/${i}/.`;
	    `rm -f ./SM${d}/${i}/eTauSM.root`;
	    `cp eTauSM.root ./SM${d}/${i}/.`;
	}
    }
    print "computing limits per category:\n";
    foreach $d (@cat){
	#print "SM${d}\n";
	`limit.py --asymptotic --expectedOnly  --userOpt '--minosAlgo stepping ' SM${d}/*`;
    }
    ######plot the limits 
    `rm -f limits_sm.root`;
    foreach $d (@cat){
	#print "plot $d\n";
	`rm -f SM$d_sm.*`;
	`plot asymptotic $layoutDir/sm_htt_layout.py SM$d`;
    }
   
    #######Compute combined limit
    print "make directory for combined:\n";
    `rm -rf SMAll`;
    `mkdir SMAll`;
    foreach $i (@mass){
	#print "SMAll/${i}\n";
	`mkdir SMAll/${i}`;
	`rm -f ./SMAll/${i}/eTauSM.root`;
	`cp eTauSM.root ./SMAll/${i}/.`;
	for $d (@cat){
	    #print "${d}\n";
	    `rm -f ./SMAll/${i}/eTau_${d}_${i}.txt`;
	    `cp eTau_${d}_${i}.txt ./SMAll/${i}/.`;
	}
    }
    print "compute combined limit:\n";
    `limit.py --asymptotic --expectedOnly  --userOpt '--minosAlgo stepping ' SMAll/*`;
    `plot asymptotic $layoutDir/sm_htt_layout.py SMAll`;

    chdir("..");
}


if($channel==12){
    
    #####setup root file
    print "Setup root file\n";
    `rm -rf muTaueTau`;
    `mkdir muTaueTau`;
    chdir("muTaueTau");
    `cp ${muTauInput} ./muTauSM.root`;
    `cp ${eTauInput} ./eTauSM.root`;
    `scale2SM.py -i muTauSM.root  -e 7 -v 110-145:5`;
    `scale2SM.py -i eTauSM.root  -e 7 -v 110-145:5`;

    ##########create datacards
    print "make the data cards:\n";
    foreach $d (@cat){
	foreach $i (@mass){
	    #print "SM${d}_${i}\n";
	    `create-datacard.py -i muTauSM.root -o muTau_${d}_${i}.txt -c ${muTauUncsDir}/cgs-sm-7TeV-0${d}.conf -u ${muTauUncsDir}/unc-sm-7TeV-0${d}.vals -d ${muTauUncsDir}/unc-sm-7TeV-0${d}.conf ${i}`;
	    `create-datacard.py -i eTauSM.root -o eTau_${d}_${i}.txt -c ${eTauUncsDir}/cgs-sm-7TeV-0${d}.conf -u ${eTauUncsDir}/unc-sm-7TeV-0${d}.vals -d ${eTauUncsDir}/unc-sm-7TeV-0${d}.conf ${i}`;

	}
    }
    
    #######compute limit for each category
    print "make directories per category:\n";
    foreach $d (@cat){
	#print "SM${d}\n";
	`rm -rf SM${d}`;
	`mkdir SM${d}`;
	foreach  $i (@mass){
	    #print "SM${d}/${i}\n";
	    `mkdir SM${d}/${i}`;

	    `rm -f ./SM${d}/${i}/muTau_${d}_${i}.txt`;
	    `cp muTau_${d}_${i}.txt ./SM${d}/${i}/.`;
	    `rm -f ./SM${d}/${i}/muTauSM.root`;
	    `cp muTauSM.root ./SM${d}/${i}/.`;

	    `rm -f ./SM${d}/${i}/eTau_${d}_${i}.txt`;
	    `cp eTau_${d}_${i}.txt ./SM${d}/${i}/.`;
	    `rm -f ./SM${d}/${i}/eTauSM.root`;
	    `cp eTauSM.root ./SM${d}/${i}/.`;

	}
    }
    print "computing limits per category:\n";
    foreach $d (@cat){
	#print "SM${d}\n";
	`limit.py --asymptotic --expectedOnly  --userOpt '--minosAlgo stepping ' SM${d}/*`;
    }
    ######plot the limits 
    `rm -f limits_sm.root`;
    foreach $d (@cat){
	#print "plot $d\n";
	`rm -f SM$d_sm.*`;
	`plot asymptotic $layoutDir/sm_htt_layout.py SM$d`;
    }
   
    #######Compute combined limit
    print "make directory for combined:\n";
    `rm -rf SMAll`;
    `mkdir SMAll`;
    foreach $i (@mass){
	#print "SMAll/${i}\n";
	`mkdir SMAll/${i}`;

	`rm -f ./SMAll/${i}/muTauSM.root`;
	`cp muTauSM.root ./SMAll/${i}/.`;

	`rm -f ./SMAll/${i}/eTauSM.root`;
	`cp eTauSM.root ./SMAll/${i}/.`;

	for $d (@cat){
	    #print "${d}\n";
	    `rm -f ./SMAll/${i}/muTau_${d}_${i}.txt`;
	    `cp muTau_${d}_${i}.txt ./SMAll/${i}/.`;

	    `rm -f ./SMAll/${i}/eTau_${d}_${i}.txt`;
	    `cp eTau_${d}_${i}.txt ./SMAll/${i}/.`;
	}
    }
    print "compute combined limit:\n";
    `limit.py --asymptotic --expectedOnly  --userOpt '--minosAlgo stepping ' SMAll/*`;
    `plot asymptotic $layoutDir/sm_htt_layout.py SMAll`;

    chdir("..");
}

