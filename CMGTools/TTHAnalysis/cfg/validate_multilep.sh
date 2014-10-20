X=$PWD/$1; 
if test \! -d $X/TTHToWW_PUS14; then echo "Did not find TTHToWW_PUS14 in $X"; exit 1; fi
test -L $PWD/Reference_TTHToWW_PUS14 || ln -sd $PWD/Reference_TTHToWW_PUS14 $X/ -v;
( cd ../python/plotter; 
  python mcPlots.py -f --s2v --tree treeProducerSusyMultilepton  -P $X bins/validation_mca.txt bins/validation.txt bins/validation_plots.txt --pdir plots/70X/validation -p ref_ttHWW,ttHWW -u -e --plotmode=nostack --showRatio --maxRatioRange 0.65 1.35 --flagDifferences
)
