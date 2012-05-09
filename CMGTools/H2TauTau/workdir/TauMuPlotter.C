//#include "CMGTools/H2TauTau/interface/TauMuPlotter.h"
#include "TauMuPlotter.h"
#include <THStack.h>
#include <TString.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TLegend.h>
#include <TLine.h>
#include <TVectorD.h>

TauMuPlotter::TauMuPlotter():
  TNamed("","")
{
}

TauMuPlotter::TauMuPlotter(const char * name):
  TNamed(name,name),
  smearHistoRes_(0.),
  Isocat_(-1),
  MTcat_(-1),
  SMcat_(-1),
  extrasel_("1"),
  plotvar_("ditaumass"),
  nbins_(100),
  xmin_(0.),
  xmax_(1000.),
  QCDOStoSSRatio_(1.11),
  QCDOStoSSRatioErr_(0.02),
  QCDMuIsoSideRatio_(0.),
  QCDOSTauIsoSideRatio_(0),
  QCDSSTauIsoSideRatio_(0),
  WJetsOSSideCorr_(1.),
  WJetsSSSideCorr_(1.),
  WJetsOSSideCorrErr_(0.),
  WJetsSSSideCorrErr_(0.)
{
 
  //qcdTauIsoRatio_="(0.190265-0.00241721*taupt)*(taupt<80)";
  //p0                        =     0.190265   +/-   0.0110291   
  //p1                        =  -0.00241721   +/-   0.000393394 

  //vs njet
  //qcdTauIsoRatio_="((njet==0)*0.142+(njet==1)*0.090+(njet==2)*0.075+(njet==3)*0.095)";
  //qcdTauIsoRatio_="((njet==0)*0.138+(njet==1)*0.086+(njet==2)*0.086+(njet==3)*0.046)";
  //Feb28
  //qcdTauIsoRatio_="((njet==0)*0.141179+(njet==1)*0.090182+(njet==2)*0.074195+(njet==3)*0.07)"; //last number is a guess
  //njet and pt dependent
  //n=0
  //qcdTauIsoRatio_="((njet==0)*(0.139577*(20<taupt&&taupt<=30) + 0.127993*(30<taupt&&taupt<=40) + 0.119574*(40<taupt&&taupt<=50) + 0.982777*(50<taupt&&taupt<=60) + 0.*(60<taupt)) + (njet==1)*(0.0988439*(20<taupt&&taupt<=30) + 0.0762868*(30<taupt&&taupt<=40) + 0.085495*(40<taupt&&taupt<=50) + 0.0186197*(50<taupt&&taupt<=60) + 0.*(60<taupt)) + (njet==2)*(0.0769998*(20<taupt&&taupt<=30) + 0.0700322*(30<taupt&&taupt<=40) + 0.0491831*(40<taupt&&taupt<=50) + 0.0*(50<taupt&&taupt<=60) + 0.*(60<taupt)) + (njet>2)*0.0)";


//   ///tau jet pt dependent inclusive ratio
//  qcdTauIsoRatio_="( 1.56883*(20<taujetpt&&taujetpt<=30) + 0.281955*(30<taujetpt&&taujetpt<=40) + 0.0985824*(40<taujetpt&&taujetpt<=50) + 0.0462377*(50<taujetpt&&taujetpt<=60) + 0.0288263*(60<taujetpt&&taujetpt<=70) + 0.0208332*(70<taujetpt&&taujetpt<=80) + 0.0145108*(80<taujetpt&&taujetpt<=90) + 0.0209432*(90<taujetpt&&taujetpt<=100) + 0.0*(100<taujetpt) )";

//   ///fit to the njet=0 sample [0]*exp([1]*x+[2]*x*x)
//   NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
//    1  p0           3.42541e+02   1.04396e+02   8.21319e-03  -5.28112e-07
//    2  p1          -2.67974e-01   1.43404e-02  -1.18736e-06  -7.39962e-03
//    3  p2           1.87351e-03   1.63759e-04   1.46712e-08  -3.64506e-01
//  qcdTauIsoRatio_="(20<taujetpt&&taujetpt<80)*(3.42541e+02*exp(-2.67974e-01*taujetpt+1.87351e-03*taujetpt*taujetpt))";
//--> problem with first bin  

  //fine binning for inclusive
  //qcdTauIsoRatio_="((20<taujetpt&&taujetpt<=25)*10.14+(25<taujetpt&&taujetpt<=30)*1.37238+(30<taujetpt&&taujetpt<=35)*0.425582+(35<taujetpt&&taujetpt<=40)*0.201691+(40<taujetpt&&taujetpt<=45)*0.117622+(45<taujetpt&&taujetpt<=50)*0.0729338+(50<taujetpt&&taujetpt<=55)*0.0500108+(55<taujetpt&&taujetpt<=60)*0.0330328+(60<taujetpt&&taujetpt<=65)*0.0300232+(65<taujetpt&&taujetpt<=70)*0.0170739+(70<taujetpt&&taujetpt<=75)*0.020635+(75<taujetpt&&taujetpt<=80)*0.0148927+(80<taujetpt&&taujetpt<=85)*0.0127072+(85<taujetpt&&taujetpt<=90)*0.0139045+(90<taujetpt&&taujetpt<=95)*0.0185733+(95<taujetpt&&taujetpt<=100)*0.0157922)";

  //fine ratio njet=0
  //qcdTauIsoRatio_="((njet==0)*1.0+(njet==1||njet==2)*1.067)*((20<taujetpt&&taujetpt<=25)*11.5247+(25<taujetpt&&taujetpt<=30)*1.37424+(30<taujetpt&&taujetpt<=35)*0.41256+(35<taujetpt&&taujetpt<=40)*0.19751+(40<taujetpt&&taujetpt<=45)*0.115416+(45<taujetpt&&taujetpt<=50)*0.0759201+(50<taujetpt&&taujetpt<=55)*0.0424669+(55<taujetpt&&taujetpt<=60)*0.0328491+(60<taujetpt&&taujetpt<=65)*0.0328291+(65<taujetpt&&taujetpt<=70)*0.0152393+(70<taujetpt&&taujetpt<=75)*0.0153301+(75<taujetpt&&taujetpt<=80)*0.0128618+(80<taujetpt&&taujetpt<=85)*0+(85<taujetpt&&taujetpt<=90)*0+(90<taujetpt&&taujetpt<=95)*0+(95<taujetpt&&taujetpt<=100)*0)";

  //fine ratio njet=1
  //qcdTauIsoRatio_="(njet==1||njet==2)*((20<taujetpt&&taujetpt<=25)*6.06837+(25<taujetpt&&taujetpt<=30)*1.21503+(30<taujetpt&&taujetpt<=35)*0.510201+(35<taujetpt&&taujetpt<=40)*0.223787+(40<taujetpt&&taujetpt<=45)*0.122981+(45<taujetpt&&taujetpt<=50)*0.0639785+(50<taujetpt&&taujetpt<=55)*0.0677511+(55<taujetpt&&taujetpt<=60)*0.038791+(60<taujetpt&&taujetpt<=65)*0.0251048+(65<taujetpt&&taujetpt<=70)*0.0181727+(70<taujetpt&&taujetpt<=75)*0.0206081+(75<taujetpt&&taujetpt<=80)*0.02035+(80<taujetpt&&taujetpt<=85)*0.0385446+(85<taujetpt&&taujetpt<=90)*0.0129727+(90<taujetpt&&taujetpt<=95)*0.0204044+(95<taujetpt&&taujetpt<=100)*0.0438523)";
 
  /////transverse mass dependence 
  //original 0.9765+0.0128*
  //from isolated muons njet=1 : 0.9647+0.005948*

  //ratio from NonIso Muons inclusive //problem with transverse mass shape correction
  //qcdTauIsoRatio_="((njet==0)*1.0+(njet==1||njet==2)*1.067)*(transversemass<60.)*(0.97+0.0128*transversemass)*((20<taujetpt&&taujetpt<=25)*10.5+(25<taujetpt&&taujetpt<=30)*1.31+(30<taujetpt&&taujetpt<=35)*0.405+(35<taujetpt&&taujetpt<=40)*0.184+(40<taujetpt&&taujetpt<=45)*0.103+(45<taujetpt&&taujetpt<=50)*0.0624+(50<taujetpt&&taujetpt<=55)*0.0418+(55<taujetpt&&taujetpt<=60)*0.0308+(60<taujetpt&&taujetpt<=65)*0.0265+(65<taujetpt&&taujetpt<=70)*0.0206+(70<taujetpt&&taujetpt<=75)*0.0157+(75<taujetpt&&taujetpt<=80)*0.0143+(80<taujetpt&&taujetpt<=85)*0.0125+(85<taujetpt&&taujetpt<=90)*0.0123+(90<taujetpt&&taujetpt<=95)*0.0108+(95<taujetpt&&taujetpt<=100)*0.0103)";
  
  //ratio from Anti-Iso Muons njet=0
  //qcdTauIsoRatio_="((20<taujetpt&&taujetpt<=25)*10.1928+(25<taujetpt&&taujetpt<=30)*1.29939+(30<taujetpt&&taujetpt<=35)*0.396667+(35<taujetpt&&taujetpt<=40)*0.180465+(40<taujetpt&&taujetpt<=45)*0.101745+(45<taujetpt&&taujetpt<=50)*0.0627062+(50<taujetpt&&taujetpt<=55)*0.0396773+(55<taujetpt&&taujetpt<=60)*0.0304729+(60<taujetpt&&taujetpt<=65)*0.0272851+(65<taujetpt&&taujetpt<=70)*0.0202714+(70<taujetpt&&taujetpt<=75)*0.0154489+(75<taujetpt&&taujetpt<=80)*0.0138551+(80<taujetpt&&taujetpt<=85)*0.00838189+(85<taujetpt&&taujetpt<=90)*0.00806126+(90<taujetpt&&taujetpt<=95)*0.00781547+(95<taujetpt&&taujetpt<=100)*0.0093825)";
  
  //ratio from Anti-Iso Muons njet=1
  //qcdTauIsoRatio_="((20<taujetpt&&taujetpt<=25)*11.3082+(25<taujetpt&&taujetpt<=30)*1.28471+(30<taujetpt&&taujetpt<=35)*0.45327+(35<taujetpt&&taujetpt<=40)*0.197425+(40<taujetpt&&taujetpt<=45)*0.108273+(45<taujetpt&&taujetpt<=50)*0.0608861+(50<taujetpt&&taujetpt<=55)*0.0477466+(55<taujetpt&&taujetpt<=60)*0.0329757+(60<taujetpt&&taujetpt<=65)*0.0246043+(65<taujetpt&&taujetpt<=70)*0.0203719+(70<taujetpt&&taujetpt<=75)*0.0150402+(75<taujetpt&&taujetpt<=80)*0.0167879+(80<taujetpt&&taujetpt<=85)*0.0203463+(85<taujetpt&&taujetpt<=90)*0.0151795+(90<taujetpt&&taujetpt<=95)*0.00986912+(95<taujetpt&&taujetpt<=100)*0.0103063)";
  
  ////////////////////////////////////////////////////
  ///Ratio from QCD+WJets inclusive (isolated muons) ---> doesnt work W+jets ratio cannot be obtained from SS samples
  //qcdTauIsoRatio_="((20<taujetpt&&taujetpt<=25)*9.15984+(25<taujetpt&&taujetpt<=30)*1.06513+(30<taujetpt&&taujetpt<=35)*0.310882+(35<taujetpt&&taujetpt<=40)*0.146297+(40<taujetpt&&taujetpt<=45)*0.0791099+(45<taujetpt&&taujetpt<=50)*0.0466985+(50<taujetpt&&taujetpt<=55)*0.0284409+(55<taujetpt&&taujetpt<=60)*0.0182324+(60<taujetpt&&taujetpt<=65)*0.0144373+(65<taujetpt&&taujetpt<=70)*0.00725716+(70<taujetpt&&taujetpt<=75)*0.0079622+(75<taujetpt&&taujetpt<=80)*0.00519162+(80<taujetpt&&taujetpt<=85)*0.00285895+(85<taujetpt&&taujetpt<=90)*0.00391671+(90<taujetpt&&taujetpt<=95)*0.00312257+(95<taujetpt&&taujetpt<=100)*0.00350573)";

  //njet=0 
  //qcdTauIsoRatio_="((transversemass<40.)*((20<taujetpt&&taujetpt<=25)*14.3472+(25<taujetpt&&taujetpt<=30)*1.27985+(30<taujetpt&&taujetpt<=35)*0.393001+(35<taujetpt&&taujetpt<=40)*0.185462+(40<taujetpt&&taujetpt<=45)*0.107212+(45<taujetpt&&taujetpt<=50)*0.0655227+(50<taujetpt&&taujetpt<=55)*0.0418201+(55<taujetpt&&taujetpt<=60)*0.0317598+(60<taujetpt&&taujetpt<=65)*0.027422+(65<taujetpt&&taujetpt<=70)*0.0199582+(70<taujetpt&&taujetpt<=75)*0.0133117+(75<taujetpt&&taujetpt<=80)*0.0167705+(80<taujetpt&&taujetpt<=85)*0.0120297+(85<taujetpt&&taujetpt<=90)*0.0088611+(90<taujetpt&&taujetpt<=95)*0.00835595+(95<taujetpt&&taujetpt<=100)*0.00683616)+(transversemass>=40.0)*((20<taujetpt&&taujetpt<=25)*8.87949+(25<taujetpt&&taujetpt<=30)*1.07561+(30<taujetpt&&taujetpt<=35)*0.334037+(35<taujetpt&&taujetpt<=40)*0.14342+(40<taujetpt&&taujetpt<=45)*0.0787955+(45<taujetpt&&taujetpt<=50)*0.0492029+(50<taujetpt&&taujetpt<=55)*0.0326973+(55<taujetpt&&taujetpt<=60)*0.0224543+(60<taujetpt&&taujetpt<=65)*0.0178901+(65<taujetpt&&taujetpt<=70)*0.0131373+(70<taujetpt&&taujetpt<=75)*0.0108719+(75<taujetpt&&taujetpt<=80)*0.0109507+(80<taujetpt&&taujetpt<=85)*0.00757953+(85<taujetpt&&taujetpt<=90)*0.00874692+(90<taujetpt&&taujetpt<=95)*0.00679206+(95<taujetpt&&taujetpt<=100)*0.0064818))";

  /////////////////////////////////////////////////////
  //QCD only jetpt vs transversemass
  //from njet=0 
  //qcdTauIsoRatio_="((transversemass<15)*((20<taujetpt&&taujetpt<=25)*34.1173+(25<taujetpt&&taujetpt<=30)*1.40915+(30<taujetpt&&taujetpt<=35)*0.382103+(35<taujetpt&&taujetpt<=40)*0.194543+(40<taujetpt&&taujetpt<=45)*0.108395+(45<taujetpt&&taujetpt<=50)*0.072867+(50<taujetpt&&taujetpt<=55)*0.0428369+(55<taujetpt&&taujetpt<=60)*0.0395885+(60<taujetpt&&taujetpt<=65)*0.0290214+(65<taujetpt&&taujetpt<=70)*0.012012)+(15<=transversemass&&transversemass<30)*((20<taujetpt&&taujetpt<=25)*8.8432+(25<taujetpt&&taujetpt<=30)*1.30919+(30<taujetpt&&taujetpt<=35)*0.429458+(35<taujetpt&&taujetpt<=40)*0.198089+(40<taujetpt&&taujetpt<=45)*0.121607+(45<taujetpt&&taujetpt<=50)*0.0800996+(50<taujetpt&&taujetpt<=55)*0.0500691+(55<taujetpt&&taujetpt<=60)*0.0293687+(60<taujetpt&&taujetpt<=65)*0.0457763+(65<taujetpt&&taujetpt<=70)*0.0186038)+(30<=transversemass)*((20<taujetpt&&taujetpt<=25)*-9.8643+(25<taujetpt&&taujetpt<=30)*7.6307+(30<taujetpt&&taujetpt<=35)*0.918767+(35<taujetpt&&taujetpt<=40)*0.290637+(40<taujetpt&&taujetpt<=45)*0.179094+(45<taujetpt&&taujetpt<=50)*0.110233))";
  
  ///////////////////////////////////////////
  ///QCD only with non-iso muons njet=0
  //qcdTauIsoRatio_="((njet==0)*1.0+(njet==1||njet==2)*1.07)*(1.08*(transversemass<15)*((20<taujetpt&&taujetpt<=25)*11.5876+(25<taujetpt&&taujetpt<=30)*1.31259+(30<taujetpt&&taujetpt<=35)*0.387394+(35<taujetpt&&taujetpt<=40)*0.174441+(40<taujetpt&&taujetpt<=45)*0.100234+(45<taujetpt&&taujetpt<=50)*0.0597124+(50<taujetpt&&taujetpt<=55)*0.0390836+(55<taujetpt&&taujetpt<=60)*0.0314727+(60<taujetpt&&taujetpt<=65)*0.0266647+(65<taujetpt&&taujetpt<=70)*0.0196102+(70<taujetpt&&taujetpt<=75)*0.0141931+(75<taujetpt&&taujetpt<=80)*0.0146757+(80<taujetpt&&taujetpt<=85)*0.0135062+(85<taujetpt&&taujetpt<=90)*0.00982585+(90<taujetpt&&taujetpt<=95)*0.00601405+(95<taujetpt&&taujetpt<=100)*0.00500968)+1.104*(15<=transversemass&&transversemass<30)*((20<taujetpt&&taujetpt<=25)*9.13214+(25<taujetpt&&taujetpt<=30)*1.29603+(30<taujetpt&&taujetpt<=35)*0.393275+(35<taujetpt&&taujetpt<=40)*0.184762+(40<taujetpt&&taujetpt<=45)*0.100406+(45<taujetpt&&taujetpt<=50)*0.0654513+(50<taujetpt&&taujetpt<=55)*0.0426565+(55<taujetpt&&taujetpt<=60)*0.0285307+(60<taujetpt&&taujetpt<=65)*0.0277684+(65<taujetpt&&taujetpt<=70)*0.0201812+(70<taujetpt&&taujetpt<=75)*0.0195524+(75<taujetpt&&taujetpt<=80)*0.0141385)+1.38*(30<=transversemass&&transversemass<60)*((20<taujetpt&&taujetpt<=25)*7.72557+(25<taujetpt&&taujetpt<=30)*1.3997+(30<taujetpt&&taujetpt<=35)*0.509583+(35<taujetpt&&taujetpt<=40)*0.213929+(40<taujetpt&&taujetpt<=45)*0.121862+(45<taujetpt&&taujetpt<=50)*0.0723259+(50<taujetpt&&taujetpt<=55)*0.0322856+(55<taujetpt&&taujetpt<=60)*0.0344744+(60<taujetpt&&taujetpt<=65)*0.0219804+(65<taujetpt&&taujetpt<=70)*0.000350094))";
  //do not use correction factors as function of mT (not well determined) ---> Last Good ratio
  //qcdTauIsoRatio_="1.09*((njet==0)*1.0+(njet==1)*1.07+(njet==2)*1.09)*((transversemass<15)*((20<taujetpt&&taujetpt<=25)*11.5876+(25<taujetpt&&taujetpt<=30)*1.31259+(30<taujetpt&&taujetpt<=35)*0.387394+(35<taujetpt&&taujetpt<=40)*0.174441+(40<taujetpt&&taujetpt<=45)*0.100234+(45<taujetpt&&taujetpt<=50)*0.0597124+(50<taujetpt&&taujetpt<=55)*0.0390836+(55<taujetpt&&taujetpt<=60)*0.0314727+(60<taujetpt&&taujetpt<=65)*0.0266647+(65<taujetpt&&taujetpt<=70)*0.0196102+(70<taujetpt&&taujetpt<=75)*0.0141931+(75<taujetpt&&taujetpt<=80)*0.0146757+(80<taujetpt&&taujetpt<=85)*0.0135062+(85<taujetpt&&taujetpt<=90)*0.00982585+(90<taujetpt&&taujetpt<=95)*0.00601405+(95<taujetpt&&taujetpt<=100)*0.00500968)+(15<=transversemass&&transversemass<30)*((20<taujetpt&&taujetpt<=25)*9.13214+(25<taujetpt&&taujetpt<=30)*1.29603+(30<taujetpt&&taujetpt<=35)*0.393275+(35<taujetpt&&taujetpt<=40)*0.184762+(40<taujetpt&&taujetpt<=45)*0.100406+(45<taujetpt&&taujetpt<=50)*0.0654513+(50<taujetpt&&taujetpt<=55)*0.0426565+(55<taujetpt&&taujetpt<=60)*0.0285307+(60<taujetpt&&taujetpt<=65)*0.0277684+(65<taujetpt&&taujetpt<=70)*0.0201812+(70<taujetpt&&taujetpt<=75)*0.0195524+(75<taujetpt&&taujetpt<=80)*0.0141385)+(30<=transversemass&&transversemass<60)*((20<taujetpt&&taujetpt<=25)*7.72557+(25<taujetpt&&taujetpt<=30)*1.3997+(30<taujetpt&&taujetpt<=35)*0.509583+(35<taujetpt&&taujetpt<=40)*0.213929+(40<taujetpt&&taujetpt<=45)*0.121862+(45<taujetpt&&taujetpt<=50)*0.0723259+(50<taujetpt&&taujetpt<=55)*0.0322856+(55<taujetpt&&taujetpt<=60)*0.0344744+(60<taujetpt&&taujetpt<=65)*0.0219804+(65<taujetpt&&taujetpt<=70)*0.000350094))";

  
  ///ratio between Tau and Mu anti-Iso and fully iso 
  //qcdTauIsoRatio_="((20<taujetpt&&taujetpt<=25)*0.454+(25<taujetpt&&taujetpt<=30)*0.217+(30<taujetpt&&taujetpt<=35)*0.0827+(35<taujetpt&&taujetpt<=40)*0.0415+(40<taujetpt&&taujetpt<=45)*0.022+(45<taujetpt&&taujetpt<=50)*0.0142+(50<taujetpt&&taujetpt<=55)*0.00735+(55<taujetpt&&taujetpt<=60)*0.00548+(60<taujetpt&&taujetpt<=65)*0.00502+(65<taujetpt&&taujetpt<=70)*0.00221+(70<taujetpt&&taujetpt<=75)*0.00214+(75<taujetpt&&taujetpt<=80)*0.00167)";
    
  //ratio for the different regions
  //Region 2 : anti-Iso tau
  //qcdTauIsoRatio_ = "((tauisodisc>=2)*((17<mujetpt&&mujetpt<=22)*6.07+(22<mujetpt&&mujetpt<=27)*0.81+(27<mujetpt&&mujetpt<=32)*0.298+(32<mujetpt&&mujetpt<=37)*0.21+(37<mujetpt&&mujetpt<=42)*0.203+(42<mujetpt&&mujetpt<=47)*0.181+(47<mujetpt&&mujetpt<=52)*0.315+(52<mujetpt&&mujetpt<=57)*0.221))";
  //Region 3 : anti-Iso mu
  //qcdTauIsoRatio_ = "((muiso<0.1)*((20<taujetpt&&taujetpt<=25)*11.5+(25<taujetpt&&taujetpt<=30)*1.37+(30<taujetpt&&taujetpt<=35)*0.412+(35<taujetpt&&taujetpt<=40)*0.198+(40<taujetpt&&taujetpt<=45)*0.115+(45<taujetpt&&taujetpt<=50)*0.0758+(50<taujetpt&&taujetpt<=55)*0.0424+(55<taujetpt&&taujetpt<=60)*0.0329+(60<taujetpt&&taujetpt<=65)*0.0329))";
  //Region 4 : anti-Iso tau and mu
  //qcdTauIsoRatio_ = "((muiso>0.1&&tauisodisc<2)*((20<taujetpt&&taujetpt<=25)*11.5+(25<taujetpt&&taujetpt<=30)*1.37+(30<taujetpt&&taujetpt<=35)*0.412+(35<taujetpt&&taujetpt<=40)*0.198+(40<taujetpt&&taujetpt<=45)*0.115+(45<taujetpt&&taujetpt<=50)*0.0758+(50<taujetpt&&taujetpt<=55)*0.0424+(55<taujetpt&&taujetpt<=60)*0.0329+(60<taujetpt&&taujetpt<=65)*0.0329)*((17<mujetpt&&mujetpt<=22)*6.07+(22<mujetpt&&mujetpt<=27)*0.81+(27<mujetpt&&mujetpt<=32)*0.298+(32<mujetpt&&mujetpt<=37)*0.21+(37<mujetpt&&mujetpt<=42)*0.203+(42<mujetpt&&mujetpt<=47)*0.181+(47<mujetpt&&mujetpt<=52)*0.315+(52<mujetpt&&mujetpt<=57)*0.221))";
  //qcdTauIsoRatio_ = "0.3333*((muiso<0.1)*((20<taujetpt&&taujetpt<=25)*11.5+(25<taujetpt&&taujetpt<=30)*1.37+(30<taujetpt&&taujetpt<=35)*0.412+(35<taujetpt&&taujetpt<=40)*0.198+(40<taujetpt&&taujetpt<=45)*0.115+(45<taujetpt&&taujetpt<=50)*0.0758+(50<taujetpt&&taujetpt<=55)*0.0424+(55<taujetpt&&taujetpt<=60)*0.0329+(60<taujetpt&&taujetpt<=65)*0.0329)+(tauisodisc>=2)*((17<mujetpt&&mujetpt<=22)*6.07+(22<mujetpt&&mujetpt<=27)*0.81+(27<mujetpt&&mujetpt<=32)*0.298+(32<mujetpt&&mujetpt<=37)*0.21+(37<mujetpt&&mujetpt<=42)*0.203+(42<mujetpt&&mujetpt<=47)*0.181+(47<mujetpt&&mujetpt<=52)*0.315+(52<mujetpt&&mujetpt<=57)*0.221)+((muiso>0.1&&tauisodisc<2)*((20<taujetpt&&taujetpt<=25)*11.5+(25<taujetpt&&taujetpt<=30)*1.37+(30<taujetpt&&taujetpt<=35)*0.412+(35<taujetpt&&taujetpt<=40)*0.198+(40<taujetpt&&taujetpt<=45)*0.115+(45<taujetpt&&taujetpt<=50)*0.0758+(50<taujetpt&&taujetpt<=55)*0.0424+(55<taujetpt&&taujetpt<=60)*0.0329+(60<taujetpt&&taujetpt<=65)*0.0329)*((17<mujetpt&&mujetpt<=22)*6.07+(22<mujetpt&&mujetpt<=27)*0.81+(27<mujetpt&&mujetpt<=32)*0.298+(32<mujetpt&&mujetpt<=37)*0.21+(37<mujetpt&&mujetpt<=42)*0.203+(42<mujetpt&&mujetpt<=47)*0.181+(47<mujetpt&&mujetpt<=52)*0.315+(52<mujetpt&&mujetpt<=57)*0.221)))";

  //
  //qcdTauIsoRatio_ = "(muiso<0.1)*((20<taujetpt&&taujetpt<=25)*11.5+(25<taujetpt&&taujetpt<=30)*1.37+(30<taujetpt&&taujetpt<=35)*0.412+(35<taujetpt&&taujetpt<=40)*0.198+(40<taujetpt&&taujetpt<=45)*0.115+(45<taujetpt&&taujetpt<=50)*0.0758+(50<taujetpt&&taujetpt<=55)*0.0424+(55<taujetpt&&taujetpt<=60)*0.0329+(60<taujetpt&&taujetpt<=65)*0.0329)";
  

  qcdTauIsoRatio_ = "((20<taujetpt&&taujetpt<=25)*9.89+(25<taujetpt&&taujetpt<=30)*1.38+(30<taujetpt&&taujetpt<=35)*0.423+(35<taujetpt&&taujetpt<=40)*0.2+(40<taujetpt&&taujetpt<=45)*0.118+(45<taujetpt&&taujetpt<=50)*0.0725+(50<taujetpt&&taujetpt<=55)*0.0485+(55<taujetpt&&taujetpt<=60)*0.0325+(60<taujetpt&&taujetpt<=65)*0.0299+(65<taujetpt&&taujetpt<=70)*0.0156+(70<taujetpt&&taujetpt<=75)*0.0191+(75<taujetpt&&taujetpt<=80)*0.011+(80<taujetpt&&taujetpt<=85)*0.00971+(85<taujetpt&&taujetpt<=90)*0.0121+(90<taujetpt&&taujetpt<=95)*0.0108+(95<taujetpt&&taujetpt<=100)*0.0128)";
  qcdMuIsoRatio_ = "((17<mujetpt&&mujetpt<=22)*5.61+(22<mujetpt&&mujetpt<=27)*0.81+(27<mujetpt&&mujetpt<=32)*0.307+(32<mujetpt&&mujetpt<=37)*0.208+(37<mujetpt&&mujetpt<=42)*0.202+(42<mujetpt&&mujetpt<=47)*0.173+(47<mujetpt&&mujetpt<=52)*0.247+(52<mujetpt&&mujetpt<=57)*0.235+(57<mujetpt&&mujetpt<=62)*0.217+(62<mujetpt&&mujetpt<=67)*0.289+(67<mujetpt&&mujetpt<=72)*0.0971+(72<mujetpt&&mujetpt<=77)*0.742+(77<mujetpt&&mujetpt<=82)*1.3+(82<mujetpt&&mujetpt<=87)*0.821+(87<mujetpt&&mujetpt<=92)*3.02+(92<mujetpt&&mujetpt<=97)*3.88)";
  qcdTauIsoRatioMuNonIso_ = "((20<taujetpt&&taujetpt<=25)*10.1+(25<taujetpt&&taujetpt<=30)*1.29+(30<taujetpt&&taujetpt<=35)*0.399+(35<taujetpt&&taujetpt<=40)*0.182+(40<taujetpt&&taujetpt<=45)*0.101+(45<taujetpt&&taujetpt<=50)*0.0612+(50<taujetpt&&taujetpt<=55)*0.0409+(55<taujetpt&&taujetpt<=60)*0.0299+(60<taujetpt&&taujetpt<=65)*0.0259+(65<taujetpt&&taujetpt<=70)*0.0197+(70<taujetpt&&taujetpt<=75)*0.0151+(75<taujetpt&&taujetpt<=80)*0.0132+(80<taujetpt&&taujetpt<=85)*0.011+(85<taujetpt&&taujetpt<=90)*0.0115+(90<taujetpt&&taujetpt<=95)*0.00885+(95<taujetpt&&taujetpt<=100)*0.00922)";
  qcdMuIsoRatioTauNonIso_ = "((17<mujetpt&&mujetpt<=22)*4.11+(22<mujetpt&&mujetpt<=27)*0.664+(27<mujetpt&&mujetpt<=32)*0.253+(32<mujetpt&&mujetpt<=37)*0.168+(37<mujetpt&&mujetpt<=42)*0.142+(42<mujetpt&&mujetpt<=47)*0.137+(47<mujetpt&&mujetpt<=52)*0.136+(52<mujetpt&&mujetpt<=57)*0.166+(57<mujetpt&&mujetpt<=62)*0.153+(62<mujetpt&&mujetpt<=67)*0.159+(67<mujetpt&&mujetpt<=72)*0.205+(72<mujetpt&&mujetpt<=77)*0.0954+(77<mujetpt&&mujetpt<=82)*0.285+(82<mujetpt&&mujetpt<=87)*0.328+(87<mujetpt&&mujetpt<=92)*0.252+(92<mujetpt&&mujetpt<=97)*0.224)";
  
  //Ratio for WJets 
  //inclusive  
  //wjetsTauIsoRatio_ = "(0.103)"; //avg
  wjetsTauIsoRatio_ = "((20<taujetpt&&taujetpt<=25)*15.2+(25<taujetpt&&taujetpt<=30)*1.53+(30<taujetpt&&taujetpt<=35)*0.492+(35<taujetpt&&taujetpt<=40)*0.237+(40<taujetpt&&taujetpt<=45)*0.124+(45<taujetpt&&taujetpt<=50)*0.0864+(50<taujetpt&&taujetpt<=55)*0.0613+(55<taujetpt&&taujetpt<=60)*0.0465+(60<taujetpt&&taujetpt<=65)*0.0405+(65<taujetpt&&taujetpt<=70)*0.0342+(70<taujetpt&&taujetpt<=75)*0.0302+(75<taujetpt&&taujetpt<=80)*0.0253+(80<taujetpt&&taujetpt<=85)*0.0208+(85<taujetpt&&taujetpt<=90)*0.0228+(90<taujetpt&&taujetpt<=95)*0.0157+(95<taujetpt&&taujetpt<=100)*0.0174)";
  //inclusive SS  
  //wjetsTauIsoRatioSS_ = "(0.061)";//avg
  wjetsTauIsoRatioSS_ = "((20<taujetpt&&taujetpt<=25)*7.63+(25<taujetpt&&taujetpt<=30)*1.14+(30<taujetpt&&taujetpt<=35)*0.34+(35<taujetpt&&taujetpt<=40)*0.156+(40<taujetpt&&taujetpt<=45)*0.0855+(45<taujetpt&&taujetpt<=50)*0.0517+(50<taujetpt&&taujetpt<=55)*0.0349+(55<taujetpt&&taujetpt<=60)*0.0239+(60<taujetpt&&taujetpt<=65)*0.0207+(65<taujetpt&&taujetpt<=70)*0.0166+(70<taujetpt&&taujetpt<=75)*0.0143+(75<taujetpt&&taujetpt<=80)*0.0109+(80<taujetpt&&taujetpt<=85)*0.0113+(85<taujetpt&&taujetpt<=90)*0.00844+(90<taujetpt&&taujetpt<=95)*0.00702+(95<taujetpt&&taujetpt<=100)*0.00695)";


}

