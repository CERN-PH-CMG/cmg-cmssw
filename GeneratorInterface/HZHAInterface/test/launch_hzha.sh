
#!/bin/sh

eosDir=/store/cmst3/user/pjanot/LEP3/
cmsswDir=$CMSSW_BASE/src

for ((job=0;job<100;job++));
  do
  echo "JOB "$job
  name="HZHA_Full_"${job}
  aodfilename="aod_"${name}".root"
  recofilename="reco_"${name}".root"
  logfilename="log_"${name}".txt"
  echo $name

  seed1=$(( ($job+1)*14312 ))
  nevt=1000
  sed -e "s/==SEED==/${seed1}/" -e "s/==NEVT==/${nevt}/" hzha_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO.py > tmp_cfg

#Start to write the script
cat > job_${name}.sh << EOF
#!/bin/sh
cd $cmsswDir
eval \`scramv1 runtime -sh\`
cd -
#commande pour decoder le .cfg
cat > TEST_cfg.py << "EOF"
EOF

#Ajoute le .cfg au script
cat  tmp_cfg>> job_${name}.sh

# On poursuit le script
echo "EOF" >> job_${name}.sh
cat >> job_${name}.sh << EOF
cmsRun TEST_cfg.py >& log

cmsStage -f aod.root $eosDir$aodfilename
cmsStage -f reco.root $eosDir$recofilename
cmsStage -f log $eosDir$logfilename

EOF
chmod 755 job_${name}.sh
bsub -q 1nd -J $name -R "mem>2000" $PWD/job_${name}.sh


done

