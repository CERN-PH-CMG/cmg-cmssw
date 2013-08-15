DIR=/afs/cern.ch/user/q/querten/private/2012_HZZ2l2v_AN/cmsnotes/notes/AN-12-371/trunk/img/ 

for EXT in .pdf .C
do

cp cards_SB7TeV/0200/plot_Shape$EXT ${DIR}ShapeBased_200_7$EXT
cp cards_SB7TeV/0400/plot_Shape$EXT ${DIR}ShapeBased_400_7$EXT
cp cards_SB7TeV/0600/plot_Shape$EXT ${DIR}ShapeBased_600_7$EXT
cp cards_SB7TeV/0800/plot_Shape$EXT ${DIR}ShapeBased_800_7$EXT

cp cards_SB8TeV/0200/plot_Shape$EXT ${DIR}ShapeBased_200$EXT
cp cards_SB8TeV/0400/plot_Shape$EXT ${DIR}ShapeBased_400$EXT
cp cards_SB8TeV/0600/plot_Shape$EXT ${DIR}ShapeBased_600$EXT
cp cards_SB8TeV/0800/plot_Shape$EXT ${DIR}ShapeBased_800$EXT

cp cards_CC7TeV_GG/XSec_Limit$EXT ${DIR}GGxsec_Limit_7$EXT
cp cards_SB7TeV_GG/XSec_Limit$EXT ${DIR}GGxsec_Limit_shape_7$EXT
cp cards_CC7TeV_QQ/XSec_Limit$EXT ${DIR}VBFxsec_Limit_7$EXT
cp cards_SB7TeV_QQ/XSec_Limit$EXT ${DIR}VBFxsec_Limit_shape_7$EXT

cp cards_CC8TeV_GG/XSec_Limit$EXT ${DIR}GGxsec_Limit$EXT
cp cards_SB8TeV_GG/XSec_Limit$EXT ${DIR}GGxsec_Limit_shape$EXT
cp cards_CC8TeV_QQ/XSec_Limit$EXT ${DIR}VBFxsec_Limit$EXT
cp cards_SB8TeV_QQ/XSec_Limit$EXT ${DIR}VBFxsec_Limit_shape$EXT

cp cards_CC7TeV/Stength_Limit$EXT ${DIR}ExpectedLimits7TeV_log$EXT
cp cards_SB7TeV/Stength_Limit$EXT ${DIR}ExpectedLimits7TeV_log_shape$EXT

cp cards_CC8TeV/Stength_Limit$EXT ${DIR}ExpectedLimits8TeV_log$EXT
cp cards_SB8TeV/Stength_Limit$EXT ${DIR}ExpectedLimits8TeV_log_shape$EXT

cp cards_CCComb/Stength_Limit$EXT ${DIR}ExpectedLimitsCombined_log$EXT
cp cards_SBComb/Stength_Limit$EXT ${DIR}ExpectedLimitsCombined_log_shape$EXT

cp cards_CCComb/perCat_FinalPlot$EXT ${DIR}perCat_FinalPlotCombined_log$EXT
cp cards_SBComb/perCat_FinalPlot$EXT ${DIR}perCat_FinalPlotCombined_log_shape$EXT

cp cards_CC7TeV_GG/XSec_FinalPlot$EXT ${DIR}NR_GG_XSecLimit_7$EXT
cp cards_SB7TeV_GG/XSec_FinalPlot$EXT ${DIR}NR_GG_XSecLimit_shape_7$EXT
cp cards_CC7TeV_QQ/XSec_FinalPlot$EXT ${DIR}NR_QQ_XSecLimit_7$EXT
cp cards_SB7TeV_QQ/XSec_FinalPlot$EXT ${DIR}NR_QQ_XSecLimit_shape_7$EXT

