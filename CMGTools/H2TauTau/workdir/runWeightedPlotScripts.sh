python produce_macros.py  -a sm -c 'mt, et, em' -u 1 -p "7TeV 8TeV"
python produce_macros.py  -a sm -c 'tt' -u 1 -p "8TeV"
sed -i 's/bool log=true/bool log=false/g' *_0_*.C
sed -i 's/BLIND_DATA = true/BLIND_DATA = false/g' *.C
python run_macros.py  -a sm -c 'mt, et, em'  -p "7TeV 8TeV"
python run_macros.py  -a sm -c 'tt'  -p "8TeV"
####cumulativePlot mt et em 7TeV 8TeV