TauMuPlotter::~TauMuPlotter(){
}

/////////////General methods start here

bool TauMuPlotter::scaleSamplesLumi(){

  /////////determine total lumi and scale MCs
  float totalDataLumi=0.;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data")
      totalDataLumi+=(*s)->getLumi();
  cout<<"Total Data lumi = "<<totalDataLumi<<endl;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC" 
       || (*s)->getDataType()=="MCCat" 
       || (*s)->getDataType()=="Signal" ){
      //cout<<(*s)->GetName()<<" lumi scale "<<totalDataLumi/(*s)->getLumi()<<endl;
      (*s)->resetScale();
      (*s)->scale(totalDataLumi/(*s)->getLumi());
    }



  ///////////////////////////////////
  ////Scale the embedded samples at inclusive level:  Should there be separate scale factor for OS and SS ? --> should be Ok. ZTauTau is small in SS
  /////////////////////////////////
  Float_t ZToTauTauMC=0.;
  Float_t EmbeddedOS=0.;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Embedded"){
      TH1F*h=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==1&&categoryIso==1)"));
      EmbeddedOS+=h->Integral();
      delete h;
    }
    if(TString((*s)->GetName())=="ZToTauTau"){
      TH1F*h=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==1&&categoryIso==1)"));
      ZToTauTauMC=h->Integral()*(*s)->getNorm();
      delete h;
    }
  }
  if(ZToTauTauMC==0. || EmbeddedOS==0.){
    cout<<"Warning!!!: bad ZToTauTauMC or EmbeddedOS yields: ZToTauTauMC="<<ZToTauTauMC<<" EmbeddedOS="<<EmbeddedOS<<endl;
  } else {
    cout<<"Rescaling Embedded samples by factor : "<<ZToTauTauMC/EmbeddedOS<<endl;
    for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
      if((*s)->getDataType()=="Embedded"){
	(*s)->resetScale();
	(*s)->scale(ZToTauTauMC/EmbeddedOS);
      }
  }



  return 1;
}




