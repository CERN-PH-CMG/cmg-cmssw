X=$PWD/$1; 

# $1 is VALIDATEMT2
# copy files mt2_tree.root inside
# CMGTools/TTHAnalysis/cfg/VALIDATEMT2/SNT/mt2/mt2_tree.root
# CMGTools/TTHAnalysis/cfg/VALIDATEMT2/ETHCERN/mt2/mt2_tree.root
# to dun execute: source validate_MT2.sh VALIDATEMT2


if test \! -d $X/ETHCERN; then echo "Did not find ETHCERN in $X"; exit 1; fi
test -L $PWD/SNT || ln -sd $PWD/SNT $X/ -v;
( cd ../python/plotter; 
  python mcPlots.py -f --tree mt2  -P $X susy-mT2/validation_mca_MT2.txt susy-mT2/validation_MT2.txt susy-mT2/validation_plots_MT2.txt --pdir plots/70X/validation -p ref_ttHWWdata,ttHWWdata -u -e --plotmode=norm --showRatio --maxRatioRange 0.65 1.35 --flagDifferences
)

## susy-mT2/validation_mca.txt --> this contains stile
## susy-mT2/validation.txt --> contains selections
## susy-mT2/validation_plots.txt --> contains content and tille and ranges

