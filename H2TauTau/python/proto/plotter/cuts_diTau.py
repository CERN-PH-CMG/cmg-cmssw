from CMGTools.H2TauTau.proto.plotter.binning import *
from ROOT import gROOT, gStyle

### style parameters
gROOT.SetStyle("Plain")
gROOT.SetBatch(True)
gStyle.SetLegendFillColor(0)
gStyle.SetLegendBorderSize(0)
gStyle.SetStatBorderSize(0)
gStyle.SetTitleBorderSize(0)
gStyle.SetPadTickX(1)
gStyle.SetPadTickY(1)
gStyle.SetTitleX(0.125)
gStyle.SetTitleY(0.96)
gStyle.SetTitleW(0.8)
gStyle.SetTextFont(42)
gStyle.SetLegendFont(42)
gStyle.SetLabelFont(42, 'xy')
gStyle.SetTitleFont(42, 'xy')

tauScale   = '0.03'
shiftedMet = 'sqrt(pow(mex+'+tauScale+'*l1Px+'+tauScale+'*l2Px,2)+pow(mey+'+tauScale+'*l1Py+'+tauScale+'*l2Py,2))' 

baseline           =  ' l1Pt>35 && l2Pt>35 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 '
baseline           += ' && l2MVAEle>0.5 '
baseline           += ' && muon1Pt==-1 && electron1Pt==-1 '
baseline           += ' && (l1TrigMatched!=0 && l2TrigMatched!=0 && jetTrigMatched!=0) '

baselineSS = baseline.replace('diTauCharge==0','diTauCharge!=0')

isolationLL4       =  ' && l1RawMVAIso>0.5 && l2RawMVAIso>0.5'
isolationML        =  ' && ((l1MedMVAIso>0.5 && l2RawMVAIso>0.795) || (l1RawMVAIso>0.795 && l2MedMVAIso>0.5))'
isolationMM        =  ' && l1MedMVAIso>0.5 && l2MedMVAIso>0.5'
isolationTM        =  ' && ((l1MedMVAIso>0.5 && l2TigMVAIso>0.5) || (l1TigMVAIso>0.5 && l2MedMVAIso>0.5))'
isolationTT        =  ' && l1TigMVAIso>0.5 && l2TigMVAIso>0.5'

Jet0               =  ' && jet1Pt<50.'	
BOOSTED            =  ' && jet1Pt>50.'

VBF                =  ' && jet2Pt>30. && abs(jet2Eta)<4.7 && abs(jet1Eta - jet2Eta)>2.5 && mjj>250 && nCentralJets==0' 
VBFloose           =  ' && jet2Pt>30. && abs(jet2Eta)<4.7 && abs(jet1Eta - jet2Eta)>2.0 && mjj>200 && nCentralJets==0 '
VBFmedium          =  ' && jet2Pt>30. && abs(jet2Eta)<4.7 && abs(jet1Eta - jet2Eta)>3.0 && mjj>550 && nCentralJets==0 '
VBFmediumloose     =  ' && jet2Pt>30. && abs(jet2Eta)<4.7 && abs(jet1Eta - jet2Eta)>2.5 && mjj>400 && nCentralJets==0 '
VBFmediumtight     =  ' && jet2Pt>30. && abs(jet2Eta)<4.7 && abs(jet1Eta - jet2Eta)>3.0 && mjj>400 && nCentralJets==0 '
VBFtight           =  ' && jet2Pt>30. && abs(jet2Eta)<4.7 && abs(jet1Eta - jet2Eta)>4.0 && mjj>400 && nCentralJets==0 '
VBFsupertight      =  ' && jet2Pt>30. && abs(jet2Eta)<4.7 && abs(jet1Eta - jet2Eta)>4.0 && mjj>500 && nCentralJets==0 '
VBFsupertight2     =  ' && jet2Pt>30. && abs(jet2Eta)<4.7 && abs(jet1Eta - jet2Eta)>3.5 && mjj>500 && nCentralJets==0 '

