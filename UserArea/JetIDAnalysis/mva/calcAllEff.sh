
python  calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-00-08/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -v fullId:10,0,10 -T common.json -o ~/www/higgs/jetid_optimization_v5/fullId -b &
python calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-00-08/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -v simpleId:21,-10.5,10.5 -T common.json -o ~/www/higgs/jetid_optimization_v5/simpleId -b &
python calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-00-08/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -v philv1Id:21,-10.5,10.5 -T common.json -o ~/www/higgs/jetid_optimization_v5/philv1Id -b &


wait


