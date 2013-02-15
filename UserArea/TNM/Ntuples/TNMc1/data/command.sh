ProductionTasks.py -w 'patTuple*.root' -c -N 5 -q 2nd -t NTUPLE_Feb15 --output_wildcard '[!h]*.root' --cfg TNMc1_Feb15_cfg.py --max_threads 6 `cat data/samples_Moriond.txt`


cmsBatch.py 5 TNMc1_Feb15_cfg.py -b 'bsub -q 1nd < ./batchScript.sh' -f -r /store/cmst3/user/hinzmann/NTUPLE/summer12_WJetsPt100 -o summer12_WJetsPt100