TH1F* TauMuPlotter::getTotalData(){
  TString sel="eventweight*(categoryCh==1)";
  if(Isocat_>0)    sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0)     sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)    sel += TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F("hData","Data",nbins_,xmin_,xmax_);
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd);
      delete hd;
    }
 
  cout<<"TotalData : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}


TH1F* TauMuPlotter::getTotalDataSS(){
  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel += TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F("hDataSS","Data SS",nbins_,xmin_,xmax_);
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd);
      delete hd;
    }
  cout<<"TotalDataSS : "<<h->Integral()<<endl;

  return h;
}


TH2F* TauMuPlotter::getTotalDataSS2D(){
  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel += TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH2F* h=new TH2F("hDataSS","Data SS",nbins_,xmin_,xmax_,nbins2_,xmin2_,xmax2_);
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data"){
      TH2F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,plotvar2_,nbins2_,xmin2_,xmax2_,sel);
      h->Add(hd);
      delete hd;
    }
  cout<<"TotalDataSS : "<<h->Integral()<<endl;

  return h;
}

TH1F* TauMuPlotter::getTotalMC(){
  TString sel="eventweight*(categoryCh==1)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0) sel+=TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F("hTotalMC","TotalMC",nbins_,xmin_,xmax_);
  h->Sumw2();

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC"){// && TString((*s)->GetName()) != "ZToTauTau" && TString((*s)->GetName()) != "WJetsToLNu"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());  
      delete hd;
    }
 
  cout<<"Total MC : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}

TH1F* TauMuPlotter::getTotalMCSS(){
  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F("hMC_SS","",nbins_,xmin_,xmax_);
  h->Sumw2();

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC"){// && TString((*s)->GetName()) != "ZToTauTau" && TString((*s)->GetName()) != "WJetsToLNu" ){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());  
      delete hd;
    }
 
  cout<<"MCSS : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}


TH1F* TauMuPlotter::getTotalEmbedded(){
  TString sel="eventweight*(categoryCh==1)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F("hEmbedded","",nbins_,xmin_,xmax_);
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Embedded"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());  
      delete hd;
    }
 
  cout<<"TotalEmbedded : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}


TH1F* TauMuPlotter::getTotalEmbeddedSS(){
  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F("hEmbeddedSS","",nbins_,xmin_,xmax_);
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Embedded"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());  
      delete hd;
    }
 
  cout<<"TotalEmbeddedSS : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}

TH1F* TauMuPlotter::getSample(TString samplename){

  TString sel="eventweight*(categoryCh==1)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F(samplename,"",nbins_,xmin_,xmax_);
  h->Sumw2();

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if(TString((*s)->GetName())==samplename){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());
      delete hd;
    }  
  cout<<" getSample: "<<samplename<<" : "<<h->Integral()<<endl;

  return h;
}

TH1F* TauMuPlotter::getSampleSS(TString samplename){

  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F(samplename+"SS","",nbins_,xmin_,xmax_);
  h->Sumw2();

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if(TString((*s)->GetName())==samplename){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());
      delete hd;
    }  
  cout<<" getSampleSS: "<<samplename<<" : "<<h->Integral()<<endl;

  return h;
}

TH2F* TauMuPlotter::getSampleSS2D(TString samplename){

  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH2F* h=new TH2F(samplename+"SS","",nbins_,xmin_,xmax_,nbins2_,xmin2_,xmax2_);
  h->Sumw2();

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if(TString((*s)->GetName())==samplename){
      TH2F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,plotvar2_,nbins2_,xmin2_,xmax2_,sel);
      h->Add(hd,(*s)->getNorm());
      delete hd;
    }  
  cout<<samplename<<" : "<<h->Integral()<<endl;

  return h;
}


TH1F* TauMuPlotter::getDiBoson(){

  TH1F* h=new TH1F("hVV","VV",nbins_,xmin_,xmax_);
  h->Sumw2();  

  TH1F* hWW=getSample("WW");
  if(!hWW)return 0;
  h->Add(hWW);
  delete hWW;

  TH1F* hWZ=getSample("WZ");
  if(!hWZ)return 0;
  h->Add(hWZ);
  delete hWZ;

  TH1F* hZZ=getSample("ZZ");
  if(!hZZ)return 0;
  h->Add(hZZ);
  delete hZZ;

  cout<<"DiBoson : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}

TH1F* TauMuPlotter::getDiBosonSS(){

  TH1F* h=new TH1F("hVV","VV",nbins_,xmin_,xmax_);
  h->Sumw2();  

  TH1F* hWW=getSampleSS("WW");
  if(!hWW)return 0;
  h->Add(hWW);
  delete hWW;

  TH1F* hWZ=getSampleSS("WZ");
  if(!hWZ)return 0;
  h->Add(hWZ);
  delete hWZ;

  TH1F* hZZ=getSampleSS("ZZ");
  if(!hZZ)return 0;
  h->Add(hZZ);
  delete hZZ;

  cout<<"DiBoson : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}




TH1F* TauMuPlotter::getZToTauTau(){  
  TH1F*h=getSample("ZToTauTau");
  //TH1F*h=getTotalEmbedded(); 
  return h;
}

TH1F* TauMuPlotter::getZToTauTauSS(){  
  //TH1F*h=getSampleSS("ZToTauTau");
  TH1F*h=getTotalEmbeddedSS();  
  return h;
}



///////////////////Methods for inclusive categories start here
bool TauMuPlotter::correctSamplesInc(){

  //determine these factors here as they may be used by SM methods too
  /////Correct WJets using m_T high side with inclusive level selections
  cout<<"Determining WJets  from sidebands"<<endl;
  TString WJetsOSCorrSel=TString("eventweight*(categoryCh==1&&categoryMT==3)");
  TString WJetsSSCorrSel=TString("eventweight*(categoryCh==2&&categoryMT==3)");
  if(Isocat_>0){//scale factors depend on the isolation
    WJetsOSCorrSel+=TString("*(categoryIso==")+(long)Isocat_+")";
    WJetsSSCorrSel+=TString("*(categoryIso==")+(long)Isocat_+")";
  }
  if(SMcat_>=0){//should only be >0 if using plotSM()
    WJetsOSCorrSel+=TString("*(categorySM==")+(long)SMcat_+")";
    WJetsSSCorrSel+=TString("*(categorySM==")+(long)SMcat_+")";
  }
  if(extrasel_.CompareTo("1")!=0){//some other selection may be required like njet=0
    WJetsOSCorrSel+=TString("*")+extrasel_;
    WJetsSSCorrSel+=TString("*")+extrasel_;
  }


  TH1F HDataOS("HDataOSWJets","",400,0,400);
  TH1F HMCOS("HMCOSWJets","",400,0,400);
  TH1F HWJetsOS("HWJetsOSWJets","",400,0,400);
  TH1F HDataSS("HDataSSWJets","",400,0,400);
  TH1F HMCSS("HMCSSWJets","",400,0,400);
  TH1F HWJetsSS("HWJetsSSWJets","",400,0,400);
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if(TString((*s)->getDataType())=="Data"){
      TH1F* hos=(*s)->getHistoNtpFile("transversemass",400,0,400,WJetsOSCorrSel.Data());
      HDataOS.Add(hos);
      delete hos;
      TH1F* hss=(*s)->getHistoNtpFile("transversemass",400,0,400,WJetsSSCorrSel.Data());
      HDataSS.Add(hss);
      delete hss;
    }
    //scale only the WJets:
//     if(TString((*s)->GetName())=="WJetsToLNu"){
//       TH1F* hos=(*s)->getHistoNtpFile("transversemass",400,0,400,WJetsOSCorrSel.Data());
//       HWJetsOS.Add(hos,(*s)->getNorm());
//       delete hos;
//       TH1F* hss=(*s)->getHistoNtpFile("transversemass",400,0,400,WJetsSSCorrSel.Data());
//       HWJetsSS.Add(hss,(*s)->getNorm());
//       delete hss;
//     }
//     if(TString((*s)->getDataType())=="MC" && TString((*s)->GetName())!="WJetsToLNu"){
//       TH1F* hos=(*s)->getHistoNtpFile("transversemass",400,0,400,WJetsOSCorrSel.Data());
//       HMCOS.Add(hos,(*s)->getNorm());
//       delete hos;
//       TH1F* hss=(*s)->getHistoNtpFile("transversemass",400,0,400,WJetsSSCorrSel.Data());
//       HMCSS.Add(hss,(*s)->getNorm());
//       delete hss;
//     }

//Scale the total MC at high mT ( needed for example in njet==2 because TT+Zljet are large)
    if(TString((*s)->GetName())=="WJetsToLNu"
       || TString((*s)->GetName())=="TTJets"
       || TString((*s)->GetName())=="ZToLJet"
       || TString((*s)->GetName())=="ZToMuMu"
       ){
      TH1F* hos=(*s)->getHistoNtpFile("transversemass",400,0,400,WJetsOSCorrSel.Data());
      HWJetsOS.Add(hos,(*s)->getNorm());
      delete hos;
      TH1F* hss=(*s)->getHistoNtpFile("transversemass",400,0,400,WJetsSSCorrSel.Data());
      HWJetsSS.Add(hss,(*s)->getNorm());
      delete hss;
    }
    if(TString((*s)->getDataType())=="MC"
       && TString((*s)->GetName())!="WJetsToLNu"
       && TString((*s)->GetName())!="TTJets"
       && TString((*s)->GetName())!="ZToLJet"
       && TString((*s)->GetName())!="ZToMuMu"
       ){
      TH1F* hos=(*s)->getHistoNtpFile("transversemass",400,0,400,WJetsOSCorrSel.Data());
      HMCOS.Add(hos,(*s)->getNorm());
      delete hos;
      TH1F* hss=(*s)->getHistoNtpFile("transversemass",400,0,400,WJetsSSCorrSel.Data());
      HMCSS.Add(hss,(*s)->getNorm());
      delete hss;
    }

    
  }  
  
  WJetsOSSideCorr_=1.; Float_t WJetsOSSide=0; Float_t MCOSSide=0; Float_t DataOSSide=0;  
  WJetsOSSide=HWJetsOS.Integral();
  MCOSSide=HMCOS.Integral();
  DataOSSide=HDataOS.Integral();
  if(WJetsOSSide>0){
    WJetsOSSideCorr_=(DataOSSide-MCOSSide)/WJetsOSSide;  

    Float_t WJetsOSSideEr=0; Float_t MCOSSideEr=0; Float_t DataOSSideEr=0;
    for(Int_t b=1;b<=HWJetsOS.GetNbinsX();b++){
      if(HWJetsOS.GetBinContent(b)>0)WJetsOSSideEr+=HWJetsOS.GetBinError(b)*HWJetsOS.GetBinError(b);
      if(HMCOS.GetBinContent(b)>0)MCOSSideEr+=HMCOS.GetBinError(b)*HMCOS.GetBinError(b);
      if(HDataOS.GetBinContent(b)>0)DataOSSideEr+=HDataOS.GetBinError(b)*HDataOS.GetBinError(b);
    }
    WJetsOSSideCorrErr_=WJetsOSSideCorr_*sqrt((DataOSSideEr+MCOSSideEr)/((DataOSSide-MCOSSide)*(DataOSSide-MCOSSide)) + WJetsOSSideEr/(WJetsOSSide*WJetsOSSide) );
  }
  cout<<"WJetsIncOS  Correction = "<<WJetsOSSideCorr_<<" +- "<<WJetsOSSideCorrErr_<<endl;

  WJetsSSSideCorr_=1.; Float_t WJetsSSSide=0; Float_t MCSSSide=0; Float_t DataSSSide=0;
  WJetsSSSide=HWJetsSS.Integral();
  MCSSSide=HMCSS.Integral();
  DataSSSide=HDataSS.Integral();
  if(WJetsSSSide>0){
    WJetsSSSideCorr_=(DataSSSide-MCSSSide)/WJetsSSSide;

    Float_t WJetsSSSideEr=0; Float_t MCSSSideEr=0; Float_t DataSSSideEr=0;
    for(Int_t b=1;b<=HWJetsSS.GetNbinsX();b++){
      if(HWJetsSS.GetBinContent(b)>0)WJetsSSSideEr+=HWJetsSS.GetBinError(b)*HWJetsSS.GetBinError(b);
      if(HMCSS.GetBinContent(b)>0)MCSSSideEr+=HMCSS.GetBinError(b)*HMCSS.GetBinError(b);
      if(HDataSS.GetBinContent(b)>0)DataSSSideEr+=HDataSS.GetBinError(b)*HDataSS.GetBinError(b);
    }
    WJetsSSSideCorrErr_=WJetsSSSideCorr_*sqrt((DataSSSideEr+MCSSSideEr)/((DataSSSide-MCSSSide)*(DataSSSide-MCSSSide)) + WJetsSSSideEr/(WJetsSSSide*WJetsSSSide) );
  }
  cout<<"WJetsIncSS  Correction = "<<WJetsSSSideCorr_<<" +- "<<WJetsSSSideCorrErr_<<endl;


  //QCD correction factor
  cout<<" QCD Inc OS/SS scale factor "<<QCDOStoSSRatio_<<" +- "<<QCDOStoSSRatioErr_<<endl;


  return 1;
}

//WJets
TH1F* TauMuPlotter::getWJetsInc(){
  TH1F*h=getSample("WJetsToLNu");
  h->Scale(WJetsOSSideCorr_);
  float y=h->Integral();
  float ye=0;
  for(Int_t b=1;b<=h->GetNbinsX();b++)
    if(h->GetBinContent(b)>0) ye+=pow(h->GetBinContent(b)*sqrt(pow(h->GetBinError(b)/h->GetBinContent(b),2)+pow(WJetsOSSideCorrErr_/WJetsOSSideCorr_,2)),2);
  cout<<" WJets : "<<y<<" +- "<<sqrt(ye)<<endl;
  return h;
}

TH1F* TauMuPlotter::getWJetsIncSS(){
  TH1F*h=getSampleSS("WJetsToLNu");
  h->Scale(WJetsSSSideCorr_);
  float y=h->Integral();
  float ye=0;
  for(Int_t b=1;b<=h->GetNbinsX();b++)
    if(h->GetBinContent(b)>0) ye+=pow(h->GetBinContent(b)*sqrt(pow(h->GetBinError(b)/h->GetBinContent(b),2)+pow(WJetsSSSideCorrErr_/WJetsSSSideCorr_,2)),2);
  cout<<" WJets : "<<y<<" +- "<<sqrt(ye)<<endl;
  return h;
}

//TTJets
TH1F* TauMuPlotter::getTTJetsInc(){
  TH1F*h=getSample("TTJets");
  h->Scale(WJetsOSSideCorr_);
  float y=h->Integral();
  float ye=0;
  for(Int_t b=1;b<=h->GetNbinsX();b++)
    if(h->GetBinContent(b)>0) ye+=pow(h->GetBinContent(b)*sqrt(pow(h->GetBinError(b)/h->GetBinContent(b),2)+pow(WJetsOSSideCorrErr_/WJetsOSSideCorr_,2)),2);
  cout<<" TTJets : "<<y<<" +- "<<sqrt(ye)<<endl;
  return h;
}
TH1F* TauMuPlotter::getTTJetsIncSS(){
  TH1F*h=getSampleSS("TTJets");
  h->Scale(WJetsSSSideCorr_);
  float y=h->Integral();
  float ye=0;
  for(Int_t b=1;b<=h->GetNbinsX();b++)
    if(h->GetBinContent(b)>0) ye+=pow(h->GetBinContent(b)*sqrt(pow(h->GetBinError(b)/h->GetBinContent(b),2)+pow(WJetsSSSideCorrErr_/WJetsSSSideCorr_,2)),2);
  cout<<" TTJets : "<<y<<" +- "<<sqrt(ye)<<endl;
  return h;
}

//ZLJet
TH1F* TauMuPlotter::getZToLJetInc(){
  TH1F*h=getSample("ZToLJet");
  h->Scale(WJetsOSSideCorr_);
  float y=h->Integral();
  float ye=0;
  for(Int_t b=1;b<=h->GetNbinsX();b++)
    if(h->GetBinContent(b)>0) ye+=pow(h->GetBinContent(b)*sqrt(pow(h->GetBinError(b)/h->GetBinContent(b),2)+pow(WJetsOSSideCorrErr_/WJetsOSSideCorr_,2)),2);
  cout<<" ZToLJet : "<<y<<" +- "<<sqrt(ye)<<endl;
  return h;
}
TH1F* TauMuPlotter::getZToLJetIncSS(){
  TH1F*h=getSampleSS("ZToLJet");
  h->Scale(WJetsSSSideCorr_);
  float y=h->Integral();
  float ye=0;
  for(Int_t b=1;b<=h->GetNbinsX();b++)
    if(h->GetBinContent(b)>0) ye+=pow(h->GetBinContent(b)*sqrt(pow(h->GetBinError(b)/h->GetBinContent(b),2)+pow(WJetsSSSideCorrErr_/WJetsSSSideCorr_,2)),2);
  cout<<" ZToLJet : "<<y<<" +- "<<sqrt(ye)<<endl;
  return h;
}

//ZMuMu
TH1F* TauMuPlotter::getZToMuMuInc(){
  TH1F*h=getSample("ZToMuMu");
  h->Scale(WJetsOSSideCorr_);
  float y=h->Integral();
  float ye=0;
  for(Int_t b=1;b<=h->GetNbinsX();b++)
    if(h->GetBinContent(b)>0) ye+=pow(h->GetBinContent(b)*sqrt(pow(h->GetBinError(b)/h->GetBinContent(b),2)+pow(WJetsOSSideCorrErr_/WJetsOSSideCorr_,2)),2);
  cout<<" ZToMuMu : "<<y<<" +- "<<sqrt(ye)<<endl;
  return h;
}
TH1F* TauMuPlotter::getZToMuMuIncSS(){
  TH1F*h=getSampleSS("ZToMuMu");
  h->Scale(WJetsSSSideCorr_);
  float y=h->Integral();
  float ye=0;
  for(Int_t b=1;b<=h->GetNbinsX();b++)
    if(h->GetBinContent(b)>0) ye+=pow(h->GetBinContent(b)*sqrt(pow(h->GetBinError(b)/h->GetBinContent(b),2)+pow(WJetsSSSideCorrErr_/WJetsSSSideCorr_,2)),2);
  cout<<" ZToMuMu : "<<y<<" +- "<<sqrt(ye)<<endl;
  return h;
}

//
TH2F* TauMuPlotter::getWJetsIncSS2D(){
  TH2F*h=getSampleSS2D("WJetsToLNu");
  h->Scale(WJetsSSSideCorr_);
  return h;
}



TH1F* TauMuPlotter::getTotalBackgroundIncSS(){

  TH1F* h=new TH1F("hBackgroundSS","",nbins_,xmin_,xmax_);
  h->Sumw2();

  TH1F*hZToTauTauSS=getZToTauTauSS();//could be from Embedded
  if(!hZToTauTauSS)return 0;
  h->Add(hZToTauTauSS);
  delete hZToTauTauSS;

  TH1F*hWJetsSS=getWJetsIncSS();//WJets needs a correction factor
  //TH1F*hWJetsSS=getWJetsTauIsoSMSS(); hWJetsSS->Scale(1.29);//Estimation for njet==2 
  if(!hWJetsSS)return 0;
  h->Add(hWJetsSS);
  delete hWJetsSS;

  TH1F*hTTJetsSS=getTTJetsIncSS();//TTJets needs a correction factor
  if(!hTTJetsSS)return 0;
  h->Add(hTTJetsSS);
  delete hTTJetsSS;

  TH1F*hZToLJetSS=getZToLJetIncSS();//ZToLJet needs a correction factor
  if(!hZToLJetSS)return 0;
  h->Add(hZToLJetSS);
  delete hZToLJetSS;

  TH1F*hZToMuMuSS=getZToMuMuIncSS();//ZToMuMu needs a correction factor
  if(!hZToMuMuSS)return 0;
  h->Add(hZToMuMuSS);
  delete hZToMuMuSS;


  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel += TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC"
       && TString((*s)->GetName()) != "ZToTauTau"
       && TString((*s)->GetName()) != "WJetsToLNu"
       && TString((*s)->GetName()) != "TTJets"
       && TString((*s)->GetName()) != "ZToLJet"
       && TString((*s)->GetName()) != "ZToMuMu"
       ){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm()); 
      delete hd;
    }

  
  return h;
}