NOVBF              =  ' && ( jet2Pt<30. || abs(jet2Eta)>4.7 || abs(jet1Eta - jet2Eta)<2.5 || mjj<250 || nCentralJets >0)'
NOVBFloose         =  ' && ( jet2Pt<30. || abs(jet2Eta)>4.7 || abs(jet1Eta - jet2Eta)<2.0 || mjj<200 || nCentralJets >0)'
NOVBFmedium        =  ' && ( jet2Pt<30. || abs(jet2Eta)>4.7 || abs(jet1Eta - jet2Eta)<3.0 || mjj<550 || nCentralJets >0)'
NOVBFmediumloose   =  ' && ( jet2Pt<30. || abs(jet2Eta)>4.7 || abs(jet1Eta - jet2Eta)<2.5 || mjj<400 || nCentralJets >0)'
NOVBFmediumtight   =  ' && ( jet2Pt<30. || abs(jet2Eta)>4.7 || abs(jet1Eta - jet2Eta)<3.0 || mjj<400 || nCentralJets >0)'
NOVBFsupertight    =  ' && ( jet2Pt<30. || abs(jet2Eta)>4.7 || abs(jet1Eta - jet2Eta)<4.0 || mjj<400 || nCentralJets >0)'

#VH                 =  ' &&  ((mttj>200 && mttj<500) || (jet2Pt>40. && abs(jet1Eta-jet2Eta)<2. && mjj>60 && mjj<120) )'
VH                 =  ' && jet2Pt>40. && abs(jet1Eta-jet2Eta)<2. && mjj>60 && mjj<120'
NOVH               =  ' && ( jet2Pt<40. || abs(jet1Eta-jet2Eta)>2. || mjj<60 || mjj>120 ) '

myVBF   = '&&   jet2Pt>30. && abs(jet2Eta)<4.7 && abs(jet1Eta - jet2Eta)>2.5 && mjj>600 && nCentralJets==0'
NOmyVBF = '&& ( jet2Pt<30. || abs(jet2Eta)>4.7 || abs(jet1Eta - jet2Eta)<2.5 || mjj>600 || nCentralJets >0 )'

