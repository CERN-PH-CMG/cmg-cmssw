# !/bin/bash

read -p "Are you sure? (Type 'y' to confirm) " -n 1 -r
echo    # (optional) move to a new line
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
    exit 1
fi

read -p "Are you REALLY sure? (Type 'n' to confirm this time) " -n 1 -r
echo    # (optional) move to a new line
if [[ ! $REPLY =~ ^[Nn]$ ]]
then
    exit 1
fi

echo 'Waiting 5 sec before to START KILLING JOBS to let you think about it...'
sleep 5

declare RESULT=($(bjobs |grep 1nh | awk '{print $1}'))
# declare RESULT=($(bjobs | awk '{print $1}'))

for (( job_id=0; job_id<${#RESULT[@]}; job_id++ ))
  do
  echo 'killing job ' ${RESULT[job_id]}
  bkill  ${RESULT[job_id]}
  # sleep 1

done


# declare RESULT=($(bjobs |awk '{print $1}'))

# for (( job_id=0; job_id<${#RESULT[@]}; job_id++ ))
  # do
  # echo 'switching job ' ${RESULT[job_id]} ' to queue 1nd'
  # bswitch 1nd ${RESULT[job_id]}
  # sleep 1

# done