TH2F* TauMuPlotter::getTotalBackgroundIncSS2D(){
  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel += TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH2F* h=new TH2F("hBackgroundSS","",nbins_,xmin_,xmax_,nbins2_,xmin2_,xmax2_);
  h->Sumw2();


  TH2F*hWJetsSS=getWJetsIncSS2D();//WJets needs a correction factor
  if(!hWJetsSS)return 0;
  h->Add(hWJetsSS);
  delete hWJetsSS;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC" && TString((*s)->GetName()) != "ZToTauTau" && TString((*s)->GetName()) != "WJetsToLNu"){
      TH2F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,plotvar2_,nbins2_,xmin2_,xmax2_,sel);
      h->Add(hd,(*s)->getNorm()); 
      delete hd;
    }

  
  return h;
}


TH1F* TauMuPlotter::getQCDInc(){

  TH1F* h=new TH1F("hQCDInc","QCDInc",nbins_,xmin_,xmax_);
  h->Sumw2();
  TH1F* hDataSS=getTotalDataSS();
  h->Add(hDataSS);
  delete hDataSS;
  TH1F* hMCSS=getTotalBackgroundIncSS();
  h->Add(hMCSS,-1.);
  delete hMCSS;

  cout<<"Scaling QCDInc by OS/SS factor "<<QCDOStoSSRatio_<<endl;
  h->Scale(QCDOStoSSRatio_);

  float yerr=0.;
  for(Int_t b=1;b<=nbins_;b++)
    yerr+=h->GetBinError(b)*h->GetBinError(b)+QCDOStoSSRatioErr_*QCDOStoSSRatioErr_;
  

  cout<<"QCDInc : "<<h->Integral()<<" +- "<<sqrt(yerr)<<endl;

  return h;
}



bool TauMuPlotter::plotInc(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax,  TString extrasel, TString xlabel, TString ylabel, Float_t* legendcoords, bool log){


  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  Isocat_=Isocat;
  MTcat_=MTcat;
  SMcat_=SMcat;
  if(extrasel.CompareTo("")!=0) extrasel_ += TString("*")+extrasel;


  TString plotvarname=plotvar_+extrasel_;
  fixFileTag(&plotvarname);

  TString filename=outputpath_+"/TauMuPlotter_"+plotvarname;
  if(Isocat_>0) filename=filename+"_Iso"+(long)Isocat_;
  if(MTcat_>0) filename=filename+"_MT"+(long)MTcat_;
  if(SMcat_>=0) filename=filename+"_SM"+(long)SMcat_;
  filename+=".ps";

  TCanvas C("C",filename);  
  C.Print(filename+"[");

  //for checking mT in the antiIso samples
  //extrasel_ = extrasel_+"*"+qcdTauIsoRatio_;//apply  ratio here 
  //extrasel_ = extrasel_+"*"+qcdMuIsoRatio_;//apply  ratio here 
  //extrasel_ = extrasel_+"*"+qcdTauIsoRatio_+"*"+qcdMuIsoRatio_;//apply  ratio here 

  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;

   
  
  TH1F*hQCD=0;
  if(SMcat_==-1 || SMcat_==0 || Isocat_!=1) hQCD=getQCDInc();
  else if(SMcat_==1 || SMcat_==2) hQCD=getQCDIsoSM();
  else hQCD=0;
  if(hQCD){
    hQCD->SetLineWidth(1);
    hQCD->SetLineColor(QCDColor_);
    hQCD->SetFillColor(QCDColor_);
  }
  
  //TH1F*hTTJets=getSample("TTJets");
  TH1F*hTTJets=getTTJetsInc();
  if(!hTTJets)return 0;
  hTTJets->SetLineWidth(1);
  hTTJets->SetLineColor(TTJetsColor_);
  hTTJets->SetFillColor(TTJetsColor_);
 
  ///Z->tau tau
  TH1F*hZToTauTau=getZToTauTau();
  if(!hZToTauTau)return 0;
  hZToTauTau->SetLineWidth(1);
  hZToTauTau->SetLineColor(ZTauTauColor_);
  hZToTauTau->SetFillColor(ZTauTauColor_);
 
  
  TH1F*hWJetsToLNu=0;
  hWJetsToLNu=getWJetsInc();
  //hWJetsToLNu=getWJetsTauIsoSM();
  if(!hWJetsToLNu)return 0;
  hWJetsToLNu->SetLineWidth(1);
  hWJetsToLNu->SetLineColor(WJetsColor_);
  hWJetsToLNu->SetFillColor(WJetsColor_);
  
  TH1F*hEWK=(TH1F*)hWJetsToLNu->Clone("hElectroWeak");
  delete hWJetsToLNu;

  //combine Di-Bosons
  TH1F*hVV=getDiBoson();
  if(!hVV)return 0;
  hEWK->Add(hVV);
  delete hVV;

  //combine ZLJet
  //TH1F*hZToLJet=getSample("ZToLJet");
  TH1F*hZToLJet=getZToLJetInc();
  if(!hZToLJet)return 0;
  hEWK->Add(hZToLJet);
  delete hZToLJet;  

  //combine Z->MuMu
  //TH1F*hZToMuMu=getSample("ZToMuMu");
  TH1F*hZToMuMu=getZToMuMuInc();
  if(!hZToMuMu)return 0;
  hZToMuMu->SetLineWidth(1);
  hZToMuMu->SetLineColor(ZMuMuColor_);
  hZToMuMu->SetFillColor(ZMuMuColor_);
  hEWK->Add(hZToMuMu);
  delete hZToMuMu;

  
  ////////////////////////
  cout<<"Creating Bkg Stack:"<<endl;
  THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
  if(hQCD)hMCStack.Add(hQCD,"hist");
  hMCStack.Add(hEWK,"hist");
  hMCStack.Add(hTTJets,"hist");
  hMCStack.Add(hZToTauTau,"hist");



  //create total Background
  TH1F* hBkg=new TH1F("hBkg","Bkg",nbins_,xmin_,xmax_);
  TList* histList= hMCStack.GetHists();
  for(Int_t i=0;i<histList->GetSize();i++){
    hBkg->Add((TH1F*)(histList->At(i)));
  }


  ////Total Data
  TH1F* hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
 

  //create legend key
  TLegend legend;
  legend.SetLineColor(0);
  legend.SetBorderSize(1);
  legend.AddEntry(hData,hData->GetTitle(),"p");
  legend.AddEntry(hZToTauTau,"Z#rightarrow#tau^{+}#tau^{-}","f");
  legend.AddEntry(hTTJets,"t#bar{t}","f");
  legend.AddEntry(hEWK,"EWK","f");
  if(hQCD)  legend.AddEntry(hQCD,"QCD","f");

  cout<<" Summary of yields : "<<endl;
  if(hQCD) cout<<"QCD "<<hQCD->Integral(0,hQCD->GetNbinsX()+1)<<endl;
  cout<<"ElectroWeak "<<hEWK->Integral(0,hEWK->GetNbinsX()+1)<<endl;
  cout<<"TTJets "<<hTTJets->Integral(0,hTTJets->GetNbinsX()+1)<<endl;
  cout<<"ZToTauTau "<<hZToTauTau->Integral(0,hZToTauTau->GetNbinsX()+1)<<endl;
  cout<<"Total Bkg "<<hBkg->Integral(0,hBkg->GetNbinsX()+1)<<endl;
  cout<<"Total Data "<<(int)(hData->Integral(0,hData->GetNbinsX()+1))<<endl;

  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  //hData->GetYaxis()->SetRangeUser(axesrange[2],axesrange[3]);
  if(log) C.SetLogy(1);
  hData->SetStats(0);
  hData->GetXaxis()->SetTitle(xlabel);
  hData->GetYaxis()->SetTitle(ylabel);
  hData->Draw("hist pe");
  //////////////////////////
  hMCStack.Draw("hist same");  
  hData->Draw("hist pe same");//bring Data points back to front
  hBkg->SetFillColor(0);
  hBkg->SetLineColor(1);
  hBkg->Draw("histsame");//draw outline of MC        
  hMCStack.GetHistogram()->Draw("same");//bring axes back to front 
  if(legendcoords){
    legend.SetX1NDC(legendcoords[0]);
    legend.SetX2NDC(legendcoords[1]);
    legend.SetY1NDC(legendcoords[2]);
    legend.SetY2NDC(legendcoords[3]);
  }
  legend.SetTextSize(.04);
  legend.Draw();
  C.Update();
  C.Print(filename);  
  if(log) C.SetLogy(0);


  //make the difference plot  
  TH1F* hDiff=new TH1F("hDiff","Difference",nbins_,xmin_,xmax_);
  hDiff->GetXaxis()->SetTitle(plotvar_);
  hDiff->Add(hData);
  hDiff->Add(hBkg,-1);
  C.Clear();
  hDiff->Draw("hist pe");
  C.Print(filename);  


  //make the ratio plot
  TH1F* hRatio=new TH1F("hRatio","Ratio",nbins_,xmin_,xmax_);
  hRatio->GetXaxis()->SetTitle(plotvar_);
  hRatio->Add(hData);
  hRatio->Divide(hBkg);
  C.Clear();
  hRatio->Draw("hist pe");
  C.Print(filename);  

  C.Print(filename+"]");
  cout<<"closed ps file"<<endl;


  TFile DiffFile(outputpath_+"/DiffFileInc_"+plotvarname+"_TauIso"+(long)Isocat_+"_MT"+(long)MTcat_+"_SM"+(long)SMcat_+".root","recreate");
  hDiff->Write();
  hRatio->Write();
  DiffFile.ls();
  DiffFile.Close();


  return 1;

}


