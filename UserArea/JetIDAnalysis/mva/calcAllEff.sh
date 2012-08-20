
###### python  calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-00-08/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -v fullId:10,0,10 -T common.json -o ~/www/higgs/jetid_optimization_v5/fullId -b &
###### python calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-00-08/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -v simpleId:21,-10.5,10.5 -T common.json -o ~/www/higgs/jetid_optimization_v5/simpleId -b &
###### python calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-00-08/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -v philv1Id:21,-10.5,10.5 -T common.json -o ~/www/higgs/jetid_optimization_v5/philv1Id -b &

#### python  calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-00-08/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -v fullId:10,0,10 -T common.json -o ~/www/higgs/jetid_optimization_v5/fullId_vtx10_20 -b &
#### python calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-00-08/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -v simpleId:21,-10.5,10.5 -T common.json -o ~/www/higgs/jetid_optimization_v5/simpleId_vtx10_20 -b &
#### python calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-00-08/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -v philv1Id:21,-10.5,10.5 -T common.json -o ~/www/higgs/jetid_optimization_v5/philv1Id_vtx10_20 -b &

### python  calcEff.py  -i  /store/cmst3/user/musella/JETID_NTUPLES/V00-02-01/DYJetsToLL-51 -v fullId:10,0,10 -T common.json -o ~/www/higgs/jetid_optimization_5x_v3/eff_fullId_retuned -b &
### python calcEff.py  -i  /store/cmst3/user/musella/JETID_NTUPLES/V00-02-01/DYJetsToLL-51 -v simpleId:21,-10.5,10.5 -T common.json -o ~/www/higgs/jetid_optimization_5x_v3/eff_simpleId_retuned -b &
### python calcEff.py  -i  /store/cmst3/user/musella/JETID_NTUPLES/V00-02-01/DYJetsToLL-51 -v cutbasedId:21,-10.5,10.5 -T common.json -o ~/www/higgs/jetid_optimization_5x_v3/eff_cutbasedId_retuned -b &

#### python  calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-02-06/44X/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -v fullId:10,0,10 -T common.json -o ~/www/higgs/jetid_V00-02-06/eff_fullId_4X -b &
#### python calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-02-06/44X/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -v simpleId:21,-10.5,10.5 -T common.json -o ~/www/higgs/jetid_V00-02-06/eff_simpleId_4X -b &
#### python calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-02-06/44X/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -v cutbasedId:21,-10.5,10.5 -T common.json -o ~/www/higgs/jetid_V00-02-06/eff_cutbasedId_4X -b &
#### 
#### 
#### python  calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-02-06/52X_newJec/DYJetsToLL-51 -v fullId:10,0,10 -T common.json -o ~/www/higgs/jetid_V00-02-06/eff_fullId_5X -b &
#### python calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-02-06/52X_newJec/DYJetsToLL-51 -v simpleId:21,-10.5,10.5 -T common.json -o ~/www/higgs/jetid_V00-02-06/eff_simpleId_5X -b &
#### python calcEff.py  -i  /store/cmst3/user/malberti/JETID_NTUPLES/V00-02-06/52X_newJec/DYJetsToLL-51 -v cutbasedId:21,-10.5,10.5 -T common.json -o ~/www/higgs/jetid_V00-02-06/eff_cutbasedId_5X -b &

python calcEff.py -i /store/cmst3/user/malberti/JETID_NTUPLES/V00-02-06/52X_newJec/DYJetsToLL-51 -v cutbasedId:21,-10.5,10.5 -T common.json,leadjet.json -o ~/www/higgs/jetid_V00-02-06/eff_cutbasedId_leadjet_5X -b &

wait


