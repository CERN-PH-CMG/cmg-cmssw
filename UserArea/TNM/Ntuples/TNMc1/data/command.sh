ProductionTasks.py -w 'patTuple*.root' -c -N 5 -q 2nd -t NTUPLE_Oct25 --output_wildcard '[!h]*.root' --cfg TNMc1_cfg.py --max_threads 6 `cat data/samples_53X.txt`


cmsBatch.py 1 cfg.py -b 'bsub -q 1nd < ./batchScript.sh' -f -r /store/cmst3/user/hinzmann/NTUPLE/RadionToHHTo4B_1TeV