bool TauMuPlotter::plotIncSS(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax,  TString extrasel, TString xlabel, TString ylabel, Float_t* legendcoords, bool log){
 

  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  Isocat_=Isocat;
  MTcat_=MTcat;
  SMcat_=SMcat;
  if(extrasel.CompareTo("")!=0) extrasel_ += TString("*")+extrasel;

  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;


  TString plotvarname=plotvar_+extrasel_;
  fixFileTag(&plotvarname);


  TString filename=outputpath_+"/TauMuPlotterSS_"+plotvarname;
  if(Isocat_>0) filename=filename+"_Iso"+(long)Isocat_;
  if(MTcat_>0) filename=filename+"_MT"+(long)MTcat_;
  if(SMcat_>=0) filename=filename+"_SM"+(long)SMcat_;
  filename+=".ps";

  TCanvas C("C",filename);  
  C.Print(filename+"[");
  
  
  ////Total Data
  TH1F* hData=getTotalDataSS();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
 
  //
  TH1F*hQCD=0;//QCD for SS inclusive not defined
//   if(Isocat_==1)hQCD=getQCDIsoSMSS();
//   if(hQCD){
//     hQCD->SetLineWidth(1);
//     hQCD->SetLineColor(QCDColor_);
//     hQCD->SetFillColor(QCDColor_);
//   }
  

  //
  //TH1F*hTTJets=getSampleSS("TTJets");
  TH1F*hTTJets=getTTJetsIncSS();
  if(!hTTJets)return 0;
  hTTJets->SetLineWidth(1);
  hTTJets->SetLineColor(TTJetsColor_);
  hTTJets->SetFillColor(TTJetsColor_);
 
  ///Z->tau tau
  TH1F*hZToTauTau=getZToTauTauSS();
  if(!hZToTauTau)return 0;
  hZToTauTau->SetLineWidth(1);
  hZToTauTau->SetLineColor(ZTauTauColor_);
  hZToTauTau->SetFillColor(ZTauTauColor_);

  //
  TH1F*hWJetsToLNu=getWJetsIncSS();
  //TH1F*hWJetsToLNu=getWJetsTauIsoSMSS(); hWJetsToLNu->Scale(1.29); //estimation in njet=2
  if(!hWJetsToLNu)return 0;
  hWJetsToLNu->SetLineWidth(1);
  hWJetsToLNu->SetLineColor(WJetsColor_);
  hWJetsToLNu->SetFillColor(WJetsColor_);
  
  TH1F*hEWK=(TH1F*)hWJetsToLNu->Clone("hElectroWeak");
  delete hWJetsToLNu;

  //combine Di-Bosons
  TH1F*hVV=getDiBosonSS();
  if(!hVV)return 0;
  hEWK->Add(hVV);
  delete hVV;

  //combine ZLJet
  //TH1F*hZToLJet=getSampleSS("ZToLJet");
  TH1F*hZToLJet=getZToLJetIncSS();
  if(!hZToLJet)return 0;
  hEWK->Add(hZToLJet);
  delete hZToLJet;  

  //combine Z->MuMu
  //TH1F*hZToMuMu=getSampleSS("ZToMuMu");
  TH1F*hZToMuMu=getZToMuMuIncSS();
  if(!hZToMuMu)return 0;
  hZToMuMu->SetLineWidth(1);
  hZToMuMu->SetLineColor(ZMuMuColor_);
  hZToMuMu->SetFillColor(ZMuMuColor_);
  hEWK->Add(hZToMuMu);
  delete hZToMuMu;


  ///////////
  cout<<"Creating Bkg Stack:"<<endl;
  THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
  if(hQCD)hMCStack.Add(hQCD,"hist");
  hMCStack.Add(hEWK,"hist");
  hMCStack.Add(hTTJets,"hist");
  hMCStack.Add(hZToTauTau,"hist");

  //create legend key
  TLegend legend;
  legend.SetLineColor(0);
  legend.SetBorderSize(1);
  legend.AddEntry(hData,hData->GetTitle(),"p");
  legend.AddEntry(hZToTauTau,"Z#rightarrow#tau^{+}#tau^{-}","f");
  legend.AddEntry(hTTJets,"t#bar{t}","f");
  legend.AddEntry(hEWK,"EWK","f");
  if(hQCD)legend.AddEntry(hQCD,"QCD","f");


  //create total Background
  TH1F* hBkg=new TH1F("hBkg","Bkg",nbins_,xmin_,xmax_);
  TList* histList= hMCStack.GetHists();
  for(Int_t i=0;i<histList->GetSize();i++){
    hBkg->Add((TH1F*)(histList->At(i)));
  }


  cout<<" Summary of yields : "<<endl;
  if(hQCD)  cout<<"QCD "<<hQCD->Integral(0,hQCD->GetNbinsX()+1)<<endl;
  cout<<"ElectroWeak "<<hEWK->Integral(0,hEWK->GetNbinsX()+1)<<endl;
  cout<<"TTJets "<<hTTJets->Integral(0,hTTJets->GetNbinsX()+1)<<endl;
  cout<<"ZToTauTau "<<hZToTauTau->Integral(0,hZToTauTau->GetNbinsX()+1)<<endl;
  cout<<"Total Bkg "<<hBkg->Integral(0,hBkg->GetNbinsX()+1)<<endl;
  cout<<"Total Data "<<(int)(hData->Integral(0,hData->GetNbinsX()+1))<<endl;
  
  

  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  if(log) C.SetLogy(1);
  hData->SetStats(0);
  hData->GetXaxis()->SetTitle(xlabel);
  hData->GetYaxis()->SetTitle(ylabel);
  hData->Draw("hist pe");
  //////////////////////////
  hMCStack.Draw("hist same");  
  hData->Draw("hist pe same");//bring Data points back to front
  hMCStack.GetHistogram()->Draw("same");//bring axes back to front 
  if(legendcoords){
    legend.SetX1NDC(legendcoords[0]);
    legend.SetX2NDC(legendcoords[1]);
    legend.SetY1NDC(legendcoords[2]);
    legend.SetY2NDC(legendcoords[3]);
  }
  legend.SetTextSize(.04);
  legend.Draw();
  C.Update();
  C.Print(filename);  
  if(log) C.SetLogy(0);


  //make the difference plot  
  TH1F* hDiff=new TH1F("hDiff","Difference",nbins_,xmin_,xmax_);
  hDiff->GetXaxis()->SetTitle(plotvar_);
  hDiff->Add(hData);
  hDiff->Add(hBkg,-1);
  C.Clear();
  hDiff->Draw("hist pe");
  C.Print(filename);  

  //make the ratio plot
  TH1F* hRatio=new TH1F("hRatio","Ratio",nbins_,xmin_,xmax_);
  hRatio->GetXaxis()->SetTitle(plotvar_);
  hRatio->Add(hData);
  hRatio->Divide(hBkg);
  C.Clear();
  hRatio->Draw("hist pe");
  C.Print(filename);  

 
  C.Print(filename+"]");
  cout<<"closed ps file"<<endl;


  TFile DiffFile(outputpath_+"/DiffFileIncSS_"+plotvarname+"_Iso"+(long)Isocat_+"_MT"+(long)MTcat_+"_SM"+(long)SMcat_+".root","recreate");
  hDiff->Write();
  hRatio->Write();
  DiffFile.ls();
  DiffFile.Close();
    

  return 1;

}



TH1F* TauMuPlotter::getWJetsSM(){
  TH1F* h=0;
  float y=0.;
  float ye=0.;
  //samples with low stats at high mT dont apply scale factor
  h=getSample("WJetsToLNu");
  y=h->Integral();
  ye=0;
  for(Int_t b=1;b<=h->GetNbinsX();b++){
    if(h->GetBinContent(b)>0)ye+=h->GetBinError(b)*h->GetBinError(b);
  }
  
  cout<<" WJetsSM : "<<y<<" +- "<<sqrt(ye)<<endl;
  return h;
}

TH1F* TauMuPlotter::getWJetsSMSS(){
  TH1F* h=0;
  float y=0.;
  float ye=0.;

  //samples with low stats at high mT dont apply scale factor
  h=getSampleSS("WJetsToLNu");
  y=h->Integral();
  ye=0;
  for(Int_t b=1;b<=h->GetNbinsX();b++){
    if(h->GetBinContent(b)>0)ye+=h->GetBinError(b)*h->GetBinError(b);
  }
  
  cout<<" WJetsSM : "<<y<<" +- "<<sqrt(ye)<<endl;
  return h;
}


TH1F* TauMuPlotter::getQCDMuIsoSM(){
  TString sel="eventweight*(categoryCh==1&&0.3<muiso&&muiso<0.5)";
  if(MTcat_>0)  sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0) sel += TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F("hSMQCD","SMQCD",nbins_,xmin_,xmax_);
  h->Sumw2();
 
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd);
      delete hd;
    }
  }

  h->Scale(QCDMuIsoSideRatio_);

  return h;
}

TH1F* TauMuPlotter::getTotalMCSM(){
  TH1F* h=new TH1F("hTotalMCSM","total MC",nbins_,xmin_,xmax_);
  h->Sumw2();
   

  TH1F*hZToTauTau=getZToTauTau();
  if(!hZToTauTau)return 0;
  h->Add(hZToTauTau);
  delete hZToTauTau;
  //TH1F*hWJets=getWJetsSM();
  //TH1F*hWJets=getSample("WJetsToLNu");
  TH1F*hWJets=getWJetsInc();//do not use IF highMT correction factors are calculated separately for in the QCD method
  if(!hWJets)return 0;
  h->Add(hWJets);
  delete hWJets;
  TH1F*hDiBoson=getDiBoson();
  if(!hDiBoson)return 0;
  h->Add(hDiBoson);
  delete hDiBoson;
  //TH1F*hTTJets=getSample("TTJets");
  TH1F*hTTJets=getTTJetsInc();
  if(!hTTJets)return 0;
  h->Add(hTTJets);
  delete hTTJets;
  //TH1F*hZLJet=getSample("ZToLJet");
  TH1F*hZLJet=getZToLJetInc();
  if(!hZLJet)return 0;
  h->Add(hZLJet);
  delete hZLJet;
  //TH1F*hZToMuMu=getSample("ZToMuMu");
  TH1F*hZToMuMu=getZToMuMuInc();
  if(!hZToMuMu)return 0;
  h->Add(hZToMuMu);
  delete hZToMuMu;

  cout<<" TotalMCSM : "<<h->Integral()<<endl;
  return h;
}


TH1F* TauMuPlotter::getTotalMCSMSS(){
  TH1F* h=new TH1F("hTotalMCSM","total MC",nbins_,xmin_,xmax_);
  h->Sumw2();
   

  TH1F*hZToTauTau=getZToTauTauSS();
  if(!hZToTauTau)return 0;
  h->Add(hZToTauTau);
  delete hZToTauTau;
  //TH1F*hWJets=getWJetsSMSS();
  TH1F*hWJets=getWJetsIncSS();
  if(!hWJets)return 0;
  h->Add(hWJets);
  delete hWJets;
  TH1F*hDiBoson=getDiBosonSS();
  if(!hDiBoson)return 0;
  h->Add(hDiBoson);
  delete hDiBoson;
  //TH1F*hTTJets=getSampleSS("TTJets");
  TH1F*hTTJets=getTTJetsIncSS();
  if(!hTTJets)return 0;
  h->Add(hTTJets);
  delete hTTJets;
  //TH1F*hZLJet=getSampleSS("ZToLJet");
  TH1F*hZLJet=getZToLJetIncSS();
  if(!hZLJet)return 0;
  h->Add(hZLJet);
  delete hZLJet;
  //TH1F*hZToMuMu=getSampleSS("ZToMuMu");
  TH1F*hZToMuMu=getZToMuMuIncSS();
  if(!hZToMuMu)return 0;
  h->Add(hZToMuMu);
  delete hZToMuMu;

  cout<<" TotalMCSMSS : "<<h->Integral()<<endl;

  return h;
}


TH1F* TauMuPlotter::getQCDMike(){

  Int_t TmpSMcat=SMcat_;
  Int_t TmpIsocat=Isocat_;
  TString TmpExtrasel=extrasel_;  

   
  //SS incl QCD 
  Isocat_=1;
  SMcat_=-1;
  extrasel_="1";
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;
  TH1F* hQCDInc = getQCDInc();  if(!hQCDInc){cout<<" QCDInc not determined "<<endl; return 0;}   hQCDInc->SetName("hQCDInc");


  //SS Loose Incl QCD 
  Isocat_=-1;  
  SMcat_=-1;
  extrasel_="1";
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;
  TH1F* hDataIncLoose = getTotalDataSS();  if(!hDataIncLoose){cout<<" Total Data not determined "<<endl; return 0;}   hDataIncLoose->SetName("hDataIncLoose");
  TH1F* hMCIncLoose = getTotalBackgroundIncSS();  if(!hMCIncLoose){cout<<" Total MC not determined "<<endl; return 0;}   hMCIncLoose->SetName("hMCIncLoose");

  //SS Loose VBF QCD 
  SMcat_=TmpSMcat;
  Isocat_=-1;  
  extrasel_=TmpExtrasel;
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;
  TH1F* hDataVBFLoose=getTotalDataSS();  if(!hDataVBFLoose){cout<<" Total Data not determined "<<endl; return 0;}   hDataVBFLoose->SetName("hDataVBFLoose");
  TH1F* hMCVBFLoose=getTotalBackgroundIncSS();   if(!hMCVBFLoose){cout<<" Total MC not determined "<<endl; return 0;}   hMCVBFLoose->SetName("hMCVBFLoose");
  
  //return scale factors to normal
  Isocat_=TmpIsocat;
  extrasel_=TmpExtrasel;
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;

  //OS VBF QCD = 1.11 * (SS VBF QCD with loose iso.) * [(SS incl QCD) / ( SS incl QCD with loose iso.)]
  TH1F* h=new TH1F("hSMQCD","QCD",nbins_,xmin_,xmax_);
  h->Sumw2();
  h->Add(hDataVBFLoose);
  h->Add(hMCVBFLoose,-1.);
  cout<<" QCDInc "<<hQCDInc->Integral()
      <<" DataVBFLoose "<<hDataVBFLoose->Integral()
      <<" VBFLoose "<<hMCVBFLoose->Integral()
      <<" DataIncLoose "<<hDataIncLoose->Integral()
      <<" MCIncLoose "<<hMCIncLoose->Integral()<<endl;
  float ratio=hQCDInc->Integral()/(hDataIncLoose->Integral()-hMCIncLoose->Integral());
  //float ratio=(hQCDInc->Integral()/hDataIncLoose->Integral());
  cout<<" Scaling Incl QCD by "<<ratio<<endl;
  h->Scale(ratio);
  cout<<"QCDMike "<<h->Integral()<<endl;

  TFile FQCD(outputpath_+"/FQCDMike_"+plotvar_+"_TauIso"+(long)Isocat_+"_MT"+(long)MTcat_+"_SM"+(long)SMcat_+".root","recreate");
  h->Write();
  hDataIncLoose->Write();
  hMCIncLoose->Write();
  hDataVBFLoose->Write();
  hMCVBFLoose->Write();
  FQCD.ls();
  FQCD.Close();

  delete hDataIncLoose;
  delete hMCIncLoose;
  delete hDataVBFLoose;
  delete hMCVBFLoose;

  return h;


}

