#!/usr/bin/env bash

ProductionTasks.py --check -w PFAOD*.root -N 1 -q 2nd -t PAT_CMG_V2_4_0 --output_wildcard [!h]*.root --cfg patTuple_PF2PAT_forCMG_cfg.py `cat samples_mc.txt`
