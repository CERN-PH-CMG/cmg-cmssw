#!/usr/bin/perl
# -w

$taskdir=shift;
#$resubmit=shift;
#$prevpath=$ENV{'PWD'};

print "checkFailed.pl will check for failed jobs in $taskdir\n";

opendir(DIR,$taskdir) or die "can't opendir ${taskdir} $!";
closedir(DIR);
@executefiles = `ls ${taskdir} | grep Job_ `;
$njobs=@executefiles;
if($njobs<1){ die "no jobs found in ${taskdir} $!";}

$ngoodfiles=0;
$totalfiles=0;
$ngoodevents=0;
$ntotpassedevents=0;
$nresubmitted=0;
foreach $job (@executefiles){
	chomp($job);
	$logfile=`find $taskdir/$job/ | grep STDOUT `;	
	chomp($logfile);
#	@logfiletype=split("STDOUT",$logfile);
#	if($logfiletype[1] eq ".gz"){
#	    system("gunzip $logfile");
#	    $logfile=`find $taskdir/$job/ | grep STDOUT `;	
#	    chomp($logfile);
#	}
	#print "$logfile\n";


	$success=0; 
	$exitcode="";
	$execemption=0;
	$nevents=0;
	$npassedevents=0;	
	$logexists=`[ -f ${logfile} ] && echo 1 || echo 0`;
	if($logexists==1){
	    $success=1; 
	    open FILE, "< $logfile";	
	    while($buffer=<FILE>){
		chomp $buffer;
		#print "$buffer \n";
		@exitword=split(" ",$buffer);
		

		if( $exitword[0] eq "cms::Exception" and $execemption==0){
		    $success=0;
		    $exitcode="${exitcode} cms::Exception";
		    $execemption=1;
		}	

		if( $exitworkd[0] eq "Problem" and $exitworkd[1] eq "with" and $exitworkd[0] eq "configuration" and $exitworkd[0] eq "file"){
		    $success=0;
		    $exitcode="${exitcode} configuration";
		}

		if( $exitword[0] eq "----" and 	$exitword[1] eq "ProductNotFound"){
		    $success=0;
		    $exitcode="${exitcode} ProductNotFound";
		}
		

		############look for bad collections
		if($exitword[0] eq "RootInputFileSequence::initFile():" and $exitword[1] eq "Input" and $exitword[2] eq "file"
		   and $exitword[4] eq "was" and $exitword[5] eq "not"  and $exitword[6] eq "found,"){
		    print "bad collection : $exitword[3] \n";
		    $badcolletionlist[$badcollectionindex]=$exitword[3];
		    $badcollectionindex++;
		}

		##########check number of processed events
		if($exitword[0] eq "TrigReport" and $exitword[1] eq "Events" and $exitword[2] eq "total" and $exitword[5] eq "passed" and $exitword[8] eq "failed" ){
		    $nevents = $exitword[4];
		    $npassedevents=$exitword[7];
		}				
	    }


	    if($nevents < 2){ $success=0; $exitcode="${exitcode} EventsProcessed"; $noevents++;}
	    
	}
	

	###check the root file
	#$rootexists = `nsls /castor/cern.ch/cms$outputdir/tree_CMG_${id}.root`;
	#chomp($rootexists);
	#if($rootexists eq ""){ $success=0;   $exitcode="no root file"; $nbadrootfiles++;}


	############
	if($success==1){
	    $ngoodfiles++;
	    #print "$logfile : Success : EventsPassed $npassedevents/$nevents \n";
	}	
	if($success==0){
	    print "${taskdir}/${job} : Failed : EventsPassed $npassedevents/$nevents : $exitcode \n";
#	    if($resubmit==1){
#		print "resubmitting ${taskdir}/${job}\n";
#
#		$lsf=`ls ${taskdir}/${job}/ | grep LSFJOB_ `;	
#		chomp($lsf);
#		@lsftype=split("LSFJOB_",$lsf);
#		if($lsftype[1]>0){
#		    #print "rm -R -f ${taskdir}/${job}/${lsf}\n";
#		    `rm -R -f ${taskdir}/${job}/${lsf}`;
#		}
#	
#		$cfg=`ls ${taskdir}/${job}/ | grep run_cfg `;	
#		chomp($cfg);
#		@cfgtype=split("py",$cfg);
#		if($cfgtype[1] eq ".gz"){
#		    #print "gunzip ${taskdir}/${job}/${cfg}\n";
#		    `gunzip ${taskdir}/${job}/${cfg}`;
#		}
#
#
#		chdir("${taskdir}/${job}/") or die "Cant chdir to ${taskdir}/${job}/ $!";
#		#print "bsub -q 8nh < batchScript.sh \n";
#		`bsub -q 8nh < batchScript.sh`;
#		chdir($prevpath);
#		$nresubmitted++;
#	    }
	}		
	$totalfiles++;
	$ngoodevents+=$nevents; 
	$ntotpassedevents+=$npassedevents;
	close FILE;
}    

print "checkFailed.pl Summary : files: $ngoodfiles/$totalfiles, events = $ntotpassedevents/$ngoodevents \n";
