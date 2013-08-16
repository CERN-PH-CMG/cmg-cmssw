import os, sys
import array
from ROOT import * 
from os import path

gROOT.Reset()
gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)
gStyle.SetOptFit(0)
gStyle.SetTitleOffset(1.2,"Y")
gStyle.SetPadLeftMargin(0.18)
gStyle.SetPadBottomMargin(0.15)
gStyle.SetPadTopMargin(0.08)
gStyle.SetPadRightMargin(0.05)
gStyle.SetMarkerSize(0.5)
gStyle.SetHistLineWidth(1)
gStyle.SetStatFontSize(0.020)
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetLabelSize(0.05, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetLegendBorderSize(0)

if __name__ == '__main__':

 runSet=11

 if runSet<10:
   samples = ["dijetWtag_Moriond_WWHpp1000.root",
            "dijetWtag_Moriond_ZZHpp1000.root",
	    "dijetWtag_Moriond_WWPy61000.root",
            "dijetWtag_Moriond_ZZPy61000.root",
	    "dijetWtag_Moriond_WWHpp1500.root",
            "dijetWtag_Moriond_ZZHpp1500.root",
	    "dijetWtag_Moriond_WWPy61500.root",
            "dijetWtag_Moriond_ZZPy61500.root",
	    "dijetWtag_Moriond_WWHpp2000.root",
            "dijetWtag_Moriond_ZZHpp2000.root",
	    "dijetWtag_Moriond_WWPy62000.root",
            "dijetWtag_Moriond_ZZPy62000.root",
	    "dijetWtag_Moriond_WWHpp3000.root",
            "dijetWtag_Moriond_ZZHpp3000.root",
	    "dijetWtag_Moriond_WWPy63000.root",
            "dijetWtag_Moriond_ZZPy63000.root",
	    "dijetWtag_Moriond_QCD1000.root",
	    "dijetWtag_Moriond_Mar6.root",
	    ]
   set=""

 if runSet==1:
  plots = [#[("DijetMass","((abs(Jet1eta-Jet2eta)<1.3))","|#Delta #eta|<1.3"),
          # ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100))","|#Delta #eta|<1.3, jet1 70<m<100"),
          # ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop>0.25))","|#Delta #eta|<1.3, jet1 70<m<100, jet1 #mu<0.25"),
          # ],
          #[("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&(!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)))))","0x 70<m<100"),
          # ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop>0.25)))&&(!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop>0.25)))&&(!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))))))","1x 70<m<100, 0x #mu<0.25"),
          # ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.25)))&&(!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.25)))&&(!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))))))","1x 70<m<100, 1x #mu<0.25"),
          # ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop>0.25))","2x 70<m<100, 0x #mu<0.25"),
          # ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop>0.25))||((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop<0.25))))","2x 70<m<100, 1x #mu<0.25"),
          # ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25))","2x 70<m<100, 2x #mu<0.25"),
          # ],
          #[("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25))","|#Delta #eta|<1.3, 2x 70<m<100, 2x #mu<0.25"),
          # ],
          #[("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25))","|#Delta #eta|<1.3, 2x 70<m<100, 2x #mu<0.25"),
          # ("DijetMass","((abs(Jet1eta)<1.0)&&(abs(Jet2eta)<1.0)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25))","|#eta_{1,2}|<1.0 2x 70<m<100, 2x #mu<0.25"),
          # ],
          #[("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8C2beta17<0.1)&&(Jet2CA8C2beta17<0.1))","2x 70<m<100, 2x C_{2}^{1.7}<0.1"),
          # ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8C2beta17<0.2)&&(Jet2CA8C2beta17<0.2))","2x 70<m<100, 2x C_{2}^{1.7}<0.2"),
          # ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8C2beta17<0.3)&&(Jet2CA8C2beta17<0.3))","2x 70<m<100, 2x C_{2}^{1.7}<0.3"),
          # ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8C2beta17<0.4)&&(Jet2CA8C2beta17<0.4))","2x 70<m<100, 2x C_{2}^{1.7}<0.4"),
          # ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8C2beta17<0.5)&&(Jet2CA8C2beta17<0.5))","2x 70<m<100, 2x C_{2}^{1.7}<0.5"),
          # ],
          [#("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8MassDrop<0.2)&&(Jet2CA8MassDrop<0.2))","2x 70<m<100, 2x mu<0.2"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25))","2x 70<m<100, 2x mu<0.25"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8MassDrop<0.3)&&(Jet2CA8MassDrop<0.3))","2x 70<m<100, 2x mu<0.3"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8MassDrop<0.35)&&(Jet2CA8MassDrop<0.35))","2x 70<m<100, 2x mu<0.35"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8MassDrop<0.4)&&(Jet2CA8MassDrop<0.4))","2x 70<m<100, 2x mu<0.4"),
           #("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","2x 70<m<100, 2x tau_{12}<0.35"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","2x 70<m<100, 2x tau_{12}<0.4"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","2x 70<m<100, 2x tau_{12}<0.45"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","2x 70<m<100, 2x tau_{12}<0.5"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","2x 70<m<100, 2x tau_{12}<0.55"),
           ],
          ]

  names = [#"dijets",
          #"baseline_fine",
	  #"baseline2_fine",
	  #"etacut",
	  "optimize_tagger",
	 ]

 if runSet==2:
  plots = [[#("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","2x 70<m<100, 2x tau_{12}<0.35"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","2x 70<m<100, 2x tau_{12}<0.4"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","2x 70<m<100, 2x tau_{12}<0.45"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","2x 70<m<100, 2x tau_{12}<0.5"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","2x 70<m<100, 2x tau_{12}<0.55"),
           ],
          [#("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>75)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>75)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","2x 75<m<100, 2x tau_{12}<0.35"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>75)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>75)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","2x 75<m<100, 2x tau_{12}<0.4"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>75)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>75)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.35))","2x 75<m<100, 2x tau_{12}<0.45"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>75)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>75)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","2x 75<m<100, 2x tau_{12}<0.5"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>75)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>75)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","2x 75<m<100, 2x tau_{12}<0.55"),
           #("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>65)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>65)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","2x 65<m<100, 2x tau_{12}<0.35"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>65)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>65)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","2x 65<m<100, 2x tau_{12}<0.4"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>65)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>65)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","2x 65<m<100, 2x tau_{12}<0.45"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>65)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>65)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","2x 65<m<100, 2x tau_{12}<0.5"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>65)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>65)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","2x 65<m<100, 2x tau_{12}<0.55"),
           ],
          [#("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<95)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<95)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","2x 70<m<95, 2x tau_{12}<0.35"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<95)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<95)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","2x 70<m<95, 2x tau_{12}<0.4"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<95)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<95)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","2x 70<m<95, 2x tau_{12}<0.45"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<95)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<95)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","2x 70<m<95, 2x tau_{12}<0.5"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<95)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<95)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","2x 70<m<95, 2x tau_{12}<0.55"),
           #("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<105)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<105)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","2x 70<m<105, 2x tau_{12}<0.35"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<105)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<105)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","2x 70<m<105, 2x tau_{12}<0.4"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<105)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<105)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","2x 70<m<105, 2x tau_{12}<0.45"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<105)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<105)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","2x 70<m<105, 2x tau_{12}<0.5"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<105)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<105)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","2x 70<m<105, 2x tau_{12}<0.55"),
           ],
          ]

  names = ["optimize_taggerDefault",
	  "optimize_mass_low",
	  "optimize_mass_high",
	 ]

 if runSet==3:
  plots = [[#("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","2x 70<m<100, 2x tau_{12}<0.35"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","2x 70<m<100, 2x tau_{12}<0.4"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","2x 70<m<100, 2x tau_{12}<0.45"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","2x 70<m<100, 2x tau_{12}<0.5"),
           ("DijetMass","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","2x 70<m<100, 2x tau_{12}<0.55"),
           ],
          [#("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","CA8 2x 70<m<100, 2x tau_{12}<0.35"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","CA8 2x 70<m<100, 2x tau_{12}<0.4"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","CA8 2x 70<m<100, 2x tau_{12}<0.45"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","CA8 2x 70<m<100, 2x tau_{12}<0.5"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","CA8 2x 70<m<100, 2x tau_{12}<0.55"),
           ],
          [#("DijetMassAK7","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1AK7Mass>70)&&(Jet1AK7Mass<100)&&(Jet2AK7Mass>70)&&(Jet2AK7Mass<100)&&(Jet1AK7Nsub<0.35)&&(Jet2AK7Nsub<0.35))","AK7 2x 70<m<100, 2x tau_{12}<0.35"),
           ("DijetMassAK7","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1AK7Mass>70)&&(Jet1AK7Mass<100)&&(Jet2AK7Mass>70)&&(Jet2AK7Mass<100)&&(Jet1AK7Nsub<0.4)&&(Jet2AK7Nsub<0.4))","AK7 2x 70<m<100, 2x tau_{12}<0.4"),
           ("DijetMassAK7","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1AK7Mass>70)&&(Jet1AK7Mass<100)&&(Jet2AK7Mass>70)&&(Jet2AK7Mass<100)&&(Jet1AK7Nsub<0.45)&&(Jet2AK7Nsub<0.45))","AK7 2x 70<m<100, 2x tau_{12}<0.45"),
           ("DijetMassAK7","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1AK7Mass>70)&&(Jet1AK7Mass<100)&&(Jet2AK7Mass>70)&&(Jet2AK7Mass<100)&&(Jet1AK7Nsub<0.5)&&(Jet2AK7Nsub<0.5))","AK7 2x 70<m<100, 2x tau_{12}<0.5"),
           ("DijetMassAK7","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1AK7Mass>70)&&(Jet1AK7Mass<100)&&(Jet2AK7Mass>70)&&(Jet2AK7Mass<100)&&(Jet1AK7Nsub<0.55)&&(Jet2AK7Nsub<0.55))","AK7 2x 70<m<100, 2x tau_{12}<0.55"),
           ],
          ]

  names = ["optimize_taggerDefault",
	  "optimize_taggerCA8",
	  "optimize_taggerAK7",
	 ]

 if runSet>10:
  samples = ["dijetWtag_Moriond_HHPy61000.root",
	    "dijetWtag_Moriond_HHPy61500.root",
	    "dijetWtag_Moriond_HHPy62000.root",
	    "dijetWtag_Moriond_HHPy63000.root",
	    "dijetWtag_Moriond_QCD1000.root",
	    "dijetWtag_Moriond_Mar6.root",
            ]
  set="_H"

  plots = [[("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","CA8 2x 110<m<130, 2x #tau_{12}<0.35"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","CA8 2x 110<m<130, 2x #tau_{12}<0.4"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","CA8 2x 110<m<130, 2x #tau_{12}<0.45"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","CA8 2x 110<m<130, 2x #tau_{12}<0.5"),
           #("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","CA8 2x 110<m<130, 2x #tau_{12}<0.55"),
           ],
          [("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>115)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>115)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","CA8 2x 115<m<130, 2x #tau_{12}<0.35"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>115)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>115)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","CA8 2x 115<m<130, 2x #tau_{12}<0.4"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>115)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>115)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","CA8 2x 115<m<130, 2x #tau_{12}<0.45"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>115)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>115)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","CA8 2x 115<m<130, 2x #tau_{12}<0.5"),
           #("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>115)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>115)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","CA8 2x 115<m<130, 2x #tau_{12}<0.55"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>105)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>105)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","CA8 2x 105<m<130, 2x #tau_{12}<0.35"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>105)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>105)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","CA8 2x 105<m<130, 2x #tau_{12}<0.4"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>105)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>105)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","CA8 2x 105<m<130, 2x #tau_{12}<0.45"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>105)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>105)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","CA8 2x 105<m<130, 2x #tau_{12}<0.5"),
           #("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>105)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>105)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","CA8 2x 105<m<130, 2x #tau_{12}<0.55"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>100)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>100)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","CA8 2x 100<m<130, 2x #tau_{12}<0.35"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>100)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>100)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","CA8 2x 100<m<130, 2x #tau_{12}<0.4"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>100)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>100)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","CA8 2x 100<m<130, 2x #tau_{12}<0.45"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>100)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>100)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","CA8 2x 100<m<130, 2x #tau_{12}<0.5"),
           #("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>100)&&(Jet1CA8Mass<130)&&(Jet2CA8Mass>100)&&(Jet2CA8Mass<130)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","CA8 2x 100<m<130, 2x #tau_{12}<0.55"),
           ],
          [("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<125)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<125)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","CA8 2x 110<m<125, 2x #tau_{12}<0.35"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<125)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<125)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","CA8 2x 110<m<125, 2x #tau_{12}<0.4"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<125)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<125)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","CA8 2x 110<m<125, 2x #tau_{12}<0.45"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<125)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<125)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","CA8 2x 110<m<125, 2x #tau_{12}<0.5"),
           #("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<125)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<125)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","CA8 2x 110<m<125, 2x #tau_{12}<0.55"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<135)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<135)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","CA8 2x 110<m<135, 2x #tau_{12}<0.35"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<135)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<135)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","CA8 2x 110<m<135, 2x #tau_{12}<0.4"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<135)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<135)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","CA8 2x 110<m<135, 2x #tau_{12}<0.45"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<135)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<135)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","CA8 2x 110<m<135, 2x #tau_{12}<0.5"),
           #("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<135)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<135)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","CA8 2x 110<m<135, 2x #tau_{12}<0.55"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<140)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<140)&&(Jet1CA8Nsub<0.35)&&(Jet2CA8Nsub<0.35))","CA8 2x 110<m<140, 2x #tau_{12}<0.35"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<140)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<140)&&(Jet1CA8Nsub<0.4)&&(Jet2CA8Nsub<0.4))","CA8 2x 110<m<140, 2x #tau_{12}<0.4"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<140)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<140)&&(Jet1CA8Nsub<0.45)&&(Jet2CA8Nsub<0.45))","CA8 2x 110<m<140, 2x #tau_{12}<0.45"),
           ("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<140)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<140)&&(Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5))","CA8 2x 110<m<140, 2x #tau_{12}<0.5"),
           #("DijetMassCA8","((abs(Jet1eta-Jet2eta)<1.3)&&(Jet1CA8Mass>110)&&(Jet1CA8Mass<140)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<140)&&(Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55))","CA8 2x 110<m<140, 2x #tau_{12}<0.55"),
           ],
          ]

  names = ["optimize_HtaggerCA8",
	  "optimize_HtaggerCA8_mass_low",
	  "optimize_HtaggerCA8_mass_high",
	 ]

 hists=[]
 
 bins = [890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687,
 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509,
 4686, 4869, 5058]

 bins_fine=[]
 for i in range(5000/30):
     bins_fine+=[i*30]

 binning=array.array('d')
 for bin in bins:
    binning.append(bin)

 binning_fine=array.array('d')
 for bin in bins_fine:
    binning_fine.append(bin)

 colors=[1,2,4,7,6,8,9,11,40,41,42,43,44,45,46,47]
 styles=[1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4]

 for ratio in ["","ratio"]:
  results=[]
  for sample in samples:
   signal = "Hpp" in sample or "Py6" in sample
   for plot in plots:
    canvas = TCanvas("","",0,0,200,200)
    
    legend=TLegend(0.2,0.7,0.9,0.90,"")

    f=TFile.Open(sample)
    tree=f.Get("dijetWtag")
    print plot[0][2]
    hist=TH1F("plot"+str(plot[0][2]),"M_{jj}",8000,0,8000);
    tree.Project("plot"+str(plot[0][2]),plot[0][0],plot[0][1])
    outfile=TFile("data2012_v2_tree_"+ratio+"_"+sample.split(".")[0]+"_"+names[plots.index(plot)]+"_0.root","RECREATE")
    hist.Write("dijet_mass_0mtag")
    hist.Write("dijet_mass_1mtag_0mdtag")
    hist.Write("dijet_mass_1mtag_1mdtag")
    hist.Write("dijet_mass_2mtag_0mdtag")
    hist.Write("dijet_mass_2mtag_1mdtag")
    hist.Write("dijet_mass_2mtag_2mdtag")
    outfile.Close()
    if "fine" in names[plots.index(plot)]:
      hist=hist.Rebin(len(bins_fine)-1,hist.GetName()+"_rebin",binning_fine)
    else:
      hist=hist.Rebin(len(bins)-1,hist.GetName()+"_rebin",binning)
    if len(plot[0])>3:
        hist3=f.Get(plot[0][3])
        if "fine" in names[plots.index(plot)]:
          hist3=hist3.Rebin(len(bins_fine)-1,hist3.GetName()+"_rebin",binning_fine)
        else:
          hist3=hist3.Rebin(len(bins)-1,hist3.GetName()+"_rebin",binning)
        hist.Divide(hist3)
    hist.SetTitle("")
    hist.SetLineColor(1)
    hist.SetLineStyle(1)
    hist.SetLineWidth(2)
    hist.SetFillStyle(0)
    hist.SetMarkerStyle(0)
    hist.GetXaxis().SetTitle("dijet mass")
    hist.GetYaxis().SetTitle("N")
    efficiency=""
    if signal:
       mass = float(sample[sample.index("00")-1:sample.index("00")+3])
       if "WW" in sample or "WZ" in samples or "ZZ" in samples:
           ngenevents=30000
       if "HH" in sample:
           ngenevents=20000
       print "normalize by number of gen-events", ngenevents
       hist.Scale(1./ngenevents)
       efficiency= 'eff=%(number).3f' % {"number":hist.Integral(hist.FindBin(mass*0.57),hist.FindBin(mass*1.3))}
       print efficiency
       hist.Draw("hist")
    else:
       hist.Draw("histe")
    legend.AddEntry(hist,plot[0][2]+" "+efficiency,"l")
    hist.GetXaxis().SetRangeUser(890,4000)
    hist.GetYaxis().SetRangeUser(0.5,1000000)
    if signal:
        hist.GetXaxis().SetRangeUser(0,4000)
        hist.GetYaxis().SetRangeUser(0,0.05)
    if "QCD" in sample:
        hist.GetXaxis().SetRangeUser(1200,4000)
    if len(plot[0])>3:
        hist.GetYaxis().SetTitle("ratio")
        hist.GetYaxis().SetRangeUser(0,2)
    if names[plots.index(plot)]=="massdropside":
        hist.GetYaxis().SetRangeUser(0,0.2)
    if "massside2" in names[plots.index(plot)]:
        hist.GetYaxis().SetRangeUser(0,1)
    if "compare" in names[plots.index(plot)]:
       hist.Scale(1./hist.Integral())
       hist.GetYaxis().SetRangeUser(0.0001,0.5)
    hists+=[hist]
    fit=TF1('fit1','[0]*pow(1.0-x/7000.0,[1])/pow(x/7000.0,[2]+[3]*log(x/7000.0))',890,10000)
    fit.SetParameter(0,hist.Integral())
    fit.SetParameter(1,5)
    fit.SetParameter(2,5)
    fit.SetParameter(3,0)
    if "2x 70<m<100" in plot[0][2]:
        fit.FixParameter(3,0)
    fit.SetLineWidth(1)
    fit.SetLineColor(hist.GetLineColor())
    fit.SetLineStyle(hist.GetLineStyle())
    hist.Fit(fit,"R0Q")
    if not signal:
	events1=0
        for b in range(hist.FindBin(1000.*0.9),hist.FindBin(1000.*1.1)):
            events1+=fit.Integral(hist.GetXaxis().GetBinLowEdge(b+1),hist.GetXaxis().GetBinUpEdge(b+1))/hist.GetBinWidth(b+1)
	events2=0
        for b in range(hist.FindBin(2000.*0.9),hist.FindBin(2000.*1.1)):
            events2+=fit.Integral(hist.GetXaxis().GetBinLowEdge(b+1),hist.GetXaxis().GetBinUpEdge(b+1))/hist.GetBinWidth(b+1)
        results += [(sample,plot[0][2],events1,events2)]
    else:
        results += [(sample,plot[0][2],hist.Integral(hist.FindBin(mass*0.9),hist.FindBin(mass*1.1)))]
    if ratio:
        hist.GetYaxis().SetTitle("(data-fit)/#sigma_{data}")
        hist.GetYaxis().SetRangeUser(-5,5)
	for b in range(hist.GetNbinsX()):
	    if hist.GetBinError(b+1)>0:
                hist.SetBinContent(b+1,(hist.GetBinContent(b+1)-fit.Integral(hist.GetXaxis().GetBinLowEdge(b+1),hist.GetXaxis().GetBinUpEdge(b+1))/hist.GetBinWidth(b+1))/hist.GetBinError(b+1))
                #hist.SetBinContent(b+1,(hist.GetBinContent(b+1)-fit.Eval(hist.GetBinCenter(b+1)))/hist.GetBinError(b+1))
	    else:
                hist.SetBinContent(b+1,0)
	    hist.SetBinError(b+1,0)
    elif not len(plot[0])>3 and not signal:
        fit.Draw('lsame')
        canvas.SetLogy(True)

    for i in range(1,len(plot)):
        print plot[i][2]
        hist2=TH1F("plot"+str(plot[i][2]),"M_{jj}",8000,0,8000);
        tree.Project("plot"+str(plot[i][2]),plot[i][0],plot[i][1])
     	outfile=TFile("data2012_v2_tree_"+ratio+"_"+sample.split(".")[0]+"_"+names[plots.index(plot)]+"_"+str(i)+".root","RECREATE")
     	hist2.Write("dijet_mass_0mtag")
     	hist2.Write("dijet_mass_1mtag_0mdtag")
     	hist2.Write("dijet_mass_1mtag_1mdtag")
     	hist2.Write("dijet_mass_2mtag_0mdtag")
     	hist2.Write("dijet_mass_2mtag_1mdtag")
     	hist2.Write("dijet_mass_2mtag_2mdtag")
     	outfile.Close()
        if "fine" in names[plots.index(plot)]:
          hist2=hist2.Rebin(len(bins_fine)-1,hist2.GetName()+"_rebin",binning_fine)
        else:
          hist2=hist2.Rebin(len(bins)-1,hist2.GetName()+"_rebin",binning)
        if len(plot[i])>3:
            hist4=f.Get(plot[i][3])
            if "fine" in names[plots.index(plot)]:
              hist4=hist4.Rebin(len(bins_fine)-1,hist4.GetName()+"_rebin",binning_fine)
            else:
              hist4=hist4.Rebin(len(bins)-1,hist4.GetName()+"_rebin",binning)
            hist2.Divide(hist4)
        hist2.SetLineColor(colors[i])
        hist2.SetLineStyle(styles[i])
        hist2.SetFillStyle(0)
        hist2.SetLineWidth(2)
        hist2.SetMarkerStyle(0)
        efficiency=""
        if signal:
	   hist2.Scale(1./ngenevents)
           efficiency= 'eff=%(number).3f' % {"number":hist2.Integral(hist2.FindBin(mass*0.57),hist2.FindBin(mass*1.3))}
           print efficiency
           hist2.Draw("histsame")
	else:
           hist2.Draw("histesame")
        legend.AddEntry(hist2,plot[i][2]+" "+efficiency,"l")
        hist2.GetYaxis().SetRangeUser(0.5,1000000)
        if signal:
            hist2.GetXaxis().SetRangeUser(0,4000)
            hist2.GetYaxis().SetRangeUser(0,0.05)
        if "QCD" in sample:
            hist2.GetXaxis().SetRangeUser(1200,4000)
        if len(plot[0])>3:
            hist2.GetYaxis().SetTitle("ratio")
            hist2.GetYaxis().SetRangeUser(0,2)
        if names[plots.index(plot)]=="massdropside":
            hist.GetYaxis().SetRangeUser(0,0.2)
        if "massside2" in names[plots.index(plot)]:
            hist2.GetYaxis().SetRangeUser(0,1)
        if "compare" in names[plots.index(plot)]:
            hist2.Scale(1./hist2.Integral())
            hist2.GetYaxis().SetRangeUser(0.0001,0.5)
        hists+=[hist2]
        fit2=TF1('fit2','[0]*pow(1.0-x/7000.0,[1])/pow(x/7000.0,[2]+[3]*log(x/7000.0))',890,10000)
        fit2.SetParameter(0,hist2.Integral())
        fit2.SetParameter(1,5)
        fit2.SetParameter(2,5)
        fit2.SetParameter(3,0)
        if "2x 70<m<100" in plot[i][2]:
            fit2.FixParameter(3,0)
        fit2.SetLineWidth(1)
        fit2.SetLineColor(hist2.GetLineColor())
        fit2.SetLineStyle(hist2.GetLineStyle())
        hist2.Fit(fit2,"R0Q")
        if not signal:
	    events1=0
            for b in range(hist2.FindBin(1000.*0.9),hist2.FindBin(1000.*1.1)):
                events1+=fit2.Integral(hist2.GetXaxis().GetBinLowEdge(b+1),hist2.GetXaxis().GetBinUpEdge(b+1))/hist2.GetBinWidth(b+1)
	    events2=0
            for b in range(hist2.FindBin(2000.*0.9),hist2.FindBin(2000.*1.1)):
                events2+=fit2.Integral(hist2.GetXaxis().GetBinLowEdge(b+1),hist2.GetXaxis().GetBinUpEdge(b+1))/hist2.GetBinWidth(b+1)
            results += [(sample,plot[i][2],events1,events2)]
        else:
            results += [(sample,plot[i][2],hist2.Integral(hist2.FindBin(mass*0.9),hist2.FindBin(mass*1.1)))]
        if ratio:
            hist2.GetYaxis().SetTitle("(data-fit)/#sigma_{data}")
            hist2.GetYaxis().SetRangeUser(-5,5)
  	    for b in range(hist2.GetNbinsX()):
  	    	if hist2.GetBinContent(b+1)==0: continue
  	    	if hist2.GetBinError(b+1)>0:
        	    hist2.SetBinContent(b+1,(hist2.GetBinContent(b+1)-fit2.Integral(hist2.GetXaxis().GetBinLowEdge(b+1),hist2.GetXaxis().GetBinUpEdge(b+1))/hist2.GetBinWidth(b+1))/hist2.GetBinError(b+1))
        	    #hist2.SetBinContent(b+1,(hist2.GetBinContent(b+1)-fit2.Eval(hist2.GetBinCenter(b+1)))/hist2.GetBinError(b+1))
  	    	else:
        	    hist2.SetBinContent(b+1,0)
  	    	hist2.SetBinError(b+1,0)
        elif not len(plot[0])>3 and not signal:
            fit2.Draw('lsame')

    legend.SetTextSize(0.03)
    legend.SetFillStyle(0)
    legend.Draw("same")

    canvas.SaveAs("data2012_v2_tree_"+ratio+"_"+sample.split(".")[0]+"_"+names[plots.index(plot)]+".pdf")
    canvas.SaveAs("data2012_v2_tree_"+ratio+"_"+sample.split(".")[0]+"_"+names[plots.index(plot)]+".root")
    f.Close()

  f=open("data2012_v2_tree"+str(runSet)+"_"+ratio+".txt","w")
  f.write(str(results))
  f.close()
 