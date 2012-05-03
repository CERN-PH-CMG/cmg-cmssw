#### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v3 -v BDT_full:100,-1,1 -f tmva_full.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v3/wp_BDT_full -b &
#### 
#### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v3 -v BDT_simpleNoVtxCat:100,-1,1 -f tmva_simpleNoVtxCat.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v3/wp_BDT_simpleNoVtxCat -b &
#### 
#### python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v3 -v BDT_simple:100,-1,1 -f tmva_simple.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v3/wp_BDT_simple -b &
#### 

python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v3 -v BDT_full:100,-1,1 -f tmva_full.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v3/wp_BDT_full_int -a -b &

python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v3 -v BDT_simpleNoVtxCat:100,-1,1 -f tmva_simpleNoVtxCat.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v3/wp_BDT_simpleNoVtxCat_int -a -b &

python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v3 -v BDT_fullPlusRMS:100,-1,1 -f tmva_fullPlusRMS.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v3/wp_BDT_fullPlusRMS_int -a -b &

python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v3 -v BDT_full:100,-1,1 -f tmva_full.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v3/wp_BDT_full_nvtx -V -a -b &

python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v3 -v BDT_simpleNoVtxCat:100,-1,1 -f tmva_simpleNoVtxCat.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v3/wp_BDT_simpleNoVtxCat_nvtx -V -a -b &

python calcWp.py  -i  /store/cmst3/user/musella/JETID_MVA_5x_v3 -v BDT_fullPlusRMS:100,-1,1 -f tmva_fullPlusRMS.root              -t TestTree  -T common.json,testtree.json -o ~/www/higgs/jetid_optimization_5x_v3/wp_BDT_fullPlusRMS_nvtx -V -a -b &

wait


