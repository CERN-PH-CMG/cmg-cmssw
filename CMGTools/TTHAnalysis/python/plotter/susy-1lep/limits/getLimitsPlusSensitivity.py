
#!/usr/bin/env python

import shutil
import subprocess
import os


# check CMSSW version
cmssw_vers = os.environ['CMSSW_VERSION']

if 'CMSSW_7_1' not in cmssw_vers:
    print "You've got the wrong CMSSW env:"
    print "Need CMSSW_7_1_X with limit tool!"
    exit(0)

cardDirectory="susy_cards_1l_4fb_allBins"
cardDirectory = os.path.abspath(cardDirectory)

limitDir = cardDirectory+"/limits/"
if not os.path.isdir(limitDir):
    print 'Creating limit dir', limitDir
    os.mkdir(limitDir)

print 'Entering out dir', limitDir
os.chdir(limitDir)
print

Samples = []
Samples.append("T1tttt_HL_1500_100")
Samples.append("T1tttt_HM_1200_800")
#Samples.append("T1ttbbWW_1300_300_290")
#Samples.append("T1ttbbWW_1300_300_295")
#Samples.append("T5qqqqWW_Gl1200_Chi1000_LSP800")
#Samples.append("T5ttttDeg_1300_300_280")
#Samples.append("T2tt_425_325")
#Samples.append("T2tt_650_325")
#Samples.append("T2tt_850_100")



#VariantSnippet = ["standardnJ_HTTTYesNo","standardnJ_HT1","finenJ_HTTTYesNo","finenJ_HT1"]
#VariantSnippet = ["standardnJ_HTLowLepPt","standardnJ_HTLowLepPtDPhi","standardnJ_HT1","finenJ_HTLowLepPt","finenJ_HTLowLepPtDPhi","finenJ_HT1"]
#VariantSnippet = ["standardnJ_HTLowLepPt","standardnJ_HT1","standardnJ_HighLowLepPt","finenJ_HTLowLepPt","finenJ_HT1","finenJ_HighLowLepPt"]
#VariantSnippet = ["standardnJ_HighLowLepPt","finenJ_HighLowLepPt"]
#VariantSnippet = ["standardnJ", "finenJ","finenJ_HT1","standardnJ_HT1"]
#VariantSnippet = ["finenJ_HT1","standardnJ_HT1","finenJ_HTTop","standardnJ_HTTop"]
#VariantSnippet = ["standardnJ", "finenJ","allnJ", "allnJTopnesses"]
VariantSnippet = ["standardnJ", "finenJ"]
#VariantSnippet = ["stdNj_TopTag", "fineNj_TopTag", "stdNj_TopTagTau", "fineNj_TopTagTau"]
#VariantSnippet = ["fineNj_TopTagTau", "stdNj_TopTagTau"]
#VariantSnippet = ["stdNj_TopTag"]#,"fineNj_TopTag"]
#VariantSnippet = ["NonExtreme_standardnJ", "NonExtreme_68nJ", "NonExtreme_68nJTopness", "NonExtreme_68nJDPhi05", "NonExtreme_68nJSingleTopness"]


#VariantSnippet = []
#
##for ST in ["ST4"]:
#for ST in ["ST0", "ST1", "ST2", "ST3", "ST4"]:
##    for nJ in ["68j", "6Infj", "9Infj"]:
#    for nJ in ["45j"]:
#        for nB in ["2B", "3p"]:
##        for nB in ["2B"]:
#            for HT in ["HT0", "HT1"]:
##            for HT in ["HT0"]:
#                for RD in ["DPhi00", "DPhi05", "DPhi10", "Stop", "Top"]:
#                    VariantSnippet.append(nB+"_"+ST+"_"+nJ+"_"+HT+"_"+RD)

#VariantSnippet = ["2B_ST0_9Infj_HT0_Top"]


#standard: baseline
#finenJ: baseline plus highnJ-bin
#finenJ_HT*: only high HT-bin
#"finenJ_HT1": baselin, only high HT
#"finenJ_HT1TT": baseline+ toptag-counting
#"finenJ_HTTop": baseline+ dphi>.05 single topness >1.25, topness > 5
#"finenJ_HTTopTT: baseline+ toptag-counting + dphi>.05 single topness >1.25, topness > 5
#"finenJ_HighLowLepPt" combination of hard/soft leptons

limitdict = {}
sigdict = {}
for s_i, s in enumerate(Samples):

    print 80*'#'
    print "Limits for", s

    for v_i, v in enumerate(VariantSnippet):

        print 80*'#'
        print cardDirectory+"/"+s+"/CnC2015X_"+v+".card.txt"
        print  "Limits_"+s+"_"+v+".txt"
        limfName = "Limits_"+s+"_"+v
        signfName = "Significance_"+s+"_"+v
        cardName = cardDirectory+"/"+s+"/CnC2015X_"+v+".card.txt"

        sigline = 'Fail'
        limitline = 'Fail'

        print "# Variant:", v
        os.system("combine -M Asymptotic "+ cardName + " -n " + limfName+ " &> "+ limfName + ".txt")
        os.system("combine -M ProfileLikelihood --significance "+ cardName + " -t -1 --expectSignal=1 -n "+ signfName +" &> "+ signfName + ".txt")

        lf=open(limfName+".txt")
        llines=lf.readlines()

        for line in llines:
            if "50.0%" in line:
                limitline = line
        #limitline = llines[6]
        limitdict[(s,v)]= (limitline.split("<")[1]).split()[0]

        sf=open(signfName+".txt")
        slines=sf.readlines()

        for sline in slines:
            if "Signif" in sline:
                sigline = sline
        sigdict[(s,v)]= (sigline.split(":")[1]).split()[0]
        #sigline= slines[2]

        if limitline == 'Fail':
            print 'Limit not found!'
            open("Failed_"+s+"_"+v+".txt", 'a').close()
        elif sigline == 'Fail':
            print 'Significance not found!'
            open("Failed_"+s+"_"+v+".txt", 'a').close()
        else:
            print limitline, sigline


print limitdict
print sigdict
