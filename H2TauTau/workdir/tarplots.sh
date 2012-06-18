export path="/data/benitezj/Samples"
#export tag="TauMuV541June2_PFMET"
export tag="TauMuV541June2_TrigEff"

rm -rf Plots_$tag
mkdir Plots_$tag
mkdir Plots_$tag/epslog
mv $path/$tag/*log.eps Plots_$tag/epslog
mkdir Plots_$tag/eps
mv $path/$tag/*.eps Plots_$tag/eps
mkdir Plots_$tag/pnglog
mv $path/$tag/*log.png Plots_$tag/pnglog
mkdir Plots_$tag/png
mv $path/$tag/*.png Plots_$tag/png
tar -cvf Plots_$tag.tar Plots_$tag