def hists_pref( rebin1, mjjMin1, mjjMax1, mjjBin1, susy ) :
  rebin  = rebin1
  mjjMin = mjjMin1
  mjjMax = mjjMax1
  mjjBin = mjjBin1

  variables = []

  if rebin != 1 :
    binning = binning_svfitMass
    if susy :
      binning = binning_svfitMass_mssm
  else :
    binning = binning_svfitMass_finer
    if susy :
      binning = binning_svfitMass_finer_mssm
  
  variablesDataCard = [ 
    ('svfitMass'  ,len(binning) , binning ),
    #('visMass'    ,len(binning) , binning ),
  ]

  otherVariables1 = [
    #('dRtt'	  ,int(20/rebin), 0      , 5      ),
    #('jet1Pt'     ,int(30/rebin), 0      , 600    ),
    #('nJets'      ,10           , 0      , 10     ),
    ('mjj'        ,20       , 0 , 200 ),
    ('dEtajj'     ,int(18/rebin), -6 , 6     ),
    ('nJets'      ,10           , 0 , 10     ),
    ('dPhittjj'   ,int(16/rebin), 0 , 3.2    ),
    ('dEtattjj'   ,int(20/rebin), 0 , 4    ),
    ('mttj'       ,int(40/rebin), 0 , 1000   ),
    ('pThiggs'    ,int(30/rebin), 0 , 300    ),
    ('jet1Pt'     ,int(30/rebin), 0 , 600    ),
    ('jet2Pt'     ,int(25/rebin), 0 , 500    ),
    ('l1JetInvMass'     ,30           , 0 , 600    ),
    ('l2JetInvMass'     ,30           , 0 , 600    ),
    #('met'        ,int(25/rebin), 0      , 200    ),
    #('nVert'  	  ,int(25/rebin), 0      , 50	  ),
    #('l1Pt'       ,int(60/rebin), 0      , 300    ),    
    #('l2Pt'       ,int(40/rebin), 0      , 200    ),   
  ]

  otherVariables2 = [
    ('tau1Mass'         ,20 , 0 , 2.    ),
    ('tau2Mass'         ,20 , 0 , 2.    ),
    #('svfitMass'        ,int(1000/rebin), 0 , 500    ),
    #('svfitMass'        ,27,massBins),  
    #('svfitMass'        ,massBins, 0 , 300    ), 
    #('svfitMass'        ,int(30/rebin), 0 , 300    ), 
    #('svfitMass'        ,int(20/rebin), 0 , 300    ), 
    #('svfitMass'        ,35 , 0 , 350    ),
    #('svfitMass*1.03'   ,35 , 0 , 350    ),
    #('svfitMass'        ,350, 0 , 350    ),
    #('svfitMass*1.03'   ,350, 0 , 350    ),
    #('svfitMass*0.97'   ,350, 0 , 350    ),
    #('svfitMass'        ,70 , 0 , 350    ), 
    #('svfitMass'        ,46 , 0 , 345    ), 
    #('svfitMass*1.0'    ,35 , 0 , 350    ), 
    #('svfitMass*1.03'   ,70 , 0 , 350    ),
    #('svfitMass*0.97'   ,70 , 0 , 350    ),
    #('svfitMass*1.06'   ,350, 0 , 350    ),
    #('svfitMass*1.06'   ,70 , 0 , 350    ),
    #('visMass'          ,46, 0 , 345     ),
    #('visMass'          ,35, 0 , 350     ),
    #('visMass'          ,35, 0 , 350     ),
    #('visMass*1.03'     ,35, 0 , 350     ),
    #('visMass'          ,350, 0 , 350    ),
    #('visMass*1.03'     ,350, 0 , 350    ),
    #('visMass*0.97'     ,350, 0 , 350    ),
    #('visMass'          ,70 , 0 , 350    ),
    #('visMass*1.0'      ,35 , 0 , 350    ),
    #('visMass*1.03'     ,70 , 0 , 350    ),
    #('visMass*0.97'     ,70 , 0 , 350    ),
    #('dRtt'	       ,int(20/rebin), 0 , 5 	),
    #('nVert'  	       ,int(25/rebin), 0 , 50	),
    #('l1Pt'             ,int(15/rebin), 0 , 300    ),   
    #('l1Pt'             ,300          , 0 , 300    ),   
    #('l1Pt'             ,int(15/rebin), 0 , 300    ),   
    #('l1Pt*1.03'        ,300          , 0 , 300    ),   
    #('l1Pt*1.03'        ,int(15/rebin), 0 , 300    ),   
    #('l1Pt*0.97'        ,300          , 0 , 300    ),   
    #('l1Pt*0.97'        ,int(15/rebin), 0 , 300    ),   
    #('l1Pt'             ,int(60/rebin), 0 , 300    ),    
    #('l1Pt'             ,int(30/rebin), 0 , 300    ),
    #('l2Pt'             ,int(20/rebin), 0 , 200    ),
    #('l2Pt'             ,int(40/rebin), 0 , 200    ),   
    #('jet1Pt'           ,int(20/rebin), 0 , 600    ),
    ('jet1Pt'           ,int(30/rebin), 0 , 600    ),
    #('jet2Pt'           ,int(25/rebin), 0 , 500    ),
    #('met'              ,int(25/rebin), 0 , 200    ),
    #('l1Eta'            ,int(30/rebin), -3, 3      ),   
    #('l2Eta'            ,int(30/rebin), -3, 3      ),   
    #('l1Eta'            ,int(15/rebin), -3, 3      ),   
    #('l2Eta'            ,int(15/rebin), -3, 3      ),   
    ('jet1Eta'          ,int(20/rebin), -5, 5      ),
    ('jet2Eta'          ,int(20/rebin), -5, 5      ),
    #('mjj'              ,mjjBin , mjjMin , mjjMax  ),
    ('dEtajj'           ,int(18/rebin), -6 , 6     ),
    ('nJets'            ,10           , 0 , 10     ),
    ('dPhitt'           ,int(16/rebin), 0 , 3.2    ),
    ('dPhittjj'         ,int(16/rebin), 0 , 3.2    ),
    ('dEtatt'           ,int(22/rebin), 0 , 4.5    ),
    ('dEtattjj'         ,int(20/rebin), 0 , 4    ),
    ('pThiggs'          ,int(30/rebin), 0 , 300    ),
    ('diTauPt'          ,int(20/rebin), 0 , 300    ),
    ('mt1'              ,int(25/rebin), 0 , 200    ),
    ('mt2'              ,int(25/rebin), 0 , 200    ),
    ('l1JetInvMass'     ,30           , 0 , 600    ),
    ('l2JetInvMass'     ,30           , 0 , 600    ),
    ('l1jetPt'          ,30           , 0 , 300    ),
    ('l2jetPt'          ,20           , 0 , 200    ),
    ('l1jetMass'        ,30           , 0 , 150    ),
    ('l2jetMass'        ,30           , 0 , 150    ),
    ('l1jetWidth'       ,50           , 0 , 0.05   ),
    ('l2jetWidth'       ,50           , 0 , 0.05   ),
    #('l1jetBtag'        ,50           , -1 , 1     ),
    #('l2jetBtag'        ,50           , -1 , 1     ),
    #('l1RawMVAIso'      ,int(50/rebin) , 0.75 , 1.00001),
    #('l2RawMVAIso'      ,int(50/rebin) , 0.75 , 1.00001),
    ('l1DecayMode'      ,12           , 0 , 12     ),
    ('l2DecayMode'      ,12           , 0 , 12     ),
    ('mttj'             ,int(40/rebin), 0 , 1000   ),
    ('diTauCharge'      ,7            , -3, 3      ),
    ('diTauE'           ,35           , 0 , 350    ),
    ('diTauEta'         ,15           , -3, 3      ),
    ('diTauPhi'         ,16           , 0 , 3.2    ),
    ('diTauPx'          ,60           , -300, 300  ),
    ('diTauPy'          ,60           , -300, 300  ),
    ('diTauPz'          ,60           , -300, 300  ),
    #('l1LooIso'         ,2            , 0,  2      ),
    #('l2LooIso'         ,2            , 0,  2      ),
    #('l1MedIso'         ,2            , 0,  2      ),
    #('l2MedIso'         ,2            , 0,  2      ),
    #('l1TigIso'         ,2            , 0,  2      ),
    #('l2TigIso'         ,2            , 0,  2      ),
    ('l1RawMVAIso'      ,100          , 0 , 1.00001),
    ('l1MedMVAIso'      ,2            , 0 , 2      ),
    ('l1TigMVAIso'      ,2            , 0 , 2      ),
    #('l1LooseEle'       ,2            , 0 , 2      ),
    ('l1MVAEle'         ,2            , 0 , 2      ),
    #('l1LooseMu'        ,2            , 0 , 2      ),
    ('l2RawMVAIso'      ,100          , 0 , 1.00001),
    ('l2MedMVAIso'      ,2            , 0 , 2      ),
    ('l2TigMVAIso'      ,2            , 0 , 2      ),
    #('l2LooseEle'       ,2            , 0 , 2      ),
    ('l2MVAEle'         ,2            , 0 , 2      ),
    #('l2LooseMu'        ,2            , 0 , 2      ),
  ]
  
  variables = []
  variables.extend(variablesDataCard)
  #variables.extend(otherVariables1)
  #variables.extend(otherVariables2)
  
  return variables
 