TH1F* TauMuPlotter::getQCDTauIsoSM(){
  
  TH1F* h=new TH1F("hSMQCD","SMQCD",nbins_,xmin_,xmax_);
  h->Sumw2();
   
  Int_t TmpIsocat=Isocat_;  
  Isocat_=2;//switch temporarily while keeping all other categories the same  
  if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
  if(!correctSamplesInc())return 0;


  TFile FQCDTauIsoSM(outputpath_+"/FQCDTauIsoSM_"+plotvar_+"_TauIso"+(long)Isocat_+"_MT"+(long)MTcat_+"_SM"+(long)SMcat_+".root","recreate");


  /////////////////////
  cout<<"Applying Tau Iso QCD ratio : "<<qcdTauIsoRatio_<<endl;
  TString tmpextrasel=extrasel_;
  extrasel_ += TString("*(nditau==1)*")+qcdTauIsoRatio_;//apply  ratio here 

  TH1F* hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
  TH1F* hMC=getTotalMCSM();
  if(!hMC){cout<<" Total Data not determined "<<endl; return 0;}
  h->Add(hData);
  h->Add(hMC,-1);
  hData->SetName("hData"); hData->Write();
  hMC->SetName("hMC"); hMC->Write();
  delete hData;
  delete hMC;
  //fix the negative bins
  for(Int_t b=1;b<=nbins_;b++) 
    if(h->GetBinContent(b)<0.){
      h->SetBinContent(b,0); h->SetBinError(b,0);
    }
  h->SetName("hQCD");h->Write();


//   //correct the tau pt shape but keep the same yield
//   Float_t QCDYield=h->Integral();
//   cout<<"Mean before "<<h->GetMean()<<endl;
//   h->Clear();
//   TString tauptshape="*(0.25*(taupt-20))";
//   extrasel_+=tauptshape;
//   hData=getTotalData();
//   if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
//   hMC=getTotalMCSM();
//   if(!hMC){cout<<" Total Data not determined "<<endl; return 0;}
//   h->Add(hData);
//   h->Add(hMC,-1);
//   hData->SetName("hDataShape"); hData->Write();
//   hMC->SetName("hMCShape"); hMC->Write();
//   delete hData;
//   delete hMC;
//   //fix the negative bins
//   for(Int_t b=1;b<=nbins_;b++) 
//     if(h->GetBinContent(b)<0.){
//       h->SetBinContent(b,0); h->SetBinError(b,0);
//     }
//   h->Scale(QCDYield/h->Integral());
//   cout<<"Mean after "<<h->GetMean()<<endl;
//   h->SetName("hQCDShape");h->Write();
 
  FQCDTauIsoSM.ls();
  FQCDTauIsoSM.Close();

  cout<<"QCDTauIso "<<h->Integral()<<endl;
  extrasel_=tmpextrasel;
  ////////////////////////

  Isocat_=TmpIsocat;//return to normal
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;


  return h;

}


TH1F* TauMuPlotter::getQCDTauIsoSMSS(){  //needed for cross-check

  TH1F* h=new TH1F("hSMQCDSS","SMQCDSS",nbins_,xmin_,xmax_);
  h->Sumw2();
   
  Int_t TmpIsocat=Isocat_;  
  Isocat_=2;//switch temporarily while keeping all other categories the same
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;

  
  cout<<"Applying Tau Iso QCD ratio : "<<qcdTauIsoRatio_<<endl;
  TString tmpextrasel=extrasel_;
  extrasel_ +=  TString("*(nditau==1)*")+qcdTauIsoRatio_;//apply pt dependent ratio here 
  TH1F* hData=getTotalDataSS();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
  h->Add(hData);
  delete hData;
  TH1F* hMC=getTotalMCSMSS();
  if(!hMC){cout<<" Total Data not determined "<<endl; return 0;}
  h->Add(hMC,-1);
  delete hMC;
  extrasel_=tmpextrasel;

  Isocat_=TmpIsocat;
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;


  return h;

}


TH1F* TauMuPlotter::getQCDIsoSM(){
   

  TH1F* h=new TH1F("hQCD","SMQCD",nbins_,xmin_,xmax_);
  h->Sumw2();
  h->GetXaxis()->SetTitle(plotvar_);
  
  Int_t TmpIsocat=Isocat_;  
  Isocat_=-1;  
  TString tmpextrasel=extrasel_;
  

  TFile FQCDIsoSM(outputpath_+"/FQCDIsoSM_"+plotvar_+"_TauIso"+(long)Isocat_+"_MT"+(long)MTcat_+"_SM"+(long)SMcat_+".root","recreate");
  //Save the intermediate histograms in a root file

//   //First calculate MC correction factors from high mT
//   Float_t MCCorrB=1.;
//   Float_t MCCorrC=1.;
//   Float_t MCCorrD=1.;

//   Int_t tmpMTcat=MTcat_;
//   MTcat_=3;
//   extrasel_ += "*(muiso<0.1&&tauisodisc<2)";
//   if(!scaleSamplesLumi())return 0;
//   if(!correctSamplesInc())return 0;
//   extrasel_ = extrasel_+"*"+qcdTauIsoRatio_;//apply  ratio here 
//   cout<<"Applying Tau Iso QCD ratio : "<<qcdTauIsoRatio_<<endl;
//   TH1F* hDataMTB=getTotalData();
//   TH1F* hMCMTB=getTotalMCSM();
//   extrasel_ = tmpextrasel;
//   MCCorrB=hDataMTB->Integral()/hMCMTB->Integral();
//   delete hDataMTB; delete hMCMTB;

//   extrasel_ += "*(muiso>=0.1&&tauisodisc>=2)";
//   if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
//   if(!correctSamplesInc())return 0;
//   cout<<"Applying Mu Iso QCD ratio : "<<qcdMuIsoRatio_<<endl;
//   extrasel_ = extrasel_+"*"+qcdMuIsoRatio_;//apply  ratio here 
//   TH1F* hDataMTC=getTotalData();
//   TH1F* hMCMTC=getTotalMCSM();
//   extrasel_ = tmpextrasel;
//   MCCorrC=hDataMTC->Integral()/hMCMTC->Integral();
//   delete hDataMTC; delete hMCMTC;


//   extrasel_ += "*(muiso>0.1&&tauisodisc<2)";
//   if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
//   if(!correctSamplesInc())return 0;
//   cout<<"Applying Tau-Mu Iso QCD ratio : "<<qcdTauIsoRatioMuNonIso_+"*"+qcdMuIsoRatioTauNonIso_<<endl;
//   extrasel_ = extrasel_+"*"+qcdTauIsoRatioMuNonIso_+"*"+qcdMuIsoRatioTauNonIso_;//apply  ratio here 
//   //cout<<"Applying Tau-Mu Iso QCD ratio : "<<qcdTauIsoRatio_+"*"+qcdMuIsoRatio_<<endl;
//   //extrasel_ = extrasel_+"*"+qcdTauIsoRatio_+"*"+qcdMuIsoRatio_;//apply  ratio here 
//   TH1F* hDataMTD=getTotalData();
//   TH1F* hMCMTD=getTotalMCSM();
//   extrasel_ = tmpextrasel;
//   MCCorrD=hDataMTD->Integral()/hMCMTD->Integral();
//   delete hDataMTD; delete hMCMTD;
//   ///////////////////
//   MTcat_=tmpMTcat;
  
//   cout<<"high mT correction factor B: "<<MCCorrB<<endl;
//   cout<<"high mT correction factor C: "<<MCCorrC<<endl;
//   cout<<"high mT correction factor D: "<<MCCorrD<<endl;

  ///Now calculate the QCD in the low mT region (while applying the above corrections to the MC)
  //QCD From Region B
  TH1F* hB=new TH1F("hSMQCDB","SMQCDB",nbins_,xmin_,xmax_);
  hB->Sumw2();
  extrasel_ += "*(muiso<0.1&&tauisodisc<2)";
  cout<<"Applying Tau Iso QCD ratio : "<<qcdTauIsoRatio_<<endl;
  extrasel_ = extrasel_+"*"+qcdTauIsoRatio_;//apply  ratio here 
  if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
  if(!correctSamplesInc())return 0;
  TH1F* hDataB=getTotalData();  if(!hDataB){cout<<" Total Data not determined "<<endl; return 0;}   hDataB->SetName("hDataB");
  TH1F* hMCB=getTotalMCSM();   if(!hMCB){cout<<" Total Data not determined "<<endl; return 0;}   hMCB->SetName("hMCB");
  extrasel_ = tmpextrasel;
  hB->Add(hDataB,1.);
  //hB->Add(hMCB,-1.*MCCorrB);
  hB->Add(hMCB,-1.);
  cout<<"QCD Region B "<<hDataB->Integral()<<" - "<<hMCB->Integral()<<endl;
  hDataB->Write(); delete hDataB;
  hMCB->Write(); delete hMCB;

  //QCD From Region C
  TH1F* hC=new TH1F("hSMQCC","SMQCC",nbins_,xmin_,xmax_);
  hC->Sumw2();
  extrasel_ += "*(muiso>=0.1&&tauisodisc>=2)";
  cout<<"Applying Mu Iso QCD ratio : "<<qcdMuIsoRatio_<<endl;
  extrasel_ = extrasel_+"*"+qcdMuIsoRatio_;//apply  ratio here 
  if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
  if(!correctSamplesInc())return 0;
  TH1F* hDataC=getTotalData();  if(!hDataC){cout<<" Total Data not determined "<<endl; return 0;}   hDataC->SetName("hDataC");
  TH1F* hMCC=getTotalMCSM();   if(!hMCC){cout<<" Total Data not determined "<<endl; return 0;}   hMCC->SetName("hMCC");
  extrasel_ = tmpextrasel;
  hC->Add(hDataC,1.);
  //hC->Add(hMCC,-1.*MCCorrC);
  hC->Add(hMCC,-1.);
  Float_t Cerr=0.;
  for(Int_t b=1;b<=nbins_;b++) Cerr+=hC->GetBinError(b)*hC->GetBinError(b);
  cout<<"QCD Region C "<<hDataC->Integral()<<" - "<<hMCC->Integral()<<endl;
  hDataC->Write();delete hDataC;
  hMCC->Write();delete hMCC;


  //QCD From Region D
  TH1F* hD=new TH1F("hSMQCD","SMQCD",nbins_,xmin_,xmax_);
  hD->Sumw2();
  extrasel_ += "*(muiso>0.1&&tauisodisc<2)";
  //cout<<"Applying Tau-Mu Iso QCD ratio : "<<qcdTauIsoRatioMuNonIso_+"*"+qcdMuIsoRatioTauNonIso_<<endl;
  //extrasel_ = extrasel_+"*"+qcdTauIsoRatioMuNonIso_+"*"+qcdMuIsoRatioTauNonIso_;//apply  ratio here 
  cout<<"Applying Tau-Mu Iso QCD ratio : "<<qcdTauIsoRatio_+"*"+qcdMuIsoRatio_<<endl;
  extrasel_ = extrasel_+"*"+qcdTauIsoRatio_+"*"+qcdMuIsoRatio_;//apply  ratio here 
  if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
  if(!correctSamplesInc())return 0;
  TH1F* hDataD=getTotalData();  if(!hDataD){cout<<" Total Data not determined "<<endl; return 0;}   hDataD->SetName("hDataD");
  TH1F* hMCD=getTotalMCSM();   if(!hMCD){cout<<" Total Data not determined "<<endl; return 0;}   hMCD->SetName("hMCD");
  extrasel_ = tmpextrasel;
  hD->Add(hDataD,1.);
  //hD->Add(hMCD,-1.*MCCorrD);
  hD->Add(hMCD,-1.);
  cout<<"QCD Region D "<<hDataD->Integral()<<" - "<<hMCD->Integral()<<endl;
  hDataD->Write();delete hDataD;
  hMCD->Write();delete hMCD;

  //////Determine yield for each region with its error
  Float_t QCDYieldB=hB->Integral();
  Float_t QCDerrB=0.;
  Float_t QCDYieldC=hC->Integral();
  Float_t QCDerrC=0.;
  Float_t QCDYieldD=hD->Integral();
  Float_t QCDerrD=0.;
  for(Int_t b=1;b<=nbins_;b++){
    QCDerrB+=hB->GetBinError(b)*hB->GetBinError(b);
    QCDerrC+=hC->GetBinError(b)*hC->GetBinError(b);
    QCDerrD+=hD->GetBinError(b)*hD->GetBinError(b);
  }

  cout<<"summary of QCD estimates:"<<endl;
  cout<<"QCD Region B "<<QCDYieldB<<" +- "<<sqrt(QCDerrB)<<endl;
  cout<<"QCD Region C "<<QCDYieldC<<" +- "<<sqrt(QCDerrC)<<endl;
  cout<<"QCD Region D "<<QCDYieldD<<" +- "<<sqrt(QCDerrD)<<endl;

  //for the average make an error weighted average
  Float_t wsum=(1./QCDerrB+1./QCDerrC+1./QCDerrD);
  Float_t QCDYield=(QCDYieldB/QCDerrB+QCDYieldC/QCDerrC+QCDYieldD/QCDerrD)/wsum;
  //Float_t QCDerr=(1./sqrt(QCDerrB)+1./sqrt(QCDerrC)+1./sqrt(QCDerrD))/(1./QCDerrB+1./QCDerrC+1./QCDerrD);
  Float_t QCDerr=sqrt((QCDerrB*pow((1./QCDerrB)/wsum,2)+QCDerrC*pow((1./QCDerrC)/wsum,2)+QCDerrD*pow((1./QCDerrD)/wsum,2)));
  
  //find the maximum deviation from the mean for systematic estimate:
  float SystErr=0.;
  if(fabs(QCDYield-QCDYieldB)>SystErr)SystErr=fabs(QCDYield-QCDYieldB);
  if(fabs(QCDYield-QCDYieldC)>SystErr)SystErr=fabs(QCDYield-QCDYieldC);
  if(fabs(QCDYield-QCDYieldD)>SystErr)SystErr=fabs(QCDYield-QCDYieldD);
  cout<<"QCDIso: "<<QCDYield<<" +- "<<QCDerr<<" +- "<<SystErr<<endl;



  //Shape is taken from D region which has best purity otherwise the other regions pollute the shape
  h->Add(hD,QCDYield/hD->Integral());
  //fix any negative bins
  for(Int_t b=1;b<=nbins_;b++) 
    if(h->GetBinContent(b)<0.){
      h->SetBinContent(b,0); h->SetBinError(b,0);
    }
  h->Write();




  FQCDIsoSM.ls();
  FQCDIsoSM.Close();

  /////////return to normal
  Isocat_=TmpIsocat;
  extrasel_=tmpextrasel;
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;

  return h;

}