cp cards_CC8TeV_GG/XSec_FinalPlot$EXT ${DIR}NR_GG_XSecLimit$EXT
cp cards_SB8TeV_GG/XSec_FinalPlot$EXT ${DIR}NR_GG_XSecLimit_shape$EXT
cp cards_CC8TeV_QQ/XSec_FinalPlot$EXT ${DIR}NR_QQ_XSecLimit$EXT
cp cards_SB8TeV_QQ/XSec_FinalPlot$EXT ${DIR}NR_QQ_XSecLimit_shape$EXT

cp cards_CC7TeV/Stength_FinalPlot$EXT ${DIR}NR_StengthLimit_7$EXT
cp cards_SB7TeV/Stength_FinalPlot$EXT ${DIR}NR_StengthLimit_shape_7$EXT

cp cards_CC8TeV/Stength_FinalPlot$EXT ${DIR}NR_StengthLimit$EXT
cp cards_SB8TeV/Stength_FinalPlot$EXT ${DIR}NR_StengthLimit_shape$EXT

cp cards_CCComb/Stength_FinalPlot$EXT ${DIR}NR_StengthLimit_Comb$EXT
cp cards_SBComb/Stength_FinalPlot$EXT ${DIR}NR_StengthLimit_shape_Comb$EXT

cp cards_SB7TeV/Stength_FinalPlot2D_0200$EXT  ${DIR}NR_StengthLimit2D_0200_shape_7$EXT
cp cards_SB7TeV/Stength_FinalPlot2D_0400$EXT  ${DIR}NR_StengthLimit2D_0400_shape_7$EXT
cp cards_SB7TeV/Stength_FinalPlot2D_0600$EXT  ${DIR}NR_StengthLimit2D_0600_shape_7$EXT
cp cards_SB7TeV/Stength_FinalPlot2D_0800$EXT  ${DIR}NR_StengthLimit2D_0800_shape_7$EXT

cp cards_SB8TeV/Stength_FinalPlot2D_0200$EXT  ${DIR}NR_StengthLimit2D_0200_shape$EXT
cp cards_SB8TeV/Stength_FinalPlot2D_0400$EXT  ${DIR}NR_StengthLimit2D_0400_shape$EXT
cp cards_SB8TeV/Stength_FinalPlot2D_0600$EXT  ${DIR}NR_StengthLimit2D_0600_shape$EXT
cp cards_SB8TeV/Stength_FinalPlot2D_0800$EXT  ${DIR}NR_StengthLimit2D_0800_shape$EXT

cp cards_SBComb/Stength_FinalPlot2D_0200$EXT  ${DIR}NR_StengthLimit2D_0200_shape_Comb$EXT
cp cards_SBComb/Stength_FinalPlot2D_0400$EXT  ${DIR}NR_StengthLimit2D_0400_shape_Comb$EXT
cp cards_SBComb/Stength_FinalPlot2D_0600$EXT  ${DIR}NR_StengthLimit2D_0600_shape_Comb$EXT
cp cards_SBComb/Stength_FinalPlot2D_0800$EXT  ${DIR}NR_StengthLimit2D_0800_shape_Comb$EXT


cp cards_SB7TeV/Stength_FinalPlot2D_0200_Width$EXT  ${DIR}NR_StengthLimit2D_0200_shape_7_Width$EXT
cp cards_SB7TeV/Stength_FinalPlot2D_0400_Width$EXT  ${DIR}NR_StengthLimit2D_0400_shape_7_Width$EXT
cp cards_SB7TeV/Stength_FinalPlot2D_0600_Width$EXT  ${DIR}NR_StengthLimit2D_0600_shape_7_Width$EXT
cp cards_SB7TeV/Stength_FinalPlot2D_0800_Width$EXT  ${DIR}NR_StengthLimit2D_0800_shape_7_Width$EXT

