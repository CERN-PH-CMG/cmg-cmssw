#!/usr/bin/perl

print "reading ".$ARGV[0]."\n" ;

open (USERCONFIG,$ARGV[0]) ;

while (<USERCONFIG>)
{
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
#    next unless length;     # anything left?
    my ($var, $value) = split(/\s*=\s*/, $_, 2);
    $User_Preferences{$var} = $value;
}


$DATASETPATH      = $User_Preferences{"DATASETPATH"} ;
$CFGTEMPLATE      = $User_Preferences{"CFGTEMPLATE"} ;
$JOBDIR           = $User_Preferences{"JOBDIR"} ;
$OUTPUTDIR        = $User_Preferences{"OUTPUTDIR"} ;
$OUTPUTFILENAME   = $User_Preferences{"OUTPUTFILENAME"} ;
$JOBMODULO        = $User_Preferences{"JOBMODULO"} ;
$QUEUE            = $User_Preferences{"QUEUE"} ;

$SCRAM_ARCH = $ENV{"SCRAM_ARCH"};

$LISTFile = "./list.txt" ;
system ("cmsLs ".$DATASETPATH." | grep root | awk '{print \$5}' > ".$LISTFile."\n") ;

$totNumber = 0;
$jobNumber = 0;

open (LISTFile,$LISTFile) ;
while (<LISTFile>)
{
##print "File = ".$_;
  ++$totNumber;
}

$jobNumber = int($totNumber/$JOBMODULO);
if( $totNumber%$JOBMODULO != 0)
{
  $jobNumber = $jobNumber+1;
}

print "NumberOfFiles = ".$totNumber."\n";
print "NumberOfJobs = ".$jobNumber."\n";



###############################
# create folders for job output
##############################    

print ("mkdir ".$JOBDIR."\n") ;
system ("mkdir ".$JOBDIR."\n") ;


################
# loop over jobs 
################

for($jobIt = 1; $jobIt <= $jobNumber; ++$jobIt)
{ 
  $currDir = `pwd`;
  system ("echo ".$currDir." \n");
  chomp ($currDir) ;
  system ("cd ".$currDir." \n");
  system ("mkdir ".$JOBDIR."/JOB_".$jobIt." \n") ;
  $currDir = $currDir."/".$JOBDIR."/JOB_".$jobIt ;
 
  ## set output file name in template cfg
  $tempo1 = "./tempo1" ;
  $tempfilename = $OUTPUTFILENAME."_".$jobIt.".root" ;
  system ("cat ".$CFGTEMPLATE." | sed -e s%OUTPUTFILENAME%".$tempfilename."%g > ".$tempo1."\n") ;
  

  ## make list of files
  $listOfFiles;   
  $it = 0;
  open (LISTFile2,$LISTFile) ;
  while (<LISTFile2>)
  {
      chomp; 
      s/#.*//;                # no comments
      s/^\s+//;               # no leading white
      s/\s+$//;               # no trailing white
      $file = $_ ;
      
      if( ($it >= ($jobIt - 1)*$JOBMODULO) && ($it < ($jobIt)*$JOBMODULO) )
      { 
	  $listOfFiles = $listOfFiles."\\'root://eoscms//eos/cms/".$file."\\',\\\n" ; # ma t pare?!?!
      }
      ++$it;
  }
  
  chop($listOfFiles) ;
  #print $listOfFiles."\n" ;

  $tempo2 = "./tempo2" ;    
  system ("cat ".$tempo1." | sed -e s%LISTOFFILES%".$listOfFiles."%g > ".$tempo2."\n") ;
  $listOfFiles = "" ;
     
  print("echo ".$currDir);
  $CFGFILE = $currDir."/".$jobIt."_cfg.py" ;
  system ("mv ".$tempo2." ".$CFGFILE) ;
  system ("rm ./tempo*") ;
  
  $tempBjob = $currDir."/bjob_".$jobIt.".sh" ;
  $command = "touch ".$tempBjob ;
  system ($command) ;
  $command = "chmod 777 ".$tempBjob ;
  system ($command) ;
    
  $command = "cd ".$currDir ;
  system ("echo ".$command." > ".$tempBjob) ;
  
  ## Does not work for 52x
  ### $command = "source /afs/cern.ch/cms/sw/cmsset_default.sh" ;
  ### system ("echo ".$command." >> ".$tempBjob) ;

  $command = "export SCRAM_ARCH=".$SCRAM_ARCH ;
  system ("echo ".$command." >> ".$tempBjob) ;
  
  $command = "eval '\$(scram ru -sh)'" ;
  system ("echo ".$command." >> ".$tempBjob) ;

  ### $command = "cmsenv" ;
  ### system ("echo ".$command." >> ".$tempBjob) ;

  $command = "" ;
  system ("echo ".$command." >> ".$tempBjob) ;

  $command = "cd -" ;
  system ("echo ".$command." >> ".$tempBjob) ;
  
  $command = "cmsRun ".$CFGFILE ;
  system ("echo ".$command." >> ".$tempBjob) ;
  
  
  
######################
# copy files to CMS T3?
######################    
  
  $command = "cmsMkdir ".$OUTPUTDIR; 
  system ("echo ".$command." >> ".$tempBjob) ;

  $command = "cmsStage -f ./".$OUTPUTFILENAME."_".$jobIt.".root ".$OUTPUTDIR; 
  system ("echo ".$command." >> ".$tempBjob) ;
  
############
# submit job
############
  
  system ("cd ".$currDir." \n") ;
  print ("bsub -q ".$QUEUE." -cwd ".$currDir." ".$tempBjob."\n") ;
  system ("bsub -q ".$QUEUE." -cwd ".$currDir." ".$tempBjob."\n") ;
  
  print "\n" ;
}
  

 

   
