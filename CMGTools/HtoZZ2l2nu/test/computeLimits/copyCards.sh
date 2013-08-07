mkdir -p DATACARDS
cd DATACARDS
cp -rd ../cards7TeV/* .
cp -rd ../cards8TeV/* .
rm */*.tex
rm */*.png
rm */*.pdf
rm */*.C
rm */*.log
rm */*.sh
rm */card_*.dat
rm */*_Hybrid*.root
rm */higgsCombine*.root
rm */fit.txt
rm */mlfit.root
rm */roostats*
cd ..
#cp -rd DATACARDS/* /afs/cern.ch/user/q/querten/scratch0/ICHEP2012_Higgs/ichep2012/.