cp cards_SB8TeV/Stength_FinalPlot2D_0200_Width$EXT  ${DIR}NR_StengthLimit2D_0200_shape_Width$EXT
cp cards_SB8TeV/Stength_FinalPlot2D_0400_Width$EXT  ${DIR}NR_StengthLimit2D_0400_shape_Width$EXT
cp cards_SB8TeV/Stength_FinalPlot2D_0600_Width$EXT  ${DIR}NR_StengthLimit2D_0600_shape_Width$EXT
cp cards_SB8TeV/Stength_FinalPlot2D_0800_Width$EXT  ${DIR}NR_StengthLimit2D_0800_shape_Width$EXT

cp cards_SBComb/Stength_FinalPlot2D_0200_Width$EXT  ${DIR}NR_StengthLimit2D_0200_shape_Comb_Width$EXT
cp cards_SBComb/Stength_FinalPlot2D_0400_Width$EXT  ${DIR}NR_StengthLimit2D_0400_shape_Comb_Width$EXT
cp cards_SBComb/Stength_FinalPlot2D_0600_Width$EXT  ${DIR}NR_StengthLimit2D_0600_shape_Comb_Width$EXT
cp cards_SBComb/Stength_FinalPlot2D_0800_Width$EXT  ${DIR}NR_StengthLimit2D_0800_shape_Comb_Width$EXT


cp cards_CC7TeV_GG/XSec_FinalPlot_Obs$EXT ${DIR}NR_GG_XSecLimit_7_Obs$EXT
cp cards_SB7TeV_GG/XSec_FinalPlot_Obs$EXT ${DIR}NR_GG_XSecLimit_shape_7_Obs$EXT
cp cards_CC7TeV_QQ/XSec_FinalPlot_Obs$EXT ${DIR}NR_QQ_XSecLimit_7_Obs$EXT
cp cards_SB7TeV_QQ/XSec_FinalPlot_Obs$EXT ${DIR}NR_QQ_XSecLimit_shape_7_Obs$EXT

cp cards_CC8TeV_GG/XSec_FinalPlot_Obs$EXT ${DIR}NR_GG_XSecLimit_Obs$EXT
cp cards_SB8TeV_GG/XSec_FinalPlot_Obs$EXT ${DIR}NR_GG_XSecLimit_shape_Obs$EXT
cp cards_CC8TeV_QQ/XSec_FinalPlot_Obs$EXT ${DIR}NR_QQ_XSecLimit_Obs$EXT
cp cards_SB8TeV_QQ/XSec_FinalPlot_Obs$EXT ${DIR}NR_QQ_XSecLimit_shape_Obs$EXT

cp cards_CC7TeV/Stength_FinalPlot_Obs$EXT ${DIR}NR_StengthLimit_7_Obs$EXT
cp cards_SB7TeV/Stength_FinalPlot_Obs$EXT ${DIR}NR_StengthLimit_shape_7_Obs$EXT

cp cards_CC8TeV/Stength_FinalPlot_Obs$EXT ${DIR}NR_StengthLimit_Obs$EXT
cp cards_SB8TeV/Stength_FinalPlot_Obs$EXT ${DIR}NR_StengthLimit_shape_Obs$EXT

cp cards_CCComb/Stength_FinalPlot_Obs$EXT ${DIR}NR_StengthLimit_Comb_Obs$EXT
cp cards_SBComb/Stength_FinalPlot_Obs$EXT ${DIR}NR_StengthLimit_shape_Comb_Obs$EXT

cp cards_CCComb/Stength_FinalPlot_Cprime$EXT ${DIR}NR_StengthLimit_Comb_Cprime$EXT
cp cards_SBComb/Stength_FinalPlot_Cprime$EXT ${DIR}NR_StengthLimit_shape_Comb_Cprime$EXT

cp cards_SB7TeV/Stength_FinalPlot2D_0200_Obs$EXT  ${DIR}NR_StengthLimit2D_0200_shape_7_Obs$EXT
cp cards_SB7TeV/Stength_FinalPlot2D_0400_Obs$EXT  ${DIR}NR_StengthLimit2D_0400_shape_7_Obs$EXT
cp cards_SB7TeV/Stength_FinalPlot2D_0600_Obs$EXT  ${DIR}NR_StengthLimit2D_0600_shape_7_Obs$EXT
cp cards_SB7TeV/Stength_FinalPlot2D_0800_Obs$EXT  ${DIR}NR_StengthLimit2D_0800_shape_7_Obs$EXT

