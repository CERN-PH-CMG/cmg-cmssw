ProductionTasks.py -w 'patTuple*.root' -c -N 5 -q 2nd -t NTUPLE_Feb15 --output_wildcard '[!h]*.root' --cfg TNMc1_Feb15_cfg.py --max_threads 6 `cat data/samples_Moriond.txt`


cmsBatch.py 1 cfg.py -b 'bsub -q 1nd < ./batchScript.sh' -f -r /store/cmst3/user/hinzmann/NTUPLE/RadionToHHTo4B_1TeV
