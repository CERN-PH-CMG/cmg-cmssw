X=$PWD/$1; 
if test \! -d $X/TTH; then echo "Did not find TTH in $X"; exit 1; fi
test -L $PWD/Reference_TTH || ln -sd $PWD/Reference_TTH $X/ -v;
( cd ../python/plotter; 
  python mcPlots.py -f --s2v --tree treeProducerSusyMultilepton  -P $X susy-multilepton/validation_mca.txt susy-multilepton/validation.txt susy-multilepton/validation_plots.txt --pdir plots/72X/validation -p ref_ttH,ttH -u -e --plotmode=nostack --showRatio --maxRatioRange 0.65 1.35 --flagDifferences
)
