###### python calcWp.py  -i /store/cmst3/user/musella/JETID_NTUPLES/13_04_2012_v3/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -t pfjetanalyzer_novtxcat/tree -o ~/www/higgs/jetid_wp/baseNoVtxCat -b & 
###### python calcWp.py  -i /store/cmst3/user/musella/JETID_NTUPLES/13_04_2012_v3/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -t pfjetanalyzer/tree -o ~/www/higgs/jetid_wp/basePtVtxCat -b & 
###### python calcWp.py  -i /store/cmst3/user/musella/JETID_NTUPLES/13_04_2012_v3/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -t pfjetanalyzer_philv1/tree -o ~/www/higgs/jetid_wp/PhilV1 -b & 
###### python calcWp.py  -i /store/cmst3/user/musella/JETID_NTUPLES/13_04_2012_v3/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola -t pfjetanalyzer/tree -o ~/www/higgs/jetid_wp/dR2Mean -v -dR2Mean:100,-0.2,0. &

### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v4 -v BDT_baseNoPtCats:100,-1,1 -f tmva_baseNoPtCats.root		-t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_wp/baseNoPtCats &
### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v4 -v BDT_basePtCats:100,-1,1 -f tmva_basePtCats.root		-t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_wp/basePtCats &
### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v4 -v BDT_minimalNoPtCats:100,-1,1 -f tmva_minimalNoPtCats.root	-t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_wp/minimalNoPtCats &
### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v4 -v BDT_minimalPtCats:100,-1,1 -f tmva_minimalPtCats.root		-t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_wp/minimalPtCats &
### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v4 -v Fisher_tkImpact.root:100,-1,1 -f tmva_tkImpact.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_wp/tkImpact &

## python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v5 -v Fisher_tkImpact:100,-1,1 -f tmva_tkImpact.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_v5/wp_Fisher_tkImpact -b &
#### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v5 -v BDT_tkImpact:100,-1,1 -f tmva_tkImpact.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_v5/wp_BDT_tkImpact -b &
#### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v5 -v beta:200,0,1 -f tmva_tkImpact.root              -t TestTree  -T tmva_TK.json,testtree.json -o ~/www/higgs/jetid_optimization_v5/wp_beta_tkImpact -b &

### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v5 -v BDT_optimalTwoPtCats:100,-1,1 -f tmva_optimalTwoPtCats.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_v5/wp_BDT_optimalTwoPtCats -b &
### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v5 -v BDT_optimalPtTwoPtCats:100,-1,1 -f tmva_optimalPtTwoPtCats.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_v5/wp_BDT_optimalPtTwoPtCats -b &
### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v5 -v BDT_optimalNoPtCats:100,-1,1 -f tmva_optimalNoPtCats.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_v5/wp_BDT_optimalNoPtCats -b &
### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v5 -v BDT_optimalNoVtxCatsTwoPtCats:100,-1,1 -f tmva_optimalNoVtxCatsTwoPtCats.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_v5/wp_BDT_optimalNoVtxCatsTwoPtCats -b &
### 
### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v5 -v BDT_minimalTwoPtCats:100,-1,1 -f tmva_minimalTwoPtCats.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_v5/wp_BDT_minimalTwoPtCats -b &
### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v5 -v BDT_minimalPtTwoPtCats:100,-1,1 -f tmva_minimalPtTwoPtCats.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_v5/wp_BDT_minimalPtTwoPtCats -b &
### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_v5 -v BDT_minimalNoPtCats:100,-1,1 -f tmva_minimalNoPtCats.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_v5/wp_BDT_minimalNoPtCats -b &


#### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v2 -v BDT_full:100,-1,1 -f tmva_full.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v2/wp_BDT_full -b &
#### 
#### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v2 -v BDT_simpleNoVtxCat:100,-1,1 -f tmva_simpleNoVtxCat.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v2/wp_BDT_simpleNoVtxCat -b &
#### 
#### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v2 -v BDT_simple:100,-1,1 -f tmva_simple.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v2/wp_BDT_simple -b &
#### 

### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v2 -v BDT_full:100,-1,1 -f tmva_full.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v2/wp_BDT_full_int -a -b &
### 
### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v2 -v BDT_simpleNoVtxCat:100,-1,1 -f tmva_simpleNoVtxCat.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v2/wp_BDT_simpleNoVtxCat_int -a -b &
### 
### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v2 -v BDT_simple:100,-1,1 -f tmva_simple.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v2/wp_BDT_simple_int -a -b &
### 
python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v2 -v BDT_full:100,-1,1 -f tmva_full.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v2/wp_BDT_full_nvtx -V -a -b &

python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v2 -v BDT_simpleNoVtxCat:100,-1,1 -f tmva_simpleNoVtxCat.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v2/wp_BDT_simpleNoVtxCat_nvtx -V -a -b &

python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v2 -v BDT_simple:100,-1,1 -f tmva_simple.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v2/wp_BDT_simple_nvtx -V -a -b &

wait