cp cards_SB8TeV/Stength_FinalPlot2D_0200_Obs$EXT  ${DIR}NR_StengthLimit2D_0200_shape_Obs$EXT
cp cards_SB8TeV/Stength_FinalPlot2D_0400_Obs$EXT  ${DIR}NR_StengthLimit2D_0400_shape_Obs$EXT
cp cards_SB8TeV/Stength_FinalPlot2D_0600_Obs$EXT  ${DIR}NR_StengthLimit2D_0600_shape_Obs$EXT
cp cards_SB8TeV/Stength_FinalPlot2D_0800_Obs$EXT  ${DIR}NR_StengthLimit2D_0800_shape_Obs$EXT

cp cards_SBComb/Stength_FinalPlot2D_0200_Obs$EXT  ${DIR}NR_StengthLimit2D_0200_shape_Comb_Obs$EXT
cp cards_SBComb/Stength_FinalPlot2D_0400_Obs$EXT  ${DIR}NR_StengthLimit2D_0400_shape_Comb_Obs$EXT
cp cards_SBComb/Stength_FinalPlot2D_0600_Obs$EXT  ${DIR}NR_StengthLimit2D_0600_shape_Comb_Obs$EXT
cp cards_SBComb/Stength_FinalPlot2D_0800_Obs$EXT  ${DIR}NR_StengthLimit2D_0800_shape_Comb_Obs$EXT


cp cards_SB7TeV/Stength_FinalPlot2D_0200_Width_Obs$EXT  ${DIR}NR_StengthLimit2D_0200_shape_7_Width_Obs$EXT
cp cards_SB7TeV/Stength_FinalPlot2D_0400_Width_Obs$EXT  ${DIR}NR_StengthLimit2D_0400_shape_7_Width_Obs$EXT
cp cards_SB7TeV/Stength_FinalPlot2D_0600_Width_Obs$EXT  ${DIR}NR_StengthLimit2D_0600_shape_7_Width_Obs$EXT
cp cards_SB7TeV/Stength_FinalPlot2D_0800_Width_Obs$EXT  ${DIR}NR_StengthLimit2D_0800_shape_7_Width_Obs$EXT

cp cards_SB8TeV/Stength_FinalPlot2D_0200_Width_Obs$EXT  ${DIR}NR_StengthLimit2D_0200_shape_Width_Obs$EXT
cp cards_SB8TeV/Stength_FinalPlot2D_0400_Width_Obs$EXT  ${DIR}NR_StengthLimit2D_0400_shape_Width_Obs$EXT
cp cards_SB8TeV/Stength_FinalPlot2D_0600_Width_Obs$EXT  ${DIR}NR_StengthLimit2D_0600_shape_Width_Obs$EXT
cp cards_SB8TeV/Stength_FinalPlot2D_0800_Width_Obs$EXT  ${DIR}NR_StengthLimit2D_0800_shape_Width_Obs$EXT

cp cards_SBComb/Stength_FinalPlot2D_0200_Width_Obs$EXT  ${DIR}NR_StengthLimit2D_0200_shape_Comb_Width_Obs$EXT
cp cards_SBComb/Stength_FinalPlot2D_0400_Width_Obs$EXT  ${DIR}NR_StengthLimit2D_0400_shape_Comb_Width_Obs$EXT
cp cards_SBComb/Stength_FinalPlot2D_0600_Width_Obs$EXT  ${DIR}NR_StengthLimit2D_0600_shape_Comb_Width_Obs$EXT
cp cards_SBComb/Stength_FinalPlot2D_0800_Width_Obs$EXT  ${DIR}NR_StengthLimit2D_0800_shape_Comb_Width_Obs$EXT



done