TH1F* TauMuPlotter::getQCDIsoSMSS(){


  TH1F* h=new TH1F("hQCD","SMQCD",nbins_,xmin_,xmax_);
  h->Sumw2();
  h->GetXaxis()->SetTitle(plotvar_);
  
  Int_t TmpIsocat=Isocat_;  
  Isocat_=-1;  
  TString tmpextrasel=extrasel_;
  

  TFile FQCDIsoSM(outputpath_+"/FQCDIsoSMSS_"+plotvar_+"_TauIso"+(long)Isocat_+"_MT"+(long)MTcat_+"_SM"+(long)SMcat_+".root","recreate");
  //Save the intermediate histograms in a root file


  ///Now calculate the QCD in the low mT region (while applying the above corrections to the MC)
  //QCD From Region B
  TH1F* hB=new TH1F("hSMQCDB","SMQCDB",nbins_,xmin_,xmax_);
  hB->Sumw2();
  extrasel_ += "*(muiso<0.1&&tauisodisc<2)";
  cout<<"Applying Tau Iso QCD ratio : "<<qcdTauIsoRatio_<<endl;
  extrasel_ = extrasel_+"*"+qcdTauIsoRatio_;//apply  ratio here 
  if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
  if(!correctSamplesInc())return 0;
  TH1F* hDataB=getTotalDataSS();  if(!hDataB){cout<<" Total Data not determined "<<endl; return 0;}   hDataB->SetName("hDataB");
  TH1F* hMCB=getTotalMCSMSS();   if(!hMCB){cout<<" Total Data not determined "<<endl; return 0;}   hMCB->SetName("hMCB");
  extrasel_ = tmpextrasel;
  hB->Add(hDataB,1.);
  //hB->Add(hMCB,-1.*MCCorrB);
  hB->Add(hMCB,-1.);
  cout<<"QCD Region B "<<hDataB->Integral()<<" - "<<hMCB->Integral()<<endl;
  hDataB->Write(); delete hDataB;
  hMCB->Write(); delete hMCB;

  //QCD From Region C
  TH1F* hC=new TH1F("hSMQCC","SMQCC",nbins_,xmin_,xmax_);
  hC->Sumw2();
  extrasel_ += "*(muiso>=0.1&&tauisodisc>=2)";
  cout<<"Applying Mu Iso QCD ratio : "<<qcdMuIsoRatio_<<endl;
  extrasel_ = extrasel_+"*"+qcdMuIsoRatio_;//apply  ratio here 
  if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
  if(!correctSamplesInc())return 0;
  TH1F* hDataC=getTotalDataSS();  if(!hDataC){cout<<" Total Data not determined "<<endl; return 0;}   hDataC->SetName("hDataC");
  TH1F* hMCC=getTotalMCSMSS();   if(!hMCC){cout<<" Total Data not determined "<<endl; return 0;}   hMCC->SetName("hMCC");
  extrasel_ = tmpextrasel;
  hC->Add(hDataC,1.);
  //hC->Add(hMCC,-1.*MCCorrC);
  hC->Add(hMCC,-1.);
  Float_t Cerr=0.;
  for(Int_t b=1;b<=nbins_;b++) Cerr+=hC->GetBinError(b)*hC->GetBinError(b);
  cout<<"QCD Region C "<<hDataC->Integral()<<" - "<<hMCC->Integral()<<endl;
  hDataC->Write();delete hDataC;
  hMCC->Write();delete hMCC;


  //QCD From Region D
  TH1F* hD=new TH1F("hSMQCD","SMQCD",nbins_,xmin_,xmax_);
  hD->Sumw2();
  extrasel_ += "*(muiso>0.1&&tauisodisc<2)";
  //cout<<"Applying Tau-Mu Iso QCD ratio : "<<qcdTauIsoRatioMuNonIso_+"*"+qcdMuIsoRatioTauNonIso_<<endl;
  //extrasel_ = extrasel_+"*"+qcdTauIsoRatioMuNonIso_+"*"+qcdMuIsoRatioTauNonIso_;//apply  ratio here 
  cout<<"Applying Tau-Mu Iso QCD ratio : "<<qcdTauIsoRatio_+"*"+qcdMuIsoRatio_<<endl;
  extrasel_ = extrasel_+"*"+qcdTauIsoRatio_+"*"+qcdMuIsoRatio_;//apply  ratio here 
  if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
  if(!correctSamplesInc())return 0;
  TH1F* hDataD=getTotalDataSS();  if(!hDataD){cout<<" Total Data not determined "<<endl; return 0;}   hDataD->SetName("hDataD");
  TH1F* hMCD=getTotalMCSMSS();   if(!hMCD){cout<<" Total Data not determined "<<endl; return 0;}   hMCD->SetName("hMCD");
  extrasel_ = tmpextrasel;
  hD->Add(hDataD,1.);
  //hD->Add(hMCD,-1.*MCCorrD);
  hD->Add(hMCD,-1.);
  cout<<"QCD Region D "<<hDataD->Integral()<<" - "<<hMCD->Integral()<<endl;
  hDataD->Write();delete hDataD;
  hMCD->Write();delete hMCD;

  //////Determine yield for each region with its error
  Float_t QCDYieldB=hB->Integral();
  Float_t QCDerrB=0.;
  Float_t QCDYieldC=hC->Integral();
  Float_t QCDerrC=0.;
  Float_t QCDYieldD=hD->Integral();
  Float_t QCDerrD=0.;
  for(Int_t b=1;b<=nbins_;b++){
    QCDerrB+=hB->GetBinError(b)*hB->GetBinError(b);
    QCDerrC+=hC->GetBinError(b)*hC->GetBinError(b);
    QCDerrD+=hD->GetBinError(b)*hD->GetBinError(b);
  }

  cout<<"summary of QCD estimates:"<<endl;
  cout<<"QCD Region B "<<QCDYieldB<<" +- "<<sqrt(QCDerrB)<<endl;
  cout<<"QCD Region C "<<QCDYieldC<<" +- "<<sqrt(QCDerrC)<<endl;
  cout<<"QCD Region D "<<QCDYieldD<<" +- "<<sqrt(QCDerrD)<<endl;

  //for the average make an error weighted average
  Float_t wsum=(1./QCDerrB+1./QCDerrC+1./QCDerrD);
  Float_t QCDYield=(QCDYieldB/QCDerrB+QCDYieldC/QCDerrC+QCDYieldD/QCDerrD)/wsum;
  //Float_t QCDerr=(1./sqrt(QCDerrB)+1./sqrt(QCDerrC)+1./sqrt(QCDerrD))/(1./QCDerrB+1./QCDerrC+1./QCDerrD);
  Float_t QCDerr=sqrt((QCDerrB*pow((1./QCDerrB)/wsum,2)+QCDerrC*pow((1./QCDerrC)/wsum,2)+QCDerrD*pow((1./QCDerrD)/wsum,2)));
  
  //find the maximum deviation from the mean for systematic estimate:
  float SystErr=0.;
  if(fabs(QCDYield-QCDYieldB)>SystErr)SystErr=fabs(QCDYield-QCDYieldB);
  if(fabs(QCDYield-QCDYieldC)>SystErr)SystErr=fabs(QCDYield-QCDYieldC);
  if(fabs(QCDYield-QCDYieldD)>SystErr)SystErr=fabs(QCDYield-QCDYieldD);
  cout<<"QCDIso: "<<QCDYield<<" +- "<<QCDerr<<" +- "<<SystErr<<endl;



  //Shape is taken from D region which has best purity otherwise the other regions pollute the shape
  h->Add(hD,QCDYield/hD->Integral());
  //fix any negative bins
  for(Int_t b=1;b<=nbins_;b++) 
    if(h->GetBinContent(b)<0.){
      h->SetBinContent(b,0); h->SetBinError(b,0);
    }
  h->Write();




  FQCDIsoSM.ls();
  FQCDIsoSM.Close();

  /////////return to normal
  Isocat_=TmpIsocat;
  extrasel_=tmpextrasel;
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;

  return h;


}




TH1F* TauMuPlotter::getWJetsTauIsoSM(){
  
  //1.normalize WJets with Anti-Iso sample at high MT
  //2.apply ratio to predict the yield in the Iso sample

  TString tmpextrasel=extrasel_;
  Int_t TmpIsocat=Isocat_;  

  //switch temporarily while keeping all other categories the same  
  Isocat_=2;
  extrasel_ += "*(muiso<0.1)";//
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;//fix the WJets at high mT

  /////////////////////
  cout<<"Applying Tau Iso WJets ratio : "<<wjetsTauIsoRatio_<<endl;
  extrasel_ += TString("*")+wjetsTauIsoRatio_;//
  TH1F* h=getWJetsInc();
  /////////////////////

  extrasel_=tmpextrasel;
  Isocat_=TmpIsocat;//return to normal
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;

  float yielder=0.;
  for(Int_t b=1;b<=h->GetNbinsX();b++){
    yielder+=h->GetBinError(b)*h->GetBinError(b);
  }

  cout<<"WJetsTauIso : "<<h->Integral()<<" +- "<<sqrt(yielder)<<endl;
  return h;

}

TH1F* TauMuPlotter::getWJetsTauIsoSMSS(){
  TString tmpextrasel=extrasel_;
  Int_t TmpIsocat=Isocat_;  

  //switch temporarily while keeping all other categories the same  
  Isocat_=2;
  extrasel_ += "*(muiso<0.1)";//
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;//fix the WJets at high mT

  /////////////////////
  cout<<"Applying Tau Iso WJets ratio : "<<wjetsTauIsoRatioSS_<<endl;
  extrasel_ += TString("*")+wjetsTauIsoRatioSS_;//
  TH1F* h=getWJetsIncSS();
  /////////////////////

  extrasel_=tmpextrasel;
  Isocat_=TmpIsocat;//return to normal
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;

  float yielder=0.;
  for(Int_t b=1;b<=h->GetNbinsX();b++){
    yielder+=h->GetBinError(b)*h->GetBinError(b);
  }

  cout<<"WJetsTauIso : "<<h->Integral()<<" +- "<<sqrt(yielder)<<endl;

  return h;
}


bool TauMuPlotter::plotSM(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel, TString xlabel, TString ylabel, Float_t* legendcoords, bool log){
 
 
  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  Isocat_=Isocat;
  MTcat_=MTcat;
  SMcat_=SMcat;
  if(extrasel.CompareTo("")!=0) extrasel_ += TString("*")+extrasel;
  
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;


  TString plotvarname=plotvar_+extrasel_;
  fixFileTag(&plotvarname);


  TString filename=outputpath_+"/TauMuPlotterSM_"+plotvarname;
  if(Isocat_>0) filename=filename+"_Iso"+(long)Isocat_;
  if(MTcat_>0) filename=filename+"_MT"+(long)MTcat_;
  if(SMcat_>=0) filename=filename+"_SM"+(long)SMcat_;
  filename+=".ps";
  TCanvas C("C",filename);  
  C.Print(filename+"[");
   


  //Fakes
  TH1F*hQCD=0;
  //if(Isocat_==1){
  //hQCD=getQCDInc();//use just for speed
  //hQCD=getQCDMike();
  //hQCD=getQCDTauIsoSM();
  hQCD=getQCDIsoSM();
  if(!hQCD)return 0;
  if(smearHistoRes_>0) hQCD=smearHisto(hQCD);
  hQCD->SetLineWidth(1);
  hQCD->SetLineColor(QCDColor_);
  hQCD->SetFillColor(QCDColor_);
  //}


 
  ///Z->tau tau
  TH1F*hZToTauTau=getZToTauTau();
  if(!hZToTauTau)return 0;
  if(smearHistoRes_>0) hZToTauTau=smearHisto(hZToTauTau);
  hZToTauTau->SetLineWidth(1);
  hZToTauTau->SetLineColor(ZTauTauColor_);
  hZToTauTau->SetFillColor(ZTauTauColor_);

  
  //
  TH1F*hTTJets=getSample("TTJets");
  if(!hTTJets)return 0;
  if(smearHistoRes_>0) hTTJets=smearHisto(hTTJets);
  hTTJets->SetLineWidth(1);
  hTTJets->SetLineColor(TTJetsColor_);
  hTTJets->SetFillColor(TTJetsColor_);

  
  ///Note that this should be in sync with QCD determination from TauIso
  TH1F*hWJetsToLNu=0;
  //hWJetsToLNu=getWJetsInc();
  //hWJetsToLNu=getWJetsSM();
  hWJetsToLNu=getWJetsTauIsoSM();
  if(!hWJetsToLNu)return 0;
  hWJetsToLNu->SetLineWidth(1);
  hWJetsToLNu->SetLineColor(WJetsColor_);
  hWJetsToLNu->SetFillColor(WJetsColor_);


  TH1F*hEWK=0;
  hEWK=(TH1F*)hWJetsToLNu->Clone("hElectroWeak");
  delete hWJetsToLNu;
  hEWK->SetLineWidth(1);
  hEWK->SetLineColor(WJetsColor_);
  hEWK->SetFillColor(WJetsColor_);


  //combine Di-Bosons
  TH1F*hVV=getDiBoson();
  if(!hVV)return 0;
  hTTJets->Add(hVV);
  delete hVV;

  //combine ZLJet
  TH1F*hZToLJet=getSample("ZToLJet");
  if(!hZToLJet)return 0;
  hTTJets->Add(hZToLJet);
  delete hZToLJet;  

  //combine Z->MuMu
  TH1F*hZToMuMu=getSample("ZToMuMu");
  if(!hZToMuMu)return 0;
  hZToMuMu->SetLineWidth(1);
  hZToMuMu->SetLineColor(ZMuMuColor_);
  hZToMuMu->SetFillColor(ZMuMuColor_);
  hTTJets->Add(hZToMuMu);
  delete hZToMuMu;


  ////Total Data
  TH1F* hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}


  ////////////////////////
  cout<<"Creating Bkg Stack:"<<endl;
  THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
  if(hQCD)hMCStack.Add(hQCD,"hist");
  if(hEWK)hMCStack.Add(hEWK,"hist");
  hMCStack.Add(hTTJets,"hist");
  hMCStack.Add(hZToTauTau,"hist");

  //create legend key
  TLegend legend;
  legend.SetLineColor(0);
  legend.SetBorderSize(1);
  legend.AddEntry(hData,hData->GetTitle(),"p");
  legend.AddEntry(hZToTauTau,"Z#rightarrow#tau^{+}#tau^{-}","f");
  legend.AddEntry(hTTJets,"other","f");
  if(hEWK)legend.AddEntry(hEWK,"W+jets","f");
  if(hQCD)legend.AddEntry(hQCD,"QCD","f");


  //create total Background
  TH1F* hBkg=new TH1F("hBkg","Bkg",nbins_,xmin_,xmax_);
  TList* histList= hMCStack.GetHists();
  for(Int_t i=0;i<histList->GetSize();i++){
    hBkg->Add((TH1F*)(histList->At(i)));
  }


  cout<<" Summary of yields : "<<endl;
  if(hQCD) cout<<"QCD "<<hQCD->Integral()<<endl;
  if(hEWK) cout<<"W "<<hEWK->Integral()<<endl;
  cout<<"TTJets "<<hTTJets->Integral()<<endl;
  cout<<"ZToTauTau "<<hZToTauTau->Integral()<<endl;
  cout<<"Total Background "<<(int)(hBkg->Integral())<<endl;
  cout<<"Total Data "<<(int)(hData->Integral())<<endl;



  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  hData->GetYaxis()->SetRangeUser(log ? 1 : 0, hData->GetMaximum()*(log ? 10 : 1.3));
  if(log) C.SetLogy(1);
  hData->SetStats(0);
  hData->GetXaxis()->SetTitle(xlabel);
  hData->GetYaxis()->SetTitle(ylabel);
  hData->Draw("hist pe");
  //////////////////////////
  hMCStack.Draw("hist same");  
  hData->Draw("hist pe same");//bring Data points back to front
  hBkg->SetFillColor(0);
  hBkg->SetLineColor(1);
  hBkg->Draw("histsame");//draw outline of MC        
  hMCStack.GetHistogram()->Draw("same");//bring axes back to front 
  if(legendcoords){
    legend.SetX1NDC(legendcoords[0]);
    legend.SetX2NDC(legendcoords[1]);
    legend.SetY1NDC(legendcoords[2]);
    legend.SetY2NDC(legendcoords[3]);
  }
  legend.SetTextSize(.04);
  legend.Draw();
  C.Update();
  C.Print(filename);  
  if(log) C.SetLogy(0);


  TFile DiffFile(outputpath_+"/DiffFileSM_"+plotvarname+"_TauIso"+(long)Isocat_+"_MT"+(long)MTcat_+"_SM"+(long)SMcat_+".root","recreate");
  
  TLine line;

  hData->SetTitle("hData");  hData->Write();
  hBkg->SetTitle("hBkg");  hBkg->Write();


  //make the difference plot  
  TH1F* hDiff=new TH1F("hDiff","Difference",nbins_,xmin_,xmax_);
  hDiff->GetXaxis()->SetTitle(plotvar_);
  hDiff->Add(hData);
  hDiff->Add(hBkg,-1);
  C.Clear();
  hDiff->Draw("hist pe");
  line.DrawLine(xmin_,0,xmax_,0);
  C.Print(filename);  
  hDiff->Write();
  //make the ratio plot
  TH1F* hRatio=new TH1F("hRatio","Ratio",nbins_,xmin_,xmax_);
  hRatio->GetXaxis()->SetTitle(plotvar_);
  hRatio->Add(hData);
  hRatio->Divide(hBkg);
  C.Clear();
  hRatio->Draw("hist pe");
  line.DrawLine(xmin_,1,xmax_,1);
  C.Print(filename);  
  hRatio->Write();


  DiffFile.ls();
  DiffFile.Close();
    

  
  C.Print(filename+"]");
  cout<<"closed ps file"<<endl;

  return 1;

}



