export path="/data/benitezj/Samples"

##mu-tau 2011
#export tag="TauMuV541June2_TrigEff"
#export tag="TauMu2011FlatNtpJuly25"
#export tag="TauMu2011FlatNtpJuly26"
#export tag="TauMu2011FlatNtpJuly26svfit"

##e-tau 2011
#export tag="TauEleV541June29_FlatNtpJune29"
#export tag="TauEle2011FlatNtpJuly25"
#export tag="TauEle2011FlatNtpJuly26"
#export tag="TauEle2011FlatNtpJuly26svfit"

##mu-tau 2012
#export tag="TauMu2012V5_4_0_NewType1MET"
#export tag="TauMu2012V5_4_0_NewType1MET4"
#export tag="TauMu2012V5_4_0_NewType1MET4_2011WJets"
#export tag="TauMu2012V540_July7_2011WJets"
#export tag="TauMu2012V551Aug8svfitPFMET"
#export tag="TauMu2012V551Aug6svfit"

###e-tau 2012
#export tag="TauEle2012V5_4_0_NewType1MET3"
#export tag="TauEle2012V5_4_0_NewType1MET4"
#export tag="TauEle2012V5_4_0_NewType1MET5"
#export tag="TauEle2012V551Aug8svfitPFMET"
export tag="TauEle2012V551Aug7svfit"


export dirname=Plots_${tag}_${1}
echo $dirname
############################################
rm -rf $dirname
mkdir $dirname

#mkdir $dirname/epslog
#mv $path/$tag/*log.eps $dirname/epslog
#mkdir $dirname/eps
#mv $path/$tag/*.eps $dirname/eps

mkdir $dirname/pnglog
mv $path/$tag/*log.png $dirname/pnglog

mkdir $dirname/png
mv $path/$tag/*.png $dirname/png

tar -cvf $dirname.tar $dirname
rm -rf $dirname
###########################################