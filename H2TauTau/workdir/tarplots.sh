export path="/data/benitezj/Samples"

##mu-tau 2011
#export tag="TauMuV541June2_TrigEff"

##e-tau 2011
#export tag="TauEleV541June29_FlatNtpJune29"

##mu-tau 2012
#export tag="TauMu2012V5_4_0_NewType1MET"
#export tag="TauMu2012V5_4_0_NewType1MET4"
#export tag="TauMu2012V5_4_0_NewType1MET4_2011WJets"
#export tag="TauMu2012V540_July7_2011WJets"

###e-tau 2012
#export tag="TauEle2012V5_4_0_NewType1MET3"
export tag="TauEle2012V5_4_0_NewType1MET4"
#export tag="TauEle2012V5_4_0_NewType1MET5"


rm -rf Plots_$tag
mkdir Plots_$tag
#mkdir Plots_$tag/epslog
#mv $path/$tag/*log.eps Plots_$tag/epslog
#mkdir Plots_$tag/eps
#mv $path/$tag/*.eps Plots_$tag/eps
mkdir Plots_$tag/pnglog
mv $path/$tag/*log.png Plots_$tag/pnglog
mkdir Plots_$tag/png
mv $path/$tag/*.png Plots_$tag/png
tar -cvf Plots_$tag.tar Plots_$tag