bool TauMuPlotter::plotSMSS(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel, TString xlabel, TString ylabel, Float_t* legendcoords, bool log){
   

  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  Isocat_=Isocat;
  MTcat_=MTcat;
  SMcat_=SMcat;
  if(extrasel.CompareTo("")!=0) extrasel_ += TString("*")+extrasel;

  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;

  TString plotvarname=plotvar_+extrasel_;  
  fixFileTag(&plotvarname);

  TString filename=outputpath_+"/TauMuPlotterSMSS_"+plotvarname;
  if(Isocat_>0) filename=filename+"_Iso"+(long)Isocat_;
  if(MTcat_>0) filename=filename+"_MT"+(long)MTcat_;
  if(SMcat_>=0) filename=filename+"_SM"+(long)SMcat_;
  filename+=".ps";



  TCanvas C("C",filename);  
  C.Print(filename+"[");
   


  //Fakes
  TH1F*hQCD=0;
  if(Isocat_==1){
    //hQCD=getQCDTauIsoSMSS();
    hQCD=getQCDIsoSMSS();
    if(!hQCD)return 0;
    hQCD->SetLineWidth(1);
    hQCD->SetLineColor(QCDColor_);
    hQCD->SetFillColor(QCDColor_);
  }

 
  ///Z->tau tau
  TH1F*hZToTauTau=getZToTauTauSS();
  if(!hZToTauTau)return 0;
  hZToTauTau->SetLineWidth(1);
  hZToTauTau->SetLineColor(ZTauTauColor_);
  hZToTauTau->SetFillColor(ZTauTauColor_);

  //
  TH1F*hTTJets=getSampleSS("TTJets");
  if(!hTTJets)return 0;
  hTTJets->SetLineWidth(1);
  hTTJets->SetLineColor(TTJetsColor_);
  hTTJets->SetFillColor(TTJetsColor_);
     
  TH1F*hWJetsToLNu=getWJetsSMSS();///Note that this should be in sync with QCD determination from TauIso
  if(!hWJetsToLNu)return 0;
  hWJetsToLNu->SetLineWidth(1);
  hWJetsToLNu->SetLineColor(WJetsColor_);
  hWJetsToLNu->SetFillColor(WJetsColor_);
  TH1F*hEWK=0;
  hEWK=(TH1F*)hWJetsToLNu->Clone("hElectroWeak");
  delete hWJetsToLNu;

  //combine Di-Bosons
  TH1F*hVV=getDiBosonSS();
  if(!hVV)return 0;
  hTTJets->Add(hVV);
  delete hVV;

  //combine ZLJet
  TH1F*hZToLJet=getSampleSS("ZToLJet");
  if(!hZToLJet)return 0;
  hTTJets->Add(hZToLJet);
  delete hZToLJet;  

  //combine Z->MuMu
  TH1F*hZToMuMu=getSampleSS("ZToMuMu");
  if(!hZToMuMu)return 0;
  hZToMuMu->SetLineWidth(1);
  hZToMuMu->SetLineColor(ZMuMuColor_);
  hZToMuMu->SetFillColor(ZMuMuColor_);
  hTTJets->Add(hZToMuMu);
  delete hZToMuMu;

  
  ////Total Data
  TH1F* hData=getTotalDataSS();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}


  ////////////////////////
  cout<<"Creating Bkg Stack:"<<endl;
  THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
  if(hQCD)hMCStack.Add(hQCD,"hist");
  if(hEWK)hMCStack.Add(hEWK,"hist");
  hMCStack.Add(hTTJets,"hist");
  hMCStack.Add(hZToTauTau,"hist");

  //create legend key
  TLegend legend;
  legend.SetLineColor(0);
  legend.SetBorderSize(1);
  legend.AddEntry(hData,hData->GetTitle(),"p");
  legend.AddEntry(hZToTauTau,"Z#rightarrow#tau^{+}#tau^{-}","f");
  legend.AddEntry(hTTJets,"other","f");
  if(hEWK)legend.AddEntry(hEWK,"W+jets","f");
  if(hQCD)legend.AddEntry(hQCD,"QCD","f");


  //create total Background
  TH1F* hBkg=new TH1F("hBkg","Bkg",nbins_,xmin_,xmax_);
  TList* histList= hMCStack.GetHists();
  for(Int_t i=0;i<histList->GetSize();i++){
    hBkg->Add((TH1F*)(histList->At(i)));
  }


  cout<<" Summary of yields : "<<endl;
  if(hQCD) cout<<"QCD "<<hQCD->Integral(0,hQCD->GetNbinsX()+1)<<endl;
  if(hEWK) cout<<"W "<<hEWK->Integral(0,hEWK->GetNbinsX()+1)<<endl;
  cout<<"TTJets "<<hTTJets->Integral(0,hTTJets->GetNbinsX()+1)<<endl;
  cout<<"ZToTauTau "<<hZToTauTau->Integral(0,hZToTauTau->GetNbinsX()+1)<<endl;
  cout<<"Total Background "<<(int)(hBkg->Integral(0,hBkg->GetNbinsX()+1))<<endl;
  cout<<"Total Data "<<(int)(hData->Integral(0,hData->GetNbinsX()+1))<<endl;



  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  hData->GetYaxis()->SetRangeUser(log ? 1 : 0, hData->GetMaximum()*(log ? 10 : 1.3));
  if(log) C.SetLogy(1);
  hData->SetStats(0);
  hData->GetXaxis()->SetTitle(xlabel);
  hData->GetYaxis()->SetTitle(ylabel);
  hData->Draw("hist pe");
  //////////////////////////
  hMCStack.Draw("hist same");  
  hData->Draw("hist pe same");//bring Data points back to front
  hBkg->SetFillColor(0);
  hBkg->SetLineColor(1);
  hBkg->Draw("histsame");//draw outline of MC        
  hMCStack.GetHistogram()->Draw("same");//bring axes back to front 
  if(legendcoords){
    legend.SetX1NDC(legendcoords[0]);
    legend.SetX2NDC(legendcoords[1]);
    legend.SetY1NDC(legendcoords[2]);
    legend.SetY2NDC(legendcoords[3]);
  }
  legend.SetTextSize(.04);
  legend.Draw();
  C.Update();
  C.Print(filename);  
  if(log) C.SetLogy(0);


  TH1F* hDiff=new TH1F("hDiff","Difference",nbins_,xmin_,xmax_);

  TFile DiffFile(outputpath_+"/DiffFileSMSS_"+plotvarname+"_TauIso"+(long)Isocat_+"_MT"+(long)MTcat+"_SM"+(long)SMcat_+".root","recreate");
  //make the difference plot  
  TLine line;

  hDiff->GetXaxis()->SetTitle(plotvar_);
  hDiff->Add(hData);
  hDiff->Add(hBkg,-1);
  C.Clear();
  hDiff->Draw("hist pe");
  line.DrawLine(xmin_,0,xmax_,0);
  C.Print(filename);  
  hDiff->Write();

  //make the ratio plot
  TH1F* hRatio=new TH1F("hRatio","Ratio",nbins_,xmin_,xmax_);
  hRatio->GetXaxis()->SetTitle(plotvar_);
  hRatio->Add(hData);
  hRatio->Divide(hBkg);
  C.Clear();
  hRatio->Draw("hist pe");
  line.DrawLine(xmin_,1,xmax_,1);
  C.Print(filename);  
  hRatio->Write();

  DiffFile.ls();
  DiffFile.Close();
    

  
  C.Print(filename+"]");
  cout<<"closed ps file"<<endl;

  return 1;

}


void TauMuPlotter::plotIsoFakeRate(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel, TString IsoSel, Float_t ymax,Bool_t log){
  
  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  MTcat_=1;//QCD is enhanced in the signal region
  SMcat_=-1;  
  Isocat_=-1;
  if(extrasel!="")extrasel_=extrasel_+"*"+extrasel;//something like (njet==0)

  TString filetag=plotvar_+TString("MTcat")+(long)MTcat_+"_SMcat"+(long)SMcat_+extrasel_;
  fixFileTag(&filetag);

 
  TCanvas C("C");
  TString filename = outputpath_ + "/TauMuPlotterIsoFakeRate_"+filetag;
  C.Print(filename+".ps[");


  /////////Anti-Isolated taus
  TString tmpextrasel=extrasel_;
  extrasel_=extrasel_+"*(!"+IsoSel+")";
  if(!scaleSamplesLumi())return;
  if(!correctSamplesInc())return;
  TH1F* hData=getTotalDataSS();
  if(!hData){cout<<" Total Data not determined "<<endl; return;}
  TH1F* hMC=getTotalBackgroundIncSS();
  if(!hMC){cout<<" Total MC not determined "<<endl; return;}  
  extrasel_=tmpextrasel;

  C.Clear();
  hData->Draw("pe");
  hMC->Draw("hist same");
  C.Print(filename+".ps");

  TH1F* hA=new TH1F("hDiffAnti","",nbins_,xmin_,xmax_);
  hA->Add(hData);
  hA->Add(hMC,-1);
  delete hData; delete hMC;


  /////////////Isolated taus
  extrasel_=extrasel_+"*"+IsoSel;
  if(!scaleSamplesLumi())return;
  if(!correctSamplesInc())return;
  hData=getTotalDataSS();
  if(!hData){cout<<" Total Data not determined "<<endl; return;}
  hMC=getTotalBackgroundIncSS();
  if(!hMC){cout<<" Total MC not determined "<<endl; return;}

  C.Clear();
  hData->Draw("pe");
  hMC->Draw("hist same");
  C.Print(filename+".ps");

  TH1F* hI=new TH1F("hDiffIso","",nbins_,xmin_,xmax_);
  hI->Add(hData);
  hI->Add(hMC,-1);
  delete hData; delete hMC;
 
  C.Clear();
  C.SetLogy(1);
  hA->Draw("pe");
  hI->Draw("pesame");
  C.Print(filename+".ps");
  C.SetLogy(0);

  //average ratio
  float r=hI->Integral()/hA->Integral();
  float re=r*sqrt(1./hI->Integral() + 1./hA->Integral());
  cout<<" Ratio = "<<r<<" +- "<<re<<endl;


  ////Calculate the ratio as a function of the variable
  TH1F* hR=new TH1F("hRatio","",nbins_,xmin_,xmax_);
  hR->Add(hI);
  hR->Divide(hA);
  delete hA; delete hI;

  C.Clear();
  C.SetLogy(log);
  delete hR->FindObject("stats");
  hR->GetYaxis()->SetRangeUser(log ? 0.001 : 0,ymax);
  hR->GetXaxis()->SetTitle(plotvar_);
  hR->GetYaxis()->SetTitle("ratio");
  hR->SetTitle("");
  hR->Draw("pe");
  C.Print(filename+".ps");


  cout.precision(3);
  cout<<"(";
  Int_t b=1;
  while(hR->GetBinContent(b)>0&&b<=hR->GetNbinsX()){
    cout<<"("<<hR->GetBinLowEdge(b)<<"<"<<plotvar_<<"&&"<<plotvar_<<"<="<<hR->GetBinLowEdge(b)+hR->GetBinWidth(b)<<")*";
    cout<<hR->GetBinContent(b)<<"+";
    b++;
  }
  cout<<")"<<endl;  


  ///save the ratio histo to a root file:
  TFile FRatio(filename+".root","recreate");
  hR->Write();
  FRatio.ls();
  FRatio.Close();
  

  delete hR;

  C.Print(filename+".ps]");

}


void TauMuPlotter::plotTauFakeRateWJets(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel, Float_t ymax,Bool_t log){
  
  //Ratio of Isolated/Anti-Isolated taus using the the high mT region
  //-Should be computed at inclusive level
  //-Should be computed with OS samples only (SS is different)

  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
 
  MTcat_=3;
  SMcat_=-1;  
  extrasel_+="*(muiso<0.1)";  //Ratio is computed only for IsoTaus / Anti-Iso taus , there is no gain from anti-iso muons
  if(extrasel!="")extrasel_=extrasel_+"*"+extrasel;

  TString filetag=plotvar_+TString("MTcat")+(long)MTcat_+"_SMcat"+(long)SMcat_+extrasel_;
  fixFileTag(&filetag);

 
  TCanvas C("C");
  TString filename = outputpath_ + "/TauMuPlotterTauFakeRateWJets_"+filetag;
  C.Print(filename+".ps[");


  /////////Anti-Isolated taus
  Isocat_=2;
  if(!scaleSamplesLumi())return;
  if(!correctSamplesInc())return;

  TH1F* hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return;}

  TH1F* hMC=new TH1F("hMCAnti","",nbins_,xmin_,xmax_);
  TH1F* hZ=getZToTauTau(); hMC->Add(hZ); delete hZ;
  TH1F* hTT=getSample("TTJets"); hMC->Add(hTT); delete hTT;
 
  C.Clear();
  hData->Draw("pe");
  hMC->Draw("hist same");
  C.Print(filename+".ps");

  TH1F* hA=new TH1F("hDiffAnti","",nbins_,xmin_,xmax_);
  hA->Add(hData); delete hData; 
  hA->Add(hMC,-1); delete hMC;

  C.Clear();
  hA->Draw("pe");
  C.Print(filename+".ps");


  /////////////Isolated taus
  Isocat_=1;
  if(!scaleSamplesLumi())return;
  if(!correctSamplesInc())return;

  hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return;}

  hMC=new TH1F("hMCIso","",nbins_,xmin_,xmax_);
  hZ=getZToTauTau(); hMC->Add(hZ); delete hZ;
  hTT=getSample("TTJets"); hMC->Add(hTT); delete hTT;


  C.Clear();
  hData->Draw("pe");
  hMC->Draw("hist same");
  C.Print(filename+".ps");

  TH1F* hI=new TH1F("hDiffIso","",nbins_,xmin_,xmax_);
  hI->Add(hData);delete hData;
  hI->Add(hMC,-1);delete hMC;
  
  C.Clear();
  hI->Draw("pe");
  C.Print(filename+".ps");

  //average ratio
  float r=hI->Integral()/hA->Integral();
  float re=r*sqrt(1./hI->Integral() + 1./hA->Integral());
  cout<<" Ratio = "<<r<<" +- "<<re<<endl;


  ////Calculate the ratio as a function of the variable
  TH1F* hR=new TH1F("hRatio","",nbins_,xmin_,xmax_);
  hR->Add(hI); 
  hR->Divide(hA); 

  C.Clear();
  C.SetLogy(log);
  delete hR->FindObject("stats");
  hR->GetYaxis()->SetRangeUser(log ? 0.001 : 0,ymax);
  hR->GetXaxis()->SetTitle(plotvar_);
  hR->GetYaxis()->SetTitle("ratio");
  hR->SetTitle("");
  hR->Draw("pe");
  C.Print(filename+".ps");


  cout.precision(3);
  cout<<"(";
  Int_t b=1;
  while(hR->GetBinContent(b)>0&&b<=hR->GetNbinsX()){
    cout<<"("<<hR->GetBinLowEdge(b)<<"<"<<plotvar_<<"&&"<<plotvar_<<"<="<<hR->GetBinLowEdge(b)+hR->GetBinWidth(b)<<")*"<<hR->GetBinContent(b)<<"+";
    b++;
  }
  cout<<")"<<endl;  



  ///save the ratio histo to a root file:
  TFile FRatio(filename+".root","recreate");
  hI->Write();
  hA->Write();
  hR->Write();
  FRatio.ls();
  FRatio.Close();
  
  delete hI;
  delete hA; 
  delete hR;

  C.Print(filename+".ps]");

}



bool TauMuPlotter::printRawYields(TString selection){
  
  Float_t totalData=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,selection.Data()));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      totalData+=hmass->Integral(0,hmass->GetNbinsX()+1);
      delete hmass;
    }
  }
  cout<<"Total Data  = "<<(int)(totalData)<<endl;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,selection.Data()));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      delete hmass;
    }
  } 

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MCCat"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,selection.Data()));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      delete hmass;
    }
  }

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Embedded"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,selection.Data()));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      delete hmass;
    }
  }  

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Signal"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,selection.Data()));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      delete hmass;
    }
  }
  

  return 1;
}





TH1F* TauMuPlotter::smearHisto(TH1F* h){
  if(!h){
    cout<<"BaseAnalysis::smearHisto : histo is NULL"<<endl;
    return 0;
  }

 
  TH1F* hs=0;
 
  if(smearHistoRes_==0.){//just copy
    hs=(TH1F*)h->Clone(TString(h->GetName())+"smeared");
  }else {
    hs=new TH1F(TString(h->GetName())+"smeared",h->GetTitle(),h->GetXaxis()->GetNbins(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
    TF1 gaus("gauss","[0]*exp(-0.5*(x-[1])**2/[2]**2)",h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
    gaus.SetParameter(2,smearHistoRes_);

    for(Int_t b=1;b<=h->GetXaxis()->GetNbins();b++){
      gaus.SetParameter(0,h->GetBinContent(b));
      gaus.SetParameter(1,h->GetBinCenter(b));
      for(Int_t bs=1;bs<=h->GetXaxis()->GetNbins();bs++){
        hs->AddBinContent(bs,gaus.Eval(hs->GetBinCenter(bs)));
      }
    }
    for(Int_t bs=1;bs<=h->GetXaxis()->GetNbins();bs++){
      hs->SetBinError(bs,0.);//not sure this is necessary
    }
    hs->Scale(h->Integral()/hs->Integral());
  }
  
  return hs;
}


void TauMuPlotter::fixFileTag(TString * filetag){

  for(Int_t l=0;l<filetag->Sizeof();l++){//fix the variable name to work with the file name
    if(TString((*filetag)[l])=="(") filetag->Replace(l,1,"_");
    if(TString((*filetag)[l])==")") filetag->Replace(l,1,"_");
    if(TString((*filetag)[l])=="*") filetag->Replace(l,1,"X");
    if(TString((*filetag)[l])=="=") filetag->Replace(l,1,"Q");
    if(TString((*filetag)[l])=="<") filetag->Replace(l,1,"L");
    if(TString((*filetag)[l])==">") filetag->Replace(l,1,"M");
    if(TString((*filetag)[l])=="&") filetag->Replace(l,1,"A");
  }

}



