python calcWp.py  -i /store/cmst3/user/musella/JETID_NTUPLES/13_04_2012_v3/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -t pfjetanalyzer_novtxcat/tree -o ~/www/higgs/jetid_wp/baseNoVtxCat -b & 
python calcWp.py  -i /store/cmst3/user/musella/JETID_NTUPLES/13_04_2012_v3/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -t pfjetanalyzer/tree -o ~/www/higgs/jetid_wp/basePtVtxCat -b & 
python calcWp.py  -i /store/cmst3/user/musella/JETID_NTUPLES/13_04_2012_v3/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -t pfjetanalyzer_philv1/tree -o ~/www/higgs/jetid_wp/PhilV1 -b & 
python calcWp.py  -i /store/cmst3/user/musella/JETID_NTUPLES/13_04_2012_v3/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -t pfjetanalyzer/tree -o ~/www/higgs/jetid_wp/dR2Mean -v -dR2Mean:100,-0.2,0. &

wait


